#include "precomp.h"

#include "runtime_range.h"

namespace das
{
    bool RangeIterator::first ( Context & context, IteratorContext & itc ) {
        __m128 ll = subexpr->eval(context);
        DAS_ITERATOR_EXCEPTION_POINT;
        range r = cast<range>::to(ll);  // does not matter if its range or urange, hence only one type
        itc.value    = cast<int32_t>::from(r.from);
        itc.range_to = r.to;
        return (r.from!=r.to);
    }
    
    bool RangeIterator::next  ( Context & context, IteratorContext & itc ) {
        int32_t nextValue = cast<int32_t>::to(itc.value) + 1;
        itc.value = cast<int32_t>::from(nextValue);
        return (nextValue != itc.range_to);
    }

    void RangeIterator::close ( Context & context, IteratorContext & itc ) {
    }
    
    __m128 SimNode_RangeIterator::eval ( Context & context ) {
        return cast<Iterator *>::from(static_cast<RangeIterator *>(this));
    }
    
    __m128 SimNode_ForRange::eval ( Context & context ) {
        __m128 ll = sources[0]->eval(context);
        DAS_EXCEPTION_POINT;
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stackTop + stackTop[0]);
		int32_t r_to = r.to;
        for (int32_t i = r.from; i != r_to && !context.stopFlags; ++i) {
            *pi = i;
            body->eval(context);
        }
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return _mm_setzero_ps();
    }
}
