#include "UTFR_APPS_DAC.h"

UTFR_APPS_DAC apps;
const int DAC_CS = 8;      // DAC CS pin
    
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize DAC:
  apps.begin(DAC_CS);
  
}

void loop()
{
  apps.processThrottlePosition();
}
