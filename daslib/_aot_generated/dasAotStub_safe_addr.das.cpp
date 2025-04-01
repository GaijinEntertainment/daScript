#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require safe_addr
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
 // require is_local

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
namespace _anon_6541284229524661428 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

namespace safe_addr { struct SafeAddrMacro; };
namespace safe_addr { struct SharedAddrMacro; };
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
namespace safe_addr {

struct SafeAddrMacro {
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
static_assert(sizeof(SafeAddrMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(SafeAddrMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace safe_addr {

struct SharedAddrMacro {
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
static_assert(sizeof(SharedAddrMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(SharedAddrMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8f037f9c1c601a5d ( Context * __context__, safe_addr::SafeAddrMacro const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7eab4ea751cd84e9 ( Context * __context__, safe_addr::SharedAddrMacro const  & __cl_rename_at_116_3 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_7457f22fe48fd8bd ( Context * __context__, char * const  __name_rename_at_622_4, safe_addr::SafeAddrMacro * __someClassPtr_rename_at_622_5 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_b25e1af0f54a9664 ( Context * __context__, char * const  __name_rename_at_622_7, safe_addr::SharedAddrMacro * __someClassPtr_rename_at_622_8 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_cac749fa4a363860 ( Context * __context__, char * const  __name_rename_at_273_10, char * const  __tag_rename_at_273_11, safe_addr::SafeAddrMacro * __classPtr_rename_at_273_12 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_8af6b8463e730cbf ( Context * __context__, char * const  __name_rename_at_273_14, char * const  __tag_rename_at_273_15, safe_addr::SharedAddrMacro * __classPtr_rename_at_273_16 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8f037f9c1c601a5d ( Context * __context__, safe_addr::SafeAddrMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7eab4ea751cd84e9 ( Context * __context__, safe_addr::SharedAddrMacro const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_7457f22fe48fd8bd ( Context * __context__, char * const  __name_rename_at_622_4, safe_addr::SafeAddrMacro * __someClassPtr_rename_at_622_5 )
{
    _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_622_5));
    StructInfo const  * __classInfo_rename_at_625_6 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_8f037f9c1c601a5d(__context__,das_arg<safe_addr::SafeAddrMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_622_5))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_622_4,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_622_5),__classInfo_rename_at_625_6,__context__));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_b25e1af0f54a9664 ( Context * __context__, char * const  __name_rename_at_622_7, safe_addr::SharedAddrMacro * __someClassPtr_rename_at_622_8 )
{
    _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_622_8));
    StructInfo const  * __classInfo_rename_at_625_9 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_7eab4ea751cd84e9(__context__,das_arg<safe_addr::SharedAddrMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_622_8))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_622_7,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_622_8),__classInfo_rename_at_625_9,__context__));
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_cac749fa4a363860 ( Context * __context__, char * const  __name_rename_at_273_10, char * const  __tag_rename_at_273_11, safe_addr::SafeAddrMacro * __classPtr_rename_at_273_12 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_274_13; memset(&__ann_rename_at_274_13,0,sizeof(__ann_rename_at_274_13));
    /* finally */ auto __finally_273= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_274_13);
    /* end finally */ });
    __ann_rename_at_274_13; das_zero(__ann_rename_at_274_13); das_move(__ann_rename_at_274_13, _FuncastTickmake_function_annotationTick3074191368936885601_7457f22fe48fd8bd(__context__,__name_rename_at_273_10,__classPtr_rename_at_273_12));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 8536632802977626562u)),__tag_rename_at_273_11,__ann_rename_at_274_13);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_274_13,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_8af6b8463e730cbf ( Context * __context__, char * const  __name_rename_at_273_14, char * const  __tag_rename_at_273_15, safe_addr::SharedAddrMacro * __classPtr_rename_at_273_16 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_274_17; memset(&__ann_rename_at_274_17,0,sizeof(__ann_rename_at_274_17));
    /* finally */ auto __finally_273= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_274_17);
    /* end finally */ });
    __ann_rename_at_274_17; das_zero(__ann_rename_at_274_17); das_move(__ann_rename_at_274_17, _FuncastTickmake_function_annotationTick3074191368936885601_b25e1af0f54a9664(__context__,__name_rename_at_273_14,__classPtr_rename_at_273_16));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 8536632802977626562u)),__tag_rename_at_273_15,__ann_rename_at_274_17);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_274_17,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x17cd053e5d46f3ad] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & , ast::AstFunctionAnnotation *  ),&_FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a>>();
    };
    aotLib[0xb5be8b11e33ff868] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_8f037f9c1c601a5d
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, safe_addr::SafeAddrMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_8f037f9c1c601a5d>>();
    };
    aotLib[0x4954c522c9bb5d03] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_7eab4ea751cd84e9
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, safe_addr::SharedAddrMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_7eab4ea751cd84e9>>();
    };
    aotLib[0xdd2a37db07aaa895] = +[](Context & ctx) -> SimNode* { // _FuncastTickmake_function_annotationTick3074191368936885601_7457f22fe48fd8bd
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<FunctionAnnotation> (*) ( Context * __context__, char * const  , safe_addr::SafeAddrMacro *  ),&_FuncastTickmake_function_annotationTick3074191368936885601_7457f22fe48fd8bd>>();
    };
    aotLib[0x57ddc1212130e776] = +[](Context & ctx) -> SimNode* { // _FuncastTickmake_function_annotationTick3074191368936885601_b25e1af0f54a9664
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<FunctionAnnotation> (*) ( Context * __context__, char * const  , safe_addr::SharedAddrMacro *  ),&_FuncastTickmake_function_annotationTick3074191368936885601_b25e1af0f54a9664>>();
    };
    aotLib[0x200c365867f2c975] = +[](Context & ctx) -> SimNode* { // _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_cac749fa4a363860
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, char * const  , char * const  , safe_addr::SafeAddrMacro *  ),&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_cac749fa4a363860>>();
    };
    aotLib[0xdd8d05867540de26] = +[](Context & ctx) -> SimNode* { // _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_8af6b8463e730cbf
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, char * const  , char * const  , safe_addr::SharedAddrMacro *  ),&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_8af6b8463e730cbf>>();
    };
    // [[ init script ]]
    aotLib[0x5464bbe131808aa5] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<SimNode_Aot<void (*)(Context *, bool),&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_6541284229524661428
AotListBase impl_aot_safe_addr(_anon_6541284229524661428::registerAotFunctions);
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
