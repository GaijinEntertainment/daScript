#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {

    struct das_stack_prologue {
        __forceinline das_stack_prologue ( Context * __context__, uint32_t stackSize, int32_t line )
            : context(__context__) {
            if (!context->stack.push(stackSize, EP, SP)) {
                context->throw_error("stack overflow");
            }
#if DAS_ENABLE_STACK_WALK
            Prologue * pp = (Prologue *)context->stack.sp();
            pp->arguments = nullptr;    // arguments
            pp->info = nullptr;         // FunctionDebugInfo
            pp->line = line;
#endif
        }
        __forceinline ~das_stack_prologue () {
            context->stack.pop(EP, SP);
        }
        Context * context;
        char * EP, * SP;
    };

    template <typename ResType>
    struct das_invoke {
        template <typename ...ArgType>
        static __forceinline ResType invoke ( Context * __context__, const Block & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<ArgType>::from(arg)... };
            vec4f result = __context__->invoke(blk, arguments, nullptr);
            return cast<ResType>::to(result);
        }
    };

    template <>
    struct das_invoke<void> {
        template <typename ...ArgType>
        static __forceinline void invoke ( Context * __context__, const Block & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<ArgType>::from(arg)... };
            __context__->invoke(blk, arguments, nullptr);
        }
    };


    template <typename resType, typename ...argType>
    struct das_make_block : Block, SimNode {
        typedef function < resType ( argType... ) > BlockFn;
        __forceinline das_make_block ( Context * context, uint32_t argStackTop, BlockFn && func )
        : SimNode(LineInfo()), blockFunction(func) {
            stackOffset = context->stack.spi();
            argumentsOffset = argStackTop ? (context->stack.spi() + argStackTop) : 0;
            body = this;
            functionArguments = context->abiArguments();
        };
        virtual vec4f eval ( Context & context ) override {
            vec4f ** arguments = (vec4f **)(context.stack.bottom() + argumentsOffset);
            vec4f * arg = *arguments;
            return cast<resType>::from ( blockFunction ( cast<argType>::to(*arg++)... ) );
        }
        BlockFn blockFunction;
    };

    namespace aot {
        int32_t badd ( Context * __context__, int32_t ext, const Block /*int32_t,int32_t*/ & b );
        bool test ( Context * __context__ );

        int32_t badd ( Context * __context__, int32_t ext, const Block /*int32_t,int32_t*/ &  b ) { das_stack_prologue __prologue(__context__,32,__LINE__);
            {
                return das_invoke<int32_t>::invoke<int32_t>(__context__,b,ext);
            }}

        bool test ( Context * __context__ ) { das_stack_prologue __prologue(__context__,96,__LINE__);
            {
                int32_t res = badd(__context__,1000,das_make_block<int32_t,int32_t>(__context__,80,[&](int32_t a1)->int32_t{
                    return 3 + a1;
                }));
                DAS_ASSERT(res == 1003);
                return true;
            }}

        void registerAot ( AotLibrary & aotLib )
        {
            // badd
            aotLib[0xcc64039be0d22892] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(badd)>>();
            };
            // test
            aotLib[0xe3cbe80141123b50] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test)>>();
            };
        }
    }
}
