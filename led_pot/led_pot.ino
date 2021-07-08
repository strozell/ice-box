// Version 1.0

#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define MLX_I2C_ADDR 0x5A
int potentiometerPin = A1; // pot
int potValue = 0;
int tempF = 0;
int maxBrightness = 255;
int threshold;
int rLedPin = 5; //pwm
int gLedPin = 3; //pwm
int bLedPin = 6; //pwm
int selectedLedPin;
int delta;
int redBrightness;
int blueBrightness;

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
  threshold = potValue/10 + 30; // want 30 to 130 F from 0-1023 (80+/-50
  Serial.print("Threshold = "); Serial.println(threshold);

  // get new object and ambient temperature
  tempF = mlx.readObjectTempF();
  //Serial.print("Object = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F"); 
  //Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); Serial.println("*F"); 
  Serial.print("tempF = "); Serial.println(tempF);

  // red/blue scale from threshold
  delta = tempF - threshold; //temp - (30-130). If positive, skew red. If zero, purple. If negative, skew blue.
  Serial.print("delta = "); Serial.println(delta);
  if (delta > 0) {
    redBrightness = maxBrightness;
    blueBrightness = maxBrightness-(delta*4)-15;
  } else {
    blueBrightness = maxBrightness;
    redBrightness = maxBrightness+(delta*4)-15;
  }
  Serial.print("redBrightness = "); Serial.println(redBrightness);
  Serial.print("blueBrightness = "); Serial.println(blueBrightness);
  
  analogWrite(rLedPin, redBrightness);
  analogWrite(bLedPin, blueBrightness);
  
  delay(500);

  
}
