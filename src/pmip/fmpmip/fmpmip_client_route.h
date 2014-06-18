
#ifndef __FMPMIP_CLIENT_ROUTE_H__
#    define __FMPMIP_CLIENT_ROUTE_H__
//-----------------------------------------------------------------------------
#ifdef __FMPMIP_CLIENT_ROUTE_C
#        define private_client_routing(x) x
#        define protected_client_routing(x) x
#        define public_client_routing(x) x
#    else
#            define private_client_routing(x)
#            define protected_client_routing(x)
#            define public_client_routing(x) extern x
#endif
//-----------------------------------------------------------------------------
#    include "util.h"
#    include "pmip/pmip_types.h"
#	 include "conf.h"

//-----------------------------------------------------------------------------

struct mag{
	struct in6_addr	ip6_addr;
	uint32_t		mark;
	struct mag *next;
};


struct route{
	ip6mn_nai_t client_nai;
	struct mag *mag;
	struct route * next;
};

protected_client_routing(pthread_rwlock_t client_route_list_lock;)


public_client_routing (int add_client_route(ip6mn_nai_t client_nai, struct in6_addr mag_address);)
public_client_routing (int client_route_init());
public_client_routing (int client_route_iterate( int (*func)(void *, void *), void *arg);)
public_client_routing (int remove_client_route(ip6mn_nai_t client_nai, struct in6_addr mag_address);)
public_client_routing (void remove_client(ip6mn_nai_t client_nai);)

public_client_routing( struct route * client_is_in_list(ip6mn_nai_t client_nai);)

private_client_routing( struct route * getClient(ip6mn_nai_t client_nai);)
private_client_routing( int add_new_client(ip6mn_nai_t client_nai, struct route **route);)
private_client_routing( int get_mag_mark(struct in6_addr mag_address);)
private_client_routing( int add_route_to_client(struct route *client, struct in6_addr mag_address);)





#endif
/** @}*/
