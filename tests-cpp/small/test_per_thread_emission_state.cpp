#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

using namespace das;

namespace das {
    void jit_par_emit_begin ();
    void jit_par_emit_add ( void * mod, void * tm, void * pbo, const char * pipeline, const char * objPath );
    bool jit_par_emit_run ( int32_t threads, int32_t fileType, void * fnRunPasses, void * fnVerifyModule,
                            void * fnEmitToFile, void * fnGetErrorMessage, void * fnDisposeErrorMessage,
                            void * fnDisposeMessage, bool logTimes, Context * context );
    string aotModuleName ( Module * pm );
}

namespace {

    std::mutex g_verifiedMutex;
    std::vector<void *> g_verified;

    int32_t verify_records_the_module ( void * mod, int32_t, char ** ) {
        std::lock_guard<std::mutex> guard(g_verifiedMutex);
        g_verified.push_back(mod);
        return 0;
    }

    void * never_runs_passes ( void *, const char *, void *, void * ) { return nullptr; }
    int32_t never_emits ( void *, void *, const char *, int32_t, char ** ) { return 0; }
    char * no_error_message ( void * ) { return nullptr; }
    void disposes_nothing ( void * ) {}

    bool runQueuedJobs ( int32_t threads ) {
        return jit_par_emit_run(threads, 0, (void *) &never_runs_passes, (void *) &verify_records_the_module,
            (void *) &never_emits, (void *) &no_error_message, (void *) &disposes_nothing, (void *) &disposes_nothing,
            false, nullptr);
    }

    void queueJob ( int & module ) {
        jit_par_emit_add(&module, nullptr, nullptr, "", "");
    }
}

TEST_CASE("the parallel JIT emit queue belongs to the thread that fills it") {
    int mine = 0, theirs = 0;
    g_verified.clear();
    jit_par_emit_begin();
    queueJob(mine);
    std::thread other([&]() {
        jit_par_emit_begin();
        queueJob(theirs);
    });
    other.join();
    REQUIRE(runQueuedJobs(1));
    REQUIRE_EQ(g_verified.size(), 1u);
    CHECK_EQ(g_verified[0], (void *) &mine);
}

TEST_CASE("the parallel JIT emit workers run the jobs of the thread that called run") {
    int first = 0, second = 0;
    g_verified.clear();
    jit_par_emit_begin();
    queueJob(first);
    queueJob(second);
    REQUIRE(runQueuedJobs(2));
    CHECK_EQ(g_verified.size(), 2u);
}

TEST_CASE("the AOT main module name belongs to the thread that set it") {
    auto program = make_smart<Program>();
    setAotMainModuleName("alpha");
    std::thread other([]() { setAotMainModuleName("beta"); });
    other.join();
    const string name = aotModuleName(program->thisModule.get());
    setAotMainModuleName("");
    CHECK_EQ(name, "alpha");
}
