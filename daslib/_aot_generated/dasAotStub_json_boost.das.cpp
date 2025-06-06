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
 // require json
 // require ast_boost
 // require contracts
 // require templates
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require apply
 // require json_boost

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
namespace _anon_4702729321666677987 {

namespace json_boost { struct BetterJsonMacro; };
namespace json_boost { struct JsonReader; };
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
namespace json { struct JsonValue; };
namespace json { struct TokenAt; };
namespace json { struct _lambda_json_99_1; };
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
namespace templates { struct DecltypeMacro; };
namespace templates { struct DecltypeNoRefMacro; };
namespace templates { struct TemplateMacro; };
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
namespace macro_boost { struct MacroVerifyMacro; };
namespace macro_boost { struct CaptureBlock; };
namespace macro_boost { struct CapturedVariable; };
namespace macro_boost { struct ColletFinally; };
namespace macro_boost { struct ColletLabels; };
namespace macro_boost { struct ReturnSkipLockcheck; };
namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
namespace apply { struct ApplyMacro; };
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
#if 0 // external enum
namespace rtti {

enum class Type : int32_t {
    none = int32_t(INT64_C(0)),
    autoinfer = int32_t(INT64_C(1)),
    alias = int32_t(INT64_C(2)),
    option = int32_t(INT64_C(3)),
    typeDecl = int32_t(INT64_C(4)),
    typeMacro = int32_t(INT64_C(5)),
    fakeContext = int32_t(INT64_C(6)),
    fakeLineInfo = int32_t(INT64_C(7)),
    anyArgument = int32_t(INT64_C(8)),
    tVoid = int32_t(INT64_C(9)),
    tBool = int32_t(INT64_C(10)),
    tInt8 = int32_t(INT64_C(11)),
    tUInt8 = int32_t(INT64_C(12)),
    tInt16 = int32_t(INT64_C(13)),
    tUInt16 = int32_t(INT64_C(14)),
    tInt64 = int32_t(INT64_C(15)),
    tUInt64 = int32_t(INT64_C(16)),
    tInt = int32_t(INT64_C(17)),
    tInt2 = int32_t(INT64_C(18)),
    tInt3 = int32_t(INT64_C(19)),
    tInt4 = int32_t(INT64_C(20)),
    tUInt = int32_t(INT64_C(21)),
    tUInt2 = int32_t(INT64_C(22)),
    tUInt3 = int32_t(INT64_C(23)),
    tUInt4 = int32_t(INT64_C(24)),
    tFloat = int32_t(INT64_C(25)),
    tFloat2 = int32_t(INT64_C(26)),
    tFloat3 = int32_t(INT64_C(27)),
    tFloat4 = int32_t(INT64_C(28)),
    tDouble = int32_t(INT64_C(29)),
    tRange = int32_t(INT64_C(30)),
    tURange = int32_t(INT64_C(31)),
    tRange64 = int32_t(INT64_C(32)),
    tURange64 = int32_t(INT64_C(33)),
    tString = int32_t(INT64_C(34)),
    tStructure = int32_t(INT64_C(35)),
    tHandle = int32_t(INT64_C(36)),
    tEnumeration = int32_t(INT64_C(37)),
    tEnumeration8 = int32_t(INT64_C(38)),
    tEnumeration16 = int32_t(INT64_C(39)),
    tEnumeration64 = int32_t(INT64_C(40)),
    tBitfield = int32_t(INT64_C(41)),
    tPointer = int32_t(INT64_C(42)),
    tFunction = int32_t(INT64_C(43)),
    tLambda = int32_t(INT64_C(44)),
    tIterator = int32_t(INT64_C(45)),
    tArray = int32_t(INT64_C(46)),
    tTable = int32_t(INT64_C(47)),
    tBlock = int32_t(INT64_C(48)),
    tTuple = int32_t(INT64_C(49)),
    tVariant = int32_t(INT64_C(50)),
};
}
#endif // external enum
// unused enumeration ConversionResult
// unused enumeration CaptureMode
// unused enumeration SideEffects
// unused structure AstFunctionAnnotation
// unused structure AstBlockAnnotation
// unused structure AstStructureAnnotation
// unused structure AstPassMacro
namespace ast {

struct AstVariantMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
}
// unused structure AstForLoopMacro
// unused structure AstCaptureMacro
// unused structure AstTypeMacro
// unused structure AstSimulateMacro
namespace ast {

struct AstReaderMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
}
// unused structure AstCommentReader
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
// unused structure AstVisitor
namespace json {

struct JsonValue {
    AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> value;
};
}
// unused structure TokenAt
// unused structure _lambda_json_99_1
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
// unused structure DecltypeMacro
// unused structure DecltypeNoRefMacro
// unused structure TemplateMacro
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
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
// unused structure DeferMacro
// unused structure DeferDeleteMacro
// unused structure ApplyMacro
namespace json_boost {

struct BetterJsonMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
}
namespace json_boost {

struct JsonReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
}
extern TypeInfo __type_info__8d8d8008262e16ec;
extern TypeInfo __type_info__b68d800849332aec;

TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int64_t>::size, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__b68d800849332aec = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<uint64_t>::size, UINT64_C(0xb68d800849332aec) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__8d8d8008262e16ec };
TypeInfo * __tinfo_1[1] = { &__type_info__b68d800849332aec };

