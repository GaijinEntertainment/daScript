#include <doctest/doctest.h>

#include "daScript/daScript.h"

#include <cstring>
#include <string>

// The C-boundary runtime for `daslang -lib`, from src/builtin/module_jit.cpp. The generated
// entry points call exactly these; this test drives them without any LLVM in the picture.
extern "C" {
    DAS_API das::Context * jit_create_standalone_ctx ( uint64_t totalVariables,
                                                       uint64_t totalFunctions,
                                                       uint64_t globalStringHeapSize,
                                                       uint64_t globalsSize,
                                                       uint64_t sharedSize,
                                                       bool pinvoke,
                                                       uint64_t stackSize );
    DAS_API int32_t jit_lib_invoke_guarded ( das::Context * ctx,
                                             void (*tramp)(das::Context *, void *),
                                             void * frame );
    DAS_API das::Context * jit_lib_create_finish ( das::Context * ctx, int32_t ok );
    DAS_API const char * jit_lib_last_error ( das::Context * ctx );
    DAS_API void jit_destroy_standalone_ctx ( das::Context * ctx );
    DAS_API int32_t jit_lib_run_once ( int32_t * guard, void (*fn)() );
}

namespace {

static int g_ran = 0;

static void tramp_quiet ( das::Context *, void * frame ) {
    if ( frame ) *(int *) frame = 7;
    g_ran ++;
}

static void tramp_raises ( das::Context * ctx, void * ) {
    ctx->throw_error("boom in the body");
}

static int g_once_calls = 0;
static void bump_once () { g_once_calls ++; }

static das::Context * make_ctx () {
    return jit_create_standalone_ctx(0, 1, 0, 0, 0, false, 16 * 1024);
}

}

TEST_CASE("jit_lib_invoke_guarded runs a body and reports success") {
    das::Context * ctx = make_ctx();
    REQUIRE(ctx != nullptr);
    g_ran = 0;
    int slot = 0;
    CHECK(jit_lib_invoke_guarded(ctx, &tramp_quiet, &slot) == 1);
    CHECK(g_ran == 1);
    CHECK(slot == 7);
    CHECK(jit_lib_last_error(ctx) == nullptr);
    jit_destroy_standalone_ctx(ctx);
}

TEST_CASE("jit_lib_invoke_guarded turns a das panic into a return code plus a message") {
    das::Context * ctx = make_ctx();
    REQUIRE(ctx != nullptr);
    CHECK(jit_lib_invoke_guarded(ctx, &tramp_raises, nullptr) == 0);
    const char * err = jit_lib_last_error(ctx);
    REQUIRE(err != nullptr);
    CHECK(std::strstr(err, "boom in the body") != nullptr);

    // the context stays usable, and a call that completes clears the message
    int slot = 0;
    CHECK(jit_lib_invoke_guarded(ctx, &tramp_quiet, &slot) == 1);
    CHECK(slot == 7);
    CHECK(jit_lib_last_error(ctx) == nullptr);
    jit_destroy_standalone_ctx(ctx);
}

TEST_CASE("jit_lib_create_finish drops a context whose init raised, and keeps the message") {
    das::Context * ctx = make_ctx();
    REQUIRE(ctx != nullptr);
    jit_lib_invoke_guarded(ctx, &tramp_raises, nullptr);
    CHECK(jit_lib_create_finish(ctx, 0) == nullptr);
    const char * err = jit_lib_last_error(nullptr);
    REQUIRE(err != nullptr);
    CHECK(std::strstr(err, "boom in the body") != nullptr);

    das::Context * good = make_ctx();
    REQUIRE(good != nullptr);
    CHECK(jit_lib_create_finish(good, 1) == good);
    CHECK(jit_lib_last_error(nullptr) == nullptr);
    jit_destroy_standalone_ctx(good);
}

TEST_CASE("jit_lib_run_once declines a process that already carries a daslang runtime") {
    // this test binary registers the modules itself, which is the embedding-host case: the
    // library must decline rather than re-register and abort
    das::daScriptEnvironment::ensure();
    REQUIRE(das::daScriptEnvironment::getBound() != nullptr);
    REQUIRE(das::daScriptEnvironment::getBound()->modules != nullptr);
    g_once_calls = 0;
    int32_t guard = 0;
    CHECK(jit_lib_run_once(&guard, &bump_once) == 0);
    CHECK(g_once_calls == 0);
    CHECK(guard == 0);
    const char * err = jit_lib_last_error(nullptr);
    REQUIRE(err != nullptr);
    CHECK(std::strstr(err, "already carries a daslang runtime") != nullptr);
}
