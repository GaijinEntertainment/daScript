#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings_boost
 // require ast_boost
 // require regex
 // require regex_boost
 // require random
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require static_let
 // require macro_boost
 // require defer
 // require math_bits
 // require faker

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
namespace _anon_341163587376737593 {

namespace faker { struct MonthNameAndDay; };
namespace faker { struct Faker; };
namespace ast { struct AstFunctionAnnotation; };
namespace ast { struct AstBlockAnnotation; };
namespace ast { struct AstStructureAnnotation; };
namespace ast { struct AstPassMacro; };
namespace ast { struct AstVariantMacro; };
namespace ast { struct AstForLoopMacro; };
namespace ast { struct AstCaptureMacro; };
namespace ast { struct AstTypeMacro; };
namespace ast { struct AstSimulateMacro; };
namespace ast { struct AstReaderMacro; };
namespace ast { struct AstCommentReader; };
namespace ast { struct AstCallMacro; };
namespace ast { struct AstTypeInfoMacro; };
namespace ast { struct AstEnumerationAnnotation; };
namespace ast { struct AstVisitor; };
namespace ast_boost { struct MacroMacro; };
namespace ast_boost { struct TagFunctionAnnotation; };
namespace ast_boost { struct TagStructureAnnotation; };
namespace ast_boost { struct SetupAnyAnnotation; };
namespace ast_boost { struct SetupFunctionAnnotation; };
namespace ast_boost { struct SetupBlockAnnotation; };
namespace ast_boost { struct SetupStructureAnnotation; };
namespace ast_boost { struct SetupEnumerationAnnotation; };
namespace ast_boost { struct SetupContractAnnotation; };
namespace ast_boost { struct SetupReaderMacro; };
namespace ast_boost { struct SetupCommentReader; };
namespace ast_boost { struct SetupVariantMacro; };
namespace ast_boost { struct SetupForLoopMacro; };
namespace ast_boost { struct SetupCaptureMacro; };
namespace ast_boost { struct SetupTypeMacro; };
namespace ast_boost { struct SetupSimulateMacro; };
namespace ast_boost { struct SetupCallMacro; };
namespace ast_boost { struct SetupTypeInfoMacro; };
namespace ast_boost { struct SetupInferMacro; };
namespace ast_boost { struct SetupDirtyInferMacro; };
namespace ast_boost { struct SetupLintMacro; };
namespace ast_boost { struct SetupGlobalLintMacro; };
namespace ast_boost { struct SetupOptimizationMacro; };
namespace ast_boost { struct TagFunctionMacro; };
namespace ast_boost { struct BetterRttiVisitor; };
namespace regex { struct ReNode; };
namespace regex { struct Regex; };
namespace regex_boost { struct RegexReader; };
namespace random { struct _lambda_random_111_1; };
namespace templates { struct DecltypeMacro; };
namespace templates { struct DecltypeNoRefMacro; };
namespace templates { struct TemplateMacro; };
namespace contracts { struct IsAnyType; };
namespace contracts { struct IsAnyArrayMacro; };
namespace contracts { struct IsAnyEnumMacro; };
namespace contracts { struct IsAnyBitfieldMacro; };
namespace contracts { struct IsAnyVectorType; };
namespace contracts { struct IsAnyStructMacro; };
namespace contracts { struct IsAnyNumericMacro; };
namespace contracts { struct IsAnyWorkhorse; };
namespace contracts { struct IsAnyWorkhorseNonPtrMacro; };
namespace contracts { struct IsAnyTupleNonPtrMacro; };
namespace contracts { struct IsAnyVariantNonPtrMacro; };
namespace contracts { struct IsAnyFunctionNonPtrMacro; };
namespace contracts { struct IsAnyLambdaMacro; };
namespace contracts { struct IsRefMacro; };
namespace contracts { struct IsPointer; };
namespace contracts { struct IsClass; };
namespace contracts { struct IsValueHandle; };
namespace templates_boost { struct Template; };
namespace templates_boost { struct TemplateVisitor; };
namespace templates_boost { struct RemoveDerefVisitor; };
namespace templates_boost { struct QRulesVisitor; };
namespace templates_boost { struct AstQCallMacro; };
namespace templates_boost { struct QMacro; };
namespace templates_boost { struct QBlockMacro; };
namespace templates_boost { struct QBlockToArrayMacro; };
namespace templates_boost { struct QBlockExprMacro; };
namespace templates_boost { struct QTypeMacro; };
namespace templates_boost { struct AstQNamedMacro; };
namespace templates_boost { struct AstQFunctionMacro; };
namespace templates_boost { struct AstQVariableMacro; };
namespace templates_boost { struct AstQNamedClassMacro; };
namespace templates_boost { struct AstQMethodMacro; };
namespace static_let { struct StaticLetMacro; };
namespace macro_boost { struct MacroVerifyMacro; };
namespace macro_boost { struct CaptureBlock; };
namespace macro_boost { struct CapturedVariable; };
namespace macro_boost { struct ColletFinally; };
namespace macro_boost { struct ColletLabels; };
namespace macro_boost { struct ReturnSkipLockcheck; };
namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
// unused enumeration ConversionResult
// unused enumeration CaptureMode
// unused enumeration SideEffects
// unused structure AstFunctionAnnotation
// unused structure AstBlockAnnotation
// unused structure AstStructureAnnotation
// unused structure AstPassMacro
// unused structure AstVariantMacro
// unused structure AstForLoopMacro
// unused structure AstCaptureMacro
// unused structure AstTypeMacro
// unused structure AstSimulateMacro
// unused structure AstReaderMacro
// unused structure AstCommentReader
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
// unused structure AstVisitor
// unused structure MacroMacro
// unused structure TagFunctionAnnotation
// unused structure TagStructureAnnotation
// unused structure SetupAnyAnnotation
// unused structure SetupFunctionAnnotation
// unused structure SetupBlockAnnotation
// unused structure SetupStructureAnnotation
// unused structure SetupEnumerationAnnotation
// unused structure SetupContractAnnotation
// unused structure SetupReaderMacro
// unused structure SetupCommentReader
// unused structure SetupVariantMacro
// unused structure SetupForLoopMacro
// unused structure SetupCaptureMacro
// unused structure SetupTypeMacro
// unused structure SetupSimulateMacro
// unused structure SetupCallMacro
// unused structure SetupTypeInfoMacro
// unused structure SetupInferMacro
// unused structure SetupDirtyInferMacro
// unused structure SetupLintMacro
// unused structure SetupGlobalLintMacro
// unused structure SetupOptimizationMacro
// unused structure TagFunctionMacro
// unused structure BetterRttiVisitor
namespace regex {

enum class ReOp : int32_t {
    Char = int32_t(0),
    Set = int32_t(1),
    Any = int32_t(2),
    Eos = int32_t(3),
    Group = int32_t(4),
    Plus = int32_t(5),
    Star = int32_t(6),
    Question = int32_t(7),
    Concat = int32_t(8),
    Union = int32_t(9),
};
}
namespace regex {

struct ReNode {
    DAS_COMMENT(enum) regex::ReOp op;
    int32_t id;
    Func DAS_COMMENT((uint8_t const  *,regex::Regex,regex::ReNode *,uint8_t const  * const )) fun2;
    Func DAS_COMMENT((void,regex::ReNode *,Sequence DAS_COMMENT((uint32_t)),StringBuilderWriter)) gen2;
    range at;
    char * text;
    int32_t textLen;
    TArray<regex::ReNode *> all;
    regex::ReNode * left;
    regex::ReNode * right;
    regex::ReNode * subexpr;
    regex::ReNode * next;
    TDim<uint32_t,8> cset;
    int32_t index;
    uint8_t const  * tail;
};
}
namespace regex {

struct Regex {
    regex::ReNode * root;
    uint8_t const  * match;
    TArray<AutoTuple<range,char *>> groups;
    TDim<uint32_t,8> earlyOut;
    bool canEarlyOut;
};
}
// unused structure RegexReader
namespace random {

struct _lambda_random_111_1 {
    Func DAS_COMMENT((bool,random::_lambda_random_111_1,uint32_t &)) __lambda;
    Func DAS_COMMENT((void,random::_lambda_random_111_1 *)) __finalize;
    int32_t __yield;
    int32_t rnd_seed;
    int4 seed;
};
}
// unused structure DecltypeMacro
// unused structure DecltypeNoRefMacro
// unused structure TemplateMacro
// unused structure IsAnyType
// unused structure IsAnyArrayMacro
// unused structure IsAnyEnumMacro
// unused structure IsAnyBitfieldMacro
// unused structure IsAnyVectorType
// unused structure IsAnyStructMacro
// unused structure IsAnyNumericMacro
// unused structure IsAnyWorkhorse
// unused structure IsAnyWorkhorseNonPtrMacro
// unused structure IsAnyTupleNonPtrMacro
// unused structure IsAnyVariantNonPtrMacro
// unused structure IsAnyFunctionNonPtrMacro
// unused structure IsAnyLambdaMacro
// unused structure IsRefMacro
// unused structure IsPointer
// unused structure IsClass
// unused structure IsValueHandle
// unused structure Template
// unused structure TemplateVisitor
// unused structure RemoveDerefVisitor
// unused structure QRulesVisitor
// unused structure AstQCallMacro
// unused structure QMacro
// unused structure QBlockMacro
// unused structure QBlockToArrayMacro
// unused structure QBlockExprMacro
// unused structure QTypeMacro
// unused structure AstQNamedMacro
// unused structure AstQFunctionMacro
// unused structure AstQVariableMacro
// unused structure AstQNamedClassMacro
// unused structure AstQMethodMacro
// unused structure StaticLetMacro
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
// unused structure DeferMacro
// unused structure DeferDeleteMacro
namespace faker {

struct MonthNameAndDay {
    char * name;
    int32_t days;
};
}
namespace faker {

struct Faker {
    uint32_t min_year;
    uint32_t total_years;
    Sequence DAS_COMMENT((uint32_t)) rnd;
    uint32_t max_long_string;
};
}
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af63ee4c86020b22;

TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int32_t>::size, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xaf63ee4c86020b22) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[7] = { &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_93e93a07b629c7a3 ( Context * __context__, TArray<faker::MonthNameAndDay> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_de3bec42d99954f8 ( Context * __context__, TArray<AutoTuple<range,char *>> & __Arr_rename_at_68_2, int32_t __newSize_rename_at_68_3 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_b92c6f816db4602 ( Context * __context__, TArray<regex::ReNode *> & __Arr_rename_at_68_4, int32_t __newSize_rename_at_68_5 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_856be2a5daceccf2 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_68_6, int32_t __newSize_rename_at_68_7 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_bd2ed0e765c61464 ( Context * __context__, TDim<faker::MonthNameAndDay,12> const  & __a_rename_at_581_8 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_7bf97eaaff0543bb ( Context * __context__, TDim<AutoTuple<range,char *>,1> const  & __a_rename_at_581_9 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_f3bb1928c94b9b52 ( Context * __context__, TDim<AutoTuple<range,char *>,2> const  & __a_rename_at_581_10 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_ca7d613c38c76fb8 ( Context * __context__, TDim<regex::ReNode *,11> const  & __a_rename_at_581_11 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_67d7519b16eea304 ( Context * __context__, TDim<int32_t,12> const  & __a_rename_at_581_12 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_8dc91cad3b75b3c8 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1215_13 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0 ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) & __it_rename_at_1256_14, uint32_t & __value_rename_at_1256_15 );
inline void _FuncbuiltinTickreserveTick3994685146752941225_71af0b01dbdb30b2 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_125_16, int32_t __newSize_rename_at_125_17 );
inline void _FuncbuiltinTickpushTick14133213201864676143_d8f0a51920afdbe4 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_18, uint8_t __value_rename_at_165_19 );
inline faker::Faker Faker_120e1a6bb086dd13 ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) & __rng_rename_at_57_20 );
inline char * long_string_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_70_21 );
inline char * any_string_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_85_26 );
inline char * any_file_name_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_90_27 );
inline int32_t random_int_db927c5b51337757 ( Context * __context__, faker::Faker & __faker_rename_at_95_28 );
inline uint32_t random_uint_1fc7e41ed0d6ffdd ( Context * __context__, faker::Faker & __faker_rename_at_102_30 );
inline int8_t random_int8_9e48de99fcf0b4bb ( Context * __context__, faker::Faker & __faker_rename_at_109_32 );
inline uint8_t random_uint8_eb9ad5532c2efba ( Context * __context__, faker::Faker & __faker_rename_at_116_34 );
inline int16_t random_int16_65cfde74f34ebeec ( Context * __context__, faker::Faker & __faker_rename_at_123_36 );
inline uint16_t random_uint16_27625a3cd1234fe8 ( Context * __context__, faker::Faker & __faker_rename_at_130_38 );
inline float random_float_3b1c10c7ec3bd27d ( Context * __context__, faker::Faker & __faker_rename_at_137_40 );
inline int2 random_int2_163549ee4deaeda5 ( Context * __context__, faker::Faker & __faker_rename_at_144_42 );
inline range random_range_82207fb65a0097a8 ( Context * __context__, faker::Faker & __faker_rename_at_149_43 );
inline range64 random_range64_76c394d5233908be ( Context * __context__, faker::Faker & __faker_rename_at_154_44 );
inline int3 random_int3_4e81314eab7f0e58 ( Context * __context__, faker::Faker & __faker_rename_at_159_45 );
inline int4 random_int4_24c671e0ae1fb943 ( Context * __context__, faker::Faker & __faker_rename_at_164_46 );
inline uint2 random_uint2_74dfcbc709ea1d13 ( Context * __context__, faker::Faker & __faker_rename_at_169_47 );
inline urange random_urange_5f399937e7e7ad83 ( Context * __context__, faker::Faker & __faker_rename_at_174_48 );
inline urange64 random_urange64_64b82994fec321bd ( Context * __context__, faker::Faker & __faker_rename_at_179_49 );
inline uint3 random_uint3_82c843a2e2dcd1ad ( Context * __context__, faker::Faker & __faker_rename_at_184_50 );
inline uint4 random_uint4_c168676f0192f143 ( Context * __context__, faker::Faker & __faker_rename_at_189_51 );
inline float2 random_float2_de4a2c3aa9fc390 ( Context * __context__, faker::Faker & __faker_rename_at_194_52 );
inline float3 random_float3_176dbf53ad107c9a ( Context * __context__, faker::Faker & __faker_rename_at_199_53 );
inline float4 random_float4_d05db2140ce31987 ( Context * __context__, faker::Faker & __faker_rename_at_204_54 );
inline float3x3 random_float3x3_e049ec11115607a5 ( Context * __context__, faker::Faker & __faker_rename_at_209_55 );
inline float3x4 random_float3x4_ae3c09f14778b039 ( Context * __context__, faker::Faker & __faker_rename_at_218_57 );
inline float4x4 random_float4x4_4db49814bded2b7d ( Context * __context__, faker::Faker & __faker_rename_at_228_59 );
inline int64_t random_int64_20af9383ba5df64b ( Context * __context__, faker::Faker & __faker_rename_at_245_61 );
inline uint64_t random_uint64_9d78736b94d2bb65 ( Context * __context__, faker::Faker & __faker_rename_at_251_63 );
inline double random_double_ee86008619feb237 ( Context * __context__, faker::Faker & __faker_rename_at_257_65 );
inline TDim<uint32_t,8> any_set_d6fb89a5c88f1b2c ( Context * __context__, faker::Faker & __faker_rename_at_263_67 );
inline int32_t any_char_db927c5b51337757 ( Context * __context__, faker::Faker & __faker_rename_at_272_70 );
inline char * number_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_279_72 );
inline char * positive_int_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_284_73 );
inline char * any_int_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_289_74 );
inline char * any_hex_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_294_75 );
inline char * any_float_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_299_76 );
inline char * any_uint_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_304_77 );
inline char * month_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_309_78 );
inline char * day_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_317_80 );
inline bool is_leap_year_f4966f72627a00c6 ( Context * __context__, uint32_t __year_rename_at_325_82 );
inline int32_t week_day_c239077242e7d543 ( Context * __context__, uint32_t __year_rename_at_330_83, uint32_t __month_rename_at_330_84, uint32_t __day_rename_at_330_85 );
inline int32_t week_day_a63db001e98f1f1d ( Context * __context__, int32_t __year_rename_at_335_86, int32_t __month_rename_at_335_87, int32_t __day_rename_at_335_88 );
inline char * date_2c99640021fae61f ( Context * __context__, faker::Faker & __faker_rename_at_357_92 );
inline faker::Faker Faker_b38307f5f81d41da ( Context * __context__ );
inline TArray<faker::MonthNameAndDay> _FuncbuiltinTickto_array_moveTick3185538323411982277_a6478d4cc12bcff8 ( Context * __context__, TDim<faker::MonthNameAndDay,12> & __a_rename_at_1375_98 );
inline TArray<AutoTuple<range,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_d240a9f91d9ede74 ( Context * __context__, TDim<AutoTuple<range,char *>,1> & __a_rename_at_1375_100 );
inline TArray<AutoTuple<range,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_c7508367b622b66d ( Context * __context__, TDim<AutoTuple<range,char *>,2> & __a_rename_at_1375_102 );
inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_37174cace0912109 ( Context * __context__, TDim<regex::ReNode *,11> & __a_rename_at_1375_104 );
inline TArray<int32_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_afe546e6423dd8f3 ( Context * __context__, TDim<int32_t,12> & __a_rename_at_1375_106 );

