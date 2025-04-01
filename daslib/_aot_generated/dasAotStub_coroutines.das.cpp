#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require coroutines
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
namespace _anon_15530702915337158574 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

namespace coroutines { struct YieldFrom; };
namespace coroutines { struct CoContinue; };
namespace coroutines { struct CoAwait; };
namespace coroutines { struct CoroutineMacro; };
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
// unused enumeration Type
// unused enumeration RefMatters
// unused enumeration ConstMatters
// unused enumeration TemporaryMatters
// unused enumeration ConversionResult
// unused enumeration SideEffects
// unused enumeration CaptureMode
namespace ast {

struct AstFunctionAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,das::string)) transform;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) verifyCall;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) generic_apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) fixup;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) lint;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,das::vector<smart_ptr<TypeDecl>>,AnnotationDeclaration const ,das::string)) isCompatible;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation)) isSpecialized;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function> const ,AnnotationDeclaration const ,das::string)) appendToMangledName;
};
static_assert(sizeof(AstFunctionAnnotation)==112,"structure size mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
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
namespace ast {

struct AstCallMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
static_assert(sizeof(AstCallMacro)==48,"structure size mismatch with DAS");
static_assert(offsetof(AstCallMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstCallMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstCallMacro,preVisit)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstCallMacro,visit)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstCallMacro,canVisitArgument)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(AstCallMacro,canFoldReturnResult)==40,"structure field offset mismatch with DAS");
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
namespace coroutines {

struct YieldFrom {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
static_assert(sizeof(YieldFrom)==48,"structure size mismatch with DAS");
static_assert(offsetof(YieldFrom,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(YieldFrom,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(YieldFrom,preVisit)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(YieldFrom,visit)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(YieldFrom,canVisitArgument)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(YieldFrom,canFoldReturnResult)==40,"structure field offset mismatch with DAS");
}
namespace coroutines {

struct CoContinue {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
static_assert(sizeof(CoContinue)==48,"structure size mismatch with DAS");
static_assert(offsetof(CoContinue,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(CoContinue,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(CoContinue,preVisit)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(CoContinue,visit)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(CoContinue,canVisitArgument)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(CoContinue,canFoldReturnResult)==40,"structure field offset mismatch with DAS");
}
namespace coroutines {

struct CoAwait {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
static_assert(sizeof(CoAwait)==48,"structure size mismatch with DAS");
static_assert(offsetof(CoAwait,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(CoAwait,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(CoAwait,preVisit)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(CoAwait,visit)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(CoAwait,canVisitArgument)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(CoAwait,canFoldReturnResult)==40,"structure field offset mismatch with DAS");
}
namespace coroutines {

struct CoroutineMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,das::string)) transform;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) verifyCall;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) generic_apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) fixup;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) lint;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,das::vector<smart_ptr<TypeDecl>>,AnnotationDeclaration const ,das::string)) isCompatible;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation)) isSpecialized;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function> const ,AnnotationDeclaration const ,das::string)) appendToMangledName;
};
static_assert(sizeof(CoroutineMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(CoroutineMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(CoroutineMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}

inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1060_0, smart_ptr_raw<Expression> const  __src_rename_at_1060_1 );
inline void _FuncbuiltinTickpushTick10769833213962245646_3771763ef4e38f67 ( Context * __context__, TArray<ast::AstCallMacro *> & __Arr_rename_at_181_2, ast::AstCallMacro * __value_rename_at_181_3 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_473f16910ed8ed4c ( Context * __context__, coroutines::YieldFrom const  & __cl_rename_at_116_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e6d28334897e87cd ( Context * __context__, coroutines::CoContinue const  & __cl_rename_at_116_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_fb3c0bbea8b7355 ( Context * __context__, coroutines::CoAwait const  & __cl_rename_at_116_6 );
inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_7, ast::AstFunctionAnnotation * __value_rename_at_181_8 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c46027390bfcc614 ( Context * __context__, coroutines::CoroutineMacro const  & __cl_rename_at_116_9 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_fdd31121648b840f ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1057_10 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_c38be5eeaf10cee3 ( Context * __context__, smart_ptr<Expression> const  __src_rename_at_1745_12 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_76c025eb1c6d5401 ( Context * __context__, Sequence DAS_COMMENT((bool)) & __it_rename_at_1224_14, bool & __value_rename_at_1224_15 );
inline void _FuncbuiltinTickeraseTick16646986352019611268_20dfa2ac4bec4492 ( Context * __context__, TArray<Sequence DAS_COMMENT((bool))> & __Arr_rename_at_535_16, int32_t __at_rename_at_535_17 );
inline void cr_run_71b7571079173083 ( Context * __context__, Sequence DAS_COMMENT((bool)) & __a_rename_at_100_18 );
inline void cr_run_all_6cfec073aa72fb77 ( Context * __context__, TArray<Sequence DAS_COMMENT((bool))> & __a_rename_at_107_20 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1060_0, smart_ptr_raw<Expression> const  __src_rename_at_1060_1 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1060_0),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1060_1),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_3771763ef4e38f67 ( Context * __context__, TArray<ast::AstCallMacro *> &  __Arr_rename_at_181_2, ast::AstCallMacro * __value_rename_at_181_3 )
{
    das_copy(__Arr_rename_at_181_2(builtin_array_push_back(das_arg<TArray<ast::AstCallMacro *>>::pass(__Arr_rename_at_181_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_3);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_473f16910ed8ed4c ( Context * __context__, coroutines::YieldFrom const  &  __cl_rename_at_116_4 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_4.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e6d28334897e87cd ( Context * __context__, coroutines::CoContinue const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_fb3c0bbea8b7355 ( Context * __context__, coroutines::CoAwait const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_7, ast::AstFunctionAnnotation * __value_rename_at_181_8 )
{
    das_copy(__Arr_rename_at_181_7(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_7),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_8);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c46027390bfcc614 ( Context * __context__, coroutines::CoroutineMacro const  &  __cl_rename_at_116_9 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_9.__rtti))).getStructType())));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_fdd31121648b840f ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1057_10 )
{
    smart_ptr_raw<Expression> __clone_dest_rename_at_1059_11; das_zero(__clone_dest_rename_at_1059_11);
    clone_912660ab1e667e4b(__context__,__clone_dest_rename_at_1059_11,__clone_src_rename_at_1057_10);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__clone_dest_rename_at_1059_11);
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_c38be5eeaf10cee3 ( Context * __context__, smart_ptr<Expression> const  __src_rename_at_1745_12 )
{
    smart_ptr_raw<Expression> __dst_rename_at_1747_13; das_zero(__dst_rename_at_1747_13); das_move(__dst_rename_at_1747_13, _FuncbuiltinTickclone_to_moveTick2007252383599261567_fdd31121648b840f(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__src_rename_at_1745_12)));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__dst_rename_at_1747_13);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_76c025eb1c6d5401 ( Context * __context__, Sequence DAS_COMMENT((bool)) &  __it_rename_at_1224_14, bool & __value_rename_at_1224_15 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((bool))>::pass(__it_rename_at_1224_14),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1224_15)),__context__));
}

inline void _FuncbuiltinTickeraseTick16646986352019611268_20dfa2ac4bec4492 ( Context * __context__, TArray<Sequence DAS_COMMENT((bool))> &  __Arr_rename_at_535_16, int32_t __at_rename_at_535_17 )
{
    builtin_array_erase(das_arg<TArray<Sequence DAS_COMMENT((bool))>>::pass(__Arr_rename_at_535_16),__at_rename_at_535_17,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void cr_run_71b7571079173083 ( Context * __context__, Sequence DAS_COMMENT((bool)) &  __a_rename_at_100_18 )
{
    {
        bool __need_loop_102 = true;
        // t: bool
        das_iterator<Sequence DAS_COMMENT((bool))> __t_iterator(__a_rename_at_100_18);
        bool __t_rename_at_102_19;
        __need_loop_102 = __t_iterator.first(__context__,(__t_rename_at_102_19)) && __need_loop_102;
        for ( ; __need_loop_102 ; __need_loop_102 = __t_iterator.next(__context__,(__t_rename_at_102_19)) )
        {
        }
        __t_iterator.close(__context__,(__t_rename_at_102_19));
    };
}

inline void cr_run_all_6cfec073aa72fb77 ( Context * __context__, TArray<Sequence DAS_COMMENT((bool))> &  __a_rename_at_107_20 )
{
    while ( true )
    {
        int32_t __i_rename_at_110_21 = builtin_array_size(das_arg<TArray<Sequence DAS_COMMENT((bool))>>::pass(__a_rename_at_107_20));
        if ( __i_rename_at_110_21 == 0 )
        {
            break;
        } else {
            while ( __i_rename_at_110_21 > 0 )
            {
                --__i_rename_at_110_21;
                bool __t_rename_at_116_22 = 0;
                _FuncbuiltinTicknextTick17450348357676149856_76c025eb1c6d5401(__context__,das_arg<Sequence DAS_COMMENT((bool))>::pass(__a_rename_at_107_20(__i_rename_at_110_21,__context__)),__t_rename_at_116_22);
                if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((bool))>::pass(__a_rename_at_107_20(__i_rename_at_110_21,__context__))) )
                {
                    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((bool))>::pass(__a_rename_at_107_20(__i_rename_at_110_21,__context__)),__context__);
                    _FuncbuiltinTickeraseTick16646986352019611268_20dfa2ac4bec4492(__context__,das_arg<TArray<Sequence DAS_COMMENT((bool))>>::pass(__a_rename_at_107_20),__i_rename_at_110_21);
                };
            };
        };
    };
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x74b4210f5379bdbd] = +[](Context & ctx) -> SimNode* { // clone_912660ab1e667e4b
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr_raw<Expression> & , smart_ptr_raw<Expression> const   ),&clone_912660ab1e667e4b>>();
    };
    aotLib[0xfca27d59ea83e1a5] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_3771763ef4e38f67
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstCallMacro *> & , ast::AstCallMacro *  ),&_FuncbuiltinTickpushTick10769833213962245646_3771763ef4e38f67>>();
    };
    aotLib[0xd46f9fd6e313ce94] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_473f16910ed8ed4c
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, coroutines::YieldFrom const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_473f16910ed8ed4c>>();
    };
    aotLib[0x364618eab5ad87a8] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_e6d28334897e87cd
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, coroutines::CoContinue const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_e6d28334897e87cd>>();
    };
    aotLib[0xe72879fc3be599d0] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_fb3c0bbea8b7355
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, coroutines::CoAwait const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_fb3c0bbea8b7355>>();
    };
    aotLib[0x17cd053e5d46f3ad] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & , ast::AstFunctionAnnotation *  ),&_FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a>>();
    };
    aotLib[0xcd5eed0e44a8a825] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_c46027390bfcc614
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, coroutines::CoroutineMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_c46027390bfcc614>>();
    };
    aotLib[0x6612fc109a1cba8d] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickclone_to_moveTick2007252383599261567_fdd31121648b840f
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, smart_ptr_raw<Expression> const   ),&_FuncbuiltinTickclone_to_moveTick2007252383599261567_fdd31121648b840f>>();
    };
    aotLib[0xa8229fc6887cd60c] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickadd_ptr_refTick918185754185293024_c38be5eeaf10cee3
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, smart_ptr<Expression> const   ),&_FuncbuiltinTickadd_ptr_refTick918185754185293024_c38be5eeaf10cee3>>();
    };
    aotLib[0xcc5016df6cc04729] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTicknextTick17450348357676149856_76c025eb1c6d5401
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, Sequence DAS_COMMENT((bool)) & , bool &  ),&_FuncbuiltinTicknextTick17450348357676149856_76c025eb1c6d5401>>();
    };
    aotLib[0x5771fba2ebbff1d5] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickeraseTick16646986352019611268_20dfa2ac4bec4492
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<Sequence DAS_COMMENT((bool))> & , int32_t  ),&_FuncbuiltinTickeraseTick16646986352019611268_20dfa2ac4bec4492>>();
    };
    aotLib[0xf37536e10865245a] = +[](Context & ctx) -> SimNode* { // cr_run_71b7571079173083
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, Sequence DAS_COMMENT((bool)) &  ),&cr_run_71b7571079173083>>();
    };
    aotLib[0x27967023a6aadf6e] = +[](Context & ctx) -> SimNode* { // cr_run_all_6cfec073aa72fb77
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<Sequence DAS_COMMENT((bool))> &  ),&cr_run_all_6cfec073aa72fb77>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_15530702915337158574
AotListBase impl_aot_coroutines(_anon_15530702915337158574::registerAotFunctions);
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
