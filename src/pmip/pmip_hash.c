/*
 * $Id: hash.c 1.22 06/04/25 13:24:14+03:00 anttit@tcs.hut.fi $
 *
 * This file is part of the MIPL Mobile IPv6 for Linux.
 * 
 * Author: Henrik Petander <petander@tcs.hut.fi>
 *
 * Copyright 2003-2005 Go-Core Project
 * Copyright 2003-2006 Helsinki University of Technology
 *
 * MIPL Mobile IPv6 for Linux is free software; you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; version 2 of
 * the License.
 *
 * MIPL Mobile IPv6 for Linux is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MIPL Mobile IPv6 for Linux; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA
 */

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
#include "pmip_hash.h"
#include "util.h"

/* Hash entry */

 struct hash_entry {
	ip6mn_nai_t *mn_nai; /* pointer to mn_nai in data */
	struct in6_addr *mag_address; /* pointer to mag_address in data */
 	struct hash_entry *next;
 	void *data;
 };

 static uint32_t calc_hash(int s, const ip6mn_nai_t *mn_nai,
 	const struct in6_addr *mag_address)
 {
 	assert(mn_nai);
 	uint32_t hash=0;

 	int i=0;

 	for(i=0;i<NAI_SIZE;i++){
 		hash= mn_nai->nai[i] ^ hash;
 	}
 	hash = hash ^ mag_address->s6_addr32[0] ^
 	mag_address->s6_addr32[1] ^
 	mag_address->s6_addr32[2] ^
 	mag_address->s6_addr32[3];

 	hash = hash % s;

 	return hash;

 }


 static int match(struct hash_entry *h, const ip6mn_nai_t *mn_nai, 
 	const struct in6_addr *mag_address)
 {
 	assert(h);
 	assert(mn_nai && h->mn_nai);
 	assert(mag_address && h->mag_address);

 	if(memcmp(mn_nai , h->mn_nai , sizeof(ip6mn_nai_t)) ==0 && IN6_ARE_ADDR_EQUAL(h->mag_address, mag_address)){
 		return 1;
 	}

 	return 0;

 }


/* Caller must have reserved the memory for the hash */
 int pmip_hash_init(struct pmip_hash *h, int buckets)
 {
 	assert(h);
 	assert(buckets);
 	h->hash_buckets = (struct hash_entry **)malloc(buckets * sizeof(struct hash_entry *));
 	if (!h->hash_buckets)
 		return -ENOMEM;
 	memset(h->hash_buckets, 0, buckets * sizeof(struct hash_entry *));
 	h->buckets = buckets;
 	return 0;
 }

 void pmip_hash_cleanup(struct pmip_hash *h)
 {
 	struct hash_entry *he, *tmp;
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

 void *pmip_hash_get(const struct pmip_hash *h,
 	const ip6mn_nai_t *mn_nai, 
 	const struct in6_addr *mag_address)
 {
 	struct hash_entry *hptr = NULL;

 	assert(h);
 	assert(mag_address);

 	uint32_t hashind=calc_hash(h->buckets, mn_nai , mag_address);
 	hptr = h->hash_buckets[hashind];

 	while(hptr) {
 		if (match(hptr, mn_nai, mag_address))
 			return hptr->data;
 		hptr = hptr->next;
 	}
 	return NULL;
 }

 int pmip_hash_add(const struct pmip_hash *h, void *data,
 	ip6mn_nai_t *mn_nai, struct in6_addr *mag_address)
 {
 	struct hash_entry *hptr = NULL, *new;
 	uint32_t hash_ind;

 	assert(h); 
 		assert(mn_nai); 
 	assert(mag_address);

 	if ((new = (struct hash_entry *)malloc(sizeof(struct hash_entry))) == NULL) {
 		dbg("out of memory\n");
 		return -ENOMEM;
 	}
 	new->mn_nai = mn_nai;
 	new->mag_address = mag_address;
 	new->data = data;
 	hash_ind  = calc_hash(h->buckets, mn_nai, mag_address);
 	hptr = h->hash_buckets[hash_ind];
 	if (hptr) {
 		while (hptr->next){
 			if (match(hptr, mn_nai, mag_address)) {
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

 int pmip_hash_iterate(const struct pmip_hash *h, int (*func)(void *, void *), void *arg)
 {
 	int err = 0;
 	int i;
 	struct hash_entry *hptr, *nptr;

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

 void pmip_hash_delete(const struct pmip_hash *h,
 	const ip6mn_nai_t *mn_nai,
 	const struct in6_addr *mag_address)
 {
 	struct hash_entry *hptr, *pptr, *head;
 	int hash_ind;

 	assert(h);
 	 assert(mn_nai);
 	assert(mag_address);

 	hash_ind = calc_hash(h->buckets, mn_nai, mag_address);
 	head = h->hash_buckets[hash_ind];

 	pptr = hptr = head;
 	while (hptr) {
 		if (match(hptr, mn_nai, mag_address)){
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
