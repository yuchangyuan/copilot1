#include "i2c.h"

#include "stm32f3xx.h"
#include "int_types.h"

void i2c_init()
{
    // enable GPIOB
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // map PB6, PB7 to I2C1
    GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
    GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1; // 10, AF
    GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7; // 1 open drain
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);
    GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0); // 01, PU
    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL6 | GPIO_AFRL_AFRL7);
    GPIOB->AFR[0] |= (4 << (6*4)) | (4 << (7*4)); // AF4 for 6,7

    // enable I2C
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // init i2c

    // 1. clear PE
    I2C1->CR1 &= ~I2C_CR1_PE;
    // 2. config anfoff & dnf
    I2C1->CR1 &= ~I2C_CR1_ANFOFF;
    I2C1->CR1 &= ~I2C_CR1_DFN;
    // 3. setup PRESC, SCLH, SCLL
    I2C1->TIMINGR &= ~(I2C_TIMINGR_SCLL |
                       I2C_TIMINGR_SCLH |
                       I2C_TIMINGR_PRESC);
    I2C1->TIMINGR |=
        (0x9 << 0) | // SCLL
        (0x3 << 8) | // SCLH
        (5 << 28);   // PRESC
    // 4. config nostretch
    I2C1->CR1 |= I2C_CR1_NOSTRETCH;
    I2C1->CR1 |= I2C_CR1_PE;
}

void i2c_write_read(u8 addr, u8 wlen, u8 *wbuf, u8 rlen, u8 *rbuf)
{
    int i;

    // configure master
    I2C1->CR2 &= ~I2C_CR2_ADD10; // 7bit
    I2C1->CR2 &= ~I2C_CR2_SADD;
    I2C1->CR2 |= addr << 1; // addr, SADD[7:1]
    I2C1->CR1 &= ~I2C_CR2_RELOAD; // not auto reload

    if (wlen == 0) goto READ;

    // write
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    if (rlen == 0) {
        I2C1->CR2 |= I2C_CR2_AUTOEND; // auto stop
    }
    else {
        I2C1->CR2 &= ~I2C_CR2_AUTOEND; // soft stop
    }
    I2C1->CR2 &= ~I2C_CR2_NBYTES;
    I2C1->CR2 |= ((u32)wlen << 16);

    I2C1->CR2 |= I2C_CR2_START;

    for (i = 0; i < wlen; ++i) {
        while (!(I2C1->ISR & I2C_ISR_TXIS)); // wait txis
        I2C1->TXDR = wbuf[i];
    }

    if (rlen == 0) goto DONE;

    // read
  READ:
    I2C1->CR2 |= I2C_CR2_RD_WRN;
    I2C1->CR2 |= I2C_CR2_AUTOEND;
    I2C1->CR2 &= ~I2C_CR2_NBYTES;
    I2C1->CR2 |= ((u32)rlen << 16);

    I2C1->CR2 |= I2C_CR2_START;

    for (i = 0; i < rlen; ++i) {
        while (!(I2C1->ISR & I2C_ISR_RXNE)); // wait txis
        rbuf[i] = I2C1->RXDR;
    }

  DONE:
    return;
}
