#pragma once

#include "daScript/simulate/simulate.h"
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

    template <typename R, typename ...Args>
    struct CallStaticFunction {
        template <size_t... I>
        static __forceinline R call ( R (* fn) (Args...), Context & ctx, SimNode ** args, index_sequence<I...> ) {
            return fn(cast_arg<Args>::to(ctx,args[I])...);
        }
    };

    template <typename FunctionType>
    struct ImplCallStaticFunction;

    template <typename R, typename ...Args>
    struct ImplCallStaticFunction<R (*)(Args...)> {
        static __forceinline R call_imm( R (*fn)(Args...), Context & ctx, SimNode ** args ) {
            return CallStaticFunction<R,Args...>::call(fn,ctx,args,make_index_sequence<sizeof...(Args)>());
        }
        static __forceinline vec4f call( R (*fn)(Args...), Context & ctx, SimNode ** args ) {
            return cast<R>::from(call_imm(fn,ctx,args));
        }
    };

    template <typename R, typename ...Args>
    struct ImplCallStaticFunction<smart_ptr<R> (*)(Args...)> {
        static __forceinline R * call_imm ( smart_ptr<R> (*fn)(Args...), Context & ctx, SimNode ** args ) {
            return CallStaticFunction<smart_ptr<R>,Args...>::call(fn,ctx,args,make_index_sequence<sizeof...(Args)>()).orphan();
        }
        static __forceinline vec4f call ( smart_ptr<R> (*fn)(Args...), Context & ctx, SimNode ** args ) {
            return cast<R *>::from(call_imm(fn,ctx,args));
        }
    };

    template <typename R, typename ...Args>
    struct ImplCallStaticFunction<smart_ptr_raw<R> (*)(Args...)> {
        static __forceinline R * call_imm ( smart_ptr_raw<R> (*fn)(Args...), Context & ctx, SimNode ** args ) {
            return CallStaticFunction<smart_ptr_raw<R>,Args...>::call(fn,ctx,args,make_index_sequence<sizeof...(Args)>()).get();
        }
        static __forceinline vec4f call ( smart_ptr_raw<R> (*fn)(Args...), Context & ctx, SimNode ** args ) {
            return cast<R *>::from(call_imm(fn,ctx,args));
        }
    };

    template <typename ...Args>
    struct ImplCallStaticFunction<void (*)(Args...)> {
        static __forceinline void call_imm ( void (*fn)(Args...), Context & ctx, SimNode ** args ) {
            CallStaticFunction<void,Args...>::call(fn,ctx,args,make_index_sequence<sizeof...(Args)>());
        }
        static __forceinline vec4f call ( void (*fn)(Args...), Context & ctx, SimNode ** args ) {
            call_imm(fn,ctx,args);
            return v_zero();
        }
    };

    template <typename T>
    struct is_any_pointer {
        enum { value = is_pointer<T>::value || is_smart_ptr<T>::value };
    };

    template <typename FunctionType, typename Result, typename CType, bool Pointer, bool IsEnum>
    struct ImplCallStaticFunctionImpl {
        static __forceinline CType call(FunctionType &&, Context & context, SimNode **) {
            context.throw_error("internal integration error");
            return CType();
        }
    };

    template <typename FunctionType, typename Result, typename CType>   // any pointer or smart_pointer
    struct ImplCallStaticFunctionImpl<FunctionType, Result, CType, true, false> {
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args) {
            return (CType) ImplCallStaticFunction<FunctionType>::call_imm(fn,ctx,args);
        }
    };

    template <typename FunctionType, typename Result, typename CType>   // any enum
    struct ImplCallStaticFunctionImpl<FunctionType, Result, CType, false, true> {
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args) {
            return (CType) ImplCallStaticFunction<FunctionType>::call_imm(fn,ctx,args);
        }
    };

    template <typename FunctionType, typename Result>
    struct ImplCallStaticFunctionImpl<FunctionType,Result,Result, false, false> {   // no cast
        static __forceinline Result call(FunctionType && fn, Context & ctx, SimNode ** args) {
            return ImplCallStaticFunction<FunctionType>::call_imm(fn,ctx,args);
        }
    };

    template <typename FunctionType, typename CType, bool Pointer, bool IsEnum> // void
    struct ImplCallStaticFunctionImpl<FunctionType,void,CType,Pointer,IsEnum> {
        static __forceinline CType call(FunctionType && fn, Context & ctx, SimNode ** args) {
            ImplCallStaticFunction<FunctionType>::call_imm(fn,ctx,args);
            return CType();
        }
    };

    template <typename FuncType, typename CType>
    struct ImplCallStaticFunctionImm;

    template <typename R, typename ...Args, typename CType>
    struct ImplCallStaticFunctionImm<R (*)(Args...),CType>
        : ImplCallStaticFunctionImpl<R (*)(Args...), R, CType,
            is_any_pointer<R>::value && is_any_pointer<CType>::value,
            is_enum<R>::value> {
    };

    template <typename FunctionType>
    struct ImplCallStaticFunctionAndCopy;

    template <typename R, typename ...Args>
    struct ImplCallStaticFunctionAndCopy < R (*)(Args...) > {
        static __forceinline void call ( R (*fn)(Args...), Context & ctx, void * res, SimNode ** args ) {
            using result = typename remove_const<R>::type;
            *((result *)res) = CallStaticFunction<R,Args...>::call(fn,ctx,args,make_index_sequence<sizeof...(Args)>());
        }
    };

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
            return ImplCallStaticFunction<FuncT>::call(*fn, context, arguments);
        }
#define EVAL_NODE(TYPE,CTYPE)\
        virtual CTYPE eval##TYPE ( Context & context ) override { \
                DAS_PROFILE_NODE \
                return ImplCallStaticFunctionImm<FuncT,CTYPE>::call(*fn, context, arguments); \
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
            void * cmres = cmresEval->evalPtr(context);
            ImplCallStaticFunctionAndCopy<FuncT>::call(*fn, context, cmres, arguments);
            return cast<void *>::from(cmres);
        }
    };


    template <typename FunctionType>
    struct ImplCallStaticFunctionRef;

    template <typename R, typename ...Args>
    struct ImplCallStaticFunctionRef < R (*)(Args...) > {
        static __forceinline char * call ( R (*fn)(Args...), Context & ctx, SimNode ** args ) {
            return (char *) & CallStaticFunction<R,Args...>::call(fn,ctx,args,make_index_sequence<sizeof...(Args)>());
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
            return ImplCallStaticFunctionRef<FuncT>::call(*fn, context, arguments);
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


