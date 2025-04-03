#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require json_boost
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
namespace _anon_11606215633914179217 {

extern StructInfo __struct_info__69e2ef6fbeda51a0;
extern TypeInfo __type_info__994066f681df7bcd;
extern TypeInfo __type_info__3fb2f51b6ebec3f2;
extern TypeInfo __type_info__e796e1d4af74511c;
extern TypeInfo __type_info__12283e04d98e7c73;
extern TypeInfo __type_info__4344a8f43280de9f;
extern TypeInfo __type_info__8d8d8008262e16ec;
extern TypeInfo __type_info__b68d800849332aec;
extern TypeInfo __type_info__790033b1854b444b;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63d94c8601e773;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__a798e985ff4af814;

TypeInfo * __type_info__a798e985ff4af814_arg_types_var_7629923981941428640[6] = { &__type_info__e796e1d4af74511c, &__type_info__994066f681df7bcd, &__type_info__af63ee4c86020b22, &__type_info__af63d94c8601e773, &__type_info__af63df4c8601f1a5, &__type_info__12283e04d98e7c73 };
const char * __type_info__a798e985ff4af814_arg_names_var_7629923981941428640[6] = { "_object", "_array", "_string", "_number", "_bool", "_null" };
VarInfo __struct_info__69e2ef6fbeda51a0_field_0 =  { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__a798e985ff4af814_arg_types_var_7629923981941428640, __type_info__a798e985ff4af814_arg_names_var_7629923981941428640, 6, 0, nullptr, 57346, 56, UINT64_C(0xa798e985ff4af814), "value", 0, 1 };
VarInfo * __struct_info__69e2ef6fbeda51a0_fields[1] =  { &__struct_info__69e2ef6fbeda51a0_field_0 };
StructInfo __struct_info__69e2ef6fbeda51a0 = {"JsonValue", "json", 28, __struct_info__69e2ef6fbeda51a0_fields, 1, 56, UINT64_C(0x0), nullptr, UINT64_C(0x69e2ef6fbeda51a0), 0 };
TypeInfo __type_info__994066f681df7bcd = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x994066f681df7bcd) };
TypeInfo __type_info__3fb2f51b6ebec3f2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__790033b1854b444b, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x3fb2f51b6ebec3f2) };
TypeInfo __type_info__e796e1d4af74511c = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0xe796e1d4af74511c) };
TypeInfo __type_info__12283e04d98e7c73 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x12283e04d98e7c73) };
TypeInfo __type_info__4344a8f43280de9f = { Type::tStructure, &__struct_info__69e2ef6fbeda51a0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 56, UINT64_C(0x4344a8f43280de9f) };
TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__b68d800849332aec = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0xb68d800849332aec) };
TypeInfo __type_info__790033b1854b444b = { Type::tStructure, &__struct_info__69e2ef6fbeda51a0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 56, UINT64_C(0x790033b1854b444b) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63d94c8601e773 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xaf63d94c8601e773) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__4344a8f43280de9f };
TypeInfo * __tinfo_1[1] = { &__type_info__8d8d8008262e16ec };
TypeInfo * __tinfo_2[1] = { &__type_info__b68d800849332aec };
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
// unused enumeration Type
// unused enumeration RefMatters
// unused enumeration ConstMatters
// unused enumeration TemporaryMatters
// unused enumeration ConversionResult
// unused enumeration SideEffects
// unused enumeration CaptureMode
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
static_assert(sizeof(AstVariantMacro)==40,"structure size mismatch with DAS");
static_assert(offsetof(AstVariantMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprIsVariant)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprAsVariant)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprSafeAsVariant)==32,"structure field offset mismatch with DAS");
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
static_assert(sizeof(AstReaderMacro)==40,"structure size mismatch with DAS");
static_assert(offsetof(AstReaderMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstReaderMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstReaderMacro,accept)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstReaderMacro,suffix)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstReaderMacro,visit)==32,"structure field offset mismatch with DAS");
}
// unused structure AstCommentReader
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
// unused structure AstVisitor
namespace json {

struct JsonValue {
    TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> value;
};
static_assert(sizeof(JsonValue)==56,"structure size mismatch with DAS");
static_assert(offsetof(JsonValue,value)==0,"structure field offset mismatch with DAS");
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
static_assert(sizeof(BetterJsonMacro)==40,"structure size mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,visitExprIsVariant)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,visitExprAsVariant)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,visitExprSafeAsVariant)==32,"structure field offset mismatch with DAS");
}
namespace json_boost {

struct JsonReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
static_assert(sizeof(JsonReader)==40,"structure size mismatch with DAS");
static_assert(offsetof(JsonReader,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonReader,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonReader,accept)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonReader,suffix)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonReader,visit)==32,"structure field offset mismatch with DAS");
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5fc7cdb5157ef6b5 ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_181_0, ast::AstVariantMacro * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9fbe49e2ac074bad ( Context * __context__, json_boost::BetterJsonMacro const  & __cl_rename_at_116_2 );
inline void _FuncbuiltinTickpushTick10769833213962245646_b8e74323e5fe5ed3 ( Context * __context__, TArray<ast::AstReaderMacro *> & __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b01bbfe8af31c12b ( Context * __context__, json_boost::JsonReader const  & __cl_rename_at_116_5 );
inline json::JsonValue & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b83b8a09925a03 ( Context * __context__, json::JsonValue & __a_rename_at_50_6 );
inline json::JsonValue * _FuncQmarkSqblSqbr_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_21_11, char * const  __key_rename_at_21_12 );
inline json::JsonValue * _FuncQmarkSqblSqbr_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_26_13, char * const  __key_rename_at_26_14 );
inline json::JsonValue * _FuncQmarkDot_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_31_15, char * const  __key_rename_at_31_16 );
inline json::JsonValue * _FuncQmarkDot_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_36_17, char * const  __key_rename_at_36_18 );
inline json::JsonValue * _FuncQmarkSqblSqbr_ba0b0aab6632dd31 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_41_19, int32_t __idx_rename_at_41_20 );
inline json::JsonValue * _FuncQmarkSqblSqbr_d744859b79ee77ab ( Context * __context__, json::JsonValue * __a_rename_at_46_21, int32_t __idx_rename_at_46_22 );
inline double _FuncQmarkQmark_11719d2be16bd01 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_51_23, double __val_rename_at_51_24 );
inline float _FuncQmarkQmark_233f48b50ce5f404 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_56_25, float __val_rename_at_56_26 );
inline int8_t _FuncQmarkQmark_1a8257b5434aa8b3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_61_27, int8_t __val_rename_at_61_28 );
inline int16_t _FuncQmarkQmark_f62e6510f721332c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_66_29, int16_t __val_rename_at_66_30 );
inline int32_t _FuncQmarkQmark_6a40c116b1d48e91 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_31, int32_t __val_rename_at_71_32 );
inline int64_t _FuncQmarkQmark_e763b5dc20b3213 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_76_33, int64_t __val_rename_at_76_34 );
inline uint8_t _FuncQmarkQmark_e5a6fe7b7f0aecc6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_81_35, uint8_t __val_rename_at_81_36 );
inline uint16_t _FuncQmarkQmark_b339f9b2027db27b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_86_37, uint16_t __val_rename_at_86_38 );
inline uint32_t _FuncQmarkQmark_bab9e0c43667d51a ( Context * __context__, json::JsonValue const  * const  __a_rename_at_91_39, uint32_t __val_rename_at_91_40 );
inline uint64_t _FuncQmarkQmark_56f43cacc498853c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_96_41, uint64_t __val_rename_at_96_42 );
inline bool _FuncQmarkQmark_4fa1031aa0e66319 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_101_43, bool __val_rename_at_101_44 );
inline char * _FuncQmarkQmark_5415a6b0ff5b4eb5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_106_45, char * const  __val_rename_at_106_46 );
inline TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  * _FuncQmarkDotTickvalue_7c5354fba05537e2 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_111_47 );
inline TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> * _FuncQmarkDotTickvalue_2bdf5bca6f9d0002 ( Context * __context__, json::JsonValue * __a_rename_at_116_48 );
inline json::JsonValue * JV_d65621deb51ffafc ( Context * __context__, float __v_rename_at_469_49 );
inline json::JsonValue * JV_ae886dab4ae4fa83 ( Context * __context__, int32_t __v_rename_at_473_50 );
inline json::JsonValue * JV_60bc939cffb9c46f ( Context * __context__, Bitfield __v_rename_at_477_51 );
inline json::JsonValue * JV_c853e30ff7129438 ( Context * __context__, int8_t __val_rename_at_481_52 );
inline json::JsonValue * JV_7e21a5b9a72123b4 ( Context * __context__, uint8_t __val_rename_at_485_53 );
inline json::JsonValue * JV_ecbc62cf23d48227 ( Context * __context__, int16_t __val_rename_at_489_54 );
inline json::JsonValue * JV_c22d4289e598b050 ( Context * __context__, uint16_t __val_rename_at_493_55 );
inline json::JsonValue * JV_9e8cb5048f43d078 ( Context * __context__, uint32_t __val_rename_at_497_56 );
inline json::JsonValue * JV_2427086886bbfd8f ( Context * __context__, int64_t __val_rename_at_501_57 );
inline json::JsonValue * JV_d32493edd280d9c1 ( Context * __context__, uint64_t __val_rename_at_509_58 );
inline json::JsonValue JsonValue_f3fa9a5f04ae4e4f ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5fc7cdb5157ef6b5 ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_181_0, ast::AstVariantMacro * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9fbe49e2ac074bad ( Context * __context__, json_boost::BetterJsonMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_b8e74323e5fe5ed3 ( Context * __context__, TArray<ast::AstReaderMacro *> &  __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 )
{
    das_copy(__Arr_rename_at_181_3(builtin_array_push_back(das_arg<TArray<ast::AstReaderMacro *>>::pass(__Arr_rename_at_181_3),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_4);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b01bbfe8af31c12b ( Context * __context__, json_boost::JsonReader const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline json::JsonValue & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b83b8a09925a03 ( Context * __context__, json::JsonValue &  __a_rename_at_50_6 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<json::JsonValue &>::from(__a_rename_at_50_6)));
    return das_auto_cast_ref<json::JsonValue &>::cast(__a_rename_at_50_6);
}

inline json::JsonValue * _FuncQmarkSqblSqbr_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_21_11, char * const  __key_rename_at_21_12 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_21_11 != nullptr) && das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::is(__a_rename_at_21_11->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__a_rename_at_21_11->value,__context__)),__key_rename_at_21_12,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_26_13, char * const  __key_rename_at_26_14 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_26_13 != nullptr) && das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::is(__a_rename_at_26_13->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__a_rename_at_26_13->value,__context__)),__key_rename_at_26_14,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u)))));
}

