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
namespace _anon_9859992971649349273 {

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
namespace ast_boost { struct CaptureEntryInitData; };
namespace ast_boost { struct AnnotationArgumentInitData; };
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
#if 0 // external enum
namespace ast {

enum class CaptureMode : int32_t {
    capture_any = int32_t(INT64_C(0)),
    capture_by_copy = int32_t(INT64_C(1)),
    capture_by_reference = int32_t(INT64_C(2)),
    capture_by_clone = int32_t(INT64_C(3)),
    capture_by_move = int32_t(INT64_C(4)),
};
}
#endif // external enum
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
namespace ast_boost {

struct CaptureEntryInitData {
    char * name;
    DAS_COMMENT(bound_enum) das::CaptureMode mode;
};
}
namespace ast_boost {

struct AnnotationArgumentInitData {
    DAS_COMMENT(bound_enum) das::Type basicType;
    char * name;
    char * sValue;
    bool bValue;
    int32_t iValue;
    float fValue;
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
TypeInfo * __tinfo_15[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_16[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_17[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_18[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_19[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_20[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_21[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_22[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_23[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_24[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_25[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_26[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_27[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_28[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_29[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_30[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_31[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_32[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_33[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_34[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_35[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec };
TypeInfo * __tinfo_36[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_37[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__b68d800849332aec };
TypeInfo * __tinfo_38[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_39[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af81fe4c86352052 };
TypeInfo * __tinfo_40[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_41[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_42[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_43[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_44[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_45[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_46[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_47[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_48[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_49[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_50[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_51[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_52[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_53[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_54[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_55[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_1e3552d3bc6b6de6 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_5fb0f8e94e78667b ( Context * __context__, TDim<smart_ptr_raw<Expression>,2> const  & __a_rename_at_586_2 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_8f6af4333a24497d ( Context * __context__, TDim<smart_ptr_raw<Expression>,1> const  & __a_rename_at_586_3 );
inline void _FuncbuiltinTickpushTick10769833213962245646_f2e12ba33c8e5800 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_4, ast::AstFunctionAnnotation * __value_rename_at_181_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_dc38b502bf9875cd ( Context * __context__, ast_boost::MacroMacro const  & __cl_rename_at_116_6 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_33dd2607299b73e2 ( Context * __context__, ast_boost::TagFunctionAnnotation const  & __cl_rename_at_116_7 );
inline void _FuncbuiltinTickpushTick10769833213962245646_e0f0db26c9bddfb9 ( Context * __context__, TArray<ast::AstStructureAnnotation *> & __Arr_rename_at_181_8, ast::AstStructureAnnotation * __value_rename_at_181_9 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_25cb48d0685fc01e ( Context * __context__, ast_boost::SetupFunctionAnnotation const  & __cl_rename_at_116_10 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a4299e2b282e186f ( Context * __context__, ast_boost::SetupBlockAnnotation const  & __cl_rename_at_116_11 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_eacbdf6c5c838463 ( Context * __context__, ast_boost::SetupStructureAnnotation const  & __cl_rename_at_116_12 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7bda8577897747f3 ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  & __cl_rename_at_116_13 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1040c47ee99273dc ( Context * __context__, ast_boost::SetupContractAnnotation const  & __cl_rename_at_116_14 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_22b4bbfa1962edcb ( Context * __context__, ast_boost::SetupReaderMacro const  & __cl_rename_at_116_15 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_318dbb9fe3ad8d26 ( Context * __context__, ast_boost::SetupCommentReader const  & __cl_rename_at_116_16 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f9e84a1132cd421c ( Context * __context__, ast_boost::SetupCallMacro const  & __cl_rename_at_116_17 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_598afebfc43d1b82 ( Context * __context__, ast_boost::SetupTypeInfoMacro const  & __cl_rename_at_116_18 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b7283bfd4d6912a ( Context * __context__, ast_boost::SetupVariantMacro const  & __cl_rename_at_116_19 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d424ba6952970d20 ( Context * __context__, ast_boost::SetupForLoopMacro const  & __cl_rename_at_116_20 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c64c64f8ce19c4f1 ( Context * __context__, ast_boost::SetupCaptureMacro const  & __cl_rename_at_116_21 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_fd693b0656e58d42 ( Context * __context__, ast_boost::SetupTypeMacro const  & __cl_rename_at_116_22 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cc5e722ec5d18084 ( Context * __context__, ast_boost::SetupSimulateMacro const  & __cl_rename_at_116_23 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5ecbb6bf9d3c5ce5 ( Context * __context__, ast_boost::TagStructureAnnotation const  & __cl_rename_at_116_24 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_86a7b966da7cc372 ( Context * __context__, ast_boost::TagFunctionMacro const  & __cl_rename_at_116_25 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_10296bf9fbc53845 ( Context * __context__, ast_boost::SetupInferMacro const  & __cl_rename_at_116_26 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_efc8b692d683dde6 ( Context * __context__, ast_boost::SetupDirtyInferMacro const  & __cl_rename_at_116_27 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_135df9c4c86bd47a ( Context * __context__, ast_boost::SetupOptimizationMacro const  & __cl_rename_at_116_28 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d80d89f4e2ee24a ( Context * __context__, ast_boost::SetupLintMacro const  & __cl_rename_at_116_29 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_747b059c62e5428d ( Context * __context__, ast_boost::SetupGlobalLintMacro const  & __cl_rename_at_116_30 );
inline void _FuncbuiltinTickpushTick10769833213962245646_1f060b3476600b3f ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_181_31, ast::AstVariantMacro * __value_rename_at_181_32 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3c026b31961c9c97 ( Context * __context__, ast_boost::BetterRttiVisitor const  & __cl_rename_at_116_33 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_bfb1abbb07340073 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_68_34, int32_t __newSize_rename_at_68_35 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_e945fcf444587b9b ( Context * __context__, TArray<char *> & __Arr_rename_at_68_36, int32_t __newSize_rename_at_68_37 );
inline void clone_be3fb1336b24ed81 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & __dest_rename_at_1050_38, smart_ptr_raw<FunctionAnnotation> const  __src_rename_at_1050_39 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_72481d9703572422 ( Context * __context__, TDim<AutoTuple<char *,char *>,16> const  & __a_rename_at_586_40 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_6b6c8f819c74b7dd ( Context * __context__, TDim<char *,6> const  & __a_rename_at_586_41 );
inline TArray<smart_ptr_raw<Expression>> _FuncbuiltinTickto_array_moveTick3185538323411982277_574cffbb66f47535 ( Context * __context__, TDim<smart_ptr_raw<Expression>,2> & __a_rename_at_1352_42 );
inline TArray<smart_ptr_raw<Expression>> _FuncbuiltinTickto_array_moveTick3185538323411982277_5767cf0f8c77bc34 ( Context * __context__, TDim<smart_ptr_raw<Expression>,1> & __a_rename_at_1352_44 );
inline smart_ptr_raw<FunctionAnnotation> _FuncbuiltinTickclone_to_moveTick2007252383599261567_c0fff2b1179fc9e2 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> const  __clone_src_rename_at_1047_46 );
inline void clone_7f0eb2e30132a5ec ( Context * __context__, smart_ptr_raw<ExprMakeArray> & __dest_rename_at_1041_48, smart_ptr_raw<ExprMakeArray> const  __src_rename_at_1041_49 );
inline Module * _FuncastTickfind_moduleTick11101329256228773934_f6b4e714927216e3 ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_948_50, char * const  __name_rename_at_948_51 );
inline void _FuncbuiltinTickpushTick10769833213962245646_ff37717a0d8d2276 ( Context * __context__, TArray<char *> & __Arr_rename_at_181_54, char * __value_rename_at_181_55 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_7c54c515f2166fd4 ( Context * __context__, TArray<char *> const  & __it_rename_at_22_56, char * const  __separator_rename_at_22_57 );
inline void _FuncbuiltinTickpushTick14133213201864676143_e2397053ec0a37a8 ( Context * __context__, TArray<char *> & __Arr_rename_at_165_62, char * const  __value_rename_at_165_63 );
inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_3ddfc60f52ac9066 ( Context * __context__, TArray<smart_ptr_raw<Function>> & __Arr_rename_at_381_64, smart_ptr_raw<Function> __value_rename_at_381_65 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5024a114a1aa8a05 ( Context * __context__, TArray<smart_ptr_raw<Function>> & __a_rename_at_1192_66 );
inline void clone_8539bd2d8add7e73 ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_255_68, smart_ptr_raw<Function> const  __src_rename_at_255_69 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_f304d102cb128dc5 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __a_rename_at_1192_70 );
inline smart_ptr_raw<ExprCall> _Funcast_boostTickget_find_in_module_exprTick15814845442638081433_afbcc9cf766060f5 ( Context * __context__, char * const  __fn_name_rename_at_985_72, das::string const  & __module_name_rename_at_985_73, das::string const  & __obj_name_rename_at_985_74, LineInfo const  & __at_rename_at_985_75 );
inline smart_ptr_raw<ExprCall> _Funcast_boostTickfind_module_exprTick15293324431947437245_e34e570ad6f2dd1 ( Context * __context__, das::string const  & __name_rename_at_980_77, LineInfo const  & __at_rename_at_980_78 );
inline char * _FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_80, bool __extra_rename_at_38_81, bool __contracts_rename_at_38_82, bool __modules_rename_at_38_83 );
inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_ec2d113b7019dee3 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1736_84 );
inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_86d8ab1b6e687fa0 ( Context * __context__ );
inline Function * _FuncbuiltinTickget_ptrTick5807679485210906136_2d5f62712f1b31fa ( Context * __context__, smart_ptr_raw<Function> __src_rename_at_1742_86 );
inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_4e4f7194eb0890d6 ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1742_87 );
inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_dd70711ad532d4ac ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1742_88 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_2dc189ce58b0f9ff ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_492_89, smart_ptr_raw<Expression> __enull_rename_at_492_90 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_990556032a1b320b ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_492_108, smart_ptr_raw<Expression> __enull_rename_at_492_109 );
inline Annotation * _FuncbuiltinTickget_ptrTick8468476673553620226_fa24e3c4e479e739 ( Context * __context__, smart_ptr_raw<Annotation> const  __src_rename_at_1736_127 );
inline smart_ptr_raw<ExprMakeArray> _FuncbuiltinTickcloneTick9602082126133410484_54c8d45902a12b6a ( Context * __context__, smart_ptr_raw<ExprMakeArray> const  __clone_src_rename_at_1038_128 );
inline void clone_69ab1a2184065196 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_658_130, smart_ptr_raw<TypeDecl> const  __src_rename_at_658_131 );
inline void clone_6965baf83e84e943 ( Context * __context__, smart_ptr_raw<Enumeration> & __dest_rename_at_810_132, Enumeration * const  __src_rename_at_810_133 );
inline void _FuncbuiltinTickcloneTick9409548443506319159_df2e383d4b48e41d ( Context * __context__, das::vector<smart_ptr<Expression>> & __args_rename_at_1129_134, TArray<smart_ptr_raw<Expression>> & __nargs_rename_at_1129_135 );
inline smart_ptr_raw<TypeDecl> _FuncbuiltinTickcloneTick9602082126133410484_ff5eacb201903461 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __clone_src_rename_at_1038_137 );
inline TypeDecl * _FuncbuiltinTickget_ptrTick5807679485210906136_800d8621d2b80648 ( Context * __context__, smart_ptr_raw<TypeDecl> __src_rename_at_1742_139 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_a043c7eded26b0c1 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __Arr_rename_at_289_140, smart_ptr_raw<Expression> & __value_rename_at_289_141 );
inline bool _Funcast_boostTickis_quote_field_allowedTick3545799563217800274_8df56ac57aa1aa67 ( Context * __context__, char * const  __f_name_rename_at_994_142, char * const  __class_name_rename_at_994_143, uint8_t const  * const  __data_rename_at_994_144 );
inline smart_ptr_raw<ExprConstString> _Funcast_boostTickcreate_stringTick16871435846852833435_ac46da0203621c82 ( Context * __context__, das::string const  & __name_rename_at_943_146 );
inline Annotation * _FuncbuiltinTickget_ptrTick5807679485210906136_15ca7add977814e6 ( Context * __context__, smart_ptr_raw<Annotation> __src_rename_at_1742_148 );
inline Expression * _FuncbuiltinTickget_ptrTick5807679485210906136_5f1c776d88550ffa ( Context * __context__, smart_ptr_raw<Expression> __src_rename_at_1742_149 );
inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_2ad37c2da999dae1 ( Context * __context__, char * const  __name_rename_at_967_150 );
inline void clone_d857f1a129cf603a ( Context * __context__, smart_ptr_raw<Annotation> & __dest_rename_at_1157_151, smart_ptr_raw<Annotation> const  __src_rename_at_1157_152 );
inline char * _FuncastTickdescribeTick842554968825501494_79ad6734798edcff ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_153 );
inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_6d4e7dbe2a2e0573 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1736_154 );
inline bool isVectorType_6b8b46476d1214b4 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_16_155 );
inline bool isYetAnotherVectorTemplate_6471d45428bc715 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_26_156 );
inline char * describe_e17337a4a8ea5e ( Context * __context__, AnnotationArgumentList const  & __list_rename_at_31_157 );
inline char * describe_917d990dd3d0a69e ( Context * __context__, AnnotationDeclaration const  & __ann_rename_at_35_160 );
inline char * describe_13e8bddff42f7783 ( Context * __context__, AnnotationList const  & __list_rename_at_43_161 );
inline char * describe_6b6cafda29d5fa62 ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_47_164 );
inline bool isExpression_991e7b9aabc55e05 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_68_165, bool __top_rename_at_68_166 );
inline bool is_same_or_inherited_b1f5309ae03c83d0 ( Context * __context__, Structure const  * const  __parent_rename_at_86_167, Structure const  * const  __child_rename_at_86_168 );
inline bool is_class_method_9615c7e22d648a7e ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_97_170, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_97_171 );
inline void emplace_new_60d5871f9e959ff9 ( Context * __context__, das::vector<smart_ptr<Expression>> & __vec_rename_at_119_172, smart_ptr_raw<Expression> __ptr_rename_at_119_173 );
inline void emplace_new_e80d981a19168757 ( Context * __context__, das::vector<smart_ptr<TypeDecl>> & __vec_rename_at_123_174, smart_ptr_raw<TypeDecl> __ptr_rename_at_123_175 );
inline void emplace_new_6a9a9c984e26a11b ( Context * __context__, das::vector<smart_ptr<Variable>> & __vec_rename_at_127_176, smart_ptr_raw<Variable> __ptr_rename_at_127_177 );
inline void emplace_new_eba12a326cb79d4c ( Context * __context__, MakeStruct & __vec_rename_at_131_178, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_131_179 );
inline bool override_method_3ad2fc756944d0e7 ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_135_180, char * const  __name_rename_at_135_181, char * const  __funcName_rename_at_135_182 );
inline void for_each_tag_function_4aa6238332e92abb ( Context * __context__, Module * const  __mod_rename_at_188_186, char * const  __tag_rename_at_188_187, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __blk_rename_at_188_188 );
inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_a97f351b7d06ed8f ( Context * __context__, char * const  __argn_rename_at_214_195, AnnotationArgumentList const  & __args_rename_at_214_196 );
inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_b6ffaee9719f2933 ( Context * __context__, AnnotationArgumentList const  & __args_rename_at_223_198, char * const  __argn_rename_at_223_199 );
inline void apply_tag_annotation_8a88cbaecaf3bdaa ( Context * __context__, char * const  __tag_rename_at_232_201, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_232_202 );
inline smart_ptr_raw<Function> find_unique_function_d2492cf8492d63ba ( Context * __context__, Module * const  __mod_rename_at_251_207, char * const  __name_rename_at_251_208, bool __canfail_rename_at_251_209 );
inline Function * find_unique_function_ptr_98335b992a400b59 ( Context * __context__, Module * const  __mod_rename_at_267_213, char * const  __name_rename_at_267_214, bool __canfail_rename_at_267_215 );
inline smart_ptr_raw<Function> find_unique_generic_b8b7754471b20ed2 ( Context * __context__, Module * const  __mod_rename_at_272_217, char * const  __name_rename_at_272_218, bool __canfail_rename_at_272_219 );
inline ExprBlock * setup_call_list_86425b60d55e3039 ( Context * __context__, char * const  __name_rename_at_288_223, LineInfo const  & __at_rename_at_288_224, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __subblock_rename_at_288_225 );
inline ExprBlock * setup_call_list_180d45678895ad8a ( Context * __context__, char * const  __name_rename_at_309_230, LineInfo const  & __at_rename_at_309_231, bool __isInit_rename_at_309_232, bool __isPrivate_rename_at_309_233, bool __isLateInit_rename_at_309_234 );
inline ExprBlock * setup_macro_1eb22d9624e827a6 ( Context * __context__, char * const  __name_rename_at_323_236, LineInfo const  & __at_rename_at_323_237 );
inline void * panic_expr_as_4c38e300a4bc92c4 ( Context * __context__ );
inline bool _FuncTickisTickBuiltInFunction_67b8133cad1f66d3 ( Context * __context__, Function * const  __foo_rename_at_563_250 );
inline BuiltInFunction * _FuncTickasTickBuiltInFunction_86b221c9be037d90 ( Context * __context__, Function * const  __foo_rename_at_567_251 );
inline bool _FuncTickisTickExternalFnBase_f41cfb59a73b3862 ( Context * __context__, Function * const  __foo_rename_at_576_252 );
inline ExternalFnBase * _FuncTickasTickExternalFnBase_fb0ab060e41b89fb ( Context * __context__, Function * const  __foo_rename_at_580_253 );
inline bool _FuncTickisTickFunctionAnnotation_a9c64a70780109c6 ( Context * __context__, Annotation * const  __foo_rename_at_589_254 );
inline bool _FuncTickisTickFunctionAnnotation_55ebe0a1ac05ff73 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_593_255 );
inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_a2759eda58bebeb4 ( Context * __context__, Annotation * const  __foo_rename_at_597_256 );
inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_d2eaa6a8d42295ac ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_602_257 );
inline bool _FuncTickisTickStructureAnnotation_3284abe043d58747 ( Context * __context__, Annotation * const  __foo_rename_at_612_258 );
inline bool _FuncTickisTickStructureAnnotation_8e0018d5eb2637f2 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_616_259 );
inline StructureAnnotation * _FuncTickasTickStructureAnnotation_e33130a234a0a0d3 ( Context * __context__, Annotation * const  __foo_rename_at_620_260 );
inline StructureAnnotation * _FuncTickasTickStructureAnnotation_9f2533762c926957 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_625_261 );
inline smart_ptr_raw<ExprCall> to_array_move_expr_6c95c6c3a47afc48 ( Context * __context__, smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_630_262, LineInfo const  & __at_rename_at_630_263 );
inline smart_ptr_raw<Expression> walk_and_convert_array_6fddd8e31acbfae0 ( Context * __context__, uint8_t const  * const  __data_rename_at_638_267, smart_ptr_raw<TypeDecl> const  __info_rename_at_638_268, LineInfo const  & __at_rename_at_638_269 );
inline smart_ptr_raw<Expression> walk_and_convert_dim_5e577a4630aa7d27 ( Context * __context__, uint8_t const  * const  __data_rename_at_653_274, smart_ptr_raw<TypeDecl> const  __info_rename_at_653_275, LineInfo const  & __at_rename_at_653_276 );
inline smart_ptr_raw<Expression> walk_and_convert_pointer_c823df9ddee9c45f ( Context * __context__, uint8_t const  * const  __data_rename_at_668_284, smart_ptr_raw<TypeDecl> const  __info_rename_at_668_285, LineInfo const  & __at_rename_at_668_286 );
inline smart_ptr_raw<Expression> walk_and_convert_tuple_7bb6d375ae2e2688 ( Context * __context__, uint8_t const  * const  __data_rename_at_677_290, smart_ptr_raw<TypeDecl> const  __info_rename_at_677_291, LineInfo const  & __at_rename_at_677_292 );
inline smart_ptr_raw<Expression> walk_and_convert_structure_b13b947d23bd62c7 ( Context * __context__, uint8_t const  * const  __data_rename_at_689_297, smart_ptr_raw<TypeDecl> const  __info_rename_at_689_298, LineInfo const  & __at_rename_at_689_299 );
inline smart_ptr_raw<Expression> walk_and_convert_variant_1aa4185043fe5555 ( Context * __context__, uint8_t const  * const  __data_rename_at_710_308, smart_ptr_raw<TypeDecl> const  __info_rename_at_710_309, LineInfo const  & __at_rename_at_710_310 );
inline smart_ptr_raw<Expression> walk_and_convert_table_8242a4ff974756c0 ( Context * __context__, uint8_t const  * const  __data_rename_at_725_317, smart_ptr_raw<TypeDecl> const  __info_rename_at_725_318, LineInfo const  & __at_rename_at_725_319 );
inline smart_ptr_raw<Expression> walk_and_convert_basic_f4fa8f2044f51a22 ( Context * __context__, uint8_t const  * const  __data_rename_at_751_332, smart_ptr_raw<TypeDecl> const  __info_rename_at_751_333, LineInfo const  & __at_rename_at_751_334 );
inline smart_ptr_raw<Expression> walk_and_convert_enumeration_75ada03d57f913e7 ( Context * __context__, uint8_t const  * const  __data_rename_at_794_336, smart_ptr_raw<TypeDecl> const  __info_rename_at_794_337, LineInfo const  & __at_rename_at_794_338 );
inline smart_ptr_raw<Expression> walk_and_convert_badf850f139f3bb3 ( Context * __context__, uint8_t const  * const  __data_rename_at_814_342, smart_ptr_raw<TypeDecl> const  __info_rename_at_814_343, LineInfo const  & __at_rename_at_814_344 );
inline void clone_dc75d99dda1cee2c ( Context * __context__, das::vector<CaptureEntry> & __a_rename_at_870_345, TArray<ast_boost::CaptureEntryInitData> const  & __b_rename_at_870_346 );
inline void clone_eb879c2dc1a7e11e ( Context * __context__, AnnotationArgumentList & __a_rename_at_906_351, TArray<ast_boost::AnnotationArgumentInitData> const  & __b_rename_at_906_352 );
inline smart_ptr_raw<TypeDecl> get_vec_dst_type_a1ed8a6bb17eda3a ( Context * __context__, smart_ptr_raw<TypeDecl> __src_tp_rename_at_924_357 );
inline smart_ptr_raw<ExprCall> to_arr_move_args_7ac8193c43e93926 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __args_rename_at_947_361, smart_ptr_raw<TypeDecl> __dst_tp_rename_at_947_362, LineInfo const  & __at_rename_at_947_363 );
inline smart_ptr_raw<Expression> convert_vec_cf3d8478a556d0d6 ( Context * __context__, Module * const  __mod_rename_at_954_365, uint8_t const  * const  __field_values_rename_at_954_366, smart_ptr_raw<TypeDecl> const  __xtype_rename_at_954_367, LineInfo const  & __at_rename_at_954_368 );
inline smart_ptr_raw<MakeFieldDecl> convert_quote_struct_field_529c713c43cf65dc ( Context * __context__, Module * const  __mod_rename_at_1006_376, uint8_t const  * const  __data_rename_at_1006_377, char * const  __tstr_rename_at_1007_378, char * const  __name_rename_at_1007_379, uint32_t __offset_rename_at_1008_380, smart_ptr_raw<TypeDecl> const  __xtype_rename_at_1008_381, LineInfo const  & __parent_loc_rename_at_1008_382 );
inline smart_ptr_raw<ExprMakeStruct> convert_quote_structure_7513fe9a14da0cec ( Context * __context__, Module * const  __mod_rename_at_1027_388, uint8_t const  * const  __data_rename_at_1027_389, char * const  __tstr_rename_at_1028_390, smart_ptr_raw<TypeDecl> const  __info_rename_at_1028_391, LineInfo const  & __at_rename_at_1028_392 );
inline smart_ptr_raw<Expression> convert_quote_expr_d61fc2e3f77fdf2e ( Context * __context__, Module * const  __mod_rename_at_1043_400, uint8_t const  * __data_rename_at_1043_401, smart_ptr_raw<TypeDecl> const  __info_rename_at_1043_402, LineInfo const  & __at_rename_at_1043_403 );
inline smart_ptr_raw<ExprAscend> convert_quote_to_expression_214bfe21da346d3e ( Context * __context__, smart_ptr_raw<Expression> __arg_expr_rename_at_1113_413, LineInfo const  & __at_rename_at_1113_414 );
inline Annotation const  * find_annotation_3750ee9a7cdedb92 ( Context * __context__, char * const  __mod_name_rename_at_1137_417, char * const  __ann_name_rename_at_1137_418 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_7ea25e1c3613ccaa ( Context * __context__, char * const  __mod_name_rename_at_1153_422, char * const  __ann_name_rename_at_1153_423, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_1153_424 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_5ce31aec0dc3e827 ( Context * __context__, char * const  __mod_name_rename_at_1176_430, char * const  __ann_name_rename_at_1176_431 );
inline void append_annotation_77e3691cc75925e6 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_1184_434, char * const  __mod_name_rename_at_1184_435, char * const  __ann_name_rename_at_1184_436 );
inline void append_annotation_2795d5527c6b541c ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_1191_439, char * const  __mod_name_rename_at_1191_440, char * const  __ann_name_rename_at_1191_441 );
inline void append_annotation_6137d9bcdc59acfc ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_1198_444, char * const  __mod_name_rename_at_1198_445, char * const  __ann_name_rename_at_1198_446 );
inline void append_annotation_a7f0a49b54847e5b ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_1205_449, char * const  __mod_name_rename_at_1205_450, char * const  __ann_name_rename_at_1205_451, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_1205_452 );
inline void append_annotation_c867e1d1bd1e709d ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_1212_455, char * const  __mod_name_rename_at_1212_456, char * const  __ann_name_rename_at_1212_457, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_1212_458 );
inline void append_annotation_a0a2cf8e458c7814 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_1219_461, char * const  __mod_name_rename_at_1219_462, char * const  __ann_name_rename_at_1219_463, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_1219_464 );
inline int32_t add_annotation_argument_b1f0a75a869f9d8e ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1226_467, char * const  __argName_rename_at_1226_468, bool __val_rename_at_1226_469 );
inline int32_t add_annotation_argument_1cccdd04ffc213ae ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1233_471, char * const  __argName_rename_at_1233_472, int32_t __val_rename_at_1233_473 );
inline int32_t add_annotation_argument_c587cb52cc713868 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1240_475, char * const  __argName_rename_at_1240_476, float __val_rename_at_1240_477 );
inline int32_t add_annotation_argument_64b4b11cebf2a3a4 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1247_479, char * const  __argName_rename_at_1247_480, char * const  __val_rename_at_1247_481 );
inline int32_t add_annotation_argument_8f7d2e8ac5cc9c46 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1254_483, AnnotationArgument const  & __ann_rename_at_1254_484 );
inline int32_t get_for_source_index_4960346f545ad8a5 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1271_486, smart_ptr_raw<Variable> const  __svar_rename_at_1271_487 );
inline int32_t get_for_source_index_e610aeac2c72af06 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1280_492, smart_ptr_raw<Expression> const  __source_rename_at_1280_493 );
inline smart_ptr_raw<TypeDecl> function_to_type_3ce465c9c4c12ac9 ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_1328_498 );
inline void visit_finally_eebe169335929629 ( Context * __context__, ExprBlock * const  __blk_rename_at_1340_504, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1340_505 );
inline bool isCMRES_18c2571faf70a24 ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1346_506 );
inline bool isCMRES_2379351cb339af97 ( Context * __context__, Function * const  __fun_rename_at_1350_507 );
inline bool isMakeLocal_5ae2b5812df61b98 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1354_508 );
inline bool isExprCallFunc_905b0dfa902dbccd ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1362_509 );
inline TDim<DAS_COMMENT(bound_enum) das::Type,31> get_workhorse_types_f596638550511fde ( Context * __context__ );
inline int32_t find_argument_index_ae234aada3e459c5 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1382_510, char * const  __name_rename_at_1382_511 );
inline bool isCMRESType_781422633ef81b3 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1391_516 );
inline void debug_expression_impl_eaefe21084abd004 ( Context * __context__, StringBuilderWriter & __writer_rename_at_1399_517, smart_ptr_raw<Expression> const  __expr_rename_at_1399_518, Bitfield __deFlags_rename_at_1399_519, int32_t __tabs_rename_at_1399_520 );
inline char * debug_expression_cdb88f2c378a9017 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1505_554, Bitfield __deFlags_rename_at_1505_555 );
inline char * debug_expression_803962089c61eb61 ( Context * __context__, Expression * const  __expr_rename_at_1511_557 );
inline char * describe_8f9450a77a61aede ( Context * __context__, Expression * const  __expr_rename_at_1517_558 );
inline int32_t getVectorElementCount_dae41b2187c417ca ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1523_559 );
inline int32_t getVectorElementSize_ba6e81198dcbc886 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1530_560 );
inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_c96967adbe9325ce ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1535_561 );
inline int32_t getVectorOffset_56f905013ae8c372 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1544_562, char * const  __ident_rename_at_1544_563 );
inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_de6dfc85173d067c ( Context * __context__, TDim<AutoTuple<char *,char *>,16> & __a_rename_at_1352_566 );
inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_898d9918ac5a3e3e ( Context * __context__, TDim<char *,6> & __a_rename_at_1352_568 );

void __init_script ( Context * __context__, bool __init_shared )
{
    TDim<AutoTuple<char *,char *>,16> _temp_make_local_878_16_0;
    TDim<char *,6> _temp_make_local_897_20_1;
    das_global<TArray<AutoTuple<char *,char *>>,0xf54bf794d25ccb4e>(__context__) = _FuncbuiltinTickto_array_moveTick3185538323411982277_de6dfc85173d067c(__context__,das_arg<TDim<AutoTuple<char *,char *>,16>>::pass((([&]() -> TDim<AutoTuple<char *,char *>,16>& {
        _temp_make_local_878_16_0(0,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_879;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_879) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_879) = ((char *) "inferStack");
            return __mkt_879;
        })());
        _temp_make_local_878_16_0(1,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_880;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_880) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_880) = ((char *) "annotations");
            return __mkt_880;
        })());
        _temp_make_local_878_16_0(2,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_881;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_881) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_881) = ((char *) "at");
            return __mkt_881;
        })());
        _temp_make_local_878_16_0(3,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_882;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_882) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_882) = ((char *) "atDecl");
            return __mkt_882;
        })());
        _temp_make_local_878_16_0(4,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_883;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_883) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_883) = ((char *) "module");
            return __mkt_883;
        })());
        _temp_make_local_878_16_0(5,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_884;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_884) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_884) = ((char *) "useGlobalVariables");
            return __mkt_884;
        })());
        _temp_make_local_878_16_0(6,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_885;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_885) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_885) = ((char *) "useFunctions");
            return __mkt_885;
        })());
        _temp_make_local_878_16_0(7,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_886;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_886) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_886) = ((char *) "resultAliasesGlobals");
            return __mkt_886;
        })());
        _temp_make_local_878_16_0(8,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_887;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_887) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_887) = ((char *) "fromGeneric");
            return __mkt_887;
        })());
        _temp_make_local_878_16_0(9,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_888;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_888) = ((char *) "ast::Function");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_888) = ((char *) "classParent");
            return __mkt_888;
        })());
        _temp_make_local_878_16_0(10,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_889;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_889) = ((char *) "ast::ExprMakeStruct");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_889) = ((char *) "structs");
            return __mkt_889;
        })());
        _temp_make_local_878_16_0(11,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_890;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_890) = ((char *) "ast::ExprReturn");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_890) = ((char *) "_block");
            return __mkt_890;
        })());
        _temp_make_local_878_16_0(12,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_891;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_891) = ((char *) "ast::ExprVar");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_891) = ((char *) "pBlock");
            return __mkt_891;
        })());
        _temp_make_local_878_16_0(13,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_892;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_892) = ((char *) "ast::CaptureEntry");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_892) = ((char *) "at");
            return __mkt_892;
        })());
        _temp_make_local_878_16_0(14,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_893;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_893) = ((char *) "rtti::AnnotationArgument");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_893) = ((char *) "at");
            return __mkt_893;
        })());
        _temp_make_local_878_16_0(15,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_894;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_894) = ((char *) "rtti::LineInfo");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_894) = ((char *) "fileInfo");
            return __mkt_894;
        })());
        return _temp_make_local_878_16_0;
    })())));/*blacklist*/
    das_global<TArray<char *>,0x764f734b280c42ca>(__context__) = _FuncbuiltinTickto_array_moveTick3185538323411982277_898d9918ac5a3e3e(__context__,das_arg<TDim<char *,6>>::pass((([&]() -> TDim<char *,6>& {
        _temp_make_local_897_20_1(0,__context__) = ((char *) "rtti::Module");
        _temp_make_local_897_20_1(1,__context__) = ((char *) "rtti::Annotation");
        _temp_make_local_897_20_1(2,__context__) = ((char *) "rtti::TypeAnnotation");
        _temp_make_local_897_20_1(3,__context__) = ((char *) "ast::Structure");
        _temp_make_local_897_20_1(4,__context__) = ((char *) "ast::CallMacro");
        _temp_make_local_897_20_1(5,__context__) = ((char *) "ast::Function");
        return _temp_make_local_897_20_1;
    })())));/*managed_types*/
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_1e3552d3bc6b6de6 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_5fb0f8e94e78667b ( Context * __context__, TDim<smart_ptr_raw<Expression>,2> const  &  __a_rename_at_586_2 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_8f6af4333a24497d ( Context * __context__, TDim<smart_ptr_raw<Expression>,1> const  &  __a_rename_at_586_3 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_f2e12ba33c8e5800 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_4, ast::AstFunctionAnnotation * __value_rename_at_181_5 )
{
    das_copy(__Arr_rename_at_181_4(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_4),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_5);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_dc38b502bf9875cd ( Context * __context__, ast_boost::MacroMacro const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_33dd2607299b73e2 ( Context * __context__, ast_boost::TagFunctionAnnotation const  &  __cl_rename_at_116_7 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_7.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e0f0db26c9bddfb9 ( Context * __context__, TArray<ast::AstStructureAnnotation *> &  __Arr_rename_at_181_8, ast::AstStructureAnnotation * __value_rename_at_181_9 )
{
    das_copy(__Arr_rename_at_181_8(builtin_array_push_back(das_arg<TArray<ast::AstStructureAnnotation *>>::pass(__Arr_rename_at_181_8),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_9);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_25cb48d0685fc01e ( Context * __context__, ast_boost::SetupFunctionAnnotation const  &  __cl_rename_at_116_10 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_10.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a4299e2b282e186f ( Context * __context__, ast_boost::SetupBlockAnnotation const  &  __cl_rename_at_116_11 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_11.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_eacbdf6c5c838463 ( Context * __context__, ast_boost::SetupStructureAnnotation const  &  __cl_rename_at_116_12 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_12.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7bda8577897747f3 ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  &  __cl_rename_at_116_13 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_13.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1040c47ee99273dc ( Context * __context__, ast_boost::SetupContractAnnotation const  &  __cl_rename_at_116_14 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_14.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_22b4bbfa1962edcb ( Context * __context__, ast_boost::SetupReaderMacro const  &  __cl_rename_at_116_15 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_15.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_318dbb9fe3ad8d26 ( Context * __context__, ast_boost::SetupCommentReader const  &  __cl_rename_at_116_16 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_16.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f9e84a1132cd421c ( Context * __context__, ast_boost::SetupCallMacro const  &  __cl_rename_at_116_17 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_17.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_598afebfc43d1b82 ( Context * __context__, ast_boost::SetupTypeInfoMacro const  &  __cl_rename_at_116_18 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_18.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b7283bfd4d6912a ( Context * __context__, ast_boost::SetupVariantMacro const  &  __cl_rename_at_116_19 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_19.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d424ba6952970d20 ( Context * __context__, ast_boost::SetupForLoopMacro const  &  __cl_rename_at_116_20 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_20.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c64c64f8ce19c4f1 ( Context * __context__, ast_boost::SetupCaptureMacro const  &  __cl_rename_at_116_21 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_21.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_fd693b0656e58d42 ( Context * __context__, ast_boost::SetupTypeMacro const  &  __cl_rename_at_116_22 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_22.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cc5e722ec5d18084 ( Context * __context__, ast_boost::SetupSimulateMacro const  &  __cl_rename_at_116_23 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_23.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5ecbb6bf9d3c5ce5 ( Context * __context__, ast_boost::TagStructureAnnotation const  &  __cl_rename_at_116_24 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_24.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_86a7b966da7cc372 ( Context * __context__, ast_boost::TagFunctionMacro const  &  __cl_rename_at_116_25 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_25.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_10296bf9fbc53845 ( Context * __context__, ast_boost::SetupInferMacro const  &  __cl_rename_at_116_26 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_26.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_efc8b692d683dde6 ( Context * __context__, ast_boost::SetupDirtyInferMacro const  &  __cl_rename_at_116_27 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_27.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_135df9c4c86bd47a ( Context * __context__, ast_boost::SetupOptimizationMacro const  &  __cl_rename_at_116_28 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_28.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d80d89f4e2ee24a ( Context * __context__, ast_boost::SetupLintMacro const  &  __cl_rename_at_116_29 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_29.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_747b059c62e5428d ( Context * __context__, ast_boost::SetupGlobalLintMacro const  &  __cl_rename_at_116_30 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_30.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_1f060b3476600b3f ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_181_31, ast::AstVariantMacro * __value_rename_at_181_32 )
{
    das_copy(__Arr_rename_at_181_31(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_181_31),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_32);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3c026b31961c9c97 ( Context * __context__, ast_boost::BetterRttiVisitor const  &  __cl_rename_at_116_33 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_33.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_bfb1abbb07340073 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_68_34, int32_t __newSize_rename_at_68_35 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_68_34),__newSize_rename_at_68_35,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_e945fcf444587b9b ( Context * __context__, TArray<char *> &  __Arr_rename_at_68_36, int32_t __newSize_rename_at_68_37 )
{
    builtin_array_resize(das_arg<TArray<char *>>::pass(__Arr_rename_at_68_36),__newSize_rename_at_68_37,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_be3fb1336b24ed81 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & __dest_rename_at_1050_38, smart_ptr_raw<FunctionAnnotation> const  __src_rename_at_1050_39 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1050_38),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1050_39),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_72481d9703572422 ( Context * __context__, TDim<AutoTuple<char *,char *>,16> const  &  __a_rename_at_586_40 )
{
    return das_auto_cast<int32_t>::cast(16);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_6b6c8f819c74b7dd ( Context * __context__, TDim<char *,6> const  &  __a_rename_at_586_41 )
{
    return das_auto_cast<int32_t>::cast(6);
}

inline TArray<smart_ptr_raw<Expression>> _FuncbuiltinTickto_array_moveTick3185538323411982277_574cffbb66f47535 ( Context * __context__, TDim<smart_ptr_raw<Expression>,2> &  __a_rename_at_1352_42 )
{
    TArray<smart_ptr_raw<Expression>> __arr_rename_at_1354_43;das_zero(__arr_rename_at_1354_43);
    _FuncbuiltinTickresizeTick4811697762258667383_1e3552d3bc6b6de6(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__arr_rename_at_1354_43),2);
    das_move(das_cast<TDim<smart_ptr_raw<Expression>,2>>::cast(das_ref(__context__,__arr_rename_at_1354_43(0,__context__))),__a_rename_at_1352_42);
    return /* <- */ das_auto_cast_move<TArray<smart_ptr_raw<Expression>>>::cast(__arr_rename_at_1354_43);
}

inline TArray<smart_ptr_raw<Expression>> _FuncbuiltinTickto_array_moveTick3185538323411982277_5767cf0f8c77bc34 ( Context * __context__, TDim<smart_ptr_raw<Expression>,1> &  __a_rename_at_1352_44 )
{
    TArray<smart_ptr_raw<Expression>> __arr_rename_at_1354_45;das_zero(__arr_rename_at_1354_45);
    _FuncbuiltinTickresizeTick4811697762258667383_1e3552d3bc6b6de6(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__arr_rename_at_1354_45),1);
    das_move(das_cast<TDim<smart_ptr_raw<Expression>,1>>::cast(das_ref(__context__,__arr_rename_at_1354_45(0,__context__))),__a_rename_at_1352_44);
    return /* <- */ das_auto_cast_move<TArray<smart_ptr_raw<Expression>>>::cast(__arr_rename_at_1354_45);
}

inline smart_ptr_raw<FunctionAnnotation> _FuncbuiltinTickclone_to_moveTick2007252383599261567_c0fff2b1179fc9e2 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> const  __clone_src_rename_at_1047_46 )
{
    smart_ptr_raw<FunctionAnnotation> __clone_dest_rename_at_1049_47;das_zero(__clone_dest_rename_at_1049_47);
    clone_be3fb1336b24ed81(__context__,__clone_dest_rename_at_1049_47,__clone_src_rename_at_1047_46);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(__clone_dest_rename_at_1049_47);
}

inline void clone_7f0eb2e30132a5ec ( Context * __context__, smart_ptr_raw<ExprMakeArray> & __dest_rename_at_1041_48, smart_ptr_raw<ExprMakeArray> const  __src_rename_at_1041_49 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1041_48),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1041_49),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Module * _FuncastTickfind_moduleTick11101329256228773934_f6b4e714927216e3 ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_948_50, char * const  __name_rename_at_948_51 ) { das_stack_prologue __prologue(__context__,112,"ast`find_module`11101329256228773934 " DAS_FILE_LINE);
{
    Module * __rm_rename_at_949_52 = 0;
    rtti_builtin_program_for_each_module(__prog_rename_at_948_50,das_make_block<void,Module *>(__context__,96,0,&__func_info__4dedd3fc50b439aa,[&](Module * __mod_rename_at_950_53) -> void{
        if ( SimPolicy<char *>::Equ(cast<char *>::from(((char * const )(to_das_string(das_arg<das::string>::pass(__mod_rename_at_950_53->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),cast<char *>::from(__name_rename_at_948_51),*__context__,nullptr) )
        {
            das_copy(__rm_rename_at_949_52,__mod_rename_at_950_53);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __rm_rename_at_949_52 == nullptr )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) "module '")), cast<char * const >::from(__name_rename_at_948_51), cast<char * const >::from(((char *) "' not found")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<Module *>::cast(__rm_rename_at_949_52);
}}

inline void _FuncbuiltinTickpushTick10769833213962245646_ff37717a0d8d2276 ( Context * __context__, TArray<char *> &  __Arr_rename_at_181_54, char * __value_rename_at_181_55 )
{
    das_copy(__Arr_rename_at_181_54(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_181_54),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_55);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_7c54c515f2166fd4 ( Context * __context__, TArray<char *> const  &  __it_rename_at_22_56, char * const  __separator_rename_at_22_57 )
{
    char * __st_rename_at_27_58 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_27_59) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_28_60 = true;
        {
            bool __need_loop_29 = true;
            // elem: string const&
            das_iterator<TArray<char *> const > __elem_iterator(__it_rename_at_22_56);
            char * const  * __elem_rename_at_29_61;
            __need_loop_29 = __elem_iterator.first(__context__,(__elem_rename_at_29_61)) && __need_loop_29;
            for ( ; __need_loop_29 ; __need_loop_29 = __elem_iterator.next(__context__,(__elem_rename_at_29_61)) )
            {
                if ( __skip_first_rename_at_28_60 )
                {
                    das_copy(__skip_first_rename_at_28_60,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_27_59),cast<char * const >::from(__separator_rename_at_22_57)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_27_59),cast<char * const >::from((*__elem_rename_at_29_61))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_29_61));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_27_58);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_e2397053ec0a37a8 ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_62, char * const  __value_rename_at_165_63 )
{
    das_copy(__Arr_rename_at_165_62(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_62),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_63);
}

inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_3ddfc60f52ac9066 ( Context * __context__, TArray<smart_ptr_raw<Function>> &  __Arr_rename_at_381_64, smart_ptr_raw<Function> __value_rename_at_381_65 )
{
    clone_8539bd2d8add7e73(__context__,__Arr_rename_at_381_64(builtin_array_push_back_zero(das_arg<TArray<smart_ptr_raw<Function>>>::pass(__Arr_rename_at_381_64),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_381_65);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5024a114a1aa8a05 ( Context * __context__, TArray<smart_ptr_raw<Function>> &  __a_rename_at_1192_66 )
{
    {
        bool __need_loop_1194 = true;
        // aV: smart_ptr<ast::Function> aka TT&
        das_iterator<TArray<smart_ptr_raw<Function>>> __aV_iterator(__a_rename_at_1192_66);
        smart_ptr_raw<Function> * __aV_rename_at_1194_67;
        __need_loop_1194 = __aV_iterator.first(__context__,(__aV_rename_at_1194_67)) && __need_loop_1194;
        for ( ; __need_loop_1194 ; __need_loop_1194 = __aV_iterator.next(__context__,(__aV_rename_at_1194_67)) )
        {
            das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,(*__aV_rename_at_1194_67));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1194_67));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Function>>>::pass(__a_rename_at_1192_66),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_8539bd2d8add7e73 ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_255_68, smart_ptr_raw<Function> const  __src_rename_at_255_69 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_255_68),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_255_69),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_f304d102cb128dc5 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __a_rename_at_1192_70 )
{
    {
        bool __need_loop_1194 = true;
        // aV: smart_ptr<ast::Expression> aka TT&
        das_iterator<TArray<smart_ptr_raw<Expression>>> __aV_iterator(__a_rename_at_1192_70);
        smart_ptr_raw<Expression> * __aV_rename_at_1194_71;
        __need_loop_1194 = __aV_iterator.first(__context__,(__aV_rename_at_1194_71)) && __need_loop_1194;
        for ( ; __need_loop_1194 ; __need_loop_1194 = __aV_iterator.next(__context__,(__aV_rename_at_1194_71)) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,(*__aV_rename_at_1194_71));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1194_71));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__a_rename_at_1192_70),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<ExprCall> _Funcast_boostTickget_find_in_module_exprTick15814845442638081433_afbcc9cf766060f5 ( Context * __context__, char * const  __fn_name_rename_at_985_72, das::string const  &  __module_name_rename_at_985_73, das::string const  &  __obj_name_rename_at_985_74, LineInfo const  &  __at_rename_at_985_75 ) { das_stack_prologue __prologue(__context__,160,"ast_boost`get_find_in_module_expr`15814845442638081433 " DAS_FILE_LINE);
{
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprCall>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_986) {
        das_copy((__mks_986.at /*at*/),(__at_rename_at_985_75));
        {
            TArray<smart_ptr_raw<Expression>> _temp_make_local_987_22_2; _temp_make_local_987_22_2;
            TDim<smart_ptr_raw<Expression>,2> _temp_make_local_987_22_3; _temp_make_local_987_22_3;
            _FuncbuiltinTickcloneTick9409548443506319159_df2e383d4b48e41d(__context__,das_arg<das::vector<smart_ptr<Expression>>>::pass(__mks_986.arguments /*arguments*/),das_arg<TArray<smart_ptr_raw<Expression>>>::pass((_temp_make_local_987_22_2 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_574cffbb66f47535(__context__,das_arg<TDim<smart_ptr_raw<Expression>,2>>::pass((([&]() -> TDim<smart_ptr_raw<Expression>,2>& {
                _temp_make_local_987_22_3(0,__context__) = _Funcast_boostTickfind_module_exprTick15293324431947437245_e34e570ad6f2dd1(__context__,__module_name_rename_at_985_73,__at_rename_at_985_75);
                _temp_make_local_987_22_3(1,__context__) = _Funcast_boostTickcreate_stringTick16871435846852833435_ac46da0203621c82(__context__,__obj_name_rename_at_985_74);
                return _temp_make_local_987_22_3;
            })())))))));
            set_das_string(das_arg<das::string>::pass(__mks_986.name /*name*/),__fn_name_rename_at_985_72);
        }    }))));
}}

inline smart_ptr_raw<ExprCall> _Funcast_boostTickfind_module_exprTick15293324431947437245_e34e570ad6f2dd1 ( Context * __context__, das::string const  &  __name_rename_at_980_77, LineInfo const  &  __at_rename_at_980_78 ) { das_stack_prologue __prologue(__context__,160,"ast_boost`find_module_expr`15293324431947437245 " DAS_FILE_LINE);
{
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprCall>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_981) {
        das_copy((__mks_981.at /*at*/),(__at_rename_at_980_78));
        {
            TArray<smart_ptr_raw<Expression>> _temp_make_local_982_41_4; _temp_make_local_982_41_4;
            TDim<smart_ptr_raw<Expression>,1> _temp_make_local_982_41_5; _temp_make_local_982_41_5;
            _FuncbuiltinTickcloneTick9409548443506319159_df2e383d4b48e41d(__context__,das_arg<das::vector<smart_ptr<Expression>>>::pass(__mks_981.arguments /*arguments*/),das_arg<TArray<smart_ptr_raw<Expression>>>::pass((_temp_make_local_982_41_4 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_5767cf0f8c77bc34(__context__,das_arg<TDim<smart_ptr_raw<Expression>,1>>::pass((([&]() -> TDim<smart_ptr_raw<Expression>,1>& {
                _temp_make_local_982_41_5(0,__context__) = _Funcast_boostTickcreate_stringTick16871435846852833435_ac46da0203621c82(__context__,__name_rename_at_980_77);
                return _temp_make_local_982_41_5;
            })())))))));
            set_das_string(das_arg<das::string>::pass(__mks_981.name /*name*/),((char *) "get_module"));
        }    }))));
}}

inline char * _FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_80, bool __extra_rename_at_38_81, bool __contracts_rename_at_38_82, bool __modules_rename_at_38_83 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_80,__extra_rename_at_38_81,__contracts_rename_at_38_82,__modules_rename_at_38_83,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_ec2d113b7019dee3 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1736_84 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1736_84));
}

inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_86d8ab1b6e687fa0 ( Context * __context__ )
{
    AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> __t_rename_at_164_85;das_zero(__t_rename_at_164_85);
    set_variant_index(das_arg<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::pass(__t_rename_at_164_85),8);
    return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(__t_rename_at_164_85);
}

inline Function * _FuncbuiltinTickget_ptrTick5807679485210906136_2d5f62712f1b31fa ( Context * __context__, smart_ptr_raw<Function> __src_rename_at_1742_86 )
{
    return das_auto_cast<Function *>::cast(das_cast<Function *>::cast(__src_rename_at_1742_86));
}

inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_4e4f7194eb0890d6 ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1742_87 )
{
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__src_rename_at_1742_87));
}

inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_dd70711ad532d4ac ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1742_88 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1742_88));
}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_2dc189ce58b0f9ff ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_492_89, smart_ptr_raw<Expression> __enull_rename_at_492_90 ) { das_stack_prologue __prologue(__context__,816,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_493_91; memset((void*)&__vdr_rename_at_493_91,0,sizeof(__vdr_rename_at_493_91));
    smart_ptr_raw<ExprConstString> __cna_rename_at_494_93; memset((void*)&__cna_rename_at_494_93,0,sizeof(__cna_rename_at_494_93));
    smart_ptr_raw<ExprOp2> __cond_rename_at_495_95; memset((void*)&__cond_rename_at_495_95,0,sizeof(__cond_rename_at_495_95));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_496_97; memset((void*)&__ctype_rename_at_496_97,0,sizeof(__ctype_rename_at_496_97));
    smart_ptr_raw<Expression> __esub_rename_at_500_99; memset((void*)&__esub_rename_at_500_99,0,sizeof(__esub_rename_at_500_99));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_502_100; memset((void*)&__neqz_rename_at_502_100,0,sizeof(__neqz_rename_at_502_100));
    smart_ptr_raw<ExprOp2> __nand_rename_at_506_102; memset((void*)&__nand_rename_at_506_102,0,sizeof(__nand_rename_at_506_102));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_512_104; memset((void*)&__nsub_rename_at_512_104,0,sizeof(__nsub_rename_at_512_104));
    smart_ptr_raw<ExprCast> __vcast_rename_at_518_105; memset((void*)&__vcast_rename_at_518_105,0,sizeof(__vcast_rename_at_518_105));
    smart_ptr_raw<ExprOp3> __tri_rename_at_524_106; memset((void*)&__tri_rename_at_524_106,0,sizeof(__tri_rename_at_524_106));
    /* finally */ auto __finally_492= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_524_106);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_518_105);
    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__esub_rename_at_500_99);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_496_97);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_495_95);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_494_93);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_493_91);
    /* end finally */ });
    __vdr_rename_at_493_91; das_zero(__vdr_rename_at_493_91); das_move(__vdr_rename_at_493_91, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_493) {
        das_copy((__mks_493.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
        das_move((__mks_493.value /*value*/),(clone_expression(__expr_rename_at_492_89->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_493.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_494_93; das_zero(__cna_rename_at_494_93); das_move(__cna_rename_at_494_93, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_494) {
        das_copy((__mks_494.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_494.text /*value*/,__expr_rename_at_492_89->name /*name*/);
        }    }))));
    __cond_rename_at_495_95; das_zero(__cond_rename_at_495_95); das_move(__cond_rename_at_495_95, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_495) {
        das_copy((__mks_495.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
        das_move((__mks_495.left /*left*/),(__vdr_rename_at_493_91));
        das_move((__mks_495.right /*right*/),(__cna_rename_at_494_93));
        {
            set_das_string(das_arg<das::string>::pass(__mks_495.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_496_97; das_zero(__ctype_rename_at_496_97); das_move(__ctype_rename_at_496_97, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_496) {
        das_copy((__mks_496.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
        das_copy((__mks_496.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_496.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_498) {
            das_copy((__mks_498.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
            das_copy((__mks_498.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_498.alias /*alias*/,__expr_rename_at_492_89->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_500_99; das_zero(__esub_rename_at_500_99); das_move(__esub_rename_at_500_99, clone_expression(__expr_rename_at_492_89->value /*value*/));
    if ( __expr_rename_at_492_89->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_501= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_506_102);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_502_100);
        /* end finally */ });
        __neqz_rename_at_502_100; das_zero(__neqz_rename_at_502_100); das_move(__neqz_rename_at_502_100, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_502) {
            das_copy((__mks_502.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
            das_move((__mks_502.left /*left*/),(clone_expression(__expr_rename_at_492_89->value /*value*/)));
            das_move((__mks_502.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_504) {
                das_copy((__mks_504.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_502.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_506_102; das_zero(__nand_rename_at_506_102); das_move(__nand_rename_at_506_102, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_506) {
            das_copy((__mks_506.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
            das_move((__mks_506.left /*left*/),(__neqz_rename_at_502_100));
            das_move((__mks_506.right /*right*/),(__cond_rename_at_495_95));
            {
                set_das_string(das_arg<das::string>::pass(__mks_506.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_495_95),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_506_102),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_511= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_512_104);
        /* end finally */ });
        __nsub_rename_at_512_104; das_zero(__nsub_rename_at_512_104); das_move(__nsub_rename_at_512_104, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_512) {
            das_copy((__mks_512.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
            das_move((__mks_512.subexpr /*subexpr*/),(__esub_rename_at_500_99));
            das_copy((__mks_512.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_500_99),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_512_104),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_518_105; das_zero(__vcast_rename_at_518_105); das_move(__vcast_rename_at_518_105, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_518) {
        das_copy((__mks_518.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
        das_move((__mks_518.subexpr /*subexpr*/),(__esub_rename_at_500_99));
        das_move((__mks_518.castType /*castType*/),(__ctype_rename_at_496_97));
        das_copy((__mks_518.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_518.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_524_106; das_zero(__tri_rename_at_524_106); das_move(__tri_rename_at_524_106, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_524) {
        das_copy((__mks_524.at /*at*/),(__expr_rename_at_492_89->at /*at*/));
        das_move((__mks_524.subexpr /*subexpr*/),(__cond_rename_at_495_95));
        das_move((__mks_524.left /*left*/),(__vcast_rename_at_518_105));
        das_move((__mks_524.right /*right*/),(__enull_rename_at_492_90));
        {
            set_das_string(das_arg<das::string>::pass(__mks_524.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_524_106->genFlags /*genFlags*/,__expr_rename_at_492_89->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_524_106);
}}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_990556032a1b320b ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_492_108, smart_ptr_raw<Expression> __enull_rename_at_492_109 ) { das_stack_prologue __prologue(__context__,816,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_493_110; memset((void*)&__vdr_rename_at_493_110,0,sizeof(__vdr_rename_at_493_110));
    smart_ptr_raw<ExprConstString> __cna_rename_at_494_112; memset((void*)&__cna_rename_at_494_112,0,sizeof(__cna_rename_at_494_112));
    smart_ptr_raw<ExprOp2> __cond_rename_at_495_114; memset((void*)&__cond_rename_at_495_114,0,sizeof(__cond_rename_at_495_114));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_496_116; memset((void*)&__ctype_rename_at_496_116,0,sizeof(__ctype_rename_at_496_116));
    smart_ptr_raw<Expression> __esub_rename_at_500_118; memset((void*)&__esub_rename_at_500_118,0,sizeof(__esub_rename_at_500_118));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_502_119; memset((void*)&__neqz_rename_at_502_119,0,sizeof(__neqz_rename_at_502_119));
    smart_ptr_raw<ExprOp2> __nand_rename_at_506_121; memset((void*)&__nand_rename_at_506_121,0,sizeof(__nand_rename_at_506_121));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_512_123; memset((void*)&__nsub_rename_at_512_123,0,sizeof(__nsub_rename_at_512_123));
    smart_ptr_raw<ExprCast> __vcast_rename_at_518_124; memset((void*)&__vcast_rename_at_518_124,0,sizeof(__vcast_rename_at_518_124));
    smart_ptr_raw<ExprOp3> __tri_rename_at_524_125; memset((void*)&__tri_rename_at_524_125,0,sizeof(__tri_rename_at_524_125));
    /* finally */ auto __finally_492= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_524_125);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_518_124);
    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__esub_rename_at_500_118);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_496_116);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_495_114);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_494_112);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_493_110);
    /* end finally */ });
    __vdr_rename_at_493_110; das_zero(__vdr_rename_at_493_110); das_move(__vdr_rename_at_493_110, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_493) {
        das_copy((__mks_493.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
        das_move((__mks_493.value /*value*/),(clone_expression(__expr_rename_at_492_108->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_493.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_494_112; das_zero(__cna_rename_at_494_112); das_move(__cna_rename_at_494_112, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_494) {
        das_copy((__mks_494.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_494.text /*value*/,__expr_rename_at_492_108->name /*name*/);
        }    }))));
    __cond_rename_at_495_114; das_zero(__cond_rename_at_495_114); das_move(__cond_rename_at_495_114, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_495) {
        das_copy((__mks_495.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
        das_move((__mks_495.left /*left*/),(__vdr_rename_at_493_110));
        das_move((__mks_495.right /*right*/),(__cna_rename_at_494_112));
        {
            set_das_string(das_arg<das::string>::pass(__mks_495.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_496_116; das_zero(__ctype_rename_at_496_116); das_move(__ctype_rename_at_496_116, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_496) {
        das_copy((__mks_496.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
        das_copy((__mks_496.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_496.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_498) {
            das_copy((__mks_498.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
            das_copy((__mks_498.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_498.alias /*alias*/,__expr_rename_at_492_108->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_500_118; das_zero(__esub_rename_at_500_118); das_move(__esub_rename_at_500_118, clone_expression(__expr_rename_at_492_108->value /*value*/));
    if ( __expr_rename_at_492_108->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_501= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_506_121);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_502_119);
        /* end finally */ });
        __neqz_rename_at_502_119; das_zero(__neqz_rename_at_502_119); das_move(__neqz_rename_at_502_119, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_502) {
            das_copy((__mks_502.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
            das_move((__mks_502.left /*left*/),(clone_expression(__expr_rename_at_492_108->value /*value*/)));
            das_move((__mks_502.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_504) {
                das_copy((__mks_504.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_502.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_506_121; das_zero(__nand_rename_at_506_121); das_move(__nand_rename_at_506_121, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_506) {
            das_copy((__mks_506.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
            das_move((__mks_506.left /*left*/),(__neqz_rename_at_502_119));
            das_move((__mks_506.right /*right*/),(__cond_rename_at_495_114));
            {
                set_das_string(das_arg<das::string>::pass(__mks_506.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_495_114),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_506_121),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_511= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_512_123);
        /* end finally */ });
        __nsub_rename_at_512_123; das_zero(__nsub_rename_at_512_123); das_move(__nsub_rename_at_512_123, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_512) {
            das_copy((__mks_512.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
            das_move((__mks_512.subexpr /*subexpr*/),(__esub_rename_at_500_118));
            das_copy((__mks_512.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_500_118),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_512_123),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_518_124; das_zero(__vcast_rename_at_518_124); das_move(__vcast_rename_at_518_124, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_518) {
        das_copy((__mks_518.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
        das_move((__mks_518.subexpr /*subexpr*/),(__esub_rename_at_500_118));
        das_move((__mks_518.castType /*castType*/),(__ctype_rename_at_496_116));
        das_copy((__mks_518.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_518.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_524_125; das_zero(__tri_rename_at_524_125); das_move(__tri_rename_at_524_125, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_524) {
        das_copy((__mks_524.at /*at*/),(__expr_rename_at_492_108->at /*at*/));
        das_move((__mks_524.subexpr /*subexpr*/),(__cond_rename_at_495_114));
        das_move((__mks_524.left /*left*/),(__vcast_rename_at_518_124));
        das_move((__mks_524.right /*right*/),(__enull_rename_at_492_109));
        {
            set_das_string(das_arg<das::string>::pass(__mks_524.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_524_125->genFlags /*genFlags*/,__expr_rename_at_492_108->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_524_125);
}}

inline Annotation * _FuncbuiltinTickget_ptrTick8468476673553620226_fa24e3c4e479e739 ( Context * __context__, smart_ptr_raw<Annotation> const  __src_rename_at_1736_127 )
{
    return das_auto_cast<Annotation *>::cast(das_cast<Annotation *>::cast(__src_rename_at_1736_127));
}

inline smart_ptr_raw<ExprMakeArray> _FuncbuiltinTickcloneTick9602082126133410484_54c8d45902a12b6a ( Context * __context__, smart_ptr_raw<ExprMakeArray> const  __clone_src_rename_at_1038_128 )
{
    smart_ptr_raw<ExprMakeArray> __clone_dest_rename_at_1040_129;das_zero(__clone_dest_rename_at_1040_129);
    clone_7f0eb2e30132a5ec(__context__,__clone_dest_rename_at_1040_129,__clone_src_rename_at_1038_128);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprMakeArray>>::cast(__clone_dest_rename_at_1040_129);
}

inline void clone_69ab1a2184065196 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_658_130, smart_ptr_raw<TypeDecl> const  __src_rename_at_658_131 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_658_130),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_658_131),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_6965baf83e84e943 ( Context * __context__, smart_ptr_raw<Enumeration> & __dest_rename_at_810_132, Enumeration * const  __src_rename_at_810_133 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_810_132),das_auto_cast<void * const >::cast(__src_rename_at_810_133),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickcloneTick9409548443506319159_df2e383d4b48e41d ( Context * __context__, das::vector<smart_ptr<Expression>> &  __args_rename_at_1129_134, TArray<smart_ptr_raw<Expression>> &  __nargs_rename_at_1129_135 )
{
    das_vector_clear(das_arg<das::vector<smart_ptr<Expression>>>::pass(__args_rename_at_1129_134));
    {
        bool __need_loop_1131 = true;
        // narg: smart_ptr<ast::Expression aka TT>&
        das_iterator<TArray<smart_ptr_raw<Expression>>> __narg_iterator(__nargs_rename_at_1129_135);
        smart_ptr_raw<Expression> * __narg_rename_at_1131_136;
        __need_loop_1131 = __narg_iterator.first(__context__,(__narg_rename_at_1131_136)) && __need_loop_1131;
        for ( ; __need_loop_1131 ; __need_loop_1131 = __narg_iterator.next(__context__,(__narg_rename_at_1131_136)) )
        {
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__args_rename_at_1129_134),(*__narg_rename_at_1131_136));
        }
        __narg_iterator.close(__context__,(__narg_rename_at_1131_136));
    };
    _FuncbuiltinTickfinalizeTick13836114024949725080_f304d102cb128dc5(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__nargs_rename_at_1129_135));
}

inline smart_ptr_raw<TypeDecl> _FuncbuiltinTickcloneTick9602082126133410484_ff5eacb201903461 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __clone_src_rename_at_1038_137 )
{
    smart_ptr_raw<TypeDecl> __clone_dest_rename_at_1040_138;das_zero(__clone_dest_rename_at_1040_138);
    clone_69ab1a2184065196(__context__,__clone_dest_rename_at_1040_138,__clone_src_rename_at_1038_137);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(__clone_dest_rename_at_1040_138);
}

inline TypeDecl * _FuncbuiltinTickget_ptrTick5807679485210906136_800d8621d2b80648 ( Context * __context__, smart_ptr_raw<TypeDecl> __src_rename_at_1742_139 )
{
    return das_auto_cast<TypeDecl *>::cast(das_cast<TypeDecl *>::cast(__src_rename_at_1742_139));
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_a043c7eded26b0c1 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __Arr_rename_at_289_140, smart_ptr_raw<Expression> & __value_rename_at_289_141 )
{
    das_move(__Arr_rename_at_289_140(builtin_array_push_back(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__Arr_rename_at_289_140),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_289_141);
}

inline bool _Funcast_boostTickis_quote_field_allowedTick3545799563217800274_8df56ac57aa1aa67 ( Context * __context__, char * const  __f_name_rename_at_994_142, char * const  __class_name_rename_at_994_143, uint8_t const  * const  __data_rename_at_994_144 )
{
    {
        bool __need_loop_995 = true;
        // cl`fld: tuple<string;string> const&
        das_iterator<TArray<AutoTuple<char *,char *>> const > __clTickfld_iterator(das_global<TArray<AutoTuple<char *,char *>>,0xf54bf794d25ccb4e>(__context__) /*blacklist*/);
        AutoTuple<char *,char *> const  * ___VarclTickfld_rename_at_995_145;
        __need_loop_995 = __clTickfld_iterator.first(__context__,(___VarclTickfld_rename_at_995_145)) && __need_loop_995;
        for ( ; __need_loop_995 ; __need_loop_995 = __clTickfld_iterator.next(__context__,(___VarclTickfld_rename_at_995_145)) )
        {
            if ( (SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_tuple_field<char *,1,char *,char *>::get((*___VarclTickfld_rename_at_995_145))),cast<char *>::from(__f_name_rename_at_994_142),*__context__,nullptr)) && ((SimPolicy<char *>::Equ(cast<char *>::from(__class_name_rename_at_994_143),cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,char *>::get((*___VarclTickfld_rename_at_995_145))),*__context__,nullptr)) || ((SimPolicy<char *>::Equ(cast<char *>::from(__class_name_rename_at_994_143),cast<char *>::from(((char *) "ast::Expression")),*__context__,nullptr)) && (SimPolicy<char *>::Equ(cast<char *>::from(((char * const )(das_cast<Expression>::cast(__data_rename_at_994_144).__rtti /*__rtti*/))),cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,char *>::get((*___VarclTickfld_rename_at_995_145))),*__context__,nullptr)))) )
            {
                return das_auto_cast<bool>::cast(false);
            };
        }
        __clTickfld_iterator.close(__context__,(___VarclTickfld_rename_at_995_145));
    };
    return das_auto_cast<bool>::cast(true);
}

inline smart_ptr_raw<ExprConstString> _Funcast_boostTickcreate_stringTick16871435846852833435_ac46da0203621c82 ( Context * __context__, das::string const  &  __name_rename_at_943_146 ) { das_stack_prologue __prologue(__context__,112,"ast_boost`create_string`16871435846852833435 " DAS_FILE_LINE);
{
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprConstString>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_944) {
        {
            das_clone<das::string,das::string>::clone(__mks_944.text /*value*/,__name_rename_at_943_146);
        }    }))));
}}

inline Annotation * _FuncbuiltinTickget_ptrTick5807679485210906136_15ca7add977814e6 ( Context * __context__, smart_ptr_raw<Annotation> __src_rename_at_1742_148 )
{
    return das_auto_cast<Annotation *>::cast(das_cast<Annotation *>::cast(__src_rename_at_1742_148));
}

inline Expression * _FuncbuiltinTickget_ptrTick5807679485210906136_5f1c776d88550ffa ( Context * __context__, smart_ptr_raw<Expression> __src_rename_at_1742_149 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1742_149));
}

inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_2ad37c2da999dae1 ( Context * __context__, char * const  __name_rename_at_967_150 )
{
    return das_auto_cast<Module *>::cast(_FuncastTickfind_moduleTick11101329256228773934_f6b4e714927216e3(__context__,compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_967_150));
}

inline void clone_d857f1a129cf603a ( Context * __context__, smart_ptr_raw<Annotation> & __dest_rename_at_1157_151, smart_ptr_raw<Annotation> const  __src_rename_at_1157_152 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1157_151),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1157_152),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick842554968825501494_79ad6734798edcff ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_153 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_153,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_6d4e7dbe2a2e0573 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1736_154 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1736_154));
}

inline bool isVectorType_6b8b46476d1214b4 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_16_155 )
{
    return das_auto_cast<bool>::cast(((((((((((((__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tFloat2)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__typ_rename_at_16_155 == DAS_COMMENT(bound_enum) das::Type::tURange64));
}

inline bool isYetAnotherVectorTemplate_6471d45428bc715 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_26_156 )
{
    return das_auto_cast<bool>::cast((((das_deref(__context__,__td_rename_at_26_156)).isHandle()) && (__td_rename_at_26_156->annotation /*annotation*/ != nullptr)) && ((das_deref(__context__,__td_rename_at_26_156->annotation /*annotation*/)).isYetAnotherVectorTemplate()));
}

inline char * describe_e17337a4a8ea5e ( Context * __context__, AnnotationArgumentList const  &  __list_rename_at_31_157 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_7c54c515f2166fd4(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_32_18_rename_at_32_158;das_zero(____acomp_32_18_rename_at_32_158);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_32_18_rename_at_32_158),false);
        {
            bool __need_loop_32 = true;
            // arg: rtti::AnnotationArgument const&
            das_iterator<AnnotationArgumentList const > __arg_iterator(__list_rename_at_31_157);
            AnnotationArgument const  * __arg_rename_at_32_159;
            __need_loop_32 = __arg_iterator.first(__context__,(__arg_rename_at_32_159)) && __need_loop_32;
            for ( ; __need_loop_32 ; __need_loop_32 = __arg_iterator.next(__context__,(__arg_rename_at_32_159)) )
            {
                AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_32_50_6; _temp_make_local_32_50_6;
                _FuncbuiltinTickpushTick10769833213962245646_ff37717a0d8d2276(__context__,das_arg<TArray<char *>>::pass(____acomp_32_18_rename_at_32_158),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_3, cast<das::string const  &>::from((*__arg_rename_at_32_159).name /*name*/), cast<char * const >::from(((char *) "=")), cast<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> &>::from((_temp_make_local_32_50_6 = (rtti_builtin_argument_value((*__arg_rename_at_32_159),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_32_159));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_32_18_rename_at_32_158),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_32_18_rename_at_32_158);
    })),((char *) ",")));
}

inline char * describe_917d990dd3d0a69e ( Context * __context__, AnnotationDeclaration const  &  __ann_rename_at_35_160 )
{
    return das_auto_cast<char *>::cast((das_vector_length(__ann_rename_at_35_160.arguments /*arguments*/) != 0) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_4, cast<das::string const  &>::from(__ann_rename_at_35_160.annotation /*annotation*/->name /*name*/), cast<char * const >::from(((char *) "(")), cast<char * const >::from(describe_e17337a4a8ea5e(__context__,__ann_rename_at_35_160.arguments /*arguments*/)), cast<char * const >::from(((char *) ")"))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<das::string const  &>::from(__ann_rename_at_35_160.annotation /*annotation*/->name /*name*/)))));
}

inline char * describe_13e8bddff42f7783 ( Context * __context__, AnnotationList const  &  __list_rename_at_43_161 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_7c54c515f2166fd4(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_44_18_rename_at_44_162;das_zero(____acomp_44_18_rename_at_44_162);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_44_18_rename_at_44_162),false);
        {
            bool __need_loop_44 = true;
            // arg: smart_ptr<rtti::AnnotationDeclaration> const&
            das_iterator<AnnotationList const > __arg_iterator(__list_rename_at_43_161);
            smart_ptr_raw<AnnotationDeclaration> const  * __arg_rename_at_44_163;
            __need_loop_44 = __arg_iterator.first(__context__,(__arg_rename_at_44_163)) && __need_loop_44;
            for ( ; __need_loop_44 ; __need_loop_44 = __arg_iterator.next(__context__,(__arg_rename_at_44_163)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_e2397053ec0a37a8(__context__,das_arg<TArray<char *>>::pass(____acomp_44_18_rename_at_44_162),describe_917d990dd3d0a69e(__context__,das_deref(__context__,(*__arg_rename_at_44_163))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_44_163));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_44_18_rename_at_44_162),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_44_18_rename_at_44_162);
    })),((char *) ",")));
}

inline char * describe_6b6cafda29d5fa62 ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_47_164 )
{
    return das_auto_cast<char *>::cast((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__vvar_rename_at_47_164->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<das::string const  &>::from(__vvar_rename_at_47_164->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef(__context__,__vvar_rename_at_47_164->type /*_type*/,true,true,true))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_7, cast<das::string const  &>::from(__vvar_rename_at_47_164->name /*name*/), cast<char * const >::from(((char *) ":null"))))));
}

inline bool isExpression_991e7b9aabc55e05 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_68_165, bool __top_rename_at_68_166 )
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__t_rename_at_68_165),das_auto_cast<void * const >::cast(nullptr)) )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( das_vector_length(__t_rename_at_68_165->dim /*dim*/) != 0 )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( __t_rename_at_68_165->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle )
    {
        return das_auto_cast<bool>::cast((eq_dstr_str(__t_rename_at_68_165->annotation /*annotation*/->module /*_module*/->name /*name*/,((char *) "ast"))) ? das_auto_cast<bool>::cast(builtin_string_startswith(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_8, cast<das::string const  &>::from(__t_rename_at_68_165->annotation /*annotation*/->name /*name*/))),((char *) "Expr"),__context__)) : das_auto_cast<bool>::cast(false));
    } else return das_auto_cast<bool>::cast(((__t_rename_at_68_165->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer) && __top_rename_at_68_166) ? das_auto_cast<bool>::cast(isExpression_991e7b9aabc55e05(__context__,__t_rename_at_68_165->firstType /*firstType*/,false)) : das_auto_cast<bool>::cast(false));
}

inline bool is_same_or_inherited_b1f5309ae03c83d0 ( Context * __context__, Structure const  * const  __parent_rename_at_86_167, Structure const  * const  __child_rename_at_86_168 )
{
    Structure const  * __ch_rename_at_87_169 = __child_rename_at_86_168;
    while ( __ch_rename_at_87_169 != nullptr )
    {
        if ( __parent_rename_at_86_167 == __ch_rename_at_87_169 )
        {
            return das_auto_cast<bool>::cast(true);
        } else {
            das_copy(__ch_rename_at_87_169,__ch_rename_at_87_169->parent /*parent*/);
        };
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool is_class_method_9615c7e22d648a7e ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_97_170, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_97_171 )
{
    return das_auto_cast<bool>::cast((__finfo_rename_at_97_171->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tFunction) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_97_171->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_97_171->argTypes /*argTypes*/) == 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_97_171->argTypes /*argTypes*/,0,__context__)->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tStructure) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_97_171->argTypes /*argTypes*/,0,__context__)->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(is_same_or_inherited_b1f5309ae03c83d0(__context__,das_reinterpret<Structure const  * const >::pass(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_97_171->argTypes /*argTypes*/,0,__context__)->structType /*structType*/),das_reinterpret<Structure const  * const >::pass(_FuncbuiltinTickget_ptrTick8468476673553620226_ec2d113b7019dee3(__context__,__cinfo_rename_at_97_170)))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))))))))));
}

inline void emplace_new_60d5871f9e959ff9 ( Context * __context__, das::vector<smart_ptr<Expression>> &  __vec_rename_at_119_172, smart_ptr_raw<Expression> __ptr_rename_at_119_173 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__vec_rename_at_119_172),__ptr_rename_at_119_173);
}

inline void emplace_new_e80d981a19168757 ( Context * __context__, das::vector<smart_ptr<TypeDecl>> &  __vec_rename_at_123_174, smart_ptr_raw<TypeDecl> __ptr_rename_at_123_175 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__vec_rename_at_123_174),__ptr_rename_at_123_175);
}

inline void emplace_new_6a9a9c984e26a11b ( Context * __context__, das::vector<smart_ptr<Variable>> &  __vec_rename_at_127_176, smart_ptr_raw<Variable> __ptr_rename_at_127_177 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Variable>>>::pass(__vec_rename_at_127_176),__ptr_rename_at_127_177);
}

inline void emplace_new_eba12a326cb79d4c ( Context * __context__, MakeStruct &  __vec_rename_at_131_178, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_131_179 )
{
    mks_vector_emplace(das_arg<MakeStruct>::pass(__vec_rename_at_131_178),__ptr_rename_at_131_179);
}

inline bool override_method_3ad2fc756944d0e7 ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_135_180, char * const  __name_rename_at_135_181, char * const  __funcName_rename_at_135_182 ) { das_stack_prologue __prologue(__context__,272,"override_method " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprCast> __vcast_rename_at_138_184; memset((void*)&__vcast_rename_at_138_184,0,sizeof(__vcast_rename_at_138_184));
    {
        bool __need_loop_136 = true;
        // fld: ast::FieldDeclaration&
        das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__str_rename_at_135_180->fields /*fields*/);
        Structure::FieldDeclaration * __fld_rename_at_136_183;
        __need_loop_136 = __fld_iterator.first(__context__,(__fld_rename_at_136_183)) && __need_loop_136;
        for ( ; __need_loop_136 ; __need_loop_136 = __fld_iterator.next(__context__,(__fld_rename_at_136_183)) )
        {
            if ( eq_dstr_str(das_arg<das::string>::pass((*__fld_rename_at_136_183).name /*name*/),__name_rename_at_135_181) )
            {
                /* finally */ auto __finally_137= das_finally([&](){
                das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_138_184);
                /* end finally */ });
                __vcast_rename_at_138_184; das_zero(__vcast_rename_at_138_184); das_move(__vcast_rename_at_138_184, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_138) {
                    das_copy((__mks_138.at /*at*/),(__str_rename_at_135_180->at /*at*/));
                    das_move((__mks_138.subexpr /*subexpr*/),(das_ascend_handle<true,smart_ptr_raw<ExprAddr>>::make(__context__,nullptr,(([&](ExprAddr & __mks_139) {
                        das_copy((__mks_139.at /*at*/),(__str_rename_at_135_180->at /*at*/));
                        {
                            set_das_string(das_arg<das::string>::pass(__mks_139.target /*target*/),__funcName_rename_at_135_182);
                        }                    })))));
                    das_move((__mks_138.castType /*castType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_140) {
                        das_copy((__mks_140.at /*at*/),(__str_rename_at_135_180->at /*at*/));
                        das_copy((__mks_140.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                    })))));
                }))));
                builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast((*__fld_rename_at_136_183).init /*init*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__vcast_rename_at_138_184),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                return das_auto_cast<bool>::cast(true);
            };
        }
        __fld_iterator.close(__context__,(__fld_rename_at_136_183));
    };
    return das_auto_cast<bool>::cast(false);
}}

inline void for_each_tag_function_4aa6238332e92abb ( Context * __context__, Module * const  __mod_rename_at_188_186, char * const  __tag_rename_at_188_187, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __blk_rename_at_188_188 ) { das_stack_prologue __prologue(__context__,416,"for_each_tag_function " DAS_FILE_LINE);
{
    forEachFunction(__mod_rename_at_188_186,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_189_189) -> void{
        {
            bool __need_loop_190 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_189_189->annotations /*annotations*/);
            smart_ptr_raw<AnnotationDeclaration> * __ann_rename_at_190_190;
            __need_loop_190 = __ann_iterator.first(__context__,(__ann_rename_at_190_190)) && __need_loop_190;
            for ( ; __need_loop_190 ; __need_loop_190 = __ann_iterator.next(__context__,(__ann_rename_at_190_190)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_190_190)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_190_190)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_192 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_190_190)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_192_191;
                        __need_loop_192 = __t_iterator.first(__context__,(__t_rename_at_192_191)) && __need_loop_192;
                        for ( ; __need_loop_192 ; __need_loop_192 = __t_iterator.next(__context__,(__t_rename_at_192_191)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_192_191).name /*name*/),__tag_rename_at_188_187) )
                            {
                                das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__blk_rename_at_188_188,__func_rename_at_189_189);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_192_191));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_190_190));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    forEachGenericFunction(__mod_rename_at_188_186,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,272,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_200_192) -> void{
        {
            bool __need_loop_201 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_200_192->annotations /*annotations*/);
            smart_ptr_raw<AnnotationDeclaration> * __ann_rename_at_201_193;
            __need_loop_201 = __ann_iterator.first(__context__,(__ann_rename_at_201_193)) && __need_loop_201;
            for ( ; __need_loop_201 ; __need_loop_201 = __ann_iterator.next(__context__,(__ann_rename_at_201_193)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_201_193)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_201_193)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_203 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_201_193)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_203_194;
                        __need_loop_203 = __t_iterator.first(__context__,(__t_rename_at_203_194)) && __need_loop_203;
                        for ( ; __need_loop_203 ; __need_loop_203 = __t_iterator.next(__context__,(__t_rename_at_203_194)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_203_194).name /*name*/),__tag_rename_at_188_187) )
                            {
                                das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__blk_rename_at_188_188,__func_rename_at_200_192);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_203_194));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_201_193));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_a97f351b7d06ed8f ( Context * __context__, char * const  __argn_rename_at_214_195, AnnotationArgumentList const  &  __args_rename_at_214_196 )
{
    {
        bool __need_loop_215 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_214_196);
        AnnotationArgument const  * __a_rename_at_215_197;
        __need_loop_215 = __a_iterator.first(__context__,(__a_rename_at_215_197)) && __need_loop_215;
        for ( ; __need_loop_215 ; __need_loop_215 = __a_iterator.next(__context__,(__a_rename_at_215_197)) )
        {
            if ( eq_dstr_str((*__a_rename_at_215_197).name /*name*/,__argn_rename_at_214_195) )
            {
                return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(rtti_builtin_argument_value((*__a_rename_at_215_197),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_215_197));
    };
    return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_86d8ab1b6e687fa0(__context__));
}

inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_b6ffaee9719f2933 ( Context * __context__, AnnotationArgumentList const  &  __args_rename_at_223_198, char * const  __argn_rename_at_223_199 )
{
    {
        bool __need_loop_224 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_223_198);
        AnnotationArgument const  * __a_rename_at_224_200;
        __need_loop_224 = __a_iterator.first(__context__,(__a_rename_at_224_200)) && __need_loop_224;
        for ( ; __need_loop_224 ; __need_loop_224 = __a_iterator.next(__context__,(__a_rename_at_224_200)) )
        {
            if ( eq_dstr_str((*__a_rename_at_224_200).name /*name*/,__argn_rename_at_223_199) )
            {
                return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(rtti_builtin_argument_value((*__a_rename_at_224_200),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_224_200));
    };
    return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_86d8ab1b6e687fa0(__context__));
}

inline void apply_tag_annotation_8a88cbaecaf3bdaa ( Context * __context__, char * const  __tag_rename_at_232_201, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_232_202 ) { das_stack_prologue __prologue(__context__,160,"apply_tag_annotation " DAS_FILE_LINE);
{
    TArray<smart_ptr_raw<Function>> __funcs_rename_at_233_203; memset((void*)&__funcs_rename_at_233_203,0,sizeof(__funcs_rename_at_233_203));
    smart_ptr_raw<FunctionAnnotation> __tagAnn_rename_at_239_206; memset((void*)&__tagAnn_rename_at_239_206,0,sizeof(__tagAnn_rename_at_239_206));
    /* finally */ auto __finally_232= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_5024a114a1aa8a05(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_233_203));
    /* end finally */ });
    das_zero(__funcs_rename_at_233_203);
    for_each_tag_function_4aa6238332e92abb(__context__,thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__tag_rename_at_232_201,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__806bd08fbfbd09f4,[&](smart_ptr_raw<Function> __func_rename_at_234_204) -> void{
        _FuncbuiltinTickpush_cloneTick2035469273396957942_3ddfc60f52ac9066(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_233_203),__func_rename_at_234_204);
    }));
    {
        bool __need_loop_237 = true;
        // func: smart_ptr<ast::Function> aka FunctionPtr&
        das_iterator<TArray<smart_ptr_raw<Function>>> __func_iterator(__funcs_rename_at_233_203);
        smart_ptr_raw<Function> * __func_rename_at_237_205;
        __need_loop_237 = __func_iterator.first(__context__,(__func_rename_at_237_205)) && __need_loop_237;
        for ( ; __need_loop_237 ; __need_loop_237 = __func_iterator.next(__context__,(__func_rename_at_237_205)) )
        {
            {
                /* finally */ auto __finally_238= das_finally([&](){
                das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__tagAnn_rename_at_239_206);
                /* end finally */ });
                __tagAnn_rename_at_239_206; das_zero(__tagAnn_rename_at_239_206); das_move(__tagAnn_rename_at_239_206, _FuncbuiltinTickclone_to_moveTick2007252383599261567_c0fff2b1179fc9e2(__context__,__ann_rename_at_232_202));
                addFunctionFunctionAnnotation((*__func_rename_at_237_205),__tagAnn_rename_at_239_206,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __func_iterator.close(__context__,(__func_rename_at_237_205));
    };
}}