inline void _FuncbuiltinTickpushTick10769833213962245646_bea46e5118b8a97f ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_181_0, ast::AstVariantMacro * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cdf93cb6c5a4a259 ( Context * __context__, json_boost::BetterJsonMacro const  & __cl_rename_at_116_2 );
inline void _FuncbuiltinTickpushTick10769833213962245646_cc8787f882c60e16 ( Context * __context__, TArray<ast::AstReaderMacro *> & __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c642f300851ed2b8 ( Context * __context__, json_boost::JsonReader const  & __cl_rename_at_116_5 );
inline json::JsonValue * _FuncQmarkSqblSqbr_10d4ad37f3ca7d86 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_21_10, char * const  __key_rename_at_21_11 );
inline json::JsonValue * _FuncQmarkSqblSqbr_681d089550d4cb6c ( Context * __context__, json::JsonValue * __a_rename_at_26_12, char * const  __key_rename_at_26_13 );
inline json::JsonValue * _FuncQmarkDot_10d4ad37f3ca7d86 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_31_14, char * const  __key_rename_at_31_15 );
inline json::JsonValue * _FuncQmarkDot_681d089550d4cb6c ( Context * __context__, json::JsonValue * __a_rename_at_36_16, char * const  __key_rename_at_36_17 );
inline json::JsonValue * _FuncQmarkSqblSqbr_8951bd16cedf5eee ( Context * __context__, json::JsonValue const  * const  __a_rename_at_41_18, int32_t __idx_rename_at_41_19 );
inline json::JsonValue * _FuncQmarkSqblSqbr_7b9d121489614fc0 ( Context * __context__, json::JsonValue * __a_rename_at_46_20, int32_t __idx_rename_at_46_21 );
inline double _FuncQmarkQmark_9d5aea8a82e6429b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_51_22, double __val_rename_at_51_23 );
inline float _FuncQmarkQmark_acbca43b013428d3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_56_24, float __val_rename_at_56_25 );
inline int8_t _FuncQmarkQmark_229e01e7629d1f3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_61_26, int8_t __val_rename_at_61_27 );
inline int16_t _FuncQmarkQmark_3e36539d97df378b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_66_28, int16_t __val_rename_at_66_29 );
inline int32_t _FuncQmarkQmark_78038b94941077d3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_30, int32_t __val_rename_at_71_31 );
inline int64_t _FuncQmarkQmark_b2fb40911b8cb673 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_76_32, int64_t __val_rename_at_76_33 );
inline uint8_t _FuncQmarkQmark_ac5e5f9e70ed46ff ( Context * __context__, json::JsonValue const  * const  __a_rename_at_81_34, uint8_t __val_rename_at_81_35 );
inline uint16_t _FuncQmarkQmark_d1d73d626d306e6f ( Context * __context__, json::JsonValue const  * const  __a_rename_at_86_36, uint16_t __val_rename_at_86_37 );
inline uint32_t _FuncQmarkQmark_5e6d5729b29ebc7b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_91_38, uint32_t __val_rename_at_91_39 );
inline uint64_t _FuncQmarkQmark_b564f0fd640f01f3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_96_40, uint64_t __val_rename_at_96_41 );
inline bool _FuncQmarkQmark_3297589a3165767 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_101_42, bool __val_rename_at_101_43 );
inline char * _FuncQmarkQmark_3881c45eff047673 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_106_44, char * const  __val_rename_at_106_45 );
inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  * _FuncQmarkDotTickvalue_8e3bcdc230113e04 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_111_46 );
inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> * _FuncQmarkDotTickvalue_e2e263d355de35e4 ( Context * __context__, json::JsonValue * __a_rename_at_116_47 );
inline json::JsonValue * JV_b5baa59b7d2537ff ( Context * __context__, float __v_rename_at_469_48 );
inline json::JsonValue * JV_2a8221214f29ec9d ( Context * __context__, int32_t __v_rename_at_473_49 );
inline json::JsonValue * JV_e8ad4eaf5470db47 ( Context * __context__, Bitfield __v_rename_at_477_50 );
inline json::JsonValue * JV_88e103e0204c6b1d ( Context * __context__, int8_t __val_rename_at_481_51 );
inline json::JsonValue * JV_880b4ce3378b9c2c ( Context * __context__, uint8_t __val_rename_at_485_52 );
inline json::JsonValue * JV_4168b1098b7251fa ( Context * __context__, int16_t __val_rename_at_489_53 );
inline json::JsonValue * JV_1b506ad2a1609b1e ( Context * __context__, uint16_t __val_rename_at_493_54 );
inline json::JsonValue * JV_331a3fac10a44e07 ( Context * __context__, uint32_t __val_rename_at_497_55 );
inline json::JsonValue * JV_12ff9893b060155d ( Context * __context__, int64_t __val_rename_at_501_56 );
inline json::JsonValue * JV_70fe5e3179f6d257 ( Context * __context__, uint64_t __val_rename_at_509_57 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_bea46e5118b8a97f ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_181_0, ast::AstVariantMacro * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cdf93cb6c5a4a259 ( Context * __context__, json_boost::BetterJsonMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_cc8787f882c60e16 ( Context * __context__, TArray<ast::AstReaderMacro *> &  __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 )
{
    das_copy(__Arr_rename_at_181_3(builtin_array_push_back(das_arg<TArray<ast::AstReaderMacro *>>::pass(__Arr_rename_at_181_3),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_4);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c642f300851ed2b8 ( Context * __context__, json_boost::JsonReader const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_10d4ad37f3ca7d86 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_21_10, char * const  __key_rename_at_21_11 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_21_10 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_21_10->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_21_10->value,__context__)),__key_rename_at_21_11,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_681d089550d4cb6c ( Context * __context__, json::JsonValue * __a_rename_at_26_12, char * const  __key_rename_at_26_13 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_26_12 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_26_12->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_26_12->value,__context__)),__key_rename_at_26_13,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkDot_10d4ad37f3ca7d86 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_31_14, char * const  __key_rename_at_31_15 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_31_14 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_31_14->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_31_14->value,__context__)),__key_rename_at_31_15,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkDot_681d089550d4cb6c ( Context * __context__, json::JsonValue * __a_rename_at_36_16, char * const  __key_rename_at_36_17 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_36_16 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_36_16->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_36_16->value,__context__)),__key_rename_at_36_17,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_8951bd16cedf5eee ( Context * __context__, json::JsonValue const  * const  __a_rename_at_41_18, int32_t __idx_rename_at_41_19 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_41_18 != nullptr) && das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_41_18->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_41_18->value,__context__)),__idx_rename_at_41_19,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_7b9d121489614fc0 ( Context * __context__, json::JsonValue * __a_rename_at_46_20, int32_t __idx_rename_at_46_21 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_46_20 != nullptr) && das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_46_20->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_46_20->value,__context__)),__idx_rename_at_46_21,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline double _FuncQmarkQmark_9d5aea8a82e6429b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_51_22, double __val_rename_at_51_23 )
{
    return das_auto_cast<double>::cast(((__a_rename_at_51_22 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_51_22->value)) ? das_auto_cast<double>::cast(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_51_22->value,__context__)) : das_auto_cast<double>::cast(__val_rename_at_51_23));
}

