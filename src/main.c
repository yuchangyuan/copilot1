#include <stdlib.h>
#include "int_types.h"

#include "sys.h"
#include "led.h"
#include "btn.h"


int main(int argc, char **argv)
{
    int i;

    sys_init();

  L:
    for (i = 0; i < 8; ++i) {
        led_set_all(1 << i);
        while (!btn_status());
        while (btn_status());
    }
    goto L;

    return 0;
}
