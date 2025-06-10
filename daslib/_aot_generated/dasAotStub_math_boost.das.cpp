#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require math_bits
 // require math_boost

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
namespace _anon_5222113748529321679 {

namespace math_boost { struct AABR; };
namespace math_boost { struct AABB; };
namespace math_boost { struct Ray; };
namespace math_boost {

struct AABR {
    float2 min;
    float2 max;
};
}
namespace math_boost {

struct AABB {
    float3 min;
    float3 max;
};
}
namespace math_boost {

struct Ray {
    float3 dir;
    float3 origin;
};
}


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline float degrees_5f249a2927244f88 ( Context * __context__, float __f_rename_at_31_0 );
inline float radians_57b357af30d76dd3 ( Context * __context__, float __f_rename_at_36_1 );
inline uint32_t RGBA_TO_UCOLOR_7dc715ea708e1dcf ( Context * __context__, float __x_rename_at_41_2, float __y_rename_at_41_3, float __z_rename_at_41_4, float __w_rename_at_41_5 );
inline uint32_t RGBA_TO_UCOLOR_fb8dcce4cf4bf337 ( Context * __context__, float4 __xyzw_rename_at_46_6 );
inline float4 UCOLOR_TO_RGBA_122d272645420117 ( Context * __context__, uint32_t __x_rename_at_50_7 );
inline float3 UCOLOR_TO_RGB_150bcaeff21136f5 ( Context * __context__, uint32_t __x_rename_at_59_8 );
inline float linear_to_SRGB_e60b033cd2c66c96 ( Context * __context__, float __x_rename_at_67_9 );
inline float3 linear_to_SRGB_8dce3429276ccd5c ( Context * __context__, float3 __c_rename_at_76_10 );
inline float4 linear_to_SRGB_ee1acb339d8852b4 ( Context * __context__, float4 __c_rename_at_80_11 );
inline bool is_intersecting_eadba3e0f58085ff ( Context * __context__, math_boost::AABR const  & __a_rename_at_84_12, math_boost::AABR const  & __b_rename_at_84_13 );
inline bool is_intersecting_d7fd0cb5fd9dfea1 ( Context * __context__, math_boost::AABB const  & __a_rename_at_90_14, math_boost::AABB const  & __b_rename_at_90_15 );
inline bool is_intersecting_b3552c1e8df8c1 ( Context * __context__, math_boost::Ray const  & __ray_rename_at_95_16, math_boost::AABB const  & __aabb_rename_at_95_17, float __Tmin_rename_at_95_18, float __Tmax_rename_at_95_19 );
inline float4x4 look_at_lh_d1c0097885e05205 ( Context * __context__, float3 __Eye_rename_at_106_27, float3 __At_rename_at_106_28, float3 __Up_rename_at_106_29 );
inline float4x4 look_at_rh_c44f641993eea3d2 ( Context * __context__, float3 __Eye_rename_at_121_34, float3 __At_rename_at_121_35, float3 __Up_rename_at_121_36 );
inline float4x4 perspective_lh_d4750cbeea4193ff ( Context * __context__, float __fovy_rename_at_136_41, float __aspect_rename_at_136_42, float __zn_rename_at_136_43, float __zf_rename_at_136_44 );
inline float4x4 perspective_rh_7371923d3c9e2e94 ( Context * __context__, float __fovy_rename_at_150_48, float __aspect_rename_at_150_49, float __zn_rename_at_150_50, float __zf_rename_at_150_51 );
inline float4x4 perspective_rh_opengl_aeaeb5be074e7848 ( Context * __context__, float __fovy_rename_at_164_55, float __aspect_rename_at_164_56, float __zn_rename_at_164_57, float __zf_rename_at_164_58 );
inline float4x4 ortho_rh_b77b1078f7aaafbf ( Context * __context__, float __left_rename_at_178_62, float __right_rename_at_178_63, float __bottom_rename_at_178_64, float __top_rename_at_178_65, float __zNear_rename_at_178_66, float __zFar_rename_at_178_67 );
inline float plane_dot_cd83897bbf4b3561 ( Context * __context__, float4 __Plane_rename_at_191_69, float4 __Vec_rename_at_191_70 );
inline float4 plane_normalize_fead8a8a65eea3f0 ( Context * __context__, float4 __Plane_rename_at_196_71 );
inline float4 plane_from_point_normal_b248ffb7d8d67d08 ( Context * __context__, float3 __p_rename_at_202_73, float3 __n_rename_at_202_74 );
inline float4x4 planar_shadow_d6827d119541b633 ( Context * __context__, float4 __Light_rename_at_207_75, float4 __Plane_rename_at_207_76 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline float degrees_5f249a2927244f88 ( Context * __context__, float __f_rename_at_31_0 )
{
    return das_auto_cast<float>::cast(SimPolicy<float>::Div((__f_rename_at_31_0 * 1.800000e+02f),3.141593e+00f,*__context__,nullptr));
}

inline float radians_57b357af30d76dd3 ( Context * __context__, float __f_rename_at_36_1 )
{
    return das_auto_cast<float>::cast(SimPolicy<float>::Div((__f_rename_at_36_1 * 3.141593e+00f),1.800000e+02f,*__context__,nullptr));
}

inline uint32_t RGBA_TO_UCOLOR_7dc715ea708e1dcf ( Context * __context__, float __x_rename_at_41_2, float __y_rename_at_41_3, float __z_rename_at_41_4, float __w_rename_at_41_5 )
{
    return das_auto_cast<uint32_t>::cast(pack_float_to_byte(SimPolicy<float4>::MulVecScal(float4(__x_rename_at_41_2,__y_rename_at_41_3,__z_rename_at_41_4,__w_rename_at_41_5),cast<float>::from(2.550000e+02f),*__context__,nullptr)));
}

inline uint32_t RGBA_TO_UCOLOR_fb8dcce4cf4bf337 ( Context * __context__, float4 __xyzw_rename_at_46_6 )
{
    return das_auto_cast<uint32_t>::cast(pack_float_to_byte(SimPolicy<float4>::MulVecScal(__xyzw_rename_at_46_6,cast<float>::from(2.550000e+02f),*__context__,nullptr)));
}

inline float4 UCOLOR_TO_RGBA_122d272645420117 ( Context * __context__, uint32_t __x_rename_at_50_7 )
{
    return das_auto_cast<float4>::cast(float4(SimPolicy<float>::Div(float((__x_rename_at_50_7 >> 0x10u) & 0xffu),2.550000e+02f,*__context__,nullptr),SimPolicy<float>::Div(float((__x_rename_at_50_7 >> 0x8u) & 0xffu),2.550000e+02f,*__context__,nullptr),SimPolicy<float>::Div(float((__x_rename_at_50_7 >> 0x0u) & 0xffu),2.550000e+02f,*__context__,nullptr),SimPolicy<float>::Div(float((__x_rename_at_50_7 >> 0x18u) & 0xffu),2.550000e+02f,*__context__,nullptr)));
}

inline float3 UCOLOR_TO_RGB_150bcaeff21136f5 ( Context * __context__, uint32_t __x_rename_at_59_8 )
{
    return das_auto_cast<float3>::cast(float3(SimPolicy<float>::Div(float((__x_rename_at_59_8 >> 0x10u) & 0xffu),2.550000e+02f,*__context__,nullptr),SimPolicy<float>::Div(float((__x_rename_at_59_8 >> 0x8u) & 0xffu),2.550000e+02f,*__context__,nullptr),SimPolicy<float>::Div(float((__x_rename_at_59_8 >> 0x0u) & 0xffu),2.550000e+02f,*__context__,nullptr)));
}

inline float linear_to_SRGB_e60b033cd2c66c96 ( Context * __context__, float __x_rename_at_67_9 )
{
    return das_auto_cast<float>::cast((__x_rename_at_67_9 <= 3.130800e-04f) ? das_auto_cast<float>::cast((__x_rename_at_67_9 * 1.292000e+01f)) : das_auto_cast<float>::cast(((SimPolicy<float>::Pow(__x_rename_at_67_9,4.166667e-01f,*__context__,nullptr) * 1.055000e+00f) - 5.500000e-02f)));
}

inline float3 linear_to_SRGB_8dce3429276ccd5c ( Context * __context__, float3 __c_rename_at_76_10 )
{
    return das_auto_cast<float3>::cast(float3(linear_to_SRGB_e60b033cd2c66c96(__context__,v_extract_x(__c_rename_at_76_10) /*x*/),linear_to_SRGB_e60b033cd2c66c96(__context__,v_extract_y(__c_rename_at_76_10) /*y*/),linear_to_SRGB_e60b033cd2c66c96(__context__,v_extract_z(__c_rename_at_76_10) /*z*/)));
}

inline float4 linear_to_SRGB_ee1acb339d8852b4 ( Context * __context__, float4 __c_rename_at_80_11 )
{
    return das_auto_cast<float4>::cast(float4(linear_to_SRGB_e60b033cd2c66c96(__context__,v_extract_x(__c_rename_at_80_11) /*x*/),linear_to_SRGB_e60b033cd2c66c96(__context__,v_extract_y(__c_rename_at_80_11) /*y*/),linear_to_SRGB_e60b033cd2c66c96(__context__,v_extract_z(__c_rename_at_80_11) /*z*/),v_extract_w(__c_rename_at_80_11) /*w*/));
}

inline bool is_intersecting_eadba3e0f58085ff ( Context * __context__, math_boost::AABR const  &  __a_rename_at_84_12, math_boost::AABR const  &  __b_rename_at_84_13 )
{
    return das_auto_cast<bool>::cast((((v_extract_x(__a_rename_at_84_12.min) /*x*/ <= v_extract_x(__b_rename_at_84_13.max) /*x*/) && (v_extract_y(__a_rename_at_84_12.min) /*y*/ <= v_extract_y(__b_rename_at_84_13.max) /*y*/)) && (v_extract_x(__a_rename_at_84_12.max) /*x*/ >= v_extract_x(__b_rename_at_84_13.min) /*x*/)) && (v_extract_y(__a_rename_at_84_12.max) /*y*/ >= v_extract_y(__b_rename_at_84_13.min) /*y*/));
}

inline bool is_intersecting_d7fd0cb5fd9dfea1 ( Context * __context__, math_boost::AABB const  &  __a_rename_at_90_14, math_boost::AABB const  &  __b_rename_at_90_15 )
{
    return das_auto_cast<bool>::cast((((((v_extract_x(__a_rename_at_90_14.min) /*x*/ <= v_extract_x(__b_rename_at_90_15.max) /*x*/) && (v_extract_y(__a_rename_at_90_14.min) /*y*/ <= v_extract_y(__b_rename_at_90_15.max) /*y*/)) && (v_extract_z(__a_rename_at_90_14.min) /*z*/ <= v_extract_z(__b_rename_at_90_15.max) /*z*/)) && (v_extract_x(__a_rename_at_90_14.max) /*x*/ >= v_extract_x(__b_rename_at_90_15.min) /*x*/)) && (v_extract_y(__a_rename_at_90_14.max) /*y*/ >= v_extract_y(__b_rename_at_90_15.min) /*y*/)) && (v_extract_z(__a_rename_at_90_14.max) /*z*/ >= v_extract_z(__b_rename_at_90_15.min) /*z*/));
}

inline bool is_intersecting_b3552c1e8df8c1 ( Context * __context__, math_boost::Ray const  &  __ray_rename_at_95_16, math_boost::AABB const  &  __aabb_rename_at_95_17, float __Tmin_rename_at_95_18, float __Tmax_rename_at_95_19 )
{
    float3 __invD_rename_at_96_20 = ((float3)SimPolicy<float3>::Rcp(__ray_rename_at_95_16.dir,*__context__,nullptr));
    float3 __t0s_rename_at_97_21 = ((float3)(SimPolicy<float3>::Mul((SimPolicy<float3>::Sub(__aabb_rename_at_95_17.min,__ray_rename_at_95_16.origin,*__context__,nullptr)),__invD_rename_at_96_20,*__context__,nullptr)));
    float3 __t1s_rename_at_98_22 = ((float3)(SimPolicy<float3>::Mul((SimPolicy<float3>::Sub(__aabb_rename_at_95_17.max,__ray_rename_at_95_16.origin,*__context__,nullptr)),__invD_rename_at_96_20,*__context__,nullptr)));
    float3 __tsmaller_rename_at_99_23 = ((float3)SimPolicy<float3>::Min(__t0s_rename_at_97_21,__t1s_rename_at_98_22,*__context__,nullptr));
    float3 __tbigger_rename_at_100_24 = ((float3)SimPolicy<float3>::Max(__t0s_rename_at_97_21,__t1s_rename_at_98_22,*__context__,nullptr));
    float __tmin_rename_at_101_25 = ((float)SimPolicy<float>::Max(__Tmin_rename_at_95_18,SimPolicy<float>::Max(das_index<float3 const >::at(__tsmaller_rename_at_99_23,0,__context__),SimPolicy<float>::Max(das_index<float3 const >::at(__tsmaller_rename_at_99_23,1,__context__),das_index<float3 const >::at(__tsmaller_rename_at_99_23,2,__context__),*__context__,nullptr),*__context__,nullptr),*__context__,nullptr));
    float __tmax_rename_at_102_26 = ((float)SimPolicy<float>::Min(__Tmax_rename_at_95_19,SimPolicy<float>::Min(das_index<float3 const >::at(__tbigger_rename_at_100_24,0,__context__),SimPolicy<float>::Min(das_index<float3 const >::at(__tbigger_rename_at_100_24,1,__context__),das_index<float3 const >::at(__tbigger_rename_at_100_24,2,__context__),*__context__,nullptr),*__context__,nullptr),*__context__,nullptr));
    return das_auto_cast<bool>::cast(__tmin_rename_at_101_25 < __tmax_rename_at_102_26);
}

inline float4x4 look_at_lh_d1c0097885e05205 ( Context * __context__, float3 __Eye_rename_at_106_27, float3 __At_rename_at_106_28, float3 __Up_rename_at_106_29 )
{
    float3 __zaxis_rename_at_108_30 = ((float3)safe_normalize3(SimPolicy<float3>::Sub(__At_rename_at_106_28,__Eye_rename_at_106_27,*__context__,nullptr)));
    float3 __xaxis_rename_at_109_31 = ((float3)safe_normalize3(cross3(__Up_rename_at_106_29,__zaxis_rename_at_108_30)));
    float3 __yaxis_rename_at_110_32 = ((float3)cross3(__zaxis_rename_at_108_30,__xaxis_rename_at_109_31));
    float4x4 __tm_rename_at_111_33;das_zero(__tm_rename_at_111_33);
    das_copy(das_index<float4x4>::at(__tm_rename_at_111_33,0,__context__),float4(v_extract_x(__xaxis_rename_at_109_31) /*x*/,v_extract_x(__yaxis_rename_at_110_32) /*x*/,v_extract_x(__zaxis_rename_at_108_30) /*x*/,0.000000e+00f));
    das_copy(das_index<float4x4>::at(__tm_rename_at_111_33,1,__context__),float4(v_extract_y(__xaxis_rename_at_109_31) /*y*/,v_extract_y(__yaxis_rename_at_110_32) /*y*/,v_extract_y(__zaxis_rename_at_108_30) /*y*/,0.000000e+00f));
    das_copy(das_index<float4x4>::at(__tm_rename_at_111_33,2,__context__),float4(v_extract_z(__xaxis_rename_at_109_31) /*z*/,v_extract_z(__yaxis_rename_at_110_32) /*z*/,v_extract_z(__zaxis_rename_at_108_30) /*z*/,0.000000e+00f));
    das_copy(das_index<float4x4>::at(__tm_rename_at_111_33,3,__context__),float4(-dot3(__xaxis_rename_at_109_31,__Eye_rename_at_106_27),-dot3(__yaxis_rename_at_110_32,__Eye_rename_at_106_27),-dot3(__zaxis_rename_at_108_30,__Eye_rename_at_106_27),1.000000e+00f));
    return das_auto_cast_ref<float4x4>::cast(__tm_rename_at_111_33);
}

inline float4x4 look_at_rh_c44f641993eea3d2 ( Context * __context__, float3 __Eye_rename_at_121_34, float3 __At_rename_at_121_35, float3 __Up_rename_at_121_36 )
{
    float3 __zaxis_rename_at_123_37 = ((float3)safe_normalize3(SimPolicy<float3>::Sub(__Eye_rename_at_121_34,__At_rename_at_121_35,*__context__,nullptr)));
    float3 __xaxis_rename_at_124_38 = ((float3)safe_normalize3(cross3(__Up_rename_at_121_36,__zaxis_rename_at_123_37)));
    float3 __yaxis_rename_at_125_39 = ((float3)cross3(__zaxis_rename_at_123_37,__xaxis_rename_at_124_38));
    float4x4 __tm_rename_at_126_40;das_zero(__tm_rename_at_126_40);
    das_copy(das_index<float4x4>::at(__tm_rename_at_126_40,0,__context__),float4(v_extract_x(__xaxis_rename_at_124_38) /*x*/,v_extract_x(__yaxis_rename_at_125_39) /*x*/,v_extract_x(__zaxis_rename_at_123_37) /*x*/,0.000000e+00f));
    das_copy(das_index<float4x4>::at(__tm_rename_at_126_40,1,__context__),float4(v_extract_y(__xaxis_rename_at_124_38) /*y*/,v_extract_y(__yaxis_rename_at_125_39) /*y*/,v_extract_y(__zaxis_rename_at_123_37) /*y*/,0.000000e+00f));
    das_copy(das_index<float4x4>::at(__tm_rename_at_126_40,2,__context__),float4(v_extract_z(__xaxis_rename_at_124_38) /*z*/,v_extract_z(__yaxis_rename_at_125_39) /*z*/,v_extract_z(__zaxis_rename_at_123_37) /*z*/,0.000000e+00f));
    das_copy(das_index<float4x4>::at(__tm_rename_at_126_40,3,__context__),float4(-dot3(__xaxis_rename_at_124_38,__Eye_rename_at_121_34),-dot3(__yaxis_rename_at_125_39,__Eye_rename_at_121_34),-dot3(__zaxis_rename_at_123_37,__Eye_rename_at_121_34),1.000000e+00f));
    return das_auto_cast_ref<float4x4>::cast(__tm_rename_at_126_40);
}

inline float4x4 perspective_lh_d4750cbeea4193ff ( Context * __context__, float __fovy_rename_at_136_41, float __aspect_rename_at_136_42, float __zn_rename_at_136_43, float __zf_rename_at_136_44 )
{
    float __yScale_rename_at_138_45 = ((float)(SimPolicy<float>::Div(1.000000e+00f,SimPolicy<float>::Tan(SimPolicy<float>::Div(__fovy_rename_at_136_41,2.000000e+00f,*__context__,nullptr),*__context__,nullptr),*__context__,nullptr)));
    float __xScale_rename_at_139_46 = ((float)(SimPolicy<float>::Div(__yScale_rename_at_138_45,__aspect_rename_at_136_42,*__context__,nullptr)));
    float4x4 __Result_rename_at_140_47;das_zero(__Result_rename_at_140_47);
    float4x4_identity(das_arg<float4x4>::pass(__Result_rename_at_140_47));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_140_47,0,__context__),0,__context__),__xScale_rename_at_139_46);
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_140_47,1,__context__),1,__context__),__yScale_rename_at_138_45);
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_140_47,2,__context__),2,__context__),SimPolicy<float>::Div(__zf_rename_at_136_44,(__zf_rename_at_136_44 - __zn_rename_at_136_43),*__context__,nullptr));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_140_47,2,__context__),3,__context__),-1.000000e+00f);
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_140_47,3,__context__),2,__context__),SimPolicy<float>::Div((-__zn_rename_at_136_43 * __zf_rename_at_136_44),(__zn_rename_at_136_43 - __zf_rename_at_136_44),*__context__,nullptr));
    return das_auto_cast_ref<float4x4>::cast(__Result_rename_at_140_47);
}

