
#ifndef __PMIP_ODTONE_H__
#    define __PMIP_ODTONE_H__
//-----------------------------------------------------------------------------
#    ifdef PMIP_ODTONE_C
#        define private_pmip_odtone(x) x
#        define protected_pmip_odtone(x) x
#        define public_pmip_odtone(x) x
#    else
#        ifdef PMIP
#            define private_pmip_odtone(x)
#            define protected_pmip_odtone(x) extern x
#            define public_pmip_odtone(x) extern x
#        else
#            define private_pmip_odtone(x)
#            define protected_pmip_odtone(x)
#            define public_pmip_odtone(x) extern x
#        endif
#    endif
//-----------------------------------------------------------------------------
#    include <pthread.h>

char g_pcap_devname[32];
int   g_pcap_iif;

protected_pmip_odtone( int start_odtone_listener(void);)


private_pmip_odtone( int start_MIHF_socket(void);)

private_pmip_odtone( int registInMIHF(void);)

private_pmip_odtone( void* process_incoming_message(__attribute__ ((unused)) void *arg);)

private_pmip_odtone( int send_to_mih(char * str, int prim_length);)

private_pmip_odtone( int decode_capability_discover_response(char* str);)
private_pmip_odtone( int send_user_reg_indication(void);)
private_pmip_odtone( int send_capability_discover_request(void);)

private_pmip_odtone( int send_event_subscribe_request(void);)


#endif