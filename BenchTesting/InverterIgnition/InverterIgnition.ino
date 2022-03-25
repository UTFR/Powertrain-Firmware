#define HW_PIN_INVERTER_IGNITION 38
#define HW_PIN_CHASSIS_GND_DSC 39

void setup() {
  Serial.begin(9600);
  pinMode(HW_PIN_CHASSIS_GND_DSC, OUTPUT);
  digitalWrite(HW_PIN_CHASSIS_GND_DSC, HIGH);
  pinMode(HW_PIN_INVERTER_IGNITION, OUTPUT);
}

void loop() {
  Serial.println("Inverter ignition: ON");
  digitalWrite(HW_PIN_INVERTER_IGNITION, HIGH);
  delay(4000);
  Serial.println("Inverter ignition: OFF");
  digitalWrite(HW_PIN_INVERTER_IGNITION, LOW);
  delay(4000);
}