inline smart_ptr_raw<Function> find_unique_function_d2492cf8492d63ba ( Context * __context__, Module * const  __mod_rename_at_251_207, char * const  __name_rename_at_251_208, bool __canfail_rename_at_251_209 ) { das_stack_prologue __prologue(__context__,128,"find_unique_function " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_252_210; memset((void*)&__res_rename_at_252_210,0,sizeof(__res_rename_at_252_210));
    int32_t __count_rename_at_253_211; memset((void*)&__count_rename_at_253_211,0,sizeof(__count_rename_at_253_211));
    /* finally */ auto __finally_251= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_252_210);
    /* end finally */ });
    das_zero(__res_rename_at_252_210);
    __count_rename_at_253_211 = 0;
    forEachFunction(__mod_rename_at_251_207,__name_rename_at_251_208,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_254_212) -> void{
        clone_8539bd2d8add7e73(__context__,__res_rename_at_252_210,__func_rename_at_254_212);
        ++__count_rename_at_253_211;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_253_211 > 1 )
    {
        if ( !__canfail_rename_at_251_209 )
        {
            builtin_throw(((char *) "more than one function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_252_210);
    };
}}

inline Function * find_unique_function_ptr_98335b992a400b59 ( Context * __context__, Module * const  __mod_rename_at_267_213, char * const  __name_rename_at_267_214, bool __canfail_rename_at_267_215 )
{
    smart_ptr_raw<Function> __fn_rename_at_268_216; memset((void*)&__fn_rename_at_268_216,0,sizeof(__fn_rename_at_268_216));
    /* finally */ auto __finally_267= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_268_216);
    /* end finally */ });
    __fn_rename_at_268_216; das_zero(__fn_rename_at_268_216); das_move(__fn_rename_at_268_216, find_unique_function_d2492cf8492d63ba(__context__,__mod_rename_at_267_213,__name_rename_at_267_214,__canfail_rename_at_267_215));
    return /* <- */ das_auto_cast_move<Function *>::cast(_FuncbuiltinTickget_ptrTick5807679485210906136_2d5f62712f1b31fa(__context__,__fn_rename_at_268_216));
}