void __init_script ( Context * __context__, bool __init_shared )
{
    TDim<faker::MonthNameAndDay,12> _temp_make_local_24_24_32;
    regex::Regex _temp_make_local_61_31_32;
    TDim<AutoTuple<range,char *>,1> _temp_make_local_61_31_176;
    regex::Regex _temp_make_local_62_22_32;
    TDim<AutoTuple<range,char *>,1> _temp_make_local_62_22_224;
    regex::Regex _temp_make_local_63_23_32;
    TDim<AutoTuple<range,char *>,1> _temp_make_local_63_23_176;
    regex::Regex _temp_make_local_64_22_32;
    TDim<AutoTuple<range,char *>,1> _temp_make_local_64_22_176;
    regex::Regex _temp_make_local_65_25_32;
    TDim<AutoTuple<range,char *>,1> _temp_make_local_65_25_144;
    regex::Regex _temp_make_local_66_28_32;
    TDim<AutoTuple<range,char *>,1> _temp_make_local_66_28_144;
    regex::Regex _temp_make_local_67_24_32;
    TDim<AutoTuple<range,char *>,2> _temp_make_local_67_24_336;
    regex::Regex _temp_make_local_68_17_32;
    TDim<regex::ReNode *,11> _temp_make_local_68_17_144;
    TDim<AutoTuple<range,char *>,2> _temp_make_local_68_17_416;
    TDim<int32_t,12> _temp_make_local_338_22_32;
    das_global<TArray<faker::MonthNameAndDay>,0x1d0bed6094e46065>(__context__) = _FuncbuiltinTickto_array_moveTick3185538323411982277_a6478d4cc12bcff8(__context__,das_arg<TDim<faker::MonthNameAndDay,12>>::pass((([&]() -> TDim<faker::MonthNameAndDay,12>& {
        _temp_make_local_24_24_32(0,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_24;
            das_copy((__mks_24.name),(((char *) "January")));
            das_copy((__mks_24.days),(31));
            return __mks_24;
        })());
        _temp_make_local_24_24_32(1,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_25;
            das_copy((__mks_25.name),(((char *) "February")));
            das_copy((__mks_25.days),(28));
            return __mks_25;
        })());
        _temp_make_local_24_24_32(2,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_26;
            das_copy((__mks_26.name),(((char *) "March")));
            das_copy((__mks_26.days),(31));
            return __mks_26;
        })());
        _temp_make_local_24_24_32(3,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_27;
            das_copy((__mks_27.name),(((char *) "April")));
            das_copy((__mks_27.days),(30));
            return __mks_27;
        })());
        _temp_make_local_24_24_32(4,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_28;
            das_copy((__mks_28.name),(((char *) "May")));
            das_copy((__mks_28.days),(31));
            return __mks_28;
        })());
        _temp_make_local_24_24_32(5,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_29;
            das_copy((__mks_29.name),(((char *) "June")));
            das_copy((__mks_29.days),(30));
            return __mks_29;
        })());
        _temp_make_local_24_24_32(6,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_30;
            das_copy((__mks_30.name),(((char *) "July")));
            das_copy((__mks_30.days),(31));
            return __mks_30;
        })());
        _temp_make_local_24_24_32(7,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_31;
            das_copy((__mks_31.name),(((char *) "August")));
            das_copy((__mks_31.days),(31));
            return __mks_31;
        })());
        _temp_make_local_24_24_32(8,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_32;
            das_copy((__mks_32.name),(((char *) "September")));
            das_copy((__mks_32.days),(30));
            return __mks_32;
        })());
        _temp_make_local_24_24_32(9,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_33;
            das_copy((__mks_33.name),(((char *) "October")));
            das_copy((__mks_33.days),(31));
            return __mks_33;
        })());
        _temp_make_local_24_24_32(10,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_34;
            das_copy((__mks_34.name),(((char *) "November")));
            das_copy((__mks_34.days),(30));
            return __mks_34;
        })());
        _temp_make_local_24_24_32(11,__context__) = (([&]() -> faker::MonthNameAndDay {
            faker::MonthNameAndDay __mks_35;
            das_copy((__mks_35.name),(((char *) "December")));
            das_copy((__mks_35.days),(31));
            return __mks_35;
        })());
        return _temp_make_local_24_24_32;
    })())));/*g_months*/
    das_global<TDim<char *,7>,0x34cefaea736497f>(__context__) = (([&]() -> TDim<char *,7> {
        TDim<char *,7> __mka_39;
        __mka_39(0,__context__) = ((char *) "Sunday");
        __mka_39(1,__context__) = ((char *) "Monday");
        __mka_39(2,__context__) = ((char *) "Tuesday");
        __mka_39(3,__context__) = ((char *) "Wednesday");
        __mka_39(4,__context__) = ((char *) "Thursday");
        __mka_39(5,__context__) = ((char *) "Friday");
        __mka_39(6,__context__) = ((char *) "Saturday");
        return __mka_39;
    })());/*g_days*/
    das_global<regex::Regex,0x24f6b93c51999c1e>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_61_31_32);
        das_copy((_temp_make_local_61_31_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_61;
            das_zero(__mks_61);
            das_copy((__mks_61.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_61.id),(0));
            das_copy((__mks_61.at),(range(0,11)));
            das_copy((__mks_61.text),(nullptr));
            das_copy((__mks_61.textLen),(0));
            das_move((__mks_61.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_61;
                das_zero(__mks_61);
                return __mks_61;
            })())));
            das_copy((__mks_61.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_61;
                das_zero(__mks_61);
                das_copy((__mks_61.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                das_copy((__mks_61.id),(1));
                das_copy((__mks_61.at),(range(0,5)));
                das_copy((__mks_61.text),(nullptr));
                das_copy((__mks_61.textLen),(0));
                das_move((__mks_61.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_61;
                    das_zero(__mks_61);
                    return __mks_61;
                })())));
                das_copy((__mks_61.left),(nullptr));
                das_copy((__mks_61.right),(nullptr));
                das_copy((__mks_61.subexpr),(nullptr));
                das_copy((__mks_61.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_61;
                    __mka_61(0,__context__) = 0x0u;
                    __mka_61(1,__context__) = 0x3ff0000u;
                    __mka_61(2,__context__) = 0x0u;
                    __mka_61(3,__context__) = 0x0u;
                    __mka_61(4,__context__) = 0x0u;
                    __mka_61(5,__context__) = 0x0u;
                    __mka_61(6,__context__) = 0x0u;
                    __mka_61(7,__context__) = 0x0u;
                    return __mka_61;
                })())));
                das_copy((__mks_61.index),(0));
                return __mks_61;
            })()))));
            das_copy((__mks_61.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_61;
                das_zero(__mks_61);
                das_copy((__mks_61.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                das_copy((__mks_61.id),(2));
                das_copy((__mks_61.at),(range(5,11)));
                das_copy((__mks_61.text),(nullptr));
                das_copy((__mks_61.textLen),(0));
                das_move((__mks_61.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_61;
                    das_zero(__mks_61);
                    return __mks_61;
                })())));
                das_copy((__mks_61.left),(nullptr));
                das_copy((__mks_61.right),(nullptr));
                das_copy((__mks_61.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_61;
                    das_zero(__mks_61);
                    das_copy((__mks_61.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                    das_copy((__mks_61.id),(3));
                    das_copy((__mks_61.at),(range(5,10)));
                    das_copy((__mks_61.text),(nullptr));
                    das_copy((__mks_61.textLen),(0));
                    das_move((__mks_61.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_61;
                        das_zero(__mks_61);
                        return __mks_61;
                    })())));
                    das_copy((__mks_61.left),(nullptr));
                    das_copy((__mks_61.right),(nullptr));
                    das_copy((__mks_61.subexpr),(nullptr));
                    das_copy((__mks_61.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_61;
                        __mka_61(0,__context__) = 0x0u;
                        __mka_61(1,__context__) = 0x3ff0000u;
                        __mka_61(2,__context__) = 0x0u;
                        __mka_61(3,__context__) = 0x0u;
                        __mka_61(4,__context__) = 0x0u;
                        __mka_61(5,__context__) = 0x0u;
                        __mka_61(6,__context__) = 0x0u;
                        __mka_61(7,__context__) = 0x0u;
                        return __mka_61;
                    })())));
                    das_copy((__mks_61.index),(0));
                    return __mks_61;
                })()))));
                das_copy((__mks_61.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_61;
                    __mka_61(0,__context__) = 0x0u;
                    __mka_61(1,__context__) = 0x0u;
                    __mka_61(2,__context__) = 0x0u;
                    __mka_61(3,__context__) = 0x0u;
                    __mka_61(4,__context__) = 0x0u;
                    __mka_61(5,__context__) = 0x0u;
                    __mka_61(6,__context__) = 0x0u;
                    __mka_61(7,__context__) = 0x0u;
                    return __mka_61;
                })())));
                das_copy((__mks_61.index),(0));
                return __mks_61;
            })()))));
            das_copy((__mks_61.subexpr),(nullptr));
            das_copy((__mks_61.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_61;
                __mka_61(0,__context__) = 0x0u;
                __mka_61(1,__context__) = 0x0u;
                __mka_61(2,__context__) = 0x0u;
                __mka_61(3,__context__) = 0x0u;
                __mka_61(4,__context__) = 0x0u;
                __mka_61(5,__context__) = 0x0u;
                __mka_61(6,__context__) = 0x0u;
                __mka_61(7,__context__) = 0x0u;
                return __mka_61;
            })())));
            das_copy((__mks_61.index),(0));
            return __mks_61;
        })()))));
        das_move((_temp_make_local_61_31_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_d240a9f91d9ede74(__context__,das_arg<TDim<AutoTuple<range,char *>,1>>::pass((([&]() -> TDim<AutoTuple<range,char *>,1>& {
            _temp_make_local_61_31_176(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_61;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_61) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_61) = nullptr;
                return __mkt_61;
            })());
            return _temp_make_local_61_31_176;
        })())))));
        das_copy((_temp_make_local_61_31_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_61;
            __mka_61(0,__context__) = 0x0u;
            __mka_61(1,__context__) = 0x3ff0000u;
            __mka_61(2,__context__) = 0x0u;
            __mka_61(3,__context__) = 0x0u;
            __mka_61(4,__context__) = 0x0u;
            __mka_61(5,__context__) = 0x0u;
            __mka_61(6,__context__) = 0x0u;
            __mka_61(7,__context__) = 0x0u;
            return __mka_61;
        })())));
        das_copy((_temp_make_local_61_31_32.canEarlyOut),(true));
        return _temp_make_local_61_31_32;
    })())));/*re_positive_int*/
    das_global<regex::Regex,0x21aa63079ae9f66c>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_62_22_32);
        das_copy((_temp_make_local_62_22_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_62;
            das_zero(__mks_62);
            das_copy((__mks_62.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_62.id),(0));
            das_copy((__mks_62.at),(range(0,18)));
            das_copy((__mks_62.text),(nullptr));
            das_copy((__mks_62.textLen),(0));
            das_move((__mks_62.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_62;
                das_zero(__mks_62);
                return __mks_62;
            })())));
            das_copy((__mks_62.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_62;
                das_zero(__mks_62);
                das_copy((__mks_62.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                das_copy((__mks_62.id),(1));
                das_copy((__mks_62.at),(range(0,12)));
                das_copy((__mks_62.text),(nullptr));
                das_copy((__mks_62.textLen),(0));
                das_move((__mks_62.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_62;
                    das_zero(__mks_62);
                    return __mks_62;
                })())));
                das_copy((__mks_62.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_62;
                    das_zero(__mks_62);
                    das_copy((__mks_62.op),(DAS_COMMENT(enum) regex::ReOp::Question));
                    das_copy((__mks_62.id),(2));
                    das_copy((__mks_62.at),(range(0,7)));
                    das_copy((__mks_62.text),(nullptr));
                    das_copy((__mks_62.textLen),(0));
                    das_move((__mks_62.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_62;
                        das_zero(__mks_62);
                        return __mks_62;
                    })())));
                    das_copy((__mks_62.left),(nullptr));
                    das_copy((__mks_62.right),(nullptr));
                    das_copy((__mks_62.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_62;
                        das_zero(__mks_62);
                        das_copy((__mks_62.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_62.id),(3));
                        das_copy((__mks_62.at),(range(0,6)));
                        das_copy((__mks_62.text),(nullptr));
                        das_copy((__mks_62.textLen),(0));
                        das_move((__mks_62.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_62;
                            das_zero(__mks_62);
                            return __mks_62;
                        })())));
                        das_copy((__mks_62.left),(nullptr));
                        das_copy((__mks_62.right),(nullptr));
                        das_copy((__mks_62.subexpr),(nullptr));
                        das_copy((__mks_62.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_62;
                            __mka_62(0,__context__) = 0x0u;
                            __mka_62(1,__context__) = 0x2800u;
                            __mka_62(2,__context__) = 0x0u;
                            __mka_62(3,__context__) = 0x0u;
                            __mka_62(4,__context__) = 0x0u;
                            __mka_62(5,__context__) = 0x0u;
                            __mka_62(6,__context__) = 0x0u;
                            __mka_62(7,__context__) = 0x0u;
                            return __mka_62;
                        })())));
                        das_copy((__mks_62.index),(0));
                        return __mks_62;
                    })()))));
                    das_copy((__mks_62.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_62;
                        __mka_62(0,__context__) = 0x0u;
                        __mka_62(1,__context__) = 0x0u;
                        __mka_62(2,__context__) = 0x0u;
                        __mka_62(3,__context__) = 0x0u;
                        __mka_62(4,__context__) = 0x0u;
                        __mka_62(5,__context__) = 0x0u;
                        __mka_62(6,__context__) = 0x0u;
                        __mka_62(7,__context__) = 0x0u;
                        return __mka_62;
                    })())));
                    das_copy((__mks_62.index),(0));
                    return __mks_62;
                })()))));
                das_copy((__mks_62.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_62;
                    das_zero(__mks_62);
                    das_copy((__mks_62.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                    das_copy((__mks_62.id),(4));
                    das_copy((__mks_62.at),(range(7,12)));
                    das_copy((__mks_62.text),(nullptr));
                    das_copy((__mks_62.textLen),(0));
                    das_move((__mks_62.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_62;
                        das_zero(__mks_62);
                        return __mks_62;
                    })())));
                    das_copy((__mks_62.left),(nullptr));
                    das_copy((__mks_62.right),(nullptr));
                    das_copy((__mks_62.subexpr),(nullptr));
                    das_copy((__mks_62.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_62;
                        __mka_62(0,__context__) = 0x0u;
                        __mka_62(1,__context__) = 0x3ff0000u;
                        __mka_62(2,__context__) = 0x0u;
                        __mka_62(3,__context__) = 0x0u;
                        __mka_62(4,__context__) = 0x0u;
                        __mka_62(5,__context__) = 0x0u;
                        __mka_62(6,__context__) = 0x0u;
                        __mka_62(7,__context__) = 0x0u;
                        return __mka_62;
                    })())));
                    das_copy((__mks_62.index),(0));
                    return __mks_62;
                })()))));
                das_copy((__mks_62.subexpr),(nullptr));
                das_copy((__mks_62.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_62;
                    __mka_62(0,__context__) = 0x0u;
                    __mka_62(1,__context__) = 0x0u;
                    __mka_62(2,__context__) = 0x0u;
                    __mka_62(3,__context__) = 0x0u;
                    __mka_62(4,__context__) = 0x0u;
                    __mka_62(5,__context__) = 0x0u;
                    __mka_62(6,__context__) = 0x0u;
                    __mka_62(7,__context__) = 0x0u;
                    return __mka_62;
                })())));
                das_copy((__mks_62.index),(0));
                return __mks_62;
            })()))));
            das_copy((__mks_62.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_62;
                das_zero(__mks_62);
                das_copy((__mks_62.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                das_copy((__mks_62.id),(5));
                das_copy((__mks_62.at),(range(12,18)));
                das_copy((__mks_62.text),(nullptr));
                das_copy((__mks_62.textLen),(0));
                das_move((__mks_62.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_62;
                    das_zero(__mks_62);
                    return __mks_62;
                })())));
                das_copy((__mks_62.left),(nullptr));
                das_copy((__mks_62.right),(nullptr));
                das_copy((__mks_62.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_62;
                    das_zero(__mks_62);
                    das_copy((__mks_62.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                    das_copy((__mks_62.id),(6));
                    das_copy((__mks_62.at),(range(12,17)));
                    das_copy((__mks_62.text),(nullptr));
                    das_copy((__mks_62.textLen),(0));
                    das_move((__mks_62.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_62;
                        das_zero(__mks_62);
                        return __mks_62;
                    })())));
                    das_copy((__mks_62.left),(nullptr));
                    das_copy((__mks_62.right),(nullptr));
                    das_copy((__mks_62.subexpr),(nullptr));
                    das_copy((__mks_62.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_62;
                        __mka_62(0,__context__) = 0x0u;
                        __mka_62(1,__context__) = 0x3ff0000u;
                        __mka_62(2,__context__) = 0x0u;
                        __mka_62(3,__context__) = 0x0u;
                        __mka_62(4,__context__) = 0x0u;
                        __mka_62(5,__context__) = 0x0u;
                        __mka_62(6,__context__) = 0x0u;
                        __mka_62(7,__context__) = 0x0u;
                        return __mka_62;
                    })())));
                    das_copy((__mks_62.index),(0));
                    return __mks_62;
                })()))));
                das_copy((__mks_62.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_62;
                    __mka_62(0,__context__) = 0x0u;
                    __mka_62(1,__context__) = 0x0u;
                    __mka_62(2,__context__) = 0x0u;
                    __mka_62(3,__context__) = 0x0u;
                    __mka_62(4,__context__) = 0x0u;
                    __mka_62(5,__context__) = 0x0u;
                    __mka_62(6,__context__) = 0x0u;
                    __mka_62(7,__context__) = 0x0u;
                    return __mka_62;
                })())));
                das_copy((__mks_62.index),(0));
                return __mks_62;
            })()))));
            das_copy((__mks_62.subexpr),(nullptr));
            das_copy((__mks_62.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_62;
                __mka_62(0,__context__) = 0x0u;
                __mka_62(1,__context__) = 0x0u;
                __mka_62(2,__context__) = 0x0u;
                __mka_62(3,__context__) = 0x0u;
                __mka_62(4,__context__) = 0x0u;
                __mka_62(5,__context__) = 0x0u;
                __mka_62(6,__context__) = 0x0u;
                __mka_62(7,__context__) = 0x0u;
                return __mka_62;
            })())));
            das_copy((__mks_62.index),(0));
            return __mks_62;
        })()))));
        das_move((_temp_make_local_62_22_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_d240a9f91d9ede74(__context__,das_arg<TDim<AutoTuple<range,char *>,1>>::pass((([&]() -> TDim<AutoTuple<range,char *>,1>& {
            _temp_make_local_62_22_224(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_62;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_62) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_62) = nullptr;
                return __mkt_62;
            })());
            return _temp_make_local_62_22_224;
        })())))));
        das_copy((_temp_make_local_62_22_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_62;
            __mka_62(0,__context__) = 0x0u;
            __mka_62(1,__context__) = 0x3ff2800u;
            __mka_62(2,__context__) = 0x0u;
            __mka_62(3,__context__) = 0x0u;
            __mka_62(4,__context__) = 0x0u;
            __mka_62(5,__context__) = 0x0u;
            __mka_62(6,__context__) = 0x0u;
            __mka_62(7,__context__) = 0x0u;
            return __mka_62;
        })())));
        das_copy((_temp_make_local_62_22_32.canEarlyOut),(true));
        return _temp_make_local_62_22_32;
    })())));/*re_int*/
    das_global<regex::Regex,0x4df21bb6fd7700f>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_63_23_32);
        das_copy((_temp_make_local_63_23_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_63;
            das_zero(__mks_63);
            das_copy((__mks_63.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_63.id),(0));
            das_copy((__mks_63.at),(range(0,11)));
            das_copy((__mks_63.text),(nullptr));
            das_copy((__mks_63.textLen),(0));
            das_move((__mks_63.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_63;
                das_zero(__mks_63);
                return __mks_63;
            })())));
            das_copy((__mks_63.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_63;
                das_zero(__mks_63);
                das_copy((__mks_63.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                das_copy((__mks_63.id),(1));
                das_copy((__mks_63.at),(range(0,5)));
                das_copy((__mks_63.text),(nullptr));
                das_copy((__mks_63.textLen),(0));
                das_move((__mks_63.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_63;
                    das_zero(__mks_63);
                    return __mks_63;
                })())));
                das_copy((__mks_63.left),(nullptr));
                das_copy((__mks_63.right),(nullptr));
                das_copy((__mks_63.subexpr),(nullptr));
                das_copy((__mks_63.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_63;
                    __mka_63(0,__context__) = 0x0u;
                    __mka_63(1,__context__) = 0x3ff0000u;
                    __mka_63(2,__context__) = 0x0u;
                    __mka_63(3,__context__) = 0x0u;
                    __mka_63(4,__context__) = 0x0u;
                    __mka_63(5,__context__) = 0x0u;
                    __mka_63(6,__context__) = 0x0u;
                    __mka_63(7,__context__) = 0x0u;
                    return __mka_63;
                })())));
                das_copy((__mks_63.index),(0));
                return __mks_63;
            })()))));
            das_copy((__mks_63.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_63;
                das_zero(__mks_63);
                das_copy((__mks_63.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                das_copy((__mks_63.id),(2));
                das_copy((__mks_63.at),(range(5,11)));
                das_copy((__mks_63.text),(nullptr));
                das_copy((__mks_63.textLen),(0));
                das_move((__mks_63.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_63;
                    das_zero(__mks_63);
                    return __mks_63;
                })())));
                das_copy((__mks_63.left),(nullptr));
                das_copy((__mks_63.right),(nullptr));
                das_copy((__mks_63.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_63;
                    das_zero(__mks_63);
                    das_copy((__mks_63.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                    das_copy((__mks_63.id),(3));
                    das_copy((__mks_63.at),(range(5,10)));
                    das_copy((__mks_63.text),(nullptr));
                    das_copy((__mks_63.textLen),(0));
                    das_move((__mks_63.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_63;
                        das_zero(__mks_63);
                        return __mks_63;
                    })())));
                    das_copy((__mks_63.left),(nullptr));
                    das_copy((__mks_63.right),(nullptr));
                    das_copy((__mks_63.subexpr),(nullptr));
                    das_copy((__mks_63.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_63;
                        __mka_63(0,__context__) = 0x0u;
                        __mka_63(1,__context__) = 0x3ff0000u;
                        __mka_63(2,__context__) = 0x0u;
                        __mka_63(3,__context__) = 0x0u;
                        __mka_63(4,__context__) = 0x0u;
                        __mka_63(5,__context__) = 0x0u;
                        __mka_63(6,__context__) = 0x0u;
                        __mka_63(7,__context__) = 0x0u;
                        return __mka_63;
                    })())));
                    das_copy((__mks_63.index),(0));
                    return __mks_63;
                })()))));
                das_copy((__mks_63.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_63;
                    __mka_63(0,__context__) = 0x0u;
                    __mka_63(1,__context__) = 0x0u;
                    __mka_63(2,__context__) = 0x0u;
                    __mka_63(3,__context__) = 0x0u;
                    __mka_63(4,__context__) = 0x0u;
                    __mka_63(5,__context__) = 0x0u;
                    __mka_63(6,__context__) = 0x0u;
                    __mka_63(7,__context__) = 0x0u;
                    return __mka_63;
                })())));
                das_copy((__mks_63.index),(0));
                return __mks_63;
            })()))));
            das_copy((__mks_63.subexpr),(nullptr));
            das_copy((__mks_63.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_63;
                __mka_63(0,__context__) = 0x0u;
                __mka_63(1,__context__) = 0x0u;
                __mka_63(2,__context__) = 0x0u;
                __mka_63(3,__context__) = 0x0u;
                __mka_63(4,__context__) = 0x0u;
                __mka_63(5,__context__) = 0x0u;
                __mka_63(6,__context__) = 0x0u;
                __mka_63(7,__context__) = 0x0u;
                return __mka_63;
            })())));
            das_copy((__mks_63.index),(0));
            return __mks_63;
        })()))));
        das_move((_temp_make_local_63_23_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_d240a9f91d9ede74(__context__,das_arg<TDim<AutoTuple<range,char *>,1>>::pass((([&]() -> TDim<AutoTuple<range,char *>,1>& {
            _temp_make_local_63_23_176(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_63;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_63) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_63) = nullptr;
                return __mkt_63;
            })());
            return _temp_make_local_63_23_176;
        })())))));
        das_copy((_temp_make_local_63_23_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_63;
            __mka_63(0,__context__) = 0x0u;
            __mka_63(1,__context__) = 0x3ff0000u;
            __mka_63(2,__context__) = 0x0u;
            __mka_63(3,__context__) = 0x0u;
            __mka_63(4,__context__) = 0x0u;
            __mka_63(5,__context__) = 0x0u;
            __mka_63(6,__context__) = 0x0u;
            __mka_63(7,__context__) = 0x0u;
            return __mka_63;
        })())));
        das_copy((_temp_make_local_63_23_32.canEarlyOut),(true));
        return _temp_make_local_63_23_32;
    })())));/*re_uint*/
    das_global<regex::Regex,0x51c1ac4928d6065d>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_64_22_32);
        das_copy((_temp_make_local_64_22_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_64;
            das_zero(__mks_64);
            das_copy((__mks_64.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_64.id),(0));
            das_copy((__mks_64.at),(range(0,23)));
            das_copy((__mks_64.text),(nullptr));
            das_copy((__mks_64.textLen),(0));
            das_move((__mks_64.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_64;
                das_zero(__mks_64);
                return __mks_64;
            })())));
            das_copy((__mks_64.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_64;
                das_zero(__mks_64);
                das_copy((__mks_64.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                das_copy((__mks_64.id),(1));
                das_copy((__mks_64.at),(range(0,11)));
                das_copy((__mks_64.text),(nullptr));
                das_copy((__mks_64.textLen),(0));
                das_move((__mks_64.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_64;
                    das_zero(__mks_64);
                    return __mks_64;
                })())));
                das_copy((__mks_64.left),(nullptr));
                das_copy((__mks_64.right),(nullptr));
                das_copy((__mks_64.subexpr),(nullptr));
                das_copy((__mks_64.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_64;
                    __mka_64(0,__context__) = 0x0u;
                    __mka_64(1,__context__) = 0x3ff0000u;
                    __mka_64(2,__context__) = 0x7fffffeu;
                    __mka_64(3,__context__) = 0x7fffffeu;
                    __mka_64(4,__context__) = 0x0u;
                    __mka_64(5,__context__) = 0x0u;
                    __mka_64(6,__context__) = 0x0u;
                    __mka_64(7,__context__) = 0x0u;
                    return __mka_64;
                })())));
                das_copy((__mks_64.index),(0));
                return __mks_64;
            })()))));
            das_copy((__mks_64.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_64;
                das_zero(__mks_64);
                das_copy((__mks_64.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                das_copy((__mks_64.id),(2));
                das_copy((__mks_64.at),(range(11,23)));
                das_copy((__mks_64.text),(nullptr));
                das_copy((__mks_64.textLen),(0));
                das_move((__mks_64.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_64;
                    das_zero(__mks_64);
                    return __mks_64;
                })())));
                das_copy((__mks_64.left),(nullptr));
                das_copy((__mks_64.right),(nullptr));
                das_copy((__mks_64.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_64;
                    das_zero(__mks_64);
                    das_copy((__mks_64.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                    das_copy((__mks_64.id),(3));
                    das_copy((__mks_64.at),(range(11,22)));
                    das_copy((__mks_64.text),(nullptr));
                    das_copy((__mks_64.textLen),(0));
                    das_move((__mks_64.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_64;
                        das_zero(__mks_64);
                        return __mks_64;
                    })())));
                    das_copy((__mks_64.left),(nullptr));
                    das_copy((__mks_64.right),(nullptr));
                    das_copy((__mks_64.subexpr),(nullptr));
                    das_copy((__mks_64.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_64;
                        __mka_64(0,__context__) = 0x0u;
                        __mka_64(1,__context__) = 0x3ff0000u;
                        __mka_64(2,__context__) = 0x7fffffeu;
                        __mka_64(3,__context__) = 0x7fffffeu;
                        __mka_64(4,__context__) = 0x0u;
                        __mka_64(5,__context__) = 0x0u;
                        __mka_64(6,__context__) = 0x0u;
                        __mka_64(7,__context__) = 0x0u;
                        return __mka_64;
                    })())));
                    das_copy((__mks_64.index),(0));
                    return __mks_64;
                })()))));
                das_copy((__mks_64.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_64;
                    __mka_64(0,__context__) = 0x0u;
                    __mka_64(1,__context__) = 0x0u;
                    __mka_64(2,__context__) = 0x0u;
                    __mka_64(3,__context__) = 0x0u;
                    __mka_64(4,__context__) = 0x0u;
                    __mka_64(5,__context__) = 0x0u;
                    __mka_64(6,__context__) = 0x0u;
                    __mka_64(7,__context__) = 0x0u;
                    return __mka_64;
                })())));
                das_copy((__mks_64.index),(0));
                return __mks_64;
            })()))));
            das_copy((__mks_64.subexpr),(nullptr));
            das_copy((__mks_64.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_64;
                __mka_64(0,__context__) = 0x0u;
                __mka_64(1,__context__) = 0x0u;
                __mka_64(2,__context__) = 0x0u;
                __mka_64(3,__context__) = 0x0u;
                __mka_64(4,__context__) = 0x0u;
                __mka_64(5,__context__) = 0x0u;
                __mka_64(6,__context__) = 0x0u;
                __mka_64(7,__context__) = 0x0u;
                return __mka_64;
            })())));
            das_copy((__mks_64.index),(0));
            return __mks_64;
        })()))));
        das_move((_temp_make_local_64_22_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_d240a9f91d9ede74(__context__,das_arg<TDim<AutoTuple<range,char *>,1>>::pass((([&]() -> TDim<AutoTuple<range,char *>,1>& {
            _temp_make_local_64_22_176(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_64;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_64) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_64) = nullptr;
                return __mkt_64;
            })());
            return _temp_make_local_64_22_176;
        })())))));
        das_copy((_temp_make_local_64_22_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_64;
            __mka_64(0,__context__) = 0x0u;
            __mka_64(1,__context__) = 0x3ff0000u;
            __mka_64(2,__context__) = 0x7fffffeu;
            __mka_64(3,__context__) = 0x7fffffeu;
            __mka_64(4,__context__) = 0x0u;
            __mka_64(5,__context__) = 0x0u;
            __mka_64(6,__context__) = 0x0u;
            __mka_64(7,__context__) = 0x0u;
            return __mka_64;
        })())));
        das_copy((_temp_make_local_64_22_32.canEarlyOut),(true));
        return _temp_make_local_64_22_32;
    })())));/*re_hex*/
    das_global<regex::Regex,0xf00d441d5ca5cb8f>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_65_25_32);
        das_copy((_temp_make_local_65_25_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_65;
            das_zero(__mks_65);
            das_copy((__mks_65.op),(DAS_COMMENT(enum) regex::ReOp::Star));
            das_copy((__mks_65.id),(0));
            das_copy((__mks_65.at),(range(0,2)));
            das_copy((__mks_65.text),(nullptr));
            das_copy((__mks_65.textLen),(0));
            das_move((__mks_65.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_65;
                das_zero(__mks_65);
                return __mks_65;
            })())));
            das_copy((__mks_65.left),(nullptr));
            das_copy((__mks_65.right),(nullptr));
            das_copy((__mks_65.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_65;
                das_zero(__mks_65);
                das_copy((__mks_65.op),(DAS_COMMENT(enum) regex::ReOp::Any));
                das_copy((__mks_65.id),(1));
                das_copy((__mks_65.at),(range(0,1)));
                das_copy((__mks_65.text),(nullptr));
                das_copy((__mks_65.textLen),(0));
                das_move((__mks_65.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_65;
                    das_zero(__mks_65);
                    return __mks_65;
                })())));
                das_copy((__mks_65.left),(nullptr));
                das_copy((__mks_65.right),(nullptr));
                das_copy((__mks_65.subexpr),(nullptr));
                das_copy((__mks_65.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_65;
                    __mka_65(0,__context__) = 0x0u;
                    __mka_65(1,__context__) = 0x0u;
                    __mka_65(2,__context__) = 0x0u;
                    __mka_65(3,__context__) = 0x0u;
                    __mka_65(4,__context__) = 0x0u;
                    __mka_65(5,__context__) = 0x0u;
                    __mka_65(6,__context__) = 0x0u;
                    __mka_65(7,__context__) = 0x0u;
                    return __mka_65;
                })())));
                das_copy((__mks_65.index),(0));
                return __mks_65;
            })()))));
            das_copy((__mks_65.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_65;
                __mka_65(0,__context__) = 0x0u;
                __mka_65(1,__context__) = 0x0u;
                __mka_65(2,__context__) = 0x0u;
                __mka_65(3,__context__) = 0x0u;
                __mka_65(4,__context__) = 0x0u;
                __mka_65(5,__context__) = 0x0u;
                __mka_65(6,__context__) = 0x0u;
                __mka_65(7,__context__) = 0x0u;
                return __mka_65;
            })())));
            das_copy((__mks_65.index),(0));
            return __mks_65;
        })()))));
        das_move((_temp_make_local_65_25_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_d240a9f91d9ede74(__context__,das_arg<TDim<AutoTuple<range,char *>,1>>::pass((([&]() -> TDim<AutoTuple<range,char *>,1>& {
            _temp_make_local_65_25_144(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_65;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_65) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_65) = nullptr;
                return __mkt_65;
            })());
            return _temp_make_local_65_25_144;
        })())))));
        das_copy((_temp_make_local_65_25_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_65;
            __mka_65(0,__context__) = 0xffffffffu;
            __mka_65(1,__context__) = 0xffffffffu;
            __mka_65(2,__context__) = 0xffffffffu;
            __mka_65(3,__context__) = 0xffffffffu;
            __mka_65(4,__context__) = 0xffffffffu;
            __mka_65(5,__context__) = 0xffffffffu;
            __mka_65(6,__context__) = 0xffffffffu;
            __mka_65(7,__context__) = 0xffffffffu;
            return __mka_65;
        })())));
        das_copy((_temp_make_local_65_25_32.canEarlyOut),(true));
        return _temp_make_local_65_25_32;
    })())));/*re_string*/
    das_global<regex::Regex,0x9b6bf05d029f187a>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_66_28_32);
        das_copy((_temp_make_local_66_28_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_66;
            das_zero(__mks_66);
            das_copy((__mks_66.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
            das_copy((__mks_66.id),(0));
            das_copy((__mks_66.at),(range(0,15)));
            das_copy((__mks_66.text),(nullptr));
            das_copy((__mks_66.textLen),(0));
            das_move((__mks_66.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_66;
                das_zero(__mks_66);
                return __mks_66;
            })())));
            das_copy((__mks_66.left),(nullptr));
            das_copy((__mks_66.right),(nullptr));
            das_copy((__mks_66.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_66;
                das_zero(__mks_66);
                das_copy((__mks_66.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                das_copy((__mks_66.id),(1));
                das_copy((__mks_66.at),(range(0,14)));
                das_copy((__mks_66.text),(nullptr));
                das_copy((__mks_66.textLen),(0));
                das_move((__mks_66.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_66;
                    das_zero(__mks_66);
                    return __mks_66;
                })())));
                das_copy((__mks_66.left),(nullptr));
                das_copy((__mks_66.right),(nullptr));
                das_copy((__mks_66.subexpr),(nullptr));
                das_copy((__mks_66.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_66;
                    __mka_66(0,__context__) = 0x0u;
                    __mka_66(1,__context__) = 0x3ff4000u;
                    __mka_66(2,__context__) = 0x87fffffeu;
                    __mka_66(3,__context__) = 0x7fffffeu;
                    __mka_66(4,__context__) = 0x0u;
                    __mka_66(5,__context__) = 0x0u;
                    __mka_66(6,__context__) = 0x0u;
                    __mka_66(7,__context__) = 0x0u;
                    return __mka_66;
                })())));
                das_copy((__mks_66.index),(0));
                return __mks_66;
            })()))));
            das_copy((__mks_66.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_66;
                __mka_66(0,__context__) = 0x0u;
                __mka_66(1,__context__) = 0x3ff4000u;
                __mka_66(2,__context__) = 0x87fffffeu;
                __mka_66(3,__context__) = 0x7fffffeu;
                __mka_66(4,__context__) = 0x0u;
                __mka_66(5,__context__) = 0x0u;
                __mka_66(6,__context__) = 0x0u;
                __mka_66(7,__context__) = 0x0u;
                return __mka_66;
            })())));
            das_copy((__mks_66.index),(0));
            return __mks_66;
        })()))));
        das_move((_temp_make_local_66_28_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_d240a9f91d9ede74(__context__,das_arg<TDim<AutoTuple<range,char *>,1>>::pass((([&]() -> TDim<AutoTuple<range,char *>,1>& {
            _temp_make_local_66_28_144(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_66;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_66) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_66) = nullptr;
                return __mkt_66;
            })());
            return _temp_make_local_66_28_144;
        })())))));
        das_copy((_temp_make_local_66_28_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_66;
            __mka_66(0,__context__) = 0x0u;
            __mka_66(1,__context__) = 0x3ff4000u;
            __mka_66(2,__context__) = 0x87fffffeu;
            __mka_66(3,__context__) = 0x7fffffeu;
            __mka_66(4,__context__) = 0x0u;
            __mka_66(5,__context__) = 0x0u;
            __mka_66(6,__context__) = 0x0u;
            __mka_66(7,__context__) = 0x0u;
            return __mka_66;
        })())));
        das_copy((_temp_make_local_66_28_32.canEarlyOut),(true));
        return _temp_make_local_66_28_32;
    })())));/*re_file_name*/
    das_global<regex::Regex,0xb47e9636ac90db6a>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_67_24_32);
        das_copy((_temp_make_local_67_24_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_67;
            das_zero(__mks_67);
            das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_67.id),(0));
            das_copy((__mks_67.at),(range(0,29)));
            das_copy((__mks_67.text),(nullptr));
            das_copy((__mks_67.textLen),(0));
            das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_67;
                das_zero(__mks_67);
                return __mks_67;
            })())));
            das_copy((__mks_67.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_67;
                das_zero(__mks_67);
                das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                das_copy((__mks_67.id),(1));
                das_copy((__mks_67.at),(range(0,18)));
                das_copy((__mks_67.text),(nullptr));
                das_copy((__mks_67.textLen),(0));
                das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_67;
                    das_zero(__mks_67);
                    return __mks_67;
                })())));
                das_copy((__mks_67.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_67;
                    das_zero(__mks_67);
                    das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                    das_copy((__mks_67.id),(2));
                    das_copy((__mks_67.at),(range(0,12)));
                    das_copy((__mks_67.text),(nullptr));
                    das_copy((__mks_67.textLen),(0));
                    das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_67;
                        das_zero(__mks_67);
                        return __mks_67;
                    })())));
                    das_copy((__mks_67.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_67;
                        das_zero(__mks_67);
                        das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Question));
                        das_copy((__mks_67.id),(3));
                        das_copy((__mks_67.at),(range(0,7)));
                        das_copy((__mks_67.text),(nullptr));
                        das_copy((__mks_67.textLen),(0));
                        das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_67;
                            das_zero(__mks_67);
                            return __mks_67;
                        })())));
                        das_copy((__mks_67.left),(nullptr));
                        das_copy((__mks_67.right),(nullptr));
                        das_copy((__mks_67.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_67;
                            das_zero(__mks_67);
                            das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                            das_copy((__mks_67.id),(4));
                            das_copy((__mks_67.at),(range(0,6)));
                            das_copy((__mks_67.text),(nullptr));
                            das_copy((__mks_67.textLen),(0));
                            das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_67;
                                das_zero(__mks_67);
                                return __mks_67;
                            })())));
                            das_copy((__mks_67.left),(nullptr));
                            das_copy((__mks_67.right),(nullptr));
                            das_copy((__mks_67.subexpr),(nullptr));
                            das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_67;
                                __mka_67(0,__context__) = 0x0u;
                                __mka_67(1,__context__) = 0x2800u;
                                __mka_67(2,__context__) = 0x0u;
                                __mka_67(3,__context__) = 0x0u;
                                __mka_67(4,__context__) = 0x0u;
                                __mka_67(5,__context__) = 0x0u;
                                __mka_67(6,__context__) = 0x0u;
                                __mka_67(7,__context__) = 0x0u;
                                return __mka_67;
                            })())));
                            das_copy((__mks_67.index),(0));
                            return __mks_67;
                        })()))));
                        das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_67;
                            __mka_67(0,__context__) = 0x0u;
                            __mka_67(1,__context__) = 0x0u;
                            __mka_67(2,__context__) = 0x0u;
                            __mka_67(3,__context__) = 0x0u;
                            __mka_67(4,__context__) = 0x0u;
                            __mka_67(5,__context__) = 0x0u;
                            __mka_67(6,__context__) = 0x0u;
                            __mka_67(7,__context__) = 0x0u;
                            return __mka_67;
                        })())));
                        das_copy((__mks_67.index),(0));
                        return __mks_67;
                    })()))));
                    das_copy((__mks_67.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_67;
                        das_zero(__mks_67);
                        das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_67.id),(5));
                        das_copy((__mks_67.at),(range(7,12)));
                        das_copy((__mks_67.text),(nullptr));
                        das_copy((__mks_67.textLen),(0));
                        das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_67;
                            das_zero(__mks_67);
                            return __mks_67;
                        })())));
                        das_copy((__mks_67.left),(nullptr));
                        das_copy((__mks_67.right),(nullptr));
                        das_copy((__mks_67.subexpr),(nullptr));
                        das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_67;
                            __mka_67(0,__context__) = 0x0u;
                            __mka_67(1,__context__) = 0x3ff0000u;
                            __mka_67(2,__context__) = 0x0u;
                            __mka_67(3,__context__) = 0x0u;
                            __mka_67(4,__context__) = 0x0u;
                            __mka_67(5,__context__) = 0x0u;
                            __mka_67(6,__context__) = 0x0u;
                            __mka_67(7,__context__) = 0x0u;
                            return __mka_67;
                        })())));
                        das_copy((__mks_67.index),(0));
                        return __mks_67;
                    })()))));
                    das_copy((__mks_67.subexpr),(nullptr));
                    das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_67;
                        __mka_67(0,__context__) = 0x0u;
                        __mka_67(1,__context__) = 0x0u;
                        __mka_67(2,__context__) = 0x0u;
                        __mka_67(3,__context__) = 0x0u;
                        __mka_67(4,__context__) = 0x0u;
                        __mka_67(5,__context__) = 0x0u;
                        __mka_67(6,__context__) = 0x0u;
                        __mka_67(7,__context__) = 0x0u;
                        return __mka_67;
                    })())));
                    das_copy((__mks_67.index),(0));
                    return __mks_67;
                })()))));
                das_copy((__mks_67.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_67;
                    das_zero(__mks_67);
                    das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                    das_copy((__mks_67.id),(6));
                    das_copy((__mks_67.at),(range(12,18)));
                    das_copy((__mks_67.text),(nullptr));
                    das_copy((__mks_67.textLen),(0));
                    das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_67;
                        das_zero(__mks_67);
                        return __mks_67;
                    })())));
                    das_copy((__mks_67.left),(nullptr));
                    das_copy((__mks_67.right),(nullptr));
                    das_copy((__mks_67.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_67;
                        das_zero(__mks_67);
                        das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_67.id),(7));
                        das_copy((__mks_67.at),(range(12,17)));
                        das_copy((__mks_67.text),(nullptr));
                        das_copy((__mks_67.textLen),(0));
                        das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_67;
                            das_zero(__mks_67);
                            return __mks_67;
                        })())));
                        das_copy((__mks_67.left),(nullptr));
                        das_copy((__mks_67.right),(nullptr));
                        das_copy((__mks_67.subexpr),(nullptr));
                        das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_67;
                            __mka_67(0,__context__) = 0x0u;
                            __mka_67(1,__context__) = 0x3ff0000u;
                            __mka_67(2,__context__) = 0x0u;
                            __mka_67(3,__context__) = 0x0u;
                            __mka_67(4,__context__) = 0x0u;
                            __mka_67(5,__context__) = 0x0u;
                            __mka_67(6,__context__) = 0x0u;
                            __mka_67(7,__context__) = 0x0u;
                            return __mka_67;
                        })())));
                        das_copy((__mks_67.index),(0));
                        return __mks_67;
                    })()))));
                    das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_67;
                        __mka_67(0,__context__) = 0x0u;
                        __mka_67(1,__context__) = 0x0u;
                        __mka_67(2,__context__) = 0x0u;
                        __mka_67(3,__context__) = 0x0u;
                        __mka_67(4,__context__) = 0x0u;
                        __mka_67(5,__context__) = 0x0u;
                        __mka_67(6,__context__) = 0x0u;
                        __mka_67(7,__context__) = 0x0u;
                        return __mka_67;
                    })())));
                    das_copy((__mks_67.index),(0));
                    return __mks_67;
                })()))));
                das_copy((__mks_67.subexpr),(nullptr));
                das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_67;
                    __mka_67(0,__context__) = 0x0u;
                    __mka_67(1,__context__) = 0x0u;
                    __mka_67(2,__context__) = 0x0u;
                    __mka_67(3,__context__) = 0x0u;
                    __mka_67(4,__context__) = 0x0u;
                    __mka_67(5,__context__) = 0x0u;
                    __mka_67(6,__context__) = 0x0u;
                    __mka_67(7,__context__) = 0x0u;
                    return __mka_67;
                })())));
                das_copy((__mks_67.index),(0));
                return __mks_67;
            })()))));
            das_copy((__mks_67.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_67;
                das_zero(__mks_67);
                das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Question));
                das_copy((__mks_67.id),(8));
                das_copy((__mks_67.at),(range(18,29)));
                das_copy((__mks_67.text),(nullptr));
                das_copy((__mks_67.textLen),(0));
                das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_67;
                    das_zero(__mks_67);
                    return __mks_67;
                })())));
                das_copy((__mks_67.left),(nullptr));
                das_copy((__mks_67.right),(nullptr));
                das_copy((__mks_67.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_67;
                    das_zero(__mks_67);
                    das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                    das_copy((__mks_67.id),(9));
                    das_copy((__mks_67.at),(range(18,28)));
                    das_copy((__mks_67.text),(nullptr));
                    das_copy((__mks_67.textLen),(0));
                    das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_67;
                        das_zero(__mks_67);
                        return __mks_67;
                    })())));
                    das_copy((__mks_67.left),(nullptr));
                    das_copy((__mks_67.right),(nullptr));
                    das_copy((__mks_67.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_67;
                        das_zero(__mks_67);
                        das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_67.id),(10));
                        das_copy((__mks_67.at),(range(19,27)));
                        das_copy((__mks_67.text),(nullptr));
                        das_copy((__mks_67.textLen),(0));
                        das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_67;
                            das_zero(__mks_67);
                            return __mks_67;
                        })())));
                        das_copy((__mks_67.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_67;
                            das_zero(__mks_67);
                            das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                            das_copy((__mks_67.id),(11));
                            das_copy((__mks_67.at),(range(19,21)));
                            das_copy((__mks_67.text),(((char *) ".")));
                            das_copy((__mks_67.textLen),(1));
                            das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_67;
                                das_zero(__mks_67);
                                return __mks_67;
                            })())));
                            das_copy((__mks_67.left),(nullptr));
                            das_copy((__mks_67.right),(nullptr));
                            das_copy((__mks_67.subexpr),(nullptr));
                            das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_67;
                                __mka_67(0,__context__) = 0x0u;
                                __mka_67(1,__context__) = 0x0u;
                                __mka_67(2,__context__) = 0x0u;
                                __mka_67(3,__context__) = 0x0u;
                                __mka_67(4,__context__) = 0x0u;
                                __mka_67(5,__context__) = 0x0u;
                                __mka_67(6,__context__) = 0x0u;
                                __mka_67(7,__context__) = 0x0u;
                                return __mka_67;
                            })())));
                            das_copy((__mks_67.index),(0));
                            return __mks_67;
                        })()))));
                        das_copy((__mks_67.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_67;
                            das_zero(__mks_67);
                            das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                            das_copy((__mks_67.id),(12));
                            das_copy((__mks_67.at),(range(21,27)));
                            das_copy((__mks_67.text),(nullptr));
                            das_copy((__mks_67.textLen),(0));
                            das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_67;
                                das_zero(__mks_67);
                                return __mks_67;
                            })())));
                            das_copy((__mks_67.left),(nullptr));
                            das_copy((__mks_67.right),(nullptr));
                            das_copy((__mks_67.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_67;
                                das_zero(__mks_67);
                                das_copy((__mks_67.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_67.id),(13));
                                das_copy((__mks_67.at),(range(21,26)));
                                das_copy((__mks_67.text),(nullptr));
                                das_copy((__mks_67.textLen),(0));
                                das_move((__mks_67.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_67;
                                    das_zero(__mks_67);
                                    return __mks_67;
                                })())));
                                das_copy((__mks_67.left),(nullptr));
                                das_copy((__mks_67.right),(nullptr));
                                das_copy((__mks_67.subexpr),(nullptr));
                                das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_67;
                                    __mka_67(0,__context__) = 0x0u;
                                    __mka_67(1,__context__) = 0x3ff0000u;
                                    __mka_67(2,__context__) = 0x0u;
                                    __mka_67(3,__context__) = 0x0u;
                                    __mka_67(4,__context__) = 0x0u;
                                    __mka_67(5,__context__) = 0x0u;
                                    __mka_67(6,__context__) = 0x0u;
                                    __mka_67(7,__context__) = 0x0u;
                                    return __mka_67;
                                })())));
                                das_copy((__mks_67.index),(0));
                                return __mks_67;
                            })()))));
                            das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_67;
                                __mka_67(0,__context__) = 0x0u;
                                __mka_67(1,__context__) = 0x0u;
                                __mka_67(2,__context__) = 0x0u;
                                __mka_67(3,__context__) = 0x0u;
                                __mka_67(4,__context__) = 0x0u;
                                __mka_67(5,__context__) = 0x0u;
                                __mka_67(6,__context__) = 0x0u;
                                __mka_67(7,__context__) = 0x0u;
                                return __mka_67;
                            })())));
                            das_copy((__mks_67.index),(0));
                            return __mks_67;
                        })()))));
                        das_copy((__mks_67.subexpr),(nullptr));
                        das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_67;
                            __mka_67(0,__context__) = 0x0u;
                            __mka_67(1,__context__) = 0x0u;
                            __mka_67(2,__context__) = 0x0u;
                            __mka_67(3,__context__) = 0x0u;
                            __mka_67(4,__context__) = 0x0u;
                            __mka_67(5,__context__) = 0x0u;
                            __mka_67(6,__context__) = 0x0u;
                            __mka_67(7,__context__) = 0x0u;
                            return __mka_67;
                        })())));
                        das_copy((__mks_67.index),(0));
                        return __mks_67;
                    })()))));
                    das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_67;
                        __mka_67(0,__context__) = 0x0u;
                        __mka_67(1,__context__) = 0x0u;
                        __mka_67(2,__context__) = 0x0u;
                        __mka_67(3,__context__) = 0x0u;
                        __mka_67(4,__context__) = 0x0u;
                        __mka_67(5,__context__) = 0x0u;
                        __mka_67(6,__context__) = 0x0u;
                        __mka_67(7,__context__) = 0x0u;
                        return __mka_67;
                    })())));
                    das_copy((__mks_67.index),(1));
                    return __mks_67;
                })()))));
                das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_67;
                    __mka_67(0,__context__) = 0x0u;
                    __mka_67(1,__context__) = 0x0u;
                    __mka_67(2,__context__) = 0x0u;
                    __mka_67(3,__context__) = 0x0u;
                    __mka_67(4,__context__) = 0x0u;
                    __mka_67(5,__context__) = 0x0u;
                    __mka_67(6,__context__) = 0x0u;
                    __mka_67(7,__context__) = 0x0u;
                    return __mka_67;
                })())));
                das_copy((__mks_67.index),(0));
                return __mks_67;
            })()))));
            das_copy((__mks_67.subexpr),(nullptr));
            das_copy((__mks_67.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_67;
                __mka_67(0,__context__) = 0x0u;
                __mka_67(1,__context__) = 0x0u;
                __mka_67(2,__context__) = 0x0u;
                __mka_67(3,__context__) = 0x0u;
                __mka_67(4,__context__) = 0x0u;
                __mka_67(5,__context__) = 0x0u;
                __mka_67(6,__context__) = 0x0u;
                __mka_67(7,__context__) = 0x0u;
                return __mka_67;
            })())));
            das_copy((__mks_67.index),(0));
            return __mks_67;
        })()))));
        das_move((_temp_make_local_67_24_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_c7508367b622b66d(__context__,das_arg<TDim<AutoTuple<range,char *>,2>>::pass((([&]() -> TDim<AutoTuple<range,char *>,2>& {
            _temp_make_local_67_24_336(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_67;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_67) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_67) = nullptr;
                return __mkt_67;
            })());
            _temp_make_local_67_24_336(1,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_67;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_67) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_67) = ((char *) "1");
                return __mkt_67;
            })());
            return _temp_make_local_67_24_336;
        })())))));
        das_copy((_temp_make_local_67_24_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_67;
            __mka_67(0,__context__) = 0x0u;
            __mka_67(1,__context__) = 0x3ff2800u;
            __mka_67(2,__context__) = 0x0u;
            __mka_67(3,__context__) = 0x0u;
            __mka_67(4,__context__) = 0x0u;
            __mka_67(5,__context__) = 0x0u;
            __mka_67(6,__context__) = 0x0u;
            __mka_67(7,__context__) = 0x0u;
            return __mka_67;
        })())));
        das_copy((_temp_make_local_67_24_32.canEarlyOut),(true));
        return _temp_make_local_67_24_32;
    })())));/*re_float*/
    das_global<regex::Regex,0xcced403b1d8ad2c6>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_68_17_32);
        das_copy((_temp_make_local_68_17_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_68;
            das_zero(__mks_68);
            das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Group));
            das_copy((__mks_68.id),(0));
            das_copy((__mks_68.at),(range(0,55)));
            das_copy((__mks_68.text),(nullptr));
            das_copy((__mks_68.textLen),(0));
            das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_68;
                das_zero(__mks_68);
                return __mks_68;
            })())));
            das_copy((__mks_68.left),(nullptr));
            das_copy((__mks_68.right),(nullptr));
            das_copy((__mks_68.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_68;
                das_zero(__mks_68);
                das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Union));
                das_copy((__mks_68.id),(1));
                das_copy((__mks_68.at),(range(1,54)));
                das_copy((__mks_68.text),(nullptr));
                das_copy((__mks_68.textLen),(0));
                das_move((__mks_68.all),(_FuncbuiltinTickto_array_moveTick3185538323411982277_37174cace0912109(__context__,das_arg<TDim<regex::ReNode *,11>>::pass((([&]() -> TDim<regex::ReNode *,11>& {
                    _temp_make_local_68_17_144(0,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(2));
                        das_copy((__mks_68.at),(range(46,50)));
                        das_copy((__mks_68.text),(((char *) "nine")));
                        das_copy((__mks_68.textLen),(4));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(1,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(3));
                        das_copy((__mks_68.at),(range(51,54)));
                        das_copy((__mks_68.text),(((char *) "ten")));
                        das_copy((__mks_68.textLen),(3));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(2,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(4));
                        das_copy((__mks_68.at),(range(40,45)));
                        das_copy((__mks_68.text),(((char *) "eight")));
                        das_copy((__mks_68.textLen),(5));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(3,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(5));
                        das_copy((__mks_68.at),(range(34,39)));
                        das_copy((__mks_68.text),(((char *) "seven")));
                        das_copy((__mks_68.textLen),(5));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(4,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(6));
                        das_copy((__mks_68.at),(range(30,33)));
                        das_copy((__mks_68.text),(((char *) "six")));
                        das_copy((__mks_68.textLen),(3));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(5,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(7));
                        das_copy((__mks_68.at),(range(25,29)));
                        das_copy((__mks_68.text),(((char *) "five")));
                        das_copy((__mks_68.textLen),(4));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(6,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(8));
                        das_copy((__mks_68.at),(range(20,24)));
                        das_copy((__mks_68.text),(((char *) "four")));
                        das_copy((__mks_68.textLen),(4));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(7,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(9));
                        das_copy((__mks_68.at),(range(14,19)));
                        das_copy((__mks_68.text),(((char *) "three")));
                        das_copy((__mks_68.textLen),(5));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(8,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(10));
                        das_copy((__mks_68.at),(range(10,13)));
                        das_copy((__mks_68.text),(((char *) "two")));
                        das_copy((__mks_68.textLen),(3));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(9,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(11));
                        das_copy((__mks_68.at),(range(6,9)));
                        das_copy((__mks_68.text),(((char *) "one")));
                        das_copy((__mks_68.textLen),(3));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    _temp_make_local_68_17_144(10,__context__) = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_68;
                        das_zero(__mks_68);
                        das_copy((__mks_68.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_68.id),(12));
                        das_copy((__mks_68.at),(range(1,5)));
                        das_copy((__mks_68.text),(((char *) "zero")));
                        das_copy((__mks_68.textLen),(4));
                        das_move((__mks_68.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_68;
                            das_zero(__mks_68);
                            return __mks_68;
                        })())));
                        das_copy((__mks_68.left),(nullptr));
                        das_copy((__mks_68.right),(nullptr));
                        das_copy((__mks_68.subexpr),(nullptr));
                        das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_68;
                            __mka_68(0,__context__) = 0x0u;
                            __mka_68(1,__context__) = 0x0u;
                            __mka_68(2,__context__) = 0x0u;
                            __mka_68(3,__context__) = 0x0u;
                            __mka_68(4,__context__) = 0x0u;
                            __mka_68(5,__context__) = 0x0u;
                            __mka_68(6,__context__) = 0x0u;
                            __mka_68(7,__context__) = 0x0u;
                            return __mka_68;
                        })())));
                        das_copy((__mks_68.index),(0));
                        return __mks_68;
                    })()));
                    return _temp_make_local_68_17_144;
                })())))));
                das_copy((__mks_68.left),(nullptr));
                das_copy((__mks_68.right),(nullptr));
                das_copy((__mks_68.subexpr),(nullptr));
                das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_68;
                    __mka_68(0,__context__) = 0x0u;
                    __mka_68(1,__context__) = 0x0u;
                    __mka_68(2,__context__) = 0x0u;
                    __mka_68(3,__context__) = 0x0u;
                    __mka_68(4,__context__) = 0x0u;
                    __mka_68(5,__context__) = 0x0u;
                    __mka_68(6,__context__) = 0x0u;
                    __mka_68(7,__context__) = 0x0u;
                    return __mka_68;
                })())));
                das_copy((__mks_68.index),(0));
                return __mks_68;
            })()))));
            das_copy((__mks_68.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_68;
                __mka_68(0,__context__) = 0x0u;
                __mka_68(1,__context__) = 0x0u;
                __mka_68(2,__context__) = 0x0u;
                __mka_68(3,__context__) = 0x0u;
                __mka_68(4,__context__) = 0x0u;
                __mka_68(5,__context__) = 0x0u;
                __mka_68(6,__context__) = 0x0u;
                __mka_68(7,__context__) = 0x0u;
                return __mka_68;
            })())));
            das_copy((__mks_68.index),(1));
            return __mks_68;
        })()))));
        das_move((_temp_make_local_68_17_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_c7508367b622b66d(__context__,das_arg<TDim<AutoTuple<range,char *>,2>>::pass((([&]() -> TDim<AutoTuple<range,char *>,2>& {
            _temp_make_local_68_17_416(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_68;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_68) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_68) = nullptr;
                return __mkt_68;
            })());
            _temp_make_local_68_17_416(1,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_68;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_68) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_68) = ((char *) "1");
                return __mkt_68;
            })());
            return _temp_make_local_68_17_416;
        })())))));
        das_copy((_temp_make_local_68_17_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_68;
            __mka_68(0,__context__) = 0x0u;
            __mka_68(1,__context__) = 0x0u;
            __mka_68(2,__context__) = 0x0u;
            __mka_68(3,__context__) = 0x418c060u;
            __mka_68(4,__context__) = 0x0u;
            __mka_68(5,__context__) = 0x0u;
            __mka_68(6,__context__) = 0x0u;
            __mka_68(7,__context__) = 0x0u;
            return __mka_68;
        })())));
        das_copy((_temp_make_local_68_17_32.canEarlyOut),(true));
        return _temp_make_local_68_17_32;
    })())));/*re_number*/
    das_global<TArray<int32_t>,0x6d18b96a1112d3df>(__context__) = _FuncbuiltinTickto_array_moveTick3185538323411982277_afe546e6423dd8f3(__context__,das_arg<TDim<int32_t,12>>::pass((([&]() -> TDim<int32_t,12>& {
        _temp_make_local_338_22_32(0,__context__) = 0;
        _temp_make_local_338_22_32(1,__context__) = 31;
        _temp_make_local_338_22_32(2,__context__) = 59;
        _temp_make_local_338_22_32(3,__context__) = 90;
        _temp_make_local_338_22_32(4,__context__) = 120;
        _temp_make_local_338_22_32(5,__context__) = 151;
        _temp_make_local_338_22_32(6,__context__) = 181;
        _temp_make_local_338_22_32(7,__context__) = 212;
        _temp_make_local_338_22_32(8,__context__) = 243;
        _temp_make_local_338_22_32(9,__context__) = 273;
        _temp_make_local_338_22_32(10,__context__) = 304;
        _temp_make_local_338_22_32(11,__context__) = 334;
        return _temp_make_local_338_22_32;
    })())));/*`static`at_line_337`offset*/
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_93e93a07b629c7a3 ( Context * __context__, TArray<faker::MonthNameAndDay> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<faker::MonthNameAndDay>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_de3bec42d99954f8 ( Context * __context__, TArray<AutoTuple<range,char *>> &  __Arr_rename_at_68_2, int32_t __newSize_rename_at_68_3 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<range,char *>>>::pass(__Arr_rename_at_68_2),__newSize_rename_at_68_3,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_b92c6f816db4602 ( Context * __context__, TArray<regex::ReNode *> &  __Arr_rename_at_68_4, int32_t __newSize_rename_at_68_5 )
{
    builtin_array_resize(das_arg<TArray<regex::ReNode *>>::pass(__Arr_rename_at_68_4),__newSize_rename_at_68_5,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_856be2a5daceccf2 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_68_6, int32_t __newSize_rename_at_68_7 )
{
    builtin_array_resize(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_68_6),__newSize_rename_at_68_7,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_bd2ed0e765c61464 ( Context * __context__, TDim<faker::MonthNameAndDay,12> const  &  __a_rename_at_581_8 )
{
    return das_auto_cast<int32_t>::cast(12);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_7bf97eaaff0543bb ( Context * __context__, TDim<AutoTuple<range,char *>,1> const  &  __a_rename_at_581_9 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_f3bb1928c94b9b52 ( Context * __context__, TDim<AutoTuple<range,char *>,2> const  &  __a_rename_at_581_10 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_ca7d613c38c76fb8 ( Context * __context__, TDim<regex::ReNode *,11> const  &  __a_rename_at_581_11 )
{
    return das_auto_cast<int32_t>::cast(11);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_67d7519b16eea304 ( Context * __context__, TDim<int32_t,12> const  &  __a_rename_at_581_12 )
{
    return das_auto_cast<int32_t>::cast(12);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_8dc91cad3b75b3c8 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1215_13 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1215_13),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0 ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) &  __it_rename_at_1256_14, uint32_t & __value_rename_at_1256_15 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__it_rename_at_1256_14),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1256_15)),__context__));
}

inline void _FuncbuiltinTickreserveTick3994685146752941225_71af0b01dbdb30b2 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_125_16, int32_t __newSize_rename_at_125_17 )
{
    builtin_array_reserve(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_125_16),__newSize_rename_at_125_17,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_d8f0a51920afdbe4 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_18, uint8_t __value_rename_at_165_19 )
{
    das_copy(__Arr_rename_at_165_18(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_18),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_19);
}

