#include "daScript/daScript.h"
#include "msvc32.inc"

using namespace das;

#define TUTORIAL_NAME   "/examples/tutorial/tutorial01.das"

typedef map<string, float> ExprVars;

class ExprCalc {
public:
    ExprCalc ( const string & expr, const ExprVars & vars );
    float compute(ExprVars & vars);
protected:
    ContextPtr      ctx;
    SimFunction *   fni = nullptr;
    string          text;
    map<string,int> variables;
};

float ExprCalc::compute(ExprVars & vars) {
    if ( fni==nullptr ) {
        return 0.0f;
    }
    for ( auto & va : vars ) {
        auto it = variables.find(va.first);
        if ( it != variables.end() ) {
            auto vidx = it->second;
            if ( auto pvar = (float *) ctx->getVariable(vidx) ) {
                *pvar = va.second;
            }
        }
    }
    auto res = ctx->evalWithCatch(fni, nullptr);
    if ( auto ex = ctx->getException() ) {
        TextPrinter tout;
        tout << "with exception " << ex << "\n";
        return 0.0f;
    }
    return cast<float>::to(res);
}

ExprCalc::ExprCalc ( const string & expr, const ExprVars & vars ) {
    // build expression
    TextWriter ss;
    // ss  << "options log\n";
    ss  << "require math\n";
    for ( auto & va : vars ) {
        ss << "var " << va.first << " = 0.0f\n";
    }
    ss  << "[export]\n"
        << "def test\n"
        << "\treturn " << expr << "\n"
        << "\n\n";
    text = ss.str();
    // make file access, introduce string as if it was a file
    TextPrinter tout;
    auto fAccess = make_smart<FsFileAccess>();
    auto fileInfo = make_unique<FileInfo>(text.c_str(), uint32_t(text.length()));
    fAccess->setFileInfo("dummy.das", move(fileInfo));
    // compile script
    ModuleGroup dummyLibGroup;
    auto program = compileDaScript("dummy.das", fAccess, tout, dummyLibGroup);
    if ( program->failed() ) {
        // if compilation failed, report errors
        tout << "failed to compile\n";
        for ( auto & err : program->errors ) {
            tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
        }
        return;
    }
    // create context
    ctx = make_shared<Context>(program->getContextStackSize());
    if ( !program->simulate(*ctx, tout) ) {
        // if interpretation failed, report errors
        tout << "failed to simulate\n";
        for ( auto & err : program->errors ) {
            tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
        }
        return;
    }
    // call context function
    fni = ctx->findFunction("test");
    // variables
    for ( auto & va : vars ) {
        auto idx = ctx->findVariable(va.first.c_str());
        assert(idx != -1);
        variables[va.first] = idx;
    }
}

int main( int, char * [] ) {
    // request all da-script built in modules
    NEED_ALL_DEFAULT_MODULES;
    // Initialize modules
    Module::Initialize();

    TextPrinter tp;

    ExprVars variables;
    variables["a"] = 1.0f;
    variables["b"] = 2.0f;
    variables["c"] = 3.0f;

    ExprCalc calc("a*b+c", variables);

    auto res = calc.compute(variables);
    assert(res==5.0f);

    for ( float a=1.0f; a<4.0f; a++ ) {
        variables["a"] = a;
        for ( float b=1.0f; b<5.0f; b++ ) {
            variables["b"] = b;
            for ( float c=1.0f; c<6.0f; c++ ) {
                variables["c"] = c;
                float rref = a * b + c;
                float reval = calc.compute(variables);
                assert(rref==reval);
            }
        }
    }


    // shut-down daScript, free all memory
    Module::Shutdown();
    return 0;
}

