#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_range.h"

namespace das
{
    bool RangeIterator::first ( Context &, char * _value ) {
        int32_t * value = (int32_t *) _value;
        *value      = rng.from;
        range_to    = rng.to;
        return (rng.from!=rng.to);
    }

    bool RangeIterator::next  ( Context &, char * _value ) {
        int32_t * value = (int32_t *) _value;
        int32_t nextValue = *value + 1;
        *value = nextValue;
        return (nextValue != range_to);
    }

    void RangeIterator::close ( Context &, char * ) {
    }

    vec4f SimNode_RangeIterator::eval ( Context & context ) {
        vec4f ll = subexpr->eval(context);
        range r = cast<range>::to(ll);
        char * iter = context.heap.allocate(sizeof(RangeIterator));
        new (iter) RangeIterator(r);
        return cast<char *>::from(iter);
    }

    vec4f SimNode_ForRange::eval ( Context & context ) {
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t r_to = r.to;
        SimNode ** __restrict tail = list + total;
        for (int32_t i = r.from; i != r_to; ++i) {
            *pi = i;
            for (SimNode ** __restrict body = list; body!=tail; ++body) {
                (*body)->eval(context);
                if (context.stopFlags) goto loopend;
            }
        }
    loopend:;
        evalFinal(context);
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }

    vec4f SimNode_ForRangeNF::eval ( Context & context ) {
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t r_to = r.to;
        SimNode ** __restrict tail = list + total;
        for (int32_t i = r.from; i != r_to; ++i) {
            *pi = i;
            for (SimNode ** __restrict body = list; body!=tail; ++body) {
                (*body)->eval(context);
            }
        }
        evalFinal(context);
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }

    vec4f SimNode_ForRange1::eval ( Context & context ) {
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t count = r.to - r.from; // [0,3] 0,1,2
        auto i = r.from;
        SimNode * __restrict pbody = list[0];
        while ( count>=4 ) {
            *pi=i++; pbody->eval(context); if ( context.stopFlags ) goto done;
            *pi=i++; pbody->eval(context); if ( context.stopFlags ) goto done;
            *pi=i++; pbody->eval(context); if ( context.stopFlags ) goto done;
            *pi=i++; pbody->eval(context); if ( context.stopFlags ) goto done;
            count -= 4;
        }
        if ( count & 2 ) {
            *pi=i++; pbody->eval(context); if ( context.stopFlags ) goto done;
            *pi=i++; pbody->eval(context); if ( context.stopFlags ) goto done;
        }
        if ( count & 1 ) {
            *pi=i++; pbody->eval(context); if ( context.stopFlags ) goto done;
        }
    done:;
        evalFinal(context);
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }

    vec4f SimNode_ForRangeNF1::eval ( Context & context ) {
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t count = r.to - r.from; // [0,3] 0,1,2
        auto i = r.from;
        SimNode * __restrict pbody = list[0];
        while ( count>=4 ) {
            *pi=i++; pbody->eval(context);
            *pi=i++; pbody->eval(context);
            *pi=i++; pbody->eval(context);
            *pi=i++; pbody->eval(context);
            count -= 4;
        }
        if ( count & 2 ) {
            *pi=i++; pbody->eval(context);
            *pi=i++; pbody->eval(context);
        }
        if ( count & 1 ) {
            *pi=i++; pbody->eval(context);
        }
        evalFinal(context);
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }
}
