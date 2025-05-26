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
namespace _anon_307576627166999491 {

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
// unused enumeration Type
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
}
template <> struct cast< das::_anon_307576627166999491::match::MatchType > : cast_enum < das::_anon_307576627166999491::match::MatchType > {};
namespace _anon_307576627166999491 {
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

TypeInfo __type_info__960dd6428887a234 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Expression"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 72, UINT64_C(0x960dd6428887a234) };
TypeInfo __type_info__6636442e03391ebf = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__960dd6428887a234, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0x6636442e03391ebf) };
TypeInfo __type_info__f40dfc0210a91e40 = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__6636442e03391ebf, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0xf40dfc0210a91e40) };
TypeInfo __type_info__62934ec39d8af40 = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__6636442e03391ebf, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0x62934ec39d8af40) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__960dd6428887a234, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__f40dfc0210a91e40 };
TypeInfo * __tinfo_1[1] = { &__type_info__62934ec39d8af40 };

inline void finalize_414b0e9d7e0d05d4 ( Context * __context__, match::MatchError & ____this_rename_at_22_0 );
inline void _FuncbuiltinTickpushTick10769833213962245646_9a94d5ed4f7c5f3d ( Context * __context__, TArray<ast::AstCallMacro *> & __Arr_rename_at_181_1, ast::AstCallMacro * __value_rename_at_181_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_14482599e5d01774 ( Context * __context__, match::MatchMacro const  & __cl_rename_at_116_3 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cb4cd03341f58180 ( Context * __context__, match::StaticMatchMacro const  & __cl_rename_at_116_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4a6f9c0a519aac5b ( Context * __context__, match::MultiMatchMacro const  & __cl_rename_at_116_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7cd1d67f64fe3e91 ( Context * __context__, match::StaticMultiMatchMacro const  & __cl_rename_at_116_6 );
inline void _FuncbuiltinTickpushTick10769833213962245646_7af251da517ddbb8 ( Context * __context__, TArray<ast::AstStructureAnnotation *> & __Arr_rename_at_181_7, ast::AstStructureAnnotation * __value_rename_at_181_8 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7ba19a495954c081 ( Context * __context__, match::MatchAsIs const  & __cl_rename_at_116_9 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_76522b9b3e47321 ( Context * __context__, match::MatchCopy const  & __cl_rename_at_116_10 );
inline Expression * _FuncbuiltinTickget_ptrTick5807679485210906136_e7e091949cec6eca ( Context * __context__, smart_ptr_raw<Expression> __src_rename_at_1732_11 );
inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1059_12, smart_ptr_raw<Expression> const  __src_rename_at_1059_13 );
inline void _FuncbuiltinTickclearTick13739625760977891612_25b71f090d00bdfd ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __t_rename_at_1860_22 );
inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1935193042646774172_f514d43564b04cd6 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  & __a_rename_at_1143_23 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_466efab99bab67f9 ( Context * __context__, TArray<match::MatchError> & __a_rename_at_1182_25 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_df7cbfbee29f183e ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  & __Tab_rename_at_1014_27, char * const  __at_rename_at_1014_28 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_3be19cef7fa4f1bb ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_29 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_819be9fd29ef5a2a ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1744_31 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_dc408e0240d4e964 ( Context * __context__, TTable<char *,void> & __a_rename_at_1193_33 );
inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick11707039267614988023_ad6868fe4237fa10 ( Context * __context__, int32_t __value_rename_at_882_34 );
inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick2307832460972925437_9df83295710ab92 ( Context * __context__, int32_t __value_rename_at_878_35 );
inline void _FuncbuiltinTickcloneTick3147220447302434744_a74294804c3d9cbb ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_1119_36, TTable<char *,smart_ptr_raw<Expression>> const  & __b_rename_at_1119_37 );
inline bool _FuncbuiltinTickeraseTick5639988512056021548_91f8484c392734b0 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __Tab_rename_at_888_43, char * const  __at_rename_at_888_44 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_72f851f70bf1b9a1 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_1193_45 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_66bdfa02abd78edc ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_32_47, TTable<char *,smart_ptr_raw<Expression>> & __b_rename_at_32_48 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __a_rename_at_1182_49 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5dcbdce47081955f ( Context * __context__, TArray<smart_ptr_raw<ExprIfThenElse>> & __a_rename_at_1182_51 );
inline void _FuncbuiltinTickpushTick10769833213962245646_3b73112f82278e2d ( Context * __context__, TArray<match::MatchError> & __Arr_rename_at_181_53, match::MatchError & __value_rename_at_181_54 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_55, bool __extra_rename_at_38_56, bool __contracts_rename_at_38_57, bool __modules_rename_at_38_58 );
inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_59 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_b92bd4043c99d0b3 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __Arr_rename_at_287_60, smart_ptr_raw<Expression> & __value_rename_at_287_61 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_7d01ceb11c7f2e53 ( Context * __context__, TTable<char *,void> const  & __Tab_rename_at_1014_62, char * const  __at_rename_at_1014_63 );
inline void _FuncbuiltinTickinsertTick10959621454228962049_c5249085906957b9 ( Context * __context__, TTable<char *,void> & __Tab_rename_at_895_64, char * const  __at_rename_at_895_65 );
inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_1bb8b96fa387e2c9 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1726_66 );
inline void clone_5060aa66df6619fa ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_271_67, smart_ptr_raw<TypeDecl> const  __src_rename_at_271_68 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_c034f3dacbf79ab1 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  & __a_rename_at_1128_69 );
inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1351216622833168869_9f93f2d584a44443 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_1150_71 );
inline void finalize_6a3a1e840a2c86c6 ( Context * __context__, match::MatchTo & ____this_rename_at_27_73 );
inline Expression * _FuncbuiltinTickcopy_to_localTick18007282088148548588_b574445f4ca8c1bd ( Context * __context__, Expression * const  __a_rename_at_1029_74 );
inline smart_ptr_raw<Expression> & _FuncbuiltinTickbackTick18296309835877697278_f87543352f697520 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __a_rename_at_473_75 );
inline void _FuncbuiltinTickpopTick1161079256290593740_b6f51194af943535 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __Arr_rename_at_132_77 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_c7cece75493a5357 ( Context * __context__, TArray<smart_ptr_raw<ExprIfThenElse>> & __Arr_rename_at_287_78, smart_ptr_raw<ExprIfThenElse> & __value_rename_at_287_79 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_9e263c2fcec12657 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __Arr_rename_at_68_80, int32_t __newSize_rename_at_68_81 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<bool,0x2c53781188f39986>(__context__) = false;/*LOG_MATCH*/
}

