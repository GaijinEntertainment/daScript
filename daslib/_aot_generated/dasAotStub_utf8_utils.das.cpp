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
 // require utf8_utils

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
namespace _anon_2135218099027497698 {

// unused enumeration ConversionResult
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__9b3fbfdbc965601f;
extern TypeInfo __type_info__b208477a53cb453e;
extern TypeInfo __type_info__af63e84c860200f0;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;

VarInfo __func_info__5f4b6da70ca496e2_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, 24, UINT64_C(0x1f33941668547bfd), "arr", 0, 0 };
VarInfo * __func_info__5f4b6da70ca496e2_fields[1] =  { &__func_info__5f4b6da70ca496e2_field_0 };
FuncInfo __func_info__5f4b6da70ca496e2 = {"invoke block<(arr:array<uint8> const#):void> const", "", __func_info__5f4b6da70ca496e2_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x5f4b6da70ca496e2), 0x0 };
VarInfo __func_info__2f76c7f7dd4f0b27_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, 24, UINT64_C(0x3aefdf5fd38bc810), "p", 0, 0 };
VarInfo * __func_info__2f76c7f7dd4f0b27_fields[1] =  { &__func_info__2f76c7f7dd4f0b27_field_0 };
FuncInfo __func_info__2f76c7f7dd4f0b27 = {"invoke block<(p:array<uint8> const#):void> const", "", __func_info__2f76c7f7dd4f0b27_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x2f76c7f7dd4f0b27), 0x0 };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__9b3fbfdbc965601f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x9b3fbfdbc965601f) };
TypeInfo __type_info__b208477a53cb453e = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63e84c860200f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xb208477a53cb453e) };
TypeInfo __type_info__af63e84c860200f0 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e84c860200f0) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[2] = { &__type_info__9b3fbfdbc965601f, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[2] = { &__type_info__b208477a53cb453e, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_2[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_424e5a5af176be26 ( Context * __context__, TDim<uint8_t,2> const  & __a_rename_at_598_0 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_2d5894d58d05f52 ( Context * __context__, TDim<uint8_t,3> const  & __a_rename_at_598_1 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_5e3beb551331dc06 ( Context * __context__, TDim<uint8_t,4> const  & __a_rename_at_598_2 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_1e99f990a58976cb ( Context * __context__, TDim<uint8_t,1> const  & __a_rename_at_598_3 );
inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_8a2a57662f1d46d ( Context * __context__, TDim<uint8_t,2> & __a_rename_at_1523_4 );
inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_7d924998213ec597 ( Context * __context__, TDim<uint8_t,3> & __a_rename_at_1523_6 );
inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_aea3d34625ab51b4 ( Context * __context__, TDim<uint8_t,4> & __a_rename_at_1523_8 );
inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_ad51b48331aa2869 ( Context * __context__, TDim<uint8_t,1> & __a_rename_at_1523_10 );
inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d ( Context * __context__, TArray<uint8_t> & __a_rename_at_50_12 );
inline TArray<uint32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8494a8b6b6480f2 ( Context * __context__, TArray<uint32_t> & __a_rename_at_50_13 );
inline void _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_177_14, uint8_t __value_rename_at_177_15 );
inline void _FuncbuiltinTickreserveTick3994685146752941225_5d874a62d9325833 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_137_16, int32_t __newSize_rename_at_137_17 );
inline void _FuncbuiltinTickpushTick10769833213962245646_a51cd706faf9e203 ( Context * __context__, TArray<uint32_t> & __Arr_rename_at_193_18, uint32_t __value_rename_at_193_19 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_20, int32_t __newSize_rename_at_68_21 );
inline uint32_t utf16_to_utf32_49f3f9f3ecf44a4a ( Context * __context__, uint32_t __high_rename_at_15_22, uint32_t __low_rename_at_15_23 );
inline void utf8_encode_af85f8b19afd18de ( Context * __context__, TArray<uint8_t> & __dest_array_rename_at_24_24, uint32_t __ch_rename_at_24_25 );
inline TArray<uint8_t> utf8_encode_b7d52a486d026a93 ( Context * __context__, uint32_t __ch_rename_at_44_26 );
inline void utf8_encode_8e4897038ba5ca3b ( Context * __context__, TArray<uint8_t> & __dest_array_rename_at_67_27, TArray<uint32_t> const  & __source_utf32_string_rename_at_67_28 );
inline TArray<uint8_t> utf8_encode_de3cdc7453a168bb ( Context * __context__, TArray<uint32_t> const  & __source_utf32_string_rename_at_75_30 );
inline int32_t utf8_length_5ae766430c8d22e1 ( Context * __context__, TArray<uint8_t> const  & __utf8_string_rename_at_86_33 );
inline int32_t utf8_length_e8e8058edf449d85 ( Context * __context__, char * const  __utf8_string_rename_at_98_36 );
inline bool is_first_byte_of_utf8_char_ec6547e881783261 ( Context * __context__, uint8_t __ch_rename_at_110_39 );
inline bool contains_utf8_bom_190bd0f92360f1e8 ( Context * __context__, TArray<uint8_t> const  & __utf8_string_rename_at_117_41 );
inline bool contains_utf8_bom_1dac111ec6d7fb8f ( Context * __context__, char * const  __utf8_string_rename_at_124_42 );
inline bool is_utf8_string_valid_1a64d709eddf87fd ( Context * __context__, TArray<uint8_t> const  & __utf8_string_rename_at_162_43 );
inline bool is_utf8_string_valid_fb0b28f1946e058c ( Context * __context__, char * const  __utf8_string_rename_at_176_49 );
inline void utf8_decode_4dcab8dbe3c80d3d ( Context * __context__, TArray<uint32_t> & __dest_utf32_string_rename_at_190_55, TArray<uint8_t> const  & __source_utf8_string_rename_at_190_56 );
inline TArray<uint32_t> utf8_decode_67bf22c3da8b41ed ( Context * __context__, TArray<uint8_t> const  & __source_utf8_string_rename_at_209_63 );
inline TArray<uint32_t> utf8_decode_f7fbdc235a8d69f7 ( Context * __context__, char * const  __source_utf8_string_rename_at_217_65 );
inline void utf8_decode_d3d9f41f1665ca29 ( Context * __context__, TArray<uint32_t> & __dest_utf32_string_rename_at_227_68, char * const  __source_utf8_string_rename_at_227_69 );
inline char * decode_unicode_escape_bdbb72c622da2400 ( Context * __context__, char * const  __str_rename_at_234_71 );

void __init_script ( Context * __context__, bool __init_shared )
{
    if ( __init_shared ) das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) = (([&]() -> TDim<uint32_t,364> {
        TDim<uint32_t,364> __mka_139;
        __mka_139(0,__context__) = 0x0u;
        __mka_139(1,__context__) = 0x0u;
        __mka_139(2,__context__) = 0x0u;
        __mka_139(3,__context__) = 0x0u;
        __mka_139(4,__context__) = 0x0u;
        __mka_139(5,__context__) = 0x0u;
        __mka_139(6,__context__) = 0x0u;
        __mka_139(7,__context__) = 0x0u;
        __mka_139(8,__context__) = 0x0u;
        __mka_139(9,__context__) = 0x0u;
        __mka_139(10,__context__) = 0x0u;
        __mka_139(11,__context__) = 0x0u;
        __mka_139(12,__context__) = 0x0u;
        __mka_139(13,__context__) = 0x0u;
        __mka_139(14,__context__) = 0x0u;
        __mka_139(15,__context__) = 0x0u;
        __mka_139(16,__context__) = 0x0u;
        __mka_139(17,__context__) = 0x0u;
        __mka_139(18,__context__) = 0x0u;
        __mka_139(19,__context__) = 0x0u;
        __mka_139(20,__context__) = 0x0u;
        __mka_139(21,__context__) = 0x0u;
        __mka_139(22,__context__) = 0x0u;
        __mka_139(23,__context__) = 0x0u;
        __mka_139(24,__context__) = 0x0u;
        __mka_139(25,__context__) = 0x0u;
        __mka_139(26,__context__) = 0x0u;
        __mka_139(27,__context__) = 0x0u;
        __mka_139(28,__context__) = 0x0u;
        __mka_139(29,__context__) = 0x0u;
        __mka_139(30,__context__) = 0x0u;
        __mka_139(31,__context__) = 0x0u;
        __mka_139(32,__context__) = 0x0u;
        __mka_139(33,__context__) = 0x0u;
        __mka_139(34,__context__) = 0x0u;
        __mka_139(35,__context__) = 0x0u;
        __mka_139(36,__context__) = 0x0u;
        __mka_139(37,__context__) = 0x0u;
        __mka_139(38,__context__) = 0x0u;
        __mka_139(39,__context__) = 0x0u;
        __mka_139(40,__context__) = 0x0u;
        __mka_139(41,__context__) = 0x0u;
        __mka_139(42,__context__) = 0x0u;
        __mka_139(43,__context__) = 0x0u;
        __mka_139(44,__context__) = 0x0u;
        __mka_139(45,__context__) = 0x0u;
        __mka_139(46,__context__) = 0x0u;
        __mka_139(47,__context__) = 0x0u;
        __mka_139(48,__context__) = 0x0u;
        __mka_139(49,__context__) = 0x0u;
        __mka_139(50,__context__) = 0x0u;
        __mka_139(51,__context__) = 0x0u;
        __mka_139(52,__context__) = 0x0u;
        __mka_139(53,__context__) = 0x0u;
        __mka_139(54,__context__) = 0x0u;
        __mka_139(55,__context__) = 0x0u;
        __mka_139(56,__context__) = 0x0u;
        __mka_139(57,__context__) = 0x0u;
        __mka_139(58,__context__) = 0x0u;
        __mka_139(59,__context__) = 0x0u;
        __mka_139(60,__context__) = 0x0u;
        __mka_139(61,__context__) = 0x0u;
        __mka_139(62,__context__) = 0x0u;
        __mka_139(63,__context__) = 0x0u;
        __mka_139(64,__context__) = 0x0u;
        __mka_139(65,__context__) = 0x0u;
        __mka_139(66,__context__) = 0x0u;
        __mka_139(67,__context__) = 0x0u;
        __mka_139(68,__context__) = 0x0u;
        __mka_139(69,__context__) = 0x0u;
        __mka_139(70,__context__) = 0x0u;
        __mka_139(71,__context__) = 0x0u;
        __mka_139(72,__context__) = 0x0u;
        __mka_139(73,__context__) = 0x0u;
        __mka_139(74,__context__) = 0x0u;
        __mka_139(75,__context__) = 0x0u;
        __mka_139(76,__context__) = 0x0u;
        __mka_139(77,__context__) = 0x0u;
        __mka_139(78,__context__) = 0x0u;
        __mka_139(79,__context__) = 0x0u;
        __mka_139(80,__context__) = 0x0u;
        __mka_139(81,__context__) = 0x0u;
        __mka_139(82,__context__) = 0x0u;
        __mka_139(83,__context__) = 0x0u;
        __mka_139(84,__context__) = 0x0u;
        __mka_139(85,__context__) = 0x0u;
        __mka_139(86,__context__) = 0x0u;
        __mka_139(87,__context__) = 0x0u;
        __mka_139(88,__context__) = 0x0u;
        __mka_139(89,__context__) = 0x0u;
        __mka_139(90,__context__) = 0x0u;
        __mka_139(91,__context__) = 0x0u;
        __mka_139(92,__context__) = 0x0u;
        __mka_139(93,__context__) = 0x0u;
        __mka_139(94,__context__) = 0x0u;
        __mka_139(95,__context__) = 0x0u;
        __mka_139(96,__context__) = 0x0u;
        __mka_139(97,__context__) = 0x0u;
        __mka_139(98,__context__) = 0x0u;
        __mka_139(99,__context__) = 0x0u;
        __mka_139(100,__context__) = 0x0u;
        __mka_139(101,__context__) = 0x0u;
        __mka_139(102,__context__) = 0x0u;
        __mka_139(103,__context__) = 0x0u;
        __mka_139(104,__context__) = 0x0u;
        __mka_139(105,__context__) = 0x0u;
        __mka_139(106,__context__) = 0x0u;
        __mka_139(107,__context__) = 0x0u;
        __mka_139(108,__context__) = 0x0u;
        __mka_139(109,__context__) = 0x0u;
        __mka_139(110,__context__) = 0x0u;
        __mka_139(111,__context__) = 0x0u;
        __mka_139(112,__context__) = 0x0u;
        __mka_139(113,__context__) = 0x0u;
        __mka_139(114,__context__) = 0x0u;
        __mka_139(115,__context__) = 0x0u;
        __mka_139(116,__context__) = 0x0u;
        __mka_139(117,__context__) = 0x0u;
        __mka_139(118,__context__) = 0x0u;
        __mka_139(119,__context__) = 0x0u;
        __mka_139(120,__context__) = 0x0u;
        __mka_139(121,__context__) = 0x0u;
        __mka_139(122,__context__) = 0x0u;
        __mka_139(123,__context__) = 0x0u;
        __mka_139(124,__context__) = 0x0u;
        __mka_139(125,__context__) = 0x0u;
        __mka_139(126,__context__) = 0x0u;
        __mka_139(127,__context__) = 0x0u;
        __mka_139(128,__context__) = 0x1u;
        __mka_139(129,__context__) = 0x1u;
        __mka_139(130,__context__) = 0x1u;
        __mka_139(131,__context__) = 0x1u;
        __mka_139(132,__context__) = 0x1u;
        __mka_139(133,__context__) = 0x1u;
        __mka_139(134,__context__) = 0x1u;
        __mka_139(135,__context__) = 0x1u;
        __mka_139(136,__context__) = 0x1u;
        __mka_139(137,__context__) = 0x1u;
        __mka_139(138,__context__) = 0x1u;
        __mka_139(139,__context__) = 0x1u;
        __mka_139(140,__context__) = 0x1u;
        __mka_139(141,__context__) = 0x1u;
        __mka_139(142,__context__) = 0x1u;
        __mka_139(143,__context__) = 0x1u;
        __mka_139(144,__context__) = 0x9u;
        __mka_139(145,__context__) = 0x9u;
        __mka_139(146,__context__) = 0x9u;
        __mka_139(147,__context__) = 0x9u;
        __mka_139(148,__context__) = 0x9u;
        __mka_139(149,__context__) = 0x9u;
        __mka_139(150,__context__) = 0x9u;
        __mka_139(151,__context__) = 0x9u;
        __mka_139(152,__context__) = 0x9u;
        __mka_139(153,__context__) = 0x9u;
        __mka_139(154,__context__) = 0x9u;
        __mka_139(155,__context__) = 0x9u;
        __mka_139(156,__context__) = 0x9u;
        __mka_139(157,__context__) = 0x9u;
        __mka_139(158,__context__) = 0x9u;
        __mka_139(159,__context__) = 0x9u;
        __mka_139(160,__context__) = 0x7u;
        __mka_139(161,__context__) = 0x7u;
        __mka_139(162,__context__) = 0x7u;
        __mka_139(163,__context__) = 0x7u;
        __mka_139(164,__context__) = 0x7u;
        __mka_139(165,__context__) = 0x7u;
        __mka_139(166,__context__) = 0x7u;
        __mka_139(167,__context__) = 0x7u;
        __mka_139(168,__context__) = 0x7u;
        __mka_139(169,__context__) = 0x7u;
        __mka_139(170,__context__) = 0x7u;
        __mka_139(171,__context__) = 0x7u;
        __mka_139(172,__context__) = 0x7u;
        __mka_139(173,__context__) = 0x7u;
        __mka_139(174,__context__) = 0x7u;
        __mka_139(175,__context__) = 0x7u;
        __mka_139(176,__context__) = 0x7u;
        __mka_139(177,__context__) = 0x7u;
        __mka_139(178,__context__) = 0x7u;
        __mka_139(179,__context__) = 0x7u;
        __mka_139(180,__context__) = 0x7u;
        __mka_139(181,__context__) = 0x7u;
        __mka_139(182,__context__) = 0x7u;
        __mka_139(183,__context__) = 0x7u;
        __mka_139(184,__context__) = 0x7u;
        __mka_139(185,__context__) = 0x7u;
        __mka_139(186,__context__) = 0x7u;
        __mka_139(187,__context__) = 0x7u;
        __mka_139(188,__context__) = 0x7u;
        __mka_139(189,__context__) = 0x7u;
        __mka_139(190,__context__) = 0x7u;
        __mka_139(191,__context__) = 0x7u;
        __mka_139(192,__context__) = 0x8u;
        __mka_139(193,__context__) = 0x8u;
        __mka_139(194,__context__) = 0x2u;
        __mka_139(195,__context__) = 0x2u;
        __mka_139(196,__context__) = 0x2u;
        __mka_139(197,__context__) = 0x2u;
        __mka_139(198,__context__) = 0x2u;
        __mka_139(199,__context__) = 0x2u;
        __mka_139(200,__context__) = 0x2u;
        __mka_139(201,__context__) = 0x2u;
        __mka_139(202,__context__) = 0x2u;
        __mka_139(203,__context__) = 0x2u;
        __mka_139(204,__context__) = 0x2u;
        __mka_139(205,__context__) = 0x2u;
        __mka_139(206,__context__) = 0x2u;
        __mka_139(207,__context__) = 0x2u;
        __mka_139(208,__context__) = 0x2u;
        __mka_139(209,__context__) = 0x2u;
        __mka_139(210,__context__) = 0x2u;
        __mka_139(211,__context__) = 0x2u;
        __mka_139(212,__context__) = 0x2u;
        __mka_139(213,__context__) = 0x2u;
        __mka_139(214,__context__) = 0x2u;
        __mka_139(215,__context__) = 0x2u;
        __mka_139(216,__context__) = 0x2u;
        __mka_139(217,__context__) = 0x2u;
        __mka_139(218,__context__) = 0x2u;
        __mka_139(219,__context__) = 0x2u;
        __mka_139(220,__context__) = 0x2u;
        __mka_139(221,__context__) = 0x2u;
        __mka_139(222,__context__) = 0x2u;
        __mka_139(223,__context__) = 0x2u;
        __mka_139(224,__context__) = 0xau;
        __mka_139(225,__context__) = 0x3u;
        __mka_139(226,__context__) = 0x3u;
        __mka_139(227,__context__) = 0x3u;
        __mka_139(228,__context__) = 0x3u;
        __mka_139(229,__context__) = 0x3u;
        __mka_139(230,__context__) = 0x3u;
        __mka_139(231,__context__) = 0x3u;
        __mka_139(232,__context__) = 0x3u;
        __mka_139(233,__context__) = 0x3u;
        __mka_139(234,__context__) = 0x3u;
        __mka_139(235,__context__) = 0x3u;
        __mka_139(236,__context__) = 0x3u;
        __mka_139(237,__context__) = 0x4u;
        __mka_139(238,__context__) = 0x3u;
        __mka_139(239,__context__) = 0x3u;
        __mka_139(240,__context__) = 0xbu;
        __mka_139(241,__context__) = 0x6u;
        __mka_139(242,__context__) = 0x6u;
        __mka_139(243,__context__) = 0x6u;
        __mka_139(244,__context__) = 0x5u;
        __mka_139(245,__context__) = 0x8u;
        __mka_139(246,__context__) = 0x8u;
        __mka_139(247,__context__) = 0x8u;
        __mka_139(248,__context__) = 0x8u;
        __mka_139(249,__context__) = 0x8u;
        __mka_139(250,__context__) = 0x8u;
        __mka_139(251,__context__) = 0x8u;
        __mka_139(252,__context__) = 0x8u;
        __mka_139(253,__context__) = 0x8u;
        __mka_139(254,__context__) = 0x8u;
        __mka_139(255,__context__) = 0x8u;
        __mka_139(256,__context__) = 0x0u;
        __mka_139(257,__context__) = 0xcu;
        __mka_139(258,__context__) = 0x18u;
        __mka_139(259,__context__) = 0x24u;
        __mka_139(260,__context__) = 0x3cu;
        __mka_139(261,__context__) = 0x60u;
        __mka_139(262,__context__) = 0x54u;
        __mka_139(263,__context__) = 0xcu;
        __mka_139(264,__context__) = 0xcu;
        __mka_139(265,__context__) = 0xcu;
        __mka_139(266,__context__) = 0x30u;
        __mka_139(267,__context__) = 0x48u;
        __mka_139(268,__context__) = 0xcu;
        __mka_139(269,__context__) = 0xcu;
        __mka_139(270,__context__) = 0xcu;
        __mka_139(271,__context__) = 0xcu;
        __mka_139(272,__context__) = 0xcu;
        __mka_139(273,__context__) = 0xcu;
        __mka_139(274,__context__) = 0xcu;
        __mka_139(275,__context__) = 0xcu;
        __mka_139(276,__context__) = 0xcu;
        __mka_139(277,__context__) = 0xcu;
        __mka_139(278,__context__) = 0xcu;
        __mka_139(279,__context__) = 0xcu;
        __mka_139(280,__context__) = 0xcu;
        __mka_139(281,__context__) = 0x0u;
        __mka_139(282,__context__) = 0xcu;
        __mka_139(283,__context__) = 0xcu;
        __mka_139(284,__context__) = 0xcu;
        __mka_139(285,__context__) = 0xcu;
        __mka_139(286,__context__) = 0xcu;
        __mka_139(287,__context__) = 0x0u;
        __mka_139(288,__context__) = 0xcu;
        __mka_139(289,__context__) = 0x0u;
        __mka_139(290,__context__) = 0xcu;
        __mka_139(291,__context__) = 0xcu;
        __mka_139(292,__context__) = 0xcu;
        __mka_139(293,__context__) = 0x18u;
        __mka_139(294,__context__) = 0xcu;
        __mka_139(295,__context__) = 0xcu;
        __mka_139(296,__context__) = 0xcu;
        __mka_139(297,__context__) = 0xcu;
        __mka_139(298,__context__) = 0xcu;
        __mka_139(299,__context__) = 0x18u;
        __mka_139(300,__context__) = 0xcu;
        __mka_139(301,__context__) = 0x18u;
        __mka_139(302,__context__) = 0xcu;
        __mka_139(303,__context__) = 0xcu;
        __mka_139(304,__context__) = 0xcu;
        __mka_139(305,__context__) = 0xcu;
        __mka_139(306,__context__) = 0xcu;
        __mka_139(307,__context__) = 0xcu;
        __mka_139(308,__context__) = 0xcu;
        __mka_139(309,__context__) = 0xcu;
        __mka_139(310,__context__) = 0xcu;
        __mka_139(311,__context__) = 0x18u;
        __mka_139(312,__context__) = 0xcu;
        __mka_139(313,__context__) = 0xcu;
        __mka_139(314,__context__) = 0xcu;
        __mka_139(315,__context__) = 0xcu;
        __mka_139(316,__context__) = 0xcu;
        __mka_139(317,__context__) = 0x18u;
        __mka_139(318,__context__) = 0xcu;
        __mka_139(319,__context__) = 0xcu;
        __mka_139(320,__context__) = 0xcu;
        __mka_139(321,__context__) = 0xcu;
        __mka_139(322,__context__) = 0xcu;
        __mka_139(323,__context__) = 0xcu;
        __mka_139(324,__context__) = 0xcu;
        __mka_139(325,__context__) = 0x18u;
        __mka_139(326,__context__) = 0xcu;
        __mka_139(327,__context__) = 0xcu;
        __mka_139(328,__context__) = 0xcu;
        __mka_139(329,__context__) = 0xcu;
        __mka_139(330,__context__) = 0xcu;
        __mka_139(331,__context__) = 0xcu;
        __mka_139(332,__context__) = 0xcu;
        __mka_139(333,__context__) = 0xcu;
        __mka_139(334,__context__) = 0xcu;
        __mka_139(335,__context__) = 0x24u;
        __mka_139(336,__context__) = 0xcu;
        __mka_139(337,__context__) = 0x24u;
        __mka_139(338,__context__) = 0xcu;
        __mka_139(339,__context__) = 0xcu;
        __mka_139(340,__context__) = 0xcu;
        __mka_139(341,__context__) = 0x24u;
        __mka_139(342,__context__) = 0xcu;
        __mka_139(343,__context__) = 0xcu;
        __mka_139(344,__context__) = 0xcu;
        __mka_139(345,__context__) = 0xcu;
        __mka_139(346,__context__) = 0xcu;
        __mka_139(347,__context__) = 0x24u;
        __mka_139(348,__context__) = 0xcu;
        __mka_139(349,__context__) = 0x24u;
        __mka_139(350,__context__) = 0xcu;
        __mka_139(351,__context__) = 0xcu;
        __mka_139(352,__context__) = 0xcu;
        __mka_139(353,__context__) = 0x24u;
        __mka_139(354,__context__) = 0xcu;
        __mka_139(355,__context__) = 0xcu;
        __mka_139(356,__context__) = 0xcu;
        __mka_139(357,__context__) = 0xcu;
        __mka_139(358,__context__) = 0xcu;
        __mka_139(359,__context__) = 0xcu;
        __mka_139(360,__context__) = 0xcu;
        __mka_139(361,__context__) = 0xcu;
        __mka_139(362,__context__) = 0xcu;
        __mka_139(363,__context__) = 0xcu;
        return __mka_139;
    })());/*s_utf8d*/
    das_global<uint32_t,0x32e75a6a55963340>(__context__) = 0x0u;/*UTF8_ACCEPT*/
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_424e5a5af176be26 ( Context * __context__, TDim<uint8_t,2> const  &  __a_rename_at_598_0 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_2d5894d58d05f52 ( Context * __context__, TDim<uint8_t,3> const  &  __a_rename_at_598_1 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_5e3beb551331dc06 ( Context * __context__, TDim<uint8_t,4> const  &  __a_rename_at_598_2 )
{
    return das_auto_cast<int32_t>::cast(4);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_1e99f990a58976cb ( Context * __context__, TDim<uint8_t,1> const  &  __a_rename_at_598_3 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_8a2a57662f1d46d ( Context * __context__, TDim<uint8_t,2> &  __a_rename_at_1523_4 )
{
    TArray<uint8_t> __arr_rename_at_1525_5;das_zero(__arr_rename_at_1525_5);
    _FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_1525_5),2);
    das_copy(das_cast<TDim<uint8_t,2>>::cast(das_ref(__context__,__arr_rename_at_1525_5(0,__context__))),__a_rename_at_1523_4);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__arr_rename_at_1525_5);
}

inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_7d924998213ec597 ( Context * __context__, TDim<uint8_t,3> &  __a_rename_at_1523_6 )
{
    TArray<uint8_t> __arr_rename_at_1525_7;das_zero(__arr_rename_at_1525_7);
    _FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_1525_7),3);
    das_copy(das_cast<TDim<uint8_t,3>>::cast(das_ref(__context__,__arr_rename_at_1525_7(0,__context__))),__a_rename_at_1523_6);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__arr_rename_at_1525_7);
}

inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_aea3d34625ab51b4 ( Context * __context__, TDim<uint8_t,4> &  __a_rename_at_1523_8 )
{
    TArray<uint8_t> __arr_rename_at_1525_9;das_zero(__arr_rename_at_1525_9);
    _FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_1525_9),4);
    das_copy(das_cast<TDim<uint8_t,4>>::cast(das_ref(__context__,__arr_rename_at_1525_9(0,__context__))),__a_rename_at_1523_8);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__arr_rename_at_1525_9);
}

inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_ad51b48331aa2869 ( Context * __context__, TDim<uint8_t,1> &  __a_rename_at_1523_10 )
{
    TArray<uint8_t> __arr_rename_at_1525_11;das_zero(__arr_rename_at_1525_11);
    _FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_1525_11),1);
    das_copy(das_cast<TDim<uint8_t,1>>::cast(das_ref(__context__,__arr_rename_at_1525_11(0,__context__))),__a_rename_at_1523_10);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__arr_rename_at_1525_11);
}

inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d ( Context * __context__, TArray<uint8_t> &  __a_rename_at_50_12 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<2>(__tinfo_0,cast<TArray<uint8_t> &>::from(__a_rename_at_50_12),cast<char * const >::from(((char *) "object can't be returned because it is locked"))));
    return das_auto_cast_ref<TArray<uint8_t> &>::cast(__a_rename_at_50_12);
}

inline TArray<uint32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8494a8b6b6480f2 ( Context * __context__, TArray<uint32_t> &  __a_rename_at_50_13 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<TArray<uint32_t> &>::from(__a_rename_at_50_13),cast<char * const >::from(((char *) "object can't be returned because it is locked"))));
    return das_auto_cast_ref<TArray<uint32_t> &>::cast(__a_rename_at_50_13);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_177_14, uint8_t __value_rename_at_177_15 )
{
    das_copy(__Arr_rename_at_177_14(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_177_14),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_177_15);
}

