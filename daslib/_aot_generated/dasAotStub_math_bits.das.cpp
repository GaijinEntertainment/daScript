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
namespace _anon_13085340064208493338 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline float int_bits_to_float_883296f6e4b4684a ( Context * __context__, int32_t __x_rename_at_12_0 );
inline double int64_bits_to_double_9c562bd3c094b9f9 ( Context * __context__, int64_t __x_rename_at_19_1 );
inline float2 int_bits_to_float_470ed75b7d56cb9d ( Context * __context__, int2 __x_rename_at_26_2 );
inline float3 int_bits_to_float_12a202c31c63901c ( Context * __context__, int3 __x_rename_at_32_3 );
inline float4 int_bits_to_float_27b6f8f28423e0d ( Context * __context__, int4 __x_rename_at_38_4 );
inline float uint_bits_to_float_e5705907a9ab2444 ( Context * __context__, uint32_t __x_rename_at_44_5 );
inline double uint64_bits_to_double_6c50c67fb2a69fcb ( Context * __context__, uint64_t __x_rename_at_51_6 );
inline float2 uint_bits_to_float_72f584c996843894 ( Context * __context__, uint2 __x_rename_at_58_7 );
inline float3 uint_bits_to_float_9600a50e608c203a ( Context * __context__, uint3 __x_rename_at_64_8 );
inline float4 uint_bits_to_float_6c9d877e5970ae75 ( Context * __context__, uint4 __x_rename_at_70_9 );
inline int32_t float_bits_to_int_d39d1145c6f1c76e ( Context * __context__, float __x_rename_at_76_10 );
inline int64_t double_bits_to_int64_dfd11b0196cbd7ea ( Context * __context__, double __x_rename_at_83_11 );
inline int2 float_bits_to_int_2207e40d0abaa8ca ( Context * __context__, float2 __x_rename_at_90_12 );
inline int3 float_bits_to_int_573a7c36aa190a7f ( Context * __context__, float3 __x_rename_at_96_13 );
inline int4 float_bits_to_int_1b601fc34b678897 ( Context * __context__, float4 __x_rename_at_102_14 );
inline uint32_t float_bits_to_uint_e2ad1b5a0356f0a7 ( Context * __context__, float __x_rename_at_108_15 );
inline uint64_t double_bits_to_uint64_f6fc094185eebca1 ( Context * __context__, double __x_rename_at_115_16 );
inline uint2 float_bits_to_uint_fe5d04362afca591 ( Context * __context__, float2 __x_rename_at_122_17 );
inline uint3 float_bits_to_uint_56040d599637a629 ( Context * __context__, float3 __x_rename_at_128_18 );
inline uint4 float_bits_to_uint_43e3f764909f86fc ( Context * __context__, float4 __x_rename_at_134_19 );
inline float4 cast_to_vec4f_d0f877b1c4a32423 ( Context * __context__, bool __x_rename_at_152_20 );
inline float4 cast_to_vec4f_bb6c18c765c7aeb ( Context * __context__, int64_t __x_rename_at_161_22 );
inline int64_t cast_to_int64_87fcf8a73b5a8dc3 ( Context * __context__, float4 __data_rename_at_170_24 );
inline int32_t cast_to_int32_c7650e39baa9efac ( Context * __context__, float4 __data_rename_at_179_26 );
inline int16_t cast_to_int16_551e68c61d731907 ( Context * __context__, float4 __data_rename_at_188_28 );
inline int8_t cast_to_int8_a667b315415367bf ( Context * __context__, float4 __data_rename_at_197_30 );
inline char * cast_to_string_ce46628cf58efbe6 ( Context * __context__, float4 __data_rename_at_206_32 );
inline void * cast_to_pointer_5622d18454d928a8 ( Context * __context__, float4 __data_rename_at_215_34 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline float int_bits_to_float_883296f6e4b4684a ( Context * __context__, int32_t __x_rename_at_12_0 )
{
    return das_auto_cast<float>::cast(das_cast<float>::cast(__x_rename_at_12_0));
}

inline double int64_bits_to_double_9c562bd3c094b9f9 ( Context * __context__, int64_t __x_rename_at_19_1 )
{
    return das_auto_cast<double>::cast(das_cast<double>::cast(__x_rename_at_19_1));
}

inline float2 int_bits_to_float_470ed75b7d56cb9d ( Context * __context__, int2 __x_rename_at_26_2 )
{
    return das_auto_cast<float2>::cast(das_cast<float2>::cast(__x_rename_at_26_2));
}

inline float3 int_bits_to_float_12a202c31c63901c ( Context * __context__, int3 __x_rename_at_32_3 )
{
    return das_auto_cast<float3>::cast(das_cast<float3>::cast(__x_rename_at_32_3));
}

inline float4 int_bits_to_float_27b6f8f28423e0d ( Context * __context__, int4 __x_rename_at_38_4 )
{
    return das_auto_cast<float4>::cast(das_cast<float4>::cast(__x_rename_at_38_4));
}

inline float uint_bits_to_float_e5705907a9ab2444 ( Context * __context__, uint32_t __x_rename_at_44_5 )
{
    return das_auto_cast<float>::cast(das_cast<float>::cast(__x_rename_at_44_5));
}

inline double uint64_bits_to_double_6c50c67fb2a69fcb ( Context * __context__, uint64_t __x_rename_at_51_6 )
{
    return das_auto_cast<double>::cast(das_cast<double>::cast(__x_rename_at_51_6));
}

inline float2 uint_bits_to_float_72f584c996843894 ( Context * __context__, uint2 __x_rename_at_58_7 )
{
    return das_auto_cast<float2>::cast(das_cast<float2>::cast(__x_rename_at_58_7));
}

inline float3 uint_bits_to_float_9600a50e608c203a ( Context * __context__, uint3 __x_rename_at_64_8 )
{
    return das_auto_cast<float3>::cast(das_cast<float3>::cast(__x_rename_at_64_8));
}

inline float4 uint_bits_to_float_6c9d877e5970ae75 ( Context * __context__, uint4 __x_rename_at_70_9 )
{
    return das_auto_cast<float4>::cast(das_cast<float4>::cast(__x_rename_at_70_9));
}

inline int32_t float_bits_to_int_d39d1145c6f1c76e ( Context * __context__, float __x_rename_at_76_10 )
{
    return das_auto_cast<int32_t>::cast(das_cast<int32_t>::cast(__x_rename_at_76_10));
}

inline int64_t double_bits_to_int64_dfd11b0196cbd7ea ( Context * __context__, double __x_rename_at_83_11 )
{
    return das_auto_cast<int64_t>::cast(das_cast<int64_t>::cast(__x_rename_at_83_11));
}

inline int2 float_bits_to_int_2207e40d0abaa8ca ( Context * __context__, float2 __x_rename_at_90_12 )
{
    return das_auto_cast<int2>::cast(das_cast<int2>::cast(__x_rename_at_90_12));
}

inline int3 float_bits_to_int_573a7c36aa190a7f ( Context * __context__, float3 __x_rename_at_96_13 )
{
    return das_auto_cast<int3>::cast(das_cast<int3>::cast(__x_rename_at_96_13));
}

inline int4 float_bits_to_int_1b601fc34b678897 ( Context * __context__, float4 __x_rename_at_102_14 )
{
    return das_auto_cast<int4>::cast(das_cast<int4>::cast(__x_rename_at_102_14));
}

inline uint32_t float_bits_to_uint_e2ad1b5a0356f0a7 ( Context * __context__, float __x_rename_at_108_15 )
{
    return das_auto_cast<uint32_t>::cast(das_cast<uint32_t>::cast(__x_rename_at_108_15));
}

inline uint64_t double_bits_to_uint64_f6fc094185eebca1 ( Context * __context__, double __x_rename_at_115_16 )
{
    return das_auto_cast<uint64_t>::cast(das_cast<uint64_t>::cast(__x_rename_at_115_16));
}

inline uint2 float_bits_to_uint_fe5d04362afca591 ( Context * __context__, float2 __x_rename_at_122_17 )
{
    return das_auto_cast<uint2>::cast(das_cast<uint2>::cast(__x_rename_at_122_17));
}

inline uint3 float_bits_to_uint_56040d599637a629 ( Context * __context__, float3 __x_rename_at_128_18 )
{
    return das_auto_cast<uint3>::cast(das_cast<uint3>::cast(__x_rename_at_128_18));
}

inline uint4 float_bits_to_uint_43e3f764909f86fc ( Context * __context__, float4 __x_rename_at_134_19 )
{
    return das_auto_cast<uint4>::cast(das_cast<uint4>::cast(__x_rename_at_134_19));
}

inline float4 cast_to_vec4f_d0f877b1c4a32423 ( Context * __context__, bool __x_rename_at_152_20 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_154_21;das_zero(__v_rename_at_154_21);
    das_copy(das_get_auto_variant_field<bool,7,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_154_21),__x_rename_at_152_20);
    return das_auto_cast<float4>::cast(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_154_21));
}