inline json::JsonValue * _FuncQmarkDot_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_31_15, char * const  __key_rename_at_31_16 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_31_15 != nullptr) && das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::is(__a_rename_at_31_15->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__a_rename_at_31_15->value,__context__)),__key_rename_at_31_16,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u)))));
}

inline json::JsonValue * _FuncQmarkDot_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_36_17, char * const  __key_rename_at_36_18 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_36_17 != nullptr) && das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::is(__a_rename_at_36_17->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__a_rename_at_36_17->value,__context__)),__key_rename_at_36_18,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_ba0b0aab6632dd31 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_41_19, int32_t __idx_rename_at_41_20 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_41_19 != nullptr) && das_get_variant_field<TArray<json::JsonValue *>,8,1>::is(__a_rename_at_41_19->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_variant_field<TArray<json::JsonValue *>,8,1>::as(__a_rename_at_41_19->value,__context__)),__idx_rename_at_41_20,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_d744859b79ee77ab ( Context * __context__, json::JsonValue * __a_rename_at_46_21, int32_t __idx_rename_at_46_22 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_46_21 != nullptr) && das_get_variant_field<TArray<json::JsonValue *>,8,1>::is(__a_rename_at_46_21->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_variant_field<TArray<json::JsonValue *>,8,1>::as(__a_rename_at_46_21->value,__context__)),__idx_rename_at_46_22,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 17375517337867118711u)))));
}

