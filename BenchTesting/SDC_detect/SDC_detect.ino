/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
 

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define HW_PIN_SDC_INT 19
#define HW_PIN_CHASSIS_GND_DSC 39


/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/


/******************************************************************************
 *         F O R W A R D   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/
void SDC_ISR(void);


/******************************************************************************
 *                  D A T A   D E F I N I T I O N S                           *
 *****************************************************************************/
bool SDC_tripped = false;
int SDC_pin_state = 0;

/******************************************************************************
 *              F U N C T I O N S  D E F I N I T I O N S                      *
 *****************************************************************************/
void SDC_ISR(){
  SDC_tripped = true;
}

/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/
void setup()
{
  Serial.begin(9600);

  pinMode(HW_PIN_CHASSIS_GND_DSC, OUTPUT);           
  digitalWrite(HW_PIN_CHASSIS_GND_DSC, HIGH);       // Close mosfet to connect Mega GND to Chassis GND
  
  pinMode(HW_PIN_SDC_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(HW_PIN_SDC_INT), SDC_ISR, RISING);
}

void loop()
{
  
  if (SDC_tripped){
    // Check again for SDC trip
    if (digitalRead(HW_PIN_SDC_INT) == 0){
      SDC_tripped = false;
    }
    else{
      Serial.println("SDC trip detected!");
    }
  }
  
  SDC_pin_state = digitalRead(HW_PIN_SDC_INT);
  Serial.print("Pin state: "); Serial.println(SDC_pin_state);
  
}