inline float4 cast_to_vec4f_bb6c18c765c7aeb ( Context * __context__, int64_t __x_rename_at_161_22 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_163_23;das_zero(__v_rename_at_163_23);
    das_copy(das_get_auto_variant_field<int64_t,1,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_163_23),__x_rename_at_161_22);
    return das_auto_cast<float4>::cast(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_163_23));
}

inline int64_t cast_to_int64_87fcf8a73b5a8dc3 ( Context * __context__, float4 __data_rename_at_170_24 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_172_25;das_zero(__v_rename_at_172_25);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_172_25),__data_rename_at_170_24);
    return das_auto_cast<int64_t>::cast(das_get_auto_variant_field<int64_t,1,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_172_25));
}

inline int32_t cast_to_int32_c7650e39baa9efac ( Context * __context__, float4 __data_rename_at_179_26 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_181_27;das_zero(__v_rename_at_181_27);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_181_27),__data_rename_at_179_26);
    return das_auto_cast<int32_t>::cast(das_get_auto_variant_field<int32_t,2,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_181_27));
}

inline int16_t cast_to_int16_551e68c61d731907 ( Context * __context__, float4 __data_rename_at_188_28 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_190_29;das_zero(__v_rename_at_190_29);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_190_29),__data_rename_at_188_28);
    return das_auto_cast<int16_t>::cast(das_get_auto_variant_field<int16_t,3,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_190_29));
}

