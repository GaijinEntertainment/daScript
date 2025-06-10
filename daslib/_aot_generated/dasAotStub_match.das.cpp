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
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require match

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
namespace _anon_17347265907563674293 {

namespace match { struct MatchError; };
namespace match { struct MatchTo; };
namespace match { struct MatchMacro; };
namespace match { struct StaticMatchMacro; };
namespace match { struct MultiMatchMacro; };
namespace match { struct StaticMultiMatchMacro; };
namespace match { struct MatchAsIs; };
namespace match { struct MatchCopy; };
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
namespace ast {

struct AstStructureAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
};
}
// unused structure AstPassMacro
// unused structure AstVariantMacro
// unused structure AstForLoopMacro
// unused structure AstCaptureMacro
// unused structure AstTypeMacro
// unused structure AstSimulateMacro
// unused structure AstReaderMacro
// unused structure AstCommentReader
namespace ast {

struct AstCallMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
}
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
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
// unused structure DeferMacro
// unused structure DeferDeleteMacro
namespace match {

enum class MatchType : int32_t {
    none = int32_t(0),
    as_is = int32_t(1),
    copy = int32_t(2),
};
}
namespace match {

struct MatchError {
    char * msg;
    LineInfo const  * at;
};
}
namespace match {

struct MatchTo {
    TArray<smart_ptr_raw<Expression>> conditions;
    TTable<char *,smart_ptr_raw<Expression>> declarations;
    TArray<smart_ptr_raw<Expression>> prefix;
    TArray<match::MatchError> errors;
    bool failed;
};
}
namespace match {

struct MatchMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
    bool report_match_errors;
    bool multi_match;
};
}
namespace match {

struct StaticMatchMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
    bool report_match_errors;
    bool multi_match;
};
}
namespace match {

struct MultiMatchMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
    bool report_match_errors;
    bool multi_match;
};
}
namespace match {

struct StaticMultiMatchMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
    bool report_match_errors;
    bool multi_match;
};
}
namespace match {

struct MatchAsIs {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
};
}
namespace match {

struct MatchCopy {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
};
}
extern TypeInfo __type_info__960dd6428887a234;
extern TypeInfo __type_info__6636442e03391ebf;
extern TypeInfo __type_info__f40dfc0210a91e40;
extern TypeInfo __type_info__62934ec39d8af40;
extern TypeInfo __type_info__af63ee4c86020b22;

TypeInfo __type_info__960dd6428887a234 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Expression"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<Expression>::size, UINT64_C(0x960dd6428887a234) };
TypeInfo __type_info__6636442e03391ebf = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__960dd6428887a234, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<Expression>>::size, UINT64_C(0x6636442e03391ebf) };
TypeInfo __type_info__f40dfc0210a91e40 = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__6636442e03391ebf, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TTable<char *,smart_ptr_raw<Expression>>>::size, UINT64_C(0xf40dfc0210a91e40) };
TypeInfo __type_info__62934ec39d8af40 = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__6636442e03391ebf, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TTable<char *,smart_ptr_raw<Expression>>>::size, UINT64_C(0x62934ec39d8af40) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xaf63ee4c86020b22) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__960dd6428887a234, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__f40dfc0210a91e40 };
TypeInfo * __tinfo_1[1] = { &__type_info__62934ec39d8af40 };