inline float _FuncQmarkQmark_acbca43b013428d3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_56_24, float __val_rename_at_56_25 )
{
    return das_auto_cast<float>::cast(((__a_rename_at_56_24 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_56_24->value)) ? das_auto_cast<float>::cast(float(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_56_24->value,__context__))) : das_auto_cast<float>::cast(__val_rename_at_56_25));
}

inline int8_t _FuncQmarkQmark_229e01e7629d1f3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_61_26, int8_t __val_rename_at_61_27 )
{
    return das_auto_cast<int8_t>::cast(((__a_rename_at_61_26 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_61_26->value)) ? das_auto_cast<int8_t>::cast(int8_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_61_26->value,__context__))) : das_auto_cast<int8_t>::cast(__val_rename_at_61_27));
}

inline int16_t _FuncQmarkQmark_3e36539d97df378b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_66_28, int16_t __val_rename_at_66_29 )
{
    return das_auto_cast<int16_t>::cast(((__a_rename_at_66_28 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_66_28->value)) ? das_auto_cast<int16_t>::cast(int16_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_66_28->value,__context__))) : das_auto_cast<int16_t>::cast(__val_rename_at_66_29));
}

inline int32_t _FuncQmarkQmark_78038b94941077d3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_30, int32_t __val_rename_at_71_31 )
{
    return das_auto_cast<int32_t>::cast(((__a_rename_at_71_30 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_71_30->value)) ? das_auto_cast<int32_t>::cast(int32_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_71_30->value,__context__))) : das_auto_cast<int32_t>::cast(__val_rename_at_71_31));
}

