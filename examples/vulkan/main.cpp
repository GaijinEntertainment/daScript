#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"
#include "daScript/misc/performance_time.h"
#include "daScript/misc/fpe.h"
#include "daScript/misc/sysos.h"

#ifdef _MSC_VER
#include <io.h>
#else
#include <dirent.h>
#endif

using namespace das;

#if !defined(DAS_GLOBAL_NEW) && defined(_MSC_VER) && !defined(_WIN64)

void * operator new(std::size_t n) throw(std::bad_alloc)
{
    return das_aligned_alloc16(n);
}
void operator delete(void * p) throw()
{
    das_aligned_free16(p);
}

#endif

TextPrinter tout;

bool run_das ( const string & das_path ) {
    uint64_t timeStamp = ref_time_ticks();
    tout << das_path << " ";
    auto fAccess = make_smart<FsFileAccess>();
    ModuleGroup dummyLibGroup;
    CodeOfPolicies policies;
    policies.fail_on_no_aot = true;
    if ( auto program = compileDaScript(das_path, fAccess, tout, dummyLibGroup, false, policies) ) {
        if ( program->failed() ) {
            tout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
            }
            return false;
        } else {
            if (program->unsafe) tout << "[unsafe] ";
            Context ctx(program->getContextStackSize());
            if ( !program->simulate(ctx, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
                }
                return false;
            }
            if ( auto fnTest = ctx.findFunction("main") ) {
                if ( !verifyCall<bool>(fnTest->debugInfo, dummyLibGroup) ) {
                    tout << "function 'main', call arguments do not match\n";
                    return false;
                }
                ctx.restart();
                ctx.runInitScript();    // this is here for testing purposes only
                bool result = cast<bool>::to(ctx.eval(fnTest, nullptr));
                if ( auto ex = ctx.getException() ) {
                    tout << "exception: " << ex << "\n";
                    return false;
                }
                if ( !result ) {
                    tout << "failed\n";
                    return false;
                }
                int usec = get_time_usec(timeStamp);
                tout << "ok " << ((usec/1000)/1000.0) << "\n";
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

int main( int argc, char * argv[] ) {
    if ( argc>2 ) {
        tout << "vulkanExample [pathToDasRoot]\n";
        return -1;
    }  else if ( argc==2 ) {
        setDasRoot(argv[1]);
    }
    setCommandLineArguments(argc,argv);
    _mm_setcsr((_mm_getcsr()&~_MM_ROUND_MASK) | _MM_FLUSH_ZERO_MASK | _MM_ROUND_NEAREST | 0x40);//0x40
    FPE_ENABLE_ALL;

    // register modules
    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_vulkan);

    uint64_t timeStamp = ref_time_ticks();
    bool ok = true;
    ok = run_das(getDasRoot() +  "/examples/vulkan/main.das") && ok;
    int usec = get_time_usec(timeStamp);
    tout << "EXAMPLE " << (ok ? "FINISHED SUCCESSFULLY " : "FAILED!!! ") << ((usec/1000)/1000.0) << "\n";
    // shutdown
    Module::Shutdown();
    return ok ? 0 : -1;
}
