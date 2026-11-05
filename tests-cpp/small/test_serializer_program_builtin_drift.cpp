#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"

using namespace das;

namespace {

bool readProgram ( const SerializationStorageVector & written ) {
    SerializationStorageVector stream;
    stream.buffer = written.buffer;
    AstSerializer reader(&stream, false);
    ModuleGroup readGroup;
    reader.thisModuleGroup = &readGroup;
    auto read = make_smart<Program>();
    bool ok = reader.serializeScript(read);
    if ( ok ) {
        read->library.foreach([&](Module * m) { readGroup.addModule(m); return true; }, "*");
        readGroup.getModules().pop_back();
    } else {
        read->library.reset();
    }
    read.reset();
    readGroup.reset();
    reader.moduleLibrary = nullptr;
    return ok;
}

} // namespace

TEST_CASE("a whole-program stream is not read back once a builtin module it names changed") {
    gc_guard guard;
    auto fAccess = make_smart<FsFileAccess>();
    TextWriter logs;
    ModuleGroup libGroup;
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_env_serializer.das", fAccess, logs, libGroup);
    REQUIRE(program != nullptr);
    REQUIRE_FALSE(program->failed());

    SerializationStorageVector written;
    {
        AstSerializer writer(&written, true);
        program->serialize(writer);
        writer.moduleLibrary = nullptr;
    }
    written.flush();

    CHECK(readProgram(written));
    uint64_t builtinHash = Module::require("$")->getOwnSemanticHash();
    int patched = 0;
    for ( size_t i = 0; i + sizeof(builtinHash) <= written.buffer.size(); ++i ) {
        if ( memcmp(written.buffer.data() + i, &builtinHash, sizeof(builtinHash)) == 0 ) {
            written.buffer[i] ^= 1;
            patched ++;
        }
    }
    REQUIRE(patched > 0);
    CHECK_FALSE(readProgram(written));
}
