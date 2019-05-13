#include "daScript/daScript.h"
#include "../examples/common/fileAccess.h"

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
            // lets comment on required modules
            program->library.foreach([&](Module * mod){
                if ( mod->name=="" ) {
                    // nothing, its main program module. i.e ::
                } else if ( mod->name=="$" ) {
                    mainTw << " // require builtin\n";
                } else {
                    mainTw << " // require " << mod->name << "\n";
                }
                return true;
            },"*");
            // sample
#if 0
            for (const auto & st : program->thisModule->structuresInOrder) {
                mainTw << " // structure " << st->name << "\n";
                for (const auto & fl : st->fields) {
                    mainTw << " //     " << describeCppType(fl.type,true) << " " << fl.name << ";";
                    if (fl.annotation.arguments.size()) {
                        mainTw << " // ";
                        for (const auto & arg : fl.annotation.arguments) {
                            mainTw << " " << arg.name << "=";
                            switch (arg.type) {
                            case Type::tInt:    mainTw << arg.iValue; break;
                            case Type::tFloat:  mainTw << arg.fValue; break;
                            case Type::tBool:   mainTw << (arg.bValue ? "true" : "false"); break;
                            case Type::tString: mainTw << arg.sValue; break;
                            default:    mainTw << "??"; break;
                            }
                        }
                    }
                    mainTw << "\n";
                }
            }
#endif
            // AOT actual
            program->aotCpp(ctx, mainTw);
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
    NEED_MODULE(Module_TestProfile);
    NEED_MODULE(Module_UnitTest);
    bool compiled = compile(argv[1], argv[2], argv[3]);
    Module::Shutdown();
    return compiled ? 0 : -1;
}


