#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
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
extern TypeInfo __type_info__4ac1d999a882997b;
extern TypeInfo __type_info__5ee52699b9679f52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__af63e84c860200f0;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__1f33941668547bfd;
extern VarInfo __var_info__3aefdf5fd38bc810;
extern FuncInfo __func_info__5f4b6da70ca496e2;
extern FuncInfo __func_info__2f76c7f7dd4f0b27;

VarInfo __func_info__5f4b6da70ca496e2_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x1f33941668547bfd), "arr", 0, 0 };
VarInfo * __func_info__5f4b6da70ca496e2_fields[1] =  { &__func_info__5f4b6da70ca496e2_field_0 };
FuncInfo __func_info__5f4b6da70ca496e2 = {"invoke block<(arr:array<uint8> const#):void> const", "", __func_info__5f4b6da70ca496e2_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x5f4b6da70ca496e2), 0x0 };
VarInfo __func_info__2f76c7f7dd4f0b27_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x3aefdf5fd38bc810), "p", 0, 0 };
VarInfo * __func_info__2f76c7f7dd4f0b27_fields[1] =  { &__func_info__2f76c7f7dd4f0b27_field_0 };
FuncInfo __func_info__2f76c7f7dd4f0b27 = {"invoke block<(p:array<uint8> const#):void> const", "", __func_info__2f76c7f7dd4f0b27_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x2f76c7f7dd4f0b27), 0x0 };
TypeInfo __type_info__4ac1d999a882997b = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x4ac1d999a882997b) };
TypeInfo __type_info__5ee52699b9679f52 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63e84c860200f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint32_t>>::size, UINT64_C(0x5ee52699b9679f52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<StringBuilderWriter>::size, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__af63e84c860200f0 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint32_t>::size, UINT64_C(0xaf63e84c860200f0) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint8_t>::size, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<void>::size, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__4ac1d999a882997b };
TypeInfo * __tinfo_1[1] = { &__type_info__5ee52699b9679f52 };
TypeInfo * __tinfo_2[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_3[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3dbee92a7efb07d9 ( Context * __context__, TDim<uint8_t,2> const  & __a_rename_at_586_0 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_194914ba4b4e9234 ( Context * __context__, TDim<uint8_t,3> const  & __a_rename_at_586_1 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3d13e435f27e824d ( Context * __context__, TDim<uint8_t,4> const  & __a_rename_at_586_2 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_a94ae7fe5f83f893 ( Context * __context__, TDim<uint8_t,1> const  & __a_rename_at_586_3 );
inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_a6123e6697ded437 ( Context * __context__, TDim<uint8_t,2> & __a_rename_at_1352_4 );
inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_dd87c6f1da83ea06 ( Context * __context__, TDim<uint8_t,3> & __a_rename_at_1352_6 );
inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_4c32e98c64dc9655 ( Context * __context__, TDim<uint8_t,4> & __a_rename_at_1352_8 );
inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_52af64c8664bb146 ( Context * __context__, TDim<uint8_t,1> & __a_rename_at_1352_10 );
inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_26755ce4d233ca82 ( Context * __context__, TArray<uint8_t> & __a_rename_at_50_12 );
inline TArray<uint32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3de422272adba695 ( Context * __context__, TArray<uint32_t> & __a_rename_at_50_13 );
inline void _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_14, uint8_t __value_rename_at_165_15 );
inline void _FuncbuiltinTickreserveTick3994685146752941225_d0790c6d74061d34 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_125_16, int32_t __newSize_rename_at_125_17 );
inline void _FuncbuiltinTickpushTick10769833213962245646_c46ce71d6a41166a ( Context * __context__, TArray<uint32_t> & __Arr_rename_at_181_18, uint32_t __value_rename_at_181_19 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_71b1749d234f576e ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_20, int32_t __newSize_rename_at_68_21 );
inline uint32_t utf16_to_utf32_67298994a5baa194 ( Context * __context__, uint32_t __high_rename_at_10_22, uint32_t __low_rename_at_10_23 );
inline void utf8_encode_b6fe59c3abe9a444 ( Context * __context__, TArray<uint8_t> & __dest_array_rename_at_18_24, uint32_t __ch_rename_at_18_25 );
inline TArray<uint8_t> utf8_encode_97b551f35cf80978 ( Context * __context__, uint32_t __ch_rename_at_38_26 );
inline void utf8_encode_e2de5a2c582774dc ( Context * __context__, TArray<uint8_t> & __dest_array_rename_at_61_27, TArray<uint32_t> const  & __source_utf32_string_rename_at_61_28 );
inline TArray<uint8_t> utf8_encode_5c19a2da34c8f208 ( Context * __context__, TArray<uint32_t> const  & __source_utf32_string_rename_at_69_30 );
inline int32_t utf8_length_c0cb690c545d23a5 ( Context * __context__, TArray<uint8_t> const  & __utf8_string_rename_at_80_33 );
inline int32_t utf8_length_9bc85c1c74b26c7e ( Context * __context__, char * const  __utf8_string_rename_at_92_36 );
inline bool is_first_byte_of_utf8_char_26620640f6460cdb ( Context * __context__, uint8_t __ch_rename_at_104_39 );
inline bool contains_utf8_bom_f2cf32cf55e0a6d4 ( Context * __context__, TArray<uint8_t> const  & __utf8_string_rename_at_110_41 );
inline bool contains_utf8_bom_d5ae1a8e5b1e79db ( Context * __context__, char * const  __utf8_string_rename_at_116_42 );
inline bool is_utf8_string_valid_6da0450b19500e4d ( Context * __context__, TArray<uint8_t> const  & __utf8_string_rename_at_154_43 );
inline bool is_utf8_string_valid_bcfa16997a4edaaf ( Context * __context__, char * const  __utf8_string_rename_at_167_49 );
inline void utf8_decode_16c21049b073c3e2 ( Context * __context__, TArray<uint32_t> & __dest_utf32_string_rename_at_180_55, TArray<uint8_t> const  & __source_utf8_string_rename_at_180_56 );
inline TArray<uint32_t> utf8_decode_d1ca0cab78c6a9ea ( Context * __context__, TArray<uint8_t> const  & __source_utf8_string_rename_at_199_63 );
inline TArray<uint32_t> utf8_decode_1df490f6da36ae8 ( Context * __context__, char * const  __source_utf8_string_rename_at_207_65 );
inline void utf8_decode_233a82235363f092 ( Context * __context__, TArray<uint32_t> & __dest_utf32_string_rename_at_217_68, char * const  __source_utf8_string_rename_at_217_69 );
inline char * decode_unicode_escape_511f3efa8f121c21 ( Context * __context__, char * const  __str_rename_at_224_71 );

void __init_script ( Context * __context__, bool __init_shared )
{
    if ( __init_shared ) das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) = (([&]() -> TDim<uint32_t,364> {
        TDim<uint32_t,364> __mka_130;
        __mka_130(0,__context__) = 0x0u;
        __mka_130(1,__context__) = 0x0u;
        __mka_130(2,__context__) = 0x0u;
        __mka_130(3,__context__) = 0x0u;
        __mka_130(4,__context__) = 0x0u;
        __mka_130(5,__context__) = 0x0u;
        __mka_130(6,__context__) = 0x0u;
        __mka_130(7,__context__) = 0x0u;
        __mka_130(8,__context__) = 0x0u;
        __mka_130(9,__context__) = 0x0u;
        __mka_130(10,__context__) = 0x0u;
        __mka_130(11,__context__) = 0x0u;
        __mka_130(12,__context__) = 0x0u;
        __mka_130(13,__context__) = 0x0u;
        __mka_130(14,__context__) = 0x0u;
        __mka_130(15,__context__) = 0x0u;
        __mka_130(16,__context__) = 0x0u;
        __mka_130(17,__context__) = 0x0u;
        __mka_130(18,__context__) = 0x0u;
        __mka_130(19,__context__) = 0x0u;
        __mka_130(20,__context__) = 0x0u;
        __mka_130(21,__context__) = 0x0u;
        __mka_130(22,__context__) = 0x0u;
        __mka_130(23,__context__) = 0x0u;
        __mka_130(24,__context__) = 0x0u;
        __mka_130(25,__context__) = 0x0u;
        __mka_130(26,__context__) = 0x0u;
        __mka_130(27,__context__) = 0x0u;
        __mka_130(28,__context__) = 0x0u;
        __mka_130(29,__context__) = 0x0u;
        __mka_130(30,__context__) = 0x0u;
        __mka_130(31,__context__) = 0x0u;
        __mka_130(32,__context__) = 0x0u;
        __mka_130(33,__context__) = 0x0u;
        __mka_130(34,__context__) = 0x0u;
        __mka_130(35,__context__) = 0x0u;
        __mka_130(36,__context__) = 0x0u;
        __mka_130(37,__context__) = 0x0u;
        __mka_130(38,__context__) = 0x0u;
        __mka_130(39,__context__) = 0x0u;
        __mka_130(40,__context__) = 0x0u;
        __mka_130(41,__context__) = 0x0u;
        __mka_130(42,__context__) = 0x0u;
        __mka_130(43,__context__) = 0x0u;
        __mka_130(44,__context__) = 0x0u;
        __mka_130(45,__context__) = 0x0u;
        __mka_130(46,__context__) = 0x0u;
        __mka_130(47,__context__) = 0x0u;
        __mka_130(48,__context__) = 0x0u;
        __mka_130(49,__context__) = 0x0u;
        __mka_130(50,__context__) = 0x0u;
        __mka_130(51,__context__) = 0x0u;
        __mka_130(52,__context__) = 0x0u;
        __mka_130(53,__context__) = 0x0u;
        __mka_130(54,__context__) = 0x0u;
        __mka_130(55,__context__) = 0x0u;
        __mka_130(56,__context__) = 0x0u;
        __mka_130(57,__context__) = 0x0u;
        __mka_130(58,__context__) = 0x0u;
        __mka_130(59,__context__) = 0x0u;
        __mka_130(60,__context__) = 0x0u;
        __mka_130(61,__context__) = 0x0u;
        __mka_130(62,__context__) = 0x0u;
        __mka_130(63,__context__) = 0x0u;
        __mka_130(64,__context__) = 0x0u;
        __mka_130(65,__context__) = 0x0u;
        __mka_130(66,__context__) = 0x0u;
        __mka_130(67,__context__) = 0x0u;
        __mka_130(68,__context__) = 0x0u;
        __mka_130(69,__context__) = 0x0u;
        __mka_130(70,__context__) = 0x0u;
        __mka_130(71,__context__) = 0x0u;
        __mka_130(72,__context__) = 0x0u;
        __mka_130(73,__context__) = 0x0u;
        __mka_130(74,__context__) = 0x0u;
        __mka_130(75,__context__) = 0x0u;
        __mka_130(76,__context__) = 0x0u;
        __mka_130(77,__context__) = 0x0u;
        __mka_130(78,__context__) = 0x0u;
        __mka_130(79,__context__) = 0x0u;
        __mka_130(80,__context__) = 0x0u;
        __mka_130(81,__context__) = 0x0u;
        __mka_130(82,__context__) = 0x0u;
        __mka_130(83,__context__) = 0x0u;
        __mka_130(84,__context__) = 0x0u;
        __mka_130(85,__context__) = 0x0u;
        __mka_130(86,__context__) = 0x0u;
        __mka_130(87,__context__) = 0x0u;
        __mka_130(88,__context__) = 0x0u;
        __mka_130(89,__context__) = 0x0u;
        __mka_130(90,__context__) = 0x0u;
        __mka_130(91,__context__) = 0x0u;
        __mka_130(92,__context__) = 0x0u;
        __mka_130(93,__context__) = 0x0u;
        __mka_130(94,__context__) = 0x0u;
        __mka_130(95,__context__) = 0x0u;
        __mka_130(96,__context__) = 0x0u;
        __mka_130(97,__context__) = 0x0u;
        __mka_130(98,__context__) = 0x0u;
        __mka_130(99,__context__) = 0x0u;
        __mka_130(100,__context__) = 0x0u;
        __mka_130(101,__context__) = 0x0u;
        __mka_130(102,__context__) = 0x0u;
        __mka_130(103,__context__) = 0x0u;
        __mka_130(104,__context__) = 0x0u;
        __mka_130(105,__context__) = 0x0u;
        __mka_130(106,__context__) = 0x0u;
        __mka_130(107,__context__) = 0x0u;
        __mka_130(108,__context__) = 0x0u;
        __mka_130(109,__context__) = 0x0u;
        __mka_130(110,__context__) = 0x0u;
        __mka_130(111,__context__) = 0x0u;
        __mka_130(112,__context__) = 0x0u;
        __mka_130(113,__context__) = 0x0u;
        __mka_130(114,__context__) = 0x0u;
        __mka_130(115,__context__) = 0x0u;
        __mka_130(116,__context__) = 0x0u;
        __mka_130(117,__context__) = 0x0u;
        __mka_130(118,__context__) = 0x0u;
        __mka_130(119,__context__) = 0x0u;
        __mka_130(120,__context__) = 0x0u;
        __mka_130(121,__context__) = 0x0u;
        __mka_130(122,__context__) = 0x0u;
        __mka_130(123,__context__) = 0x0u;
        __mka_130(124,__context__) = 0x0u;
        __mka_130(125,__context__) = 0x0u;
        __mka_130(126,__context__) = 0x0u;
        __mka_130(127,__context__) = 0x0u;
        __mka_130(128,__context__) = 0x1u;
        __mka_130(129,__context__) = 0x1u;
        __mka_130(130,__context__) = 0x1u;
        __mka_130(131,__context__) = 0x1u;
        __mka_130(132,__context__) = 0x1u;
        __mka_130(133,__context__) = 0x1u;
        __mka_130(134,__context__) = 0x1u;
        __mka_130(135,__context__) = 0x1u;
        __mka_130(136,__context__) = 0x1u;
        __mka_130(137,__context__) = 0x1u;
        __mka_130(138,__context__) = 0x1u;
        __mka_130(139,__context__) = 0x1u;
        __mka_130(140,__context__) = 0x1u;
        __mka_130(141,__context__) = 0x1u;
        __mka_130(142,__context__) = 0x1u;
        __mka_130(143,__context__) = 0x1u;
        __mka_130(144,__context__) = 0x9u;
        __mka_130(145,__context__) = 0x9u;
        __mka_130(146,__context__) = 0x9u;
        __mka_130(147,__context__) = 0x9u;
        __mka_130(148,__context__) = 0x9u;
        __mka_130(149,__context__) = 0x9u;
        __mka_130(150,__context__) = 0x9u;
        __mka_130(151,__context__) = 0x9u;
        __mka_130(152,__context__) = 0x9u;
        __mka_130(153,__context__) = 0x9u;
        __mka_130(154,__context__) = 0x9u;
        __mka_130(155,__context__) = 0x9u;
        __mka_130(156,__context__) = 0x9u;
        __mka_130(157,__context__) = 0x9u;
        __mka_130(158,__context__) = 0x9u;
        __mka_130(159,__context__) = 0x9u;
        __mka_130(160,__context__) = 0x7u;
        __mka_130(161,__context__) = 0x7u;
        __mka_130(162,__context__) = 0x7u;
        __mka_130(163,__context__) = 0x7u;
        __mka_130(164,__context__) = 0x7u;
        __mka_130(165,__context__) = 0x7u;
        __mka_130(166,__context__) = 0x7u;
        __mka_130(167,__context__) = 0x7u;
        __mka_130(168,__context__) = 0x7u;
        __mka_130(169,__context__) = 0x7u;
        __mka_130(170,__context__) = 0x7u;
        __mka_130(171,__context__) = 0x7u;
        __mka_130(172,__context__) = 0x7u;
        __mka_130(173,__context__) = 0x7u;
        __mka_130(174,__context__) = 0x7u;
        __mka_130(175,__context__) = 0x7u;
        __mka_130(176,__context__) = 0x7u;
        __mka_130(177,__context__) = 0x7u;
        __mka_130(178,__context__) = 0x7u;
        __mka_130(179,__context__) = 0x7u;
        __mka_130(180,__context__) = 0x7u;
        __mka_130(181,__context__) = 0x7u;
        __mka_130(182,__context__) = 0x7u;
        __mka_130(183,__context__) = 0x7u;
        __mka_130(184,__context__) = 0x7u;
        __mka_130(185,__context__) = 0x7u;
        __mka_130(186,__context__) = 0x7u;
        __mka_130(187,__context__) = 0x7u;
        __mka_130(188,__context__) = 0x7u;
        __mka_130(189,__context__) = 0x7u;
        __mka_130(190,__context__) = 0x7u;
        __mka_130(191,__context__) = 0x7u;
        __mka_130(192,__context__) = 0x8u;
        __mka_130(193,__context__) = 0x8u;
        __mka_130(194,__context__) = 0x2u;
        __mka_130(195,__context__) = 0x2u;
        __mka_130(196,__context__) = 0x2u;
        __mka_130(197,__context__) = 0x2u;
        __mka_130(198,__context__) = 0x2u;
        __mka_130(199,__context__) = 0x2u;
        __mka_130(200,__context__) = 0x2u;
        __mka_130(201,__context__) = 0x2u;
        __mka_130(202,__context__) = 0x2u;
        __mka_130(203,__context__) = 0x2u;
        __mka_130(204,__context__) = 0x2u;
        __mka_130(205,__context__) = 0x2u;
        __mka_130(206,__context__) = 0x2u;
        __mka_130(207,__context__) = 0x2u;
        __mka_130(208,__context__) = 0x2u;
        __mka_130(209,__context__) = 0x2u;
        __mka_130(210,__context__) = 0x2u;
        __mka_130(211,__context__) = 0x2u;
        __mka_130(212,__context__) = 0x2u;
        __mka_130(213,__context__) = 0x2u;
        __mka_130(214,__context__) = 0x2u;
        __mka_130(215,__context__) = 0x2u;
        __mka_130(216,__context__) = 0x2u;
        __mka_130(217,__context__) = 0x2u;
        __mka_130(218,__context__) = 0x2u;
        __mka_130(219,__context__) = 0x2u;
        __mka_130(220,__context__) = 0x2u;
        __mka_130(221,__context__) = 0x2u;
        __mka_130(222,__context__) = 0x2u;
        __mka_130(223,__context__) = 0x2u;
        __mka_130(224,__context__) = 0xau;
        __mka_130(225,__context__) = 0x3u;
        __mka_130(226,__context__) = 0x3u;
        __mka_130(227,__context__) = 0x3u;
        __mka_130(228,__context__) = 0x3u;
        __mka_130(229,__context__) = 0x3u;
        __mka_130(230,__context__) = 0x3u;
        __mka_130(231,__context__) = 0x3u;
        __mka_130(232,__context__) = 0x3u;
        __mka_130(233,__context__) = 0x3u;
        __mka_130(234,__context__) = 0x3u;
        __mka_130(235,__context__) = 0x3u;
        __mka_130(236,__context__) = 0x3u;
        __mka_130(237,__context__) = 0x4u;
        __mka_130(238,__context__) = 0x3u;
        __mka_130(239,__context__) = 0x3u;
        __mka_130(240,__context__) = 0xbu;
        __mka_130(241,__context__) = 0x6u;
        __mka_130(242,__context__) = 0x6u;
        __mka_130(243,__context__) = 0x6u;
        __mka_130(244,__context__) = 0x5u;
        __mka_130(245,__context__) = 0x8u;
        __mka_130(246,__context__) = 0x8u;
        __mka_130(247,__context__) = 0x8u;
        __mka_130(248,__context__) = 0x8u;
        __mka_130(249,__context__) = 0x8u;
        __mka_130(250,__context__) = 0x8u;
        __mka_130(251,__context__) = 0x8u;
        __mka_130(252,__context__) = 0x8u;
        __mka_130(253,__context__) = 0x8u;
        __mka_130(254,__context__) = 0x8u;
        __mka_130(255,__context__) = 0x8u;
        __mka_130(256,__context__) = 0x0u;
        __mka_130(257,__context__) = 0xcu;
        __mka_130(258,__context__) = 0x18u;
        __mka_130(259,__context__) = 0x24u;
        __mka_130(260,__context__) = 0x3cu;
        __mka_130(261,__context__) = 0x60u;
        __mka_130(262,__context__) = 0x54u;
        __mka_130(263,__context__) = 0xcu;
        __mka_130(264,__context__) = 0xcu;
        __mka_130(265,__context__) = 0xcu;
        __mka_130(266,__context__) = 0x30u;
        __mka_130(267,__context__) = 0x48u;
        __mka_130(268,__context__) = 0xcu;
        __mka_130(269,__context__) = 0xcu;
        __mka_130(270,__context__) = 0xcu;
        __mka_130(271,__context__) = 0xcu;
        __mka_130(272,__context__) = 0xcu;
        __mka_130(273,__context__) = 0xcu;
        __mka_130(274,__context__) = 0xcu;
        __mka_130(275,__context__) = 0xcu;
        __mka_130(276,__context__) = 0xcu;
        __mka_130(277,__context__) = 0xcu;
        __mka_130(278,__context__) = 0xcu;
        __mka_130(279,__context__) = 0xcu;
        __mka_130(280,__context__) = 0xcu;
        __mka_130(281,__context__) = 0x0u;
        __mka_130(282,__context__) = 0xcu;
        __mka_130(283,__context__) = 0xcu;
        __mka_130(284,__context__) = 0xcu;
        __mka_130(285,__context__) = 0xcu;
        __mka_130(286,__context__) = 0xcu;
        __mka_130(287,__context__) = 0x0u;
        __mka_130(288,__context__) = 0xcu;
        __mka_130(289,__context__) = 0x0u;
        __mka_130(290,__context__) = 0xcu;
        __mka_130(291,__context__) = 0xcu;
        __mka_130(292,__context__) = 0xcu;
        __mka_130(293,__context__) = 0x18u;
        __mka_130(294,__context__) = 0xcu;
        __mka_130(295,__context__) = 0xcu;
        __mka_130(296,__context__) = 0xcu;
        __mka_130(297,__context__) = 0xcu;
        __mka_130(298,__context__) = 0xcu;
        __mka_130(299,__context__) = 0x18u;
        __mka_130(300,__context__) = 0xcu;
        __mka_130(301,__context__) = 0x18u;
        __mka_130(302,__context__) = 0xcu;
        __mka_130(303,__context__) = 0xcu;
        __mka_130(304,__context__) = 0xcu;
        __mka_130(305,__context__) = 0xcu;
        __mka_130(306,__context__) = 0xcu;
        __mka_130(307,__context__) = 0xcu;
        __mka_130(308,__context__) = 0xcu;
        __mka_130(309,__context__) = 0xcu;
        __mka_130(310,__context__) = 0xcu;
        __mka_130(311,__context__) = 0x18u;
        __mka_130(312,__context__) = 0xcu;
        __mka_130(313,__context__) = 0xcu;
        __mka_130(314,__context__) = 0xcu;
        __mka_130(315,__context__) = 0xcu;
        __mka_130(316,__context__) = 0xcu;
        __mka_130(317,__context__) = 0x18u;
        __mka_130(318,__context__) = 0xcu;
        __mka_130(319,__context__) = 0xcu;
        __mka_130(320,__context__) = 0xcu;
        __mka_130(321,__context__) = 0xcu;
        __mka_130(322,__context__) = 0xcu;
        __mka_130(323,__context__) = 0xcu;
        __mka_130(324,__context__) = 0xcu;
        __mka_130(325,__context__) = 0x18u;
        __mka_130(326,__context__) = 0xcu;
        __mka_130(327,__context__) = 0xcu;
        __mka_130(328,__context__) = 0xcu;
        __mka_130(329,__context__) = 0xcu;
        __mka_130(330,__context__) = 0xcu;
        __mka_130(331,__context__) = 0xcu;
        __mka_130(332,__context__) = 0xcu;
        __mka_130(333,__context__) = 0xcu;
        __mka_130(334,__context__) = 0xcu;
        __mka_130(335,__context__) = 0x24u;
        __mka_130(336,__context__) = 0xcu;
        __mka_130(337,__context__) = 0x24u;
        __mka_130(338,__context__) = 0xcu;
        __mka_130(339,__context__) = 0xcu;
        __mka_130(340,__context__) = 0xcu;
        __mka_130(341,__context__) = 0x24u;
        __mka_130(342,__context__) = 0xcu;
        __mka_130(343,__context__) = 0xcu;
        __mka_130(344,__context__) = 0xcu;
        __mka_130(345,__context__) = 0xcu;
        __mka_130(346,__context__) = 0xcu;
        __mka_130(347,__context__) = 0x24u;
        __mka_130(348,__context__) = 0xcu;
        __mka_130(349,__context__) = 0x24u;
        __mka_130(350,__context__) = 0xcu;
        __mka_130(351,__context__) = 0xcu;
        __mka_130(352,__context__) = 0xcu;
        __mka_130(353,__context__) = 0x24u;
        __mka_130(354,__context__) = 0xcu;
        __mka_130(355,__context__) = 0xcu;
        __mka_130(356,__context__) = 0xcu;
        __mka_130(357,__context__) = 0xcu;
        __mka_130(358,__context__) = 0xcu;
        __mka_130(359,__context__) = 0xcu;
        __mka_130(360,__context__) = 0xcu;
        __mka_130(361,__context__) = 0xcu;
        __mka_130(362,__context__) = 0xcu;
        __mka_130(363,__context__) = 0xcu;
        return __mka_130;
    })());/*s_utf8d*/
    das_global<uint32_t,0x32e75a6a55963340>(__context__) = 0x0u;/*UTF8_ACCEPT*/
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3dbee92a7efb07d9 ( Context * __context__, TDim<uint8_t,2> const  &  __a_rename_at_586_0 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_194914ba4b4e9234 ( Context * __context__, TDim<uint8_t,3> const  &  __a_rename_at_586_1 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3d13e435f27e824d ( Context * __context__, TDim<uint8_t,4> const  &  __a_rename_at_586_2 )
{
    return das_auto_cast<int32_t>::cast(4);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_a94ae7fe5f83f893 ( Context * __context__, TDim<uint8_t,1> const  &  __a_rename_at_586_3 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_a6123e6697ded437 ( Context * __context__, TDim<uint8_t,2> &  __a_rename_at_1352_4 )
{
    TArray<uint8_t> __arr_rename_at_1354_5;das_zero(__arr_rename_at_1354_5);
    _FuncbuiltinTickresizeTick4811697762258667383_71b1749d234f576e(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_1354_5),2);
    das_copy(das_cast<TDim<uint8_t,2>>::cast(das_ref(__context__,__arr_rename_at_1354_5(0,__context__))),__a_rename_at_1352_4);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__arr_rename_at_1354_5);
}

inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_dd87c6f1da83ea06 ( Context * __context__, TDim<uint8_t,3> &  __a_rename_at_1352_6 )
{
    TArray<uint8_t> __arr_rename_at_1354_7;das_zero(__arr_rename_at_1354_7);
    _FuncbuiltinTickresizeTick4811697762258667383_71b1749d234f576e(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_1354_7),3);
    das_copy(das_cast<TDim<uint8_t,3>>::cast(das_ref(__context__,__arr_rename_at_1354_7(0,__context__))),__a_rename_at_1352_6);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__arr_rename_at_1354_7);
}

inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_4c32e98c64dc9655 ( Context * __context__, TDim<uint8_t,4> &  __a_rename_at_1352_8 )
{
    TArray<uint8_t> __arr_rename_at_1354_9;das_zero(__arr_rename_at_1354_9);
    _FuncbuiltinTickresizeTick4811697762258667383_71b1749d234f576e(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_1354_9),4);
    das_copy(das_cast<TDim<uint8_t,4>>::cast(das_ref(__context__,__arr_rename_at_1354_9(0,__context__))),__a_rename_at_1352_8);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__arr_rename_at_1354_9);
}

inline TArray<uint8_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_52af64c8664bb146 ( Context * __context__, TDim<uint8_t,1> &  __a_rename_at_1352_10 )
{
    TArray<uint8_t> __arr_rename_at_1354_11;das_zero(__arr_rename_at_1354_11);
    _FuncbuiltinTickresizeTick4811697762258667383_71b1749d234f576e(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_1354_11),1);
    das_copy(das_cast<TDim<uint8_t,1>>::cast(das_ref(__context__,__arr_rename_at_1354_11(0,__context__))),__a_rename_at_1352_10);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__arr_rename_at_1354_11);
}

inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_26755ce4d233ca82 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_50_12 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<uint8_t> &>::from(__a_rename_at_50_12)));
    return das_auto_cast_ref<TArray<uint8_t> &>::cast(__a_rename_at_50_12);
}

inline TArray<uint32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3de422272adba695 ( Context * __context__, TArray<uint32_t> &  __a_rename_at_50_13 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TArray<uint32_t> &>::from(__a_rename_at_50_13)));
    return das_auto_cast_ref<TArray<uint32_t> &>::cast(__a_rename_at_50_13);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_14, uint8_t __value_rename_at_165_15 )
{
    das_copy(__Arr_rename_at_165_14(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_14),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_15);
}

inline void _FuncbuiltinTickreserveTick3994685146752941225_d0790c6d74061d34 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_125_16, int32_t __newSize_rename_at_125_17 )
{
    builtin_array_reserve(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_125_16),__newSize_rename_at_125_17,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_c46ce71d6a41166a ( Context * __context__, TArray<uint32_t> &  __Arr_rename_at_181_18, uint32_t __value_rename_at_181_19 )
{
    das_copy(__Arr_rename_at_181_18(builtin_array_push_back(das_arg<TArray<uint32_t>>::pass(__Arr_rename_at_181_18),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_19);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_71b1749d234f576e ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_20, int32_t __newSize_rename_at_68_21 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_20),__newSize_rename_at_68_21,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline uint32_t utf16_to_utf32_67298994a5baa194 ( Context * __context__, uint32_t __high_rename_at_10_22, uint32_t __low_rename_at_10_23 )
{
    return das_auto_cast<uint32_t>::cast(((__high_rename_at_10_22 >= 0xd800u) && (__high_rename_at_10_22 <= 0xdbffu)) ? das_auto_cast<uint32_t>::cast(((((__high_rename_at_10_22 - 0xd800u) << 0xau) + (__low_rename_at_10_23 - 0xdc00u)) + 0x10000u)) : das_auto_cast<uint32_t>::cast(__high_rename_at_10_22));
}

inline void utf8_encode_b6fe59c3abe9a444 ( Context * __context__, TArray<uint8_t> &  __dest_array_rename_at_18_24, uint32_t __ch_rename_at_18_25 )
{
    if ( __ch_rename_at_18_25 < 0x80u )
    {
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t(__ch_rename_at_18_25));
    } else if ( __ch_rename_at_18_25 < 0x800u )
    {
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t((__ch_rename_at_18_25 >> 0x6u) + 0xc0u));
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t((__ch_rename_at_18_25 & 0x3fu) + 0x80u));
    } else if ( __ch_rename_at_18_25 < 0x10000u )
    {
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t((__ch_rename_at_18_25 >> 0xcu) + 0xe0u));
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t(((__ch_rename_at_18_25 >> 0x6u) & 0x3fu) + 0x80u));
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t((__ch_rename_at_18_25 & 0x3fu) + 0x80u));
    } else {
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t((__ch_rename_at_18_25 >> 0x12u) + 0xf0u));
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t(((__ch_rename_at_18_25 >> 0xcu) & 0x3fu) + 0x80u));
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t(((__ch_rename_at_18_25 >> 0x6u) & 0x3fu) + 0x80u));
        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_18_24),uint8_t((__ch_rename_at_18_25 & 0x3fu) + 0x80u));
    };
}

