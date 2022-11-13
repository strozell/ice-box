#ifndef ICE_BOX_H
#define ICE_BOX_H

// BOARD MODULE
#define PIN_FAN_TACH (2)
#define PIN_FAN_PWM (3)
#define PIN_FAN_POWER (4)
#define PIN_POT_FRONT (A1)
#define PIN_SWITCH  (5)

// FAN MODULE
#define FAN_TACH_DEBOUNCE (20U) // 0 is fine for most fans, crappy fans may require 10 or 20 to filter out noise
#define FAN_TACH_STUCK_THRESHOLD (500U) // if no interrupts were received for 500ms, consider the fan as stuck and report 0 RPM
#define FAN_TACH_RPM_INVALID  (0UL)
#define FAN_PWM_OCR2A_TOP (79)
#define FAN_PWM_DC_OFF (0U)
#define FAN_PWM_DC_ON_MIN (20U)
#define FAN_PWM_DC_ON_MAX (100U)

// TEMPERATURE MODULE
#define TEMP_DC_OFF_F (77)
#define TEMP_DC_ON_MAX_F (89)
#endif //ICE_BOX_H
