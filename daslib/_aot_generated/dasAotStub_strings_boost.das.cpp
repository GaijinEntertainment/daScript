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
extern TypeInfo __type_info__b8d4477a5991973e;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63eb4c86020609;

VarInfo __func_info__d23ecd373e379628_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57442, TypeSize<TArray<char *>>::size, UINT64_C(0x335dc116793f6a22), "arr", 0, 0 };
VarInfo * __func_info__d23ecd373e379628_fields[1] =  { &__func_info__d23ecd373e379628_field_0 };
FuncInfo __func_info__d23ecd373e379628 = {"invoke block<(arr:array<string> const#):void> const", "", __func_info__d23ecd373e379628_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xd23ecd373e379628), 0x0 };
TypeInfo __type_info__5673b06bfec20c2c = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__dbbca5962c1c455f, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<TArray<int32_t>>>::size, UINT64_C(0x5673b06bfec20c2c) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<StringBuilderWriter>::size, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__dbbca5962c1c455f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<int32_t>>::size, UINT64_C(0xdbbca5962c1c455f) };
TypeInfo __type_info__b8d4477a5991973e = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<char *>>::size, UINT64_C(0xb8d4477a5991973e) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<void>::size, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__b8d4477a5991973e };
TypeInfo * __tinfo_1[1] = { &__type_info__5673b06bfec20c2c };
TypeInfo * __tinfo_2[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8e7e2f2250c59e0a ( Context * __context__, TArray<char *> & __a_rename_at_50_2 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5be9a99f60cdfbba ( Context * __context__, TArray<TArray<int32_t>> & __a_rename_at_1308_3 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8 ( Context * __context__, TArray<int32_t> & __a_rename_at_1308_5 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_4060192a3cb0948 ( Context * __context__, TArray<char *> & __a_rename_at_1114_6, TArray<char *> const  & __b_rename_at_1114_7 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_c1a73db481e59a8b ( Context * __context__, TArray<TArray<int32_t>> & __Arr_rename_at_68_13, int32_t __newSize_rename_at_68_14 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_68_15, int32_t __newSize_rename_at_68_16 );
inline void _FuncbuiltinTickswapTick6899974565646937647_4b12db88e6e6aa18 ( Context * __context__, TArray<int32_t> & __a_rename_at_1906_17, TArray<int32_t> & __b_rename_at_1906_18 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_17e819552d1fbeb4 ( Context * __context__, TArray<AutoTuple<char *,char *>> const  & __a_rename_at_590_20 );
inline char * wide_53059f73cffda483 ( Context * __context__, char * const  __text_rename_at_12_21, int32_t __width_rename_at_12_22 );
inline TArray<char *> split_e671885aa89ee032 ( Context * __context__, char * const  __text_rename_at_96_24, char * const  __delim_rename_at_96_25 );
inline TArray<char *> split_by_chars_143571b759d8cb31 ( Context * __context__, char * const  __text_rename_at_108_28, char * const  __delim_rename_at_108_29 );
inline int32_t levenshtein_distance_549bdedc98dec005 ( Context * __context__, char * const  __s_rename_at_133_32, char * const  __t_rename_at_133_33 );
inline int32_t levenshtein_distance_fast_57b8adcc7543af10 ( Context * __context__, char * const  __s_rename_at_161_42, char * const  __t_rename_at_161_43 );
inline char * replace_multiple_d0cf6948c5333e25 ( Context * __context__, char * const  __source_rename_at_191_54, TArray<AutoTuple<char *,char *>> const  & __replaces_rename_at_191_55 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<char *>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8e7e2f2250c59e0a ( Context * __context__, TArray<char *> &  __a_rename_at_50_2 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<char *> &>::from(__a_rename_at_50_2)));
    return das_auto_cast_ref<TArray<char *> &>::cast(__a_rename_at_50_2);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5be9a99f60cdfbba ( Context * __context__, TArray<TArray<int32_t>> &  __a_rename_at_1308_3 )
{
    {
        bool __need_loop_1310 = true;
        // aV: array<int> aka TT&
        das_iterator<TArray<TArray<int32_t>>> __aV_iterator(__a_rename_at_1308_3);
        TArray<int32_t> * __aV_rename_at_1310_4;
        __need_loop_1310 = __aV_iterator.first(__context__,(__aV_rename_at_1310_4)) && __need_loop_1310;
        for ( ; __need_loop_1310 ; __need_loop_1310 = __aV_iterator.next(__context__,(__aV_rename_at_1310_4)) )
        {
            _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8(__context__,das_arg<TArray<int32_t>>::pass((*__aV_rename_at_1310_4)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1310_4));
    };
    builtin_array_free(das_arg<TArray<TArray<int32_t>>>::pass(__a_rename_at_1308_3),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8 ( Context * __context__, TArray<int32_t> &  __a_rename_at_1308_5 )
{
    builtin_array_free(das_arg<TArray<int32_t>>::pass(__a_rename_at_1308_5),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_4060192a3cb0948 ( Context * __context__, TArray<char *> &  __a_rename_at_1114_6, TArray<char *> const  &  __b_rename_at_1114_7 )
{
    int32_t __ln_rename_at_1115_8 = ((int32_t)builtin_array_size(__b_rename_at_1114_7));
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__a_rename_at_1114_6),__ln_rename_at_1115_8);
    if ( __ln_rename_at_1115_8 == 0 )
    {
        return ;
    } else {
        {
            bool __need_loop_1125 = true;
            // aV: string aka TT&
            das_iterator<TArray<char *>> __aV_iterator(__a_rename_at_1114_6);
            char * * __aV_rename_at_1125_11;
            __need_loop_1125 = __aV_iterator.first(__context__,(__aV_rename_at_1125_11)) && __need_loop_1125;
            // bV: string const&#
            das_iterator<TArray<char *> const > __bV_iterator(__b_rename_at_1114_7);
            char * const  * __bV_rename_at_1125_12;
            __need_loop_1125 = __bV_iterator.first(__context__,(__bV_rename_at_1125_12)) && __need_loop_1125;
            for ( ; __need_loop_1125 ; __need_loop_1125 = __aV_iterator.next(__context__,(__aV_rename_at_1125_11)) && __bV_iterator.next(__context__,(__bV_rename_at_1125_12)) )
            {
                das_copy((*__aV_rename_at_1125_11),((char * const )(builtin_string_clone((*__bV_rename_at_1125_12),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            }
            __aV_iterator.close(__context__,(__aV_rename_at_1125_11));
            __bV_iterator.close(__context__,(__bV_rename_at_1125_12));
        };
    };
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_c1a73db481e59a8b ( Context * __context__, TArray<TArray<int32_t>> &  __Arr_rename_at_68_13, int32_t __newSize_rename_at_68_14 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TArray<TArray<int32_t>> &>::from(__Arr_rename_at_68_13)));
    builtin_array_resize(das_arg<TArray<TArray<int32_t>>>::pass(__Arr_rename_at_68_13),__newSize_rename_at_68_14,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_68_15, int32_t __newSize_rename_at_68_16 )
{
    builtin_array_resize(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_68_15),__newSize_rename_at_68_16,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickswapTick6899974565646937647_4b12db88e6e6aa18 ( Context * __context__, TArray<int32_t> &  __a_rename_at_1906_17, TArray<int32_t> &  __b_rename_at_1906_18 )
{
    TArray<int32_t> __t_rename_at_1908_19; das_zero(__t_rename_at_1908_19); das_move(__t_rename_at_1908_19, __a_rename_at_1906_17);
    das_move(__a_rename_at_1906_17,__b_rename_at_1906_18);
    das_move(__b_rename_at_1906_18,__t_rename_at_1908_19);
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_17e819552d1fbeb4 ( Context * __context__, TArray<AutoTuple<char *,char *>> const  &  __a_rename_at_590_20 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_590_20) == 0);
}

inline char * wide_53059f73cffda483 ( Context * __context__, char * const  __text_rename_at_12_21, int32_t __width_rename_at_12_22 )
{
    int32_t __extra_rename_at_13_23 = ((int32_t)(__width_rename_at_12_22 - builtin_string_length(__text_rename_at_12_21,__context__)));
    return das_auto_cast<char *>::cast((__extra_rename_at_13_23 > 0) ? das_auto_cast<char * const >::cast((cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__text_rename_at_12_21),cast<char *>::from(((char * const )(string_repeat(((char *) " "),__extra_rename_at_13_23,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),*__context__,nullptr)))) : das_auto_cast<char * const >::cast(__text_rename_at_12_21));
}

inline TArray<char *> split_e671885aa89ee032 ( Context * __context__, char * const  __text_rename_at_96_24, char * const  __delim_rename_at_96_25 ) { das_stack_prologue __prologue(__context__,96,"split " DAS_FILE_LINE);
{
    TArray<char *> __res_rename_at_97_26;das_zero(__res_rename_at_97_26);
    builtin_string_split(__text_rename_at_96_24,__delim_rename_at_96_25,das_make_block<void,TArray<char *> const  &>(__context__,80,0,&__func_info__d23ecd373e379628,[&](TArray<char *> const  & __arr_rename_at_98_27) -> void{
        _FuncbuiltinTickcloneTick3038771811667655495_4060192a3cb0948(__context__,das_arg<TArray<char *>>::pass(__res_rename_at_97_26),__arr_rename_at_98_27);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8e7e2f2250c59e0a(__context__,das_arg<TArray<char *>>::pass(__res_rename_at_97_26)));
}}

inline TArray<char *> split_by_chars_143571b759d8cb31 ( Context * __context__, char * const  __text_rename_at_108_28, char * const  __delim_rename_at_108_29 ) { das_stack_prologue __prologue(__context__,96,"split_by_chars " DAS_FILE_LINE);
{
    TArray<char *> __res_rename_at_109_30;das_zero(__res_rename_at_109_30);
    builtin_string_split_by_char(__text_rename_at_108_28,__delim_rename_at_108_29,das_make_block<void,TArray<char *> const  &>(__context__,80,0,&__func_info__d23ecd373e379628,[&](TArray<char *> const  & __arr_rename_at_110_31) -> void{
        _FuncbuiltinTickcloneTick3038771811667655495_4060192a3cb0948(__context__,das_arg<TArray<char *>>::pass(__res_rename_at_109_30),__arr_rename_at_110_31);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8e7e2f2250c59e0a(__context__,das_arg<TArray<char *>>::pass(__res_rename_at_109_30)));
}}

inline int32_t levenshtein_distance_549bdedc98dec005 ( Context * __context__, char * const  __s_rename_at_133_32, char * const  __t_rename_at_133_33 )
{
    int32_t __sLen_rename_at_134_34; memset((void*)&__sLen_rename_at_134_34,0,sizeof(__sLen_rename_at_134_34));
    int32_t __tLen_rename_at_135_35; memset((void*)&__tLen_rename_at_135_35,0,sizeof(__tLen_rename_at_135_35));
    TArray<TArray<int32_t>> __d_rename_at_136_36; memset((void*)&__d_rename_at_136_36,0,sizeof(__d_rename_at_136_36));
    int32_t __substitutionCost_rename_at_148_41; memset((void*)&__substitutionCost_rename_at_148_41,0,sizeof(__substitutionCost_rename_at_148_41));
    /* finally */ auto __finally_133= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_5be9a99f60cdfbba(__context__,das_arg<TArray<TArray<int32_t>>>::pass(__d_rename_at_136_36));
    /* end finally */ });
    __sLen_rename_at_134_34 = ((int32_t)builtin_string_length(__s_rename_at_133_32,__context__));
    __tLen_rename_at_135_35 = ((int32_t)builtin_string_length(__t_rename_at_133_33,__context__));
    das_zero(__d_rename_at_136_36);
    _FuncbuiltinTickresizeTick4811697762258667383_c1a73db481e59a8b(__context__,das_arg<TArray<TArray<int32_t>>>::pass(__d_rename_at_136_36),__sLen_rename_at_134_34 + 1);
    {
        bool __need_loop_138 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__sLen_rename_at_134_34 + 1));
        int32_t __i_rename_at_138_37;
        __need_loop_138 = __i_iterator.first(__context__,(__i_rename_at_138_37)) && __need_loop_138;
        for ( ; __need_loop_138 ; __need_loop_138 = __i_iterator.next(__context__,(__i_rename_at_138_37)) )
        {
            _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51(__context__,das_arg<TArray<int32_t>>::pass(__d_rename_at_136_36(__i_rename_at_138_37,__context__)),__tLen_rename_at_135_35 + 1);
            das_copy(__d_rename_at_136_36(__i_rename_at_138_37,__context__)(0,__context__),__i_rename_at_138_37);
        }
        __i_iterator.close(__context__,(__i_rename_at_138_37));
    };
    {
        bool __need_loop_142 = true;
        // j: int const
        das_iterator<range> __j_iterator(mk_range(__tLen_rename_at_135_35 + 1));
        int32_t __j_rename_at_142_38;
        __need_loop_142 = __j_iterator.first(__context__,(__j_rename_at_142_38)) && __need_loop_142;
        for ( ; __need_loop_142 ; __need_loop_142 = __j_iterator.next(__context__,(__j_rename_at_142_38)) )
        {
            das_copy(__d_rename_at_136_36(0,__context__)(__j_rename_at_142_38,__context__),__j_rename_at_142_38);
        }
        __j_iterator.close(__context__,(__j_rename_at_142_38));
    };
    {
        bool __need_loop_146 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,__sLen_rename_at_134_34 + 1));
        int32_t __i_rename_at_146_39;
        __need_loop_146 = __i_iterator.first(__context__,(__i_rename_at_146_39)) && __need_loop_146;
        for ( ; __need_loop_146 ; __need_loop_146 = __i_iterator.next(__context__,(__i_rename_at_146_39)) )
        {
            {
                bool __need_loop_147 = true;
                // j: int const
                das_iterator<range> __j_iterator(range(1,__tLen_rename_at_135_35 + 1));
                int32_t __j_rename_at_147_40;
                __need_loop_147 = __j_iterator.first(__context__,(__j_rename_at_147_40)) && __need_loop_147;
                for ( ; __need_loop_147 ; __need_loop_147 = __j_iterator.next(__context__,(__j_rename_at_147_40)) )
                {
                    __substitutionCost_rename_at_148_41 = ((int32_t)((get_character_uat(__s_rename_at_133_32,__i_rename_at_146_39 - 1) == get_character_uat(__t_rename_at_133_33,__j_rename_at_147_40 - 1)) ? das_auto_cast<int32_t>::cast(0) : das_auto_cast<int32_t>::cast(1)));
                    das_copy(__d_rename_at_136_36(__i_rename_at_146_39,__context__)(__j_rename_at_147_40,__context__),SimPolicy<int32_t>::Min(__d_rename_at_136_36((__i_rename_at_146_39 - 1),__context__)(__j_rename_at_147_40,__context__) + 1,__d_rename_at_136_36(__i_rename_at_146_39,__context__)((__j_rename_at_147_40 - 1),__context__) + 1,*__context__,nullptr));
                    das_copy(__d_rename_at_136_36(__i_rename_at_146_39,__context__)(__j_rename_at_147_40,__context__),SimPolicy<int32_t>::Min(__d_rename_at_136_36(__i_rename_at_146_39,__context__)(__j_rename_at_147_40,__context__),__d_rename_at_136_36((__i_rename_at_146_39 - 1),__context__)((__j_rename_at_147_40 - 1),__context__) + __substitutionCost_rename_at_148_41,*__context__,nullptr));
                }
                __j_iterator.close(__context__,(__j_rename_at_147_40));
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_146_39));
    };
    return das_auto_cast<int32_t>::cast(__d_rename_at_136_36(__sLen_rename_at_134_34,__context__)(__tLen_rename_at_135_35,__context__));
}

inline int32_t levenshtein_distance_fast_57b8adcc7543af10 ( Context * __context__, char * const  __s_rename_at_161_42, char * const  __t_rename_at_161_43 )
{
    int32_t __sLen_rename_at_162_44; memset((void*)&__sLen_rename_at_162_44,0,sizeof(__sLen_rename_at_162_44));
    int32_t __tLen_rename_at_163_45; memset((void*)&__tLen_rename_at_163_45,0,sizeof(__tLen_rename_at_163_45));
    TArray<int32_t> __v0_rename_at_164_46; memset((void*)&__v0_rename_at_164_46,0,sizeof(__v0_rename_at_164_46));
    TArray<int32_t> __v1_rename_at_166_47; memset((void*)&__v1_rename_at_166_47,0,sizeof(__v1_rename_at_166_47));
    int32_t __deletionCost_rename_at_175_51; memset((void*)&__deletionCost_rename_at_175_51,0,sizeof(__deletionCost_rename_at_175_51));
    int32_t __insertionCost_rename_at_176_52; memset((void*)&__insertionCost_rename_at_176_52,0,sizeof(__insertionCost_rename_at_176_52));
    int32_t __substitutionCost_rename_at_177_53; memset((void*)&__substitutionCost_rename_at_177_53,0,sizeof(__substitutionCost_rename_at_177_53));
    /* finally */ auto __finally_161= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8(__context__,das_arg<TArray<int32_t>>::pass(__v0_rename_at_164_46));
    _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8(__context__,das_arg<TArray<int32_t>>::pass(__v1_rename_at_166_47));
    /* end finally */ });
    __sLen_rename_at_162_44 = ((int32_t)builtin_string_length(__s_rename_at_161_42,__context__));
    __tLen_rename_at_163_45 = ((int32_t)builtin_string_length(__t_rename_at_161_43,__context__));
    das_zero(__v0_rename_at_164_46);
    _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51(__context__,das_arg<TArray<int32_t>>::pass(__v0_rename_at_164_46),__tLen_rename_at_163_45 + 1);
    das_zero(__v1_rename_at_166_47);
    _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51(__context__,das_arg<TArray<int32_t>>::pass(__v1_rename_at_166_47),__tLen_rename_at_163_45 + 1);
    {
        bool __need_loop_168 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__tLen_rename_at_163_45 + 1));
        int32_t __i_rename_at_168_48;
        __need_loop_168 = __i_iterator.first(__context__,(__i_rename_at_168_48)) && __need_loop_168;
        for ( ; __need_loop_168 ; __need_loop_168 = __i_iterator.next(__context__,(__i_rename_at_168_48)) )
        {
            das_copy(__v0_rename_at_164_46(__i_rename_at_168_48,__context__),__i_rename_at_168_48);
        }
        __i_iterator.close(__context__,(__i_rename_at_168_48));
    };
    {
        bool __need_loop_172 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,__sLen_rename_at_162_44));
        int32_t __i_rename_at_172_49;
        __need_loop_172 = __i_iterator.first(__context__,(__i_rename_at_172_49)) && __need_loop_172;
        for ( ; __need_loop_172 ; __need_loop_172 = __i_iterator.next(__context__,(__i_rename_at_172_49)) )
        {
            das_copy(__v1_rename_at_166_47(0,__context__),__i_rename_at_172_49 + 1);
            {
                bool __need_loop_174 = true;
                // j: int const
                das_iterator<range> __j_iterator(range(0,__tLen_rename_at_163_45));
                int32_t __j_rename_at_174_50;
                __need_loop_174 = __j_iterator.first(__context__,(__j_rename_at_174_50)) && __need_loop_174;
                for ( ; __need_loop_174 ; __need_loop_174 = __j_iterator.next(__context__,(__j_rename_at_174_50)) )
                {
                    __deletionCost_rename_at_175_51 = ((int32_t)(__v0_rename_at_164_46((__j_rename_at_174_50 + 1),__context__) + 1));
                    __insertionCost_rename_at_176_52 = ((int32_t)(__v1_rename_at_166_47(__j_rename_at_174_50,__context__) + 1));
                    __substitutionCost_rename_at_177_53 = ((int32_t)(__v0_rename_at_164_46(__j_rename_at_174_50,__context__) + ((get_character_uat(__s_rename_at_161_42,__i_rename_at_172_49) == get_character_uat(__t_rename_at_161_43,__j_rename_at_174_50)) ? das_auto_cast<int32_t>::cast(0) : das_auto_cast<int32_t>::cast(1))));
                    das_copy(__v1_rename_at_166_47((__j_rename_at_174_50 + 1),__context__),SimPolicy<int32_t>::Min(__deletionCost_rename_at_175_51,__insertionCost_rename_at_176_52,*__context__,nullptr));
                    das_copy(__v1_rename_at_166_47((__j_rename_at_174_50 + 1),__context__),SimPolicy<int32_t>::Min(__v1_rename_at_166_47((__j_rename_at_174_50 + 1),__context__),__substitutionCost_rename_at_177_53,*__context__,nullptr));
                }
                __j_iterator.close(__context__,(__j_rename_at_174_50));
            };
            _FuncbuiltinTickswapTick6899974565646937647_4b12db88e6e6aa18(__context__,das_arg<TArray<int32_t>>::pass(__v0_rename_at_164_46),das_arg<TArray<int32_t>>::pass(__v1_rename_at_166_47));
        }
        __i_iterator.close(__context__,(__i_rename_at_172_49));
    };
    return das_auto_cast<int32_t>::cast(__v0_rename_at_164_46(__tLen_rename_at_163_45,__context__));
}

inline char * replace_multiple_d0cf6948c5333e25 ( Context * __context__, char * const  __source_rename_at_191_54, TArray<AutoTuple<char *,char *>> const  &  __replaces_rename_at_191_55 )
{
    return das_auto_cast<char *>::cast((builtin_empty(__source_rename_at_191_54) || _FuncbuiltinTickemptyTick15399874715904164783_17e819552d1fbeb4(__context__,__replaces_rename_at_191_55)) ? das_auto_cast<char * const >::cast(__source_rename_at_191_54) : das_auto_cast<char * const >::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_196_56) DAS_AOT_INLINE_LAMBDA -> void{
        int32_t __index_rename_at_197_57 = 0;
        int32_t __len_rename_at_198_58 = ((int32_t)builtin_string_length(__source_rename_at_191_54,__context__));
        while ( __index_rename_at_197_57 < __len_rename_at_198_58 )
        {
            bool __found_rename_at_200_59 = false;
            {
                bool __need_loop_201 = true;
                // replace: tuple<text:string;replacement:string> const&
                das_iterator<TArray<AutoTuple<char *,char *>> const > __replace_iterator(__replaces_rename_at_191_55);
                AutoTuple<char *,char *> const  * __replace_rename_at_201_60;
                __need_loop_201 = __replace_iterator.first(__context__,(__replace_rename_at_201_60)) && __need_loop_201;
                for ( ; __need_loop_201 ; __need_loop_201 = __replace_iterator.next(__context__,(__replace_rename_at_201_60)) )
                {
                    if ( builtin_string_startswith3(__source_rename_at_191_54,__index_rename_at_197_57,das_get_auto_tuple_field<char *,0,char *,char *>::get((*__replace_rename_at_201_60)),__context__) )
                    {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_196_56),cast<char * const >::from(das_get_auto_tuple_field<char *,1,char *,char *>::get((*__replace_rename_at_201_60)))));
                        __index_rename_at_197_57 += builtin_string_length(das_get_auto_tuple_field<char *,0,char *,char *>::get((*__replace_rename_at_201_60)),__context__);
                        das_copy(__found_rename_at_200_59,true);
                        break;
                    };
                }
                __replace_iterator.close(__context__,(__replace_rename_at_201_60));
            };
            if ( !__found_rename_at_200_59 )
            {
                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_196_56),get_character_uat(__source_rename_at_191_54,__index_rename_at_197_57));
                ++__index_rename_at_197_57;
            };
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}
static vec4f __wrap__FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__ ) {
    TArray<char *> &  arg_Arr = cast_aot_arg<TArray<char *> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8e7e2f2250c59e0a ( Context * __context__ ) {
    TArray<char *> &  arg_a = cast_aot_arg<TArray<char *> & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<TArray<char *> &>::from(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8e7e2f2250c59e0a(__context__, arg_a));
}
static vec4f __wrap__FuncbuiltinTickfinalizeTick13836114024949725080_5be9a99f60cdfbba ( Context * __context__ ) {
    TArray<TArray<int32_t>> &  arg_a = cast_aot_arg<TArray<TArray<int32_t>> & >::to(*__context__,__context__->abiArguments()[0]);
    _FuncbuiltinTickfinalizeTick13836114024949725080_5be9a99f60cdfbba(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8 ( Context * __context__ ) {
    TArray<int32_t> &  arg_a = cast_aot_arg<TArray<int32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickcloneTick3038771811667655495_4060192a3cb0948 ( Context * __context__ ) {
    TArray<char *> &  arg_a = cast_aot_arg<TArray<char *> & >::to(*__context__,__context__->abiArguments()[0]);
    TArray<char *> const  &  arg_b = cast_aot_arg<TArray<char *> const  & >::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickcloneTick3038771811667655495_4060192a3cb0948(__context__, arg_a, arg_b);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickresizeTick4811697762258667383_c1a73db481e59a8b ( Context * __context__ ) {
    TArray<TArray<int32_t>> &  arg_Arr = cast_aot_arg<TArray<TArray<int32_t>> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickresizeTick4811697762258667383_c1a73db481e59a8b(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__ ) {
    TArray<int32_t> &  arg_Arr = cast_aot_arg<TArray<int32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickswapTick6899974565646937647_4b12db88e6e6aa18 ( Context * __context__ ) {
    TArray<int32_t> &  arg_a = cast_aot_arg<TArray<int32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    TArray<int32_t> &  arg_b = cast_aot_arg<TArray<int32_t> & >::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickswapTick6899974565646937647_4b12db88e6e6aa18(__context__, arg_a, arg_b);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickemptyTick15399874715904164783_17e819552d1fbeb4 ( Context * __context__ ) {
    TArray<AutoTuple<char *,char *>> const  &  arg_a = cast_aot_arg<TArray<AutoTuple<char *,char *>> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncbuiltinTickemptyTick15399874715904164783_17e819552d1fbeb4(__context__, arg_a));
}
static vec4f __wrap_wide_53059f73cffda483 ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_width = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(wide_53059f73cffda483(__context__, arg_text, arg_width));
}
static vec4f __wrap_split_e671885aa89ee032 ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_delim = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    *((TArray<char *> *) __context__->abiCMRES) = split_e671885aa89ee032(__context__, arg_text, arg_delim);
    return v_zero();
}
static vec4f __wrap_split_by_chars_143571b759d8cb31 ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_delim = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    *((TArray<char *> *) __context__->abiCMRES) = split_by_chars_143571b759d8cb31(__context__, arg_text, arg_delim);
    return v_zero();
}
static vec4f __wrap_levenshtein_distance_549bdedc98dec005 ( Context * __context__ ) {
    char * const  arg_s = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_t = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(levenshtein_distance_549bdedc98dec005(__context__, arg_s, arg_t));
}
static vec4f __wrap_levenshtein_distance_fast_57b8adcc7543af10 ( Context * __context__ ) {
    char * const  arg_s = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_t = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(levenshtein_distance_fast_57b8adcc7543af10(__context__, arg_s, arg_t));
}
static vec4f __wrap_replace_multiple_d0cf6948c5333e25 ( Context * __context__ ) {
    char * const  arg_source = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    TArray<AutoTuple<char *,char *>> const  &  arg_replaces = cast_aot_arg<TArray<AutoTuple<char *,char *>> const  & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(replace_multiple_d0cf6948c5333e25(__context__, arg_source, arg_replaces));
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0x380b7455b0a2f9f5, false, (void*)&_FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1, &__wrap__FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 },
    { 0x1530dde33162bb3b, false, (void*)&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8e7e2f2250c59e0a, &__wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8e7e2f2250c59e0a },
    { 0x8cfab1c459867f92, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_5be9a99f60cdfbba, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_5be9a99f60cdfbba },
    { 0x439426c0e62e2f81, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8 },
    { 0x4dae975ff549d69b, false, (void*)&_FuncbuiltinTickcloneTick3038771811667655495_4060192a3cb0948, &__wrap__FuncbuiltinTickcloneTick3038771811667655495_4060192a3cb0948 },
    { 0x31e785d0d32596a0, false, (void*)&_FuncbuiltinTickresizeTick4811697762258667383_c1a73db481e59a8b, &__wrap__FuncbuiltinTickresizeTick4811697762258667383_c1a73db481e59a8b },
    { 0xd5b298fc293f15ad, false, (void*)&_FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51, &__wrap__FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 },
    { 0x983ece9be972c6d9, false, (void*)&_FuncbuiltinTickswapTick6899974565646937647_4b12db88e6e6aa18, &__wrap__FuncbuiltinTickswapTick6899974565646937647_4b12db88e6e6aa18 },
    { 0x93680ae9c38c532, false, (void*)&_FuncbuiltinTickemptyTick15399874715904164783_17e819552d1fbeb4, &__wrap__FuncbuiltinTickemptyTick15399874715904164783_17e819552d1fbeb4 },
    { 0x2cdd6a9efa3197df, false, (void*)&wide_53059f73cffda483, &__wrap_wide_53059f73cffda483 },
    { 0xf003acfe168cecd9, true, (void*)&split_e671885aa89ee032, &__wrap_split_e671885aa89ee032 },
    { 0x89518ecab6848141, true, (void*)&split_by_chars_143571b759d8cb31, &__wrap_split_by_chars_143571b759d8cb31 },
    { 0x4e334b2beef80f94, false, (void*)&levenshtein_distance_549bdedc98dec005, &__wrap_levenshtein_distance_549bdedc98dec005 },
    { 0x8ee88e6c572f5f7e, false, (void*)&levenshtein_distance_fast_57b8adcc7543af10, &__wrap_levenshtein_distance_fast_57b8adcc7543af10 },
    { 0x80367ae3d8465b8f, false, (void*)&replace_multiple_d0cf6948c5333e25, &__wrap_replace_multiple_d0cf6948c5333e25 },
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
