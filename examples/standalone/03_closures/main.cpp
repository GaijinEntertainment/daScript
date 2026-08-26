// Tier C: lambdas, function pointers and generators on nano.
//
// Each of these looks a function up at runtime, through the tables the
// generated constructor fills in. Nothing about that path is compiled away.

#include "daScript/nano_print.h"
#include "closures.das.h"

#include <stdio.h>

using namespace das;

static int failures = 0;

static void expect_int ( const char * what, int have, int want ) {
    if ( have != want ) {
        printf("%s = %d, expected %d\n", what, have, want);
        failures ++;
    }
}

static void to_console ( const char * text ) {
    fputs(text, stdout);
}

int main () {
    das_nano_set_print(&to_console);

    closures::Standalone ctx;

    expect_int("apply_twice(10)", ctx.apply_twice(10), 16);
    expect_int("call_through_pointer(21)", ctx.call_through_pointer(21), 42);
    expect_int("sum_squares(5)", ctx.sum_squares(5), 30);
    expect_int("sum_squares(0)", ctx.sum_squares(0), 0);
    expect_int("count_up_to(7)", ctx.count_up_to(7), 7);

    // Capture frames and generator state are heap allocations with finalizers.
    // Repeating the work must not leave any of them behind.
    for ( int i = 0; i != 200; ++i ) {
        ctx.sum_squares(5);
        ctx.count_up_to(3);
    }
    expect_int("sum_squares(5) after 200 rounds", ctx.sum_squares(5), 30);

    printf(failures ? "03_closures: %d failure(s)\n" : "03_closures: ok\n", failures);
    return failures ? 1 : 0;
}
