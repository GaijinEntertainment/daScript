#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"

using namespace das;

TEST_CASE("a whole-program stream reads the program module back as the program module") {
    gc_guard guard;
    auto fAccess = make_smart<FsFileAccess>();
    fAccess->addExtraModule("serializer_extra_module", getDasRoot() + "/tests-cpp/small/_serializer_extra_module.das");
    TextWriter logs;
    ModuleGroup libGroup;
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_serializer_program_module_last.das", fAccess, logs, libGroup);
    REQUIRE(program != nullptr);
    REQUIRE_FALSE(program->failed());
    REQUIRE(program->library.findModule("serializer_extra_module") != nullptr);
    const string programModule = program->thisModule->name;

    SerializationStorageVector written;
    {
        AstSerializer writer(&written, true);
        program->serialize(writer);
    }
    written.flush();

    SerializationStorageVector stream;
    stream.buffer = written.buffer;
    AstSerializer reader(&stream, false);
    ModuleGroup readGroup;
    reader.thisModuleGroup = &readGroup;
    auto read = make_smart<Program>();
    REQUIRE(reader.serializeScript(read));
    read->thisModuleGroup = &readGroup;
    read->library.foreach([&](Module * m) { readGroup.addModule(m); return true; }, "*");
    readGroup.getModules().pop_back();
    REQUIRE(read->thisModule != nullptr);
    CHECK(read->thisModule->name == programModule);
    CHECK(read->thisModule->findFunction(program->thisModule->findUniqueFunction("program_answer")->getMangledName()) != nullptr);
    read.reset();
    readGroup.reset();
    reader.moduleLibrary = nullptr;
}