inline void finalize_7de70b3a87da99c ( Context * __context__, match::MatchError & ____this_rename_at_22_0 );
inline void _FuncbuiltinTickpushTick10769833213962245646_b9312a94adb052e2 ( Context * __context__, TArray<ast::AstCallMacro *> & __Arr_rename_at_181_1, ast::AstCallMacro * __value_rename_at_181_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e814d7eb7ed32a41 ( Context * __context__, match::MatchMacro const  & __cl_rename_at_116_3 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_891c7b72c4609483 ( Context * __context__, match::StaticMatchMacro const  & __cl_rename_at_116_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_ad047021c028e08e ( Context * __context__, match::MultiMatchMacro const  & __cl_rename_at_116_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cd71dfc9f5f43568 ( Context * __context__, match::StaticMultiMatchMacro const  & __cl_rename_at_116_6 );
inline void _FuncbuiltinTickpushTick10769833213962245646_f075d577d5ac90cf ( Context * __context__, TArray<ast::AstStructureAnnotation *> & __Arr_rename_at_181_7, ast::AstStructureAnnotation * __value_rename_at_181_8 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_370314afa283e506 ( Context * __context__, match::MatchAsIs const  & __cl_rename_at_116_9 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_31258e2a1de9312d ( Context * __context__, match::MatchCopy const  & __cl_rename_at_116_10 );
inline Expression * _FuncbuiltinTickget_ptrTick5807679485210906136_f531b48dc835d1d0 ( Context * __context__, smart_ptr_raw<Expression> __src_rename_at_1784_11 );
inline void clone_a26f8516739f700c ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1156_20, smart_ptr_raw<Expression> const  __src_rename_at_1156_21 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_72c70d983283f783 ( Context * __context__, TArray<match::MatchError> & __a_rename_at_1234_22 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_fe9afc88da6f2b8e ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  & __Tab_rename_at_1047_24, char * const  __at_rename_at_1047_25 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_794d03aa0bcd2402 ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1089_26 );
inline void _FuncbuiltinTickclearTick13739625760977891612_8e0248a95951e6b5 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __t_rename_at_1912_28 );
inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1935193042646774172_6b6e3377af02313c ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  & __a_rename_at_1195_29 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_38d04e24c5355465 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1796_31 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_5d8d8913eafebde ( Context * __context__, TTable<char *,void> & __a_rename_at_1245_33 );
inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick11707039267614988023_f29ab3c342af3ae8 ( Context * __context__, int32_t __value_rename_at_849_34 );
inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick2307832460972925437_fb1fa3c5a95a22b5 ( Context * __context__, int32_t __value_rename_at_845_35 );
inline bool _FuncbuiltinTickeraseTick5639988512056021548_2e7256b451ea3841 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __Tab_rename_at_888_36, char * const  __at_rename_at_888_37 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_d8ec63490871010b ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_1245_38 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_4fc6a0de51b821d1 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_32_40, TTable<char *,smart_ptr_raw<Expression>> & __b_rename_at_32_41 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_aecd77b0d22800cb ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __a_rename_at_1234_42 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_e5def9d2e098a29b ( Context * __context__, TArray<smart_ptr_raw<ExprIfThenElse>> & __a_rename_at_1234_44 );
inline void _FuncbuiltinTickpushTick10769833213962245646_1335728b9b97e58a ( Context * __context__, TArray<match::MatchError> & __Arr_rename_at_181_46, match::MatchError & __value_rename_at_181_47 );
inline char * _FuncastTickdescribeTick2562845734617055679_bbbb9047f2ea1f10 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_48, bool __extra_rename_at_38_49, bool __contracts_rename_at_38_50, bool __modules_rename_at_38_51 );
inline char * _FuncastTickdescribeTick842554968825501494_54d2507a5b959fef ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_52 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_c03eeaac05f59d1c ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __Arr_rename_at_287_53, smart_ptr_raw<Expression> & __value_rename_at_287_54 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_c219a27a82924d04 ( Context * __context__, TTable<char *,void> const  & __Tab_rename_at_1047_55, char * const  __at_rename_at_1047_56 );
inline void _FuncbuiltinTickinsertTick10959621454228962049_73755b668ac27e60 ( Context * __context__, TTable<char *,void> & __Tab_rename_at_895_57, char * const  __at_rename_at_895_58 );
inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_ec0a4f330584599 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1778_59 );
inline void clone_e70ebf4308af3640 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_271_60, smart_ptr_raw<TypeDecl> const  __src_rename_at_271_61 );
inline void _FuncbuiltinTickemplace_newTick990257600204377963_7a4e9660ab7b333c ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __Arr_rename_at_1036_62, smart_ptr_raw<Expression> __value_rename_at_1036_63 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_6fa84009b4f6f658 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  & __a_rename_at_1180_64 );
inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1351216622833168869_598b45e67d3b15e7 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_1202_66 );
inline void _FuncbuiltinTickcloneTick3147220447302434744_b66d431794fbf1f4 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_1152_68, TTable<char *,smart_ptr_raw<Expression>> const  & __b_rename_at_1152_69 );
inline void finalize_e2ab2b98368a70a9 ( Context * __context__, match::MatchTo & ____this_rename_at_27_75 );
inline Expression * _FuncbuiltinTickcopy_to_localTick18007282088148548588_3c3467e92f539b70 ( Context * __context__, Expression * const  __a_rename_at_1062_76 );
inline smart_ptr_raw<Expression> & _FuncbuiltinTickbackTick18296309835877697278_f546a19c7d57fc90 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __a_rename_at_473_77 );
inline void _FuncbuiltinTickpopTick1161079256290593740_a412c4f535d9d848 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __Arr_rename_at_132_79 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_c98f3e0d4cb371b7 ( Context * __context__, TArray<smart_ptr_raw<ExprIfThenElse>> & __Arr_rename_at_287_80, smart_ptr_raw<ExprIfThenElse> & __value_rename_at_287_81 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_e5810af7f4ff9bb6 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __Arr_rename_at_68_82, int32_t __newSize_rename_at_68_83 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<bool,0x2c53781188f39986>(__context__) = false;/*LOG_MATCH*/
}

