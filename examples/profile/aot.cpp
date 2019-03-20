#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "test_profile.h"

namespace das {
    namespace aot {
        bool test ( Context * __context__ );
        int32_t testAdds ( Context * __context__ );

        void __init_script ( Context * __context__ )
        {
        }

        bool test ( Context * __context__ ) { das_stack_prologue __prologue(__context__,80,__LINE__);
        {
            int32_t count = 0;
            builtin_profile(20,"native loop",das_arg<const Block /*void*/>::pass(das_make_block<void>(__context__,0,[&]()->void{
                count = testAdds(__context__);
            })),__context__);
            DAS_ASSERT((10000000 == count));
            return true;
        }}

        int32_t testAdds ( Context * __context__ )
        {
            int32_t count = 0;
            {
                bool __need_loop_8 = true;
                das_iterator<range> __i_iterator(range(0,10000000));
                int32_t i;
                __need_loop_8 = __i_iterator.first(__context__,i) && __need_loop_8;
                for ( ; __need_loop_8 ; __need_loop_8 = __i_iterator.next(__context__,i) )
                {
                    count = AddOne(count);
                }
                __i_iterator.close(__context__,i);
            };
            return count;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // test
            aotLib[0xae64d95eaa39e898] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test>>();
            };
            // testAdds
            aotLib[0x90579899daba5b44] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<int32_t  ( Context * __context__ ),testAdds>>();
            };
        }
    }
}
