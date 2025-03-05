// Module 
#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

namespace das {
namespace context {


class Standalone : public Context {
public: 
    Standalone();
    auto test (  ) -> void;
};
} // namespace context
} // namespace das