inline double _FuncQmarkQmark_11719d2be16bd01 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_51_23, double __val_rename_at_51_24 )
{
    return das_auto_cast<double>::cast(((__a_rename_at_51_23 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_51_23->value)) ? das_auto_cast<double>::cast(das_get_variant_field<double,8,3>::as(__a_rename_at_51_23->value,__context__)) : das_auto_cast<double>::cast(__val_rename_at_51_24));
}

inline float _FuncQmarkQmark_233f48b50ce5f404 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_56_25, float __val_rename_at_56_26 )
{
    return das_auto_cast<float>::cast(((__a_rename_at_56_25 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_56_25->value)) ? das_auto_cast<float>::cast(float(das_get_variant_field<double,8,3>::as(__a_rename_at_56_25->value,__context__))) : das_auto_cast<float>::cast(__val_rename_at_56_26));
}

inline int8_t _FuncQmarkQmark_1a8257b5434aa8b3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_61_27, int8_t __val_rename_at_61_28 )
{
    return das_auto_cast<int8_t>::cast(((__a_rename_at_61_27 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_61_27->value)) ? das_auto_cast<int8_t>::cast(int8_t(das_get_variant_field<double,8,3>::as(__a_rename_at_61_27->value,__context__))) : das_auto_cast<int8_t>::cast(__val_rename_at_61_28));
}

