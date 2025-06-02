#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require base64

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
namespace _anon_8990523418378393095 {

// unused enumeration ConversionResult
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__c149136652fe1c57;
extern FuncInfo __func_info__920fb1be533a8af4;

VarInfo __func_info__920fb1be533a8af4_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, 24, UINT64_C(0xc149136652fe1c57), "inp", 0, 0 };
VarInfo * __func_info__920fb1be533a8af4_fields[1] =  { &__func_info__920fb1be533a8af4_field_0 };
FuncInfo __func_info__920fb1be533a8af4 = {"invoke block<(inp:array<uint8> const#):void> const", "", __func_info__920fb1be533a8af4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x920fb1be533a8af4), 0x0 };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline int32_t _Funcbase64Tickbase64_decodeTick6318730225425972510_f3ca55d5d6b47398 ( Context * __context__, TArray<uint8_t> const  & __inp_rename_at_77_2, TArray<uint8_t> & __out_rename_at_77_3 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> & __a_rename_at_1182_10 );
inline char * _Funcbase64Tickbase64_encodeTick16780245076805804220_3d161b0536e07d09 ( Context * __context__, TArray<uint8_t> const  & __inp_rename_at_132_11 );
inline int32_t BASE64_DECODE_OUT_SIZE_52c7be8c3e0ae854 ( Context * __context__, int32_t __s_rename_at_51_18 );
inline int32_t BASE64_ENCODE_OUT_SIZE_5f651cc9c2907b91 ( Context * __context__, int32_t __s_rename_at_55_19 );
inline AutoTuple<char *,int32_t> base64_decode_3d5513d573c262f1 ( Context * __context__, char * const  ___in_rename_at_59_20 );
inline int32_t base64_decode_2e5f373672d155a6 ( Context * __context__, char * const  ___in_rename_at_69_24, TArray<uint8_t> & __out_rename_at_69_25 );
inline char * base64_encode_80cb32b433fba65f ( Context * __context__, char * const  ___inp_rename_at_123_28 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0xa9c311aa11adec42>(__context__) = 61;/*BASE64_PAD*/
    das_global<int32_t,0xa4db727f6c6abd1f>(__context__) = 43;/*BASE64DE_FIRST*/
    das_global<int32_t,0x81824871513cd90e>(__context__) = 122;/*BASE64DE_LAST*/
    das_global<TDim<int32_t,64>,0x2def1b333a3160fa>(__context__) = (([&]() -> TDim<int32_t,64> {
        TDim<int32_t,64> __mka_16;
        __mka_16(0,__context__) = 65;
        __mka_16(1,__context__) = 66;
        __mka_16(2,__context__) = 67;
        __mka_16(3,__context__) = 68;
        __mka_16(4,__context__) = 69;
        __mka_16(5,__context__) = 70;
        __mka_16(6,__context__) = 71;
        __mka_16(7,__context__) = 72;
        __mka_16(8,__context__) = 73;
        __mka_16(9,__context__) = 74;
        __mka_16(10,__context__) = 75;
        __mka_16(11,__context__) = 76;
        __mka_16(12,__context__) = 77;
        __mka_16(13,__context__) = 78;
        __mka_16(14,__context__) = 79;
        __mka_16(15,__context__) = 80;
        __mka_16(16,__context__) = 81;
        __mka_16(17,__context__) = 82;
        __mka_16(18,__context__) = 83;
        __mka_16(19,__context__) = 84;
        __mka_16(20,__context__) = 85;
        __mka_16(21,__context__) = 86;
        __mka_16(22,__context__) = 87;
        __mka_16(23,__context__) = 88;
        __mka_16(24,__context__) = 89;
        __mka_16(25,__context__) = 90;
        __mka_16(26,__context__) = 97;
        __mka_16(27,__context__) = 98;
        __mka_16(28,__context__) = 99;
        __mka_16(29,__context__) = 100;
        __mka_16(30,__context__) = 101;
        __mka_16(31,__context__) = 102;
        __mka_16(32,__context__) = 103;
        __mka_16(33,__context__) = 104;
        __mka_16(34,__context__) = 105;
        __mka_16(35,__context__) = 106;
        __mka_16(36,__context__) = 107;
        __mka_16(37,__context__) = 108;
        __mka_16(38,__context__) = 109;
        __mka_16(39,__context__) = 110;
        __mka_16(40,__context__) = 111;
        __mka_16(41,__context__) = 112;
        __mka_16(42,__context__) = 113;
        __mka_16(43,__context__) = 114;
        __mka_16(44,__context__) = 115;
        __mka_16(45,__context__) = 116;
        __mka_16(46,__context__) = 117;
        __mka_16(47,__context__) = 118;
        __mka_16(48,__context__) = 119;
        __mka_16(49,__context__) = 120;
        __mka_16(50,__context__) = 121;
        __mka_16(51,__context__) = 122;
        __mka_16(52,__context__) = 48;
        __mka_16(53,__context__) = 49;
        __mka_16(54,__context__) = 50;
        __mka_16(55,__context__) = 51;
        __mka_16(56,__context__) = 52;
        __mka_16(57,__context__) = 53;
        __mka_16(58,__context__) = 54;
        __mka_16(59,__context__) = 55;
        __mka_16(60,__context__) = 56;
        __mka_16(61,__context__) = 57;
        __mka_16(62,__context__) = 43;
        __mka_16(63,__context__) = 47;
        return __mka_16;
    })());/*base64en*/
    das_global<TDim<int32_t,80>,0xd4e398d94efee1c9>(__context__) = (([&]() -> TDim<int32_t,80> {
        TDim<int32_t,80> __mka_28;
        __mka_28(0,__context__) = 62;
        __mka_28(1,__context__) = -1;
        __mka_28(2,__context__) = -1;
        __mka_28(3,__context__) = -1;
        __mka_28(4,__context__) = 63;
        __mka_28(5,__context__) = 52;
        __mka_28(6,__context__) = 53;
        __mka_28(7,__context__) = 54;
        __mka_28(8,__context__) = 55;
        __mka_28(9,__context__) = 56;
        __mka_28(10,__context__) = 57;
        __mka_28(11,__context__) = 58;
        __mka_28(12,__context__) = 59;
        __mka_28(13,__context__) = 60;
        __mka_28(14,__context__) = 61;
        __mka_28(15,__context__) = -1;
        __mka_28(16,__context__) = -1;
        __mka_28(17,__context__) = -1;
        __mka_28(18,__context__) = -1;
        __mka_28(19,__context__) = -1;
        __mka_28(20,__context__) = -1;
        __mka_28(21,__context__) = -1;
        __mka_28(22,__context__) = 0;
        __mka_28(23,__context__) = 1;
        __mka_28(24,__context__) = 2;
        __mka_28(25,__context__) = 3;
        __mka_28(26,__context__) = 4;
        __mka_28(27,__context__) = 5;
        __mka_28(28,__context__) = 6;
        __mka_28(29,__context__) = 7;
        __mka_28(30,__context__) = 8;
        __mka_28(31,__context__) = 9;
        __mka_28(32,__context__) = 10;
        __mka_28(33,__context__) = 11;
        __mka_28(34,__context__) = 12;
        __mka_28(35,__context__) = 13;
        __mka_28(36,__context__) = 14;
        __mka_28(37,__context__) = 15;
        __mka_28(38,__context__) = 16;
        __mka_28(39,__context__) = 17;
        __mka_28(40,__context__) = 18;
        __mka_28(41,__context__) = 19;
        __mka_28(42,__context__) = 20;
        __mka_28(43,__context__) = 21;
        __mka_28(44,__context__) = 22;
        __mka_28(45,__context__) = 23;
        __mka_28(46,__context__) = 24;
        __mka_28(47,__context__) = 25;
        __mka_28(48,__context__) = -1;
        __mka_28(49,__context__) = -1;
        __mka_28(50,__context__) = -1;
        __mka_28(51,__context__) = -1;
        __mka_28(52,__context__) = -1;
        __mka_28(53,__context__) = -1;
        __mka_28(54,__context__) = 26;
        __mka_28(55,__context__) = 27;
        __mka_28(56,__context__) = 28;
        __mka_28(57,__context__) = 29;
        __mka_28(58,__context__) = 30;
        __mka_28(59,__context__) = 31;
        __mka_28(60,__context__) = 32;
        __mka_28(61,__context__) = 33;
        __mka_28(62,__context__) = 34;
        __mka_28(63,__context__) = 35;
        __mka_28(64,__context__) = 36;
        __mka_28(65,__context__) = 37;
        __mka_28(66,__context__) = 38;
        __mka_28(67,__context__) = 39;
        __mka_28(68,__context__) = 40;
        __mka_28(69,__context__) = 41;
        __mka_28(70,__context__) = 42;
        __mka_28(71,__context__) = 43;
        __mka_28(72,__context__) = 44;
        __mka_28(73,__context__) = 45;
        __mka_28(74,__context__) = 46;
        __mka_28(75,__context__) = 47;
        __mka_28(76,__context__) = 48;
        __mka_28(77,__context__) = 49;
        __mka_28(78,__context__) = 50;
        __mka_28(79,__context__) = 51;
        return __mka_28;
    })());/*base64de*/
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _Funcbase64Tickbase64_decodeTick6318730225425972510_f3ca55d5d6b47398 ( Context * __context__, TArray<uint8_t> const  &  __inp_rename_at_77_2, TArray<uint8_t> &  __out_rename_at_77_3 )
{
    int32_t __inlen_rename_at_78_4 = ((int32_t)builtin_array_size(__inp_rename_at_77_2));
    int32_t __j_rename_at_79_5 = 0;
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(__out_rename_at_77_3),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176(__context__,das_arg<TArray<uint8_t>>::pass(__out_rename_at_77_3),BASE64_DECODE_OUT_SIZE_52c7be8c3e0ae854(__context__,__inlen_rename_at_78_4));
    {
        bool __need_loop_82 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(builtin_array_size(__inp_rename_at_77_2)));
        int32_t __i_rename_at_82_6;
        __need_loop_82 = __i_iterator.first(__context__,(__i_rename_at_82_6)) && __need_loop_82;
        for ( ; __need_loop_82 ; __need_loop_82 = __i_iterator.next(__context__,(__i_rename_at_82_6)) )
        {
            int32_t __c_rename_at_83_7 = 0;
            int32_t __s_rename_at_84_8 = (SimPolicy<int32_t>::Mod(__i_rename_at_82_6,4,*__context__,nullptr));
            int32_t __ii_rename_at_85_9 = int32_t(__inp_rename_at_77_2(__i_rename_at_82_6,__context__));
            if ( __ii_rename_at_85_9 == 61 )
            {
                break;
            } else {
                if ( (__ii_rename_at_85_9 < 43) || (__ii_rename_at_85_9 > 122) )
                {
                    return das_auto_cast<int32_t>::cast(-1);
                } else {
                    das_copy(__c_rename_at_83_7,das_global<TDim<int32_t,80>,0xd4e398d94efee1c9>(__context__) /*base64de*/((__ii_rename_at_85_9 - 43),__context__));
                    if ( __c_rename_at_83_7 == -1 )
                    {
                        return das_auto_cast<int32_t>::cast(-1);
                    } else {
                        if ( __s_rename_at_84_8 == 0 )
                        {
                            das_copy(__out_rename_at_77_3(__j_rename_at_79_5,__context__),uint8_t((uint32_t(__c_rename_at_83_7) << 0x2u) & 0xffu));
                            continue;
                        } else if ( __s_rename_at_84_8 == 1 )
                        {
                            das_copy(__out_rename_at_77_3(__j_rename_at_79_5,__context__),uint8_t(uint32_t(__out_rename_at_77_3(__j_rename_at_79_5,__context__)) + ((uint32_t(__c_rename_at_83_7) >> 0x4u) & 0x3u)));
                            ++__j_rename_at_79_5;
                            if ( (__i_rename_at_82_6 < (__inlen_rename_at_78_4 - 3)) || (int32_t(__inp_rename_at_77_2((__inlen_rename_at_78_4 - 2),__context__)) != 61) )
                            {
                                das_copy(__out_rename_at_77_3(__j_rename_at_79_5,__context__),uint8_t((uint32_t(__c_rename_at_83_7) & 0xfu) << 0x4u));
                            };
                            continue;
                        } else if ( __s_rename_at_84_8 == 2 )
                        {
                            das_copy(__out_rename_at_77_3(__j_rename_at_79_5,__context__),uint8_t(uint32_t(__out_rename_at_77_3(__j_rename_at_79_5,__context__)) + ((uint32_t(__c_rename_at_83_7) >> 0x2u) & 0xfu)));
                            ++__j_rename_at_79_5;
                            if ( (__i_rename_at_82_6 < (__inlen_rename_at_78_4 - 2)) || (int32_t(__inp_rename_at_77_2((__inlen_rename_at_78_4 - 1),__context__)) != 61) )
                            {
                                das_copy(__out_rename_at_77_3(__j_rename_at_79_5,__context__),uint8_t((uint32_t(__c_rename_at_83_7) & 0x3u) << 0x6u));
                            };
                            continue;
                        } else if ( __s_rename_at_84_8 == 3 )
                        {
                            das_copy(__out_rename_at_77_3(__j_rename_at_79_5,__context__),uint8_t(int32_t(__out_rename_at_77_3(__j_rename_at_79_5,__context__)) + __c_rename_at_83_7));
                            ++__j_rename_at_79_5;
                        };
                    };
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_82_6));
    };
    if ( __j_rename_at_79_5 != -1 )
    {
        _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176(__context__,das_arg<TArray<uint8_t>>::pass(__out_rename_at_77_3),__j_rename_at_79_5);
    };
    return das_auto_cast<int32_t>::cast(__j_rename_at_79_5);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1182_10 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1182_10),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _Funcbase64Tickbase64_encodeTick16780245076805804220_3d161b0536e07d09 ( Context * __context__, TArray<uint8_t> const  &  __inp_rename_at_132_11 )
{
    int32_t __inlen_rename_at_133_12 = builtin_array_size(__inp_rename_at_132_11);
    int32_t __j_rename_at_134_13 = 0;
    TArray<uint8_t> __out_rename_at_135_14;das_zero(__out_rename_at_135_14);
    _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176(__context__,das_arg<TArray<uint8_t>>::pass(__out_rename_at_135_14),BASE64_ENCODE_OUT_SIZE_5f651cc9c2907b91(__context__,__inlen_rename_at_133_12));
    {
        bool __need_loop_137 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__inlen_rename_at_133_12));
        int32_t __i_rename_at_137_15;
        __need_loop_137 = __i_iterator.first(__context__,(__i_rename_at_137_15)) && __need_loop_137;
        for ( ; __need_loop_137 ; __need_loop_137 = __i_iterator.next(__context__,(__i_rename_at_137_15)) )
        {
            int32_t __s_rename_at_138_16 = (SimPolicy<int32_t>::Mod(__i_rename_at_137_15,3,*__context__,nullptr));
            if ( __s_rename_at_138_16 == 0 )
            {
                das_copy(__out_rename_at_135_14(__j_rename_at_134_13++,__context__),uint8_t(das_global<TDim<int32_t,64>,0x2def1b333a3160fa>(__context__) /*base64en*/(((uint32_t(__inp_rename_at_132_11(__i_rename_at_137_15,__context__)) >> 0x2u) & 0x3fu),__context__)));
                continue;
            } else if ( __s_rename_at_138_16 == 1 )
            {
                das_copy(__out_rename_at_135_14(__j_rename_at_134_13++,__context__),uint8_t(das_global<TDim<int32_t,64>,0x2def1b333a3160fa>(__context__) /*base64en*/((((uint32_t(__inp_rename_at_132_11((__i_rename_at_137_15 - 1),__context__)) & 0x3u) << 0x4u) + ((uint32_t(__inp_rename_at_132_11(__i_rename_at_137_15,__context__)) >> 0x4u) & 0xfu)),__context__)));
                continue;
            } else if ( __s_rename_at_138_16 == 2 )
            {
                das_copy(__out_rename_at_135_14(__j_rename_at_134_13++,__context__),uint8_t(das_global<TDim<int32_t,64>,0x2def1b333a3160fa>(__context__) /*base64en*/((((uint32_t(__inp_rename_at_132_11((__i_rename_at_137_15 - 1),__context__)) & 0xfu) << 0x2u) + ((uint32_t(__inp_rename_at_132_11(__i_rename_at_137_15,__context__)) >> 0x6u) & 0x3u)),__context__)));
                das_copy(__out_rename_at_135_14(__j_rename_at_134_13++,__context__),uint8_t(das_global<TDim<int32_t,64>,0x2def1b333a3160fa>(__context__) /*base64en*/((uint32_t(__inp_rename_at_132_11(__i_rename_at_137_15,__context__)) & 0x3fu),__context__)));
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_137_15));
    };
    int32_t __i_rename_at_151_17 = (__inlen_rename_at_133_12 - 1);
    if ( (SimPolicy<int32_t>::Mod(__i_rename_at_151_17,3,*__context__,nullptr)) == 0 )
    {
        das_copy(__out_rename_at_135_14(__j_rename_at_134_13++,__context__),uint8_t(das_global<TDim<int32_t,64>,0x2def1b333a3160fa>(__context__) /*base64en*/(((uint32_t(__inp_rename_at_132_11(__i_rename_at_151_17,__context__)) & 0x3u) << 0x4u),__context__)));
        das_copy(__out_rename_at_135_14(__j_rename_at_134_13++,__context__),0x3d);
        das_copy(__out_rename_at_135_14(__j_rename_at_134_13++,__context__),0x3d);
    } else if ( (SimPolicy<int32_t>::Mod(__i_rename_at_151_17,3,*__context__,nullptr)) == 1 )
    {
        das_copy(__out_rename_at_135_14(__j_rename_at_134_13++,__context__),uint8_t(das_global<TDim<int32_t,64>,0x2def1b333a3160fa>(__context__) /*base64en*/(((uint32_t(__inp_rename_at_132_11(__i_rename_at_151_17,__context__)) & 0xfu) << 0x2u),__context__)));
        das_copy(__out_rename_at_135_14(__j_rename_at_134_13++,__context__),0x3d);
    };
    return das_auto_cast<char *>::cast(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__out_rename_at_135_14),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline int32_t BASE64_DECODE_OUT_SIZE_52c7be8c3e0ae854 ( Context * __context__, int32_t __s_rename_at_51_18 )
{
    return das_auto_cast<int32_t>::cast((SimPolicy<int32_t>::Div(__s_rename_at_51_18,4,*__context__,nullptr)) * 3);
}