inline void finalize_7de70b3a87da99c ( Context * __context__, match::MatchError &  ____this_rename_at_22_0 )
{
    memset((void*)&(____this_rename_at_22_0), 0, TypeSize<match::MatchError>::size);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_b9312a94adb052e2 ( Context * __context__, TArray<ast::AstCallMacro *> &  __Arr_rename_at_181_1, ast::AstCallMacro * __value_rename_at_181_2 )
{
    das_copy(__Arr_rename_at_181_1(builtin_array_push_back(das_arg<TArray<ast::AstCallMacro *>>::pass(__Arr_rename_at_181_1),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_2);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e814d7eb7ed32a41 ( Context * __context__, match::MatchMacro const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_891c7b72c4609483 ( Context * __context__, match::StaticMatchMacro const  &  __cl_rename_at_116_4 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_4.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_ad047021c028e08e ( Context * __context__, match::MultiMatchMacro const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cd71dfc9f5f43568 ( Context * __context__, match::StaticMultiMatchMacro const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_f075d577d5ac90cf ( Context * __context__, TArray<ast::AstStructureAnnotation *> &  __Arr_rename_at_181_7, ast::AstStructureAnnotation * __value_rename_at_181_8 )
{
    das_copy(__Arr_rename_at_181_7(builtin_array_push_back(das_arg<TArray<ast::AstStructureAnnotation *>>::pass(__Arr_rename_at_181_7),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_8);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_370314afa283e506 ( Context * __context__, match::MatchAsIs const  &  __cl_rename_at_116_9 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_9.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_31258e2a1de9312d ( Context * __context__, match::MatchCopy const  &  __cl_rename_at_116_10 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_10.__rtti))).getStructType())));
}

inline Expression * _FuncbuiltinTickget_ptrTick5807679485210906136_f531b48dc835d1d0 ( Context * __context__, smart_ptr_raw<Expression> __src_rename_at_1784_11 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1784_11));
}

inline void clone_a26f8516739f700c ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1156_20, smart_ptr_raw<Expression> const  __src_rename_at_1156_21 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1156_20),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1156_21),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_72c70d983283f783 ( Context * __context__, TArray<match::MatchError> &  __a_rename_at_1234_22 )
{
    {
        bool __need_loop_1236 = true;
        // aV: match::MatchError aka TT&
        das_iterator<TArray<match::MatchError>> __aV_iterator(__a_rename_at_1234_22);
        match::MatchError * __aV_rename_at_1236_23;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_23)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_23)) )
        {
            finalize_7de70b3a87da99c(__context__,das_arg<match::MatchError>::pass((*__aV_rename_at_1236_23)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_23));
    };
    builtin_array_free(das_arg<TArray<match::MatchError>>::pass(__a_rename_at_1234_22),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_fe9afc88da6f2b8e ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  &  __Tab_rename_at_1047_24, char * const  __at_rename_at_1047_25 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_24,__at_rename_at_1047_25));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_794d03aa0bcd2402 ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1089_26 )
{
    smart_ptr_raw<Expression> __clone_dest_rename_at_1091_27;das_zero(__clone_dest_rename_at_1091_27);
    clone_a26f8516739f700c(__context__,__clone_dest_rename_at_1091_27,__clone_src_rename_at_1089_26);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__clone_dest_rename_at_1091_27);
}

