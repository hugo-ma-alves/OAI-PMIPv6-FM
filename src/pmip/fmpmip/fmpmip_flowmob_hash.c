
#define PMIP
#define FMPMIP_FLOWMOB_HASH_C

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include <stdio.h>

#include "debug.h"
#include "util.h"

#include "fmpmip_flowmob_hash.h"

#define FLOWMOB_CACHE_BUCKETS              128


pthread_rwlock_t flowmob_cache_lock;



uint32_t calc_hash(int s,  struct in6_addr *ip6_destination)
{
    uint32_t hash= (ip6_destination->s6_addr[0] ^
        ip6_destination->s6_addr[1] ^
        ip6_destination->s6_addr[2] ^
        ip6_destination->s6_addr[3]) % s;
    return hash;
    
}

int match(struct flow_mob_hash_entry *h,  
    struct in6_addr     *ip6_source,
    struct in6_addr     *ip6_destination,
    uint32_t            transport_protocol ,
    uint16_t            transport_source_port ,
    uint16_t            transport_destination_port ,
    uint8_t             traffic_class,
    uint32_t            flow_label)
{

    assert(h);

    if (IN6_ARE_ADDR_EQUAL(&(h->ip6_source), ip6_source)
        && IN6_ARE_ADDR_EQUAL(&(h->ip6_destination), ip6_destination)
        && h->transport_protocol == transport_protocol 
        && h->transport_source_port == transport_source_port
        && h->transport_destination_port == transport_destination_port
        && h->flow_label == flow_label
        && h->traffic_class == traffic_class ){

      return 1;

}

return 0;
}

/* Caller must have reserved the memory for the hash */
int flow_mob_hash_init(struct flow_mob_hash_structure *h, int buckets)
{
    assert(h);
    assert(buckets);
    h->hash_buckets = (struct flow_mob_hash_entry **)malloc(buckets * sizeof(struct flow_mob_hash_entry *));
    if (!h->hash_buckets)
        return -ENOMEM;
    memset(h->hash_buckets, 0, buckets * sizeof(struct flow_mob_hash_entry *));
    h->buckets = buckets;

    return 0;
}

void flow_mob_hash_cleanup(struct flow_mob_hash_structure *h)
{
    struct flow_mob_hash_entry *he, *tmp;
    int i;
    assert(h);
    
    for(i=0; i < h->buckets; i++) {
        he = h->hash_buckets[i];
        while(he) {
            tmp = he;
            he = he->next;
            free(tmp);
        }
    }
    free(h->hash_buckets);
}

void * flow_mob_hash_get( const struct flow_mob_hash_structure *h, 
   struct in6_addr *ip6_source,
   struct in6_addr *ip6_destination,
   uint32_t  transport_protocol ,
   uint16_t  transport_source_port ,
   uint16_t  transport_destination_port ,
   uint8_t             traffic_class,
   uint32_t            flow_label)
{
    struct flow_mob_hash_entry *hptr = NULL;
    
    assert(h);

    hptr = h->hash_buckets[calc_hash(h->buckets,ip6_destination)];

    while(hptr) {
        if (match(hptr, ip6_source,ip6_destination, transport_protocol, 
            transport_source_port, transport_destination_port,
            traffic_class,flow_label))
            return hptr->data;
        hptr = hptr->next;
    }
    return NULL;
}

int flow_mob_hash_add(const struct flow_mob_hash_structure *h, void *data , struct in6_addr *ip6_source,
    struct in6_addr *ip6_destination,
    uint32_t  transport_protocol ,
    uint16_t  transport_source_port ,
    uint16_t  transport_destination_port ,
    uint8_t             traffic_class,
    uint32_t            flow_label)
{

    struct flow_mob_hash_entry *hptr = NULL, *new;
    uint32_t hash_ind;
    
    assert(h); 
    //assert(fmpmip_flow_cache_entry);

    if ((new = (struct flow_mob_hash_entry *)malloc(sizeof(struct flow_mob_hash_entry))) == NULL) {
        dbg("out of memory\n");
        return -ENOMEM;
    }

    new->data = data;
    new->ip6_source =ip6_source;
    new->ip6_destination=ip6_destination;
    new->transport_protocol=transport_protocol;
    new->transport_destination_port= transport_destination_port;
    new->transport_source_port=transport_source_port;
    new->flow_label=flow_label;
    new->traffic_class=traffic_class;

    hash_ind  = calc_hash(h->buckets, ip6_destination);
    hptr = h->hash_buckets[hash_ind];
    if (hptr) {
        while (hptr->next){
            if (match(hptr, ip6_source,ip6_destination, transport_protocol, 
                transport_source_port, transport_destination_port,
                traffic_class,flow_label)) {
                free(new);
            return -EEXIST;
        }
        hptr = hptr->next;
    }
    hptr->next = new;
} else {
    h->hash_buckets[hash_ind] = new;
}

new->next = NULL;
return 0;
}

int flow_mob_hash_iterate(const struct flow_mob_hash_structure *h, int (*func)(void *, void *), void *arg)
{
    int err = 0;
    int i;
    struct flow_mob_hash_entry *hptr, *nptr;

    assert(func);
    
    for (i=0; i < h->buckets; i++) {
        if((hptr = h->hash_buckets[i]) != NULL) {
            while(hptr) {
                nptr = hptr->next;
                if ((err = func(hptr->data, arg)))
                    return err;
                hptr = nptr;
            }
        }
    }
    return 0;
}

void flow_mob_hash_delete(const struct flow_mob_hash_structure *h , struct in6_addr *ip6_source,
    struct in6_addr *ip6_destination,
    uint32_t  transport_protocol ,
    uint16_t  transport_source_port ,
    uint16_t  transport_destination_port ,
    uint8_t             traffic_class,
    uint32_t            flow_label)
{
    struct flow_mob_hash_entry *hptr, *pptr, *head;
    int hash_ind;

    assert(h);

    hash_ind = calc_hash(h->buckets, ip6_destination);
    head = h->hash_buckets[hash_ind];
    
    pptr = hptr = head;
    while (hptr) {
        if (match(hptr, ip6_source,ip6_destination, transport_protocol, 
                transport_source_port, transport_destination_port,
                traffic_class,flow_label)) {
            if (hptr != head) 
                pptr->next = hptr->next;
            else {
                /* First entry */ 
                h->hash_buckets[hash_ind] = hptr->next;
            }
            hptr->data = NULL;
            free(hptr);
            hptr = NULL;
            return;
        }
        pptr = hptr;
        hptr = hptr->next;
    }
    /* Not found */
    return;
}   