inline float4x4 perspective_rh_7371923d3c9e2e94 ( Context * __context__, float __fovy_rename_at_150_48, float __aspect_rename_at_150_49, float __zn_rename_at_150_50, float __zf_rename_at_150_51 )
{
    float __yScale_rename_at_152_52 = ((float)(SimPolicy<float>::Div(1.000000e+00f,SimPolicy<float>::Tan(SimPolicy<float>::Div(__fovy_rename_at_150_48,2.000000e+00f,*__context__,nullptr),*__context__,nullptr),*__context__,nullptr)));
    float __xScale_rename_at_153_53 = ((float)(SimPolicy<float>::Div(__yScale_rename_at_152_52,__aspect_rename_at_150_49,*__context__,nullptr)));
    float4x4 __Result_rename_at_154_54;das_zero(__Result_rename_at_154_54);
    float4x4_identity(das_arg<float4x4>::pass(__Result_rename_at_154_54));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_154_54,0,__context__),0,__context__),__xScale_rename_at_153_53);
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_154_54,1,__context__),1,__context__),__yScale_rename_at_152_52);
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_154_54,2,__context__),2,__context__),SimPolicy<float>::Div(__zf_rename_at_150_51,(__zn_rename_at_150_50 - __zf_rename_at_150_51),*__context__,nullptr));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_154_54,2,__context__),3,__context__),-1.000000e+00f);
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_154_54,3,__context__),2,__context__),SimPolicy<float>::Div((__zn_rename_at_150_50 * __zf_rename_at_150_51),(__zn_rename_at_150_50 - __zf_rename_at_150_51),*__context__,nullptr));
    return das_auto_cast_ref<float4x4>::cast(__Result_rename_at_154_54);
}