inline TArray<uint8_t> utf8_encode_97b551f35cf80978 ( Context * __context__, uint32_t __ch_rename_at_38_26 )
{
    if ( __ch_rename_at_38_26 < 0x80u )
    {
        TArray<uint8_t> _temp_make_local_41_19_0; _temp_make_local_41_19_0;
        TDim<uint8_t,1> _temp_make_local_41_19_1; _temp_make_local_41_19_1;
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_26755ce4d233ca82(__context__,das_arg<TArray<uint8_t>>::pass((_temp_make_local_41_19_0 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_52af64c8664bb146(__context__,das_arg<TDim<uint8_t,1>>::pass((([&]() -> TDim<uint8_t,1>& {
            _temp_make_local_41_19_1(0,__context__) = uint8_t(__ch_rename_at_38_26);
            return _temp_make_local_41_19_1;
        })()))))))));
    } else if ( __ch_rename_at_38_26 < 0x800u )
    {
        TArray<uint8_t> _temp_make_local_43_19_2; _temp_make_local_43_19_2;
        TDim<uint8_t,2> _temp_make_local_43_19_3; _temp_make_local_43_19_3;
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_26755ce4d233ca82(__context__,das_arg<TArray<uint8_t>>::pass((_temp_make_local_43_19_2 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_a6123e6697ded437(__context__,das_arg<TDim<uint8_t,2>>::pass((([&]() -> TDim<uint8_t,2>& {
            _temp_make_local_43_19_3(0,__context__) = uint8_t((__ch_rename_at_38_26 >> 0x6u) + 0xc0u);
            _temp_make_local_43_19_3(1,__context__) = uint8_t((__ch_rename_at_38_26 & 0x3fu) + 0x80u);
            return _temp_make_local_43_19_3;
        })()))))))));
    } else if ( __ch_rename_at_38_26 < 0x10000u )
    {
        TArray<uint8_t> _temp_make_local_47_19_4; _temp_make_local_47_19_4;
        TDim<uint8_t,3> _temp_make_local_47_19_5; _temp_make_local_47_19_5;
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_26755ce4d233ca82(__context__,das_arg<TArray<uint8_t>>::pass((_temp_make_local_47_19_4 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_dd87c6f1da83ea06(__context__,das_arg<TDim<uint8_t,3>>::pass((([&]() -> TDim<uint8_t,3>& {
            _temp_make_local_47_19_5(0,__context__) = uint8_t((__ch_rename_at_38_26 >> 0xcu) + 0xe0u);
            _temp_make_local_47_19_5(1,__context__) = uint8_t(((__ch_rename_at_38_26 >> 0x6u) & 0x3fu) + 0x80u);
            _temp_make_local_47_19_5(2,__context__) = uint8_t((__ch_rename_at_38_26 & 0x3fu) + 0x80u);
            return _temp_make_local_47_19_5;
        })()))))))));
    } else {
        TArray<uint8_t> _temp_make_local_52_19_6; _temp_make_local_52_19_6;
        TDim<uint8_t,4> _temp_make_local_52_19_7; _temp_make_local_52_19_7;
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_26755ce4d233ca82(__context__,das_arg<TArray<uint8_t>>::pass((_temp_make_local_52_19_6 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_4c32e98c64dc9655(__context__,das_arg<TDim<uint8_t,4>>::pass((([&]() -> TDim<uint8_t,4>& {
            _temp_make_local_52_19_7(0,__context__) = uint8_t((__ch_rename_at_38_26 >> 0x12u) + 0xf0u);
            _temp_make_local_52_19_7(1,__context__) = uint8_t(((__ch_rename_at_38_26 >> 0xcu) & 0x3fu) + 0x80u);
            _temp_make_local_52_19_7(2,__context__) = uint8_t(((__ch_rename_at_38_26 >> 0x6u) & 0x3fu) + 0x80u);
            _temp_make_local_52_19_7(3,__context__) = uint8_t((__ch_rename_at_38_26 & 0x3fu) + 0x80u);
            return _temp_make_local_52_19_7;
        })()))))))));
    };
}