inline int16_t _FuncQmarkQmark_f62e6510f721332c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_66_29, int16_t __val_rename_at_66_30 )
{
    return das_auto_cast<int16_t>::cast(((__a_rename_at_66_29 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_66_29->value)) ? das_auto_cast<int16_t>::cast(int16_t(das_get_variant_field<double,8,3>::as(__a_rename_at_66_29->value,__context__))) : das_auto_cast<int16_t>::cast(__val_rename_at_66_30));
}

inline int32_t _FuncQmarkQmark_6a40c116b1d48e91 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_31, int32_t __val_rename_at_71_32 )
{
    return das_auto_cast<int32_t>::cast(((__a_rename_at_71_31 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_71_31->value)) ? das_auto_cast<int32_t>::cast(int32_t(das_get_variant_field<double,8,3>::as(__a_rename_at_71_31->value,__context__))) : das_auto_cast<int32_t>::cast(__val_rename_at_71_32));
}

inline int64_t _FuncQmarkQmark_e763b5dc20b3213 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_76_33, int64_t __val_rename_at_76_34 )
{
    return das_auto_cast<int64_t>::cast(((__a_rename_at_76_33 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_76_33->value)) ? das_auto_cast<int64_t>::cast(int64_t(das_get_variant_field<double,8,3>::as(__a_rename_at_76_33->value,__context__))) : das_auto_cast<int64_t>::cast(__val_rename_at_76_34));
}

inline uint8_t _FuncQmarkQmark_e5a6fe7b7f0aecc6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_81_35, uint8_t __val_rename_at_81_36 )
{
    return das_auto_cast<uint8_t>::cast(((__a_rename_at_81_35 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_81_35->value)) ? das_auto_cast<uint8_t>::cast(uint8_t(das_get_variant_field<double,8,3>::as(__a_rename_at_81_35->value,__context__))) : das_auto_cast<uint8_t>::cast(__val_rename_at_81_36));
}

inline uint16_t _FuncQmarkQmark_b339f9b2027db27b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_86_37, uint16_t __val_rename_at_86_38 )
{
    return das_auto_cast<uint16_t>::cast(((__a_rename_at_86_37 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_86_37->value)) ? das_auto_cast<uint16_t>::cast(uint16_t(das_get_variant_field<double,8,3>::as(__a_rename_at_86_37->value,__context__))) : das_auto_cast<uint16_t>::cast(__val_rename_at_86_38));
}

inline uint32_t _FuncQmarkQmark_bab9e0c43667d51a ( Context * __context__, json::JsonValue const  * const  __a_rename_at_91_39, uint32_t __val_rename_at_91_40 )
{
    return das_auto_cast<uint32_t>::cast(((__a_rename_at_91_39 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_91_39->value)) ? das_auto_cast<uint32_t>::cast(uint32_t(das_get_variant_field<double,8,3>::as(__a_rename_at_91_39->value,__context__))) : das_auto_cast<uint32_t>::cast(__val_rename_at_91_40));
}

