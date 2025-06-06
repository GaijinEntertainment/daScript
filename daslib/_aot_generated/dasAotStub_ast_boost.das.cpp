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
namespace _anon_24078389025493969 {

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
namespace ast {

struct AstVariantMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
}
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
namespace ast_boost {

struct MacroMacro {
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
namespace ast_boost {

struct TagFunctionAnnotation {
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
namespace ast_boost {

struct TagStructureAnnotation {
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
namespace ast_boost {

struct SetupAnyAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupFunctionAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupBlockAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupStructureAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupEnumerationAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupContractAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupReaderMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupCommentReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupVariantMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupForLoopMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupCaptureMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupTypeMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupSimulateMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupCallMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupTypeInfoMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupInferMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupDirtyInferMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupLintMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupGlobalLintMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct SetupOptimizationMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
}
namespace ast_boost {

struct TagFunctionMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
    char * tag;
};
}
namespace ast_boost {

struct BetterRttiVisitor {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
}
extern TypeInfo __type_info__6b1c7db4b71a781f;
extern TypeInfo __type_info__af63a74c8601927d;
extern TypeInfo __type_info__3c61146b2bdfb90;
extern TypeInfo __type_info__4fd1011fade7876f;
extern TypeInfo __type_info__fa593d0882a72913;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__8d8d8008262e16ec;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af96fe4c8658cf52;
extern TypeInfo __type_info__b68d800849332aec;
extern TypeInfo __type_info__a57bf935c2dd03;
extern TypeInfo __type_info__ce241e3005cc873b;
extern TypeInfo __type_info__8afce1a80940fc9e;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__38be04c990d4b416;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63d94c8601e773;
extern TypeInfo __type_info__af63db4c8601ead9;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__d922fe078cefab30;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63e84c860200f0;
extern TypeInfo __type_info__d9307e078cfb0f0c;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__1b2f537dda20b669;
extern VarInfo __var_info__4cf955bf86f60534;
extern VarInfo __var_info__ec89c0a8da510f28;
extern VarInfo __var_info__ec83c0a8da46dd28;
extern VarInfo __var_info__c9a6e7cc498a17c9;
extern VarInfo __var_info__2a896b18ac8c5dc;
extern VarInfo __var_info__c3630b856e2c8315;
extern VarInfo __var_info__ceb2c4cde2c49d3a;
extern VarInfo __var_info__447579d184dfd9f7;
extern VarInfo __var_info__8603fbcef5fdd683;
extern VarInfo __var_info__5ac39c9b48fed41d;
extern VarInfo __var_info__5671bd5411275d07;
extern FuncInfo __func_info__f381b060f4dbce7;
extern FuncInfo __func_info__3b2c4edb933bef4a;
extern FuncInfo __func_info__806bd08fbfbd09f4;
extern FuncInfo __func_info__8075d08fbfce07f4;
extern FuncInfo __func_info__4dedd3fc50b439aa;
extern FuncInfo __func_info__762f57a17c2dce28;
extern FuncInfo __func_info__b50c88059cb1bcc;
extern FuncInfo __func_info__b28b606ddb7a6760;
extern EnumInfo __enum_info__c897fe55afe7f727;

EnumValueInfo __enum_info__c897fe55afe7f727_value_0 = { "none", 0 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_1 = { "autoinfer", 1 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_2 = { "alias", 2 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_3 = { "option", 3 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_4 = { "typeDecl", 4 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_5 = { "typeMacro", 5 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_6 = { "fakeContext", 6 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_7 = { "fakeLineInfo", 7 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_8 = { "anyArgument", 8 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_9 = { "tVoid", 9 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_10 = { "tBool", 10 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_11 = { "tInt8", 11 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_12 = { "tUInt8", 12 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_13 = { "tInt16", 13 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_14 = { "tUInt16", 14 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_15 = { "tInt64", 15 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_16 = { "tUInt64", 16 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_17 = { "tInt", 17 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_18 = { "tInt2", 18 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_19 = { "tInt3", 19 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_20 = { "tInt4", 20 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_21 = { "tUInt", 21 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_22 = { "tUInt2", 22 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_23 = { "tUInt3", 23 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_24 = { "tUInt4", 24 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_25 = { "tFloat", 25 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_26 = { "tFloat2", 26 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_27 = { "tFloat3", 27 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_28 = { "tFloat4", 28 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_29 = { "tDouble", 29 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_30 = { "tRange", 30 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_31 = { "tURange", 31 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_32 = { "tRange64", 32 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_33 = { "tURange64", 33 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_34 = { "tString", 34 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_35 = { "tStructure", 35 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_36 = { "tHandle", 36 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_37 = { "tEnumeration", 37 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_38 = { "tEnumeration8", 38 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_39 = { "tEnumeration16", 39 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_40 = { "tEnumeration64", 40 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_41 = { "tBitfield", 41 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_42 = { "tPointer", 42 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_43 = { "tFunction", 43 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_44 = { "tLambda", 44 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_45 = { "tIterator", 45 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_46 = { "tArray", 46 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_47 = { "tTable", 47 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_48 = { "tBlock", 48 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_49 = { "tTuple", 49 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_50 = { "tVariant", 50 };
EnumValueInfo * __enum_info__c897fe55afe7f727_values [] = { &__enum_info__c897fe55afe7f727_value_0, &__enum_info__c897fe55afe7f727_value_1, &__enum_info__c897fe55afe7f727_value_2, &__enum_info__c897fe55afe7f727_value_3, &__enum_info__c897fe55afe7f727_value_4, &__enum_info__c897fe55afe7f727_value_5, &__enum_info__c897fe55afe7f727_value_6, &__enum_info__c897fe55afe7f727_value_7, &__enum_info__c897fe55afe7f727_value_8, &__enum_info__c897fe55afe7f727_value_9, &__enum_info__c897fe55afe7f727_value_10, &__enum_info__c897fe55afe7f727_value_11, &__enum_info__c897fe55afe7f727_value_12, &__enum_info__c897fe55afe7f727_value_13, &__enum_info__c897fe55afe7f727_value_14, &__enum_info__c897fe55afe7f727_value_15, &__enum_info__c897fe55afe7f727_value_16, &__enum_info__c897fe55afe7f727_value_17, &__enum_info__c897fe55afe7f727_value_18, &__enum_info__c897fe55afe7f727_value_19, &__enum_info__c897fe55afe7f727_value_20, &__enum_info__c897fe55afe7f727_value_21, &__enum_info__c897fe55afe7f727_value_22, &__enum_info__c897fe55afe7f727_value_23, &__enum_info__c897fe55afe7f727_value_24, &__enum_info__c897fe55afe7f727_value_25, &__enum_info__c897fe55afe7f727_value_26, &__enum_info__c897fe55afe7f727_value_27, &__enum_info__c897fe55afe7f727_value_28, &__enum_info__c897fe55afe7f727_value_29, &__enum_info__c897fe55afe7f727_value_30, &__enum_info__c897fe55afe7f727_value_31, &__enum_info__c897fe55afe7f727_value_32, &__enum_info__c897fe55afe7f727_value_33, &__enum_info__c897fe55afe7f727_value_34, &__enum_info__c897fe55afe7f727_value_35, &__enum_info__c897fe55afe7f727_value_36, &__enum_info__c897fe55afe7f727_value_37, &__enum_info__c897fe55afe7f727_value_38, &__enum_info__c897fe55afe7f727_value_39, &__enum_info__c897fe55afe7f727_value_40, &__enum_info__c897fe55afe7f727_value_41, &__enum_info__c897fe55afe7f727_value_42, &__enum_info__c897fe55afe7f727_value_43, &__enum_info__c897fe55afe7f727_value_44, &__enum_info__c897fe55afe7f727_value_45, &__enum_info__c897fe55afe7f727_value_46, &__enum_info__c897fe55afe7f727_value_47, &__enum_info__c897fe55afe7f727_value_48, &__enum_info__c897fe55afe7f727_value_49, &__enum_info__c897fe55afe7f727_value_50 };
EnumInfo __enum_info__c897fe55afe7f727 = { "Type", "rtti", __enum_info__c897fe55afe7f727_values, 51, UINT64_C(0xc897fe55afe7f727) };
VarInfo __func_info__f381b060f4dbce7_field_0 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Annotation"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8226, TypeSize<Annotation>::size, UINT64_C(0x5ac39c9b48fed41d), "value", 0, 0 };
VarInfo * __func_info__f381b060f4dbce7_fields[1] =  { &__func_info__f381b060f4dbce7_field_0 };
FuncInfo __func_info__f381b060f4dbce7 = {"invoke block<(value:rtti::Annotation const):void> const", "", __func_info__f381b060f4dbce7_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xf381b060f4dbce7), 0x0 };
VarInfo __func_info__3b2c4edb933bef4a_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<Function>>::size, UINT64_C(0x4cf955bf86f60534), "fn", 0, 0 };
VarInfo * __func_info__3b2c4edb933bef4a_fields[1] =  { &__func_info__3b2c4edb933bef4a_field_0 };
FuncInfo __func_info__3b2c4edb933bef4a = {"invoke block<(var fn:smart_ptr<ast::Function>):void> const", "", __func_info__3b2c4edb933bef4a_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x3b2c4edb933bef4a), 0x0 };
VarInfo __func_info__806bd08fbfbd09f4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<Function>>::size, UINT64_C(0xec89c0a8da510f28), "func", 0, 0 };
VarInfo * __func_info__806bd08fbfbd09f4_fields[1] =  { &__func_info__806bd08fbfbd09f4_field_0 };
FuncInfo __func_info__806bd08fbfbd09f4 = {"invoke block<(var func:smart_ptr<ast::Function>):void> const", "", __func_info__806bd08fbfbd09f4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x806bd08fbfbd09f4), 0x0 };
VarInfo __func_info__8075d08fbfce07f4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, TypeSize<smart_ptr_raw<Function>>::size, UINT64_C(0xec83c0a8da46dd28), "func", 0, 0 };
VarInfo * __func_info__8075d08fbfce07f4_fields[1] =  { &__func_info__8075d08fbfce07f4_field_0 };
FuncInfo __func_info__8075d08fbfce07f4 = {"invoke block<(var func:smart_ptr<ast::Function>):void> const", "", __func_info__8075d08fbfce07f4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8075d08fbfce07f4), 0x0 };
VarInfo __func_info__4dedd3fc50b439aa_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8afce1a80940fc9e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<Module *>::size, UINT64_C(0xc9a6e7cc498a17c9), "mod", 0, 0 };
VarInfo * __func_info__4dedd3fc50b439aa_fields[1] =  { &__func_info__4dedd3fc50b439aa_field_0 };
FuncInfo __func_info__4dedd3fc50b439aa = {"invoke block<(var mod:rtti::Module?):void> const", "", __func_info__4dedd3fc50b439aa_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x4dedd3fc50b439aa), 0x0 };
VarInfo __func_info__762f57a17c2dce28_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x2a896b18ac8c5dc), "name", 0, 0 };
VarInfo __func_info__762f57a17c2dce28_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x1b2f537dda20b669), "cppName", 0, 0 };
VarInfo __func_info__762f57a17c2dce28_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ce241e3005cc873b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<TypeDecl>>::size, UINT64_C(0x5671bd5411275d07), "xtype", 0, 0 };
VarInfo __func_info__762f57a17c2dce28_field_3 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint32_t>::size, UINT64_C(0xc3630b856e2c8315), "offset", 0, 0 };
VarInfo * __func_info__762f57a17c2dce28_fields[4] =  { &__func_info__762f57a17c2dce28_field_0, &__func_info__762f57a17c2dce28_field_1, &__func_info__762f57a17c2dce28_field_2, &__func_info__762f57a17c2dce28_field_3 };
FuncInfo __func_info__762f57a17c2dce28 = {"invoke block<(var name:string;var cppName:string;var xtype:smart_ptr<ast::TypeDecl>;var offset:uint):void> const", "", __func_info__762f57a17c2dce28_fields, 4, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x762f57a17c2dce28), 0x0 };
VarInfo __func_info__b50c88059cb1bcc_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0xceb2c4cde2c49d3a), "pkey", 0, 0 };
VarInfo __func_info__b50c88059cb1bcc_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0x447579d184dfd9f7), "pvalue", 0, 0 };
VarInfo * __func_info__b50c88059cb1bcc_fields[2] =  { &__func_info__b50c88059cb1bcc_field_0, &__func_info__b50c88059cb1bcc_field_1 };
FuncInfo __func_info__b50c88059cb1bcc = {"invoke block<(var pkey:void?;var pvalue:void?):void> const", "", __func_info__b50c88059cb1bcc_fields, 2, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xb50c88059cb1bcc), 0x0 };
VarInfo __func_info__b28b606ddb7a6760_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0x8603fbcef5fdd683), "value", 0, 0 };
VarInfo * __func_info__b28b606ddb7a6760_fields[1] =  { &__func_info__b28b606ddb7a6760_field_0 };
FuncInfo __func_info__b28b606ddb7a6760 = {"invoke block<(var value:void?):void> const", "", __func_info__b28b606ddb7a6760_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xb28b606ddb7a6760), 0x0 };
TypeInfo __type_info__6b1c7db4b71a781f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 258, TypeSize<das::string>::size, UINT64_C(0x6b1c7db4b71a781f) };
TypeInfo __type_info__af63a74c8601927d = { Type::anyArgument, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<vec4f>::size, UINT64_C(0xaf63a74c8601927d) };
TypeInfo __type_info__3c61146b2bdfb90 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 290, TypeSize<das::string>::size, UINT64_C(0x3c61146b2bdfb90) };
TypeInfo __type_info__4fd1011fade7876f = { Type::tEnumeration, nullptr, &__enum_info__c897fe55afe7f727, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<DAS_COMMENT(bound_enum) das::Type>::size, UINT64_C(0x4fd1011fade7876f) };
TypeInfo __type_info__fa593d0882a72913 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16548, TypeSize<char *>::size, UINT64_C(0xfa593d0882a72913) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<bool>::size, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int32_t>::size, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int64_t>::size, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af96fe4c8658cf52 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<uint32_t>::size, UINT64_C(0xaf96fe4c8658cf52) };
TypeInfo __type_info__b68d800849332aec = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<uint64_t>::size, UINT64_C(0xb68d800849332aec) };
TypeInfo __type_info__a57bf935c2dd03 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Function"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<Function>::size, UINT64_C(0xa57bf935c2dd03) };
TypeInfo __type_info__ce241e3005cc873b = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::TypeDecl"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<TypeDecl>::size, UINT64_C(0xce241e3005cc873b) };
TypeInfo __type_info__8afce1a80940fc9e = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Module"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<Module>::size, UINT64_C(0x8afce1a80940fc9e) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<StringBuilderWriter>::size, UINT64_C(0x37d36026a6078a42) };
TypeInfo * __type_info__38be04c990d4b416_arg_types[9] = { &__type_info__af63df4c8601f1a5, &__type_info__af63e44c8601fa24, &__type_info__af63e84c860200f0, &__type_info__d922fe078cefab30, &__type_info__d9307e078cfb0f0c, &__type_info__af63db4c8601ead9, &__type_info__af63d94c8601e773, &__type_info__af63ee4c86020b22, &__type_info__af63a74c8601927d };
const char * __type_info__38be04c990d4b416_arg_names[9] = { "tBool", "tInt", "tUInt", "tInt64", "tUInt64", "tFloat", "tDouble", "tString", "nothing" };
TypeInfo __type_info__38be04c990d4b416 = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__38be04c990d4b416_arg_types, __type_info__38be04c990d4b416_arg_names, 9, 0, nullptr, 16390, TypeSize<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::size, UINT64_C(0x38be04c990d4b416) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63d94c8601e773 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<double>::size, UINT64_C(0xaf63d94c8601e773) };
TypeInfo __type_info__af63db4c8601ead9 = { Type::tFloat, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<float>::size, UINT64_C(0xaf63db4c8601ead9) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__d922fe078cefab30 = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int64_t>::size, UINT64_C(0xd922fe078cefab30) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63e84c860200f0 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint32_t>::size, UINT64_C(0xaf63e84c860200f0) };
TypeInfo __type_info__d9307e078cfb0f0c = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0xd9307e078cfb0f0c) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<void>::size, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__6b1c7db4b71a781f, __type_info__3c61146b2bdfb90, __type_info__a57bf935c2dd03, __type_info__ce241e3005cc873b, __type_info__8afce1a80940fc9e, __type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[2] = { &__type_info__37d36026a6078a42, &__type_info__fa593d0882a72913 };
TypeInfo * __tinfo_2[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_3[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__38be04c990d4b416 };
TypeInfo * __tinfo_4[4] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_5[1] = { &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_6[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_7[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_8[1] = { &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_9[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[2] = { &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f };
TypeInfo * __tinfo_11[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_12[2] = { &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f };
TypeInfo * __tinfo_13[2] = { &__type_info__af90fe4c864e9d52, &__type_info__4fd1011fade7876f };
TypeInfo * __tinfo_14[4] = { &__type_info__af90fe4c864e9d52, &__type_info__d922fe078cefab30, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_15[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_16[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_17[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_18[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_19[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_20[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_22[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_23[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_24[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_25[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_26[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_27[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_28[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_29[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_30[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_31[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_32[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_33[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_34[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec };
TypeInfo * __tinfo_35[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_36[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__b68d800849332aec };
TypeInfo * __tinfo_37[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_38[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af81fe4c86352052 };
TypeInfo * __tinfo_39[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_40[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_41[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_42[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_43[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_44[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_45[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_46[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_47[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_48[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_49[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_50[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_51[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_52[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_53[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_54[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };

inline void _FuncbuiltinTickpushTick10769833213962245646_577e5c7310ce42 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b52298649fd5c67b ( Context * __context__, ast_boost::MacroMacro const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6114197bfba716e9 ( Context * __context__, ast_boost::TagFunctionAnnotation const  & __cl_rename_at_116_3 );
inline void _FuncbuiltinTickpushTick10769833213962245646_d8d4a649436ee872 ( Context * __context__, TArray<ast::AstStructureAnnotation *> & __Arr_rename_at_181_4, ast::AstStructureAnnotation * __value_rename_at_181_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3dc324a94b22dc94 ( Context * __context__, ast_boost::SetupFunctionAnnotation const  & __cl_rename_at_116_6 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_ada54212b7f744a6 ( Context * __context__, ast_boost::SetupBlockAnnotation const  & __cl_rename_at_116_7 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d5f5b99ec073834e ( Context * __context__, ast_boost::SetupStructureAnnotation const  & __cl_rename_at_116_8 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bf8bd9eebb3a2d23 ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  & __cl_rename_at_116_9 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7e21e784fc551687 ( Context * __context__, ast_boost::SetupContractAnnotation const  & __cl_rename_at_116_10 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_15d783f7e9c7ea7a ( Context * __context__, ast_boost::SetupReaderMacro const  & __cl_rename_at_116_11 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9300fad43e61790a ( Context * __context__, ast_boost::SetupCommentReader const  & __cl_rename_at_116_12 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bda902199373ea0 ( Context * __context__, ast_boost::SetupCallMacro const  & __cl_rename_at_116_13 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_170bf2ffb5736ec4 ( Context * __context__, ast_boost::SetupTypeInfoMacro const  & __cl_rename_at_116_14 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b8586b2d29046ffa ( Context * __context__, ast_boost::SetupVariantMacro const  & __cl_rename_at_116_15 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_80c16e0c2632db9b ( Context * __context__, ast_boost::SetupForLoopMacro const  & __cl_rename_at_116_16 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6b4b3f0ecc112b40 ( Context * __context__, ast_boost::SetupCaptureMacro const  & __cl_rename_at_116_17 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_101e4eb21b625567 ( Context * __context__, ast_boost::SetupTypeMacro const  & __cl_rename_at_116_18 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_591efe2bffa234e ( Context * __context__, ast_boost::SetupSimulateMacro const  & __cl_rename_at_116_19 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_73d636384df024a7 ( Context * __context__, ast_boost::TagStructureAnnotation const  & __cl_rename_at_116_20 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_aec7f4795a71d05e ( Context * __context__, ast_boost::TagFunctionMacro const  & __cl_rename_at_116_21 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4f45e677a4b772d8 ( Context * __context__, ast_boost::SetupInferMacro const  & __cl_rename_at_116_22 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7a6a7aa50553599a ( Context * __context__, ast_boost::SetupDirtyInferMacro const  & __cl_rename_at_116_23 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7edb518827a4cdad ( Context * __context__, ast_boost::SetupOptimizationMacro const  & __cl_rename_at_116_24 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_dc816cef426a7f26 ( Context * __context__, ast_boost::SetupLintMacro const  & __cl_rename_at_116_25 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5355aa25e23f93e6 ( Context * __context__, ast_boost::SetupGlobalLintMacro const  & __cl_rename_at_116_26 );
inline void _FuncbuiltinTickpushTick10769833213962245646_bea46e5118b8a97f ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_181_27, ast::AstVariantMacro * __value_rename_at_181_28 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_585b805c28f303bc ( Context * __context__, ast_boost::BetterRttiVisitor const  & __cl_rename_at_116_29 );
inline void clone_a8a561a0ef667b7 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & __dest_rename_at_1092_30, smart_ptr_raw<FunctionAnnotation> const  __src_rename_at_1092_31 );
inline smart_ptr_raw<FunctionAnnotation> _FuncbuiltinTickclone_to_moveTick2007252383599261567_a3509efc10f05106 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> const  __clone_src_rename_at_1089_32 );
inline Module * _FuncastTickfind_moduleTick11101329256228773934_d842461399a3c7ba ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_948_34, char * const  __name_rename_at_948_35 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c1a84a72f7e61d2c ( Context * __context__, TArray<char *> & __a_rename_at_1215_38 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_976f9f8d3c51340f ( Context * __context__, TArray<smart_ptr_raw<TypeDecl>> & __a_rename_at_1215_39 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_70f04f60487df041 ( Context * __context__, TArray<smart_ptr_raw<Variable>> & __a_rename_at_1215_41 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_9f199f97decbabbe ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __a_rename_at_1215_43 );
inline void _FuncbuiltinTickpushTick10769833213962245646_1445ed0430a8bb9b ( Context * __context__, TArray<char *> & __Arr_rename_at_181_45, char * __value_rename_at_181_46 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_daeec2492818d4d1 ( Context * __context__, TArray<char *> const  & __it_rename_at_22_47, char * const  __separator_rename_at_22_48 );
inline void _FuncbuiltinTickpushTick14133213201864676143_c66cc804629e0825 ( Context * __context__, TArray<char *> & __Arr_rename_at_165_53, char * const  __value_rename_at_165_54 );
inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_18f7d5397c9c7e3 ( Context * __context__, TArray<smart_ptr_raw<Function>> & __Arr_rename_at_377_55, smart_ptr_raw<Function> __value_rename_at_377_56 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_7cde73d852192941 ( Context * __context__, TArray<smart_ptr_raw<Function>> & __a_rename_at_1215_57 );
inline void clone_f2eaefba27105789 ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_283_59, smart_ptr_raw<Function> const  __src_rename_at_283_60 );
inline char * _FuncastTickdescribeTick2562845734617055679_b1fd376d21aa823f ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_61, bool __extra_rename_at_38_62, bool __contracts_rename_at_38_63, bool __modules_rename_at_38_64 );
inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_130b6b8d46be3007 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1759_65 );
inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_4da357be30f10f8d ( Context * __context__ );
inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_1836be9fc5e8fc93 ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1765_67 );
inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_899221f202686a02 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1765_68 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_7477174a10d67388 ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_515_69, smart_ptr_raw<Expression> __enull_rename_at_515_70 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_e3597b8badb6cad2 ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_515_88, smart_ptr_raw<Expression> __enull_rename_at_515_89 );
inline Annotation * _FuncbuiltinTickget_ptrTick8468476673553620226_7e735142241eeece ( Context * __context__, smart_ptr_raw<Annotation> const  __src_rename_at_1759_107 );
inline void clone_46d70cd8acd38c11 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_675_108, smart_ptr_raw<TypeDecl> const  __src_rename_at_675_109 );
inline void clone_692a562a355a89d ( Context * __context__, smart_ptr_raw<Enumeration> & __dest_rename_at_827_110, Enumeration * const  __src_rename_at_827_111 );
inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_bd462bbf8da5b2bf ( Context * __context__, char * const  __name_rename_at_967_112 );
inline void clone_f708d7eb74451775 ( Context * __context__, smart_ptr_raw<Annotation> & __dest_rename_at_906_113, smart_ptr_raw<Annotation> const  __src_rename_at_906_114 );
inline char * _FuncastTickdescribeTick842554968825501494_e6a33d3e7995fe10 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_115 );
inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_9c918ef85c4c40f9 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1759_116 );
inline void clone_e550d220fd0c5d56 ( Context * __context__, das::vector<das::string> & __args_rename_at_16_117, TArray<char *> & __nargs_rename_at_16_118 );
inline void clone_4e7109a5607458d9 ( Context * __context__, das::vector<smart_ptr<TypeDecl>> & __args_rename_at_25_121, TArray<smart_ptr_raw<TypeDecl>> & __nargs_rename_at_25_122 );
inline void clone_983de54be7cdf35d ( Context * __context__, das::vector<smart_ptr<Variable>> & __args_rename_at_33_124, TArray<smart_ptr_raw<Variable>> & __nargs_rename_at_33_125 );
inline void clone_4a8f1905102d3a4d ( Context * __context__, das::vector<smart_ptr<Expression>> & __args_rename_at_41_127, TArray<smart_ptr_raw<Expression>> & __nargs_rename_at_41_128 );
inline bool isVectorType_2f119d66db3670e0 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_49_130 );
inline char * describe_b10821fc8be60048 ( Context * __context__, AnnotationArgumentList const  & __list_rename_at_59_131 );
inline char * describe_de6262ea55644562 ( Context * __context__, AnnotationDeclaration const  & __ann_rename_at_63_134 );
inline char * describe_9df8440d80d5b168 ( Context * __context__, AnnotationList const  & __list_rename_at_71_135 );
inline char * describe_aa8f12fd84e4c466 ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_75_138 );
inline bool isExpression_8997b287eaa15b6 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_96_139, bool __top_rename_at_96_140 );
inline bool is_same_or_inherited_468337318a989d6a ( Context * __context__, Structure const  * const  __parent_rename_at_114_141, Structure const  * const  __child_rename_at_114_142 );
inline bool is_class_method_9235fd3f9e8a4f9f ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_125_144, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_125_145 );
inline void emplace_new_e80b751cadf868b4 ( Context * __context__, das::vector<smart_ptr<Expression>> & __vec_rename_at_147_146, smart_ptr_raw<Expression> __ptr_rename_at_147_147 );
inline void emplace_new_64d0d765e96319e1 ( Context * __context__, das::vector<smart_ptr<TypeDecl>> & __vec_rename_at_151_148, smart_ptr_raw<TypeDecl> __ptr_rename_at_151_149 );
inline void emplace_new_edb6c239444b3d2a ( Context * __context__, das::vector<smart_ptr<Variable>> & __vec_rename_at_155_150, smart_ptr_raw<Variable> __ptr_rename_at_155_151 );
inline void emplace_new_63295e37889c520a ( Context * __context__, MakeStruct & __vec_rename_at_159_152, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_159_153 );
inline bool override_method_3856620b83eb08d5 ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_163_154, char * const  __name_rename_at_163_155, char * const  __funcName_rename_at_163_156 );
inline void for_each_tag_function_da829e21b9c7f2ff ( Context * __context__, Module * const  __mod_rename_at_216_160, char * const  __tag_rename_at_216_161, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __blk_rename_at_216_162 );
inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_d980886dc0e2db90 ( Context * __context__, char * const  __argn_rename_at_242_169, AnnotationArgumentList const  & __args_rename_at_242_170 );
inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_a35a5e6fbc8f2644 ( Context * __context__, AnnotationArgumentList const  & __args_rename_at_251_172, char * const  __argn_rename_at_251_173 );
inline void apply_tag_annotation_5141104131781cdd ( Context * __context__, char * const  __tag_rename_at_260_175, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_260_176 );
inline smart_ptr_raw<Function> find_unique_function_fa121c0afbb73b96 ( Context * __context__, Module * const  __mod_rename_at_279_181, char * const  __name_rename_at_279_182, bool __canfail_rename_at_279_183 );
inline smart_ptr_raw<Function> find_unique_generic_fa121c0afbb73b96 ( Context * __context__, Module * const  __mod_rename_at_295_187, char * const  __name_rename_at_295_188, bool __canfail_rename_at_295_189 );
inline ExprBlock * setup_call_list_6f07c8272f142ab7 ( Context * __context__, char * const  __name_rename_at_311_193, LineInfo const  & __at_rename_at_311_194, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __subblock_rename_at_311_195 );
inline ExprBlock * setup_call_list_bc9603dfbe371e25 ( Context * __context__, char * const  __name_rename_at_332_200, LineInfo const  & __at_rename_at_332_201, bool __isInit_rename_at_332_202, bool __isPrivate_rename_at_332_203, bool __isLateInit_rename_at_332_204 );
inline ExprBlock * setup_macro_8a720e08f64feaa2 ( Context * __context__, char * const  __name_rename_at_346_206, LineInfo const  & __at_rename_at_346_207 );
inline void * panic_expr_as_594c2d3529d6030b ( Context * __context__ );
inline bool _FuncTickisTickBuiltInFunction_ff36804875e46671 ( Context * __context__, Function * const  __foo_rename_at_586_220 );
inline BuiltInFunction * _FuncTickasTickBuiltInFunction_31b52fee11dc1291 ( Context * __context__, Function * const  __foo_rename_at_590_221 );
inline bool _FuncTickisTickExternalFnBase_ff36804875e46671 ( Context * __context__, Function * const  __foo_rename_at_599_222 );
inline ExternalFnBase * _FuncTickasTickExternalFnBase_e366761490f25f61 ( Context * __context__, Function * const  __foo_rename_at_603_223 );
inline bool _FuncTickisTickFunctionAnnotation_e8ecf59c01f982e2 ( Context * __context__, Annotation * const  __foo_rename_at_612_224 );
inline bool _FuncTickisTickFunctionAnnotation_4c5cbfd429c14538 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_616_225 );
inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_ff32b81c03ca0ab7 ( Context * __context__, Annotation * const  __foo_rename_at_620_226 );
inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_9eed8a3c62d29bc1 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_625_227 );
inline bool _FuncTickisTickStructureAnnotation_e8ecf59c01f982e2 ( Context * __context__, Annotation * const  __foo_rename_at_635_228 );
inline bool _FuncTickisTickStructureAnnotation_4c5cbfd429c14538 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_639_229 );
inline StructureAnnotation * _FuncTickasTickStructureAnnotation_775bba9924ff4625 ( Context * __context__, Annotation * const  __foo_rename_at_643_230 );
inline StructureAnnotation * _FuncTickasTickStructureAnnotation_511c2a8dbf5866df ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_648_231 );
inline smart_ptr_raw<Expression> walk_and_convert_array_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_653_232, smart_ptr_raw<TypeDecl> const  __info_rename_at_653_233, LineInfo const  & __at_rename_at_653_234 );
inline smart_ptr_raw<Expression> walk_and_convert_dim_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_670_241, smart_ptr_raw<TypeDecl> const  __info_rename_at_670_242, LineInfo const  & __at_rename_at_670_243 );
inline smart_ptr_raw<Expression> walk_and_convert_pointer_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_685_251, smart_ptr_raw<TypeDecl> const  __info_rename_at_685_252, LineInfo const  & __at_rename_at_685_253 );
inline smart_ptr_raw<Expression> walk_and_convert_tuple_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_694_257, smart_ptr_raw<TypeDecl> const  __info_rename_at_694_258, LineInfo const  & __at_rename_at_694_259 );
inline smart_ptr_raw<Expression> walk_and_convert_structure_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_706_264, smart_ptr_raw<TypeDecl> const  __info_rename_at_706_265, LineInfo const  & __at_rename_at_706_266 );
inline smart_ptr_raw<Expression> walk_and_convert_variant_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_727_275, smart_ptr_raw<TypeDecl> const  __info_rename_at_727_276, LineInfo const  & __at_rename_at_727_277 );
inline smart_ptr_raw<Expression> walk_and_convert_table_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_742_284, smart_ptr_raw<TypeDecl> const  __info_rename_at_742_285, LineInfo const  & __at_rename_at_742_286 );
inline smart_ptr_raw<Expression> walk_and_convert_basic_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_768_299, smart_ptr_raw<TypeDecl> const  __info_rename_at_768_300, LineInfo const  & __at_rename_at_768_301 );
inline smart_ptr_raw<Expression> walk_and_convert_enumeration_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_811_303, smart_ptr_raw<TypeDecl> const  __info_rename_at_811_304, LineInfo const  & __at_rename_at_811_305 );
inline smart_ptr_raw<Expression> walk_and_convert_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_831_309, smart_ptr_raw<TypeDecl> const  __info_rename_at_831_310, LineInfo const  & __at_rename_at_831_311 );
inline Annotation const  * find_annotation_c19256528c6f4891 ( Context * __context__, char * const  __mod_name_rename_at_886_312, char * const  __ann_name_rename_at_886_313 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_6df7f24b25adbbe9 ( Context * __context__, char * const  __mod_name_rename_at_902_317, char * const  __ann_name_rename_at_902_318, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_902_319 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_5a18550a173ee340 ( Context * __context__, char * const  __mod_name_rename_at_925_325, char * const  __ann_name_rename_at_925_326 );
inline void append_annotation_e9a416b646100dc5 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_933_329, char * const  __mod_name_rename_at_933_330, char * const  __ann_name_rename_at_933_331 );
inline void append_annotation_dfaac475e585cd10 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_940_334, char * const  __mod_name_rename_at_940_335, char * const  __ann_name_rename_at_940_336 );
inline void append_annotation_47fac1008de8360a ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_947_339, char * const  __mod_name_rename_at_947_340, char * const  __ann_name_rename_at_947_341 );
inline void append_annotation_4bdfb65e8a2b2d70 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_954_344, char * const  __mod_name_rename_at_954_345, char * const  __ann_name_rename_at_954_346, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_954_347 );
inline void append_annotation_5c5029f094b33ed9 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_961_350, char * const  __mod_name_rename_at_961_351, char * const  __ann_name_rename_at_961_352, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_961_353 );
inline void append_annotation_44c3a98ac2aa054f ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_968_356, char * const  __mod_name_rename_at_968_357, char * const  __ann_name_rename_at_968_358, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_968_359 );
inline int32_t add_annotation_argument_5bf2cbe639678fe9 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_975_362, char * const  __argName_rename_at_975_363, bool __val_rename_at_975_364 );
inline int32_t add_annotation_argument_d2c3575c5a39da7c ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_982_366, char * const  __argName_rename_at_982_367, int32_t __val_rename_at_982_368 );
inline int32_t add_annotation_argument_deb37309ab8a2522 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_989_370, char * const  __argName_rename_at_989_371, float __val_rename_at_989_372 );
inline int32_t add_annotation_argument_f3e2ee746b536f08 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_996_374, char * const  __argName_rename_at_996_375, char * const  __val_rename_at_996_376 );
inline int32_t add_annotation_argument_2298c6271bc3206b ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1003_378, AnnotationArgument const  & __ann_rename_at_1003_379 );
inline int32_t get_for_source_index_c1882058c708b81b ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1020_381, smart_ptr_raw<Variable> const  __svar_rename_at_1020_382 );
inline int32_t get_for_source_index_1517d199a978d69d ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1029_387, smart_ptr_raw<Expression> const  __source_rename_at_1029_388 );
inline smart_ptr_raw<TypeDecl> function_to_type_793f284cf58fe257 ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_1077_393 );
inline void visit_finally_7ef3ac0e30ffa4e9 ( Context * __context__, ExprBlock * const  __blk_rename_at_1089_399, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1089_400 );
inline bool isCMRES_6adc4be0ebbb0a22 ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1095_401 );
inline bool isCMRES_ff36804875e46671 ( Context * __context__, Function * const  __fun_rename_at_1099_402 );
inline bool isMakeLocal_5dd70dbec15d4405 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1103_403 );
inline bool isExprCallFunc_5dd70dbec15d4405 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1111_404 );
inline TDim<DAS_COMMENT(bound_enum) das::Type,31> get_workhorse_types_a90ec7ad16111ce3 ( Context * __context__ );
inline int32_t find_argument_index_c700ea639affec6e ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1131_405, char * const  __name_rename_at_1131_406 );
inline bool isCMRESType_cb4539c6e62e1c89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1140_411 );
inline void debug_expression_impl_e624caf7951258cb ( Context * __context__, StringBuilderWriter & __writer_rename_at_1148_412, smart_ptr_raw<Expression> const  __expr_rename_at_1148_413, Bitfield __deFlags_rename_at_1148_414, int32_t __tabs_rename_at_1148_415 );
inline char * debug_expression_46b4fe8de76ec412 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1254_449, Bitfield __deFlags_rename_at_1254_450 );
inline char * debug_expression_ba409c75004d2596 ( Context * __context__, Expression * const  __expr_rename_at_1260_452 );
inline char * describe_ba409c75004d2596 ( Context * __context__, Expression * const  __expr_rename_at_1266_453 );
inline int32_t getVectorElementCount_712a3c4e8dfb5421 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1272_454 );
inline int32_t getVectorElementSize_712a3c4e8dfb5421 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1279_455 );
inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_b00d209bec8bc9fd ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1284_456 );
inline int32_t getVectorOffset_f53fa57fd6e226fb ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1293_457, char * const  __ident_rename_at_1293_458 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_577e5c7310ce42 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b52298649fd5c67b ( Context * __context__, ast_boost::MacroMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6114197bfba716e9 ( Context * __context__, ast_boost::TagFunctionAnnotation const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_d8d4a649436ee872 ( Context * __context__, TArray<ast::AstStructureAnnotation *> &  __Arr_rename_at_181_4, ast::AstStructureAnnotation * __value_rename_at_181_5 )
{
    das_copy(__Arr_rename_at_181_4(builtin_array_push_back(das_arg<TArray<ast::AstStructureAnnotation *>>::pass(__Arr_rename_at_181_4),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_5);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3dc324a94b22dc94 ( Context * __context__, ast_boost::SetupFunctionAnnotation const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_ada54212b7f744a6 ( Context * __context__, ast_boost::SetupBlockAnnotation const  &  __cl_rename_at_116_7 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_7.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d5f5b99ec073834e ( Context * __context__, ast_boost::SetupStructureAnnotation const  &  __cl_rename_at_116_8 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_8.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bf8bd9eebb3a2d23 ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  &  __cl_rename_at_116_9 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_9.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7e21e784fc551687 ( Context * __context__, ast_boost::SetupContractAnnotation const  &  __cl_rename_at_116_10 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_10.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_15d783f7e9c7ea7a ( Context * __context__, ast_boost::SetupReaderMacro const  &  __cl_rename_at_116_11 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_11.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9300fad43e61790a ( Context * __context__, ast_boost::SetupCommentReader const  &  __cl_rename_at_116_12 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_12.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bda902199373ea0 ( Context * __context__, ast_boost::SetupCallMacro const  &  __cl_rename_at_116_13 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_13.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_170bf2ffb5736ec4 ( Context * __context__, ast_boost::SetupTypeInfoMacro const  &  __cl_rename_at_116_14 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_14.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b8586b2d29046ffa ( Context * __context__, ast_boost::SetupVariantMacro const  &  __cl_rename_at_116_15 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_15.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_80c16e0c2632db9b ( Context * __context__, ast_boost::SetupForLoopMacro const  &  __cl_rename_at_116_16 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_16.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6b4b3f0ecc112b40 ( Context * __context__, ast_boost::SetupCaptureMacro const  &  __cl_rename_at_116_17 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_17.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_101e4eb21b625567 ( Context * __context__, ast_boost::SetupTypeMacro const  &  __cl_rename_at_116_18 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_18.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_591efe2bffa234e ( Context * __context__, ast_boost::SetupSimulateMacro const  &  __cl_rename_at_116_19 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_19.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_73d636384df024a7 ( Context * __context__, ast_boost::TagStructureAnnotation const  &  __cl_rename_at_116_20 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_20.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_aec7f4795a71d05e ( Context * __context__, ast_boost::TagFunctionMacro const  &  __cl_rename_at_116_21 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_21.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4f45e677a4b772d8 ( Context * __context__, ast_boost::SetupInferMacro const  &  __cl_rename_at_116_22 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_22.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7a6a7aa50553599a ( Context * __context__, ast_boost::SetupDirtyInferMacro const  &  __cl_rename_at_116_23 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_23.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7edb518827a4cdad ( Context * __context__, ast_boost::SetupOptimizationMacro const  &  __cl_rename_at_116_24 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_24.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_dc816cef426a7f26 ( Context * __context__, ast_boost::SetupLintMacro const  &  __cl_rename_at_116_25 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_25.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5355aa25e23f93e6 ( Context * __context__, ast_boost::SetupGlobalLintMacro const  &  __cl_rename_at_116_26 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_26.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_bea46e5118b8a97f ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_181_27, ast::AstVariantMacro * __value_rename_at_181_28 )
{
    das_copy(__Arr_rename_at_181_27(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_181_27),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_28);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_585b805c28f303bc ( Context * __context__, ast_boost::BetterRttiVisitor const  &  __cl_rename_at_116_29 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_29.__rtti))).getStructType())));
}

inline void clone_a8a561a0ef667b7 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & __dest_rename_at_1092_30, smart_ptr_raw<FunctionAnnotation> const  __src_rename_at_1092_31 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1092_30),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1092_31),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncbuiltinTickclone_to_moveTick2007252383599261567_a3509efc10f05106 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> const  __clone_src_rename_at_1089_32 )
{
    smart_ptr_raw<FunctionAnnotation> __clone_dest_rename_at_1091_33;das_zero(__clone_dest_rename_at_1091_33);
    clone_a8a561a0ef667b7(__context__,__clone_dest_rename_at_1091_33,__clone_src_rename_at_1089_32);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(__clone_dest_rename_at_1091_33);
}

inline Module * _FuncastTickfind_moduleTick11101329256228773934_d842461399a3c7ba ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_948_34, char * const  __name_rename_at_948_35 ) { das_stack_prologue __prologue(__context__,112,"ast`find_module`11101329256228773934 " DAS_FILE_LINE);
{
    Module * __rm_rename_at_949_36 = 0;
    rtti_builtin_program_for_each_module(__prog_rename_at_948_34,das_make_block<void,Module *>(__context__,96,0,&__func_info__4dedd3fc50b439aa,[&](Module * __mod_rename_at_950_37) -> void{
        if ( SimPolicy<char *>::Equ(cast<char *>::from(((char * const )(to_das_string(das_arg<das::string>::pass(__mod_rename_at_950_37->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),cast<char *>::from(__name_rename_at_948_35),*__context__,nullptr) )
        {
            das_copy(__rm_rename_at_949_36,__mod_rename_at_950_37);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __rm_rename_at_949_36 == nullptr )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) "module '")), cast<char * const >::from(__name_rename_at_948_35), cast<char * const >::from(((char *) "' not found")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<Module *>::cast(__rm_rename_at_949_36);
}}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c1a84a72f7e61d2c ( Context * __context__, TArray<char *> &  __a_rename_at_1215_38 )
{
    builtin_array_free(das_arg<TArray<char *>>::pass(__a_rename_at_1215_38),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_976f9f8d3c51340f ( Context * __context__, TArray<smart_ptr_raw<TypeDecl>> &  __a_rename_at_1215_39 )
{
    {
        bool __need_loop_1217 = true;
        // aV: smart_ptr<ast::TypeDecl> aka TT&
        das_iterator<TArray<smart_ptr_raw<TypeDecl>>> __aV_iterator(__a_rename_at_1215_39);
        smart_ptr_raw<TypeDecl> * __aV_rename_at_1217_40;
        __need_loop_1217 = __aV_iterator.first(__context__,(__aV_rename_at_1217_40)) && __need_loop_1217;
        for ( ; __need_loop_1217 ; __need_loop_1217 = __aV_iterator.next(__context__,(__aV_rename_at_1217_40)) )
        {
            das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,(*__aV_rename_at_1217_40));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1217_40));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<TypeDecl>>>::pass(__a_rename_at_1215_39),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_70f04f60487df041 ( Context * __context__, TArray<smart_ptr_raw<Variable>> &  __a_rename_at_1215_41 )
{
    {
        bool __need_loop_1217 = true;
        // aV: smart_ptr<ast::Variable> aka TT&
        das_iterator<TArray<smart_ptr_raw<Variable>>> __aV_iterator(__a_rename_at_1215_41);
        smart_ptr_raw<Variable> * __aV_rename_at_1217_42;
        __need_loop_1217 = __aV_iterator.first(__context__,(__aV_rename_at_1217_42)) && __need_loop_1217;
        for ( ; __need_loop_1217 ; __need_loop_1217 = __aV_iterator.next(__context__,(__aV_rename_at_1217_42)) )
        {
            das_delete_handle<smart_ptr_raw<Variable>>::clear(__context__,(*__aV_rename_at_1217_42));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1217_42));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Variable>>>::pass(__a_rename_at_1215_41),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_9f199f97decbabbe ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __a_rename_at_1215_43 )
{
    {
        bool __need_loop_1217 = true;
        // aV: smart_ptr<ast::Expression> aka TT&
        das_iterator<TArray<smart_ptr_raw<Expression>>> __aV_iterator(__a_rename_at_1215_43);
        smart_ptr_raw<Expression> * __aV_rename_at_1217_44;
        __need_loop_1217 = __aV_iterator.first(__context__,(__aV_rename_at_1217_44)) && __need_loop_1217;
        for ( ; __need_loop_1217 ; __need_loop_1217 = __aV_iterator.next(__context__,(__aV_rename_at_1217_44)) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,(*__aV_rename_at_1217_44));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1217_44));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__a_rename_at_1215_43),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_1445ed0430a8bb9b ( Context * __context__, TArray<char *> &  __Arr_rename_at_181_45, char * __value_rename_at_181_46 )
{
    das_copy(__Arr_rename_at_181_45(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_181_45),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_46);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_daeec2492818d4d1 ( Context * __context__, TArray<char *> const  &  __it_rename_at_22_47, char * const  __separator_rename_at_22_48 )
{
    char * __st_rename_at_27_49 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_27_50) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_28_51 = true;
        {
            bool __need_loop_29 = true;
            // elem: string const&
            das_iterator<TArray<char *> const > __elem_iterator(__it_rename_at_22_47);
            char * const  * __elem_rename_at_29_52;
            __need_loop_29 = __elem_iterator.first(__context__,(__elem_rename_at_29_52)) && __need_loop_29;
            for ( ; __need_loop_29 ; __need_loop_29 = __elem_iterator.next(__context__,(__elem_rename_at_29_52)) )
            {
                if ( __skip_first_rename_at_28_51 )
                {
                    das_copy(__skip_first_rename_at_28_51,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_27_50),cast<char * const >::from(__separator_rename_at_22_48)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_27_50),cast<char * const >::from((*__elem_rename_at_29_52))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_29_52));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_27_49);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_c66cc804629e0825 ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_53, char * const  __value_rename_at_165_54 )
{
    das_copy(__Arr_rename_at_165_53(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_53),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_54);
}

inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_18f7d5397c9c7e3 ( Context * __context__, TArray<smart_ptr_raw<Function>> &  __Arr_rename_at_377_55, smart_ptr_raw<Function> __value_rename_at_377_56 )
{
    clone_f2eaefba27105789(__context__,__Arr_rename_at_377_55(builtin_array_push_back_zero(das_arg<TArray<smart_ptr_raw<Function>>>::pass(__Arr_rename_at_377_55),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_377_56);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_7cde73d852192941 ( Context * __context__, TArray<smart_ptr_raw<Function>> &  __a_rename_at_1215_57 )
{
    {
        bool __need_loop_1217 = true;
        // aV: smart_ptr<ast::Function> aka TT&
        das_iterator<TArray<smart_ptr_raw<Function>>> __aV_iterator(__a_rename_at_1215_57);
        smart_ptr_raw<Function> * __aV_rename_at_1217_58;
        __need_loop_1217 = __aV_iterator.first(__context__,(__aV_rename_at_1217_58)) && __need_loop_1217;
        for ( ; __need_loop_1217 ; __need_loop_1217 = __aV_iterator.next(__context__,(__aV_rename_at_1217_58)) )
        {
            das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,(*__aV_rename_at_1217_58));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1217_58));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Function>>>::pass(__a_rename_at_1215_57),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_f2eaefba27105789 ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_283_59, smart_ptr_raw<Function> const  __src_rename_at_283_60 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_283_59),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_283_60),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick2562845734617055679_b1fd376d21aa823f ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_61, bool __extra_rename_at_38_62, bool __contracts_rename_at_38_63, bool __modules_rename_at_38_64 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_61,__extra_rename_at_38_62,__contracts_rename_at_38_63,__modules_rename_at_38_64,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_130b6b8d46be3007 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1759_65 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1759_65));
}

inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_4da357be30f10f8d ( Context * __context__ )
{
    AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> __t_rename_at_164_66;das_zero(__t_rename_at_164_66);
    set_variant_index(das_arg<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::pass(__t_rename_at_164_66),8);
    return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(__t_rename_at_164_66);
}

inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_1836be9fc5e8fc93 ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1765_67 )
{
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__src_rename_at_1765_67));
}

inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_899221f202686a02 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1765_68 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1765_68));
}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_7477174a10d67388 ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_515_69, smart_ptr_raw<Expression> __enull_rename_at_515_70 ) { das_stack_prologue __prologue(__context__,816,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_516_71; memset((void*)&__vdr_rename_at_516_71,0,sizeof(__vdr_rename_at_516_71));
    smart_ptr_raw<ExprConstString> __cna_rename_at_517_73; memset((void*)&__cna_rename_at_517_73,0,sizeof(__cna_rename_at_517_73));
    smart_ptr_raw<ExprOp2> __cond_rename_at_518_75; memset((void*)&__cond_rename_at_518_75,0,sizeof(__cond_rename_at_518_75));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_519_77; memset((void*)&__ctype_rename_at_519_77,0,sizeof(__ctype_rename_at_519_77));
    smart_ptr_raw<Expression> __esub_rename_at_523_79; memset((void*)&__esub_rename_at_523_79,0,sizeof(__esub_rename_at_523_79));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_525_80; memset((void*)&__neqz_rename_at_525_80,0,sizeof(__neqz_rename_at_525_80));
    smart_ptr_raw<ExprOp2> __nand_rename_at_529_82; memset((void*)&__nand_rename_at_529_82,0,sizeof(__nand_rename_at_529_82));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_535_84; memset((void*)&__nsub_rename_at_535_84,0,sizeof(__nsub_rename_at_535_84));
    smart_ptr_raw<ExprCast> __vcast_rename_at_541_85; memset((void*)&__vcast_rename_at_541_85,0,sizeof(__vcast_rename_at_541_85));
    smart_ptr_raw<ExprOp3> __tri_rename_at_547_86; memset((void*)&__tri_rename_at_547_86,0,sizeof(__tri_rename_at_547_86));
    /* finally */ auto __finally_515= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_547_86);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_541_85);
    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__esub_rename_at_523_79);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_519_77);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_518_75);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_517_73);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_516_71);
    /* end finally */ });
    __vdr_rename_at_516_71; das_zero(__vdr_rename_at_516_71); das_move(__vdr_rename_at_516_71, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_516) {
        das_copy((__mks_516.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_move((__mks_516.value /*value*/),(clone_expression(__expr_rename_at_515_69->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_516.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_517_73; das_zero(__cna_rename_at_517_73); das_move(__cna_rename_at_517_73, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_517) {
        das_copy((__mks_517.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_517.text /*value*/,__expr_rename_at_515_69->name /*name*/);
        }    }))));
    __cond_rename_at_518_75; das_zero(__cond_rename_at_518_75); das_move(__cond_rename_at_518_75, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_518) {
        das_copy((__mks_518.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_move((__mks_518.left /*left*/),(__vdr_rename_at_516_71));
        das_move((__mks_518.right /*right*/),(__cna_rename_at_517_73));
        {
            set_das_string(das_arg<das::string>::pass(__mks_518.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_519_77; das_zero(__ctype_rename_at_519_77); das_move(__ctype_rename_at_519_77, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_519) {
        das_copy((__mks_519.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_copy((__mks_519.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_519.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_521) {
            das_copy((__mks_521.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            das_copy((__mks_521.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_521.alias /*alias*/,__expr_rename_at_515_69->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_523_79; das_zero(__esub_rename_at_523_79); das_move(__esub_rename_at_523_79, clone_expression(__expr_rename_at_515_69->value /*value*/));
    if ( __expr_rename_at_515_69->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_524= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_529_82);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_525_80);
        /* end finally */ });
        __neqz_rename_at_525_80; das_zero(__neqz_rename_at_525_80); das_move(__neqz_rename_at_525_80, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_525) {
            das_copy((__mks_525.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            das_move((__mks_525.left /*left*/),(clone_expression(__expr_rename_at_515_69->value /*value*/)));
            das_move((__mks_525.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_527) {
                das_copy((__mks_527.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_525.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_529_82; das_zero(__nand_rename_at_529_82); das_move(__nand_rename_at_529_82, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_529) {
            das_copy((__mks_529.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            das_move((__mks_529.left /*left*/),(__neqz_rename_at_525_80));
            das_move((__mks_529.right /*right*/),(__cond_rename_at_518_75));
            {
                set_das_string(das_arg<das::string>::pass(__mks_529.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_518_75),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_529_82),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_534= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_535_84);
        /* end finally */ });
        __nsub_rename_at_535_84; das_zero(__nsub_rename_at_535_84); das_move(__nsub_rename_at_535_84, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_535) {
            das_copy((__mks_535.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            das_move((__mks_535.subexpr /*subexpr*/),(__esub_rename_at_523_79));
            das_copy((__mks_535.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_523_79),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_535_84),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_541_85; das_zero(__vcast_rename_at_541_85); das_move(__vcast_rename_at_541_85, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_541) {
        das_copy((__mks_541.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_move((__mks_541.subexpr /*subexpr*/),(__esub_rename_at_523_79));
        das_move((__mks_541.castType /*castType*/),(__ctype_rename_at_519_77));
        das_copy((__mks_541.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_541.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_547_86; das_zero(__tri_rename_at_547_86); das_move(__tri_rename_at_547_86, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_547) {
        das_copy((__mks_547.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_move((__mks_547.subexpr /*subexpr*/),(__cond_rename_at_518_75));
        das_move((__mks_547.left /*left*/),(__vcast_rename_at_541_85));
        das_move((__mks_547.right /*right*/),(__enull_rename_at_515_70));
        {
            set_das_string(das_arg<das::string>::pass(__mks_547.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_547_86->genFlags /*genFlags*/,__expr_rename_at_515_69->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_547_86);
}}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_e3597b8badb6cad2 ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_515_88, smart_ptr_raw<Expression> __enull_rename_at_515_89 ) { das_stack_prologue __prologue(__context__,816,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_516_90; memset((void*)&__vdr_rename_at_516_90,0,sizeof(__vdr_rename_at_516_90));
    smart_ptr_raw<ExprConstString> __cna_rename_at_517_92; memset((void*)&__cna_rename_at_517_92,0,sizeof(__cna_rename_at_517_92));
    smart_ptr_raw<ExprOp2> __cond_rename_at_518_94; memset((void*)&__cond_rename_at_518_94,0,sizeof(__cond_rename_at_518_94));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_519_96; memset((void*)&__ctype_rename_at_519_96,0,sizeof(__ctype_rename_at_519_96));
    smart_ptr_raw<Expression> __esub_rename_at_523_98; memset((void*)&__esub_rename_at_523_98,0,sizeof(__esub_rename_at_523_98));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_525_99; memset((void*)&__neqz_rename_at_525_99,0,sizeof(__neqz_rename_at_525_99));
    smart_ptr_raw<ExprOp2> __nand_rename_at_529_101; memset((void*)&__nand_rename_at_529_101,0,sizeof(__nand_rename_at_529_101));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_535_103; memset((void*)&__nsub_rename_at_535_103,0,sizeof(__nsub_rename_at_535_103));
    smart_ptr_raw<ExprCast> __vcast_rename_at_541_104; memset((void*)&__vcast_rename_at_541_104,0,sizeof(__vcast_rename_at_541_104));
    smart_ptr_raw<ExprOp3> __tri_rename_at_547_105; memset((void*)&__tri_rename_at_547_105,0,sizeof(__tri_rename_at_547_105));
    /* finally */ auto __finally_515= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_547_105);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_541_104);
    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__esub_rename_at_523_98);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_519_96);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_518_94);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_517_92);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_516_90);
    /* end finally */ });
    __vdr_rename_at_516_90; das_zero(__vdr_rename_at_516_90); das_move(__vdr_rename_at_516_90, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_516) {
        das_copy((__mks_516.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_move((__mks_516.value /*value*/),(clone_expression(__expr_rename_at_515_88->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_516.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_517_92; das_zero(__cna_rename_at_517_92); das_move(__cna_rename_at_517_92, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_517) {
        das_copy((__mks_517.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_517.text /*value*/,__expr_rename_at_515_88->name /*name*/);
        }    }))));
    __cond_rename_at_518_94; das_zero(__cond_rename_at_518_94); das_move(__cond_rename_at_518_94, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_518) {
        das_copy((__mks_518.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_move((__mks_518.left /*left*/),(__vdr_rename_at_516_90));
        das_move((__mks_518.right /*right*/),(__cna_rename_at_517_92));
        {
            set_das_string(das_arg<das::string>::pass(__mks_518.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_519_96; das_zero(__ctype_rename_at_519_96); das_move(__ctype_rename_at_519_96, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_519) {
        das_copy((__mks_519.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_copy((__mks_519.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_519.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_521) {
            das_copy((__mks_521.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            das_copy((__mks_521.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_521.alias /*alias*/,__expr_rename_at_515_88->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_523_98; das_zero(__esub_rename_at_523_98); das_move(__esub_rename_at_523_98, clone_expression(__expr_rename_at_515_88->value /*value*/));
    if ( __expr_rename_at_515_88->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_524= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_529_101);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_525_99);
        /* end finally */ });
        __neqz_rename_at_525_99; das_zero(__neqz_rename_at_525_99); das_move(__neqz_rename_at_525_99, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_525) {
            das_copy((__mks_525.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            das_move((__mks_525.left /*left*/),(clone_expression(__expr_rename_at_515_88->value /*value*/)));
            das_move((__mks_525.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_527) {
                das_copy((__mks_527.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_525.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_529_101; das_zero(__nand_rename_at_529_101); das_move(__nand_rename_at_529_101, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_529) {
            das_copy((__mks_529.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            das_move((__mks_529.left /*left*/),(__neqz_rename_at_525_99));
            das_move((__mks_529.right /*right*/),(__cond_rename_at_518_94));
            {
                set_das_string(das_arg<das::string>::pass(__mks_529.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_518_94),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_529_101),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_534= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_535_103);
        /* end finally */ });
        __nsub_rename_at_535_103; das_zero(__nsub_rename_at_535_103); das_move(__nsub_rename_at_535_103, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_535) {
            das_copy((__mks_535.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            das_move((__mks_535.subexpr /*subexpr*/),(__esub_rename_at_523_98));
            das_copy((__mks_535.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_523_98),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_535_103),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_541_104; das_zero(__vcast_rename_at_541_104); das_move(__vcast_rename_at_541_104, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_541) {
        das_copy((__mks_541.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_move((__mks_541.subexpr /*subexpr*/),(__esub_rename_at_523_98));
        das_move((__mks_541.castType /*castType*/),(__ctype_rename_at_519_96));
        das_copy((__mks_541.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_541.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_547_105; das_zero(__tri_rename_at_547_105); das_move(__tri_rename_at_547_105, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_547) {
        das_copy((__mks_547.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_move((__mks_547.subexpr /*subexpr*/),(__cond_rename_at_518_94));
        das_move((__mks_547.left /*left*/),(__vcast_rename_at_541_104));
        das_move((__mks_547.right /*right*/),(__enull_rename_at_515_89));
        {
            set_das_string(das_arg<das::string>::pass(__mks_547.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_547_105->genFlags /*genFlags*/,__expr_rename_at_515_88->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_547_105);
}}

inline Annotation * _FuncbuiltinTickget_ptrTick8468476673553620226_7e735142241eeece ( Context * __context__, smart_ptr_raw<Annotation> const  __src_rename_at_1759_107 )
{
    return das_auto_cast<Annotation *>::cast(das_cast<Annotation *>::cast(__src_rename_at_1759_107));
}

inline void clone_46d70cd8acd38c11 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_675_108, smart_ptr_raw<TypeDecl> const  __src_rename_at_675_109 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_675_108),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_675_109),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_692a562a355a89d ( Context * __context__, smart_ptr_raw<Enumeration> & __dest_rename_at_827_110, Enumeration * const  __src_rename_at_827_111 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_827_110),das_auto_cast<void * const >::cast(__src_rename_at_827_111),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_bd462bbf8da5b2bf ( Context * __context__, char * const  __name_rename_at_967_112 )
{
    return das_auto_cast<Module *>::cast(_FuncastTickfind_moduleTick11101329256228773934_d842461399a3c7ba(__context__,compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_967_112));
}

inline void clone_f708d7eb74451775 ( Context * __context__, smart_ptr_raw<Annotation> & __dest_rename_at_906_113, smart_ptr_raw<Annotation> const  __src_rename_at_906_114 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_906_113),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_906_114),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick842554968825501494_e6a33d3e7995fe10 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_115 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_115,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_9c918ef85c4c40f9 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1759_116 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1759_116));
}

inline void clone_e550d220fd0c5d56 ( Context * __context__, das::vector<das::string> &  __args_rename_at_16_117, TArray<char *> &  __nargs_rename_at_16_118 )
{
    int32_t __tot_rename_at_17_119 = ((int32_t)builtin_array_size(das_arg<TArray<char *>>::pass(__nargs_rename_at_16_118)));
    das_vector_resize(das_arg<das::vector<das::string>>::pass(__args_rename_at_16_117),__tot_rename_at_17_119);
    {
        bool __need_loop_19 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__tot_rename_at_17_119));
        int32_t __i_rename_at_19_120;
        __need_loop_19 = __i_iterator.first(__context__,(__i_rename_at_19_120)) && __need_loop_19;
        for ( ; __need_loop_19 ; __need_loop_19 = __i_iterator.next(__context__,(__i_rename_at_19_120)) )
        {
            set_das_string(das_arg<das::string>::pass(das_index<das::vector<das::string>>::at(__args_rename_at_16_117,__i_rename_at_19_120,__context__)),__nargs_rename_at_16_118(__i_rename_at_19_120,__context__));
        }
        __i_iterator.close(__context__,(__i_rename_at_19_120));
    };
    _FuncbuiltinTickfinalizeTick13836114024949725080_c1a84a72f7e61d2c(__context__,das_arg<TArray<char *>>::pass(__nargs_rename_at_16_118));
}

inline void clone_4e7109a5607458d9 ( Context * __context__, das::vector<smart_ptr<TypeDecl>> &  __args_rename_at_25_121, TArray<smart_ptr_raw<TypeDecl>> &  __nargs_rename_at_25_122 )
{
    das_vector_clear(das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__args_rename_at_25_121));
    {
        bool __need_loop_27 = true;
        // narg: smart_ptr<ast::TypeDecl> aka TypeDeclPtr&
        das_iterator<TArray<smart_ptr_raw<TypeDecl>>> __narg_iterator(__nargs_rename_at_25_122);
        smart_ptr_raw<TypeDecl> * __narg_rename_at_27_123;
        __need_loop_27 = __narg_iterator.first(__context__,(__narg_rename_at_27_123)) && __need_loop_27;
        for ( ; __need_loop_27 ; __need_loop_27 = __narg_iterator.next(__context__,(__narg_rename_at_27_123)) )
        {
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__args_rename_at_25_121),(*__narg_rename_at_27_123));
        }
        __narg_iterator.close(__context__,(__narg_rename_at_27_123));
    };
    _FuncbuiltinTickfinalizeTick13836114024949725080_976f9f8d3c51340f(__context__,das_arg<TArray<smart_ptr_raw<TypeDecl>>>::pass(__nargs_rename_at_25_122));
}

inline void clone_983de54be7cdf35d ( Context * __context__, das::vector<smart_ptr<Variable>> &  __args_rename_at_33_124, TArray<smart_ptr_raw<Variable>> &  __nargs_rename_at_33_125 )
{
    das_vector_clear(das_arg<das::vector<smart_ptr<Variable>>>::pass(__args_rename_at_33_124));
    {
        bool __need_loop_35 = true;
        // narg: smart_ptr<ast::Variable> aka VariablePtr&
        das_iterator<TArray<smart_ptr_raw<Variable>>> __narg_iterator(__nargs_rename_at_33_125);
        smart_ptr_raw<Variable> * __narg_rename_at_35_126;
        __need_loop_35 = __narg_iterator.first(__context__,(__narg_rename_at_35_126)) && __need_loop_35;
        for ( ; __need_loop_35 ; __need_loop_35 = __narg_iterator.next(__context__,(__narg_rename_at_35_126)) )
        {
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Variable>>>::pass(__args_rename_at_33_124),(*__narg_rename_at_35_126));
        }
        __narg_iterator.close(__context__,(__narg_rename_at_35_126));
    };
    _FuncbuiltinTickfinalizeTick13836114024949725080_70f04f60487df041(__context__,das_arg<TArray<smart_ptr_raw<Variable>>>::pass(__nargs_rename_at_33_125));
}

inline void clone_4a8f1905102d3a4d ( Context * __context__, das::vector<smart_ptr<Expression>> &  __args_rename_at_41_127, TArray<smart_ptr_raw<Expression>> &  __nargs_rename_at_41_128 )
{
    das_vector_clear(das_arg<das::vector<smart_ptr<Expression>>>::pass(__args_rename_at_41_127));
    {
        bool __need_loop_43 = true;
        // narg: smart_ptr<ast::Expression> aka ExpressionPtr&
        das_iterator<TArray<smart_ptr_raw<Expression>>> __narg_iterator(__nargs_rename_at_41_128);
        smart_ptr_raw<Expression> * __narg_rename_at_43_129;
        __need_loop_43 = __narg_iterator.first(__context__,(__narg_rename_at_43_129)) && __need_loop_43;
        for ( ; __need_loop_43 ; __need_loop_43 = __narg_iterator.next(__context__,(__narg_rename_at_43_129)) )
        {
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__args_rename_at_41_127),(*__narg_rename_at_43_129));
        }
        __narg_iterator.close(__context__,(__narg_rename_at_43_129));
    };
    _FuncbuiltinTickfinalizeTick13836114024949725080_9f199f97decbabbe(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__nargs_rename_at_41_128));
}

inline bool isVectorType_2f119d66db3670e0 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_49_130 )
{
    return das_auto_cast<bool>::cast(((((((((((((__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tFloat2)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__typ_rename_at_49_130 == DAS_COMMENT(bound_enum) das::Type::tURange64));
}

inline char * describe_b10821fc8be60048 ( Context * __context__, AnnotationArgumentList const  &  __list_rename_at_59_131 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_daeec2492818d4d1(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_60_18_rename_at_60_132;das_zero(____acomp_60_18_rename_at_60_132);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_60_18_rename_at_60_132),false);
        {
            bool __need_loop_60 = true;
            // arg: rtti::AnnotationArgument const&
            das_iterator<AnnotationArgumentList const > __arg_iterator(__list_rename_at_59_131);
            AnnotationArgument const  * __arg_rename_at_60_133;
            __need_loop_60 = __arg_iterator.first(__context__,(__arg_rename_at_60_133)) && __need_loop_60;
            for ( ; __need_loop_60 ; __need_loop_60 = __arg_iterator.next(__context__,(__arg_rename_at_60_133)) )
            {
                AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_60_50_272; _temp_make_local_60_50_272;
                _FuncbuiltinTickpushTick10769833213962245646_1445ed0430a8bb9b(__context__,das_arg<TArray<char *>>::pass(____acomp_60_18_rename_at_60_132),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_3, cast<das::string const  &>::from((*__arg_rename_at_60_133).name /*name*/), cast<char * const >::from(((char *) "=")), cast<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> &>::from((_temp_make_local_60_50_272 = (rtti_builtin_argument_value((*__arg_rename_at_60_133),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_60_133));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_60_18_rename_at_60_132),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_60_18_rename_at_60_132);
    })),((char *) ",")));
}

inline char * describe_de6262ea55644562 ( Context * __context__, AnnotationDeclaration const  &  __ann_rename_at_63_134 )
{
    return das_auto_cast<char *>::cast((das_vector_length(__ann_rename_at_63_134.arguments /*arguments*/) != 0) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_4, cast<das::string const  &>::from(__ann_rename_at_63_134.annotation /*annotation*/->name /*name*/), cast<char * const >::from(((char *) "(")), cast<char * const >::from(describe_b10821fc8be60048(__context__,__ann_rename_at_63_134.arguments /*arguments*/)), cast<char * const >::from(((char *) ")"))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<das::string const  &>::from(__ann_rename_at_63_134.annotation /*annotation*/->name /*name*/)))));
}

inline char * describe_9df8440d80d5b168 ( Context * __context__, AnnotationList const  &  __list_rename_at_71_135 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_daeec2492818d4d1(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_72_18_rename_at_72_136;das_zero(____acomp_72_18_rename_at_72_136);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_72_18_rename_at_72_136),false);
        {
            bool __need_loop_72 = true;
            // arg: smart_ptr<rtti::AnnotationDeclaration> const&
            das_iterator<AnnotationList const > __arg_iterator(__list_rename_at_71_135);
            smart_ptr_raw<AnnotationDeclaration> const  * __arg_rename_at_72_137;
            __need_loop_72 = __arg_iterator.first(__context__,(__arg_rename_at_72_137)) && __need_loop_72;
            for ( ; __need_loop_72 ; __need_loop_72 = __arg_iterator.next(__context__,(__arg_rename_at_72_137)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_c66cc804629e0825(__context__,das_arg<TArray<char *>>::pass(____acomp_72_18_rename_at_72_136),describe_de6262ea55644562(__context__,das_deref(__context__,(*__arg_rename_at_72_137))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_72_137));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_72_18_rename_at_72_136),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_72_18_rename_at_72_136);
    })),((char *) ",")));
}

inline char * describe_aa8f12fd84e4c466 ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_75_138 )
{
    return das_auto_cast<char *>::cast((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__vvar_rename_at_75_138->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<das::string const  &>::from(__vvar_rename_at_75_138->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_b1fd376d21aa823f(__context__,__vvar_rename_at_75_138->type /*_type*/,true,true,true))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_7, cast<das::string const  &>::from(__vvar_rename_at_75_138->name /*name*/), cast<char * const >::from(((char *) ":null"))))));
}

inline bool isExpression_8997b287eaa15b6 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_96_139, bool __top_rename_at_96_140 )
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__t_rename_at_96_139),das_auto_cast<void * const >::cast(nullptr)) )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( das_vector_length(__t_rename_at_96_139->dim /*dim*/) != 0 )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( __t_rename_at_96_139->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle )
    {
        return das_auto_cast<bool>::cast((eq_dstr_str(__t_rename_at_96_139->annotation /*annotation*/->module /*_module*/->name /*name*/,((char *) "ast"))) ? das_auto_cast<bool>::cast(builtin_string_startswith(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_8, cast<das::string const  &>::from(__t_rename_at_96_139->annotation /*annotation*/->name /*name*/))),((char *) "Expr"),__context__)) : das_auto_cast<bool>::cast(false));
    } else return das_auto_cast<bool>::cast(((__t_rename_at_96_139->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer) && __top_rename_at_96_140) ? das_auto_cast<bool>::cast(isExpression_8997b287eaa15b6(__context__,__t_rename_at_96_139->firstType /*firstType*/,false)) : das_auto_cast<bool>::cast(false));
}

inline bool is_same_or_inherited_468337318a989d6a ( Context * __context__, Structure const  * const  __parent_rename_at_114_141, Structure const  * const  __child_rename_at_114_142 )
{
    Structure const  * __ch_rename_at_115_143 = __child_rename_at_114_142;
    while ( __ch_rename_at_115_143 != nullptr )
    {
        if ( __parent_rename_at_114_141 == __ch_rename_at_115_143 )
        {
            return das_auto_cast<bool>::cast(true);
        } else {
            das_copy(__ch_rename_at_115_143,__ch_rename_at_115_143->parent /*parent*/);
        };
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool is_class_method_9235fd3f9e8a4f9f ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_125_144, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_125_145 )
{
    return das_auto_cast<bool>::cast((__finfo_rename_at_125_145->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tFunction) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_125_145->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_125_145->argTypes /*argTypes*/) == 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_125_145->argTypes /*argTypes*/,0,__context__)->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tStructure) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_125_145->argTypes /*argTypes*/,0,__context__)->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(is_same_or_inherited_468337318a989d6a(__context__,das_reinterpret<Structure const  * const >::pass(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_125_145->argTypes /*argTypes*/,0,__context__)->structType /*structType*/),das_reinterpret<Structure const  * const >::pass(_FuncbuiltinTickget_ptrTick8468476673553620226_130b6b8d46be3007(__context__,__cinfo_rename_at_125_144)))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))))))))));
}

inline void emplace_new_e80b751cadf868b4 ( Context * __context__, das::vector<smart_ptr<Expression>> &  __vec_rename_at_147_146, smart_ptr_raw<Expression> __ptr_rename_at_147_147 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__vec_rename_at_147_146),__ptr_rename_at_147_147);
}

inline void emplace_new_64d0d765e96319e1 ( Context * __context__, das::vector<smart_ptr<TypeDecl>> &  __vec_rename_at_151_148, smart_ptr_raw<TypeDecl> __ptr_rename_at_151_149 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__vec_rename_at_151_148),__ptr_rename_at_151_149);
}

inline void emplace_new_edb6c239444b3d2a ( Context * __context__, das::vector<smart_ptr<Variable>> &  __vec_rename_at_155_150, smart_ptr_raw<Variable> __ptr_rename_at_155_151 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Variable>>>::pass(__vec_rename_at_155_150),__ptr_rename_at_155_151);
}

inline void emplace_new_63295e37889c520a ( Context * __context__, MakeStruct &  __vec_rename_at_159_152, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_159_153 )
{
    mks_vector_emplace(das_arg<MakeStruct>::pass(__vec_rename_at_159_152),__ptr_rename_at_159_153);
}

inline bool override_method_3856620b83eb08d5 ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_163_154, char * const  __name_rename_at_163_155, char * const  __funcName_rename_at_163_156 ) { das_stack_prologue __prologue(__context__,272,"override_method " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprCast> __vcast_rename_at_166_158; memset((void*)&__vcast_rename_at_166_158,0,sizeof(__vcast_rename_at_166_158));
    {
        bool __need_loop_164 = true;
        // fld: ast::FieldDeclaration&
        das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__str_rename_at_163_154->fields /*fields*/);
        Structure::FieldDeclaration * __fld_rename_at_164_157;
        __need_loop_164 = __fld_iterator.first(__context__,(__fld_rename_at_164_157)) && __need_loop_164;
        for ( ; __need_loop_164 ; __need_loop_164 = __fld_iterator.next(__context__,(__fld_rename_at_164_157)) )
        {
            if ( eq_dstr_str(das_arg<das::string>::pass((*__fld_rename_at_164_157).name /*name*/),__name_rename_at_163_155) )
            {
                /* finally */ auto __finally_165= das_finally([&](){
                das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_166_158);
                /* end finally */ });
                __vcast_rename_at_166_158; das_zero(__vcast_rename_at_166_158); das_move(__vcast_rename_at_166_158, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_166) {
                    das_copy((__mks_166.at /*at*/),(__str_rename_at_163_154->at /*at*/));
                    das_move((__mks_166.subexpr /*subexpr*/),(das_ascend_handle<true,smart_ptr_raw<ExprAddr>>::make(__context__,nullptr,(([&](ExprAddr & __mks_167) {
                        das_copy((__mks_167.at /*at*/),(__str_rename_at_163_154->at /*at*/));
                        {
                            set_das_string(das_arg<das::string>::pass(__mks_167.target /*target*/),__funcName_rename_at_163_156);
                        }                    })))));
                    das_move((__mks_166.castType /*castType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_168) {
                        das_copy((__mks_168.at /*at*/),(__str_rename_at_163_154->at /*at*/));
                        das_copy((__mks_168.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                    })))));
                }))));
                builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast((*__fld_rename_at_164_157).init /*init*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__vcast_rename_at_166_158),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                return das_auto_cast<bool>::cast(true);
            };
        }
        __fld_iterator.close(__context__,(__fld_rename_at_164_157));
    };
    return das_auto_cast<bool>::cast(false);
}}

inline void for_each_tag_function_da829e21b9c7f2ff ( Context * __context__, Module * const  __mod_rename_at_216_160, char * const  __tag_rename_at_216_161, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __blk_rename_at_216_162 ) { das_stack_prologue __prologue(__context__,416,"for_each_tag_function " DAS_FILE_LINE);
{
    forEachFunction(__mod_rename_at_216_160,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_217_163) -> void{
        {
            bool __need_loop_218 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_217_163->annotations /*annotations*/);
            smart_ptr_raw<AnnotationDeclaration> * __ann_rename_at_218_164;
            __need_loop_218 = __ann_iterator.first(__context__,(__ann_rename_at_218_164)) && __need_loop_218;
            for ( ; __need_loop_218 ; __need_loop_218 = __ann_iterator.next(__context__,(__ann_rename_at_218_164)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_218_164)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_218_164)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_220 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_218_164)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_220_165;
                        __need_loop_220 = __t_iterator.first(__context__,(__t_rename_at_220_165)) && __need_loop_220;
                        for ( ; __need_loop_220 ; __need_loop_220 = __t_iterator.next(__context__,(__t_rename_at_220_165)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_220_165).name /*name*/),__tag_rename_at_216_161) )
                            {
                                das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__blk_rename_at_216_162,__func_rename_at_217_163);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_220_165));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_218_164));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    forEachGenericFunction(__mod_rename_at_216_160,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,272,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_228_166) -> void{
        {
            bool __need_loop_229 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_228_166->annotations /*annotations*/);
            smart_ptr_raw<AnnotationDeclaration> * __ann_rename_at_229_167;
            __need_loop_229 = __ann_iterator.first(__context__,(__ann_rename_at_229_167)) && __need_loop_229;
            for ( ; __need_loop_229 ; __need_loop_229 = __ann_iterator.next(__context__,(__ann_rename_at_229_167)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_229_167)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_229_167)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_231 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_229_167)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_231_168;
                        __need_loop_231 = __t_iterator.first(__context__,(__t_rename_at_231_168)) && __need_loop_231;
                        for ( ; __need_loop_231 ; __need_loop_231 = __t_iterator.next(__context__,(__t_rename_at_231_168)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_231_168).name /*name*/),__tag_rename_at_216_161) )
                            {
                                das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__blk_rename_at_216_162,__func_rename_at_228_166);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_231_168));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_229_167));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_d980886dc0e2db90 ( Context * __context__, char * const  __argn_rename_at_242_169, AnnotationArgumentList const  &  __args_rename_at_242_170 )
{
    {
        bool __need_loop_243 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_242_170);
        AnnotationArgument const  * __a_rename_at_243_171;
        __need_loop_243 = __a_iterator.first(__context__,(__a_rename_at_243_171)) && __need_loop_243;
        for ( ; __need_loop_243 ; __need_loop_243 = __a_iterator.next(__context__,(__a_rename_at_243_171)) )
        {
            if ( eq_dstr_str((*__a_rename_at_243_171).name /*name*/,__argn_rename_at_242_169) )
            {
                return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(rtti_builtin_argument_value((*__a_rename_at_243_171),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_243_171));
    };
    return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_4da357be30f10f8d(__context__));
}

inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_a35a5e6fbc8f2644 ( Context * __context__, AnnotationArgumentList const  &  __args_rename_at_251_172, char * const  __argn_rename_at_251_173 )
{
    {
        bool __need_loop_252 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_251_172);
        AnnotationArgument const  * __a_rename_at_252_174;
        __need_loop_252 = __a_iterator.first(__context__,(__a_rename_at_252_174)) && __need_loop_252;
        for ( ; __need_loop_252 ; __need_loop_252 = __a_iterator.next(__context__,(__a_rename_at_252_174)) )
        {
            if ( eq_dstr_str((*__a_rename_at_252_174).name /*name*/,__argn_rename_at_251_173) )
            {
                return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(rtti_builtin_argument_value((*__a_rename_at_252_174),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_252_174));
    };
    return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_4da357be30f10f8d(__context__));
}

inline void apply_tag_annotation_5141104131781cdd ( Context * __context__, char * const  __tag_rename_at_260_175, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_260_176 ) { das_stack_prologue __prologue(__context__,160,"apply_tag_annotation " DAS_FILE_LINE);
{
    TArray<smart_ptr_raw<Function>> __funcs_rename_at_261_177; memset((void*)&__funcs_rename_at_261_177,0,sizeof(__funcs_rename_at_261_177));
    smart_ptr_raw<FunctionAnnotation> __tagAnn_rename_at_267_180; memset((void*)&__tagAnn_rename_at_267_180,0,sizeof(__tagAnn_rename_at_267_180));
    /* finally */ auto __finally_260= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_7cde73d852192941(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_261_177));
    /* end finally */ });
    das_zero(__funcs_rename_at_261_177);
    for_each_tag_function_da829e21b9c7f2ff(__context__,thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__tag_rename_at_260_175,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__806bd08fbfbd09f4,[&](smart_ptr_raw<Function> __func_rename_at_262_178) -> void{
        _FuncbuiltinTickpush_cloneTick2035469273396957942_18f7d5397c9c7e3(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_261_177),__func_rename_at_262_178);
    }));
    {
        bool __need_loop_265 = true;
        // func: smart_ptr<ast::Function> aka FunctionPtr&
        das_iterator<TArray<smart_ptr_raw<Function>>> __func_iterator(__funcs_rename_at_261_177);
        smart_ptr_raw<Function> * __func_rename_at_265_179;
        __need_loop_265 = __func_iterator.first(__context__,(__func_rename_at_265_179)) && __need_loop_265;
        for ( ; __need_loop_265 ; __need_loop_265 = __func_iterator.next(__context__,(__func_rename_at_265_179)) )
        {
            {
                /* finally */ auto __finally_266= das_finally([&](){
                das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__tagAnn_rename_at_267_180);
                /* end finally */ });
                __tagAnn_rename_at_267_180; das_zero(__tagAnn_rename_at_267_180); das_move(__tagAnn_rename_at_267_180, _FuncbuiltinTickclone_to_moveTick2007252383599261567_a3509efc10f05106(__context__,__ann_rename_at_260_176));
                addFunctionFunctionAnnotation((*__func_rename_at_265_179),__tagAnn_rename_at_267_180,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __func_iterator.close(__context__,(__func_rename_at_265_179));
    };
}}

inline smart_ptr_raw<Function> find_unique_function_fa121c0afbb73b96 ( Context * __context__, Module * const  __mod_rename_at_279_181, char * const  __name_rename_at_279_182, bool __canfail_rename_at_279_183 ) { das_stack_prologue __prologue(__context__,128,"find_unique_function " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_280_184; memset((void*)&__res_rename_at_280_184,0,sizeof(__res_rename_at_280_184));
    int32_t __count_rename_at_281_185; memset((void*)&__count_rename_at_281_185,0,sizeof(__count_rename_at_281_185));
    /* finally */ auto __finally_279= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_280_184);
    /* end finally */ });
    das_zero(__res_rename_at_280_184);
    __count_rename_at_281_185 = 0;
    forEachFunction(__mod_rename_at_279_181,__name_rename_at_279_182,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_282_186) -> void{
        clone_f2eaefba27105789(__context__,__res_rename_at_280_184,__func_rename_at_282_186);
        ++__count_rename_at_281_185;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_281_185 > 1 )
    {
        if ( !__canfail_rename_at_279_183 )
        {
            builtin_throw(((char *) "more than one function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_280_184);
    };
}}

inline smart_ptr_raw<Function> find_unique_generic_fa121c0afbb73b96 ( Context * __context__, Module * const  __mod_rename_at_295_187, char * const  __name_rename_at_295_188, bool __canfail_rename_at_295_189 ) { das_stack_prologue __prologue(__context__,128,"find_unique_generic " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_296_190; memset((void*)&__res_rename_at_296_190,0,sizeof(__res_rename_at_296_190));
    int32_t __count_rename_at_297_191; memset((void*)&__count_rename_at_297_191,0,sizeof(__count_rename_at_297_191));
    /* finally */ auto __finally_295= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_296_190);
    /* end finally */ });
    das_zero(__res_rename_at_296_190);
    __count_rename_at_297_191 = 0;
    forEachGenericFunction(__mod_rename_at_295_187,__name_rename_at_295_188,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_298_192) -> void{
        clone_f2eaefba27105789(__context__,__res_rename_at_296_190,__func_rename_at_298_192);
        ++__count_rename_at_297_191;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_297_191 > 1 )
    {
        if ( !__canfail_rename_at_295_189 )
        {
            builtin_throw(((char *) "more than one generic function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_296_190);
    };
}}

inline ExprBlock * setup_call_list_6f07c8272f142ab7 ( Context * __context__, char * const  __name_rename_at_311_193, LineInfo const  &  __at_rename_at_311_194, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __subblock_rename_at_311_195 ) { das_stack_prologue __prologue(__context__,192,"setup_call_list " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_312_196; memset((void*)&__fn_rename_at_312_196,0,sizeof(__fn_rename_at_312_196));
    smart_ptr_raw<ExprBlock> __blk_rename_at_318_198; memset((void*)&__blk_rename_at_318_198,0,sizeof(__blk_rename_at_318_198));
    ExprBlock * __res_rename_at_319_199; memset((void*)&__res_rename_at_319_199,0,sizeof(__res_rename_at_319_199));
    /* finally */ auto __finally_311= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_312_196);
    /* end finally */ });
    __fn_rename_at_312_196; das_zero(__fn_rename_at_312_196); das_move(__fn_rename_at_312_196, find_unique_function_fa121c0afbb73b96(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_311_193,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_312_196),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_313= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_318_198);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_312_196),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_314) {
            das_copy((__mks_314.at /*at*/),(__at_rename_at_311_194));
            das_copy((__mks_314.atDecl /*atDecl*/),(__at_rename_at_311_194));
            das_copy((__mks_314.flags /*flags*/),(0x200000u));
            das_move((__mks_314.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_316) {
                das_copy((__mks_316.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_316.at /*at*/),(__at_rename_at_311_194));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_314.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_9, cast<char * const >::from(__name_rename_at_311_193))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_318_198; das_zero(__blk_rename_at_318_198); das_move(__blk_rename_at_318_198, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_318) {
            das_copy((__mks_318.at /*at*/),(__at_rename_at_311_194));
        }))));
        __res_rename_at_319_199 = _FuncbuiltinTickget_ptrTick5807679485210906136_1836be9fc5e8fc93(__context__,__blk_rename_at_318_198);
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_312_196->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_318_198),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__subblock_rename_at_311_195,__fn_rename_at_312_196);
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_312_196,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_10, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_312_196->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(__res_rename_at_319_199);
    } else if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_312_196->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
    {
        builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__fn_rename_at_312_196->body /*body*/));
}}

inline ExprBlock * setup_call_list_bc9603dfbe371e25 ( Context * __context__, char * const  __name_rename_at_332_200, LineInfo const  &  __at_rename_at_332_201, bool __isInit_rename_at_332_202, bool __isPrivate_rename_at_332_203, bool __isLateInit_rename_at_332_204 ) { das_stack_prologue __prologue(__context__,96,"setup_call_list " DAS_FILE_LINE);
{
    return das_auto_cast<ExprBlock *>::cast(setup_call_list_6f07c8272f142ab7(__context__,__name_rename_at_332_200,__at_rename_at_332_201,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__3b2c4edb933bef4a,[&](smart_ptr_raw<Function> __fn_rename_at_333_205) -> void{
        if ( __isInit_rename_at_332_202 )
        {
            __fn_rename_at_333_205->flags /*flags*/ |= 0x100u;
            if ( __isLateInit_rename_at_332_204 )
            {
                __fn_rename_at_333_205->moreFlags /*moreFlags*/ |= 0x8u;
            };
        };
        if ( __isPrivate_rename_at_332_203 )
        {
            __fn_rename_at_333_205->flags /*flags*/ |= 0x400000u;
        };
    })));
}}

inline ExprBlock * setup_macro_8a720e08f64feaa2 ( Context * __context__, char * const  __name_rename_at_346_206, LineInfo const  &  __at_rename_at_346_207 ) { das_stack_prologue __prologue(__context__,480,"setup_macro " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_348_208; memset((void*)&__fn_rename_at_348_208,0,sizeof(__fn_rename_at_348_208));
    smart_ptr_raw<ExprBlock> __blk_rename_at_354_210; memset((void*)&__blk_rename_at_354_210,0,sizeof(__blk_rename_at_354_210));
    smart_ptr_raw<ExprBlock> __iblk_rename_at_355_211; memset((void*)&__iblk_rename_at_355_211,0,sizeof(__iblk_rename_at_355_211));
    ExprBlock * __res_rename_at_356_212; memset((void*)&__res_rename_at_356_212,0,sizeof(__res_rename_at_356_212));
    smart_ptr_raw<ExprCall> __ifm_rename_at_357_213; memset((void*)&__ifm_rename_at_357_213,0,sizeof(__ifm_rename_at_357_213));
    smart_ptr_raw<ExprConstString> __ifmn_rename_at_358_215; memset((void*)&__ifmn_rename_at_358_215,0,sizeof(__ifmn_rename_at_358_215));
    smart_ptr_raw<ExprIfThenElse> __ife_rename_at_360_217; memset((void*)&__ife_rename_at_360_217,0,sizeof(__ife_rename_at_360_217));
    ExprBlock * __blk_rename_at_372_218; memset((void*)&__blk_rename_at_372_218,0,sizeof(__blk_rename_at_372_218));
    ExprIfThenElse * __ife_rename_at_376_219; memset((void*)&__ife_rename_at_376_219,0,sizeof(__ife_rename_at_376_219));
    /* finally */ auto __finally_346= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_348_208);
    /* end finally */ });
    compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->flags /*flags*/ |= 0x20u;
    __fn_rename_at_348_208; das_zero(__fn_rename_at_348_208); das_move(__fn_rename_at_348_208, find_unique_function_fa121c0afbb73b96(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_346_206,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_348_208),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_349= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprIfThenElse>>::clear(__context__,__ife_rename_at_360_217);
        das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__ifmn_rename_at_358_215);
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__ifm_rename_at_357_213);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__iblk_rename_at_355_211);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_354_210);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_348_208),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_350) {
            das_copy((__mks_350.at /*at*/),(__at_rename_at_346_207));
            das_copy((__mks_350.atDecl /*atDecl*/),(__at_rename_at_346_207));
            das_copy((__mks_350.flags /*flags*/),(0x80600000u));
            das_move((__mks_350.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_352) {
                das_copy((__mks_352.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_352.at /*at*/),(__at_rename_at_346_207));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_350.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_11, cast<char * const >::from(__name_rename_at_346_206))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_354_210; das_zero(__blk_rename_at_354_210); das_move(__blk_rename_at_354_210, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_354) {
            das_copy((__mks_354.at /*at*/),(__at_rename_at_346_207));
        }))));
        __iblk_rename_at_355_211; das_zero(__iblk_rename_at_355_211); das_move(__iblk_rename_at_355_211, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_355) {
            das_copy((__mks_355.at /*at*/),(__at_rename_at_346_207));
        }))));
        __res_rename_at_356_212 = ((ExprBlock *)_FuncbuiltinTickget_ptrTick5807679485210906136_1836be9fc5e8fc93(__context__,__iblk_rename_at_355_211));
        __ifm_rename_at_357_213; das_zero(__ifm_rename_at_357_213); das_move(__ifm_rename_at_357_213, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_357) {
            das_copy((__mks_357.at /*at*/),(__at_rename_at_346_207));
            {
                set_das_string(das_arg<das::string>::pass(__mks_357.name /*name*/),((char *) "is_compiling_macros_in_module"));
            }        }))));
        __ifmn_rename_at_358_215; das_zero(__ifmn_rename_at_358_215); das_move(__ifmn_rename_at_358_215, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_358) {
            das_copy((__mks_358.at /*at*/),(__at_rename_at_346_207));
            {
                das_clone<das::string,das::string>::clone(__mks_358.text /*value*/,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->name /*name*/);
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__ifm_rename_at_357_213->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__ifmn_rename_at_358_215));
        __ife_rename_at_360_217; das_zero(__ife_rename_at_360_217); das_move(__ife_rename_at_360_217, das_ascend_handle<true,smart_ptr_raw<ExprIfThenElse>>::make(__context__,nullptr,(([&](ExprIfThenElse & __mks_360) {
            das_copy((__mks_360.at /*at*/),(__at_rename_at_346_207));
            das_move((__mks_360.cond /*cond*/),(__ifm_rename_at_357_213));
            das_move((__mks_360.if_true /*if_true*/),(__iblk_rename_at_355_211));
        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_354_210->list /*list*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__ife_rename_at_360_217));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_348_208->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_354_210),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_348_208,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_12, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_348_208->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__res_rename_at_356_212));
    } else {
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_348_208->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __blk_rename_at_372_218 = das_cast<ExprBlock *>::cast(__fn_rename_at_348_208->body /*body*/);
        if ( (das_vector_length(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_372_218->list /*list*/)) != 1) || (SimPolicy<char *>::NotEqu(cast<char *>::from(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_372_218->list /*list*/,0,__context__)->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprIfThenElse")),*__context__,nullptr)) )
        {
            builtin_throw(((char *) "expecting is_compiling_macros"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __ife_rename_at_376_219 = das_cast<ExprIfThenElse *>::cast(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_372_218->list /*list*/,0,__context__));
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__ife_rename_at_376_219->if_true /*if_true*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting ife.if_true.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__ife_rename_at_376_219->if_true /*if_true*/));
    };
}}

inline void * panic_expr_as_594c2d3529d6030b ( Context * __context__ )
{
    builtin_throw(((char *) "invalid 'as' expression or null pointer dereference"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast<void *>::cast(nullptr);
}

inline bool _FuncTickisTickBuiltInFunction_ff36804875e46671 ( Context * __context__, Function * const  __foo_rename_at_586_220 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__foo_rename_at_586_220->flags /*flags*/,1u << 0));
}

inline BuiltInFunction * _FuncTickasTickBuiltInFunction_31b52fee11dc1291 ( Context * __context__, Function * const  __foo_rename_at_590_221 )
{
    DAS_VERIFY((_FuncTickisTickBuiltInFunction_ff36804875e46671(__context__,__foo_rename_at_590_221)));
    return das_auto_cast<BuiltInFunction *>::cast(das_cast<BuiltInFunction *>::cast(__foo_rename_at_590_221));
}

inline bool _FuncTickisTickExternalFnBase_ff36804875e46671 ( Context * __context__, Function * const  __foo_rename_at_599_222 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__foo_rename_at_599_222->moreFlags /*moreFlags*/,1u << 12) && das_get_bitfield(__foo_rename_at_599_222->flags /*flags*/,1u << 0));
}

inline ExternalFnBase * _FuncTickasTickExternalFnBase_e366761490f25f61 ( Context * __context__, Function * const  __foo_rename_at_603_223 )
{
    DAS_VERIFY((_FuncTickisTickExternalFnBase_ff36804875e46671(__context__,__foo_rename_at_603_223)));
    return das_auto_cast<ExternalFnBase *>::cast(das_cast<ExternalFnBase *>::cast(__foo_rename_at_603_223));
}

inline bool _FuncTickisTickFunctionAnnotation_e8ecf59c01f982e2 ( Context * __context__, Annotation * const  __foo_rename_at_612_224 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_612_224)).rtti_isFunctionAnnotation()));
}

inline bool _FuncTickisTickFunctionAnnotation_4c5cbfd429c14538 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_616_225 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_616_225)).rtti_isFunctionAnnotation()));
}

inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_ff32b81c03ca0ab7 ( Context * __context__, Annotation * const  __foo_rename_at_620_226 )
{
    DAS_VERIFY((_FuncTickisTickFunctionAnnotation_e8ecf59c01f982e2(__context__,__foo_rename_at_620_226)));
    return das_auto_cast<FunctionAnnotation *>::cast(das_cast<FunctionAnnotation *>::cast(__foo_rename_at_620_226));
}

inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_9eed8a3c62d29bc1 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_625_227 )
{
    DAS_VERIFY((_FuncTickisTickFunctionAnnotation_4c5cbfd429c14538(__context__,__foo_rename_at_625_227)));
    return das_auto_cast<FunctionAnnotation *>::cast(das_cast<FunctionAnnotation *>::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_7e735142241eeece(__context__,__foo_rename_at_625_227)));
}

inline bool _FuncTickisTickStructureAnnotation_e8ecf59c01f982e2 ( Context * __context__, Annotation * const  __foo_rename_at_635_228 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_635_228)).rtti_isStructureAnnotation()));
}

