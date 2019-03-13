#include "daScript/daScript.h"

#include "daScript/simulate/runtime_profile.h"
#include "daScript/simulate/debug_print.h"

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

    namespace aot {
        void registerAot ( AotLibrary & aotLib )
        {
        }
    }
}
