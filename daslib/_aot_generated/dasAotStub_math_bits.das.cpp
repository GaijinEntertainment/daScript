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
namespace _anon_2189613692116171520 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline float int_bits_to_float_109b6fe9980793a0 ( Context * __context__, int32_t __x_rename_at_12_0 );
inline double int64_bits_to_double_f341a11369a16d9a ( Context * __context__, int64_t __x_rename_at_19_1 );
inline float2 int_bits_to_float_a3203dc4704b857b ( Context * __context__, int2 __x_rename_at_26_2 );
inline float3 int_bits_to_float_5d5178c04de56920 ( Context * __context__, int3 __x_rename_at_32_3 );
inline float4 int_bits_to_float_154acf41523d16ba ( Context * __context__, int4 __x_rename_at_38_4 );
inline float uint_bits_to_float_cd65e9203dc433e6 ( Context * __context__, uint32_t __x_rename_at_44_5 );
inline double uint64_bits_to_double_a4105deac1fc20dc ( Context * __context__, uint64_t __x_rename_at_51_6 );
inline float2 uint_bits_to_float_53a390d46dbecb45 ( Context * __context__, uint2 __x_rename_at_58_7 );
inline float3 uint_bits_to_float_a8dba1a5d5efc8e5 ( Context * __context__, uint3 __x_rename_at_64_8 );
inline float4 uint_bits_to_float_18078b8f5dd5a3be ( Context * __context__, uint4 __x_rename_at_70_9 );
inline int32_t float_bits_to_int_6c5be20094d18844 ( Context * __context__, float __x_rename_at_76_10 );
inline int64_t double_bits_to_int64_840790635fdbe8c6 ( Context * __context__, double __x_rename_at_83_11 );
inline int2 float_bits_to_int_d691d5a8ec55824f ( Context * __context__, float2 __x_rename_at_90_12 );
inline int3 float_bits_to_int_da69ad4cf3600e70 ( Context * __context__, float3 __x_rename_at_96_13 );
inline int4 float_bits_to_int_71b6a3e812f6612e ( Context * __context__, float4 __x_rename_at_102_14 );
inline uint32_t float_bits_to_uint_7af9cb214b2b0b66 ( Context * __context__, float __x_rename_at_108_15 );
inline uint64_t double_bits_to_uint64_820f4edb0a695f48 ( Context * __context__, double __x_rename_at_115_16 );
inline uint2 float_bits_to_uint_c907f2021e063bb5 ( Context * __context__, float2 __x_rename_at_122_17 );
inline uint3 float_bits_to_uint_d5678121862a7529 ( Context * __context__, float3 __x_rename_at_128_18 );
inline uint4 float_bits_to_uint_c2dd43cd0950592e ( Context * __context__, float4 __x_rename_at_134_19 );
inline float4 cast_to_vec4f_420ab89e818304a9 ( Context * __context__, bool __x_rename_at_152_20 );
inline float4 cast_to_vec4f_e7bf826166977985 ( Context * __context__, int64_t __x_rename_at_161_22 );
inline int64_t cast_to_int64_fb85960df919cc92 ( Context * __context__, float4 __data_rename_at_170_24 );
inline int32_t cast_to_int32_cba141cd2a1f2bdc ( Context * __context__, float4 __data_rename_at_179_26 );
inline int16_t cast_to_int16_7f2b6aa5810856fc ( Context * __context__, float4 __data_rename_at_188_28 );
inline int8_t cast_to_int8_1046b46273a4972d ( Context * __context__, float4 __data_rename_at_197_30 );
inline char * cast_to_string_ad7494d09b0ab5da ( Context * __context__, float4 __data_rename_at_206_32 );
inline void * cast_to_pointer_dd95fc0327c9fd68 ( Context * __context__, float4 __data_rename_at_215_34 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline float int_bits_to_float_109b6fe9980793a0 ( Context * __context__, int32_t __x_rename_at_12_0 )
{
    return das_auto_cast<float>::cast(das_cast<float>::cast(__x_rename_at_12_0));
}

inline double int64_bits_to_double_f341a11369a16d9a ( Context * __context__, int64_t __x_rename_at_19_1 )
{
    return das_auto_cast<double>::cast(das_cast<double>::cast(__x_rename_at_19_1));
}

inline float2 int_bits_to_float_a3203dc4704b857b ( Context * __context__, int2 __x_rename_at_26_2 )
{
    return das_auto_cast<float2>::cast(das_cast<float2>::cast(__x_rename_at_26_2));
}

inline float3 int_bits_to_float_5d5178c04de56920 ( Context * __context__, int3 __x_rename_at_32_3 )
{
    return das_auto_cast<float3>::cast(das_cast<float3>::cast(__x_rename_at_32_3));
}

inline float4 int_bits_to_float_154acf41523d16ba ( Context * __context__, int4 __x_rename_at_38_4 )
{
    return das_auto_cast<float4>::cast(das_cast<float4>::cast(__x_rename_at_38_4));
}

inline float uint_bits_to_float_cd65e9203dc433e6 ( Context * __context__, uint32_t __x_rename_at_44_5 )
{
    return das_auto_cast<float>::cast(das_cast<float>::cast(__x_rename_at_44_5));
}

inline double uint64_bits_to_double_a4105deac1fc20dc ( Context * __context__, uint64_t __x_rename_at_51_6 )
{
    return das_auto_cast<double>::cast(das_cast<double>::cast(__x_rename_at_51_6));
}

inline float2 uint_bits_to_float_53a390d46dbecb45 ( Context * __context__, uint2 __x_rename_at_58_7 )
{
    return das_auto_cast<float2>::cast(das_cast<float2>::cast(__x_rename_at_58_7));
}

inline float3 uint_bits_to_float_a8dba1a5d5efc8e5 ( Context * __context__, uint3 __x_rename_at_64_8 )
{
    return das_auto_cast<float3>::cast(das_cast<float3>::cast(__x_rename_at_64_8));
}

inline float4 uint_bits_to_float_18078b8f5dd5a3be ( Context * __context__, uint4 __x_rename_at_70_9 )
{
    return das_auto_cast<float4>::cast(das_cast<float4>::cast(__x_rename_at_70_9));
}

inline int32_t float_bits_to_int_6c5be20094d18844 ( Context * __context__, float __x_rename_at_76_10 )
{
    return das_auto_cast<int32_t>::cast(das_cast<int32_t>::cast(__x_rename_at_76_10));
}

inline int64_t double_bits_to_int64_840790635fdbe8c6 ( Context * __context__, double __x_rename_at_83_11 )
{
    return das_auto_cast<int64_t>::cast(das_cast<int64_t>::cast(__x_rename_at_83_11));
}

inline int2 float_bits_to_int_d691d5a8ec55824f ( Context * __context__, float2 __x_rename_at_90_12 )
{
    return das_auto_cast<int2>::cast(das_cast<int2>::cast(__x_rename_at_90_12));
}

inline int3 float_bits_to_int_da69ad4cf3600e70 ( Context * __context__, float3 __x_rename_at_96_13 )
{
    return das_auto_cast<int3>::cast(das_cast<int3>::cast(__x_rename_at_96_13));
}

inline int4 float_bits_to_int_71b6a3e812f6612e ( Context * __context__, float4 __x_rename_at_102_14 )
{
    return das_auto_cast<int4>::cast(das_cast<int4>::cast(__x_rename_at_102_14));
}

inline uint32_t float_bits_to_uint_7af9cb214b2b0b66 ( Context * __context__, float __x_rename_at_108_15 )
{
    return das_auto_cast<uint32_t>::cast(das_cast<uint32_t>::cast(__x_rename_at_108_15));
}

inline uint64_t double_bits_to_uint64_820f4edb0a695f48 ( Context * __context__, double __x_rename_at_115_16 )
{
    return das_auto_cast<uint64_t>::cast(das_cast<uint64_t>::cast(__x_rename_at_115_16));
}

inline uint2 float_bits_to_uint_c907f2021e063bb5 ( Context * __context__, float2 __x_rename_at_122_17 )
{
    return das_auto_cast<uint2>::cast(das_cast<uint2>::cast(__x_rename_at_122_17));
}

inline uint3 float_bits_to_uint_d5678121862a7529 ( Context * __context__, float3 __x_rename_at_128_18 )
{
    return das_auto_cast<uint3>::cast(das_cast<uint3>::cast(__x_rename_at_128_18));
}

inline uint4 float_bits_to_uint_c2dd43cd0950592e ( Context * __context__, float4 __x_rename_at_134_19 )
{
    return das_auto_cast<uint4>::cast(das_cast<uint4>::cast(__x_rename_at_134_19));
}

inline float4 cast_to_vec4f_420ab89e818304a9 ( Context * __context__, bool __x_rename_at_152_20 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_154_21; das_zero(__v_rename_at_154_21);
    das_copy(das_get_auto_variant_field<bool,7,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_154_21),__x_rename_at_152_20);
    return das_auto_cast<float4>::cast(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_154_21));
}

