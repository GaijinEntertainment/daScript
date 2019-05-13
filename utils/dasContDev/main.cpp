#include "daScript/daScript.h"
#include "../examples/common/fileAccess.h"

using namespace das;

TextPrinter tout;

#include <sys/stat.h>
#include <unistd.h>

void wait_for_file_to_change ( const char * fn ) {
    time_t ft = 0;
    for ( ;; ) {
        struct stat st;
        if ( !stat(fn,&st) ) {
            if ( ft==0 ) {
                ft = st.st_mtimespec.tv_sec;
            }
            if ( ft!=st.st_mtimespec.tv_sec ) {
                return;
            }
        }
        usleep(100);
    }
}

bool compile_and_run ( const char * fn ) {
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
            if ( auto fnTest = ctx.findFunction("main") ) {
                ctx.restart();
                vec4f args[1] = {
                    cast<char *>::from(fn)
                };
                ctx.eval(fnTest, args);
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

int main(int argc, const char * argv[]) {
    if ( argc!=2 ) {
        tout << "dasContDev [script.das]\n";
        return -1;
    }
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_Random);
    NEED_MODULE(Module_Rtti);
    NEED_MODULE(Module_FIO);
    for ( ;; ) {
        if ( !compile_and_run(argv[1]) ) {
            wait_for_file_to_change(argv[1]);
        }
    }
    Module::Shutdown();
    return 0;
}