inline void _FuncbuiltinTickclearTick13739625760977891612_8e0248a95951e6b5 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __t_rename_at_1912_28 )
{
    builtin_table_clear(das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__t_rename_at_1912_28),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1935193042646774172_6b6e3377af02313c ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  &  __a_rename_at_1195_29 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) __it_rename_at_1196_30;das_zero(__it_rename_at_1196_30);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((smart_ptr_raw<Expression> const ))>::pass(__it_rename_at_1196_30),__a_rename_at_1195_29,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((smart_ptr_raw<Expression> &))>::cast(__it_rename_at_1196_30);
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_38d04e24c5355465 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1796_31 )
{
    smart_ptr_raw<Expression> __dst_rename_at_1798_32; das_zero(__dst_rename_at_1798_32); das_move(__dst_rename_at_1798_32, _FuncbuiltinTickclone_to_moveTick2007252383599261567_794d03aa0bcd2402(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__src_rename_at_1796_31)));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__dst_rename_at_1798_32);
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_5d8d8913eafebde ( Context * __context__, TTable<char *,void> &  __a_rename_at_1245_33 )
{
    builtin_table_free(das_arg<TTable<char *,void>>::pass(__a_rename_at_1245_33),8,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick11707039267614988023_f29ab3c342af3ae8 ( Context * __context__, int32_t __value_rename_at_849_34 )
{
    LineInfo _temp_make_local_850_43_0; _temp_make_local_850_43_0;
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_invoke_function<smart_ptr_raw<Expression>>::invoke<int32_t,LineInfo const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@match::ast_boost`convert_to_expression`16483834305137942954 C=Xi CH<rtti::LineInfo>*/ 0x9948a15c469f344d)),__value_rename_at_849_34,das_arg<LineInfo>::pass((_temp_make_local_850_43_0 = (/*c-tor*/ LineInfo(/*end-c-tor*/))))));
}

inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick2307832460972925437_fb1fa3c5a95a22b5 ( Context * __context__, int32_t __value_rename_at_845_35 )
{
    LineInfo _temp_make_local_846_43_1; _temp_make_local_846_43_1;
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_invoke_function<smart_ptr_raw<Expression>>::invoke<int32_t &,LineInfo const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@match::ast_boost`convert_to_expression`6276016433326983145 &=Xi CH<rtti::LineInfo>*/ 0x2b8e615a4ccdb016)),__value_rename_at_845_35,das_arg<LineInfo>::pass((_temp_make_local_846_43_1 = (/*c-tor*/ LineInfo(/*end-c-tor*/))))));
}

