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
 // require is_local
 // require safe_addr

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
namespace _anon_74461333406690314 {

namespace safe_addr { struct SafeAddrMacro; };
namespace safe_addr { struct SharedAddrMacro; };
namespace safe_addr { struct TempValueMacro; };
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
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
// unused enumeration ConversionResult
// unused enumeration CaptureMode
// unused enumeration SideEffects
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
}
namespace safe_addr {

struct TempValueMacro {
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
}


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e4d068f8c9d808e5 ( Context * __context__, safe_addr::SafeAddrMacro const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a511a317554846b4 ( Context * __context__, safe_addr::SharedAddrMacro const  & __cl_rename_at_116_3 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6d4ea7031e951f7d ( Context * __context__, safe_addr::TempValueMacro const  & __cl_rename_at_116_4 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_4418d6a36c08d1b9 ( Context * __context__, char * const  __name_rename_at_631_5, safe_addr::SafeAddrMacro * __someClassPtr_rename_at_631_6 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_c48f5c2d32f99dfd ( Context * __context__, char * const  __name_rename_at_631_8, safe_addr::SharedAddrMacro * __someClassPtr_rename_at_631_9 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_93921a0e6fa79f2b ( Context * __context__, char * const  __name_rename_at_631_11, safe_addr::TempValueMacro * __someClassPtr_rename_at_631_12 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_47a4aec5bc3e02fc ( Context * __context__, char * const  __name_rename_at_273_14, char * const  __tag_rename_at_273_15, safe_addr::SafeAddrMacro * __classPtr_rename_at_273_16 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_bd8aa2a7ab06a770 ( Context * __context__, char * const  __name_rename_at_273_18, char * const  __tag_rename_at_273_19, safe_addr::SharedAddrMacro * __classPtr_rename_at_273_20 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_80a1209dc7a73916 ( Context * __context__, char * const  __name_rename_at_273_22, char * const  __tag_rename_at_273_23, safe_addr::TempValueMacro * __classPtr_rename_at_273_24 );
inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_26 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e4d068f8c9d808e5 ( Context * __context__, safe_addr::SafeAddrMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a511a317554846b4 ( Context * __context__, safe_addr::SharedAddrMacro const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6d4ea7031e951f7d ( Context * __context__, safe_addr::TempValueMacro const  &  __cl_rename_at_116_4 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_4.__rtti))).getStructType())));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_4418d6a36c08d1b9 ( Context * __context__, char * const  __name_rename_at_631_5, safe_addr::SafeAddrMacro * __someClassPtr_rename_at_631_6 )
{
    _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_631_6));
    StructInfo const  * __classInfo_rename_at_634_7 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_e4d068f8c9d808e5(__context__,das_arg<safe_addr::SafeAddrMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_631_6))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_631_5,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_631_6),__classInfo_rename_at_634_7,__context__));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_c48f5c2d32f99dfd ( Context * __context__, char * const  __name_rename_at_631_8, safe_addr::SharedAddrMacro * __someClassPtr_rename_at_631_9 )
{
    _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_631_9));
    StructInfo const  * __classInfo_rename_at_634_10 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_a511a317554846b4(__context__,das_arg<safe_addr::SharedAddrMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_631_9))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_631_8,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_631_9),__classInfo_rename_at_634_10,__context__));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_93921a0e6fa79f2b ( Context * __context__, char * const  __name_rename_at_631_11, safe_addr::TempValueMacro * __someClassPtr_rename_at_631_12 )
{
    _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_631_12));
    StructInfo const  * __classInfo_rename_at_634_13 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_6d4ea7031e951f7d(__context__,das_arg<safe_addr::TempValueMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_631_12))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_631_11,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_631_12),__classInfo_rename_at_634_13,__context__));
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_47a4aec5bc3e02fc ( Context * __context__, char * const  __name_rename_at_273_14, char * const  __tag_rename_at_273_15, safe_addr::SafeAddrMacro * __classPtr_rename_at_273_16 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_274_17; memset((void*)&__ann_rename_at_274_17,0,sizeof(__ann_rename_at_274_17));
    /* finally */ auto __finally_273= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_274_17);
    /* end finally */ });
    __ann_rename_at_274_17; das_zero(__ann_rename_at_274_17); das_move(__ann_rename_at_274_17, _FuncastTickmake_function_annotationTick3074191368936885601_4418d6a36c08d1b9(__context__,__name_rename_at_273_14,__classPtr_rename_at_273_16));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 0x7678364e276db9c2)),__tag_rename_at_273_15,__ann_rename_at_274_17);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_274_17,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_bd8aa2a7ab06a770 ( Context * __context__, char * const  __name_rename_at_273_18, char * const  __tag_rename_at_273_19, safe_addr::SharedAddrMacro * __classPtr_rename_at_273_20 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_274_21; memset((void*)&__ann_rename_at_274_21,0,sizeof(__ann_rename_at_274_21));
    /* finally */ auto __finally_273= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_274_21);
    /* end finally */ });
    __ann_rename_at_274_21; das_zero(__ann_rename_at_274_21); das_move(__ann_rename_at_274_21, _FuncastTickmake_function_annotationTick3074191368936885601_c48f5c2d32f99dfd(__context__,__name_rename_at_273_18,__classPtr_rename_at_273_20));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 0x7678364e276db9c2)),__tag_rename_at_273_19,__ann_rename_at_274_21);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_274_21,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_80a1209dc7a73916 ( Context * __context__, char * const  __name_rename_at_273_22, char * const  __tag_rename_at_273_23, safe_addr::TempValueMacro * __classPtr_rename_at_273_24 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_274_25; memset((void*)&__ann_rename_at_274_25,0,sizeof(__ann_rename_at_274_25));
    /* finally */ auto __finally_273= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_274_25);
    /* end finally */ });
    __ann_rename_at_274_25; das_zero(__ann_rename_at_274_25); das_move(__ann_rename_at_274_25, _FuncastTickmake_function_annotationTick3074191368936885601_93921a0e6fa79f2b(__context__,__name_rename_at_273_22,__classPtr_rename_at_273_24));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 0x7678364e276db9c2)),__tag_rename_at_273_23,__ann_rename_at_274_25);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_274_25,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_26 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_26,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xd09660c6ccc577ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795>>();
    };
    aotLib[0xc3bfc2a2e6d4e241] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_e4d068f8c9d808e5>>();
    };
    aotLib[0x1e818db192e17aef] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_a511a317554846b4>>();
    };
    aotLib[0x19a2435d0af034e2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_6d4ea7031e951f7d>>();
    };
    aotLib[0x8ee4d783874f461b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_function_annotationTick3074191368936885601_4418d6a36c08d1b9>>();
    };
    aotLib[0xcb995bd93c1034db] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_function_annotationTick3074191368936885601_c48f5c2d32f99dfd>>();
    };
    aotLib[0x7096795ea6bb698] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_function_annotationTick3074191368936885601_93921a0e6fa79f2b>>();
    };
    aotLib[0x5417968edebc0ee7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_47a4aec5bc3e02fc>>();
    };
    aotLib[0x78ea98730bbfa960] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_bd8aa2a7ab06a770>>();
    };
    aotLib[0x4e135c4bc67027eb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_80a1209dc7a73916>>();
    };
    aotLib[0xfbfe800d5700e08f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick842554968825501494_452cfc8955963a18>>();
    };
    // [[ init script ]]
    aotLib[0x5464bbe131808aa5] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_74461333406690314
AotListBase impl_aot_safe_addr(_anon_74461333406690314::registerAotFunctions);
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
