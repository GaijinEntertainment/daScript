#include "daScript/daScript.h"

using namespace std;
using namespace das;

TextPrinter tout;

class FsFileInfo : public FileInfo {
    virtual ~FsFileInfo() {
        das_aligned_free16(source);
    }
};

class FsFileAccess : public FileAccess {
    virtual FileInfo * getNewFileInfo(const string & fileName) override {
        if ( FILE * ff = fopen ( fileName.c_str(), "rb" ) ) {
            auto info = new FsFileInfo();
            fseek(ff,0,SEEK_END);
            info->sourceLength = uint32_t(ftell(ff));
            fseek(ff,0,SEEK_SET);
            info->source = (char *) das_aligned_alloc16(info->sourceLength+1);
            fread(info->source, 1, info->sourceLength, ff);
            info->source[info->sourceLength] = 0;
            return setFileInfo(fileName, info);
        }
        return nullptr;
    }
};

bool unit_test ( const string & fn ) {
	tout << fn << " ";
    auto access = make_shared<FsFileAccess>();
    if ( auto program = parseDaScript(fn, access, tout) ) {
        if ( program->failed() ) {
			tout << "failed to compile\n";
            for ( auto & err : program->errors ) {
				tout <<  reportError(err.at, err.what, err.cerr );
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