inline void utf8_encode_e2de5a2c582774dc ( Context * __context__, TArray<uint8_t> &  __dest_array_rename_at_61_27, TArray<uint32_t> const  &  __source_utf32_string_rename_at_61_28 )
{
    {
        bool __need_loop_63 = true;
        // ch: uint const&
        das_iterator<TArray<uint32_t> const > __ch_iterator(__source_utf32_string_rename_at_61_28);
        uint32_t const  * __ch_rename_at_63_29;
        __need_loop_63 = __ch_iterator.first(__context__,(__ch_rename_at_63_29)) && __need_loop_63;
        for ( ; __need_loop_63 ; __need_loop_63 = __ch_iterator.next(__context__,(__ch_rename_at_63_29)) )
        {
            utf8_encode_b6fe59c3abe9a444(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_61_27),(*__ch_rename_at_63_29));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_63_29));
    };
}

inline TArray<uint8_t> utf8_encode_5c19a2da34c8f208 ( Context * __context__, TArray<uint32_t> const  &  __source_utf32_string_rename_at_69_30 )
{
    TArray<uint8_t> __dest_array_rename_at_71_31;das_zero(__dest_array_rename_at_71_31);
    _FuncbuiltinTickreserveTick3994685146752941225_d0790c6d74061d34(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_71_31),builtin_array_size(__source_utf32_string_rename_at_69_30));
    {
        bool __need_loop_73 = true;
        // ch: uint const&
        das_iterator<TArray<uint32_t> const > __ch_iterator(__source_utf32_string_rename_at_69_30);
        uint32_t const  * __ch_rename_at_73_32;
        __need_loop_73 = __ch_iterator.first(__context__,(__ch_rename_at_73_32)) && __need_loop_73;
        for ( ; __need_loop_73 ; __need_loop_73 = __ch_iterator.next(__context__,(__ch_rename_at_73_32)) )
        {
            utf8_encode_b6fe59c3abe9a444(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_71_31),(*__ch_rename_at_73_32));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_73_32));
    };
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_26755ce4d233ca82(__context__,das_arg<TArray<uint8_t>>::pass(__dest_array_rename_at_71_31)));
}

