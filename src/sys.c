#include "sys.h"
#include "led.h"
#include "btn.h"
#include "stm32f3xx.h"

/*
static void setup_clock()
{
    // set pll x 8
    RCC->CFGR &= ~(0xf << 18);
    RCC->CFGR |=  (0xe << 18); // 0b1110 -> x16
    // set HSI/2 as pll source
    RCC->CFGR &= ~(0x3 << 15);
    // enable pll
    RCC->CR |= (1 << 24);
    // set pll as system clock
    RCC->CFGR &= ~(0x3 << 0);
    RCC->CFGR |= 0x2;

    SystemCoreClockUpdate();
}
*/

void sys_init()
{
    // setup_clock();
    btn_init();
    led_init();
}
