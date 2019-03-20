#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "test_profile.h"

namespace das {
    namespace aot {
        struct NObject {
            float3 position;
            float pad1;
            float3 velocity;
            float pad2;
        };
        static_assert(sizeof(NObject)==32,"structure size mismatch with DAS");

        void init ( Context * __context__, TArray<struct NObject> & objects );
        void resize ( Context * __context__, TArray<struct NObject> & Arr, int32_t newSize );
        bool test ( Context * __context__ );
        void testSim ( Context * __context__, TArray<struct NObject> & objects );
        void testSim2 ( Context * __context__, TArray<struct NObject> & objects, int32_t count );
        void testSim2I ( Context * __context__, TArray<struct NObject> & objects, int32_t count );
        void testSimI ( Context * __context__, TArray<struct NObject> & objects );
        void update ( Context * __context__, struct NObject & a );

        void __init_script ( Context * __context__ )
        {
        }

        void init ( Context * __context__, TArray<struct NObject> &  objects )
        {
            resize(__context__,das_arg<TArray<struct NObject>>::pass(objects),50000);
            int32_t i = 0;
            {
                bool __need_loop_35 = true;
                das_iterator<TArray<struct NObject>> __obj_iterator(objects);
                struct NObject * obj;
                __need_loop_35 = __obj_iterator.first(__context__,obj) && __need_loop_35;
                for ( ; __need_loop_35 ; __need_loop_35 = __obj_iterator.next(__context__,obj) )
                {
                    (*obj).position = float3(i++,i + 1,i + 2);
                    (*obj).velocity = v_make_vec4f(1.f,2.f,3.f,0.f);
                }
                __obj_iterator.close(__context__,obj);
            };
            DAS_ASSERT((i == builtin_array_size(das_arg<TArray<struct NObject>>::pass(objects))));
        }

        void resize ( Context * __context__, TArray<struct NObject> &  Arr, int32_t newSize )
        {
            builtin_array_resize(das_arg<TArray<struct NObject>>::pass(Arr),newSize,32,__context__);
        }

        bool test ( Context * __context__ ) { das_stack_prologue __prologue(__context__,128,__LINE__);
            {
                TArray<struct NObject> objects; das_zero(objects);
                init(__context__,das_arg<TArray<struct NObject>>::pass(objects));
                builtin_profile(20,"particles kinematics",das_arg<const Block /*void*/>::pass(das_make_block<void>(__context__,0,[&]()->void{
                    testSim2(__context__,das_arg<TArray<struct NObject>>::pass(objects),100);
                })),__context__);
                builtin_profile(20,"particles kinematics, inlined",das_arg<const Block /*void*/>::pass(das_make_block<void>(__context__,0,[&]()->void{
                    testSim2I(__context__,das_arg<TArray<struct NObject>>::pass(objects),100);
                })),__context__);
                return true;
            }}

        void testSim ( Context * __context__, TArray<struct NObject> &  objects )
        {
            {
                bool __need_loop_17 = true;
                das_iterator<TArray<struct NObject>> __obj_iterator(objects);
                struct NObject * obj;
                __need_loop_17 = __obj_iterator.first(__context__,obj) && __need_loop_17;
                for ( ; __need_loop_17 ; __need_loop_17 = __obj_iterator.next(__context__,obj) )
                {
                    update(__context__,das_arg<struct NObject>::pass((*obj)));
                }
                __obj_iterator.close(__context__,obj);
            };
        }

        void testSim2 ( Context * __context__, TArray<struct NObject> &  objects, int32_t count )
        {
            {
                bool __need_loop_21 = true;
                das_iterator<range> __i_iterator(range(0,count));
                int32_t i;
                __need_loop_21 = __i_iterator.first(__context__,i) && __need_loop_21;
                for ( ; __need_loop_21 ; __need_loop_21 = __i_iterator.next(__context__,i) )
                {
                    testSim(__context__,das_arg<TArray<struct NObject>>::pass(objects));
                }
                __i_iterator.close(__context__,i);
            };
        }

        void testSim2I ( Context * __context__, TArray<struct NObject> &  objects, int32_t count )
        {
            {
                bool __need_loop_29 = true;
                das_iterator<range> __i_iterator(range(0,count));
                int32_t i;
                __need_loop_29 = __i_iterator.first(__context__,i) && __need_loop_29;
                for ( ; __need_loop_29 ; __need_loop_29 = __i_iterator.next(__context__,i) )
                {
                    testSimI(__context__,das_arg<TArray<struct NObject>>::pass(objects));
                }
                __i_iterator.close(__context__,i);
            };
        }

        void testSimI ( Context * __context__, TArray<struct NObject> &  objects )
        {
            {
                bool __need_loop_25 = true;
                das_iterator<TArray<struct NObject>> __obj_iterator(objects);
                struct NObject * obj;
                __need_loop_25 = __obj_iterator.first(__context__,obj) && __need_loop_25;
                for ( ; __need_loop_25 ; __need_loop_25 = __obj_iterator.next(__context__,obj) )
                {
                    SimPolicy<float3>::SetAdd((char *)&((*obj).position),(*obj).velocity,*__context__);
                }
                __obj_iterator.close(__context__,obj);
            };
        }

        void update ( Context * __context__, struct NObject &  a )
        {
            SimPolicy<float3>::SetAdd((char *)&(a.position),a.velocity,*__context__);
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // init
            aotLib[0xd0a04d597abf98e6] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, TArray<struct NObject> & objects ),init>>();
            };
            // resize
            aotLib[0x74f80c933eeb043f] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, TArray<struct NObject> & Arr, int32_t newSize ),resize>>();
            };
            // test
            aotLib[0x12a91e62a039b87e] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test>>();
            };
            // testSim
            aotLib[0x424f1c06fda0915] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, TArray<struct NObject> & objects ),testSim>>();
            };
            // testSim2
            aotLib[0x25de74e07521a1e1] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, TArray<struct NObject> & objects, int32_t count ),testSim2>>();
            };
            // testSim2I
            aotLib[0x9bb9d4057e379790] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, TArray<struct NObject> & objects, int32_t count ),testSim2I>>();
            };
            // testSimI
            aotLib[0xbc6f5e71df0cf5af] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, TArray<struct NObject> & objects ),testSimI>>();
            };
            // update
            aotLib[0x34565643b147cd37] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, struct NObject & a ),update>>();
            };
        }
    }
}
