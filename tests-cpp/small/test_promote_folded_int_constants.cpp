#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"

using namespace das;

TEST_CASE("folded int constants promote in binary operators unless the policy or the option turns it off") {
    bool promote = true;
    bool succeeds = false;
    const char * body = nullptr;
    const char * option = "";
    SUBCASE("the default promotes the right operand") {
        succeeds = true;
        body = "return 2.0 + int(1)";
    }
    SUBCASE("the default promotes the left operand") {
        succeeds = true;
        body = "return int(1) + 2.0";
    }
    SUBCASE("the default promotes in a compound assignment") {
        succeeds = true;
        body = "var velocity = 3.0\nvelocity *= int(1)\nreturn velocity";
    }
    SUBCASE("the policy off rejects a folded int") {
        promote = false;
        body = "return 2.0 + int(1)";
    }
    SUBCASE("the policy off rejects a folded compound assignment") {
        promote = false;
        body = "var velocity = 3.0\nvelocity *= int(1)\nreturn velocity";
    }
    SUBCASE("the option off overrides the policy") {
        option = "options promote_folded_int_constants = false\n";
        body = "return 2.0 + int(1)";
    }
    SUBCASE("the option on overrides the policy") {
        promote = false;
        succeeds = true;
        option = "options promote_folded_int_constants = true\n";
        body = "return 2.0 + int(1)";
    }
    SUBCASE("a runtime int is rejected either way") {
        body = "return 2.0 + value";
    }
    SUBCASE("a source literal works with the policy off") {
        promote = false;
        succeeds = true;
        body = "return 2.0 + 1";
    }
    REQUIRE(body);
    string source = string("options gen2\n") + option + "[export]\ndef test(value : int) : float {\n" + body + "\n}\n";
    auto access = make_smart<FsFileAccess>();
    access->setFileInfo("folded_int_policy.das",
        make_unique<TextFileInfo>(source.c_str(), uint32_t(source.size()), false));
    TextWriter logs;
    ModuleGroup group;
    CodeOfPolicies policies;
    CHECK(policies.promote_folded_int_constants);
    policies.promote_folded_int_constants = promote;
    auto program = compileDaScript("folded_int_policy.das", access, logs, group, policies);
    REQUIRE(program);
    for (auto & error : program->errors) {
        logs << reportError(error.at, error.what, error.extra, error.fixme, error.cerr);
    }
    INFO(logs.str());
    REQUIRE(program->failed() == !succeeds);
    if (!succeeds) return;
    Context context(program->getContextStackSize());
    REQUIRE(program->simulate(context, logs));
    auto fn = context.findFunction("test");
    REQUIRE(fn);
    vec4f args[] = {cast<int32_t>::from(1)};
    auto result = context.evalWithCatch(fn, args);
    REQUIRE(context.getException() == nullptr);
    CHECK(cast<float>::to(result) == 3.0f);
}

TEST_CASE("folded int compatibility policies survive the module cache and reject a policy switch") {
    const string source = "options gen2\n[export]\ndef test : float { return 2.0 + int(1) }\n";
    auto access = make_smart<FsFileAccess>();
    access->setFileInfo("folded_int_cache.das",
        make_unique<TextFileInfo>(source.c_str(), uint32_t(source.size()), false));
    CodeOfPolicies policies;
    policies.promote_folded_int_constants = true;
    {
        TextWriter logs;
        ModuleGroup group;
        auto program = compileDaScript("folded_int_cache.das", access, logs, group, policies);
        REQUIRE(program);
        REQUIRE_FALSE(program->failed());
    }
    SerializationStorageVector storage;
    {
        TextWriter logs;
        ModuleGroup group;
        AstSerializer writer(&storage, true);
        auto program = compileDaScript("folded_int_cache.das", access, logs, group, policies,
            ModuleCacheStream{nullptr, &writer});
        writer.moduleLibrary = nullptr;
        REQUIRE(program);
        REQUIRE_FALSE(program->failed());
        REQUIRE(storage.writingSize() > 0);
    }
    {
        TextWriter logs;
        ModuleGroup group;
        AstSerializer reader(&storage, false);
        auto program = compileDaScript("folded_int_cache.das", access, logs, group, policies,
            ModuleCacheStream{&reader, nullptr});
        reader.moduleLibrary = nullptr;
        REQUIRE(program);
        REQUIRE_FALSE(program->failed());
        CHECK_FALSE(reader.failed);
        CHECK(reader.servedModules > 0);
        CHECK(program->policies.promote_folded_int_constants);
    }
    {
        storage.bufferPos = 0;
        TextWriter logs;
        ModuleGroup group;
        AstSerializer reader(&storage, false);
        policies.promote_folded_int_constants = false;
        auto program = compileDaScript("folded_int_cache.das", access, logs, group, policies,
            ModuleCacheStream{&reader, nullptr});
        reader.moduleLibrary = nullptr;
        REQUIRE(program);
        INFO(logs.str());
        CHECK(program->failed());
        CHECK(logs.str().find("compile policies changed") != string::npos);
    }
}