inline faker::Faker Faker_120e1a6bb086dd13 ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) &  __rng_rename_at_57_20 )
{
    return /* <- */ das_auto_cast_move<faker::Faker>::cast((([&]() -> faker::Faker {
        faker::Faker __mks_58 = Faker_b38307f5f81d41da(__context__);
        das_move((__mks_58.rnd),(__rng_rename_at_57_20));
        return __mks_58;
    })()));
}

inline char * long_string_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_70_21 )
{
    uint32_t __len_rename_at_72_22; memset((void*)&__len_rename_at_72_22,0,sizeof(__len_rename_at_72_22));
    TArray<uint8_t> __arr_rename_at_75_23; memset((void*)&__arr_rename_at_75_23,0,sizeof(__arr_rename_at_75_23));
    uint32_t __Ch_rename_at_78_25; memset((void*)&__Ch_rename_at_78_25,0,sizeof(__Ch_rename_at_78_25));
    /* finally */ auto __finally_70= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_8dc91cad3b75b3c8(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_75_23));
    /* end finally */ });
    __len_rename_at_72_22 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_70_21.rnd),__len_rename_at_72_22);
    __len_rename_at_72_22 %= __faker_rename_at_70_21.max_long_string;
    das_zero(__arr_rename_at_75_23);
    _FuncbuiltinTickreserveTick3994685146752941225_71af0b01dbdb30b2(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_75_23),int32_t(__len_rename_at_72_22));
    {
        bool __need_loop_77 = true;
        // t: uint const
        das_iterator<urange> __t_iterator(mk_urange(__len_rename_at_72_22));
        uint32_t __t_rename_at_77_24;
        __need_loop_77 = __t_iterator.first(__context__,(__t_rename_at_77_24)) && __need_loop_77;
        for ( ; __need_loop_77 ; __need_loop_77 = __t_iterator.next(__context__,(__t_rename_at_77_24)) )
        {
            __Ch_rename_at_78_25 = 0x0u;
            _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_70_21.rnd),__Ch_rename_at_78_25);
            _FuncbuiltinTickpushTick14133213201864676143_d8f0a51920afdbe4(__context__,das_arg<TArray<uint8_t>>::pass(__arr_rename_at_75_23),uint8_t(__Ch_rename_at_78_25));
        }
        __t_iterator.close(__context__,(__t_rename_at_77_24));
    };
    return das_auto_cast<char *>::cast(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__arr_rename_at_75_23),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * any_string_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_85_26 )
{
    return das_auto_cast<char *>::cast(das_invoke_function<char * const >::invoke<regex::Regex &,Sequence DAS_COMMENT((uint32_t)) &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::re_gen S<regex::Regex> Y<ReGenRandom>1<u>G*/ 0x4a287ee9ffbe5ccd)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0xf00d441d5ca5cb8f>(__context__) /*re_string*/),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_85_26.rnd)));
}

