// Version 0.0.2 - Fan PWM Cycle plus Tach
// Sam Rozell
// 23 July 2022

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "ice_box.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  BoardPinsConfigure();
  FanPwmTimerConfigure();
  FanPwmSetDutyCyclePercent(FAN_PWM_DC_OFF);
  FanTachConfigure();
  mlx.begin();
  Serial.begin(9600);
}

void loop() {

  // Capture new input
//  unsigned int new_duty_cycle = PotCalculateDutyCycle(); // Pot control
  unsigned int new_duty_cycle = TempCalculateDutyCyclePercent(); // Temp control
  bool laptop_contact = SwitchIsClosed();
  int fan_power_enable = laptop_contact; // If laptop is removed, turn off fan power

  // Apply new input
  digitalWrite(PIN_FAN_POWER, HIGH);
  FanPwmSetDutyCyclePercent(new_duty_cycle);

  // Loop
  TempSerialPrint();
  FanTachSerialPrint();
  delay(500);
}

/*
* Calculates the Duty Cycle setting from Potentiometer position
* Returns:
*     duty_cycle_percent - integer percentage (0-100)
* Note: allows 0-100, not ON_MIN-100
*/
unsigned int PotCalculateDutyCycle() {
  int pot_value = analogRead(PIN_POT_FRONT);

  // scale to duty cycle percent
  long int duty_cycle_percent = round(pot_value * 100L / 1023L); // want 0-100 from 0-1023

  Serial.print("Pot: ");
  Serial.println(pot_value);
  Serial.print("Pot DC: ");
  Serial.print(duty_cycle_percent);
  Serial.println(" %");

  return duty_cycle_percent;
}

bool SwitchIsClosed() {
  return digitalRead(PIN_SWITCH);
}

void TempSerialPrint() {
 Serial.print("T: ");
 Serial.print(TempGetObjectC());
 Serial.print(" C, ");;
 Serial.print(TempGetObjectF());
 Serial.println(" F");
}

int TempGetObjectF() {
  return mlx.readObjectTempF();
}

int TempGetObjectC() {
  return mlx.readObjectTempC();
}

int TempCalculateDutyCyclePercent() {

  int new_temp = TempGetObjectF();
  if (new_temp < TEMP_DC_OFF_F) {
    return FAN_PWM_DC_OFF;
  } 

  if (new_temp > TEMP_DC_ON_MAX_F) {
    return FAN_PWM_DC_ON_MAX;
  }

  // scale to duty cycle
  int duty_cycle = (new_temp - TEMP_DC_OFF_F) * 10; // remove offset, 1F=10%

  // return duty cycle limited from 0-100
  return (duty_cycle < 0 ? 0 : (duty_cycle > 100 ? 100 : duty_cycle));
}

void FanTachSerialPrint() {
  Serial.print("Fan:");
  Serial.print(FanTachCalculateRPM());
  Serial.println(" RPM");
}


/*
* Initializes mode for all pins on the board
*/
void BoardPinsConfigure() {
  pinMode(PIN_POT_FRONT, INPUT);
  pinMode(PIN_SWITCH, INPUT);
  pinMode(PIN_FAN_POWER, OUTPUT);
  pinMode(PIN_FAN_PWM, OUTPUT);
  pinMode(PIN_FAN_TACH, INPUT_PULLUP);
}


/*
* Configures Timer 2 (pin 3) to output 25kHz PWM
* Note: Pin 11 will be unavailable for output in this mode
*/
void FanPwmTimerConfigure() {
  TCCR2A = 0;   // TC2 Control Register A
  TCCR2B = 0;   // TC2 Control Register B
  TIMSK2 = 0;   // TC2 Interrupt Mask Register
  TIFR2 = 0;    // TC2 Interrupt Flag Register
  TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);  // OC2B cleared/set on match when up/down counting, fast PWM
  TCCR2B |= (1 << WGM22) | (1 << CS21);     // prescaler 8
  OCR2A = FAN_PWM_OCR2A_TOP;   // TOP overflow value (Hz)
  OCR2B = 0;
}

/*
* Sets a new Duty Cycle value for Fan PWM
* equivalent of analogWrite on pin 3
* Parameters:
*     percent - integer percentage (0-100) for desired duty cycle
* Note: percents below the minimum fan DC will turn the fan off
*/
void FanPwmSetDutyCyclePercent( unsigned int percent ){
  // limit percent to safe range
  percent = (percent < FAN_PWM_DC_ON_MIN) ? FAN_PWM_DC_OFF : (percent > FAN_PWM_DC_ON_MAX ? FAN_PWM_DC_ON_MAX : percent);

  // scale to FAN_PWM_OCR2A_TOP setting
  uint8_t ocr2b = FAN_PWM_OCR2A_TOP * percent * 0.01f;
  Serial.print("OCR2B: ");
  Serial.println(ocr2b);

  OCR2B = ocr2b;
}


// Interrupt handler. Stores the timestamps of the last 2 interrupts and handles debouncing
unsigned long volatile ts1 = 0, ts2 = 0;
void TachISR() {
  unsigned long m = millis();
  if( (m - ts2) > FAN_TACH_DEBOUNCE ) {
    ts1 = ts2;
    ts2 = m;
  }
}

/*
* Configures Fan tachometer pin interrupt
*/
void FanTachConfigure() {
  attachInterrupt(digitalPinToInterrupt(PIN_FAN_TACH), TachISR, FALLING); 
}

/* 
* Calculates the RPM based on the timestamps of the last 2 interrupts.
* Can be called at any time
*/
unsigned long FanTachCalculateRPM() {
  if ( (millis() - ts2) < FAN_TACH_STUCK_THRESHOLD && ts2 != 0 ) {
    return ( 60000 / (ts2 - ts1) ) / 2;
  }
  return FAN_TACH_RPM_INVALID;
}
