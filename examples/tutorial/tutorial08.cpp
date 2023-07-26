#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"

using namespace das;

// function, which we are going to expose to daScript
float xmadd ( float a, float b, float c, float d ) {
    return a*b + c*d;
}

// making custom builtin module
class Module_Tutorial08 : public Module {
public:
    Module_Tutorial08() : Module("tutorial_08") {   // module name, when used from das file
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // adding constant to the module
        addConstant(*this,"SQRT2",sqrtf(2.0));
        // adding function to the module
        addExtern<DAS_BIND_FUN(xmadd)>(*this, lib, "xmadd",SideEffects::none, "xmadd");
    }
};

// registering module, so that its available via 'NEED_MODULE' macro
REGISTER_MODULE(Module_Tutorial08);

#define TUTORIAL_NAME   "/examples/tutorial/tutorial08.das"

void tutorial () {
    TextPrinter tout;                               // output stream for all compiler messages (stdout. for stringstream use TextWriter)
    ModuleGroup dummyLibGroup;                      // module group for compiled program
    auto fAccess = make_smart<FsFileAccess>();      // default file access
    // compile program
    auto program = compileDaScript(getDasRoot() + TUTORIAL_NAME, fAccess, tout, dummyLibGroup);
    if ( program->failed() ) {
        // if compilation failed, report errors
        tout << "failed to compile\n";
        for ( auto & err : program->errors ) {
            tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
        }
        return;
    }
// serialize
    AstSerializer ser;
    program->serialize(ser);
// deserialize
    AstSerializer deser ( ForReading{}, move(ser.buffer) );
    program->serialize(deser);

    // create daScript context
    Context ctx(program->getContextStackSize());
    if ( !program->simulate(ctx, tout) ) {
        // if interpretation failed, report errors
        tout << "failed to simulate\n";
        for ( auto & err : program->errors ) {
            tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
        }
        return;
    }
    // find function 'test' in the context
    auto fnTest = ctx.findFunction("test");
    if ( !fnTest ) {
        tout << "function 'test' not found\n";
        return;
    }
    // verify if 'test' is a function, with the correct signature
    // note, this operation is slow, so don't do it every time for every call
    if ( !verifyCall<void>(fnTest->debugInfo, dummyLibGroup) ) {
        tout << "function 'test', call arguments do not match. expecting def test : void\n";
        return;
    }
    // evaluate 'test' function in the context
    ctx.evalWithCatch(fnTest, nullptr);
    if ( auto ex = ctx.getException() ) {       // if function cased panic, report it
        tout << "exception: " << ex << "\n";
        return;
    }
}

int main( int, char * [] ) {
    // request all da-script built in modules
    NEED_ALL_DEFAULT_MODULES;
    // request our custom module
    NEED_MODULE(Module_Tutorial08);
    // Initialize modules
    Module::Initialize();
    // run the tutorial
    tutorial();
    // shut-down daScript, free all memory
    Module::Shutdown();
    return 0;
}