inline float4x4 perspective_rh_opengl_aeaeb5be074e7848 ( Context * __context__, float __fovy_rename_at_164_55, float __aspect_rename_at_164_56, float __zn_rename_at_164_57, float __zf_rename_at_164_58 )
{
    float __yScale_rename_at_166_59 = ((float)(SimPolicy<float>::Div(1.000000e+00f,SimPolicy<float>::Tan(SimPolicy<float>::Div(__fovy_rename_at_164_55,2.000000e+00f,*__context__,nullptr),*__context__,nullptr),*__context__,nullptr)));
    float __xScale_rename_at_167_60 = ((float)(SimPolicy<float>::Div(__yScale_rename_at_166_59,__aspect_rename_at_164_56,*__context__,nullptr)));
    float4x4 __Result_rename_at_168_61;das_zero(__Result_rename_at_168_61);
    float4x4_identity(das_arg<float4x4>::pass(__Result_rename_at_168_61));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_168_61,0,__context__),0,__context__),__xScale_rename_at_167_60);
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_168_61,1,__context__),1,__context__),__yScale_rename_at_166_59);
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_168_61,2,__context__),2,__context__),SimPolicy<float>::Div((__zf_rename_at_164_58 + __zn_rename_at_164_57),(__zn_rename_at_164_57 - __zf_rename_at_164_58),*__context__,nullptr));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_168_61,2,__context__),3,__context__),-1.000000e+00f);
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_168_61,3,__context__),2,__context__),SimPolicy<float>::Div(((__zn_rename_at_164_57 * 2.000000e+00f) * __zf_rename_at_164_58),(__zn_rename_at_164_57 - __zf_rename_at_164_58),*__context__,nullptr));
    return das_auto_cast_ref<float4x4>::cast(__Result_rename_at_168_61);
}

