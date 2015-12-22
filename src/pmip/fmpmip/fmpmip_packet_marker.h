#ifndef __FMPMIP_PACKET_MARKER_H__
#    define __FMPMIP_PACKET_MARKER_H__

<<<<<<< HEAD

=======
>>>>>>> origin/implementation-draft-ietf-netext-pmipv6-flowmob-08
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <linux/netfilter.h>		/* for NF_ACCEPT */
#include <netinet/tcp.h>			/* for TCP header*/
#include <netinet/udp.h>			/* for UDP header*/
#include <netinet/ip6.h>			/* for IPv6 header*/

#include <libnetfilter_queue/libnetfilter_queue.h>
#include "flowmob_hash_struct.h"

struct packet {
	
	int len;
	unsigned char * raw_packet; 
	struct ip6_hdr *ip6;
	/*Insert extensions headers here, and set the pointers in the parse function;
	struct nd_neighbor_solicit *ns;
	struct nd_neighbor_advert *na;
	struct nd_opt_hdr *opt_slla;
	struct nd_opt_hdr *opt_tlla;
	*/

	int transport_protocol;

	struct tcphdr *tcp;
	struct udphdr *udp;

	uint16_t transport_source_port; //in host byte order
	uint16_t transport_destination_port; //in host byte order

};


int fmpmip_flow_handler_init(uint16_t userSpace_queue_id);
=======


int flowMarker_add_fwmark(struct in6_addr ip_source,struct in6_addr ip_destination,
					int transport_protocol, 
					int transport_protocol_source_port,int transport_protocol_destination_port,uint32_t mark);

int flowMarker_delete_mark(struct in6_addr ip_source,struct in6_addr ip_destination,
					int transport_protocol, 
					int transport_protocol_source_port,int transport_protocol_destination_port, uint32_t mark);

>>>>>>> origin/implementation-draft-ietf-netext-pmipv6-flowmob-08

#endif