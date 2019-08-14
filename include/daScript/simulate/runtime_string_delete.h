#pragma once

#include <climits>
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das
{
    struct StringIterator : Iterator {
        StringIterator ( char * st ) : str(st) {}
        virtual bool first ( Context & context, char * value ) override;
        virtual bool next  ( Context & context, char * value ) override;
        virtual void close ( Context & context, char * value ) override;
        char * str;
    };

    struct SimNode_StringIterator : SimNode {
        SimNode_StringIterator ( const LineInfo & at, SimNode * s )
            : SimNode(at), source(s) { }
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
        SimNode *   source;
    };

    struct SimNode_DeleteString : SimNode_Delete {
        SimNode_DeleteString ( const LineInfo & a, SimNode * s, uint32_t t )
            : SimNode_Delete(a,s,t) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
    };
}

#include "daScript/simulate/simulate_visit_op_undef.h"