inline smart_ptr_raw<Function> find_unique_generic_b8b7754471b20ed2 ( Context * __context__, Module * const  __mod_rename_at_272_217, char * const  __name_rename_at_272_218, bool __canfail_rename_at_272_219 ) { das_stack_prologue __prologue(__context__,128,"find_unique_generic " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_273_220; memset((void*)&__res_rename_at_273_220,0,sizeof(__res_rename_at_273_220));
    int32_t __count_rename_at_274_221; memset((void*)&__count_rename_at_274_221,0,sizeof(__count_rename_at_274_221));
    /* finally */ auto __finally_272= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_273_220);
    /* end finally */ });
    das_zero(__res_rename_at_273_220);
    __count_rename_at_274_221 = 0;
    forEachGenericFunction(__mod_rename_at_272_217,__name_rename_at_272_218,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_275_222) -> void{
        clone_8539bd2d8add7e73(__context__,__res_rename_at_273_220,__func_rename_at_275_222);
        ++__count_rename_at_274_221;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_274_221 > 1 )
    {
        if ( !__canfail_rename_at_272_219 )
        {
            builtin_throw(((char *) "more than one generic function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_273_220);
    };
}}

inline ExprBlock * setup_call_list_86425b60d55e3039 ( Context * __context__, char * const  __name_rename_at_288_223, LineInfo const  &  __at_rename_at_288_224, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __subblock_rename_at_288_225 ) { das_stack_prologue __prologue(__context__,192,"setup_call_list " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_289_226; memset((void*)&__fn_rename_at_289_226,0,sizeof(__fn_rename_at_289_226));
    smart_ptr_raw<ExprBlock> __blk_rename_at_295_228; memset((void*)&__blk_rename_at_295_228,0,sizeof(__blk_rename_at_295_228));
    ExprBlock * __res_rename_at_296_229; memset((void*)&__res_rename_at_296_229,0,sizeof(__res_rename_at_296_229));
    /* finally */ auto __finally_288= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_289_226);
    /* end finally */ });
    __fn_rename_at_289_226; das_zero(__fn_rename_at_289_226); das_move(__fn_rename_at_289_226, find_unique_function_d2492cf8492d63ba(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_288_223,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_289_226),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_290= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_295_228);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_289_226),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_291) {
            das_copy((__mks_291.at /*at*/),(__at_rename_at_288_224));
            das_copy((__mks_291.atDecl /*atDecl*/),(__at_rename_at_288_224));
            das_copy((__mks_291.flags /*flags*/),(0x200000u));
            das_move((__mks_291.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_293) {
                das_copy((__mks_293.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_293.at /*at*/),(__at_rename_at_288_224));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_291.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_9, cast<char * const >::from(__name_rename_at_288_223))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_295_228; das_zero(__blk_rename_at_295_228); das_move(__blk_rename_at_295_228, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_295) {
            das_copy((__mks_295.at /*at*/),(__at_rename_at_288_224));
        }))));
        __res_rename_at_296_229 = _FuncbuiltinTickget_ptrTick5807679485210906136_4e4f7194eb0890d6(__context__,__blk_rename_at_295_228);
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_289_226->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_295_228),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__subblock_rename_at_288_225,__fn_rename_at_289_226);
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_289_226,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_10, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_289_226->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(__res_rename_at_296_229);
    } else if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_289_226->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
    {
        builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__fn_rename_at_289_226->body /*body*/));
}}

inline ExprBlock * setup_call_list_180d45678895ad8a ( Context * __context__, char * const  __name_rename_at_309_230, LineInfo const  &  __at_rename_at_309_231, bool __isInit_rename_at_309_232, bool __isPrivate_rename_at_309_233, bool __isLateInit_rename_at_309_234 ) { das_stack_prologue __prologue(__context__,96,"setup_call_list " DAS_FILE_LINE);
{
    return das_auto_cast<ExprBlock *>::cast(setup_call_list_86425b60d55e3039(__context__,__name_rename_at_309_230,__at_rename_at_309_231,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__3b2c4edb933bef4a,[&](smart_ptr_raw<Function> __fn_rename_at_310_235) -> void{
        if ( __isInit_rename_at_309_232 )
        {
            __fn_rename_at_310_235->flags /*flags*/ |= 0x100u;
            if ( __isLateInit_rename_at_309_234 )
            {
                __fn_rename_at_310_235->moreFlags /*moreFlags*/ |= 0x8u;
            };
        };
        if ( __isPrivate_rename_at_309_233 )
        {
            __fn_rename_at_310_235->flags /*flags*/ |= 0x400000u;
        };
    })));
}}

inline ExprBlock * setup_macro_1eb22d9624e827a6 ( Context * __context__, char * const  __name_rename_at_323_236, LineInfo const  &  __at_rename_at_323_237 ) { das_stack_prologue __prologue(__context__,480,"setup_macro " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_325_238; memset((void*)&__fn_rename_at_325_238,0,sizeof(__fn_rename_at_325_238));
    smart_ptr_raw<ExprBlock> __blk_rename_at_331_240; memset((void*)&__blk_rename_at_331_240,0,sizeof(__blk_rename_at_331_240));
    smart_ptr_raw<ExprBlock> __iblk_rename_at_332_241; memset((void*)&__iblk_rename_at_332_241,0,sizeof(__iblk_rename_at_332_241));
    ExprBlock * __res_rename_at_333_242; memset((void*)&__res_rename_at_333_242,0,sizeof(__res_rename_at_333_242));
    smart_ptr_raw<ExprCall> __ifm_rename_at_334_243; memset((void*)&__ifm_rename_at_334_243,0,sizeof(__ifm_rename_at_334_243));
    smart_ptr_raw<ExprConstString> __ifmn_rename_at_335_245; memset((void*)&__ifmn_rename_at_335_245,0,sizeof(__ifmn_rename_at_335_245));
    smart_ptr_raw<ExprIfThenElse> __ife_rename_at_337_247; memset((void*)&__ife_rename_at_337_247,0,sizeof(__ife_rename_at_337_247));
    ExprBlock * __blk_rename_at_349_248; memset((void*)&__blk_rename_at_349_248,0,sizeof(__blk_rename_at_349_248));
    ExprIfThenElse * __ife_rename_at_353_249; memset((void*)&__ife_rename_at_353_249,0,sizeof(__ife_rename_at_353_249));
    /* finally */ auto __finally_323= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_325_238);
    /* end finally */ });
    compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->flags /*flags*/ |= 0x20u;
    __fn_rename_at_325_238; das_zero(__fn_rename_at_325_238); das_move(__fn_rename_at_325_238, find_unique_function_d2492cf8492d63ba(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_323_236,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_325_238),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_326= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprIfThenElse>>::clear(__context__,__ife_rename_at_337_247);
        das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__ifmn_rename_at_335_245);
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__ifm_rename_at_334_243);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__iblk_rename_at_332_241);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_331_240);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_325_238),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_327) {
            das_copy((__mks_327.at /*at*/),(__at_rename_at_323_237));
            das_copy((__mks_327.atDecl /*atDecl*/),(__at_rename_at_323_237));
            das_copy((__mks_327.flags /*flags*/),(0x80600000u));
            das_move((__mks_327.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_329) {
                das_copy((__mks_329.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_329.at /*at*/),(__at_rename_at_323_237));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_327.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_11, cast<char * const >::from(__name_rename_at_323_236))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_331_240; das_zero(__blk_rename_at_331_240); das_move(__blk_rename_at_331_240, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_331) {
            das_copy((__mks_331.at /*at*/),(__at_rename_at_323_237));
        }))));
        __iblk_rename_at_332_241; das_zero(__iblk_rename_at_332_241); das_move(__iblk_rename_at_332_241, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_332) {
            das_copy((__mks_332.at /*at*/),(__at_rename_at_323_237));
        }))));
        __res_rename_at_333_242 = ((ExprBlock *)_FuncbuiltinTickget_ptrTick5807679485210906136_4e4f7194eb0890d6(__context__,__iblk_rename_at_332_241));
        __ifm_rename_at_334_243; das_zero(__ifm_rename_at_334_243); das_move(__ifm_rename_at_334_243, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_334) {
            das_copy((__mks_334.at /*at*/),(__at_rename_at_323_237));
            {
                set_das_string(das_arg<das::string>::pass(__mks_334.name /*name*/),((char *) "is_compiling_macros_in_module"));
            }        }))));
        __ifmn_rename_at_335_245; das_zero(__ifmn_rename_at_335_245); das_move(__ifmn_rename_at_335_245, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_335) {
            das_copy((__mks_335.at /*at*/),(__at_rename_at_323_237));
            {
                das_clone<das::string,das::string>::clone(__mks_335.text /*value*/,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->name /*name*/);
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__ifm_rename_at_334_243->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__ifmn_rename_at_335_245));
        __ife_rename_at_337_247; das_zero(__ife_rename_at_337_247); das_move(__ife_rename_at_337_247, das_ascend_handle<true,smart_ptr_raw<ExprIfThenElse>>::make(__context__,nullptr,(([&](ExprIfThenElse & __mks_337) {
            das_copy((__mks_337.at /*at*/),(__at_rename_at_323_237));
            das_move((__mks_337.cond /*cond*/),(__ifm_rename_at_334_243));
            das_move((__mks_337.if_true /*if_true*/),(__iblk_rename_at_332_241));
        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_331_240->list /*list*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__ife_rename_at_337_247));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_325_238->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_331_240),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_325_238,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_12, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_325_238->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__res_rename_at_333_242));
    } else {
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_325_238->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __blk_rename_at_349_248 = das_cast<ExprBlock *>::cast(__fn_rename_at_325_238->body /*body*/);
        if ( (das_vector_length(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_349_248->list /*list*/)) != 1) || (SimPolicy<char *>::NotEqu(cast<char *>::from(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_349_248->list /*list*/,0,__context__)->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprIfThenElse")),*__context__,nullptr)) )
        {
            builtin_throw(((char *) "expecting is_compiling_macros"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __ife_rename_at_353_249 = das_cast<ExprIfThenElse *>::cast(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_349_248->list /*list*/,0,__context__));
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__ife_rename_at_353_249->if_true /*if_true*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting ife.if_true.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__ife_rename_at_353_249->if_true /*if_true*/));
    };
}}

inline void * panic_expr_as_4c38e300a4bc92c4 ( Context * __context__ )
{
    builtin_throw(((char *) "invalid 'as' expression or null pointer dereference"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast<void *>::cast(nullptr);
}

inline bool _FuncTickisTickBuiltInFunction_67b8133cad1f66d3 ( Context * __context__, Function * const  __foo_rename_at_563_250 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__foo_rename_at_563_250->flags /*flags*/,1u << 0));
}

inline BuiltInFunction * _FuncTickasTickBuiltInFunction_86b221c9be037d90 ( Context * __context__, Function * const  __foo_rename_at_567_251 )
{
    DAS_VERIFY((_FuncTickisTickBuiltInFunction_67b8133cad1f66d3(__context__,__foo_rename_at_567_251)));
    return das_auto_cast<BuiltInFunction *>::cast(das_cast<BuiltInFunction *>::cast(__foo_rename_at_567_251));
}

inline bool _FuncTickisTickExternalFnBase_f41cfb59a73b3862 ( Context * __context__, Function * const  __foo_rename_at_576_252 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__foo_rename_at_576_252->moreFlags /*moreFlags*/,1u << 12) && das_get_bitfield(__foo_rename_at_576_252->flags /*flags*/,1u << 0));
}

inline ExternalFnBase * _FuncTickasTickExternalFnBase_fb0ab060e41b89fb ( Context * __context__, Function * const  __foo_rename_at_580_253 )
{
    DAS_VERIFY((_FuncTickisTickExternalFnBase_f41cfb59a73b3862(__context__,__foo_rename_at_580_253)));
    return das_auto_cast<ExternalFnBase *>::cast(das_cast<ExternalFnBase *>::cast(__foo_rename_at_580_253));
}

inline bool _FuncTickisTickFunctionAnnotation_a9c64a70780109c6 ( Context * __context__, Annotation * const  __foo_rename_at_589_254 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_589_254)).rtti_isFunctionAnnotation()));
}

inline bool _FuncTickisTickFunctionAnnotation_55ebe0a1ac05ff73 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_593_255 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_593_255)).rtti_isFunctionAnnotation()));
}

inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_a2759eda58bebeb4 ( Context * __context__, Annotation * const  __foo_rename_at_597_256 )
{
    DAS_VERIFY((_FuncTickisTickFunctionAnnotation_a9c64a70780109c6(__context__,__foo_rename_at_597_256)));
    return das_auto_cast<FunctionAnnotation *>::cast(das_cast<FunctionAnnotation *>::cast(__foo_rename_at_597_256));
}

inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_d2eaa6a8d42295ac ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_602_257 )
{
    DAS_VERIFY((_FuncTickisTickFunctionAnnotation_55ebe0a1ac05ff73(__context__,__foo_rename_at_602_257)));
    return das_auto_cast<FunctionAnnotation *>::cast(das_cast<FunctionAnnotation *>::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_fa24e3c4e479e739(__context__,__foo_rename_at_602_257)));
}

inline bool _FuncTickisTickStructureAnnotation_3284abe043d58747 ( Context * __context__, Annotation * const  __foo_rename_at_612_258 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_612_258)).rtti_isStructureAnnotation()));
}

inline bool _FuncTickisTickStructureAnnotation_8e0018d5eb2637f2 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_616_259 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_616_259)).rtti_isStructureAnnotation()));
}

inline StructureAnnotation * _FuncTickasTickStructureAnnotation_e33130a234a0a0d3 ( Context * __context__, Annotation * const  __foo_rename_at_620_260 )
{
    DAS_VERIFY((_FuncTickisTickStructureAnnotation_3284abe043d58747(__context__,__foo_rename_at_620_260)));
    return das_auto_cast<StructureAnnotation *>::cast(das_cast<StructureAnnotation *>::cast(__foo_rename_at_620_260));
}

inline StructureAnnotation * _FuncTickasTickStructureAnnotation_9f2533762c926957 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_625_261 )
{
    DAS_VERIFY((_FuncTickisTickStructureAnnotation_8e0018d5eb2637f2(__context__,__foo_rename_at_625_261)));
    return das_auto_cast<StructureAnnotation *>::cast(das_cast<StructureAnnotation *>::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_fa24e3c4e479e739(__context__,__foo_rename_at_625_261)));
}

inline smart_ptr_raw<ExprCall> to_array_move_expr_6c95c6c3a47afc48 ( Context * __context__, smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_630_262, LineInfo const  &  __at_rename_at_630_263 ) { das_stack_prologue __prologue(__context__,160,"to_array_move_expr " DAS_FILE_LINE);
{
    LineInfo __tmp_loc_rename_at_631_264; memset((void*)&__tmp_loc_rename_at_631_264,0,sizeof(__tmp_loc_rename_at_631_264));
    smart_ptr_raw<ExprCall> __mkToArrayMove_rename_at_633_265; memset((void*)&__mkToArrayMove_rename_at_633_265,0,sizeof(__mkToArrayMove_rename_at_633_265));
    /* finally */ auto __finally_630= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToArrayMove_rename_at_633_265);
    /* end finally */ });
    __tmp_loc_rename_at_631_264 = __at_rename_at_630_263;
    __tmp_loc_rename_at_631_264.column /*column*/ += 0x5u;
    __mkToArrayMove_rename_at_633_265; das_zero(__mkToArrayMove_rename_at_633_265); das_move(__mkToArrayMove_rename_at_633_265, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_633) {
        das_copy((__mks_633.at /*at*/),(__tmp_loc_rename_at_631_264));
        {
            set_das_string(das_arg<das::string>::pass(__mks_633.name /*name*/),((char *) "to_array_move"));
        }    }))));
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToArrayMove_rename_at_633_265->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkArr_rename_at_630_262));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprCall>>::cast(__mkToArrayMove_rename_at_633_265);
}}

inline smart_ptr_raw<Expression> walk_and_convert_array_6fddd8e31acbfae0 ( Context * __context__, uint8_t const  * const  __data_rename_at_638_267, smart_ptr_raw<TypeDecl> const  __info_rename_at_638_268, LineInfo const  &  __at_rename_at_638_269 ) { das_stack_prologue __prologue(__context__,176,"walk_and_convert_array " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_641_271; memset((void*)&__stride_rename_at_641_271,0,sizeof(__stride_rename_at_641_271));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_642_272; memset((void*)&__mkArr_rename_at_642_272,0,sizeof(__mkArr_rename_at_642_272));
    int32_t __total_rename_at_639_270 = ((int32_t)any_array_size(das_auto_cast<void * const >::cast(__data_rename_at_638_267)));
    if ( __total_rename_at_639_270 != 0 )
    {
        /* finally */ auto __finally_640= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_642_272);
        /* end finally */ });
        __stride_rename_at_641_271 = ((int32_t)((das_deref(__context__,__info_rename_at_638_268->firstType /*firstType*/)).getSizeOf()));
        __mkArr_rename_at_642_272; das_zero(__mkArr_rename_at_642_272); das_move(__mkArr_rename_at_642_272, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_642) {
            das_copy((__mks_642.at /*at*/),(__at_rename_at_638_269));
            das_move((__mks_642.makeType /*makeType*/),(clone_type(__info_rename_at_638_268->firstType /*firstType*/)));
        }))));
        any_array_foreach(das_auto_cast<void * const >::cast(__data_rename_at_638_267),__stride_rename_at_641_271,das_make_block<void,void *>(__context__,144,0,&__func_info__b28b606ddb7a6760,[&](void * __value_rename_at_643_273) -> void{
            emplace_new_60d5871f9e959ff9(__context__,das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_642_272->values /*values*/),walk_and_convert_badf850f139f3bb3(__context__,das_auto_cast<uint8_t const  * const >::cast(__value_rename_at_643_273),__info_rename_at_638_268->firstType /*firstType*/,__at_rename_at_638_269));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_642_272->makeType /*makeType*/->dim /*dim*/),__total_rename_at_639_270);
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(to_array_move_expr_6c95c6c3a47afc48(__context__,_FuncbuiltinTickcloneTick9602082126133410484_54c8d45902a12b6a(__context__,__mkArr_rename_at_642_272),__at_rename_at_638_269));
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_649) {
            das_copy((__mks_649.at /*at*/),(__at_rename_at_638_269));
            das_move((__mks_649.makeType /*makeType*/),(clone_type(__info_rename_at_638_268)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_dim_5e577a4630aa7d27 ( Context * __context__, uint8_t const  * const  __data_rename_at_653_274, smart_ptr_raw<TypeDecl> const  __info_rename_at_653_275, LineInfo const  &  __at_rename_at_653_276 ) { das_stack_prologue __prologue(__context__,208,"walk_and_convert_dim " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_654_277; memset((void*)&__stride_rename_at_654_277,0,sizeof(__stride_rename_at_654_277));
    int32_t __total_rename_at_655_278; memset((void*)&__total_rename_at_655_278,0,sizeof(__total_rename_at_655_278));
    smart_ptr_raw<TypeDecl> __einfo_rename_at_656_279; memset((void*)&__einfo_rename_at_656_279,0,sizeof(__einfo_rename_at_656_279));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_658_280; memset((void*)&__mkArr_rename_at_658_280,0,sizeof(__mkArr_rename_at_658_280));
    smart_ptr_raw<Expression> __elem_rename_at_661_283; memset((void*)&__elem_rename_at_661_283,0,sizeof(__elem_rename_at_661_283));
    /* finally */ auto __finally_653= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_658_280);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__einfo_rename_at_656_279);
    /* end finally */ });
    __stride_rename_at_654_277 = ((int32_t)((das_deref(__context__,__info_rename_at_653_275)).getBaseSizeOf()));
    __total_rename_at_655_278 = ((int32_t)((das_deref(__context__,__info_rename_at_653_275)).getCountOf()));
    __einfo_rename_at_656_279; das_zero(__einfo_rename_at_656_279); das_move(__einfo_rename_at_656_279, clone_type(__info_rename_at_653_275));
    das_vector_clear(das_arg<das::vector<int32_t>>::pass(__einfo_rename_at_656_279->dim /*dim*/));
    __mkArr_rename_at_658_280; das_zero(__mkArr_rename_at_658_280); das_move(__mkArr_rename_at_658_280, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_658) {
        das_copy((__mks_658.at /*at*/),(__at_rename_at_653_276));
        {
            clone_69ab1a2184065196(__context__,__mks_658.makeType /*makeType*/,__info_rename_at_653_275);
        }    }))));
    {
        bool __need_loop_659 = true;
        // x: int const
        das_iterator<range> __x_iterator(mk_range(__total_rename_at_655_278));
        int32_t __x_rename_at_659_282;
        __need_loop_659 = __x_iterator.first(__context__,(__x_rename_at_659_282)) && __need_loop_659;
        for ( ; __need_loop_659 ; __need_loop_659 = __x_iterator.next(__context__,(__x_rename_at_659_282)) )
        {
            __elem_rename_at_661_283; das_zero(__elem_rename_at_661_283); das_move(__elem_rename_at_661_283, walk_and_convert_badf850f139f3bb3(__context__,das_ptr_add_int32(__data_rename_at_653_274,__stride_rename_at_654_277 * __x_rename_at_659_282,1),__einfo_rename_at_656_279,__at_rename_at_653_276));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_658_280->values /*values*/),__elem_rename_at_661_283);
        }
        __x_iterator.close(__context__,(__x_rename_at_659_282));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_658_280);
}}

inline smart_ptr_raw<Expression> walk_and_convert_pointer_c823df9ddee9c45f ( Context * __context__, uint8_t const  * const  __data_rename_at_668_284, smart_ptr_raw<TypeDecl> const  __info_rename_at_668_285, LineInfo const  &  __at_rename_at_668_286 )
{
    uint8_t const  * __pdata_rename_at_670_287 = ((uint8_t const  *)das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_668_284)));
    smart_ptr_raw<Expression> __elem_rename_at_671_288; das_zero(__elem_rename_at_671_288); das_move(__elem_rename_at_671_288, walk_and_convert_badf850f139f3bb3(__context__,__pdata_rename_at_670_287,__info_rename_at_668_285->firstType /*firstType*/,__at_rename_at_668_286));
    smart_ptr_raw<ExprAscend> __mkAsc_rename_at_672_289; das_zero(__mkAsc_rename_at_672_289); das_move(__mkAsc_rename_at_672_289, das_ascend_handle<true,smart_ptr_raw<ExprAscend>>::make(__context__,nullptr,(([&](ExprAscend & __mks_672) {
        das_copy((__mks_672.at /*at*/),(__at_rename_at_668_286));
        das_move((__mks_672.subexpr /*subexpr*/),(__elem_rename_at_671_288));
    }))));
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkAsc_rename_at_672_289);
}

inline smart_ptr_raw<Expression> walk_and_convert_tuple_7bb6d375ae2e2688 ( Context * __context__, uint8_t const  * const  __data_rename_at_677_290, smart_ptr_raw<TypeDecl> const  __info_rename_at_677_291, LineInfo const  &  __at_rename_at_677_292 )
{
    smart_ptr_raw<ExprMakeTuple> __mkArr_rename_at_678_293; memset((void*)&__mkArr_rename_at_678_293,0,sizeof(__mkArr_rename_at_678_293));
    int32_t __offset_rename_at_680_295; memset((void*)&__offset_rename_at_680_295,0,sizeof(__offset_rename_at_680_295));
    smart_ptr_raw<Expression> __elem_rename_at_682_296; memset((void*)&__elem_rename_at_682_296,0,sizeof(__elem_rename_at_682_296));
    /* finally */ auto __finally_677= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkArr_rename_at_678_293);
    /* end finally */ });
    __mkArr_rename_at_678_293; das_zero(__mkArr_rename_at_678_293); das_move(__mkArr_rename_at_678_293, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_678) {
        das_copy((__mks_678.at /*at*/),(__at_rename_at_677_292));
        das_move((__mks_678.recordType /*recordType*/),(clone_type(__info_rename_at_677_291)));
    }))));
    {
        bool __need_loop_679 = true;
        // idx: int const
        das_iterator<range> __idx_iterator(mk_range(das_vector_length(__info_rename_at_677_291->argTypes /*argTypes*/)));
        int32_t __idx_rename_at_679_294;
        __need_loop_679 = __idx_iterator.first(__context__,(__idx_rename_at_679_294)) && __need_loop_679;
        for ( ; __need_loop_679 ; __need_loop_679 = __idx_iterator.next(__context__,(__idx_rename_at_679_294)) )
        {
            __offset_rename_at_680_295 = ((int32_t)get_tuple_field_offset(__info_rename_at_677_291,__idx_rename_at_679_294,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            __elem_rename_at_682_296; das_zero(__elem_rename_at_682_296); das_move(__elem_rename_at_682_296, walk_and_convert_badf850f139f3bb3(__context__,das_ptr_add_int32(__data_rename_at_677_290,__offset_rename_at_680_295,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_677_291->argTypes /*argTypes*/,__idx_rename_at_679_294,__context__),__at_rename_at_677_292));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_678_293->values /*values*/),__elem_rename_at_682_296);
        }
        __idx_iterator.close(__context__,(__idx_rename_at_679_294));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_678_293);
}

