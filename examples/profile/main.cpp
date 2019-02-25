#include "daScript/daScript.h"
#include "../common/fileAccess.h"

using namespace das;
#include "test_profile.h"

TextPrinter tout;
bool unit_test ( const string & fn ) {
    auto access = make_shared<FsFileAccess>();
    ModuleGroup dummyGroup;
    if ( auto program = compileDaScript(fn,access,tout,dummyGroup) ) {
        if ( program->failed() ) {
            tout << fn << " failed to compile\n";
            for ( auto & err : program->errors ) {
                tout << reportError(err.at, err.what, err.cerr );
            }
            return false;
        } else {
            // tout << *program << "\n";
            Context ctx;
            if ( !program->simulate(ctx, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.cerr );
                }
                return false;
            }
            // vector of 10000 objects
            vector<Object> objects;
            objects.resize(10000);
            // run the test
            if ( auto fnTest = ctx.findFunction("test") ) {
                ctx.restart();
                vec4f args[1] = { cast<vector<Object> *>::from(&objects) };
                bool result = cast<bool>::to(ctx.eval(fnTest, args));
                if ( auto ex = ctx.getException() ) {
                    tout << fn << ", exception: " << ex << "\n";
                    return false;
                }
                if ( !result ) {
                    tout << fn << ", failed\n";
                    return false;
                }
                // tout << "ok\n";
                return true;
            } else {
                tout << fn << ", function 'test' not found\n";
                return false;
            }
        }
    } else {
        return false;
    }
}

bool run_tests( const string & path, bool (*test_fn)(const string &) ) {
    vector<string> files;
#ifdef _MSC_VER
    _finddata_t c_file;
    intptr_t hFile;
    string findPath = path + "/*.das";
    if ((hFile = _findfirst(findPath.c_str(), &c_file)) != -1L) {
        do {
            files.push_back(path + "/" + c_file.name);
        } while (_findnext(hFile, &c_file) == 0);
    }
    _findclose(hFile);
#else
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if ( strstr(ent->d_name,".das") ) {
                files.push_back(path + "/" + ent->d_name);
            }
        }
        closedir (dir);
    }
#endif
    sort(files.begin(),files.end());
    bool ok = true;
    for ( auto & fn : files ) {
        ok = test_fn(fn) && ok;
    }
    return ok;
}

int main(int argc, const char * argv[]) {
#ifdef _MSC_VER
    #define    TEST_PATH "../"
#else
    #define TEST_PATH "../../"
#endif
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_TestProfile);
#if 1
    unit_test(TEST_PATH "examples/profile/tests/annotation.das");
    Module::Shutdown();
    return 0;
#endif
    // run tests
    if (argc == 1)
        run_tests(TEST_PATH "examples/profile/tests", unit_test);
    for ( int i=1; i!=argc; ++i ) {
        string path=argv[i];
        unit_test(path);
    }
    // and done
    Module::Shutdown();
    return 0;
}