inline uint64_t _FuncQmarkQmark_56f43cacc498853c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_96_41, uint64_t __val_rename_at_96_42 )
{
    return das_auto_cast<uint64_t>::cast(((__a_rename_at_96_41 != nullptr) && das_get_variant_field<double,8,3>::is(__a_rename_at_96_41->value)) ? das_auto_cast<uint64_t>::cast(uint64_t(das_get_variant_field<double,8,3>::as(__a_rename_at_96_41->value,__context__))) : das_auto_cast<uint64_t>::cast(__val_rename_at_96_42));
}

inline bool _FuncQmarkQmark_4fa1031aa0e66319 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_101_43, bool __val_rename_at_101_44 )
{
    return das_auto_cast<bool>::cast(((__a_rename_at_101_43 != nullptr) && das_get_variant_field<bool,8,4>::is(__a_rename_at_101_43->value)) ? das_auto_cast<bool>::cast(das_get_variant_field<bool,8,4>::as(__a_rename_at_101_43->value,__context__)) : das_auto_cast<bool>::cast(__val_rename_at_101_44));
}

inline char * _FuncQmarkQmark_5415a6b0ff5b4eb5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_106_45, char * const  __val_rename_at_106_46 )
{
    return das_auto_cast<char *>::cast(((__a_rename_at_106_45 != nullptr) && das_get_variant_field<char *,8,2>::is(__a_rename_at_106_45->value)) ? das_auto_cast<char * const >::cast(das_get_variant_field<char *,8,2>::as(__a_rename_at_106_45->value,__context__)) : das_auto_cast<char * const >::cast(__val_rename_at_106_46));
}

inline TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  * _FuncQmarkDotTickvalue_7c5354fba05537e2 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_111_47 )
{
    return das_auto_cast<TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  *>::cast(das_safe_navigation<json::JsonValue const ,TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_111_47));
}

inline TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> * _FuncQmarkDotTickvalue_2bdf5bca6f9d0002 ( Context * __context__, json::JsonValue * __a_rename_at_116_48 )
{
    return das_auto_cast<TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> *>::cast(das_safe_navigation<json::JsonValue,TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_116_48));
}

inline json::JsonValue * JV_d65621deb51ffafc ( Context * __context__, float __v_rename_at_469_49 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_470 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_470.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_470;
            das_get_variant_field<double,8,3>::set(__mkv_470) = double(__v_rename_at_469_49);
            return __mkv_470;
        })())));
        return __mks_470;
    })())));
}

inline json::JsonValue * JV_ae886dab4ae4fa83 ( Context * __context__, int32_t __v_rename_at_473_50 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_474 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_474.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_474;
            das_get_variant_field<double,8,3>::set(__mkv_474) = double(__v_rename_at_473_50);
            return __mkv_474;
        })())));
        return __mks_474;
    })())));
}

inline json::JsonValue * JV_60bc939cffb9c46f ( Context * __context__, Bitfield __v_rename_at_477_51 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_478 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_478.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_478;
            das_get_variant_field<double,8,3>::set(__mkv_478) = double(__v_rename_at_477_51);
            return __mkv_478;
        })())));
        return __mks_478;
    })())));
}

inline json::JsonValue * JV_c853e30ff7129438 ( Context * __context__, int8_t __val_rename_at_481_52 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_482 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_482.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_482;
            das_get_variant_field<double,8,3>::set(__mkv_482) = double(__val_rename_at_481_52);
            return __mkv_482;
        })())));
        return __mks_482;
    })())));
}

inline json::JsonValue * JV_7e21a5b9a72123b4 ( Context * __context__, uint8_t __val_rename_at_485_53 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_486 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_486.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_486;
            das_get_variant_field<double,8,3>::set(__mkv_486) = double(__val_rename_at_485_53);
            return __mkv_486;
        })())));
        return __mks_486;
    })())));
}

inline json::JsonValue * JV_ecbc62cf23d48227 ( Context * __context__, int16_t __val_rename_at_489_54 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_490 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_490.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_490;
            das_get_variant_field<double,8,3>::set(__mkv_490) = double(__val_rename_at_489_54);
            return __mkv_490;
        })())));
        return __mks_490;
    })())));
}

