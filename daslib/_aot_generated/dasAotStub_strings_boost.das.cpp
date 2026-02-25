#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/runtime_profile.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require strings_boost

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
namespace _anon_7907293547004634370 {

// unused enumeration ConversionResult
extern TypeInfo __type_info__5673b06bfec20c2c;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__dbbca5962c1c455f;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63eb4c86020609;

VarInfo __func_info__d23ecd373e379628_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57442, 24, UINT64_C(0x335dc116793f6a22), "arr", 0, 0 };
VarInfo * __func_info__d23ecd373e379628_fields[1] =  { &__func_info__d23ecd373e379628_field_0 };
FuncInfo __func_info__d23ecd373e379628 = {"invoke block<(arr:array<string> const#):void> const", "", __func_info__d23ecd373e379628_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xd23ecd373e379628), 0x0 };
TypeInfo __type_info__5673b06bfec20c2c = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__dbbca5962c1c455f, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x5673b06bfec20c2c) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__dbbca5962c1c455f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xdbbca5962c1c455f) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[2] = { &__type_info__5673b06bfec20c2c, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_e56c873843c4f009 ( Context * __context__, TArray<char *> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_b6271402b6b97090 ( Context * __context__, TArray<TArray<int32_t>> & __a_rename_at_1336_2 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_3ad9824da09a5cdd ( Context * __context__, TArray<int32_t> & __a_rename_at_1336_4 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_91adfc69ef3461f9 ( Context * __context__, TArray<char *> & __a_rename_at_1142_5, TArray<char *> const  & __b_rename_at_1142_6 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_45eaa341aab85549 ( Context * __context__, TArray<TArray<int32_t>> & __Arr_rename_at_68_12, int32_t __newSize_rename_at_68_13 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_2acb50e8203df3c ( Context * __context__, TArray<int32_t> & __Arr_rename_at_68_14, int32_t __newSize_rename_at_68_15 );
inline void _FuncbuiltinTickswapTick6899974565646937647_30e37cee9b54428 ( Context * __context__, TArray<int32_t> & __a_rename_at_1968_16, TArray<int32_t> & __b_rename_at_1968_17 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_c16d2a7d1c1e09b ( Context * __context__, TArray<TTuple<16,char *,char *>> const  & __a_rename_at_602_19 );
inline char * wide_ff36c19f3a418ae ( Context * __context__, char * const  __text_rename_at_18_20, int32_t __width_rename_at_18_21 );
inline TArray<char *> split_25708434e18a9426 ( Context * __context__, char * const  __text_rename_at_111_23, char * const  __delim_rename_at_111_24 );
inline TArray<char *> split_by_chars_59d4621f1120a10f ( Context * __context__, char * const  __text_rename_at_126_27, char * const  __delim_rename_at_126_28 );
inline int32_t levenshtein_distance_113ddd1474fbed03 ( Context * __context__, char * const  __s_rename_at_155_31, char * const  __t_rename_at_155_32 );
inline int32_t levenshtein_distance_fast_910203ffab0c8f71 ( Context * __context__, char * const  __s_rename_at_182_41, char * const  __t_rename_at_182_42 );
inline char * replace_multiple_a1aee0e7830a105a ( Context * __context__, char * const  __source_rename_at_210_53, TArray<TTuple<16,char *,char *>> const  & __replaces_rename_at_210_54 );
inline bool contains_7bb746b252f89f18 ( Context * __context__, char * const  __str_rename_at_237_60, char * const  __sub_rename_at_237_61 );
inline int32_t count_9994bcd069dcd7a5 ( Context * __context__, char * const  __str_rename_at_242_62, char * const  __sub_rename_at_242_63 );
inline int32_t last_index_of_85710c373eb19edf ( Context * __context__, char * const  __str_rename_at_258_67, char * const  __sub_rename_at_258_68 );
inline int32_t last_index_of_eeae6e4eda360b1d ( Context * __context__, char * const  __str_rename_at_272_71, char * const  __sub_rename_at_272_72, int32_t __start_rename_at_272_73 );
inline char * pad_right_eb51912b440745fc ( Context * __context__, char * const  __str_rename_at_286_76, int32_t __width_rename_at_286_77, int32_t __ch_rename_at_286_78 );
inline char * pad_left_dce6b3e50ac400ca ( Context * __context__, char * const  __str_rename_at_299_81, int32_t __width_rename_at_299_82, int32_t __ch_rename_at_299_83 );
inline char * trim_prefix_32af29e52f33c299 ( Context * __context__, char * const  __str_rename_at_312_86, char * const  __prefix_rename_at_312_87 );
inline char * trim_suffix_9b9c3b172b01d96b ( Context * __context__, char * const  __str_rename_at_321_88, char * const  __suffix_rename_at_321_89 );
inline char * capitalize_9eb25af82d5ac808 ( Context * __context__, char * const  __str_rename_at_330_90 );
inline bool is_null_or_whitespace_f290521a73f831f9 ( Context * __context__, char * const  __str_rename_at_348_93 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_e56c873843c4f009 ( Context * __context__, TArray<char *> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<char *>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_b6271402b6b97090 ( Context * __context__, TArray<TArray<int32_t>> &  __a_rename_at_1336_2 )
{
    {
        bool __need_loop_1338 = true;
        // aV: array<int> aka TT&
        das_iterator<TArray<TArray<int32_t>>> __aV_iterator(__a_rename_at_1336_2);
        TArray<int32_t> * __aV_rename_at_1338_3;
        __need_loop_1338 = __aV_iterator.first(__context__,(__aV_rename_at_1338_3)) && __need_loop_1338;
        for ( ; __need_loop_1338 ; __need_loop_1338 = __aV_iterator.next(__context__,(__aV_rename_at_1338_3)) )
        {
            _FuncbuiltinTickfinalizeTick13836114024949725080_3ad9824da09a5cdd(__context__,das_arg<TArray<int32_t>>::pass((*__aV_rename_at_1338_3)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1338_3));
    };
    builtin_array_free(das_arg<TArray<TArray<int32_t>>>::pass(__a_rename_at_1336_2),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_3ad9824da09a5cdd ( Context * __context__, TArray<int32_t> &  __a_rename_at_1336_4 )
{
    builtin_array_free(das_arg<TArray<int32_t>>::pass(__a_rename_at_1336_4),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_91adfc69ef3461f9 ( Context * __context__, TArray<char *> &  __a_rename_at_1142_5, TArray<char *> const  &  __b_rename_at_1142_6 )
{
    int32_t __ln_rename_at_1143_7 = ((int32_t)builtin_array_size(__b_rename_at_1142_6));
    _FuncbuiltinTickresizeTick4811697762258667383_e56c873843c4f009(__context__,das_arg<TArray<char *>>::pass(__a_rename_at_1142_5),__ln_rename_at_1143_7);
    if ( __ln_rename_at_1143_7 == 0 )
    {
        return ;
    } else {
        {
            bool __need_loop_1153 = true;
            // aV: string aka TT&
            das_iterator<TArray<char *>> __aV_iterator(__a_rename_at_1142_5);
            char * * __aV_rename_at_1153_10;
            __need_loop_1153 = __aV_iterator.first(__context__,(__aV_rename_at_1153_10)) && __need_loop_1153;
            // bV: string aka TT const&#
            das_iterator<TArray<char *> const > __bV_iterator(__b_rename_at_1142_6);
            char * const  * __bV_rename_at_1153_11;
            __need_loop_1153 = __bV_iterator.first(__context__,(__bV_rename_at_1153_11)) && __need_loop_1153;
            for ( ; __need_loop_1153 ; __need_loop_1153 = __aV_iterator.next(__context__,(__aV_rename_at_1153_10)) && __bV_iterator.next(__context__,(__bV_rename_at_1153_11)) )
            {
                das_copy((*__aV_rename_at_1153_10),((char * const )(builtin_string_clone((*__bV_rename_at_1153_11),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            }
            __bV_iterator.close(__context__,(__bV_rename_at_1153_11));
            __aV_iterator.close(__context__,(__aV_rename_at_1153_10));
        };
    };
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_45eaa341aab85549 ( Context * __context__, TArray<TArray<int32_t>> &  __Arr_rename_at_68_12, int32_t __newSize_rename_at_68_13 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<2>(__tinfo_0,cast<TArray<TArray<int32_t>> &>::from(__Arr_rename_at_68_12),cast<char * const >::from(((char *) "array can't be resized because it contains locked elements"))));
    builtin_array_resize(das_arg<TArray<TArray<int32_t>>>::pass(__Arr_rename_at_68_12),__newSize_rename_at_68_13,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_2acb50e8203df3c ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_68_14, int32_t __newSize_rename_at_68_15 )
{
    builtin_array_resize(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_68_14),__newSize_rename_at_68_15,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickswapTick6899974565646937647_30e37cee9b54428 ( Context * __context__, TArray<int32_t> &  __a_rename_at_1968_16, TArray<int32_t> &  __b_rename_at_1968_17 )
{
    TArray<int32_t> __t_rename_at_1970_18; das_zero(__t_rename_at_1970_18); das_move(__t_rename_at_1970_18, __a_rename_at_1968_16);
    das_move(__a_rename_at_1968_16,__b_rename_at_1968_17);
    das_move(__b_rename_at_1968_17,__t_rename_at_1970_18);
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_c16d2a7d1c1e09b ( Context * __context__, TArray<TTuple<16,char *,char *>> const  &  __a_rename_at_602_19 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_602_19) == 0);
}

inline char * wide_ff36c19f3a418ae ( Context * __context__, char * const  __text_rename_at_18_20, int32_t __width_rename_at_18_21 )
{
    int32_t __extra_rename_at_20_22 = ((int32_t)(__width_rename_at_18_21 - builtin_string_length(__text_rename_at_18_20,__context__)));
    return das_auto_cast<char *>::cast((__extra_rename_at_20_22 > 0) ? das_auto_cast<char * const >::cast((cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__text_rename_at_18_20),cast<char *>::from(((char * const )(string_repeat(((char *) " "),__extra_rename_at_20_22,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),*__context__,nullptr)))) : das_auto_cast<char * const >::cast(__text_rename_at_18_20));
}

inline TArray<char *> split_25708434e18a9426 ( Context * __context__, char * const  __text_rename_at_111_23, char * const  __delim_rename_at_111_24 ) { das_stack_prologue __prologue(__context__,96,"split " DAS_FILE_LINE);
{
    TArray<char *> __res_rename_at_113_25;das_zero(__res_rename_at_113_25);
    builtin_string_split(__text_rename_at_111_23,__delim_rename_at_111_24,das_make_block<void,TArray<char *> const  &>(__context__,80,0,&__func_info__d23ecd373e379628,[&](TArray<char *> const  & __arr_rename_at_114_26) -> void{
        _FuncbuiltinTickcloneTick3038771811667655495_91adfc69ef3461f9(__context__,das_arg<TArray<char *>>::pass(__res_rename_at_113_25),__arr_rename_at_114_26);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__res_rename_at_113_25);
}}

inline TArray<char *> split_by_chars_59d4621f1120a10f ( Context * __context__, char * const  __text_rename_at_126_27, char * const  __delim_rename_at_126_28 ) { das_stack_prologue __prologue(__context__,96,"split_by_chars " DAS_FILE_LINE);
{
    TArray<char *> __res_rename_at_128_29;das_zero(__res_rename_at_128_29);
    builtin_string_split_by_char(__text_rename_at_126_27,__delim_rename_at_126_28,das_make_block<void,TArray<char *> const  &>(__context__,80,0,&__func_info__d23ecd373e379628,[&](TArray<char *> const  & __arr_rename_at_129_30) -> void{
        _FuncbuiltinTickcloneTick3038771811667655495_91adfc69ef3461f9(__context__,das_arg<TArray<char *>>::pass(__res_rename_at_128_29),__arr_rename_at_129_30);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__res_rename_at_128_29);
}}

inline int32_t levenshtein_distance_113ddd1474fbed03 ( Context * __context__, char * const  __s_rename_at_155_31, char * const  __t_rename_at_155_32 )
{
    int32_t __sLen_rename_at_157_33; memset((void*)&__sLen_rename_at_157_33,0,sizeof(__sLen_rename_at_157_33));
    int32_t __tLen_rename_at_158_34; memset((void*)&__tLen_rename_at_158_34,0,sizeof(__tLen_rename_at_158_34));
    TArray<TArray<int32_t>> __d_rename_at_159_35; memset((void*)&__d_rename_at_159_35,0,sizeof(__d_rename_at_159_35));
    int32_t __substitutionCost_rename_at_171_40; memset((void*)&__substitutionCost_rename_at_171_40,0,sizeof(__substitutionCost_rename_at_171_40));
    /* finally */ auto __finally_155= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_b6271402b6b97090(__context__,das_arg<TArray<TArray<int32_t>>>::pass(__d_rename_at_159_35));
    /* end finally */ });
    __sLen_rename_at_157_33 = ((int32_t)builtin_string_length(__s_rename_at_155_31,__context__));
    __tLen_rename_at_158_34 = ((int32_t)builtin_string_length(__t_rename_at_155_32,__context__));
    das_zero(__d_rename_at_159_35);
    _FuncbuiltinTickresizeTick4811697762258667383_45eaa341aab85549(__context__,das_arg<TArray<TArray<int32_t>>>::pass(__d_rename_at_159_35),__sLen_rename_at_157_33 + 1);
    {
        bool __need_loop_161 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__sLen_rename_at_157_33 + 1));
        int32_t __i_rename_at_161_36;
        __need_loop_161 = __i_iterator.first(__context__,(__i_rename_at_161_36)) && __need_loop_161;
        for ( ; __need_loop_161 ; __need_loop_161 = __i_iterator.next(__context__,(__i_rename_at_161_36)) )
        {
            _FuncbuiltinTickresizeTick4811697762258667383_2acb50e8203df3c(__context__,das_arg<TArray<int32_t>>::pass(__d_rename_at_159_35(__i_rename_at_161_36,__context__)),__tLen_rename_at_158_34 + 1);
            das_copy(__d_rename_at_159_35(__i_rename_at_161_36,__context__)(0,__context__),__i_rename_at_161_36);
        }
        __i_iterator.close(__context__,(__i_rename_at_161_36));
    };
    {
        bool __need_loop_165 = true;
        // j: int const
        das_iterator<range> __j_iterator(mk_range(__tLen_rename_at_158_34 + 1));
        int32_t __j_rename_at_165_37;
        __need_loop_165 = __j_iterator.first(__context__,(__j_rename_at_165_37)) && __need_loop_165;
        for ( ; __need_loop_165 ; __need_loop_165 = __j_iterator.next(__context__,(__j_rename_at_165_37)) )
        {
            das_copy(__d_rename_at_159_35(0,__context__)(__j_rename_at_165_37,__context__),__j_rename_at_165_37);
        }
        __j_iterator.close(__context__,(__j_rename_at_165_37));
    };
    {
        bool __need_loop_169 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,__sLen_rename_at_157_33 + 1));
        int32_t __i_rename_at_169_38;
        __need_loop_169 = __i_iterator.first(__context__,(__i_rename_at_169_38)) && __need_loop_169;
        for ( ; __need_loop_169 ; __need_loop_169 = __i_iterator.next(__context__,(__i_rename_at_169_38)) )
        {
            {
                bool __need_loop_170 = true;
                // j: int const
                das_iterator<range> __j_iterator(range(1,__tLen_rename_at_158_34 + 1));
                int32_t __j_rename_at_170_39;
                __need_loop_170 = __j_iterator.first(__context__,(__j_rename_at_170_39)) && __need_loop_170;
                for ( ; __need_loop_170 ; __need_loop_170 = __j_iterator.next(__context__,(__j_rename_at_170_39)) )
                {
                    __substitutionCost_rename_at_171_40 = ((int32_t)((get_character_uat(__s_rename_at_155_31,__i_rename_at_169_38 - 1) == get_character_uat(__t_rename_at_155_32,__j_rename_at_170_39 - 1)) ? das_auto_cast<int32_t>::cast(0) : das_auto_cast<int32_t>::cast(1)));
                    das_copy(__d_rename_at_159_35(__i_rename_at_169_38,__context__)(__j_rename_at_170_39,__context__),SimPolicy<int32_t>::Min(__d_rename_at_159_35((__i_rename_at_169_38 - 1),__context__)(__j_rename_at_170_39,__context__) + 1,__d_rename_at_159_35(__i_rename_at_169_38,__context__)((__j_rename_at_170_39 - 1),__context__) + 1,*__context__,nullptr));
                    das_copy(__d_rename_at_159_35(__i_rename_at_169_38,__context__)(__j_rename_at_170_39,__context__),SimPolicy<int32_t>::Min(__d_rename_at_159_35(__i_rename_at_169_38,__context__)(__j_rename_at_170_39,__context__),__d_rename_at_159_35((__i_rename_at_169_38 - 1),__context__)((__j_rename_at_170_39 - 1),__context__) + __substitutionCost_rename_at_171_40,*__context__,nullptr));
                }
                __j_iterator.close(__context__,(__j_rename_at_170_39));
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_169_38));
    };
    return das_auto_cast<int32_t>::cast(__d_rename_at_159_35(__sLen_rename_at_157_33,__context__)(__tLen_rename_at_158_34,__context__));
}

inline int32_t levenshtein_distance_fast_910203ffab0c8f71 ( Context * __context__, char * const  __s_rename_at_182_41, char * const  __t_rename_at_182_42 )
{
    int32_t __sLen_rename_at_184_43; memset((void*)&__sLen_rename_at_184_43,0,sizeof(__sLen_rename_at_184_43));
    int32_t __tLen_rename_at_185_44; memset((void*)&__tLen_rename_at_185_44,0,sizeof(__tLen_rename_at_185_44));
    TArray<int32_t> __v0_rename_at_186_45; memset((void*)&__v0_rename_at_186_45,0,sizeof(__v0_rename_at_186_45));
    TArray<int32_t> __v1_rename_at_188_46; memset((void*)&__v1_rename_at_188_46,0,sizeof(__v1_rename_at_188_46));
    int32_t __deletionCost_rename_at_197_50; memset((void*)&__deletionCost_rename_at_197_50,0,sizeof(__deletionCost_rename_at_197_50));
    int32_t __insertionCost_rename_at_198_51; memset((void*)&__insertionCost_rename_at_198_51,0,sizeof(__insertionCost_rename_at_198_51));
    int32_t __substitutionCost_rename_at_199_52; memset((void*)&__substitutionCost_rename_at_199_52,0,sizeof(__substitutionCost_rename_at_199_52));
    /* finally */ auto __finally_182= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_3ad9824da09a5cdd(__context__,das_arg<TArray<int32_t>>::pass(__v1_rename_at_188_46));
    _FuncbuiltinTickfinalizeTick13836114024949725080_3ad9824da09a5cdd(__context__,das_arg<TArray<int32_t>>::pass(__v0_rename_at_186_45));
    /* end finally */ });
    __sLen_rename_at_184_43 = ((int32_t)builtin_string_length(__s_rename_at_182_41,__context__));
    __tLen_rename_at_185_44 = ((int32_t)builtin_string_length(__t_rename_at_182_42,__context__));
    das_zero(__v0_rename_at_186_45);
    _FuncbuiltinTickresizeTick4811697762258667383_2acb50e8203df3c(__context__,das_arg<TArray<int32_t>>::pass(__v0_rename_at_186_45),__tLen_rename_at_185_44 + 1);
    das_zero(__v1_rename_at_188_46);
    _FuncbuiltinTickresizeTick4811697762258667383_2acb50e8203df3c(__context__,das_arg<TArray<int32_t>>::pass(__v1_rename_at_188_46),__tLen_rename_at_185_44 + 1);
    {
        bool __need_loop_190 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__tLen_rename_at_185_44 + 1));
        int32_t __i_rename_at_190_47;
        __need_loop_190 = __i_iterator.first(__context__,(__i_rename_at_190_47)) && __need_loop_190;
        for ( ; __need_loop_190 ; __need_loop_190 = __i_iterator.next(__context__,(__i_rename_at_190_47)) )
        {
            das_copy(__v0_rename_at_186_45(__i_rename_at_190_47,__context__),__i_rename_at_190_47);
        }
        __i_iterator.close(__context__,(__i_rename_at_190_47));
    };
    {
        bool __need_loop_194 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,__sLen_rename_at_184_43));
        int32_t __i_rename_at_194_48;
        __need_loop_194 = __i_iterator.first(__context__,(__i_rename_at_194_48)) && __need_loop_194;
        for ( ; __need_loop_194 ; __need_loop_194 = __i_iterator.next(__context__,(__i_rename_at_194_48)) )
        {
            das_copy(__v1_rename_at_188_46(0,__context__),__i_rename_at_194_48 + 1);
            {
                bool __need_loop_196 = true;
                // j: int const
                das_iterator<range> __j_iterator(range(0,__tLen_rename_at_185_44));
                int32_t __j_rename_at_196_49;
                __need_loop_196 = __j_iterator.first(__context__,(__j_rename_at_196_49)) && __need_loop_196;
                for ( ; __need_loop_196 ; __need_loop_196 = __j_iterator.next(__context__,(__j_rename_at_196_49)) )
                {
                    __deletionCost_rename_at_197_50 = ((int32_t)(__v0_rename_at_186_45((__j_rename_at_196_49 + 1),__context__) + 1));
                    __insertionCost_rename_at_198_51 = ((int32_t)(__v1_rename_at_188_46(__j_rename_at_196_49,__context__) + 1));
                    __substitutionCost_rename_at_199_52 = ((int32_t)(__v0_rename_at_186_45(__j_rename_at_196_49,__context__) + ((get_character_uat(__s_rename_at_182_41,__i_rename_at_194_48) == get_character_uat(__t_rename_at_182_42,__j_rename_at_196_49)) ? das_auto_cast<int32_t>::cast(0) : das_auto_cast<int32_t>::cast(1))));
                    das_copy(__v1_rename_at_188_46((__j_rename_at_196_49 + 1),__context__),SimPolicy<int32_t>::Min(__deletionCost_rename_at_197_50,__insertionCost_rename_at_198_51,*__context__,nullptr));
                    das_copy(__v1_rename_at_188_46((__j_rename_at_196_49 + 1),__context__),SimPolicy<int32_t>::Min(__v1_rename_at_188_46((__j_rename_at_196_49 + 1),__context__),__substitutionCost_rename_at_199_52,*__context__,nullptr));
                }
                __j_iterator.close(__context__,(__j_rename_at_196_49));
            };
            _FuncbuiltinTickswapTick6899974565646937647_30e37cee9b54428(__context__,das_arg<TArray<int32_t>>::pass(__v0_rename_at_186_45),das_arg<TArray<int32_t>>::pass(__v1_rename_at_188_46));
        }
        __i_iterator.close(__context__,(__i_rename_at_194_48));
    };
    return das_auto_cast<int32_t>::cast(__v0_rename_at_186_45(__tLen_rename_at_185_44,__context__));
}

inline char * replace_multiple_a1aee0e7830a105a ( Context * __context__, char * const  __source_rename_at_210_53, TArray<TTuple<16,char *,char *>> const  &  __replaces_rename_at_210_54 )
{
    return das_auto_cast<char *>::cast((builtin_empty(__source_rename_at_210_53) || _FuncbuiltinTickemptyTick15399874715904164783_c16d2a7d1c1e09b(__context__,__replaces_rename_at_210_54)) ? das_auto_cast<char * const >::cast(__source_rename_at_210_53) : das_auto_cast<char * const >::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_216_55) DAS_AOT_INLINE_LAMBDA -> void{
        int32_t __index_rename_at_217_56 = 0;
        int32_t __len_rename_at_218_57 = ((int32_t)builtin_string_length(__source_rename_at_210_53,__context__));
        while ( __index_rename_at_217_56 < __len_rename_at_218_57 )
        {
            bool __found_rename_at_220_58 = false;
            {
                bool __need_loop_221 = true;
                // replace: tuple<text:string;replacement:string> const&
                das_iterator<TArray<TTuple<16,char *,char *>> const > __replace_iterator(__replaces_rename_at_210_54);
                TTuple<16,char *,char *> const  * __replace_rename_at_221_59;
                __need_loop_221 = __replace_iterator.first(__context__,(__replace_rename_at_221_59)) && __need_loop_221;
                for ( ; __need_loop_221 ; __need_loop_221 = __replace_iterator.next(__context__,(__replace_rename_at_221_59)) )
                {
                    if ( builtin_string_startswith3(__source_rename_at_210_53,__index_rename_at_217_56,das_get_tuple_field<char *,0>::get((*__replace_rename_at_221_59)),__context__) )
                    {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_216_55),cast<char * const >::from(das_get_tuple_field<char *,8>::get((*__replace_rename_at_221_59)))));
                        __index_rename_at_217_56 += builtin_string_length(das_get_tuple_field<char *,0>::get((*__replace_rename_at_221_59)),__context__);
                        das_copy(__found_rename_at_220_58,true);
                        break;
                    };
                }
                __replace_iterator.close(__context__,(__replace_rename_at_221_59));
            };
            if ( !__found_rename_at_220_58 )
            {
                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_216_55),get_character_uat(__source_rename_at_210_53,__index_rename_at_217_56));
                ++__index_rename_at_217_56;
            };
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline bool contains_7bb746b252f89f18 ( Context * __context__, char * const  __str_rename_at_237_60, char * const  __sub_rename_at_237_61 )
{
    return das_auto_cast<bool>::cast(builtin_string_find2(__str_rename_at_237_60,__sub_rename_at_237_61) >= 0);
}

inline int32_t count_9994bcd069dcd7a5 ( Context * __context__, char * const  __str_rename_at_242_62, char * const  __sub_rename_at_242_63 )
{
    if ( builtin_empty(__sub_rename_at_242_63) )
    {
        return das_auto_cast<int32_t>::cast(0);
    } else {
        int32_t __n_rename_at_248_64 = 0;
        int32_t __pos_rename_at_249_65 = builtin_string_find2(__str_rename_at_242_62,__sub_rename_at_242_63);
        int32_t __subLen_rename_at_250_66 = ((int32_t)builtin_string_length(__sub_rename_at_242_63,__context__));
        while ( __pos_rename_at_249_65 >= 0 )
        {
            ++__n_rename_at_248_64;
            das_copy(__pos_rename_at_249_65,builtin_string_find1(__str_rename_at_242_62,__sub_rename_at_242_63,__pos_rename_at_249_65 + __subLen_rename_at_250_66,__context__));
        };
        return das_auto_cast<int32_t>::cast(__n_rename_at_248_64);
    };
}

inline int32_t last_index_of_85710c373eb19edf ( Context * __context__, char * const  __str_rename_at_258_67, char * const  __sub_rename_at_258_68 )
{
    if ( builtin_empty(__sub_rename_at_258_68) || builtin_empty(__str_rename_at_258_67) )
    {
        return das_auto_cast<int32_t>::cast(-1);
    } else {
        int32_t __lastPos_rename_at_263_69 = -1;
        int32_t __pos_rename_at_264_70 = builtin_string_find2(__str_rename_at_258_67,__sub_rename_at_258_68);
        while ( __pos_rename_at_264_70 >= 0 )
        {
            das_copy(__lastPos_rename_at_263_69,__pos_rename_at_264_70);
            das_copy(__pos_rename_at_264_70,builtin_string_find1(__str_rename_at_258_67,__sub_rename_at_258_68,__pos_rename_at_264_70 + 1,__context__));
        };
        return das_auto_cast<int32_t>::cast(__lastPos_rename_at_263_69);
    };
}

inline int32_t last_index_of_eeae6e4eda360b1d ( Context * __context__, char * const  __str_rename_at_272_71, char * const  __sub_rename_at_272_72, int32_t __start_rename_at_272_73 )
{
    if ( builtin_empty(__sub_rename_at_272_72) || builtin_empty(__str_rename_at_272_71) )
    {
        return das_auto_cast<int32_t>::cast(-1);
    } else {
        int32_t __lastPos_rename_at_277_74 = -1;
        int32_t __pos_rename_at_278_75 = builtin_string_find2(__str_rename_at_272_71,__sub_rename_at_272_72);
        while ( (__pos_rename_at_278_75 >= 0) && (__pos_rename_at_278_75 < __start_rename_at_272_73) )
        {
            das_copy(__lastPos_rename_at_277_74,__pos_rename_at_278_75);
            das_copy(__pos_rename_at_278_75,builtin_string_find1(__str_rename_at_272_71,__sub_rename_at_272_72,__pos_rename_at_278_75 + 1,__context__));
        };
        return das_auto_cast<int32_t>::cast(__lastPos_rename_at_277_74);
    };
}

inline char * pad_right_eb51912b440745fc ( Context * __context__, char * const  __str_rename_at_286_76, int32_t __width_rename_at_286_77, int32_t __ch_rename_at_286_78 )
{
    int32_t __l_rename_at_289_79 = ((int32_t)builtin_string_length(__str_rename_at_286_76,__context__));
    return das_auto_cast<char *>::cast((__l_rename_at_289_79 >= __width_rename_at_286_77) ? das_auto_cast<char * const >::cast(__str_rename_at_286_76) : das_auto_cast<char * const >::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_293_80) DAS_AOT_INLINE_LAMBDA -> void{
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_293_80),cast<char * const >::from(__str_rename_at_286_76)));
        write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_293_80),__ch_rename_at_286_78,__width_rename_at_286_77 - __l_rename_at_289_79);
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline char * pad_left_dce6b3e50ac400ca ( Context * __context__, char * const  __str_rename_at_299_81, int32_t __width_rename_at_299_82, int32_t __ch_rename_at_299_83 )
{
    int32_t __l_rename_at_302_84 = ((int32_t)builtin_string_length(__str_rename_at_299_81,__context__));
    return das_auto_cast<char *>::cast((__l_rename_at_302_84 >= __width_rename_at_299_82) ? das_auto_cast<char * const >::cast(__str_rename_at_299_81) : das_auto_cast<char * const >::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_306_85) DAS_AOT_INLINE_LAMBDA -> void{
        write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_306_85),__ch_rename_at_299_83,__width_rename_at_299_82 - __l_rename_at_302_84);
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_306_85),cast<char * const >::from(__str_rename_at_299_81)));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline char * trim_prefix_32af29e52f33c299 ( Context * __context__, char * const  __str_rename_at_312_86, char * const  __prefix_rename_at_312_87 )
{
    return das_auto_cast<char *>::cast(builtin_string_startswith(__str_rename_at_312_86,__prefix_rename_at_312_87,__context__) ? das_auto_cast<char * const >::cast(((char * const )(builtin_string_slice2(__str_rename_at_312_86,builtin_string_length(__prefix_rename_at_312_87,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) : das_auto_cast<char * const >::cast(__str_rename_at_312_86));
}

inline char * trim_suffix_9b9c3b172b01d96b ( Context * __context__, char * const  __str_rename_at_321_88, char * const  __suffix_rename_at_321_89 )
{
    return das_auto_cast<char *>::cast(builtin_string_endswith(__str_rename_at_321_88,__suffix_rename_at_321_89,__context__) ? das_auto_cast<char * const >::cast(((char * const )(builtin_string_slice1(__str_rename_at_321_88,0,builtin_string_length(__str_rename_at_321_88,__context__) - builtin_string_length(__suffix_rename_at_321_89,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) : das_auto_cast<char * const >::cast(__str_rename_at_321_88));
}

inline char * capitalize_9eb25af82d5ac808 ( Context * __context__, char * const  __str_rename_at_330_90 )
{
    if ( builtin_empty(__str_rename_at_330_90) )
    {
        return das_auto_cast<char *>::cast(__str_rename_at_330_90);
    } else {
        int32_t __first_rename_at_336_91 = ((int32_t)get_character_at(__str_rename_at_330_90,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        return das_auto_cast<char *>::cast(((__first_rename_at_336_91 >= 97) && (__first_rename_at_336_91 <= 122)) ? das_auto_cast<char * const >::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_338_92) DAS_AOT_INLINE_LAMBDA -> void{
            write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_338_92),(__first_rename_at_336_91 - 97) + 65);
            if ( builtin_string_length(__str_rename_at_330_90,__context__) > 1 )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_338_92),cast<char * const >::from(((char * const )(builtin_string_slice2(__str_rename_at_330_90,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
            };
        },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) : das_auto_cast<char * const >::cast(__str_rename_at_330_90));
    };
}

inline bool is_null_or_whitespace_f290521a73f831f9 ( Context * __context__, char * const  __str_rename_at_348_93 )
{
    if ( builtin_empty(__str_rename_at_348_93) )
    {
        return das_auto_cast<bool>::cast(true);
    } else {
        {
            bool __need_loop_353 = true;
            // i: int const
            das_iterator<range> __i_iterator(mk_range(builtin_string_length(__str_rename_at_348_93,__context__)));
            int32_t __i_rename_at_353_94;
            __need_loop_353 = __i_iterator.first(__context__,(__i_rename_at_353_94)) && __need_loop_353;
            for ( ; __need_loop_353 ; __need_loop_353 = __i_iterator.next(__context__,(__i_rename_at_353_94)) )
            {
                if ( !is_white_space(get_character_at(__str_rename_at_348_93,__i_rename_at_353_94,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))) )
                {
                    return das_auto_cast<bool>::cast(false);
                };
            }
            __i_iterator.close(__context__,(__i_rename_at_353_94));
        };
        return das_auto_cast<bool>::cast(true);
    };
}
static vec4f __wrap__FuncbuiltinTickresizeTick4811697762258667383_e56c873843c4f009 ( Context * __context__ ) {
    TArray<char *> &  arg_Arr = cast_aot_arg<TArray<char *> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickresizeTick4811697762258667383_e56c873843c4f009(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickfinalizeTick13836114024949725080_b6271402b6b97090 ( Context * __context__ ) {
    TArray<TArray<int32_t>> &  arg_a = cast_aot_arg<TArray<TArray<int32_t>> & >::to(*__context__,__context__->abiArguments()[0]);
    _FuncbuiltinTickfinalizeTick13836114024949725080_b6271402b6b97090(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickfinalizeTick13836114024949725080_3ad9824da09a5cdd ( Context * __context__ ) {
    TArray<int32_t> &  arg_a = cast_aot_arg<TArray<int32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    _FuncbuiltinTickfinalizeTick13836114024949725080_3ad9824da09a5cdd(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickcloneTick3038771811667655495_91adfc69ef3461f9 ( Context * __context__ ) {
    TArray<char *> &  arg_a = cast_aot_arg<TArray<char *> & >::to(*__context__,__context__->abiArguments()[0]);
    TArray<char *> const  &  arg_b = cast_aot_arg<TArray<char *> const  & >::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickcloneTick3038771811667655495_91adfc69ef3461f9(__context__, arg_a, arg_b);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickresizeTick4811697762258667383_45eaa341aab85549 ( Context * __context__ ) {
    TArray<TArray<int32_t>> &  arg_Arr = cast_aot_arg<TArray<TArray<int32_t>> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickresizeTick4811697762258667383_45eaa341aab85549(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickresizeTick4811697762258667383_2acb50e8203df3c ( Context * __context__ ) {
    TArray<int32_t> &  arg_Arr = cast_aot_arg<TArray<int32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickresizeTick4811697762258667383_2acb50e8203df3c(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickswapTick6899974565646937647_30e37cee9b54428 ( Context * __context__ ) {
    TArray<int32_t> &  arg_a = cast_aot_arg<TArray<int32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    TArray<int32_t> &  arg_b = cast_aot_arg<TArray<int32_t> & >::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickswapTick6899974565646937647_30e37cee9b54428(__context__, arg_a, arg_b);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickemptyTick15399874715904164783_c16d2a7d1c1e09b ( Context * __context__ ) {
    TArray<TTuple<16,char *,char *>> const  &  arg_a = cast_aot_arg<TArray<TTuple<16,char *,char *>> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncbuiltinTickemptyTick15399874715904164783_c16d2a7d1c1e09b(__context__, arg_a));
}
static vec4f __wrap_wide_ff36c19f3a418ae ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_width = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(wide_ff36c19f3a418ae(__context__, arg_text, arg_width));
}
static vec4f __wrap_split_25708434e18a9426 ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_delim = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    *((TArray<char *> *) __context__->abiCMRES) = split_25708434e18a9426(__context__, arg_text, arg_delim);
    return v_zero();
}
static vec4f __wrap_split_by_chars_59d4621f1120a10f ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_delim = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    *((TArray<char *> *) __context__->abiCMRES) = split_by_chars_59d4621f1120a10f(__context__, arg_text, arg_delim);
    return v_zero();
}
static vec4f __wrap_levenshtein_distance_113ddd1474fbed03 ( Context * __context__ ) {
    char * const  arg_s = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_t = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(levenshtein_distance_113ddd1474fbed03(__context__, arg_s, arg_t));
}
static vec4f __wrap_levenshtein_distance_fast_910203ffab0c8f71 ( Context * __context__ ) {
    char * const  arg_s = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_t = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(levenshtein_distance_fast_910203ffab0c8f71(__context__, arg_s, arg_t));
}
static vec4f __wrap_replace_multiple_a1aee0e7830a105a ( Context * __context__ ) {
    char * const  arg_source = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    TArray<TTuple<16,char *,char *>> const  &  arg_replaces = cast_aot_arg<TArray<TTuple<16,char *,char *>> const  & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(replace_multiple_a1aee0e7830a105a(__context__, arg_source, arg_replaces));
}
static vec4f __wrap_contains_7bb746b252f89f18 ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_sub = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(contains_7bb746b252f89f18(__context__, arg_str, arg_sub));
}
static vec4f __wrap_count_9994bcd069dcd7a5 ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_sub = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(count_9994bcd069dcd7a5(__context__, arg_str, arg_sub));
}
static vec4f __wrap_last_index_of_85710c373eb19edf ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_sub = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(last_index_of_85710c373eb19edf(__context__, arg_str, arg_sub));
}
static vec4f __wrap_last_index_of_eeae6e4eda360b1d ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_sub = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_start = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(last_index_of_eeae6e4eda360b1d(__context__, arg_str, arg_sub, arg_start));
}
static vec4f __wrap_pad_right_eb51912b440745fc ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_width = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    return cast<char *>::from(pad_right_eb51912b440745fc(__context__, arg_str, arg_width, arg_ch));
}
static vec4f __wrap_pad_left_dce6b3e50ac400ca ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_width = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    return cast<char *>::from(pad_left_dce6b3e50ac400ca(__context__, arg_str, arg_width, arg_ch));
}
static vec4f __wrap_trim_prefix_32af29e52f33c299 ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_prefix = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(trim_prefix_32af29e52f33c299(__context__, arg_str, arg_prefix));
}
static vec4f __wrap_trim_suffix_9b9c3b172b01d96b ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_suffix = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(trim_suffix_9b9c3b172b01d96b(__context__, arg_str, arg_suffix));
}
static vec4f __wrap_capitalize_9eb25af82d5ac808 ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(capitalize_9eb25af82d5ac808(__context__, arg_str));
}
static vec4f __wrap_is_null_or_whitespace_f290521a73f831f9 ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_null_or_whitespace_f290521a73f831f9(__context__, arg_str));
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0xedd3c4f89cba620e, false, (void*)&_FuncbuiltinTickresizeTick4811697762258667383_e56c873843c4f009, &__wrap__FuncbuiltinTickresizeTick4811697762258667383_e56c873843c4f009 },
    { 0x57cf6eebce5610d8, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_b6271402b6b97090, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_b6271402b6b97090 },
    { 0x84908fa671ce3a30, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_3ad9824da09a5cdd, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_3ad9824da09a5cdd },
    { 0xdc88925fc4a84c9a, false, (void*)&_FuncbuiltinTickcloneTick3038771811667655495_91adfc69ef3461f9, &__wrap__FuncbuiltinTickcloneTick3038771811667655495_91adfc69ef3461f9 },
    { 0x4fbf2c6331e1f592, false, (void*)&_FuncbuiltinTickresizeTick4811697762258667383_45eaa341aab85549, &__wrap__FuncbuiltinTickresizeTick4811697762258667383_45eaa341aab85549 },
    { 0xa51a880d594e91fd, false, (void*)&_FuncbuiltinTickresizeTick4811697762258667383_2acb50e8203df3c, &__wrap__FuncbuiltinTickresizeTick4811697762258667383_2acb50e8203df3c },
    { 0xeb56ea4904494c06, false, (void*)&_FuncbuiltinTickswapTick6899974565646937647_30e37cee9b54428, &__wrap__FuncbuiltinTickswapTick6899974565646937647_30e37cee9b54428 },
    { 0x61bae83783541fd8, false, (void*)&_FuncbuiltinTickemptyTick15399874715904164783_c16d2a7d1c1e09b, &__wrap__FuncbuiltinTickemptyTick15399874715904164783_c16d2a7d1c1e09b },
    { 0xf72349b9dfd70ed0, false, (void*)&wide_ff36c19f3a418ae, &__wrap_wide_ff36c19f3a418ae },
    { 0x45778d4232b8eaf1, true, (void*)&split_25708434e18a9426, &__wrap_split_25708434e18a9426 },
    { 0x1f2c032e256b174e, true, (void*)&split_by_chars_59d4621f1120a10f, &__wrap_split_by_chars_59d4621f1120a10f },
    { 0xe5b7ab2b147b152e, false, (void*)&levenshtein_distance_113ddd1474fbed03, &__wrap_levenshtein_distance_113ddd1474fbed03 },
    { 0x93b753a43ff03439, false, (void*)&levenshtein_distance_fast_910203ffab0c8f71, &__wrap_levenshtein_distance_fast_910203ffab0c8f71 },
    { 0x38dfec62f852eb36, false, (void*)&replace_multiple_a1aee0e7830a105a, &__wrap_replace_multiple_a1aee0e7830a105a },
    { 0xe5c80174a708b1ca, false, (void*)&contains_7bb746b252f89f18, &__wrap_contains_7bb746b252f89f18 },
    { 0x37db1962f2fbc95e, false, (void*)&count_9994bcd069dcd7a5, &__wrap_count_9994bcd069dcd7a5 },
    { 0xb321fdc0f329e706, false, (void*)&last_index_of_85710c373eb19edf, &__wrap_last_index_of_85710c373eb19edf },
    { 0xb4d4ce4e2eb2ba78, false, (void*)&last_index_of_eeae6e4eda360b1d, &__wrap_last_index_of_eeae6e4eda360b1d },
    { 0x4f55cb7bbd283fb3, false, (void*)&pad_right_eb51912b440745fc, &__wrap_pad_right_eb51912b440745fc },
    { 0xa9a8b2009f06217, false, (void*)&pad_left_dce6b3e50ac400ca, &__wrap_pad_left_dce6b3e50ac400ca },
    { 0xea5f15b757723409, false, (void*)&trim_prefix_32af29e52f33c299, &__wrap_trim_prefix_32af29e52f33c299 },
    { 0x1ec5b9da47f74118, false, (void*)&trim_suffix_9b9c3b172b01d96b, &__wrap_trim_suffix_9b9c3b172b01d96b },
    { 0xf121294336b2b42d, false, (void*)&capitalize_9eb25af82d5ac808, &__wrap_capitalize_9eb25af82d5ac808 },
    { 0x5ce4348131c7d0ce, false, (void*)&is_null_or_whitespace_f290521a73f831f9, &__wrap_is_null_or_whitespace_f290521a73f831f9 },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
    resolveTypeInfoAnnotations();
}

} // namespace _anon_7907293547004634370
AotListBase impl_aot_strings_boost(_anon_7907293547004634370::registerAotFunctions);
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
