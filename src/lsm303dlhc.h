#ifndef LSM303DLHC_H
#define LSM303DLHC_H

#include "int_types.h"

void lsm303dlhc_init();
u8 lsm303dlhc_mfs_read(s16 *xm, s16 *ym, s16 *zm);
u8 lsm303dlhc_la_read(s16 *xa, s16 *ya, s16 *za);

#endif