inline bool _FuncTickisTickStructureAnnotation_4c5cbfd429c14538 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_639_229 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_639_229)).rtti_isStructureAnnotation()));
}

inline StructureAnnotation * _FuncTickasTickStructureAnnotation_775bba9924ff4625 ( Context * __context__, Annotation * const  __foo_rename_at_643_230 )
{
    DAS_VERIFY((_FuncTickisTickStructureAnnotation_e8ecf59c01f982e2(__context__,__foo_rename_at_643_230)));
    return das_auto_cast<StructureAnnotation *>::cast(das_cast<StructureAnnotation *>::cast(__foo_rename_at_643_230));
}

inline StructureAnnotation * _FuncTickasTickStructureAnnotation_511c2a8dbf5866df ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_648_231 )
{
    DAS_VERIFY((_FuncTickisTickStructureAnnotation_4c5cbfd429c14538(__context__,__foo_rename_at_648_231)));
    return das_auto_cast<StructureAnnotation *>::cast(das_cast<StructureAnnotation *>::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_7e735142241eeece(__context__,__foo_rename_at_648_231)));
}

inline smart_ptr_raw<Expression> walk_and_convert_array_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_653_232, smart_ptr_raw<TypeDecl> const  __info_rename_at_653_233, LineInfo const  &  __at_rename_at_653_234 ) { das_stack_prologue __prologue(__context__,272,"walk_and_convert_array " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_656_236; memset((void*)&__stride_rename_at_656_236,0,sizeof(__stride_rename_at_656_236));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_657_237; memset((void*)&__mkArr_rename_at_657_237,0,sizeof(__mkArr_rename_at_657_237));
    smart_ptr_raw<ExprCall> __mkToArrayMove_rename_at_662_239; memset((void*)&__mkToArrayMove_rename_at_662_239,0,sizeof(__mkToArrayMove_rename_at_662_239));
    int32_t __total_rename_at_654_235 = ((int32_t)any_array_size(das_auto_cast<void * const >::cast(__data_rename_at_653_232)));
    if ( __total_rename_at_654_235 != 0 )
    {
        /* finally */ auto __finally_655= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToArrayMove_rename_at_662_239);
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_657_237);
        /* end finally */ });
        __stride_rename_at_656_236 = ((int32_t)((das_deref(__context__,__info_rename_at_653_233->firstType /*firstType*/)).getSizeOf()));
        __mkArr_rename_at_657_237; das_zero(__mkArr_rename_at_657_237); das_move(__mkArr_rename_at_657_237, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_657) {
            das_copy((__mks_657.at /*at*/),(__at_rename_at_653_234));
            das_move((__mks_657.makeType /*makeType*/),(clone_type(__info_rename_at_653_233->firstType /*firstType*/)));
        }))));
        any_array_foreach(das_auto_cast<void * const >::cast(__data_rename_at_653_232),__stride_rename_at_656_236,das_make_block<void,void *>(__context__,144,0,&__func_info__b28b606ddb7a6760,[&](void * __value_rename_at_658_238) -> void{
            emplace_new_e80b751cadf868b4(__context__,das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_657_237->values /*values*/),walk_and_convert_80eb2f001ccee1a8(__context__,das_auto_cast<uint8_t const  * const >::cast(__value_rename_at_658_238),__info_rename_at_653_233->firstType /*firstType*/,__at_rename_at_653_234));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_657_237->makeType /*makeType*/->dim /*dim*/),__total_rename_at_654_235);
        __mkToArrayMove_rename_at_662_239; das_zero(__mkToArrayMove_rename_at_662_239); das_move(__mkToArrayMove_rename_at_662_239, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_662) {
            das_copy((__mks_662.at /*at*/),(__at_rename_at_653_234));
            {
                set_das_string(das_arg<das::string>::pass(__mks_662.name /*name*/),((char *) "to_array_move"));
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToArrayMove_rename_at_662_239->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkArr_rename_at_657_237));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkToArrayMove_rename_at_662_239);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_666) {
            das_copy((__mks_666.at /*at*/),(__at_rename_at_653_234));
            das_move((__mks_666.makeType /*makeType*/),(clone_type(__info_rename_at_653_233)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_dim_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_670_241, smart_ptr_raw<TypeDecl> const  __info_rename_at_670_242, LineInfo const  &  __at_rename_at_670_243 ) { das_stack_prologue __prologue(__context__,208,"walk_and_convert_dim " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_671_244; memset((void*)&__stride_rename_at_671_244,0,sizeof(__stride_rename_at_671_244));
    int32_t __total_rename_at_672_245; memset((void*)&__total_rename_at_672_245,0,sizeof(__total_rename_at_672_245));
    smart_ptr_raw<TypeDecl> __einfo_rename_at_673_246; memset((void*)&__einfo_rename_at_673_246,0,sizeof(__einfo_rename_at_673_246));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_675_247; memset((void*)&__mkArr_rename_at_675_247,0,sizeof(__mkArr_rename_at_675_247));
    smart_ptr_raw<Expression> __elem_rename_at_678_250; memset((void*)&__elem_rename_at_678_250,0,sizeof(__elem_rename_at_678_250));
    /* finally */ auto __finally_670= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_675_247);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__einfo_rename_at_673_246);
    /* end finally */ });
    __stride_rename_at_671_244 = ((int32_t)((das_deref(__context__,__info_rename_at_670_242)).getBaseSizeOf()));
    __total_rename_at_672_245 = ((int32_t)((das_deref(__context__,__info_rename_at_670_242)).getCountOf()));
    __einfo_rename_at_673_246; das_zero(__einfo_rename_at_673_246); das_move(__einfo_rename_at_673_246, clone_type(__info_rename_at_670_242));
    das_vector_clear(das_arg<das::vector<int32_t>>::pass(__einfo_rename_at_673_246->dim /*dim*/));
    __mkArr_rename_at_675_247; das_zero(__mkArr_rename_at_675_247); das_move(__mkArr_rename_at_675_247, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_675) {
        das_copy((__mks_675.at /*at*/),(__at_rename_at_670_243));
        {
            clone_46d70cd8acd38c11(__context__,__mks_675.makeType /*makeType*/,__info_rename_at_670_242);
        }    }))));
    {
        bool __need_loop_676 = true;
        // x: int const
        das_iterator<range> __x_iterator(mk_range(__total_rename_at_672_245));
        int32_t __x_rename_at_676_249;
        __need_loop_676 = __x_iterator.first(__context__,(__x_rename_at_676_249)) && __need_loop_676;
        for ( ; __need_loop_676 ; __need_loop_676 = __x_iterator.next(__context__,(__x_rename_at_676_249)) )
        {
            __elem_rename_at_678_250; das_zero(__elem_rename_at_678_250); das_move(__elem_rename_at_678_250, walk_and_convert_80eb2f001ccee1a8(__context__,das_ptr_add_int32(__data_rename_at_670_241,__stride_rename_at_671_244 * __x_rename_at_676_249,1),__einfo_rename_at_673_246,__at_rename_at_670_243));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_675_247->values /*values*/),__elem_rename_at_678_250);
        }
        __x_iterator.close(__context__,(__x_rename_at_676_249));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_675_247);
}}

