

#include "iptables.h"
#include <stdlib.h>
#include "debug.h"



int iptables_add_mark_rule(int table, char * protocol, struct in6_addr *ip_orig, struct in6_addr *ip_dest, 
						int dest_port, int orig_port, int mark){

	dbg("IPTA");
	//system("ls -l");
	return 1;
}


int iptables_delete_mark_rule(int table, char * protocol, struct in6_addr *ip_orig, struct in6_addr *ip_dest, 
						int dest_port, int orig_port, int mark){

	return 1;

}
