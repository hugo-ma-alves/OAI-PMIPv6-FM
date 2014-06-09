
#ifndef __FMPMIP_FLOWMOB_HASH_H__
#    define __FMPMIP_FLOWMOB_HASH_H__
//-----------------------------------------------------------------------------
#    ifdef FMPMIP_FLOWMOB_HASH_C
#        define private_fmpmip_flowmob_hash(x) x
#        define protected_fmpmip_flowmob_hash(x) x
#        define public_fmpmip_flowmob_hash(x) x
#    else
#        ifdef PMIP
#            define private_fmpmip_flowmob_hash(x)
#            define protected_fmpmip_flowmob_hash(x) extern x
#            define public_fmpmip_flowmob_hash(x) extern x
#        else
#            define private_fmpmip_flowmob_hash(x)
#            define protected_fmpmip_flowmob_hash(x)
#            define public_fmpmip_flowmob_hash(x) extern x
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


 /* Hash entry */
struct flow_mob_hash_entry {
    struct in6_addr *ip6_destination; /* pointer to ip6 destination in data */
    struct in6_addr *ip6_source; /* pointer to ip6 source in data */
    uint32_t  transport_protocol  ; /* Transport protocol */
    uint16_t  transport_source_port ; /* transport potocol source port */
    uint16_t  transport_destination_port  ; /* transport protocol destination port */
	uint8_t 			traffic_class;
	uint32_t			flow_label;
	struct flow_mob_hash_entry *next;
	void *data;
};


/** Hash structure
 * @buckets : number of buckets in hash 
 * @hash: array of hashbuckets 
 */
 struct flow_mob_hash_structure {
 	int buckets;
 	struct flow_mob_hash_entry **hash_buckets;
 };


/** hash_init - initializes hash 
 * 
 * @h: uninitialized memory with enough space for hash 
 * @type: either double address, or single address
 * @buckets: number of hashbuckets in hash
 */  
 protected_fmpmip_flowmob_hash( int flow_mob_hash_init(struct flow_mob_hash_structure *h, int buckets);)

/** hash_cleanup - cleans up hash and all its entries
 *
 */
 protected_fmpmip_flowmob_hash( void flow_mob_hash_cleanup(struct flow_mob_hash_structure *h);)
/*
 *  Get data stored in a hash entry based on one or two addresses, depending on type of hash
 */
 protected_fmpmip_flowmob_hash( void * flow_mob_hash_get(
 	const struct flow_mob_hash_structure *h, 
 	struct in6_addr *ip6_source,
 	struct in6_addr *ip6_destination,
 	uint32_t  transport_protocol ,
 	uint16_t  transport_source_port ,
 	uint16_t  transport_destination_port ,
 	uint8_t 			traffic_class,
 	uint32_t			flow_label) ;)

/* Iterate through the hash and call func for every entry */

 protected_fmpmip_flowmob_hash( int flow_mob_hash_iterate(const struct flow_mob_hash_structure *h, int (*func)(void *data, void *arg), void *arg);)

/* Add a hash entry to hash */		 
 protected_fmpmip_flowmob_hash( int flow_mob_hash_add(const struct flow_mob_hash_structure *h, void *data , struct in6_addr *ip6_source,
 	struct in6_addr *ip6_destination,
 	uint32_t  transport_protocol ,
 	uint16_t  transport_source_port ,
 	uint16_t  transport_destination_port ,
 	uint8_t 			traffic_class,
 	uint32_t			flow_label );)

/*
 *Delete entry from hash
 */
protected_fmpmip_flowmob_hash( void  flow_mob_hash_delete(
 	const struct flow_mob_hash_structure *h, 
 	struct in6_addr *ip6_source,
 	struct in6_addr *ip6_destination,
 	uint32_t  transport_protocol ,
 	uint16_t  transport_source_port ,
 	uint16_t  transport_destination_port ,
 	uint8_t 			traffic_class,
 	uint32_t			flow_label) ;)


#endif
/** @}*/
