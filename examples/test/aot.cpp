#include "daScript/daScript.h"

#include "daScript/simulate/runtime_profile.h"
#include "daScript/simulate/debug_print.h"

#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/aot_builtin_math.h"

namespace das {

    template <typename TT>
    struct TArray : Array {
        TArray() {
            data = nullptr;
            size = 0;
            capacity = 0;
            lock = 0;
        }
    };

    template <typename TT>
    struct das_iterator;

    template <>
    struct das_iterator <range> {
        das_iterator(const range & r) : that(r) {}
        __forceinline bool first ( Context *, int32_t & i ) { i = that.from; return i!=that.to; }
        __forceinline bool next  ( Context *, int32_t & i ) { i++; return i!=that.to; }
        __forceinline void close ( Context *, int32_t &   ) {}
        range that;
    };

    template <typename TT>
    struct das_iterator<TArray<TT>> {
        das_iterator(TArray<TT> & r) : that(&r) {
            array_end = (TT *)(r.data + r.size*sizeof(TT));
        }
        __forceinline bool first ( Context * __context__, TT * & i ) {
            array_lock(*__context__, *that);
            i = (TT *) that->data;
            return i!=array_end;
        }
        __forceinline bool next  ( Context *, TT * & i ) {
            i++;
            return i!=array_end;
        }
        __forceinline void close ( Context * __context__, TT * & i ) {
            array_unlock(*__context__, *that);
            i = nullptr;
        }
        Array * that;
        TT * array_end;
    };

    template <typename resType, typename ...argType>
    struct das_make_block : Block, SimNode {
        typedef function < resType ( argType... ) > BlockFn;
        das_make_block ( Context * context, BlockFn && func )
        : SimNode(LineInfo()), blockFunction(func) {
            stackOffset = context->stack.spi();
            argumentsOffset = 0;                            // TODO: allocate on the stack
            body = this;
            functionArguments = context->abiArguments();
        };
        virtual vec4f eval ( Context & ) override {
            blockFunction();
            return v_zero();
        }
        BlockFn blockFunction;
    };


    template <typename TT>
    struct cast_aot_arg {
        static __forceinline TT to ( Context &, vec4f x ) {
            return cast<TT>::to(x);
        }
    };

    template <>
    struct cast_aot_arg<Context *> {
        static __forceinline Context * to ( Context & ctx, vec4f ) {
            return &ctx;
        }
    };

    template <>
    struct cast_aot_arg<vec4f> {
        static __forceinline vec4f to ( Context &, vec4f x ) {
            return x;
        }
    };