inline int32_t BASE64_ENCODE_OUT_SIZE_5f651cc9c2907b91 ( Context * __context__, int32_t __s_rename_at_55_19 )
{
    return das_auto_cast<int32_t>::cast((SimPolicy<int32_t>::Div((__s_rename_at_55_19 + 2),3,*__context__,nullptr)) * 4);
}

inline AutoTuple<char *,int32_t> base64_decode_3d5513d573c262f1 ( Context * __context__, char * const  ___in_rename_at_59_20 ) { das_stack_prologue __prologue(__context__,160,"base64_decode " DAS_FILE_LINE);
{
    TArray<uint8_t> __out_rename_at_61_21; memset((void*)&__out_rename_at_61_21,0,sizeof(__out_rename_at_61_21));
    int32_t __outlen_rename_at_62_22; memset((void*)&__outlen_rename_at_62_22,0,sizeof(__outlen_rename_at_62_22));
    /* finally */ auto __finally_59= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(__out_rename_at_61_21));
    /* end finally */ });
    das_zero(__out_rename_at_61_21);
    __outlen_rename_at_62_22 = 0;
    builtin_string_peek(___in_rename_at_59_20,das_make_block<void,TArray<uint8_t> const  &>(__context__,128,0,&__func_info__920fb1be533a8af4,[&](TArray<uint8_t> const  & __inp_rename_at_63_23) -> void{
        das_copy(__outlen_rename_at_62_22,_Funcbase64Tickbase64_decodeTick6318730225425972510_f3ca55d5d6b47398(__context__,__inp_rename_at_63_23,das_arg<TArray<uint8_t>>::pass(__out_rename_at_61_21)));
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast_ref<AutoTuple<char *,int32_t>>::cast((([&]() -> AutoTuple<char *,int32_t> {
        AutoTuple<char *,int32_t> __mkt_66;
        das_get_auto_tuple_field<char *,0,char *,int32_t>::get(__mkt_66) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__out_rename_at_61_21),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
        das_get_auto_tuple_field<int32_t,1,char *,int32_t>::get(__mkt_66) = __outlen_rename_at_62_22;
        return __mkt_66;
    })()));
}}