inline int64_t _FuncQmarkQmark_b2fb40911b8cb673 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_76_32, int64_t __val_rename_at_76_33 )
{
    return das_auto_cast<int64_t>::cast(((__a_rename_at_76_32 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_76_32->value)) ? das_auto_cast<int64_t>::cast(int64_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_76_32->value,__context__))) : das_auto_cast<int64_t>::cast(__val_rename_at_76_33));
}

inline uint8_t _FuncQmarkQmark_ac5e5f9e70ed46ff ( Context * __context__, json::JsonValue const  * const  __a_rename_at_81_34, uint8_t __val_rename_at_81_35 )
{
    return das_auto_cast<uint8_t>::cast(((__a_rename_at_81_34 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_81_34->value)) ? das_auto_cast<uint8_t>::cast(uint8_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_81_34->value,__context__))) : das_auto_cast<uint8_t>::cast(__val_rename_at_81_35));
}

inline uint16_t _FuncQmarkQmark_d1d73d626d306e6f ( Context * __context__, json::JsonValue const  * const  __a_rename_at_86_36, uint16_t __val_rename_at_86_37 )
{
    return das_auto_cast<uint16_t>::cast(((__a_rename_at_86_36 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_86_36->value)) ? das_auto_cast<uint16_t>::cast(uint16_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_86_36->value,__context__))) : das_auto_cast<uint16_t>::cast(__val_rename_at_86_37));
}

inline uint32_t _FuncQmarkQmark_5e6d5729b29ebc7b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_91_38, uint32_t __val_rename_at_91_39 )
{
    return das_auto_cast<uint32_t>::cast(((__a_rename_at_91_38 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_91_38->value)) ? das_auto_cast<uint32_t>::cast(uint32_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_91_38->value,__context__))) : das_auto_cast<uint32_t>::cast(__val_rename_at_91_39));
}

inline uint64_t _FuncQmarkQmark_b564f0fd640f01f3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_96_40, uint64_t __val_rename_at_96_41 )
{
    return das_auto_cast<uint64_t>::cast(((__a_rename_at_96_40 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_96_40->value)) ? das_auto_cast<uint64_t>::cast(uint64_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_96_40->value,__context__))) : das_auto_cast<uint64_t>::cast(__val_rename_at_96_41));
}

inline bool _FuncQmarkQmark_3297589a3165767 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_101_42, bool __val_rename_at_101_43 )
{
    return das_auto_cast<bool>::cast(((__a_rename_at_101_42 != nullptr) && das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_101_42->value)) ? das_auto_cast<bool>::cast(das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_101_42->value,__context__)) : das_auto_cast<bool>::cast(__val_rename_at_101_43));
}