inline smart_ptr_raw<Expression> walk_and_convert_pointer_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_685_251, smart_ptr_raw<TypeDecl> const  __info_rename_at_685_252, LineInfo const  &  __at_rename_at_685_253 )
{
    uint8_t const  * __pdata_rename_at_687_254 = ((uint8_t const  *)das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_685_251)));
    smart_ptr_raw<Expression> __elem_rename_at_688_255; das_zero(__elem_rename_at_688_255); das_move(__elem_rename_at_688_255, walk_and_convert_80eb2f001ccee1a8(__context__,__pdata_rename_at_687_254,__info_rename_at_685_252->firstType /*firstType*/,__at_rename_at_685_253));
    smart_ptr_raw<ExprAscend> __mkAsc_rename_at_689_256; das_zero(__mkAsc_rename_at_689_256); das_move(__mkAsc_rename_at_689_256, das_ascend_handle<true,smart_ptr_raw<ExprAscend>>::make(__context__,nullptr,(([&](ExprAscend & __mks_689) {
        das_copy((__mks_689.at /*at*/),(__at_rename_at_685_253));
        das_move((__mks_689.subexpr /*subexpr*/),(__elem_rename_at_688_255));
    }))));
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkAsc_rename_at_689_256);
}

inline smart_ptr_raw<Expression> walk_and_convert_tuple_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_694_257, smart_ptr_raw<TypeDecl> const  __info_rename_at_694_258, LineInfo const  &  __at_rename_at_694_259 )
{
    smart_ptr_raw<ExprMakeTuple> __mkArr_rename_at_695_260; memset((void*)&__mkArr_rename_at_695_260,0,sizeof(__mkArr_rename_at_695_260));
    int32_t __offset_rename_at_697_262; memset((void*)&__offset_rename_at_697_262,0,sizeof(__offset_rename_at_697_262));
    smart_ptr_raw<Expression> __elem_rename_at_699_263; memset((void*)&__elem_rename_at_699_263,0,sizeof(__elem_rename_at_699_263));
    /* finally */ auto __finally_694= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkArr_rename_at_695_260);
    /* end finally */ });
    __mkArr_rename_at_695_260; das_zero(__mkArr_rename_at_695_260); das_move(__mkArr_rename_at_695_260, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_695) {
        das_copy((__mks_695.at /*at*/),(__at_rename_at_694_259));
        das_move((__mks_695.recordType /*recordType*/),(clone_type(__info_rename_at_694_258)));
    }))));
    {
        bool __need_loop_696 = true;
        // idx: int const
        das_iterator<range> __idx_iterator(mk_range(das_vector_length(__info_rename_at_694_258->argTypes /*argTypes*/)));
        int32_t __idx_rename_at_696_261;
        __need_loop_696 = __idx_iterator.first(__context__,(__idx_rename_at_696_261)) && __need_loop_696;
        for ( ; __need_loop_696 ; __need_loop_696 = __idx_iterator.next(__context__,(__idx_rename_at_696_261)) )
        {
            __offset_rename_at_697_262 = ((int32_t)get_tuple_field_offset(__info_rename_at_694_258,__idx_rename_at_696_261,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            __elem_rename_at_699_263; das_zero(__elem_rename_at_699_263); das_move(__elem_rename_at_699_263, walk_and_convert_80eb2f001ccee1a8(__context__,das_ptr_add_int32(__data_rename_at_694_257,__offset_rename_at_697_262,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_694_258->argTypes /*argTypes*/,__idx_rename_at_696_261,__context__),__at_rename_at_694_259));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_695_260->values /*values*/),__elem_rename_at_699_263);
        }
        __idx_iterator.close(__context__,(__idx_rename_at_696_261));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_695_260);
}