inline float4x4 ortho_rh_b77b1078f7aaafbf ( Context * __context__, float __left_rename_at_178_62, float __right_rename_at_178_63, float __bottom_rename_at_178_64, float __top_rename_at_178_65, float __zNear_rename_at_178_66, float __zFar_rename_at_178_67 )
{
    float4x4 __Result_rename_at_180_68;das_zero(__Result_rename_at_180_68);
    float4x4_identity(das_arg<float4x4>::pass(__Result_rename_at_180_68));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_180_68,0,__context__),0,__context__),SimPolicy<float>::Div(2.000000e+00f,(__right_rename_at_178_63 - __left_rename_at_178_62),*__context__,nullptr));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_180_68,1,__context__),1,__context__),SimPolicy<float>::Div(2.000000e+00f,(__top_rename_at_178_65 - __bottom_rename_at_178_64),*__context__,nullptr));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_180_68,2,__context__),2,__context__),SimPolicy<float>::Div(-2.000000e+00f,(__zFar_rename_at_178_67 - __zNear_rename_at_178_66),*__context__,nullptr));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_180_68,3,__context__),0,__context__),SimPolicy<float>::Div(-((__right_rename_at_178_63 + __left_rename_at_178_62)),(__right_rename_at_178_63 - __left_rename_at_178_62),*__context__,nullptr));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_180_68,3,__context__),1,__context__),SimPolicy<float>::Div(-((__top_rename_at_178_65 + __bottom_rename_at_178_64)),(__top_rename_at_178_65 - __bottom_rename_at_178_64),*__context__,nullptr));
    das_copy(das_index<float4>::at(das_index<float4x4>::at(__Result_rename_at_180_68,3,__context__),2,__context__),SimPolicy<float>::Div(-((__zFar_rename_at_178_67 + __zNear_rename_at_178_66)),(__zFar_rename_at_178_67 - __zNear_rename_at_178_66),*__context__,nullptr));
    return das_auto_cast_ref<float4x4>::cast(__Result_rename_at_180_68);
}

