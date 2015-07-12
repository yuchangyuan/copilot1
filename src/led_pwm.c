#include "led_pwm.h"
#include "led.h"
#include "stm32f3xx.h"
#include <math.h>
/*
  - use TIM2 ch1 ~ ch4 for id 0 ~ 3
  - use TIM3 ch1 ~ ch4 for id 4 ~ 7
 */

u16 _level[LED_NUM] = {0};

#define LEVEL_MAX 10000

// inturrpet handle TIM1
static inline void tim_handle(int base, TIM_TypeDef *tim)
{
    u16 sr = tim->SR &
        (TIM_SR_UIF   |
         TIM_SR_CC1IF |
         TIM_SR_CC2IF |
         TIM_SR_CC3IF |
         TIM_SR_CC4IF);

    if (sr & TIM_SR_UIF) {
        led_set(base + 0, true);
        led_set(base + 1, true);
        led_set(base + 2, true);
        led_set(base + 3, true);
    }

    if (sr & TIM_SR_CC1IF) {
        led_set(base + 0, false);
        tim->CCR1 = _level[base+0];
    }

    if (sr & TIM_SR_CC2IF) {
        led_set(base + 1, false);
        tim->CCR2 = _level[base+1];
    }

    if (sr & TIM_SR_CC3IF) {
        led_set(base + 2, false);
        tim->CCR3 = _level[base+2];
    }

    if (sr & TIM_SR_CC4IF) {
        led_set(base + 3, false);
        tim->CCR4 = _level[base+3];
    }
        /*
        */


    tim->SR &= ~sr;
}

void TIM2_IRQHandler()
{
    tim_handle(0, TIM2);
}

void TIM3_IRQHandler()
{
    tim_handle(4, TIM3);
}

static void init_tim(TIM_TypeDef *tim, u16 cnt0)
{
    // 1. up, auto reload
    // 2. trigger event when

    // set cnt clk to 10KHz
    tim->PSC = (SystemCoreClock / 1000 / 300);

    tim->CR1 &= ~TIM_CR1_CKD; // use clk_int
    tim->CR1 &= ~TIM_CR1_CMS; // use edge align mode
    tim->CR1 &= ~TIM_CR1_DIR; // use up count mode
    tim->CR1 &= ~TIM_CR1_OPM; // continuous mode
    tim->CR1 |= TIM_CR1_URS;  // only overflow generate event
    tim->CR1 &= ~TIM_CR1_UDIS; // enable event generate

    // reload value, set to 10K
    tim->ARR = LEVEL_MAX;

    // reset compare reg
    tim->CCR1 = 0;
    tim->CCR2 = 0;
    tim->CCR3 = 0;
    tim->CCR4 = 0;
    tim->CNT = cnt0;

    // output mode
    tim->CCMR1 &= ~TIM_CCMR1_CC1S & ~TIM_CCMR1_CC2S;
    tim->CCMR2 &= ~TIM_CCMR2_CC3S & ~TIM_CCMR2_CC4S;

    // enalbe update & 4 compare interrupt
    tim->DIER =
        TIM_DIER_UIE   |
        TIM_DIER_CC1IE |
        TIM_DIER_CC2IE |
        TIM_DIER_CC3IE |
        TIM_DIER_CC4IE;

    tim->CR1 |= TIM_CR1_CEN; // enable timer
}

void led_pwm_init()
{
    // set APB1 clock to HCLK
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;

    // enalbe APB1 for TIM2 & TIM3
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // setup TIM2 & TIM3
    init_tim(TIM2, LEVEL_MAX / 2);
    init_tim(TIM3, LEVEL_MAX / 4);

    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(TIM3_IRQn);
}

float _val[LED_NUM] = {0};

void led_pwm_set(u8 id, float val)
{
    u16 level;
    TIM_TypeDef *tim = (id < 4) ? TIM2 : TIM3;

    if (id > LED_NUM) return;

    _val[id] = val;

    // compute u16 level from val
    level = (u16)(pow(val, 2.2) / 2.2 * LEVEL_MAX);

    _level[id] = level;
}

float led_pwm_get(u8 id)
{
    if (id >= LED_NUM) return 0;
    return _val[id];
}