    template <typename Result>
    struct ImplAotStaticFunction {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType && fn, Context & ctx, index_sequence<I...> ) {
            return cast<Result>::from(
            fn( cast_aot_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, ctx.abiArguments()[ I ? I-1 : 0 ] )... ) );
        }
    };

    template <>
    struct ImplAotStaticFunction<void> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType && fn, Context & ctx, index_sequence<I...> ) {
            fn( cast_aot_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, ctx.abiArguments()[ I ? I-1 : 0 ] )... );
            return v_zero();
        }
    };

    template <typename FuncT, FuncT fn>
    struct SimNode_Aot : SimNode_CallBase {
        const char * extFnName = nullptr;
        SimNode_Aot ( ) : SimNode_CallBase(LineInfo()) {
        }
        virtual vec4f eval ( Context & context ) override {
            using FunctionTrait = function_traits<FuncT>;
            using Result = typename FunctionTrait::return_type;
            using Arguments = typename FunctionTrait::arguments;
            const int nargs = tuple_size<Arguments>::value;
            using Indices = make_index_sequence<nargs>;
            // TODO: sort out interop
            vec4f * aa = context.abiArg;
            vec4f stub[1];
            if ( !aa ) context.abiArg = stub;
            auto res = ImplAotStaticFunction<Result>::template
                call<FuncT,Arguments>(*fn, context, Indices());
            context.abiArg = aa;
            context.abiResult() = res;
            return res;
        }
    };

    struct SimNode_AotInteropBase : SimNode_CallBase {
        SimNode_AotInteropBase() : SimNode_CallBase(LineInfo()) {}
        virtual vec4f eval ( Context & ) override {
            return v_zero();
        };
        vec4f *     argumentValues;
    };

    template <int argumentCount>
    struct SimNode_AotInterop : SimNode_AotInteropBase {
        template <typename ...VI>
        SimNode_AotInterop ( TypeInfo ** tinfo, VI... args ) {
            nArguments = argumentCount;
            types = tinfo;
            argumentValues = argValues;
            vec4f argsE[argumentCount] = { args... };
            for ( int i=0; i!=argumentCount; ++i ) {
                argumentValues[i] = argsE[i];
            }
        };
        vec4f       argValues[argumentCount];
    };

    template <>
    struct SimNode_AotInterop<0> : SimNode_AotInteropBase {
        SimNode_AotInterop () {
            nArguments = 0;
            types = nullptr;
            argumentValues = nullptr;
        }
    };

    char * das_string_builder ( Context * __context__, const SimNode_AotInteropBase & node ) {
        StringBuilderWriter writer(__context__->heap);
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i = 0; i!=node.nArguments; ++i ) {
            walker.walk(node.argumentValues[i], node.types[i]);
        }
        auto pStr = writer.c_str();
        if ( !pStr ) {
            __context__->throw_error("can't allocate string builder result, out of heap");
        }
        return pStr;
    }

    void builtin_print ( char * text, Context * context );
    void builtin_array_resize ( Array & pArray, int newSize, int stride, Context * context );
    int builtin_array_size ( const Array & arr );

    namespace aot {
        TypeInfo __type_info__fa15db70 = { Type::tFloat, nullptr, nullptr, /*annotation*/ nullptr, nullptr, nullptr, 0, nullptr, 12, 0xfa15db70 };
        TypeInfo __type_info__a7069c83 = { Type::tString, nullptr, nullptr, /*annotation*/ nullptr, nullptr, nullptr, 0, nullptr, 4, 0xa7069c83 };
        TypeInfo * __tinfo_0[6] = { &__type_info__fa15db70, &__type_info__a7069c83, &__type_info__fa15db70, &__type_info__a7069c83, &__type_info__fa15db70, &__type_info__a7069c83 };

        float expLoop ( Context * __context__, int32_t n );
        float expLoopU ( Context * __context__, int32_t n );
        float expLoopUV ( Context * __context__, int32_t n );
        bool test ( Context * __context__ );

        float expLoop ( Context * __context__, int32_t n )
        {
            float sum = 0;
            {
                bool __need_loop_8 = true;
                das_iterator<range> __i_iterator(range(0,n));
                int32_t i;
                __need_loop_8 = __i_iterator.first(__context__,i) && __need_loop_8;
                for ( ; __need_loop_8 ; __need_loop_8 = __i_iterator.next(__context__,i) )
                {
                    sum += SimPolicy<float>::Exp(SimPolicy<float>::RcpEst(1 + float(i),*__context__),*__context__);
                }
                __i_iterator.close(__context__,i);
            };
            return sum;
        }

        float expLoopU ( Context * __context__, int32_t n )
        {
            float sum = 0;
            {
                bool __need_loop_15 = true;
                das_iterator<range> __i_iterator(range(0,n / 4));
                int32_t i;
                __need_loop_15 = __i_iterator.first(__context__,i) && __need_loop_15;
                for ( ; __need_loop_15 ; __need_loop_15 = __i_iterator.next(__context__,i) )
                {
                    float k = float(i * 4);
                    sum += (((SimPolicy<float>::Exp(SimPolicy<float>::RcpEst(1 + k,*__context__),*__context__) + SimPolicy<float>::Exp(SimPolicy<float>::RcpEst(2 + k,*__context__),*__context__)) + SimPolicy<float>::Exp(SimPolicy<float>::RcpEst(3 + k,*__context__),*__context__)) + SimPolicy<float>::Exp(SimPolicy<float>::RcpEst(4 + k,*__context__),*__context__));
                }
                __i_iterator.close(__context__,i);
            };
            return sum;
        }

        float expLoopUV ( Context * __context__, int32_t n )
        {
            float4 sumV = float4(0,0,0,0);
            {
                bool __need_loop_23 = true;
                das_iterator<range> __i_iterator(range(0,n / 4));
                int32_t i;
                __need_loop_23 = __i_iterator.first(__context__,i) && __need_loop_23;
                for ( ; __need_loop_23 ; __need_loop_23 = __i_iterator.next(__context__,i) )
                {
                    SimPolicy<float4>::SetAdd((char *)&(sumV),SimPolicy<float4>::Exp(SimPolicy<float4>::RcpEst(SimPolicy<float4>::Add(float4(i * 16),float4(1,2,3,4),*__context__),*__context__),*__context__),*__context__);
                }
                __i_iterator.close(__context__,i);
            };
            return ((v_extract_x(sumV) /*x*/ + v_extract_y(sumV) /*y*/) + v_extract_z(sumV) /*z*/) + v_extract_w(sumV) /*w*/;
        }

        bool test ( Context * __context__ )
        {
            float l1 = 0;
            float l2 = 0;
            float l3 = 0;
            builtin_profile(20,"exp loop",das_make_block<void>(__context__,[&]()->void{
                l1 = expLoop(__context__,1000000);
            }),__context__);
            builtin_profile(20,"exp loop, unrolled",das_make_block<void>(__context__,[&]()->void{
                l2 = expLoopU(__context__,1000000);
            }),__context__);
            builtin_profile(20,"exp loop, unrolled and vectorized",das_make_block<void>(__context__,[&]()->void{
                l3 = expLoopUV(__context__,1000000);
            }),__context__);
            builtin_print(das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_0, cast<float>::from(l1), cast<char *>::from(" "), cast<float>::from(l2), cast<char *>::from(" "), cast<float>::from(l3), cast<char *>::from("\n"))),__context__);
            return true;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // expLoop
            aotLib[0xb1257f787d1e1f5f] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(expLoop)>>();
            };
            // expLoopU
            aotLib[0x3b6672d35f5f6887] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(expLoopU)>>();
            };
            // expLoopUV
            aotLib[0xffe70d9155a9c61] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(expLoopUV)>>();
            };
            // test
            aotLib[0x320335ac69f5e9bf] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test)>>();
            };
        }
    }
}
