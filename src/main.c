#include <stdlib.h>
#include "int_types.h"

#include "sys.h"
#include "led.h"
#include "led_pwm.h"
#include "btn.h"
#include "i2c.h"
#include "lsm303dlhc.h"

static void wait_btn()
{
    while (!btn_status());
    while (btn_status());
}

/*
typedef struct _xyz_t
{
    s16 x;
    s16 y;
    s16 z;
    u8 r;
} xyz_t;

    //i2c_init();
    //lsm303dlhc_init();
    //xyz.r = lsm303dlhc_mfs_read(&xyz.x, &xyz.y, &xyz.z);
    //xyz.r = lsm303dlhc_la_read(&xyz.x, &xyz.y, &xyz.z);
*/

int main(int argc, char **argv)
{
    int i = 0;
    bool dir = true;
    int j;

    sys_init();

  L:
    if (btn_status()) {
        while (btn_status());
        dir = !dir;
    }

    i += dir ? 1 : -1;
    i = (i + LED_NUM) % LED_NUM;

    if (btn_status()) {
        while (btn_status());
        dir = !dir;
    }

    led_pwm_set(i, 1.0);

    for (j = 0; j < 30; ++j) {
        int id;
        for (id = 0; id < 8; ++id) {
            float level = led_pwm_get(id) - 0.005;
            led_pwm_set(id, level);
        }
        sys_wait(5);
    }

    goto L;
}