inline float4 cast_to_vec4f_e7bf826166977985 ( Context * __context__, int64_t __x_rename_at_161_22 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_163_23; das_zero(__v_rename_at_163_23);
    das_copy(das_get_auto_variant_field<int64_t,1,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_163_23),__x_rename_at_161_22);
    return das_auto_cast<float4>::cast(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_163_23));
}

inline int64_t cast_to_int64_fb85960df919cc92 ( Context * __context__, float4 __data_rename_at_170_24 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_172_25; das_zero(__v_rename_at_172_25);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_172_25),__data_rename_at_170_24);
    return das_auto_cast<int64_t>::cast(das_get_auto_variant_field<int64_t,1,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_172_25));
}

inline int32_t cast_to_int32_cba141cd2a1f2bdc ( Context * __context__, float4 __data_rename_at_179_26 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_181_27; das_zero(__v_rename_at_181_27);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_181_27),__data_rename_at_179_26);
    return das_auto_cast<int32_t>::cast(das_get_auto_variant_field<int32_t,2,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_181_27));
}

inline int16_t cast_to_int16_7f2b6aa5810856fc ( Context * __context__, float4 __data_rename_at_188_28 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_190_29; das_zero(__v_rename_at_190_29);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_190_29),__data_rename_at_188_28);
    return das_auto_cast<int16_t>::cast(das_get_auto_variant_field<int16_t,3,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_190_29));
}

