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

mutex global_printer;

class TextPrinterMT : public TextWriter {
public:
    virtual void output() override {
        int newPos = tellp();
        if (newPos != pos) {
            string st(data.data() + pos, newPos - pos);
            lock_guard<mutex> lock(global_printer);
            printf("%s", st.c_str());
            fflush(stdout);
            pos = newPos;
        }
    }
protected:
    int pos = 0;
};

#include <sstream>

bool VerboseTests = false;

string this_thread_id() {
    stringstream ss;
    ss << this_thread::get_id();
    return ss.str();
}

bool g_reportCompilationFailErrors = false;

bool performance_test ( const string & fn, bool useAot ) {
    TextPrinterMT tout;
    if ( VerboseTests )
        tout << "[" << this_thread_id() << "]";
    // tout << fn << "\n";
    auto fAccess = make_smart<FsFileAccess>();
    ModuleGroup dummyLibGroup;
    CodeOfPolicies policies;
    policies.aot = useAot;
    policies.fail_on_no_aot = true;
    // policies.intern_strings = true;
    // policies.intern_const_strings = true;
    // policies.no_unsafe = true;
    if ( auto program = compileDaScript(fn, fAccess, tout, dummyLibGroup, false, policies) ) {
        if ( program->failed() ) {
            tout << fn << " failed to compile (" << this_thread_id() << ")\n";
            for ( auto & err : program->errors ) {
                tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
            }
            return false;
        } else {
            Context ctx(program->getContextStackSize());
            if ( !program->simulate(ctx, tout) ) {
                tout << fn << " failed to simulate(" << this_thread_id() << ")\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
                }
                return false;
            }
            return true;
        }
    } else {
        return false;
    }
}


bool run_tests( const string & path, bool (*test_fn)(const string &, bool useAot), bool useAot ) {
#ifdef _MSC_VER
    bool ok = true;
    _finddata_t c_file;
    intptr_t hFile;
    string findPath = path + "/*.das";
    if ((hFile = _findfirst(findPath.c_str(), &c_file)) != -1L) {
        do {
            const char * atDas = strstr(c_file.name,".das");
            if ( atDas && strcmp(atDas,".das")==0 && c_file.name[0]!='_' ) {
                ok = test_fn(path + "/" + c_file.name, useAot) && ok;
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
                ok = test_fn(path + "/" + ent->d_name, useAot) && ok;
            }
        }
        closedir (dir);
    }
    return ok;
#endif
}

#include <smmalloc.h>

void sm_thread_cache_create();
void sm_thread_cache_destroy();

void test_thread() {
    sm_thread_cache_create();

    TextPrinterMT tout;
    tout << "test_thread: " << this_thread_id() << "\n";
    if ( VerboseTests )
        tout << "NEED MODULE (" << this_thread_id() << ")\n";
    uint64_t timeStamp0 = ref_time_ticks();
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_Strings);
    NEED_MODULE(Module_UnitTest);
    NEED_MODULE(Module_Rtti);
    NEED_MODULE(Module_Ast);
    NEED_MODULE(Module_Debugger);
    NEED_MODULE(Module_Network);
    NEED_MODULE(Module_UriParser);
    NEED_MODULE(Module_JobQue);
    NEED_MODULE(Module_FIO);
    NEED_MODULE(Module_DASBIND);
    if ( VerboseTests )
        tout << "Module::Initialize (" << this_thread_id() << ")\n";
    Module::Initialize();
    int usec0 = get_time_usec(timeStamp0);
    tout << "Initialized in " << ((usec0/1000)/1000.0) << " (" << this_thread_id() << ")\n";
    // run em
    uint64_t timeStamp = ref_time_ticks();
    if ( !run_tests(getDasRoot() +  "/examples/test/unit_tests", performance_test, false) ) {
        if ( VerboseTests )
            tout << "TESTS FAILED (" << this_thread_id() << ")\n";
    } else {
        int usec = get_time_usec(timeStamp);
        tout << "Passed in " << ((usec/1000)/1000.0) << " (" << this_thread_id() << ")\n";
    }
    if ( VerboseTests )
        tout << "Module::Shutdown (" << this_thread_id() << ")\n";
    Module::Shutdown();

    sm_thread_cache_destroy();
}

int main( int argc, char * argv[] ) {
    TextPrinterMT tout;
    if ( argc>2 ) {
        tout << "daScriptTestThreads [pathToDasRoot]\n";
        return -1;
    }  else if ( argc==2 ) {
        setDasRoot(argv[1]);
    }
    setCommandLineArguments(argc,argv);

#if 0
    test_thread();
#else
    vector<thread> THREADS;
    auto total_threads = 4; // max(1,int(thread::hardware_concurrency()));
    for ( int i=0; i<total_threads; ++i ) {
        THREADS.emplace_back(thread([=](){
            test_thread();
        }));
    }
    for ( auto & th : THREADS ) {
        th.join();
    }
#endif
    return 0;
}

