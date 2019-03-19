#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {

    namespace aot {
        TypeInfo __type_info__ec0ec894 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 12, 0xec0ec894 };
        TypeInfo * __tinfo_0[1] = { &__type_info__ec0ec894 };

        int32_t dict ( Context * __context__, TTable<char *,int32_t> & tab, TArray<char *> & src );
        void makeRandomSequence ( Context * __context__, TArray<char *> & src );
        void resize ( Context * __context__, TArray<char *> & Arr, int32_t newSize );
        bool test ( Context * __context__ );

        int32_t dict ( Context * __context__, TTable<char *,int32_t> &  tab, TArray<char *> &  src )
        {
            builtin_table_clear(das_arg<TTable<char *,int32_t>>::pass(tab),__context__);
            int32_t maxOcc = 0;
            {
                bool __need_loop_18 = true;
                das_iterator<TArray<char *>> __s_iterator(src);
                char * * s;
                __need_loop_18 = __s_iterator.first(__context__,s) && __need_loop_18;
                for ( ; __need_loop_18 ; __need_loop_18 = __s_iterator.next(__context__,s) )
                {
                    maxOcc = SimPolicy<int32_t>::Max(++tab((*s),__context__),maxOcc,*__context__);
                }
                __s_iterator.close(__context__,s);
            };
            return maxOcc;
        }

        void makeRandomSequence ( Context * __context__, TArray<char *> &  src )
        {
            resize(__context__,das_arg<TArray<char *>>::pass(src),500000);
            {
                bool __need_loop_11 = true;
                das_iterator<range> __i_iterator(range(0,500000));
                int32_t i;
                __need_loop_11 = __i_iterator.first(__context__,i) && __need_loop_11;
                for ( ; __need_loop_11 ; __need_loop_11 = __i_iterator.next(__context__,i) )
                {
                    uint32_t num = ((0x1033c4d7u ^ uint32_t(i * 119)) % 0x7a120u);
                    src(i,__context__) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_0, cast<uint32_t>::from(num)));
                }
                __i_iterator.close(__context__,i);
            };
        }

        void resize ( Context * __context__, TArray<char *> &  Arr, int32_t newSize )
        {
            builtin_array_resize(das_arg<TArray<char *>>::pass(Arr),newSize,8,__context__);
        }

        bool test ( Context * __context__ ) { das_stack_prologue __prologue(__context__,144,__LINE__);
            {
                TTable<char *,int32_t> tab; das_zero(tab);
                TArray<char *> src; das_zero(src);
                makeRandomSequence(__context__,das_arg<TArray<char *>>::pass(src));
                builtin_profile(20,"dictionary",das_arg<const Block /*void*/>::pass(das_make_block<void>(__context__,0,[&]()->void{
                    dict(__context__,das_arg<TTable<char *,int32_t>>::pass(tab),das_arg<TArray<char *>>::pass(src));
                })),__context__);
                DAS_ASSERT(heap_depth(__context__) == 1);
                return true;
            }}

        void registerAot ( AotLibrary & aotLib )
        {
            // dict
            aotLib[0xae931145262ae07a] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<int32_t  ( Context * __context__, TTable<char *,int32_t> & tab, TArray<char *> & src ),dict>>();
            };
            // makeRandomSequence
            aotLib[0xe95eb3db2fdfd555] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, TArray<char *> & src ),makeRandomSequence>>();
            };
            // resize
            aotLib[0xe9cd57f4be1ff7f7] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, TArray<char *> & Arr, int32_t newSize ),resize>>();
            };
            // test
            aotLib[0xa64f158bcf3d1c9d] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test>>();
            };
        }
    }
}