inline void _FuncbuiltinTickreserveTick3994685146752941225_5d874a62d9325833 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_137_16, int32_t __newSize_rename_at_137_17 )
{
    builtin_array_reserve(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_137_16),__newSize_rename_at_137_17,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_a51cd706faf9e203 ( Context * __context__, TArray<uint32_t> &  __Arr_rename_at_193_18, uint32_t __value_rename_at_193_19 )
{
    das_copy(__Arr_rename_at_193_18(builtin_array_push_back(das_arg<TArray<uint32_t>>::pass(__Arr_rename_at_193_18),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_19);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_20, int32_t __newSize_rename_at_68_21 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_20),__newSize_rename_at_68_21,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline uint32_t utf16_to_utf32_49f3f9f3ecf44a4a ( Context * __context__, uint32_t __high_rename_at_15_22, uint32_t __low_rename_at_15_23 )
{
    return das_auto_cast<uint32_t>::cast(((__high_rename_at_15_22 >= 0xd800u) && (__high_rename_at_15_22 <= 0xdbffu)) ? das_auto_cast<uint32_t>::cast((((SimPolicy<uint32_t>::BinShl((__high_rename_at_15_22 - 0xd800u),0xau,*__context__,nullptr)) + (__low_rename_at_15_23 - 0xdc00u)) + 0x10000u)) : das_auto_cast<uint32_t>::cast(__high_rename_at_15_22));
}

inline void utf8_encode_af85f8b19afd18de ( Context * __context__, TArray<uint8_t> &  __dest_array_rename_at_24_24, uint32_t __ch_rename_at_24_25 )
{
    if ( __ch_rename_at_24_25 < 0x80u )
    {
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t(__ch_rename_at_24_25));
    } else if ( __ch_rename_at_24_25 < 0x800u )
    {
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t((SimPolicy<uint32_t>::BinShr(__ch_rename_at_24_25,0x6u,*__context__,nullptr)) + 0xc0u));
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t((__ch_rename_at_24_25 & 0x3fu) + 0x80u));
    } else if ( __ch_rename_at_24_25 < 0x10000u )
    {
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t((SimPolicy<uint32_t>::BinShr(__ch_rename_at_24_25,0xcu,*__context__,nullptr)) + 0xe0u));
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t(((SimPolicy<uint32_t>::BinShr(__ch_rename_at_24_25,0x6u,*__context__,nullptr)) & 0x3fu) + 0x80u));
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t((__ch_rename_at_24_25 & 0x3fu) + 0x80u));
    } else {
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t((SimPolicy<uint32_t>::BinShr(__ch_rename_at_24_25,0x12u,*__context__,nullptr)) + 0xf0u));
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t(((SimPolicy<uint32_t>::BinShr(__ch_rename_at_24_25,0xcu,*__context__,nullptr)) & 0x3fu) + 0x80u));
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t(((SimPolicy<uint32_t>::BinShr(__ch_rename_at_24_25,0x6u,*__context__,nullptr)) & 0x3fu) + 0x80u));
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_24_24),uint8_t((__ch_rename_at_24_25 & 0x3fu) + 0x80u));
    };
}

