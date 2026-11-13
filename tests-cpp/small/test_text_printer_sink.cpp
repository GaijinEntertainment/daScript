#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include <atomic>
#include <chrono>
#include <filesystem>
#include <random>
#include <thread>

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

namespace {

    std::atomic<bool> g_insidePrint(false);
    std::atomic<bool> g_finishPrint(false);

    void sink_that_waits ( const char * ) {
        g_insidePrint = true;
        while ( !g_finishPrint.load() ) std::this_thread::yield();
    }

    void sink_to_nowhere ( const char * ) {
    }

    template <typename Change>
    bool changedUnderAHeldPrint ( Change && change ) {
        g_insidePrint = false;
        g_finishPrint = false;
        setTextPrinterSink(&sink_that_waits);
        std::thread printer([]() {
            TextPrinter tp;
            tp << "held\n";
        });
        while ( !g_insidePrint.load() ) std::this_thread::yield();
        std::atomic<bool> changing(false);
        std::atomic<bool> changed(false);
        std::thread changer([&]() {
            changing = true;
            change();
            changed = true;
        });
        while ( !changing.load() ) std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        const bool changedUnderThePrint = changed.load();
        g_finishPrint = true;
        printer.join();
        changer.join();
        setTextPrinterSink(nullptr);
        return changedUnderThePrint;
    }

    void sink_that_redirects ( const char * ) {
        setTextPrinterSink(&sink_to_string);
    }
}

TEST_CASE("a sink change waits for the print in progress") {
    SUBCASE("to another sink") {
        CHECK_FALSE(changedUnderAHeldPrint([]() { setTextPrinterSink(&sink_to_nowhere); }));
    }
    SUBCASE("to a file") {
        namespace fs = std::filesystem;
        std::random_device rd;
        const fs::path dir = fs::temp_directory_path() / ("das_printer_sink_" + std::to_string(rd()));
        fs::create_directories(dir);
        const std::string file = (dir / "diagnostics.log").generic_string();
        std::atomic<bool> opened(false);
        const bool under = changedUnderAHeldPrint([&]() { opened = textPrinterToFile(file.c_str()); });
        std::error_code ec;
        fs::remove_all(dir, ec);
        CHECK(opened.load());
        CHECK_FALSE(under);
    }
}

TEST_CASE("a sink may change the sink from inside the print it serves") {
    g_sunk.clear();
    setTextPrinterSink(&sink_that_redirects);
    {
        TextPrinter tp;
        tp << "taken by the redirecting sink\n";
        tp << "sunk\n";
    }
    setTextPrinterSink(nullptr);
    CHECK_EQ(g_sunk, "sunk\n");
}
