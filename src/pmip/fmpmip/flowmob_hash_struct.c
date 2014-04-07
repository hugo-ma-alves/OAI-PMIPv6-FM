
#define PMIP
#define FMPMIP_FLOWMOB_CACHE_C

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

#include "flowmob_hash_struct.h"

#define FLOWMOB_CACHE_BUCKETS              128


static struct flow_mob_hash_structure flowmob_hash;
static int    flowmob_cache_count = 0;
pthread_rwlock_t flowmob_cache_lock;

/* Hash entry */
struct flow_mob_hash_entry {
    struct flow_mob_hash_entry *next;
    fmpmip_flow_cache_entry *fmpmip_flow_cache_entry;
};


int get_flowmob_cache_count()
{
    return flowmob_cache_count;
}

int flowmob_cache_init(void)
{
    int ret;
    int mutex_return_code;
    if (pthread_rwlock_init(&flowmob_cache_lock, NULL)) {
        return -1;
    }

    mutex_return_code = pthread_rwlock_wrlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_wrlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }

    ret = flow_mob_hash_init(&flowmob_hash, FLOWMOB_CACHE_BUCKETS);

    mutex_return_code = pthread_rwlock_unlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_unlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }

    return ret;
}

fmpmip_flow_cache_entry * flowmob_cache_alloc(void){

 fmpmip_flow_cache_entry *tmp;
 tmp = malloc(sizeof(fmpmip_flow_cache_entry));

 if (tmp == NULL) {
    dbg("NO memory allocated for Flow mobility cache entry..\n");
    return NULL;
}

memset(tmp, 0, sizeof(*tmp));

if (pthread_rwlock_init(&tmp->lock, NULL)) {
    free(tmp);
    return NULL;
}
dbg("Flow cache entry memory is allocated in the heap..\n");
return tmp;
}

static int __insert_in_hashtable(fmpmip_flow_cache_entry * bce)
{
    int ret;
    ret = flow_mob_hash_add(&flowmob_hash, bce);
    if (ret) {
        return ret;
    }
    flowmob_cache_count++;
    return 0;
}

fmpmip_flow_cache_entry * flowmob_cache_add(fmpmip_flow_cache_entry * bce){

    int ret = 1;
    int mutex_return_code;
    assert(bce);
    mutex_return_code = pthread_rwlock_wrlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_wrlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }
    if ((ret = __insert_in_hashtable(bce)) != 0) {
        pthread_rwlock_unlock(&flowmob_cache_lock);
        dbg("WARNING: Flow mob cache entry ENTRY NOT INSERTED\n");
        return NULL;
    }
    dbg("Created a new FLow mob entry\n");
    mutex_return_code = pthread_rwlock_unlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_unlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }
    return bce;

}


fmpmip_flow_cache_entry * flowmob_cache_get(fmpmip_flow_cache_entry * bce){

    int mutex_return_code;

    assert(bce);
    mutex_return_code = pthread_rwlock_rdlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_rdlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }
    bce = flow_mob_hash_get(&flowmob_hash, bce);
    if (bce) {
        mutex_return_code = pthread_rwlock_wrlock(&bce->lock);
        if (mutex_return_code != 0) {
            dbg("pthread_rwlock_wrlock(&bce->lock) %s\n", strerror(mutex_return_code));
        }
    } else {
        mutex_return_code = pthread_rwlock_unlock(&flowmob_cache_lock);
        if (mutex_return_code != 0) {
            dbg("pthread_rwlock_unlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
        }
    }
    return bce;
}


