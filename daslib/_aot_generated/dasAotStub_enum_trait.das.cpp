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
 // require enum_trait

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
namespace _anon_4562643193483458131 {

namespace enum_trait { struct TypeInfoGetEnumLength; };
namespace enum_trait { struct TypeInfoGetEnumNames; };
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
// unused structure AstCallMacro
namespace ast {

struct AstTypeInfoMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstTypeInfoMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstTypeInfoMacro,smart_ptr_raw<ExprTypeInfo> const ,das::string)) getAstChange;
    Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstTypeInfoMacro,ModuleLibrary,smart_ptr_raw<ExprTypeInfo> const ,das::string)) getAstType;
};
}
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
namespace enum_trait {

struct TypeInfoGetEnumLength {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstTypeInfoMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstTypeInfoMacro,smart_ptr_raw<ExprTypeInfo> const ,das::string)) getAstChange;
    Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstTypeInfoMacro,ModuleLibrary,smart_ptr_raw<ExprTypeInfo> const ,das::string)) getAstType;
};
}
namespace enum_trait {

struct TypeInfoGetEnumNames {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstTypeInfoMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstTypeInfoMacro,smart_ptr_raw<ExprTypeInfo> const ,das::string)) getAstChange;
    Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstTypeInfoMacro,ModuleLibrary,smart_ptr_raw<ExprTypeInfo> const ,das::string)) getAstType;
};
}


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline void _FuncbuiltinTickpushTick10769833213962245646_e98940bd647b096c ( Context * __context__, TArray<ast::AstTypeInfoMacro *> & __Arr_rename_at_181_0, ast::AstTypeInfoMacro * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_db89bd2a6ff1ad7e ( Context * __context__, enum_trait::TypeInfoGetEnumLength const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_544f4e3f951187ef ( Context * __context__, enum_trait::TypeInfoGetEnumNames const  & __cl_rename_at_116_3 );
inline range _FuncbuiltinTickiter_rangeTick1065493836214144684_7880c0bbee885dd8 ( Context * __context__, das::vector<Enumeration::EnumEntry> const  & __foo_rename_at_1250_4 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e98940bd647b096c ( Context * __context__, TArray<ast::AstTypeInfoMacro *> &  __Arr_rename_at_181_0, ast::AstTypeInfoMacro * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstTypeInfoMacro *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_db89bd2a6ff1ad7e ( Context * __context__, enum_trait::TypeInfoGetEnumLength const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_544f4e3f951187ef ( Context * __context__, enum_trait::TypeInfoGetEnumNames const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline range _FuncbuiltinTickiter_rangeTick1065493836214144684_7880c0bbee885dd8 ( Context * __context__, das::vector<Enumeration::EnumEntry> const  &  __foo_rename_at_1250_4 )
{
    return das_auto_cast<range>::cast(mk_range(das_vector_length(__foo_rename_at_1250_4)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xf0e3fd00abc15718] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e98940bd647b096c>>();
    };
    aotLib[0x48e5f1e30db38e93] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_db89bd2a6ff1ad7e>>();
    };
    aotLib[0x35cb746aff3af72c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_544f4e3f951187ef>>();
    };
    aotLib[0x2ec7a324528feed3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickiter_rangeTick1065493836214144684_7880c0bbee885dd8>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_4562643193483458131
AotListBase impl_aot_enum_trait(_anon_4562643193483458131::registerAotFunctions);
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