inline TArray<uint8_t> utf8_encode_b7d52a486d026a93 ( Context * __context__, uint32_t __ch_rename_at_44_26 )
{
    if ( __ch_rename_at_44_26 < 0x80u )
    {
        TArray<uint8_t> _temp_make_local_47_19_0; _temp_make_local_47_19_0;
        TDim<uint8_t,1> _temp_make_local_47_19_1; _temp_make_local_47_19_1;
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d(__context__,das_arg<TArray<uint8_t>>::pass((_temp_make_local_47_19_0 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_ad51b48331aa2869(__context__,das_arg<TDim<uint8_t,1>>::pass((([&]() -> TDim<uint8_t,1>& {
            _temp_make_local_47_19_1(0,__context__) = uint8_t(__ch_rename_at_44_26);
            return _temp_make_local_47_19_1;
        })()))))))));
    } else if ( __ch_rename_at_44_26 < 0x800u )
    {
        TArray<uint8_t> _temp_make_local_49_19_2; _temp_make_local_49_19_2;
        TDim<uint8_t,2> _temp_make_local_49_19_3; _temp_make_local_49_19_3;
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d(__context__,das_arg<TArray<uint8_t>>::pass((_temp_make_local_49_19_2 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_8a2a57662f1d46d(__context__,das_arg<TDim<uint8_t,2>>::pass((([&]() -> TDim<uint8_t,2>& {
            _temp_make_local_49_19_3(0,__context__) = uint8_t((SimPolicy<uint32_t>::BinShr(__ch_rename_at_44_26,0x6u,*__context__,nullptr)) + 0xc0u);
            _temp_make_local_49_19_3(1,__context__) = uint8_t((__ch_rename_at_44_26 & 0x3fu) + 0x80u);
            return _temp_make_local_49_19_3;
        })()))))))));
    } else if ( __ch_rename_at_44_26 < 0x10000u )
    {
        TArray<uint8_t> _temp_make_local_53_19_4; _temp_make_local_53_19_4;
        TDim<uint8_t,3> _temp_make_local_53_19_5; _temp_make_local_53_19_5;
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d(__context__,das_arg<TArray<uint8_t>>::pass((_temp_make_local_53_19_4 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_7d924998213ec597(__context__,das_arg<TDim<uint8_t,3>>::pass((([&]() -> TDim<uint8_t,3>& {
            _temp_make_local_53_19_5(0,__context__) = uint8_t((SimPolicy<uint32_t>::BinShr(__ch_rename_at_44_26,0xcu,*__context__,nullptr)) + 0xe0u);
            _temp_make_local_53_19_5(1,__context__) = uint8_t(((SimPolicy<uint32_t>::BinShr(__ch_rename_at_44_26,0x6u,*__context__,nullptr)) & 0x3fu) + 0x80u);
            _temp_make_local_53_19_5(2,__context__) = uint8_t((__ch_rename_at_44_26 & 0x3fu) + 0x80u);
            return _temp_make_local_53_19_5;
        })()))))))));
    } else {
        TArray<uint8_t> _temp_make_local_58_19_6; _temp_make_local_58_19_6;
        TDim<uint8_t,4> _temp_make_local_58_19_7; _temp_make_local_58_19_7;
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d(__context__,das_arg<TArray<uint8_t>>::pass((_temp_make_local_58_19_6 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_aea3d34625ab51b4(__context__,das_arg<TDim<uint8_t,4>>::pass((([&]() -> TDim<uint8_t,4>& {
            _temp_make_local_58_19_7(0,__context__) = uint8_t((SimPolicy<uint32_t>::BinShr(__ch_rename_at_44_26,0x12u,*__context__,nullptr)) + 0xf0u);
            _temp_make_local_58_19_7(1,__context__) = uint8_t(((SimPolicy<uint32_t>::BinShr(__ch_rename_at_44_26,0xcu,*__context__,nullptr)) & 0x3fu) + 0x80u);
            _temp_make_local_58_19_7(2,__context__) = uint8_t(((SimPolicy<uint32_t>::BinShr(__ch_rename_at_44_26,0x6u,*__context__,nullptr)) & 0x3fu) + 0x80u);
            _temp_make_local_58_19_7(3,__context__) = uint8_t((__ch_rename_at_44_26 & 0x3fu) + 0x80u);
            return _temp_make_local_58_19_7;
        })()))))))));
    };
}

