#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/misc/function_traits.h"

namespace das
{
    template <typename TT>
    struct cast_arg {
        static __forceinline TT to ( Context &, vec4f x ) {
            return cast<TT>::to(x);
        }
    };
    
    template <>
    struct cast_arg<Context *> {
        static __forceinline Context * to ( Context & ctx, vec4f ) {
            return &ctx;
        }
    };
    
    template <>
    struct cast_arg<vec4f> {
        static __forceinline vec4f to ( Context &, vec4f x ) {
            return x;
        }
    };

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4100)
#endif
    template <typename Result>
    struct ImplCallStaticFunction {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType & fn, Context & ctx, vec4f * args, index_sequence<I...> ) {
            return cast<Result>::from ( fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... ) );
        }
    };

    template <>
    struct ImplCallStaticFunction<void> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType & fn, Context & ctx, vec4f * args, index_sequence<I...> ) {
            fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
            return vec_setzero_ps();
        }
    };
    
    template <typename Result>
    struct ImplCallStaticFunctionAndCopy {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline void call(FunctionType & fn, Context & ctx, Result * res, vec4f * args, index_sequence<I...> ) {
            *res = fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
        }
    };
    
    template <>
    struct ImplCallStaticFunctionAndCopy<void> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline void call(FunctionType & fn, Context & ctx, void *, vec4f * args, index_sequence<I...> ) {
            assert(0 && "we should not even be here");
            fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
        }
    };
#ifdef _MSC_VER
#pragma warning(pop)
#endif

    template <typename FuncT, FuncT fn>
    struct SimNode_ExtFuncCall : SimNode_CallBase {
        SimNode_ExtFuncCall ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            using FunctionTrait = function_traits<FuncT>;
            using Result = typename FunctionTrait::return_type;
            using Arguments = typename FunctionTrait::arguments;
            const int nargs = tuple_size<Arguments>::value;
            using Indices = make_index_sequence<nargs>;
			vec4f * args = (vec4f *)(alloca(nArguments*sizeof(vec4f)));
			evalArgs(context, args);
            DAS_EXCEPTION_POINT;
            // PUSH
            char * top = context.invokeStackTop ? context.invokeStackTop : context.stackTop;
            if ( context.stack - ( top - sizeof(Prologue) ) > context.stackSize ) {
                context.throw_error("stack overflow");
                return vec_setzero_ps();
            }
            char * pushStack = context.stackTop;
            char * pushInvokeStack = context.invokeStackTop;
			context.invokeStackTop = nullptr;
		#if DAS_ENABLE_STACK_WALK
            context.stackTop = top - sizeof(Prologue);
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
            // fill prologue
            Prologue * pp = (Prologue *) context.stackTop;
            pp->arguments =     args;
            pp->copyOrMoveResult = nullptr;
            pp->info =          info;
            pp->line =          debug.line;
        #endif
            // calc
            auto res = ImplCallStaticFunction<Result>::template call<FuncT,Arguments>(*fn, context, args, Indices());
            // POP
            context.invokeStackTop = pushInvokeStack;
            context.stackTop = pushStack;
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
            return res;
        }
        FuncInfo * info = nullptr;
    };
    
    template <typename FuncT, FuncT fn>
    struct SimNode_ExtFuncCallAndCopyOrMove : SimNode_CallBase {
        SimNode_ExtFuncCallAndCopyOrMove ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            using FunctionTrait = function_traits<FuncT>;
            using Result = typename FunctionTrait::return_type;
            using Arguments = typename FunctionTrait::arguments;
            const int nargs = tuple_size<Arguments>::value;
            using Indices = make_index_sequence<nargs>;
			vec4f * args = (vec4f *)(alloca(nArguments*sizeof(vec4f)));
			evalArgs(context, args);
            DAS_EXCEPTION_POINT;
            Result * cmres = (Result *)(context.stackTop + stackTop);
            // PUSH
            char * top = context.invokeStackTop ? context.invokeStackTop : context.stackTop;
            if ( context.stack - ( top - sizeof(Prologue) ) > context.stackSize ) {
                context.throw_error("stack overflow");
                return vec_setzero_ps();
            }
            char * pushStack = context.stackTop;
            char * pushInvokeStack = context.invokeStackTop;
			context.invokeStackTop = nullptr;
		#if DAS_ENABLE_STACK_WALK
            context.stackTop = top - sizeof(Prologue);
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
            // fill prologue
            Prologue * pp = (Prologue *) context.stackTop;
            pp->arguments =     args;
            pp->copyOrMoveResult = nullptr;
            pp->info =          info;
            pp->line =          debug.line;
        #endif
            // calc
            ImplCallStaticFunctionAndCopy<Result>::template call<FuncT,Arguments>(*fn, context, cmres, args, Indices());
            // POP
            context.invokeStackTop = pushInvokeStack;
            context.stackTop = pushStack;
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
            return cast<Result *>::from(cmres);
        }
        FuncInfo * info = nullptr;
    };

    
    typedef vec4f ( InteropFunction ) ( Context & context, SimNode_CallBase * node, vec4f * args );
    
    template <InteropFunction fn>
    struct SimNode_InteropFuncCall : SimNode_CallBase {
        SimNode_InteropFuncCall ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override {
			vec4f * args = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
            evalArgs(context, args);
            DAS_EXCEPTION_POINT;
            // PUSH
            char * top = context.invokeStackTop ? context.invokeStackTop : context.stackTop;
            if ( context.stack - ( top - sizeof(Prologue) ) > context.stackSize ) {
                context.throw_error("stack overflow");
                return vec_setzero_ps();
            }
            char * pushStack = context.stackTop;
            char * pushInvokeStack = context.invokeStackTop;
			context.invokeStackTop = nullptr;
#if DAS_ENABLE_STACK_WALK
            context.stackTop = top - sizeof(Prologue);
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
            // fill prologue
            Prologue * pp = (Prologue *) context.stackTop;
            pp->arguments =     args;
            pp->copyOrMoveResult = nullptr;
            pp->info =          info;
            pp->line =          debug.line;
#endif
            // calc
            auto res = fn(context,this,args);
            // POP
            context.invokeStackTop = pushInvokeStack;
            context.stackTop = pushStack;
            assert ( context.stackTop >= context.stack && context.stackTop < context.stackTop + context.stackSize );
            return res;
        }
        FuncInfo * info = nullptr;
    };
}

