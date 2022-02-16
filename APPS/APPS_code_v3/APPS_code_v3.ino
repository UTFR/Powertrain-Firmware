#include "UTFR_APPS_DAC.h"

UTFR_APPS_DAC apps;
const int kDAC_CS_ = 7;      // DAC CS pin
    
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize DAC:
  apps.begin(kDAC_CS_);
  
}

void loop()
{
  apps.processThrottlePosition();
}
