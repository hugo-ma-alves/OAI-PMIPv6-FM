
#ifndef __FMPMIP_FLOWMOB_CACHE_H__
#    define __FMPMIP_FLOWMOB_CACHE_H__
//-----------------------------------------------------------------------------
#    ifdef FMPMIP_FLOWMOB_CACHE_C
#        define private_fmpmip_flowmob_cache(x) x
#        define protected_fmpmip_flowmob_cache(x) x
#        define public_fmpmip_flowmob_cache(x) x
#    else
#        ifdef PMIP
#            define private_fmpmip_flowmob_cache(x)
#            define protected_fmpmip_flowmob_cache(x) extern x
#            define public_fmpmip_flowmob_cache(x) extern x
#        else
#            define private_fmpmip_flowmob_cache(x)
#            define protected_fmpmip_flowmob_cache(x)
#            define public_fmpmip_flowmob_cache(x) extern x
#        endif
#    endif
//-----------------------------------------------------------------------------
#    include <pthread.h>
#    include <arpa/inet.h>
#    include <netinet/in.h>
//-----------------------------------------------------------------------------
#    include "tqueue.h"
#    include "util.h"
#    include "hash.h"
//-----------------------------------------------------------------------------
#    include "pmip_types.h"

 typedef struct fmpmip_flow_cache_entry{

 	int BID;
 	uint32_t	mark;
 	uint32_t	priority;
 	uint32_t	action;
 	int 		active;
	struct in6_addr ip6_origin;
	struct in6_addr ip6_destination;
	int transport_protocol;
	uint16_t transport_source_port; //in host byte order
	uint16_t transport_destination_port;
	pthread_rwlock_t 		lock;				/*!< \brief Protects the entry, for internal use only*/

}fmpmip_flow_cache_entry;


 struct flow_mob_hash_entry;

/** Hash structure
 * @buckets : number of buckets in hash 
 * @hash: array of hashbuckets 
 */
 struct flow_mob_hash_structure {
	int buckets;
	struct flow_mob_hash_entry **hash_buckets;
};


 protected_fmpmip_flowmob_cache( int get_flowmob_cache_count(void);)

 protected_fmpmip_flowmob_cache( int flowmob_cache_init(void);)

 protected_fmpmip_flowmob_cache( fmpmip_flow_cache_entry * flowmob_cache_alloc(void);)

 protected_fmpmip_flowmob_cache( fmpmip_flow_cache_entry * flowmob_cache_add(fmpmip_flow_cache_entry * bce);)

 protected_fmpmip_flowmob_cache( fmpmip_flow_cache_entry * flowmob_cache_get(fmpmip_flow_cache_entry * bce);)

 protected_fmpmip_flowmob_cache( void flowmob_cache_release_entry(fmpmip_flow_cache_entry * bce);)

 protected_fmpmip_flowmob_cache( int flowmob_cache_exists(fmpmip_flow_cache_entry * bce);)

protected_fmpmip_flowmob_cache(  void flowmob_cache_free(fmpmip_flow_cache_entry * bce);)

 protected_fmpmip_flowmob_cache( void flowmob_entry_delete(fmpmip_flow_cache_entry * bce);)




/** hash_init - initializes hash 
 * 
 * @h: uninitialized memory with enough space for hash 
 * @type: either double address, or single address
 * @buckets: number of hashbuckets in hash
 */  
 private_fmpmip_flowmob_cache( int flow_mob_hash_init(struct flow_mob_hash_structure *h, int buckets);)

/** hash_cleanup - cleans up hash and all its entries
 *
 */
 private_fmpmip_flowmob_cache( void flow_mob_hash_cleanup(struct flow_mob_hash_structure *h);)
/*
 *  Get data stored in a hash entry based on one or two addresses, depending on type of hash
 */
 private_fmpmip_flowmob_cache( fmpmip_flow_cache_entry * flow_mob_hash_get(const struct flow_mob_hash_structure *h, fmpmip_flow_cache_entry * fmpmip_flow_cache_entry) ;)

/* Iterate through the hash and call func for every entry */

 private_fmpmip_flowmob_cache( int flow_mob_hash_iterate(const struct flow_mob_hash_structure *h, int (*func)(void *data, void *arg), void *arg);)

/* Add a hash entry to hash */		 
 private_fmpmip_flowmob_cache( int flow_mob_hash_add(const struct flow_mob_hash_structure *h, fmpmip_flow_cache_entry * fmpmip_flow_cache_entry);)

/*
 *Delete entry from hash
 */
 private_fmpmip_flowmob_cache( void flow_mob_hash_delete(const struct flow_mob_hash_structure *h, fmpmip_flow_cache_entry * fmpmip_flow_cache_entry);)

#endif
/** @}*/
