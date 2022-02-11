#include <SPI.h>
#include <mcp2515.h>

struct can_frame BMS_RX;
struct can_frame ACM_TX;

MCP2515 ACM_controller(10);

//void readMessage();

void setup() {
  Serial.begin(115200);
  pinMode(10, OUTPUT);
  //digitalWrite(10, HIGH);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), BMStoACM, LOW);
  
  ACM_controller.reset();
  ACM_controller.setBitrate(CAN_500KBPS);
  ACM_controller.setNormalMode();

  ACM_TX.can_id = 0x0A1;
  ACM_TX.can_dlc = 8;
  ACM_TX.data[0] = 0xA2;
  ACM_TX.data[1] = 0xA2;
  ACM_TX.data[2] = 0xA2;
  ACM_TX.data[3] = 0xA2;
  ACM_TX.data[4] = 0xA2;
  ACM_TX.data[5] = 0xA2;
  ACM_TX.data[6] = 0xA2;
  ACM_TX.data[7] = 0xA2;

  //pinMode(9, OUTPUT);
  
  
  
  
}

void loop() {
  ACM_controller.sendMessage(&ACM_TX);
  delay(10);
  //delay(1000);
}

void BMStoACM(){

  if (ACM_controller.readMessage(&BMS_RX) == MCP2515::ERROR_OK) {
    
    Serial.print("DLC:");
    Serial.print(BMS_RX.can_dlc);
    for (int i = 0; i<BMS_RX.can_dlc; i++)  {  // print the data
      
      Serial.print(" ");
      Serial.print(BMS_RX.data[i],HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }
  else
  {
    Serial.println("ERROR: interrupt without message");
  }
}

//void TEMtoBMS(){
//  //Serial.println("In TEMToBMS");
//  if (TEM_controller.readMessage(&TEM_RX) == MCP2515::ERROR_OK) {
//    ACM_controller.sendMessage(&TEM_RX);
//
//  }
//}


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