inline smart_ptr_raw<Expression> walk_and_convert_structure_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_706_264, smart_ptr_raw<TypeDecl> const  __info_rename_at_706_265, LineInfo const  &  __at_rename_at_706_266 ) { das_stack_prologue __prologue(__context__,368,"walk_and_convert_structure " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprMakeStruct> __mkStruct_rename_at_707_267; memset((void*)&__mkStruct_rename_at_707_267,0,sizeof(__mkStruct_rename_at_707_267));
    Structure * __stype_rename_at_708_268; memset((void*)&__stype_rename_at_708_268,0,sizeof(__stype_rename_at_708_268));
    smart_ptr_raw<MakeStruct> __mkS_rename_at_709_269; memset((void*)&__mkS_rename_at_709_269,0,sizeof(__mkS_rename_at_709_269));
    uint8_t const  * __fdata_rename_at_713_271; memset((void*)&__fdata_rename_at_713_271,0,sizeof(__fdata_rename_at_713_271));
    smart_ptr_raw<Expression> __elem_rename_at_714_272; memset((void*)&__elem_rename_at_714_272,0,sizeof(__elem_rename_at_714_272));
    smart_ptr_raw<MakeFieldDecl> __mkF_rename_at_715_273; memset((void*)&__mkF_rename_at_715_273,0,sizeof(__mkF_rename_at_715_273));
    /* finally */ auto __finally_706= das_finally([&](){
    das_delete_handle<smart_ptr_raw<MakeStruct>>::clear(__context__,__mkS_rename_at_709_269);
    das_delete_handle<smart_ptr_raw<ExprMakeStruct>>::clear(__context__,__mkStruct_rename_at_707_267);
    /* end finally */ });
    __mkStruct_rename_at_707_267; das_zero(__mkStruct_rename_at_707_267); das_move(__mkStruct_rename_at_707_267, das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_707) {
        das_copy((__mks_707.at /*at*/),(__at_rename_at_706_266));
        das_move((__mks_707.makeType /*makeType*/),(clone_type(__info_rename_at_706_265)));
    }))));
    __stype_rename_at_708_268 = ((Structure *)__info_rename_at_706_265->structType /*structType*/);
    __mkS_rename_at_709_269; das_zero(__mkS_rename_at_709_269); das_move(__mkS_rename_at_709_269, das_new_handle<MakeStruct,true>::make(__context__));
    {
        bool __need_loop_710 = true;
        // x: ast::FieldDeclaration const&
        das_iterator<das::vector<Structure::FieldDeclaration> const > __x_iterator(__stype_rename_at_708_268->fields /*fields*/);
        Structure::FieldDeclaration const  * __x_rename_at_710_270;
        __need_loop_710 = __x_iterator.first(__context__,(__x_rename_at_710_270)) && __need_loop_710;
        for ( ; __need_loop_710 ; __need_loop_710 = __x_iterator.next(__context__,(__x_rename_at_710_270)) )
        {
            AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_711_28_208; _temp_make_local_711_28_208;
            if ( das_get_auto_variant_field<vec4f,8,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((_temp_make_local_711_28_208 = (find_arg_a35a5e6fbc8f2644(__context__,(*__x_rename_at_710_270).annotation /*annotation*/,((char *) "do_not_convert"))))) )
            {
                __fdata_rename_at_713_271 = ((uint8_t const  *)das_ptr_add_int32(__data_rename_at_706_264,(*__x_rename_at_710_270).offset /*offset*/,1));
                __elem_rename_at_714_272; das_zero(__elem_rename_at_714_272); das_move(__elem_rename_at_714_272, walk_and_convert_80eb2f001ccee1a8(__context__,__fdata_rename_at_713_271,(*__x_rename_at_710_270).type /*_type*/,__at_rename_at_706_266));
                __mkF_rename_at_715_273; das_zero(__mkF_rename_at_715_273); das_move(__mkF_rename_at_715_273, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_715) {
                    das_copy((__mks_715.at /*at*/),(__at_rename_at_706_266));
                    das_move((__mks_715.value /*value*/),(__elem_rename_at_714_272));
                    {
                        das_clone<das::string,das::string>::clone(__mks_715.name /*name*/,(*__x_rename_at_710_270).name /*name*/);
                    }                }))));
                if ( !((das_deref(__context__,(*__x_rename_at_710_270).type /*_type*/)).canCopy()) )
                {
                    das_copy(__mkF_rename_at_715_273->flags /*flags*/,0x1u);
                };
                emplace_new_63295e37889c520a(__context__,das_arg<MakeStruct>::pass(das_deref(__context__,__mkS_rename_at_709_269)),__mkF_rename_at_715_273);
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_710_270));
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeStruct>>>::pass(__mkStruct_rename_at_707_267->structs /*structs*/),__mkS_rename_at_709_269);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkStruct_rename_at_707_267);
}}

