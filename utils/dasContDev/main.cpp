#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"
#include "daScript/misc/fpe.h"
#include "daScript/misc/sysos.h"

using namespace das;

TextPrinter tout;

#include <sys/stat.h>


#if defined(_MSC_VER)
#else
    #include <unistd.h>
#endif

void wait_for_file_to_change ( const string & fn ) {
    time_t ft = 0;
    for ( ;; ) {
        struct stat st;
#if defined(_MSC_VER)
        if ( !stat(fn.c_str(),&st) ) {
            if ( ft==0 ) {
                ft = st.st_mtime;
            }
            if ( ft!=st.st_mtime ) {
                return;
            }
        }
        _sleep(100);
#else
        if ( !stat(fn.c_str(),&st) ) {
            if ( ft==0 ) {
                ft = st.st_mtime;
            }
            if ( ft!=st.st_mtime ) {
                return;
            }
        }
        usleep(100);
#endif
    }
}

bool compile_and_run(const string& fn, bool recompile) {
    auto access = make_smart<FsFileAccess>();
    ModuleGroup dummyGroup;
    CodeOfPolicies policies;
    policies.ignore_shared_modules = recompile;
    if ( auto program = compileDaScript(fn,access,tout,dummyGroup, false, policies) ) {
        if ( program->failed() ) {
            for ( auto & err : program->errors ) {
                tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
            }
            return false;
        } else {
            Context ctx(program->getContextStackSize());
            if ( !program->simulate(ctx, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
                }
                return false;
            }
            if ( auto fnTest = ctx.findFunction("main") ) {
                if ( verifyCall<void>(fnTest->debugInfo, dummyGroup) || verifyCall<bool>(fnTest->debugInfo, dummyGroup) ) {
                    ctx.restart();
                    ctx.eval(fnTest, nullptr);
                } else {
                    tout << "function 'main' call arguments do not match, expecting main():void or main():bool\n";
                }
                return true;
            } else {
                tout << "function 'main' not found\n";
                return false;
            }
        }
    } else {
        return false;
    }
}

void require_project_specific_modules();//link time resolved dependencies

int main(int argc, char * argv[]) {
    setCommandLineArguments(argc,argv);
    _mm_setcsr((_mm_getcsr()&~_MM_ROUND_MASK) | _MM_FLUSH_ZERO_MASK | _MM_ROUND_NEAREST | 0x40);//0x40
    FPE_ENABLE_ALL;
    string main_das;
    if ( argc<2 ) {
        tout << "dasContDev [script.das] {arguments}\n";
        return -1;
    }
    main_das = argv[1];
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_Strings);
    NEED_MODULE(Module_Rtti);
    NEED_MODULE(Module_Ast);
    NEED_MODULE(Module_Debugger);
    NEED_MODULE(Module_Network);
    NEED_MODULE(Module_UriParser);
    NEED_MODULE(Module_JobQue);
    NEED_MODULE(Module_FIO);
    NEED_MODULE(Module_DASBIND);
    require_project_specific_modules();
    #include "modules/external_need.inc"
    Module::Initialize();
    bool recompile = false;
    for ( ;; ) {
        if ( !compile_and_run(main_das, recompile) ) {
            wait_for_file_to_change(main_das);
        }
        recompile = true;
    }
    Module::Shutdown();
    return 0;
}


