// Application Code
// tests to enable/disable features on the circuit board
#include "HW_ACM.h" // not sure about how to include the API

/* OVERVIEW
 * all features are disabled at the beginning
 * enter the number of the feature you want to test in serial monitor
 * the current will be measured and displayed...
 * ... before enabling the feature, after enabling the feature, and once again after disabling the feature
 * serial monitor will prompt you for a new feature

FEATURE NUMBERS AND CORRESPONDING FEATURES
feature = 0    featureName = "Mux"               featurePin = HW_PIN_MUX
feature = 1    featureName = "Serial Clock"      featurePin = HW_PIN_SCK
feature = 2    featureName = "AIR+"              featurePin = HW_PIN_EN_AIRP
feature = 3    featureName = "Mux Select 0"      featurePin = HW_PIN_MUX_S0
feature = 4    featureName = "Mux Select 1"      featurePin = HW_PIN_MUX_S1
feature = 5    featureName = "Mux Select 2"      featurePin = HW_PIN_MUX_S2
feature = 6    featureName = "Mux Select 3"      featurePin = HW_PIN_MUX_S3
feature = 7    featureName = "Precharge"         featurePin = HW_PIN_EN_PRECHARGE
feature = 8    featureName = "AMS Fault"         featurePin = HW_PIN_AMS_FAULT_EN
feature = 9    featureName = "RTD Buzzer"        featurePin = HW_PIN_RTD_BUZZER_EN
feature = 10   featureName = "AIR-"              featurePin = HW_PIN_EN_AIRN
feature = 11   featureName = "Shutdown Circuit"  featurePin = HW_PIN_SDC_EN
feature = 12   featureName = "RTD Confirmation"  featurePin = HW_PIN_RTD_CONFIRM
feature = 13   featureName = "CAN2 INT"          featurePin = HW_PIN_CAN2_INT
feature = 14   featureName = "CAN1 CS"           featurePin = HW_PIN_CAN1_CS
feature = 15   featureName = "MISO"              featurePin = HW_PIN_MISO
feature = 16   featureName = "MOSI"              featurePin = HW_PIN_MOSI
feature = 17   featureName = "CAN1 INT"          featurePin = HW_PIN_CAN1_INT
feature = 18   featureName = "CAN2 CS"           featurePin = HW_PIN_CAN2_CS
feature = 19   featureName = "Hard Fault Latch"  featurePin = HW_PIN_AMS_HFL_FAULT

*/

// assigning pins
int v_outPin = A20;
int v_refPin = A21;


int feature; // stores what feature the user wants to test (different numbers correspond to different parts)
String featureName; // stores name of feature for user to double check what they're testing
String featurePin; // stores pin names according to API

void setup() 

  // assigning pins to get current values
  pinMode(v_outPin, INPUT);
  pinMode(v_refPin, INPUT);
  Serial.begin(9600);

}

void loop() {

  turnOff();
  Serial.println("Enter the Feature You Would Like to Test: ");
  while (Serial.available() == 0) { // wait for user to enter a number
  } 
   
  feature = Serial.parseInt();
  Serial.print("Testing Feature #");
  Serial.print(feature);
  Serial.print(": ");
  getFeatureName(feature); // function will assign featureName to the appropriate name based on feature number
  Serial.println(featureName); // print out the name
  testFeature(feature); // test the feature (gets current before enabling, after enabling, and once again after disabling)
  
}


// TEST FEATURE ----------------------------------------------------------------------------------------------------------------------


