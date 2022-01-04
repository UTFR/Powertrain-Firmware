===================================================================================================
What are these files?
===================================================================================================

 --> can.h, mcp2515.h, and mcp2515.cpp are all from https://github.com/autowp/arduino-mcp2515
  	- This is driver code for the mcp2515 CAN controller chip.

 --> mcp2515_c_connector.h and mcp2515_c_connector.cpp expose the functions in the MCP2515 class
     in pure C so the driver code can extended without C++. 
	- Simply call the functions in these files from any C file or directly from your Arduino code.


====================================================================================================
Function Signature Convention and Type Conventions (subject to change)
====================================================================================================

 --> return_type mcp2515_c_functionName(void *device, other_params){ ... }
 	- NOTE: return_type and other_params must be C compatible types/structs
	
 --> typedef enum {
           ENUMNAME_MEMBERDESCRIPTION0_C = 0,
	   ENUMNAME_MEMBERDESCRIPTION1_C = 1,
     } ENUMNAME_C;