inline smart_ptr_raw<Expression> walk_and_convert_variant_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_727_275, smart_ptr_raw<TypeDecl> const  __info_rename_at_727_276, LineInfo const  &  __at_rename_at_727_277 ) { das_stack_prologue __prologue(__context__,208,"walk_and_convert_variant " DAS_FILE_LINE);
{
    int32_t __vindex_rename_at_729_278 = ((int32_t)das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_727_275)));
    int32_t __offset_rename_at_730_279 = ((int32_t)get_variant_field_offset(__info_rename_at_727_276,__vindex_rename_at_729_278,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    smart_ptr_raw<ExprMakeVariant> __mkVariant_rename_at_731_280; das_zero(__mkVariant_rename_at_731_280); das_move(__mkVariant_rename_at_731_280, das_ascend_handle<true,smart_ptr_raw<ExprMakeVariant>>::make(__context__,nullptr,(([&](ExprMakeVariant & __mks_731) {
        das_copy((__mks_731.at /*at*/),(__at_rename_at_727_277));
        das_move((__mks_731.makeType /*makeType*/),(clone_type(__info_rename_at_727_276)));
    }))));
    smart_ptr_raw<Expression> __elem_rename_at_732_281; das_zero(__elem_rename_at_732_281); das_move(__elem_rename_at_732_281, walk_and_convert_80eb2f001ccee1a8(__context__,das_ptr_add_int32(__data_rename_at_727_275,__offset_rename_at_730_279,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_727_276->argTypes /*argTypes*/,__vindex_rename_at_729_278,__context__),__at_rename_at_727_277));
    smart_ptr_raw<MakeFieldDecl> __mkS_rename_at_733_282; das_zero(__mkS_rename_at_733_282); das_move(__mkS_rename_at_733_282, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_733) {
        das_copy((__mks_733.at /*at*/),(__at_rename_at_727_277));
        das_move((__mks_733.value /*value*/),(__elem_rename_at_732_281));
        {
            das_clone<das::string,das::string>::clone(__mks_733.name /*name*/,das_index<das::vector<das::string> const >::at(__info_rename_at_727_276->argNames /*argNames*/,__vindex_rename_at_729_278,__context__));
        }    }))));
    if ( !((das_deref(__context__,das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_727_276->argTypes /*argTypes*/,__vindex_rename_at_729_278,__context__))).canCopy()) )
    {
        das_copy(__mkS_rename_at_733_282->flags /*flags*/,0x1u);
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeFieldDecl>>>::pass(__mkVariant_rename_at_731_280->variants /*variants*/),__mkS_rename_at_733_282);
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkVariant_rename_at_731_280);
}}