void testFeature(int feature) {
    
    // assign corrrect pin name to featurePin variable
    if (feature == 0)
    {
      featurePin = HW_PIN_MUX;
    } 
    else if (feature == 1)
    {
      featurePin = HW_PIN_SCK;
    }
    else if (feature == 2)
    {
      featurePin = HW_PIN_EN_AIRP;
    } 
    else if (feature == 3)
    {
      featurePin = HW_PIN_MUX_S0;
    }
    else if (feature == 4)
    {
      featurePin = HW_PIN_MUX_S1;
    }
    else if (feature == 5)
    {
      featurePin = HW_PIN_MUX_S2;
    }
    else if (feature == 6)
    {
       featurePin = HW_PIN_MUX_S3;
    }
    else if (feature == 7)
    {
       featurePin = HW_PIN_EN_PRECHARGE;
    }
    else if (feature == 8)
    {
       featurePin = HW_PIN_AMS_FAULT_EN;
    }
    else if (feature == 9)
    {
       featurePin = HW_PIN_RTD_BUZZER_EN;
    }
    else if (feature == 10)
    {
       featurePin = HW_PIN_EN_AIRN;
    }
    else if (feature == 11)
    {
       featurePin = HW_PIN_SDC_EN;
    } 
    else if (feature == 12)
    {
       featurePin = HW_PIN_RTD_CONFIRM;
    } 
    else if (feature == 13)
    {
       featurePin = HW_PIN_CAN2_INT;
    } 
    else if (feature == 14)
    {
       featurePin = HW_PIN_CAN1_CS;
    } 
    else if (feature == 15)
    {
       featurePin = HW_PIN_MISO;
    }
    else if (feature == 16)
    {
       featurePin = HW_PIN_MOSI;
    }
    else if (feature == 17)
    {
       featurePin = HW_PIN_CAN1_INT;
    }
    else if (feature == 18)
    {
       featurePin = HW_PIN_CAN2_CS;
    } 
    else if (feature == 19)
    {
       featurePin = HW_PIN_AMS_HFL_FAULT;
    } 

    Serial.print("Current before enabling ... ");
    getCurrent(); // find current before enabling feature
    HW_digitalWrite(featurePin, true); // enabling the appropriate feature
    delay(1000); // not sure if needed
    Serial.print("Current after enabling ... ");
    getCurrent(); // find current after enabling feature
    HW_digitalWrite(featurePin, false); // disable feature
    Serial.print("Current after disabling ... ");
    getCurrent(); // make sure it worked
    
    
}

// DISABLE ALL FEATURES ----------------------------------------------------------------------------------------------------------------------

void turnOff() {
  
   // digitalWrite everything to low at the beginning and end for good measure
  HW_digitalWrite(HW_PIN_MUX, false);
  HW_digitalWrite(HW_PIN_SCK, false);
  HW_digitalWrite(HW_PIN_EN_AIRP, false);
  HW_digitalWrite(HW_PIN_MUX_S0, false);
  HW_digitalWrite(HW_PIN_MUX_S1, false);
  HW_digitalWrite(HW_PIN_MUX_S2, false);
  HW_digitalWrite(HW_PIN_MUX_S3, false);
  HW_digitalWrite(HW_PIN_EN_PRECHARGE, false);
  HW_digitalWrite(HW_PIN_AMS_FAULT_EN, false);
  HW_digitalWrite(HW_PIN_RTD_BUZZER_EN, false);
  HW_digitalWrite(HW_PIN_EN_AIRN, false);
  HW_digitalWrite(HW_PIN_SDC_EN, false);
  HW_digitalWrite(HW_PIN_RTD_CONFIRM, false);
  HW_digitalWrite(HW_PIN_CAN2_INT, false);
  HW_digitalWrite(HW_PIN_CAN1_CS, false);
  HW_digitalWrite(HW_PIN_MISO, false);
  HW_digitalWrite(HW_PIN_MOSI, false);
  HW_digitalWrite(HW_PIN_CAN1_INT, false);
  HW_digitalWrite(HW_PIN_CAN2_CS, false);
  HW_digitalWrite(HW_PIN_AMS_HFL_FAULT, false);
   
}

// GET FEATURE NAME ----------------------------------------------------------------------------------------------------------------------