inline json::JsonValue * JV_c22d4289e598b050 ( Context * __context__, uint16_t __val_rename_at_493_55 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_494 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_494.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_494;
            das_get_variant_field<double,8,3>::set(__mkv_494) = double(__val_rename_at_493_55);
            return __mkv_494;
        })())));
        return __mks_494;
    })())));
}

inline json::JsonValue * JV_9e8cb5048f43d078 ( Context * __context__, uint32_t __val_rename_at_497_56 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_498 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_498.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_498;
            das_get_variant_field<double,8,3>::set(__mkv_498) = double(__val_rename_at_497_56);
            return __mkv_498;
        })())));
        return __mks_498;
    })())));
}

inline json::JsonValue * JV_2427086886bbfd8f ( Context * __context__, int64_t __val_rename_at_501_57 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__val_rename_at_501_57 < INT64_C(-2147483648)) || (__val_rename_at_501_57 > INT64_C(2147483647))) ? das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_503 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_503.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_503;
            das_get_variant_field<char *,8,2>::set(__mkv_503) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_1, cast<int64_t>::from(__val_rename_at_501_57)));
            return __mkv_503;
        })())));
        return __mks_503;
    })()))) : das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_505 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_505.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_505;
            das_get_variant_field<double,8,3>::set(__mkv_505) = double(__val_rename_at_501_57);
            return __mkv_505;
        })())));
        return __mks_505;
    })()))));
}

inline json::JsonValue * JV_d32493edd280d9c1 ( Context * __context__, uint64_t __val_rename_at_509_58 )
{
    return das_auto_cast<json::JsonValue *>::cast((__val_rename_at_509_58 > UINT64_C(0xffffffff)) ? das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_511 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_511.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_511;
            das_get_variant_field<char *,8,2>::set(__mkv_511) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_2, cast<uint64_t>::from(__val_rename_at_509_58)));
            return __mkv_511;
        })())));
        return __mks_511;
    })()))) : das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_513 = JsonValue_f3fa9a5f04ae4e4f(__context__);
        das_move((__mks_513.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_513;
            das_get_variant_field<double,8,3>::set(__mkv_513) = double(__val_rename_at_509_58);
            return __mkv_513;
        })())));
        return __mks_513;
    })()))));
}

