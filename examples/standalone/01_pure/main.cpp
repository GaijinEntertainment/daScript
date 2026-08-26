// Tier A: a C++ program whose whole daslang dependency is libDaScriptNano.
//
// There is no daScript.h here, no Module, no compiler - just the header the
// standalone emitter generated. The context is a plain C++ object: construct
// it, call its methods, let it go out of scope.

#include "daScript/nano_print.h"
#include "pure_math.das.h"

#include <stdio.h>

using namespace das;

static int failures = 0;

static void expect_int ( const char * what, int have, int want ) {
    if ( have != want ) {
        printf("%s = %d, expected %d\n", what, have, want);
        failures ++;
    }
}

static void expect_float ( const char * what, float have, float want ) {
    const float d = have > want ? have - want : want - have;
    if ( d > 1e-5f ) {
        printf("%s = %f, expected %f\n", what, double(have), double(want));
        failures ++;
    }
}

static void to_console ( const char * text ) {
    fputs(text, stdout);
}

int main () {
    // On a board this is where printk or a UART write goes. Setting it before
    // the context exists means even a panic during construction is visible.
    das_nano_set_print(&to_console);

    pure_math::Standalone ctx;

    pure_math::Vec3 a; a.x = 1.0f; a.y = 2.0f; a.z = 3.0f;
    pure_math::Vec3 b; b.x = 4.0f; b.y = 5.0f; b.z = 6.0f;

    expect_float("dot(a,b)", ctx.dot(a, b), 32.0f);

    pure_math::Vec3 s = ctx.scale(a, 2.0f);
    expect_float("scale(a,2).x", s.x, 2.0f);
    expect_float("scale(a,2).z", s.z, 6.0f);

    expect_float("component(a,y)", ctx.component(a, pure_math::Axis::y), 2.0f);
    expect_float("weighted_sum(a)", ctx.weighted_sum(a), 1.0f*0.25f + 2.0f*0.5f + 3.0f*0.25f);

    expect_int("collatz_steps(27)", ctx.collatz_steps(27), 111);
    expect_int("collatz_steps(1)", ctx.collatz_steps(1), 0);

    printf(failures ? "01_pure: %d failure(s)\n" : "01_pure: ok\n", failures);
    return failures ? 1 : 0;
}