inline bool _FuncbuiltinTickeraseTick5639988512056021548_2e7256b451ea3841 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __Tab_rename_at_888_36, char * const  __at_rename_at_888_37 )
{
    return das_auto_cast<bool>::cast(__builtin_table_erase(__context__,__Tab_rename_at_888_36,__at_rename_at_888_37));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_d8ec63490871010b ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_1245_38 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) _temp_make_local_1247_19_2; _temp_make_local_1247_19_2;
    {
        bool __need_loop_1247 = true;
        // aV: smart_ptr<ast::Expression>&
        das_iterator<Sequence DAS_COMMENT((smart_ptr_raw<Expression>))> __aV_iterator((_temp_make_local_1247_19_2 = (_FuncbuiltinTickvaluesTick1351216622833168869_598b45e67d3b15e7(__context__,das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1245_38)))));
        smart_ptr_raw<Expression> * __aV_rename_at_1247_39;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_39)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_39)) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,(*__aV_rename_at_1247_39));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_39));
    };
    builtin_table_free(das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1245_38),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_4fc6a0de51b821d1 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_32_40, TTable<char *,smart_ptr_raw<Expression>> &  __b_rename_at_32_41 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TTable<char *,smart_ptr_raw<Expression>> &>::from(__a_rename_at_32_40)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TTable<char *,smart_ptr_raw<Expression>> &>::from(__b_rename_at_32_41)));
    das_move(__a_rename_at_32_40,__b_rename_at_32_41);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_aecd77b0d22800cb ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __a_rename_at_1234_42 )
{
    {
        bool __need_loop_1236 = true;
        // aV: smart_ptr<ast::Expression> aka TT&
        das_iterator<TArray<smart_ptr_raw<Expression>>> __aV_iterator(__a_rename_at_1234_42);
        smart_ptr_raw<Expression> * __aV_rename_at_1236_43;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_43)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_43)) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,(*__aV_rename_at_1236_43));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_43));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__a_rename_at_1234_42),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_e5def9d2e098a29b ( Context * __context__, TArray<smart_ptr_raw<ExprIfThenElse>> &  __a_rename_at_1234_44 )
{
    {
        bool __need_loop_1236 = true;
        // aV: smart_ptr<ast::ExprIfThenElse> aka TT&
        das_iterator<TArray<smart_ptr_raw<ExprIfThenElse>>> __aV_iterator(__a_rename_at_1234_44);
        smart_ptr_raw<ExprIfThenElse> * __aV_rename_at_1236_45;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_45)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_45)) )
        {
            das_delete_handle<smart_ptr_raw<ExprIfThenElse>>::clear(__context__,(*__aV_rename_at_1236_45));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_45));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<ExprIfThenElse>>>::pass(__a_rename_at_1234_44),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_1335728b9b97e58a ( Context * __context__, TArray<match::MatchError> &  __Arr_rename_at_181_46, match::MatchError &  __value_rename_at_181_47 )
{
    das_copy(__Arr_rename_at_181_46(builtin_array_push_back(das_arg<TArray<match::MatchError>>::pass(__Arr_rename_at_181_46),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_47);
}

inline char * _FuncastTickdescribeTick2562845734617055679_bbbb9047f2ea1f10 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_48, bool __extra_rename_at_38_49, bool __contracts_rename_at_38_50, bool __modules_rename_at_38_51 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_48,__extra_rename_at_38_49,__contracts_rename_at_38_50,__modules_rename_at_38_51,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * _FuncastTickdescribeTick842554968825501494_54d2507a5b959fef ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_52 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_52,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_c03eeaac05f59d1c ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __Arr_rename_at_287_53, smart_ptr_raw<Expression> & __value_rename_at_287_54 )
{
    das_move(__Arr_rename_at_287_53(builtin_array_push_back(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_287_53),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_54);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_c219a27a82924d04 ( Context * __context__, TTable<char *,void> const  &  __Tab_rename_at_1047_55, char * const  __at_rename_at_1047_56 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_55,__at_rename_at_1047_56));
}

inline void _FuncbuiltinTickinsertTick10959621454228962049_73755b668ac27e60 ( Context * __context__, TTable<char *,void> &  __Tab_rename_at_895_57, char * const  __at_rename_at_895_58 )
{
    __builtin_table_set_insert(__context__,__Tab_rename_at_895_57,__at_rename_at_895_58);
}

inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_ec0a4f330584599 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1778_59 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1778_59));
}

inline void clone_e70ebf4308af3640 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_271_60, smart_ptr_raw<TypeDecl> const  __src_rename_at_271_61 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_271_60),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_271_61),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickemplace_newTick990257600204377963_7a4e9660ab7b333c ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __Arr_rename_at_1036_62, smart_ptr_raw<Expression> __value_rename_at_1036_63 )
{
    das_move(__Arr_rename_at_1036_62(builtin_array_push_back(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_1036_62),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_1036_63);
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_6fa84009b4f6f658 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  &  __a_rename_at_1180_64 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1181_65;das_zero(__it_rename_at_1181_65);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1181_65),__a_rename_at_1180_64,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1181_65);
}

inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1351216622833168869_598b45e67d3b15e7 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_1202_66 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) __it_rename_at_1203_67;das_zero(__it_rename_at_1203_67);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((smart_ptr_raw<Expression>))>::pass(__it_rename_at_1203_67),das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1202_66),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((smart_ptr_raw<Expression> &))>::cast(__it_rename_at_1203_67);
}

