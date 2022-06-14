#include "UTFR_APPS_V2.h"


UTFR_APPS::UTFR_APPS(uint8_t dataOut, uint8_t clock)  : DAC(dataOut, clock)
{

}

void UTFR_APPS::begin(int CS){

    // give the sensor time to set up:
    delay(100);

    // initalize the  data ready and chip select pins:
    pinMode(THR_1_IN_PIN, INPUT);                 // APPS_In_1_Analog pin
    pinMode(THR_2_IN_PIN, INPUT);                 // APPS_In_2_Analog pin
    pinMode(BRAKE_IN_PIN, INPUT);                 // _Brake_in pin
    pinMode(DAC_IN_PIN, INPUT);                 // APPS_Out_Analog

    DAC.begin(CS);

    this->calibrateInputs();

}

void UTFR_APPS::calibrateInputs()
{
  #ifdef debugMode
  Serial.println("UTFR_APPS::calibrateInputs : Starting Throttle Calibration....");
  #endif

  int counter = 0;

  while (counter < 10000/_kCALIBRATIONRATE_) //5000ms calibration time
  {
    _thr_1_in_ = analogRead(A0);
    _thr_2_in_ = analogRead(A1);
    _brake_in_ = analogRead(A2);

    if (_thr_1_in_ > _thr_1_high_lim_){
      _thr_1_high_lim_ = _thr_1_in_;
    }

    if (_thr_1_in_ < _thr_1_low_lim_){
      _thr_1_low_lim_ = _thr_1_in_;
    }

    if (_thr_2_in_ > _thr_2_high_lim_){
      _thr_2_high_lim_ = _thr_2_in_;
    }

    if (_thr_2_in_ < _thr_2_low_lim_){
      _thr_2_low_lim_ = _thr_2_in_;
    }

    if (_brake_in_ > _brake_in_high_lim_){
      _brake_in_high_lim_ = _brake_in_;
    }

    if (_brake_in_ < _brake_in_low_lim_){
      _brake_in_low_lim_ = _brake_in_;
    }

    ++counter;
    delay(_kCALIBRATIONRATE_);
  }

  _thr_1_high_lim_ += _lim_margin_;
  _thr_2_high_lim_ += _lim_margin_;

  #ifdef debugMode
  Serial.println("UTFR_APPS::calibrateInputs: Calibration Complete:");
  Serial.print("Thr 1- High: ");Serial.print(_thr_1_high_lim_);
  Serial.print(" Low: ");Serial.println(_thr_1_low_lim_);
  Serial.print("Thr 2- High: ");Serial.print(_thr_2_high_lim_);
  Serial.print(" Low: ");Serial.println(_thr_2_low_lim_);
  Serial.print("Brakes- High: ");Serial.print(_brake_in_high_lim_);
  Serial.print(" Low: ");Serial.println(_brake_in_low_lim_);
  #endif

  _thr_out_ = analogRead(DAC_IN_PIN);
}

void UTFR_APPS::processThrottlePosition()
{   
  if (_shutdown_active_)
  {
    DAC.analogWrite(0, 0);
    return;
  }

  // read in the voltage values and convert to digital
  _thr_1_in_ = analogRead(THR_1_IN_PIN);
  _thr_2_in_ = analogRead(THR_2_IN_PIN);
  _brake_in_ = analogRead(BRAKE_IN_PIN);

  // Normalize pedal position readings
  
  _thr_1_normalized_ = 
      map_Generic(_thr_1_in_, _thr_1_low_lim_, _thr_1_high_lim_, 0.0, _normalized_max_); 
  _thr_2_normalized_ = 
      map_Generic(_thr_2_in_, _thr_2_low_lim_, _thr_2_high_lim_, 0.0, _normalized_max_);
  _brake_normalized_ = 
      map_Generic(_brake_in_, _brake_in_low_lim_, _brake_in_high_lim_, 0.0, _normalized_max_);

   _thr_out_ = _normalized_max_ - ((_thr_1_normalized_ + _thr_2_normalized_) / 2.0);      // Take average and round

  if (this->checkErrorCountdown() && 
      this->checkThrottleConditions() && 
      this->checkBrakeConditions())
  {
    this->sendOutput();
    this->checkOutputConditions();
  }

}

bool UTFR_APPS::checkErrorCountdown()
{
  //T4.2.5
  if (_thr_plausibility_error_active_)
  {
    if (millis() - _thr_plausibility_error_time_ > _kERROR_TIME_THRESHOLD_)
    {
      Serial.print("UTFR_APPS::checkErrorCountdown: Throttle Plausiblity Shutdown");
      _shutdown_active_ = true;
      return false;
    }
  }

  if (_thr_limit_error_active_)
  {
    if (millis() - _thr_limit_error_time_ > _kERROR_TIME_THRESHOLD_)
    {
      Serial.print("UTFR_APPS::checkErrorCountdown: Throttle Limit Shutdown");
      _shutdown_active_ = true;
      return false;   
    }
  }

  if (_thr_output_error_active_)
  {
    if (millis() - _thr_output_error_time_ > _kERROR_TIME_THRESHOLD_)
    {
      Serial.print("UTFR_APPS::checkErrorCountdown: Output Plausiblity Shutdown");
      _shutdown_active_ = true;
      return false;   
    }

  }

  //T4.3.3
  if (_brake_limit_error_active_)
  {
    if (millis() - _brake_limit_error_time_ >  _kERROR_TIME_THRESHOLD_)
    {      
      Serial.print("UTFR_APPS::checkErrorCountdown: Brake Limit Shutdown");
      _shutdown_active_ = true;
      return false;
    }
  }

  return true;

}

