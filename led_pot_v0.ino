
int potentiometerPin = A5; // pot
int potValue = 0;
int brightness;
int gLedPin = 3; //pwm

void setup() {
  // put your setup code here, to run once:
  pinMode(potentiometerPin, INPUT);
  pinMode(gLedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  potValue = analogRead(potentiometerPin);
  brightness = potValue/4; // want 0-255 brightness from 0-1023
  analogWrite(gLedPin, brightness);
  Serial.println(brightness);
  delay(1);
}