inline smart_ptr_raw<Expression> walk_and_convert_structure_b13b947d23bd62c7 ( Context * __context__, uint8_t const  * const  __data_rename_at_689_297, smart_ptr_raw<TypeDecl> const  __info_rename_at_689_298, LineInfo const  &  __at_rename_at_689_299 ) { das_stack_prologue __prologue(__context__,368,"walk_and_convert_structure " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprMakeStruct> __mkStruct_rename_at_690_300; memset((void*)&__mkStruct_rename_at_690_300,0,sizeof(__mkStruct_rename_at_690_300));
    Structure * __stype_rename_at_691_301; memset((void*)&__stype_rename_at_691_301,0,sizeof(__stype_rename_at_691_301));
    smart_ptr_raw<MakeStruct> __mkS_rename_at_692_302; memset((void*)&__mkS_rename_at_692_302,0,sizeof(__mkS_rename_at_692_302));
    uint8_t const  * __fdata_rename_at_696_304; memset((void*)&__fdata_rename_at_696_304,0,sizeof(__fdata_rename_at_696_304));
    smart_ptr_raw<Expression> __elem_rename_at_697_305; memset((void*)&__elem_rename_at_697_305,0,sizeof(__elem_rename_at_697_305));
    smart_ptr_raw<MakeFieldDecl> __mkF_rename_at_698_306; memset((void*)&__mkF_rename_at_698_306,0,sizeof(__mkF_rename_at_698_306));
    /* finally */ auto __finally_689= das_finally([&](){
    das_delete_handle<smart_ptr_raw<MakeStruct>>::clear(__context__,__mkS_rename_at_692_302);
    das_delete_handle<smart_ptr_raw<ExprMakeStruct>>::clear(__context__,__mkStruct_rename_at_690_300);
    /* end finally */ });
    __mkStruct_rename_at_690_300; das_zero(__mkStruct_rename_at_690_300); das_move(__mkStruct_rename_at_690_300, das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_690) {
        das_copy((__mks_690.at /*at*/),(__at_rename_at_689_299));
        das_move((__mks_690.makeType /*makeType*/),(clone_type(__info_rename_at_689_298)));
    }))));
    __stype_rename_at_691_301 = ((Structure *)__info_rename_at_689_298->structType /*structType*/);
    __mkS_rename_at_692_302; das_zero(__mkS_rename_at_692_302); das_move(__mkS_rename_at_692_302, das_new_handle<MakeStruct,true>::make(__context__));
    {
        bool __need_loop_693 = true;
        // x: ast::FieldDeclaration const&
        das_iterator<das::vector<Structure::FieldDeclaration> const > __x_iterator(__stype_rename_at_691_301->fields /*fields*/);
        Structure::FieldDeclaration const  * __x_rename_at_693_303;
        __need_loop_693 = __x_iterator.first(__context__,(__x_rename_at_693_303)) && __need_loop_693;
        for ( ; __need_loop_693 ; __need_loop_693 = __x_iterator.next(__context__,(__x_rename_at_693_303)) )
        {
            AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_694_28_7; _temp_make_local_694_28_7;
            if ( das_get_auto_variant_field<vec4f,8,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((_temp_make_local_694_28_7 = (find_arg_b6ffaee9719f2933(__context__,(*__x_rename_at_693_303).annotation /*annotation*/,((char *) "do_not_convert"))))) )
            {
                __fdata_rename_at_696_304 = ((uint8_t const  *)das_ptr_add_int32(__data_rename_at_689_297,(*__x_rename_at_693_303).offset /*offset*/,1));
                __elem_rename_at_697_305; das_zero(__elem_rename_at_697_305); das_move(__elem_rename_at_697_305, walk_and_convert_badf850f139f3bb3(__context__,__fdata_rename_at_696_304,(*__x_rename_at_693_303).type /*_type*/,__at_rename_at_689_299));
                __mkF_rename_at_698_306; das_zero(__mkF_rename_at_698_306); das_move(__mkF_rename_at_698_306, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_698) {
                    das_copy((__mks_698.at /*at*/),(__at_rename_at_689_299));
                    das_move((__mks_698.value /*value*/),(__elem_rename_at_697_305));
                    {
                        das_clone<das::string,das::string>::clone(__mks_698.name /*name*/,(*__x_rename_at_693_303).name /*name*/);
                    }                }))));
                if ( !((das_deref(__context__,(*__x_rename_at_693_303).type /*_type*/)).canCopy()) )
                {
                    das_copy(__mkF_rename_at_698_306->flags /*flags*/,0x1u);
                };
                emplace_new_eba12a326cb79d4c(__context__,das_arg<MakeStruct>::pass(das_deref(__context__,__mkS_rename_at_692_302)),__mkF_rename_at_698_306);
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_693_303));
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeStruct>>>::pass(__mkStruct_rename_at_690_300->structs /*structs*/),__mkS_rename_at_692_302);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkStruct_rename_at_690_300);
}}

inline smart_ptr_raw<Expression> walk_and_convert_variant_1aa4185043fe5555 ( Context * __context__, uint8_t const  * const  __data_rename_at_710_308, smart_ptr_raw<TypeDecl> const  __info_rename_at_710_309, LineInfo const  &  __at_rename_at_710_310 ) { das_stack_prologue __prologue(__context__,208,"walk_and_convert_variant " DAS_FILE_LINE);
{
    int32_t __vindex_rename_at_712_311 = ((int32_t)das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_710_308)));
    int32_t __offset_rename_at_713_312 = ((int32_t)get_variant_field_offset(__info_rename_at_710_309,__vindex_rename_at_712_311,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    smart_ptr_raw<ExprMakeVariant> __mkVariant_rename_at_714_313; das_zero(__mkVariant_rename_at_714_313); das_move(__mkVariant_rename_at_714_313, das_ascend_handle<true,smart_ptr_raw<ExprMakeVariant>>::make(__context__,nullptr,(([&](ExprMakeVariant & __mks_714) {
        das_copy((__mks_714.at /*at*/),(__at_rename_at_710_310));
        das_move((__mks_714.makeType /*makeType*/),(clone_type(__info_rename_at_710_309)));
    }))));
    smart_ptr_raw<Expression> __elem_rename_at_715_314; das_zero(__elem_rename_at_715_314); das_move(__elem_rename_at_715_314, walk_and_convert_badf850f139f3bb3(__context__,das_ptr_add_int32(__data_rename_at_710_308,__offset_rename_at_713_312,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_710_309->argTypes /*argTypes*/,__vindex_rename_at_712_311,__context__),__at_rename_at_710_310));
    smart_ptr_raw<MakeFieldDecl> __mkS_rename_at_716_315; das_zero(__mkS_rename_at_716_315); das_move(__mkS_rename_at_716_315, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_716) {
        das_copy((__mks_716.at /*at*/),(__at_rename_at_710_310));
        das_move((__mks_716.value /*value*/),(__elem_rename_at_715_314));
        {
            das_clone<das::string,das::string>::clone(__mks_716.name /*name*/,das_index<das::vector<das::string> const >::at(__info_rename_at_710_309->argNames /*argNames*/,__vindex_rename_at_712_311,__context__));
        }    }))));
    if ( !((das_deref(__context__,das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_710_309->argTypes /*argTypes*/,__vindex_rename_at_712_311,__context__))).canCopy()) )
    {
        das_copy(__mkS_rename_at_716_315->flags /*flags*/,0x1u);
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeFieldDecl>>>::pass(__mkVariant_rename_at_714_313->variants /*variants*/),__mkS_rename_at_716_315);
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkVariant_rename_at_714_313);
}}