inline smart_ptr_raw<Expression> walk_and_convert_table_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_742_284, smart_ptr_raw<TypeDecl> const  __info_rename_at_742_285, LineInfo const  &  __at_rename_at_742_286 ) { das_stack_prologue __prologue(__context__,384,"walk_and_convert_table " DAS_FILE_LINE);
{
    smart_ptr_raw<TypeDecl> __tupT_rename_at_745_288; memset((void*)&__tupT_rename_at_745_288,0,sizeof(__tupT_rename_at_745_288));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_748_289; memset((void*)&__mkArr_rename_at_748_289,0,sizeof(__mkArr_rename_at_748_289));
    int32_t __key_stride_rename_at_749_290; memset((void*)&__key_stride_rename_at_749_290,0,sizeof(__key_stride_rename_at_749_290));
    int32_t __value_stride_rename_at_750_291; memset((void*)&__value_stride_rename_at_750_291,0,sizeof(__value_stride_rename_at_750_291));
    smart_ptr_raw<ExprCall> __mkToTableMove_rename_at_760_297; memset((void*)&__mkToTableMove_rename_at_760_297,0,sizeof(__mkToTableMove_rename_at_760_297));
    int32_t __total_rename_at_743_287 = ((int32_t)any_table_size(das_auto_cast<void * const >::cast(__data_rename_at_742_284)));
    if ( __total_rename_at_743_287 != 0 )
    {
        /* finally */ auto __finally_744= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToTableMove_rename_at_760_297);
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_748_289);
        das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__tupT_rename_at_745_288);
        /* end finally */ });
        __tupT_rename_at_745_288; das_zero(__tupT_rename_at_745_288); das_move(__tupT_rename_at_745_288, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_745) {
            das_copy((__mks_745.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tTuple));
            das_copy((__mks_745.at /*at*/),(__at_rename_at_742_286));
        }))));
        emplace_new_64d0d765e96319e1(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_745_288->argTypes /*argTypes*/),clone_type(__info_rename_at_742_285->firstType /*firstType*/));
        emplace_new_64d0d765e96319e1(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_745_288->argTypes /*argTypes*/),clone_type(__info_rename_at_742_285->secondType /*secondType*/));
        __mkArr_rename_at_748_289; das_zero(__mkArr_rename_at_748_289); das_move(__mkArr_rename_at_748_289, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_748) {
            das_copy((__mks_748.at /*at*/),(__at_rename_at_742_286));
            das_move((__mks_748.makeType /*makeType*/),(clone_type(__tupT_rename_at_745_288)));
        }))));
        __key_stride_rename_at_749_290 = ((int32_t)((das_deref(__context__,__info_rename_at_742_285->firstType /*firstType*/)).getSizeOf()));
        __value_stride_rename_at_750_291 = ((int32_t)((das_deref(__context__,__info_rename_at_742_285->secondType /*secondType*/)).getSizeOf()));
        any_table_foreach(das_auto_cast<void * const >::cast(__data_rename_at_742_284),__key_stride_rename_at_749_290,__value_stride_rename_at_750_291,das_make_block<void,void *,void *>(__context__,192,0,&__func_info__b50c88059cb1bcc,[&](void * __pkey_rename_at_751_292, void * __pvalue_rename_at_751_293) -> void{
            smart_ptr_raw<Expression> __key_rename_at_752_294; memset((void*)&__key_rename_at_752_294,0,sizeof(__key_rename_at_752_294));
            smart_ptr_raw<Expression> __value_rename_at_753_295; memset((void*)&__value_rename_at_753_295,0,sizeof(__value_rename_at_753_295));
            smart_ptr_raw<ExprMakeTuple> __mkTup_rename_at_754_296; memset((void*)&__mkTup_rename_at_754_296,0,sizeof(__mkTup_rename_at_754_296));
            /* finally */ auto __finally_751= das_finally([&](){
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__value_rename_at_753_295);
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__key_rename_at_752_294);
            das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkTup_rename_at_754_296);
            /* end finally */ });
            __key_rename_at_752_294; das_zero(__key_rename_at_752_294); das_move(__key_rename_at_752_294, walk_and_convert_80eb2f001ccee1a8(__context__,das_auto_cast<uint8_t const  * const >::cast(__pkey_rename_at_751_292),__info_rename_at_742_285->firstType /*firstType*/,__at_rename_at_742_286));
            __value_rename_at_753_295; das_zero(__value_rename_at_753_295); das_move(__value_rename_at_753_295, walk_and_convert_80eb2f001ccee1a8(__context__,das_auto_cast<uint8_t const  * const >::cast(__pvalue_rename_at_751_293),__info_rename_at_742_285->secondType /*secondType*/,__at_rename_at_742_286));
            __mkTup_rename_at_754_296; das_zero(__mkTup_rename_at_754_296); das_move(__mkTup_rename_at_754_296, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_754) {
                das_copy((__mks_754.at /*at*/),(__at_rename_at_742_286));
                das_move((__mks_754.recordType /*recordType*/),(clone_type(__tupT_rename_at_745_288)));
                das_copy((__mks_754.isKeyValue /*isKeyValue*/),(true));
            }))));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_754_296->values /*values*/),__key_rename_at_752_294);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_754_296->values /*values*/),__value_rename_at_753_295);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_748_289->values /*values*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkTup_rename_at_754_296));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_748_289->makeType /*makeType*/->dim /*dim*/),__total_rename_at_743_287);
        __mkToTableMove_rename_at_760_297; das_zero(__mkToTableMove_rename_at_760_297); das_move(__mkToTableMove_rename_at_760_297, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_760) {
            das_copy((__mks_760.at /*at*/),(__at_rename_at_742_286));
            {
                set_das_string(das_arg<das::string>::pass(__mks_760.name /*name*/),((char *) "to_table_move"));
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToTableMove_rename_at_760_297->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkArr_rename_at_748_289));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkToTableMove_rename_at_760_297);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_764) {
            das_copy((__mks_764.at /*at*/),(__at_rename_at_742_286));
            das_move((__mks_764.makeType /*makeType*/),(clone_type(__info_rename_at_742_285)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_basic_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_768_299, smart_ptr_raw<TypeDecl> const  __info_rename_at_768_300, LineInfo const  &  __at_rename_at_768_301 ) { das_stack_prologue __prologue(__context__,368,"walk_and_convert_basic " DAS_FILE_LINE);
{
    if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt8 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt8>>::make(__context__,nullptr,(([&](ExprConstInt8 & __mks_771) {
            das_copy((__mks_771.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_771.cvalue<int8_t>() /*value*/),(das_deref(__context__,das_cast<int8_t *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt16 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt16>>::make(__context__,nullptr,(([&](ExprConstInt16 & __mks_773) {
            das_copy((__mks_773.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_773.cvalue<int16_t>() /*value*/),(das_deref(__context__,das_cast<int16_t *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt>>::make(__context__,nullptr,(([&](ExprConstInt & __mks_775) {
            das_copy((__mks_775.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_775.cvalue<int32_t>() /*value*/),(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt64>>::make(__context__,nullptr,(([&](ExprConstInt64 & __mks_777) {
            das_copy((__mks_777.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_777.cvalue<int64_t>() /*value*/),(das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt8 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt8>>::make(__context__,nullptr,(([&](ExprConstUInt8 & __mks_779) {
            das_copy((__mks_779.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_779.cvalue<uint8_t>() /*value*/),(das_deref(__context__,das_cast<uint8_t *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt16 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt16>>::make(__context__,nullptr,(([&](ExprConstUInt16 & __mks_781) {
            das_copy((__mks_781.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_781.cvalue<uint16_t>() /*value*/),(das_deref(__context__,das_cast<uint16_t *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt>>::make(__context__,nullptr,(([&](ExprConstUInt & __mks_783) {
            das_copy((__mks_783.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_783.cvalue<uint32_t>() /*value*/),(das_deref(__context__,das_cast<uint32_t *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt64>>::make(__context__,nullptr,(([&](ExprConstUInt64 & __mks_785) {
            das_copy((__mks_785.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_785.cvalue<uint64_t>() /*value*/),(das_deref(__context__,das_cast<uint64_t *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstFloat>>::make(__context__,nullptr,(([&](ExprConstFloat & __mks_787) {
            das_copy((__mks_787.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_787.cvalue<float>() /*value*/),(das_deref(__context__,das_cast<float *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tDouble )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstDouble>>::make(__context__,nullptr,(([&](ExprConstDouble & __mks_789) {
            das_copy((__mks_789.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_789.cvalue<double>() /*value*/),(das_deref(__context__,das_cast<double *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBool>>::make(__context__,nullptr,(([&](ExprConstBool & __mks_791) {
            das_copy((__mks_791.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_791.cvalue<bool>() /*value*/),(das_deref(__context__,das_cast<bool *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange>>::make(__context__,nullptr,(([&](ExprConstRange & __mks_793) {
            das_copy((__mks_793.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_793.cvalue<range>() /*value*/),(das_deref(__context__,das_cast<range *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange>>::make(__context__,nullptr,(([&](ExprConstURange & __mks_795) {
            das_copy((__mks_795.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_795.cvalue<urange>() /*value*/),(das_deref(__context__,das_cast<urange *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange64>>::make(__context__,nullptr,(([&](ExprConstRange64 & __mks_797) {
            das_copy((__mks_797.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_797.cvalue<range64>() /*value*/),(das_deref(__context__,das_cast<range64 *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange64>>::make(__context__,nullptr,(([&](ExprConstURange64 & __mks_799) {
            das_copy((__mks_799.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_799.cvalue<urange64>() /*value*/),(das_deref(__context__,das_cast<urange64 *>::cast(__data_rename_at_768_299))));
        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_801) {
            das_copy((__mks_801.at /*at*/),(__at_rename_at_768_301));
            {
                set_das_string(das_arg<das::string>::pass(__mks_801.text /*value*/),das_deref(__context__,das_cast<char * *>::cast(__data_rename_at_768_299)));
            }        }))));
    } else if ( __info_rename_at_768_300->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBitfield>>::make(__context__,nullptr,(([&](ExprConstBitfield & __mks_803) {
            das_copy((__mks_803.at /*at*/),(__at_rename_at_768_301));
            das_copy((__mks_803.cvalue<Bitfield>() /*value*/),(das_deref(__context__,das_cast<Bitfield *>::cast(__data_rename_at_768_299))));
        }))));
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<char * const >::from(((char *) "unsupported ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__info_rename_at_768_300->baseType /*baseType*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(nullptr);
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_enumeration_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_811_303, smart_ptr_raw<TypeDecl> const  __info_rename_at_811_304, LineInfo const  &  __at_rename_at_811_305 ) { das_stack_prologue __prologue(__context__,144,"walk_and_convert_enumeration " DAS_FILE_LINE);
{
    int64_t __eval_rename_at_812_306 = INT64_C(0);
    if ( __info_rename_at_811_304->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration )
    {
        das_copy(__eval_rename_at_812_306,int64_t(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_811_303))));
    } else if ( __info_rename_at_811_304->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8 )
    {
        das_copy(__eval_rename_at_812_306,int64_t(das_deref(__context__,__data_rename_at_811_303)));
    } else if ( __info_rename_at_811_304->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16 )
    {
        das_copy(__eval_rename_at_812_306,int64_t(das_deref(__context__,das_cast<int16_t *>::cast(__data_rename_at_811_303))));
    } else if ( __info_rename_at_811_304->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64 )
    {
        das_copy(__eval_rename_at_812_306,das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_811_303)));
    } else {
        builtin_throw(((char *) "unsupported enumeration"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    char * __name_rename_at_824_307 = ((char *)(char *)(((char * const )(ast_find_enum_name(__info_rename_at_811_304->enumType /*enumType*/,__eval_rename_at_812_306,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_824_307),cast<char *>::from(nullptr),*__context__,nullptr) )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_14, cast<char * const >::from(((char *) "can't find name of enum value ")), cast<int64_t>::from(__eval_rename_at_812_306), cast<char * const >::from(((char *) " in ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_b1fd376d21aa823f(__context__,__info_rename_at_811_304,true,true,true)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstEnumeration>>::make(__context__,nullptr,(([&](ExprConstEnumeration & __mks_826) {
        {
            set_das_string(das_arg<das::string>::pass(__mks_826.text /*value*/),__name_rename_at_824_307);
            clone_692a562a355a89d(__context__,__mks_826.enumType /*enumType*/,das_cast<Enumeration *>::cast(__info_rename_at_811_304->enumType /*enumType*/));
        }    }))));
}}

inline smart_ptr_raw<Expression> walk_and_convert_80eb2f001ccee1a8 ( Context * __context__, uint8_t const  * const  __data_rename_at_831_309, smart_ptr_raw<TypeDecl> const  __info_rename_at_831_310, LineInfo const  &  __at_rename_at_831_311 )
{
    if ( das_vector_length(__info_rename_at_831_310->dim /*dim*/) != 0 )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_dim_80eb2f001ccee1a8(__context__,__data_rename_at_831_309,__info_rename_at_831_310,__at_rename_at_831_311));
    } else if ( __info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tArray )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_array_80eb2f001ccee1a8(__context__,__data_rename_at_831_309,__info_rename_at_831_310,__at_rename_at_831_311));
    } else if ( __info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        if ( das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_831_309)) == nullptr )
        {
            return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_840) {
                das_copy((__mks_840.at /*at*/),(__at_rename_at_831_311));
            }))));
        } else return das_auto_cast<smart_ptr_raw<Expression>>::cast(((equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__info_rename_at_831_310->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr))) || ((das_deref(__context__,__info_rename_at_831_310->firstType /*firstType*/)).isVoid())) ? das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_842) {
            das_copy((__mks_842.at /*at*/),(__at_rename_at_831_311));
        })))) : das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(walk_and_convert_pointer_80eb2f001ccee1a8(__context__,__data_rename_at_831_309,__info_rename_at_831_310,__at_rename_at_831_311)));
    } else if ( __info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tStructure )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_structure_80eb2f001ccee1a8(__context__,__data_rename_at_831_309,__info_rename_at_831_310,__at_rename_at_831_311));
    } else if ( __info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tVariant )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_variant_80eb2f001ccee1a8(__context__,__data_rename_at_831_309,__info_rename_at_831_310,__at_rename_at_831_311));
    } else if ( __info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTuple )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_tuple_80eb2f001ccee1a8(__context__,__data_rename_at_831_309,__info_rename_at_831_310,__at_rename_at_831_311));
    } else if ( __info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTable )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_table_80eb2f001ccee1a8(__context__,__data_rename_at_831_309,__info_rename_at_831_310,__at_rename_at_831_311));
    } else return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(((((__info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8) || (__info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16)) || (__info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64)) || (__info_rename_at_831_310->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration)) ? das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_enumeration_80eb2f001ccee1a8(__context__,__data_rename_at_831_309,__info_rename_at_831_310,__at_rename_at_831_311)) : das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_basic_80eb2f001ccee1a8(__context__,__data_rename_at_831_309,__info_rename_at_831_310,__at_rename_at_831_311)));
}

inline Annotation const  * find_annotation_c19256528c6f4891 ( Context * __context__, char * const  __mod_name_rename_at_886_312, char * const  __ann_name_rename_at_886_313 ) { das_stack_prologue __prologue(__context__,128,"find_annotation " DAS_FILE_LINE);
{
    Module * __mod_rename_at_887_314 = _FuncastTickfind_compiling_moduleTick4523452840392654583_bd462bbf8da5b2bf(__context__,__mod_name_rename_at_886_312);
    if ( __mod_rename_at_887_314 == nullptr )
    {
        return das_auto_cast<Annotation const  *>::cast(nullptr);
    } else {
        Annotation const  * __ann_rename_at_891_315 = 0;
        rtti_builtin_module_for_each_annotation(__mod_rename_at_887_314,das_make_block<void,Annotation const  &>(__context__,112,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_892_316) -> void{
            if ( eq_dstr_str(__value_rename_at_892_316.name /*name*/,__ann_name_rename_at_886_313) )
            {
                das_copy(__ann_rename_at_891_315,das_ref(__context__,__value_rename_at_892_316));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<Annotation const  *>::cast(__ann_rename_at_891_315);
    };
}}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_6df7f24b25adbbe9 ( Context * __context__, char * const  __mod_name_rename_at_902_317, char * const  __ann_name_rename_at_902_318, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_902_319 )
{
    Annotation const  * __ann_rename_at_903_320; memset((void*)&__ann_rename_at_903_320,0,sizeof(__ann_rename_at_903_320));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_905_321; memset((void*)&__decl_rename_at_905_321,0,sizeof(__decl_rename_at_905_321));
    char * * __argName_rename_at_908_323; memset((void*)&__argName_rename_at_908_323,0,sizeof(__argName_rename_at_908_323));
    AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> * __arg_rename_at_909_324; memset((void*)&__arg_rename_at_909_324,0,sizeof(__arg_rename_at_909_324));
    /* finally */ auto __finally_902= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_905_321);
    /* end finally */ });
    __ann_rename_at_903_320 = find_annotation_c19256528c6f4891(__context__,__mod_name_rename_at_902_317,__ann_name_rename_at_902_318);
    DAS_ASSERT((__ann_rename_at_903_320 != nullptr));
    __decl_rename_at_905_321; das_zero(__decl_rename_at_905_321); das_move(__decl_rename_at_905_321, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_f708d7eb74451775(__context__,__decl_rename_at_905_321->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_903_320));
    {
        bool __need_loop_907 = true;
        // argP: tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any> aka RttiValue> const&
        das_iterator<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const > __argP_iterator(__args_rename_at_902_319);
        AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>> const  * __argP_rename_at_907_322;
        __need_loop_907 = __argP_iterator.first(__context__,(__argP_rename_at_907_322)) && __need_loop_907;
        for ( ; __need_loop_907 ; __need_loop_907 = __argP_iterator.next(__context__,(__argP_rename_at_907_322)) )
        {
            __argName_rename_at_908_323 = ((char * *)&((char * &)(das_get_auto_tuple_field<char *,0,char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::get((*__argP_rename_at_907_322)))));
            __arg_rename_at_909_324 = ((AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *)&(das_get_auto_tuple_field<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,1,char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::get((*__argP_rename_at_907_322))));
            if ( das_get_auto_variant_field<bool,0,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_909_324)) )
            {
                add_annotation_argument_5bf2cbe639678fe9(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_905_321->arguments /*arguments*/),(*__argName_rename_at_908_323),das_get_auto_variant_field<bool,0,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_909_324),__context__));
            } else if ( das_get_auto_variant_field<int32_t,1,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_909_324)) )
            {
                add_annotation_argument_d2c3575c5a39da7c(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_905_321->arguments /*arguments*/),(*__argName_rename_at_908_323),das_get_auto_variant_field<int32_t,1,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_909_324),__context__));
            } else if ( das_get_auto_variant_field<float,5,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_909_324)) )
            {
                add_annotation_argument_deb37309ab8a2522(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_905_321->arguments /*arguments*/),(*__argName_rename_at_908_323),das_get_auto_variant_field<float,5,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_909_324),__context__));
            } else if ( das_get_auto_variant_field<char *,7,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_909_324)) )
            {
                add_annotation_argument_f3e2ee746b536f08(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_905_321->arguments /*arguments*/),(*__argName_rename_at_908_323),das_get_auto_variant_field<char *,7,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_909_324),__context__));
            } else {
                builtin_throw(((char *) "Invalid rtti value"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __argP_iterator.close(__context__,(__argP_rename_at_907_322));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_905_321);
}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_5a18550a173ee340 ( Context * __context__, char * const  __mod_name_rename_at_925_325, char * const  __ann_name_rename_at_925_326 )
{
    Annotation const  * __ann_rename_at_926_327; memset((void*)&__ann_rename_at_926_327,0,sizeof(__ann_rename_at_926_327));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_928_328; memset((void*)&__decl_rename_at_928_328,0,sizeof(__decl_rename_at_928_328));
    /* finally */ auto __finally_925= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_928_328);
    /* end finally */ });
    __ann_rename_at_926_327 = find_annotation_c19256528c6f4891(__context__,__mod_name_rename_at_925_325,__ann_name_rename_at_925_326);
    DAS_ASSERT((__ann_rename_at_926_327 != nullptr));
    __decl_rename_at_928_328; das_zero(__decl_rename_at_928_328); das_move(__decl_rename_at_928_328, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_f708d7eb74451775(__context__,__decl_rename_at_928_328->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_926_327));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_928_328);
}

inline void append_annotation_e9a416b646100dc5 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_933_329, char * const  __mod_name_rename_at_933_330, char * const  __ann_name_rename_at_933_331 )
{
    Annotation const  * __ann_rename_at_934_332; memset((void*)&__ann_rename_at_934_332,0,sizeof(__ann_rename_at_934_332));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_936_333; memset((void*)&__decl_rename_at_936_333,0,sizeof(__decl_rename_at_936_333));
    /* finally */ auto __finally_933= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_936_333);
    /* end finally */ });
    __ann_rename_at_934_332 = find_annotation_c19256528c6f4891(__context__,__mod_name_rename_at_933_330,__ann_name_rename_at_933_331);
    DAS_ASSERT((__ann_rename_at_934_332 != nullptr));
    __decl_rename_at_936_333; das_zero(__decl_rename_at_936_333); das_move(__decl_rename_at_936_333, append_annotation_5a18550a173ee340(__context__,__mod_name_rename_at_933_330,__ann_name_rename_at_933_331));
    addAndApplyFunctionAnnotation(__func_rename_at_933_329,__decl_rename_at_936_333,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_dfaac475e585cd10 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_940_334, char * const  __mod_name_rename_at_940_335, char * const  __ann_name_rename_at_940_336 )
{
    Annotation const  * __ann_rename_at_941_337; memset((void*)&__ann_rename_at_941_337,0,sizeof(__ann_rename_at_941_337));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_943_338; memset((void*)&__decl_rename_at_943_338,0,sizeof(__decl_rename_at_943_338));
    /* finally */ auto __finally_940= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_943_338);
    /* end finally */ });
    __ann_rename_at_941_337 = find_annotation_c19256528c6f4891(__context__,__mod_name_rename_at_940_335,__ann_name_rename_at_940_336);
    DAS_ASSERT((__ann_rename_at_941_337 != nullptr));
    __decl_rename_at_943_338; das_zero(__decl_rename_at_943_338); das_move(__decl_rename_at_943_338, append_annotation_5a18550a173ee340(__context__,__mod_name_rename_at_940_335,__ann_name_rename_at_940_336));
    addAndApplyBlockAnnotation(__blk_rename_at_940_334,__decl_rename_at_943_338,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_47fac1008de8360a ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_947_339, char * const  __mod_name_rename_at_947_340, char * const  __ann_name_rename_at_947_341 )
{
    Annotation const  * __ann_rename_at_948_342; memset((void*)&__ann_rename_at_948_342,0,sizeof(__ann_rename_at_948_342));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_950_343; memset((void*)&__decl_rename_at_950_343,0,sizeof(__decl_rename_at_950_343));
    /* finally */ auto __finally_947= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_950_343);
    /* end finally */ });
    __ann_rename_at_948_342 = find_annotation_c19256528c6f4891(__context__,__mod_name_rename_at_947_340,__ann_name_rename_at_947_341);
    DAS_ASSERT((__ann_rename_at_948_342 != nullptr));
    __decl_rename_at_950_343; das_zero(__decl_rename_at_950_343); das_move(__decl_rename_at_950_343, append_annotation_5a18550a173ee340(__context__,__mod_name_rename_at_947_340,__ann_name_rename_at_947_341));
    addAndApplyStructAnnotation(__st_rename_at_947_339,__decl_rename_at_950_343,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_4bdfb65e8a2b2d70 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_954_344, char * const  __mod_name_rename_at_954_345, char * const  __ann_name_rename_at_954_346, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_954_347 )
{
    Annotation const  * __ann_rename_at_955_348; memset((void*)&__ann_rename_at_955_348,0,sizeof(__ann_rename_at_955_348));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_957_349; memset((void*)&__decl_rename_at_957_349,0,sizeof(__decl_rename_at_957_349));
    /* finally */ auto __finally_954= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_957_349);
    /* end finally */ });
    __ann_rename_at_955_348 = find_annotation_c19256528c6f4891(__context__,__mod_name_rename_at_954_345,__ann_name_rename_at_954_346);
    DAS_ASSERT((__ann_rename_at_955_348 != nullptr));
    __decl_rename_at_957_349; das_zero(__decl_rename_at_957_349); das_move(__decl_rename_at_957_349, append_annotation_6df7f24b25adbbe9(__context__,__mod_name_rename_at_954_345,__ann_name_rename_at_954_346,__args_rename_at_954_347));
    addAndApplyFunctionAnnotation(__func_rename_at_954_344,__decl_rename_at_957_349,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_5c5029f094b33ed9 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_961_350, char * const  __mod_name_rename_at_961_351, char * const  __ann_name_rename_at_961_352, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_961_353 )
{
    Annotation const  * __ann_rename_at_962_354; memset((void*)&__ann_rename_at_962_354,0,sizeof(__ann_rename_at_962_354));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_964_355; memset((void*)&__decl_rename_at_964_355,0,sizeof(__decl_rename_at_964_355));
    /* finally */ auto __finally_961= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_964_355);
    /* end finally */ });
    __ann_rename_at_962_354 = find_annotation_c19256528c6f4891(__context__,__mod_name_rename_at_961_351,__ann_name_rename_at_961_352);
    DAS_ASSERT((__ann_rename_at_962_354 != nullptr));
    __decl_rename_at_964_355; das_zero(__decl_rename_at_964_355); das_move(__decl_rename_at_964_355, append_annotation_6df7f24b25adbbe9(__context__,__mod_name_rename_at_961_351,__ann_name_rename_at_961_352,__args_rename_at_961_353));
    addAndApplyBlockAnnotation(__blk_rename_at_961_350,__decl_rename_at_964_355,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_44c3a98ac2aa054f ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_968_356, char * const  __mod_name_rename_at_968_357, char * const  __ann_name_rename_at_968_358, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_968_359 )
{
    Annotation const  * __ann_rename_at_969_360; memset((void*)&__ann_rename_at_969_360,0,sizeof(__ann_rename_at_969_360));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_971_361; memset((void*)&__decl_rename_at_971_361,0,sizeof(__decl_rename_at_971_361));
    /* finally */ auto __finally_968= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_971_361);
    /* end finally */ });
    __ann_rename_at_969_360 = find_annotation_c19256528c6f4891(__context__,__mod_name_rename_at_968_357,__ann_name_rename_at_968_358);
    DAS_ASSERT((__ann_rename_at_969_360 != nullptr));
    __decl_rename_at_971_361; das_zero(__decl_rename_at_971_361); das_move(__decl_rename_at_971_361, append_annotation_6df7f24b25adbbe9(__context__,__mod_name_rename_at_968_357,__ann_name_rename_at_968_358,__args_rename_at_968_359));
    addAndApplyStructAnnotation(__st_rename_at_968_356,__decl_rename_at_971_361,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t add_annotation_argument_5bf2cbe639678fe9 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_975_362, char * const  __argName_rename_at_975_363, bool __val_rename_at_975_364 )
{
    int32_t __argIdx_rename_at_976_365 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_975_362),__argName_rename_at_975_363));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_975_362,__argIdx_rename_at_976_365,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tBool);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_975_362,__argIdx_rename_at_976_365,__context__).bValue /*bValue*/,__val_rename_at_975_364);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_976_365);
}

inline int32_t add_annotation_argument_d2c3575c5a39da7c ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_982_366, char * const  __argName_rename_at_982_367, int32_t __val_rename_at_982_368 )
{
    int32_t __argIdx_rename_at_983_369 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_982_366),__argName_rename_at_982_367));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_982_366,__argIdx_rename_at_983_369,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tInt);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_982_366,__argIdx_rename_at_983_369,__context__).iValue /*iValue*/,__val_rename_at_982_368);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_983_369);
}

inline int32_t add_annotation_argument_deb37309ab8a2522 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_989_370, char * const  __argName_rename_at_989_371, float __val_rename_at_989_372 )
{
    int32_t __argIdx_rename_at_990_373 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_989_370),__argName_rename_at_989_371));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_989_370,__argIdx_rename_at_990_373,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tFloat);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_989_370,__argIdx_rename_at_990_373,__context__).fValue /*fValue*/,__val_rename_at_989_372);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_990_373);
}

inline int32_t add_annotation_argument_f3e2ee746b536f08 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_996_374, char * const  __argName_rename_at_996_375, char * const  __val_rename_at_996_376 )
{
    int32_t __argIdx_rename_at_997_377 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_996_374),__argName_rename_at_996_375));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_996_374,__argIdx_rename_at_997_377,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tString);
    set_das_string(das_arg<das::string>::pass(das_index<AnnotationArgumentList>::at(__arguments_rename_at_996_374,__argIdx_rename_at_997_377,__context__).sValue /*sValue*/),__val_rename_at_996_376);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_997_377);
}

inline int32_t add_annotation_argument_2298c6271bc3206b ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1003_378, AnnotationArgument const  &  __ann_rename_at_1003_379 )
{
    int32_t __argIdx_rename_at_1004_380 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1003_378),((char * const )(to_das_string(__ann_rename_at_1003_379.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1003_378,__argIdx_rename_at_1004_380,__context__).type /*basicType*/,__ann_rename_at_1003_379.type /*basicType*/);
    if ( __ann_rename_at_1003_379.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1003_378,__argIdx_rename_at_1004_380,__context__).bValue /*bValue*/,__ann_rename_at_1003_379.bValue /*bValue*/);
    } else if ( __ann_rename_at_1003_379.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1003_378,__argIdx_rename_at_1004_380,__context__).iValue /*iValue*/,__ann_rename_at_1003_379.iValue /*iValue*/);
    } else if ( __ann_rename_at_1003_379.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1003_378,__argIdx_rename_at_1004_380,__context__).fValue /*fValue*/,__ann_rename_at_1003_379.fValue /*fValue*/);
    } else if ( __ann_rename_at_1003_379.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        das_clone<das::string,das::string>::clone(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1003_378,__argIdx_rename_at_1004_380,__context__).sValue /*sValue*/,__ann_rename_at_1003_379.sValue /*sValue*/);
    } else {
        DAS_ASSERTF((false),(((char *) "unsupported annotation type, add more types")));
    };
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1004_380);
}

