#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"

using namespace std;
using namespace das;

TextPrinter tout;

bool unit_test ( const string & fn ) {
	tout << fn << " ";
    auto access = make_shared<FsFileAccess>();
    ModuleGroup dummyGroup;
    if ( auto program = compileDaScript(fn, access, tout, dummyGroup) ) {
        if ( program->failed() ) {
			tout << "failed to compile\n";
            for ( auto & err : program->errors ) {
				tout <<  reportError(err.at, err.what, err.cerr );
            }
            return false;
        } else {
            Context ctx(program->policies.stack);
            if ( !program->simulate(ctx, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.cerr );
                }
                return false;
            }
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
    unit_test(TEST_PATH "examples/json/test.das");
    Module::Shutdown();
    return 0;
}