inline char * any_file_name_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_90_27 )
{
    return das_auto_cast<char *>::cast(das_invoke_function<char * const >::invoke<regex::Regex &,Sequence DAS_COMMENT((uint32_t)) &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::re_gen S<regex::Regex> Y<ReGenRandom>1<u>G*/ 0x4a287ee9ffbe5ccd)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0x9b6bf05d029f187a>(__context__) /*re_file_name*/),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_90_27.rnd)));
}

inline int32_t random_int_db927c5b51337757 ( Context * __context__, faker::Faker &  __faker_rename_at_95_28 )
{
    uint32_t __i_rename_at_97_29 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_95_28.rnd),__i_rename_at_97_29);
    return das_auto_cast<int32_t>::cast(int32_t(__i_rename_at_97_29));
}

inline uint32_t random_uint_1fc7e41ed0d6ffdd ( Context * __context__, faker::Faker &  __faker_rename_at_102_30 )
{
    uint32_t __i_rename_at_104_31 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_102_30.rnd),__i_rename_at_104_31);
    return das_auto_cast<uint32_t>::cast(__i_rename_at_104_31);
}

inline int8_t random_int8_9e48de99fcf0b4bb ( Context * __context__, faker::Faker &  __faker_rename_at_109_32 )
{
    uint32_t __i_rename_at_111_33 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_109_32.rnd),__i_rename_at_111_33);
    return das_auto_cast<int8_t>::cast(int8_t(__i_rename_at_111_33));
}

