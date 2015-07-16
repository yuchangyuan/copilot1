#ifndef I2C_H
#define I2C_H

#include "int_types.h"

void i2c_init();
void i2c_write_read(u8 addr, u8 wlen, u8 *wbuf, u8 rlen, u8 *rbuf);

#endif
