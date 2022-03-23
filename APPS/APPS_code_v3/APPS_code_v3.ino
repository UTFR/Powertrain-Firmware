#include "UTFR_APPS_DAC.h"

UTFR_APPS_DAC APPS;
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
}
