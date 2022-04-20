#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(10); // init controller

struct can_frame can1; 
struct can_frame can2;
struct can_frame can3; // 200ms
struct can_frame can4;

int i = 0;

void setup() {
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();

  can1.can_id = 0x80;
  can1.can_dlc = 0x4;
  can1.data[0] = 0x5;
  can1.data[1] = 0x15;
  can1.data[2] = 0x0;
  can1.data[3] = 0x9E;

  can2.can_id = 0x9838F380;
  can2.can_dlc = 8;
  can2.data[0] = 0x0; 
  can2.data[1] = 0xE;
  can2.data[2] = 0x15; 
  can2.data[3] = 0xE; 
  can2.data[4] = 0x15; 
  can2.data[5] = 0x15; 
  can2.data[6] = 0xE; 
  can2.data[7] = 0xE;

  can3.can_id = 0x98EEFF80;
  can3.can_dlc = 8;
  can3.data[0] = 0xF3; 
  can3.data[1] = 0x0;
  can3.data[2] = 0x80; 
  can3.data[3] = 0xF3; 
  can3.data[4] = 0x0; 
  can3.data[5] = 0x40; 
  can3.data[6] = 0x1E; 
  can3.data[7] = 0x90;

  can4.can_id = 0x9839F380;
  can4.can_dlc = 8;
  can4.data[0] = 0x0; 
  can4.data[1] = 0x15;
  can4.data[2] = 0x15; 
  can4.data[3] = 0x15; 
  can4.data[4] = 0x1; 
  can4.data[5] = 0xE; 
  can4.data[6] = 0xE; 
  can4.data[7] = 0x9D;

}

void loop() {
  i++;
  if (i % 2 == 0){
      mcp2515.sendMessage(&can3);
  }
  mcp2515.sendMessage(&can1);
  mcp2515.sendMessage(&can2);
  mcp2515.sendMessage(&can4);
  delay(100);
}
