#include "led.h"
#include "int_types.h"
#include "stm32f3xx.h"

/*

  | id | name | pin  |
  |  0 | LD3  | PE9  |
  |  1 | LD4  | PE8  |
  |  2 | LD6  | PE15 |
  |  3 | LD8  | PE14 |
  |  4 | LD10 | PE13 |
  |  5 | LD9  | PE12 |
  |  6 | LD7  | PE11 |
  |  7 | LD5  | PE10 |

 */

static u16 led_tbl[LED_NUM] = {
    9, 8, 15, 14, 13, 12, 11, 10
};

void led_init()
{
    int i;

    // enable clock for GPIOE
    RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
    // set led bit of GPIOE to output mode
    for (i = 0; i < LED_NUM; ++i) {
        GPIOE->MODER |= (0x01 << (2 * led_tbl[i]));
        GPIOE->OSPEEDR |= (0x3 << (2 * led_tbl[i])); // high speed
    }
    // turn led off
    for (i = 0; i < LED_NUM; ++i) {
        GPIOE->ODR |= (1 << led_tbl[i]);
    }
}

void led_set(u8 id, bool val)
{
    if (id >= LED_NUM) return;

    if (val) GPIOE->ODR |= (1 << led_tbl[id]);
    else GPIOE->ODR &= ~(1 << led_tbl[id]);
}

bool led_get(u8 id)
{
    if (id >= LED_NUM) return 0;
    return ((GPIOE->ODR & (1 << led_tbl[id])) != 0);
}

void led_set_all(u8 val)
{
    u32 res = GPIOE->ODR;
    int i;
    for (i = 0; i < LED_NUM; ++i) {
        res &= ~(1 << led_tbl[i]);
        if (val & (1 << i)) res |= (1 << led_tbl[i]);
    }

    GPIOE->ODR = res;
}
