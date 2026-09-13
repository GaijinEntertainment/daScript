// Regression tests for the daScriptEnvironment serializer rail (module cache):
// serializer_read / serializer_write installed by the embedder, consumed by
// compileDaScript / parseDaScript, and force-disabled by
// disableSerializationOnDebugger when daslib/debug.das is in the require chain.
// The dastest --ser/--deser lane covers Program::serialize only — this rail had
// no coverage at all, which is how the disable ended up broken (write pointer
// aliased to read; loose "daslib/debug" filename match) with nothing going red.

#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"

using namespace das;

namespace {

// exception-safe restore: an escaped exception must not leave a dangling
// serializer installed for every later compile in the process
struct EnvSerializerGuard {
    ~EnvSerializerGuard() {
        auto & env = *daScriptEnvironment::getBound();
        env.serializer_read = nullptr;
        env.serializer_write = nullptr;
    }
};

ProgramPtr compileFixture ( const char * relPath, TextWriter & logs, ModuleGroup & libGroup ) {
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + relPath, fAccess, logs, libGroup);
    if ( program && program->failed() ) {
        for ( auto & err : program->errors ) {
            MESSAGE(reportError(err.at, err.what, err.extra, err.fixme, err.cerr));
        }
    }
    return program;
}

// first-ever compile in a process promotes shared modules (builtin.das etc.);
// warm up so the write and read passes below see the same promoted-module set
// and their streams stay symmetric.
void warmUp ( const char * relPath ) {
    TextWriter logs;
    ModuleGroup libGroup;
    auto program = compileFixture(relPath, logs, libGroup);
    REQUIRE(program != nullptr);
    REQUIRE_FALSE(program->failed());
}

// every expression's position and its type's position, in visit order - what the packed
// LineInfo, the type table's per-reference `at` and the deep copy must all preserve
struct PositionCensus : Visitor {
    vector<uint64_t> positions;
    void record ( const LineInfo & at ) {
        positions.push_back(at.line);
        positions.push_back(at.last_line);
        positions.push_back(at.column);
        positions.push_back(at.last_column);
        positions.push_back(uint64_t(hash_blockz64((const uint8_t *) (at.fileInfo ? at.fileInfo->name.c_str() : ""))));
    }
    void preVisitExpression ( Expression * expr ) override {
        record(expr->at);
        if ( expr->type ) record(expr->type->at);
    }
    void preVisit ( Function * fn ) override {
        record(fn->at);
        for ( auto & arg : fn->arguments ) {
            record(arg->at);
            if ( arg->type ) record(arg->type->at);
        }
    }
};

// the program's own module and the modules the record carries for it (the fixture's
// required module among them); the builtins are not in the stream
vector<uint64_t> positionCensus ( const ProgramPtr & program ) {
    PositionCensus census;
    for ( auto m : program->library.getModules() ) {
        if ( m->builtIn && !m->promoted ) continue;
        if ( m->name != program->thisModule->name && m->name != "test_env_serializer_module" ) continue;
        m->functions.foreach([&](FunctionPtr fn) { fn->visit(census); });
    }
    return census.positions;
}

} // namespace

TEST_CASE("env serializer: module cache round trip") {
    auto & env = *daScriptEnvironment::getBound();
    EnvSerializerGuard envGuard;
    REQUIRE(env.serializer_read == nullptr);
    REQUIRE(env.serializer_write == nullptr);
    warmUp("/tests-cpp/small/test_env_serializer.das");
    SerializationStorageVector storage;
    vector<uint64_t> coldPositions;
    // cold pass — modules written into the cache
    {
        TextWriter logs;
        ModuleGroup libGroup;
        AstSerializer writer(&storage, true);
        env.serializer_write = &writer;
        auto program = compileFixture("/tests-cpp/small/test_env_serializer.das", logs, libGroup);
        env.serializer_write = nullptr;
        writer.moduleLibrary = nullptr;
        REQUIRE(program != nullptr);
        REQUIRE_FALSE(program->failed());
        coldPositions = positionCensus(program);
    }
    CHECK(storage.buffer.size() > 0);
    CHECK(storage.buffer.size() == storage.writingSize());  // flushed: the vector is exactly the stream, not its growth
    // warm pass — modules read back instead of parsed
    {
        TextWriter logs;
        ModuleGroup libGroup;
        AstSerializer reader(&storage, false);
        env.serializer_read = &reader;
        auto program = compileFixture("/tests-cpp/small/test_env_serializer.das", logs, libGroup);
        env.serializer_read = nullptr;
        REQUIRE(program != nullptr);
        REQUIRE_FALSE(program->failed());
        CHECK_FALSE(reader.failed);                              // no fallback to reparse
        CHECK(logs.str().find("ser:") == string::npos);          // no cache-miss diagnostics
        CHECK(coldPositions.size() > 8);
        CHECK(positionCensus(program) == coldPositions);         // every line, column and file survives the record
        reader.moduleLibrary = nullptr;
        // the cache-fed program must actually run
        Context ctx(program->getContextStackSize());
        TextWriter rout;
        REQUIRE(program->simulate(ctx, rout));
        auto fn = ctx.findFunction("test");
        REQUIRE(fn != nullptr);
        ctx.evalWithCatch(fn, nullptr);
        CHECK(ctx.getException() == nullptr);
    }
}

