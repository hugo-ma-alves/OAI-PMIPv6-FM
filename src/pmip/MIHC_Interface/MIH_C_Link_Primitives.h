/**************************************************************************
                          MIH_C_Link_Primitives.h  -  description
                             -------------------
    copyright            : (C) 2012 by Eurecom
    email                : davide.brizzolara@eurecom.fr; michelle.wetterwald@eurecom.fr
***************************************************************************
Type definition and structure for ODTONE interface
***************************************************************************/

#include <sys/types.h>
#include "MIH_C_Link_Constants.h"
#include "MIH_C_Variables.h"

typedef u_int8_t MIH_C_Header_Vers;
typedef u_int8_t MIH_C_Header_FN;
typedef u_int16_t MIH_C_Message_ID;
typedef u_int16_t MIH_C_Transaction_ID;
typedef u_int16_t MIH_C_Variable_Payload_Length;

typedef u_int8_t MIH_C_Status_Value;

typedef u_int8_t MIH_C_Type;
typedef u_int8_t MIH_C_Length;

typedef u_int8_t MIH_C_VariablePayload;
typedef u_int16_t MIH_C_Address_Family;
typedef u_int8_t MIH_C_Length_Address;
typedef u_int8_t MIH_C_Link_Type;


// Struct Message Header
typedef struct{
	MIH_C_Header_Vers Header_Vers;
	MIH_C_Header_FN Header_FN; //FRagment number
	MIH_C_Message_ID Message_ID;
	MIH_C_Transaction_ID Transaction_ID;
	MIH_C_Variable_Payload_Length Payload_Length;
}__attribute__((__packed__))MIH_C_Header;


typedef struct{
	u_int8_t Length; // the lenght of the user name string
	char Name[MIHF_MAX_LINK_ID_NAME_USER];
}__attribute__((__packed__))MIH_C_Link_ID_Name_User;

typedef struct{
	 u_int8_t Length;
	 char Link_ID_Char[MIHF_MAX_LINK_ID_NAME_MIHF];
}__attribute__((__packed__))MIH_C_Link_ID_Name_Mihf;

typedef struct{
	 u_int8_t Evt_List_Oct[MIHF_EVT_LIST_LENGTH];
}__attribute__((__packed__))MIH_C_Evt_List_Data;

typedef struct{
	 u_int8_t Cmd_List_Oct[MIHF_CMD_LIST_LENGTH];
}__attribute__((__packed__))MIH_C_Cmd_List_Data;

typedef struct{
	 u_int8_t Cmd_List_Oct[MIHF_ADDR_LIST_LENGTH];
}__attribute__((__packed__))MIH_C_Addr_List_Data;

typedef struct{
	u_int8_t Length; // the lenght of the next field, because this is a list
	 u_int8_t MAC_Addr_Val[MIH_MAC_ADDRESS_LENGTH];
}__attribute__((__packed__))MIH_C_MAC_Address;

/******************************
*  TLV List
******************************/

// MIHF - TLV source 1 - TLV destination 2
typedef struct{
	MIH_C_Type  Type;
	MIH_C_Length Length;
	MIH_C_Link_ID_Name_Mihf Link_ID_Name_Mihf;
}__attribute__((__packed__))MIH_C_Mihf_ID_Mihf;

// User - TLV source 1 - TLV destination 2
typedef struct{
    MIH_C_Type  Type;
	MIH_C_Length Length;
	MIH_C_Link_ID_Name_User User_Name;
}__attribute__((__packed__))MIH_C_Mihf_ID_User;


typedef struct {
	MIH_C_Type Type;
	MIH_C_Length Length;
	MIH_C_Evt_List_Data Evt_List_Data;
}__attribute__((__packed__))MIH_C_MIHF_Evt_List;

typedef struct {
	MIH_C_Type Type;
	MIH_C_Length Length;
	MIH_C_Cmd_List_Data Cmd_List_Data;
}__attribute__((__packed__))MIH_C_MIHF_Cmd_List;

typedef struct {
	MIH_C_Type Type;
	MIH_C_Length Length;
	MIH_C_Addr_List_Data Evt_List_Data;
}__attribute__((__packed__))MIH_C_MIHF_Addr_List;

typedef struct{
	MIH_C_Type Type;
    MIH_C_Length Length;
	MIH_C_Link_Type LinkType;
	MIH_C_Address_Family AddressFamily;
	MIH_C_Length_Address LengthAddress;
	MIH_C_MAC_Address MAC_Address;
    u_int8_t            Padding; //I don't know why, but it works, read the 802.21
}__attribute__((__packed__))MIH_C_Link_ID;

// STATUS - 3
typedef struct {
	MIH_C_Type Type;
	MIH_C_Length Length;
	MIH_C_Status_Value Status_Value;
}__attribute__((__packed__))MIH_C_Status;

/*****************************************************
 *  MIH_C_User_Register_Request (SID=1 Op=3 AID=7)
*****************************************************/

typedef struct MIH_C_User_Register_Request{
	MIH_C_Header Header;
	MIH_C_Mihf_ID_User User_Source;
	MIH_C_Mihf_ID_Mihf MIHF_Destination;
}__attribute__((__packed__))MIH_C_User_Register_Request;


/**************************************************
 *  Capability_Discover.request (SID=1 Op=1 AID=1)
*************************************************/

typedef struct MIH_C_Capability_Discover_Request{
      MIH_C_Header Header;
      MIH_C_Mihf_ID_User User_Source;
      MIH_C_Mihf_ID_Mihf MIHF_Destination;
}__attribute__((__packed__))MIH_C_Capability_Discover_Request;

/**************************************************
 *  Capability_Discover.confirm (SID=1 Op=2 AID=1)
*************************************************/
typedef struct MIH_C_Capability_Discover_Conf{
      MIH_C_Header Header;
      MIH_C_Mihf_ID_Mihf MIHF_Source;
      MIH_C_Mihf_ID_User User_Destination;
      MIH_C_Status Status;
      MIH_C_MIHF_Addr_List MIHF_addr_List;
      MIH_C_MIHF_Evt_List MIHF_Event_List;
      MIH_C_MIHF_Cmd_List MIHF_Command_List;
        //There are some fields missing, check 802.21 document
}__attribute__((__packed__))MIH_C_Capability_Discover_Conf;

/**************************************************
 *  Event_Subscribe.request (SID=1 Op=1 AID=4)
*************************************************/
typedef struct MIH_C_Event_Subscribe_Request{
      MIH_C_Header Header;
      MIH_C_Mihf_ID_User  User_Source;
      MIH_C_Mihf_ID_Mihf  MIHF_Destination;
      MIH_C_Link_ID       MIHF_Link_ID;
      MIH_C_MIHF_Evt_List MIHF_Event_List;
}__attribute__((__packed__))MIH_C_Event_Subscribe_Request;



/************* support for reception *************/
typedef struct MIH_C_ODTONE_Message{
	MIH_C_Header Header;
	MIH_C_Mihf_ID_Mihf MIHF_Source;
	MIH_C_Mihf_ID_User User_Destination;
	MIH_C_VariablePayload variablePayload[MIH_C_MESSAGE_MAX_LENGTH];
}__attribute__((__packed__))MIH_C_ODTONE_Message;
/************************************************/
