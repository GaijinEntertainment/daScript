// C owns the hardware, daslang owns the decision.
//
// This is the tier a microcontroller program lives in. The "GPIO register" here
// is a variable, but nothing else is pretend: the loop is C, the pattern comes
// from a compiled daslang function, and everything the script prints leaves
// through the one sink nano asks the embedder for.

#include "daScript/nano_print.h"
#include "blinker.das.h"

#include <stdio.h>

using namespace das;

// Stand-in for a memory-mapped output register.
static unsigned g_gpio_out = 0;

static void gpio_write ( unsigned bits ) {
    g_gpio_out = bits;
}

// On a board this is printk, HAL_UART_Transmit, or an append to a ring buffer.
static void board_print ( const char * text ) {
    fputs(text, stdout);
}

int main () {
    das_nano_set_print(&board_print);

    blinker::Standalone ctx;

    // One full sweep of the scanner: 1,2,4,8,4,2 then back to 1.
    static const unsigned expected[] = { 1, 2, 4, 8, 4, 2, 1, 2 };
    int failures = 0;

    for ( int tick = 0; tick != 8; ++tick ) {
        const unsigned pattern = unsigned(ctx.lamp_pattern(tick));
        gpio_write(pattern);
        ctx.announce(tick, int(pattern));
        if ( g_gpio_out != expected[tick] ) {
            printf("tick %d wrote %u, expected %u\n", tick, g_gpio_out, expected[tick]);
            failures ++;
        }
    }

    printf(failures ? "04_c_binding: %d failure(s)\n" : "04_c_binding: ok\n", failures);
    return failures ? 1 : 0;
}
