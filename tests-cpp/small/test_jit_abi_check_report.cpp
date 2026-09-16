// The --jit-check-abi report: a recorded layout drift is one stderr warning and the program
// goes on; a clean sweep (and the sweep after a reported one) prints nothing. A wasm app
// pipes its stdout, so nothing of the check may land there.
#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include <cstdio>
#include <cstring>
#include <string>
#ifdef _WIN32
#include <io.h>
#define DAS_DUP _dup
#define DAS_DUP2 _dup2
#define DAS_FILENO _fileno
#define DAS_CLOSE _close
#else
#include <unistd.h>
#define DAS_DUP dup
#define DAS_DUP2 dup2
#define DAS_FILENO fileno
#define DAS_CLOSE close
#endif

extern "C" {
    DAS_API void jit_check_handled_type_size ( const char * moduleName, const char * typeName, uint32_t hostSize );
    DAS_API void jit_handled_abi_check_report ();
}

// runs `fn` with stderr redirected into a temp file; returns what it wrote
template <typename Fn>
static std::string captureStderr ( Fn && fn ) {
    fflush(stderr);
    FILE * tmp = tmpfile();
    REQUIRE(tmp);
    int saved = DAS_DUP(DAS_FILENO(stderr));
    REQUIRE(saved >= 0);
    REQUIRE(DAS_DUP2(DAS_FILENO(tmp), DAS_FILENO(stderr)) >= 0);
    fn();
    fflush(stderr);
    DAS_DUP2(saved, DAS_FILENO(stderr));
    DAS_CLOSE(saved);
    std::string out;
    fseek(tmp, 0, SEEK_SET);
    char buf[4096];
    size_t got;
    while ( (got = fread(buf, 1, sizeof(buf), tmp)) > 0 ) out.append(buf, got);
    fclose(tmp);
    return out;
}

TEST_CASE("jit abi check: a drift is one stderr warning, never an abort; a clean sweep is silent") {
    // rtti::LineInfo is a handled type every build registers; the host value is deliberately wrong
    std::string first = captureStderr([]() {
        jit_check_handled_type_size("rtti", "LineInfo", 1);
        jit_handled_abi_check_report();   // returns - the old report aborted here
    });
    CHECK_MESSAGE(first.find("JIT ABI CHECK: 1 handled-type layout mismatch") != std::string::npos, "stderr was: " << first);
    CHECK_MESSAGE(first.find("size   rtti::LineInfo  host=1 target=") != std::string::npos, "stderr was: " << first);

    // the report reset its counters: a following sweep with no drift prints nothing
    std::string second = captureStderr([]() {
        jit_check_handled_type_size("rtti", "LineInfo", uint32_t(sizeof(das::LineInfo)));
        jit_handled_abi_check_report();
    });
    CHECK_MESSAGE(second.empty(), "a clean sweep wrote to stderr: " << second);
}
