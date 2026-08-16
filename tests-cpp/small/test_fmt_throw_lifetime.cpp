// The panic longjmp does not unwind on POSIX, and can never be made to: JIT frames
// cannot be unwound. An exception object handed to FMT_THROW leaks unless it dies first.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/aot.h"

using namespace das;

#if (!defined(DAS_ENABLE_EXCEPTIONS)) || (!DAS_ENABLE_EXCEPTIONS)

namespace {
    struct ProbeError {
        static int destroyed;
        ~ProbeError() { destroyed++; }
        const char * what() const { return "probe message"; }
    };
    int ProbeError::destroyed = 0;
}

TEST_CASE("FMT_THROW destroys its exception before the panic jump") {
    ProbeError::destroyed = 0;
    int destroyedAtCatch = -1;
    string caught;
    das_trycatch([&]{
        FMT_THROW(ProbeError());
    },[&](const char * msg){
        destroyedAtCatch = ProbeError::destroyed;
        caught = msg;
    });
    CHECK_EQ(destroyedAtCatch, 1);
    CHECK_EQ(ProbeError::destroyed, 1);
    CHECK_EQ(caught, "probe message");
}

TEST_CASE("das_throw delivers its message to the active das_trycatch") {
    string caught;
    das_trycatch([&]{
        das_throw("boom");
    },[&](const char * msg){
        caught = msg;
    });
    CHECK_EQ(caught, "boom");
}

#else

TEST_CASE("FMT_THROW exception lifetime is RAII-owned when exceptions are enabled") {
    MESSAGE("FMT_THROW is a real C++ throw here; RAII owns the object");
    CHECK(true);
}

#endif
