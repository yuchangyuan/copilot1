#include "sys.h"
#include "led.h"
#include "btn.h"
#include "led_pwm.h"
#include "stm32f3xx.h"

static vu32 tick_cnt = 0;

void SysTick_Handler()
{
    tick_cnt++;
    //led_set_all(TIM2->CNT);
}

void sys_wait_next_tick()
{
    u32 tick = tick_cnt;
    while (tick_cnt != tick);
}

// wait for next N ms
void sys_wait(int n)
{
    u32 tick = tick_cnt;

    while ((s32)(tick_cnt - tick) < n);
}

void sys_step()
{
    static u32 cnt = 0;
    cnt++;
    while ((s32)(cnt - tick_cnt) >= 0);
}

static void setup_clock()
{
    // set pll, 8M * 10 / 2 = 40M
    RCC->CFGR &= ~RCC_CFGR_PLLMUL;
    // set any value > 12 will cause problem
    // and set 12 is not stable
    RCC->CFGR |=  RCC_CFGR_PLLMUL10;

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

    SysTick_Config(SystemCoreClock / 1000);

    btn_init();
    led_init();
    led_pwm_init();
}