inline void finalize_414b0e9d7e0d05d4 ( Context * __context__, match::MatchError &  ____this_rename_at_22_0 )
{
    memset(&(____this_rename_at_22_0), 0, TypeSize<match::MatchError>::size);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_9a94d5ed4f7c5f3d ( Context * __context__, TArray<ast::AstCallMacro *> &  __Arr_rename_at_181_1, ast::AstCallMacro * __value_rename_at_181_2 )
{
    das_copy(__Arr_rename_at_181_1(builtin_array_push_back(das_arg<TArray<ast::AstCallMacro *>>::pass(__Arr_rename_at_181_1),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_2);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_14482599e5d01774 ( Context * __context__, match::MatchMacro const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cb4cd03341f58180 ( Context * __context__, match::StaticMatchMacro const  &  __cl_rename_at_116_4 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_4.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4a6f9c0a519aac5b ( Context * __context__, match::MultiMatchMacro const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7cd1d67f64fe3e91 ( Context * __context__, match::StaticMultiMatchMacro const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_7af251da517ddbb8 ( Context * __context__, TArray<ast::AstStructureAnnotation *> &  __Arr_rename_at_181_7, ast::AstStructureAnnotation * __value_rename_at_181_8 )
{
    das_copy(__Arr_rename_at_181_7(builtin_array_push_back(das_arg<TArray<ast::AstStructureAnnotation *>>::pass(__Arr_rename_at_181_7),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_8);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7ba19a495954c081 ( Context * __context__, match::MatchAsIs const  &  __cl_rename_at_116_9 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_9.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_76522b9b3e47321 ( Context * __context__, match::MatchCopy const  &  __cl_rename_at_116_10 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_10.__rtti))).getStructType())));
}

inline Expression * _FuncbuiltinTickget_ptrTick5807679485210906136_e7e091949cec6eca ( Context * __context__, smart_ptr_raw<Expression> __src_rename_at_1732_11 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1732_11));
}

inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1059_12, smart_ptr_raw<Expression> const  __src_rename_at_1059_13 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1059_12),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1059_13),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickclearTick13739625760977891612_25b71f090d00bdfd ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __t_rename_at_1860_22 )
{
    builtin_table_clear(das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__t_rename_at_1860_22),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1935193042646774172_f514d43564b04cd6 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  &  __a_rename_at_1143_23 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) __it_rename_at_1144_24; das_zero(__it_rename_at_1144_24);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((smart_ptr_raw<Expression> const ))>::pass(__it_rename_at_1144_24),__a_rename_at_1143_23,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((smart_ptr_raw<Expression> &))>::cast(__it_rename_at_1144_24);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_466efab99bab67f9 ( Context * __context__, TArray<match::MatchError> &  __a_rename_at_1182_25 )
{
    {
        bool __need_loop_1184 = true;
        // aV: match::MatchError aka TT&
        das_iterator<TArray<match::MatchError>> __aV_iterator(__a_rename_at_1182_25);
        match::MatchError * __aV_rename_at_1184_26;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_26)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_26)) )
        {
            finalize_414b0e9d7e0d05d4(__context__,das_arg<match::MatchError>::pass((*__aV_rename_at_1184_26)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_26));
    };
    builtin_array_free(das_arg<TArray<match::MatchError>>::pass(__a_rename_at_1182_25),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_df7cbfbee29f183e ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  &  __Tab_rename_at_1014_27, char * const  __at_rename_at_1014_28 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_27,__at_rename_at_1014_28));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_3be19cef7fa4f1bb ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_29 )
{
    smart_ptr_raw<Expression> __clone_dest_rename_at_1058_30; das_zero(__clone_dest_rename_at_1058_30);
    clone_912660ab1e667e4b(__context__,__clone_dest_rename_at_1058_30,__clone_src_rename_at_1056_29);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__clone_dest_rename_at_1058_30);
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_819be9fd29ef5a2a ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1744_31 )
{
    smart_ptr_raw<Expression> __dst_rename_at_1746_32; das_zero(__dst_rename_at_1746_32); das_move(__dst_rename_at_1746_32, _FuncbuiltinTickclone_to_moveTick2007252383599261567_3be19cef7fa4f1bb(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__src_rename_at_1744_31)));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__dst_rename_at_1746_32);
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_dc408e0240d4e964 ( Context * __context__, TTable<char *,void> &  __a_rename_at_1193_33 )
{
    builtin_table_free(das_arg<TTable<char *,void>>::pass(__a_rename_at_1193_33),8,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick11707039267614988023_ad6868fe4237fa10 ( Context * __context__, int32_t __value_rename_at_882_34 )
{
    LineInfo _temp_make_local_883_43_32; _temp_make_local_883_43_32;
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_invoke_function<smart_ptr_raw<Expression>>::invoke<int32_t,LineInfo const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@match::ast_boost`convert_to_expression`16483834305137942954 C=Xi CH<rtti::LineInfo>*/ 0x9948a15c469f344d)),__value_rename_at_882_34,das_arg<LineInfo>::pass((_temp_make_local_883_43_32 = (/*c-tor*/ LineInfo(/*end-c-tor*/))))));
}

inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick2307832460972925437_9df83295710ab92 ( Context * __context__, int32_t __value_rename_at_878_35 )
{
    LineInfo _temp_make_local_879_43_32; _temp_make_local_879_43_32;
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_invoke_function<smart_ptr_raw<Expression>>::invoke<int32_t &,LineInfo const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@match::ast_boost`convert_to_expression`6276016433326983145 &=Xi CH<rtti::LineInfo>*/ 0x2b8e615a4ccdb016)),__value_rename_at_878_35,das_arg<LineInfo>::pass((_temp_make_local_879_43_32 = (/*c-tor*/ LineInfo(/*end-c-tor*/))))));
}

inline void _FuncbuiltinTickcloneTick3147220447302434744_a74294804c3d9cbb ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_1119_36, TTable<char *,smart_ptr_raw<Expression>> const  &  __b_rename_at_1119_37 )
{
    Sequence DAS_COMMENT((char *)) _temp_make_local_1121_17_32; _temp_make_local_1121_17_32;
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) _temp_make_local_1121_26_48; _temp_make_local_1121_26_48;
    _FuncbuiltinTickclearTick13739625760977891612_25b71f090d00bdfd(__context__,das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1119_36));
    {
        bool __need_loop_1121 = true;
        // k: string
        das_iterator<Sequence DAS_COMMENT((char *))> __k_iterator((_temp_make_local_1121_17_32 = (_FuncbuiltinTickkeysTick2205854368403803976_c034f3dacbf79ab1(__context__,__b_rename_at_1119_37))));
        char * __k_rename_at_1121_40;
        __need_loop_1121 = __k_iterator.first(__context__,(__k_rename_at_1121_40)) && __need_loop_1121;
        // v: smart_ptr<ast::Expression> const&
        das_iterator<Sequence DAS_COMMENT((smart_ptr_raw<Expression> const ))> __v_iterator((_temp_make_local_1121_26_48 = (_FuncbuiltinTickvaluesTick1935193042646774172_f514d43564b04cd6(__context__,__b_rename_at_1119_37))));
        smart_ptr_raw<Expression> const  * __v_rename_at_1121_41;
        __need_loop_1121 = __v_iterator.first(__context__,(__v_rename_at_1121_41)) && __need_loop_1121;
        for ( ; __need_loop_1121 ; __need_loop_1121 = __k_iterator.next(__context__,(__k_rename_at_1121_40)) && __v_iterator.next(__context__,(__v_rename_at_1121_41)) )
        {
            char * __kk_rename_at_1122_42 = ((char *)(char *)(__k_rename_at_1121_40));
            clone_912660ab1e667e4b(__context__,__a_rename_at_1119_36(__kk_rename_at_1122_42,__context__),(*__v_rename_at_1121_41));
        }
        __k_iterator.close(__context__,(__k_rename_at_1121_40));
        __v_iterator.close(__context__,(__v_rename_at_1121_41));
    };
}

