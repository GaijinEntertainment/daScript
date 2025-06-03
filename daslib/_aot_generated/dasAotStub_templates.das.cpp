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
namespace _anon_250365952365424398 {

namespace templates { struct DecltypeMacro; };
namespace templates { struct DecltypeNoRefMacro; };
namespace templates { struct TemplateMacro; };
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
namespace templates {

struct DecltypeMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
    bool remove_ref;
};
}
namespace templates {

struct DecltypeNoRefMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
    bool remove_ref;
};
}
namespace templates {

struct TemplateMacro {
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


inline void _FuncbuiltinTickpushTick10769833213962245646_e608f61b6dfcd4ce ( Context * __context__, TArray<ast::AstCallMacro *> & __Arr_rename_at_181_0, ast::AstCallMacro * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f86a5e7a2e26b7aa ( Context * __context__, templates::DecltypeMacro const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8fc3e281cd5d4d47 ( Context * __context__, templates::DecltypeNoRefMacro const  & __cl_rename_at_116_3 );
inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_4, ast::AstFunctionAnnotation * __value_rename_at_181_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_249decbf87fecb03 ( Context * __context__, templates::TemplateMacro const  & __cl_rename_at_116_6 );
inline void clone_1eed983bd4841917 ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_1059_7, smart_ptr_raw<Function> const  __src_rename_at_1059_8 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_7a593ef35695477e ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_9 );
inline smart_ptr_raw<Function> _FuncbuiltinTickclone_to_moveTick2007252383599261567_2d559201c7991b3d ( Context * __context__, smart_ptr_raw<Function> const  __clone_src_rename_at_1056_11 );
inline void clone_a842a9854f1fbc85 ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_92_13, smart_ptr_raw<Expression> const  __src_rename_at_92_14 );
inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_15 );
inline int32_t _FuncbuiltinTickfind_index_ifTick4436313710217261332_6d4d92e49ed1dae3 ( Context * __context__, Sequence DAS_COMMENT((smart_ptr_raw<AnnotationDeclaration> &)) & __arr_rename_at_1694_16, Block DAS_COMMENT((bool,smart_ptr_raw<AnnotationDeclaration> const )) const  & __blk_rename_at_1694_17 );
inline int32_t _FuncbuiltinTickfind_index_ifTick4436313710217261332_4b8aca0798d52ea ( Context * __context__, Sequence DAS_COMMENT((smart_ptr_raw<Variable> &)) & __arr_rename_at_1694_22, Block DAS_COMMENT((bool,smart_ptr_raw<Variable> const )) const  & __blk_rename_at_1694_23 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e608f61b6dfcd4ce ( Context * __context__, TArray<ast::AstCallMacro *> &  __Arr_rename_at_181_0, ast::AstCallMacro * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstCallMacro *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f86a5e7a2e26b7aa ( Context * __context__, templates::DecltypeMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8fc3e281cd5d4d47 ( Context * __context__, templates::DecltypeNoRefMacro const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_4, ast::AstFunctionAnnotation * __value_rename_at_181_5 )
{
    das_copy(__Arr_rename_at_181_4(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_4),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_5);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_249decbf87fecb03 ( Context * __context__, templates::TemplateMacro const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline void clone_1eed983bd4841917 ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_1059_7, smart_ptr_raw<Function> const  __src_rename_at_1059_8 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1059_7),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1059_8),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_7a593ef35695477e ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_9 )
{
    smart_ptr_raw<Expression> __clone_dest_rename_at_1058_10;das_zero(__clone_dest_rename_at_1058_10);
    clone_a842a9854f1fbc85(__context__,__clone_dest_rename_at_1058_10,__clone_src_rename_at_1056_9);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__clone_dest_rename_at_1058_10);
}

inline smart_ptr_raw<Function> _FuncbuiltinTickclone_to_moveTick2007252383599261567_2d559201c7991b3d ( Context * __context__, smart_ptr_raw<Function> const  __clone_src_rename_at_1056_11 )
{
    smart_ptr_raw<Function> __clone_dest_rename_at_1058_12;das_zero(__clone_dest_rename_at_1058_12);
    clone_1eed983bd4841917(__context__,__clone_dest_rename_at_1058_12,__clone_src_rename_at_1056_11);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__clone_dest_rename_at_1058_12);
}

inline void clone_a842a9854f1fbc85 ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_92_13, smart_ptr_raw<Expression> const  __src_rename_at_92_14 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_92_13),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_92_14),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_15 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_15,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline int32_t _FuncbuiltinTickfind_index_ifTick4436313710217261332_6d4d92e49ed1dae3 ( Context * __context__, Sequence DAS_COMMENT((smart_ptr_raw<AnnotationDeclaration> &)) &  __arr_rename_at_1694_16, Block DAS_COMMENT((bool,smart_ptr_raw<AnnotationDeclaration> const )) const  &  __blk_rename_at_1694_17 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1695_22_32; _temp_make_local_1695_22_32;
    {
        bool __need_loop_1695 = true;
        // o: smart_ptr<rtti::AnnotationDeclaration> aka TT&
        das_iterator<Sequence DAS_COMMENT((smart_ptr_raw<AnnotationDeclaration>))> __o_iterator(__arr_rename_at_1694_16);
        smart_ptr_raw<AnnotationDeclaration> * __o_rename_at_1695_20;
        __need_loop_1695 = __o_iterator.first(__context__,(__o_rename_at_1695_20)) && __need_loop_1695;
        // i: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1695_22_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
        int32_t __i_rename_at_1695_21;
        __need_loop_1695 = __i_iterator.first(__context__,(__i_rename_at_1695_21)) && __need_loop_1695;
        for ( ; __need_loop_1695 ; __need_loop_1695 = __o_iterator.next(__context__,(__o_rename_at_1695_20)) && __i_iterator.next(__context__,(__i_rename_at_1695_21)) )
        {
            if ( das_invoke<bool>::invoke<smart_ptr_raw<AnnotationDeclaration>>(__context__,nullptr,__blk_rename_at_1694_17,(*__o_rename_at_1695_20)) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1695_21);
            };
        }
        __o_iterator.close(__context__,(__o_rename_at_1695_20));
        __i_iterator.close(__context__,(__i_rename_at_1695_21));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline int32_t _FuncbuiltinTickfind_index_ifTick4436313710217261332_4b8aca0798d52ea ( Context * __context__, Sequence DAS_COMMENT((smart_ptr_raw<Variable> &)) &  __arr_rename_at_1694_22, Block DAS_COMMENT((bool,smart_ptr_raw<Variable> const )) const  &  __blk_rename_at_1694_23 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1695_22_32; _temp_make_local_1695_22_32;
    {
        bool __need_loop_1695 = true;
        // o: smart_ptr<ast::Variable> aka TT&
        das_iterator<Sequence DAS_COMMENT((smart_ptr_raw<Variable>))> __o_iterator(__arr_rename_at_1694_22);
        smart_ptr_raw<Variable> * __o_rename_at_1695_26;
        __need_loop_1695 = __o_iterator.first(__context__,(__o_rename_at_1695_26)) && __need_loop_1695;
        // i: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1695_22_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
        int32_t __i_rename_at_1695_27;
        __need_loop_1695 = __i_iterator.first(__context__,(__i_rename_at_1695_27)) && __need_loop_1695;
        for ( ; __need_loop_1695 ; __need_loop_1695 = __o_iterator.next(__context__,(__o_rename_at_1695_26)) && __i_iterator.next(__context__,(__i_rename_at_1695_27)) )
        {
            if ( das_invoke<bool>::invoke<smart_ptr_raw<Variable>>(__context__,nullptr,__blk_rename_at_1694_23,(*__o_rename_at_1695_26)) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1695_27);
            };
        }
        __o_iterator.close(__context__,(__o_rename_at_1695_26));
        __i_iterator.close(__context__,(__i_rename_at_1695_27));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xe553d306a044fc24] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e608f61b6dfcd4ce>>();
    };
    aotLib[0x612a7d06202e6804] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_f86a5e7a2e26b7aa>>();
    };
    aotLib[0x75bcb34e530686ea] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_8fc3e281cd5d4d47>>();
    };
    aotLib[0xd09660c6ccc577ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795>>();
    };
    aotLib[0x5897e6e2667df721] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_249decbf87fecb03>>();
    };
    aotLib[0x4cfffbead2f23e62] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_1eed983bd4841917>>();
    };
    aotLib[0x6adada27a198f519] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_7a593ef35695477e>>();
    };
    aotLib[0xaf21198ba11ce18b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_2d559201c7991b3d>>();
    };
    aotLib[0xc105bc1c43b6486b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_a842a9854f1fbc85>>();
    };
    aotLib[0xfbfe800d5700e08f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick842554968825501494_452cfc8955963a18>>();
    };
    aotLib[0x1d9ea3885ecf850] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_index_ifTick4436313710217261332_6d4d92e49ed1dae3>>();
    };
    aotLib[0x606ceac33cc83caf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_index_ifTick4436313710217261332_4b8aca0798d52ea>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_250365952365424398
AotListBase impl_aot_templates(_anon_250365952365424398::registerAotFunctions);
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
