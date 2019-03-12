#include "daScript/daScript.h"

#include <daScript/simulate/runtime_profile.h>

namespace das {

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

    namespace aot {
        int fibI ( Context * __context__, int n )
        {
            int last = 0;
            int cur = 1;
            {
                bool __need_loop = true;
                das_iterator<range> __i_iterator(range(0,n - 1));
                int i;
                __need_loop = __i_iterator.first(__context__,i) && __need_loop;
                for ( ; __need_loop ; __need_loop &= __need_loop && __i_iterator.next(__context__,i) )
                {
                    int tmp = cur;
                    cur += last;
                    last = tmp;
                }
                __i_iterator.close(__context__,i);
            };
            return cur;
        }

        int fibR ( Context * __context__, int n )
        {
            return (n < 2) ? n : (fibR(__context__,n - 1) + fibR(__context__,n - 2));
        }

        bool test ( Context * __context__ )
        {
            int ii, rr;
            builtin_profile(20,"fibbonacci loop",das_make_block<void>(__context__,[&]()->void{
                ii = fibI(__context__,6511134);
            }),__context__);
            builtin_profile(20,"fibbonacci recursive",das_make_block<void>(__context__,[&]()->void{
                rr = fibR(__context__,31);
            }),__context__);
            printf("%i %i\n", ii, rr);
            return true;
        }

        void registerAot(AotLibrary & aotLib)
        {
            // fibI
            aotLib[0xfb2b67397b8e1040] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(fibI)>>();
            };
            // fibR
            aotLib[0x1699e592401b4ce5] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(fibR)>>();
            };
            // test
            aotLib[0xda3d144b0aa3a86a] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test)>>();
            };
        }
    }
}