inline int8_t cast_to_int8_1046b46273a4972d ( Context * __context__, float4 __data_rename_at_197_30 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_199_31; das_zero(__v_rename_at_199_31);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_199_31),__data_rename_at_197_30);
    return das_auto_cast<int8_t>::cast(das_get_auto_variant_field<int8_t,4,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_199_31));
}

inline char * cast_to_string_ad7494d09b0ab5da ( Context * __context__, float4 __data_rename_at_206_32 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_208_33; das_zero(__v_rename_at_208_33);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_208_33),__data_rename_at_206_32);
    return das_auto_cast<char *>::cast(das_get_auto_variant_field<char *,5,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_208_33));
}

inline void * cast_to_pointer_dd95fc0327c9fd68 ( Context * __context__, float4 __data_rename_at_215_34 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_217_35; das_zero(__v_rename_at_217_35);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_217_35),__data_rename_at_215_34);
    return das_auto_cast<void *>::cast(das_get_auto_variant_field<void *,6,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_217_35));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x99a2716facbbb3ce] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_109b6fe9980793a0>>();
    };
    aotLib[0x8eaf8deb28cfb0f8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int64_bits_to_double_f341a11369a16d9a>>();
    };
    aotLib[0x3e436375c9498934] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_a3203dc4704b857b>>();
    };
    aotLib[0x689d87b4a4109140] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_5d5178c04de56920>>();
    };
    aotLib[0x1576b0d25a0b38bf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_154acf41523d16ba>>();
    };
    aotLib[0x5410ea40a8076066] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_cd65e9203dc433e6>>();
    };
    aotLib[0xe9a76b16e8e6b6c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint64_bits_to_double_a4105deac1fc20dc>>();
    };
    aotLib[0xb3bf5bb7d19c561a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_53a390d46dbecb45>>();
    };
    aotLib[0x2f07dbd03df0549d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_a8dba1a5d5efc8e5>>();
    };
    aotLib[0x273e9b876c405a9b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_18078b8f5dd5a3be>>();
    };
    aotLib[0x22c9ad58029af397] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_6c5be20094d18844>>();
    };
    aotLib[0x418598135e1362fc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&double_bits_to_int64_840790635fdbe8c6>>();
    };
    aotLib[0x8847c49c3927f7da] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_d691d5a8ec55824f>>();
    };
    aotLib[0x9d8763a6a963f680] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_da69ad4cf3600e70>>();
    };
    aotLib[0x33d88ca5291399c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_71b6a3e812f6612e>>();
    };
    aotLib[0x3527c4319c90510f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_7af9cb214b2b0b66>>();
    };
    aotLib[0x9ec5eeb8dca2edf3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&double_bits_to_uint64_820f4edb0a695f48>>();
    };
    aotLib[0xa39333e23df3cfda] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_c907f2021e063bb5>>();
    };
    aotLib[0x2f784e28fc04e6a7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_d5678121862a7529>>();
    };
    aotLib[0x1af4985a1f1e2b0d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_c2dd43cd0950592e>>();
    };
    aotLib[0xd01c1a4d19e400ff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_vec4f_420ab89e818304a9>>();
    };
    aotLib[0xbb78cd51a72ec939] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_vec4f_e7bf826166977985>>();
    };
    aotLib[0x94638a9ba4a3c6d4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int64_fb85960df919cc92>>();
    };
    aotLib[0x8c9c1a4a4d6fa85f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int32_cba141cd2a1f2bdc>>();
    };
    aotLib[0xc6bd3df4eea0a40b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int16_7f2b6aa5810856fc>>();
    };
    aotLib[0x419ce3da5b19c81d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int8_1046b46273a4972d>>();
    };
    aotLib[0xe1871970f71671bc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_string_ad7494d09b0ab5da>>();
    };
    aotLib[0x408e5e845f01898f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_pointer_dd95fc0327c9fd68>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_2189613692116171520
AotListBase impl_aot_math_bits(_anon_2189613692116171520::registerAotFunctions);
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
