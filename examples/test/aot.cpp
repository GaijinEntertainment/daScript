#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {

    namespace aot {
        TypeInfo __type_info__ec0ec894 = { Type::tUInt, nullptr, nullptr, /*annotation*/ nullptr, nullptr, nullptr, 0, nullptr, 12, 0xec0ec894 };
        TypeInfo * __tinfo_0[1] = { &__type_info__ec0ec894 };

        int32_t dict ( Context * __context__, TTable<char *,int32_t> & tab, TArray<char *> & src );
        void makeRandomSequence ( Context * __context__, TArray<char *> & src );
        void resize ( Context * __context__, TArray<char *> & Arr, int32_t newSize );
        bool test ( Context * __context__ );

        int32_t dict ( Context * __context__, TTable<char *,int32_t> &  tab, TArray<char *> &  src )
        {
            builtin_table_clear(tab,__context__);
            int32_t maxOcc = 0;
            {
                bool __need_loop_16 = true;
                das_iterator<TArray<char *>> __s_iterator(src);
                char * * s;
                __need_loop_16 = __s_iterator.first(__context__,s) && __need_loop_16;
                for ( ; __need_loop_16 ; __need_loop_16 = __s_iterator.next(__context__,s) )
                {
                    maxOcc = SimPolicy<int32_t>::Max(++tab((*s),__context__),maxOcc,*__context__);
                }
                __s_iterator.close(__context__,s);
            };
            return maxOcc;
        }

        void makeRandomSequence ( Context * __context__, TArray<char *> &  src )
        {
            resize(__context__,src,500000);
            {
                bool __need_loop_9 = true;
                das_iterator<range> __i_iterator(range(0,500000));
                int32_t i;
                __need_loop_9 = __i_iterator.first(__context__,i) && __need_loop_9;
                for ( ; __need_loop_9 ; __need_loop_9 = __i_iterator.next(__context__,i) )
                {
                    uint32_t num = ((0x1033c4d7u ^ uint32_t(i * 119)) % 0x7a120u);
                    src(i,__context__) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_0, cast<uint32_t>::from(num)));
                }
                __i_iterator.close(__context__,i);
            };
        }

        void resize ( Context * __context__, TArray<char *> &  Arr, int32_t newSize )
        {
            builtin_array_resize(Arr,newSize,8,__context__);
        }

        bool test ( Context * __context__ )
        {
            TTable<char *,int32_t> tab; das_zero(tab);
            TArray<char *> src; das_zero(src);
            makeRandomSequence(__context__,src);
            builtin_profile(20,"dictionary",das_make_block<void>(__context__,[&]()->void{
                dict(__context__,tab,src);
            }),__context__);
            return true;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // dict
            aotLib[0xae931145262ae07a] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(dict)>>();
            };
            // makeRandomSequence
            aotLib[0xe95eb3db2fdfd555] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(makeRandomSequence)>>();
            };
            // resize
            aotLib[0xe9cd57f4be1ff7f7] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(resize)>>();
            };
            // test
            aotLib[0xc397fa877b1da7ce] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test)>>();
            };
        }
    }
}