inline bool _FuncbuiltinTickeraseTick5639988512056021548_91f8484c392734b0 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __Tab_rename_at_888_43, char * const  __at_rename_at_888_44 )
{
    return das_auto_cast<bool>::cast(__builtin_table_erase(__context__,__Tab_rename_at_888_43,__at_rename_at_888_44));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_72f851f70bf1b9a1 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_1193_45 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) _temp_make_local_1195_19_32; _temp_make_local_1195_19_32;
    {
        bool __need_loop_1195 = true;
        // aV: smart_ptr<ast::Expression>&
        das_iterator<Sequence DAS_COMMENT((smart_ptr_raw<Expression>))> __aV_iterator((_temp_make_local_1195_19_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_9f93f2d584a44443(__context__,das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1193_45)))));
        smart_ptr_raw<Expression> * __aV_rename_at_1195_46;
        __need_loop_1195 = __aV_iterator.first(__context__,(__aV_rename_at_1195_46)) && __need_loop_1195;
        for ( ; __need_loop_1195 ; __need_loop_1195 = __aV_iterator.next(__context__,(__aV_rename_at_1195_46)) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,(*__aV_rename_at_1195_46));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1195_46));
    };
    builtin_table_free(das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1193_45),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_66bdfa02abd78edc ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_32_47, TTable<char *,smart_ptr_raw<Expression>> &  __b_rename_at_32_48 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TTable<char *,smart_ptr_raw<Expression>> &>::from(__a_rename_at_32_47)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TTable<char *,smart_ptr_raw<Expression>> &>::from(__b_rename_at_32_48)));
    das_move(__a_rename_at_32_47,__b_rename_at_32_48);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __a_rename_at_1182_49 )
{
    {
        bool __need_loop_1184 = true;
        // aV: smart_ptr<ast::Expression> aka TT&
        das_iterator<TArray<smart_ptr_raw<Expression>>> __aV_iterator(__a_rename_at_1182_49);
        smart_ptr_raw<Expression> * __aV_rename_at_1184_50;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_50)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_50)) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,(*__aV_rename_at_1184_50));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_50));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__a_rename_at_1182_49),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5dcbdce47081955f ( Context * __context__, TArray<smart_ptr_raw<ExprIfThenElse>> &  __a_rename_at_1182_51 )
{
    {
        bool __need_loop_1184 = true;
        // aV: smart_ptr<ast::ExprIfThenElse> aka TT&
        das_iterator<TArray<smart_ptr_raw<ExprIfThenElse>>> __aV_iterator(__a_rename_at_1182_51);
        smart_ptr_raw<ExprIfThenElse> * __aV_rename_at_1184_52;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_52)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_52)) )
        {
            das_delete_handle<smart_ptr_raw<ExprIfThenElse>>::clear(__context__,(*__aV_rename_at_1184_52));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_52));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<ExprIfThenElse>>>::pass(__a_rename_at_1182_51),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_3b73112f82278e2d ( Context * __context__, TArray<match::MatchError> &  __Arr_rename_at_181_53, match::MatchError &  __value_rename_at_181_54 )
{
    das_copy(__Arr_rename_at_181_53(builtin_array_push_back(das_arg<TArray<match::MatchError>>::pass(__Arr_rename_at_181_53),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_54);
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_55, bool __extra_rename_at_38_56, bool __contracts_rename_at_38_57, bool __modules_rename_at_38_58 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_55,__extra_rename_at_38_56,__contracts_rename_at_38_57,__modules_rename_at_38_58,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_59 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_59,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_b92bd4043c99d0b3 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __Arr_rename_at_287_60, smart_ptr_raw<Expression> & __value_rename_at_287_61 )
{
    das_move(__Arr_rename_at_287_60(builtin_array_push_back(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_287_60),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_61);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_7d01ceb11c7f2e53 ( Context * __context__, TTable<char *,void> const  &  __Tab_rename_at_1014_62, char * const  __at_rename_at_1014_63 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_62,__at_rename_at_1014_63));
}

inline void _FuncbuiltinTickinsertTick10959621454228962049_c5249085906957b9 ( Context * __context__, TTable<char *,void> &  __Tab_rename_at_895_64, char * const  __at_rename_at_895_65 )
{
    __builtin_table_set_insert(__context__,__Tab_rename_at_895_64,__at_rename_at_895_65);
}

inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_1bb8b96fa387e2c9 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1726_66 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1726_66));
}

