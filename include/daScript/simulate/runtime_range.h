#pragma once

#include "daScript/simulate/simulate.h"

namespace das
{
    struct RangeIterator : Iterator {
        RangeIterator ( const range & r, bool s ) : rng(r), isSigned(s) {}
        virtual bool first ( Context & context, char * value ) override;
        virtual bool next  ( Context & context, char * value ) override;
        virtual void close ( Context & context, char * value ) override;
        range   rng;
        int32_t range_to;
        bool    isSigned;
    };

    struct SimNode_RangeIterator : SimNode {
        SimNode_RangeIterator ( const LineInfo & at, SimNode * rng, bool s )
            : SimNode(at), subexpr(rng), isSigned(s) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override;
        SimNode * subexpr;
        bool isSigned;
    };

    ////////////
    // FOR RANGE
    ////////////

    struct SimNode_ForRangeBase : SimNode_ForBase {
        SimNode_ForRangeBase ( const LineInfo & at, bool s )
            : SimNode_ForBase(at), isSigned(s) {}
        __forceinline bool first( int32_t r_from, int32_t r_to ) const {
            return isSigned ? (r_from < r_to) : (uint32_t(r_from) < uint32_t(r_to));
        }
        bool isSigned = true;
    };

    struct SimNode_ForRange : SimNode_ForRangeBase  {
        SimNode_ForRange ( const LineInfo & at, bool s )
            : SimNode_ForRangeBase(at,s) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override;
    };

    struct SimNode_ForRangeNF : SimNode_ForRangeBase  {
        SimNode_ForRangeNF ( const LineInfo & at, bool s )
            : SimNode_ForRangeBase(at,s) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override;
    };

    struct SimNode_ForRange1 : SimNode_ForRangeBase  {
        SimNode_ForRange1 ( const LineInfo & at, bool s )
            : SimNode_ForRangeBase(at,s) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override;
    };

    struct SimNode_ForRangeNF1 : SimNode_ForRangeBase  {
        SimNode_ForRangeNF1 ( const LineInfo & at, bool s )
            : SimNode_ForRangeBase(at,s) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override;
    };

    //////////////////
    // FOR RANGE DEBUG
    //////////////////

#if DAS_DEBUGGER

    struct SimNodeDebug_ForRange : SimNode_ForRange  {
        SimNodeDebug_ForRange ( const LineInfo & at, bool s )
            : SimNode_ForRange(at,s) {}
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override;
    };

    struct SimNodeDebug_ForRangeNF : SimNode_ForRangeNF  {
        SimNodeDebug_ForRangeNF ( const LineInfo & at, bool s )
            : SimNode_ForRangeNF(at,s) {}
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override;
    };

    struct SimNodeDebug_ForRange1 : SimNode_ForRange1  {
        SimNodeDebug_ForRange1 ( const LineInfo & at, bool s )
            : SimNode_ForRange1(at,s) {}
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override;
    };

    struct SimNodeDebug_ForRangeNF1 : SimNode_ForRangeNF1  {
        SimNodeDebug_ForRangeNF1 ( const LineInfo & at, bool s )
            : SimNode_ForRangeNF1(at,s) {}
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override;
    };

#endif

}