inline char * _FuncQmarkQmark_3881c45eff047673 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_106_44, char * const  __val_rename_at_106_45 )
{
    return das_auto_cast<char *>::cast(((__a_rename_at_106_44 != nullptr) && das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_106_44->value)) ? das_auto_cast<char * const >::cast(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_106_44->value,__context__)) : das_auto_cast<char * const >::cast(__val_rename_at_106_45));
}

inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  * _FuncQmarkDotTickvalue_8e3bcdc230113e04 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_111_46 )
{
    return das_auto_cast<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  *>::cast(das_safe_navigation<json::JsonValue const ,AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_111_46));
}

inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> * _FuncQmarkDotTickvalue_e2e263d355de35e4 ( Context * __context__, json::JsonValue * __a_rename_at_116_47 )
{
    return das_auto_cast<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> *>::cast(das_safe_navigation<json::JsonValue,AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_116_47));
}

inline json::JsonValue * JV_b5baa59b7d2537ff ( Context * __context__, float __v_rename_at_469_48 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_470;
        das_zero(__mks_470);
        das_move((__mks_470.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_470;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_470) = double(__v_rename_at_469_48);
            return __mkv_470;
        })())));
        return __mks_470;
    })())));
}

inline json::JsonValue * JV_2a8221214f29ec9d ( Context * __context__, int32_t __v_rename_at_473_49 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_474;
        das_zero(__mks_474);
        das_move((__mks_474.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_474;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_474) = double(__v_rename_at_473_49);
            return __mkv_474;
        })())));
        return __mks_474;
    })())));
}

inline json::JsonValue * JV_e8ad4eaf5470db47 ( Context * __context__, Bitfield __v_rename_at_477_50 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_478;
        das_zero(__mks_478);
        das_move((__mks_478.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_478;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_478) = double(__v_rename_at_477_50);
            return __mkv_478;
        })())));
        return __mks_478;
    })())));
}

inline json::JsonValue * JV_88e103e0204c6b1d ( Context * __context__, int8_t __val_rename_at_481_51 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_482;
        das_zero(__mks_482);
        das_move((__mks_482.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_482;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_482) = double(__val_rename_at_481_51);
            return __mkv_482;
        })())));
        return __mks_482;
    })())));
}

inline json::JsonValue * JV_880b4ce3378b9c2c ( Context * __context__, uint8_t __val_rename_at_485_52 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_486;
        das_zero(__mks_486);
        das_move((__mks_486.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_486;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_486) = double(__val_rename_at_485_52);
            return __mkv_486;
        })())));
        return __mks_486;
    })())));
}

inline json::JsonValue * JV_4168b1098b7251fa ( Context * __context__, int16_t __val_rename_at_489_53 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_490;
        das_zero(__mks_490);
        das_move((__mks_490.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_490;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_490) = double(__val_rename_at_489_53);
            return __mkv_490;
        })())));
        return __mks_490;
    })())));
}

inline json::JsonValue * JV_1b506ad2a1609b1e ( Context * __context__, uint16_t __val_rename_at_493_54 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_494;
        das_zero(__mks_494);
        das_move((__mks_494.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_494;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_494) = double(__val_rename_at_493_54);
            return __mkv_494;
        })())));
        return __mks_494;
    })())));
}

inline json::JsonValue * JV_331a3fac10a44e07 ( Context * __context__, uint32_t __val_rename_at_497_55 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_498;
        das_zero(__mks_498);
        das_move((__mks_498.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_498;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_498) = double(__val_rename_at_497_55);
            return __mkv_498;
        })())));
        return __mks_498;
    })())));
}

inline json::JsonValue * JV_12ff9893b060155d ( Context * __context__, int64_t __val_rename_at_501_56 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__val_rename_at_501_56 < INT64_C(-2147483648)) || (__val_rename_at_501_56 > INT64_C(2147483647))) ? das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_503;
        das_zero(__mks_503);
        das_move((__mks_503.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_503;
            das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_503) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_0, cast<int64_t>::from(__val_rename_at_501_56)));
            return __mkv_503;
        })())));
        return __mks_503;
    })()))) : das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_505;
        das_zero(__mks_505);
        das_move((__mks_505.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_505;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_505) = double(__val_rename_at_501_56);
            return __mkv_505;
        })())));
        return __mks_505;
    })()))));
}

