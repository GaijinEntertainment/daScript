#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/runtime_profile.h"
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
namespace _anon_14538537767599647147 {

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

inline void _FuncbuiltinTickpushTick10769833213962245646_42a1594b8ce5f90a ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_181_0, ast::AstVariantMacro * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a504fdd4d2585454 ( Context * __context__, json_boost::BetterJsonMacro const  & __cl_rename_at_116_2 );
inline void _FuncbuiltinTickpushTick10769833213962245646_d013d9b30ff96e9d ( Context * __context__, TArray<ast::AstReaderMacro *> & __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_63e6deea4cedeb9e ( Context * __context__, json_boost::JsonReader const  & __cl_rename_at_116_5 );
inline json::JsonValue * _FuncQmarkSqblSqbr_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_21_10, char * const  __key_rename_at_21_11 );
inline json::JsonValue * _FuncQmarkSqblSqbr_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_26_12, char * const  __key_rename_at_26_13 );
inline json::JsonValue * _FuncQmarkDot_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_31_14, char * const  __key_rename_at_31_15 );
inline json::JsonValue * _FuncQmarkDot_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_36_16, char * const  __key_rename_at_36_17 );
inline json::JsonValue * _FuncQmarkSqblSqbr_ba0b0aab6632dd31 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_41_18, int32_t __idx_rename_at_41_19 );
inline json::JsonValue * _FuncQmarkSqblSqbr_d744859b79ee77ab ( Context * __context__, json::JsonValue * __a_rename_at_46_20, int32_t __idx_rename_at_46_21 );
inline double _FuncQmarkQmark_11719d2be16bd01 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_51_22, double __val_rename_at_51_23 );
inline float _FuncQmarkQmark_233f48b50ce5f404 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_56_24, float __val_rename_at_56_25 );
inline int8_t _FuncQmarkQmark_1a8257b5434aa8b3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_61_26, int8_t __val_rename_at_61_27 );
inline int16_t _FuncQmarkQmark_f62e6510f721332c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_66_28, int16_t __val_rename_at_66_29 );
inline int32_t _FuncQmarkQmark_6a40c116b1d48e91 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_30, int32_t __val_rename_at_71_31 );
inline int64_t _FuncQmarkQmark_e763b5dc20b3213 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_76_32, int64_t __val_rename_at_76_33 );
inline uint8_t _FuncQmarkQmark_e5a6fe7b7f0aecc6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_81_34, uint8_t __val_rename_at_81_35 );
inline uint16_t _FuncQmarkQmark_b339f9b2027db27b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_86_36, uint16_t __val_rename_at_86_37 );
inline uint32_t _FuncQmarkQmark_bab9e0c43667d51a ( Context * __context__, json::JsonValue const  * const  __a_rename_at_91_38, uint32_t __val_rename_at_91_39 );
inline uint64_t _FuncQmarkQmark_56f43cacc498853c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_96_40, uint64_t __val_rename_at_96_41 );
inline bool _FuncQmarkQmark_4fa1031aa0e66319 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_101_42, bool __val_rename_at_101_43 );
inline char * _FuncQmarkQmark_5415a6b0ff5b4eb5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_106_44, char * const  __val_rename_at_106_45 );
inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  * _FuncQmarkDotTickvalue_7c5354fba05537e2 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_111_46 );
inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> * _FuncQmarkDotTickvalue_2bdf5bca6f9d0002 ( Context * __context__, json::JsonValue * __a_rename_at_116_47 );
inline json::JsonValue * JV_de08d1a11663707 ( Context * __context__, float __v_rename_at_470_48 );
inline json::JsonValue * JV_2fa74720cd155930 ( Context * __context__, int32_t __v_rename_at_474_49 );
inline json::JsonValue * JV_549d699b656608a2 ( Context * __context__, Bitfield __v_rename_at_478_50 );
inline json::JsonValue * JV_840ec78279ac852d ( Context * __context__, int8_t __val_rename_at_482_51 );
inline json::JsonValue * JV_526f504129917241 ( Context * __context__, uint8_t __val_rename_at_486_52 );
inline json::JsonValue * JV_17760da7cd1ea0ee ( Context * __context__, int16_t __val_rename_at_490_53 );
inline json::JsonValue * JV_30573b250fcf268b ( Context * __context__, uint16_t __val_rename_at_494_54 );
inline json::JsonValue * JV_c85629fe70fc95b5 ( Context * __context__, uint32_t __val_rename_at_498_55 );
inline json::JsonValue * JV_b0de7f0f7332ba2d ( Context * __context__, int64_t __val_rename_at_502_56 );
inline json::JsonValue * JV_8425ea1d753a21fb ( Context * __context__, uint64_t __val_rename_at_510_57 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_42a1594b8ce5f90a ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_181_0, ast::AstVariantMacro * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a504fdd4d2585454 ( Context * __context__, json_boost::BetterJsonMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_d013d9b30ff96e9d ( Context * __context__, TArray<ast::AstReaderMacro *> &  __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 )
{
    das_copy(__Arr_rename_at_181_3(builtin_array_push_back(das_arg<TArray<ast::AstReaderMacro *>>::pass(__Arr_rename_at_181_3),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_4);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_63e6deea4cedeb9e ( Context * __context__, json_boost::JsonReader const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_21_10, char * const  __key_rename_at_21_11 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_21_10 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_21_10->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_21_10->value,__context__)),__key_rename_at_21_11,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_26_12, char * const  __key_rename_at_26_13 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_26_12 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_26_12->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_26_12->value,__context__)),__key_rename_at_26_13,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkDot_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_31_14, char * const  __key_rename_at_31_15 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_31_14 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_31_14->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_31_14->value,__context__)),__key_rename_at_31_15,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkDot_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_36_16, char * const  __key_rename_at_36_17 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_36_16 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_36_16->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_36_16->value,__context__)),__key_rename_at_36_17,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_ba0b0aab6632dd31 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_41_18, int32_t __idx_rename_at_41_19 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_41_18 != nullptr) && das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_41_18->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_41_18->value,__context__)),__idx_rename_at_41_19,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_d744859b79ee77ab ( Context * __context__, json::JsonValue * __a_rename_at_46_20, int32_t __idx_rename_at_46_21 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_46_20 != nullptr) && das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_46_20->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_46_20->value,__context__)),__idx_rename_at_46_21,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline double _FuncQmarkQmark_11719d2be16bd01 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_51_22, double __val_rename_at_51_23 )
{
    return das_auto_cast<double>::cast(((__a_rename_at_51_22 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_51_22->value)) ? das_auto_cast<double>::cast(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_51_22->value,__context__)) : das_auto_cast<double>::cast(__val_rename_at_51_23));
}

