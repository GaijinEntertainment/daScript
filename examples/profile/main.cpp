#include "daScript/daScript.h"

using namespace das;

extern int MAIN_FUNC_NAME ( int, char * argv[] );

int main(int argc, char * argv[]) {
    // request our custom module
    NEED_MODULE(Module_TestProfile);
    // call daScript for main.das
    string dasRoot = getDasRoot() + "/examples/profile/main.das";
    char * newArgv[3] = { argv[0], (char *)dasRoot.c_str(), "-jit" };
    return MAIN_FUNC_NAME(3,newArgv);
}
