#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_range.h"

namespace das
{
    bool RangeIterator::first ( Context & context, IteratorContext & itc ) {
        vec4f ll = subexpr->eval(context);
        range r = cast<range>::to(ll);  // does not matter if its range or urange, hence only one type
        itc.value    = cast<int32_t>::from(r.from);
        itc.range_to = r.to;
        return (r.from!=r.to);
    }
    
    bool RangeIterator::next  ( Context &, IteratorContext & itc ) {
        int32_t nextValue = cast<int32_t>::to(itc.value) + 1;
        itc.value = cast<int32_t>::from(nextValue);
        return (nextValue != itc.range_to);
    }

    void RangeIterator::close ( Context &, IteratorContext & ) {
    }
    
    vec4f SimNode_RangeIterator::eval ( Context & ) {
        return cast<Iterator *>::from(static_cast<RangeIterator *>(this));
    }
    
    vec4f SimNode_ForRange::eval ( Context & context ) {
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t count = r.to - r.from; // [0,3] 0,1,2
        int32_t i = r.from;
        while ( count>=4 ) {
            *pi = i++; body->eval(context); if ( context.stopFlags ) goto done;
            *pi = i++; body->eval(context); if ( context.stopFlags ) goto done;
            *pi = i++; body->eval(context); if ( context.stopFlags ) goto done;
            *pi = i++; body->eval(context); if ( context.stopFlags ) goto done;
            count -= 4;
        }
        if ( count & 2 ) {
            *pi = i++; body->eval(context); if ( context.stopFlags ) goto done;
            *pi = i++; body->eval(context); if ( context.stopFlags ) goto done;
        }
        if ( count & 1 ) {
            *pi = i++; body->eval(context); if ( context.stopFlags ) goto done;
        }
    done:;
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }
}
