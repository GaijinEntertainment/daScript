#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "path_tracer_helper.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)   // unreferenced formal parameter
#pragma warning(disable:4189)   // local variable is initialized but not referenced
#pragma warning(disable:4244)   // conversion from 'int32_t' to 'float', possible loss of data
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wwritable-strings"
#endif

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

#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif