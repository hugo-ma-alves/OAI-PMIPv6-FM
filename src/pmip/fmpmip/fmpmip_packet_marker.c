
#include "debug.h"
#include "conf.h"
#include <pthread.h>
#include "fmpmip_packet_marker.h"
#include "util.h"
//#   include "flowmob_hash_struct.h"

//#include "iptables.h"

int parse_packet(unsigned char *packet , int len);
int process_queue_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,struct nfq_data *nfa, void *data);
void parse_udp(struct packet *packet);
void parse_tcp(struct packet *packet);
void parse_ipv6(struct packet *packet);
int loop_queue(uint16_t userSpace_queue_id);
void save_to_flowmob_cache(struct packet packet);

struct nfq_handle *h;
struct nfq_q_handle *qh;

pthread_t iptables_queue_listener;


int fmpmip_flow_handler_init(uint16_t userSpace_queue_id){

	int res;

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

	dbg("binding this socket to queue '0'\n");
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


int loop_queue(uint16_t userSpace_queue_id)
{
	
	int fd;
	int rv;
	char buf[4096] __attribute__ ((aligned));

	fd = nfq_fd(h);

	while ((rv = recv(fd, buf, sizeof(buf), 0)) && rv >= 0) {
		nfq_handle_packet(h, buf, rv);
	}

	dbg("unbinding from queue %d\n",userSpace_queue_id);
	nfq_destroy_queue(qh);

	dbg("closing library handle\n");
	nfq_close(h);

	return 0;
}

int process_queue_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
	struct nfq_data *nfa, void *data)
{
	int len=0;
	int id;
	struct nfqnl_msg_packet_hdr *ph;

	dbg("Processing queue packet\n");

	len = nfq_get_payload(nfa, &data);
	ph = nfq_get_msg_packet_hdr(nfa);

	if (ph) {
		id = ntohl(ph->packet_id);
		parse_packet(data, len);
	}

	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

int parse_packet(unsigned char *data, int len){

	unsigned char ipversion = data[0];
	struct packet packet;
	memset(&packet,0,sizeof(struct packet));

	packet.len=len;
	packet.raw_packet= data;

	ipversion = ipversion >> 4;

	if(ipversion  == 0x45){
		dbg("IPV4 package found - Ignoring\n");
		return 1;
	}

	parse_ipv6(&packet);
	if(packet.transport_protocol==0){
		dbg("Analyzed a network packet but not found a valid transport protocol. Ignoring this packet.\n");
		return 1;
	}

	dump_packet(packet);
	
	save_to_flowmob_cache(packet);
	
	return 0;

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


void parse_ipv6(struct packet *packet){
	
	int next_header=0;
	struct ip6_hdr	*ip6;
	struct ip6_ext	*ip6e;

	dbg("Start the parse of Ipv6 header\n");

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
			case IPPROTO_UDP:
			parse_udp(packet);
			dbg ("Found UDP transport layer\n");
			next_header=-1;
			break;
			case IPPROTO_TCP:
			parse_tcp(packet);
			dbg ("Found TCP transport layer\n");
			next_header=-1;
			break;
			case IPPROTO_NONE:
			dbg ("No more Headers - Ignoring this packet\n");
			next_header=-1;
			break;
			default:
			next_header=-1;
			dbg ("Unknow Next header value found - Ignoring this packet\n");
			break;
		}
	}

}


void dump_packet(struct  packet packet){

	dbg("Flow parameters\n");
	dbg("Ipv6 Origin: %x:%x:%x:%x:%x:%x:%x:%x\n", NIP6ADDR(&(packet.ip6)->ip6_src));
	dbg("Ipv6 Destination:%x:%x:%x:%x:%x:%x:%x:%x\n", NIP6ADDR(&(packet.ip6)->ip6_dst));
	dbg("Transport protocol: %d\n", packet.transport_protocol);
	dbg("Transport Source Port: %d\n", packet.transport_source_port);
	dbg("Transport Destination Port: %d\n",packet.transport_destination_port);
	//iptables_add_rule(1,"NULL",&(packet.ip6)->ip6_src,&(packet.ip6)->ip6_dst,packet.transport_destination_port,packet.transport_source_port);
}

void save_to_flowmob_cache(struct packet packet){

	fmpmip_flow_cache_entry * flowMob= flowmob_cache_alloc();
	memcpy(&flowMob->ip6_origin , &(packet.ip6)->ip6_src , sizeof(struct in6_addr));
	memcpy(&flowMob->ip6_destination , &(packet.ip6)->ip6_dst, sizeof(struct in6_addr));
	flowMob->transport_protocol = packet.transport_protocol;
	flowMob->transport_source_port = packet.transport_source_port;
	flowMob->transport_destination_port = packet.transport_destination_port;

	if(flowmob_cache_exists(flowMob)>0){
		dbg("This flow is already in the flow cache table\n");
		free(flowMob);
	}else{
		flowmob_cache_add(flowMob);
		dbg("This is a new flow. A new entry in the flow cache table was created.\n");
	}

}