inline void clone_5060aa66df6619fa ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_271_67, smart_ptr_raw<TypeDecl> const  __src_rename_at_271_68 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_271_67),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_271_68),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_c034f3dacbf79ab1 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> const  &  __a_rename_at_1128_69 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1129_70; das_zero(__it_rename_at_1129_70);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1129_70),__a_rename_at_1128_69,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1129_70);
}

inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1351216622833168869_9f93f2d584a44443 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_1150_71 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) __it_rename_at_1151_72; das_zero(__it_rename_at_1151_72);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((smart_ptr_raw<Expression>))>::pass(__it_rename_at_1151_72),das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1150_71),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((smart_ptr_raw<Expression> &))>::cast(__it_rename_at_1151_72);
}

inline void finalize_6a3a1e840a2c86c6 ( Context * __context__, match::MatchTo &  ____this_rename_at_27_73 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(____this_rename_at_27_73.conditions));
    _FuncbuiltinTickfinalizeTick5454204887383796109_72f851f70bf1b9a1(__context__,das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(____this_rename_at_27_73.declarations));
    _FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(____this_rename_at_27_73.prefix));
    _FuncbuiltinTickfinalizeTick13836114024949725080_466efab99bab67f9(__context__,das_arg<TArray<match::MatchError>>::pass(____this_rename_at_27_73.errors));
    memset(&(____this_rename_at_27_73), 0, TypeSize<match::MatchTo>::size);
}

inline Expression * _FuncbuiltinTickcopy_to_localTick18007282088148548588_b574445f4ca8c1bd ( Context * __context__, Expression * const  __a_rename_at_1029_74 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__a_rename_at_1029_74));
}

