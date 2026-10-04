#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/daScriptC.h"
#include "daScript/simulate/aot.h"

#include <cstdio>
#include <string>

using namespace das;

namespace {
    bool invoke_ex_bool ( const TBlock<bool> & blk, Context * context, LineInfoArg * at ) {
        context->abiResult() = v_zero();
        vec4f res = context->invokeEx(blk, nullptr, nullptr, [&](SimNode * code) {
            code->eval(*context);
        }, at);
        return cast<bool>::to(res);
    }
}

class Module_JitBlockResultTest : public Module {
public:
    Module_JitBlockResultTest() : Module("jit_block_result_test") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        addExtern<DAS_BIND_FUN(invoke_ex_bool)>(*this, lib, "invoke_ex_bool",
            SideEffects::invoke, "invoke_ex_bool")
                ->args({"blk","context","at"});
    }
};
REGISTER_MODULE(Module_JitBlockResultTest);

namespace {

const char * JIT_BLOCK_SCRIPT =
    "options gen2\n"
    "require jit_block_result_test\n"
    "[jit]\n"
    "def run : bool {\n"
    "    return invoke_ex_bool() $() {\n"
    "        return true\n"
    "    }\n"
    "}\n"
    "[export]\n"
    "def main {\n"
    "    if (!is_jit_function(@@run)) {\n"
    "        panic(\"run was not JIT compiled\")\n"
    "    }\n"
    "    if (!run()) {\n"
    "        panic(\"invokeEx returned a stale result for a jitted block\")\n"
    "    }\n"
    "}\n";

bool fileExists ( const std::string & path ) {
    if ( FILE * f = fopen(path.c_str(), "rb") ) {
        fclose(f);
        return true;
    }
    return false;
}

struct CApiProgram {
    das_text_writer * output = nullptr;
    das_module_group * modules = nullptr;
    das_file_access * files = nullptr;
    das_policies * policies = nullptr;
    das_program * program = nullptr;
    das_context * context = nullptr;

    ~CApiProgram() {
        if ( context ) das_context_release(context);
        if ( program ) das_program_release(program);
        if ( policies ) das_policies_release(policies);
        if ( files ) das_fileaccess_release(files);
        if ( modules ) das_modulegroup_release(modules);
        if ( output ) das_text_release(output);
    }
};

}

TEST_CASE("invokeEx returns the result of a jitted block") {
    char root[4096];
    das_get_root(root, int(sizeof(root)));
    std::string dasRoot = root;
    if ( !fileExists(dasRoot + "/lib/LLVM.dll") ) {
        MESSAGE("no lib/LLVM.dll in this build tree - skipping the jitted block invokeEx test");
        return;
    }

    CApiProgram capi;
    capi.output = das_text_make_writer();
    capi.modules = das_modulegroup_make();
    capi.files = das_fileaccess_make_default();
    capi.policies = das_policies_make();
    REQUIRE(das_policies_set_bool(capi.policies, DAS_POLICY_JIT_ENABLED, 1) == 1);
    REQUIRE(das_policies_set_bool(capi.policies, DAS_POLICY_JIT_DLL_MODE, 0) == 1);
    REQUIRE(das_register_dynamic_modules(capi.files, dasRoot.c_str(), nullptr, 0, capi.output) == 0);
    std::string jitModule = dasRoot + "/daslib/just_in_time.das";
    das_fileaccess_add_extra_module(capi.files, "just_in_time", jitModule.c_str());

    das_fileaccess_introduce_file(capi.files, "jit_block_result.das", JIT_BLOCK_SCRIPT, 0);
    const char scriptName[] = "jit_block_result.das";
    capi.program = das_program_compile_policies_n(scriptName, sizeof(scriptName) - 1,
        capi.files, capi.output, capi.modules, capi.policies);
    REQUIRE(capi.program != nullptr);
    if ( das_program_err_count(capi.program) != 0 ) {
        for ( int i = 0; i != das_program_err_count(capi.program); ++i ) {
            das_error * err = das_program_get_error(capi.program, i);
            char buf[2048];
            das_error_report(err, buf, int(sizeof(buf)));
            MESSAGE(buf);
        }
        FAIL("compilation failed");
    }

    capi.context = das_context_make(das_program_context_stack_size(capi.program));
    REQUIRE(capi.context != nullptr);
    REQUIRE(das_program_simulate(capi.program, capi.context, capi.output) == 1);

    das_function * fnMain = das_context_find_function(capi.context, "main");
    REQUIRE(fnMain != nullptr);
    das_context_eval_with_catch(capi.context, fnMain, nullptr);
    char * exception = das_context_get_exception(capi.context);
    CHECK_MESSAGE(exception == nullptr, std::string(exception ? exception : ""));
}
