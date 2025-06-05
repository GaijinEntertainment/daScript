#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require math_bits

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)   // unreferenced formal parameter
#pragma warning(disable:4189)   // local variable is initialized but not referenced
#pragma warning(disable:4244)   // conversion from 'int32_t' to 'float', possible loss of data
#pragma warning(disable:4114)   // same qualifier more than once
#pragma warning(disable:4623)   // default constructor was implicitly defined as deleted
#pragma warning(disable:4946)   // reinterpret_cast used between related classes
#pragma warning(disable:4269)   // 'const' automatic data initialized with compiler generated default constructor produces unreliable results
#pragma warning(disable:4555)   // result of expression not used
#endif
#if defined(__EDG__)
#pragma diag_suppress 826
#elif defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wwritable-strings"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#pragma clang diagnostic ignored "-Wunsequenced"
#pragma clang diagnostic ignored "-Wunused-function"
#endif

namespace das {
namespace _anon_4402346706582421074 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline float int_bits_to_float_5661d7c872a376d7 ( Context * __context__, int32_t __x_rename_at_12_0 );
inline double int64_bits_to_double_548b085f12a59c21 ( Context * __context__, int64_t __x_rename_at_19_1 );
inline float2 int_bits_to_float_89c9cee88d8c0768 ( Context * __context__, int2 __x_rename_at_26_2 );
inline float3 int_bits_to_float_e2c2dbacebc7057 ( Context * __context__, int3 __x_rename_at_32_3 );
inline float4 int_bits_to_float_f7016869e1d4c441 ( Context * __context__, int4 __x_rename_at_38_4 );
inline float uint_bits_to_float_186d5760444bbb85 ( Context * __context__, uint32_t __x_rename_at_44_5 );
inline double uint64_bits_to_double_2f9a8df65ab2c19b ( Context * __context__, uint64_t __x_rename_at_51_6 );
inline float2 uint_bits_to_float_37f5acb6e42a3a72 ( Context * __context__, uint2 __x_rename_at_58_7 );
inline float3 uint_bits_to_float_bd1aead92fb7c892 ( Context * __context__, uint3 __x_rename_at_64_8 );
inline float4 uint_bits_to_float_621fe31896217edd ( Context * __context__, uint4 __x_rename_at_70_9 );
inline int32_t float_bits_to_int_d77cfe445a750463 ( Context * __context__, float __x_rename_at_76_10 );
inline int64_t double_bits_to_int64_b277151c3c6ab1e5 ( Context * __context__, double __x_rename_at_83_11 );
inline int2 float_bits_to_int_b6985b657f9ab1a4 ( Context * __context__, float2 __x_rename_at_90_12 );
inline int3 float_bits_to_int_ce48b037ccb96327 ( Context * __context__, float3 __x_rename_at_96_13 );
inline int4 float_bits_to_int_8f0076236d1fb70d ( Context * __context__, float4 __x_rename_at_102_14 );
inline uint32_t float_bits_to_uint_749d2563863b6705 ( Context * __context__, float __x_rename_at_108_15 );
inline uint64_t double_bits_to_uint64_80e4483b964d603f ( Context * __context__, double __x_rename_at_115_16 );
inline uint2 float_bits_to_uint_15dba7c285276ce2 ( Context * __context__, float2 __x_rename_at_122_17 );
inline uint3 float_bits_to_uint_372a5f9e37619b1e ( Context * __context__, float3 __x_rename_at_128_18 );
inline uint4 float_bits_to_uint_2ba26bb1c092ef0d ( Context * __context__, float4 __x_rename_at_134_19 );
inline float4 cast_to_vec4f_51ca011a5e87b774 ( Context * __context__, bool __x_rename_at_152_20 );
inline float4 cast_to_vec4f_3eb54330cb0e2cf1 ( Context * __context__, int64_t __x_rename_at_161_22 );
inline int64_t cast_to_int64_4ec4ddc73187c475 ( Context * __context__, float4 __data_rename_at_170_24 );
inline int32_t cast_to_int32_633048e550d83e51 ( Context * __context__, float4 __data_rename_at_179_26 );
inline int16_t cast_to_int16_ad1316b2e3e0c4b2 ( Context * __context__, float4 __data_rename_at_188_28 );
inline int8_t cast_to_int8_49a1b1a5bbb7e65 ( Context * __context__, float4 __data_rename_at_197_30 );
inline char * cast_to_string_f9e08771dd99d6d9 ( Context * __context__, float4 __data_rename_at_206_32 );
inline void * cast_to_pointer_319c7779d940a37d ( Context * __context__, float4 __data_rename_at_215_34 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline float int_bits_to_float_5661d7c872a376d7 ( Context * __context__, int32_t __x_rename_at_12_0 )
{
    return das_auto_cast<float>::cast(das_cast<float>::cast(__x_rename_at_12_0));
}

inline double int64_bits_to_double_548b085f12a59c21 ( Context * __context__, int64_t __x_rename_at_19_1 )
{
    return das_auto_cast<double>::cast(das_cast<double>::cast(__x_rename_at_19_1));
}

inline float2 int_bits_to_float_89c9cee88d8c0768 ( Context * __context__, int2 __x_rename_at_26_2 )
{
    return das_auto_cast<float2>::cast(das_cast<float2>::cast(__x_rename_at_26_2));
}

inline float3 int_bits_to_float_e2c2dbacebc7057 ( Context * __context__, int3 __x_rename_at_32_3 )
{
    return das_auto_cast<float3>::cast(das_cast<float3>::cast(__x_rename_at_32_3));
}

inline float4 int_bits_to_float_f7016869e1d4c441 ( Context * __context__, int4 __x_rename_at_38_4 )
{
    return das_auto_cast<float4>::cast(das_cast<float4>::cast(__x_rename_at_38_4));
}

inline float uint_bits_to_float_186d5760444bbb85 ( Context * __context__, uint32_t __x_rename_at_44_5 )
{
    return das_auto_cast<float>::cast(das_cast<float>::cast(__x_rename_at_44_5));
}

inline double uint64_bits_to_double_2f9a8df65ab2c19b ( Context * __context__, uint64_t __x_rename_at_51_6 )
{
    return das_auto_cast<double>::cast(das_cast<double>::cast(__x_rename_at_51_6));
}

inline float2 uint_bits_to_float_37f5acb6e42a3a72 ( Context * __context__, uint2 __x_rename_at_58_7 )
{
    return das_auto_cast<float2>::cast(das_cast<float2>::cast(__x_rename_at_58_7));
}

inline float3 uint_bits_to_float_bd1aead92fb7c892 ( Context * __context__, uint3 __x_rename_at_64_8 )
{
    return das_auto_cast<float3>::cast(das_cast<float3>::cast(__x_rename_at_64_8));
}

inline float4 uint_bits_to_float_621fe31896217edd ( Context * __context__, uint4 __x_rename_at_70_9 )
{
    return das_auto_cast<float4>::cast(das_cast<float4>::cast(__x_rename_at_70_9));
}

inline int32_t float_bits_to_int_d77cfe445a750463 ( Context * __context__, float __x_rename_at_76_10 )
{
    return das_auto_cast<int32_t>::cast(das_cast<int32_t>::cast(__x_rename_at_76_10));
}

inline int64_t double_bits_to_int64_b277151c3c6ab1e5 ( Context * __context__, double __x_rename_at_83_11 )
{
    return das_auto_cast<int64_t>::cast(das_cast<int64_t>::cast(__x_rename_at_83_11));
}

inline int2 float_bits_to_int_b6985b657f9ab1a4 ( Context * __context__, float2 __x_rename_at_90_12 )
{
    return das_auto_cast<int2>::cast(das_cast<int2>::cast(__x_rename_at_90_12));
}

inline int3 float_bits_to_int_ce48b037ccb96327 ( Context * __context__, float3 __x_rename_at_96_13 )
{
    return das_auto_cast<int3>::cast(das_cast<int3>::cast(__x_rename_at_96_13));
}

inline int4 float_bits_to_int_8f0076236d1fb70d ( Context * __context__, float4 __x_rename_at_102_14 )
{
    return das_auto_cast<int4>::cast(das_cast<int4>::cast(__x_rename_at_102_14));
}

inline uint32_t float_bits_to_uint_749d2563863b6705 ( Context * __context__, float __x_rename_at_108_15 )
{
    return das_auto_cast<uint32_t>::cast(das_cast<uint32_t>::cast(__x_rename_at_108_15));
}

inline uint64_t double_bits_to_uint64_80e4483b964d603f ( Context * __context__, double __x_rename_at_115_16 )
{
    return das_auto_cast<uint64_t>::cast(das_cast<uint64_t>::cast(__x_rename_at_115_16));
}

inline uint2 float_bits_to_uint_15dba7c285276ce2 ( Context * __context__, float2 __x_rename_at_122_17 )
{
    return das_auto_cast<uint2>::cast(das_cast<uint2>::cast(__x_rename_at_122_17));
}

inline uint3 float_bits_to_uint_372a5f9e37619b1e ( Context * __context__, float3 __x_rename_at_128_18 )
{
    return das_auto_cast<uint3>::cast(das_cast<uint3>::cast(__x_rename_at_128_18));
}

inline uint4 float_bits_to_uint_2ba26bb1c092ef0d ( Context * __context__, float4 __x_rename_at_134_19 )
{
    return das_auto_cast<uint4>::cast(das_cast<uint4>::cast(__x_rename_at_134_19));
}

inline float4 cast_to_vec4f_51ca011a5e87b774 ( Context * __context__, bool __x_rename_at_152_20 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_154_21;das_zero(__v_rename_at_154_21);
    das_copy(das_get_auto_variant_field<bool,7,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_154_21),__x_rename_at_152_20);
    return das_auto_cast<float4>::cast(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_154_21));
}