inline json::JsonValue JsonValue_f3fa9a5f04ae4e4f ( Context * __context__ )
{
    json::JsonValue _temp_make_local_22_6_32; _temp_make_local_22_6_32;
    return /* <- */ das_auto_cast_move<json::JsonValue>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b83b8a09925a03(__context__,das_arg<json::JsonValue>::pass((([&]() -> json::JsonValue& {
        das_zero(_temp_make_local_22_6_32);
        return _temp_make_local_22_6_32;
    })()))));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xc8eef7ae328438ae] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_5fc7cdb5157ef6b5
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstVariantMacro *> & , ast::AstVariantMacro *  ),&_FuncbuiltinTickpushTick10769833213962245646_5fc7cdb5157ef6b5>>();
    };
    aotLib[0x4448371b05259953] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_9fbe49e2ac074bad
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, json_boost::BetterJsonMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_9fbe49e2ac074bad>>();
    };
    aotLib[0xa0272cece11e535f] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_b8e74323e5fe5ed3
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstReaderMacro *> & , ast::AstReaderMacro *  ),&_FuncbuiltinTickpushTick10769833213962245646_b8e74323e5fe5ed3>>();
    };
    aotLib[0x6ea3633dda2f9692] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_b01bbfe8af31c12b
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, json_boost::JsonReader const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_b01bbfe8af31c12b>>();
    };
    aotLib[0xdd0be3a0bbc261c3] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b83b8a09925a03
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue & (*) ( Context * __context__, json::JsonValue &  ),&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b83b8a09925a03>>();
    };
    aotLib[0x98b0f2a140c71331] = +[](Context & ctx) -> SimNode* { // _FuncQmarkSqblSqbr_3538a35cc5ccc9d5
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, json::JsonValue const  * const  , char * const   ),&_FuncQmarkSqblSqbr_3538a35cc5ccc9d5>>();
    };
    aotLib[0xeb918a976f722f63] = +[](Context & ctx) -> SimNode* { // _FuncQmarkSqblSqbr_5655a09dd3be2073
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, json::JsonValue * , char * const   ),&_FuncQmarkSqblSqbr_5655a09dd3be2073>>();
    };
    aotLib[0x98b0f2a140c71331] = +[](Context & ctx) -> SimNode* { // _FuncQmarkDot_3538a35cc5ccc9d5
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, json::JsonValue const  * const  , char * const   ),&_FuncQmarkDot_3538a35cc5ccc9d5>>();
    };
    aotLib[0xeb918a976f722f63] = +[](Context & ctx) -> SimNode* { // _FuncQmarkDot_5655a09dd3be2073
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, json::JsonValue * , char * const   ),&_FuncQmarkDot_5655a09dd3be2073>>();
    };
    aotLib[0x20da6b1eafc3aa78] = +[](Context & ctx) -> SimNode* { // _FuncQmarkSqblSqbr_ba0b0aab6632dd31
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, json::JsonValue const  * const  , int32_t  ),&_FuncQmarkSqblSqbr_ba0b0aab6632dd31>>();
    };
    aotLib[0x715df0a85e8212a6] = +[](Context & ctx) -> SimNode* { // _FuncQmarkSqblSqbr_d744859b79ee77ab
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, json::JsonValue * , int32_t  ),&_FuncQmarkSqblSqbr_d744859b79ee77ab>>();
    };
    aotLib[0xf547858929db469a] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_11719d2be16bd01
        return ctx.code->makeNode<SimNode_Aot<double (*) ( Context * __context__, json::JsonValue const  * const  , double  ),&_FuncQmarkQmark_11719d2be16bd01>>();
    };
    aotLib[0x2052bfec09897f36] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_233f48b50ce5f404
        return ctx.code->makeNode<SimNode_Aot<float (*) ( Context * __context__, json::JsonValue const  * const  , float  ),&_FuncQmarkQmark_233f48b50ce5f404>>();
    };
    aotLib[0x5a64b7c5f16f6e6e] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_1a8257b5434aa8b3
        return ctx.code->makeNode<SimNode_Aot<int8_t (*) ( Context * __context__, json::JsonValue const  * const  , int8_t  ),&_FuncQmarkQmark_1a8257b5434aa8b3>>();
    };
    aotLib[0x34750f0e2f5ba325] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_f62e6510f721332c
        return ctx.code->makeNode<SimNode_Aot<int16_t (*) ( Context * __context__, json::JsonValue const  * const  , int16_t  ),&_FuncQmarkQmark_f62e6510f721332c>>();
    };
    aotLib[0x90f79a17f2b92360] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_6a40c116b1d48e91
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, json::JsonValue const  * const  , int32_t  ),&_FuncQmarkQmark_6a40c116b1d48e91>>();
    };
    aotLib[0x73710cf5db3981a] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_e763b5dc20b3213
        return ctx.code->makeNode<SimNode_Aot<int64_t (*) ( Context * __context__, json::JsonValue const  * const  , int64_t  ),&_FuncQmarkQmark_e763b5dc20b3213>>();
    };
    aotLib[0x4e8da125f981ecca] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_e5a6fe7b7f0aecc6
        return ctx.code->makeNode<SimNode_Aot<uint8_t (*) ( Context * __context__, json::JsonValue const  * const  , uint8_t  ),&_FuncQmarkQmark_e5a6fe7b7f0aecc6>>();
    };
    aotLib[0x18007b7d06aaa2f2] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_b339f9b2027db27b
        return ctx.code->makeNode<SimNode_Aot<uint16_t (*) ( Context * __context__, json::JsonValue const  * const  , uint16_t  ),&_FuncQmarkQmark_b339f9b2027db27b>>();
    };
    aotLib[0xb86241de1e2d6a52] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_bab9e0c43667d51a
        return ctx.code->makeNode<SimNode_Aot<uint32_t (*) ( Context * __context__, json::JsonValue const  * const  , uint32_t  ),&_FuncQmarkQmark_bab9e0c43667d51a>>();
    };
    aotLib[0x98aebb33edcc1a02] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_56f43cacc498853c
        return ctx.code->makeNode<SimNode_Aot<uint64_t (*) ( Context * __context__, json::JsonValue const  * const  , uint64_t  ),&_FuncQmarkQmark_56f43cacc498853c>>();
    };
    aotLib[0x96c21fd35188e801] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_4fa1031aa0e66319
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, json::JsonValue const  * const  , bool  ),&_FuncQmarkQmark_4fa1031aa0e66319>>();
    };
    aotLib[0x788a314f4ed9aff0] = +[](Context & ctx) -> SimNode* { // _FuncQmarkQmark_5415a6b0ff5b4eb5
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, json::JsonValue const  * const  , char * const   ),&_FuncQmarkQmark_5415a6b0ff5b4eb5>>();
    };
    aotLib[0xaac7aaf08c3470bf] = +[](Context & ctx) -> SimNode* { // _FuncQmarkDotTickvalue_7c5354fba05537e2
        return ctx.code->makeNode<SimNode_Aot<TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  * (*) ( Context * __context__, json::JsonValue const  * const   ),&_FuncQmarkDotTickvalue_7c5354fba05537e2>>();
    };
    aotLib[0xf4e4a4de8ae59bb1] = +[](Context & ctx) -> SimNode* { // _FuncQmarkDotTickvalue_2bdf5bca6f9d0002
        return ctx.code->makeNode<SimNode_Aot<TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> * (*) ( Context * __context__, json::JsonValue *  ),&_FuncQmarkDotTickvalue_2bdf5bca6f9d0002>>();
    };
    aotLib[0x3e00f9c65476be0] = +[](Context & ctx) -> SimNode* { // JV_d65621deb51ffafc
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, float  ),&JV_d65621deb51ffafc>>();
    };
    aotLib[0x633f962a6bab4749] = +[](Context & ctx) -> SimNode* { // JV_ae886dab4ae4fa83
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, int32_t  ),&JV_ae886dab4ae4fa83>>();
    };
    aotLib[0xc79dfe12de2ab068] = +[](Context & ctx) -> SimNode* { // JV_60bc939cffb9c46f
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, Bitfield  ),&JV_60bc939cffb9c46f>>();
    };
    aotLib[0x605cb23cbf13b908] = +[](Context & ctx) -> SimNode* { // JV_c853e30ff7129438
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, int8_t  ),&JV_c853e30ff7129438>>();
    };
    aotLib[0x2f873130d783f2ca] = +[](Context & ctx) -> SimNode* { // JV_7e21a5b9a72123b4
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, uint8_t  ),&JV_7e21a5b9a72123b4>>();
    };
    aotLib[0xf8bf231c845ca87] = +[](Context & ctx) -> SimNode* { // JV_ecbc62cf23d48227
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, int16_t  ),&JV_ecbc62cf23d48227>>();
    };
    aotLib[0x134721cd41d6d743] = +[](Context & ctx) -> SimNode* { // JV_c22d4289e598b050
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, uint16_t  ),&JV_c22d4289e598b050>>();
    };
    aotLib[0xe60586d420975f06] = +[](Context & ctx) -> SimNode* { // JV_9e8cb5048f43d078
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, uint32_t  ),&JV_9e8cb5048f43d078>>();
    };
    aotLib[0x64e606e0f420e36c] = +[](Context & ctx) -> SimNode* { // JV_2427086886bbfd8f
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, int64_t  ),&JV_2427086886bbfd8f>>();
    };
    aotLib[0x70a0d1508894563b] = +[](Context & ctx) -> SimNode* { // JV_d32493edd280d9c1
        return ctx.code->makeNode<SimNode_Aot<json::JsonValue * (*) ( Context * __context__, uint64_t  ),&JV_d32493edd280d9c1>>();
    };
    aotLib[0x5f301e389c83e78c] = +[](Context & ctx) -> SimNode* { // JsonValue_f3fa9a5f04ae4e4f
        return ctx.code->makeNode<SimNode_AotCMRES<json::JsonValue (*) ( Context * __context__ ),&JsonValue_f3fa9a5f04ae4e4f>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_11606215633914179217
AotListBase impl_aot_json_boost(_anon_11606215633914179217::registerAotFunctions);
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