inline uint8_t random_uint8_eb9ad5532c2efba ( Context * __context__, faker::Faker &  __faker_rename_at_116_34 )
{
    uint32_t __i_rename_at_118_35 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_116_34.rnd),__i_rename_at_118_35);
    return das_auto_cast<uint8_t>::cast(uint8_t(__i_rename_at_118_35));
}

inline int16_t random_int16_65cfde74f34ebeec ( Context * __context__, faker::Faker &  __faker_rename_at_123_36 )
{
    uint32_t __i_rename_at_125_37 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_123_36.rnd),__i_rename_at_125_37);
    return das_auto_cast<int16_t>::cast(int16_t(__i_rename_at_125_37));
}

inline uint16_t random_uint16_27625a3cd1234fe8 ( Context * __context__, faker::Faker &  __faker_rename_at_130_38 )
{
    uint32_t __i_rename_at_132_39 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_130_38.rnd),__i_rename_at_132_39);
    return das_auto_cast<uint16_t>::cast(uint16_t(__i_rename_at_132_39));
}

inline float random_float_3b1c10c7ec3bd27d ( Context * __context__, faker::Faker &  __faker_rename_at_137_40 )
{
    uint32_t __i_rename_at_139_41 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_137_40.rnd),__i_rename_at_139_41);
    return das_auto_cast<float>::cast(das_invoke_function<float>::invoke<uint32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@math_bits::uint_bits_to_float Cu*/ 0x9855885d8178931f)),__i_rename_at_139_41));
}

