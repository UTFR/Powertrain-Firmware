#define ANALOG_CONSTANT 1023.0  // 10-bit ADC resolution on Arduino
#define BASE_DIGI_VOLTS 510.0     // at 0 A
#define SENS 26.7             // mV/A

int analogReading = 0;
float current = 0.0;       

float getCurrent(int reading){
  float milliVolts = (((reading-BASE_DIGI_VOLTS)/ANALOG_CONSTANT) * 5.0)* 1000;
  return (milliVolts/SENS);
}

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  analogReading = analogRead(A0);
  current = getCurrent(analogReading);
  Serial.print("Current out: "); Serial.println(current);
  delay(1);
}
