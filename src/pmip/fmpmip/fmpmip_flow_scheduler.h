
#ifndef __FMPMIP_FLOW_SCHEDULER_H__
#    define __FMPMIP_FLOW_SCHEDULER_H__
//-----------------------------------------------------------------------------
#ifdef FMPMIP_FLOW_SCHEDULER_C
#        define private_flow_scheduler(x) x
#        define protected_flow_scheduler(x) x
#        define public_flow_scheduler(x) x
#    else
#            define private_flow_scheduler(x)
#            define protected_flow_scheduler(x)
#            define public_flow_scheduler(x) extern x
#endif
//-----------------------------------------------------------------------------
#    include "util.h"
#    include "fmpmip/fmpmip_flowmob_cache.h"

//just for convenience
typedef struct mn_nai_association{
     ip6mn_nai_t client_nai;
     struct in6_addr ip6;
     int BID;
} mn_nai_association;

public_flow_scheduler(int flowScheduler_schedule_flow(struct in6_addr ip6_origin,struct in6_addr ip6_destination,
											uint32_t transport_protocol, uint16_t transport_source_port,
											uint16_t transport_destination_port,
											uint8_t traffic_class, uint32_t flow_label);)

public_flow_scheduler(int flowScheduler_link_down( ip6mn_nai_t mn_nai, struct in6_addr serving_mag);)

public_flow_scheduler(void flowScheduler_link_up( ip6mn_nai_t mn_nai, struct in6_addr serving_mag);)

private_flow_scheduler( int get_flow_fwmark(ip6mn_nai_t mn_nai);)

private_flow_scheduler( static int flowScheduler_change_mark(void *data, void *arg);)
private_flow_scheduler( static int flowScheduler_new_path(void *data, void *arg);)


#endif
/** @}*/
