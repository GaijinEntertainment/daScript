#include "daScript/misc/platform.h"
#include "daScript/misc/performance_time.h"

#if DAS_PROFILE_SECTIONS
namespace das {
    ProfileSection::ProfileSection(const char * n) {
        name = n;
        timeStamp = ref_time_ticks();
    }
    ProfileSection::~ProfileSection() {
        int usec = get_time_usec(timeStamp);
        printf("\"%s\", %.4f\n", name ? name : "unspecified", usec / 1000000.0f);
    }
}

#endif

// ref_time_ticks() returns CLOCK_MONOTONIC-style nanoseconds on every platform.
// Prior to this normalization, Windows returned raw QueryPerformanceCounter
// ticks (~10 MHz typical) while Linux/macOS already returned ns — so callers
// that did `ref_time_ticks() + int64(timeout_sec * 1_000_000)` got 30 s on
// Windows (lucky math at 10 MHz) but 30 ms on POSIX. The footgun is gone: raw
// subtraction `now - then` always yields nanoseconds elapsed.
//
// Prefer `get_time_usec(start)` / `get_time_nsec(start)` for elapsed-time
// comparisons — those wrap the subtraction and continue to work portably.

#ifdef _MSC_VER

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// QueryPerformanceFrequency is invariant after boot — cache once per process
// to avoid a syscall on every ref_time_ticks() call. Race-tolerant: parallel
// initialisers all compute the same value, and int64 stores are atomic on
// x64/arm64.
static int64_t qpc_freq() {
    static int64_t cached = 0;
    if ( cached == 0 ) {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        cached = f.QuadPart;
    }
    return cached;
}

extern "C" int64_t ref_time_ticks () {
    LARGE_INTEGER t0;
    QueryPerformanceCounter(&t0);
    // Convert QPC counter to nanoseconds without overflowing int64:
    //   ns = (ticks / freq) * 1e9 + (ticks % freq) * 1e9 / freq
    // freq is typically 10 MHz, so (ticks / freq) fits comfortably and the
    // remainder * 1e9 also fits (max ~1e16, well under 2^63).
    const int64_t freq = qpc_freq();
    const int64_t whole = t0.QuadPart / freq;
    const int64_t rem   = t0.QuadPart % freq;
    return whole * 1000000000LL + (rem * 1000000000LL) / freq;
}

extern "C" int get_time_usec ( int64_t reft ) {
    return int((ref_time_ticks() - reft) / 1000LL);
}

extern "C" int64_t get_time_nsec ( int64_t reft ) {
    return ref_time_ticks() - reft;
}

extern "C" int64_t ref_time_delta_to_usec ( int64_t ref ) {
    return ref / 1000LL;
}

#elif __linux__ || defined(_EMSCRIPTEN_VER) || defined __HAIKU__

#include <time.h>

const uint64_t NSEC_IN_SEC = 1000000000LL;

extern "C" int64_t ref_time_ticks () {
    timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        DAS_ASSERT(false);
        return -1;
    }
    return ts.tv_sec * NSEC_IN_SEC + ts.tv_nsec;
}

extern "C" int get_time_usec ( int64_t reft ) {
    return int((ref_time_ticks() - reft) / 1000LL);
}

extern "C" int64_t get_time_nsec ( int64_t reft ) {
    return ref_time_ticks() - reft;
}

extern "C" int64_t ref_time_delta_to_usec ( int64_t ref ) { return ref / 1000LL; }


#else // osx

#include <time.h>

extern "C" int64_t ref_time_ticks () {
    return clock_gettime_nsec_np(CLOCK_MONOTONIC_RAW);
}

extern "C" int get_time_usec ( int64_t reft ) {
    return int((ref_time_ticks() - reft) / 1000LL);
}

extern "C" int64_t get_time_nsec ( int64_t reft ) {
    return ref_time_ticks() - reft;
}

extern "C" int64_t ref_time_delta_to_usec ( int64_t ref ) { return ref / 1000LL; }

#endif