inline int32_t utf8_length_c0cb690c545d23a5 ( Context * __context__, TArray<uint8_t> const  &  __utf8_string_rename_at_80_33 )
{
    int32_t __length_rename_at_82_34 = 0;
    {
        bool __need_loop_83 = true;
        // ch: uint8 const&
        das_iterator<TArray<uint8_t> const > __ch_iterator(__utf8_string_rename_at_80_33);
        uint8_t const  * __ch_rename_at_83_35;
        __need_loop_83 = __ch_iterator.first(__context__,(__ch_rename_at_83_35)) && __need_loop_83;
        for ( ; __need_loop_83 ; __need_loop_83 = __ch_iterator.next(__context__,(__ch_rename_at_83_35)) )
        {
            if ( (uint32_t((*__ch_rename_at_83_35)) & 0xc0u) != 0x80u )
            {
                ++__length_rename_at_82_34;
            };
        }
        __ch_iterator.close(__context__,(__ch_rename_at_83_35));
    };
    return das_auto_cast<int32_t>::cast(__length_rename_at_82_34);
}

inline int32_t utf8_length_9bc85c1c74b26c7e ( Context * __context__, char * const  __utf8_string_rename_at_92_36 )
{
    int32_t __length_rename_at_94_37 = 0;
    {
        bool __need_loop_95 = true;
        // ch: int const
        das_iterator<char * const > __ch_iterator(__utf8_string_rename_at_92_36);
        int32_t __ch_rename_at_95_38;
        __need_loop_95 = __ch_iterator.first(__context__,(__ch_rename_at_95_38)) && __need_loop_95;
        for ( ; __need_loop_95 ; __need_loop_95 = __ch_iterator.next(__context__,(__ch_rename_at_95_38)) )
        {
            if ( (uint32_t(__ch_rename_at_95_38) & 0xc0u) != 0x80u )
            {
                ++__length_rename_at_94_37;
            };
        }
        __ch_iterator.close(__context__,(__ch_rename_at_95_38));
    };
    return das_auto_cast<int32_t>::cast(__length_rename_at_94_37);
}

