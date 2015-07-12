#include "sys.h"
#include "led.h"
#include "btn.h"
#include "stm32f3xx.h"

static void setup_clock()
{
    // set pll, 8M * 12 / 2 = 48M
    RCC->CFGR &= ~RCC_CFGR_PLLMUL;
    RCC->CFGR |=  RCC_CFGR_PLLMUL12; // set any value > 12 will cause problem

    // set HSI/2 as pll source
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2;
    // enable pll
    RCC->CR |= RCC_CR_PLLON;
    // set pll as system clock
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // wait clock switch, then call SystemCoreClockUpdate
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    SystemCoreClockUpdate();
}


void sys_init()
{
    setup_clock();
    btn_init();
    led_init();
}
