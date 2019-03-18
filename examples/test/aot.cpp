#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {

    namespace aot {
        TypeInfo __type_info__33aa4c93 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("$::float4x4"), nullptr, nullptr, 0, nullptr, 14, 0x33aa4c93 };
        TypeInfo * __tinfo_0[1] = { &__type_info__33aa4c93 };

        TypeInfo * __tinfo_1[1] = { &__type_info__33aa4c93 };

        TypeInfo __type_info__a7069c83 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 4, 0xa7069c83 };
        TypeInfo * __tinfo_2[3] = { &__type_info__a7069c83, &__type_info__33aa4c93, &__type_info__a7069c83 };

        int32_t pass ( Context * __context__, int32_t a );
        bool test ( Context * __context__ );
        void test_4x4 ( Context * __context__ );
        void test_any ( Context * __context__, /* handled */ float3x4 & t );
        void test_any ( Context * __context__, /* handled */ float4x4 & t );
        bool test_r2v ( Context * __context__ );

        int32_t pass ( Context * __context__, int32_t a )
        {
            return a;
        }

        bool test ( Context * __context__ )
        {
            test_r2v(__context__);
            test_4x4(__context__);
            test_any(__context__,das_arg</* handled */ float4x4>::pass(/*c-tor*/ float4x4()));
            test_any(__context__,das_arg</* handled */ float3x4>::pass(/*c-tor*/ float3x4()));
            return true;
        }

        void test_4x4 ( Context * __context__ )
        {
            /* handled */ float4x4 t; das_zero(t);
            das_index</* handled */ float4x4>::at(t,0,__context__) = float4(1,2,3,4);
            das_index</* handled */ float4x4>::at(t,1,__context__) = float4(5,6,7,8);
            das_index</* handled */ float4x4>::at(t,2,__context__) = float4(9,10,11,12);
            das_index</* handled */ float4x4>::at(t,3,__context__) = float4(13,14,15,16);
            DAS_ASSERT(SimPolicy<char *>::Equ(cast<char *>::from(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_0, cast</* handled */ float4x4 &>::from(t)))),cast<char *>::from("[[ 1,2,3,4; 5,6,7,8; 9,10,11,12; 13,14,15,16]]"),*__context__));
            {
                bool __need_loop_39 = true;
                das_iterator<range> __y_iterator(range(0,4));
                int32_t y;
                __need_loop_39 = __y_iterator.first(__context__,y) && __need_loop_39;
                for ( ; __need_loop_39 ; __need_loop_39 = __y_iterator.next(__context__,y) )
                {
                    {
                        bool __need_loop_40 = true;
                        das_iterator<range> __x_iterator(range(0,4));
                        int32_t x;
                        __need_loop_40 = __x_iterator.first(__context__,x) && __need_loop_40;
                        for ( ; __need_loop_40 ; __need_loop_40 = __x_iterator.next(__context__,x) )
                        {
                            das_index<float4>::at(das_index</* handled */ float4x4>::at(t,y,__context__),x,__context__) = (x == y) ? 1 : 0;
                        }
                        __x_iterator.close(__context__,x);
                    };
                }
                __y_iterator.close(__context__,y);
            };
            t.m[0] /*x*/ = float4(1,2,3,4);
            t.m[1] /*y*/ = float4(5,6,7,8);
            t.m[2] /*z*/ = float4(9,10,11,12);
            t.m[3] /*w*/ = float4(13,14,15,16);
            DAS_ASSERT(SimPolicy<char *>::Equ(cast<char *>::from(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_1, cast</* handled */ float4x4 &>::from(t)))),cast<char *>::from("[[ 1,2,3,4; 5,6,7,8; 9,10,11,12; 13,14,15,16]]"),*__context__));
            float4x4_identity(das_arg</* handled */ float4x4>::pass(t));
            bool ops = false;
            das_try_recover(__context__, [&]()
                            {
                                das_index</* handled */ float4x4>::at(t,pass(__context__,4),__context__) = float4(0,0,0,0);
                            }, [&]()
                            {
                                ops = true;
                            });
            DAS_ASSERT(ops);
        }

        void test_any ( Context * __context__, /* handled */ float3x4 &  t )
        {
            float3x4_identity(das_arg</* handled */ float3x4>::pass(t));
            {
                bool __need_loop_68 = true;
                das_iterator<range> __r_iterator(range(0,4));
                int32_t r;
                __need_loop_68 = __r_iterator.first(__context__,r) && __need_loop_68;
                for ( ; __need_loop_68 ; __need_loop_68 = __r_iterator.next(__context__,r) )
                {
                    das_index</* handled */ float3x4>::at(t,r,__context__) = float3(r);
                }
                __r_iterator.close(__context__,r);
            };
            {
                bool __need_loop_71 = true;
                das_iterator<range> __y_iterator(range(0,4));
                int32_t y;
                __need_loop_71 = __y_iterator.first(__context__,y) && __need_loop_71;
                for ( ; __need_loop_71 ; __need_loop_71 = __y_iterator.next(__context__,y) )
                {
                    {
                        bool __need_loop_72 = true;
                        das_iterator<range> __x_iterator(range(0,3));
                        int32_t x;
                        __need_loop_72 = __x_iterator.first(__context__,x) && __need_loop_72;
                        for ( ; __need_loop_72 ; __need_loop_72 = __x_iterator.next(__context__,x) )
                        {
                            das_index<float3>::at(das_index</* handled */ float3x4>::at(t,y,__context__),x,__context__) = (x == y) ? 1 : 0;
                        }
                        __x_iterator.close(__context__,x);
                    };
                }
                __y_iterator.close(__context__,y);
            };
            bool ops = false;
            das_try_recover(__context__, [&]()
                            {
                                das_index</* handled */ float3x4>::at(t,pass(__context__,4),__context__) = float3(0,0,0);
                            }, [&]()
                            {
                                ops = true;
                            });
            DAS_ASSERT(ops);
        }

        void test_any ( Context * __context__, /* handled */ float4x4 &  t )
        {
            float4x4_identity(das_arg</* handled */ float4x4>::pass(t));
            {
                bool __need_loop_68 = true;
                das_iterator<range> __r_iterator(range(0,4));
                int32_t r;
                __need_loop_68 = __r_iterator.first(__context__,r) && __need_loop_68;
                for ( ; __need_loop_68 ; __need_loop_68 = __r_iterator.next(__context__,r) )
                {
                    das_index</* handled */ float4x4>::at(t,r,__context__) = float4(r);
                }
                __r_iterator.close(__context__,r);
            };
            {
                bool __need_loop_71 = true;
                das_iterator<range> __y_iterator(range(0,4));
                int32_t y;
                __need_loop_71 = __y_iterator.first(__context__,y) && __need_loop_71;
                for ( ; __need_loop_71 ; __need_loop_71 = __y_iterator.next(__context__,y) )
                {
                    {
                        bool __need_loop_72 = true;
                        das_iterator<range> __x_iterator(range(0,4));
                        int32_t x;
                        __need_loop_72 = __x_iterator.first(__context__,x) && __need_loop_72;
                        for ( ; __need_loop_72 ; __need_loop_72 = __x_iterator.next(__context__,x) )
                        {
                            das_index<float4>::at(das_index</* handled */ float4x4>::at(t,y,__context__),x,__context__) = (x == y) ? 1 : 0;
                        }
                        __x_iterator.close(__context__,x);
                    };
                }
                __y_iterator.close(__context__,y);
            };
            bool ops = false;
            das_try_recover(__context__, [&]()
                            {
                                das_index</* handled */ float4x4>::at(t,pass(__context__,4),__context__) = float4(0,0,0,0);
                            }, [&]()
                            {
                                ops = true;
                            });
            DAS_ASSERT(ops);
        }

        bool test_r2v ( Context * __context__ )
        {
            /* handled */ float4x4 m; das_zero(m);
            das_index</* handled */ float4x4>::at(m,0,__context__) = float4(1,2,3,4);
            das_swizzle_ref<float,float4,1>::swizzle(das_index</* handled */ float4x4>::at(m,1,__context__)) /*y*/ = 6;
            das_index</* handled */ float4x4>::at(m,2,__context__) = float4(5,6,7,8);
            m.m[3] /*w*/ = float4(9,10,11,12);
            das_swizzle_ref<float,float4,2>::swizzle(m.m[3] /*w*/) /*z*/ = 111;
            DAS_ASSERT(SimPolicy<float4>::Equ(das_index</* handled */ float4x4>::at(m,0,__context__),float4(1,2,3,4),*__context__));
            DAS_ASSERT(v_extract_y(das_index</* handled */ float4x4>::at(m,1,__context__)) /*y*/ == 6);
            DAS_ASSERT(SimPolicy<float4>::Equ(das_index</* handled */ float4x4>::at(m,1,__context__),float4(0,6,0,0),*__context__));
            DAS_ASSERT(SimPolicy<float4>::Equ(das_index</* handled */ float4x4>::at(m,2,__context__),float4(5,6,7,8),*__context__));
            DAS_ASSERT(v_extract_z(m.m[3] /*w*/) /*z*/ == 111);
            DAS_ASSERT(SimPolicy<float4>::Equ(m.m[3] /*w*/,float4(9,10,111,12),*__context__));
            builtin_print(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_2, cast<char *>::from("m="), cast</* handled */ float4x4 &>::from(m), cast<char *>::from("\n"))),__context__);
            return true;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // pass
            aotLib[0x7cee40be92f7b250] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<int32_t  ( Context * __context__, int32_t a ),pass>>();
            };
            // test
            aotLib[0x2dc28c4625a2adf5] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test>>();
            };
            // test_4x4
            aotLib[0xe0f179c45d837e99] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__ ),test_4x4>>();
            };
            // test_any
            aotLib[0x9ada529c42cdf44c] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, /* handled */ float3x4 & t ),test_any>>();
            };
            // test_any
            aotLib[0x921192e5ce9a10e6] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, /* handled */ float4x4 & t ),test_any>>();
            };
            // test_r2v
            aotLib[0x22ae33f40a8d16d8] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test_r2v>>();
            };
        }
    }
}
