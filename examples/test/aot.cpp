#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {
    namespace aot {
        void erase ( Context * __context__, TArray<int32_t> & Arr, int32_t at );
        void pass_array ( Context * __context__, TArray<int32_t> & arr );
        void push ( Context * __context__, TArray<int32_t> & Arr, int32_t value, int32_t at );
        void reserve ( Context * __context__, TArray<int32_t> & Arr, int32_t newSize );
        void resize ( Context * __context__, TArray<int32_t> & Arr, int32_t newSize );
        bool test ( Context * __context__ );

        void erase ( Context * __context__, TArray<int32_t> &  Arr, int32_t at )
        {
            builtin_array_erase(Arr,at,4,__context__);
        }

        void pass_array ( Context * __context__, TArray<int32_t> &  arr )
        {
            push(__context__,arr,4,-1);
        }

        void push ( Context * __context__, TArray<int32_t> &  Arr, int32_t value, int32_t at = -1 )
        {
            Arr(builtin_array_push(Arr,at,4,__context__),__context__) = value;
        }

        void reserve ( Context * __context__, TArray<int32_t> &  Arr, int32_t newSize )
        {
            builtin_array_reserve(Arr,newSize,4,__context__);
        }

        void resize ( Context * __context__, TArray<int32_t> &  Arr, int32_t newSize )
        {
            builtin_array_resize(Arr,newSize,4,__context__);
        }

        bool test ( Context * __context__ )
        {
            { /* let */
                TArray<int32_t> arr; das_zero(arr); TArray<int32_t> arr2; das_zero(arr2); {
                    DAS_ASSERTF(builtin_array_size(arr) == 0,"array starts at 0 length");
                    push(__context__,arr,1,-1);
                    push(__context__,arr,2,-1);
                    push(__context__,arr,3,-1);
                    DAS_ASSERTF(builtin_array_size(arr) == 3,"array length is 3");
                    DAS_ASSERTF(builtin_array_capacity(arr) == 16,"its 2x, but no less than 16");
                    pass_array(__context__,arr);
                    DAS_ASSERTF(builtin_array_size(arr) == 4,"array length is 4");
                    resize(__context__,arr,6);
                    DAS_ASSERTF(builtin_array_size(arr) == 6,"array length is now 6");
                    resize(__context__,arr,17);
                    DAS_ASSERTF(builtin_array_capacity(arr) == 32,"now bigger");
                    reserve(__context__,arr,33);
                    DAS_ASSERTF(builtin_array_capacity(arr) == 33,"now even bigger");
                    resize(__context__,arr,4);
                    DAS_ASSERTF(builtin_array_capacity(arr) == 33,"did not grow smaller");
                    DAS_ASSERTF(builtin_array_size(arr) == 4,"resized properly");
                    das_move(arr2,arr);
                    DAS_ASSERT(builtin_array_size(arr) == 0);
                    DAS_ASSERT(builtin_array_size(arr2) == 4);
                    { /* let */
                        int32_t index = 0; {
                            while ( index < 4 )
                            {
                                arr2(index++,__context__)++;
                            };
                        }
                    };
                    das_move(arr,arr2);
                    DAS_ASSERT(builtin_array_size(arr2) == 0);
                    DAS_ASSERT(builtin_array_size(arr) == 4);
                    resize(__context__,arr,3);
                    DAS_ASSERT(arr(2,__context__) == 4);
                    resize(__context__,arr,4);
                    DAS_ASSERTF(arr(3,__context__) == 0,"new element after resize is 0");
                    push(__context__,arr,1,0);
                    {
                        bool __need_loop_41 = true;
                        das_iterator<range> __t_iterator(range(1,5));
                        int32_t t;
                        __need_loop_41 = __t_iterator.first(__context__,t) && __need_loop_41;
                        for ( ; __need_loop_41 ; __need_loop_41 = __t_iterator.next(__context__,t) )
                        {
                            DAS_ASSERT(arr((t - 1),__context__) == t);
                        }
                        __t_iterator.close(__context__,t);
                    };
                    resize(__context__,arr,4);
                    push(__context__,arr,5,4);
                    push(__context__,arr,7,2);
                    erase(__context__,arr,2);
                    {
                        bool __need_loop_47 = true;
                        das_iterator<range> __t_iterator(range(1,5));
                        int32_t t;
                        __need_loop_47 = __t_iterator.first(__context__,t) && __need_loop_47;
                        for ( ; __need_loop_47 ; __need_loop_47 = __t_iterator.next(__context__,t) )
                        {
                            DAS_ASSERT(arr((t - 1),__context__) == t);
                        }
                        __t_iterator.close(__context__,t);
                    };
                    erase(__context__,arr,0);
                    erase(__context__,arr,3);
                    {
                        bool __need_loop_51 = true;
                        das_iterator<range> __t_iterator(range(2,4));
                        int32_t t;
                        __need_loop_51 = __t_iterator.first(__context__,t) && __need_loop_51;
                        for ( ; __need_loop_51 ; __need_loop_51 = __t_iterator.next(__context__,t) )
                        {
                            DAS_ASSERT(arr((t - 2),__context__) == t);
                        }
                        __t_iterator.close(__context__,t);
                    };
                    TArray<int32_t> a; das_zero(a);
                    push(__context__,a,1,-1);
                    push(__context__,a,2,-1);
                    TArray<int32_t> b = a;
                    DAS_ASSERT((builtin_array_size(a) == 0) & (builtin_array_size(b) == 2));
                }
            };
            return true;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // erase
            aotLib[0x9b8ff4fd5f68660f] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(erase)>>();
            };
            // pass_array
            aotLib[0x6983bd750fff848a] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(pass_array)>>();
            };
            // push
            aotLib[0xf444c43590897ecc] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(push)>>();
            };
            // reserve
            aotLib[0x4ab498a257a2e4d5] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(reserve)>>();
            };
            // resize
            aotLib[0xee40c19956b18323] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(resize)>>();
            };
            // test
            aotLib[0xfc4f6c30fe3057d6] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test)>>();
            };
        }
    }
}