inline json::JsonValue * JV_70fe5e3179f6d257 ( Context * __context__, uint64_t __val_rename_at_509_57 )
{
    return das_auto_cast<json::JsonValue *>::cast((__val_rename_at_509_57 > UINT64_C(0xffffffff)) ? das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_511;
        das_zero(__mks_511);
        das_move((__mks_511.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_511;
            das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_511) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_1, cast<uint64_t>::from(__val_rename_at_509_57)));
            return __mkv_511;
        })())));
        return __mks_511;
    })()))) : das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_513;
        das_zero(__mks_513);
        das_move((__mks_513.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_513;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_513) = double(__val_rename_at_509_57);
            return __mkv_513;
        })())));
        return __mks_513;
    })()))));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xec499a6d860ce733] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_bea46e5118b8a97f>>();
    };
    aotLib[0xd29d9a07521b4e1c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_cdf93cb6c5a4a259>>();
    };
    aotLib[0x572edd4206198143] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_cc8787f882c60e16>>();
    };
    aotLib[0xe8048df069a1cbfc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_c642f300851ed2b8>>();
    };
    aotLib[0x470e508c4800a7d9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkSqblSqbr_10d4ad37f3ca7d86>>();
    };
    aotLib[0x6f71d9a59c391755] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkSqblSqbr_681d089550d4cb6c>>();
    };
    aotLib[0x470e508c4800a7d9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkDot_10d4ad37f3ca7d86>>();
    };
    aotLib[0x6f71d9a59c391755] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkDot_681d089550d4cb6c>>();
    };
    aotLib[0x95cfa38cdc8c5b74] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkSqblSqbr_8951bd16cedf5eee>>();
    };
    aotLib[0x2260b759d8ba453f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkSqblSqbr_7b9d121489614fc0>>();
    };
    aotLib[0x19732d16a8a67f33] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_9d5aea8a82e6429b>>();
    };
    aotLib[0xad7d1e7151df79ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_acbca43b013428d3>>();
    };
    aotLib[0x8c96a161757c9895] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_229e01e7629d1f3>>();
    };
    aotLib[0x214c47a103579e9d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_3e36539d97df378b>>();
    };
    aotLib[0x5f0c3450d868628d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_78038b94941077d3>>();
    };
    aotLib[0xae20b5fdea0978df] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_b2fb40911b8cb673>>();
    };
    aotLib[0xa04eca9a60464269] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_ac5e5f9e70ed46ff>>();
    };
    aotLib[0x73664d6b23fd46d3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_d1d73d626d306e6f>>();
    };
    aotLib[0x5d75c9993436ddbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_5e6d5729b29ebc7b>>();
    };
    aotLib[0xe21271d209a3bf09] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_b564f0fd640f01f3>>();
    };
    aotLib[0x91e26d0dcef19451] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_3297589a3165767>>();
    };
    aotLib[0xa3757600f88c0e66] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_3881c45eff047673>>();
    };
    aotLib[0x5b4b046b6a0c8392] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkDotTickvalue_8e3bcdc230113e04>>();
    };
    aotLib[0x372b6c24e6e9b110] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkDotTickvalue_e2e263d355de35e4>>();
    };
    aotLib[0xb5e0b334431b136] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_b5baa59b7d2537ff>>();
    };
    aotLib[0x8baa839f0addc5c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_2a8221214f29ec9d>>();
    };
    aotLib[0xcf7fc497b7842b20] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_e8ad4eaf5470db47>>();
    };
    aotLib[0xe561ddbb347bc193] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_88e103e0204c6b1d>>();
    };
    aotLib[0x80433c836551e330] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_880b4ce3378b9c2c>>();
    };
    aotLib[0xcc82c4120263721f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_4168b1098b7251fa>>();
    };
    aotLib[0x90d88c3716ccc1ef] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_1b506ad2a1609b1e>>();
    };
    aotLib[0x2a40bab7c367938e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_331a3fac10a44e07>>();
    };
    aotLib[0x6dc3be7cc5de6be9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_12ff9893b060155d>>();
    };
    aotLib[0x2c4a88ea3bb93971] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_70fe5e3179f6d257>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_4702729321666677987
AotListBase impl_aot_json_boost(_anon_4702729321666677987::registerAotFunctions);
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