bool UTFR_APPS::checkBrakeConditions()
{
  //T4.3.4
  if (_brake_in_ > _normalized_max_ - _lim_margin_ || _brake_in_ < _lim_margin_)
  { 
    //Start 100ms countdown;
    if (!_brake_limit_error_active_)
    {
      Serial.println("UTFR_APPS::checkBrakeConditions: Brake Input Exceeds Range");
      _brake_limit_error_active_ = true;
      _brake_limit_error_time_ = millis();
    }
    return false;
  }

  else
  {
    _brake_limit_error_active_ = false;
    _brake_limit_error_time_ = 0;
  }

  //E5.7
  if (_brake_normalized_ > _kBRAKE_ON_THRESHOLD_*_normalized_max_)
  {
    Serial.print("1:");Serial.print(_thr_1_normalized_);
    Serial.print(" 2:");Serial.println(_thr_2_normalized_);
    if (_thr_out_ > 0.25*_normalized_max_)
    {
      if (!_brake_plausibility_error_active_){
        Serial.println("UTFR_APPS:checkBrakeCondition: APPS/Brake Plausibility Fail");
        _brake_plausibility_error_active_ = true;
      }
    return false;

    }
  
    //ES5.7.2
    if ((_thr_out_ < 0.05*_normalized_max_) &&
        _brake_plausibility_error_active_)
    {
      Serial.println("UTFR_APPS::checkBRakeCondition: APPS/Brake Plausiblity Reset");
      _brake_plausibility_error_active_ = false;
    }
  }
  else
  {
    _brake_plausibility_error_active_ = false;
  }

  return true;
}

bool UTFR_APPS::checkThrottleConditions()
{
  //T4.2.4
  if (abs(_thr_1_normalized_ - _thr_2_normalized_) > 
      _normalized_max_*_kTHR_MAX_DEVIATION_)
  {
    if (!_thr_plausibility_error_active_)
    {
      Serial.println("UTFR_APPS::checkThrottleconditions: Throttle Plausibility Error");
      #ifdef debugMode
      Serial.print("1:");Serial.print(_thr_1_normalized_);
      Serial.print(" 2:");Serial.println(_thr_2_normalized_);
      #endif 
      //Start 100ms countdown;
      _thr_plausibility_error_active_ = true;
      _thr_plausibility_error_time_ = millis();
    }

    return false;
  } 

  else 
  {
    //Reset Timer
    _thr_plausibility_error_active_ = false;
    _thr_plausibility_error_time_ = 0;
  }

  //T4.2.10
  if (_thr_1_in_ > _normalized_max_ || _thr_1_in_ < 0 ||
      _thr_2_in_ > _normalized_max_ || _thr_2_in_ < 0)
  {
    if (!_thr_limit_error_active_){
      Serial.println("UTFR_APPS::checkThrottleconditions: Throttle Input Exceeds Range");
      _thr_limit_error_active_ = true;
      _thr_limit_error_time_ = millis();
      #ifdef debugMode
      Serial.print("1:");Serial.print(_thr_1_in_);
      Serial.print(" 2:");Serial.println(_thr_2_in_);
      #endif 
    }
    return false;
  }
  else
  {
    //Reset Timer
    _thr_limit_error_active_ = false;
    _thr_limit_error_time_ = 0;
  }

  return true;
}

bool UTFR_APPS::checkOutputConditions()
{
  _dac_in_ = analogRead(DAC_IN_PIN);

    //T4.2.9
  if (abs(_thr_out_ - _dac_in_) >
      _normalized_max_*_kTHR_MAX_DEVIATION_)
  {
    if (!_thr_output_error_active_)
    {
      Serial.println("UTFR_APPS::checkThrottleConditions: Output Plausiblity Error");
      #ifdef debugMode
      Serial.print("Requested Output:");Serial.println(_thr_out_);
      Serial.print("Measured Output:");Serial.println(_dac_in_);
      Serial.print("1:");Serial.print(_thr_1_in_);
      Serial.print(" 2:");Serial.println(_thr_2_in_);
      #endif 
      _thr_output_error_active_ = true;
      _thr_output_error_time_ = millis();
    }
    return;
  }

  else
  {
    _thr_output_error_active_ = false;
    _thr_output_error_time_ = millis(); 
  }

}

void UTFR_APPS::sendOutput()
{

  #ifdef debugMode
  //Serial.print("1:");Serial.print(_thr_1_normalized_);
  //Serial.print(" 2:");Serial.println(_thr_2_normalized_);
  Serial.print("Throttle: "); Serial.print(_thr_out_); Serial.print(" | DAC: "); Serial.println(_dac_in_);
  #endif
  DAC.analogWrite(_thr_out_, 0);
}

void UTFR_APPS::shutdown()
{
  _shutdown_active_ = true;
}


float UTFR_APPS::getThrottlePosition()
{
  return _thr_out_;
}


bool UTFR_APPS::getShutdownState()
{
  return _shutdown_active_;
}