inline int2 random_int2_163549ee4deaeda5 ( Context * __context__, faker::Faker &  __faker_rename_at_144_42 )
{
    return das_auto_cast<int2>::cast(int2(random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_144_42)),random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_144_42))));
}

inline range random_range_82207fb65a0097a8 ( Context * __context__, faker::Faker &  __faker_rename_at_149_43 )
{
    return das_auto_cast<range>::cast(range(random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_149_43)),random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_149_43))));
}

inline range64 random_range64_76c394d5233908be ( Context * __context__, faker::Faker &  __faker_rename_at_154_44 )
{
    return das_auto_cast<range64>::cast(range64(random_int64_20af9383ba5df64b(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_154_44)),random_int64_20af9383ba5df64b(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_154_44))));
}

inline int3 random_int3_4e81314eab7f0e58 ( Context * __context__, faker::Faker &  __faker_rename_at_159_45 )
{
    return das_auto_cast<int3>::cast(int3(random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_159_45)),random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_159_45)),random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_159_45))));
}

inline int4 random_int4_24c671e0ae1fb943 ( Context * __context__, faker::Faker &  __faker_rename_at_164_46 )
{
    return das_auto_cast<int4>::cast(int4(random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_164_46)),random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_164_46)),random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_164_46)),random_int_db927c5b51337757(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_164_46))));
}

