#pragma once

#include "simulate.h"
#include "function_traits.h"

namespace yzg
{
    template <typename TT>
    struct cast_arg {
        static __forceinline TT to ( Context & ctx, __m128 x ) {
            return cast<TT>::to(x);
        }
    };
    
    template <>
    struct cast_arg<Context *> {
        static __forceinline Context * to ( Context & ctx, __m128 ) {
            return &ctx;
        }
    };

    template <typename Result>
    struct ImplCallStaticFunction {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline __m128 call(FunctionType & fn, Context & ctx, __m128 * args, index_sequence<I...> ) {
            return cast<Result>::from ( fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... ) );
        }
    };
    
    template <>
    struct ImplCallStaticFunction<void> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline __m128 call(FunctionType & fn, Context & ctx, __m128 * args, index_sequence<I...> ) {
            fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
            return _mm_setzero_ps();
        }
    };
    
    template <typename FuncT, FuncT fn >
    struct SimNode_ExtFuncCall : SimNode_Call {
        SimNode_ExtFuncCall ( const LineInfo & at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
            using FunctionTrait = function_traits<FuncT>;
            using Result = typename FunctionTrait::return_type;
            using Arguments = typename FunctionTrait::arguments;
            const int nargs = tuple_size<Arguments>::value;
            using Indices = make_index_sequence<nargs>;
			__m128 * args = (__m128 *)(alloca(nArguments*sizeof(__m128)));
			evalArgs(context, args);
            YZG_EXCEPTION_POINT;
        #if YZG_ENABLE_STACK_WALK
            // PUSH
            char * top = context.invokeStackTop ? context.invokeStackTop : context.stackTop;
            if ( context.stack - ( top - sizeof(Prologue) ) > context.stackSize ) {
                context.throw_error("stack overflow");
                return _mm_setzero_ps();
            }
            char * pushStack = context.stackTop;
            char * pushInvokeStack = context.invokeStackTop;
            context.stackTop = top - sizeof(Prologue);
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
            context.invokeStackTop = nullptr;
            // cout << "ext-call " << info->name <<  ", stack at " << (context.stack + context.stackSize - context.stackTop) << endl;
            // fill prologue
            Prologue * pp = (Prologue *) context.stackTop;
            pp->result =        _mm_setzero_ps();
            pp->arguments =     args;
            pp->info =          info;
            pp->line =          debug.line;
        #endif
            // calc
            auto res = ImplCallStaticFunction<Result>::template call<FuncT,Arguments>(*fn, context, args, Indices());
            // POP
        #if YZG_ENABLE_STACK_WALK
            context.invokeStackTop = pushInvokeStack;
            context.stackTop = pushStack;
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
        #endif
            return res;
        }
        FuncInfo * info = nullptr;
    };
    
    typedef __m128 ( InteropFunction ) ( Context & context, SimNode_Call * node, __m128 * args );
    
    template <InteropFunction fn>
    struct SimNode_InteropFuncCall : SimNode_Call {
        SimNode_InteropFuncCall ( const LineInfo & at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
			__m128 * args = (__m128 *)(alloca(nArguments * sizeof(__m128)));
            evalArgs(context, args);
            YZG_EXCEPTION_POINT;
#if YZG_ENABLE_STACK_WALK
            // PUSH
            char * top = context.invokeStackTop ? context.invokeStackTop : context.stackTop;
            if ( context.stack - ( top - sizeof(Prologue) ) > context.stackSize ) {
                context.throw_error("stack overflow");
                return _mm_setzero_ps();
            }
            char * pushStack = context.stackTop;
            char * pushInvokeStack = context.invokeStackTop;
            context.stackTop = top - sizeof(Prologue);
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
            context.invokeStackTop = nullptr;
            // cout << "ext-call " << info->name <<  ", stack at " << (context.stack + context.stackSize - context.stackTop) << endl;
            // fill prologue
            Prologue * pp = (Prologue *) context.stackTop;
            pp->result =        _mm_setzero_ps();
            pp->arguments =     args;
            pp->info =          info;
            pp->line =          debug.line;
#endif
            // calc
            auto res = fn(context,this,args);
            // POP
#if YZG_ENABLE_STACK_WALK
            context.invokeStackTop = pushInvokeStack;
            context.stackTop = pushStack;
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
#endif
            return res;
        }
        FuncInfo * info = nullptr;
    };
}

