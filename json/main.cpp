#include "daScript/daScript.h"

#include <fstream>

using namespace std;
using namespace das;

TextPrinter tout;

bool unit_test ( const string & fn ) {
	tout << fn << " ";
    string str;
    ifstream t(fn);
    if ( !t.is_open() ) {
		tout << "not found\n";
        return false;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str(), tout) ) {
        if ( program->failed() ) {
			tout << "failed to compile\n";
            for ( auto & err : program->errors ) {
				tout << reportError(str.c_str(), err.at.line, err.at.column, err.what, err.cerr );
            }
            return false;
        } else {
            Context ctx(&str);
            program->simulate(ctx, tout);
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


int main() {
#ifdef _MSC_VER
#define    TEST_PATH "../"
#else
#define TEST_PATH "../../"
#endif
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_RapidJson);
    unit_test(TEST_PATH "json/test.das");
    Module::Shutdown();
    return 0;
}