inline bool is_first_byte_of_utf8_char_26620640f6460cdb ( Context * __context__, uint8_t __ch_rename_at_104_39 )
{
    uint32_t __x_rename_at_105_40 = ((uint32_t)uint32_t(__ch_rename_at_104_39));
    return das_auto_cast<bool>::cast((__x_rename_at_105_40 > 0x0u) && ((__x_rename_at_105_40 < 0x80u) || ((__x_rename_at_105_40 & 0xc0u) == 0xc0u)));
}

inline bool contains_utf8_bom_f2cf32cf55e0a6d4 ( Context * __context__, TArray<uint8_t> const  &  __utf8_string_rename_at_110_41 )
{
    return das_auto_cast<bool>::cast((((builtin_array_size(__utf8_string_rename_at_110_41) >= 3) && (uint32_t(__utf8_string_rename_at_110_41(0,__context__)) == 0xefu)) && (uint32_t(__utf8_string_rename_at_110_41(1,__context__)) == 0xbbu)) && (uint32_t(__utf8_string_rename_at_110_41(2,__context__)) == 0xbfu));
}

inline bool contains_utf8_bom_d5ae1a8e5b1e79db ( Context * __context__, char * const  __utf8_string_rename_at_116_42 )
{
    return das_auto_cast<bool>::cast(((!(builtin_empty(__utf8_string_rename_at_116_42)) && (uint32_t(get_character_uat(__utf8_string_rename_at_116_42,0)) == 0xefu)) && (uint32_t(get_character_uat(__utf8_string_rename_at_116_42,1)) == 0xbbu)) && (uint32_t(get_character_uat(__utf8_string_rename_at_116_42,2)) == 0xbfu));
}

inline bool is_utf8_string_valid_6da0450b19500e4d ( Context * __context__, TArray<uint8_t> const  &  __utf8_string_rename_at_154_43 )
{
    uint32_t __codepoint_rename_at_155_44 = 0x0u;
    uint32_t __state_rename_at_156_45 = 0x0u;
    {
        bool __need_loop_157 = true;
        // ch: uint8 const&
        das_iterator<TArray<uint8_t> const > __ch_iterator(__utf8_string_rename_at_154_43);
        uint8_t const  * __ch_rename_at_157_46;
        __need_loop_157 = __ch_iterator.first(__context__,(__ch_rename_at_157_46)) && __need_loop_157;
        for ( ; __need_loop_157 ; __need_loop_157 = __ch_iterator.next(__context__,(__ch_rename_at_157_46)) )
        {
            uint32_t __byte_rename_at_158_47 = ((uint32_t)uint32_t((*__ch_rename_at_157_46)));
            uint32_t __type__rename_at_159_48 = ((uint32_t)das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(__byte_rename_at_158_47,__context__));
            das_copy(__codepoint_rename_at_155_44,(__state_rename_at_156_45 != 0x0u) ? das_auto_cast<uint32_t>::cast(((__byte_rename_at_158_47 & 0x3fu) | (__codepoint_rename_at_155_44 << 0x6u))) : das_auto_cast<uint32_t>::cast(((0xffu >> __type__rename_at_159_48) & __byte_rename_at_158_47)));
            das_copy(__state_rename_at_156_45,das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(((__state_rename_at_156_45 + 0x100u) + __type__rename_at_159_48),__context__));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_157_46));
    };
    return das_auto_cast<bool>::cast(__state_rename_at_156_45 == 0x0u);
}

