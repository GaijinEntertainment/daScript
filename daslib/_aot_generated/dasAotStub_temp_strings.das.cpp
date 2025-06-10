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
 // require temp_strings

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
namespace _anon_10164730010505097190 {

namespace temp_strings { struct TempStringMacro; };
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
namespace temp_strings {

struct TempStringMacro {
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


inline void _FuncbuiltinTickpushTick10769833213962245646_d6b95d70ae428605 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f0a4bf3aabc30763 ( Context * __context__, temp_strings::TempStringMacro const  & __cl_rename_at_116_2 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_7fa191bcf8a56103 ( Context * __context__, char * const  __name_rename_at_631_3, temp_strings::TempStringMacro * __someClassPtr_rename_at_631_4 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_e26daf4f97cf02b ( Context * __context__, char * const  __name_rename_at_240_6, char * const  __tag_rename_at_240_7, temp_strings::TempStringMacro * __classPtr_rename_at_240_8 );
inline void build_temp_string_9a4d0f7f62fb7d38 ( Context * __context__, Block DAS_COMMENT((void,StringBuilderWriter)) const  & __bldr_rename_at_12_10, Block DAS_COMMENT((void,char * const )) const  & __cb_rename_at_12_11 );
inline void temp_string_98c07c0ce9259b69 ( Context * __context__, char * const  __str_rename_at_25_13, Block DAS_COMMENT((void,char * const )) const  & __cb_rename_at_25_14 );
inline void temp_string_fe961e58e98f04d5 ( Context * __context__, TArray<uint8_t> const  & __arr_rename_at_48_15, Block DAS_COMMENT((void,char * const )) const  & __cb_rename_at_48_16 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_d6b95d70ae428605 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f0a4bf3aabc30763 ( Context * __context__, temp_strings::TempStringMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_7fa191bcf8a56103 ( Context * __context__, char * const  __name_rename_at_631_3, temp_strings::TempStringMacro * __someClassPtr_rename_at_631_4 )
{
    _FuncbuiltinTickpushTick10769833213962245646_d6b95d70ae428605(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_631_4));
    StructInfo const  * __classInfo_rename_at_634_5 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_f0a4bf3aabc30763(__context__,das_arg<temp_strings::TempStringMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_631_4))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_631_3,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_631_4),__classInfo_rename_at_634_5,__context__));
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_e26daf4f97cf02b ( Context * __context__, char * const  __name_rename_at_240_6, char * const  __tag_rename_at_240_7, temp_strings::TempStringMacro * __classPtr_rename_at_240_8 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_241_9; memset((void*)&__ann_rename_at_241_9,0,sizeof(__ann_rename_at_241_9));
    /* finally */ auto __finally_240= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_241_9);
    /* end finally */ });
    __ann_rename_at_241_9; das_zero(__ann_rename_at_241_9); das_move(__ann_rename_at_241_9, _FuncastTickmake_function_annotationTick3074191368936885601_7fa191bcf8a56103(__context__,__name_rename_at_240_6,__classPtr_rename_at_240_8));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 0x7678364e276db9c2)),__tag_rename_at_240_7,__ann_rename_at_241_9);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_241_9,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void build_temp_string_9a4d0f7f62fb7d38 ( Context * __context__, Block DAS_COMMENT((void,StringBuilderWriter)) const  &  __bldr_rename_at_12_10, Block DAS_COMMENT((void,char * const )) const  &  __cb_rename_at_12_11 )
{
    char * __str_rename_at_15_12 = (char *)(((char * const )(builtin_build_string_T(__bldr_rename_at_12_10,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_invoke<void>::invoke<char *>(__context__,nullptr,__cb_rename_at_12_11,das_cast<char *>::cast(__str_rename_at_15_12));
    if ( !is_intern_strings(__context__) )
    {
        delete_string(__str_rename_at_15_12,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void temp_string_98c07c0ce9259b69 ( Context * __context__, char * const  __str_rename_at_25_13, Block DAS_COMMENT((void,char * const )) const  &  __cb_rename_at_25_14 )
{
    das_invoke<void>::invoke<char *>(__context__,nullptr,__cb_rename_at_25_14,das_cast<char *>::cast(__str_rename_at_25_13));
    if ( !is_intern_strings(__context__) )
    {
        delete_string(das_cast<char * &>::cast(__str_rename_at_25_13),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void temp_string_fe961e58e98f04d5 ( Context * __context__, TArray<uint8_t> const  &  __arr_rename_at_48_15, Block DAS_COMMENT((void,char * const )) const  &  __cb_rename_at_48_16 )
{
    char * __str_rename_at_51_17 = (char *)(((char * const )(builtin_string_from_array(__arr_rename_at_48_15,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_invoke<void>::invoke<char *>(__context__,nullptr,__cb_rename_at_48_16,das_cast<char *>::cast(__str_rename_at_51_17));
    if ( !is_intern_strings(__context__) )
    {
        delete_string(__str_rename_at_51_17,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xa9f4c8b38b2703bb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_d6b95d70ae428605>>();
    };
    aotLib[0x4c9eca3c6bd3fffd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_f0a4bf3aabc30763>>();
    };
    aotLib[0xbf785d715aa9c1ac] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_function_annotationTick3074191368936885601_7fa191bcf8a56103>>();
    };
    aotLib[0x4647b080cef5c517] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_e26daf4f97cf02b>>();
    };
    aotLib[0xa8a7b05e4482c4f0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&build_temp_string_9a4d0f7f62fb7d38>>();
    };
    aotLib[0xfb951ad7df39bd0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&temp_string_98c07c0ce9259b69>>();
    };
    aotLib[0x5a9540f9c16cc39a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&temp_string_fe961e58e98f04d5>>();
    };
    // [[ init script ]]
    aotLib[0x5464bbe131808aa5] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_10164730010505097190
AotListBase impl_aot_temp_strings(_anon_10164730010505097190::registerAotFunctions);
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