inline float4 cast_to_vec4f_3eb54330cb0e2cf1 ( Context * __context__, int64_t __x_rename_at_161_22 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_163_23;das_zero(__v_rename_at_163_23);
    das_copy(das_get_auto_variant_field<int64_t,1,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_163_23),__x_rename_at_161_22);
    return das_auto_cast<float4>::cast(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_163_23));
}

inline int64_t cast_to_int64_4ec4ddc73187c475 ( Context * __context__, float4 __data_rename_at_170_24 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_172_25;das_zero(__v_rename_at_172_25);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_172_25),__data_rename_at_170_24);
    return das_auto_cast<int64_t>::cast(das_get_auto_variant_field<int64_t,1,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_172_25));
}

inline int32_t cast_to_int32_633048e550d83e51 ( Context * __context__, float4 __data_rename_at_179_26 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_181_27;das_zero(__v_rename_at_181_27);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_181_27),__data_rename_at_179_26);
    return das_auto_cast<int32_t>::cast(das_get_auto_variant_field<int32_t,2,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_181_27));
}

inline int16_t cast_to_int16_ad1316b2e3e0c4b2 ( Context * __context__, float4 __data_rename_at_188_28 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_190_29;das_zero(__v_rename_at_190_29);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_190_29),__data_rename_at_188_28);
    return das_auto_cast<int16_t>::cast(das_get_auto_variant_field<int16_t,3,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_190_29));
}

