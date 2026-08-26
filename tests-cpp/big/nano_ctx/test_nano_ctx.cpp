// All four standalone tiers, in one program, on the minimal runtime.
//
// The examples each run their own tier; this test links all four contexts into
// a single binary, which is the thing neither the examples nor the full-runtime
// standalone test cover: four generated contexts sharing one AOT registry.

#include "daScript/nano_print.h"

#include "pure_math.das.h"
#include "heap_demo.das.h"
#include "closures.das.h"
#include "blinker.das.h"

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

// What the script prints, captured instead of printed, so the test can check it.
static string g_captured;

static void capture_print ( const char * text ) {
    g_captured += text;
}

int main () {
    das_nano_set_print(&capture_print);

    {   // tier A - POD compute, no das heap
        pure_math::Standalone ctx;
        pure_math::Vec3 a; a.x = 1.0f; a.y = 2.0f; a.z = 3.0f;
        pure_math::Vec3 b; b.x = 4.0f; b.y = 5.0f; b.z = 6.0f;
        expect_float("dot", ctx.dot(a, b), 32.0f);
        expect_float("scale.y", ctx.scale(a, 3.0f).y, 6.0f);
        expect_float("component(z)", ctx.component(a, pure_math::Axis::z), 3.0f);
        expect_float("weighted_sum", ctx.weighted_sum(a), 2.0f);
        expect_int("collatz_steps(27)", ctx.collatz_steps(27), 111);
        // `options stack = 4096` is honored exactly, plus the headroom the
        // global initializers need - not rounded up to the 16k default.
        expect_int("explicit stack is honored", ctx.stack.size() >= 4096 && ctx.stack.size() < 16384 ? 1 : 0, 1);
    }

    {   // tier B - the das heap
        heap_demo::Standalone ctx;
        expect_int("sum_range(10)", ctx.sum_range(10), 285);
        expect_int("histogram_peak(20)", ctx.histogram_peak(20), 3);
        expect_int("alloc_and_free(8)", int(ctx.alloc_and_free(8)), 4);
        for ( int i = 0; i != 100; ++i ) ctx.alloc_and_free(i);
        expect_int("sum_range after churn", ctx.sum_range(10), 285);
    }

    {   // tier C - lambdas, function pointers, generators
        closures::Standalone ctx;
        expect_int("apply_twice(10)", ctx.apply_twice(10), 16);
        expect_int("call_through_pointer(21)", ctx.call_through_pointer(21), 42);
        expect_int("sum_squares(5)", ctx.sum_squares(5), 30);
        expect_int("count_up_to(7)", ctx.count_up_to(7), 7);
        for ( int i = 0; i != 200; ++i ) { ctx.sum_squares(5); ctx.count_up_to(3); }
        expect_int("sum_squares after churn", ctx.sum_squares(5), 30);
    }

    {   // output - `print` reaches the embedder's sink and nowhere else
        blinker::Standalone ctx;
        expect_int("lamp_pattern(3)", ctx.lamp_pattern(3), 8);
        expect_int("lamp_pattern(4)", ctx.lamp_pattern(4), 4);
        g_captured.clear();
        ctx.announce(2, ctx.lamp_pattern(2));
        expect_int("print reached the sink", g_captured == "tick 2: lamps 4\n" ? 1 : 0, 1);
        if ( g_captured != "tick 2: lamps 4\n" ) {
            printf("  captured: \"%s\"\n", g_captured.c_str());
        }
    }

    {   // a handled TypeInfo an embedder built by hand answers "unknown" rather
        // than dereferencing a null AnnotationInfo, matching the full runtime
        TypeInfo handled = {};
        handled.type = Type::tHandle;
        handled.annotation_info = nullptr;
        expect_int("getAnnotation on a null annotation_info", handled.getAnnotation() == nullptr ? 1 : 0, 1);
    }

    printf(failures ? "nano_ctx: %d failure(s)\n" : "nano_ctx: ok\n", failures);
    return failures ? 1 : 0;
}
