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

    ////////////
    // FOR RANGE
    ////////////

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

    //////////////////
    // FOR RANGE DEBUG
    //////////////////

#if DAS_DEBUGGER

    struct SimNodeDebug_ForRange : SimNode_ForRange  {
        SimNodeDebug_ForRange ( const LineInfo & at )
            : SimNode_ForRange(at) {}
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNodeDebug_ForRangeNF : SimNode_ForRangeNF  {
        SimNodeDebug_ForRangeNF ( const LineInfo & at )
            : SimNode_ForRangeNF(at) {}
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNodeDebug_ForRange1 : SimNode_ForRange1  {
        SimNodeDebug_ForRange1 ( const LineInfo & at )
            : SimNode_ForRange1(at) {}
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNodeDebug_ForRangeNF1 : SimNode_ForRangeNF1  {
        SimNodeDebug_ForRangeNF1 ( const LineInfo & at )
            : SimNode_ForRangeNF1(at) {}
        virtual vec4f eval ( Context & context ) override;
    };

#endif

}

