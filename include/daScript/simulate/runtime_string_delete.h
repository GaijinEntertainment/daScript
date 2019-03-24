#pragma once

#include <climits>
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das
{
    struct SimNode_DeleteString : SimNode_Delete {
        SimNode_DeleteString ( const LineInfo & a, SimNode * s, uint32_t t )
            : SimNode_Delete(a,s,t) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
    };
}

#include "daScript/simulate/simulate_visit_op_undef.h"

