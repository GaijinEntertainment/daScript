#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "test_profile.h"

namespace das {
    namespace aot {
        namespace profile_dict {
            #include "tests/dict.das.main.inc"
        }
        namespace profile_exp {
            #include "tests/exp.das.main.inc"
        }
        namespace profile_fib {
            #include "tests/fib.das.main.inc"
        }

        void registerAot(AotLibrary & aotLib)
        {
            {
                using namespace profile_dict;
                #include "tests/dict.das.register.inc"
            }
            {
                using namespace profile_exp;
                #include "tests/exp.das.register.inc"
            }
            {
                using namespace profile_fib;
                #include "tests/fib.das.register.inc"
            }
        }
    }
}
