#pragma once

#include "daScript/simulate/simulate.h"

namespace das
{
    struct RangeIterator : Iterator {
        virtual bool first ( Context & context, IteratorContext & itc ) override;
        virtual bool next  ( Context & context, IteratorContext & itc ) override;
        virtual void close ( Context & context, IteratorContext & itc ) override;
        SimNode * subexpr;
    };

    struct SimNode_RangeIterator : SimNode, RangeIterator {
            SimNode_RangeIterator ( const LineInfo & at, SimNode * rng )
            : SimNode(at) { subexpr = rng; }
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_ForRange : SimNode_ForBase  {
        SimNode_ForRange ( const LineInfo & at )
            : SimNode_ForBase(at) {}
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_ForRangeNF : SimNode_ForBase  {
        SimNode_ForRangeNF ( const LineInfo & at )
            : SimNode_ForBase(at) {}
        virtual vec4f eval ( Context & context ) override;
    };
}

