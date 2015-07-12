#include "stm32f3xx.h"
#include <stdlib.h>
#include "int_types.h"

#include  "led.h"
#include "stm32f3xx.h"

int main(int argc, char **argv)
{
    int i;

    led_init();

  L:
    for (i = 0; i < 8; ++i) {
        int j;
        for (j = 0; j < 1000; ++j) {
            led_set_all(1 << i);
        }
    }
    goto L;

    return 0;
}
