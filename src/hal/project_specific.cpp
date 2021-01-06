#include "daScript/misc/platform.h"
#include "daScript/daScriptModule.h"

using namespace das;

void require_project_specific_modules() {
    NEED_MODULE(Module_TestProfile);
    NEED_MODULE(Module_UnitTest);
}