inline bool is_utf8_string_valid_bcfa16997a4edaaf ( Context * __context__, char * const  __utf8_string_rename_at_167_49 )
{
    uint32_t __codepoint_rename_at_168_50 = 0x0u;
    uint32_t __state_rename_at_169_51 = 0x0u;
    {
        bool __need_loop_170 = true;
        // ch: int const
        das_iterator<char * const > __ch_iterator(__utf8_string_rename_at_167_49);
        int32_t __ch_rename_at_170_52;
        __need_loop_170 = __ch_iterator.first(__context__,(__ch_rename_at_170_52)) && __need_loop_170;
        for ( ; __need_loop_170 ; __need_loop_170 = __ch_iterator.next(__context__,(__ch_rename_at_170_52)) )
        {
            uint32_t __byte_rename_at_171_53 = ((uint32_t)uint32_t(__ch_rename_at_170_52));
            uint32_t __type__rename_at_172_54 = ((uint32_t)das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(__byte_rename_at_171_53,__context__));
            das_copy(__codepoint_rename_at_168_50,(__state_rename_at_169_51 != 0x0u) ? das_auto_cast<uint32_t>::cast(((__byte_rename_at_171_53 & 0x3fu) | (__codepoint_rename_at_168_50 << 0x6u))) : das_auto_cast<uint32_t>::cast(((0xffu >> __type__rename_at_172_54) & __byte_rename_at_171_53)));
            das_copy(__state_rename_at_169_51,das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(((__state_rename_at_169_51 + 0x100u) + __type__rename_at_172_54),__context__));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_170_52));
    };
    return das_auto_cast<bool>::cast(__state_rename_at_169_51 == 0x0u);
}

inline void utf8_decode_16c21049b073c3e2 ( Context * __context__, TArray<uint32_t> &  __dest_utf32_string_rename_at_180_55, TArray<uint8_t> const  &  __source_utf8_string_rename_at_180_56 )
{
    uint32_t __codepoint_rename_at_182_57 = 0x0u;
    uint32_t __state_rename_at_183_58 = 0x0u;
    int32_t __length_rename_at_184_59 = ((int32_t)builtin_array_size(__source_utf8_string_rename_at_180_56));
    int32_t __i_rename_at_185_60 = (contains_utf8_bom_f2cf32cf55e0a6d4(__context__,__source_utf8_string_rename_at_180_56) ? das_auto_cast<int32_t>::cast(3) : das_auto_cast<int32_t>::cast(0));
    while ( __i_rename_at_185_60 < __length_rename_at_184_59 )
    {
        uint32_t __byte_rename_at_188_61 = ((uint32_t)uint32_t(__source_utf8_string_rename_at_180_56(__i_rename_at_185_60++,__context__)));
        uint32_t __type__rename_at_189_62 = ((uint32_t)das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(__byte_rename_at_188_61,__context__));
        das_copy(__codepoint_rename_at_182_57,(__state_rename_at_183_58 != 0x0u) ? das_auto_cast<uint32_t>::cast(((__byte_rename_at_188_61 & 0x3fu) | (__codepoint_rename_at_182_57 << 0x6u))) : das_auto_cast<uint32_t>::cast(((0xffu >> __type__rename_at_189_62) & __byte_rename_at_188_61)));
        das_copy(__state_rename_at_183_58,das_shared<TDim<uint32_t,364>,0x28a2e6c17bde423c>(__context__) /*s_utf8d*/(((__state_rename_at_183_58 + 0x100u) + __type__rename_at_189_62),__context__));
        if ( __state_rename_at_183_58 == 0x0u )
        {
            _FuncbuiltinTickpushTick10769833213962245646_c46ce71d6a41166a(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_180_55),__codepoint_rename_at_182_57);
        };
    };
}

inline TArray<uint32_t> utf8_decode_d1ca0cab78c6a9ea ( Context * __context__, TArray<uint8_t> const  &  __source_utf8_string_rename_at_199_63 )
{
    TArray<uint32_t> __dest_utf32_string_rename_at_201_64;das_zero(__dest_utf32_string_rename_at_201_64);
    utf8_decode_16c21049b073c3e2(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_201_64),__source_utf8_string_rename_at_199_63);
    return /* <- */ das_auto_cast_move<TArray<uint32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3de422272adba695(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_201_64)));
}

inline TArray<uint32_t> utf8_decode_1df490f6da36ae8 ( Context * __context__, char * const  __source_utf8_string_rename_at_207_65 ) { das_stack_prologue __prologue(__context__,96,"utf8_decode " DAS_FILE_LINE);
{
    TArray<uint32_t> __dest_utf32_string_rename_at_209_66;das_zero(__dest_utf32_string_rename_at_209_66);
    builtin_string_peek(__source_utf8_string_rename_at_207_65,das_make_block<void,TArray<uint8_t> const  &>(__context__,80,0,&__func_info__5f4b6da70ca496e2,[&](TArray<uint8_t> const  & __arr_rename_at_210_67) -> void{
        utf8_decode_16c21049b073c3e2(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_209_66),__arr_rename_at_210_67);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<TArray<uint32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3de422272adba695(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_209_66)));
}}