void getFeatureName() {
   
   if (feature == 0)
   {
     featureName = "Mux"; // HW_PIN_MUX
   } 
   else if (feature == 1)
   {
     featureName = "Serial Clock"; // HW_PIN_SCK
   }
   else if (feature == 2)
   {
     featureName = "AIR+"; // HW_PIN_EN_AIRP
   } 
   else if (feature == 3)
   {
     featureName = "Mux Select 0"; // HW_PIN_MUX_S0
   }
   else if (feature == 4)
   {
     featureName = "Mux Select 1"; // HW_PIN_MUX_S1
   }
   else if (feature == 5)
   {
     featureName = "Mux Select 2"; // HW_PIN_MUX_S2
   }
   else if (feature == 6)
   {
      featureName = "Mux Select 3"; // HW_PIN_MUX_S3
   }
   else if (feature == 7)
   {
      featureName = "Precharge"; // HW_PIN_EN_PRECHARGE
   }
   else if (feature == 8)
   {
      featureName = "AMS Fault"; // HW_PIN_AMS_FAULT_EN
   }
   else if (feature == 9)
   {
      featureName = "RTD Buzzer"; // HW_PIN_RTD_BUZZER_EN
   }
   else if (feature == 10)
   {
      featureName = "AIR-"; // HW_PIN_EN_AIRN
   }
   else if (feature == 11)
   {
      featureName = "Shutdown Circuit"; // HW_PIN_SDC_EN
   } 
   else if (feature == 12)
   {
      featureName = "RTD Confirmation"; // HW_PIN_RTD_CONFIRM
   } 
   else if (feature == 13)
   {
      featureName = "CAN2 INT"; // HW_PIN_CAN2_INT
   } 
   else if (feature == 14)
   {
      featureName = "CAN1 CS"; // HW_PIN_CAN1_CS
   } 
   else if (feature == 15)
   {
      featureName = "MISO"; // HW_PIN_MISO
   }
   else if (feature == 16)
   {
      featureName = "MOSI"; // HW_PIN_MOSI
   }
   else if (feature == 17)
   {
      featureName = "CAN1 INT"; // HW_PIN_CAN1_INT
   }
   else if (feature == 18)
   {
      featureName = "CAN2 CS"; // HW_PIN_CAN2_CS
   } 
   else if (feature == 19)
   {
      featureName = "Hard Fault Latch"; // HW_PIN_AMS_HFL_FAULT
   } 
   
}

// GETTING CURRENT ----------------------------------------------------------------------------------------------------------------------

float get_v_out(int out) {
  int reading = analogRead(out); // get the v_out from sensor
  float v = (reading * 5) / 1023; // convert to the actual voltage
  return v;
}


float get_v_ref(int ref) {
  int reading = analogRead(ref); // get the v_ref from sensor
  float v = (reading * 5) / 1023; // convert to the actual voltage
  return v;
}


float transferFunction(int out, int ref) {
  // sub v_out and v_ref into the transfer function
  
  // NOMINAL Vout TRANSFER FUNCTION
  // MCA1101-5-5, Vout = Vref + IIN x 350 mV/A
  // i_in = (v_out - v_ref V) / 0.35 V/A
  
  float i_in = (v_out - v_ref) / 0.35;
  return i_in;
}


float getError(float c) {  
  // total error is +/- 1.0 % RD
  
  float error = 0.01 * c;
  return error;
}

void getCurrent() {
  float v_out = get_v_out(v_outPin);
  float v_ref = get_v_ref(v_refPin);
  float current = transferFunction(v_out, v_ref); // pass calculated Vout and Vref into transfer function
  float error = getError(current);
  
  Serial.print("current is ");
  Serial.print(current);
  Serial.print(" A ");
  Serial.print("± ");
  Serial.print(error);
  Serial.println(" A");

  Serial.print("V_out is ");
  Serial.print(v_out);
  Serial.print(" V")
  Serial.print(" and V_ref is ");
  Serial.print(v_ref);
  Serial.println(" V");
  Serial.println("----------------------------------------------------------------------------------------- ");
}
