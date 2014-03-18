#define PMIP
#define PMIP_PCAP_C
#ifdef HAVE_CONFIG_H
#       include <config.h>
#endif
#include <string.h>
#include <ctype.h>
//---------------------------------------------------------------------------------------------------------------------
#include "pmip_fsm.h"
#include "pmip_hnp_cache.h"
#include "pmip_pcap.h"
#include "pmip_msgs.h"
#include "pmip_mag_proc.h"
//---------------------------------------------------------------------------------------------------------------------
#ifdef ENABLE_VT
#    include "vt.h"
#endif

#include "debug.h"
#include "conf.h"


#include "MIHC_Interface/MIH_C_Link_Primitives.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


static pthread_t socket_listener;
int  sock;
struct sockaddr_in mihf_socket;

socklen_t sockaddr_len;

u_int16_t transaction_id;


//---------------------------------------------------------------------------
int start_MIHF_socket(void)
{
    //---------------------------------------------------------------------------

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    dbg ("\n###########Opening UDP socket to MIHF ###############\n");

    mihf_socket.sin_family = AF_INET;
    mihf_socket.sin_port = htons(conf.MIHFPort);
    mihf_socket.sin_addr.s_addr = conf.MIHFIPAddress.s_addr;

    bzero(&(mihf_socket.sin_zero),8);

    sockaddr_len = sizeof(struct sockaddr_in);

    return sock;
}

int send_to_mih(char * str, int prim_length)
{
    //---------------------------------------------------------------------------
    int rc;

    rc =sendto(sock, str,prim_length, 0, (struct sockaddr *)&mihf_socket, sockaddr_len);

    return rc;
}



/**
*Check if the link mac address present in the config file is present in the avaialable links list received
*/
int decodeLinkList(char*str, int index)
{

    unsigned char  mac[6];

    int addrLength=0;
    int numberOfElemets=0;
    index++; //list lenght
    addrLength=str[index];  //size of payload
    ++index;
    numberOfElemets=str[index]; //number of links in list

    do
    {
        index+=9; //advance to first link address
        mac[0]=str[index];
        dbg("Found Link sap number %d:\n",numberOfElemets);
        memcpy(mac, &str[index],6);

        debug_print_buffer(mac ,6, "decodeLinkList"," Received Link:");
        index = index+5;

        if(strcmp(mac, conf.LinkMacAddress)==0)
        {
            return 1;
        }
        numberOfElemets--;
    }
    while(numberOfElemets>0);

    return 0;

}


int decode_capability_discover_response(char* str)
{
    int i=0;
    uint16_t payload_length=0;

    dbg("Decoding capability discover response\n");
    dbg("The package HEX DUMP, for debugging purposes:\n");
    debug_print_buffer(str,MIHLink_MAX_LENGTH,"decode_capability_discover_response","Buffer received from MIHF");

    payload_length=  str[6]<<8 | str[7]; //Advance to header payload length
    i=sizeof(MIH_C_Header);// start in first TLV

    dbg("Payload length size %d \n",payload_length);
    dbg("Header size %d \n",i);

    while(i<payload_length)
    {

        if(str[i] != TLV_ADDR_LIST)
        {
            dbg("Discarding TLV FIELD of type %d \n", str[i]);
            //It's not the interfaces list
            i++; //Lenght
            i+= str[i]; //advance length
            i++; //advance to next TLV Type
        }
        else
        {
            dbg("Found link address list\n");
            return decodeLinkList(str,i);
            break;
        }

    }

    dbg("Error parsing capability discover responde - Link address list not found");
    return -1;
}



//-----------------------------------------------------------------------------
// Process incoming messages from MIHF
int process_incoming_message(void)
{
    //-----------------------------------------------------------------------------

    dbg("Starting incoming MIHF messages listener....\n");

    int n=0;
    char str[MIHLink_MAX_LENGTH];

    while(1)
    {

        MIH_C_Header *ODTONE_Header;

        bzero(str,MIHLink_MAX_LENGTH);


        n = recvfrom(sock, (void *)str, MIHLink_MAX_LENGTH, 0, (struct sockaddr *) &mihf_socket, &sockaddr_len);

        ODTONE_Header = (MIH_C_Header *) str;
        dbg("Received message from MIHF\n");
        dbg("It's ID is %d \n",ODTONE_Header->Message_ID);

        switch(ODTONE_Header->Message_ID)
        {
        case MIH_LINK_HEADER_3_Cap_Disc_Conf:
            if(decode_capability_discover_response(str))
            {
                send_event_subscribe_request();
            }
            break;

        case MIH_LINK_HEADER_3_Evt_Sub_Conf:
            dbg("Received Event subscribe response, but it isn't parsed\n");
            break;
        default:
            dbg("Unknow type of message received from MIHF\n");
            break;
        }

    }

    return n;
}