inline int32_t base64_decode_2e5f373672d155a6 ( Context * __context__, char * const  ___in_rename_at_69_24, TArray<uint8_t> &  __out_rename_at_69_25 ) { das_stack_prologue __prologue(__context__,112,"base64_decode " DAS_FILE_LINE);
{
    int32_t __outlen_rename_at_70_26 = 0;
    builtin_string_peek(___in_rename_at_69_24,das_make_block<void,TArray<uint8_t> const  &>(__context__,96,0,&__func_info__920fb1be533a8af4,[&](TArray<uint8_t> const  & __inp_rename_at_71_27) -> void{
        das_copy(__outlen_rename_at_70_26,_Funcbase64Tickbase64_decodeTick6318730225425972510_f3ca55d5d6b47398(__context__,__inp_rename_at_71_27,das_arg<TArray<uint8_t>>::pass(__out_rename_at_69_25)));
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast<int32_t>::cast(__outlen_rename_at_70_26);
}}

inline char * base64_encode_80cb32b433fba65f ( Context * __context__, char * const  ___inp_rename_at_123_28 ) { das_stack_prologue __prologue(__context__,112,"base64_encode " DAS_FILE_LINE);
{
    char * __res_rename_at_125_29 = 0;
    builtin_string_peek(___inp_rename_at_123_28,das_make_block<void,TArray<uint8_t> const  &>(__context__,96,0,&__func_info__920fb1be533a8af4,[&](TArray<uint8_t> const  & __inp_rename_at_126_30) -> void{
        das_copy(__res_rename_at_125_29,_Funcbase64Tickbase64_encodeTick16780245076805804220_3d161b0536e07d09(__context__,__inp_rename_at_126_30));
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast<char *>::cast(__res_rename_at_125_29);
}}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xc502128bbf7c5d21] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176>>();
    };
    aotLib[0x9f60f64d62951eb2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcbase64Tickbase64_decodeTick6318730225425972510_f3ca55d5d6b47398>>();
    };
    aotLib[0x1f8eb5798352d074] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb>>();
    };
    aotLib[0x8f0063f632262254] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcbase64Tickbase64_encodeTick16780245076805804220_3d161b0536e07d09>>();
    };
    aotLib[0x6e4d312664a2e0a5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&BASE64_DECODE_OUT_SIZE_52c7be8c3e0ae854>>();
    };
    aotLib[0xe75e6fa2c152211f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&BASE64_ENCODE_OUT_SIZE_5f651cc9c2907b91>>();
    };
    aotLib[0x662428414d9da582] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&base64_decode_3d5513d573c262f1>>();
    };
    aotLib[0x1124ea3fff3a8fd0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&base64_decode_2e5f373672d155a6>>();
    };
    aotLib[0x1690e9f77ac4d037] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&base64_encode_80cb32b433fba65f>>();
    };
    // [[ init script ]]
    aotLib[0x74933ef6987c7340] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_8990523418378393095
AotListBase impl_aot_base64(_anon_8990523418378393095::registerAotFunctions);
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
