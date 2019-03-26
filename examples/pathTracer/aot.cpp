#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "path_tracer_helper.h"

namespace das {
    namespace aot {
        namespace toy_path_tracer {
            #include "toy_path_tracer.das.main.inc"
        }
        void registerAot ( AotLibrary & aotLib )
        {
            {
                using namespace toy_path_tracer;
                #include "toy_path_tracer.das.register.inc"
            }
        }
    }
}