//---------------------------------------------------------------------------
int send_user_reg_indication(void)
{
    //---------------------------------------------------------------------------

    int res=0;
    char str[MIHLink_MAX_LENGTH];
    int prim_length;

    transaction_id  = rand();

    MIH_C_User_Register_Request *msgToBuild;

    bzero(str,MIHLink_MAX_LENGTH);
    prim_length =  sizeof(MIH_C_User_Register_Request);
    msgToBuild = (MIH_C_User_Register_Request *) str;

    msgToBuild->Header.Header_Vers = PROTOCOL_VERSION;
    msgToBuild->Header.Header_FN = MIH_LINK_HEADER_2;
    msgToBuild->Header.Message_ID = MIH_User_Reg_Req;
    //msgToBuild->Header.Message_ID = 5122;
    msgToBuild->Header.Transaction_ID = transaction_id;
    msgToBuild->Header.Payload_Length = (sizeof(MIH_C_User_Register_Request)-sizeof(MIH_C_Header)) ;

    // TLV  Source
    msgToBuild->User_Source.Type = TLV_MIHF_ID_SOURCE;
    msgToBuild->User_Source.Length = sizeof(MIH_C_Link_ID_Name_User);
    msgToBuild->User_Source.User_Name.Length= sizeof(MIH_C_Link_ID_Name_User)-1;
    strcpy(msgToBuild->User_Source.User_Name.Name, conf.MIHFClientUserName);

    // TLV Destination
    msgToBuild->MIHF_Destination.Type = TLV_MIHF_ID_DESTINATION;
    msgToBuild->MIHF_Destination.Length = sizeof(MIH_C_Link_ID_Name_Mihf);
    msgToBuild->MIHF_Destination.Link_ID_Name_Mihf.Length= sizeof(MIH_C_Link_ID_Name_Mihf)-1;
    strcpy(msgToBuild->MIHF_Destination.Link_ID_Name_Mihf.Link_ID_Char, conf.MIHF_ID);

    unsigned char command[6]  = {0x06,0x04,0x00,0x00,0x00,0x18};
    memcpy(msgToBuild->teste,command,6);

    res=send_to_mih(str,prim_length);
    if (res<0)
    {
        dbg("Error sending User registration to MIHF\n");
    }

    return res;
}


//---------------------------------------------------------------------------
int send_capability_discover_request(void)
{
    //---------------------------------------------------------------------------

    int res=0;
    char str[MIHLink_MAX_LENGTH];
    int prim_length;

    transaction_id  = rand();

    MIH_C_Capability_Discover_Request *msgToBuild;

    bzero(str,MIHLink_MAX_LENGTH);
    prim_length =  sizeof(MIH_C_Capability_Discover_Request);
    msgToBuild = (MIH_C_Capability_Discover_Request *) str;

    msgToBuild->Header.Header_Vers = PROTOCOL_VERSION;
    msgToBuild->Header.Header_FN = MIH_LINK_HEADER_2;
    msgToBuild->Header.Message_ID = MIH_LINK_HEADER_3_Cap_Disc_Req;
    msgToBuild->Header.Transaction_ID = transaction_id;
    msgToBuild->Header.Payload_Length = (sizeof(MIH_C_Capability_Discover_Request)-sizeof(MIH_C_Header)) ;

    // TLV  Source
    msgToBuild->User_Source.Type = TLV_MIHF_ID_SOURCE;
    msgToBuild->User_Source.Length = sizeof(MIH_C_Link_ID_Name_User);
    msgToBuild->User_Source.User_Name.Length= sizeof(MIH_C_Link_ID_Name_User)-1;
    strcpy(msgToBuild->User_Source.User_Name.Name, conf.MIHFClientUserName);

    // TLV Destination
    msgToBuild->MIHF_Destination.Type = TLV_MIHF_ID_DESTINATION;
    msgToBuild->MIHF_Destination.Length = sizeof(MIH_C_Link_ID_Name_Mihf);
    msgToBuild->MIHF_Destination.Link_ID_Name_Mihf.Length= sizeof(MIH_C_Link_ID_Name_Mihf)-1;
    strcpy(msgToBuild->MIHF_Destination.Link_ID_Name_Mihf.Link_ID_Char, conf.MIHF_ID);

    res=send_to_mih(str,prim_length);
    if (res<0)
    {
        dbg("Error sending Capability discover request to MIHF");
    }

    return res;
}


