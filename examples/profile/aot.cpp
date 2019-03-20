#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "test_profile.h"

namespace das {
    namespace aot {
        struct body {
            float3 x;
            float pad;
            float3 v;
            float mass;
        };
        static_assert(sizeof(body)==32,"structure size mismatch with DAS");

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
                TDim<struct body,5> __mks_21; das_zero(__mks_21);
                __mks_21(0,__context__).x = v_zero();
                __mks_21(0,__context__).v = v_zero();
                __mks_21(0,__context__).mass = das_global<float,0>(__context__) /*SOLAR_MASS*/;
                __mks_21(1,__context__).x = v_make_vec4f(4.84143162f,-1.16032004f,-0.103622042f,0.f);
                __mks_21(1,__context__).v = v_make_vec4f(0.606326401f,2.81198692f,-0.0252183601f,0.f);
                __mks_21(1,__context__).mass = 0.0376936793f;
                __mks_21(2,__context__).x = v_make_vec4f(8.34336662f,4.12479877f,-0.403523415f,0.f);
                __mks_21(2,__context__).v = v_make_vec4f(-1.01077425f,1.82566226f,0.0084157614f,0.f);
                __mks_21(2,__context__).mass = 0.0112863267f;
                __mks_21(3,__context__).x = v_make_vec4f(12.8943691f,-15.1111517f,-0.22330758f,0.f);
                __mks_21(3,__context__).v = v_make_vec4f(1.08279097f,0.868712962f,-0.0108326375f,0.f);
                __mks_21(3,__context__).mass = 0.00172372418f;
                __mks_21(4,__context__).x = v_make_vec4f(15.3796968f,-25.9193153f,0.179258779f,0.f);
                __mks_21(4,__context__).v = v_make_vec4f(0.979090691f,0.594698966f,-0.0347559527f,0.f);
                __mks_21(4,__context__).mass = 0.00203368696f;
                return __mks_21;
            })()); /*bodies*/
        }

        void advance ( Context * __context__ )
        {
            int32_t i = 0;
            {
                bool __need_loop_52 = true;
                das_iterator<TDim<struct body,5>> __b_iterator(das_global<TDim<struct body,5>,16>(__context__) /*bodies*/);
                struct body * b;
                __need_loop_52 = __b_iterator.first(__context__,b) && __need_loop_52;
                for ( ; __need_loop_52 ; __need_loop_52 = __b_iterator.next(__context__,b) )
                {
                    ++i;
                    {
                        bool __need_loop_54 = true;
                        das_iterator<range> __j_iterator(range(i,5));
                        int32_t j;
                        __need_loop_54 = __j_iterator.first(__context__,j) && __need_loop_54;
                        for ( ; __need_loop_54 ; __need_loop_54 = __j_iterator.next(__context__,j) )
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
                bool __need_loop_66 = true;
                das_iterator<TDim<struct body,5>> __b_iterator(das_global<TDim<struct body,5>,16>(__context__) /*bodies*/);
                struct body * b;
                __need_loop_66 = __b_iterator.first(__context__,b) && __need_loop_66;
                for ( ; __need_loop_66 ; __need_loop_66 = __b_iterator.next(__context__,b) )
                {
                    e += ((0.5f * (*b).mass) * lengthSq3((*b).v));
                    ++i;
                    {
                        bool __need_loop_69 = true;
                        das_iterator<range> __j_iterator(range(i,5));
                        int32_t j;
                        __need_loop_69 = __j_iterator.first(__context__,j) && __need_loop_69;
                        for ( ; __need_loop_69 ; __need_loop_69 = __j_iterator.next(__context__,j) )
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
                bool __need_loop_87 = true;
                das_iterator<range> __i_iterator(range(0,n));
                int32_t i;
                __need_loop_87 = __i_iterator.first(__context__,i) && __need_loop_87;
                for ( ; __need_loop_87 ; __need_loop_87 = __i_iterator.next(__context__,i) )
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
                bool __need_loop_46 = true;
                das_iterator<TDim<struct body,5>> __b_iterator(das_global<TDim<struct body,5>,16>(__context__) /*bodies*/);
                struct body * b;
                __need_loop_46 = __b_iterator.first(__context__,b) && __need_loop_46;
                for ( ; __need_loop_46 ; __need_loop_46 = __b_iterator.next(__context__,b) )
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
                bool __need_loop_81 = true;
                das_iterator<TDim<struct body,5>> __b_iterator(das_global<TDim<struct body,5>,16>(__context__) /*bodies*/);
                struct body * b;
                __need_loop_81 = __b_iterator.first(__context__,b) && __need_loop_81;
                for ( ; __need_loop_81 ; __need_loop_81 = __b_iterator.next(__context__,b) )
                {
                    (*b).mass *= (scale * scale);
                    (*b).v = SimPolicy<float3>::MulVecScal((*b).v,cast<float>::from(scale),*__context__);
                }
                __b_iterator.close(__context__,b);
            };
        }

        void registerAot(AotLibrary & aotLib)
        {
            // advance
            aotLib[0x409c2e31049d34a0] = [&](Context & ctx) {
                return ctx.code->makeNode<SimNode_Aot<void(Context * __context__), advance>>();
            };
            // energy
            aotLib[0x439708a57727de5e] = [&](Context & ctx) {
                return ctx.code->makeNode<SimNode_Aot<float(Context * __context__), energy>>();
            };
            // nbodies
            aotLib[0x6cab10ae37f8c1ee] = [&](Context & ctx) {
                return ctx.code->makeNode<SimNode_Aot<void(Context * __context__, int32_t n), nbodies>>();
            };
            // offset_momentum
            aotLib[0x7c98c79d74e250a1] = [&](Context & ctx) {
                return ctx.code->makeNode<SimNode_Aot<void(Context * __context__), offset_momentum>>();
            };
            // scale_bodies
            aotLib[0x717eb94a5739626b] = [&](Context & ctx) {
                return ctx.code->makeNode<SimNode_Aot<void(Context * __context__, float scale), scale_bodies>>();
            };
            // [[ init script ]]
            aotLib[0xf80607b48201d41] = [&](Context & ctx) {
                return ctx.code->makeNode<SimNode_Aot<void(Context *), __init_script>>();
            };
        }
    }
}
