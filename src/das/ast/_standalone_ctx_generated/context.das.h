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
    auto allExpr ( int32_t arg ) -> void;
    auto test (  ) -> bool;
    auto printAst ( smart_ptr_raw<Program> prog ) -> void;
    auto setFlags ( smart_ptr_raw<Program> prog ) -> void;
};
} // namespace context
} // namespace das