inline float _FuncQmarkQmark_233f48b50ce5f404 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_56_24, float __val_rename_at_56_25 )
{
    return das_auto_cast<float>::cast(((__a_rename_at_56_24 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_56_24->value)) ? das_auto_cast<float>::cast(float(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_56_24->value,__context__))) : das_auto_cast<float>::cast(__val_rename_at_56_25));
}

inline int8_t _FuncQmarkQmark_1a8257b5434aa8b3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_61_26, int8_t __val_rename_at_61_27 )
{
    return das_auto_cast<int8_t>::cast(((__a_rename_at_61_26 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_61_26->value)) ? das_auto_cast<int8_t>::cast(int8_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_61_26->value,__context__))) : das_auto_cast<int8_t>::cast(__val_rename_at_61_27));
}

inline int16_t _FuncQmarkQmark_f62e6510f721332c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_66_28, int16_t __val_rename_at_66_29 )
{
    return das_auto_cast<int16_t>::cast(((__a_rename_at_66_28 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_66_28->value)) ? das_auto_cast<int16_t>::cast(int16_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_66_28->value,__context__))) : das_auto_cast<int16_t>::cast(__val_rename_at_66_29));
}

inline int32_t _FuncQmarkQmark_6a40c116b1d48e91 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_30, int32_t __val_rename_at_71_31 )
{
    return das_auto_cast<int32_t>::cast(((__a_rename_at_71_30 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_71_30->value)) ? das_auto_cast<int32_t>::cast(int32_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_71_30->value,__context__))) : das_auto_cast<int32_t>::cast(__val_rename_at_71_31));
}

