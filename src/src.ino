// Version 1.0

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "ice_box.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define MLX_I2C_ADDR 0x5A

void setup() {
  pinMode(Pin_potentiometer, INPUT);
  pinMode(Pin_limit_switch, INPUT);
  pinMode(Pin_led_r, OUTPUT);
  pinMode(Pin_led_g, OUTPUT);
  pinMode(Pin_led_b, OUTPUT);
  Serial.begin(9600);
  mlx.begin();
}

void loop() {
  while (get_limit_switch_state() == LIMIT_SWITCH_CLOSED) {

    threshold = get_potentiometer_threshold();

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
      blueBrightness = maxBrightness - (delta * 4) - 15;
    } else {
      blueBrightness = maxBrightness;
      redBrightness = maxBrightness + (delta * 4) - 15;
    }
    Serial.print("redBrightness = "); Serial.println(redBrightness);
    Serial.print("blueBrightness = "); Serial.println(blueBrightness);

    led_set_rgb(redBrightness, 0, blueBrightness);

    delay(50);
  }

  led_set_all_off();
}


limit_switch_state_t get_limit_switch_state() {
  if ( digitalRead(Pin_limit_switch) == LOW ) {
    return LIMIT_SWITCH_CLOSED;
  } else {
    return LIMIT_SWITCH_OPEN;
  }
}

int get_potentiometer_threshold() {
  int potValue = analogRead(Pin_potentiometer);
  int tempThreshold = potValue / 10 + 30; // want 30 to 130 F from 0-1023 (80+/-50
  Serial.print("Threshold = "); Serial.println(threshold);
  return tempThreshold;
}

void led_set_all_off() {
  led_set_rgb(0,0,0);
}

void led_set_rgb(int r_val, int g_val, int b_val) {
  analogWrite(Pin_led_r, r_val);
  analogWrite(Pin_led_g, g_val);
  analogWrite(Pin_led_b, b_val);
}