inline int8_t cast_to_int8_a667b315415367bf ( Context * __context__, float4 __data_rename_at_197_30 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_199_31;das_zero(__v_rename_at_199_31);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_199_31),__data_rename_at_197_30);
    return das_auto_cast<int8_t>::cast(das_get_auto_variant_field<int8_t,4,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_199_31));
}

inline char * cast_to_string_ce46628cf58efbe6 ( Context * __context__, float4 __data_rename_at_206_32 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_208_33;das_zero(__v_rename_at_208_33);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_208_33),__data_rename_at_206_32);
    return das_auto_cast<char *>::cast(das_get_auto_variant_field<char *,5,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_208_33));
}

inline void * cast_to_pointer_5622d18454d928a8 ( Context * __context__, float4 __data_rename_at_215_34 )
{
    AutoVariant<float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool> __v_rename_at_217_35;das_zero(__v_rename_at_217_35);
    das_copy(das_get_auto_variant_field<float4,0,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_217_35),__data_rename_at_215_34);
    return das_auto_cast<void *>::cast(das_get_auto_variant_field<void *,6,float4,int64_t,int32_t,int16_t,int8_t,char *,void *,bool>::get(__v_rename_at_217_35));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x3eb9009aa0e8a10f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_883296f6e4b4684a>>();
    };
    aotLib[0x3382adc78a3bc763] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int64_bits_to_double_9c562bd3c094b9f9>>();
    };
    aotLib[0xc1ead16f192a79eb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_470ed75b7d56cb9d>>();
    };
    aotLib[0xf76ecdcb56aa8521] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_12a202c31c63901c>>();
    };
    aotLib[0xafe6e0e1245496b8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&int_bits_to_float_27b6f8f28423e0d>>();
    };
    aotLib[0xa7c211ddafeaf1d1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_e5705907a9ab2444>>();
    };
    aotLib[0x844ff8c7ff5c1232] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint64_bits_to_double_6c50c67fb2a69fcb>>();
    };
    aotLib[0xac6784e8c66ee3f2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_72f584c996843894>>();
    };
    aotLib[0x23f80dc894297998] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_9600a50e608c203a>>();
    };
    aotLib[0xb9bbec10a0bb7259] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uint_bits_to_float_6c9d877e5970ae75>>();
    };
    aotLib[0x93cb1de8fb449c48] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_d39d1145c6f1c76e>>();
    };
    aotLib[0x5a150318e976e97f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&double_bits_to_int64_dfd11b0196cbd7ea>>();
    };
    aotLib[0x5ce807b8eee95517] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_2207e40d0abaa8ca>>();
    };
    aotLib[0x223e54aebde1bd55] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_573a7c36aa190a7f>>();
    };
    aotLib[0x979523f860f5c1bf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_int_1b601fc34b678897>>();
    };
    aotLib[0xe408498a3add609d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_e2ad1b5a0356f0a7>>();
    };
    aotLib[0xf74f3a73cb2d6763] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&double_bits_to_uint64_f6fc094185eebca1>>();
    };
    aotLib[0x696eb96a59e38de9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_fe5d04362afca591>>();
    };
    aotLib[0x9e207e4a238be54a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_56040d599637a629>>();
    };
    aotLib[0x43bad98fcad6f144] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&float_bits_to_uint_43e3f764909f86fc>>();
    };
    aotLib[0xa1024e0d14a12865] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_vec4f_d0f877b1c4a32423>>();
    };
    aotLib[0x4b0ea37df97572e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_vec4f_bb6c18c765c7aeb>>();
    };
    aotLib[0x30869d7e68c5e84c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int64_87fcf8a73b5a8dc3>>();
    };
    aotLib[0xa765b71f9ff338c7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int32_c7650e39baa9efac>>();
    };
    aotLib[0xf1bffe7e20e64e1f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int16_551e68c61d731907>>();
    };
    aotLib[0x12a63ca9349681fa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_int8_a667b315415367bf>>();
    };
    aotLib[0xbdc01a23f23cbd66] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_string_ce46628cf58efbe6>>();
    };
    aotLib[0x3deb5495b26fef0d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&cast_to_pointer_5622d18454d928a8>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_13085340064208493338
AotListBase impl_aot_math_bits(_anon_13085340064208493338::registerAotFunctions);
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