inline void utf8_decode_233a82235363f092 ( Context * __context__, TArray<uint32_t> &  __dest_utf32_string_rename_at_217_68, char * const  __source_utf8_string_rename_at_217_69 ) { das_stack_prologue __prologue(__context__,96,"utf8_decode " DAS_FILE_LINE);
{
    builtin_string_peek(__source_utf8_string_rename_at_217_69,das_make_block<void,TArray<uint8_t> const  &>(__context__,80,0,&__func_info__5f4b6da70ca496e2,[&](TArray<uint8_t> const  & __arr_rename_at_219_70) -> void{
        utf8_decode_16c21049b073c3e2(__context__,das_arg<TArray<uint32_t>>::pass(__dest_utf32_string_rename_at_217_68),__arr_rename_at_219_70);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline char * decode_unicode_escape_511f3efa8f121c21 ( Context * __context__, char * const  __str_rename_at_224_71 ) { das_stack_prologue __prologue(__context__,384,"decode_unicode_escape " DAS_FILE_LINE);
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_225_72) DAS_AOT_INLINE_LAMBDA -> void{
        builtin_string_peek(__str_rename_at_224_71,das_make_block<void,TArray<uint8_t> const  &>(__context__,144,0,&__func_info__2f76c7f7dd4f0b27,[&](TArray<uint8_t> const  & __p_rename_at_226_73) -> void{
            int32_t __i_rename_at_227_74 = 0;
            int32_t __llen_rename_at_228_75 = ((int32_t)builtin_array_size(__p_rename_at_226_73));
            TArray<uint8_t> __bang_rename_at_229_76;das_zero(__bang_rename_at_229_76);
            _FuncbuiltinTickreserveTick3994685146752941225_d0790c6d74061d34(__context__,das_arg<TArray<uint8_t>>::pass(__bang_rename_at_229_76),8);
            uint32_t __high_surrogate_rename_at_231_77 = 0x0u;
            while ( __i_rename_at_227_74 < __llen_rename_at_228_75 )
            {
                if ( das_equ_val(__p_rename_at_226_73(__i_rename_at_227_74,__context__),0x5c) )
                {
                    int32_t __j_rename_at_234_78 = (__i_rename_at_227_74 + 1);
                    int32_t __jlen_rename_at_235_79 = ((int32_t)SimPolicy<int32_t>::Min(__llen_rename_at_228_75,__j_rename_at_234_78 + 8,*__context__,nullptr));
                    _FuncbuiltinTickresizeTick4811697762258667383_71b1749d234f576e(__context__,das_arg<TArray<uint8_t>>::pass(__bang_rename_at_229_76),0);
                    while ( (__j_rename_at_234_78 < __jlen_rename_at_235_79) && is_hex(int32_t(__p_rename_at_226_73(__j_rename_at_234_78,__context__))) )
                    {
                        _FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16(__context__,das_arg<TArray<uint8_t>>::pass(__bang_rename_at_229_76),__p_rename_at_226_73(__j_rename_at_234_78,__context__));
                        ++__j_rename_at_234_78;
                    };
                    if ( builtin_array_size(das_arg<TArray<uint8_t>>::pass(__bang_rename_at_229_76)) == 4 )
                    {
                        uint32_t __utf32_rename_at_242_80 = ((uint32_t)fast_to_uint(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__bang_rename_at_229_76),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),true));
                        if ( (__utf32_rename_at_242_80 >= 0xd800u) && (__utf32_rename_at_242_80 <= 0xdfffu) )
                        {
                            if ( __high_surrogate_rename_at_231_77 == 0x0u )
                            {
                                das_copy(__high_surrogate_rename_at_231_77,__utf32_rename_at_242_80);
                            } else {
                                TArray<uint8_t> _temp_make_local_250_55_8; _temp_make_local_250_55_8;
                                uint32_t __low_surrogate_rename_at_247_81 = ((uint32_t)__utf32_rename_at_242_80);
                                uint32_t __pair_rename_at_248_82 = ((uint32_t)((((__high_surrogate_rename_at_231_77 - 0xd800u) << 0xau) + 0x10000u) + (__low_surrogate_rename_at_247_81 - 0xdc00u)));
                                das_copy(__high_surrogate_rename_at_231_77,0x0u);
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_225_72),cast<char * const >::from(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass((_temp_make_local_250_55_8 = (utf8_encode_97b551f35cf80978(__context__,__pair_rename_at_248_82)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                            };
                        } else {
                            TArray<uint8_t> _temp_make_local_253_51_9; _temp_make_local_253_51_9;
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_3,cast<StringBuilderWriter &>::from(__writer_rename_at_225_72),cast<char * const >::from(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass((_temp_make_local_253_51_9 = (utf8_encode_97b551f35cf80978(__context__,__utf32_rename_at_242_80)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                        };
                        das_copy(__i_rename_at_227_74,__j_rename_at_234_78);
                        continue;
                    };
                };
                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_225_72),int32_t(__p_rename_at_226_73(__i_rename_at_227_74,__context__)));
                __i_rename_at_227_74 += 1;
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xc09ebdedbaa09cdd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_3dbee92a7efb07d9>>();
    };
    aotLib[0x55e8ae42bad9aab0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_194914ba4b4e9234>>();
    };
    aotLib[0x8d5d7bcf0ca0306f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_3d13e435f27e824d>>();
    };
    aotLib[0x6ecbc7360ceedde0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_a94ae7fe5f83f893>>();
    };
    aotLib[0xf04a1ead8ce3fa81] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_a6123e6697ded437>>();
    };
    aotLib[0xefd9de45790d1cee] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_dd87c6f1da83ea06>>();
    };
    aotLib[0x48edd1ea96e09090] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_4c32e98c64dc9655>>();
    };
    aotLib[0xb87c157ba48364ea] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_52af64c8664bb146>>();
    };
    aotLib[0xb45f92d1b1567597] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_26755ce4d233ca82>>();
    };
    aotLib[0xd41772edf1ed35a5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3de422272adba695>>();
    };
    aotLib[0xb826e8437742841a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_85e3c65216347e16>>();
    };
    aotLib[0x5e9acdcce3ca5e0a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickreserveTick3994685146752941225_d0790c6d74061d34>>();
    };
    aotLib[0xd685e1be2a71cb21] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_c46ce71d6a41166a>>();
    };
    aotLib[0xd952211b24df1ed4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_71b1749d234f576e>>();
    };
    aotLib[0x98d568f19bd21681] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&utf16_to_utf32_67298994a5baa194>>();
    };
    aotLib[0xc3627e7ef55a7b3b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&utf8_encode_b6fe59c3abe9a444>>();
    };
    aotLib[0x8e0402cae77fb70a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&utf8_encode_97b551f35cf80978>>();
    };
    aotLib[0x6432dd5c8b6befc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&utf8_encode_e2de5a2c582774dc>>();
    };
    aotLib[0x6349abb725e253ef] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&utf8_encode_5c19a2da34c8f208>>();
    };
    aotLib[0x4351f107078bf80a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&utf8_length_c0cb690c545d23a5>>();
    };
    aotLib[0xc9f7bbf2dc09371] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&utf8_length_9bc85c1c74b26c7e>>();
    };
    aotLib[0x53c981ce08bab93e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_first_byte_of_utf8_char_26620640f6460cdb>>();
    };
    aotLib[0x45787e4aa8c7318d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&contains_utf8_bom_f2cf32cf55e0a6d4>>();
    };
    aotLib[0xab5cf2238d531d07] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&contains_utf8_bom_d5ae1a8e5b1e79db>>();
    };
    aotLib[0x2a732b85904d52dd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_utf8_string_valid_6da0450b19500e4d>>();
    };
    aotLib[0xfcacb42b1874921a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_utf8_string_valid_bcfa16997a4edaaf>>();
    };
    aotLib[0xc9797b2bab39bbee] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&utf8_decode_16c21049b073c3e2>>();
    };
    aotLib[0x6f33c745c1c08455] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&utf8_decode_d1ca0cab78c6a9ea>>();
    };
    aotLib[0x3f3a38289d604e8d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&utf8_decode_1df490f6da36ae8>>();
    };
    aotLib[0xb1f3462ddcd863aa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&utf8_decode_233a82235363f092>>();
    };
    aotLib[0x491b63d6deab9770] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&decode_unicode_escape_511f3efa8f121c21>>();
    };
    // [[ init script ]]
    aotLib[0x79a88bf5abaa802f] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
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