inline smart_ptr_raw<Expression> walk_and_convert_table_8242a4ff974756c0 ( Context * __context__, uint8_t const  * const  __data_rename_at_725_317, smart_ptr_raw<TypeDecl> const  __info_rename_at_725_318, LineInfo const  &  __at_rename_at_725_319 ) { das_stack_prologue __prologue(__context__,384,"walk_and_convert_table " DAS_FILE_LINE);
{
    smart_ptr_raw<TypeDecl> __tupT_rename_at_728_321; memset((void*)&__tupT_rename_at_728_321,0,sizeof(__tupT_rename_at_728_321));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_731_322; memset((void*)&__mkArr_rename_at_731_322,0,sizeof(__mkArr_rename_at_731_322));
    int32_t __key_stride_rename_at_732_323; memset((void*)&__key_stride_rename_at_732_323,0,sizeof(__key_stride_rename_at_732_323));
    int32_t __value_stride_rename_at_733_324; memset((void*)&__value_stride_rename_at_733_324,0,sizeof(__value_stride_rename_at_733_324));
    smart_ptr_raw<ExprCall> __mkToTableMove_rename_at_743_330; memset((void*)&__mkToTableMove_rename_at_743_330,0,sizeof(__mkToTableMove_rename_at_743_330));
    int32_t __total_rename_at_726_320 = ((int32_t)any_table_size(das_auto_cast<void * const >::cast(__data_rename_at_725_317)));
    if ( __total_rename_at_726_320 != 0 )
    {
        /* finally */ auto __finally_727= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToTableMove_rename_at_743_330);
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_731_322);
        das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__tupT_rename_at_728_321);
        /* end finally */ });
        __tupT_rename_at_728_321; das_zero(__tupT_rename_at_728_321); das_move(__tupT_rename_at_728_321, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_728) {
            das_copy((__mks_728.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tTuple));
            das_copy((__mks_728.at /*at*/),(__at_rename_at_725_319));
        }))));
        emplace_new_e80d981a19168757(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_728_321->argTypes /*argTypes*/),clone_type(__info_rename_at_725_318->firstType /*firstType*/));
        emplace_new_e80d981a19168757(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_728_321->argTypes /*argTypes*/),clone_type(__info_rename_at_725_318->secondType /*secondType*/));
        __mkArr_rename_at_731_322; das_zero(__mkArr_rename_at_731_322); das_move(__mkArr_rename_at_731_322, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_731) {
            das_copy((__mks_731.at /*at*/),(__at_rename_at_725_319));
            das_move((__mks_731.makeType /*makeType*/),(clone_type(__tupT_rename_at_728_321)));
        }))));
        __key_stride_rename_at_732_323 = ((int32_t)((das_deref(__context__,__info_rename_at_725_318->firstType /*firstType*/)).getSizeOf()));
        __value_stride_rename_at_733_324 = ((int32_t)((das_deref(__context__,__info_rename_at_725_318->secondType /*secondType*/)).getSizeOf()));
        any_table_foreach(das_auto_cast<void * const >::cast(__data_rename_at_725_317),__key_stride_rename_at_732_323,__value_stride_rename_at_733_324,das_make_block<void,void *,void *>(__context__,192,0,&__func_info__b50c88059cb1bcc,[&](void * __pkey_rename_at_734_325, void * __pvalue_rename_at_734_326) -> void{
            smart_ptr_raw<Expression> __key_rename_at_735_327; memset((void*)&__key_rename_at_735_327,0,sizeof(__key_rename_at_735_327));
            smart_ptr_raw<Expression> __value_rename_at_736_328; memset((void*)&__value_rename_at_736_328,0,sizeof(__value_rename_at_736_328));
            smart_ptr_raw<ExprMakeTuple> __mkTup_rename_at_737_329; memset((void*)&__mkTup_rename_at_737_329,0,sizeof(__mkTup_rename_at_737_329));
            /* finally */ auto __finally_734= das_finally([&](){
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__value_rename_at_736_328);
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__key_rename_at_735_327);
            das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkTup_rename_at_737_329);
            /* end finally */ });
            __key_rename_at_735_327; das_zero(__key_rename_at_735_327); das_move(__key_rename_at_735_327, walk_and_convert_badf850f139f3bb3(__context__,das_auto_cast<uint8_t const  * const >::cast(__pkey_rename_at_734_325),__info_rename_at_725_318->firstType /*firstType*/,__at_rename_at_725_319));
            __value_rename_at_736_328; das_zero(__value_rename_at_736_328); das_move(__value_rename_at_736_328, walk_and_convert_badf850f139f3bb3(__context__,das_auto_cast<uint8_t const  * const >::cast(__pvalue_rename_at_734_326),__info_rename_at_725_318->secondType /*secondType*/,__at_rename_at_725_319));
            __mkTup_rename_at_737_329; das_zero(__mkTup_rename_at_737_329); das_move(__mkTup_rename_at_737_329, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_737) {
                das_copy((__mks_737.at /*at*/),(__at_rename_at_725_319));
                das_move((__mks_737.recordType /*recordType*/),(clone_type(__tupT_rename_at_728_321)));
                das_copy((__mks_737.isKeyValue /*isKeyValue*/),(true));
            }))));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_737_329->values /*values*/),__key_rename_at_735_327);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_737_329->values /*values*/),__value_rename_at_736_328);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_731_322->values /*values*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkTup_rename_at_737_329));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_731_322->makeType /*makeType*/->dim /*dim*/),__total_rename_at_726_320);
        __mkToTableMove_rename_at_743_330; das_zero(__mkToTableMove_rename_at_743_330); das_move(__mkToTableMove_rename_at_743_330, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_743) {
            das_copy((__mks_743.at /*at*/),(__at_rename_at_725_319));
            {
                set_das_string(das_arg<das::string>::pass(__mks_743.name /*name*/),((char *) "to_table_move"));
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToTableMove_rename_at_743_330->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkArr_rename_at_731_322));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkToTableMove_rename_at_743_330);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_747) {
            das_copy((__mks_747.at /*at*/),(__at_rename_at_725_319));
            das_move((__mks_747.makeType /*makeType*/),(clone_type(__info_rename_at_725_318)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_basic_f4fa8f2044f51a22 ( Context * __context__, uint8_t const  * const  __data_rename_at_751_332, smart_ptr_raw<TypeDecl> const  __info_rename_at_751_333, LineInfo const  &  __at_rename_at_751_334 ) { das_stack_prologue __prologue(__context__,368,"walk_and_convert_basic " DAS_FILE_LINE);
{
    if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt8 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt8>>::make(__context__,nullptr,(([&](ExprConstInt8 & __mks_754) {
            das_copy((__mks_754.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_754.cvalue<int8_t>() /*value*/),(das_deref(__context__,das_cast<int8_t *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt16 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt16>>::make(__context__,nullptr,(([&](ExprConstInt16 & __mks_756) {
            das_copy((__mks_756.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_756.cvalue<int16_t>() /*value*/),(das_deref(__context__,das_cast<int16_t *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt>>::make(__context__,nullptr,(([&](ExprConstInt & __mks_758) {
            das_copy((__mks_758.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_758.cvalue<int32_t>() /*value*/),(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt64>>::make(__context__,nullptr,(([&](ExprConstInt64 & __mks_760) {
            das_copy((__mks_760.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_760.cvalue<int64_t>() /*value*/),(das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt8 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt8>>::make(__context__,nullptr,(([&](ExprConstUInt8 & __mks_762) {
            das_copy((__mks_762.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_762.cvalue<uint8_t>() /*value*/),(das_deref(__context__,das_cast<uint8_t *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt16 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt16>>::make(__context__,nullptr,(([&](ExprConstUInt16 & __mks_764) {
            das_copy((__mks_764.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_764.cvalue<uint16_t>() /*value*/),(das_deref(__context__,das_cast<uint16_t *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt>>::make(__context__,nullptr,(([&](ExprConstUInt & __mks_766) {
            das_copy((__mks_766.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_766.cvalue<uint32_t>() /*value*/),(das_deref(__context__,das_cast<uint32_t *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt64>>::make(__context__,nullptr,(([&](ExprConstUInt64 & __mks_768) {
            das_copy((__mks_768.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_768.cvalue<uint64_t>() /*value*/),(das_deref(__context__,das_cast<uint64_t *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstFloat>>::make(__context__,nullptr,(([&](ExprConstFloat & __mks_770) {
            das_copy((__mks_770.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_770.cvalue<float>() /*value*/),(das_deref(__context__,das_cast<float *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tDouble )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstDouble>>::make(__context__,nullptr,(([&](ExprConstDouble & __mks_772) {
            das_copy((__mks_772.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_772.cvalue<double>() /*value*/),(das_deref(__context__,das_cast<double *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBool>>::make(__context__,nullptr,(([&](ExprConstBool & __mks_774) {
            das_copy((__mks_774.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_774.cvalue<bool>() /*value*/),(das_deref(__context__,das_cast<bool *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange>>::make(__context__,nullptr,(([&](ExprConstRange & __mks_776) {
            das_copy((__mks_776.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_776.cvalue<range>() /*value*/),(das_deref(__context__,das_cast<range *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange>>::make(__context__,nullptr,(([&](ExprConstURange & __mks_778) {
            das_copy((__mks_778.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_778.cvalue<urange>() /*value*/),(das_deref(__context__,das_cast<urange *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange64>>::make(__context__,nullptr,(([&](ExprConstRange64 & __mks_780) {
            das_copy((__mks_780.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_780.cvalue<range64>() /*value*/),(das_deref(__context__,das_cast<range64 *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange64>>::make(__context__,nullptr,(([&](ExprConstURange64 & __mks_782) {
            das_copy((__mks_782.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_782.cvalue<urange64>() /*value*/),(das_deref(__context__,das_cast<urange64 *>::cast(__data_rename_at_751_332))));
        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_784) {
            das_copy((__mks_784.at /*at*/),(__at_rename_at_751_334));
            {
                set_das_string(das_arg<das::string>::pass(__mks_784.text /*value*/),das_deref(__context__,das_cast<char * *>::cast(__data_rename_at_751_332)));
            }        }))));
    } else if ( __info_rename_at_751_333->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBitfield>>::make(__context__,nullptr,(([&](ExprConstBitfield & __mks_786) {
            das_copy((__mks_786.at /*at*/),(__at_rename_at_751_334));
            das_copy((__mks_786.cvalue<Bitfield>() /*value*/),(das_deref(__context__,das_cast<Bitfield *>::cast(__data_rename_at_751_332))));
        }))));
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<char * const >::from(((char *) "unsupported ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__info_rename_at_751_333->baseType /*baseType*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(nullptr);
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_enumeration_75ada03d57f913e7 ( Context * __context__, uint8_t const  * const  __data_rename_at_794_336, smart_ptr_raw<TypeDecl> const  __info_rename_at_794_337, LineInfo const  &  __at_rename_at_794_338 ) { das_stack_prologue __prologue(__context__,144,"walk_and_convert_enumeration " DAS_FILE_LINE);
{
    int64_t __eval_rename_at_795_339 = INT64_C(0);
    if ( __info_rename_at_794_337->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration )
    {
        das_copy(__eval_rename_at_795_339,int64_t(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_794_336))));
    } else if ( __info_rename_at_794_337->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8 )
    {
        das_copy(__eval_rename_at_795_339,int64_t(das_deref(__context__,__data_rename_at_794_336)));
    } else if ( __info_rename_at_794_337->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16 )
    {
        das_copy(__eval_rename_at_795_339,int64_t(das_deref(__context__,das_cast<int16_t *>::cast(__data_rename_at_794_336))));
    } else if ( __info_rename_at_794_337->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64 )
    {
        das_copy(__eval_rename_at_795_339,das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_794_336)));
    } else {
        builtin_throw(((char *) "unsupported enumeration"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    char * __name_rename_at_807_340 = ((char *)(char *)(((char * const )(ast_find_enum_name(__info_rename_at_794_337->enumType /*enumType*/,__eval_rename_at_795_339,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_807_340),cast<char *>::from(nullptr),*__context__,nullptr) )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_14, cast<char * const >::from(((char *) "can't find name of enum value ")), cast<int64_t>::from(__eval_rename_at_795_339), cast<char * const >::from(((char *) " in ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef(__context__,__info_rename_at_794_337,true,true,true)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstEnumeration>>::make(__context__,nullptr,(([&](ExprConstEnumeration & __mks_809) {
        {
            set_das_string(das_arg<das::string>::pass(__mks_809.text /*value*/),__name_rename_at_807_340);
            clone_6965baf83e84e943(__context__,__mks_809.enumType /*enumType*/,das_cast<Enumeration *>::cast(__info_rename_at_794_337->enumType /*enumType*/));
        }    }))));
}}

inline smart_ptr_raw<Expression> walk_and_convert_badf850f139f3bb3 ( Context * __context__, uint8_t const  * const  __data_rename_at_814_342, smart_ptr_raw<TypeDecl> const  __info_rename_at_814_343, LineInfo const  &  __at_rename_at_814_344 )
{
    if ( das_vector_length(__info_rename_at_814_343->dim /*dim*/) != 0 )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_dim_5e577a4630aa7d27(__context__,__data_rename_at_814_342,__info_rename_at_814_343,__at_rename_at_814_344));
    } else if ( __info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tArray )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_array_6fddd8e31acbfae0(__context__,__data_rename_at_814_342,__info_rename_at_814_343,__at_rename_at_814_344));
    } else if ( __info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        if ( das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_814_342)) == nullptr )
        {
            return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_822) {
                das_copy((__mks_822.at /*at*/),(__at_rename_at_814_344));
            }))));
        } else return das_auto_cast<smart_ptr_raw<Expression>>::cast(((equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__info_rename_at_814_343->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr))) || ((das_deref(__context__,__info_rename_at_814_343->firstType /*firstType*/)).isVoid())) ? das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_824) {
            das_copy((__mks_824.at /*at*/),(__at_rename_at_814_344));
        })))) : das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(walk_and_convert_pointer_c823df9ddee9c45f(__context__,__data_rename_at_814_342,__info_rename_at_814_343,__at_rename_at_814_344)));
    } else if ( __info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tStructure )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_structure_b13b947d23bd62c7(__context__,__data_rename_at_814_342,__info_rename_at_814_343,__at_rename_at_814_344));
    } else if ( __info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tVariant )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_variant_1aa4185043fe5555(__context__,__data_rename_at_814_342,__info_rename_at_814_343,__at_rename_at_814_344));
    } else if ( __info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTuple )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_tuple_7bb6d375ae2e2688(__context__,__data_rename_at_814_342,__info_rename_at_814_343,__at_rename_at_814_344));
    } else if ( __info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTable )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_table_8242a4ff974756c0(__context__,__data_rename_at_814_342,__info_rename_at_814_343,__at_rename_at_814_344));
    } else return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(((((__info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8) || (__info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16)) || (__info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64)) || (__info_rename_at_814_343->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration)) ? das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_enumeration_75ada03d57f913e7(__context__,__data_rename_at_814_342,__info_rename_at_814_343,__at_rename_at_814_344)) : das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_basic_f4fa8f2044f51a22(__context__,__data_rename_at_814_342,__info_rename_at_814_343,__at_rename_at_814_344)));
}

inline void clone_dc75d99dda1cee2c ( Context * __context__, das::vector<CaptureEntry> &  __a_rename_at_870_345, TArray<ast_boost::CaptureEntryInitData> const  &  __b_rename_at_870_346 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_872_20_8; _temp_make_local_872_20_8;
    das_vector_resize(das_arg<das::vector<CaptureEntry>>::pass(__a_rename_at_870_345),builtin_array_size(__b_rename_at_870_346));
    {
        bool __need_loop_872 = true;
        // e: ast_boost::CaptureEntryInitData const&
        das_iterator<TArray<ast_boost::CaptureEntryInitData> const > __e_iterator(__b_rename_at_870_346);
        ast_boost::CaptureEntryInitData const  * __e_rename_at_872_349;
        __need_loop_872 = __e_iterator.first(__context__,(__e_rename_at_872_349)) && __need_loop_872;
        // i: int
        das_iterator_count DAS_COMMENT((_temp_make_local_872_20_8 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
        int32_t __i_rename_at_872_350;
        __need_loop_872 = __i_iterator.first(__context__,(__i_rename_at_872_350)) && __need_loop_872;
        for ( ; __need_loop_872 ; __need_loop_872 = __e_iterator.next(__context__,(__e_rename_at_872_349)) && __i_iterator.next(__context__,(__i_rename_at_872_350)) )
        {
            set_das_string(das_arg<das::string>::pass(das_index<das::vector<CaptureEntry>>::at(__a_rename_at_870_345,__i_rename_at_872_350,__context__).name /*name*/),(*__e_rename_at_872_349).name);
            das_copy(das_index<das::vector<CaptureEntry>>::at(__a_rename_at_870_345,__i_rename_at_872_350,__context__).mode /*mode*/,(*__e_rename_at_872_349).mode);
        }
        __e_iterator.close(__context__,(__e_rename_at_872_349));
        __i_iterator.close(__context__,(__i_rename_at_872_350));
    };
}

inline void clone_eb879c2dc1a7e11e ( Context * __context__, AnnotationArgumentList &  __a_rename_at_906_351, TArray<ast_boost::AnnotationArgumentInitData> const  &  __b_rename_at_906_352 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_908_20_9; _temp_make_local_908_20_9;
    das_vector_resize(das_arg<AnnotationArgumentList>::pass(__a_rename_at_906_351),builtin_array_size(__b_rename_at_906_352));
    {
        bool __need_loop_908 = true;
        // e: ast_boost::AnnotationArgumentInitData const&
        das_iterator<TArray<ast_boost::AnnotationArgumentInitData> const > __e_iterator(__b_rename_at_906_352);
        ast_boost::AnnotationArgumentInitData const  * __e_rename_at_908_355;
        __need_loop_908 = __e_iterator.first(__context__,(__e_rename_at_908_355)) && __need_loop_908;
        // i: int
        das_iterator_count DAS_COMMENT((_temp_make_local_908_20_9 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
        int32_t __i_rename_at_908_356;
        __need_loop_908 = __i_iterator.first(__context__,(__i_rename_at_908_356)) && __need_loop_908;
        for ( ; __need_loop_908 ; __need_loop_908 = __e_iterator.next(__context__,(__e_rename_at_908_355)) && __i_iterator.next(__context__,(__i_rename_at_908_356)) )
        {
            set_das_string(das_arg<das::string>::pass(das_index<AnnotationArgumentList>::at(__a_rename_at_906_351,__i_rename_at_908_356,__context__).name /*name*/),(*__e_rename_at_908_355).name);
            if ( (*__e_rename_at_908_355).basicType == DAS_COMMENT(bound_enum) das::Type::tString )
            {
                set_das_string(das_arg<das::string>::pass(das_index<AnnotationArgumentList>::at(__a_rename_at_906_351,__i_rename_at_908_356,__context__).sValue /*sValue*/),(*__e_rename_at_908_355).sValue);
            } else if ( (*__e_rename_at_908_355).basicType == DAS_COMMENT(bound_enum) das::Type::tBool )
            {
                das_copy(das_index<AnnotationArgumentList>::at(__a_rename_at_906_351,__i_rename_at_908_356,__context__).bValue /*bValue*/,(*__e_rename_at_908_355).bValue);
            } else if ( (*__e_rename_at_908_355).basicType == DAS_COMMENT(bound_enum) das::Type::tInt )
            {
                das_copy(das_index<AnnotationArgumentList>::at(__a_rename_at_906_351,__i_rename_at_908_356,__context__).iValue /*iValue*/,(*__e_rename_at_908_355).iValue);
            } else if ( (*__e_rename_at_908_355).basicType == DAS_COMMENT(bound_enum) das::Type::tFloat )
            {
                das_copy(das_index<AnnotationArgumentList>::at(__a_rename_at_906_351,__i_rename_at_908_356,__context__).fValue /*fValue*/,(*__e_rename_at_908_355).fValue);
            };
            das_copy(das_index<AnnotationArgumentList>::at(__a_rename_at_906_351,__i_rename_at_908_356,__context__).type /*basicType*/,(*__e_rename_at_908_355).basicType);
        }
        __e_iterator.close(__context__,(__e_rename_at_908_355));
        __i_iterator.close(__context__,(__i_rename_at_908_356));
    };
}

inline smart_ptr_raw<TypeDecl> get_vec_dst_type_a1ed8a6bb17eda3a ( Context * __context__, smart_ptr_raw<TypeDecl> __src_tp_rename_at_924_357 )
{
    char * __tstr_rename_at_925_358 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef(__context__,__src_tp_rename_at_924_357,true,true,true)));
    if ( builtin_string_startswith(__tstr_rename_at_925_358,((char *) "$::das_string"),__context__) )
    {
        das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__src_tp_rename_at_924_357);
        return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_928) {
            das_copy((__mks_928.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tString));
        }))));
    } else if ( builtin_string_startswith(__tstr_rename_at_925_358,((char *) "ast::CaptureEntry"),__context__) )
    {
        Structure * __strct_rename_at_930_359 = module_find_structure(das_reinterpret<Module const  * const >::pass(rtti_get_builtin_module(((char *) "ast_boost"))),((char *) "CaptureEntryInitData"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__src_tp_rename_at_924_357);
        return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_932) {
            das_copy((__mks_932.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tStructure));
            das_copy((__mks_932.structType /*structType*/),(__strct_rename_at_930_359));
        }))));
    } else if ( builtin_string_startswith(__tstr_rename_at_925_358,((char *) "rtti::AnnotationArgument"),__context__) )
    {
        Structure * __strct_rename_at_934_360 = module_find_structure(das_reinterpret<Module const  * const >::pass(rtti_get_builtin_module(((char *) "ast_boost"))),((char *) "AnnotationArgumentInitData"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__src_tp_rename_at_924_357);
        return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_936) {
            das_copy((__mks_936.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tStructure));
            das_copy((__mks_936.structType /*structType*/),(__strct_rename_at_934_360));
        }))));
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(__src_tp_rename_at_924_357);
    };
}

inline smart_ptr_raw<ExprCall> to_arr_move_args_7ac8193c43e93926 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __args_rename_at_947_361, smart_ptr_raw<TypeDecl> __dst_tp_rename_at_947_362, LineInfo const  &  __at_rename_at_947_363 ) { das_stack_prologue __prologue(__context__,112,"to_arr_move_args " DAS_FILE_LINE);
{
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprCall>>::cast(to_array_move_expr_6c95c6c3a47afc48(__context__,das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_948) {
        das_copy((__mks_948.gen2 /*gen2*/),(true));
        das_move((__mks_948.makeType /*makeType*/),(clone_type(__dst_tp_rename_at_947_362)));
        das_copy((__mks_948.at /*at*/),(__at_rename_at_947_363));
        {
            _FuncbuiltinTickcloneTick9409548443506319159_df2e383d4b48e41d(__context__,das_arg<das::vector<smart_ptr<Expression>>>::pass(__mks_948.values /*values*/),das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__args_rename_at_947_361));
        }    }))),__at_rename_at_947_363));
}}

inline smart_ptr_raw<Expression> convert_vec_cf3d8478a556d0d6 ( Context * __context__, Module * const  __mod_rename_at_954_365, uint8_t const  * const  __field_values_rename_at_954_366, smart_ptr_raw<TypeDecl> const  __xtype_rename_at_954_367, LineInfo const  &  __at_rename_at_954_368 )
{
    TArray<smart_ptr_raw<Expression>> ___VarargsTickatTick964Tick16_rename_at_964_369; memset((void*)&___VarargsTickatTick964Tick16_rename_at_964_369,0,sizeof(___VarargsTickatTick964Tick16_rename_at_964_369));
    smart_ptr_raw<TypeDecl> __src_tp_rename_at_955_370; memset((void*)&__src_tp_rename_at_955_370,0,sizeof(__src_tp_rename_at_955_370));
    smart_ptr_raw<TypeDecl> __dst_tp_rename_at_958_371; memset((void*)&__dst_tp_rename_at_958_371,0,sizeof(__dst_tp_rename_at_958_371));
    int32_t __fields_num_rename_at_959_372; memset((void*)&__fields_num_rename_at_959_372,0,sizeof(__fields_num_rename_at_959_372));
    smart_ptr_raw<Expression> __arg_expr_rename_at_967_374; memset((void*)&__arg_expr_rename_at_967_374,0,sizeof(__arg_expr_rename_at_967_374));
    smart_ptr_raw<ExprMakeStruct> __make_str_rename_at_969_375; memset((void*)&__make_str_rename_at_969_375,0,sizeof(__make_str_rename_at_969_375));
    /* finally */ auto __finally_954= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__dst_tp_rename_at_958_371);
    _FuncbuiltinTickfinalizeTick13836114024949725080_f304d102cb128dc5(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(___VarargsTickatTick964Tick16_rename_at_964_369));
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__src_tp_rename_at_955_370);
    /* end finally */ });
    das_zero(___VarargsTickatTick964Tick16_rename_at_964_369);
    __src_tp_rename_at_955_370; das_zero(__src_tp_rename_at_955_370); das_move(__src_tp_rename_at_955_370, getHandledTypeIndexTypeDecl(__xtype_rename_at_954_367->annotation /*annotation*/,das_auto_cast<Expression * const >::cast(nullptr),das_auto_cast<Expression * const >::cast(nullptr),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    __src_tp_rename_at_955_370->flags /*flags*/ &= 0xfffffffeu;
    __dst_tp_rename_at_958_371; das_zero(__dst_tp_rename_at_958_371); das_move(__dst_tp_rename_at_958_371, get_vec_dst_type_a1ed8a6bb17eda3a(__context__,_FuncbuiltinTickcloneTick9602082126133410484_ff5eacb201903461(__context__,__src_tp_rename_at_955_370)));
    __fields_num_rename_at_959_372 = ((int32_t)getVectorLength(das_auto_cast<void * const >::cast(__field_values_rename_at_954_366),__src_tp_rename_at_955_370,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    if ( __fields_num_rename_at_959_372 == 0 )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_961) {
            das_copy((__mks_961.at /*at*/),(__at_rename_at_954_368));
            das_move((__mks_961.makeType /*makeType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_961) {
                das_copy((__mks_961.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tArray));
                das_move((__mks_961.firstType /*firstType*/),(clone_type(__dst_tp_rename_at_958_371)));
            })))));
            das_copy((__mks_961.makeStructFlags /*makeStructFlags*/),(0x800u));
        }))));
    } else {
        {
            bool __need_loop_965 = true;
            // idx: int const
            das_iterator<range> __idx_iterator(mk_range(__fields_num_rename_at_959_372));
            int32_t __idx_rename_at_965_373;
            __need_loop_965 = __idx_iterator.first(__context__,(__idx_rename_at_965_373)) && __need_loop_965;
            for ( ; __need_loop_965 ; __need_loop_965 = __idx_iterator.next(__context__,(__idx_rename_at_965_373)) )
            {
                {
                    /* finally */ auto __finally_966= das_finally([&](){
                    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__arg_expr_rename_at_967_374);
                    /* end finally */ });
                    __arg_expr_rename_at_967_374; das_zero(__arg_expr_rename_at_967_374); das_move(__arg_expr_rename_at_967_374, convert_quote_expr_d61fc2e3f77fdf2e(__context__,__mod_rename_at_954_365,das_reinterpret<uint8_t const  *>::pass(das_cast<uint8_t *>::cast(getVectorPtrAtIndex(das_auto_cast<void * const >::cast(__field_values_rename_at_954_366),_FuncbuiltinTickget_ptrTick5807679485210906136_800d8621d2b80648(__context__,__src_tp_rename_at_955_370),__idx_rename_at_965_373,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),__src_tp_rename_at_955_370,__at_rename_at_954_368));
                    if ( (__dst_tp_rename_at_958_371->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tString) && (nequ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__dst_tp_rename_at_958_371),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_tp_rename_at_955_370))) )
                    {
                        /* finally */ auto __finally_968= das_finally([&](){
                        das_delete_handle<smart_ptr_raw<ExprMakeStruct>>::clear(__context__,__make_str_rename_at_969_375);
                        /* end finally */ });
                        __make_str_rename_at_969_375; das_zero(__make_str_rename_at_969_375); das_move(__make_str_rename_at_969_375, das_cast<smart_ptr_raw<ExprMakeStruct>>::cast(__arg_expr_rename_at_967_374));
                        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__make_str_rename_at_969_375->makeType /*makeType*/),das_auto_cast<smart_ptr_raw<void> const >::cast(clone_type(__dst_tp_rename_at_958_371)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        _FuncbuiltinTickemplaceTick13923705686753630697_a043c7eded26b0c1(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(___VarargsTickatTick964Tick16_rename_at_964_369),das_reinterpret<smart_ptr_raw<Expression>>::pass(__make_str_rename_at_969_375));
                    } else {
                        _FuncbuiltinTickemplaceTick13923705686753630697_a043c7eded26b0c1(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(___VarargsTickatTick964Tick16_rename_at_964_369),__arg_expr_rename_at_967_374);
                    };
                };
            }
            __idx_iterator.close(__context__,(__idx_rename_at_965_373));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(to_arr_move_args_7ac8193c43e93926(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(___VarargsTickatTick964Tick16_rename_at_964_369),__dst_tp_rename_at_958_371,__at_rename_at_954_368));
    };
}

inline smart_ptr_raw<MakeFieldDecl> convert_quote_struct_field_529c713c43cf65dc ( Context * __context__, Module * const  __mod_rename_at_1006_376, uint8_t const  * const  __data_rename_at_1006_377, char * const  __tstr_rename_at_1007_378, char * const  __name_rename_at_1007_379, uint32_t __offset_rename_at_1008_380, smart_ptr_raw<TypeDecl> const  __xtype_rename_at_1008_381, LineInfo const  &  __parent_loc_rename_at_1008_382 ) { das_stack_prologue __prologue(__context__,192,"convert_quote_struct_field " DAS_FILE_LINE);
{
    smart_ptr_raw<Expression> ___Varres_fieldTickatTick1014Tick16_rename_at_1014_383; memset((void*)&___Varres_fieldTickatTick1014Tick16_rename_at_1014_383,0,sizeof(___Varres_fieldTickatTick1014Tick16_rename_at_1014_383));
    uint8_t const  * __fdata_rename_at_1013_384; memset((void*)&__fdata_rename_at_1013_384,0,sizeof(__fdata_rename_at_1013_384));
    Bitfield __flags_rename_at_1017_385; memset((void*)&__flags_rename_at_1017_385,0,sizeof(__flags_rename_at_1017_385));
    LineInfo __res_loc_rename_at_1022_386; memset((void*)&__res_loc_rename_at_1022_386,0,sizeof(__res_loc_rename_at_1022_386));
    /* finally */ auto __finally_1008= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,___Varres_fieldTickatTick1014Tick16_rename_at_1014_383);
    /* end finally */ });
    das_zero(___Varres_fieldTickatTick1014Tick16_rename_at_1014_383);
    if ( !_Funcast_boostTickis_quote_field_allowedTick3545799563217800274_8df56ac57aa1aa67(__context__,__name_rename_at_1007_379,__tstr_rename_at_1007_378,__data_rename_at_1006_377) )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<MakeFieldDecl>>::cast(nullptr);
    } else {
        __fdata_rename_at_1013_384 = das_ptr_add_int32(__data_rename_at_1006_377,int32_t(__offset_rename_at_1008_380),1);
        das_move(___Varres_fieldTickatTick1014Tick16_rename_at_1014_383,convert_quote_expr_d61fc2e3f77fdf2e(__context__,__mod_rename_at_1006_376,__fdata_rename_at_1013_384,__xtype_rename_at_1008_381,__parent_loc_rename_at_1008_382));
        if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(___Varres_fieldTickatTick1014Tick16_rename_at_1014_383),das_auto_cast<void * const >::cast(nullptr)) )
        {
            return /* <- */ das_auto_cast_move<smart_ptr_raw<MakeFieldDecl>>::cast(nullptr);
        } else {
            __flags_rename_at_1017_385 = 0;
            if ( !((das_deref(__context__,__xtype_rename_at_1008_381)).canCopy()) )
            {
                das_copy(__flags_rename_at_1017_385,((SimPolicy<char *>::Equ(cast<char *>::from(_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef(__context__,__xtype_rename_at_1008_381,true,true,true)),cast<char *>::from(((char *) "$::das_string")),*__context__,nullptr)) || isYetAnotherVectorTemplate_6471d45428bc715(__context__,__xtype_rename_at_1008_381)) ? das_auto_cast<Bitfield>::cast(0x2u) : das_auto_cast<Bitfield>::cast(0x1u));
            };
            __res_loc_rename_at_1022_386 = __parent_loc_rename_at_1008_382;
            __res_loc_rename_at_1022_386.column /*column*/ *= 0xau;
            return /* <- */ das_auto_cast_move<smart_ptr_raw<MakeFieldDecl>>::cast(das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_1024) {
                das_move((__mks_1024.value /*value*/),(___Varres_fieldTickatTick1014Tick16_rename_at_1014_383));
                das_copy((__mks_1024.flags /*flags*/),(__flags_rename_at_1017_385));
                {
                    das_clone<LineInfo,LineInfo>::clone(__mks_1024.at /*at*/,__parent_loc_rename_at_1008_382);
                    set_das_string(das_arg<das::string>::pass(__mks_1024.name /*name*/),__name_rename_at_1007_379);
                }            }))));
        };
    };
}}

inline smart_ptr_raw<ExprMakeStruct> convert_quote_structure_7513fe9a14da0cec ( Context * __context__, Module * const  __mod_rename_at_1027_388, uint8_t const  * const  __data_rename_at_1027_389, char * const  __tstr_rename_at_1028_390, smart_ptr_raw<TypeDecl> const  __info_rename_at_1028_391, LineInfo const  &  __at_rename_at_1028_392 ) { das_stack_prologue __prologue(__context__,176,"convert_quote_structure " DAS_FILE_LINE);
{
    smart_ptr_raw<MakeStruct> __mkS1_rename_at_1030_393; memset((void*)&__mkS1_rename_at_1030_393,0,sizeof(__mkS1_rename_at_1030_393));
    smart_ptr_raw<ExprMakeStruct> __mkStr_rename_at_1037_399; memset((void*)&__mkStr_rename_at_1037_399,0,sizeof(__mkStr_rename_at_1037_399));
    /* finally */ auto __finally_1028= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeStruct>>::clear(__context__,__mkStr_rename_at_1037_399);
    das_delete_handle<smart_ptr_raw<MakeStruct>>::clear(__context__,__mkS1_rename_at_1030_393);
    /* end finally */ });
    __mkS1_rename_at_1030_393; das_zero(__mkS1_rename_at_1030_393); das_move(__mkS1_rename_at_1030_393, das_ascend_handle<true,smart_ptr_raw<MakeStruct>>::make(__context__,nullptr,(([&](MakeStruct & __mks_1030) {
    }))));
    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,das_cast<BasicStructureAnnotation *>::cast(__info_rename_at_1028_391->annotation /*annotation*/))),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,112,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_1031_394, char * __cppName_rename_at_1031_395, smart_ptr_raw<TypeDecl> __xtype_rename_at_1031_396, uint32_t __offset_rename_at_1031_397) -> void{
        smart_ptr_raw<MakeFieldDecl> __mkFld_rename_at_1032_398; memset((void*)&__mkFld_rename_at_1032_398,0,sizeof(__mkFld_rename_at_1032_398));
        /* finally */ auto __finally_1031= das_finally([&](){
        das_delete_handle<smart_ptr_raw<MakeFieldDecl>>::clear(__context__,__mkFld_rename_at_1032_398);
        /* end finally */ });
        __mkFld_rename_at_1032_398; das_zero(__mkFld_rename_at_1032_398); das_move(__mkFld_rename_at_1032_398, convert_quote_struct_field_529c713c43cf65dc(__context__,__mod_rename_at_1027_388,__data_rename_at_1027_389,__tstr_rename_at_1028_390,__name_rename_at_1031_394,__offset_rename_at_1031_397,__xtype_rename_at_1031_396,__at_rename_at_1028_392));
        if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__mkFld_rename_at_1032_398),das_auto_cast<void * const >::cast(nullptr)) )
        {
            mks_vector_emplace(das_arg<MakeStruct>::pass(das_deref(__context__,__mkS1_rename_at_1030_393)),__mkFld_rename_at_1032_398);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    __mkStr_rename_at_1037_399; das_zero(__mkStr_rename_at_1037_399); das_move(__mkStr_rename_at_1037_399, das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_1037) {
        das_move((__mks_1037.makeType /*makeType*/),(clone_type(__info_rename_at_1028_391)));
        das_copy((__mks_1037.at /*at*/),(__at_rename_at_1028_392));
    }))));
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeStruct>>>::pass(__mkStr_rename_at_1037_399->structs /*structs*/),__mkS1_rename_at_1030_393);
    __mkStr_rename_at_1037_399->makeStructFlags /*makeStructFlags*/ |= 0x801u;
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprMakeStruct>>::cast(__mkStr_rename_at_1037_399);
}}

inline smart_ptr_raw<Expression> convert_quote_expr_d61fc2e3f77fdf2e ( Context * __context__, Module * const  __mod_rename_at_1043_400, uint8_t const  * __data_rename_at_1043_401, smart_ptr_raw<TypeDecl> const  __info_rename_at_1043_402, LineInfo const  &  __at_rename_at_1043_403 )
{
    smart_ptr_raw<Expression> ___VarsubexprTickatTick1088Tick20_rename_at_1088_405; memset((void*)&___VarsubexprTickatTick1088Tick20_rename_at_1088_405,0,sizeof(___VarsubexprTickatTick1088Tick20_rename_at_1088_405));
    smart_ptr_raw<TypeDecl> ___Varfinal_typeTickatTick1082Tick20_rename_at_1082_406; memset((void*)&___Varfinal_typeTickatTick1082Tick20_rename_at_1082_406,0,sizeof(___Varfinal_typeTickatTick1082Tick20_rename_at_1082_406));
    uint8_t const  * __pdata_rename_at_1077_407; memset((void*)&__pdata_rename_at_1077_407,0,sizeof(__pdata_rename_at_1077_407));
    char * __first_tstr_rename_at_1081_408; memset((void*)&__first_tstr_rename_at_1081_408,0,sizeof(__first_tstr_rename_at_1081_408));
    Annotation * __annotation_rename_at_1084_409; memset((void*)&__annotation_rename_at_1084_409,0,sizeof(__annotation_rename_at_1084_409));
    if ( __data_rename_at_1043_401 == nullptr )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(nullptr);
    } else {
        if ( (__info_rename_at_1043_402->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tHandle) && (__info_rename_at_1043_402->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tPointer) )
        {
            return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_badf850f139f3bb3(__context__,__data_rename_at_1043_401,__info_rename_at_1043_402,__at_rename_at_1043_403));
        } else {
            if ( isYetAnotherVectorTemplate_6471d45428bc715(__context__,__info_rename_at_1043_402) )
            {
                return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(convert_vec_cf3d8478a556d0d6(__context__,__mod_rename_at_1043_400,__data_rename_at_1043_401,__info_rename_at_1043_402,__at_rename_at_1043_403));
            } else {
                char * __tstr_rename_at_1053_404 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef(__context__,__info_rename_at_1043_402,true,true,true)));
                if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1053_404),cast<char *>::from(((char *) "rtti::Annotation")),*__context__,nullptr) )
                {
                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(_Funcast_boostTickget_find_in_module_exprTick15814845442638081433_afbcc9cf766060f5(__context__,((char *) "module_find_annotation"),das_arg<das::string>::pass(das_cast<Annotation>::cast(__data_rename_at_1043_401).module /*_module*/->name /*name*/),das_arg<das::string>::pass(das_cast<Annotation>::cast(__data_rename_at_1043_401).name /*name*/),__at_rename_at_1043_403));
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1053_404),cast<char *>::from(((char *) "rtti::TypeAnnotation")),*__context__,nullptr) )
                {
                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(_Funcast_boostTickget_find_in_module_exprTick15814845442638081433_afbcc9cf766060f5(__context__,((char *) "module_find_type_annotation"),das_arg<das::string>::pass(das_cast<TypeAnnotation>::cast(__data_rename_at_1043_401).module /*_module*/->name /*name*/),das_arg<das::string>::pass(das_cast<TypeAnnotation>::cast(__data_rename_at_1043_401).name /*name*/),__at_rename_at_1043_403));
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1053_404),cast<char *>::from(((char *) "ast::CallMacro")),*__context__,nullptr) )
                {
                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(_Funcast_boostTickget_find_in_module_exprTick15814845442638081433_afbcc9cf766060f5(__context__,((char *) "find_call_macro"),das_arg<das::string>::pass(das_cast<CallMacro *>::cast(__data_rename_at_1043_401)->module /*_module*/->name /*name*/),das_arg<das::string>::pass(das_cast<CallMacro *>::cast(__data_rename_at_1043_401)->name /*name*/),__at_rename_at_1043_403));
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1053_404),cast<char *>::from(((char *) "ast::Structure")),*__context__,nullptr) )
                {
                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(_Funcast_boostTickget_find_in_module_exprTick15814845442638081433_afbcc9cf766060f5(__context__,((char *) "module_find_structure"),das_arg<das::string>::pass(das_cast<Structure *>::cast(__data_rename_at_1043_401)->module /*_module*/->name /*name*/),das_arg<das::string>::pass(das_cast<Structure *>::cast(__data_rename_at_1043_401)->name /*name*/),__at_rename_at_1043_403));
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1053_404),cast<char *>::from(((char *) "ast::Function")),*__context__,nullptr) )
                {
                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(_Funcast_boostTickget_find_in_module_exprTick15814845442638081433_afbcc9cf766060f5(__context__,((char *) "find_unique_function_ptr"),das_arg<das::string>::pass(das_cast<Function *>::cast(__data_rename_at_1043_401)->module /*_module*/->name /*name*/),das_arg<das::string>::pass(das_cast<Function *>::cast(__data_rename_at_1043_401)->name /*name*/),__at_rename_at_1043_403));
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1053_404),cast<char *>::from(((char *) "rtti::Module")),*__context__,nullptr) )
                {
                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(_Funcast_boostTickfind_module_exprTick15293324431947437245_e34e570ad6f2dd1(__context__,das_arg<das::string>::pass(das_cast<Module *>::cast(__data_rename_at_1043_401)->name /*name*/),__at_rename_at_1043_403));
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1053_404),cast<char *>::from(((char *) "$::das_string")),*__context__,nullptr) )
                {
                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(_Funcast_boostTickcreate_stringTick16871435846852833435_ac46da0203621c82(__context__,das_arg<das::string>::pass(das_deref(__context__,das_cast<das::string *>::cast(__data_rename_at_1043_401)))));
                } else if ( __info_rename_at_1043_402->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
                {
                    /* finally */ auto __finally_1076= das_finally([&](){
                    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,___VarsubexprTickatTick1088Tick20_rename_at_1088_405);
                    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,___Varfinal_typeTickatTick1082Tick20_rename_at_1082_406);
                    /* end finally */ });
                    das_zero(___VarsubexprTickatTick1088Tick20_rename_at_1088_405);
                    das_zero(___Varfinal_typeTickatTick1082Tick20_rename_at_1082_406);
                    __pdata_rename_at_1077_407 = das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_1043_401));
                    if ( __pdata_rename_at_1077_407 == nullptr )
                    {
                        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(nullptr);
                    } else {
                        __first_tstr_rename_at_1081_408 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef(__context__,__info_rename_at_1043_402->firstType /*firstType*/,true,true,true)));
                        das_move(___Varfinal_typeTickatTick1082Tick20_rename_at_1082_406,_FuncbuiltinTickcloneTick9602082126133410484_ff5eacb201903461(__context__,__info_rename_at_1043_402->firstType /*firstType*/));
                        if ( SimPolicy<char *>::Equ(cast<char *>::from(__first_tstr_rename_at_1081_408),cast<char *>::from(((char *) "ast::Expression")),*__context__,nullptr) )
                        {
                            __annotation_rename_at_1084_409 = _FuncbuiltinTickget_ptrTick5807679485210906136_15ca7add977814e6(__context__,module_find_annotation(das_reinterpret<Module const  * const >::pass(__mod_rename_at_1043_400),das_cast<smart_ptr_raw<Expression>>::cast(__pdata_rename_at_1077_407)->__rtti /*__rtti*/));
                            builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(___Varfinal_typeTickatTick1082Tick20_rename_at_1082_406),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1085) {
                                das_copy((__mks_1085.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tHandle));
                                das_copy((__mks_1085.annotation /*annotation*/),(das_cast<TypeAnnotation *>::cast(__annotation_rename_at_1084_409)));
                            })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        };
                        das_move(___VarsubexprTickatTick1088Tick20_rename_at_1088_405,convert_quote_expr_d61fc2e3f77fdf2e(__context__,__mod_rename_at_1043_400,__pdata_rename_at_1077_407,___Varfinal_typeTickatTick1082Tick20_rename_at_1082_406,__at_rename_at_1043_403));
                        {
                            bool __need_loop_1092 = true;
                            // t: string const&
                            das_iterator<TArray<char *> const > __t_iterator(das_global<TArray<char *>,0x764f734b280c42ca>(__context__) /*managed_types*/);
                            char * const  * __t_rename_at_1092_410;
                            __need_loop_1092 = __t_iterator.first(__context__,(__t_rename_at_1092_410)) && __need_loop_1092;
                            for ( ; __need_loop_1092 ; __need_loop_1092 = __t_iterator.next(__context__,(__t_rename_at_1092_410)) )
                            {
                                if ( SimPolicy<char *>::Equ(cast<char *>::from((*__t_rename_at_1092_410)),cast<char *>::from(__first_tstr_rename_at_1081_408),*__context__,nullptr) )
                                {
                                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(___VarsubexprTickatTick1088Tick20_rename_at_1088_405);
                                };
                            }
                            __t_iterator.close(__context__,(__t_rename_at_1092_410));
                        };
                        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprAscend>>::make(__context__,nullptr,(([&](ExprAscend & __mks_1097) {
                            das_move((__mks_1097.subexpr /*subexpr*/),(___VarsubexprTickatTick1088Tick20_rename_at_1088_405));
                            das_copy((__mks_1097.at /*at*/),(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(___VarsubexprTickatTick1088Tick20_rename_at_1088_405),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast_ref<LineInfo const  &>::cast(___VarsubexprTickatTick1088Tick20_rename_at_1088_405->at /*at*/) : das_auto_cast_ref<LineInfo const  &>::cast(__at_rename_at_1043_403))));
                        }))));
                    };
                };
                if ( (__info_rename_at_1043_402->annotation /*annotation*/ != nullptr) && ((das_deref(__context__,__info_rename_at_1043_402->annotation /*annotation*/)).rtti_isBasicStructureAnnotation()) )
                {
                    LineInfo __field_loc_rename_at_1100_411 = __at_rename_at_1043_403;
                    if ( isExpression_991e7b9aabc55e05(__context__,__info_rename_at_1043_402,true) )
                    {
                        Expression * __fval_rename_at_1103_412 = ((Expression *)das_cast<Expression *>::cast(__data_rename_at_1043_401));
                        das_copy(__field_loc_rename_at_1100_411,__fval_rename_at_1103_412->at /*at*/);
                    };
                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(convert_quote_structure_7513fe9a14da0cec(__context__,__mod_rename_at_1043_400,__data_rename_at_1043_401,__tstr_rename_at_1053_404,__info_rename_at_1043_402,das_arg<LineInfo>::pass(__field_loc_rename_at_1100_411)));
                } else {
                    builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_15, cast<char * const >::from(((char *) "Unexpected type ")), cast<char * const >::from(__tstr_rename_at_1053_404), cast<char * const >::from(((char *) "!")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(nullptr);
                };
            };
        };
    };
}

inline smart_ptr_raw<ExprAscend> convert_quote_to_expression_214bfe21da346d3e ( Context * __context__, smart_ptr_raw<Expression> __arg_expr_rename_at_1113_413, LineInfo const  &  __at_rename_at_1113_414 )
{
    Module * __mod_rename_at_1114_415; memset((void*)&__mod_rename_at_1114_415,0,sizeof(__mod_rename_at_1114_415));
    smart_ptr_raw<TypeDecl> __tp_rename_at_1116_416; memset((void*)&__tp_rename_at_1116_416,0,sizeof(__tp_rename_at_1116_416));
    /* finally */ auto __finally_1113= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__tp_rename_at_1116_416);
    /* end finally */ });
    __mod_rename_at_1114_415 = ((Module *)rtti_get_builtin_module(((char *) "ast")));
    __tp_rename_at_1116_416; das_zero(__tp_rename_at_1116_416); das_move(__tp_rename_at_1116_416, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1116) {
        das_copy((__mks_1116.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tHandle));
        das_copy((__mks_1116.annotation /*annotation*/),(das_cast<TypeAnnotation *>::cast(module_find_annotation(das_reinterpret<Module const  * const >::pass(__mod_rename_at_1114_415),__arg_expr_rename_at_1113_413->__rtti /*__rtti*/))));
    }))));
    __tp_rename_at_1116_416->flags /*flags*/ |= 0x400u;
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprAscend>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprAscend>>::make(__context__,nullptr,(([&](ExprAscend & __mks_1118) {
        das_move((__mks_1118.subexpr /*subexpr*/),(convert_quote_expr_d61fc2e3f77fdf2e(__context__,__mod_rename_at_1114_415,das_cast<uint8_t const  *>::cast(_FuncbuiltinTickget_ptrTick5807679485210906136_5f1c776d88550ffa(__context__,__arg_expr_rename_at_1113_413)),__tp_rename_at_1116_416,__at_rename_at_1113_414)));
    }))));
}