void flowmob_cache_release_entry(fmpmip_flow_cache_entry * bce){

   int mutex_return_code;
   assert(bce);
   mutex_return_code = pthread_rwlock_unlock(&bce->lock);
   if (mutex_return_code != 0) {
    dbg("pthread_rwlock_unlock(&bce->lock) %s\n", strerror(mutex_return_code));
}
mutex_return_code = pthread_rwlock_unlock(&flowmob_cache_lock);
if (mutex_return_code != 0) {
    dbg("pthread_rwlock_unlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
}
}


int flowmob_cache_exists(fmpmip_flow_cache_entry * bce){

    bce = flowmob_cache_get(bce);
    if (bce == NULL) {
        return -1;
    }
    flowmob_cache_release_entry(bce);
    return 1;

}

void flowmob_cache_free(fmpmip_flow_cache_entry * bce){

  int mutex_return_code;
  mutex_return_code = pthread_rwlock_destroy(&bce->lock);
  if (mutex_return_code != 0) {
    dbg("pthread_rwlock_destroy(&bce->lock) %s\n", strerror(mutex_return_code));
}
free(bce);
}


void flowmob_entry_delete(fmpmip_flow_cache_entry * bce){

   int mutex_return_code;

   mutex_return_code = pthread_rwlock_wrlock(&bce->lock);
   if (mutex_return_code != 0) {
    dbg("pthread_rwlock_wrlock(&bce->lock) %s\n", strerror(mutex_return_code));
}

flowmob_cache_count--;
flow_mob_hash_delete(&flowmob_hash, bce);
mutex_return_code = pthread_rwlock_unlock(&bce->lock);
if (mutex_return_code != 0) {
    dbg("pthread_rwlock_unlock(&bce->lock) %s\n", strerror(mutex_return_code));
}
flowmob_cache_free(bce);
dbg("Deleted FLow mob cache entry!\n");


}



uint32_t calc_hash(int s,  fmpmip_flow_cache_entry * fmpmip_flow_cache_entry)
{
    uint32_t hash= (fmpmip_flow_cache_entry->ip6_destination.s6_addr[0] ^
        fmpmip_flow_cache_entry->ip6_destination.s6_addr[1] ^
        fmpmip_flow_cache_entry->ip6_destination.s6_addr[2] ^
        fmpmip_flow_cache_entry->ip6_destination.s6_addr[3]) % s;
    return hash;
    
}

int match(struct flow_mob_hash_entry *h,  fmpmip_flow_cache_entry *fmpmip_flow_cache_entry)
{
    assert(h);
    if (IN6_ARE_ADDR_EQUAL(&(h->fmpmip_flow_cache_entry->ip6_origin.s6_addr), &(fmpmip_flow_cache_entry->ip6_origin.s6_addr))
        && IN6_ARE_ADDR_EQUAL(&(h->fmpmip_flow_cache_entry->ip6_destination.s6_addr), &(fmpmip_flow_cache_entry->ip6_destination.s6_addr))
        && h->fmpmip_flow_cache_entry->transport_protocol == fmpmip_flow_cache_entry->transport_protocol 
        && h->fmpmip_flow_cache_entry->transport_source_port == fmpmip_flow_cache_entry->transport_source_port
        && h->fmpmip_flow_cache_entry->transport_destination_port == fmpmip_flow_cache_entry->transport_destination_port){

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

fmpmip_flow_cache_entry * flow_mob_hash_get(const struct flow_mob_hash_structure *h, fmpmip_flow_cache_entry * fmpmip_flow_cache_entry)
{
    struct flow_mob_hash_entry *hptr = NULL;
    
    assert(h);
    assert(fmpmip_flow_cache_entry);

    hptr = h->hash_buckets[calc_hash(h->buckets, fmpmip_flow_cache_entry)];

    while(hptr) {
        if (match(hptr, fmpmip_flow_cache_entry))
            return hptr->fmpmip_flow_cache_entry;
        hptr = hptr->next;
    }
    return NULL;
}

int flow_mob_hash_add(const struct flow_mob_hash_structure *h, fmpmip_flow_cache_entry * fmpmip_flow_cache_entry)
{
    struct flow_mob_hash_entry *hptr = NULL, *new;
    uint32_t hash_ind;
    
    assert(h); 
    assert(fmpmip_flow_cache_entry);

    if ((new = (struct flow_mob_hash_entry *)malloc(sizeof(struct flow_mob_hash_entry))) == NULL) {
        dbg("out of memory\n");
        return -ENOMEM;
    }

    new->fmpmip_flow_cache_entry = fmpmip_flow_cache_entry;
    hash_ind  = calc_hash(h->buckets, fmpmip_flow_cache_entry);
    hptr = h->hash_buckets[hash_ind];
    if (hptr) {
        while (hptr->next){
            if (match(hptr, fmpmip_flow_cache_entry)) {
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
                if ((err = func(hptr->fmpmip_flow_cache_entry, arg)))
                    return err;
                hptr = nptr;
            }
        }
    }
    return 0;
}

void flow_mob_hash_delete(const struct flow_mob_hash_structure *h, fmpmip_flow_cache_entry * fmpmip_flow_cache_entry)
{
    struct flow_mob_hash_entry *hptr, *pptr, *head;
    int hash_ind;

    assert(h);
    assert(fmpmip_flow_cache_entry);

    hash_ind = calc_hash(h->buckets, fmpmip_flow_cache_entry);
    head = h->hash_buckets[hash_ind];
    
    pptr = hptr = head;
    while (hptr) {
        if (match(hptr, fmpmip_flow_cache_entry)){
            if (hptr != head) 
                pptr->next = hptr->next;
            else {
                /* First entry */ 
                h->hash_buckets[hash_ind] = hptr->next;
            }
            free(hptr->fmpmip_flow_cache_entry);
            hptr->fmpmip_flow_cache_entry = NULL;
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
