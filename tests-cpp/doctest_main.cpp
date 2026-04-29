// tests-cpp/doctest_main.cpp
//
// Single TU that owns doctest's implementation + the daslang module bootstrap
// for the small-test executable. Every other TU under small/ includes
// <doctest/doctest.h> WITHOUT the IMPLEMENT macro, so the heavy header
// builds its implementation here only.
//
// Responsibilities:
//   - One-shot Module::Initialize() / Module::Shutdown() around the whole suite.
//   - Suite-end leak verification on three layers: gc_node, JobQue/Channel/
//     LockBox/Stream/Feature, and module-registered handles (HV etc.). Any
//     non-zero count forces a non-zero exit code.
//
// Per-TEST_CASE leak attribution: doctest_discover_tests() registers each
// TEST_CASE as its own ctest entry, which ctest runs via a separate process
// invocation (--test-case="..."). The suite-end check therefore runs once
// PER TEST CASE when invoked through ctest, giving native per-test
// attribution: a leak only shows up under the test that caused it. When the
// binary is run directly (no --test-case filter), all tests run in one
// process and the suite-end check reports the cumulative state.

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/daScriptModule.h"
#include "daScript/misc/job_que.h"
#include "daScript/misc/gc_node.h"

#include <cstdio>

int main(int argc, char** argv) {
    NEED_ALL_DEFAULT_MODULES;     // statement-form macro — must be inside a function body
    NEED_MODULE(Module_JobQue);   // ensure the JobQue module is linked in even if no test pulls it

    das::Module::Initialize();

    doctest::Context dctx;
    dctx.applyCommandLine(argc, argv);
    int rc = dctx.run();

    // ── Suite-end leak verification: detect AND fail on every layer ─────────
    int leak_rc = 0;

    // gc_node check: thread-local root only — gc_node tracking is per-thread, and
    // this samples just the suite-runner thread. Tests that allocate gc_nodes on
    // worker threads without joining-and-collecting before the test ends will not
    // surface here. None of the current small/ tests do that; revisit (per-thread
    // aggregation) when the first multi-threaded gc_node test lands.
    auto & root = das::gc_root::gc_get_thread_root();
    if ( root.gc_count != 0 ) {
        printf("\n[FAIL] gc_node leaks (suite thread): %llu node(s) still alive at suite end\n",
               (unsigned long long)root.gc_count);
        root.gc_report();
        leak_rc = 1;
    }

    if ( uint64_t n = das::JobStatus::CountJobQueLeaks() ) {
        printf("\n[FAIL] jobque leaks: %llu tracked object(s) still alive\n",
               (unsigned long long)n);
        das::JobStatus::DumpJobQueLeaks();
        leak_rc = 1;
    }

    if ( uint64_t n = das::Module::CountHandleLeaks() ) {
        printf("\n[FAIL] handle leaks: %llu module-tracked resource(s) still alive\n",
               (unsigned long long)n);
        leak_rc = 1;
        // The actual per-type dump comes out of Module::Shutdown(true) below.
    }

    das::Module::Shutdown(/*dumpHandleLeaks*/ true);

    return rc != 0 ? rc : leak_rc;
}
