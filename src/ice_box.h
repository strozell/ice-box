#ifndef ICE_BOX_H
#define ICE_BOX_H

// BOARD MODULE
int Pin_potentiometer = A1; // pot
int Pin_led_r = 5; //pwm
int Pin_led_g = 3; //pwm
int Pin_led_b = 6; //pwm
int Pin_limit_switch = 12;


// LIMIT SWITCH MODULE
typedef enum _limit_switch_state {
  LIMIT_SWITCH_OPEN=0,
  LIMIT_SWITCH_CLOSED
} limit_switch_state_t;

limit_switch_state_t get_limit_switch_state();


// POTENTIOMETER MODULE
int threshold;
int get_potentiometer_threshold();


// LED MODULE
int maxBrightness = 255;
int selectedLedPin;
int delta;
int redBrightness;
int blueBrightness;
void led_set_all_off();
void led_set_rgb(int r_val, int g_val, int b_val);


// TEMPERATURE MODULE
int tempF = 0;


#endif //ICE_BOX_H
