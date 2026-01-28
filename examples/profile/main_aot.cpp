#include "daScript/daScript.h"

using namespace das;

extern int MAIN_FUNC_NAME ( int argc, char * argv[] );

int main(int argc, char * argv[]) {
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_TestProfile);
    return MAIN_FUNC_NAME(argc,argv);
}
