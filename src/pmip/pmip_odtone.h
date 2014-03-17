


int send_to_mih(char * str, int prim_length);



int registInMIHF(void);

int start_odtone_listener(void);


int process_incoming_message(void);

int send_to_mih(char * str, int prim_length);

int decode_capability_discover_response(char* str);
int send_user_reg_indication(void);
int send_capability_discover_request(void);

int send_event_subscribe_request(void);
