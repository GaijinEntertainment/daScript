#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

namespace das {
namespace ast_aot_cpp {


class Standalone : public Context {
public:
    Standalone();
    auto aot ( char * const  input, bool isAotLib, bool paranoid_validation, bool cross_platform ) -> char *;
};
} // namespace ast_aot_cpp
} // namespace das
