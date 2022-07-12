#include <SPI.h>
#include <mcp2515.h>

struct can_frame BMS_RX;
struct can_frame TEM_RX;
struct can_frame canMsg1;

MCP2515 BMS_controller(10);
MCP2515 TEM_controller(9);

void readMessage();

void setup() {
  Serial.begin(115200);
  
  BMS_controller.reset();
  BMS_controller.setBitrate(CAN_500KBPS);
  BMS_controller.setNormalMode();

  pinMode(9, OUTPUT);
  TEM_controller.reset();
  TEM_controller.setBitrate(CAN_500KBPS);
  TEM_controller.setNormalMode();
  
  attachInterrupt(digitalPinToInterrupt(2), BMStoTEM, LOW);
  attachInterrupt(digitalPinToInterrupt(3), TEMtoBMS, LOW);
  
}

void loop() {
  
  
}

void BMStoTEM(){

  if (BMS_controller.readMessage(&BMS_RX) == MCP2515::ERROR_OK) {
    
    TEM_controller.sendMessage(&BMS_RX);
  }
  else
  {
    Serial.println("ERROR: interrupt without message");
  }
}

void TEMtoBMS(){
  //Serial.println("In TEMToBMS");
  if (TEM_controller.readMessage(&TEM_RX) == MCP2515::ERROR_OK) {
    BMS_controller.sendMessage(&TEM_RX);

  }
}


/*
    Serial.print("DLC:");
    Serial.print(BMS_RX.can_dlc);
    for (int i = 0; i<BMS_RX.can_dlc; i++)  {  // print the data
      
      Serial.print(" ");
      Serial.print(BMS_RX.data[i],HEX);
      Serial.print(" ");
    }
    Serial.println("");
*/
