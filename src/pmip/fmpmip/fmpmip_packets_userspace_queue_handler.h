#ifndef __FMPMIP_PACKETS_USERSPACE_QUEUE_HANDLER_H__
#    define __FMPMIP_PACKETS_USERSPACE_QUEUE_HANDLER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>		/* for NF_ACCEPT */
#include <netinet/tcp.h>			/* for TCP header*/
#include <netinet/udp.h>			/* for UDP header*/
#include <netinet/ip6.h>			/* for IPv6 header*/

#include <libnetfilter_queue/libnetfilter_queue.h>

struct packet {
	
	int len;
	unsigned char * raw_packet; 
	struct ip6_hdr *ip6; /* Pointer to first byte of IP Header in the raw_packet*/
	/*Insert extensions headers here, and set the pointers in the parse function;
	struct nd_neighbor_solicit *ns;
	struct nd_neighbor_advert *na;
	struct nd_opt_hdr *opt_slla;
	struct nd_opt_hdr *opt_tlla;
	*/
	
	int transport_protocol;
	
	struct tcphdr *tcp; /* Pointer to first byte of TCP Header in the raw_packet*/
	struct udphdr *udp; /* Pointer to first byte of UDP Header in the raw_packet*/

	uint16_t transport_source_port; //in host byte order
	uint16_t transport_destination_port; //in host byte order

};


int fmpmip_flow_handler_init(uint16_t userSpace_queue_id);

int parse_packet( char *raw_packet , int len, struct packet *packet);
static int process_queue_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,struct nfq_data *nfa, void *data);
void parse_udp(struct packet *packet);
void parse_tcp(struct packet *packet);
int parse_ipv6(struct packet *packet);
static void *loop_queue(__attribute__ ((unused)) void *arg);

void dump_packet(struct packet packet);
int schedule_packet(struct  packet packet);


#endif