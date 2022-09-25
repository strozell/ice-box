#ifndef ICE_BOX_H
#define ICE_BOX_H

// BOARD MODULE
#define PIN_FAN_TACH (2)
#define PIN_FAN_PWM (3)
#define PIN_FAN_POWER (4)

// FAN MODULE
#define FAN_PWM_DC_MIN (0.2f)
#define FAN_TACH_DEBOUNCE (20) //0 is fine for most fans, crappy fans may require 10 or 20 to filter out noise
#define FAN_TACH_STUCK_THRESHOLD (500) // if no interrupts were received for 500ms, consider the fan as stuck and report 0 RPM


#endif //ICE_BOX_H
