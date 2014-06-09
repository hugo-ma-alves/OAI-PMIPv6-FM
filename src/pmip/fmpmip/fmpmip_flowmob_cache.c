
#define PMIP
#define FMPMIP_FLOWMOB_CACHE_C

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef ENABLE_VT
#include "vt.h"
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

#include "fmpmip_flowmob_cache.h"
#include "fmpmip_flowmob_hash.h"

#define FLOWMOB_CACHE_BUCKETS              128


static struct flow_mob_hash_structure flowmob_hash;
static int    flowmob_cache_count = 0;
pthread_rwlock_t flowmob_cache_lock;


int flowmob_cache_get_count()
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

#ifdef ENABLE_VT
    if (ret < 0)
        return ret;
    ret = vt_fmbc_init();
#endif
    return ret;
}

fmpmip_flow_cache_entry * flowmob_cache_alloc(void){

   fmpmip_flow_cache_entry *tmp;
   tmp = malloc(sizeof(fmpmip_flow_cache_entry));

   if (tmp == NULL) {
    dbg("Error - NO memory allocated for Flow mobility cache entry..\n");
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

static int __insert_in_hashtable(fmpmip_flow_cache_entry * fce)
{
    int ret;
    ret = flow_mob_hash_add(&flowmob_hash, fce, &fce->ip6_origin,
        &fce->ip6_destination,
        fce->transport_protocol, fce->transport_source_port, fce->transport_destination_port,
        fce->traffic_class, fce->flow_label);
    if (ret) {
        return ret;
    }
    flowmob_cache_count++;
    return 0;
}

fmpmip_flow_cache_entry * flowmob_cache_add(fmpmip_flow_cache_entry * fce){

    int ret = 1;
    int mutex_return_code;
    assert(fce);
    mutex_return_code = pthread_rwlock_wrlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_wrlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }
    if ((ret = __insert_in_hashtable(fce)) != 0) {
        pthread_rwlock_unlock(&flowmob_cache_lock);
        dbg("WARNING: Flow mob cache entry ENTRY NOT INSERTED\n");
        return NULL;
    }
    dbg("Created a new FLow mob entry\n");
    mutex_return_code = pthread_rwlock_unlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_unlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }
    return fce;

}


fmpmip_flow_cache_entry * flowmob_cache_get(struct in6_addr *ip6_source,
    struct in6_addr *ip6_destination,
    uint32_t  transport_protocol ,
    uint16_t  transport_source_port ,
    uint16_t  transport_destination_port ,
    uint8_t             traffic_class,
    uint32_t            flow_label){

    int mutex_return_code;
    fmpmip_flow_cache_entry *fce;

    mutex_return_code = pthread_rwlock_rdlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_rdlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }

    fce = flow_mob_hash_get(&flowmob_hash,  ip6_source,
     ip6_destination,
     transport_protocol ,
     transport_source_port ,
     transport_destination_port ,
     traffic_class,
     flow_label);

    if (fce) {
        mutex_return_code = pthread_rwlock_wrlock(&fce->lock);
        if (mutex_return_code != 0) {
            dbg("pthread_rwlock_wrlock(&fce->lock) %s\n", strerror(mutex_return_code));
        }
    } else {
        mutex_return_code = pthread_rwlock_unlock(&flowmob_cache_lock);
        if (mutex_return_code != 0) {
            dbg("pthread_rwlock_unlock(&fce) %s\n", strerror(mutex_return_code));
        }
    }
    return fce;
}


void flowmob_cache_release_entry(fmpmip_flow_cache_entry * fce){

 int mutex_return_code;
 assert(fce);
 mutex_return_code = pthread_rwlock_unlock(&fce->lock);
 if (mutex_return_code != 0) {
    dbg("pthread_rwlock_unlock(&fce->lock) %s\n", strerror(mutex_return_code));
}
mutex_return_code = pthread_rwlock_unlock(&flowmob_cache_lock);
if (mutex_return_code != 0) {
    dbg("pthread_rwlock_unlock(&fce) %s\n", strerror(mutex_return_code));
}
}


int flowmob_cache_exists(struct in6_addr *ip6_source,
    struct in6_addr *ip6_destination,
    uint32_t  transport_protocol ,
    uint16_t  transport_source_port ,
    uint16_t  transport_destination_port ,
    uint8_t             traffic_class,
    uint32_t            flow_label){

 fmpmip_flow_cache_entry *fce;

 fce = flowmob_cache_get( ip6_source,
     ip6_destination,
     transport_protocol ,
     transport_source_port ,
     transport_destination_port ,
     traffic_class,
     flow_label);

 if (fce == NULL) {
    return -1;
}
flowmob_cache_release_entry(fce);
return 1;

}

void flowmob_cache_free(fmpmip_flow_cache_entry * fce){

  int mutex_return_code;
  mutex_return_code = pthread_rwlock_destroy(&fce->lock);
  if (mutex_return_code != 0) {
    dbg("pthread_rwlock_destroy(&fce->lock) %s\n", strerror(mutex_return_code));
}
free(fce);
}


void flowmob_entry_delete(struct in6_addr *ip6_source,
    struct in6_addr *ip6_destination,
    uint32_t  transport_protocol ,
    uint16_t  transport_source_port ,
    uint16_t  transport_destination_port ,
    uint8_t             traffic_class,
    uint32_t            flow_label){

 int mutex_return_code;
 fmpmip_flow_cache_entry *fce;

 fce = flowmob_cache_get( ip6_source,
     ip6_destination,
     transport_protocol ,
     transport_source_port ,
     transport_destination_port ,
     traffic_class,
     flow_label);

 mutex_return_code = pthread_rwlock_wrlock(&fce->lock);
 if (mutex_return_code != 0) {
    dbg("pthread_rwlock_wrlock(&fce->lock) %s\n", strerror(mutex_return_code));
}

flowmob_cache_count--;

flow_mob_hash_delete(&flowmob_hash, &fce->ip6_origin, &fce->ip6_destination, fce->transport_protocol,
    fce->transport_source_port, fce->transport_destination_port,
    fce->traffic_class, fce->flow_label);

mutex_return_code = pthread_rwlock_unlock(&fce->lock);
if (mutex_return_code != 0) {
    dbg("pthread_rwlock_unlock(&fce->lock) %s\n", strerror(mutex_return_code));
}

flowmob_cache_free(fce);
dbg("Deleted FLow mob cache entry!\n");


}


int flowmob_cache_iterate(int (*func) (void *, void *), void *arg)
{
    int err;
    int mutex_return_code;
    mutex_return_code = pthread_rwlock_rdlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_rdlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }
    err = flow_mob_hash_iterate(&flowmob_hash, func, arg);
    mutex_return_code = pthread_rwlock_unlock(&flowmob_cache_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_unlock(&flowmob_cache_lock) %s\n", strerror(mutex_return_code));
    }
    return err;
}

