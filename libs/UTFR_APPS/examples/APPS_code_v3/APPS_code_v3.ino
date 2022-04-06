#include "UTFR_APPS.h"

UTFR_APPS APPS;
const int DAC_CS = 7;      // DAC CS pin

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize DAC:
  APPS.begin(DAC_CS);
}
 
void loop()
{
  APPS.processThrottlePosition();
  delay(1000);
}
