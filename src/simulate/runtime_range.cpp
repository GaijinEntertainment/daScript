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

    void RangeIterator::close ( Context & context, char * ) {
        context.heap->free((char *)this, sizeof(RangeIterator));
    }

    vec4f SimNode_RangeIterator::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = subexpr->eval(context);
        range r = cast<range>::to(ll);
        char * iter = context.heap->allocate(sizeof(RangeIterator));
        context.heap->mark_comment(iter,"range iterator");
        context.heap->mark_location(iter,&debugInfo);
        new (iter) RangeIterator(r);
        return cast<char *>::from(iter);
    }

    ////////////
    // FOR RANGE
    ////////////

    vec4f SimNode_ForRange::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t r_to = r.to;
        SimNode ** __restrict tail = list + total;
        for (int32_t i = r.from; i != r_to; ++i) {
            *pi = i;
            SimNode ** __restrict body = list;
        loopbegin:;
            for (; body!=tail; ++body) {
                (*body)->eval(context);
                DAS_PROCESS_LOOP_FLAGS(break);
            }
        }
    loopend:;
        evalFinal(context);
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }

    vec4f SimNode_ForRangeNF::eval ( Context & context ) {
        DAS_PROFILE_NODE
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
        context.stopFlags &= ~(EvalFlags::stopForBreak | EvalFlags::stopForContinue);
        return v_zero();
    }

    vec4f SimNode_ForRange1::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t r_to = r.to;
        SimNode * __restrict pbody = list[0];
        for (int32_t i = r.from; i != r_to; ++i) {
            *pi = i;
            pbody->eval(context);
            DAS_PROCESS_LOOP1_FLAGS(continue);
        }
    loopend:;
        evalFinal(context);
        context.stopFlags &= ~(EvalFlags::stopForBreak | EvalFlags::stopForContinue);
        return v_zero();
    }

    vec4f SimNode_ForRangeNF1::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t r_to = r.to;
        SimNode * __restrict pbody = list[0];
        for (int32_t i = r.from; i != r_to; ++i) {
            *pi = i;
            pbody->eval(context);
        }
        evalFinal(context);
        context.stopFlags &= ~(EvalFlags::stopForBreak | EvalFlags::stopForContinue);
        return v_zero();
    }

#if DAS_DEBUGGER

    //////////////////
    // FOR RANGE DEBUG
    //////////////////

    vec4f SimNodeDebug_ForRange::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t r_to = r.to;
        SimNode ** __restrict tail = list + total;
        for (int32_t i = r.from; i != r_to; ++i) {
            *pi = i;
            SimNode ** __restrict body = list;
        loopbegin:;
            for (; body!=tail; ++body) {
                DAS_SINGLE_STEP(context,(*body)->debugInfo,true);
                (*body)->eval(context);
                DAS_PROCESS_LOOP_FLAGS(break);
            }
        }
    loopend:;
        evalFinal(context);
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }

    vec4f SimNodeDebug_ForRangeNF::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t r_to = r.to;
        SimNode ** __restrict tail = list + total;
        for (int32_t i = r.from; i != r_to; ++i) {
            *pi = i;
            for (SimNode ** __restrict body = list; body!=tail; ++body) {
                DAS_SINGLE_STEP(context,(*body)->debugInfo,true);
                (*body)->eval(context);
            }
        }
        evalFinal(context);
        context.stopFlags &= ~(EvalFlags::stopForBreak | EvalFlags::stopForContinue);
        return v_zero();
    }

    vec4f SimNodeDebug_ForRange1::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t r_to = r.to;
        SimNode * __restrict pbody = list[0];
        for (int32_t i = r.from; i != r_to; ++i) {
            *pi = i;
            DAS_SINGLE_STEP(context,pbody->debugInfo,true);
            pbody->eval(context);
            DAS_PROCESS_LOOP1_FLAGS(continue);
        }
    loopend:;
        evalFinal(context);
        context.stopFlags &= ~(EvalFlags::stopForBreak | EvalFlags::stopForContinue);
        return v_zero();
    }

    vec4f SimNodeDebug_ForRangeNF1::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = sources[0]->eval(context);
        range r = cast<range>::to(ll);
        int32_t * __restrict pi = (int32_t *)(context.stack.sp() + stackTop[0]);
        int32_t r_to = r.to;
        SimNode * __restrict pbody = list[0];
        for (int32_t i = r.from; i != r_to; ++i) {
            *pi = i;
            DAS_SINGLE_STEP(context,pbody->debugInfo,true);
            pbody->eval(context);
        }
        evalFinal(context);
        context.stopFlags &= ~(EvalFlags::stopForBreak | EvalFlags::stopForContinue);
        return v_zero();
    }

#endif

}
