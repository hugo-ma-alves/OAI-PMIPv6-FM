
#include "debug.h"
#include "conf.h"
#include <pthread.h>
#include "fmpmip_packets_userspace_queue_handler.h"
#include "util.h"

#include "fmpmip_flow_scheduler.h"
#include "fmpmip_packet_marker.h"



struct nfq_handle *h;
struct nfq_q_handle *qh;

pthread_t iptables_queue_listener;

uint16_t packetsQueueID=0;

int fmpmip_flow_handler_init(uint16_t userSpace_queue_id){

	int res;
	packetsQueueID=userSpace_queue_id;
	dbg("opening library handle\n");
	h = nfq_open();
	if (!h) {
		dbg("error during nfq_open()\n");
		return 1;
	}

	dbg("unbinding existing nf_queue handler for AF_INET (if any)\n");
	if (nfq_unbind_pf(h, AF_INET) < 0) {
		dbg("error during nfq_unbind_pf()\n");
		return 1;
	}

	dbg("binding nfnetlink_queue as nf_queue handler for AF_INET\n");
	if (nfq_bind_pf(h, AF_INET) < 0) {
		dbg("error during nfq_bind_pf()\n");
		return 1;
	}

	dbg("binding this socket to queue %d\n",userSpace_queue_id);
	qh = nfq_create_queue(h, userSpace_queue_id , &process_queue_packet, NULL);
	if (!qh) {
		dbg("error during nfq_create_queue()\n");
		return 1;
	}

	dbg("setting copy_packet mode\n");
	if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
		dbg("can't set packet_copy mode\n");
		return 1;
	}

	res =pthread_create(&iptables_queue_listener, NULL, loop_queue, NULL);

	return res;
}


static void *loop_queue(__attribute__ ((unused)) void *arg)
{
	
	int fd;
	int rv;
	char buf[4096] __attribute__ ((aligned));

	fd = nfq_fd(h);

	while ((rv = recv(fd, buf, sizeof(buf), 0)) && rv >= 0) {
		nfq_handle_packet(h, buf, rv);
	}

	dbg("Stoping the processing of the queue %d\n",packetsQueueID);
	nfq_destroy_queue(qh);

	dbg("closing library handler\n");
	nfq_close(h);

	pthread_exit(NULL);

}

static int process_queue_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
	struct nfq_data *nfa, void *data)
{
	int len=0;
	int id;
	uint32_t mark=0;
	struct nfqnl_msg_packet_hdr *ph;
	int result=0;
	 char * payload;
	struct packet packet;

	dbg("Processing packet from the userspace queue\n");

	len = nfq_get_payload(nfa, &payload);
	ph = nfq_get_msg_packet_hdr(nfa);

	if (ph) {
		id = ntohl(ph->packet_id);
		result = parse_packet(payload, len, &packet );
		if(result > 0){
			mark = schedule_packet(packet);
		}
	}
	
	if(mark>0){
		return nfq_set_verdict_mark(qh, id, NF_ACCEPT,mark, 0, NULL);
	}else{
		dbg("This packet is already marked, or there was an error\n");
		return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
	}
}

int parse_packet( char *raw_packet, int len, struct packet *packet){

	 char ipversion = raw_packet[0];

	memset(packet,0,sizeof(struct packet));

	packet->len=len;
	packet->raw_packet= raw_packet;

	ipversion = ipversion >> 4;

	if(ipversion  == 0x45){
		dbg("IPV4 package found - Ignoring\n");
		return 0;
	}

	int transportProtocol=0;
	transportProtocol = parse_ipv6(packet);
	if(transportProtocol == IPPROTO_TCP ){
		dbg ("Found TCP transport layer\n");
		parse_tcp(packet);
	}else if(transportProtocol == IPPROTO_UDP){
		dbg ("Found UDP transport layer\n");
		parse_udp(packet);
	}else{
		dbg("Analyzed a network packet but not found a valid transport protocol. Ignoring this packet.\n");
		return 0;
	}

	dump_packet(*packet);
	return 1;
	
}


