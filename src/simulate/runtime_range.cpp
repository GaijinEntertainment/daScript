#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_range.h"

namespace das
{
    bool RangeIterator::first ( Context & context, IteratorContext & itc ) {
        vec4f ll = subexpr->eval(context);
        DAS_ITERATOR_EXCEPTION_POINT;
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
        DAS_EXCEPTION_POINT;
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stackTop + stackTop[0]);
		int32_t r_to = r.to;
        for (int32_t i = r.from; i != r_to && !context.stopFlags; ++i) {
            *pi = i;
            body->eval(context);
        }
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }
}
