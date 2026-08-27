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
#include "thermometer.das.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

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

// nano_format.h is internal to nano/src, so the declaration is repeated rather
// than reached by include path - the linker checks it matches.
namespace das { int nano_vformat ( char * buf, size_t size, const char * fmt, va_list args ); }

static int nano_vformat_probe ( char * buf, size_t size, const char * fmt, ... ) {
    va_list args;
    va_start(args, fmt);
    int n = das::nano_vformat(buf, size, fmt, args);
    va_end(args);
    return n;
}

static void expect_fmt ( const char * what, const char * want, const char * fmt, ... ) {
    char got[128];
    va_list args;
    va_start(args, fmt);
    das::nano_vformat(got, sizeof(got), fmt, args);
    va_end(args);
    if ( strcmp(got, want) != 0 ) {
        printf("format %s = \"%s\", expected \"%s\"\n", what, got, want);
        failures ++;
    }
}

static void expect_str ( const char * what, const string & have, const char * want ) {
    if ( have != want ) {
        printf("%s = \"%s\", expected \"%s\"\n", what, have.c_str(), want);
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

    {   // nano's formatter stands in for vsnprintf on the panic path, so every
        // conversion the tree's 161 throw_error call sites use is pinned here.
        // Getting one wrong corrupts the message reporting an unrelated bug.
        expect_fmt("plain",        "no args",            "no args");
        expect_fmt("%s",           "a=[abc]",            "a=[%s]", "abc");
        expect_fmt("null %s",      "a=[(null)]",         "a=[%s]", (const char *) nullptr);
        expect_fmt("%d",           "42 and -42",         "%d and %d", 42, -42);
        // %l reads a long, %ll a long long - distinct sizes on ILP32, so the
        // argument type here must match the conversion exactly
        expect_fmt("%ld long",     "2147483647",         "%ld", (long) 2147483647L);
        expect_fmt("%ld negative", "-2147483647",        "%ld", (long) -2147483647L);
        expect_fmt("%lu long",     "4294967295",         "%lu", (unsigned long) 4294967295UL);
        expect_fmt("%lx long",     "ffffffff",           "%lx", (unsigned long) 0xffffffffUL);
        expect_fmt("%i",           "7",                  "%i", 7);
        expect_fmt("%u",           "4294967295",         "%u", 4294967295u);
        expect_fmt("%lld min",     "-9223372036854775808", "%lld", (long long) INT64_MIN);
        expect_fmt("%llu max",     "18446744073709551615", "%llu", (unsigned long long) UINT64_MAX);
        expect_fmt("%llx",         "deadbeef",           "%llx", (unsigned long long) 0xdeadbeefull);
        expect_fmt("%%",           "100%",               "100%%");
        expect_fmt("unknown conv", "%q",                 "%q");
        expect_fmt("mixed",        "i=3 s=x u=9",        "i=%d s=%s u=%u", 3, "x", 9u);

        // truncation always terminates and never overruns
        char small[8];
        int n = nano_vformat_probe(small, sizeof(small), "%s", "abcdefghijklmnop");
        expect_int("truncated length", n, 7);
        expect_int("truncated is terminated", small[7] == 0 ? 1 : 0, 1);
        expect_int("truncated content", strncmp(small, "abcdefg", 7) == 0 ? 1 : 0, 1);
    }

    {   // A table a macro built while the script compiled. What is checked here is
        // that the numbers survived into the linked program at all: nothing in this
        // binary can read thermistor.csv, so a wrong table cannot be recovered at
        // run time - it can only be wrong.
        thermometer::Standalone ctx;
        expect_int("baked table size", ctx.curve_size(), 64);
        expect_int("baked ADC low", ctx.adc_range_lo(), 267);
        expect_int("baked ADC high", ctx.adc_range_hi(), 3740);
        // the endpoints are the outermost rows of the CSV, exactly
        expect_int("curve[0] is 100.00C", ctx.curve_point(0), 10000);
        expect_int("curve[63] is -20.00C", ctx.curve_point(63), -2000);
        // and the curve is monotonic across every baked step
        int decreasing = 1;
        for ( int i = 1; i < ctx.curve_size(); ++i ) {
            if ( ctx.curve_point(i) > ctx.curve_point(i - 1) ) decreasing = 0;
        }
        expect_int("baked curve falls monotonically", decreasing, 1);
        // clamped past either end, interpolated between
        expect_int("below range clamps", ctx.temperature_centi(0), 10000);
        expect_int("above range clamps", ctx.temperature_centi(4095), -2000);
        expect_int("a calibration row round-trips", ctx.temperature_centi(2738), 1001);
    }

    {   // and through the API that actually calls it: every StringWriter number
        // goes through the same formatter, so the operators are pinned too.
        auto written = [] ( auto && fill ) {
            TextWriter tw;
            fill(tw);
            return string(tw.str());
        };
        expect_str("writer int", written([](TextWriter & w){ w << 42; }), "42");
        expect_str("writer negative", written([](TextWriter & w){ w << -7; }), "-7");
        expect_str("writer unsigned", written([](TextWriter & w){ w << 4294967295u; }), "4294967295");
        expect_str("writer int64 min", written([](TextWriter & w){ w << (int64_t) INT64_MIN; }), "-9223372036854775808");
        expect_str("writer uint64 max", written([](TextWriter & w){ w << (uint64_t) UINT64_MAX; }), "18446744073709551615");
        expect_str("writer hex", written([](TextWriter & w){ w << HEX << 48879 << DEC; }), "beef");
        expect_str("writer bool", written([](TextWriter & w){ w << true << " " << false; }), "true false");
        expect_str("writer string", written([](TextWriter & w){ w << "abc" << 1; }), "abc1");
        // floats route through snprintf, not the integer formatter - a float
        // print must produce the number, never a literal "%g"
        expect_str("writer float", written([](TextWriter & w){ w << 1.5f; }), "1.5");
        expect_str("writer double", written([](TextWriter & w){ w << 0.25; }), "0.25");
    }

    printf(failures ? "nano_ctx: %d failure(s)\n" : "nano_ctx: ok\n", failures);
    return failures ? 1 : 0;
}
