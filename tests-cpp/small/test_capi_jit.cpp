#include <doctest/doctest.h>

#include "daScript/daScriptC.h"

#include <cstdio>
#include <string>

namespace {

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

// the script asserts jit_enabled() and that the [jit] function actually went
// through the JIT — a panic surfaces as das_context_get_exception()
const char * JIT_SCRIPT =
    "options gen2\n"
    "[jit]\n"
    "def sq(x : int) {\n"
    "    return x * x\n"
    "}\n"
    "[export]\n"
    "def main {\n"
    "    if (!jit_enabled()) {\n"
    "        panic(\"jit_enabled() is false\")\n"
    "    }\n"
    "    if (!is_jit_function(@@sq)) {\n"
    "        panic(\"sq was not JIT compiled\")\n"
    "    }\n"
    "    if (sq(7) != 49) {\n"
    "        panic(\"sq(7) != 49\")\n"
    "    }\n"
    "}\n";

bool fileExists ( const std::string & path ) {
    if ( FILE * f = fopen(path.c_str(), "rb") ) {
        fclose(f);
        return true;
    }
    return false;
}

}

TEST_CASE("C API set root round trip") {
    char root[4096];
    das_get_root(root, int(sizeof(root)));
    std::string dasRoot = root;
    REQUIRE(!dasRoot.empty());
    // a root DIFFERENT from the derived one, set through the explicit-length form
    // with a non-NUL-terminated range - a no-op das_set_root fails both checks
    const char newRoot[] = {'/','n','o','/','s','u','c','h','/','r','o','o','t','X'};
    das_set_root_n(newRoot, sizeof(newRoot) - 1);
    char root2[4096];
    das_get_root(root2, int(sizeof(root2)));
    CHECK(std::string(root2) == "/no/such/root");
    das_set_root(dasRoot.c_str());
    das_get_root(root2, int(sizeof(root2)));
    CHECK(dasRoot == root2);
}

TEST_CASE("C API jit enable") {
    char root[4096];
    das_get_root(root, int(sizeof(root)));
    std::string dasRoot = root;
    if ( !fileExists(dasRoot + "/lib/LLVM.dll") ) {
        MESSAGE("no lib/LLVM.dll in this build tree - skipping the JIT C API test");
        return;
    }

    CApiProgram capi;
    capi.output = das_text_make_writer();
    capi.modules = das_modulegroup_make();
    capi.files = das_fileaccess_make_default();
    capi.policies = das_policies_make();

    // the embedded mirror of the CLI's -jit switch: the policy flag, the
    // just_in_time extra module, and dynamic-module resolution for dasLLVM
    REQUIRE(das_policies_set_bool(capi.policies, DAS_POLICY_JIT_ENABLED, 1) == 1);
    // off = the in-memory arm on every build flavor - the path the darwin fix targets
    REQUIRE(das_policies_set_bool(capi.policies, DAS_POLICY_JIT_DLL_MODE, 0) == 1);
    REQUIRE(das_register_dynamic_modules(capi.files, dasRoot.c_str(), nullptr, 0, capi.output) == 0);
    std::string jitModule = dasRoot + "/daslib/just_in_time.das";
    das_fileaccess_add_extra_module(capi.files, "just_in_time", jitModule.c_str());

    das_fileaccess_introduce_file(capi.files, "jit_script.das", JIT_SCRIPT, 0);
    const char scriptName[] = "jit_script.das";
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
// a regression of the darwin dtor-skip shows up as SIGSEGV at process exit() -
// AFTER doctest prints SUCCESS; the exit code is the signal, and ctest reads it