inline void _FuncbuiltinTickcloneTick3147220447302434744_b66d431794fbf1f4 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_1152_68, TTable<char *,smart_ptr_raw<Expression>> const  &  __b_rename_at_1152_69 )
{
    Sequence DAS_COMMENT((char *)) _temp_make_local_1154_17_3; _temp_make_local_1154_17_3;
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) _temp_make_local_1154_26_4; _temp_make_local_1154_26_4;
    _FuncbuiltinTickclearTick13739625760977891612_8e0248a95951e6b5(__context__,das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1152_68));
    {
        bool __need_loop_1154 = true;
        // k: string
        das_iterator<Sequence DAS_COMMENT((char *))> __k_iterator((_temp_make_local_1154_17_3 = (_FuncbuiltinTickkeysTick2205854368403803976_6fa84009b4f6f658(__context__,__b_rename_at_1152_69))));
        char * __k_rename_at_1154_72;
        __need_loop_1154 = __k_iterator.first(__context__,(__k_rename_at_1154_72)) && __need_loop_1154;
        // v: smart_ptr<ast::Expression> const&
        das_iterator<Sequence DAS_COMMENT((smart_ptr_raw<Expression> const ))> __v_iterator((_temp_make_local_1154_26_4 = (_FuncbuiltinTickvaluesTick1935193042646774172_6b6e3377af02313c(__context__,__b_rename_at_1152_69))));
        smart_ptr_raw<Expression> const  * __v_rename_at_1154_73;
        __need_loop_1154 = __v_iterator.first(__context__,(__v_rename_at_1154_73)) && __need_loop_1154;
        for ( ; __need_loop_1154 ; __need_loop_1154 = __k_iterator.next(__context__,(__k_rename_at_1154_72)) && __v_iterator.next(__context__,(__v_rename_at_1154_73)) )
        {
            char * __kk_rename_at_1155_74 = ((char *)(char *)(__k_rename_at_1154_72));
            clone_a26f8516739f700c(__context__,__a_rename_at_1152_68(__kk_rename_at_1155_74,__context__),(*__v_rename_at_1154_73));
        }
        __k_iterator.close(__context__,(__k_rename_at_1154_72));
        __v_iterator.close(__context__,(__v_rename_at_1154_73));
    };
}

inline void finalize_e2ab2b98368a70a9 ( Context * __context__, match::MatchTo &  ____this_rename_at_27_75 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_aecd77b0d22800cb(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(____this_rename_at_27_75.conditions));
    _FuncbuiltinTickfinalizeTick5454204887383796109_d8ec63490871010b(__context__,das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(____this_rename_at_27_75.declarations));
    _FuncbuiltinTickfinalizeTick13836114024949725080_aecd77b0d22800cb(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(____this_rename_at_27_75.prefix));
    _FuncbuiltinTickfinalizeTick13836114024949725080_72c70d983283f783(__context__,das_arg<TArray<match::MatchError>>::pass(____this_rename_at_27_75.errors));
    memset((void*)&(____this_rename_at_27_75), 0, TypeSize<match::MatchTo>::size);
}

inline Expression * _FuncbuiltinTickcopy_to_localTick18007282088148548588_3c3467e92f539b70 ( Context * __context__, Expression * const  __a_rename_at_1062_76 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__a_rename_at_1062_76));
}

