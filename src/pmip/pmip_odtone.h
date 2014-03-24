


char g_pcap_devname[32];

int   g_pcap_iif;




int start_odtone_listener(void);

int start_MIHF_socket(void);


int send_to_mih(char * str, int prim_length);



int registInMIHF(void);



int process_incoming_message(void);

int send_to_mih(char * str, int prim_length);

int decode_capability_discover_response(char* str);
int send_user_reg_indication(void);
int send_capability_discover_request(void);

int send_event_subscribe_request(void);
