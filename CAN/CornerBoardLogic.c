#include "mcp2515.h"
#include "mcp2515_c_connector.h"


// ####################################################################################
// THIS IS GARBAGE AT THE MOMENT, WILL IMPLEMENT VERSION USING HARDWARE TIMERS ASAP
// ####################################################################################

int period0 = 69;
int period1 = 69;
int period2 = 69;

void sensors_send(){
    int tick = get_time();
    sensor_0_send(device, tick);
    sensor_1_send(device, tick);
    sensor_2_send(device, tick);
}
 
int sensor_0_send(mcp2515_c device, int tick){

    int tock = get_time();  // hardware timer instead
    int elapsed = tock - tick;

    if (elapsed > period0){
        int sent = mcp2515_c_sendMessage(mcp2515_c, data0);
        while (sent == ERROR_ALLTXBUSY){
            sent = mcp2515_c_sendMessage(mcp2515_c, data0);
        }
    }
}

int sensor_1_send(mcp2515_c device, int tick){

    int tock = get_time();
    int elapsed = tock - tick;

    if (elapsed > period1){
        int sent = mcp2515_c_sendMessage(mcp2515_c, data1);
        while (sent == ERROR_ALLTXBUSY){
            sent = mcp2515_c_sendMessage(mcp2515_c, data1);
        }
    }
}

int sensor_2_send(mcp2515_c device, int tick){

    int tock = get_time();
    int elapsed = tock - tick;

    if (elapsed > period2){
        int sent = mcp2515_c_sendMessage(mcp2515_c, data2);
        while (sent == ERROR_ALLTXBUSY){
            sent = mcp2515_c_sendMessage(mcp2515_c, data2);
        }
    }
}