inline void utf8_encode_8e4897038ba5ca3b ( Context * __context__, TArray<uint8_t> &  __dest_array_rename_at_67_27, TArray<uint32_t> const  &  __source_utf32_string_rename_at_67_28 )
{
    {
        bool __need_loop_69 = true;
        // ch: uint const&
        das_iterator<TArray<uint32_t> const > __ch_iterator(__source_utf32_string_rename_at_67_28);
        uint32_t const  * __ch_rename_at_69_29;
        __need_loop_69 = __ch_iterator.first(__context__,(__ch_rename_at_69_29)) && __need_loop_69;
        for ( ; __need_loop_69 ; __need_loop_69 = __ch_iterator.next(__context__,(__ch_rename_at_69_29)) )
        {
            utf8_encode_af85f8b19afd18de(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_67_27),(*__ch_rename_at_69_29));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_69_29));
    };
}

inline TArray<uint8_t> utf8_encode_de3cdc7453a168bb ( Context * __context__, TArray<uint32_t> const  &  __source_utf32_string_rename_at_75_30 )
{
    TArray<uint8_t> __dest_array_rename_at_77_31;das_zero(__dest_array_rename_at_77_31);
    _FuncbuiltinTickreserveTick3994685146752941225_5d874a62d9325833(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_77_31),builtin_array_size(__source_utf32_string_rename_at_75_30));
    {
        bool __need_loop_79 = true;
        // ch: uint const&
        das_iterator<TArray<uint32_t> const > __ch_iterator(__source_utf32_string_rename_at_75_30);
        uint32_t const  * __ch_rename_at_79_32;
        __need_loop_79 = __ch_iterator.first(__context__,(__ch_rename_at_79_32)) && __need_loop_79;
        for ( ; __need_loop_79 ; __need_loop_79 = __ch_iterator.next(__context__,(__ch_rename_at_79_32)) )
        {
            utf8_encode_af85f8b19afd18de(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_77_31),(*__ch_rename_at_79_32));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_79_32));
    };
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_77_31)));
}

inline int32_t utf8_length_5ae766430c8d22e1 ( Context * __context__, TArray<uint8_t> const  &  __utf8_string_rename_at_86_33 )
{
    int32_t __length_rename_at_88_34 = 0;
    {
        bool __need_loop_89 = true;
        // ch: uint8 const&
        das_iterator<TArray<uint8_t> const > __ch_iterator(__utf8_string_rename_at_86_33);
        uint8_t const  * __ch_rename_at_89_35;
        __need_loop_89 = __ch_iterator.first(__context__,(__ch_rename_at_89_35)) && __need_loop_89;
        for ( ; __need_loop_89 ; __need_loop_89 = __ch_iterator.next(__context__,(__ch_rename_at_89_35)) )
        {
            if ( (uint32_t((*__ch_rename_at_89_35)) & 0xc0u) != 0x80u )
            {
                ++__length_rename_at_88_34;
            };
        }
        __ch_iterator.close(__context__,(__ch_rename_at_89_35));
    };
    return das_auto_cast<int32_t>::cast(__length_rename_at_88_34);
}

inline int32_t utf8_length_e8e8058edf449d85 ( Context * __context__, char * const  __utf8_string_rename_at_98_36 )
{
    int32_t __length_rename_at_100_37 = 0;
    {
        bool __need_loop_101 = true;
        // ch: int const
        das_iterator<char * const > __ch_iterator(__utf8_string_rename_at_98_36);
        int32_t __ch_rename_at_101_38;
        __need_loop_101 = __ch_iterator.first(__context__,(__ch_rename_at_101_38)) && __need_loop_101;
        for ( ; __need_loop_101 ; __need_loop_101 = __ch_iterator.next(__context__,(__ch_rename_at_101_38)) )
        {
            if ( (uint32_t(__ch_rename_at_101_38) & 0xc0u) != 0x80u )
            {
                ++__length_rename_at_100_37;
            };
        }
        __ch_iterator.close(__context__,(__ch_rename_at_101_38));
    };
    return das_auto_cast<int32_t>::cast(__length_rename_at_100_37);
}

inline bool is_first_byte_of_utf8_char_ec6547e881783261 ( Context * __context__, uint8_t __ch_rename_at_110_39 )
{
    uint32_t __x_rename_at_112_40 = ((uint32_t)uint32_t(__ch_rename_at_110_39));
    return das_auto_cast<bool>::cast((__x_rename_at_112_40 > 0x0u) && ((__x_rename_at_112_40 < 0x80u) || ((__x_rename_at_112_40 & 0xc0u) == 0xc0u)));
}

inline bool contains_utf8_bom_190bd0f92360f1e8 ( Context * __context__, TArray<uint8_t> const  &  __utf8_string_rename_at_117_41 )
{
    return das_auto_cast<bool>::cast((((builtin_array_size(__utf8_string_rename_at_117_41) >= 3) && (uint32_t(__utf8_string_rename_at_117_41(0,__context__)) == 0xefu)) && (uint32_t(__utf8_string_rename_at_117_41(1,__context__)) == 0xbbu)) && (uint32_t(__utf8_string_rename_at_117_41(2,__context__)) == 0xbfu));
}

inline bool contains_utf8_bom_1dac111ec6d7fb8f ( Context * __context__, char * const  __utf8_string_rename_at_124_42 )
{
    return das_auto_cast<bool>::cast(((!(builtin_empty(__utf8_string_rename_at_124_42)) && (uint32_t(get_character_uat(__utf8_string_rename_at_124_42,0)) == 0xefu)) && (uint32_t(get_character_uat(__utf8_string_rename_at_124_42,1)) == 0xbbu)) && (uint32_t(get_character_uat(__utf8_string_rename_at_124_42,2)) == 0xbfu));
}

inline bool is_utf8_string_valid_1a64d709eddf87fd ( Context * __context__, TArray<uint8_t> const  &  __utf8_string_rename_at_162_43 )
{
    uint32_t __codepoint_rename_at_164_44 = 0x0u;
    uint32_t __state_rename_at_165_45 = 0x0u;
    {
        bool __need_loop_166 = true;
        // ch: uint8 const&
        das_iterator<TArray<uint8_t> const > __ch_iterator(__utf8_string_rename_at_162_43);
        uint8_t const  * __ch_rename_at_166_46;
        __need_loop_166 = __ch_iterator.first(__context__,(__ch_rename_at_166_46)) && __need_loop_166;
        for ( ; __need_loop_166 ; __need_loop_166 = __ch_iterator.next(__context__,(__ch_rename_at_166_46)) )
        {
            uint32_t __byte_rename_at_167_47 = ((uint32_t)uint32_t((*__ch_rename_at_166_46)));
            uint32_t __type__rename_at_168_48 = ((uint32_t)das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(__byte_rename_at_167_47,__context__));
            das_copy(__codepoint_rename_at_164_44,(__state_rename_at_165_45 != 0x0u) ? das_auto_cast<uint32_t>::cast(((__byte_rename_at_167_47 & 0x3fu) | (SimPolicy<uint32_t>::BinShl(__codepoint_rename_at_164_44,0x6u,*__context__,nullptr)))) : das_auto_cast<uint32_t>::cast(((SimPolicy<uint32_t>::BinShr(0xffu,__type__rename_at_168_48,*__context__,nullptr)) & __byte_rename_at_167_47)));
            das_copy(__state_rename_at_165_45,das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(((__state_rename_at_165_45 + 0x100u) + __type__rename_at_168_48),__context__));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_166_46));
    };
    return das_auto_cast<bool>::cast(__state_rename_at_165_45 == 0x0u);
}

