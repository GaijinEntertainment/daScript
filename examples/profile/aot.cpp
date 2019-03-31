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
        namespace profile_native {
            #include "tests/native.das.main.inc"
        }
        namespace profile_nbodies {
            #include "tests/nbodies.das.main.inc"
        }
        namespace profile_particles {
            #include "tests/particles.das.main.inc"
        }
        namespace profile_primes {
            #include "tests/primes.das.main.inc"
        }
        namespace profile_aos {
            #include "tests/aos.das.main.inc"
        }
        namespace profile_annotation {
            #include "tests/annotation.das.main.inc"
        }
        namespace profile_panic {
            #include "tests/panic.das.main.inc"
        }
        void registerAot(AotLibrary & aotLib)
        {
            {   using namespace profile_dict;
                #include "tests/dict.das.register.inc"
            }
            {   using namespace profile_exp;
                #include "tests/exp.das.register.inc"
            }
            {   using namespace profile_fib;
                #include "tests/fib.das.register.inc"
            }
            {   using namespace profile_native;
                #include "tests/native.das.register.inc"
            }
            {   using namespace profile_nbodies;
                #include "tests/nbodies.das.register.inc"
            }
            {   using namespace profile_particles;
                #include "tests/particles.das.register.inc"
            }
            {   using namespace profile_primes;
                #include "tests/primes.das.register.inc"
            }
            {   using namespace profile_aos;
                #include "tests/aos.das.register.inc"
            }
            {   using namespace profile_annotation;
                #include "tests/annotation.das.register.inc"
            }
            {   using namespace profile_panic;
                #include "tests/panic.das.register.inc"
            }
        }
    }
}