inline int64_t _FuncQmarkQmark_e763b5dc20b3213 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_76_32, int64_t __val_rename_at_76_33 )
{
    return das_auto_cast<int64_t>::cast(((__a_rename_at_76_32 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_76_32->value)) ? das_auto_cast<int64_t>::cast(int64_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_76_32->value,__context__))) : das_auto_cast<int64_t>::cast(__val_rename_at_76_33));
}

inline uint8_t _FuncQmarkQmark_e5a6fe7b7f0aecc6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_81_34, uint8_t __val_rename_at_81_35 )
{
    return das_auto_cast<uint8_t>::cast(((__a_rename_at_81_34 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_81_34->value)) ? das_auto_cast<uint8_t>::cast(uint8_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_81_34->value,__context__))) : das_auto_cast<uint8_t>::cast(__val_rename_at_81_35));
}

inline uint16_t _FuncQmarkQmark_b339f9b2027db27b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_86_36, uint16_t __val_rename_at_86_37 )
{
    return das_auto_cast<uint16_t>::cast(((__a_rename_at_86_36 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_86_36->value)) ? das_auto_cast<uint16_t>::cast(uint16_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_86_36->value,__context__))) : das_auto_cast<uint16_t>::cast(__val_rename_at_86_37));
}

inline uint32_t _FuncQmarkQmark_bab9e0c43667d51a ( Context * __context__, json::JsonValue const  * const  __a_rename_at_91_38, uint32_t __val_rename_at_91_39 )
{
    return das_auto_cast<uint32_t>::cast(((__a_rename_at_91_38 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_91_38->value)) ? das_auto_cast<uint32_t>::cast(uint32_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_91_38->value,__context__))) : das_auto_cast<uint32_t>::cast(__val_rename_at_91_39));
}

inline uint64_t _FuncQmarkQmark_56f43cacc498853c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_96_40, uint64_t __val_rename_at_96_41 )
{
    return das_auto_cast<uint64_t>::cast(((__a_rename_at_96_40 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_96_40->value)) ? das_auto_cast<uint64_t>::cast(uint64_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_96_40->value,__context__))) : das_auto_cast<uint64_t>::cast(__val_rename_at_96_41));
}

inline bool _FuncQmarkQmark_4fa1031aa0e66319 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_101_42, bool __val_rename_at_101_43 )
{
    return das_auto_cast<bool>::cast(((__a_rename_at_101_42 != nullptr) && das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_101_42->value)) ? das_auto_cast<bool>::cast(das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_101_42->value,__context__)) : das_auto_cast<bool>::cast(__val_rename_at_101_43));
}

inline char * _FuncQmarkQmark_5415a6b0ff5b4eb5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_106_44, char * const  __val_rename_at_106_45 )
{
    return das_auto_cast<char *>::cast(((__a_rename_at_106_44 != nullptr) && das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_106_44->value)) ? das_auto_cast<char * const >::cast(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_106_44->value,__context__)) : das_auto_cast<char * const >::cast(__val_rename_at_106_45));
}

inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  * _FuncQmarkDotTickvalue_7c5354fba05537e2 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_111_46 )
{
    return das_auto_cast<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  *>::cast(das_safe_navigation<json::JsonValue const ,AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_111_46));
}

inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> * _FuncQmarkDotTickvalue_2bdf5bca6f9d0002 ( Context * __context__, json::JsonValue * __a_rename_at_116_47 )
{
    return das_auto_cast<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> *>::cast(das_safe_navigation<json::JsonValue,AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_116_47));
}

inline json::JsonValue * JV_de08d1a11663707 ( Context * __context__, float __v_rename_at_470_48 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_471;
        das_zero(__mks_471);
        das_move((__mks_471.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_471;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_471) = double(__v_rename_at_470_48);
            return __mkv_471;
        })())));
        return __mks_471;
    })())));
}

inline json::JsonValue * JV_2fa74720cd155930 ( Context * __context__, int32_t __v_rename_at_474_49 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_475;
        das_zero(__mks_475);
        das_move((__mks_475.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_475;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_475) = double(__v_rename_at_474_49);
            return __mkv_475;
        })())));
        return __mks_475;
    })())));
}