void parse_tcp(struct packet *packet){

	struct tcphdr	*tcp;

	tcp = (struct tcphdr *)packet->raw_packet;
	packet->tcp = tcp;

	packet->raw_packet += sizeof(struct tcphdr);
	packet->len -= sizeof(struct tcphdr);

	packet->transport_source_port= ntohs(tcp->source);
	packet->transport_destination_port= ntohs(tcp->dest);
	packet->transport_protocol= IPPROTO_TCP;

}


void parse_udp(struct packet *packet){

	struct udphdr	*udp;
	udp = (struct udphdr *)packet->raw_packet;
	packet->udp = udp;

	packet->raw_packet += sizeof(struct udphdr);
	packet->len -= sizeof(struct udphdr);


	packet->transport_source_port= ntohs(udp->source);
	packet->transport_destination_port= ntohs(udp->dest);
	packet->transport_protocol= IPPROTO_UDP;
}


int parse_ipv6(struct packet *packet){
	
	int next_header=0;
	struct ip6_hdr	*ip6;
	struct ip6_ext	*ip6e;

	dbg("Starting the parse of Ipv6 header\n");

	ip6 = (struct ip6_hdr *)packet->raw_packet;
	packet->ip6 = ip6;

	next_header = ip6->ip6_nxt;
	packet->raw_packet += sizeof(struct ip6_hdr);
	packet->len -= sizeof(struct ip6_hdr);

	//Skip ipv6 extension headers and parde UDP and TCP
	while (next_header != -1 ) {

		switch (next_header) {
			case IPPROTO_HOPOPTS:
			case IPPROTO_ROUTING:
			case IPPROTO_FRAGMENT:
			case IPPROTO_DSTOPTS:
			if (packet->len < 8) {
				next_header = -1;
				dbg("Error parsing IPv6 packet. Extension header is too small.\n");
				break; 
			}
			ip6e = (struct ip6_ext *)packet->raw_packet;
			if (packet->len < (ip6e->ip6e_len + 1) * 8) {
				next_header = -1;
				dbg( "Error parsing IPv6 packet. Extension header is too small.\n");
				break;
			}
			packet->raw_packet += (ip6e->ip6e_len + 1) * 8;
			packet->len -= (ip6e->ip6e_len + 1) * 8;
			next_header = ip6e->ip6e_nxt;
			break;
			default:
			return next_header;
			break;
		}
	}

	return next_header;

}



void dump_packet(struct  packet packet){
	int flowLabel= *(&(packet.ip6)->ip6_flow)  & 0x000fffff;
	int traffic_class= *(&(packet.ip6)->ip6_flow)  >> 20;
	
	dbg("==================================\n");
	dbg("Found flow\n");
	dbg("Ipv6 Origin: %x:%x:%x:%x:%x:%x:%x:%x\n", NIP6ADDR(&(packet.ip6)->ip6_src));
	dbg("Ipv6 Destination:%x:%x:%x:%x:%x:%x:%x:%x\n", NIP6ADDR(&(packet.ip6)->ip6_dst));
	dbg("IPv6 Flow label: %d\n",flowLabel);
	dbg("Ipv6 Traffic class: %d\n",traffic_class);
	dbg("Transport protocol: %d\n", packet.transport_protocol);
	dbg("Transport Source Port: %d\n", packet.transport_source_port);
	dbg("Transport Destination Port: %d\n",packet.transport_destination_port);
	dbg("==================================\n");

}


int schedule_packet(struct  packet packet){

	int mark =0;

	uint32_t flowLabel= *(&(packet.ip6)->ip6_flow)  & 0x000fffff;
	uint8_t traffic_class= *(&(packet.ip6)->ip6_flow)  >> 20;
	

	mark= flowScheduler_schedule_flow( (packet.ip6)->ip6_src, (packet.ip6)->ip6_dst,
		packet.transport_protocol,packet.transport_source_port,
		packet.transport_destination_port,traffic_class,flowLabel  );

	if(mark>0){
		add_new_mark((packet.ip6)->ip6_src,(packet.ip6)->ip6_dst, 
			packet.transport_protocol, packet.transport_source_port,
			packet.transport_destination_port, mark);
	}
	
	return mark;
}

