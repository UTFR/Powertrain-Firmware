#define HW_PIN_FLOW_RATE_INVERTER A3
#define HW_PIN_CHASSIS_DSC 39

void setup() {
  
  Serial.begin(9600);
  
  pinMode(HW_PIN_CHASSIS_DSC, OUTPUT);
  digitalWrite(HW_PIN_CHASSIS_DSC, HIGH);     // connect BMS ground to chassis ground

  pinMode(HW_PIN_FLOW_RATE_INVERTER, INPUT);
  
}

void loop() {
  
  delay(10);
  Serial.println(analogRead(HW_PIN_FLOW_RATE_INVERTER));
  
}
