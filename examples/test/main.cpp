#include "daScript/daScript.h"
#include "../common/fileAccess.h"

using namespace das;

bool g_reportCompilationFailErrors = false;

TextPrinter tout;

bool compilation_fail_test ( const string & fn ) {
    tout << fn << " ";
    auto access = make_shared<FsFileAccess>();
    ModuleGroup dummyLibGroup;
    if ( auto program = compileDaScript(fn, access, tout, dummyLibGroup) ) {
        if ( program->failed() ) {
            bool failed = false;
            auto errors = program->expectErrors;
            for ( auto err : program->errors ) {
                int count = -- errors[err.cerr];
                if ( g_reportCompilationFailErrors || count<0 ) {
                    tout << reportError(err.at, err.what, err.cerr );
                }
                if ( count <0 ) {
                    failed = true;
                }
            }
            bool any_errors = false;
            for ( auto err : errors ) {
                if ( err.second > 0 ) {
                    any_errors = true;
                    break;
                }
            }
            if ( any_errors || failed ) {
                tout << "failed";
                if ( any_errors ) {
                    tout << ", expecting errors";
                    for ( auto terr : errors  ) {
                        if (terr.second > 0 ) {
                            tout << " " << int(terr.first) << ":" << terr.second;
                        }
                    }
                }
                tout << "\n";
                return false;
            }
            tout << "ok\n";
            return true;
        } else {
            tout << "failed, compiled without errors\n";
            return false;
        }
    } else {
        tout << "failed, not expected to compile\n";
        return false;
    }
}

bool unit_test ( const string & fn ) {
    tout << fn << " ";
    auto access = make_shared<FsFileAccess>();
    ModuleGroup dummyLibGroup;
    if ( auto program = compileDaScript(fn, access, tout, dummyLibGroup) ) {
        if ( program->failed() ) {
            tout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                tout << reportError(err.at, err.what, err.cerr );
            }
            return false;
        } else {
            Context ctxBase;
            if ( !program->simulate(ctxBase, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.cerr );
                }
                return false;
            }
            // note: copy of the context is here for testing purposes only
            //          that way we test context-copying functionality every time
            Context ctx(ctxBase);
            if ( auto fnTest = ctx.findFunction("test") ) {
                ctx.restart();
                bool result = cast<bool>::to(ctx.eval(fnTest, nullptr));
                if ( auto ex = ctx.getException() ) {
                    tout << "exception: " << ex << "\n";
                    return false;
                }
                if ( !result ) {
                    tout << "failed\n";
                    return false;
                }
                tout << "ok\n";
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

bool exception_test ( const string & fn ) {
    tout << fn << " ";
    auto access = make_shared<FsFileAccess>();
    ModuleGroup dummyLibGroup;
    if ( auto program = compileDaScript(fn, access, tout, dummyLibGroup) ) {
        if ( program->failed() ) {
            tout << "failed to compile\n";
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
            if ( auto fnTest = ctx.findFunction("test") ) {
                ctx.restart();
                ctx.evalWithCatch(fnTest, nullptr);
                if ( auto ex = ctx.getException() ) {
                    tout << "ok\n";
                    return true;
                }
                tout << "failed, finished without exception\n";
                return false;
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
            ok = test_fn(path + "/" + c_file.name) && ok;
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
            if ( strstr(ent->d_name,".das") ) {
                ok = test_fn(path + "/" + ent->d_name) && ok;
            }
        }
        closedir (dir);
    }
    return ok;
#endif
}

bool run_unit_tests( const string & path ) {
    return run_tests(path, unit_test);
}

bool run_compilation_fail_tests( const string & path ) {
    return run_tests(path, compilation_fail_test);
}

bool run_exception_tests( const string & path ) {
    return run_tests(path, exception_test);
}

int main() {
#ifdef _MSC_VER
    #define    TEST_PATH "../"
#else
    #define TEST_PATH "../../"
#endif
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_UnitTest);
#if 0 // Debug this one test
    compilation_fail_test(TEST_PATH "examples/test/compilation_fail_tests/global.das");
    Module::Shutdown();
    return 0;
#endif
#if 0 // Debug this one test
    unit_test(TEST_PATH "examples/test/unit_tests/lambda.das");
    Module::Shutdown();
    return 0;
#endif
    bool ok = true;
    ok = run_compilation_fail_tests(TEST_PATH "examples/test/compilation_fail_tests") && ok;
    ok = run_unit_tests(TEST_PATH "examples/test/unit_tests") && ok;
    ok = run_unit_tests(TEST_PATH "examples/test/optimizations") && ok;
    ok = run_exception_tests(TEST_PATH "examples/test/runtime_errors") && ok;
    tout << "TESTS " << (ok ? "PASSED" : "FAILED!!!") << "\n";
    // shutdown
    Module::Shutdown();
    return ok ? 0 : -1;
}
