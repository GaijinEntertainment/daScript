#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

namespace {

    string g_sunk;

    void sink_to_string ( const char * text ) {
        g_sunk += text;
    }

    // the sink is process-wide, so a case that left it installed would send the
    // rest of the suite's diagnostics into a dead string
    struct SinkGuard {
        SinkGuard() { g_sunk.clear(); setTextPrinterSink(&sink_to_string); }
        ~SinkGuard() { setTextPrinterSink(nullptr); }
    };
}

TEST_CASE("the text printer sink takes every diagnostic the runtime writes") {
    SinkGuard guard;

    SUBCASE("a TextPrinter writes through the sink instead of stdout") {
        TextPrinter tp;
        tp << "sunk " << 42 << "\n";
        CHECK_EQ(g_sunk, "sunk 42\n");
    }

    SUBCASE("a fatal report goes the same way, formatted") {
        das_fatal_log("fatal %s %d\n", "here", 7);
        CHECK_EQ(g_sunk, "fatal here 7\n");
    }

    SUBCASE("an unopenable file leaves the sink where it was") {
        CHECK_FALSE(textPrinterToFile("/no/such/directory/diagnostics.log"));
        TextPrinter tp;
        tp << "still sunk\n";
        CHECK_EQ(g_sunk, "still sunk\n");
    }
}
