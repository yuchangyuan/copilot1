#include <stdlib.h>
#include "int_types.h"

#include "sys.h"
#include "led.h"
#include "led_pwm.h"
#include "btn.h"
#include "lsm303dlhc.h"

float acc_x, acc_y, acc_z;

static void update_acc()
{
    s16 x, y, z;
    lsm303dlhc_la_read(&x, &y, &z);
    acc_x = x;
    acc_y = y;
    acc_z = z;
}



int main(int argc, char **argv)
{
    sys_init();
    i2c_init();
    lsm303dlhc_init();

  L:
    update_acc();
    step();
    sys_step();

    goto L;
}