inline Annotation const  * find_annotation_3750ee9a7cdedb92 ( Context * __context__, char * const  __mod_name_rename_at_1137_417, char * const  __ann_name_rename_at_1137_418 ) { das_stack_prologue __prologue(__context__,128,"find_annotation " DAS_FILE_LINE);
{
    Module * __mod_rename_at_1138_419 = _FuncastTickfind_compiling_moduleTick4523452840392654583_2ad37c2da999dae1(__context__,__mod_name_rename_at_1137_417);
    if ( __mod_rename_at_1138_419 == nullptr )
    {
        return das_auto_cast<Annotation const  *>::cast(nullptr);
    } else {
        Annotation const  * __ann_rename_at_1142_420 = 0;
        rtti_builtin_module_for_each_annotation(__mod_rename_at_1138_419,das_make_block<void,Annotation const  &>(__context__,112,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_1143_421) -> void{
            if ( eq_dstr_str(__value_rename_at_1143_421.name /*name*/,__ann_name_rename_at_1137_418) )
            {
                das_copy(__ann_rename_at_1142_420,das_ref(__context__,__value_rename_at_1143_421));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<Annotation const  *>::cast(__ann_rename_at_1142_420);
    };
}}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_7ea25e1c3613ccaa ( Context * __context__, char * const  __mod_name_rename_at_1153_422, char * const  __ann_name_rename_at_1153_423, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_1153_424 )
{
    Annotation const  * __ann_rename_at_1154_425; memset((void*)&__ann_rename_at_1154_425,0,sizeof(__ann_rename_at_1154_425));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1156_426; memset((void*)&__decl_rename_at_1156_426,0,sizeof(__decl_rename_at_1156_426));
    char * * __argName_rename_at_1159_428; memset((void*)&__argName_rename_at_1159_428,0,sizeof(__argName_rename_at_1159_428));
    AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> * __arg_rename_at_1160_429; memset((void*)&__arg_rename_at_1160_429,0,sizeof(__arg_rename_at_1160_429));
    /* finally */ auto __finally_1153= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1156_426);
    /* end finally */ });
    __ann_rename_at_1154_425 = find_annotation_3750ee9a7cdedb92(__context__,__mod_name_rename_at_1153_422,__ann_name_rename_at_1153_423);
    DAS_ASSERT((__ann_rename_at_1154_425 != nullptr));
    __decl_rename_at_1156_426; das_zero(__decl_rename_at_1156_426); das_move(__decl_rename_at_1156_426, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_d857f1a129cf603a(__context__,__decl_rename_at_1156_426->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_1154_425));
    {
        bool __need_loop_1158 = true;
        // argP: tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any> aka RttiValue> const&
        das_iterator<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const > __argP_iterator(__args_rename_at_1153_424);
        AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>> const  * __argP_rename_at_1158_427;
        __need_loop_1158 = __argP_iterator.first(__context__,(__argP_rename_at_1158_427)) && __need_loop_1158;
        for ( ; __need_loop_1158 ; __need_loop_1158 = __argP_iterator.next(__context__,(__argP_rename_at_1158_427)) )
        {
            __argName_rename_at_1159_428 = ((char * *)&((char * &)(das_get_auto_tuple_field<char *,0,char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::get((*__argP_rename_at_1158_427)))));
            __arg_rename_at_1160_429 = ((AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *)&(das_get_auto_tuple_field<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,1,char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::get((*__argP_rename_at_1158_427))));
            if ( das_get_auto_variant_field<bool,0,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_1160_429)) )
            {
                add_annotation_argument_b1f0a75a869f9d8e(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_1156_426->arguments /*arguments*/),(*__argName_rename_at_1159_428),das_get_auto_variant_field<bool,0,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_1160_429),__context__));
            } else if ( das_get_auto_variant_field<int32_t,1,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_1160_429)) )
            {
                add_annotation_argument_1cccdd04ffc213ae(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_1156_426->arguments /*arguments*/),(*__argName_rename_at_1159_428),das_get_auto_variant_field<int32_t,1,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_1160_429),__context__));
            } else if ( das_get_auto_variant_field<float,5,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_1160_429)) )
            {
                add_annotation_argument_c587cb52cc713868(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_1156_426->arguments /*arguments*/),(*__argName_rename_at_1159_428),das_get_auto_variant_field<float,5,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_1160_429),__context__));
            } else if ( das_get_auto_variant_field<char *,7,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_1160_429)) )
            {
                add_annotation_argument_64b4b11cebf2a3a4(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_1156_426->arguments /*arguments*/),(*__argName_rename_at_1159_428),das_get_auto_variant_field<char *,7,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_1160_429),__context__));
            } else {
                builtin_throw(((char *) "Invalid rtti value"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __argP_iterator.close(__context__,(__argP_rename_at_1158_427));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_1156_426);
}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_5ce31aec0dc3e827 ( Context * __context__, char * const  __mod_name_rename_at_1176_430, char * const  __ann_name_rename_at_1176_431 )
{
    Annotation const  * __ann_rename_at_1177_432; memset((void*)&__ann_rename_at_1177_432,0,sizeof(__ann_rename_at_1177_432));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1179_433; memset((void*)&__decl_rename_at_1179_433,0,sizeof(__decl_rename_at_1179_433));
    /* finally */ auto __finally_1176= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1179_433);
    /* end finally */ });
    __ann_rename_at_1177_432 = find_annotation_3750ee9a7cdedb92(__context__,__mod_name_rename_at_1176_430,__ann_name_rename_at_1176_431);
    DAS_ASSERT((__ann_rename_at_1177_432 != nullptr));
    __decl_rename_at_1179_433; das_zero(__decl_rename_at_1179_433); das_move(__decl_rename_at_1179_433, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_d857f1a129cf603a(__context__,__decl_rename_at_1179_433->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_1177_432));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_1179_433);
}

inline void append_annotation_77e3691cc75925e6 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_1184_434, char * const  __mod_name_rename_at_1184_435, char * const  __ann_name_rename_at_1184_436 )
{
    Annotation const  * __ann_rename_at_1185_437; memset((void*)&__ann_rename_at_1185_437,0,sizeof(__ann_rename_at_1185_437));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1187_438; memset((void*)&__decl_rename_at_1187_438,0,sizeof(__decl_rename_at_1187_438));
    /* finally */ auto __finally_1184= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1187_438);
    /* end finally */ });
    __ann_rename_at_1185_437 = find_annotation_3750ee9a7cdedb92(__context__,__mod_name_rename_at_1184_435,__ann_name_rename_at_1184_436);
    DAS_ASSERT((__ann_rename_at_1185_437 != nullptr));
    __decl_rename_at_1187_438; das_zero(__decl_rename_at_1187_438); das_move(__decl_rename_at_1187_438, append_annotation_5ce31aec0dc3e827(__context__,__mod_name_rename_at_1184_435,__ann_name_rename_at_1184_436));
    addAndApplyFunctionAnnotation(__func_rename_at_1184_434,__decl_rename_at_1187_438,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_2795d5527c6b541c ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_1191_439, char * const  __mod_name_rename_at_1191_440, char * const  __ann_name_rename_at_1191_441 )
{
    Annotation const  * __ann_rename_at_1192_442; memset((void*)&__ann_rename_at_1192_442,0,sizeof(__ann_rename_at_1192_442));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1194_443; memset((void*)&__decl_rename_at_1194_443,0,sizeof(__decl_rename_at_1194_443));
    /* finally */ auto __finally_1191= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1194_443);
    /* end finally */ });
    __ann_rename_at_1192_442 = find_annotation_3750ee9a7cdedb92(__context__,__mod_name_rename_at_1191_440,__ann_name_rename_at_1191_441);
    DAS_ASSERT((__ann_rename_at_1192_442 != nullptr));
    __decl_rename_at_1194_443; das_zero(__decl_rename_at_1194_443); das_move(__decl_rename_at_1194_443, append_annotation_5ce31aec0dc3e827(__context__,__mod_name_rename_at_1191_440,__ann_name_rename_at_1191_441));
    addAndApplyBlockAnnotation(__blk_rename_at_1191_439,__decl_rename_at_1194_443,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_6137d9bcdc59acfc ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_1198_444, char * const  __mod_name_rename_at_1198_445, char * const  __ann_name_rename_at_1198_446 )
{
    Annotation const  * __ann_rename_at_1199_447; memset((void*)&__ann_rename_at_1199_447,0,sizeof(__ann_rename_at_1199_447));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1201_448; memset((void*)&__decl_rename_at_1201_448,0,sizeof(__decl_rename_at_1201_448));
    /* finally */ auto __finally_1198= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1201_448);
    /* end finally */ });
    __ann_rename_at_1199_447 = find_annotation_3750ee9a7cdedb92(__context__,__mod_name_rename_at_1198_445,__ann_name_rename_at_1198_446);
    DAS_ASSERT((__ann_rename_at_1199_447 != nullptr));
    __decl_rename_at_1201_448; das_zero(__decl_rename_at_1201_448); das_move(__decl_rename_at_1201_448, append_annotation_5ce31aec0dc3e827(__context__,__mod_name_rename_at_1198_445,__ann_name_rename_at_1198_446));
    addAndApplyStructAnnotation(__st_rename_at_1198_444,__decl_rename_at_1201_448,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_a7f0a49b54847e5b ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_1205_449, char * const  __mod_name_rename_at_1205_450, char * const  __ann_name_rename_at_1205_451, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_1205_452 )
{
    Annotation const  * __ann_rename_at_1206_453; memset((void*)&__ann_rename_at_1206_453,0,sizeof(__ann_rename_at_1206_453));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1208_454; memset((void*)&__decl_rename_at_1208_454,0,sizeof(__decl_rename_at_1208_454));
    /* finally */ auto __finally_1205= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1208_454);
    /* end finally */ });
    __ann_rename_at_1206_453 = find_annotation_3750ee9a7cdedb92(__context__,__mod_name_rename_at_1205_450,__ann_name_rename_at_1205_451);
    DAS_ASSERT((__ann_rename_at_1206_453 != nullptr));
    __decl_rename_at_1208_454; das_zero(__decl_rename_at_1208_454); das_move(__decl_rename_at_1208_454, append_annotation_7ea25e1c3613ccaa(__context__,__mod_name_rename_at_1205_450,__ann_name_rename_at_1205_451,__args_rename_at_1205_452));
    addAndApplyFunctionAnnotation(__func_rename_at_1205_449,__decl_rename_at_1208_454,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_c867e1d1bd1e709d ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_1212_455, char * const  __mod_name_rename_at_1212_456, char * const  __ann_name_rename_at_1212_457, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_1212_458 )
{
    Annotation const  * __ann_rename_at_1213_459; memset((void*)&__ann_rename_at_1213_459,0,sizeof(__ann_rename_at_1213_459));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1215_460; memset((void*)&__decl_rename_at_1215_460,0,sizeof(__decl_rename_at_1215_460));
    /* finally */ auto __finally_1212= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1215_460);
    /* end finally */ });
    __ann_rename_at_1213_459 = find_annotation_3750ee9a7cdedb92(__context__,__mod_name_rename_at_1212_456,__ann_name_rename_at_1212_457);
    DAS_ASSERT((__ann_rename_at_1213_459 != nullptr));
    __decl_rename_at_1215_460; das_zero(__decl_rename_at_1215_460); das_move(__decl_rename_at_1215_460, append_annotation_7ea25e1c3613ccaa(__context__,__mod_name_rename_at_1212_456,__ann_name_rename_at_1212_457,__args_rename_at_1212_458));
    addAndApplyBlockAnnotation(__blk_rename_at_1212_455,__decl_rename_at_1215_460,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_a0a2cf8e458c7814 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_1219_461, char * const  __mod_name_rename_at_1219_462, char * const  __ann_name_rename_at_1219_463, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_1219_464 )
{
    Annotation const  * __ann_rename_at_1220_465; memset((void*)&__ann_rename_at_1220_465,0,sizeof(__ann_rename_at_1220_465));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1222_466; memset((void*)&__decl_rename_at_1222_466,0,sizeof(__decl_rename_at_1222_466));
    /* finally */ auto __finally_1219= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1222_466);
    /* end finally */ });
    __ann_rename_at_1220_465 = find_annotation_3750ee9a7cdedb92(__context__,__mod_name_rename_at_1219_462,__ann_name_rename_at_1219_463);
    DAS_ASSERT((__ann_rename_at_1220_465 != nullptr));
    __decl_rename_at_1222_466; das_zero(__decl_rename_at_1222_466); das_move(__decl_rename_at_1222_466, append_annotation_7ea25e1c3613ccaa(__context__,__mod_name_rename_at_1219_462,__ann_name_rename_at_1219_463,__args_rename_at_1219_464));
    addAndApplyStructAnnotation(__st_rename_at_1219_461,__decl_rename_at_1222_466,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t add_annotation_argument_b1f0a75a869f9d8e ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1226_467, char * const  __argName_rename_at_1226_468, bool __val_rename_at_1226_469 )
{
    int32_t __argIdx_rename_at_1227_470 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1226_467),__argName_rename_at_1226_468));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1226_467,__argIdx_rename_at_1227_470,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tBool);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1226_467,__argIdx_rename_at_1227_470,__context__).bValue /*bValue*/,__val_rename_at_1226_469);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1227_470);
}

inline int32_t add_annotation_argument_1cccdd04ffc213ae ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1233_471, char * const  __argName_rename_at_1233_472, int32_t __val_rename_at_1233_473 )
{
    int32_t __argIdx_rename_at_1234_474 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1233_471),__argName_rename_at_1233_472));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1233_471,__argIdx_rename_at_1234_474,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tInt);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1233_471,__argIdx_rename_at_1234_474,__context__).iValue /*iValue*/,__val_rename_at_1233_473);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1234_474);
}

inline int32_t add_annotation_argument_c587cb52cc713868 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1240_475, char * const  __argName_rename_at_1240_476, float __val_rename_at_1240_477 )
{
    int32_t __argIdx_rename_at_1241_478 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1240_475),__argName_rename_at_1240_476));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1240_475,__argIdx_rename_at_1241_478,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tFloat);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1240_475,__argIdx_rename_at_1241_478,__context__).fValue /*fValue*/,__val_rename_at_1240_477);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1241_478);
}

inline int32_t add_annotation_argument_64b4b11cebf2a3a4 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1247_479, char * const  __argName_rename_at_1247_480, char * const  __val_rename_at_1247_481 )
{
    int32_t __argIdx_rename_at_1248_482 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1247_479),__argName_rename_at_1247_480));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1247_479,__argIdx_rename_at_1248_482,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tString);
    set_das_string(das_arg<das::string>::pass(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1247_479,__argIdx_rename_at_1248_482,__context__).sValue /*sValue*/),__val_rename_at_1247_481);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1248_482);
}

inline int32_t add_annotation_argument_8f7d2e8ac5cc9c46 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1254_483, AnnotationArgument const  &  __ann_rename_at_1254_484 )
{
    int32_t __argIdx_rename_at_1255_485 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1254_483),((char * const )(to_das_string(__ann_rename_at_1254_484.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1254_483,__argIdx_rename_at_1255_485,__context__).type /*basicType*/,__ann_rename_at_1254_484.type /*basicType*/);
    if ( __ann_rename_at_1254_484.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1254_483,__argIdx_rename_at_1255_485,__context__).bValue /*bValue*/,__ann_rename_at_1254_484.bValue /*bValue*/);
    } else if ( __ann_rename_at_1254_484.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1254_483,__argIdx_rename_at_1255_485,__context__).iValue /*iValue*/,__ann_rename_at_1254_484.iValue /*iValue*/);
    } else if ( __ann_rename_at_1254_484.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1254_483,__argIdx_rename_at_1255_485,__context__).fValue /*fValue*/,__ann_rename_at_1254_484.fValue /*fValue*/);
    } else if ( __ann_rename_at_1254_484.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        das_clone<das::string,das::string>::clone(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1254_483,__argIdx_rename_at_1255_485,__context__).sValue /*sValue*/,__ann_rename_at_1254_484.sValue /*sValue*/);
    } else {
        DAS_ASSERTF((false),(((char *) "unsupported annotation type, add more types")));
    };
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1255_485);
}

inline int32_t get_for_source_index_4960346f545ad8a5 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1271_486, smart_ptr_raw<Variable> const  __svar_rename_at_1271_487 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1272_41_10; _temp_make_local_1272_41_10;
    {
        bool __need_loop_1272 = true;
        // v: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __v_iterator(__expr_rename_at_1271_486->iteratorVariables /*iteratorVariables*/);
        smart_ptr_raw<Variable> const  * __v_rename_at_1272_490;
        __need_loop_1272 = __v_iterator.first(__context__,(__v_rename_at_1272_490)) && __need_loop_1272;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1272_41_10 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_1272_491;
        __need_loop_1272 = __t_iterator.first(__context__,(__t_rename_at_1272_491)) && __need_loop_1272;
        for ( ; __need_loop_1272 ; __need_loop_1272 = __v_iterator.next(__context__,(__v_rename_at_1272_490)) && __t_iterator.next(__context__,(__t_rename_at_1272_491)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__v_rename_at_1272_490)),das_auto_cast<smart_ptr_raw<void> const >::cast(__svar_rename_at_1271_487)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_1272_491);
            };
        }
        __v_iterator.close(__context__,(__v_rename_at_1272_490));
        __t_iterator.close(__context__,(__t_rename_at_1272_491));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline int32_t get_for_source_index_e610aeac2c72af06 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1280_492, smart_ptr_raw<Expression> const  __source_rename_at_1280_493 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1281_31_11; _temp_make_local_1281_31_11;
    {
        bool __need_loop_1281 = true;
        // s: smart_ptr<ast::Expression> const&
        das_iterator<das::vector<smart_ptr<Expression>> const > __s_iterator(__expr_rename_at_1280_492->sources /*sources*/);
        smart_ptr_raw<Expression> const  * __s_rename_at_1281_496;
        __need_loop_1281 = __s_iterator.first(__context__,(__s_rename_at_1281_496)) && __need_loop_1281;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1281_31_11 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_1281_497;
        __need_loop_1281 = __t_iterator.first(__context__,(__t_rename_at_1281_497)) && __need_loop_1281;
        for ( ; __need_loop_1281 ; __need_loop_1281 = __s_iterator.next(__context__,(__s_rename_at_1281_496)) && __t_iterator.next(__context__,(__t_rename_at_1281_497)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__s_rename_at_1281_496)),das_auto_cast<smart_ptr_raw<void> const >::cast(__source_rename_at_1280_493)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_1281_497);
            };
        }
        __s_iterator.close(__context__,(__s_rename_at_1281_496));
        __t_iterator.close(__context__,(__t_rename_at_1281_497));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline smart_ptr_raw<TypeDecl> function_to_type_3ce465c9c4c12ac9 ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_1328_498 )
{
    smart_ptr_raw<TypeDecl> __td_rename_at_1329_499; memset((void*)&__td_rename_at_1329_499,0,sizeof(__td_rename_at_1329_499));
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1333_36_12; _temp_make_local_1333_36_12;
    /* finally */ auto __finally_1328= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__td_rename_at_1329_499);
    /* end finally */ });
    __td_rename_at_1329_499; das_zero(__td_rename_at_1329_499); das_move(__td_rename_at_1329_499, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1329) {
        das_copy((__mks_1329.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tFunction));
        das_move((__mks_1329.firstType /*firstType*/),(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_1328_498->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<smart_ptr_raw<TypeDecl>>::cast(clone_type(__fn_rename_at_1328_498->result /*result*/)) : das_auto_cast<smart_ptr_raw<TypeDecl>>::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1330) {
            das_copy((__mks_1330.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
        })))))));
    }))));
    das_vector_resize(das_arg<das::vector<das::string>>::pass(__td_rename_at_1329_499->argNames /*argNames*/),das_vector_length(__fn_rename_at_1328_498->arguments /*arguments*/));
    {
        bool __need_loop_1333 = true;
        // arg: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __arg_iterator(__fn_rename_at_1328_498->arguments /*arguments*/);
        smart_ptr_raw<Variable> const  * __arg_rename_at_1333_502;
        __need_loop_1333 = __arg_iterator.first(__context__,(__arg_rename_at_1333_502)) && __need_loop_1333;
        // argi: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1333_36_12 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __argi_iterator(0,1);
        int32_t __argi_rename_at_1333_503;
        __need_loop_1333 = __argi_iterator.first(__context__,(__argi_rename_at_1333_503)) && __need_loop_1333;
        for ( ; __need_loop_1333 ; __need_loop_1333 = __arg_iterator.next(__context__,(__arg_rename_at_1333_502)) && __argi_iterator.next(__context__,(__argi_rename_at_1333_503)) )
        {
            emplace_new_e80d981a19168757(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__td_rename_at_1329_499->argTypes /*argTypes*/),clone_type((*__arg_rename_at_1333_502)->type /*_type*/));
            das_clone<das::string,das::string>::clone(das_index<das::vector<das::string>>::at(__td_rename_at_1329_499->argNames /*argNames*/,__argi_rename_at_1333_503,__context__),(*__arg_rename_at_1333_502)->name /*name*/);
        }
        __arg_iterator.close(__context__,(__arg_rename_at_1333_502));
        __argi_iterator.close(__context__,(__argi_rename_at_1333_503));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(__td_rename_at_1329_499);
}

inline void visit_finally_eebe169335929629 ( Context * __context__, ExprBlock * const  __blk_rename_at_1340_504, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1340_505 )
{
    astVisitBlockFinally(das_cast<smart_ptr_raw<ExprBlock>>::cast(__blk_rename_at_1340_504),__adapter_rename_at_1340_505,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool isCMRES_18c2571faf70a24 ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1346_506 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1346_506->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1346_506->flags /*flags*/,1u << 6));
}

inline bool isCMRES_2379351cb339af97 ( Context * __context__, Function * const  __fun_rename_at_1350_507 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1350_507->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1350_507->flags /*flags*/,1u << 6));
}

inline bool isMakeLocal_5ae2b5812df61b98 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1354_508 )
{
    return das_auto_cast<bool>::cast(((((SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1354_508->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeLocal")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1354_508->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeStruct")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1354_508->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeVariant")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1354_508->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeArray")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1354_508->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeTuple")),*__context__,nullptr)));
}

inline bool isExprCallFunc_905b0dfa902dbccd ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1362_509 )
{
    return das_auto_cast<bool>::cast((((SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1362_509->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCallFunc")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1362_509->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprOp")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1362_509->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprNew")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1362_509->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCall")),*__context__,nullptr)));
}