inline int32_t get_for_source_index_c1882058c708b81b ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1020_381, smart_ptr_raw<Variable> const  __svar_rename_at_1020_382 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1021_41_32; _temp_make_local_1021_41_32;
    {
        bool __need_loop_1021 = true;
        // v: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __v_iterator(__expr_rename_at_1020_381->iteratorVariables /*iteratorVariables*/);
        smart_ptr_raw<Variable> const  * __v_rename_at_1021_385;
        __need_loop_1021 = __v_iterator.first(__context__,(__v_rename_at_1021_385)) && __need_loop_1021;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1021_41_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_1021_386;
        __need_loop_1021 = __t_iterator.first(__context__,(__t_rename_at_1021_386)) && __need_loop_1021;
        for ( ; __need_loop_1021 ; __need_loop_1021 = __v_iterator.next(__context__,(__v_rename_at_1021_385)) && __t_iterator.next(__context__,(__t_rename_at_1021_386)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__v_rename_at_1021_385)),das_auto_cast<smart_ptr_raw<void> const >::cast(__svar_rename_at_1020_382)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_1021_386);
            };
        }
        __v_iterator.close(__context__,(__v_rename_at_1021_385));
        __t_iterator.close(__context__,(__t_rename_at_1021_386));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline int32_t get_for_source_index_1517d199a978d69d ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1029_387, smart_ptr_raw<Expression> const  __source_rename_at_1029_388 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1030_31_32; _temp_make_local_1030_31_32;
    {
        bool __need_loop_1030 = true;
        // s: smart_ptr<ast::Expression> const&
        das_iterator<das::vector<smart_ptr<Expression>> const > __s_iterator(__expr_rename_at_1029_387->sources /*sources*/);
        smart_ptr_raw<Expression> const  * __s_rename_at_1030_391;
        __need_loop_1030 = __s_iterator.first(__context__,(__s_rename_at_1030_391)) && __need_loop_1030;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1030_31_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_1030_392;
        __need_loop_1030 = __t_iterator.first(__context__,(__t_rename_at_1030_392)) && __need_loop_1030;
        for ( ; __need_loop_1030 ; __need_loop_1030 = __s_iterator.next(__context__,(__s_rename_at_1030_391)) && __t_iterator.next(__context__,(__t_rename_at_1030_392)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__s_rename_at_1030_391)),das_auto_cast<smart_ptr_raw<void> const >::cast(__source_rename_at_1029_388)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_1030_392);
            };
        }
        __s_iterator.close(__context__,(__s_rename_at_1030_391));
        __t_iterator.close(__context__,(__t_rename_at_1030_392));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline smart_ptr_raw<TypeDecl> function_to_type_793f284cf58fe257 ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_1077_393 )
{
    smart_ptr_raw<TypeDecl> __td_rename_at_1078_394; memset((void*)&__td_rename_at_1078_394,0,sizeof(__td_rename_at_1078_394));
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1082_36_80; _temp_make_local_1082_36_80;
    /* finally */ auto __finally_1077= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__td_rename_at_1078_394);
    /* end finally */ });
    __td_rename_at_1078_394; das_zero(__td_rename_at_1078_394); das_move(__td_rename_at_1078_394, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1078) {
        das_copy((__mks_1078.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tFunction));
        das_move((__mks_1078.firstType /*firstType*/),(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_1077_393->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<smart_ptr_raw<TypeDecl>>::cast(clone_type(__fn_rename_at_1077_393->result /*result*/)) : das_auto_cast<smart_ptr_raw<TypeDecl>>::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1079) {
            das_copy((__mks_1079.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
        })))))));
    }))));
    das_vector_resize(das_arg<das::vector<das::string>>::pass(__td_rename_at_1078_394->argNames /*argNames*/),das_vector_length(__fn_rename_at_1077_393->arguments /*arguments*/));
    {
        bool __need_loop_1082 = true;
        // arg: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __arg_iterator(__fn_rename_at_1077_393->arguments /*arguments*/);
        smart_ptr_raw<Variable> const  * __arg_rename_at_1082_397;
        __need_loop_1082 = __arg_iterator.first(__context__,(__arg_rename_at_1082_397)) && __need_loop_1082;
        // argi: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1082_36_80 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __argi_iterator(0,1);
        int32_t __argi_rename_at_1082_398;
        __need_loop_1082 = __argi_iterator.first(__context__,(__argi_rename_at_1082_398)) && __need_loop_1082;
        for ( ; __need_loop_1082 ; __need_loop_1082 = __arg_iterator.next(__context__,(__arg_rename_at_1082_397)) && __argi_iterator.next(__context__,(__argi_rename_at_1082_398)) )
        {
            emplace_new_64d0d765e96319e1(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__td_rename_at_1078_394->argTypes /*argTypes*/),clone_type((*__arg_rename_at_1082_397)->type /*_type*/));
            das_clone<das::string,das::string>::clone(das_index<das::vector<das::string>>::at(__td_rename_at_1078_394->argNames /*argNames*/,__argi_rename_at_1082_398,__context__),(*__arg_rename_at_1082_397)->name /*name*/);
        }
        __arg_iterator.close(__context__,(__arg_rename_at_1082_397));
        __argi_iterator.close(__context__,(__argi_rename_at_1082_398));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(__td_rename_at_1078_394);
}

inline void visit_finally_7ef3ac0e30ffa4e9 ( Context * __context__, ExprBlock * const  __blk_rename_at_1089_399, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1089_400 )
{
    astVisitBlockFinally(das_cast<smart_ptr_raw<ExprBlock>>::cast(__blk_rename_at_1089_399),__adapter_rename_at_1089_400,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool isCMRES_6adc4be0ebbb0a22 ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1095_401 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1095_401->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1095_401->flags /*flags*/,1u << 6));
}

inline bool isCMRES_ff36804875e46671 ( Context * __context__, Function * const  __fun_rename_at_1099_402 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1099_402->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1099_402->flags /*flags*/,1u << 6));
}

inline bool isMakeLocal_5dd70dbec15d4405 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1103_403 )
{
    return das_auto_cast<bool>::cast(((((SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1103_403->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeLocal")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1103_403->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeStruct")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1103_403->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeVariant")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1103_403->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeArray")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1103_403->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeTuple")),*__context__,nullptr)));
}

inline bool isExprCallFunc_5dd70dbec15d4405 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1111_404 )
{
    return das_auto_cast<bool>::cast((((SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1111_404->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCallFunc")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1111_404->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprOp")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1111_404->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprNew")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1111_404->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCall")),*__context__,nullptr)));
}

inline TDim<DAS_COMMENT(bound_enum) das::Type,31> get_workhorse_types_a90ec7ad16111ce3 ( Context * __context__ )
{
    return das_auto_cast_ref<TDim<DAS_COMMENT(bound_enum) das::Type,31>>::cast((([&]() -> TDim<DAS_COMMENT(bound_enum) das::Type,31> {
        TDim<DAS_COMMENT(bound_enum) das::Type,31> __mka_1119;
        __mka_1119(0,__context__) = DAS_COMMENT(bound_enum) das::Type::tBool;
        __mka_1119(1,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt8;
        __mka_1119(2,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt8;
        __mka_1119(3,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt16;
        __mka_1119(4,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt16;
        __mka_1119(5,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt64;
        __mka_1119(6,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt64;
        __mka_1119(7,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration;
        __mka_1119(8,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration8;
        __mka_1119(9,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration16;
        __mka_1119(10,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration64;
        __mka_1119(11,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt;
        __mka_1119(12,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt2;
        __mka_1119(13,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt3;
        __mka_1119(14,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt4;
        __mka_1119(15,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt;
        __mka_1119(16,__context__) = DAS_COMMENT(bound_enum) das::Type::tBitfield;
        __mka_1119(17,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt2;
        __mka_1119(18,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt3;
        __mka_1119(19,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt4;
        __mka_1119(20,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat;
        __mka_1119(21,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat2;
        __mka_1119(22,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat3;
        __mka_1119(23,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat4;
        __mka_1119(24,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange;
        __mka_1119(25,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange;
        __mka_1119(26,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange64;
        __mka_1119(27,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange64;
        __mka_1119(28,__context__) = DAS_COMMENT(bound_enum) das::Type::tString;
        __mka_1119(29,__context__) = DAS_COMMENT(bound_enum) das::Type::tDouble;
        __mka_1119(30,__context__) = DAS_COMMENT(bound_enum) das::Type::tPointer;
        return __mka_1119;
    })()));
}

inline int32_t find_argument_index_c700ea639affec6e ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1131_405, char * const  __name_rename_at_1131_406 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1132_32_32; _temp_make_local_1132_32_32;
    {
        bool __need_loop_1132 = true;
        // a: $::das_string const&
        das_iterator<das::vector<das::string> const > __a_iterator(__typ_rename_at_1131_405->argNames /*argNames*/);
        das::string const  * __a_rename_at_1132_409;
        __need_loop_1132 = __a_iterator.first(__context__,(__a_rename_at_1132_409)) && __need_loop_1132;
        // ai: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1132_32_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __ai_iterator(0,1);
        int32_t __ai_rename_at_1132_410;
        __need_loop_1132 = __ai_iterator.first(__context__,(__ai_rename_at_1132_410)) && __need_loop_1132;
        for ( ; __need_loop_1132 ; __need_loop_1132 = __a_iterator.next(__context__,(__a_rename_at_1132_409)) && __ai_iterator.next(__context__,(__ai_rename_at_1132_410)) )
        {
            if ( eq_dstr_str((*__a_rename_at_1132_409),__name_rename_at_1131_406) )
            {
                return das_auto_cast<int32_t>::cast(__ai_rename_at_1132_410);
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_1132_409));
        __ai_iterator.close(__context__,(__ai_rename_at_1132_410));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline bool isCMRESType_cb4539c6e62e1c89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1140_411 )
{
    return das_auto_cast<bool>::cast(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__blockT_rename_at_1140_411),das_auto_cast<void * const >::cast(nullptr))) && ((das_deref(__context__,__blockT_rename_at_1140_411)).isRefType())) && !(das_get_bitfield(__blockT_rename_at_1140_411->flags /*flags*/,1u << 0)));
}

inline void debug_expression_impl_e624caf7951258cb ( Context * __context__, StringBuilderWriter &  __writer_rename_at_1148_412, smart_ptr_raw<Expression> const  __expr_rename_at_1148_413, Bitfield __deFlags_rename_at_1148_414, int32_t __tabs_rename_at_1148_415 ) { das_stack_prologue __prologue(__context__,528,"debug_expression_impl " DAS_FILE_LINE);
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1148_413),das_auto_cast<void * const >::cast(nullptr)) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_15,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) "null"))));
        return ;
    } else {
        if ( builtin_empty(__expr_rename_at_1148_413->__rtti /*__rtti*/) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_16,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) "[NO RTTI]"))));
            return ;
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_18, cast<char * const >::from(((char *) "(")), cast<char * const >::from(__expr_rename_at_1148_413->__rtti /*__rtti*/))))));
            if ( das_get_bitfield(__deFlags_rename_at_1148_414,1u << 0) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_19,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_20, cast<char * const >::from(((char *) "<")), cast<uint32_t>::from(builtin_smart_ptr_use_count(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1148_413),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))), cast<char * const >::from(((char *) ">")))))));
            };
            if ( builtin_string_startswith(((char * const )(pass_string(__expr_rename_at_1148_413->__rtti /*__rtti*/))),((char *) "ExprConst"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_21,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_22, cast<char * const >::from(((char *) " ")), cast<char * const >::from(_FuncastTickdescribeTick842554968825501494_e6a33d3e7995fe10(__context__,__expr_rename_at_1148_413)), cast<char * const >::from(((char *) ")")))))));
                return ;
            } else {
                BasicStructureAnnotation * __ann_rename_at_1165_416 = das_cast<BasicStructureAnnotation *>::cast(get_expression_annotation(_FuncbuiltinTickget_ptrTick8468476673553620226_9c918ef85c4c40f9(__context__,__expr_rename_at_1148_413),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
                if ( __ann_rename_at_1165_416 == nullptr )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_23,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) ")"))));
                    return ;
                } else {
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1165_416)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,96,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_1170_417, char * __cppName_rename_at_1170_418, smart_ptr_raw<TypeDecl> __xtype_rename_at_1170_419, uint32_t __offset_rename_at_1170_420) -> void{
                        if ( __offset_rename_at_1170_420 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1172_421 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_b1fd376d21aa823f(__context__,__xtype_rename_at_1170_419,true,true,true)));
                            int8_t * __p8_rename_at_1173_422 = 0;
                            das_copy(__p8_rename_at_1173_422,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1148_413),int32_t(__offset_rename_at_1170_420),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1172_421),cast<char *>::from(((char *) "smart_ptr<ast::Expression>")),*__context__,nullptr) )
                            {
                                smart_ptr_raw<Expression> * __pv_rename_at_1178_423 = ((smart_ptr_raw<Expression> *)das_cast<smart_ptr_raw<Expression> *>::cast(__p8_rename_at_1173_422));
                                char * __ts_rename_at_1179_424 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1148_415 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_24,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_25, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1179_424), cast<char * const >::from(((char *) "(")), cast<char *>::from(__name_rename_at_1170_417), cast<char * const >::from(((char *) " ")))))));
                                debug_expression_impl_e624caf7951258cb(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1148_412),das_deref(__context__,__pv_rename_at_1178_423),__deFlags_rename_at_1148_414,__tabs_rename_at_1148_415 + 2);
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_26,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) ")"))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1172_421),cast<char *>::from(((char *) "$::das_string")),*__context__,nullptr) )
                            {
                                das::string * __pv_rename_at_1184_425 = ((das::string *)das_cast<das::string *>::cast(__p8_rename_at_1173_422));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_27,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_28, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1170_417), cast<char * const >::from(((char *) "=\"")), cast<das::string const  &>::from(das_deref(__context__,__pv_rename_at_1184_425)), cast<char * const >::from(((char *) "\"")))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1172_421),cast<char *>::from(((char *) "int")),*__context__,nullptr) )
                            {
                                int32_t * __pv_rename_at_1187_426 = ((int32_t *)das_cast<int32_t *>::cast(__p8_rename_at_1173_422));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_29,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_30, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1170_417), cast<char * const >::from(((char *) "=")), cast<int32_t>::from(das_deref(__context__,__pv_rename_at_1187_426)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1172_421),cast<char *>::from(((char *) "uint")),*__context__,nullptr) )
                            {
                                uint32_t * __pv_rename_at_1190_427 = ((uint32_t *)das_cast<uint32_t *>::cast(__p8_rename_at_1173_422));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_31,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_32, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1170_417), cast<char * const >::from(((char *) "=")), cast<uint32_t>::from(das_deref(__context__,__pv_rename_at_1190_427)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1172_421),cast<char *>::from(((char *) "int64")),*__context__,nullptr) )
                            {
                                int64_t * __pv_rename_at_1193_428 = ((int64_t *)das_cast<int64_t *>::cast(__p8_rename_at_1173_422));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_33,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_34, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1170_417), cast<char * const >::from(((char *) "=")), cast<int64_t>::from(das_deref(__context__,__pv_rename_at_1193_428)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1172_421),cast<char *>::from(((char *) "uint64")),*__context__,nullptr) )
                            {
                                uint64_t * __pv_rename_at_1196_429 = ((uint64_t *)das_cast<uint64_t *>::cast(__p8_rename_at_1173_422));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_35,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_36, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1170_417), cast<char * const >::from(((char *) "=")), cast<uint64_t>::from(das_deref(__context__,__pv_rename_at_1196_429)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1172_421),cast<char *>::from(((char *) "bool")),*__context__,nullptr) )
                            {
                                bool * __pv_rename_at_1199_430 = ((bool *)das_cast<bool *>::cast(__p8_rename_at_1173_422));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_37,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_38, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1170_417), cast<char * const >::from(((char *) "=")), cast<bool>::from(das_deref(__context__,__pv_rename_at_1199_430)))))));
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1165_416)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,320,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_1209_431, char * __cppName_rename_at_1209_432, smart_ptr_raw<TypeDecl> __xtype_rename_at_1209_433, uint32_t __offset_rename_at_1209_434) -> void{
                        if ( __offset_rename_at_1209_434 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1211_435 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_b1fd376d21aa823f(__context__,__xtype_rename_at_1209_433,true,true,true)));
                            int8_t * __p8_rename_at_1212_436 = 0;
                            das_copy(__p8_rename_at_1212_436,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1148_413),int32_t(__offset_rename_at_1209_434),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1211_435),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Expression")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Expression>> * __pv_rename_at_1217_437 = ((das::vector<smart_ptr<Expression>> *)das_cast<das::vector<smart_ptr<Expression>> *>::cast(__p8_rename_at_1212_436));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1217_437)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1221_38_400; _temp_make_local_1221_38_400;
                                    char * __ts_rename_at_1219_438 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1148_415 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_39,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_40, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1219_438), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1209_431), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1221 = true;
                                        // l: smart_ptr<ast::Expression> const&
                                        das_iterator<das::vector<smart_ptr<Expression>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1217_437));
                                        smart_ptr_raw<Expression> const  * __l_rename_at_1221_441;
                                        __need_loop_1221 = __l_iterator.first(__context__,(__l_rename_at_1221_441)) && __need_loop_1221;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1221_38_400 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1221_442;
                                        __need_loop_1221 = __i_iterator.first(__context__,(__i_rename_at_1221_442)) && __need_loop_1221;
                                        for ( ; __need_loop_1221 ; __need_loop_1221 = __l_iterator.next(__context__,(__l_rename_at_1221_441)) && __i_iterator.next(__context__,(__i_rename_at_1221_442)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_41,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_42, cast<char * const >::from(__ts_rename_at_1219_438), cast<char * const >::from(((char *) "  ")))))));
                                            debug_expression_impl_e624caf7951258cb(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1148_412),(*__l_rename_at_1221_441),__deFlags_rename_at_1148_414,__tabs_rename_at_1148_415 + 2);
                                            if ( __i_rename_at_1221_442 != (das_vector_length(das_deref(__context__,__pv_rename_at_1217_437)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_43,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __l_iterator.close(__context__,(__l_rename_at_1221_441));
                                        __i_iterator.close(__context__,(__i_rename_at_1221_442));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_44,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) "]"))));
                                };
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1211_435),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Variable")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Variable>> * __pv_rename_at_1231_443 = ((das::vector<smart_ptr<Variable>> *)das_cast<das::vector<smart_ptr<Variable>> *>::cast(__p8_rename_at_1212_436));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1231_443)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1235_38_480; _temp_make_local_1235_38_480;
                                    char * __ts_rename_at_1233_444 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1148_415 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_45,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_46, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1233_444), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1209_431), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1235 = true;
                                        // l: smart_ptr<ast::Variable> const&
                                        das_iterator<das::vector<smart_ptr<Variable>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1231_443));
                                        smart_ptr_raw<Variable> const  * __l_rename_at_1235_447;
                                        __need_loop_1235 = __l_iterator.first(__context__,(__l_rename_at_1235_447)) && __need_loop_1235;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1235_38_480 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1235_448;
                                        __need_loop_1235 = __i_iterator.first(__context__,(__i_rename_at_1235_448)) && __need_loop_1235;
                                        for ( ; __need_loop_1235 ; __need_loop_1235 = __l_iterator.next(__context__,(__l_rename_at_1235_447)) && __i_iterator.next(__context__,(__i_rename_at_1235_448)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_47,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_48, cast<char * const >::from(__ts_rename_at_1233_444), cast<char * const >::from(((char *) "  ")))))));
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_49,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_50, cast<das::string const  &>::from((*__l_rename_at_1235_447)->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_b1fd376d21aa823f(__context__,(*__l_rename_at_1235_447)->type /*_type*/,true,true,true)))))));
                                            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__l_rename_at_1235_447)->init /*init*/),das_auto_cast<void * const >::cast(nullptr)) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_51,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) " = "))));
                                                debug_expression_impl_e624caf7951258cb(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1148_412),(*__l_rename_at_1235_447)->init /*init*/,__deFlags_rename_at_1148_414,__tabs_rename_at_1148_415 + 2);
                                            };
                                            if ( __i_rename_at_1235_448 != (das_vector_length(das_deref(__context__,__pv_rename_at_1231_443)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_52,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __l_iterator.close(__context__,(__l_rename_at_1235_447));
                                        __i_iterator.close(__context__,(__i_rename_at_1235_448));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_53,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) "]"))));
                                };
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_54,cast<StringBuilderWriter &>::from(__writer_rename_at_1148_412),cast<char * const >::from(((char *) ")"))));
                };
            };
        };
    };
}}

