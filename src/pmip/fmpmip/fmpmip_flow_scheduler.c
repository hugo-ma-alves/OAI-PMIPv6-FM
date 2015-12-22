
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
    struct  mn_nai_association *association_struct = (mn_nai_association *) mn_nai_ip;
    
    if(IN6_ARE_ADDR_EQUAL(&association_struct->ip6,&bce->mn_addr)){
        memcpy(&association_struct->client_nai ,&bce->mn_nai, sizeof(ip6mn_nai_t));
        dbg("Found NAI \n");
        association_struct->BID= bce->BID;
        return 1;
    }
    return 0;

}


int flowScheduler_schedule_flow(struct in6_addr ip6_origin,struct in6_addr ip6_destination,
    uint32_t transport_protocol, uint16_t transport_source_port,
    uint16_t transport_destination_port,
    uint8_t traffic_class, uint32_t flow_label){

    fmpmip_flow_cache_entry *fce;
    struct mn_nai_association mn_nai_ip;
    memset(&mn_nai_ip, 0, sizeof(struct mn_nai_association));
    uint32_t  mark=0;

    mn_nai_ip.ip6 = ip6_destination;

    pmip_cache_iterate(find_NAI, &mn_nai_ip);

    fce = flowmob_cache_get(&ip6_origin,&ip6_destination,
        transport_protocol,transport_source_port,
        transport_destination_port,traffic_class,
        flow_label);

    if(fce != NULL){
        dbg("This flow is already in the flow cache table with the mark %d\n",fce->mark);
        flowmob_cache_release_entry(fce);
        return mark;
    }

    mark = get_flow_fwmark(mn_nai_ip.client_nai) ;
    if(mark>0){
       fmpmip_flow_cache_entry * flowMob= flowmob_cache_alloc();
       memcpy(&flowMob->ip6_origin , &ip6_origin, sizeof(struct in6_addr));
       memcpy(&flowMob->ip6_destination , &ip6_destination, sizeof(struct in6_addr));
       flowMob->transport_protocol = transport_protocol;
       flowMob->transport_source_port = transport_source_port;
       flowMob->transport_destination_port = transport_destination_port;
       flowMob->traffic_class = traffic_class;
       flowMob->flow_label = flow_label;
       flowMob->mark=mark;
       flowMob->client_nai=mn_nai_ip.client_nai;
       flowMob->BID=mn_nai_ip.BID;
       flowmob_cache_add(flowMob);
       dbg("This is a new flow. A new entry in the flow cache table was created \n");
   }else{
       dbg("Can't match this flow woth any of the connected clients\n");
       dbg("This flow will continue withouth intervention.\n");
   }

   return mark;
}



int flowScheduler_link_down( ip6mn_nai_t mn_nai, struct in6_addr serving_mag){

    dbg("Link down - Rescheduling all flows from client [PUT NAI HERE] passing in MAG %x:%x:%x:%x:%x:%x:%x:%x \n",NIP6ADDR(&serving_mag));

    flowmob_cache_iterate(flowScheduler_change_mark, &mn_nai);

    dbg("Flows Rescheduled\n");
}

void flowScheduler_link_up( ip6mn_nai_t mn_nai, struct in6_addr serving_mag){

    dbg("Link UP - Rescheduling all flows from client [PUT NAI HERE] passing in MAG %x:%x:%x:%x:%x:%x:%x:%x \n",NIP6ADDR(&serving_mag));

    flowmob_cache_iterate(flowScheduler_new_path, &mn_nai);

    dbg("Flows Rescheduled\n");
}


static int flowScheduler_change_mark(void *data, void *arg){

    ip6mn_nai_t *mn_nai = (ip6mn_nai_t*)arg;
    fmpmip_flow_cache_entry *fce = (fmpmip_flow_cache_entry *) data;
   
    /*int naiSize=sizeof(ip6mn_nai_t);
    char nai[naiSize+1];
    nai[naiSize]='\0';
    memcpy(&nai,mn_nai,naiSize);
    dbg("Client(NAI): %s\n", nai);
    memcpy(&nai,&fce->client_nai,naiSize);
    dbg("Client(NAI): %s\n", nai);*/

    if( memcmp(mn_nai,&fce->client_nai, sizeof(ip6mn_nai_t))!=0){
     return;
 }
 
 uint32_t oldfwmark = fce->mark;
 uint32_t newMark = get_flow_fwmark(*mn_nai);

 if(newMark==0){
    dbg("Error: Tried to do an handover but there are no paths avaiable\n");
    dbg("Deleting the flow mob entry \n");
    flowMarker_delete_mark(fce->ip6_origin, fce->ip6_destination, fce->transport_protocol,
    fce->transport_source_port, fce->transport_destination_port, oldfwmark);
    flowmob_entry_delete(fce);
}else{
    dbg("Handover done sucessfully from fwmark %d to %d\n",oldfwmark,newMark);
    fce->mark=newMark;
    flowMarker_add_fwmark(fce->ip6_origin, fce->ip6_destination, fce->transport_protocol,
        fce->transport_source_port, fce->transport_destination_port, newMark);
    flowMarker_delete_mark(fce->ip6_origin, fce->ip6_destination, fce->transport_protocol,
    fce->transport_source_port, fce->transport_destination_port, oldfwmark);
}



return 0;
}

static int flowScheduler_new_path(void *data, void *arg){

    fmpmip_flow_cache_entry *fce = (fmpmip_flow_cache_entry *) data;


    uint32_t newMark = get_flow_fwmark(fce->client_nai);

    if(newMark != fce->mark){
        flowMarker_add_fwmark(fce->ip6_origin, fce->ip6_destination, fce->transport_protocol,
            fce->transport_source_port, fce->transport_destination_port, newMark);
        flowMarker_delete_mark(fce->ip6_origin, fce->ip6_destination, fce->transport_protocol,
            fce->transport_source_port, fce->transport_destination_port, fce->mark);
        dbg("Handover of a flow done sucessfully from fwmark %d to %d\n",fce->mark,newMark);
                fce->mark=newMark;

    }
    return 0;
}

int get_flow_fwmark(ip6mn_nai_t mn_nai){

    int mark=0;
    int r = rand() % 20;
    struct route *client = get_route_for_client(mn_nai);
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
    client_route_release_entry(client);
    return mark;
}