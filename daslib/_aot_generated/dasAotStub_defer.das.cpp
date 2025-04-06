#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require defer
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
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
namespace _anon_1776566935523873409 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
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
// unused enumeration ConversionResult
// unused enumeration CompilationError
// unused enumeration Type
// unused enumeration RefMatters
// unused enumeration ConstMatters
// unused enumeration TemporaryMatters
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
namespace defer {

struct DeferMacro {
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
static_assert(sizeof(DeferMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(DeferMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace defer {

struct DeferDeleteMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
static_assert(sizeof(DeferDeleteMacro)==48,"structure size mismatch with DAS");
static_assert(offsetof(DeferDeleteMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferDeleteMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferDeleteMacro,preVisit)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferDeleteMacro,visit)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferDeleteMacro,canVisitArgument)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(DeferDeleteMacro,canFoldReturnResult)==40,"structure field offset mismatch with DAS");
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2429690714e88b6a ( Context * __context__, defer::DeferMacro const  & __cl_rename_at_116_2 );
inline void _FuncbuiltinTickpushTick10769833213962245646_3771763ef4e38f67 ( Context * __context__, TArray<ast::AstCallMacro *> & __Arr_rename_at_181_3, ast::AstCallMacro * __value_rename_at_181_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8c4f0807d9526548 ( Context * __context__, defer::DeferDeleteMacro const  & __cl_rename_at_116_5 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_bf79c75ba2d9438b ( Context * __context__, char * const  __name_rename_at_622_6, defer::DeferMacro * __someClassPtr_rename_at_622_7 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_48af1a6d5c1cc853 ( Context * __context__, char * const  __name_rename_at_273_9, char * const  __tag_rename_at_273_10, defer::DeferMacro * __classPtr_rename_at_273_11 );
inline void defer_d2a934f049172e6a ( Context * __context__, Block DAS_COMMENT((void)) const  & __blk_rename_at_16_13 );
inline void nada_60bdee73e3315732 ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2429690714e88b6a ( Context * __context__, defer::DeferMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_3771763ef4e38f67 ( Context * __context__, TArray<ast::AstCallMacro *> &  __Arr_rename_at_181_3, ast::AstCallMacro * __value_rename_at_181_4 )
{
    das_copy(__Arr_rename_at_181_3(builtin_array_push_back(das_arg<TArray<ast::AstCallMacro *>>::pass(__Arr_rename_at_181_3),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_4);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8c4f0807d9526548 ( Context * __context__, defer::DeferDeleteMacro const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_bf79c75ba2d9438b ( Context * __context__, char * const  __name_rename_at_622_6, defer::DeferMacro * __someClassPtr_rename_at_622_7 )
{
    _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_622_7));
    StructInfo const  * __classInfo_rename_at_625_8 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_2429690714e88b6a(__context__,das_arg<defer::DeferMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_622_7))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_622_6,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_622_7),__classInfo_rename_at_625_8,__context__));
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_48af1a6d5c1cc853 ( Context * __context__, char * const  __name_rename_at_273_9, char * const  __tag_rename_at_273_10, defer::DeferMacro * __classPtr_rename_at_273_11 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_274_12; memset(&__ann_rename_at_274_12,0,sizeof(__ann_rename_at_274_12));
    /* finally */ auto __finally_273= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_274_12);
    /* end finally */ });
    __ann_rename_at_274_12; das_zero(__ann_rename_at_274_12); das_move(__ann_rename_at_274_12, _FuncastTickmake_function_annotationTick3074191368936885601_bf79c75ba2d9438b(__context__,__name_rename_at_273_9,__classPtr_rename_at_273_11));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 8536632802977626562u)),__tag_rename_at_273_10,__ann_rename_at_274_12);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_274_12,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void defer_d2a934f049172e6a ( Context * __context__, Block DAS_COMMENT((void)) const  &  __blk_rename_at_16_13 )
{
}

inline void nada_60bdee73e3315732 ( Context * __context__ )
{
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x17cd053e5d46f3ad] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & , ast::AstFunctionAnnotation *  ),&_FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a>>();
    };
    aotLib[0x41aca0f12c4d33ec] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_2429690714e88b6a
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, defer::DeferMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_2429690714e88b6a>>();
    };
    aotLib[0xfca27d59ea83e1a5] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_3771763ef4e38f67
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstCallMacro *> & , ast::AstCallMacro *  ),&_FuncbuiltinTickpushTick10769833213962245646_3771763ef4e38f67>>();
    };
    aotLib[0x3a99dbbf8827508c] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_8c4f0807d9526548
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, defer::DeferDeleteMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_8c4f0807d9526548>>();
    };
    aotLib[0xc17fbb0db58a18a] = +[](Context & ctx) -> SimNode* { // _FuncastTickmake_function_annotationTick3074191368936885601_bf79c75ba2d9438b
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<FunctionAnnotation> (*) ( Context * __context__, char * const  , defer::DeferMacro *  ),&_FuncastTickmake_function_annotationTick3074191368936885601_bf79c75ba2d9438b>>();
    };
    aotLib[0xbe2e994ea466ff76] = +[](Context & ctx) -> SimNode* { // _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_48af1a6d5c1cc853
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, char * const  , char * const  , defer::DeferMacro *  ),&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_48af1a6d5c1cc853>>();
    };
    aotLib[0x260a6900d4df9789] = +[](Context & ctx) -> SimNode* { // defer_d2a934f049172e6a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, Block DAS_COMMENT((void)) const  &  ),&defer_d2a934f049172e6a>>();
    };
    aotLib[0x7bfa536a1eb16ea9] = +[](Context & ctx) -> SimNode* { // nada_60bdee73e3315732
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__ ),&nada_60bdee73e3315732>>();
    };
    // [[ init script ]]
    aotLib[0x5464bbe131808aa5] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<SimNode_Aot<void (*)(Context *, bool),&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_1776566935523873409
AotListBase impl_aot_defer(_anon_1776566935523873409::registerAotFunctions);
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
