
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
#    include "pmip_types.h"
#	 include "fmpmip_flowmob_hash.h"

//-----------------------------------------------------------------------------

typedef struct fmpmip_flow_cache_entry{

	int 				BID;
	ip6mn_nai_t			client_nai;
	uint32_t			mark;
	uint32_t			priority;
	uint32_t			action;
	int 				active;
	struct in6_addr 	ip6_origin;
	struct in6_addr 	ip6_destination;
	uint8_t 			traffic_class;
	uint32_t			flow_label;
	int 				transport_protocol;
	uint16_t 			transport_source_port; //in host byte order
	uint16_t 			transport_destination_port;
	pthread_rwlock_t 	lock;				/*!< \brief Protects the entry, for internal use only*/

}fmpmip_flow_cache_entry;


protected_fmpmip_flowmob_cache( int flowmob_cache_get_count(void);)

protected_fmpmip_flowmob_cache( int flowmob_cache_init(void);)

protected_fmpmip_flowmob_cache( fmpmip_flow_cache_entry * flowmob_cache_alloc(void);)

protected_fmpmip_flowmob_cache( fmpmip_flow_cache_entry * flowmob_cache_add(fmpmip_flow_cache_entry * fce);)

protected_fmpmip_flowmob_cache( fmpmip_flow_cache_entry * flowmob_cache_get(struct in6_addr *ip6_source,
	struct in6_addr *ip6_destination,
	uint32_t  transport_protocol ,
	uint16_t  transport_source_port ,
	uint16_t  transport_destination_port ,
	uint8_t 			traffic_class,
	uint32_t			flow_label);)

protected_fmpmip_flowmob_cache( void flowmob_cache_release_entry(fmpmip_flow_cache_entry * fce);)

protected_fmpmip_flowmob_cache( int flowmob_cache_exists(struct in6_addr *ip6_source,
	struct in6_addr *ip6_destination,
	uint32_t  transport_protocol ,
	uint16_t  transport_source_port ,
	uint16_t  transport_destination_port ,
	uint8_t             traffic_class,
	uint32_t            flow_label);)

protected_fmpmip_flowmob_cache(  void flowmob_cache_free(fmpmip_flow_cache_entry * fce);)

protected_fmpmip_flowmob_cache( void flowmob_entry_delete(fmpmip_flow_cache_entry * fce);)

protected_fmpmip_flowmob_cache( int flowmob_cache_iterate(int (*func) (void *, void *), void *arg);)




#endif
/** @}*/