inline char * debug_expression_46b4fe8de76ec412 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1254_449, Bitfield __deFlags_rename_at_1254_450 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1255_451) DAS_AOT_INLINE_LAMBDA -> void{
        debug_expression_impl_e624caf7951258cb(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1255_451),__expr_rename_at_1254_449,__deFlags_rename_at_1254_450,0);
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * debug_expression_ba409c75004d2596 ( Context * __context__, Expression * const  __expr_rename_at_1260_452 )
{
    return das_auto_cast<char *>::cast(debug_expression_46b4fe8de76ec412(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1260_452),0x0u));
}

inline char * describe_ba409c75004d2596 ( Context * __context__, Expression * const  __expr_rename_at_1266_453 )
{
    return das_auto_cast<char *>::cast(_FuncastTickdescribeTick842554968825501494_e6a33d3e7995fe10(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1266_453)));
}

inline int32_t getVectorElementCount_712a3c4e8dfb5421 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1272_454 )
{
    return das_auto_cast<int32_t>::cast((((((((__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tInt2)) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(2) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tFloat3) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) ? das_auto_cast<int32_t>::cast(3) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tFloat4) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1272_454 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) ? das_auto_cast<int32_t>::cast(4) : das_auto_cast<int32_t>::cast(0))))));
}

inline int32_t getVectorElementSize_712a3c4e8dfb5421 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1279_455 )
{
    return das_auto_cast<int32_t>::cast(((__bt_rename_at_1279_455 == DAS_COMMENT(bound_enum) das::Type::tRange64) || (__bt_rename_at_1279_455 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(8) : das_auto_cast<int32_t>::cast(4));
}

inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_b00d209bec8bc9fd ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1284_456 )
{
    return das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tFloat) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tRange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tUInt2) || (__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tURange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tRange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1284_456 == DAS_COMMENT(bound_enum) das::Type::tURange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tVoid))))))))));
}

inline int32_t getVectorOffset_f53fa57fd6e226fb ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1293_457, char * const  __ident_rename_at_1293_458 )
{
    int32_t __ofs_rename_at_1294_459 = -1;
    if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "x")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "X")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "r")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "R")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1294_459,0);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "y")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "Y")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "g")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "G")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1294_459,1);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "z")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "Z")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "b")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "B")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1294_459,2);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "w")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "W")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "a")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1293_458),cast<char *>::from(((char *) "A")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1294_459,3);
    };
    int32_t __count_rename_at_1304_460 = ((int32_t)getVectorElementCount_712a3c4e8dfb5421(__context__,__bt_rename_at_1293_457));
    return das_auto_cast<int32_t>::cast((__ofs_rename_at_1294_459 >= __count_rename_at_1304_460) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(__ofs_rename_at_1294_459));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x5cf5aa438da16a2b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_577e5c7310ce42>>();
    };
    aotLib[0x905d66e77028e728] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_b52298649fd5c67b>>();
    };
    aotLib[0x887ce6e0ce919940] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_6114197bfba716e9>>();
    };
    aotLib[0x3bf923f191424a6f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_d8d4a649436ee872>>();
    };
    aotLib[0x4505bb7d4dac4c2c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_3dc324a94b22dc94>>();
    };
    aotLib[0xf5d5ed2351a9bd41] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_ada54212b7f744a6>>();
    };
    aotLib[0x7b926bd54b4d2551] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_d5f5b99ec073834e>>();
    };
    aotLib[0xd0015dbf440dd2de] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_bf8bd9eebb3a2d23>>();
    };
    aotLib[0xd607690784301f70] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_7e21e784fc551687>>();
    };
    aotLib[0xd414928e3aad7511] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_15d783f7e9c7ea7a>>();
    };
    aotLib[0x6fd3202f1370ed18] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_9300fad43e61790a>>();
    };
    aotLib[0xa9f60098185ecd45] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_bda902199373ea0>>();
    };
    aotLib[0x28d8ee99d978da63] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_170bf2ffb5736ec4>>();
    };
    aotLib[0x16dda59942da14bf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_b8586b2d29046ffa>>();
    };
    aotLib[0x8f010e60f4eb870e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_80c16e0c2632db9b>>();
    };
    aotLib[0xa95037ab4d15dc60] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_6b4b3f0ecc112b40>>();
    };
    aotLib[0xd61c6c6c11562fab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_101e4eb21b625567>>();
    };
    aotLib[0xc2dd3eaca2a3d566] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_591efe2bffa234e>>();
    };
    aotLib[0x5906152367220de0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_73d636384df024a7>>();
    };
    aotLib[0xe455ad5e659f2ca6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_aec7f4795a71d05e>>();
    };
    aotLib[0xc3c2b12a8caada88] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_4f45e677a4b772d8>>();
    };
    aotLib[0x2c51ef94588bf7ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_7a6a7aa50553599a>>();
    };
    aotLib[0x1ffcb9dab232aba3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_7edb518827a4cdad>>();
    };
    aotLib[0xdb57f119ac0018c8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_dc816cef426a7f26>>();
    };
    aotLib[0x9fbd33ec69d260a6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_5355aa25e23f93e6>>();
    };
    aotLib[0xec499a6d860ce733] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_bea46e5118b8a97f>>();
    };
    aotLib[0xe2a4b399d27e02d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_585b805c28f303bc>>();
    };
    aotLib[0xf32ef025ea3efade] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_a8a561a0ef667b7>>();
    };
    aotLib[0x61ca07d134788eb4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_a3509efc10f05106>>();
    };
    aotLib[0x60b94dae93713b9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickfind_moduleTick11101329256228773934_d842461399a3c7ba>>();
    };
    aotLib[0xa80b66e78aadc363] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_c1a84a72f7e61d2c>>();
    };
    aotLib[0xdc7feac4bba2cae5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_976f9f8d3c51340f>>();
    };
    aotLib[0xd0a538146475eeb9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_70f04f60487df041>>();
    };
    aotLib[0xba1705855e0a9b18] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_9f199f97decbabbe>>();
    };
    aotLib[0x6e879a4cfaef420] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_1445ed0430a8bb9b>>();
    };
    aotLib[0xfcc66786e6a2846c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick16475640899284277631_daeec2492818d4d1>>();
    };
    aotLib[0x4e016d424a2f6b73] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_c66cc804629e0825>>();
    };
    aotLib[0x24292ddba29facd7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpush_cloneTick2035469273396957942_18f7d5397c9c7e3>>();
    };
    aotLib[0x6753d309a3adf600] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_7cde73d852192941>>();
    };
    aotLib[0x3dea08c2fb9c6da3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_f2eaefba27105789>>();
    };
    aotLib[0x7eeeb338ef74a114] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_b1fd376d21aa823f>>();
    };
    aotLib[0xf637e86c1d536cfe] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_130b6b8d46be3007>>();
    };
    aotLib[0xc3a226ccfcaa1419] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncrttiTickRttiValue_nothingTick4715542659269841615_4da357be30f10f8d>>();
    };
    aotLib[0xc905231e2084939f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_1836be9fc5e8fc93>>();
    };
    aotLib[0xbd3945acd39d4c8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_899221f202686a02>>();
    };
    aotLib[0x2815c67fa81f4747] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_7477174a10d67388>>();
    };
    aotLib[0x38616672735133e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_e3597b8badb6cad2>>();
    };
    aotLib[0xc1b49f19aecf23b2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_7e735142241eeece>>();
    };
    aotLib[0x29dac0774847674] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_46d70cd8acd38c11>>();
    };
    aotLib[0x820c08d52b5997ba] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_692a562a355a89d>>();
    };
    aotLib[0x90966d345adb4517] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickfind_compiling_moduleTick4523452840392654583_bd462bbf8da5b2bf>>();
    };
    aotLib[0xe670b899a024e575] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_f708d7eb74451775>>();
    };
    aotLib[0xe8fc9b02126590af] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick842554968825501494_e6a33d3e7995fe10>>();
    };
    aotLib[0x478d2ee7f4bf1120] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_9c918ef85c4c40f9>>();
    };
    aotLib[0xbab2cab2b0cc9665] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_e550d220fd0c5d56>>();
    };
    aotLib[0x5e9daad2eeb42ddd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_4e7109a5607458d9>>();
    };
    aotLib[0x9c3554222b2eaefc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_983de54be7cdf35d>>();
    };
    aotLib[0x4345f96cfc3b3fef] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_4a8f1905102d3a4d>>();
    };
    aotLib[0xc8e908ecbeae3c96] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isVectorType_2f119d66db3670e0>>();
    };
    aotLib[0x38d4de99d3fa10b0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_b10821fc8be60048>>();
    };
    aotLib[0x65426b795324681f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_de6262ea55644562>>();
    };
    aotLib[0x8451275fa7f98c96] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_9df8440d80d5b168>>();
    };
    aotLib[0x8ee32316fa74d423] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_aa8f12fd84e4c466>>();
    };
    aotLib[0x123edabb11fdbb57] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isExpression_8997b287eaa15b6>>();
    };
    aotLib[0xf7bd6e8ec135300c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_same_or_inherited_468337318a989d6a>>();
    };
    aotLib[0x4646d9a79d711596] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_class_method_9235fd3f9e8a4f9f>>();
    };
    aotLib[0x4a09548797dfea71] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&emplace_new_e80b751cadf868b4>>();
    };
    aotLib[0x7166f67ef077f031] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&emplace_new_64d0d765e96319e1>>();
    };
    aotLib[0x8494919705316c5a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&emplace_new_edb6c239444b3d2a>>();
    };
    aotLib[0x23fcf0d6e3d0f8c8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&emplace_new_63295e37889c520a>>();
    };
    aotLib[0x3e24a14ec52cb94] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&override_method_3856620b83eb08d5>>();
    };
    aotLib[0x45f64822043d6387] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&for_each_tag_function_da829e21b9c7f2ff>>();
    };
    aotLib[0xf6b5a6e7765df57f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&find_arg_d980886dc0e2db90>>();
    };
    aotLib[0xbd0b9ee2190b2072] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&find_arg_a35a5e6fbc8f2644>>();
    };
    aotLib[0x458548b757fa1007] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&apply_tag_annotation_5141104131781cdd>>();
    };
    aotLib[0x8aa433cf71e74488] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_unique_function_fa121c0afbb73b96>>();
    };
    aotLib[0x8aa433cf71e74488] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_unique_generic_fa121c0afbb73b96>>();
    };
    aotLib[0xe75696f0d7539fe6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&setup_call_list_6f07c8272f142ab7>>();
    };
    aotLib[0x11e8d842196370aa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&setup_call_list_bc9603dfbe371e25>>();
    };
    aotLib[0x21672e5f8d5433f0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&setup_macro_8a720e08f64feaa2>>();
    };
    aotLib[0xbdc468c9520bfe0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&panic_expr_as_594c2d3529d6030b>>();
    };
    aotLib[0x7eb5735c9fb66031] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickBuiltInFunction_ff36804875e46671>>();
    };
    aotLib[0x5410b9e799ec14c0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickBuiltInFunction_31b52fee11dc1291>>();
    };
    aotLib[0x7eb5735c9fb66031] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickExternalFnBase_ff36804875e46671>>();
    };
    aotLib[0x40a4d2714c57cbf9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickExternalFnBase_e366761490f25f61>>();
    };
    aotLib[0x969e725f9eefbe03] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickFunctionAnnotation_e8ecf59c01f982e2>>();
    };
    aotLib[0x3d8018490bc529da] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickFunctionAnnotation_4c5cbfd429c14538>>();
    };
    aotLib[0x2d2f82487060f2f7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickFunctionAnnotation_ff32b81c03ca0ab7>>();
    };
    aotLib[0x4c6135eeaa3a8f10] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickFunctionAnnotation_9eed8a3c62d29bc1>>();
    };
    aotLib[0x969e725f9eefbe03] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickStructureAnnotation_e8ecf59c01f982e2>>();
    };
    aotLib[0x3d8018490bc529da] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickStructureAnnotation_4c5cbfd429c14538>>();
    };
    aotLib[0x7540b99c304541ac] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickStructureAnnotation_775bba9924ff4625>>();
    };
    aotLib[0xf7ef5f98098dade1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickStructureAnnotation_511c2a8dbf5866df>>();
    };
    aotLib[0x7152affd9dfe54e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_array_80eb2f001ccee1a8>>();
    };
    aotLib[0x7152affd9dfe54e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_dim_80eb2f001ccee1a8>>();
    };
    aotLib[0x7152affd9dfe54e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_pointer_80eb2f001ccee1a8>>();
    };
    aotLib[0x7152affd9dfe54e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_tuple_80eb2f001ccee1a8>>();
    };
    aotLib[0x7152affd9dfe54e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_structure_80eb2f001ccee1a8>>();
    };
    aotLib[0x7152affd9dfe54e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_variant_80eb2f001ccee1a8>>();
    };
    aotLib[0x7152affd9dfe54e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_table_80eb2f001ccee1a8>>();
    };
    aotLib[0x8be83b6da3d42f77] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_basic_80eb2f001ccee1a8>>();
    };
    aotLib[0x1c2a00bc88c5f99f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_enumeration_80eb2f001ccee1a8>>();
    };
    aotLib[0x7152affd9dfe54e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_80eb2f001ccee1a8>>();
    };
    aotLib[0x41fa28acb244a358] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_annotation_c19256528c6f4891>>();
    };
    aotLib[0xa759793003b111fb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_6df7f24b25adbbe9>>();
    };
    aotLib[0xbbeeb32ec6fe6910] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_5a18550a173ee340>>();
    };
    aotLib[0x341925575ba50c9c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_e9a416b646100dc5>>();
    };
    aotLib[0x49ecf8f43952069e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_dfaac475e585cd10>>();
    };
    aotLib[0x1b2d7837e931313f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_47fac1008de8360a>>();
    };
    aotLib[0xd607c51674e9ced6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_4bdfb65e8a2b2d70>>();
    };
    aotLib[0xb69d4a99404f65ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_5c5029f094b33ed9>>();
    };
    aotLib[0xe78b30770bcf6140] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_44c3a98ac2aa054f>>();
    };
    aotLib[0xf343ac2054a6649b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_5bf2cbe639678fe9>>();
    };
    aotLib[0x1555492383bb2e63] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_d2c3575c5a39da7c>>();
    };
    aotLib[0x304275d6c6d08efa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_deb37309ab8a2522>>();
    };
    aotLib[0xd6086819f7af84f4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_f3e2ee746b536f08>>();
    };
    aotLib[0xe98e8fcc74ad047d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_2298c6271bc3206b>>();
    };
    aotLib[0x28294125889a156b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&get_for_source_index_c1882058c708b81b>>();
    };
    aotLib[0x48950dc07a88172c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&get_for_source_index_1517d199a978d69d>>();
    };
    aotLib[0x983016f50b563543] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&function_to_type_793f284cf58fe257>>();
    };
    aotLib[0xa071bc48d3e81071] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&visit_finally_7ef3ac0e30ffa4e9>>();
    };
    aotLib[0x25f8f8cfa550b543] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isCMRES_6adc4be0ebbb0a22>>();
    };
    aotLib[0x7eb5735c9fb66031] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isCMRES_ff36804875e46671>>();
    };
    aotLib[0x38da2a7bf81a7551] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isMakeLocal_5dd70dbec15d4405>>();
    };
    aotLib[0x38da2a7bf81a7551] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isExprCallFunc_5dd70dbec15d4405>>();
    };
    aotLib[0xf60d36d828930712] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&get_workhorse_types_a90ec7ad16111ce3>>();
    };
    aotLib[0xa70afb12d2c64c78] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_argument_index_c700ea639affec6e>>();
    };
    aotLib[0xf65c12f09601c578] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isCMRESType_cb4539c6e62e1c89>>();
    };
    aotLib[0x70d6b86036730a22] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_expression_impl_e624caf7951258cb>>();
    };
    aotLib[0x27da6ca135b09321] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_expression_46b4fe8de76ec412>>();
    };
    aotLib[0xad267f4d6739e5a7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_expression_ba409c75004d2596>>();
    };
    aotLib[0x2f0ddc2cc86282a7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_ba409c75004d2596>>();
    };
    aotLib[0x64aa64b2842cf012] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getVectorElementCount_712a3c4e8dfb5421>>();
    };
    aotLib[0x64aa64b2842cf012] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getVectorElementSize_712a3c4e8dfb5421>>();
    };
    aotLib[0x92f3bfe52a0cc46a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getVectorElementType_b00d209bec8bc9fd>>();
    };
    aotLib[0x516040b51088ebc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getVectorOffset_f53fa57fd6e226fb>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_24078389025493969
AotListBase impl_aot_ast_boost(_anon_24078389025493969::registerAotFunctions);
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
