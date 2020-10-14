#include "daScript/daScript.h"

using namespace das;

// MAIN_FUNC_NAME is project specific define, which overrides 'main' for the custom AOT utility
extern int MAIN_FUNC_NAME ( int argc, char * argv[] );

int main(int argc, char * argv[]) {
    // request all da-script built in modules
    NEED_ALL_DEFAULT_MODULES;
    // request our custom module
    NEED_MODULE(Module_Tutorial02);
    // call original aot MAIN_FUNC from dasAot
    return MAIN_FUNC_NAME(argc,argv);
}