inline bool is_utf8_string_valid_fb0b28f1946e058c ( Context * __context__, char * const  __utf8_string_rename_at_176_49 )
{
    uint32_t __codepoint_rename_at_178_50 = 0x0u;
    uint32_t __state_rename_at_179_51 = 0x0u;
    {
        bool __need_loop_180 = true;
        // ch: int const
        das_iterator<char * const > __ch_iterator(__utf8_string_rename_at_176_49);
        int32_t __ch_rename_at_180_52;
        __need_loop_180 = __ch_iterator.first(__context__,(__ch_rename_at_180_52)) && __need_loop_180;
        for ( ; __need_loop_180 ; __need_loop_180 = __ch_iterator.next(__context__,(__ch_rename_at_180_52)) )
        {
            uint32_t __byte_rename_at_181_53 = ((uint32_t)uint32_t(__ch_rename_at_180_52));
            uint32_t __type__rename_at_182_54 = ((uint32_t)das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(__byte_rename_at_181_53,__context__));
            das_copy(__codepoint_rename_at_178_50,(__state_rename_at_179_51 != 0x0u) ? das_auto_cast<uint32_t>::cast(((__byte_rename_at_181_53 & 0x3fu) | (SimPolicy<uint32_t>::BinShl(__codepoint_rename_at_178_50,0x6u,*__context__,nullptr)))) : das_auto_cast<uint32_t>::cast(((SimPolicy<uint32_t>::BinShr(0xffu,__type__rename_at_182_54,*__context__,nullptr)) & __byte_rename_at_181_53)));
            das_copy(__state_rename_at_179_51,das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(((__state_rename_at_179_51 + 0x100u) + __type__rename_at_182_54),__context__));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_180_52));
    };
    return das_auto_cast<bool>::cast(__state_rename_at_179_51 == 0x0u);
}

inline void utf8_decode_4dcab8dbe3c80d3d ( Context * __context__, TArray<uint32_t> &  __dest_utf32_string_rename_at_190_55, TArray<uint8_t> const  &  __source_utf8_string_rename_at_190_56 )
{
    uint32_t __codepoint_rename_at_192_57 = 0x0u;
    uint32_t __state_rename_at_193_58 = 0x0u;
    int32_t __length_rename_at_194_59 = ((int32_t)builtin_array_size(__source_utf8_string_rename_at_190_56));
    int32_t __i_rename_at_195_60 = (contains_utf8_bom_190bd0f92360f1e8(__context__,__source_utf8_string_rename_at_190_56) ? das_auto_cast<int32_t>::cast(3) : das_auto_cast<int32_t>::cast(0));
    while ( __i_rename_at_195_60 < __length_rename_at_194_59 )
    {
        uint32_t __byte_rename_at_198_61 = ((uint32_t)uint32_t(__source_utf8_string_rename_at_190_56(__i_rename_at_195_60++,__context__)));
        uint32_t __type__rename_at_199_62 = ((uint32_t)das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(__byte_rename_at_198_61,__context__));
        das_copy(__codepoint_rename_at_192_57,(__state_rename_at_193_58 != 0x0u) ? das_auto_cast<uint32_t>::cast(((__byte_rename_at_198_61 & 0x3fu) | (SimPolicy<uint32_t>::BinShl(__codepoint_rename_at_192_57,0x6u,*__context__,nullptr)))) : das_auto_cast<uint32_t>::cast(((SimPolicy<uint32_t>::BinShr(0xffu,__type__rename_at_199_62,*__context__,nullptr)) & __byte_rename_at_198_61)));
        das_copy(__state_rename_at_193_58,das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(((__state_rename_at_193_58 + 0x100u) + __type__rename_at_199_62),__context__));
        if ( __state_rename_at_193_58 == 0x0u )
        {
            _FuncbuiltinTickpushTick10769833213962245646_a51cd706faf9e203(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_190_55),__codepoint_rename_at_192_57);
        };
    };
}

inline TArray<uint32_t> utf8_decode_67bf22c3da8b41ed ( Context * __context__, TArray<uint8_t> const  &  __source_utf8_string_rename_at_209_63 )
{
    TArray<uint32_t> __dest_utf32_string_rename_at_211_64;das_zero(__dest_utf32_string_rename_at_211_64);
    utf8_decode_4dcab8dbe3c80d3d(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_211_64),__source_utf8_string_rename_at_209_63);
    return /* <- */ das_auto_cast_move<TArray<uint32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8494a8b6b6480f2(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_211_64)));
}

inline TArray<uint32_t> utf8_decode_f7fbdc235a8d69f7 ( Context * __context__, char * const  __source_utf8_string_rename_at_217_65 ) { das_stack_prologue __prologue(__context__,96,"utf8_decode " DAS_FILE_LINE);
{
    TArray<uint32_t> __dest_utf32_string_rename_at_219_66;das_zero(__dest_utf32_string_rename_at_219_66);
    builtin_string_peek(__source_utf8_string_rename_at_217_65,das_make_block<void,TArray<uint8_t> const  &>(__context__,80,0,&__func_info__5f4b6da70ca496e2,[&](TArray<uint8_t> const  & __arr_rename_at_220_67) -> void{
        utf8_decode_4dcab8dbe3c80d3d(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_219_66),__arr_rename_at_220_67);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<TArray<uint32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8494a8b6b6480f2(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_219_66)));
}}

