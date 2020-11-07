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

bool unit_test ( const string & fn ) {
    uint64_t timeStamp = ref_time_ticks();
    tout << fn << " ";
    auto fAccess = make_smart<FsFileAccess>();
    ModuleGroup dummyLibGroup;
    CodeOfPolicies policies;
    policies.fail_on_no_aot = true;
    // policies.intern_strings = true;
    // policies.intern_const_strings = true;
    // policies.no_unsafe = true;
    if ( auto program = compileDaScript(fn, fAccess, tout, dummyLibGroup, false, policies) ) {
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
            if ( auto fnTest = ctx.findFunction("test") ) {
                if ( !verifyCall<bool>(fnTest->debugInfo, dummyLibGroup) ) {
                    tout << "function 'test', call arguments do not match\n";
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
                tout << "function 'test' not found\n";
                return false;
            }
        }
    } else {
        return false;
    }
}

bool run_tests( const string & path, bool (*test_fn)(const string &) ) {
#ifdef _MSC_VER
    bool ok = true;
    _finddata_t c_file;
    intptr_t hFile;
    string findPath = path + "/*.das";
    if ((hFile = _findfirst(findPath.c_str(), &c_file)) != -1L) {
        do {
            const char * atDas = strstr(c_file.name,".das");
            if ( atDas && strcmp(atDas,".das")==0 && c_file.name[0]!='_' ) {
                ok = test_fn(path + "/" + c_file.name) && ok;
            }
        } while (_findnext(hFile, &c_file) == 0);
    }
    _findclose(hFile);
    return ok;
#else
    bool ok = true;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            const char * atDas = strstr(ent->d_name,".das");
            if ( atDas && strcmp(atDas,".das")==0 && ent->d_name[0]!='_' ) {
                ok = test_fn(path + "/" + ent->d_name) && ok;
            }
        }
        closedir (dir);
    }
    return ok;
#endif
}

bool run_unit_tests(const string & path) {
    return run_tests(path, unit_test);
}

extern int das_yydebug;

int main( int argc, char * argv[] ) {
    if ( argc>2 ) {
        tout << "dasBinderTest [pathToDasRoot]\n";
        return -1;
    }  else if ( argc==2 ) {
        setDasRoot(argv[1]);
    }
    setCommandLineArguments(argc,argv);
    _mm_setcsr((_mm_getcsr()&~_MM_ROUND_MASK) | _MM_FLUSH_ZERO_MASK | _MM_ROUND_NEAREST | 0x40);//0x40
    FPE_ENABLE_ALL;

    // register modules
    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_generatedBindings);

    uint64_t timeStamp = ref_time_ticks();
    bool ok = true;
    ok = run_unit_tests(getDasRoot() +  "/examples/test/binder") && ok;
    int usec = get_time_usec(timeStamp);
    tout << "TESTS " << (ok ? "PASSED " : "FAILED!!! ") << ((usec/1000)/1000.0) << "\n";
    // shutdown
    Module::Shutdown();
    return ok ? 0 : -1;
}