//---------------------------------------------------------------------------
int send_event_subscribe_request(void)
{
    //---------------------------------------------------------------------------

    int res=0;
    int i=0;
    char str[MIHLink_MAX_LENGTH];
    int prim_length;
    u_int8_t evt_list=0;

    transaction_id  = rand();

    MIH_C_Event_Subscribe_Request *msgToBuild;

    bzero(str,MIHLink_MAX_LENGTH);
    prim_length =  sizeof(MIH_C_Event_Subscribe_Request);
    msgToBuild = (MIH_C_Event_Subscribe_Request *) str;

    msgToBuild->Header.Header_Vers = PROTOCOL_VERSION;
    msgToBuild->Header.Header_FN = MIH_LINK_HEADER_2;
    msgToBuild->Header.Message_ID = MIH_LINK_HEADER_3_Evt_Sub_Req;
    msgToBuild->Header.Transaction_ID = transaction_id;
    msgToBuild->Header.Payload_Length = (sizeof(MIH_C_Event_Subscribe_Request)-sizeof(MIH_C_Header)) ;

    // TLV  Source
    msgToBuild->User_Source.Type = TLV_MIHF_ID_SOURCE;
    msgToBuild->User_Source.Length = sizeof(MIH_C_Link_ID_Name_User);
    msgToBuild->User_Source.User_Name.Length= sizeof(MIH_C_Link_ID_Name_User)-1;
    strcpy(msgToBuild->User_Source.User_Name.Name, conf.MIHFClientUserName);

    // TLV Destination
    msgToBuild->MIHF_Destination.Type = TLV_MIHF_ID_DESTINATION;
    msgToBuild->MIHF_Destination.Length = sizeof(MIH_C_Link_ID_Name_Mihf);
    msgToBuild->MIHF_Destination.Link_ID_Name_Mihf.Length= sizeof(MIH_C_Link_ID_Name_Mihf)-1;
    strcpy(msgToBuild->MIHF_Destination.Link_ID_Name_Mihf.Link_ID_Char, conf.MIHF_ID);


    // LINK identification
    u_int8_t mac_address[MIH_MAC_ADDRESS_LENGTH];
    memcpy(mac_address,conf.LinkMacAddress,MIH_MAC_ADDRESS_LENGTH);

    msgToBuild->MIHF_Link_ID.Type = TLV_LINK_TUPLE_ID;
    msgToBuild->MIHF_Link_ID.Length = sizeof(MIH_C_Link_ID)-2;
    //msgToBuild->MIHF_Link_ID.LinkType = 0x0f;
     //careful with this value, The correct is to extract it from the Re discovery message
     //Big problems can happen if the link type is not correct!!!LINK_TYPE_802_11
    //msgToBuild->MIHF_Link_ID.LinkType = 0x13;
    msgToBuild->MIHF_Link_ID.LinkType = LINK_TYPE_802_11;
    msgToBuild->MIHF_Link_ID.AddressFamily = 0x00;
    msgToBuild->MIHF_Link_ID.LengthAddress = 0x0006;
    msgToBuild->MIHF_Link_ID.MAC_Address.Length=0x06;
    memcpy( msgToBuild->MIHF_Link_ID.MAC_Address.MAC_Addr_Val, mac_address, sizeof(mac_address));
    msgToBuild->MIHF_Link_ID.Padding=0x00;


    //EVENTS LIST

           // load the event for Event Subscribe Confirm
    // each bit corresponding to an event is set to 1
    evt_list = evt_list | EVT_LIST_LINK_DETECTED;
    evt_list = evt_list | EVT_LIST_LINK_UP;
    evt_list = evt_list | EVT_LIST_LINK_DOWN;
   // evt_list = evt_list | EVT_LIST_LINK_GOING_DOWN;
   // evt_list = evt_list | EVT_LIST_LINK_PARAMETERS_REPORT;

    u_int8_t event_list[MIHF_EVT_LIST_LENGTH]= {0x00,0x00,0x00,0x7f};

    msgToBuild->MIHF_Event_List.Type = TLV_EVENT_LIST;
    msgToBuild->MIHF_Event_List.Length = sizeof(MIH_C_MIHF_Evt_List)-2;
   memcpy( msgToBuild->MIHF_Event_List.Evt_List_Data.Evt_List_Oct, event_list, sizeof(event_list));

    //msgToBuild->MIHF_Event_List.Evt_List_Data.Evt_List_Oct[3] = evt_list;
  // for(i=0;i<3;i++){msgToBuild->MIHF_Event_List.Evt_List_Data.Evt_List_Oct[i] = VALUE_NULL;}


    res=send_to_mih(str,prim_length);
    if (res<0)
    {
        dbg("Error sending Event subscribe request to MIHF");
        return res;
    }

    dbg("Sent Event subscribe request towards MIHF\n");
    return res;
}


int start_odtone_listener(void)
{

    int res;
    srand(time(NULL));
    transaction_id  = rand();

    res=start_MIHF_socket();

    if(res <0)
    {
        dbg("Error opening socket to MIHF");
        return res;
    }

    res =pthread_create(&socket_listener, NULL, process_incoming_message, NULL);

    if(res<0)
    {
        dbg ("Error starting thread that listens that waits for MIHF messages\n");
        return res;
    }

    res=send_user_reg_indication();

    if(res<0)
    {
        dbg ("error sending user registration to MIHF\n");
        return res;
    }

    transaction_id  = rand();
    send_capability_discover_request();
    return res;

}


