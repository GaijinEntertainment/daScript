#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {
    namespace aot {
        int32_t badd ( Context * __context__, int32_t ext, const Block /*int32_t,int32_t*/ & b );
        void crapOnStack ( Context * __context__ );
        void func1 ( Context * __context__, const Block /*void*/ & b );
        void func2 ( Context * __context__, const Block /*void*/ & b );
        void func3 ( Context * __context__, const Block /*void*/ & b );
        void func4 ( Context * __context__, const Block /*void*/ & b );
        int32_t radd ( Context * __context__, int32_t & ext, const Block /*int32_t,int32_t &*/ & b );
        bool test ( Context * __context__ );
        void twice ( Context * __context__, const Block /*void*/ & b );
        int32_t twice_good ( Context * __context__, const Block /*int32_t*/ & b );

        int32_t badd ( Context * __context__, int32_t ext, const Block /*int32_t,int32_t*/ &  b )
        {
            return das_invoke<int32_t>::invoke<int32_t>(__context__,b,ext);
        }

        void crapOnStack ( Context * __context__ )
        {
            { /* let */
                TDim<int32_t,20> a; das_zero(a); {
                    {
                        bool __need_loop_16 = true;
                        das_iterator<TDim<int32_t,20>> __b_iterator(a);
                        int32_t * b;
                        __need_loop_16 = __b_iterator.first(__context__,b) && __need_loop_16;
                        for ( ; __need_loop_16 ; __need_loop_16 = __b_iterator.next(__context__,b) )
                        {
                            (*b) = 13;
                        }
                        __b_iterator.close(__context__,b);
                    };
                }
            };
        }

        void func1 ( Context * __context__, const Block /*void*/ &  b )
        {
            crapOnStack(__context__);
            das_invoke<void>::invoke(__context__,b);
            crapOnStack(__context__);
        }

        void func2 ( Context * __context__, const Block /*void*/ &  b )
        {
            crapOnStack(__context__);
            func3(__context__,b);
            crapOnStack(__context__);
        }

        void func3 ( Context * __context__, const Block /*void*/ &  b )
        {
            crapOnStack(__context__);
            das_invoke<void>::invoke(__context__,b);
            crapOnStack(__context__);
        }

        void func4 ( Context * __context__, const Block /*void*/ &  b )
        {
            crapOnStack(__context__);
            das_invoke<void>::invoke(__context__,b);
            crapOnStack(__context__);
        }

        int32_t radd ( Context * __context__, int32_t & ext, const Block /*int32_t,int32_t &*/ &  b )
        {
            return das_invoke<int32_t>::invoke<int32_t &>(__context__,b,ext);
        }

        bool test ( Context * __context__ ) { das_stack_prologue __prologue(__context__,640,__LINE__);
            {
                { /* let */
                    int32_t a = 0; int32_t b = 0; {
                        twice(__context__,das_make_block<void>(__context__,0,[&]()->void{
                            a++;
                        }));
                        DAS_ASSERT(a == 2);
                        b = twice_good(__context__,das_make_block<int32_t>(__context__,0,[&]()->int32_t{
                            return a++;
                        }));
                        DAS_ASSERT((a == 4) & (b == 5));
                        b = twice_good(__context__,das_make_block<int32_t>(__context__,0,[&]()->int32_t{
                            return a++;
                        }));
                        DAS_ASSERT(b == 9);
                        b = twice_good(__context__,das_make_block<int32_t>(__context__,0,[&]()->int32_t{
                            return ++a;
                        }));
                        DAS_ASSERT(a == 8);
                        DAS_ASSERT(b == 15);
                    }
                };
                { /* let */
                    int32_t x = 0; int32_t y = 0; {
                        func1(__context__,das_make_block<void>(__context__,0,[&]()->void{
                            x += 1;
                            y += 2;
                            func2(__context__,das_make_block<void>(__context__,0,[&]()->void{
                                x++;
                                y += 3;
                            }));
                            func4(__context__,das_make_block<void>(__context__,0,[&]()->void{
                                x++;
                                y += 4;
                            }));
                        }));
                        DAS_ASSERT((x == 3) & (y == 9));
                    }
                };
                { /* let */
                    int32_t res = 0; int32_t v1 = 0; {
                        v1 = 1;
                        res = badd(__context__,1000,das_make_block<int32_t,int32_t>(__context__,384,[&](int32_t a1)->int32_t{
                            return v1++ + a1;
                        }));
                        DAS_ASSERT(res == 1001);
                        res = badd(__context__,1000,das_make_block<int32_t,int32_t>(__context__,432,[&](int32_t a1)->int32_t{
                            return v1 + a1;
                        }));
                        DAS_ASSERT(res == 1002);
                        v1 = 3;
                        res = badd(__context__,1000,das_make_block<int32_t,int32_t>(__context__,480,[&](int32_t a1)->int32_t{
                            return v1 + a1;
                        }));
                        DAS_ASSERT(res == 1003);
                        v1 = 4;
                        res = badd(__context__,1000,das_make_block<int32_t,int32_t>(__context__,528,[&](int32_t a2)->int32_t{
                            return badd(__context__,2000,das_make_block<int32_t,int32_t>(__context__,576,[&](int32_t a3)->int32_t{
                                return (a3 + a2) + v1;
                            }));
                        }));
                        DAS_ASSERT(res == 3004);
                        v1 = 3;
                        res = radd(__context__,v1,das_make_block<int32_t,int32_t &>(__context__,624,[&](int32_t & a)->int32_t{
                            return a++;
                        }));
                        DAS_ASSERT((res == 3) & (v1 == 4));
                    }
                };
                return true;
            }}

        void twice ( Context * __context__, const Block /*void*/ &  b )
        {
            das_invoke<void>::invoke(__context__,b);
            das_invoke<void>::invoke(__context__,b);
        }

        int32_t twice_good ( Context * __context__, const Block /*int32_t*/ &  b )
        {
            { /* let */
                int32_t t = 0; {
                    t += das_invoke<int32_t>::invoke(__context__,b);
                    t += das_invoke<int32_t>::invoke(__context__,b);
                    return t;
                }
            };
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // badd
            aotLib[0xcc64039be0d22892] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(badd)>>();
            };
            // crapOnStack
            aotLib[0x48d0f12386b5efe4] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(crapOnStack)>>();
            };
            // func1
            aotLib[0xbf334782166a82f4] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(func1)>>();
            };
            // func2
            aotLib[0xf58374cab137b34c] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(func2)>>();
            };
            // func3
            aotLib[0xbf334782166a82f4] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(func3)>>();
            };
            // func4
            aotLib[0xbf334782166a82f4] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(func4)>>();
            };
            // radd
            aotLib[0xcc64039be0d22892] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(radd)>>();
            };
            // test
            aotLib[0x4ea0e4a2ccfc6c1f] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test)>>();
            };
            // twice
            aotLib[0x16230c47da52b5d3] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(twice)>>();
            };
            // twice_good
            aotLib[0x2b784ebea222fe9b] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(twice_good)>>();
            };
        }
    }
}
