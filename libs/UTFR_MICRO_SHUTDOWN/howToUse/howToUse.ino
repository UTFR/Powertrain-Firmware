#include "UTFR_APPS.h"
#include "UTFR_MICRO_SHUTDOWN.h"


UTFR_APPS APPS;
UTFR_MICRO_SHUTDOWN SHUTDOWN(APPS);

void setup() {
  Serial.begin(9600);

  APPS.begin(10);
}

void loop() {
  
}
