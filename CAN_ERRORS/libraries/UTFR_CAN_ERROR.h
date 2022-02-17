#include "mcp_can.h"
#include <Arduino.h>

class UTFR_CAN_ERROR : public MCP_CAN {

  public: 
    void init_can(); //Remember to call this before trying to use the CAN_ERROR_LIBRARY

    bool send_can_error(const unsigned char &msg);

  private:
    const int _kERROR_MSG_ID = 0b01011111111 //TODO - verify final seven bits

}


