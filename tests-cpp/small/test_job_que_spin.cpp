#include <doctest/doctest.h>
#include "../../src/misc/job_que_spin.h"

namespace {
struct CountingSpinClock {
    using time_point = std::chrono::steady_clock::time_point;
    static int reads;
    static int micros;
    static time_point now() {
        ++reads;
        return time_point(std::chrono::microseconds(micros));
    }
};
int CountingSpinClock::reads = 0;
int CountingSpinClock::micros = 0;
using SpinDeadline = das::detail::JobQueSpinDeadline<CountingSpinClock>;
}

TEST_CASE("zero jobque spin window never reads the clock") {
    CountingSpinClock::reads = 0;
    CountingSpinClock::micros = 100;
    SpinDeadline window(0);
    CHECK(CountingSpinClock::reads == 0);
    CHECK(window.expired(false));
    CHECK(window.expired(true));
    CHECK(CountingSpinClock::reads == 0);
}

TEST_CASE("positive jobque spin window expires and renews after team work") {
    CountingSpinClock::reads = 0;
    CountingSpinClock::micros = 100;
    SpinDeadline window(20);
    CHECK(CountingSpinClock::reads == 1);
    CountingSpinClock::micros = 119;
    CHECK_FALSE(window.expired(false));
    CHECK(CountingSpinClock::reads == 2);
    CountingSpinClock::micros = 120;
    CHECK(window.expired(false));
    CHECK(CountingSpinClock::reads == 3);
    CountingSpinClock::micros = 140;
    CHECK_FALSE(window.expired(true));
    CHECK(CountingSpinClock::reads == 4);
    CountingSpinClock::micros = 159;
    CHECK_FALSE(window.expired(false));
    CountingSpinClock::micros = 160;
    CHECK(window.expired(false));
    CHECK(CountingSpinClock::reads == 6);
}