TEST_CASE("env serializer: corrupt cache stream falls back to parsing") {
    auto & env = *daScriptEnvironment::getBound();
    EnvSerializerGuard envGuard;
    REQUIRE(env.serializer_read == nullptr);
    REQUIRE(env.serializer_write == nullptr);
    SerializationStorageVector storage;
    storage.buffer = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x02, 0x03 };  // garbage, not a cache stream
    {
        TextWriter logs;
        ModuleGroup libGroup;
        AstSerializer reader(&storage, false);
        env.serializer_read = &reader;
        auto program = compileFixture("/tests-cpp/small/test_env_serializer.das", logs, libGroup);
        env.serializer_read = nullptr;
        reader.moduleLibrary = nullptr;
        REQUIRE(program != nullptr);
        REQUIRE_FALSE(program->failed());   // fell back to parsing — no exception escaped
        CHECK(bool(reader.failed));
        CHECK(logs.str().find("ser: read failed") != string::npos);
    }
}

TEST_CASE("env serializer: non-debugger program keeps serializers installed") {
    auto & env = *daScriptEnvironment::getBound();
    EnvSerializerGuard envGuard;
    REQUIRE(env.serializer_read == nullptr);
    REQUIRE(env.serializer_write == nullptr);
    warmUp("/tests-cpp/small/test_env_serializer.das");
    SerializationStorageVector storage;
    {
        TextWriter logs;
        ModuleGroup libGroup;
        AstSerializer writer(&storage, true);
        env.serializer_write = &writer;
        auto program = compileFixture("/tests-cpp/small/test_env_serializer.das", logs, libGroup);
        CHECK(env.serializer_write == &writer);  // no debugger in the chain — stays installed
        env.serializer_write = nullptr;
        writer.moduleLibrary = nullptr;
        REQUIRE(program != nullptr);
        REQUIRE_FALSE(program->failed());
    }
}

// Compiling daslib/debug installs the debug agent on its own thread (dap), whose
// context eval races with the still-compiling main thread under TSan. That's
// pre-existing debugger threading, not the serializer rail under test — skip
// this one case in the tsan lane only.
#if defined(__has_feature)
#  if __has_feature(thread_sanitizer)
#    define ENV_SER_SKIP_DEBUGGER_TEST 1
#  endif
#endif
#if defined(__SANITIZE_THREAD__)
#  define ENV_SER_SKIP_DEBUGGER_TEST 1
#endif

#ifndef ENV_SER_SKIP_DEBUGGER_TEST
// MUST BE THE LAST TEST CASE IN THIS FILE: compiling the debugger promotes
// daslib/debug into the environment, and from then on every compile in this
// process runs with env serialization force-disabled (that's the semantic —
// the debugger installs into the environment). Any case after this one would
// see its serializers cleared.
TEST_CASE("env serializer: debugger requirement disables read AND write") {
    auto & env = *daScriptEnvironment::getBound();
    EnvSerializerGuard envGuard;
    REQUIRE(env.serializer_read == nullptr);
    REQUIRE(env.serializer_write == nullptr);
    SerializationStorageVector writeStorage;
    SerializationStorageVector readStorage;
    // both installed, debugger listed in req (cold environment): the filename match
    {
        TextWriter logs;
        ModuleGroup libGroup;
        AstSerializer writer(&writeStorage, true);
        AstSerializer reader(&readStorage, false);
        env.serializer_write = &writer;
        env.serializer_read = &reader;
        auto program = compileFixture("/tests-cpp/small/test_env_serializer_debug.das", logs, libGroup);
        // disableSerializationOnDebugger must clear BOTH pointers; the original
        // copy-paste bug aliased write to read and left writes enabled
        CHECK(env.serializer_read == nullptr);
        CHECK(env.serializer_write == nullptr);
        env.serializer_read = nullptr;
        env.serializer_write = nullptr;
        writer.moduleLibrary = nullptr;
        reader.moduleLibrary = nullptr;
        REQUIRE(program != nullptr);
        REQUIRE_FALSE(program->failed());
        CHECK(writeStorage.buffer.size() == 0);  // nothing cached for a debugger program
    }
    // write-only setup, debugger now PROMOTED into the environment (not in req
    // anymore): the environment-scoped check must still fire
    {
        SerializationStorageVector coldStorage;
        TextWriter logs;
        ModuleGroup libGroup;
        AstSerializer writer(&coldStorage, true);
        env.serializer_write = &writer;
        auto program = compileFixture("/tests-cpp/small/test_env_serializer_debug.das", logs, libGroup);
        CHECK(env.serializer_write == nullptr);
        env.serializer_write = nullptr;
        writer.moduleLibrary = nullptr;
        REQUIRE(program != nullptr);
        REQUIRE_FALSE(program->failed());
        CHECK(coldStorage.buffer.size() == 0);
    }
}
#endif  // ENV_SER_SKIP_DEBUGGER_TEST
