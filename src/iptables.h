
#ifndef __IPTABLES_H__
#define __IPTABLES_H__ 1

#include <netinet/in.h>


#define TABLE_PREROUTING 1;
#define TABLE_INPUT 2;
#define TABLE_FORWARD 3;
#define TABLE_OUTPUT 4;
#define TABLE_POSTROUTING 5;



int iptables_add_mark_rule(int table, char * protocol, struct in6_addr *ip_orig, struct in6_addr *ip_dest,
						 int dest_port, int orig_port, int mark);


int iptables_delete_mark_rule(int table, char * protocol, struct in6_addr *ip_orig, struct in6_addr *ip_dest,
						 int dest_port, int orig_port , int mark);


#endif
