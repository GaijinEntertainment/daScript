#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

namespace das {
namespace main {


class Standalone : public Context {
public:
    Standalone();
    auto main ( ) -> void;
};
} // namespace main
} // namespace das
