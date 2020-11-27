#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/misc/function_traits.h"
#include "daScript/simulate/simulate_visit_op.h"

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
    struct cast_arg<LineInfoArg *> {
        static __forceinline LineInfoArg * to ( Context &, SimNode * node ) {
            return (LineInfoArg *) (&node->debugInfo);
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

    template <typename Result>
    struct ImplCallStaticFunction<smart_ptr<Result>> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            return cast<Result *>::from( fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... ).orphan() );
        }
    };

    template <typename Result>
    struct ImplCallStaticFunction<smart_ptr_raw<Result>> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            return cast<Result *>::from( fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... ).get() );
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

    template <typename T>
    struct is_any_pointer {
        enum { value = is_pointer<T>::value || is_smart_ptr<T>::value };
    };

    template <typename Result, typename CType,
        bool Pointer=is_any_pointer<Result>::value && is_any_pointer<CType>::value,
        bool IsEnum=is_enum<Result>::value
    >
    struct ImplCallStaticFunctionImm {
        enum { valid = false };
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline CType call(FunctionType &&, Context &, SimNode **, index_sequence<I...> ) {
            DAS_ASSERTF(0, "if this assert triggers, somehow prohibited call was called explicitly."
                        "like evalDouble on pointer or something similar."
                        "this means that this template needs to be revisited."
                        "we should not even be here, because code above verifies 'valid' field.");
            return CType();
        }
    };

    template <typename Result, typename CType>
    struct ImplCallStaticFunctionImm<smart_ptr<Result>, CType, true, false> {
        enum { valid = true };
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...>) {
            return (CType)fn(cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to(ctx, args[I])...).orphan();
        }
    };

    template <typename Result, typename CType>
    struct ImplCallStaticFunctionImm<smart_ptr_raw<Result>, CType, true, false> {
        enum { valid = true };
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...>) {
            return (CType)fn(cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to(ctx, args[I])...).get();
        }
    };

    template <typename Result, typename CType>
    struct ImplCallStaticFunctionImm<Result, CType, true, false> {
        enum { valid = true };
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...>) {
            return (CType)fn(cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to(ctx, args[I])...);
        }
    };

    template <typename Result, typename CType>
    struct ImplCallStaticFunctionImm<Result, CType, false, true> {
        enum { valid = true };
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...>) {
            return (CType)fn(cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to(ctx, args[I])...);
        }
    };

    template <typename Result>
    struct ImplCallStaticFunctionImm<Result,Result, false, false> {
        enum { valid = true };
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline Result call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            return fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
        }
    };

    template <typename CType, bool Pointer, bool IsEnum>
    struct ImplCallStaticFunctionImm<void,CType,Pointer,IsEnum> {
        enum { valid = true };
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
            return CType();
        }
    };

    template <typename Result>
    struct ImplCallStaticFunctionAndCopy {
        enum { valid = true };
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline void call(FunctionType && fn, Context & ctx, Result * res, SimNode ** args, index_sequence<I...> ) {
            *res = fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... );
        }
    };

    template <>
    struct ImplCallStaticFunctionAndCopy<void>;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

    struct SimNode_ExtFuncCallBase : SimNode_CallBase {
        const char* extFnName = nullptr;
        SimNode_ExtFuncCallBase(const LineInfo& at, const char* fnName)
            : SimNode_CallBase(at) {
            extFnName = fnName;
        }
        virtual SimNode* copyNode(Context& context, NodeAllocator* code) override {
            auto that = (SimNode_ExtFuncCallBase *) SimNode_CallBase::copyNode(context, code);
            that->extFnName = code->allocateName(extFnName);
            return that;
        }
        virtual SimNode* visit(SimVisitor& vis) override {
            V_BEGIN();
            vis.op(extFnName);
            V_CALL();
            V_END();
        }
    };

    template <typename FuncT, FuncT fn>
    struct SimNode_ExtFuncCall : SimNode_ExtFuncCallBase {
        enum { IS_CMRES = false };
        SimNode_ExtFuncCall ( const LineInfo & at, const char * fnName )
            : SimNode_ExtFuncCallBase(at,fnName) { }
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
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
                DAS_PROFILE_NODE \
                using FunctionTrait = function_traits<FuncT>;                       \
                using Result = typename FunctionTrait::return_type;                 \
                using Arguments = typename FunctionTrait::arguments;                \
                const int nargs = tuple_size<Arguments>::value;                     \
                using Indices = make_index_sequence<nargs>;                         \
                if ( !ImplCallStaticFunctionImm<Result,CTYPE>::valid )              \
                    context.throw_error_at(debugInfo,"internal integration error"); \
                return ImplCallStaticFunctionImm<Result,CTYPE>::template            \
                    call<FuncT,Arguments>(*fn, context, arguments, Indices());      \
        }
        DAS_EVAL_NODE
#undef  EVAL_NODE
    };

    template <typename FuncT, FuncT fn>
    struct SimNode_ExtFuncCallAndCopyOrMove : SimNode_ExtFuncCallBase {
        enum { IS_CMRES = true };
        SimNode_ExtFuncCallAndCopyOrMove ( const LineInfo & at, const char * fnName )
            : SimNode_ExtFuncCallBase(at,fnName) { }
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            using FunctionTrait = function_traits<FuncT>;
            using Result = typename FunctionTrait::return_type;
            using Arguments = typename FunctionTrait::arguments;
            const int nargs = tuple_size<Arguments>::value;
            using Indices = make_index_sequence<nargs>;
            Result * cmres = (Result *)(cmresEval->evalPtr(context));
            ImplCallStaticFunctionAndCopy<Result>::template
                call<FuncT,Arguments>(*fn, context, cmres, arguments, Indices());
            return cast<Result *>::from(cmres);
        }
    };

    struct ImplCallStaticFunctionRef {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline char * call(FunctionType && fn, Context & ctx, SimNode ** args, index_sequence<I...> ) {
            return (char *) & ( fn( cast_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, args[ I ] )... ) );
        }
    };

    template <typename FuncT, FuncT fn>
    struct SimNode_ExtFuncCallRef : SimNode_ExtFuncCallBase {
        DAS_PTR_NODE;
        enum { IS_CMRES = false };
        SimNode_ExtFuncCallRef ( const LineInfo & at, const char * fnName )
            : SimNode_ExtFuncCallBase(at,fnName) { }
        __forceinline char * compute(Context & context) {
            DAS_PROFILE_NODE
            using FunctionTrait = function_traits<FuncT>;
            using Arguments = typename FunctionTrait::arguments;
            const int nargs = tuple_size<Arguments>::value;
            using Indices = make_index_sequence<nargs>;
            return ImplCallStaticFunctionRef::template
                call<FuncT, Arguments>(*fn, context, arguments, Indices());
        }
    };

    typedef vec4f ( InteropFunction ) ( Context & context, SimNode_CallBase * node, vec4f * args );

    template <InteropFunction fn>
    struct SimNode_InteropFuncCall : SimNode_ExtFuncCallBase {
        SimNode_InteropFuncCall ( const LineInfo & at, const char * fnName )
            : SimNode_ExtFuncCallBase(at,fnName) { }
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            vec4f * args = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
            evalArgs(context, args);
            auto res = fn(context,this,args);
            return res;
        }
    };
}

#include "daScript/simulate/simulate_visit_op_undef.h"


