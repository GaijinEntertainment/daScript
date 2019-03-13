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

    __forceinline void operator += ( float3 & a, const float3 & b ) {
        float3 * pa = (float3 *) &a;
        *pa = cast<float3>::to ( v_add(cast<float3>::from(*pa), b));
    }

    namespace aot {
        struct NObject {
            float3 position;
            float3 velocity;
        };

        void init ( Context * __context__, TArray<struct NObject> & objects );
        void resize ( Context * __context__, TArray<struct NObject> & Arr, int32_t newSize );
        bool test ( Context * __context__ );
        void testSim ( Context * __context__, TArray<struct NObject> & objects );
        void testSim2 ( Context * __context__, TArray<struct NObject> & objects, int32_t count );
        void testSim2I ( Context * __context__, TArray<struct NObject> & objects, int32_t count );
        void testSimI ( Context * __context__, TArray<struct NObject> & objects );
        void update ( Context * __context__, struct NObject & a );

        void init ( Context * __context__, TArray<struct NObject> &  objects )
        {
            resize(__context__,objects,50000);
            int32_t i = 0;
            {
                bool __need_loop_28 = true;
                das_iterator<TArray<struct NObject>> __obj_iterator(objects);
                struct NObject * obj;
                __need_loop_28 = __obj_iterator.first(__context__,obj) && __need_loop_28;
                for ( ; __need_loop_28 ; __need_loop_28 &= __need_loop_28 && __obj_iterator.next(__context__,obj) )
                {
                    (*obj).position = float3(i++,i + 1,i + 2);
                    (*obj).velocity = float3(1,2,3);
                }
                __obj_iterator.close(__context__,obj);
            };
            assert(i == builtin_array_size(objects));
        }

        void resize ( Context * __context__, TArray<struct NObject> &  Arr, int32_t newSize )
        {
            builtin_array_resize(Arr,newSize,24,__context__);
        }

        bool test ( Context * __context__ )
        {
            TArray<struct NObject> objects;
            init(__context__,objects);
            builtin_profile(20,"particles kinematics",das_make_block<void>(__context__,[&]()->void{
                testSim2(__context__,objects,100);
            }),__context__);
            builtin_profile(20,"particles kinematics, inlined",das_make_block<void>(__context__,[&]()->void{
                testSim2I(__context__,objects,100);
            }),__context__);
            return true;
        }

        void testSim ( Context * __context__, TArray<struct NObject> &  objects )
        {
            {
                bool __need_loop_10 = true;
                das_iterator<TArray<struct NObject>> __obj_iterator(objects);
                struct NObject * obj;
                __need_loop_10 = __obj_iterator.first(__context__,obj) && __need_loop_10;
                for ( ; __need_loop_10 ; __need_loop_10 &= __need_loop_10 && __obj_iterator.next(__context__,obj) )
                {
                    update(__context__,(*obj));
                }
                __obj_iterator.close(__context__,obj);
            };
        }

        void testSim2 ( Context * __context__, TArray<struct NObject> &  objects, int32_t count )
        {
            {
                bool __need_loop_14 = true;
                das_iterator<range> __i_iterator(range(0,count));
                int32_t i;
                __need_loop_14 = __i_iterator.first(__context__,i) && __need_loop_14;
                for ( ; __need_loop_14 ; __need_loop_14 &= __need_loop_14 && __i_iterator.next(__context__,i) )
                {
                    testSim(__context__,objects);
                }
                __i_iterator.close(__context__,i);
            };
        }

        void testSim2I ( Context * __context__, TArray<struct NObject> &  objects, int32_t count )
        {
            {
                bool __need_loop_22 = true;
                das_iterator<range> __i_iterator(range(0,count));
                int32_t i;
                __need_loop_22 = __i_iterator.first(__context__,i) && __need_loop_22;
                for ( ; __need_loop_22 ; __need_loop_22 &= __need_loop_22 && __i_iterator.next(__context__,i) )
                {
                    testSimI(__context__,objects);
                }
                __i_iterator.close(__context__,i);
            };
        }

        void testSimI ( Context * __context__, TArray<struct NObject> &  objects )
        {
            {
                bool __need_loop_18 = true;
                das_iterator<TArray<struct NObject>> __obj_iterator(objects);
                struct NObject * obj;
                __need_loop_18 = __obj_iterator.first(__context__,obj) && __need_loop_18;
                for ( ; __need_loop_18 ; __need_loop_18 &= __need_loop_18 && __obj_iterator.next(__context__,obj) )
                {
                    (*obj).position += (*obj).velocity;
                }
                __obj_iterator.close(__context__,obj);
            };
        }

        void update ( Context * __context__, struct NObject &  a )
        {
            a.position += a.velocity;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // init
            aotLib[0x2ef0c2d40c7b9f7a] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(init)>>();
            };
            // resize
            aotLib[0x15055da9cbaf5ce7] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(resize)>>();
            };
            // test
            aotLib[0x12a91e62a039b87e] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test)>>();
            };
            // testSim
            aotLib[0x1beaf59f1850ac7d] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(testSim)>>();
            };
            // testSim2
            aotLib[0x25de74e07521a1e1] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(testSim2)>>();
            };
            // testSim2I
            aotLib[0x9bb9d4057e379790] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(testSim2I)>>();
            };
            // testSimI
            aotLib[0x7d97eddd19b95e0b] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(testSimI)>>();
            };
            // update
            aotLib[0xa9d1143522a8b7d3] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(update)>>();
            };
        }
    }
}
