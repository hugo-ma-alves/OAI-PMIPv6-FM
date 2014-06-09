
#define PMIP
#define FMPMIP_FLOW_SCHEDULER_C
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdlib.h>
#include <time.h>
//---------------------------------------------------------------------------------------------------------------------
#include "pmip_hnp_cache.h"
#include "pmip_consts.h"
#include "pmip/pmip_cache.h"

#include "util.h"
#include "debug.h"
#include "conf.h"

#include "fmpmip/fmpmip_flowmob_cache.h"

#include "fmpmip/fmpmip_flow_scheduler.h"

#include "fmpmip_client_route.h"

//---------------------------------------------------------------------------------------------------------------------
static int find_NAI(void *pmip_cache_entry,  void *mn_nai_ip)
{
    pmip_entry_t *bce = (pmip_entry_t *) pmip_cache_entry;
    struct  mn_nai_association *flowMob = (mn_nai_association *) mn_nai_ip;
    
    if(IN6_ARE_ADDR_EQUAL(&flowMob->ip6,&bce->mn_addr)){
        memcpy(&flowMob->client_nai ,&bce->mn_nai, sizeof(ip6mn_nai_t));
        dbg("Found NAI \n");
        flowMob->BID= bce->BID;
        return 1;
    }
    dbg("Did not found NAI \n");
    return 0;

}



int flowScheduler_schedule_flow(struct in6_addr ip6_origin,struct in6_addr ip6_destination,
    uint32_t transport_protocol, uint16_t transport_source_port,
    uint16_t transport_destination_port,
    uint8_t traffic_class, uint32_t flow_label){


    struct mn_nai_association mn_nai_ip;
    mn_nai_ip.ip6 = ip6_destination;

    pmip_cache_iterate(find_NAI, &mn_nai_ip);

    uint32_t  mark =0  ;
   // srand(time(NULL));
    int r = rand() % 20;

    struct route *client = client_is_in_list(mn_nai_ip.client_nai);
    if(client==NULL){
        return mark;
    }

    struct mag *mag_list = client->mag;

    while(mag_list !=NULL){
        mark = mag_list->mark;
        if(r <10){
            break;
        }
        mag_list = mag_list->next;
    }

    if(flowmob_cache_exists(&ip6_origin,&ip6_destination,
        transport_protocol,transport_source_port,
        transport_destination_port,traffic_class,
        flow_label)>0){
        dbg("This flow is already in the flow cache table with the mark %d\n",mark);
}else{
    fmpmip_flow_cache_entry * flowMob= flowmob_cache_alloc();
    memcpy(&flowMob->ip6_origin , &ip6_origin, sizeof(struct in6_addr));
    memcpy(&flowMob->ip6_destination , &ip6_destination, sizeof(struct in6_addr));
    flowMob->transport_protocol = transport_protocol;
    flowMob->transport_source_port = transport_source_port;
    flowMob->transport_destination_port = transport_destination_port;
    flowMob->traffic_class = traffic_class;
    flowMob->flow_label = flow_label;
    flowMob->mark=mark;
    flowmob_cache_add(flowMob);
    dbg("This is a new flow. A new entry in the flow cache table was created \n");
}

return mark;
}


