#include "standalone_init_fixture.das.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void expect ( const char * what, int have, int want ) {
    if ( have != want ) {
        printf("%s = %d, expected %d\n", what, have, want);
        failures ++;
    }
}

int main ( void ) {
    standalone_init_fixture_ctx * ctx = standalone_init_fixture_create();
    if ( !ctx ) {
        printf("create failed: %s\n", standalone_init_fixture_last_error(NULL));
        return 1;
    }

    expect("get_first()", standalone_init_fixture_get_first(ctx), 31);
    expect("get_second()", standalone_init_fixture_get_second(ctx), 2);
    expect("get_init_fn_stamp()", standalone_init_fixture_get_init_fn_stamp(ctx), 3);
    expect("get_reads_forward()", standalone_init_fixture_get_reads_forward(ctx), 100);
    expect("get_later()", standalone_init_fixture_get_later(ctx), 7);
    expect("get_shared_total()", standalone_init_fixture_get_shared_total(ctx), 6);

    standalone_init_fixture_Pair made;
    memset(&made, 0xAA, sizeof(made));
    standalone_init_fixture_make_pair(ctx, 3, 4, &made);
    expect("make_pair(3,4).a", made.a, 3);
    expect("make_pair(3,4).b", made.b, 4);
    expect("pair_sum(make_pair(3,4))", standalone_init_fixture_pair_sum(ctx, &made), 7);

    standalone_init_fixture_Pair mine;
    mine.a = 40;
    mine.b = 2;
    expect("pair_sum(a Pair this C host built)", standalone_init_fixture_pair_sum(ctx, &mine), 42);

    expect("flip(on)", standalone_init_fixture_flip(ctx, standalone_init_fixture_Mode_on),
           standalone_init_fixture_Mode_off);
    expect("flip(off)", standalone_init_fixture_flip(ctx, standalone_init_fixture_Mode_off),
           standalone_init_fixture_Mode_on);

    standalone_init_fixture_Outer nested;
    nested.inner.weight = 9;
    nested.tag = 5;
    expect("outer_weight(a struct holding a struct)",
           standalone_init_fixture_outer_weight(ctx, &nested), 14);

    expect("head_value() on a null safe-navigation", standalone_init_fixture_head_value(ctx), 41);

    expect("renamed_sum(20,22) through the [export_c(name=...)] symbol",
           standalone_init_fixture_renamed_sum(ctx, 20, 22), 42);

    if ( standalone_init_fixture_last_error(ctx) ) {
        printf("a call raised: %s\n", standalone_init_fixture_last_error(ctx));
        failures ++;
    }

    standalone_init_fixture_destroy(ctx);
    printf(failures ? "standalone_capi: %d failure(s)\n" : "standalone_capi: ok\n", failures);
    return failures ? 1 : 0;
}
