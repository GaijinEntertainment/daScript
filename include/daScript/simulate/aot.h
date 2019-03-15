
#include "daScript/simulate/runtime_profile.h"
#include "daScript/simulate/debug_print.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/aot_builtin.h"
#include "daScript/simulate/aot_builtin_math.h"

namespace das {

    template <typename TT>
    __forceinline void das_zero ( TT & a ) {
        memset(&a, 0, sizeof(TT));
    }

    template <typename TT>
    struct TArray : Array {
        __forceinline TT & operator () ( int32_t index, Context * __context__ ) {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error("index out of range");
            return ((TT *)data)[index];
        }
        __forceinline const TT & operator () ( int32_t index, Context * __context__ ) const {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error("index out of range");
            return ((TT *)data)[index];
        }
        __forceinline TT & operator () ( uint32_t idx, Context * __context__ ) {
            if ( idx>=size ) __context__->throw_error("index out of range");
            return ((TT *)data)[index];
        }
        __forceinline const TT & operator () ( uint32_t idx, Context * __context__ ) const {
            if ( idx>=size ) __context__->throw_error("index out of range");
            return ((TT *)data)[index];
        }
    };

    template <typename TK, typename TV>
    struct TTable : Table {
        __forceinline TV & operator () ( const TK & key, Context * __context__ ) {
            TableHash<TK> thh(__context__,sizeof(TV));
            auto hfn = hash_function(*__context__, key);
            int index = thh.reserve(*this, key, hfn);
            return ((TV *)data)[index];
        }
    };

    template <typename TT>
    struct das_iterator;

    template <>
    struct das_iterator <range> {
        __forceinline das_iterator(const range & r) : that(r) {}
        __forceinline bool first ( Context *, int32_t & i ) { i = that.from; return i!=that.to; }
        __forceinline bool next  ( Context *, int32_t & i ) { i++; return i!=that.to; }
        __forceinline void close ( Context *, int32_t &   ) {}
        range that;
    };

    template <typename TT>
    struct das_iterator<TArray<TT>> {
        __forceinline das_iterator(TArray<TT> & r) : that(&r) {
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
        __forceinline das_make_block ( Context * context, BlockFn && func )
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
        __forceinline SimNode_Aot ( ) : SimNode_CallBase(LineInfo()) {}
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
        __forceinline SimNode_AotInteropBase() : SimNode_CallBase(LineInfo()) {}
        virtual vec4f eval ( Context & ) override {
            return v_zero();
        };
        vec4f *     argumentValues;
    };

    template <int argumentCount>
    struct SimNode_AotInterop : SimNode_AotInteropBase {
        template <typename ...VI>
        __forceinline SimNode_AotInterop ( TypeInfo ** tinfo, VI... args ) {
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
        __forceinline SimNode_AotInterop () {
            nArguments = 0;
            types = nullptr;
            argumentValues = nullptr;
        }
    };

    __forceinline char * das_string_builder ( Context * __context__, const SimNode_AotInteropBase & node ) {
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
}

