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
 // require generic_return

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
namespace _anon_6361949300883520420 {

namespace generic_return { struct GenericReturn; };
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
namespace generic_return {

struct GenericReturn {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
}


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1059_0, smart_ptr_raw<Expression> const  __src_rename_at_1059_1 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_1413454fe2b92f68 ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_2 );
inline void _FuncbuiltinTickpushTick10769833213962245646_e608f61b6dfcd4ce ( Context * __context__, TArray<ast::AstCallMacro *> & __Arr_rename_at_181_4, ast::AstCallMacro * __value_rename_at_181_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_fcc707178104eec ( Context * __context__, generic_return::GenericReturn const  & __cl_rename_at_116_6 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_a573a1c727c1b6c ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1744_7 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_9, bool __extra_rename_at_38_10, bool __contracts_rename_at_38_11, bool __modules_rename_at_38_12 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1059_0, smart_ptr_raw<Expression> const  __src_rename_at_1059_1 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1059_0),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1059_1),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_1413454fe2b92f68 ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_2 )
{
    smart_ptr_raw<Expression> __clone_dest_rename_at_1058_3;das_zero(__clone_dest_rename_at_1058_3);
    clone_912660ab1e667e4b(__context__,__clone_dest_rename_at_1058_3,__clone_src_rename_at_1056_2);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__clone_dest_rename_at_1058_3);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e608f61b6dfcd4ce ( Context * __context__, TArray<ast::AstCallMacro *> &  __Arr_rename_at_181_4, ast::AstCallMacro * __value_rename_at_181_5 )
{
    das_copy(__Arr_rename_at_181_4(builtin_array_push_back(das_arg<TArray<ast::AstCallMacro *>>::pass(__Arr_rename_at_181_4),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_5);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_fcc707178104eec ( Context * __context__, generic_return::GenericReturn const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_a573a1c727c1b6c ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1744_7 )
{
    smart_ptr_raw<Expression> __dst_rename_at_1746_8; das_zero(__dst_rename_at_1746_8); das_move(__dst_rename_at_1746_8, _FuncbuiltinTickclone_to_moveTick2007252383599261567_1413454fe2b92f68(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__src_rename_at_1744_7)));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__dst_rename_at_1746_8);
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_9, bool __extra_rename_at_38_10, bool __contracts_rename_at_38_11, bool __modules_rename_at_38_12 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_9,__extra_rename_at_38_10,__contracts_rename_at_38_11,__modules_rename_at_38_12,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x74b4210f5379bdbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_912660ab1e667e4b>>();
    };
    aotLib[0xef0f4e8a845d1461] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_1413454fe2b92f68>>();
    };
    aotLib[0xe553d306a044fc24] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e608f61b6dfcd4ce>>();
    };
    aotLib[0x362d0bf5715e197f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_fcc707178104eec>>();
    };
    aotLib[0xf10cd103de568695] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickadd_ptr_refTick918185754185293024_a573a1c727c1b6c>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_6361949300883520420
AotListBase impl_aot_generic_return(_anon_6361949300883520420::registerAotFunctions);
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
