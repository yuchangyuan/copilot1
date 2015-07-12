#ifndef LED_H
#define LED_H

#include "int_types.h"

#define LED_NUM 8

void led_init();
void led_set(u8 id, bool val);
bool led_get(u8 id);
void led_set_all(u8 val);

#endif