inline TDim<DAS_COMMENT(bound_enum) das::Type,31> get_workhorse_types_f596638550511fde ( Context * __context__ )
{
    return das_auto_cast_ref<TDim<DAS_COMMENT(bound_enum) das::Type,31>>::cast((([&]() -> TDim<DAS_COMMENT(bound_enum) das::Type,31> {
        TDim<DAS_COMMENT(bound_enum) das::Type,31> __mka_1370;
        __mka_1370(0,__context__) = DAS_COMMENT(bound_enum) das::Type::tBool;
        __mka_1370(1,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt8;
        __mka_1370(2,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt8;
        __mka_1370(3,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt16;
        __mka_1370(4,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt16;
        __mka_1370(5,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt64;
        __mka_1370(6,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt64;
        __mka_1370(7,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration;
        __mka_1370(8,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration8;
        __mka_1370(9,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration16;
        __mka_1370(10,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration64;
        __mka_1370(11,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt;
        __mka_1370(12,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt2;
        __mka_1370(13,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt3;
        __mka_1370(14,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt4;
        __mka_1370(15,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt;
        __mka_1370(16,__context__) = DAS_COMMENT(bound_enum) das::Type::tBitfield;
        __mka_1370(17,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt2;
        __mka_1370(18,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt3;
        __mka_1370(19,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt4;
        __mka_1370(20,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat;
        __mka_1370(21,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat2;
        __mka_1370(22,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat3;
        __mka_1370(23,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat4;
        __mka_1370(24,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange;
        __mka_1370(25,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange;
        __mka_1370(26,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange64;
        __mka_1370(27,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange64;
        __mka_1370(28,__context__) = DAS_COMMENT(bound_enum) das::Type::tString;
        __mka_1370(29,__context__) = DAS_COMMENT(bound_enum) das::Type::tDouble;
        __mka_1370(30,__context__) = DAS_COMMENT(bound_enum) das::Type::tPointer;
        return __mka_1370;
    })()));
}

inline int32_t find_argument_index_ae234aada3e459c5 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1382_510, char * const  __name_rename_at_1382_511 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1383_32_13; _temp_make_local_1383_32_13;
    {
        bool __need_loop_1383 = true;
        // a: $::das_string const&
        das_iterator<das::vector<das::string> const > __a_iterator(__typ_rename_at_1382_510->argNames /*argNames*/);
        das::string const  * __a_rename_at_1383_514;
        __need_loop_1383 = __a_iterator.first(__context__,(__a_rename_at_1383_514)) && __need_loop_1383;
        // ai: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1383_32_13 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __ai_iterator(0,1);
        int32_t __ai_rename_at_1383_515;
        __need_loop_1383 = __ai_iterator.first(__context__,(__ai_rename_at_1383_515)) && __need_loop_1383;
        for ( ; __need_loop_1383 ; __need_loop_1383 = __a_iterator.next(__context__,(__a_rename_at_1383_514)) && __ai_iterator.next(__context__,(__ai_rename_at_1383_515)) )
        {
            if ( eq_dstr_str((*__a_rename_at_1383_514),__name_rename_at_1382_511) )
            {
                return das_auto_cast<int32_t>::cast(__ai_rename_at_1383_515);
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_1383_514));
        __ai_iterator.close(__context__,(__ai_rename_at_1383_515));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline bool isCMRESType_781422633ef81b3 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1391_516 )
{
    return das_auto_cast<bool>::cast(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__blockT_rename_at_1391_516),das_auto_cast<void * const >::cast(nullptr))) && ((das_deref(__context__,__blockT_rename_at_1391_516)).isRefType())) && !(das_get_bitfield(__blockT_rename_at_1391_516->flags /*flags*/,1u << 0)));
}

inline void debug_expression_impl_eaefe21084abd004 ( Context * __context__, StringBuilderWriter &  __writer_rename_at_1399_517, smart_ptr_raw<Expression> const  __expr_rename_at_1399_518, Bitfield __deFlags_rename_at_1399_519, int32_t __tabs_rename_at_1399_520 ) { das_stack_prologue __prologue(__context__,528,"debug_expression_impl " DAS_FILE_LINE);
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1399_518),das_auto_cast<void * const >::cast(nullptr)) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_16,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) "null"))));
        return ;
    } else {
        if ( builtin_empty(__expr_rename_at_1399_518->__rtti /*__rtti*/) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) "[NO RTTI]"))));
            return ;
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_19, cast<char * const >::from(((char *) "(")), cast<char * const >::from(__expr_rename_at_1399_518->__rtti /*__rtti*/))))));
            if ( das_get_bitfield(__deFlags_rename_at_1399_519,1u << 0) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_20,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_21, cast<char * const >::from(((char *) "<")), cast<uint32_t>::from(builtin_smart_ptr_use_count(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1399_518),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))), cast<char * const >::from(((char *) ">")))))));
            };
            if ( builtin_string_startswith(((char * const )(pass_string(__expr_rename_at_1399_518->__rtti /*__rtti*/))),((char *) "ExprConst"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_22,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_23, cast<char * const >::from(((char *) " ")), cast<char * const >::from(_FuncastTickdescribeTick842554968825501494_79ad6734798edcff(__context__,__expr_rename_at_1399_518)), cast<char * const >::from(((char *) ")")))))));
                return ;
            } else {
                BasicStructureAnnotation * __ann_rename_at_1416_521 = das_cast<BasicStructureAnnotation *>::cast(get_expression_annotation(_FuncbuiltinTickget_ptrTick8468476673553620226_6d4e7dbe2a2e0573(__context__,__expr_rename_at_1399_518),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
                if ( __ann_rename_at_1416_521 == nullptr )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_24,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) ")"))));
                    return ;
                } else {
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1416_521)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,96,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_1421_522, char * __cppName_rename_at_1421_523, smart_ptr_raw<TypeDecl> __xtype_rename_at_1421_524, uint32_t __offset_rename_at_1421_525) -> void{
                        if ( __offset_rename_at_1421_525 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1423_526 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef(__context__,__xtype_rename_at_1421_524,true,true,true)));
                            int8_t * __p8_rename_at_1424_527 = 0;
                            das_copy(__p8_rename_at_1424_527,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1399_518),int32_t(__offset_rename_at_1421_525),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1423_526),cast<char *>::from(((char *) "smart_ptr<ast::Expression>")),*__context__,nullptr) )
                            {
                                smart_ptr_raw<Expression> * __pv_rename_at_1429_528 = ((smart_ptr_raw<Expression> *)das_cast<smart_ptr_raw<Expression> *>::cast(__p8_rename_at_1424_527));
                                char * __ts_rename_at_1430_529 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1399_520 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_25,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_26, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1430_529), cast<char * const >::from(((char *) "(")), cast<char *>::from(__name_rename_at_1421_522), cast<char * const >::from(((char *) " ")))))));
                                debug_expression_impl_eaefe21084abd004(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1399_517),das_deref(__context__,__pv_rename_at_1429_528),__deFlags_rename_at_1399_519,__tabs_rename_at_1399_520 + 2);
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_27,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) ")"))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1423_526),cast<char *>::from(((char *) "$::das_string")),*__context__,nullptr) )
                            {
                                das::string * __pv_rename_at_1435_530 = ((das::string *)das_cast<das::string *>::cast(__p8_rename_at_1424_527));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_28,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_29, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1421_522), cast<char * const >::from(((char *) "=\"")), cast<das::string const  &>::from(das_deref(__context__,__pv_rename_at_1435_530)), cast<char * const >::from(((char *) "\"")))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1423_526),cast<char *>::from(((char *) "int")),*__context__,nullptr) )
                            {
                                int32_t * __pv_rename_at_1438_531 = ((int32_t *)das_cast<int32_t *>::cast(__p8_rename_at_1424_527));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_30,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_31, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1421_522), cast<char * const >::from(((char *) "=")), cast<int32_t>::from(das_deref(__context__,__pv_rename_at_1438_531)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1423_526),cast<char *>::from(((char *) "uint")),*__context__,nullptr) )
                            {
                                uint32_t * __pv_rename_at_1441_532 = ((uint32_t *)das_cast<uint32_t *>::cast(__p8_rename_at_1424_527));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_32,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_33, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1421_522), cast<char * const >::from(((char *) "=")), cast<uint32_t>::from(das_deref(__context__,__pv_rename_at_1441_532)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1423_526),cast<char *>::from(((char *) "int64")),*__context__,nullptr) )
                            {
                                int64_t * __pv_rename_at_1444_533 = ((int64_t *)das_cast<int64_t *>::cast(__p8_rename_at_1424_527));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_34,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_35, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1421_522), cast<char * const >::from(((char *) "=")), cast<int64_t>::from(das_deref(__context__,__pv_rename_at_1444_533)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1423_526),cast<char *>::from(((char *) "uint64")),*__context__,nullptr) )
                            {
                                uint64_t * __pv_rename_at_1447_534 = ((uint64_t *)das_cast<uint64_t *>::cast(__p8_rename_at_1424_527));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_36,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_37, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1421_522), cast<char * const >::from(((char *) "=")), cast<uint64_t>::from(das_deref(__context__,__pv_rename_at_1447_534)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1423_526),cast<char *>::from(((char *) "bool")),*__context__,nullptr) )
                            {
                                bool * __pv_rename_at_1450_535 = ((bool *)das_cast<bool *>::cast(__p8_rename_at_1424_527));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_38,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_39, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1421_522), cast<char * const >::from(((char *) "=")), cast<bool>::from(das_deref(__context__,__pv_rename_at_1450_535)))))));
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1416_521)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,320,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_1460_536, char * __cppName_rename_at_1460_537, smart_ptr_raw<TypeDecl> __xtype_rename_at_1460_538, uint32_t __offset_rename_at_1460_539) -> void{
                        if ( __offset_rename_at_1460_539 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1462_540 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef(__context__,__xtype_rename_at_1460_538,true,true,true)));
                            int8_t * __p8_rename_at_1463_541 = 0;
                            das_copy(__p8_rename_at_1463_541,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1399_518),int32_t(__offset_rename_at_1460_539),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1462_540),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Expression")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Expression>> * __pv_rename_at_1468_542 = ((das::vector<smart_ptr<Expression>> *)das_cast<das::vector<smart_ptr<Expression>> *>::cast(__p8_rename_at_1463_541));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1468_542)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1472_38_14; _temp_make_local_1472_38_14;
                                    char * __ts_rename_at_1470_543 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1399_520 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_40,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_41, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1470_543), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1460_536), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1472 = true;
                                        // l: smart_ptr<ast::Expression> const&
                                        das_iterator<das::vector<smart_ptr<Expression>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1468_542));
                                        smart_ptr_raw<Expression> const  * __l_rename_at_1472_546;
                                        __need_loop_1472 = __l_iterator.first(__context__,(__l_rename_at_1472_546)) && __need_loop_1472;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1472_38_14 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1472_547;
                                        __need_loop_1472 = __i_iterator.first(__context__,(__i_rename_at_1472_547)) && __need_loop_1472;
                                        for ( ; __need_loop_1472 ; __need_loop_1472 = __l_iterator.next(__context__,(__l_rename_at_1472_546)) && __i_iterator.next(__context__,(__i_rename_at_1472_547)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_42,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_43, cast<char * const >::from(__ts_rename_at_1470_543), cast<char * const >::from(((char *) "  ")))))));
                                            debug_expression_impl_eaefe21084abd004(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1399_517),(*__l_rename_at_1472_546),__deFlags_rename_at_1399_519,__tabs_rename_at_1399_520 + 2);
                                            if ( __i_rename_at_1472_547 != (das_vector_length(das_deref(__context__,__pv_rename_at_1468_542)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_44,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __l_iterator.close(__context__,(__l_rename_at_1472_546));
                                        __i_iterator.close(__context__,(__i_rename_at_1472_547));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_45,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) "]"))));
                                };
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1462_540),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Variable")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Variable>> * __pv_rename_at_1482_548 = ((das::vector<smart_ptr<Variable>> *)das_cast<das::vector<smart_ptr<Variable>> *>::cast(__p8_rename_at_1463_541));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1482_548)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1486_38_15; _temp_make_local_1486_38_15;
                                    char * __ts_rename_at_1484_549 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1399_520 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_46,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_47, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1484_549), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1460_536), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1486 = true;
                                        // l: smart_ptr<ast::Variable> const&
                                        das_iterator<das::vector<smart_ptr<Variable>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1482_548));
                                        smart_ptr_raw<Variable> const  * __l_rename_at_1486_552;
                                        __need_loop_1486 = __l_iterator.first(__context__,(__l_rename_at_1486_552)) && __need_loop_1486;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1486_38_15 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1486_553;
                                        __need_loop_1486 = __i_iterator.first(__context__,(__i_rename_at_1486_553)) && __need_loop_1486;
                                        for ( ; __need_loop_1486 ; __need_loop_1486 = __l_iterator.next(__context__,(__l_rename_at_1486_552)) && __i_iterator.next(__context__,(__i_rename_at_1486_553)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_48,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_49, cast<char * const >::from(__ts_rename_at_1484_549), cast<char * const >::from(((char *) "  ")))))));
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_50,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_51, cast<das::string const  &>::from((*__l_rename_at_1486_552)->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef(__context__,(*__l_rename_at_1486_552)->type /*_type*/,true,true,true)))))));
                                            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__l_rename_at_1486_552)->init /*init*/),das_auto_cast<void * const >::cast(nullptr)) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_52,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) " = "))));
                                                debug_expression_impl_eaefe21084abd004(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1399_517),(*__l_rename_at_1486_552)->init /*init*/,__deFlags_rename_at_1399_519,__tabs_rename_at_1399_520 + 2);
                                            };
                                            if ( __i_rename_at_1486_553 != (das_vector_length(das_deref(__context__,__pv_rename_at_1482_548)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_53,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __l_iterator.close(__context__,(__l_rename_at_1486_552));
                                        __i_iterator.close(__context__,(__i_rename_at_1486_553));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_54,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) "]"))));
                                };
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_55,cast<StringBuilderWriter &>::from(__writer_rename_at_1399_517),cast<char * const >::from(((char *) ")"))));
                };
            };
        };
    };
}}

inline char * debug_expression_cdb88f2c378a9017 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1505_554, Bitfield __deFlags_rename_at_1505_555 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1506_556) DAS_AOT_INLINE_LAMBDA -> void{
        debug_expression_impl_eaefe21084abd004(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1506_556),__expr_rename_at_1505_554,__deFlags_rename_at_1505_555,0);
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * debug_expression_803962089c61eb61 ( Context * __context__, Expression * const  __expr_rename_at_1511_557 )
{
    return das_auto_cast<char *>::cast(debug_expression_cdb88f2c378a9017(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1511_557),0x0u));
}

inline char * describe_8f9450a77a61aede ( Context * __context__, Expression * const  __expr_rename_at_1517_558 )
{
    return das_auto_cast<char *>::cast(_FuncastTickdescribeTick842554968825501494_79ad6734798edcff(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1517_558)));
}

inline int32_t getVectorElementCount_dae41b2187c417ca ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1523_559 )
{
    return das_auto_cast<int32_t>::cast((((((((__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tInt2)) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(2) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tFloat3) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) ? das_auto_cast<int32_t>::cast(3) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tFloat4) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1523_559 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) ? das_auto_cast<int32_t>::cast(4) : das_auto_cast<int32_t>::cast(0))))));
}

inline int32_t getVectorElementSize_ba6e81198dcbc886 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1530_560 )
{
    return das_auto_cast<int32_t>::cast(((__bt_rename_at_1530_560 == DAS_COMMENT(bound_enum) das::Type::tRange64) || (__bt_rename_at_1530_560 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(8) : das_auto_cast<int32_t>::cast(4));
}

inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_c96967adbe9325ce ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1535_561 )
{
    return das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tFloat) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tRange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tUInt2) || (__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tURange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tRange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1535_561 == DAS_COMMENT(bound_enum) das::Type::tURange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tVoid))))))))));
}

inline int32_t getVectorOffset_56f905013ae8c372 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1544_562, char * const  __ident_rename_at_1544_563 )
{
    int32_t __ofs_rename_at_1545_564 = -1;
    if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "x")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "X")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "r")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "R")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1545_564,0);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "y")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "Y")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "g")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "G")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1545_564,1);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "z")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "Z")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "b")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "B")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1545_564,2);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "w")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "W")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "a")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1544_563),cast<char *>::from(((char *) "A")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1545_564,3);
    };
    int32_t __count_rename_at_1555_565 = ((int32_t)getVectorElementCount_dae41b2187c417ca(__context__,__bt_rename_at_1544_562));
    return das_auto_cast<int32_t>::cast((__ofs_rename_at_1545_564 >= __count_rename_at_1555_565) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(__ofs_rename_at_1545_564));
}

inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_de6dfc85173d067c ( Context * __context__, TDim<AutoTuple<char *,char *>,16> &  __a_rename_at_1352_566 )
{
    TArray<AutoTuple<char *,char *>> __arr_rename_at_1354_567;das_zero(__arr_rename_at_1354_567);
    _FuncbuiltinTickresizeTick4811697762258667383_bfb1abbb07340073(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__arr_rename_at_1354_567),16);
    das_copy(das_cast<TDim<AutoTuple<char *,char *>,16>>::cast(das_ref(__context__,__arr_rename_at_1354_567(0,__context__))),__a_rename_at_1352_566);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<char *,char *>>>::cast(__arr_rename_at_1354_567);
}

inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_898d9918ac5a3e3e ( Context * __context__, TDim<char *,6> &  __a_rename_at_1352_568 )
{
    TArray<char *> __arr_rename_at_1354_569;das_zero(__arr_rename_at_1354_569);
    _FuncbuiltinTickresizeTick4811697762258667383_e945fcf444587b9b(__context__,das_arg<TArray<char *>>::pass(__arr_rename_at_1354_569),6);
    das_copy(das_cast<TDim<char *,6>>::cast(das_ref(__context__,__arr_rename_at_1354_569(0,__context__))),__a_rename_at_1352_568);
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__arr_rename_at_1354_569);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xb208d874d3540634] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_1e3552d3bc6b6de6>>();
    };
    aotLib[0xd3757bb391153ca4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_5fb0f8e94e78667b>>();
    };
    aotLib[0x8c1856ce3ef7cc68] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_8f6af4333a24497d>>();
    };
    aotLib[0xe7a929fd2201030] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_f2e12ba33c8e5800>>();
    };
    aotLib[0xc1a339f737c86c6c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_dc38b502bf9875cd>>();
    };
    aotLib[0x8619ab4d0b9575fb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_33dd2607299b73e2>>();
    };
    aotLib[0x3d1c1dc0bb7bc5db] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e0f0db26c9bddfb9>>();
    };
    aotLib[0x73f70ed6064d9760] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_25cb48d0685fc01e>>();
    };
    aotLib[0x596123eaf2d19d3d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_a4299e2b282e186f>>();
    };
    aotLib[0x5891c7f1a784d758] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_eacbdf6c5c838463>>();
    };
    aotLib[0x5c58e0fc79bae70f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_7bda8577897747f3>>();
    };
    aotLib[0x59cab87cf6b42375] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_1040c47ee99273dc>>();
    };
    aotLib[0xd378dc4355cdfd61] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_22b4bbfa1962edcb>>();
    };
    aotLib[0xccbeab8fc726ca34] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_318dbb9fe3ad8d26>>();
    };
    aotLib[0x355900046d2d5aa4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_f9e84a1132cd421c>>();
    };
    aotLib[0x6add01d1d7b0c407] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_598afebfc43d1b82>>();
    };
    aotLib[0xaec56ae341dbe52c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_b7283bfd4d6912a>>();
    };
    aotLib[0xb354c07867cb790a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_d424ba6952970d20>>();
    };
    aotLib[0x9b9bd6878e6991ba] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_c64c64f8ce19c4f1>>();
    };
    aotLib[0xd9898023008bc898] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_fd693b0656e58d42>>();
    };
    aotLib[0x57ad3588af10105d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_cc5e722ec5d18084>>();
    };
    aotLib[0xdd8d8c5de2b5b41b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_5ecbb6bf9d3c5ce5>>();
    };
    aotLib[0x85895ff9c585ea91] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_86a7b966da7cc372>>();
    };
    aotLib[0x9a93e0f739fe6c55] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_10296bf9fbc53845>>();
    };
    aotLib[0xbaf9bc89767c8764] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_efc8b692d683dde6>>();
    };
    aotLib[0x304771c009e0d5c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_135df9c4c86bd47a>>();
    };
    aotLib[0xade9b8fbfa30b41c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_d80d89f4e2ee24a>>();
    };
    aotLib[0xa104007e4602c51] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_747b059c62e5428d>>();
    };
    aotLib[0x1209c39c591c7bf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_1f060b3476600b3f>>();
    };
    aotLib[0x8a68a201bf2c5305] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_3c026b31961c9c97>>();
    };
    aotLib[0x6f56cab806904854] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_bfb1abbb07340073>>();
    };
    aotLib[0xc11fb2b79db8c132] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_e945fcf444587b9b>>();
    };
    aotLib[0xa2035e8f0b2fd27d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_be3fb1336b24ed81>>();
    };
    aotLib[0x8b8bc37f05d01ebf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_72481d9703572422>>();
    };
    aotLib[0xe185677853092597] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_6b6c8f819c74b7dd>>();
    };
    aotLib[0xadee77c91e0b5d1f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_574cffbb66f47535>>();
    };
    aotLib[0xa0112d3b29fbf377] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_5767cf0f8c77bc34>>();
    };
    aotLib[0x4db824a3dba58c81] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_c0fff2b1179fc9e2>>();
    };
    aotLib[0xa8de387c961179b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_7f0eb2e30132a5ec>>();
    };
    aotLib[0x36b5dd09d0d464af] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickfind_moduleTick11101329256228773934_f6b4e714927216e3>>();
    };
    aotLib[0x9ed1fdbeaa67f124] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_ff37717a0d8d2276>>();
    };
    aotLib[0x54fb32418e14f83e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick16475640899284277631_7c54c515f2166fd4>>();
    };
    aotLib[0x3c681f0f5ae3970e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_e2397053ec0a37a8>>();
    };
    aotLib[0xcdcf949b50e76990] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpush_cloneTick2035469273396957942_3ddfc60f52ac9066>>();
    };
    aotLib[0x2c32fa7224dc7ec3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_5024a114a1aa8a05>>();
    };
    aotLib[0x4d21b62835e15ffb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_8539bd2d8add7e73>>();
    };
    aotLib[0x9b3fac902795a664] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_f304d102cb128dc5>>();
    };
    aotLib[0x64b5f42a413d30d7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickget_find_in_module_exprTick15814845442638081433_afbcc9cf766060f5>>();
    };
    aotLib[0xfc033f65983d2155] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickfind_module_exprTick15293324431947437245_e34e570ad6f2dd1>>();
    };
    aotLib[0x4b6fc893746888c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_17f8c514eb6e92ef>>();
    };
    aotLib[0xa553c252dcf20b1a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_ec2d113b7019dee3>>();
    };
    aotLib[0xb20c6d3a7e18668c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncrttiTickRttiValue_nothingTick4715542659269841615_86d8ab1b6e687fa0>>();
    };
    aotLib[0xb81d1d80cb8f97d6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_2d5f62712f1b31fa>>();
    };
    aotLib[0x3b4dfdb79efbc906] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_4e4f7194eb0890d6>>();
    };
    aotLib[0x145c7fb8519ff7b1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_dd70711ad532d4ac>>();
    };
    aotLib[0x5ba4361fbe4b9ed] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_2dc189ce58b0f9ff>>();
    };
    aotLib[0xeeaa8647aa9ce3a6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_990556032a1b320b>>();
    };
    aotLib[0xb74472d8adcdad1f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_fa24e3c4e479e739>>();
    };
    aotLib[0x10b1863b5a15ef14] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick9602082126133410484_54c8d45902a12b6a>>();
    };
    aotLib[0xb502ef7a66ca1e7b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_69ab1a2184065196>>();
    };
    aotLib[0x27b80b3832705c0b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_6965baf83e84e943>>();
    };
    aotLib[0xaa882a8f8d483111] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick9409548443506319159_df2e383d4b48e41d>>();
    };
    aotLib[0xd1ce5de8d0dd4102] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick9602082126133410484_ff5eacb201903461>>();
    };
    aotLib[0x80283b5f33b6cb0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_800d8621d2b80648>>();
    };
    aotLib[0x771526e70a4b7582] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_a043c7eded26b0c1>>();
    };
    aotLib[0x191a41b03301eb84] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickis_quote_field_allowedTick3545799563217800274_8df56ac57aa1aa67>>();
    };
    aotLib[0x7630e1c614a603c0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickcreate_stringTick16871435846852833435_ac46da0203621c82>>();
    };
    aotLib[0x6f8cedfc8ebcda20] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_15ca7add977814e6>>();
    };
    aotLib[0xbd37d3903f236e54] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_5f1c776d88550ffa>>();
    };
    aotLib[0x4b29b80976a975e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickfind_compiling_moduleTick4523452840392654583_2ad37c2da999dae1>>();
    };
    aotLib[0x882c529619c12869] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_d857f1a129cf603a>>();
    };
    aotLib[0xcf218d3073ee7f79] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick842554968825501494_79ad6734798edcff>>();
    };
    aotLib[0x7b63f2f9a19c3034] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_6d4e7dbe2a2e0573>>();
    };
    aotLib[0x898df6e30c9b53f8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isVectorType_6b8b46476d1214b4>>();
    };
    aotLib[0x98eec64b9a76f227] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isYetAnotherVectorTemplate_6471d45428bc715>>();
    };
    aotLib[0x8ca5f02774831b1b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_e17337a4a8ea5e>>();
    };
    aotLib[0x7891d5fddd38c078] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_917d990dd3d0a69e>>();
    };
    aotLib[0xb1f9a4d5bee782ac] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_13e8bddff42f7783>>();
    };
    aotLib[0xd0995ced50de43] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_6b6cafda29d5fa62>>();
    };
    aotLib[0xa5179d428bf4f4de] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isExpression_991e7b9aabc55e05>>();
    };
    aotLib[0x3c8073e27dead750] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_same_or_inherited_b1f5309ae03c83d0>>();
    };
    aotLib[0x51df9a05d06fd076] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_class_method_9615c7e22d648a7e>>();
    };
    aotLib[0xff63f7dc04778a71] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&emplace_new_60d5871f9e959ff9>>();
    };
    aotLib[0xcc8bf5872ca3f09d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&emplace_new_e80d981a19168757>>();
    };
    aotLib[0x6ae004cfb8299d99] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&emplace_new_6a9a9c984e26a11b>>();
    };
    aotLib[0xde2c7ad5af05f1c5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&emplace_new_eba12a326cb79d4c>>();
    };
    aotLib[0xac90495266ae18df] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&override_method_3ad2fc756944d0e7>>();
    };
    aotLib[0xeda5d7357d56d719] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&for_each_tag_function_4aa6238332e92abb>>();
    };
    aotLib[0x6ddfb4354b89d92d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&find_arg_a97f351b7d06ed8f>>();
    };
    aotLib[0x9ba67acc6916053a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&find_arg_b6ffaee9719f2933>>();
    };
    aotLib[0x5de9b628e9ae6f4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&apply_tag_annotation_8a88cbaecaf3bdaa>>();
    };
    aotLib[0x9eafb1714e4ace16] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_unique_function_d2492cf8492d63ba>>();
    };
    aotLib[0x85404bacdc0d2c9e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_unique_function_ptr_98335b992a400b59>>();
    };
    aotLib[0xfd8d807ab8a91fd2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_unique_generic_b8b7754471b20ed2>>();
    };
    aotLib[0x70d8b340b5374d37] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&setup_call_list_86425b60d55e3039>>();
    };
    aotLib[0x6927268f64e125b0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&setup_call_list_180d45678895ad8a>>();
    };
    aotLib[0x8317f67f011dc33b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&setup_macro_1eb22d9624e827a6>>();
    };
    aotLib[0xaaf681fb5365b8cf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&panic_expr_as_4c38e300a4bc92c4>>();
    };
    aotLib[0x2d34f7fc8902cdf3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickBuiltInFunction_67b8133cad1f66d3>>();
    };
    aotLib[0xb6435425f2286037] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickBuiltInFunction_86b221c9be037d90>>();
    };
    aotLib[0x5cf502a2f914fac6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickExternalFnBase_f41cfb59a73b3862>>();
    };
    aotLib[0x26d981d30b5e128d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickExternalFnBase_fb0ab060e41b89fb>>();
    };
    aotLib[0x3eb51163c6e4cfb0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickFunctionAnnotation_a9c64a70780109c6>>();
    };
    aotLib[0xcf8b4cb8b284044f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickFunctionAnnotation_55ebe0a1ac05ff73>>();
    };
    aotLib[0x69cc2495125c164e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickFunctionAnnotation_a2759eda58bebeb4>>();
    };
    aotLib[0x94c9d7d75e0094ed] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickFunctionAnnotation_d2eaa6a8d42295ac>>();
    };
    aotLib[0x8fd817571a7d7e04] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickStructureAnnotation_3284abe043d58747>>();
    };
    aotLib[0x8a5781b3de0acacd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickisTickStructureAnnotation_8e0018d5eb2637f2>>();
    };
    aotLib[0x8aa0a0d168c547f3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickStructureAnnotation_e33130a234a0a0d3>>();
    };
    aotLib[0x9caf6de2c6e31069] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncTickasTickStructureAnnotation_9f2533762c926957>>();
    };
    aotLib[0x21ac8198a3e4b0bf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&to_array_move_expr_6c95c6c3a47afc48>>();
    };
    aotLib[0xd7a07205f8852a9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_array_6fddd8e31acbfae0>>();
    };
    aotLib[0x16a1a784ff00eb93] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_dim_5e577a4630aa7d27>>();
    };
    aotLib[0x53bc8f2fc2329c23] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_pointer_c823df9ddee9c45f>>();
    };
    aotLib[0x4ba932fe85875b46] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_tuple_7bb6d375ae2e2688>>();
    };
    aotLib[0x6b119faea35b92c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_structure_b13b947d23bd62c7>>();
    };
    aotLib[0x5057c005358e68b6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_variant_1aa4185043fe5555>>();
    };
    aotLib[0x39a6ea11e2fc6fae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_table_8242a4ff974756c0>>();
    };
    aotLib[0xbc96c80fbe0440be] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_basic_f4fa8f2044f51a22>>();
    };
    aotLib[0x6b7c572d49e598cd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_enumeration_75ada03d57f913e7>>();
    };
    aotLib[0x5a0f7aa80b4a15d6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&walk_and_convert_badf850f139f3bb3>>();
    };
    aotLib[0x20256d129aeb69bb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_dc75d99dda1cee2c>>();
    };
    aotLib[0xf5309c70913c251] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_eb879c2dc1a7e11e>>();
    };
    aotLib[0x5e9c63888941a2c3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&get_vec_dst_type_a1ed8a6bb17eda3a>>();
    };
    aotLib[0x445d7020f1c0f325] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&to_arr_move_args_7ac8193c43e93926>>();
    };
    aotLib[0x1fb54e1f3f3e1a1f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&convert_vec_cf3d8478a556d0d6>>();
    };
    aotLib[0xb4d1988d5f5b7a7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&convert_quote_struct_field_529c713c43cf65dc>>();
    };
    aotLib[0xeed4381f2cdd871d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&convert_quote_structure_7513fe9a14da0cec>>();
    };
    aotLib[0xf01ed73235bfca48] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&convert_quote_expr_d61fc2e3f77fdf2e>>();
    };
    aotLib[0xdeb7b8c8272e1a05] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&convert_quote_to_expression_214bfe21da346d3e>>();
    };
    aotLib[0x3b546630426f40b5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_annotation_3750ee9a7cdedb92>>();
    };
    aotLib[0xa947a2fc515ced40] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_7ea25e1c3613ccaa>>();
    };
    aotLib[0x6cc52a631ee064e0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_5ce31aec0dc3e827>>();
    };
    aotLib[0x81990beedd40e8c5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_77e3691cc75925e6>>();
    };
    aotLib[0xe8a6595aa983acd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_2795d5527c6b541c>>();
    };
    aotLib[0xc3a031d3147e1f02] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_6137d9bcdc59acfc>>();
    };
    aotLib[0xb10c27f92f491274] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_a7f0a49b54847e5b>>();
    };
    aotLib[0x20fa1266951a3202] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_c867e1d1bd1e709d>>();
    };
    aotLib[0x50115591488bc12f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_annotation_a0a2cf8e458c7814>>();
    };
    aotLib[0x4cc83410e910489f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_b1f0a75a869f9d8e>>();
    };
    aotLib[0xda497a7ce26e2a3d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_1cccdd04ffc213ae>>();
    };
    aotLib[0x84d8c86b960f0782] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_c587cb52cc713868>>();
    };
    aotLib[0x5a4164d4891d5152] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_64b4b11cebf2a3a4>>();
    };
    aotLib[0xde9ba0528307ff74] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&add_annotation_argument_8f7d2e8ac5cc9c46>>();
    };
    aotLib[0x39d3a5559df6932f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&get_for_source_index_4960346f545ad8a5>>();
    };
    aotLib[0x64cfbecd4c0eefe5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&get_for_source_index_e610aeac2c72af06>>();
    };
    aotLib[0x5f82a86c114d7057] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&function_to_type_3ce465c9c4c12ac9>>();
    };
    aotLib[0x8f662b55d4371fbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&visit_finally_eebe169335929629>>();
    };
    aotLib[0xa4a3f484461a5c38] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isCMRES_18c2571faf70a24>>();
    };
    aotLib[0x3591e6966345a8c1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isCMRES_2379351cb339af97>>();
    };
    aotLib[0xdedfeef8e8d634b6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isMakeLocal_5ae2b5812df61b98>>();
    };
    aotLib[0xf46dd20030bffddf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isExprCallFunc_905b0dfa902dbccd>>();
    };
    aotLib[0x1c6b6987a0b5de4f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&get_workhorse_types_f596638550511fde>>();
    };
    aotLib[0xc71a1c426244e9f2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_argument_index_ae234aada3e459c5>>();
    };
    aotLib[0xa122eaa100a337ba] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isCMRESType_781422633ef81b3>>();
    };
    aotLib[0x9c8dd3691c1d5649] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_expression_impl_eaefe21084abd004>>();
    };
    aotLib[0x62b612b1a3e60a94] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_expression_cdb88f2c378a9017>>();
    };
    aotLib[0x6c88da2ebd093633] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_expression_803962089c61eb61>>();
    };
    aotLib[0x63ce69866f852d65] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_8f9450a77a61aede>>();
    };
    aotLib[0xdd401d9140080172] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getVectorElementCount_dae41b2187c417ca>>();
    };
    aotLib[0x9113ace65485f7bd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getVectorElementSize_ba6e81198dcbc886>>();
    };
    aotLib[0x1c9cdd93b8449ed2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getVectorElementType_c96967adbe9325ce>>();
    };
    aotLib[0x43824943304ebfbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getVectorOffset_56f905013ae8c372>>();
    };
    aotLib[0xe70ddbefd00a2ca3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_de6dfc85173d067c>>();
    };
    aotLib[0x5f46e6c9d8f8ebed] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_898d9918ac5a3e3e>>();
    };
    // [[ init script ]]
    aotLib[0xbd0b151fa7f31f71] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_9859992971649349273
AotListBase impl_aot_ast_boost(_anon_9859992971649349273::registerAotFunctions);
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
