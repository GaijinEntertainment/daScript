#include "precomp.h"

#include "ast.h"

#include <fstream>
#include <iostream>

#ifdef _MSC_VER
	#include <io.h>
#else
	#include <dirent.h>
#endif

using namespace std;
using namespace das;

bool g_reportCompilationFailErrors = false;

bool compilation_fail_test ( const string & fn ) {
    cout << fn << " ";
    string str;
    ifstream t(fn);
    if ( !t.is_open() ) {
        cout << "not found\n";
        return false;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str(), cout) ) {
        if ( program->failed() ) {
            bool failed = false;
            auto errors = program->expectErrors;
            for ( auto err : program->errors ) {
                int count = -- errors[err.cerr];
                if ( g_reportCompilationFailErrors || count<0 ) {
                    cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
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
                cout << "failed";
                if ( any_errors ) {
                    cout << ", expecting errors";
                    for ( auto cerr : errors  ) {
                        if ( cerr.second > 0 ) {
                            cout << " " << int(cerr.first) << ":" << cerr.second;
                        }
                    }
                }
                cout << "\n";
                return false;
            }
            cout << "ok\n";
            return true;
        } else {
            cout << "failed, compiled without errors\n";
            return false;
        }
    } else {
        cout << "failed, not expected to compile\n";
        return false;
    }
}

bool unit_test ( const string & fn ) {
    cout << fn << " ";
    string str;
    ifstream t(fn);
    if ( !t.is_open() ) {
        cout << "not found\n";
        return false;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str(), cout) ) {
        if ( program->failed() ) {
            cout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
            }
            return false;
        } else {
            Context ctx(&str);
            program->simulate(ctx);
            int fnTest = ctx.findFunction("test");
            if ( fnTest != -1 ) {
                ctx.restart();
                bool result = cast<bool>::to(ctx.eval(fnTest, nullptr));
                if ( auto ex = ctx.getException() ) {
                    cout << "exception: " << ex << "\n";
                    return false;
                }
                if ( !result ) {
                    cout << "failed\n";
                    return false;
                }
                cout << "ok\n";
                return true;
            } else {
                cout << "function 'test' not found\n";
                return false;
            }
        }
    } else {
        return false;
    }
}

bool exception_test ( const string & fn ) {
    cout << fn << " ";
    string str;
    ifstream t(fn);
    if ( !t.is_open() ) {
        cout << "not found\n";
        return false;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str(), cout) ) {
        if ( program->failed() ) {
            cout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
            }
            return false;
        } else {
            Context ctx(&str);
            program->simulate(ctx);
            int fnTest = ctx.findFunction("test");
            if ( fnTest != -1 ) {
                ctx.restart();
                ctx.eval(fnTest, nullptr);
                if ( auto ex = ctx.getException() ) {
                    cout << "ok\n";
                    return true;
                }
                cout << "failed, finished without exception\n";
                return false;
            } else {
                cout << "function 'test' not found\n";
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

int main(int argc, const char * argv[]) {
#ifdef _MSC_VER
	#define	TEST_PATH "../"
#else
	#define TEST_PATH "../../"
#endif
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_UnitTest);
#if 0 // Debug this one test
    compilation_fail_test(TEST_PATH "test/compilation_fail_tests/new_type_infer.das");
    Module::Shutdown();
    return 0;
#endif
#if 0 // Debug this one test
    unit_test(TEST_PATH "test/hello_world.das");
    Module::Shutdown();
    return 0;
#endif
    bool ok = true;
    ok = run_compilation_fail_tests(TEST_PATH "test/compilation_fail_tests") && ok;
    ok = run_unit_tests(TEST_PATH "test/unit_tests") && ok;
    ok = run_unit_tests(TEST_PATH "test/optimizations") && ok;
    ok = run_exception_tests(TEST_PATH "test/runtime_errors") && ok;
    cout << "TESTS " << (ok ? "PASSED" : "FAILED!!!") << "\n";
    // shutdown
    Module::Shutdown();
    return ok ? 0 : -1;
}