inline int8_t cast_to_int8_49a1b1a5bbb7e65 ( Context * __context__, float4 __data_rename_at_197_30 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_199_31;das_zero(__v_rename_at_199_31);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_199_31),__data_rename_at_197_30);
    return das_auto_cast<int8_t>::cast(das_get_auto_variant_field<int8_t,4,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_199_31));
}

inline char * cast_to_string_f9e08771dd99d6d9 ( Context * __context__, float4 __data_rename_at_206_32 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_208_33;das_zero(__v_rename_at_208_33);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_208_33),__data_rename_at_206_32);
    return das_auto_cast<char *>::cast(das_get_auto_variant_field<char *,5,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_208_33));
}

inline void * cast_to_pointer_319c7779d940a37d ( Context * __context__, float4 __data_rename_at_215_34 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_217_35;das_zero(__v_rename_at_217_35);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_217_35),__data_rename_at_215_34);
    return das_auto_cast<void *>::cast(das_get_auto_variant_field<void *,6,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_217_35));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xe84fb2f5d3db2f56] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_5661d7c872a376d7>>();
    };
    aotLib[0x7255af9118686c72] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int64_bits_to_double_548b085f12a59c21>>();
    };
    aotLib[0x261f970e05709326] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_89c9cee88d8c0768>>();
    };
    aotLib[0xc7b83a9f30b8160e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_e2c2dbacebc7057>>();
    };
    aotLib[0x3a533549a79d2056] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_f7016869e1d4c441>>();
    };
    aotLib[0x65f55370d9d6fed7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_186d5760444bbb85>>();
    };
    aotLib[0x1e960e9f86f1c1f3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint64_bits_to_double_2f9a8df65ab2c19b>>();
    };
    aotLib[0x7b7418e3647767a6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_37f5acb6e42a3a72>>();
    };
    aotLib[0xfb2f702f43a49219] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_bd1aead92fb7c892>>();
    };
    aotLib[0x24822e92b121db02] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_621fe31896217edd>>();
    };
    aotLib[0x984204c2bddfab3d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_d77cfe445a750463>>();
    };
    aotLib[0x4f77ec024d61e142] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&double_bits_to_int64_b277151c3c6ab1e5>>();
    };
    aotLib[0xf4cc9eca3cb4d69f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_b6985b657f9ab1a4>>();
    };
    aotLib[0x2afc5a1ec7f26cbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_ce48b037ccb96327>>();
    };
    aotLib[0xed853bfd19b27611] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_8f0076236d1fb70d>>();
    };
    aotLib[0x669c236d6c30464a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_749d2563863b6705>>();
    };
    aotLib[0x1f5b1c884ac980a1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&double_bits_to_uint64_80e4483b964d603f>>();
    };
    aotLib[0xbbbac93065d57829] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_15dba7c285276ce2>>();
    };
    aotLib[0xa9adbd1e713e37d0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_372a5f9e37619b1e>>();
    };
    aotLib[0x489f3e2421605a43] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_2ba26bb1c092ef0d>>();
    };
    aotLib[0x78e158ea2d40be84] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_vec4f_51ca011a5e87b774>>();
    };
    aotLib[0xc2da4983270983b2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_vec4f_3eb54330cb0e2cf1>>();
    };
    aotLib[0xbeafdb8338894e0e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int64_4ec4ddc73187c475>>();
    };
    aotLib[0x2a22edd95755ff21] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int32_633048e550d83e51>>();
    };
    aotLib[0xf0b5abba9f846556] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int16_ad1316b2e3e0c4b2>>();
    };
    aotLib[0x6d442ca2a332354f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int8_49a1b1a5bbb7e65>>();
    };
    aotLib[0xf3f700d25cf0ebc9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_string_f9e08771dd99d6d9>>();
    };
    aotLib[0x2a8422d5f974f1a1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_pointer_319c7779d940a37d>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_4402346706582421074
AotListBase impl_aot_math_bits(_anon_4402346706582421074::registerAotFunctions);
} // namespace das

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#if defined(__EDG__)
#pragma diag_default 826
#elif defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
