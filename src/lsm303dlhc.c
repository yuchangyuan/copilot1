#include "lsm303dlhc.h"

#include "i2c.h"

#define ADDR_LA  (0x19)
#define ADDR_MFS (0x1E)

void lsm303dlhc_init()
{
    u8 wbuf[4];

    // config mfs
    wbuf[0] = 0x00 | 0x80; // reg addr 0
    wbuf[1] = 0x14; // disbale temp, d2-d0 = 'b101, 30Hz
    wbuf[2] = 0x20; // gn2~gn0 = 'b001
    wbuf[3] = 0x00; // md1~md0 = 'b00, continous conversion mode

    i2c_write_read(ADDR_MFS, 4, wbuf, 0, 0);

    // config la
    wbuf[0] = 0x20 | 0x80; // write begin from add 0x20
    wbuf[1] = 0x47; // ctrl_reg1_a, ORD:0100(50Hz), LPen:0, ZYX: 111
    i2c_write_read(ADDR_LA, 2, wbuf, 0, 0);

    wbuf[0] = 0x23 | 0x80; // ctrl_reg4_a
    wbuf[1] = 0x58; // bdu:0, ble:1, fs: 01, +-4g, hr:1, 0,0, sim: 0
    i2c_write_read(ADDR_LA, 2, wbuf, 0, 0);

    /*
    wbuf[0] = 0x21 | 0x80;
    wbuf[1] = 0x90; // HPM:10(normal), HPFC:01, FDS:0, 000
    i2c_write_read(ADDR_LA, 2, wbuf, 0, 0);
    */
}


u8 lsm303dlhc_mfs_read(s16 *xm, s16 *ym, s16 *zm)
{
    u8 wbuf[1];
    u8 rbuf[7];

    wbuf[0] = 3 | 0x80;

    i2c_write_read(ADDR_MFS, 1, wbuf, 7, rbuf);

    *xm = ((u16)rbuf[0] << 8) | rbuf[1];
    *ym = ((u16)rbuf[2] << 8) | rbuf[3];
    *zm = ((u16)rbuf[4] << 8) | rbuf[5];

    return rbuf[6];
}

u8 lsm303dlhc_la_read(s16 *xa, s16 *ya, s16 *za)
{
    u8 wbuf[1];
    u8 rbuf[7];

    wbuf[0] = 0x27 | 0x80;

    i2c_write_read(ADDR_LA, 1, wbuf, 6, rbuf);

    *xa = ((u16)rbuf[1] << 8) | rbuf[2];
    *ya = ((u16)rbuf[3] << 8) | rbuf[4];
    *za = ((u16)rbuf[5] << 8) | rbuf[6];

    return rbuf[0];
}