inline void utf8_decode_d3d9f41f1665ca29 ( Context * __context__, TArray<uint32_t> &  __dest_utf32_string_rename_at_227_68, char * const  __source_utf8_string_rename_at_227_69 ) { das_stack_prologue __prologue(__context__,96,"utf8_decode " DAS_FILE_LINE);
{
    builtin_string_peek(__source_utf8_string_rename_at_227_69,das_make_block<void,TArray<uint8_t> const  &>(__context__,80,0,&__func_info__5f4b6da70ca496e2,[&](TArray<uint8_t> const  & __arr_rename_at_229_70) -> void{
        utf8_decode_4dcab8dbe3c80d3d(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_227_68),__arr_rename_at_229_70);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline char * decode_unicode_escape_bdbb72c622da2400 ( Context * __context__, char * const  __str_rename_at_234_71 ) { das_stack_prologue __prologue(__context__,384,"decode_unicode_escape " DAS_FILE_LINE);
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_236_72) DAS_AOT_INLINE_LAMBDA -> void{
        builtin_string_peek(__str_rename_at_234_71,das_make_block<void,TArray<uint8_t> const  &>(__context__,144,0,&__func_info__2f76c7f7dd4f0b27,[&](TArray<uint8_t> const  & __p_rename_at_237_73) -> void{
            int32_t __i_rename_at_238_74 = 0;
            int32_t __llen_rename_at_239_75 = ((int32_t)builtin_array_size(__p_rename_at_237_73));
            TArray<uint8_t> __bang_rename_at_240_76;das_zero(__bang_rename_at_240_76);
            _FuncbuiltinTickreserveTick3994685146752941225_5d874a62d9325833(__context__,das_arg<TArray<uint8_t>>::pass(__bang_rename_at_240_76),8);
            uint32_t __high_surrogate_rename_at_242_77 = 0x0u;
            while ( __i_rename_at_238_74 < __llen_rename_at_239_75 )
            {
                if ( das_equ_val(__p_rename_at_237_73(__i_rename_at_238_74,__context__),0x5c) )
                {
                    int32_t __j_rename_at_245_78 = (__i_rename_at_238_74 + 1);
                    int32_t __jlen_rename_at_246_79 = ((int32_t)SimPolicy<int32_t>::Min(__llen_rename_at_239_75,__j_rename_at_245_78 + 8,*__context__,nullptr));
                    _FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111(__context__,das_arg<TArray<uint8_t>>::pass(__bang_rename_at_240_76),0);
                    while ( (__j_rename_at_245_78 < __jlen_rename_at_246_79) && is_hex(int32_t(__p_rename_at_237_73(__j_rename_at_245_78,__context__))) )
                    {
                        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(__bang_rename_at_240_76),__p_rename_at_237_73(__j_rename_at_245_78,__context__));
                        ++__j_rename_at_245_78;
                    };
                    if ( builtin_array_size(das_arg<TArray<uint8_t>>::pass(__bang_rename_at_240_76)) == 4 )
                    {
                        uint32_t __utf32_rename_at_253_80 = ((uint32_t)fast_to_uint(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__bang_rename_at_240_76),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),true));
                        if ( (__utf32_rename_at_253_80 >= 0xd800u) && (__utf32_rename_at_253_80 <= 0xdfffu) )
                        {
                            if ( __high_surrogate_rename_at_242_77 == 0x0u )
                            {
                                das_copy(__high_surrogate_rename_at_242_77,__utf32_rename_at_253_80);
                            } else {
                                TArray<uint8_t> _temp_make_local_261_55_8; _temp_make_local_261_55_8;
                                uint32_t __low_surrogate_rename_at_258_81 = ((uint32_t)__utf32_rename_at_253_80);
                                uint32_t __pair_rename_at_259_82 = ((uint32_t)(((SimPolicy<uint32_t>::BinShl((__high_surrogate_rename_at_242_77 - 0xd800u),0xau,*__context__,nullptr)) + 0x10000u) + (__low_surrogate_rename_at_258_81 - 0xdc00u)));
                                das_copy(__high_surrogate_rename_at_242_77,0x0u);
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_236_72),cast<char * const >::from(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass((_temp_make_local_261_55_8 = (utf8_encode_b7d52a486d026a93(__context__,__pair_rename_at_259_82)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                            };
                        } else {
                            TArray<uint8_t> _temp_make_local_264_51_9; _temp_make_local_264_51_9;
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_236_72),cast<char * const >::from(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass((_temp_make_local_264_51_9 = (utf8_encode_b7d52a486d026a93(__context__,__utf32_rename_at_253_80)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                        };
                        das_copy(__i_rename_at_238_74,__j_rename_at_245_78);
                        continue;
                    };
                };
                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_236_72),int32_t(__p_rename_at_237_73(__i_rename_at_238_74,__context__)));
                __i_rename_at_238_74 += 1;
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}}
static vec4f __wrap__FuncbuiltinTicklengthTick18150397773952384912_424e5a5af176be26 ( Context * __context__ ) {
    TDim<uint8_t,2> const  &  arg_a = cast_aot_arg<TDim<uint8_t,2> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(_FuncbuiltinTicklengthTick18150397773952384912_424e5a5af176be26(__context__, arg_a));
}
static vec4f __wrap__FuncbuiltinTicklengthTick18150397773952384912_2d5894d58d05f52 ( Context * __context__ ) {
    TDim<uint8_t,3> const  &  arg_a = cast_aot_arg<TDim<uint8_t,3> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(_FuncbuiltinTicklengthTick18150397773952384912_2d5894d58d05f52(__context__, arg_a));
}
static vec4f __wrap__FuncbuiltinTicklengthTick18150397773952384912_5e3beb551331dc06 ( Context * __context__ ) {
    TDim<uint8_t,4> const  &  arg_a = cast_aot_arg<TDim<uint8_t,4> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(_FuncbuiltinTicklengthTick18150397773952384912_5e3beb551331dc06(__context__, arg_a));
}
static vec4f __wrap__FuncbuiltinTicklengthTick18150397773952384912_1e99f990a58976cb ( Context * __context__ ) {
    TDim<uint8_t,1> const  &  arg_a = cast_aot_arg<TDim<uint8_t,1> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(_FuncbuiltinTicklengthTick18150397773952384912_1e99f990a58976cb(__context__, arg_a));
}
static vec4f __wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_8a2a57662f1d46d ( Context * __context__ ) {
    TDim<uint8_t,2> &  arg_a = cast_aot_arg<TDim<uint8_t,2> & >::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<uint8_t> *) __context__->abiCMRES) = _FuncbuiltinTickto_array_moveTick3185538323411982277_8a2a57662f1d46d(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_7d924998213ec597 ( Context * __context__ ) {
    TDim<uint8_t,3> &  arg_a = cast_aot_arg<TDim<uint8_t,3> & >::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<uint8_t> *) __context__->abiCMRES) = _FuncbuiltinTickto_array_moveTick3185538323411982277_7d924998213ec597(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_aea3d34625ab51b4 ( Context * __context__ ) {
    TDim<uint8_t,4> &  arg_a = cast_aot_arg<TDim<uint8_t,4> & >::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<uint8_t> *) __context__->abiCMRES) = _FuncbuiltinTickto_array_moveTick3185538323411982277_aea3d34625ab51b4(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_ad51b48331aa2869 ( Context * __context__ ) {
    TDim<uint8_t,1> &  arg_a = cast_aot_arg<TDim<uint8_t,1> & >::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<uint8_t> *) __context__->abiCMRES) = _FuncbuiltinTickto_array_moveTick3185538323411982277_ad51b48331aa2869(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d ( Context * __context__ ) {
    TArray<uint8_t> &  arg_a = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<TArray<uint8_t> &>::from(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d(__context__, arg_a));
}
static vec4f __wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8494a8b6b6480f2 ( Context * __context__ ) {
    TArray<uint32_t> &  arg_a = cast_aot_arg<TArray<uint32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<TArray<uint32_t> &>::from(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8494a8b6b6480f2(__context__, arg_a));
}
static vec4f __wrap__FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8 ( Context * __context__ ) {
    TArray<uint8_t> &  arg_Arr = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    uint8_t arg_value = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickreserveTick3994685146752941225_5d874a62d9325833 ( Context * __context__ ) {
    TArray<uint8_t> &  arg_Arr = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickreserveTick3994685146752941225_5d874a62d9325833(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_a51cd706faf9e203 ( Context * __context__ ) {
    TArray<uint32_t> &  arg_Arr = cast_aot_arg<TArray<uint32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    uint32_t arg_value = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_a51cd706faf9e203(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111 ( Context * __context__ ) {
    TArray<uint8_t> &  arg_Arr = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap_utf16_to_utf32_49f3f9f3ecf44a4a ( Context * __context__ ) {
    uint32_t arg_high = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[0]);
    uint32_t arg_low = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint32_t>::from(utf16_to_utf32_49f3f9f3ecf44a4a(__context__, arg_high, arg_low));
}
static vec4f __wrap_utf8_encode_af85f8b19afd18de ( Context * __context__ ) {
    TArray<uint8_t> &  arg_dest_array = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    uint32_t arg_ch = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[1]);
    utf8_encode_af85f8b19afd18de(__context__, arg_dest_array, arg_ch);
    return v_zero();
}
static vec4f __wrap_utf8_encode_b7d52a486d026a93 ( Context * __context__ ) {
    uint32_t arg_ch = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<uint8_t> *) __context__->abiCMRES) = utf8_encode_b7d52a486d026a93(__context__, arg_ch);
    return v_zero();
}
static vec4f __wrap_utf8_encode_8e4897038ba5ca3b ( Context * __context__ ) {
    TArray<uint8_t> &  arg_dest_array = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    TArray<uint32_t> const  &  arg_source_utf32_string = cast_aot_arg<TArray<uint32_t> const  & >::to(*__context__,__context__->abiArguments()[1]);
    utf8_encode_8e4897038ba5ca3b(__context__, arg_dest_array, arg_source_utf32_string);
    return v_zero();
}
static vec4f __wrap_utf8_encode_de3cdc7453a168bb ( Context * __context__ ) {
    TArray<uint32_t> const  &  arg_source_utf32_string = cast_aot_arg<TArray<uint32_t> const  & >::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<uint8_t> *) __context__->abiCMRES) = utf8_encode_de3cdc7453a168bb(__context__, arg_source_utf32_string);
    return v_zero();
}
static vec4f __wrap_utf8_length_5ae766430c8d22e1 ( Context * __context__ ) {
    TArray<uint8_t> const  &  arg_utf8_string = cast_aot_arg<TArray<uint8_t> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(utf8_length_5ae766430c8d22e1(__context__, arg_utf8_string));
}
static vec4f __wrap_utf8_length_e8e8058edf449d85 ( Context * __context__ ) {
    char * const  arg_utf8_string = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(utf8_length_e8e8058edf449d85(__context__, arg_utf8_string));
}
static vec4f __wrap_is_first_byte_of_utf8_char_ec6547e881783261 ( Context * __context__ ) {
    uint8_t arg_ch = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_first_byte_of_utf8_char_ec6547e881783261(__context__, arg_ch));
}
static vec4f __wrap_contains_utf8_bom_190bd0f92360f1e8 ( Context * __context__ ) {
    TArray<uint8_t> const  &  arg_utf8_string = cast_aot_arg<TArray<uint8_t> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(contains_utf8_bom_190bd0f92360f1e8(__context__, arg_utf8_string));
}
static vec4f __wrap_contains_utf8_bom_1dac111ec6d7fb8f ( Context * __context__ ) {
    char * const  arg_utf8_string = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(contains_utf8_bom_1dac111ec6d7fb8f(__context__, arg_utf8_string));
}
static vec4f __wrap_is_utf8_string_valid_1a64d709eddf87fd ( Context * __context__ ) {
    TArray<uint8_t> const  &  arg_utf8_string = cast_aot_arg<TArray<uint8_t> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_utf8_string_valid_1a64d709eddf87fd(__context__, arg_utf8_string));
}
static vec4f __wrap_is_utf8_string_valid_fb0b28f1946e058c ( Context * __context__ ) {
    char * const  arg_utf8_string = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_utf8_string_valid_fb0b28f1946e058c(__context__, arg_utf8_string));
}
static vec4f __wrap_utf8_decode_4dcab8dbe3c80d3d ( Context * __context__ ) {
    TArray<uint32_t> &  arg_dest_utf32_string = cast_aot_arg<TArray<uint32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    TArray<uint8_t> const  &  arg_source_utf8_string = cast_aot_arg<TArray<uint8_t> const  & >::to(*__context__,__context__->abiArguments()[1]);
    utf8_decode_4dcab8dbe3c80d3d(__context__, arg_dest_utf32_string, arg_source_utf8_string);
    return v_zero();
}
static vec4f __wrap_utf8_decode_67bf22c3da8b41ed ( Context * __context__ ) {
    TArray<uint8_t> const  &  arg_source_utf8_string = cast_aot_arg<TArray<uint8_t> const  & >::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<uint32_t> *) __context__->abiCMRES) = utf8_decode_67bf22c3da8b41ed(__context__, arg_source_utf8_string);
    return v_zero();
}
static vec4f __wrap_utf8_decode_f7fbdc235a8d69f7 ( Context * __context__ ) {
    char * const  arg_source_utf8_string = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<uint32_t> *) __context__->abiCMRES) = utf8_decode_f7fbdc235a8d69f7(__context__, arg_source_utf8_string);
    return v_zero();
}
static vec4f __wrap_utf8_decode_d3d9f41f1665ca29 ( Context * __context__ ) {
    TArray<uint32_t> &  arg_dest_utf32_string = cast_aot_arg<TArray<uint32_t> & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_source_utf8_string = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    utf8_decode_d3d9f41f1665ca29(__context__, arg_dest_utf32_string, arg_source_utf8_string);
    return v_zero();
}
static vec4f __wrap_decode_unicode_escape_bdbb72c622da2400 ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(decode_unicode_escape_bdbb72c622da2400(__context__, arg_str));
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0x5f2482cc28bc3b00, false, (void*)&_FuncbuiltinTicklengthTick18150397773952384912_424e5a5af176be26, &__wrap__FuncbuiltinTicklengthTick18150397773952384912_424e5a5af176be26 },
    { 0x6fe85bba2d9b3792, false, (void*)&_FuncbuiltinTicklengthTick18150397773952384912_2d5894d58d05f52, &__wrap__FuncbuiltinTicklengthTick18150397773952384912_2d5894d58d05f52 },
    { 0x526025a25c517ee9, false, (void*)&_FuncbuiltinTicklengthTick18150397773952384912_5e3beb551331dc06, &__wrap__FuncbuiltinTicklengthTick18150397773952384912_5e3beb551331dc06 },
    { 0x6c2b0f397612e34b, false, (void*)&_FuncbuiltinTicklengthTick18150397773952384912_1e99f990a58976cb, &__wrap__FuncbuiltinTicklengthTick18150397773952384912_1e99f990a58976cb },
    { 0x8755ba62b12ff106, true, (void*)&_FuncbuiltinTickto_array_moveTick3185538323411982277_8a2a57662f1d46d, &__wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_8a2a57662f1d46d },
    { 0x64c2592178520eb4, true, (void*)&_FuncbuiltinTickto_array_moveTick3185538323411982277_7d924998213ec597, &__wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_7d924998213ec597 },
    { 0x4c0d191d56cfe018, true, (void*)&_FuncbuiltinTickto_array_moveTick3185538323411982277_aea3d34625ab51b4, &__wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_aea3d34625ab51b4 },
    { 0xa9a06445c203be3f, true, (void*)&_FuncbuiltinTickto_array_moveTick3185538323411982277_ad51b48331aa2869, &__wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_ad51b48331aa2869 },
    { 0x5e20d4c916986156, false, (void*)&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d, &__wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_93430be91b27677d },
    { 0x222424d6214dd015, false, (void*)&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8494a8b6b6480f2, &__wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8494a8b6b6480f2 },
    { 0x89792d67359db3ab, false, (void*)&_FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8, &__wrap__FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8 },
    { 0x9ea4c6876bf01108, false, (void*)&_FuncbuiltinTickreserveTick3994685146752941225_5d874a62d9325833, &__wrap__FuncbuiltinTickreserveTick3994685146752941225_5d874a62d9325833 },
    { 0xeb8891d6f988750b, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_a51cd706faf9e203, &__wrap__FuncbuiltinTickpushTick10769833213962245646_a51cd706faf9e203 },
    { 0x43319db4719733d6, false, (void*)&_FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111, &__wrap__FuncbuiltinTickresizeTick4811697762258667383_bdb96b229e3c3111 },
    { 0x64dc2ae1cd231545, false, (void*)&utf16_to_utf32_49f3f9f3ecf44a4a, &__wrap_utf16_to_utf32_49f3f9f3ecf44a4a },
    { 0xbb37b4ecfcafea3, false, (void*)&utf8_encode_af85f8b19afd18de, &__wrap_utf8_encode_af85f8b19afd18de },
    { 0xfda948266ff164aa, true, (void*)&utf8_encode_b7d52a486d026a93, &__wrap_utf8_encode_b7d52a486d026a93 },
    { 0xb2a9ff8c7485f08f, false, (void*)&utf8_encode_8e4897038ba5ca3b, &__wrap_utf8_encode_8e4897038ba5ca3b },
    { 0x62d284a22de4b595, true, (void*)&utf8_encode_de3cdc7453a168bb, &__wrap_utf8_encode_de3cdc7453a168bb },
    { 0xbf220ab834cac93c, false, (void*)&utf8_length_5ae766430c8d22e1, &__wrap_utf8_length_5ae766430c8d22e1 },
    { 0x30eae0e4cccf4ef1, false, (void*)&utf8_length_e8e8058edf449d85, &__wrap_utf8_length_e8e8058edf449d85 },
    { 0x2c12626f426ab2ad, false, (void*)&is_first_byte_of_utf8_char_ec6547e881783261, &__wrap_is_first_byte_of_utf8_char_ec6547e881783261 },
    { 0xcb382f43e8c87fee, false, (void*)&contains_utf8_bom_190bd0f92360f1e8, &__wrap_contains_utf8_bom_190bd0f92360f1e8 },
    { 0xe825294a22b47585, false, (void*)&contains_utf8_bom_1dac111ec6d7fb8f, &__wrap_contains_utf8_bom_1dac111ec6d7fb8f },
    { 0x996ce3a66cc53240, false, (void*)&is_utf8_string_valid_1a64d709eddf87fd, &__wrap_is_utf8_string_valid_1a64d709eddf87fd },
    { 0x45f55bd9b7b3954a, false, (void*)&is_utf8_string_valid_fb0b28f1946e058c, &__wrap_is_utf8_string_valid_fb0b28f1946e058c },
    { 0xd3c99f84718383f4, false, (void*)&utf8_decode_4dcab8dbe3c80d3d, &__wrap_utf8_decode_4dcab8dbe3c80d3d },
    { 0x6c06ef8be6ed1ed, true, (void*)&utf8_decode_67bf22c3da8b41ed, &__wrap_utf8_decode_67bf22c3da8b41ed },
    { 0x7dcb4e5a7d60d6d5, true, (void*)&utf8_decode_f7fbdc235a8d69f7, &__wrap_utf8_decode_f7fbdc235a8d69f7 },
    { 0xb6608e4810df12d, false, (void*)&utf8_decode_d3d9f41f1665ca29, &__wrap_utf8_decode_d3d9f41f1665ca29 },
    { 0xe7703c8ca8934fc2, false, (void*)&decode_unicode_escape_bdbb72c622da2400, &__wrap_decode_unicode_escape_bdbb72c622da2400 },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
    resolveTypeInfoAnnotations();
}

} // namespace _anon_2135218099027497698
AotListBase impl_aot_utf8_utils(_anon_2135218099027497698::registerAotFunctions);
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
