
#include "debug.h"
#include "conf.h"
#include <pthread.h>
#include "fmpmip_packet_marker.h"
#include "util.h"
#include <linux/netfilter.h>		/* for NF_ACCEPT */
#include <netinet/tcp.h>			/* for TCP header*/
#include <netinet/udp.h>			/* for UDP header*/


int flowMarker_add_fwmark(struct in6_addr ip_source,struct in6_addr ip_destination,
	int transport_protocol, 
	int transport_protocol_source_port,int transport_protocol_destination_port, uint32_t mark){
	
	char * transport_protocol_string ="udp";
	int ret=0;

	if(transport_protocol== IPPROTO_TCP){
		transport_protocol_string="tcp";
	}
	
	char buff[350];
	sprintf(buff, "ip6tables -t mangle -C PREROUTING  -s %x:%x:%x:%x:%x:%x:%x:%x -d %x:%x:%x:%x:%x:%x:%x:%x -p %s --dport %d --sport %d -j MARK --set-mark %d ",
		NIP6ADDR(&ip_source),NIP6ADDR(&ip_destination),
		transport_protocol_string, transport_protocol_destination_port,
		transport_protocol_source_port, mark	);

   	//check if rule exists
   	dbg ("(%s) \n",buff);
	ret = system(buff);	
	
	sprintf(buff, "ip6tables  -t mangle -I PREROUTING  1 -s %x:%x:%x:%x:%x:%x:%x:%x -d %x:%x:%x:%x:%x:%x:%x:%x -p %s --dport %d --sport %d -j MARK --set-mark %d ",
		NIP6ADDR(&ip_source),NIP6ADDR(&ip_destination),
		transport_protocol_string, transport_protocol_destination_port,
		transport_protocol_source_port, mark	);
	if(ret != 0){
		ret = system(buff);	
	}

	dbg ("(%s) \n",buff);
	
	dbg("System exited with status %d\n",ret);
	return ret;


}

int flowMarker_delete_mark(struct in6_addr ip_source,struct in6_addr ip_destination,
	int transport_protocol, 
	int transport_protocol_source_port,int transport_protocol_destination_port,uint32_t mark){
	
	char * transport_protocol_string ="udp";
	int ret=0;

	if(transport_protocol== IPPROTO_TCP){
		transport_protocol_string="tcp";
	}
	
	char buff[350];
	sprintf(buff, "ip6tables -t mangle -D PREROUTING  -s %x:%x:%x:%x:%x:%x:%x:%x -d %x:%x:%x:%x:%x:%x:%x:%x -p %s --dport %d --sport %d -j MARK --set-mark %d ",
		NIP6ADDR(&ip_source),NIP6ADDR(&ip_destination),
		transport_protocol_string, transport_protocol_destination_port,
		transport_protocol_source_port, mark );
	
	dbg ("(%s) \n",buff);
	ret = system(buff);	

	dbg("System exited with status %d\n",ret);
	return ret;

}