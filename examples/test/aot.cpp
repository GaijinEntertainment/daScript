#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {
    namespace aot {
        void make_test_array ( Context * __context__, TArray<int32_t> & a, int32_t extra );
        void resize ( Context * __context__, TArray<int32_t> & Arr, int32_t newSize );
        bool test ( Context * __context__ );
        void verify_test_array ( Context * __context__, TArray<int32_t> & a, int32_t extra );

        void make_test_array ( Context * __context__, TArray<int32_t> &  a, int32_t extra = 0 )
        {
            int32_t i = 0;
            resize(__context__,a,10);
            {
                bool __need_loop_8 = true;
                das_iterator<TArray<int32_t>> __x_iterator(a);
                int32_t * x;
                __need_loop_8 = __x_iterator.first(__context__,x) && __need_loop_8;
                for ( ; __need_loop_8 ; __need_loop_8 = __x_iterator.next(__context__,x) )
                {
                    (*x) = i++ + extra;
                }
                __x_iterator.close(__context__,x);
            };
        }

        void resize ( Context * __context__, TArray<int32_t> &  Arr, int32_t newSize )
        {
            builtin_array_resize(Arr,newSize,4,__context__);
        }

        bool test ( Context * __context__ )
        {
            { /* let */
                TArray<int32_t> a; das_zero(a); {
                    make_test_array(__context__,a,0);
                    make_test_array(__context__,a,13);
                    make_test_array(__context__,a,0);
                    { /* let */
                        TArray<int32_t> b = a; {
                            DAS_ASSERT(builtin_array_size(a) == 0);
                            das_move(a,b);
                        }
                    };
                }
            };
            { /* let */
                TArray<int32_t> a; das_zero(a); TArray<int32_t> b; das_zero(b); int32_t i = 0; {
                    resize(__context__,a,10);
                    resize(__context__,b,10);
                    {
                        bool __need_loop_94 = true;
                        das_iterator<TArray<int32_t>> __l_iterator(a);
                        int32_t * l;
                        __need_loop_94 = __l_iterator.first(__context__,l) && __need_loop_94;
                        das_iterator<TArray<int32_t>> __r_iterator(b);
                        int32_t * r;
                        __need_loop_94 = __r_iterator.first(__context__,r) && __need_loop_94;
                        for ( ; __need_loop_94 ; __need_loop_94 = __l_iterator.next(__context__,l) && __r_iterator.next(__context__,r) )
                        {
                            (*l) = i;
                            (*r) = (i * 2) - 5;
                            i++;
                        }
                        __l_iterator.close(__context__,l);
                        __r_iterator.close(__context__,r);
                    };
                    i = 0;
                    {
                        bool __need_loop_100 = true;
                        das_iterator<TArray<int32_t>> __l_iterator(a);
                        int32_t * l;
                        __need_loop_100 = __l_iterator.first(__context__,l) && __need_loop_100;
                        das_iterator<TArray<int32_t>> __r_iterator(b);
                        int32_t * r;
                        __need_loop_100 = __r_iterator.first(__context__,r) && __need_loop_100;
                        for ( ; __need_loop_100 ; __need_loop_100 = __l_iterator.next(__context__,l) && __r_iterator.next(__context__,r) )
                        {
                            DAS_ASSERT((*l) == i);
                            DAS_ASSERT((*r) == ((i * 2) - 5));
                            i++;
                        }
                        __l_iterator.close(__context__,l);
                        __r_iterator.close(__context__,r);
                    };
                    i = 0;
                    {
                        bool __need_loop_105 = true;
                        das_iterator<TArray<int32_t>> __l_iterator(a);
                        int32_t * l;
                        __need_loop_105 = __l_iterator.first(__context__,l) && __need_loop_105;
                        das_iterator<TArray<int32_t>> __r_iterator(b);
                        int32_t * r;
                        __need_loop_105 = __r_iterator.first(__context__,r) && __need_loop_105;
                        for ( ; __need_loop_105 ; __need_loop_105 = __l_iterator.next(__context__,l) && __r_iterator.next(__context__,r) )
                        {
                            if ( (*l) > (*r) )
                            {
                                DAS_ASSERT((*l) > (*r));
                                i++;
                            };
                        }
                        __l_iterator.close(__context__,l);
                        __r_iterator.close(__context__,r);
                    };
                    DAS_ASSERT(i == 5);
                }
            };
            { /* let */
                TDim<TArray<int32_t>,10> a; das_zero(a); int32_t i = 0; {
                    {
                        bool __need_loop_111 = true;
                        das_iterator<TDim<TArray<int32_t>,10>> __b_iterator(a);
                        TArray<int32_t> * b;
                        __need_loop_111 = __b_iterator.first(__context__,b) && __need_loop_111;
                        for ( ; __need_loop_111 ; __need_loop_111 = __b_iterator.next(__context__,b) )
                        {
                            make_test_array(__context__,(*b),i++);
                        }
                        __b_iterator.close(__context__,b);
                    };
                    i = 0;
                    {
                        bool __need_loop_114 = true;
                        das_iterator<TDim<TArray<int32_t>,10>> __b_iterator(a);
                        TArray<int32_t> * b;
                        __need_loop_114 = __b_iterator.first(__context__,b) && __need_loop_114;
                        for ( ; __need_loop_114 ; __need_loop_114 = __b_iterator.next(__context__,b) )
                        {
                            verify_test_array(__context__,(*b),i++);
                        }
                        __b_iterator.close(__context__,b);
                    };
                }
            };
            return true;
        }

        void verify_test_array ( Context * __context__, TArray<int32_t> &  a, int32_t extra = 0 )
        {
            DAS_ASSERT(builtin_array_size(a) == 10);
            { /* let */
                int32_t i = 0; {
                    {
                        bool __need_loop_14 = true;
                        das_iterator<TArray<int32_t>> __x_iterator(a);
                        int32_t * x;
                        __need_loop_14 = __x_iterator.first(__context__,x) && __need_loop_14;
                        for ( ; __need_loop_14 ; __need_loop_14 = __x_iterator.next(__context__,x) )
                        {
                            DAS_ASSERT((*x) == (extra + i++));
                        }
                        __x_iterator.close(__context__,x);
                    };
                }
            };
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // make_test_array
            aotLib[0x9dffbe8b284cd090] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(make_test_array)>>();
            };
            // resize
            aotLib[0xee40c19956b18323] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(resize)>>();
            };
            // test
            aotLib[0x5c0b8124a2b250ff] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test)>>();
            };
            // verify_test_array
            aotLib[0xfa2f4f2092a0f924] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(verify_test_array)>>();
            };
        }
    }
}
