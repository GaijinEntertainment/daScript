#include "precomp.h"

#include "ast.h"
#include "test_profile.h"

#include <dirent.h>

using namespace std;
using namespace yzg;

bool unit_test ( const string & fn ) {
    // cout << fn << " ";
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
    if ( auto program = parseDaScript(str.c_str()) ) {
        if ( program->failed() ) {
            cout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
            }
            return false;
        } else {
            cout << *program << "\n";
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


int main(int argc, const char * argv[]) {
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_TestProfile);
    // run tests
    unit_test("../../profile/profile_array_of_structures_vec.das");
    unit_test("../../profile/profile_try_catch.das");
    // and done
    Module::Shutdown();
    return 0;
}
