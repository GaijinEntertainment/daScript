#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/misc/function_traits.h"

namespace das
{
    template <typename TT>
    struct cast_arg {
        static __forceinline TT to ( Context & ctx, SimNode * x ) {
            return EvalTT<TT>::eval(ctx,x);
        }
    };

    template <>
    struct cast_arg<Context *> {
        static __forceinline Context * to ( Context & ctx, SimNode * ) {
            return &ctx;
        }
    };

    template <>
    struct cast_arg<vec4f> {
        static __forceinline vec4f to ( Context & ctx, SimNode * x ) {
            return x->eval(ctx);
        }
    };

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4100)
#endif
    template <typename Result>
    struct ImplCallStaticFunction {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            return cast<Result>::from( fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... ) );
        }
    };

    template <>
    struct ImplCallStaticFunction<void> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
            return v_zero();
        }
    };

    template <typename Result, typename CType>
    struct ImplCallStaticFunctionImm {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            assert(0 && "we should not even be here");
            fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
            return CType();
        }
    };

    template <typename Result>
    struct ImplCallStaticFunctionImm<Result,Result> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline Result call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            return fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
        }
    };

    template <typename CType>
    struct ImplCallStaticFunctionImm<void,CType> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
            return CType();
        }
    };

    template <typename Result>
    struct ImplCallStaticFunctionAndCopy {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline void call(FunctionType && fn, Context & ctx, Result * res, SimNode ** args, index_sequence<I...> ) {
            *res = fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
        }
    };

    template <>
    struct ImplCallStaticFunctionAndCopy<void> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline void call(FunctionType && fn, Context & ctx, void *, SimNode ** args, index_sequence<I...> ) {
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
            return ImplCallStaticFunction<Result>::template
                call<FuncT,Arguments>(*fn, context, arguments, Indices());
        }
#define EVAL_NODE(TYPE,CTYPE)\
        virtual CTYPE eval##TYPE ( Context & context ) override {                   \
                using FunctionTrait = function_traits<FuncT>;                       \
                using Result = typename FunctionTrait::return_type;                 \
                using Arguments = typename FunctionTrait::arguments;                \
                const int nargs = tuple_size<Arguments>::value;                     \
                using Indices = make_index_sequence<nargs>;                         \
                return ImplCallStaticFunctionImm<Result,CTYPE>::template            \
                    call<FuncT,Arguments>(*fn, context, arguments, Indices());      \
        }
        DAS_EVAL_NODE
#undef  EVAL_NODE
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
            Result * cmres = (Result *)(context.stack.sp() + stackTop);
            ImplCallStaticFunctionAndCopy<Result>::template
                call<FuncT,Arguments>(*fn, context, cmres, arguments, Indices());
            return cast<Result *>::from(cmres);
        }
    };
    
    typedef vec4f ( InteropFunction ) ( Context & context, SimNode_CallBase * node, vec4f * args );
    
    template <InteropFunction fn>
    struct SimNode_InteropFuncCall : SimNode_CallBase {
        SimNode_InteropFuncCall ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f * args = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
            evalArgs(context, args);
            auto res = fn(context,this,args);
            return res;
        }
    };
}

