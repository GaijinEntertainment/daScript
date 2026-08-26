// Tier B: the same nano runtime, now with the das heap in play.
//
// `options heap_size_hint = 65536` in heap_demo.das is what the context asks
// its allocator for up front. Everything the script allocates - the array, the
// table, the `new Sample` - comes out of that.

#include "daScript/nano_print.h"
#include "heap_demo.das.h"

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

    heap_demo::Standalone ctx;

    // 0..9 squared
    expect_int("sum_range(10)", ctx.sum_range(10), 285);
    expect_int("sum_range(0)", ctx.sum_range(0), 0);

    // 20 values over 7 buckets: buckets 0..5 get 3, bucket 6 gets 2
    expect_int("histogram_peak(20)", ctx.histogram_peak(20), 3);

    expect_int("alloc_and_free(8)", int(ctx.alloc_and_free(8)), 4);

    // The heap is reused, not grown: running the same work again must not
    // depend on how much ran before it.
    for ( int i = 0; i != 100; ++i ) {
        ctx.alloc_and_free(i);
    }
    expect_int("sum_range(10) after 100 alloc/free", ctx.sum_range(10), 285);

    printf(failures ? "02_heap: %d failure(s)\n" : "02_heap: ok\n", failures);
    return failures ? 1 : 0;
}