inline uint2 random_uint2_74dfcbc709ea1d13 ( Context * __context__, faker::Faker &  __faker_rename_at_169_47 )
{
    return das_auto_cast<uint2>::cast(uint2(random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_169_47)),random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_169_47))));
}

inline urange random_urange_5f399937e7e7ad83 ( Context * __context__, faker::Faker &  __faker_rename_at_174_48 )
{
    return das_auto_cast<urange>::cast(urange(random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_174_48)),random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_174_48))));
}

inline urange64 random_urange64_64b82994fec321bd ( Context * __context__, faker::Faker &  __faker_rename_at_179_49 )
{
    return das_auto_cast<urange64>::cast(urange64(random_uint64_9d78736b94d2bb65(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_179_49)),random_uint64_9d78736b94d2bb65(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_179_49))));
}

inline uint3 random_uint3_82c843a2e2dcd1ad ( Context * __context__, faker::Faker &  __faker_rename_at_184_50 )
{
    return das_auto_cast<uint3>::cast(uint3(random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_184_50)),random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_184_50)),random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_184_50))));
}

inline uint4 random_uint4_c168676f0192f143 ( Context * __context__, faker::Faker &  __faker_rename_at_189_51 )
{
    return das_auto_cast<uint4>::cast(uint4(random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_189_51)),random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_189_51)),random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_189_51)),random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_189_51))));
}

inline float2 random_float2_de4a2c3aa9fc390 ( Context * __context__, faker::Faker &  __faker_rename_at_194_52 )
{
    return das_auto_cast<float2>::cast(float2(random_float_3b1c10c7ec3bd27d(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_194_52)),random_float_3b1c10c7ec3bd27d(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_194_52))));
}

inline float3 random_float3_176dbf53ad107c9a ( Context * __context__, faker::Faker &  __faker_rename_at_199_53 )
{
    return das_auto_cast<float3>::cast(float3(random_float_3b1c10c7ec3bd27d(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_199_53)),random_float_3b1c10c7ec3bd27d(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_199_53)),random_float_3b1c10c7ec3bd27d(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_199_53))));
}

inline float4 random_float4_d05db2140ce31987 ( Context * __context__, faker::Faker &  __faker_rename_at_204_54 )
{
    return das_auto_cast<float4>::cast(float4(random_float_3b1c10c7ec3bd27d(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_204_54)),random_float_3b1c10c7ec3bd27d(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_204_54)),random_float_3b1c10c7ec3bd27d(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_204_54)),random_float_3b1c10c7ec3bd27d(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_204_54))));
}

inline float3x3 random_float3x3_e049ec11115607a5 ( Context * __context__, faker::Faker &  __faker_rename_at_209_55 )
{
    float3x3 __m_rename_at_211_56;das_zero(__m_rename_at_211_56);
    das_copy(das_index<float3x3>::at(__m_rename_at_211_56,0,__context__),random_float3_176dbf53ad107c9a(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_209_55)));
    das_copy(das_index<float3x3>::at(__m_rename_at_211_56,1,__context__),random_float3_176dbf53ad107c9a(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_209_55)));
    das_copy(das_index<float3x3>::at(__m_rename_at_211_56,2,__context__),random_float3_176dbf53ad107c9a(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_209_55)));
    return das_auto_cast_ref<float3x3>::cast(__m_rename_at_211_56);
}

inline float3x4 random_float3x4_ae3c09f14778b039 ( Context * __context__, faker::Faker &  __faker_rename_at_218_57 )
{
    float3x4 __m_rename_at_220_58;das_zero(__m_rename_at_220_58);
    das_copy(das_index<float3x4>::at(__m_rename_at_220_58,0,__context__),random_float3_176dbf53ad107c9a(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_218_57)));
    das_copy(das_index<float3x4>::at(__m_rename_at_220_58,1,__context__),random_float3_176dbf53ad107c9a(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_218_57)));
    das_copy(das_index<float3x4>::at(__m_rename_at_220_58,2,__context__),random_float3_176dbf53ad107c9a(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_218_57)));
    das_copy(das_index<float3x4>::at(__m_rename_at_220_58,3,__context__),random_float3_176dbf53ad107c9a(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_218_57)));
    return das_auto_cast_ref<float3x4>::cast(__m_rename_at_220_58);
}

inline float4x4 random_float4x4_4db49814bded2b7d ( Context * __context__, faker::Faker &  __faker_rename_at_228_59 )
{
    float4x4 __m_rename_at_230_60;das_zero(__m_rename_at_230_60);
    das_copy(das_index<float4x4>::at(__m_rename_at_230_60,0,__context__),random_float4_d05db2140ce31987(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_228_59)));
    das_copy(das_index<float4x4>::at(__m_rename_at_230_60,1,__context__),random_float4_d05db2140ce31987(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_228_59)));
    das_copy(das_index<float4x4>::at(__m_rename_at_230_60,2,__context__),random_float4_d05db2140ce31987(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_228_59)));
    das_copy(das_index<float4x4>::at(__m_rename_at_230_60,3,__context__),random_float4_d05db2140ce31987(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_228_59)));
    return das_auto_cast_ref<float4x4>::cast(__m_rename_at_230_60);
}

inline int64_t random_int64_20af9383ba5df64b ( Context * __context__, faker::Faker &  __faker_rename_at_245_61 )
{
    AutoVariant<TDim<uint32_t,2>,double,int64_t,uint64_t> __b_rename_at_247_62 = (([&]() -> AutoVariant<TDim<uint32_t,2>,double,int64_t,uint64_t> {
        AutoVariant<TDim<uint32_t,2>,double,int64_t,uint64_t> __mkv_247;
        das_get_auto_variant_field<TDim<uint32_t,2>,0,TDim<uint32_t,2>,double,int64_t,uint64_t>::set(__mkv_247) = (([&]() -> TDim<uint32_t,2> {
            TDim<uint32_t,2> __mka_247;
            __mka_247(0,__context__) = random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_245_61));
            __mka_247(1,__context__) = random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_245_61));
            return __mka_247;
        })());
        return __mkv_247;
    })());
    return das_auto_cast<int64_t>::cast(das_get_auto_variant_field<int64_t,2,TDim<uint32_t,2>,double,int64_t,uint64_t>::get(__b_rename_at_247_62));
}

inline uint64_t random_uint64_9d78736b94d2bb65 ( Context * __context__, faker::Faker &  __faker_rename_at_251_63 )
{
    AutoVariant<TDim<uint32_t,2>,double,int64_t,uint64_t> __b_rename_at_253_64 = (([&]() -> AutoVariant<TDim<uint32_t,2>,double,int64_t,uint64_t> {
        AutoVariant<TDim<uint32_t,2>,double,int64_t,uint64_t> __mkv_253;
        das_get_auto_variant_field<TDim<uint32_t,2>,0,TDim<uint32_t,2>,double,int64_t,uint64_t>::set(__mkv_253) = (([&]() -> TDim<uint32_t,2> {
            TDim<uint32_t,2> __mka_253;
            __mka_253(0,__context__) = random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_251_63));
            __mka_253(1,__context__) = random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_251_63));
            return __mka_253;
        })());
        return __mkv_253;
    })());
    return das_auto_cast<uint64_t>::cast(das_get_auto_variant_field<uint64_t,3,TDim<uint32_t,2>,double,int64_t,uint64_t>::get(__b_rename_at_253_64));
}

inline double random_double_ee86008619feb237 ( Context * __context__, faker::Faker &  __faker_rename_at_257_65 )
{
    AutoVariant<TDim<uint32_t,2>,double,int64_t,uint64_t> __b_rename_at_259_66 = (([&]() -> AutoVariant<TDim<uint32_t,2>,double,int64_t,uint64_t> {
        AutoVariant<TDim<uint32_t,2>,double,int64_t,uint64_t> __mkv_259;
        das_get_auto_variant_field<TDim<uint32_t,2>,0,TDim<uint32_t,2>,double,int64_t,uint64_t>::set(__mkv_259) = (([&]() -> TDim<uint32_t,2> {
            TDim<uint32_t,2> __mka_259;
            __mka_259(0,__context__) = random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_257_65));
            __mka_259(1,__context__) = random_uint_1fc7e41ed0d6ffdd(__context__,das_arg<faker::Faker>::pass(__faker_rename_at_257_65));
            return __mka_259;
        })());
        return __mkv_259;
    })());
    return das_auto_cast<double>::cast(das_get_auto_variant_field<double,1,TDim<uint32_t,2>,double,int64_t,uint64_t>::get(__b_rename_at_259_66));
}

inline TDim<uint32_t,8> any_set_d6fb89a5c88f1b2c ( Context * __context__, faker::Faker &  __faker_rename_at_263_67 )
{
    TDim<uint32_t,8> __ch_rename_at_265_68;das_zero(__ch_rename_at_265_68);
    {
        bool __need_loop_266 = true;
        // c: uint& -const
        das_iterator<TDim<uint32_t,8>> __c_iterator(__ch_rename_at_265_68);
        uint32_t * __c_rename_at_266_69;
        __need_loop_266 = __c_iterator.first(__context__,(__c_rename_at_266_69)) && __need_loop_266;
        for ( ; __need_loop_266 ; __need_loop_266 = __c_iterator.next(__context__,(__c_rename_at_266_69)) )
        {
            _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_263_67.rnd),(*__c_rename_at_266_69));
        }
        __c_iterator.close(__context__,(__c_rename_at_266_69));
    };
    return das_auto_cast_ref<TDim<uint32_t,8>>::cast(__ch_rename_at_265_68);
}

inline int32_t any_char_db927c5b51337757 ( Context * __context__, faker::Faker &  __faker_rename_at_272_70 )
{
    uint32_t __ch_rename_at_274_71 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_272_70.rnd),__ch_rename_at_274_71);
    return das_auto_cast<int32_t>::cast(int32_t((SimPolicy<uint32_t>::Mod(__ch_rename_at_274_71,0xffu,*__context__,nullptr)) + 0x1u));
}

inline char * number_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_279_72 )
{
    return das_auto_cast<char *>::cast(das_invoke_function<char * const >::invoke<regex::Regex &,Sequence DAS_COMMENT((uint32_t)) &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::re_gen S<regex::Regex> Y<ReGenRandom>1<u>G*/ 0x4a287ee9ffbe5ccd)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0xcced403b1d8ad2c6>(__context__) /*re_number*/),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_279_72.rnd)));
}

inline char * positive_int_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_284_73 )
{
    return das_auto_cast<char *>::cast(das_invoke_function<char * const >::invoke<regex::Regex &,Sequence DAS_COMMENT((uint32_t)) &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::re_gen S<regex::Regex> Y<ReGenRandom>1<u>G*/ 0x4a287ee9ffbe5ccd)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0x24f6b93c51999c1e>(__context__) /*re_positive_int*/),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_284_73.rnd)));
}

inline char * any_int_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_289_74 )
{
    return das_auto_cast<char *>::cast(das_invoke_function<char * const >::invoke<regex::Regex &,Sequence DAS_COMMENT((uint32_t)) &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::re_gen S<regex::Regex> Y<ReGenRandom>1<u>G*/ 0x4a287ee9ffbe5ccd)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0x21aa63079ae9f66c>(__context__) /*re_int*/),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_289_74.rnd)));
}

inline char * any_hex_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_294_75 )
{
    return das_auto_cast<char *>::cast(das_invoke_function<char * const >::invoke<regex::Regex &,Sequence DAS_COMMENT((uint32_t)) &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::re_gen S<regex::Regex> Y<ReGenRandom>1<u>G*/ 0x4a287ee9ffbe5ccd)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0x51c1ac4928d6065d>(__context__) /*re_hex*/),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_294_75.rnd)));
}

inline char * any_float_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_299_76 )
{
    return das_auto_cast<char *>::cast(das_invoke_function<char * const >::invoke<regex::Regex &,Sequence DAS_COMMENT((uint32_t)) &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::re_gen S<regex::Regex> Y<ReGenRandom>1<u>G*/ 0x4a287ee9ffbe5ccd)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0xb47e9636ac90db6a>(__context__) /*re_float*/),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_299_76.rnd)));
}

inline char * any_uint_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_304_77 )
{
    return das_auto_cast<char *>::cast(das_invoke_function<char * const >::invoke<regex::Regex &,Sequence DAS_COMMENT((uint32_t)) &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::re_gen S<regex::Regex> Y<ReGenRandom>1<u>G*/ 0x4a287ee9ffbe5ccd)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0x4df21bb6fd7700f>(__context__) /*re_uint*/),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_304_77.rnd)));
}

inline char * month_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_309_78 )
{
    uint32_t __month_rename_at_311_79 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_309_78.rnd),__month_rename_at_311_79);
    das_copy(__month_rename_at_311_79,SimPolicy<uint32_t>::Mod(__month_rename_at_311_79,0xcu,*__context__,nullptr));
    return das_auto_cast<char *>::cast(das_global<TArray<faker::MonthNameAndDay>,0x1d0bed6094e46065>(__context__) /*g_months*/(__month_rename_at_311_79,__context__).name);
}

inline char * day_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_317_80 )
{
    uint32_t __day_rename_at_319_81 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_317_80.rnd),__day_rename_at_319_81);
    das_copy(__day_rename_at_319_81,SimPolicy<uint32_t>::Mod(__day_rename_at_319_81,0x7u,*__context__,nullptr));
    return das_auto_cast<char *>::cast(das_global<TDim<char *,7>,0x34cefaea736497f>(__context__) /*g_days*/(__day_rename_at_319_81,__context__));
}

inline bool is_leap_year_f4966f72627a00c6 ( Context * __context__, uint32_t __year_rename_at_325_82 )
{
    return das_auto_cast<bool>::cast(((SimPolicy<uint32_t>::Mod(__year_rename_at_325_82,0x4u,*__context__,nullptr)) == 0x0u) && (((SimPolicy<uint32_t>::Mod(__year_rename_at_325_82,0x64u,*__context__,nullptr)) != 0x0u) || ((SimPolicy<uint32_t>::Mod(__year_rename_at_325_82,0x190u,*__context__,nullptr)) == 0x0u)));
}

inline int32_t week_day_c239077242e7d543 ( Context * __context__, uint32_t __year_rename_at_330_83, uint32_t __month_rename_at_330_84, uint32_t __day_rename_at_330_85 )
{
    return das_auto_cast<int32_t>::cast(week_day_a63db001e98f1f1d(__context__,int32_t(__year_rename_at_330_83),int32_t(__month_rename_at_330_84),int32_t(__day_rename_at_330_85)));
}

