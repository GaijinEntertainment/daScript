// Content pin for the LLVM JIT emitter sources. Emitted-code changes are invisible to the
// DLL cache key (it folds the USER program's hashes, not the emitters'), so the trigger
// has to be text-level.
#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace das;

// files whose text change can alter emitted machine code without changing the user
// program's AOT hashes. llvm_tune.das stays out by design (selection only);
// x64_avx/aarch64_neon/f16_cvt are plain das reference bodies, visible to the AOT hash;
// llvm_aot/llvm_exe emit explicit artifacts, not silently cached DLLs.
static const char * EMITTER_FILES[] = {
    "llvm_jit.das",
    "llvm_jit_common.das",
    "llvm_boost.das",
    "llvm_macro.das",
    "llvm_jit_code.das",
    "llvm_code.das",
    "llvm_dsl.das",
    "llvm_jit_intrin.das",
    "llvm_targets.das",
    "llvm_user_modules.das",
};

// normalized to LF so Windows and Linux checkouts agree
static uint64_t fnv1a64 ( uint64_t h, const char * data, size_t size ) {
    for ( size_t i = 0; i != size; ++i ) {
        if ( data[i] == '\r' ) continue;
        h ^= uint8_t(data[i]);
        h *= 0x100000001b3ul;
    }
    return h;
}

static std::string readWholeFile ( const std::string & path ) {
    FILE * f = fopen(path.c_str(), "rb");
    if ( !f ) return std::string();
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::string text;
    text.resize(size_t(size));
    size_t got = fread(&text[0], 1, size_t(size), f);
    fclose(f);
    text.resize(got);
    return text;
}

TEST_CASE("jit emitter sources match the pinned hash") {
    const std::string daslib = getDasRoot() + "/modules/dasLLVM/daslib/";
    uint64_t emitterHash = 0xcbf29ce484222325ul;
    for ( const char * name : EMITTER_FILES ) {
        std::string text = readWholeFile(daslib + name);
        REQUIRE_MESSAGE(!text.empty(), "can't read emitter source: " << name);
        emitterHash = fnv1a64(emitterHash, name, strlen(name));
        emitterHash = fnv1a64(emitterHash, "\n", 1);
        emitterHash = fnv1a64(emitterHash, text.data(), text.size());
    }
    std::string runSrc = readWholeFile(daslib + "llvm_jit_run.das");
    REQUIRE_MESSAGE(!runSrc.empty(), "can't read llvm_jit_run.das");
    size_t pinAt = runSrc.find("let LLVM_JIT_EMITTER_HASH");
    REQUIRE_MESSAGE(pinAt != std::string::npos, "pin constant LLVM_JIT_EMITTER_HASH missing from llvm_jit_run.das");
    size_t hexAt = runSrc.find("0x", pinAt);
    REQUIRE_MESSAGE(hexAt != std::string::npos, "pin constant LLVM_JIT_EMITTER_HASH has no 0x value");
    uint64_t pinned = strtoull(runSrc.c_str() + hexAt, nullptr, 16);
    char pinLiteral[32];
    snprintf(pinLiteral, sizeof(pinLiteral), "0x%llxul", (unsigned long long) emitterHash);
    INFO("emitter sources changed: set LLVM_JIT_EMITTER_HASH = " << pinLiteral
        << " in llvm_jit_run.das, and bump LLVM_JIT_CODEGEN_VERSION beside it if emitted code changed");
    CHECK_EQ(emitterHash, pinned);
}
