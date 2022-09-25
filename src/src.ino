// Version 0.0.2 - Fan PWM Cycle plus Tach
// Sam Rozell
// 23 July 2022

#include "ice_box.h"

// Configure Timer 2 (pin 3) to output 25kHz PWM. 
// Pin 11 will be unavailable for output in this mode
void setupTimer2( void ){
    //Set PWM frequency to about 25khz on pin 3 (timer 2 mode 5, prescale 8, count to 79)
    TIMSK2 = 0;
    TIFR2 = 0;
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << WGM22) | (1 << CS21);
    OCR2A = 79;
    OCR2B = 0;
}

// equivalent of analogWrite on pin 3
void setPWM2( float f ){
    // limit input from FAN_PWM_DC_MIN to 1
    f= (f<FAN_PWM_DC_MIN) ? FAN_PWM_DC_MIN : (f>1 ? 1 : f); 
    OCR2B = (uint8_t)(79*f);
}


//Interrupt handler. Stores the timestamps of the last 2 interrupts and handles debouncing
unsigned long volatile ts1=0,ts2=0;
void tachISR() {
    unsigned long m=millis();
    if((m-ts2)>FAN_TACH_DEBOUNCE){
        ts1=ts2;
        ts2=m;
    }
}
//Calculates the RPM based on the timestamps of the last 2 interrupts. Can be called at any time.
unsigned long calcRPM(){
    if(millis()-ts2<FAN_TACH_STUCK_THRESHOLD&&ts2!=0){
        return (60000/(ts2-ts1))/2;
    }else return 0;
}


void setup() {
  // enable fan power through relay
  pinMode(PIN_FAN_POWER, OUTPUT);
  digitalWrite(PIN_FAN_POWER, HIGH);

  // setp fan PWM timer
  pinMode(PIN_FAN_PWM, OUTPUT);
  setupTimer2();
  setPWM2(0.4f);

  // setup fan tachometry
  pinMode(PIN_FAN_TACH, INPUT_PULLUP);
  //set tachISR to be triggered when the signal on the sense pin goes low
  attachInterrupt(digitalPinToInterrupt(PIN_FAN_TACH), tachISR, FALLING); 
  Serial.begin(9600); //enable serial so we can see the RPM in the serial monitor
}

void loop() {

  setPWM2(0.4f);
  for (int i=0; i<10; i++) {
    delay(100);
    Serial.print("RPM:");
    Serial.println(calcRPM());
  }
  setPWM2(0.6f);
  for (int i=0; i<10; i++) {
    delay(100);
    Serial.print("RPM:");
    Serial.println(calcRPM());
  }
  setPWM2(0.8f);
  for (int i=0; i<10; i++) {
    delay(100);
    Serial.print("RPM:");
    Serial.println(calcRPM());
  }
}
