                                                                                                                                                                                                                                                                                                                                                                                 #include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define HW_PIN_LED_DRIVER 5
#define HW_PIN_RGB_DRIVER 45
#define HW_PIN_CHASSIS_DSC 39

#define NUMPIXELS 1 

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.

Adafruit_NeoPixel LED_DRIVE(NUMPIXELS, HW_PIN_LED_DRIVER, NEO_GRB + NEO_KHZ400);    // !!!! CHANGE TO "NEO_KHZ800" FOR HIGH SPEED (i think - see library documentation online to confirm)
//Adafruit_NeoPixel RGB_DRIVE(NUMPIXELS, HW_PIN_RGB_DRIVER, NEO_RGB + NEO_KHZ400);

#define DELAYVAL 500 // Time (in milliseconds) to pause between updates

void setup() {

  Serial.begin(9600);

  pinMode(HW_PIN_LED_DRIVER, OUTPUT);
  //pinMode(HW_PIN_RGB_DRIVER, OUTPUT);
  
  //pinMode(HW_PIN_CHASSIS_DSC, OUTPUT);
  //digitalWrite(HW_PIN_CHASSIS_DSC, HIGH);

  
  LED_DRIVE.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  //RGB_DRIVE.begin();
  
}

void loop() {
  LED_DRIVE.clear(); // Set all pixel colors to 'off'
  //RGB_DRIVE.clear();
  
  for(int i=0; i<256; i+=15) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    Serial.print("Intensity: "); Serial.println(i);
    LED_DRIVE.setPixelColor(0, LED_DRIVE.Color(i, i, i));           // setPixelColor(pixel_index, pixel_color)

    LED_DRIVE.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
  
  delay(3000);
}