inline float plane_dot_cd83897bbf4b3561 ( Context * __context__, float4 __Plane_rename_at_191_69, float4 __Vec_rename_at_191_70 )
{
    return das_auto_cast<float>::cast(dot4(__Plane_rename_at_191_69,__Vec_rename_at_191_70));
}

inline float4 plane_normalize_fead8a8a65eea3f0 ( Context * __context__, float4 __Plane_rename_at_196_71 )
{
    float __len_rename_at_198_72 = ((float)length3(das_swizzle_seq<float3,float4 const ,0>::swizzle(__Plane_rename_at_196_71) /*xyz*/));
    return das_auto_cast<float4>::cast((__len_rename_at_198_72 != 0.000000e+00f) ? (vec4f)das_auto_cast<float4>::cast((SimPolicy<float4>::DivVecScal(__Plane_rename_at_196_71,cast<float>::from(__len_rename_at_198_72),*__context__,nullptr))) : (vec4f)das_auto_cast<float4>::cast(v_zero()));
}

inline float4 plane_from_point_normal_b248ffb7d8d67d08 ( Context * __context__, float3 __p_rename_at_202_73, float3 __n_rename_at_202_74 )
{
    return das_auto_cast<float4>::cast(float4(v_extract_x(__n_rename_at_202_74) /*x*/,v_extract_y(__n_rename_at_202_74) /*y*/,v_extract_z(__n_rename_at_202_74) /*z*/,-dot3(__p_rename_at_202_73,__n_rename_at_202_74)));
}