inline int32_t week_day_a63db001e98f1f1d ( Context * __context__, int32_t __year_rename_at_335_86, int32_t __month_rename_at_335_87, int32_t __day_rename_at_335_88 )
{
    int32_t __afterFeb_rename_at_340_89 = 1;
    if ( __month_rename_at_335_87 > 2 )
    {
        das_copy(__afterFeb_rename_at_340_89,0);
    };
    int32_t __aux_rename_at_344_90 = ((int32_t)((__year_rename_at_335_86 - 1700) - __afterFeb_rename_at_340_89));
    int32_t __dayOfWeek_rename_at_346_91 = 5;
    __dayOfWeek_rename_at_346_91 += ((__aux_rename_at_344_90 + __afterFeb_rename_at_340_89) * 365);
    __dayOfWeek_rename_at_346_91 += (((SimPolicy<int32_t>::Div(__aux_rename_at_344_90,4,*__context__,nullptr)) - (SimPolicy<int32_t>::Div(__aux_rename_at_344_90,100,*__context__,nullptr))) + (SimPolicy<int32_t>::Div((__aux_rename_at_344_90 + 100),400,*__context__,nullptr)));
    __dayOfWeek_rename_at_346_91 += (das_global<TArray<int32_t>,0x6d18b96a1112d3df>(__context__) /*`static`at_line_337`offset*/(__month_rename_at_335_87,__context__) + (__day_rename_at_335_88 - 1));
    __dayOfWeek_rename_at_346_91 %= 7;
    return das_auto_cast<int32_t>::cast(__dayOfWeek_rename_at_346_91);
}

inline char * date_2c99640021fae61f ( Context * __context__, faker::Faker &  __faker_rename_at_357_92 )
{
    uint32_t __month_rename_at_359_93 = 0x0u;
    uint32_t __day_rename_at_360_94 = 0x0u;
    uint32_t __year_rename_at_361_95 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_357_92.rnd),__month_rename_at_359_93);
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_357_92.rnd),__year_rename_at_361_95);
    _FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__faker_rename_at_357_92.rnd),__day_rename_at_360_94);
    das_copy(__month_rename_at_359_93,SimPolicy<uint32_t>::Mod(__month_rename_at_359_93,0xcu,*__context__,nullptr));
    das_copy(__year_rename_at_361_95,__faker_rename_at_357_92.min_year + (SimPolicy<uint32_t>::Mod(__year_rename_at_361_95,__faker_rename_at_357_92.total_years,*__context__,nullptr)));
    uint32_t __max_days_rename_at_367_96 = uint32_t(das_global<TArray<faker::MonthNameAndDay>,0x1d0bed6094e46065>(__context__) /*g_months*/(__month_rename_at_359_93,__context__).days);
    if ( is_leap_year_f4966f72627a00c6(__context__,__year_rename_at_361_95) && (__month_rename_at_359_93 == 0x1u) )
    {
        das_copy(__max_days_rename_at_367_96,0x1du);
    };
    das_copy(__day_rename_at_360_94,(SimPolicy<uint32_t>::Mod(__day_rename_at_360_94,__max_days_rename_at_367_96,*__context__,nullptr)) + 0x1u);
    int32_t __dow_rename_at_372_97 = ((int32_t)week_day_c239077242e7d543(__context__,__year_rename_at_361_95,__month_rename_at_359_93,__day_rename_at_360_94));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<7>(__tinfo_0, cast<char *>::from(das_global<TDim<char *,7>,0x34cefaea736497f>(__context__) /*g_days*/(__dow_rename_at_372_97,__context__)), cast<char * const >::from(((char *) ", ")), cast<char *>::from(das_global<TArray<faker::MonthNameAndDay>,0x1d0bed6094e46065>(__context__) /*g_months*/(__month_rename_at_359_93,__context__).name), cast<char * const >::from(((char *) " ")), cast<int32_t>::from(int32_t(__day_rename_at_360_94)), cast<char * const >::from(((char *) ", ")), cast<int32_t>::from(int32_t(__year_rename_at_361_95)))));
}

inline faker::Faker Faker_b38307f5f81d41da ( Context * __context__ )
{
    return /* <- */ das_auto_cast_move<faker::Faker>::cast((([&]() -> faker::Faker {
        faker::Faker __mks_49;
        das_copy((__mks_49.min_year),(0x7bcu));
        das_copy((__mks_49.total_years),(0x2au));
        das_move((__mks_49.rnd),(das_invoke_function<Sequence DAS_COMMENT((uint32_t))>::invoke_cmres<int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@random::each_random_uint Ci*/ 0x47a0f7ad90ba262c)),13)));
        das_copy((__mks_49.max_long_string),(0x1000u));
        return __mks_49;
    })()));
}

inline TArray<faker::MonthNameAndDay> _FuncbuiltinTickto_array_moveTick3185538323411982277_a6478d4cc12bcff8 ( Context * __context__, TDim<faker::MonthNameAndDay,12> &  __a_rename_at_1375_98 )
{
    TArray<faker::MonthNameAndDay> __arr_rename_at_1377_99;das_zero(__arr_rename_at_1377_99);
    _FuncbuiltinTickresizeTick4811697762258667383_93e93a07b629c7a3(__context__,das_arg<TArray<faker::MonthNameAndDay>>::pass(__arr_rename_at_1377_99),12);
    das_copy(das_cast<TDim<faker::MonthNameAndDay,12>>::cast(das_ref(__context__,__arr_rename_at_1377_99(0,__context__))),__a_rename_at_1375_98);
    return /* <- */ das_auto_cast_move<TArray<faker::MonthNameAndDay>>::cast(__arr_rename_at_1377_99);
}

inline TArray<AutoTuple<range,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_d240a9f91d9ede74 ( Context * __context__, TDim<AutoTuple<range,char *>,1> &  __a_rename_at_1375_100 )
{
    TArray<AutoTuple<range,char *>> __arr_rename_at_1377_101;das_zero(__arr_rename_at_1377_101);
    _FuncbuiltinTickresizeTick4811697762258667383_de3bec42d99954f8(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(__arr_rename_at_1377_101),1);
    das_copy(das_cast<TDim<AutoTuple<range,char *>,1>>::cast(das_ref(__context__,__arr_rename_at_1377_101(0,__context__))),__a_rename_at_1375_100);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<range,char *>>>::cast(__arr_rename_at_1377_101);
}

inline TArray<AutoTuple<range,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_c7508367b622b66d ( Context * __context__, TDim<AutoTuple<range,char *>,2> &  __a_rename_at_1375_102 )
{
    TArray<AutoTuple<range,char *>> __arr_rename_at_1377_103;das_zero(__arr_rename_at_1377_103);
    _FuncbuiltinTickresizeTick4811697762258667383_de3bec42d99954f8(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(__arr_rename_at_1377_103),2);
    das_copy(das_cast<TDim<AutoTuple<range,char *>,2>>::cast(das_ref(__context__,__arr_rename_at_1377_103(0,__context__))),__a_rename_at_1375_102);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<range,char *>>>::cast(__arr_rename_at_1377_103);
}

inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_37174cace0912109 ( Context * __context__, TDim<regex::ReNode *,11> &  __a_rename_at_1375_104 )
{
    TArray<regex::ReNode *> __arr_rename_at_1377_105;das_zero(__arr_rename_at_1377_105);
    _FuncbuiltinTickresizeTick4811697762258667383_b92c6f816db4602(__context__,das_arg<TArray<regex::ReNode *>>::pass(__arr_rename_at_1377_105),11);
    das_copy(das_cast<TDim<regex::ReNode *,11>>::cast(das_ref(__context__,__arr_rename_at_1377_105(0,__context__))),__a_rename_at_1375_104);
    return /* <- */ das_auto_cast_move<TArray<regex::ReNode *>>::cast(__arr_rename_at_1377_105);
}

inline TArray<int32_t> _FuncbuiltinTickto_array_moveTick3185538323411982277_afe546e6423dd8f3 ( Context * __context__, TDim<int32_t,12> &  __a_rename_at_1375_106 )
{
    TArray<int32_t> __arr_rename_at_1377_107;das_zero(__arr_rename_at_1377_107);
    _FuncbuiltinTickresizeTick4811697762258667383_856be2a5daceccf2(__context__,das_arg<TArray<int32_t>>::pass(__arr_rename_at_1377_107),12);
    das_copy(das_cast<TDim<int32_t,12>>::cast(das_ref(__context__,__arr_rename_at_1377_107(0,__context__))),__a_rename_at_1375_106);
    return /* <- */ das_auto_cast_move<TArray<int32_t>>::cast(__arr_rename_at_1377_107);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x646d7d573347ed78] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_93e93a07b629c7a3>>();
    };
    aotLib[0x825fdfcfc4f067c7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_de3bec42d99954f8>>();
    };
    aotLib[0x2bdf7c27002e4671] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_b92c6f816db4602>>();
    };
    aotLib[0xa62b1345ebf6da7a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_856be2a5daceccf2>>();
    };
    aotLib[0x65b2f3a6a3cb9998] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_bd2ed0e765c61464>>();
    };
    aotLib[0xba9b0c7a659af781] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_7bf97eaaff0543bb>>();
    };
    aotLib[0x8aa49cb6bce667a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_f3bb1928c94b9b52>>();
    };
    aotLib[0x1eecb2df6c7e187b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_ca7d613c38c76fb8>>();
    };
    aotLib[0x5ea9b7e22430d563] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_67d7519b16eea304>>();
    };
    aotLib[0x8935a9ce3f19bd2d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_8dc91cad3b75b3c8>>();
    };
    aotLib[0xa75945036a6700d5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_ba932d5d139ed9c0>>();
    };
    aotLib[0x5790e8dbf9ae0c92] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickreserveTick3994685146752941225_71af0b01dbdb30b2>>();
    };
    aotLib[0xfa0338f35032e36] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_d8f0a51920afdbe4>>();
    };
    aotLib[0x907d75956bf42671] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Faker_120e1a6bb086dd13>>();
    };
    aotLib[0xc28eb0e667218f16] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&long_string_2c99640021fae61f>>();
    };
    aotLib[0xd5a3c82185382c63] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&any_string_2c99640021fae61f>>();
    };
    aotLib[0xd5a3c82185382c63] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&any_file_name_2c99640021fae61f>>();
    };
    aotLib[0x1fecb06e61474984] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_int_db927c5b51337757>>();
    };
    aotLib[0xff51a5803e134b77] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_uint_1fc7e41ed0d6ffdd>>();
    };
    aotLib[0xeb07b046f9713cc4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_int8_9e48de99fcf0b4bb>>();
    };
    aotLib[0x1665336f2c18c6bc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_uint8_eb9ad5532c2efba>>();
    };
    aotLib[0x23d788909e142cb6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_int16_65cfde74f34ebeec>>();
    };
    aotLib[0xa85585b281ac3bab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_uint16_27625a3cd1234fe8>>();
    };
    aotLib[0x8304a570431dcacc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_float_3b1c10c7ec3bd27d>>();
    };
    aotLib[0x6d076afc335c64da] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_int2_163549ee4deaeda5>>();
    };
    aotLib[0x992a1bcfba552a02] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_range_82207fb65a0097a8>>();
    };
    aotLib[0x53bc82a82a8d300f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_range64_76c394d5233908be>>();
    };
    aotLib[0xd53937eb3c098229] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_int3_4e81314eab7f0e58>>();
    };
    aotLib[0x4cef996c247843d5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_int4_24c671e0ae1fb943>>();
    };
    aotLib[0xa80834e4fe9168cb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_uint2_74dfcbc709ea1d13>>();
    };
    aotLib[0x194241c07e21f800] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_urange_5f399937e7e7ad83>>();
    };
    aotLib[0xc096298706d67417] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_urange64_64b82994fec321bd>>();
    };
    aotLib[0x7a0599b944e2ffd4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_uint3_82c843a2e2dcd1ad>>();
    };
    aotLib[0xa4902347a0ace38d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_uint4_c168676f0192f143>>();
    };
    aotLib[0xed253eeb8bfe1f1b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_float2_de4a2c3aa9fc390>>();
    };
    aotLib[0x21e05fa4f695ffff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_float3_176dbf53ad107c9a>>();
    };
    aotLib[0x3629c0bf136f46f3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_float4_d05db2140ce31987>>();
    };
    aotLib[0x723288875fbe40aa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&random_float3x3_e049ec11115607a5>>();
    };
    aotLib[0xec0f34ec6b94c971] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&random_float3x4_ae3c09f14778b039>>();
    };
    aotLib[0xddd3403a144ee9cc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&random_float4x4_4db49814bded2b7d>>();
    };
    aotLib[0xd9335374cc035d76] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_int64_20af9383ba5df64b>>();
    };
    aotLib[0xdaf277c60c191c71] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_uint64_9d78736b94d2bb65>>();
    };
    aotLib[0x70fef258fd1fa0ab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&random_double_ee86008619feb237>>();
    };
    aotLib[0xbc2dc3c0e6c9e511] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&any_set_d6fb89a5c88f1b2c>>();
    };
    aotLib[0x1c55085d684c6aa9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&any_char_db927c5b51337757>>();
    };
    aotLib[0xe23e5aebbbe82fbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&number_2c99640021fae61f>>();
    };
    aotLib[0xd75973db0a13d0e3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&positive_int_2c99640021fae61f>>();
    };
    aotLib[0xd5a3c82185382c63] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&any_int_2c99640021fae61f>>();
    };
    aotLib[0xd5a3c82185382c63] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&any_hex_2c99640021fae61f>>();
    };
    aotLib[0x46d435ceec5640e8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&any_float_2c99640021fae61f>>();
    };
    aotLib[0xd5a3c82185382c63] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&any_uint_2c99640021fae61f>>();
    };
    aotLib[0xd41de5e0b978da96] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&month_2c99640021fae61f>>();
    };
    aotLib[0xf0718f45a726af31] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&day_2c99640021fae61f>>();
    };
    aotLib[0x117bf4c6882ef31d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_leap_year_f4966f72627a00c6>>();
    };
    aotLib[0xf66422ecc2810fe7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&week_day_c239077242e7d543>>();
    };
    aotLib[0xb4f52b9371db8624] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&week_day_a63db001e98f1f1d>>();
    };
    aotLib[0x238f662913aa7458] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&date_2c99640021fae61f>>();
    };
    aotLib[0xe651d45ff7d07392] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Faker_b38307f5f81d41da>>();
    };
    aotLib[0xe5565577ae6884e8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_a6478d4cc12bcff8>>();
    };
    aotLib[0x26553d33dd410b53] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_d240a9f91d9ede74>>();
    };
    aotLib[0xf6fbe28ab806781d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_c7508367b622b66d>>();
    };
    aotLib[0x125e5cb2178042f1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_37174cace0912109>>();
    };
    aotLib[0x52ff78c5aeb2bc90] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_afe546e6423dd8f3>>();
    };
    // [[ init script ]]
    aotLib[0xb6f4fd3e9e5004eb] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_341163587376737593
AotListBase impl_aot_faker(_anon_341163587376737593::registerAotFunctions);
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
