int state = 0;
int nextState = 1;

// isotemp parameters
float b = 1.25;
float m = 0.408;
float voltage;

#define SEL_A 2
#define SEL_B 3
#define SEL_C 4

#define MEASURE (((analogRead(A0) * 5.3 / 1023.0) - 1.25) / 0.408)

void setSelects(int tempSensor)
{
  int a = 0;
  int b = 0;
  int c = 0;

  switch(tempSensor)
  {
    case 1: // Calibration
      a = 0;
      b = 0;
      c = 0;
      break;
    case 2:
      a = 1;
      b = 0;
      c = 0;
      break;
    case 3:
      a = 0;
      b = 1;
      c = 0;
      break;
    case 4:
      a = 1;
      b = 1;
      c = 0;
      break;
    case 5:
      a = 0;
      b = 0;
      c = 1;
      break;
    case 6:
      a = 1;
      b = 0;
      c = 1;
      break;
    case 7:
      a = 0;
      b = 1;
      c = 1;
      break;
    case 8:
      a = 1;
      b = 1;
      c = 1;
      break;
    default:
      break;
  }
  
  digitalWrite(SEL_A, a);
  digitalWrite(SEL_B, b);
  digitalWrite(SEL_C, c);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Select signals
  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);
  pinMode(SEL_C, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  switch (state)
  {
    case 0:
      setSelects(1); // calibration resistors
      voltage = MEASURE;
      Serial.print("Calibration voltage is ");
      Serial.println(voltage);
      if ((voltage > 2.4) && (voltage < 2.8)) state = nextState;
      break;
    case 1:
      setSelects(2);
      Serial.println("Place 330 ohm resistor across TS2");
      voltage = MEASURE;
      Serial.print("TS2 = ");
      Serial.println(voltage);
      if ((voltage > 1.65) && (voltage < 1.85)) 
      {
        Serial.println("SUCCESS: SENSOR CALIBRATED");
        delay(500);
        nextState = 2;
        state = 0; // return to calibration
      }
      break;
    case 2:
      setSelects(3);
      Serial.println("Place 330 ohm resistor across TS3");
      voltage = MEASURE;
      Serial.print("TS3 = ");
      Serial.println(voltage);
      if ((voltage > 1.65) && (voltage < 1.85)) 
      {
        Serial.println("SUCCESS: SENSOR CALIBRATED");
        delay(500);
        nextState = 3;
        state = 0; // return to calibration
      }
      break;
    case 3:
      setSelects(4);
      Serial.println("Place 330 ohm resistor across TS4");
      voltage = MEASURE;
      Serial.print("TS4 = ");
      Serial.println(voltage);
      if ((voltage > 1.65) && (voltage < 1.85)) 
      {
        Serial.println("SUCCESS: SENSOR CALIBRATED");
        delay(500);
        nextState = 4;
        state = 0; // return to calibration
      }
      break;
    case 4:
      setSelects(5);
      Serial.println("Place 330 ohm resistor across TS5");
      voltage = MEASURE;
      Serial.print("TS5 = ");
      Serial.println(voltage);
      if ((voltage > 1.65) && (voltage < 1.85)) 
      {
        Serial.println("SUCCESS: SENSOR CALIBRATED");
        delay(500);
        nextState = 5;
        state = 0; // return to calibration
      }
      break;
    case 5:
      setSelects(6);
      Serial.println("Place 330 ohm resistor across TS6");
      voltage = MEASURE;
      Serial.print("TS6 = ");
      Serial.println(voltage);
      if ((voltage > 1.65) && (voltage < 1.85)) 
      {
        Serial.println("SUCCESS: SENSOR CALIBRATED");
        delay(500);
        nextState = 6;
        state = 0; // return to calibration
      }
      break;
    case 6:
      setSelects(7);
      Serial.println("Place 330 ohm resistor across TS7");
      voltage = MEASURE;
      Serial.print("TS7 = ");
      Serial.println(voltage);
      if ((voltage > 1.65) && (voltage < 1.85)) 
      {
        Serial.println("SUCCESS: SENSOR CALIBRATED");
        delay(500);
        nextState = 7;
        state = 0; // return to calibration
      }
      break;
    case 7:
      setSelects(8);
      Serial.println("Place 330 ohm resistor across TS8");
      voltage = MEASURE;
      Serial.print("TS8 = ");
      Serial.println(voltage);
      if ((voltage > 1.65) && (voltage < 1.85)) 
      {
        Serial.println("SUCCESS: SENSOR CALIBRATED");
        delay(500);
        nextState = 8;
        state = 0; // return to calibration
      }
      break;
    case 8:
      Serial.println("CALIBRATION COMPLETE");
      state = 9;
    case 9:
    default:
      break;
  }

  delay(100);
}
