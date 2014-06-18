#ifndef __FMPMIP_PACKET_MARKER_H__
#    define __FMPMIP_PACKET_MARKER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>


int flowMarker_add_fwmark(struct in6_addr ip_source,struct in6_addr ip_destination,
					int transport_protocol, 
					int transport_protocol_source_port,int transport_protocol_destination_port,uint32_t mark);

int flowMarker_delete_mark(struct in6_addr ip_source,struct in6_addr ip_destination,
					int transport_protocol, 
					int transport_protocol_source_port,int transport_protocol_destination_port, uint32_t mark);


#endif