
#include "HW_pinDriver.h"

extern const HW_pinConfig_S HW_pinConfig[HW_PIN_COUNT];

void setup() {
  // put your setup code here, to run once:
  HW_pinConfig_init(HW_pinConfig);
  HW_pinConfig_initDigOut(HW_pinConfig);
}


void loop() {
  // put your main code here, to run repeatedly:
}
