// Tutorial 22 — Namespace Integration
//
// Demonstrates how to initialize daslang modules inside a C++ namespace.
//
// Built-in registration entry points are globally declared with C linkage, so
// NEED_ALL_DEFAULT_MODULES is safe inside a namespace. Separately compiled
// custom modules use the DECLARE_MODULE / PULL_MODULE macro pair:
//   • DECLARE_MODULE  — forward-declares the register function (must be at
//                        file/global scope so it refers to the global symbol)
//   • PULL_MODULE     — calls the register function using ::qualified name
//                        (safe inside any namespace, class, or function body)
//
// NEED_MODULE and PULL_MODULE both call the global C-linkage entry point.

#include "daScript/daScript.h"

using namespace das;

#define SCRIPT_NAME "/tutorials/integration/cpp/01_hello_world.das"

// Separately compiled external modules are declared at global/file scope:
//   #include "modules/external_declare.inc"

// ------------------------------------------------------------------
// Everything below lives inside a namespace — PULL_MODULE works here.
// ------------------------------------------------------------------
namespace MyApp {

void runScript() {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + SCRIPT_NAME,
                                   fAccess, tout, dummyLibGroup);
    if (program->failed()) {
        tout << "Compilation failed:\n";
        for (auto & err : program->errors) {
            tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
        }
        return;
    }
    Context ctx(program->getContextStackSize());
    if (!simulateProgram(program, ctx, tout)) {
        tout << "Simulation failed:\n";
        for (auto & err : program->errors) {
            tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
        }
        return;
    }
    auto fnTest = ctx.findFunction("test");
    if (!fnTest) {
        tout << "Function 'test' not found\n";
        return;
    }
    ctx.evalWithCatch(fnTest, nullptr);
    if (auto ex = ctx.getException()) {
        tout << "Script exception: " << ex << "\n";
    }
}

void initialize() {
    // Built-ins are already globally declared by daScriptModule.h. The macro
    // calls their ::register_Module_* C entry points explicitly.
    NEED_ALL_DEFAULT_MODULES;

    // For external (plugin) modules, include the generated file:
    //   #include "modules/external_pull.inc"

    Module::Initialize();
}

void shutdown() {
    Module::Shutdown();
}

} // namespace MyApp

// main() delegates to the namespaced functions.
int main(int, char * []) {
    MyApp::initialize();
    MyApp::runScript();
    MyApp::shutdown();
    return 0;
}