inline smart_ptr_raw<Expression> & _FuncbuiltinTickbackTick18296309835877697278_f87543352f697520 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __a_rename_at_473_75 )
{
    int32_t __l_rename_at_474_76 = ((int32_t)builtin_array_size(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__a_rename_at_473_75)));
    if ( __l_rename_at_474_76 == 0 )
    {
        builtin_throw(((char *) "back empty array"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast_ref<smart_ptr_raw<Expression> &>::cast(__a_rename_at_473_75((__l_rename_at_474_76 - 1),__context__));
}

inline void _FuncbuiltinTickpopTick1161079256290593740_b6f51194af943535 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __Arr_rename_at_132_77 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_9e263c2fcec12657(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_132_77),builtin_array_size(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_132_77)) - 1);
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_c7cece75493a5357 ( Context * __context__, TArray<smart_ptr_raw<ExprIfThenElse>> &  __Arr_rename_at_287_78, smart_ptr_raw<ExprIfThenElse> & __value_rename_at_287_79 )
{
    das_move(__Arr_rename_at_287_78(builtin_array_push_back(das_arg<TArray<smart_ptr_raw<ExprIfThenElse>>>::pass(__Arr_rename_at_287_78),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_79);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_9e263c2fcec12657 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __Arr_rename_at_68_80, int32_t __newSize_rename_at_68_81 )
{
    builtin_array_resize(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_68_80),__newSize_rename_at_68_81,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x239f3865f432736d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_414b0e9d7e0d05d4>>();
    };
    aotLib[0x3fdd65aa3293d0d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_9a94d5ed4f7c5f3d>>();
    };
    aotLib[0x90ce7b780ab559b3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_14482599e5d01774>>();
    };
    aotLib[0x3096cfdd34ff84b0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_cb4cd03341f58180>>();
    };
    aotLib[0x847f38a00e8e7e8d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_4a6f9c0a519aac5b>>();
    };
    aotLib[0x8d92289ec99ae4e4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_7cd1d67f64fe3e91>>();
    };
    aotLib[0xa23a03df76258238] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_7af251da517ddbb8>>();
    };
    aotLib[0x9aa6322995645e2c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_7ba19a495954c081>>();
    };
    aotLib[0xf92ab6937854236c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_76522b9b3e47321>>();
    };
    aotLib[0xbdc77a5284aa34d1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_e7e091949cec6eca>>();
    };
    aotLib[0x74b4210f5379bdbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_912660ab1e667e4b>>();
    };
    aotLib[0x4b429bc40890445f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclearTick13739625760977891612_25b71f090d00bdfd>>();
    };
    aotLib[0xa3a823c94699a813] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1935193042646774172_f514d43564b04cd6>>();
    };
    aotLib[0xbe5f1274cd77e78b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_466efab99bab67f9>>();
    };
    aotLib[0x902bf0ea704b6a27] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_df7cbfbee29f183e>>();
    };
    aotLib[0x59616b0dede6cdc0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_3be19cef7fa4f1bb>>();
    };
    aotLib[0xb90ab61edb364836] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickadd_ptr_refTick918185754185293024_819be9fd29ef5a2a>>();
    };
    aotLib[0xc707bf006629afd3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_dc408e0240d4e964>>();
    };
    aotLib[0xc0b1ef038a331daf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickconvert_to_expressionTick11707039267614988023_ad6868fe4237fa10>>();
    };
    aotLib[0xb3f615defccccede] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickconvert_to_expressionTick2307832460972925437_9df83295710ab92>>();
    };
    aotLib[0x55c59a60ce9a1e81] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3147220447302434744_a74294804c3d9cbb>>();
    };
    aotLib[0x1260150baa98b822] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickeraseTick5639988512056021548_91f8484c392734b0>>();
    };
    aotLib[0xc7ee3cd12c03ddbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_72f851f70bf1b9a1>>();
    };
    aotLib[0x313f2a7a770d5505] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_66bdfa02abd78edc>>();
    };
    aotLib[0x9707e43dd37f682e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36>>();
    };
    aotLib[0x2071654b51fc0c3c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_5dcbdce47081955f>>();
    };
    aotLib[0xf14a2fbc0b10f5cc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_3b73112f82278e2d>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    aotLib[0xfbfe800d5700e08f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick842554968825501494_452cfc8955963a18>>();
    };
    aotLib[0xef8ca2af328d02ba] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_b92bd4043c99d0b3>>();
    };
    aotLib[0x45d41e57e6a0547a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_7d01ceb11c7f2e53>>();
    };
    aotLib[0x1cdfb8b03b66c023] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick10959621454228962049_c5249085906957b9>>();
    };
    aotLib[0xfb1b5f1d296293ac] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_1bb8b96fa387e2c9>>();
    };
    aotLib[0xd91c260c49716a97] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_5060aa66df6619fa>>();
    };
    aotLib[0xa973d7821c77c4cc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_c034f3dacbf79ab1>>();
    };
    aotLib[0x405a946c7f6266f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_9f93f2d584a44443>>();
    };
    aotLib[0x854a274a3c6b7e0f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_6a3a1e840a2c86c6>>();
    };
    aotLib[0x8e80661e1a73403a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcopy_to_localTick18007282088148548588_b574445f4ca8c1bd>>();
    };
    aotLib[0x2b7956fdbbbb5902] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickbackTick18296309835877697278_f87543352f697520>>();
    };
    aotLib[0x2570f03063a2fcf0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_b6f51194af943535>>();
    };
    aotLib[0xaca0dd8b4d6252a1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_c7cece75493a5357>>();
    };
    aotLib[0xac15c8ea3cbf4d34] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_9e263c2fcec12657>>();
    };
    // [[ init script ]]
    aotLib[0xfadca203af9aa883] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_307576627166999491
AotListBase impl_aot_match(_anon_307576627166999491::registerAotFunctions);
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
