#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {
    namespace aot {
        struct body {
            float3 x;
            float3 v;
            float mass;
        };
        static_assert(sizeof(body)==28,"structure size mismatch with DAS");

        void advance ( Context * __context__ );
        float energy ( Context * __context__ );
        void nbodies ( Context * __context__, int32_t n );
        void offset_momentum ( Context * __context__ );
        void scale_bodies ( Context * __context__, float scale );
        bool test ( Context * __context__ );

        void __init_script ( Context * __context__ )
        {
            das_global<float,0>(__context__) = 39.4784203f; /*SOLAR_MASS*/
            das_global<TDim<struct body,5>,16>(__context__) = (([&]() -> TDim<struct body,5> {
                TDim<struct body,5> __mks_17; das_zero(__mks_17);
                __mks_17(0,__context__).x = float3(0.f,0.f,0.f);
                __mks_17(0,__context__).v = float3(0.f,0.f,0.f);
                __mks_17(0,__context__).mass = das_global<float,0>(__context__) /*SOLAR_MASS*/;
                __mks_17(1,__context__).x = float3(4.84143162f,-1.16032004f,-0.103622042f);
                __mks_17(1,__context__).v = float3(0.606326401f,2.81198692f,-0.0252183601f);
                __mks_17(1,__context__).mass = 0.0376936793f;
                __mks_17(2,__context__).x = float3(8.34336662f,4.12479877f,-0.403523415f);
                __mks_17(2,__context__).v = float3(-1.01077425f,1.82566226f,0.0084157614f);
                __mks_17(2,__context__).mass = 0.0112863267f;
                __mks_17(3,__context__).x = float3(12.8943691f,-15.1111517f,-0.22330758f);
                __mks_17(3,__context__).v = float3(1.08279097f,0.868712962f,-0.0108326375f);
                __mks_17(3,__context__).mass = 0.00172372418f;
                __mks_17(4,__context__).x = float3(15.3796968f,-25.9193153f,0.179258779f);
                __mks_17(4,__context__).v = float3(0.979090691f,0.594698966f,-0.0347559527f);
                __mks_17(4,__context__).mass = 0.00203368696f;
                return __mks_17;
            })()); /*bodies*/
        }

        void advance ( Context * __context__ )
        {
            int32_t i = 0;
            {
                bool __need_loop_48 = true;
                das_iterator<TDim<struct body,5>> __b_iterator(das_global<TDim<struct body,5>,16>(__context__) /*bodies*/);
                struct body * b;
                __need_loop_48 = __b_iterator.first(__context__,b) && __need_loop_48;
                for ( ; __need_loop_48 ; __need_loop_48 = __b_iterator.next(__context__,b) )
                {
                    ++i;
                    {
                        bool __need_loop_50 = true;
                        das_iterator<range> __j_iterator(range(i,5));
                        int32_t j;
                        __need_loop_50 = __j_iterator.first(__context__,j) && __need_loop_50;
                        for ( ; __need_loop_50 ; __need_loop_50 = __j_iterator.next(__context__,j) )
                        {
                            struct body * b2 = &(das_global<TDim<struct body,5>,16>(__context__) /*bodies*/(j,__context__));
                            vec4f /*float3*/ dx = (SimPolicy<float3>::Sub((*b).x,(*b2).x,*__context__));
                            float inv_distance = invlength3(dx);
                            float mag = ((inv_distance * inv_distance) * inv_distance);
                            SimPolicy<float3>::SetSub((char *)&((*b).v),(SimPolicy<float3>::MulVecScal(dx,cast<float>::from(((*b2).mass * mag)),*__context__)),*__context__);
                            SimPolicy<float3>::SetAdd((char *)&((*b2).v),(SimPolicy<float3>::MulVecScal(dx,cast<float>::from(((*b).mass * mag)),*__context__)),*__context__);
                        }
                        __j_iterator.close(__context__,j);
                    };
                    SimPolicy<float3>::SetAdd((char *)&((*b).x),(*b).v,*__context__);
                }
                __b_iterator.close(__context__,b);
            };
        }

        float energy ( Context * __context__ )
        {
            float e = 0.f;
            int32_t i = 0;
            {
                bool __need_loop_62 = true;
                das_iterator<TDim<struct body,5>> __b_iterator(das_global<TDim<struct body,5>,16>(__context__) /*bodies*/);
                struct body * b;
                __need_loop_62 = __b_iterator.first(__context__,b) && __need_loop_62;
                for ( ; __need_loop_62 ; __need_loop_62 = __b_iterator.next(__context__,b) )
                {
                    e += ((0.5f * (*b).mass) * lengthSq3((*b).v));
                    ++i;
                    {
                        bool __need_loop_65 = true;
                        das_iterator<range> __j_iterator(range(i,5));
                        int32_t j;
                        __need_loop_65 = __j_iterator.first(__context__,j) && __need_loop_65;
                        for ( ; __need_loop_65 ; __need_loop_65 = __j_iterator.next(__context__,j) )
                        {
                            struct body b2 = das_global<TDim<struct body,5>,16>(__context__) /*bodies*/(j,__context__);
                            e -= (((*b).mass * b2.mass) / distance3((*b).x,b2.x));
                        }
                        __j_iterator.close(__context__,j);
                    };
                }
                __b_iterator.close(__context__,b);
            };
            return e;
        }

        void nbodies ( Context * __context__, int32_t n )
        {
            scale_bodies(__context__,0.00999999978f);
            {
                bool __need_loop_83 = true;
                das_iterator<range> __i_iterator(range(0,n));
                int32_t i;
                __need_loop_83 = __i_iterator.first(__context__,i) && __need_loop_83;
                for ( ; __need_loop_83 ; __need_loop_83 = __i_iterator.next(__context__,i) )
                {
                    advance(__context__);
                }
                __i_iterator.close(__context__,i);
            };
            scale_bodies(__context__,100.f);
        }

        void offset_momentum ( Context * __context__ )
        {
            vec4f /*float3*/ px = v_zero();
            {
                bool __need_loop_42 = true;
                das_iterator<TDim<struct body,5>> __b_iterator(das_global<TDim<struct body,5>,16>(__context__) /*bodies*/);
                struct body * b;
                __need_loop_42 = __b_iterator.first(__context__,b) && __need_loop_42;
                for ( ; __need_loop_42 ; __need_loop_42 = __b_iterator.next(__context__,b) )
                {
                    SimPolicy<float3>::SetSub((char *)&(px),(SimPolicy<float3>::MulVecScal((*b).v,cast<float>::from((*b).mass),*__context__)),*__context__);
                }
                __b_iterator.close(__context__,b);
            };
            das_global<TDim<struct body,5>,16>(__context__) /*bodies*/(0,__context__).v = SimPolicy<float3>::DivVecScal(px,cast<float>::from(39.4784203f),*__context__);
        }

        void scale_bodies ( Context * __context__, float scale )
        {
            {
                bool __need_loop_77 = true;
                das_iterator<TDim<struct body,5>> __b_iterator(das_global<TDim<struct body,5>,16>(__context__) /*bodies*/);
                struct body * b;
                __need_loop_77 = __b_iterator.first(__context__,b) && __need_loop_77;
                for ( ; __need_loop_77 ; __need_loop_77 = __b_iterator.next(__context__,b) )
                {
                    (*b).mass *= (scale * scale);
                    (*b).v = SimPolicy<float3>::MulVecScal((*b).v,cast<float>::from(scale),*__context__);
                }
                __b_iterator.close(__context__,b);
            };
        }

        bool test ( Context * __context__ ) { das_stack_prologue __prologue(__context__,64,__LINE__);
            {
                offset_momentum(__context__);
                energy(__context__);
                builtin_profile(10,"n-bodies",das_arg<const Block /*void*/>::pass(das_make_block<void>(__context__,0,[&]()->void{
                    nbodies(__context__,500000);
                })),__context__);
                energy(__context__);
                return true;
            }}

        void registerAot ( AotLibrary & aotLib )
        {
            // advance
            aotLib[0x88fac1e358ae686c] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__ ),advance>>();
            };
            // energy
            aotLib[0xcb235b5fa943f532] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<float  ( Context * __context__ ),energy>>();
            };
            // nbodies
            aotLib[0x6cab10ae37f8c1ee] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, int32_t n ),nbodies>>();
            };
            // offset_momentum
            aotLib[0x7c0b25327df46fa5] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__ ),offset_momentum>>();
            };
            // scale_bodies
            aotLib[0x8cf14e05025b3fe3] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, float scale ),scale_bodies>>();
            };
            // test
            aotLib[0x97a151fdb739f6e5] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test>>();
            };
            // [[ init script ]]
            aotLib[0x4377a0f657ee8435] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void (Context *), __init_script>>();
            };
        }
    }
}
