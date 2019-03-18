#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {

    namespace aot {

        TypeInfo __type_info__a7069c83 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 4, 0xa7069c83 };
        TypeInfo __type_info__33aa4c93 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("$::float4x4"), nullptr, nullptr, 0, nullptr, 14, 0x33aa4c93 };
        TypeInfo * __tinfo_0[3] = { &__type_info__a7069c83, &__type_info__33aa4c93, &__type_info__a7069c83 };

        bool test ( Context * __context__ );
        bool test_r2v ( Context * __context__ );

        bool test ( Context * __context__ )
        {
            test_r2v(__context__);
            return true;
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
            builtin_print(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char *>::from("m="), cast</* handled */ float4x4 &>::from(m), cast<char *>::from("\n"))),__context__);
            return true;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // test
            aotLib[0x743518538dc6f792] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test)>>();
            };
            // test_r2v
            aotLib[0x22ae33f40a8d16d8] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<DAS_BIND_FUN(test_r2v)>>();
            };
        }
    }
}
