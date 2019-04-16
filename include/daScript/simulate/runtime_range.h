#pragma once

#include "daScript/simulate/simulate.h"

namespace das
{
    struct RangeIterator : Iterator {
        RangeIterator ( const range & r ) : rng(r) {}
        virtual bool first ( Context & context, char * value ) override;
        virtual bool next  ( Context & context, char * value ) override;
        virtual void close ( Context & context, char * value ) override;
        range   rng;
        int32_t range_to;
    };

    struct SimNode_RangeIterator : SimNode {
        SimNode_RangeIterator ( const LineInfo & at, SimNode * rng )
            : SimNode(at), subexpr(rng) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
        SimNode * subexpr;
    };

    struct SimNode_ForRange : SimNode_ForBase  {
        SimNode_ForRange ( const LineInfo & at )
            : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_ForRangeNF : SimNode_ForBase  {
        SimNode_ForRangeNF ( const LineInfo & at )
            : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_ForRange1 : SimNode_ForBase  {
        SimNode_ForRange1 ( const LineInfo & at )
            : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_ForRangeNF1 : SimNode_ForBase  {
        SimNode_ForRangeNF1 ( const LineInfo & at )
            : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
    };
}