inline json::JsonValue * JV_549d699b656608a2 ( Context * __context__, Bitfield __v_rename_at_478_50 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_479;
        das_zero(__mks_479);
        das_move((__mks_479.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_479;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_479) = double(__v_rename_at_478_50);
            return __mkv_479;
        })())));
        return __mks_479;
    })())));
}

inline json::JsonValue * JV_840ec78279ac852d ( Context * __context__, int8_t __val_rename_at_482_51 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_483;
        das_zero(__mks_483);
        das_move((__mks_483.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_483;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_483) = double(__val_rename_at_482_51);
            return __mkv_483;
        })())));
        return __mks_483;
    })())));
}

inline json::JsonValue * JV_526f504129917241 ( Context * __context__, uint8_t __val_rename_at_486_52 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_487;
        das_zero(__mks_487);
        das_move((__mks_487.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_487;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_487) = double(__val_rename_at_486_52);
            return __mkv_487;
        })())));
        return __mks_487;
    })())));
}

inline json::JsonValue * JV_17760da7cd1ea0ee ( Context * __context__, int16_t __val_rename_at_490_53 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_491;
        das_zero(__mks_491);
        das_move((__mks_491.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_491;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_491) = double(__val_rename_at_490_53);
            return __mkv_491;
        })())));
        return __mks_491;
    })())));
}

inline json::JsonValue * JV_30573b250fcf268b ( Context * __context__, uint16_t __val_rename_at_494_54 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_495;
        das_zero(__mks_495);
        das_move((__mks_495.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_495;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_495) = double(__val_rename_at_494_54);
            return __mkv_495;
        })())));
        return __mks_495;
    })())));
}

inline json::JsonValue * JV_c85629fe70fc95b5 ( Context * __context__, uint32_t __val_rename_at_498_55 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_499;
        das_zero(__mks_499);
        das_move((__mks_499.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_499;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_499) = double(__val_rename_at_498_55);
            return __mkv_499;
        })())));
        return __mks_499;
    })())));
}

inline json::JsonValue * JV_b0de7f0f7332ba2d ( Context * __context__, int64_t __val_rename_at_502_56 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__val_rename_at_502_56 < INT64_C(-2147483648)) || (__val_rename_at_502_56 > INT64_C(2147483647))) ? das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_504;
        das_zero(__mks_504);
        das_move((__mks_504.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_504;
            das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_504) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_0, cast<int64_t>::from(__val_rename_at_502_56)));
            return __mkv_504;
        })())));
        return __mks_504;
    })()))) : das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_506;
        das_zero(__mks_506);
        das_move((__mks_506.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_506;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_506) = double(__val_rename_at_502_56);
            return __mkv_506;
        })())));
        return __mks_506;
    })()))));
}