inline float4x4 planar_shadow_d6827d119541b633 ( Context * __context__, float4 __Light_rename_at_207_75, float4 __Plane_rename_at_207_76 )
{
    float4 __P_rename_at_209_77 = ((float4)plane_normalize_fead8a8a65eea3f0(__context__,__Plane_rename_at_207_76));
    float4 __L_rename_at_210_78 = ((float4)__Light_rename_at_207_75);
    float __d_rename_at_211_79 = ((float)-(dot4(__P_rename_at_209_77,__L_rename_at_210_78)));
    float4x4 __Result_rename_at_212_80;das_zero(__Result_rename_at_212_80);
    das_copy(das_index<float4x4>::at(__Result_rename_at_212_80,0,__context__),float4((v_extract_x(__P_rename_at_209_77) /*x*/ * v_extract_x(__L_rename_at_210_78) /*x*/) + __d_rename_at_211_79,v_extract_x(__P_rename_at_209_77) /*x*/ * v_extract_y(__L_rename_at_210_78) /*y*/,v_extract_x(__P_rename_at_209_77) /*x*/ * v_extract_z(__L_rename_at_210_78) /*z*/,v_extract_x(__P_rename_at_209_77) /*x*/ * v_extract_w(__L_rename_at_210_78) /*w*/));
    das_copy(das_index<float4x4>::at(__Result_rename_at_212_80,1,__context__),float4(v_extract_y(__P_rename_at_209_77) /*y*/ * v_extract_x(__L_rename_at_210_78) /*x*/,(v_extract_y(__P_rename_at_209_77) /*y*/ * v_extract_y(__L_rename_at_210_78) /*y*/) + __d_rename_at_211_79,v_extract_y(__P_rename_at_209_77) /*y*/ * v_extract_z(__L_rename_at_210_78) /*z*/,v_extract_y(__P_rename_at_209_77) /*y*/ * v_extract_w(__L_rename_at_210_78) /*w*/));
    das_copy(das_index<float4x4>::at(__Result_rename_at_212_80,2,__context__),float4(v_extract_z(__P_rename_at_209_77) /*z*/ * v_extract_x(__L_rename_at_210_78) /*x*/,v_extract_z(__P_rename_at_209_77) /*z*/ * v_extract_y(__L_rename_at_210_78) /*y*/,(v_extract_z(__P_rename_at_209_77) /*z*/ * v_extract_z(__L_rename_at_210_78) /*z*/) + __d_rename_at_211_79,v_extract_z(__P_rename_at_209_77) /*z*/ * v_extract_w(__L_rename_at_210_78) /*w*/));
    das_copy(das_index<float4x4>::at(__Result_rename_at_212_80,3,__context__),float4(v_extract_w(__P_rename_at_209_77) /*w*/ * v_extract_x(__L_rename_at_210_78) /*x*/,v_extract_w(__P_rename_at_209_77) /*w*/ * v_extract_y(__L_rename_at_210_78) /*y*/,v_extract_w(__P_rename_at_209_77) /*w*/ * v_extract_z(__L_rename_at_210_78) /*z*/,(v_extract_w(__P_rename_at_209_77) /*w*/ * v_extract_w(__L_rename_at_210_78) /*w*/) + __d_rename_at_211_79));
    return das_auto_cast_ref<float4x4>::cast(__Result_rename_at_212_80);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x117d365953ba55d6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&degrees_5f249a2927244f88>>();
    };
    aotLib[0x61e984e017331ef4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&radians_57b357af30d76dd3>>();
    };
    aotLib[0xc20e127b7fa05856] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&RGBA_TO_UCOLOR_7dc715ea708e1dcf>>();
    };
    aotLib[0xf8c61fa78f3df2a3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&RGBA_TO_UCOLOR_fb8dcce4cf4bf337>>();
    };
    aotLib[0x6262d8db412a018f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&UCOLOR_TO_RGBA_122d272645420117>>();
    };
    aotLib[0x1900978cfb796d78] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&UCOLOR_TO_RGB_150bcaeff21136f5>>();
    };
    aotLib[0x80a2f7455b479f74] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&linear_to_SRGB_e60b033cd2c66c96>>();
    };
    aotLib[0x9e5b958abb45cf2f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&linear_to_SRGB_8dce3429276ccd5c>>();
    };
    aotLib[0x4dadd5f914964a81] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&linear_to_SRGB_ee1acb339d8852b4>>();
    };
    aotLib[0xb3a2ddbec72a578a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_intersecting_eadba3e0f58085ff>>();
    };
    aotLib[0xe474d6f9e7d8c0da] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_intersecting_d7fd0cb5fd9dfea1>>();
    };
    aotLib[0x3987d9db4f874b3e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_intersecting_b3552c1e8df8c1>>();
    };
    aotLib[0xaf6c6025e2806ee7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&look_at_lh_d1c0097885e05205>>();
    };
    aotLib[0x6825c9f621413476] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&look_at_rh_c44f641993eea3d2>>();
    };
    aotLib[0x3935e6d94a7036ca] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&perspective_lh_d4750cbeea4193ff>>();
    };
    aotLib[0x911f6ddd04c0c845] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&perspective_rh_7371923d3c9e2e94>>();
    };
    aotLib[0x1433f1b7c125c6cc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&perspective_rh_opengl_aeaeb5be074e7848>>();
    };
    aotLib[0x65d84aee4f29293] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&ortho_rh_b77b1078f7aaafbf>>();
    };
    aotLib[0x53320b24081cab5e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&plane_dot_cd83897bbf4b3561>>();
    };
    aotLib[0x4a4be8b19db12af4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&plane_normalize_fead8a8a65eea3f0>>();
    };
    aotLib[0x75ecbe8c98d4e73] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&plane_from_point_normal_b248ffb7d8d67d08>>();
    };
    aotLib[0xf0ecfbfc8daf2234] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&planar_shadow_d6827d119541b633>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_5222113748529321679
AotListBase impl_aot_math_boost(_anon_5222113748529321679::registerAotFunctions);
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