inline smart_ptr_raw<Expression> & _FuncbuiltinTickbackTick18296309835877697278_f546a19c7d57fc90 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __a_rename_at_473_77 )
{
    int32_t __l_rename_at_474_78 = ((int32_t)builtin_array_size(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__a_rename_at_473_77)));
    if ( __l_rename_at_474_78 == 0 )
    {
        builtin_throw(((char *) "back empty array"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast_ref<smart_ptr_raw<Expression> &>::cast(__a_rename_at_473_77((__l_rename_at_474_78 - 1),__context__));
}

inline void _FuncbuiltinTickpopTick1161079256290593740_a412c4f535d9d848 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __Arr_rename_at_132_79 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_e5810af7f4ff9bb6(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_132_79),builtin_array_size(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_132_79)) - 1);
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_c98f3e0d4cb371b7 ( Context * __context__, TArray<smart_ptr_raw<ExprIfThenElse>> &  __Arr_rename_at_287_80, smart_ptr_raw<ExprIfThenElse> & __value_rename_at_287_81 )
{
    das_move(__Arr_rename_at_287_80(builtin_array_push_back(das_arg<TArray<smart_ptr_raw<ExprIfThenElse>>>::pass(__Arr_rename_at_287_80),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_81);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_e5810af7f4ff9bb6 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __Arr_rename_at_68_82, int32_t __newSize_rename_at_68_83 )
{
    builtin_array_resize(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_68_82),__newSize_rename_at_68_83,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x5a16a983b4aa7a36] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_7de70b3a87da99c>>();
    };
    aotLib[0x993ebc516d8bca81] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_b9312a94adb052e2>>();
    };
    aotLib[0x94b437fa32cddeb8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_e814d7eb7ed32a41>>();
    };
    aotLib[0xc6b7fae98fe8901] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_891c7b72c4609483>>();
    };
    aotLib[0x1d234f74d54d706d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_ad047021c028e08e>>();
    };
    aotLib[0x5713ac5acd277c03] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_cd71dfc9f5f43568>>();
    };
    aotLib[0xa057407f174f6a8d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_f075d577d5ac90cf>>();
    };
    aotLib[0x12996d42198cc06b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_370314afa283e506>>();
    };
    aotLib[0x6868c7e6395ebfc2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_31258e2a1de9312d>>();
    };
    aotLib[0x3ec5845a72cbba9f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_f531b48dc835d1d0>>();
    };
    aotLib[0x9bd73ffdead9ca84] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_a26f8516739f700c>>();
    };
    aotLib[0x9904bb01e2e4d58] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_72c70d983283f783>>();
    };
    aotLib[0xd4fef85d65b0a3cf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_fe9afc88da6f2b8e>>();
    };
    aotLib[0xf3a5ac5c07975296] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_794d03aa0bcd2402>>();
    };
    aotLib[0xcd9dc906aea46510] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclearTick13739625760977891612_8e0248a95951e6b5>>();
    };
    aotLib[0x9ec3da62b045fc38] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1935193042646774172_6b6e3377af02313c>>();
    };
    aotLib[0x4923b3b7110455c1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickadd_ptr_refTick918185754185293024_38d04e24c5355465>>();
    };
    aotLib[0xd80f6e7a05d7ee0e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_5d8d8913eafebde>>();
    };
    aotLib[0x7318fac246a22a4d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickconvert_to_expressionTick11707039267614988023_f29ab3c342af3ae8>>();
    };
    aotLib[0xbaf44cecf3ffa216] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickconvert_to_expressionTick2307832460972925437_fb1fa3c5a95a22b5>>();
    };
    aotLib[0x5169ff62dfb428dc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickeraseTick5639988512056021548_2e7256b451ea3841>>();
    };
    aotLib[0xffb9b438bc67a19d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_d8ec63490871010b>>();
    };
    aotLib[0x77de701793340b5b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_4fc6a0de51b821d1>>();
    };
    aotLib[0x6cb936a2011ed26] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_aecd77b0d22800cb>>();
    };
    aotLib[0xc72f0699a72c217] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_e5def9d2e098a29b>>();
    };
    aotLib[0xcab443508cf57e08] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_1335728b9b97e58a>>();
    };
    aotLib[0xe59d0bcce14ff636] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_bbbb9047f2ea1f10>>();
    };
    aotLib[0x7708f8f3dd06fbab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick842554968825501494_54d2507a5b959fef>>();
    };
    aotLib[0xb61bd251761e3cf0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_c03eeaac05f59d1c>>();
    };
    aotLib[0x2a3cae48b31b67aa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_c219a27a82924d04>>();
    };
    aotLib[0xd7fc8f3a849a5c37] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick10959621454228962049_73755b668ac27e60>>();
    };
    aotLib[0x9f30ec06bccefb1c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_ec0a4f330584599>>();
    };
    aotLib[0x9dcf27b9818a2bd7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_e70ebf4308af3640>>();
    };
    aotLib[0xe9107d629866fa8d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplace_newTick990257600204377963_7a4e9660ab7b333c>>();
    };
    aotLib[0xa993d142b92509ee] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_6fa84009b4f6f658>>();
    };
    aotLib[0x8ab970f04127a14e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_598b45e67d3b15e7>>();
    };
    aotLib[0xb3c35e3644a46a4c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3147220447302434744_b66d431794fbf1f4>>();
    };
    aotLib[0x4d20c874bf9abcb6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_e2ab2b98368a70a9>>();
    };
    aotLib[0xc883f91ed83c0b03] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcopy_to_localTick18007282088148548588_3c3467e92f539b70>>();
    };
    aotLib[0xec221c6f3462b424] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickbackTick18296309835877697278_f546a19c7d57fc90>>();
    };
    aotLib[0x506d3def6871ae9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_a412c4f535d9d848>>();
    };
    aotLib[0x39aee0bfa83ac130] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_c98f3e0d4cb371b7>>();
    };
    aotLib[0xded839115812c64b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_e5810af7f4ff9bb6>>();
    };
    // [[ init script ]]
    aotLib[0x2c664719d7981584] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_17347265907563674293
AotListBase impl_aot_match(_anon_17347265907563674293::registerAotFunctions);
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
