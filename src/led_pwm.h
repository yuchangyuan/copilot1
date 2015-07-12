#ifndef LED_PWM_H
#define LED_PWM_H

#include "int_types.h"

void led_pwm_init();
void led_pwm_set(u8 id, float val);
float led_pwm_get(u8 id);

#endif
