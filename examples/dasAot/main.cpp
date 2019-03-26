#include "daScript/daScript.h"
#include "../common/fileAccess.h"

using namespace das;

TextPrinter tout;

bool saveToFile ( const string & fname, const string & str ) {
    FILE * f = fopen ( fname.c_str(), "w" );
    if ( !f ) {
        tout << "can't open " << fname << "\n";
        return false;
    }
    fwrite ( str.c_str(), str.length(), 1, f );
    return true;
}

bool compile ( const string & fn, const string & mainInc, const string & registerInc ) {
    auto access = make_shared<FsFileAccess>();
    ModuleGroup dummyGroup;
    if ( auto program = compileDaScript(fn,access,tout,dummyGroup) ) {
        if ( program->failed() ) {
            for ( auto & err : program->errors ) {
                tout << reportError(err.at, err.what, err.cerr );
            }
            return false;
        } else {
            Context ctx;
            if ( !program->simulate(ctx, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.cerr );
                }
                return false;
            }
            // AOT time
            TextWriter mainTw, registerTw;
            program->aotCpp(mainTw);
            program->registerAotCpp(registerTw, ctx, false);
            // and save
            return saveToFile(mainInc, mainTw.str()) && saveToFile(registerInc, registerTw.str());
        }
    } else {
        return false;
    }
}

int main(int argc, const char * argv[]) {
    if ( argc!=4 ) {
        tout << "dasAot [script.das] [script.main.inc] [script.decl.inc] [NAMESPACE]\n";
        return -1;
    }
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_Random);
    NEED_MODULE(Module_PathTracerHelper);
    bool compiled = compile(argv[1], argv[2], argv[3]);
    Module::Shutdown();
    return compiled ? 0 : -1;
}


