// A thermistor read through an ADC, with the calibration already solved.
//
// thermistor.csv holds thirteen bench measurements. None of them are in this
// program. While thermometer.das compiled, a daslang macro read that file,
// sorted it, inverted the curve and resampled it onto an even 64-point grid -
// so what links here is the grid, and the only arithmetic left at run time is
// one index and one lerp.
//
// Nothing in the linked image can parse a CSV, and it never needs to.

#include "daScript/nano_print.h"
#include "thermometer.das.h"

#include <stdio.h>

using namespace das;

// On a board this is printk, HAL_UART_Transmit, or an append to a ring buffer.
static void board_print ( const char * text ) {
    fputs(text, stdout);
}

// Stand-in for a 12-bit ADC channel with a thermistor on it.
static int read_adc ( int sample ) {
    static const int readings[] = { 200, 267, 613, 1081, 1825, 2738, 3495, 3740, 4000 };
    return readings[sample % (int)(sizeof(readings)/sizeof(readings[0]))];
}

int main () {
    das_nano_set_print(&board_print);

    thermometer::Standalone ctx;

    const int lo = ctx.adc_range_lo();
    const int hi = ctx.adc_range_hi();
    printf("baked table: %d points, calibrated over ADC %d..%d\n", ctx.curve_size(), lo, hi);

    // Both ends are clamped on purpose: a reading outside the calibrated range
    // is a real thing on a cold morning, and the table has to answer anyway.
    for ( int i = 0; i < 9; ++i ) {
        const int adc = read_adc(i);
        const int centi = ctx.temperature_centi(adc);
        const char * note = (adc < lo || adc > hi) ? "  (outside calibration, clamped)" : "";
        printf("adc %4d -> %7.2f C%s\n", adc, centi / 100.0, note);
    }

    // The endpoints of the baked table, which should match the outermost rows
    // of the CSV: 100.00 C at the hot end, -20.00 C at the cold end.
    printf("table[0] = %.2f C, table[%d] = %.2f C\n",
        ctx.curve_point(0) / 100.0,
        ctx.curve_size() - 1,
        ctx.curve_point(ctx.curve_size() - 1) / 100.0);

    return 0;
}
