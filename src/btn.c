#include "btn.h"
#include "int_types.h"
#include "stm32f3xx.h"

// user button is PA0

void btn_init()
{
    // enable GPIOA
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // set PA0 mode
    GPIOA->MODER &= ~0x3; // clear bit0-1
}

bool btn_status()
{
    return ((GPIOA->IDR & 1) != 0);
}
