#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/daScriptC.h"

using namespace das;

// The policy path: CodeOfPolicies::max_unreserved_size -> Context via Program::simulate.
// The `options` spelling and the runtime get/set live in tests/language/max_unreserved_size.das.

namespace {

const char * SRC =
    "options gen2\n"
    "[export] def bare_resize {\n"
    "    var a : array<uint8>\n"
    "    a |> resize(65537)\n"
    "}\n"
    "[export] def reserved_resize {\n"
    "    var a : array<uint8>\n"
    "    a |> reserve(200000)\n"
    "    a |> resize(200000)\n"
    "}\n";

} // namespace

TEST_CASE("max_unreserved_size flows from CodeOfPolicies to the context") {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    const string name = "max_unreserved_size_test.das";
    fAccess->setFileInfo(name, make_unique<TextFileInfo>(SRC, uint32_t(strlen(SRC)), /*own*/false));
    CodeOfPolicies policies;
    policies.max_unreserved_size = 65536;
    auto program = compileDaScript(name, fAccess, tout, dummyLibGroup, policies);
    REQUIRE(program);
    REQUIRE_FALSE(program->failed());
    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));
    CHECK_EQ(ctx.maxUnreservedSize, uint64_t(65536));

    SUBCASE("bare growing resize past the policy throws") {
        auto fn = ctx.findFunction("bare_resize");
        REQUIRE(fn);
        ctx.evalWithCatch(fn, nullptr);
        CHECK(ctx.getException() != nullptr);
    }
    SUBCASE("exact reserve first passes at any size") {
        auto fn = ctx.findFunction("reserved_resize");
        REQUIRE(fn);
        ctx.evalWithCatch(fn, nullptr);
        CHECK(ctx.getException() == nullptr);
    }
}

TEST_CASE("DAS_POLICY_MAX_UNRESERVED_SIZE reaches the context through the C API") {
    das_text_writer * tout = das_text_make_printer();
    das_module_group * mg = das_modulegroup_make();
    das_file_access * fa = das_fileaccess_make_default();
    das_fileaccess_introduce_file(fa, "policy_main.das", SRC, 1);
    das_policies * pol = das_policies_make();
    REQUIRE(das_policies_set_int(pol, DAS_POLICY_MAX_UNRESERVED_SIZE, 65536) == 1);
    das_program * prog = das_program_compile_policies((char*)"policy_main.das", fa, tout, mg, pol);
    REQUIRE(prog);
    REQUIRE(das_program_err_count(prog) == 0);
    das_context * ctx = das_context_make(das_program_context_stack_size(prog));
    REQUIRE(das_program_simulate(prog, ctx, tout));
    das_function * fn = das_context_find_function(ctx, "bare_resize");
    REQUIRE(fn);
    das_context_eval_with_catch(ctx, fn, nullptr);
    CHECK(das_context_get_exception(ctx) != nullptr);
    das_context_release(ctx);
    das_program_release(prog);
    das_policies_release(pol);
    das_fileaccess_release(fa);
    das_modulegroup_release(mg);
    das_text_release(tout);
}
