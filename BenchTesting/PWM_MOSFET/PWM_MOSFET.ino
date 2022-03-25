#define HW_PIN_PWM_MOTOR_MOSFET 8
#define HW_PIN_PWM_INVERTER_MOSFET 9
#define HW_PIN_CHASSIS_DSC 39

void setup() {
  Serial.begin(9600);
  pinMode(HW_PIN_CHASSIS_DSC, OUTPUT);
  digitalWrite(HW_PIN_CHASSIS_DSC, HIGH);
  
  pinMode(HW_PIN_PWM_MOTOR_MOSFET, OUTPUT);
  pinMode(HW_PIN_PWM_INVERTER_MOSFET, OUTPUT);
}

void loop() {
  
  for (int PWM = 250; PWM >= 0; PWM -= 25){
    analogWrite(HW_PIN_PWM_INVERTER_MOSFET, HIGH);
    analogWrite(HW_PIN_PWM_MOTOR_MOSFET, HIGH);
    Serial.print("PWM OUT = "); Serial.println(PWM);
    delay(1000);
  } 
  
}
