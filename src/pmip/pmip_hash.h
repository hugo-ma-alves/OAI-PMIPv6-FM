/* $Id: hash.h 1.9 05/12/08 18:25:09+02:00 vnuorval@tcs.hut.fi $ */

#    include <pmip_types.h>

#ifndef __PMIP_HASH_H__
#define __PMIP_HASH_H__ 1


struct hash_entry;
struct in6_addr;

/** Hash structure
 * @buckets : number of buckets in hash 
 * @calc_hash: internal data
 * @match: internal data
 * @pmip_hash: array of hashbuckets 
 */
struct pmip_hash {
	int buckets;
	int type; 
	struct hash_entry **hash_buckets;
};

/** pmip_hash_init - initializes hash 
 * 
 * @h: uninitialized memory with enough space for hash 
 * @buckets: number of hashbuckets in hash
 */  
int pmip_hash_init(struct pmip_hash *h, int buckets);


/** hash_cleanup - cleans up hash and all its entries
 *
 */
void pmip_hash_cleanup(struct pmip_hash *h);
/*
 *  Get data stored in a hash entry based on one or two addresses, depending on type of hash
 */
void *pmip_hash_get(const struct pmip_hash *h,
	       const ip6mn_nai_t *mn_nai, 
	       const struct in6_addr *mag_addr) ;

/* Iterate through the hash and call func for every entry */

int pmip_hash_iterate(const struct pmip_hash *h, int (*func)(void *data, void *arg), void *arg);

/* Add a hash entry to pmip_hash */		 
int pmip_hash_add(const struct pmip_hash *h, void  *data,
	      ip6mn_nai_t *mn_nai, struct in6_addr *mag_addr);

/*
 *Delete entry from hash
 */

void pmip_hash_delete(const struct pmip_hash *h,
		 const ip6mn_nai_t  *mn_nai, 
		 const struct in6_addr *mag_addr);

#endif /* _PMIP_HASH_H */
