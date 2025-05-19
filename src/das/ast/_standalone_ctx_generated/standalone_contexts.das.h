#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

namespace das {
namespace standalone_contexts {


class Standalone : public Context {
public:
    Standalone();
    auto standalone_aot ( char * const  input, char * const  output_dir, bool isAotLib, bool cross_platform, bool paranoid_validation ) -> char *;
};
} // namespace standalone_contexts
} // namespace das