inline json::JsonValue * JV_8425ea1d753a21fb ( Context * __context__, uint64_t __val_rename_at_510_57 )
{
    return das_auto_cast<json::JsonValue *>::cast((__val_rename_at_510_57 > UINT64_C(0xffffffff)) ? das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_512;
        das_zero(__mks_512);
        das_move((__mks_512.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_512;
            das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_512) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_1, cast<uint64_t>::from(__val_rename_at_510_57)));
            return __mkv_512;
        })())));
        return __mks_512;
    })()))) : das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_514;
        das_zero(__mks_514);
        das_move((__mks_514.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_514;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_514) = double(__val_rename_at_510_57);
            return __mkv_514;
        })())));
        return __mks_514;
    })()))));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_42a1594b8ce5f90a ( Context * __context__ ) {
    TArray<ast::AstVariantMacro *> &  arg_Arr = cast_aot_arg<TArray<ast::AstVariantMacro *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstVariantMacro * arg_value = cast_aot_arg<ast::AstVariantMacro *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_42a1594b8ce5f90a(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_a504fdd4d2585454 ( Context * __context__ ) {
    json_boost::BetterJsonMacro const  &  arg_cl = cast_aot_arg<json_boost::BetterJsonMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_a504fdd4d2585454(__context__, arg_cl));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_d013d9b30ff96e9d ( Context * __context__ ) {
    TArray<ast::AstReaderMacro *> &  arg_Arr = cast_aot_arg<TArray<ast::AstReaderMacro *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstReaderMacro * arg_value = cast_aot_arg<ast::AstReaderMacro *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_d013d9b30ff96e9d(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_63e6deea4cedeb9e ( Context * __context__ ) {
    json_boost::JsonReader const  &  arg_cl = cast_aot_arg<json_boost::JsonReader const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_63e6deea4cedeb9e(__context__, arg_cl));
}
static vec4f __wrap__FuncQmarkSqblSqbr_3538a35cc5ccc9d5 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_key = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkSqblSqbr_3538a35cc5ccc9d5(__context__, arg_a, arg_key));
}
static vec4f __wrap__FuncQmarkSqblSqbr_5655a09dd3be2073 ( Context * __context__ ) {
    json::JsonValue * arg_a = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_key = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkSqblSqbr_5655a09dd3be2073(__context__, arg_a, arg_key));
}
static vec4f __wrap__FuncQmarkDot_3538a35cc5ccc9d5 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_key = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkDot_3538a35cc5ccc9d5(__context__, arg_a, arg_key));
}
static vec4f __wrap__FuncQmarkDot_5655a09dd3be2073 ( Context * __context__ ) {
    json::JsonValue * arg_a = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_key = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkDot_5655a09dd3be2073(__context__, arg_a, arg_key));
}
static vec4f __wrap__FuncQmarkSqblSqbr_ba0b0aab6632dd31 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_idx = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkSqblSqbr_ba0b0aab6632dd31(__context__, arg_a, arg_idx));
}
static vec4f __wrap__FuncQmarkSqblSqbr_d744859b79ee77ab ( Context * __context__ ) {
    json::JsonValue * arg_a = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_idx = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkSqblSqbr_d744859b79ee77ab(__context__, arg_a, arg_idx));
}
static vec4f __wrap__FuncQmarkQmark_11719d2be16bd01 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    double arg_val = cast_aot_arg<double>::to(*__context__,__context__->abiArguments()[1]);
    return cast<double>::from(_FuncQmarkQmark_11719d2be16bd01(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_233f48b50ce5f404 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    float arg_val = cast_aot_arg<float>::to(*__context__,__context__->abiArguments()[1]);
    return cast<float>::from(_FuncQmarkQmark_233f48b50ce5f404(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_1a8257b5434aa8b3 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int8_t arg_val = cast_aot_arg<int8_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int8_t>::from(_FuncQmarkQmark_1a8257b5434aa8b3(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_f62e6510f721332c ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int16_t arg_val = cast_aot_arg<int16_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int16_t>::from(_FuncQmarkQmark_f62e6510f721332c(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_6a40c116b1d48e91 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_val = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(_FuncQmarkQmark_6a40c116b1d48e91(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_e763b5dc20b3213 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int64_t arg_val = cast_aot_arg<int64_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int64_t>::from(_FuncQmarkQmark_e763b5dc20b3213(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_e5a6fe7b7f0aecc6 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint8_t arg_val = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint8_t>::from(_FuncQmarkQmark_e5a6fe7b7f0aecc6(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_b339f9b2027db27b ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint16_t arg_val = cast_aot_arg<uint16_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint16_t>::from(_FuncQmarkQmark_b339f9b2027db27b(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_bab9e0c43667d51a ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint32_t arg_val = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint32_t>::from(_FuncQmarkQmark_bab9e0c43667d51a(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_56f43cacc498853c ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint64_t arg_val = cast_aot_arg<uint64_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint64_t>::from(_FuncQmarkQmark_56f43cacc498853c(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_4fa1031aa0e66319 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    bool arg_val = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncQmarkQmark_4fa1031aa0e66319(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_5415a6b0ff5b4eb5 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_val = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(_FuncQmarkQmark_5415a6b0ff5b4eb5(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkDotTickvalue_7c5354fba05537e2 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  *>::from(_FuncQmarkDotTickvalue_7c5354fba05537e2(__context__, arg_a));
}
static vec4f __wrap__FuncQmarkDotTickvalue_2bdf5bca6f9d0002 ( Context * __context__ ) {
    json::JsonValue * arg_a = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[0]);
    return cast<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> *>::from(_FuncQmarkDotTickvalue_2bdf5bca6f9d0002(__context__, arg_a));
}
static vec4f __wrap_JV_de08d1a11663707 ( Context * __context__ ) {
    float arg_v = cast_aot_arg<float>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_de08d1a11663707(__context__, arg_v));
}
static vec4f __wrap_JV_2fa74720cd155930 ( Context * __context__ ) {
    int32_t arg_v = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_2fa74720cd155930(__context__, arg_v));
}
static vec4f __wrap_JV_549d699b656608a2 ( Context * __context__ ) {
    Bitfield arg_v = cast_aot_arg<Bitfield>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_549d699b656608a2(__context__, arg_v));
}
static vec4f __wrap_JV_840ec78279ac852d ( Context * __context__ ) {
    int8_t arg_val = cast_aot_arg<int8_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_840ec78279ac852d(__context__, arg_val));
}
static vec4f __wrap_JV_526f504129917241 ( Context * __context__ ) {
    uint8_t arg_val = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_526f504129917241(__context__, arg_val));
}
static vec4f __wrap_JV_17760da7cd1ea0ee ( Context * __context__ ) {
    int16_t arg_val = cast_aot_arg<int16_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_17760da7cd1ea0ee(__context__, arg_val));
}
static vec4f __wrap_JV_30573b250fcf268b ( Context * __context__ ) {
    uint16_t arg_val = cast_aot_arg<uint16_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_30573b250fcf268b(__context__, arg_val));
}
static vec4f __wrap_JV_c85629fe70fc95b5 ( Context * __context__ ) {
    uint32_t arg_val = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_c85629fe70fc95b5(__context__, arg_val));
}
static vec4f __wrap_JV_b0de7f0f7332ba2d ( Context * __context__ ) {
    int64_t arg_val = cast_aot_arg<int64_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_b0de7f0f7332ba2d(__context__, arg_val));
}
static vec4f __wrap_JV_8425ea1d753a21fb ( Context * __context__ ) {
    uint64_t arg_val = cast_aot_arg<uint64_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_8425ea1d753a21fb(__context__, arg_val));
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0xbcd68b55f7578ddb, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_42a1594b8ce5f90a, &__wrap__FuncbuiltinTickpushTick10769833213962245646_42a1594b8ce5f90a },
    { 0xfa80a13e757b86fc, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_a504fdd4d2585454, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_a504fdd4d2585454 },
    { 0x152dd5b51aa80cc0, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_d013d9b30ff96e9d, &__wrap__FuncbuiltinTickpushTick10769833213962245646_d013d9b30ff96e9d },
    { 0x878b5698e1990fb8, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_63e6deea4cedeb9e, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_63e6deea4cedeb9e },
    { 0xf020fec97fe5f7be, false, (void*)&_FuncQmarkSqblSqbr_3538a35cc5ccc9d5, &__wrap__FuncQmarkSqblSqbr_3538a35cc5ccc9d5 },
    { 0x90ce6642cb555679, false, (void*)&_FuncQmarkSqblSqbr_5655a09dd3be2073, &__wrap__FuncQmarkSqblSqbr_5655a09dd3be2073 },
    { 0xf020fec97fe5f7be, false, (void*)&_FuncQmarkDot_3538a35cc5ccc9d5, &__wrap__FuncQmarkDot_3538a35cc5ccc9d5 },
    { 0x90ce6642cb555679, false, (void*)&_FuncQmarkDot_5655a09dd3be2073, &__wrap__FuncQmarkDot_5655a09dd3be2073 },
    { 0xa661d1639a571f4f, false, (void*)&_FuncQmarkSqblSqbr_ba0b0aab6632dd31, &__wrap__FuncQmarkSqblSqbr_ba0b0aab6632dd31 },
    { 0xbb4bf8b0c412ba3f, false, (void*)&_FuncQmarkSqblSqbr_d744859b79ee77ab, &__wrap__FuncQmarkSqblSqbr_d744859b79ee77ab },
    { 0xf547858929db469a, false, (void*)&_FuncQmarkQmark_11719d2be16bd01, &__wrap__FuncQmarkQmark_11719d2be16bd01 },
    { 0x2052bfec09897f36, false, (void*)&_FuncQmarkQmark_233f48b50ce5f404, &__wrap__FuncQmarkQmark_233f48b50ce5f404 },
    { 0x5a64b7c5f16f6e6e, false, (void*)&_FuncQmarkQmark_1a8257b5434aa8b3, &__wrap__FuncQmarkQmark_1a8257b5434aa8b3 },
    { 0x34750f0e2f5ba325, false, (void*)&_FuncQmarkQmark_f62e6510f721332c, &__wrap__FuncQmarkQmark_f62e6510f721332c },
    { 0x90f79a17f2b92360, false, (void*)&_FuncQmarkQmark_6a40c116b1d48e91, &__wrap__FuncQmarkQmark_6a40c116b1d48e91 },
    { 0x73710cf5db3981a, false, (void*)&_FuncQmarkQmark_e763b5dc20b3213, &__wrap__FuncQmarkQmark_e763b5dc20b3213 },
    { 0x4e8da125f981ecca, false, (void*)&_FuncQmarkQmark_e5a6fe7b7f0aecc6, &__wrap__FuncQmarkQmark_e5a6fe7b7f0aecc6 },
    { 0x18007b7d06aaa2f2, false, (void*)&_FuncQmarkQmark_b339f9b2027db27b, &__wrap__FuncQmarkQmark_b339f9b2027db27b },
    { 0xb86241de1e2d6a52, false, (void*)&_FuncQmarkQmark_bab9e0c43667d51a, &__wrap__FuncQmarkQmark_bab9e0c43667d51a },
    { 0x98aebb33edcc1a02, false, (void*)&_FuncQmarkQmark_56f43cacc498853c, &__wrap__FuncQmarkQmark_56f43cacc498853c },
    { 0x96c21fd35188e801, false, (void*)&_FuncQmarkQmark_4fa1031aa0e66319, &__wrap__FuncQmarkQmark_4fa1031aa0e66319 },
    { 0x788a314f4ed9aff0, false, (void*)&_FuncQmarkQmark_5415a6b0ff5b4eb5, &__wrap__FuncQmarkQmark_5415a6b0ff5b4eb5 },
    { 0xaac7aaf08c3470bf, false, (void*)&_FuncQmarkDotTickvalue_7c5354fba05537e2, &__wrap__FuncQmarkDotTickvalue_7c5354fba05537e2 },
    { 0xf4e4a4de8ae59bb1, false, (void*)&_FuncQmarkDotTickvalue_2bdf5bca6f9d0002, &__wrap__FuncQmarkDotTickvalue_2bdf5bca6f9d0002 },
    { 0xc4c8feb11410cd0a, false, (void*)&JV_de08d1a11663707, &__wrap_JV_de08d1a11663707 },
    { 0xa1a929ec68f6efa5, false, (void*)&JV_2fa74720cd155930, &__wrap_JV_2fa74720cd155930 },
    { 0xac6ad6292aec793d, false, (void*)&JV_549d699b656608a2, &__wrap_JV_549d699b656608a2 },
    { 0xabb40867ec5a2a4b, false, (void*)&JV_840ec78279ac852d, &__wrap_JV_840ec78279ac852d },
    { 0x7eebfe09b878d154, false, (void*)&JV_526f504129917241, &__wrap_JV_526f504129917241 },
    { 0xb187bc5951542f77, false, (void*)&JV_17760da7cd1ea0ee, &__wrap_JV_17760da7cd1ea0ee },
    { 0xb7aed417e8aa85cc, false, (void*)&JV_30573b250fcf268b, &__wrap_JV_30573b250fcf268b },
    { 0xfba41459b91c9a0d, false, (void*)&JV_c85629fe70fc95b5, &__wrap_JV_c85629fe70fc95b5 },
    { 0xa239927c68d23b11, false, (void*)&JV_b0de7f0f7332ba2d, &__wrap_JV_b0de7f0f7332ba2d },
    { 0x432b947077ba23da, false, (void*)&JV_8425ea1d753a21fb, &__wrap_JV_8425ea1d753a21fb },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
    resolveTypeInfoAnnotations();
}

} // namespace _anon_14538537767599647147
AotListBase impl_aot_json_boost(_anon_14538537767599647147::registerAotFunctions);
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
