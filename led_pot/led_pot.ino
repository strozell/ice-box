// Version 1.0

#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define MLX_I2C_ADDR 0x5A
int potentiometerPin = A1; // pot
int potValue = 0;
int tempF = 0;
int brightness = 255;
int threshold;
int rLedPin = 2; //pwm
int gLedPin = 3; //pwm
int bLedPin = 4; //pwm
int selectedLedPin;

void setup() {
  // put your setup code here, to run once:
  pinMode(potentiometerPin, INPUT);
  pinMode(rLedPin, OUTPUT);
  pinMode(gLedPin, OUTPUT);
  pinMode(bLedPin, OUTPUT);
  Serial.begin(9600);
  mlx.begin();
}

void loop() {
  // get new Potentiometer threshold
  potValue = analogRead(potentiometerPin);
  threshold = potValue/10 + 50; // want 50 to 150 F from 0-1023
  Serial.print("Threshold = "); Serial.println(threshold);

  // get new object and ambient temperature
  tempF = mlx.readObjectTempF();
  //Serial.print("Object = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F"); 
  //Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); Serial.println("*F"); 
  Serial.print("tempF = "); Serial.println(tempF);

  // red/blue thresholding
  selectedLedPin = tempF < threshold ? bLedPin : rLedPin;
  Serial.print("selectedLedPin = "); Serial.println(selectedLedPin);
  analogWrite(selectedLedPin, brightness);
  
  delay(500);
  analogWrite(rLedPin, 0);
  analogWrite(gLedPin, 0);
  analogWrite(bLedPin, 0);
  
}
