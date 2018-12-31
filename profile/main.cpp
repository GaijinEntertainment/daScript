#include "precomp.h"

#include "ast.h"
#include "test_profile.h"

#ifdef _MSC_VER
#include <io.h>
#else
#include <dirent.h>
#endif

using namespace std;
using namespace yzg;

bool unit_test ( const string & fn ) {
    cout << fn << "\n";
    string str;
    ifstream t(fn);
    if ( !t.is_open() ) {
        cout << "not found "<<fn<<"\n";
        return false;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str()) ) {
        if ( program->failed() ) {
            cout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
            }
            return false;
        } else {
            // cout << *program << "\n";
            Context ctx(&str);
            program->simulate(ctx);
            // vector of 10000 objects
            vector<Object> objects;
            objects.resize(10000);
            // run the test
            int fnTest = ctx.findFunction("test");
            if ( fnTest != -1 ) {
                ctx.restart();
                __m128 args[1] = { cast<vector<Object> *>::from(&objects) };
                bool result = cast<bool>::to(ctx.eval(fnTest, args));
                if ( auto ex = ctx.getException() ) {
                    cout << "exception: " << ex << "\n";
                    return false;
                }
                if ( !result ) {
                    cout << "failed\n";
                    return false;
                }
                // cout << "ok\n";
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

int main(int argc, const char * argv[]) {

#ifdef _MSC_VER
	#define	TEST_PATH "../"
#else
	#define TEST_PATH "../../"
#endif

    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_TestProfile);
    // run tests
    run_tests(TEST_PATH "profile", unit_test);
    for ( int i=1; i!=argc; ++i ) {
        string path=argv[i];
        unit_test(path);
    }
    // and done
    Module::Shutdown();
    return 0;
}
