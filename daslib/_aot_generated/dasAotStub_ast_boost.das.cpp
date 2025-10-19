#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/runtime_profile.h"
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
    tPointer = int32_t(INT64_C(45)),
    tFunction = int32_t(INT64_C(46)),
    tLambda = int32_t(INT64_C(47)),
    tIterator = int32_t(INT64_C(48)),
    tArray = int32_t(INT64_C(49)),
    tTable = int32_t(INT64_C(50)),
    tBlock = int32_t(INT64_C(51)),
    tTuple = int32_t(INT64_C(52)),
    tVariant = int32_t(INT64_C(53)),
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
EnumValueInfo __enum_info__c897fe55afe7f727_value_42 = { "tPointer", 45 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_43 = { "tFunction", 46 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_44 = { "tLambda", 47 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_45 = { "tIterator", 48 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_46 = { "tArray", 49 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_47 = { "tTable", 50 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_48 = { "tBlock", 51 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_49 = { "tTuple", 52 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_50 = { "tVariant", 53 };
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
TypeInfo * __tinfo_9[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[2] = { &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f };
TypeInfo * __tinfo_13[2] = { &__type_info__af90fe4c864e9d52, &__type_info__4fd1011fade7876f };
TypeInfo * __tinfo_14[4] = { &__type_info__af90fe4c864e9d52, &__type_info__d922fe078cefab30, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_17[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_18[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_20[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_25[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_28[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_30[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_32[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_34[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec };
TypeInfo * __tinfo_36[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__b68d800849332aec };
TypeInfo * __tinfo_38[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af81fe4c86352052 };

inline void _FuncbuiltinTickpushTick10769833213962245646_532aa57ee748e9 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_193_0, ast::AstFunctionAnnotation * __value_rename_at_193_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5ea400cd583decff ( Context * __context__, ast_boost::MacroMacro const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_ea422ce758633219 ( Context * __context__, ast_boost::TagFunctionAnnotation const  & __cl_rename_at_116_3 );
inline void _FuncbuiltinTickpushTick10769833213962245646_8072bcb5f8834d95 ( Context * __context__, TArray<ast::AstStructureAnnotation *> & __Arr_rename_at_193_4, ast::AstStructureAnnotation * __value_rename_at_193_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3d92632ea1c06f10 ( Context * __context__, ast_boost::SetupFunctionAnnotation const  & __cl_rename_at_116_6 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d3ab2c8dbad9d969 ( Context * __context__, ast_boost::SetupBlockAnnotation const  & __cl_rename_at_116_7 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3ef2f296a0f69015 ( Context * __context__, ast_boost::SetupStructureAnnotation const  & __cl_rename_at_116_8 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7910128189d4fa7c ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  & __cl_rename_at_116_9 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7c1880e164f77687 ( Context * __context__, ast_boost::SetupContractAnnotation const  & __cl_rename_at_116_10 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cf248f7c72f6077b ( Context * __context__, ast_boost::SetupReaderMacro const  & __cl_rename_at_116_11 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1e3a40dee0adc6b4 ( Context * __context__, ast_boost::SetupCommentReader const  & __cl_rename_at_116_12 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_688d5b5d381c2412 ( Context * __context__, ast_boost::SetupCallMacro const  & __cl_rename_at_116_13 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5ad54dc005365540 ( Context * __context__, ast_boost::SetupTypeInfoMacro const  & __cl_rename_at_116_14 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_763eae1f51cae115 ( Context * __context__, ast_boost::SetupVariantMacro const  & __cl_rename_at_116_15 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c247eb3e087743a6 ( Context * __context__, ast_boost::SetupForLoopMacro const  & __cl_rename_at_116_16 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_518dfc03923aac9d ( Context * __context__, ast_boost::SetupCaptureMacro const  & __cl_rename_at_116_17 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_38a2fc738f0684a7 ( Context * __context__, ast_boost::SetupTypeMacro const  & __cl_rename_at_116_18 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b87e7c5f8f5ce3c1 ( Context * __context__, ast_boost::SetupSimulateMacro const  & __cl_rename_at_116_19 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cf0d1eb6392a85d3 ( Context * __context__, ast_boost::TagStructureAnnotation const  & __cl_rename_at_116_20 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9f95af835ba924bb ( Context * __context__, ast_boost::TagFunctionMacro const  & __cl_rename_at_116_21 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c1b034570b4506a1 ( Context * __context__, ast_boost::SetupInferMacro const  & __cl_rename_at_116_22 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c4de1f05abb2f4d4 ( Context * __context__, ast_boost::SetupDirtyInferMacro const  & __cl_rename_at_116_23 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c4ac869cc1fdebdd ( Context * __context__, ast_boost::SetupOptimizationMacro const  & __cl_rename_at_116_24 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_99a4cf4ce09d9435 ( Context * __context__, ast_boost::SetupLintMacro const  & __cl_rename_at_116_25 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cb4bdd4ed7a9d0f5 ( Context * __context__, ast_boost::SetupGlobalLintMacro const  & __cl_rename_at_116_26 );
inline void _FuncbuiltinTickpushTick10769833213962245646_46d64d80c21524e3 ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_193_27, ast::AstVariantMacro * __value_rename_at_193_28 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7534c9a9e0721709 ( Context * __context__, ast_boost::BetterRttiVisitor const  & __cl_rename_at_116_29 );
inline void clone_7f4a34d879fc35f4 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & __dest_rename_at_1121_30, smart_ptr_raw<FunctionAnnotation> const  __src_rename_at_1121_31 );
inline smart_ptr_raw<FunctionAnnotation> _FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> __clone_src_rename_at_1118_32 );
inline Module * _FuncastTickfind_moduleTick11101329256228773934_92a5dd08b29912b8 ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_948_34, char * const  __name_rename_at_948_35 );
inline void _FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f ( Context * __context__, TArray<char *> & __Arr_rename_at_193_38, char * __value_rename_at_193_39 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9 ( Context * __context__, TArray<char *> const  & __it_rename_at_22_40, char * const  __separator_rename_at_22_41 );
inline void _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 ( Context * __context__, TArray<char *> & __Arr_rename_at_177_46, char * const  __value_rename_at_177_47 );
inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec ( Context * __context__, TArray<smart_ptr_raw<Function>> & __Arr_rename_at_393_48, smart_ptr_raw<Function> __value_rename_at_393_49 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2 ( Context * __context__, TArray<smart_ptr_raw<Function>> & __a_rename_at_1336_50 );
inline void clone_e5dbb6c9a183080e ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_250_52, smart_ptr_raw<Function> const  __src_rename_at_250_53 );
inline char * _FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_54, bool __extra_rename_at_38_55, bool __contracts_rename_at_38_56, bool __modules_rename_at_38_57 );
inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1886_58 );
inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8 ( Context * __context__ );
inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_eb77b9b7854f344 ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1892_60 );
inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1892_61 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_1233c5737f91b474 ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_482_62, smart_ptr_raw<Expression> __enull_rename_at_482_63 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_4aa4fc3752456c49 ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_482_81, smart_ptr_raw<Expression> __enull_rename_at_482_82 );
inline Annotation * _FuncbuiltinTickget_ptrTick8468476673553620226_49b21ea86f1f3e71 ( Context * __context__, smart_ptr_raw<Annotation> const  __src_rename_at_1886_100 );
inline void clone_23a052359607d993 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_642_101, smart_ptr_raw<TypeDecl> const  __src_rename_at_642_102 );
inline void clone_a8386ed128f4f0b4 ( Context * __context__, smart_ptr_raw<Enumeration> & __dest_rename_at_794_103, Enumeration * const  __src_rename_at_794_104 );
inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_584104531b17a816 ( Context * __context__, char * const  __name_rename_at_967_105 );
inline void clone_6e9cdc5c6ca1d1f3 ( Context * __context__, smart_ptr_raw<Annotation> & __dest_rename_at_873_106, smart_ptr_raw<Annotation> const  __src_rename_at_873_107 );
inline char * _FuncastTickdescribeTick842554968825501494_5630d72624d0f510 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_108 );
inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1886_109 );
inline bool isVectorType_34a98665df73cc6f ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_16_110 );
inline char * describe_670b9c27dbedaf8f ( Context * __context__, AnnotationArgumentList const  & __list_rename_at_26_111 );
inline char * describe_19857d293154313d ( Context * __context__, AnnotationDeclaration const  & __ann_rename_at_30_114 );
inline char * describe_d0f1c4ce2af40b5e ( Context * __context__, AnnotationList const  & __list_rename_at_38_115 );
inline char * describe_24ebe982465d57fc ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_42_118 );
inline bool isExpression_f1fadf50b6070b86 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_63_119, bool __top_rename_at_63_120 );
inline bool is_same_or_inherited_530410b8a7a25edb ( Context * __context__, Structure const  * const  __parent_rename_at_81_121, Structure const  * const  __child_rename_at_81_122 );
inline bool is_class_method_4783315be6e05a7b ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_92_124, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_92_125 );
inline void emplace_new_9bef84999dd1e3f4 ( Context * __context__, das::vector<smart_ptr<Expression>> & __vec_rename_at_114_126, smart_ptr_raw<Expression> __ptr_rename_at_114_127 );
inline void emplace_new_7386d366d948387e ( Context * __context__, das::vector<smart_ptr<TypeDecl>> & __vec_rename_at_118_128, smart_ptr_raw<TypeDecl> __ptr_rename_at_118_129 );
inline void emplace_new_2f0cb94c081aada2 ( Context * __context__, das::vector<smart_ptr<Variable>> & __vec_rename_at_122_130, smart_ptr_raw<Variable> __ptr_rename_at_122_131 );
inline void emplace_new_7782fb9c58748acd ( Context * __context__, MakeStruct & __vec_rename_at_126_132, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_126_133 );
inline bool override_method_49b0943e987177d7 ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_130_134, char * const  __name_rename_at_130_135, char * const  __funcName_rename_at_130_136 );
inline void for_each_tag_function_f441072c0eb499ad ( Context * __context__, Module * const  __mod_rename_at_183_140, char * const  __tag_rename_at_183_141, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __blk_rename_at_183_142 );
inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_a0e87cbd443f10f5 ( Context * __context__, char * const  __argn_rename_at_209_149, AnnotationArgumentList const  & __args_rename_at_209_150 );
inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_eefed45a6ebf3b9 ( Context * __context__, AnnotationArgumentList const  & __args_rename_at_218_152, char * const  __argn_rename_at_218_153 );
inline void apply_tag_annotation_1944b9a89fd11326 ( Context * __context__, char * const  __tag_rename_at_227_155, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_227_156 );
inline smart_ptr_raw<Function> find_unique_function_e26f474c35febe1d ( Context * __context__, Module * const  __mod_rename_at_246_161, char * const  __name_rename_at_246_162, bool __canfail_rename_at_246_163 );
inline smart_ptr_raw<Function> find_unique_generic_22c96d795ba15b85 ( Context * __context__, Module * const  __mod_rename_at_262_167, char * const  __name_rename_at_262_168, bool __canfail_rename_at_262_169 );
inline ExprBlock * setup_call_list_329404b139e4adba ( Context * __context__, char * const  __name_rename_at_278_173, LineInfo const  & __at_rename_at_278_174, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __subblock_rename_at_278_175 );
inline ExprBlock * setup_call_list_8438ddab4dff539b ( Context * __context__, char * const  __name_rename_at_299_180, LineInfo const  & __at_rename_at_299_181, bool __isInit_rename_at_299_182, bool __isPrivate_rename_at_299_183, bool __isLateInit_rename_at_299_184 );
inline ExprBlock * setup_macro_979cb6d902caaebd ( Context * __context__, char * const  __name_rename_at_313_186, LineInfo const  & __at_rename_at_313_187 );
inline void * panic_expr_as_2ea20f674c842c9c ( Context * __context__ );
inline bool _FuncTickisTickBuiltInFunction_3d26f4806589821b ( Context * __context__, Function * const  __foo_rename_at_553_200 );
inline BuiltInFunction * _FuncTickasTickBuiltInFunction_c47ad3b03c9bc354 ( Context * __context__, Function * const  __foo_rename_at_557_201 );
inline bool _FuncTickisTickExternalFnBase_24b69557e6b14161 ( Context * __context__, Function * const  __foo_rename_at_566_202 );
inline ExternalFnBase * _FuncTickasTickExternalFnBase_b840ea32468fb731 ( Context * __context__, Function * const  __foo_rename_at_570_203 );
inline bool _FuncTickisTickFunctionAnnotation_bf985c1f9a8f8112 ( Context * __context__, Annotation * const  __foo_rename_at_579_204 );
inline bool _FuncTickisTickFunctionAnnotation_88a1558057f6b81f ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_583_205 );
inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_8812613c169a9dba ( Context * __context__, Annotation * const  __foo_rename_at_587_206 );
inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_89843e969c415f46 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_592_207 );
inline bool _FuncTickisTickStructureAnnotation_a9f69a8ea44dcd1b ( Context * __context__, Annotation * const  __foo_rename_at_602_208 );
inline bool _FuncTickisTickStructureAnnotation_25f742c66283203c ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_606_209 );
inline StructureAnnotation * _FuncTickasTickStructureAnnotation_dac337e2d083c291 ( Context * __context__, Annotation * const  __foo_rename_at_610_210 );
inline StructureAnnotation * _FuncTickasTickStructureAnnotation_b43078c41d1f685 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_615_211 );
inline smart_ptr_raw<Expression> walk_and_convert_array_ba1cc70e35611268 ( Context * __context__, uint8_t const  * const  __data_rename_at_620_212, smart_ptr_raw<TypeDecl> const  __info_rename_at_620_213, LineInfo const  & __at_rename_at_620_214 );
inline smart_ptr_raw<Expression> walk_and_convert_dim_9001b9148a98cff6 ( Context * __context__, uint8_t const  * const  __data_rename_at_637_221, smart_ptr_raw<TypeDecl> const  __info_rename_at_637_222, LineInfo const  & __at_rename_at_637_223 );
inline smart_ptr_raw<Expression> walk_and_convert_pointer_47b794d0dd59337d ( Context * __context__, uint8_t const  * const  __data_rename_at_652_231, smart_ptr_raw<TypeDecl> const  __info_rename_at_652_232, LineInfo const  & __at_rename_at_652_233 );
inline smart_ptr_raw<Expression> walk_and_convert_tuple_9ce3b13721ba5daf ( Context * __context__, uint8_t const  * const  __data_rename_at_661_237, smart_ptr_raw<TypeDecl> const  __info_rename_at_661_238, LineInfo const  & __at_rename_at_661_239 );
inline smart_ptr_raw<Expression> walk_and_convert_structure_106d0a1bf460f63c ( Context * __context__, uint8_t const  * const  __data_rename_at_673_244, smart_ptr_raw<TypeDecl> const  __info_rename_at_673_245, LineInfo const  & __at_rename_at_673_246 );
inline smart_ptr_raw<Expression> walk_and_convert_variant_cdeba207e46113d9 ( Context * __context__, uint8_t const  * const  __data_rename_at_694_255, smart_ptr_raw<TypeDecl> const  __info_rename_at_694_256, LineInfo const  & __at_rename_at_694_257 );
inline smart_ptr_raw<Expression> walk_and_convert_table_bafed583a64bc3a1 ( Context * __context__, uint8_t const  * const  __data_rename_at_709_264, smart_ptr_raw<TypeDecl> const  __info_rename_at_709_265, LineInfo const  & __at_rename_at_709_266 );
inline smart_ptr_raw<Expression> walk_and_convert_basic_5a2f4f9254235414 ( Context * __context__, uint8_t const  * const  __data_rename_at_735_279, smart_ptr_raw<TypeDecl> const  __info_rename_at_735_280, LineInfo const  & __at_rename_at_735_281 );
inline smart_ptr_raw<Expression> walk_and_convert_enumeration_5bb2d478c4ffeaaa ( Context * __context__, uint8_t const  * const  __data_rename_at_778_283, smart_ptr_raw<TypeDecl> const  __info_rename_at_778_284, LineInfo const  & __at_rename_at_778_285 );
inline smart_ptr_raw<Expression> walk_and_convert_cfdd8313a6a5987 ( Context * __context__, uint8_t const  * const  __data_rename_at_798_289, smart_ptr_raw<TypeDecl> const  __info_rename_at_798_290, LineInfo const  & __at_rename_at_798_291 );
inline Annotation const  * find_annotation_e87941f09ff8d46c ( Context * __context__, char * const  __mod_name_rename_at_853_292, char * const  __ann_name_rename_at_853_293 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_79e01888a17a2cf8 ( Context * __context__, char * const  __mod_name_rename_at_869_297, char * const  __ann_name_rename_at_869_298, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_869_299 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_91779d544c0857a9 ( Context * __context__, char * const  __mod_name_rename_at_892_305, char * const  __ann_name_rename_at_892_306 );
inline void append_annotation_9e9720e2bdf176ae ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_900_309, char * const  __mod_name_rename_at_900_310, char * const  __ann_name_rename_at_900_311 );
inline void append_annotation_4e3ac504625b9e59 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_907_314, char * const  __mod_name_rename_at_907_315, char * const  __ann_name_rename_at_907_316 );
inline void append_annotation_1d124a466ae78997 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_914_319, char * const  __mod_name_rename_at_914_320, char * const  __ann_name_rename_at_914_321 );
inline void append_annotation_d2aadccfb018501d ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_921_324, char * const  __mod_name_rename_at_921_325, char * const  __ann_name_rename_at_921_326, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_921_327 );
inline void append_annotation_4239f8097567c00 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_928_330, char * const  __mod_name_rename_at_928_331, char * const  __ann_name_rename_at_928_332, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_928_333 );
inline void append_annotation_84797c2112b4fd76 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_935_336, char * const  __mod_name_rename_at_935_337, char * const  __ann_name_rename_at_935_338, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_935_339 );
inline int32_t add_annotation_argument_ecdc7f01cc8d28cd ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_942_342, char * const  __argName_rename_at_942_343, bool __val_rename_at_942_344 );
inline int32_t add_annotation_argument_5d1cfc8d958b1b74 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_949_346, char * const  __argName_rename_at_949_347, int32_t __val_rename_at_949_348 );
inline int32_t add_annotation_argument_2ba02ec1a95c40de ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_956_350, char * const  __argName_rename_at_956_351, float __val_rename_at_956_352 );
inline int32_t add_annotation_argument_e01dc466768bdd13 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_963_354, char * const  __argName_rename_at_963_355, char * const  __val_rename_at_963_356 );
inline int32_t add_annotation_argument_e7f221e58ec46512 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_970_358, AnnotationArgument const  & __ann_rename_at_970_359 );
inline int32_t get_for_source_index_19e9c390ee4338e2 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_987_361, smart_ptr_raw<Variable> const  __svar_rename_at_987_362 );
inline int32_t get_for_source_index_e425541a2a706c53 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_996_367, smart_ptr_raw<Expression> const  __source_rename_at_996_368 );
inline smart_ptr_raw<TypeDecl> function_to_type_1f450dac98d6071d ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_1044_373 );
inline void visit_finally_d773bb44db182978 ( Context * __context__, ExprBlock * const  __blk_rename_at_1056_379, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1056_380 );
inline bool isCMRES_2bb438cbd738593f ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1062_381 );
inline bool isCMRES_45d360720945cc74 ( Context * __context__, Function * const  __fun_rename_at_1066_382 );
inline bool isMakeLocal_c461b44592b8facf ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1070_383 );
inline bool isExprCallFunc_aac1b10a17185ef1 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1078_384 );
inline TDim<DAS_COMMENT(bound_enum) das::Type,31> get_workhorse_types_62f1d76706bcc1ad ( Context * __context__ );
inline int32_t find_argument_index_3d6a951c2870d00f ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1098_385, char * const  __name_rename_at_1098_386 );
inline bool isCMRESType_bbf11aadb0354b4d ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1107_391 );
inline void debug_expression_impl_203941aeed67e19a ( Context * __context__, StringBuilderWriter & __writer_rename_at_1115_392, smart_ptr_raw<Expression> const  __expr_rename_at_1115_393, Bitfield __deFlags_rename_at_1115_394, int32_t __tabs_rename_at_1115_395 );
inline char * debug_expression_72d312ce8f3ec686 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1221_429, Bitfield __deFlags_rename_at_1221_430 );
inline char * debug_expression_84991cbecc0ca3af ( Context * __context__, Expression * const  __expr_rename_at_1227_432 );
inline char * describe_d11036fb3823617b ( Context * __context__, Expression * const  __expr_rename_at_1233_433 );
inline int32_t getVectorElementCount_6156018073f7b991 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1239_434 );
inline int32_t getVectorElementSize_7fe52b138a9b94ac ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1246_435 );
inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_17ab3a94ab7f2835 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1251_436 );
inline int32_t getVectorOffset_cb21e53271916e73 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1260_437, char * const  __ident_rename_at_1260_438 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_532aa57ee748e9 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_193_0, ast::AstFunctionAnnotation * __value_rename_at_193_1 )
{
    das_copy(__Arr_rename_at_193_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_193_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5ea400cd583decff ( Context * __context__, ast_boost::MacroMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_ea422ce758633219 ( Context * __context__, ast_boost::TagFunctionAnnotation const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_8072bcb5f8834d95 ( Context * __context__, TArray<ast::AstStructureAnnotation *> &  __Arr_rename_at_193_4, ast::AstStructureAnnotation * __value_rename_at_193_5 )
{
    das_copy(__Arr_rename_at_193_4(builtin_array_push_back(das_arg<TArray<ast::AstStructureAnnotation *>>::pass(__Arr_rename_at_193_4),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_5);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3d92632ea1c06f10 ( Context * __context__, ast_boost::SetupFunctionAnnotation const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d3ab2c8dbad9d969 ( Context * __context__, ast_boost::SetupBlockAnnotation const  &  __cl_rename_at_116_7 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_7.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3ef2f296a0f69015 ( Context * __context__, ast_boost::SetupStructureAnnotation const  &  __cl_rename_at_116_8 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_8.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7910128189d4fa7c ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  &  __cl_rename_at_116_9 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_9.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7c1880e164f77687 ( Context * __context__, ast_boost::SetupContractAnnotation const  &  __cl_rename_at_116_10 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_10.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cf248f7c72f6077b ( Context * __context__, ast_boost::SetupReaderMacro const  &  __cl_rename_at_116_11 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_11.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1e3a40dee0adc6b4 ( Context * __context__, ast_boost::SetupCommentReader const  &  __cl_rename_at_116_12 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_12.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_688d5b5d381c2412 ( Context * __context__, ast_boost::SetupCallMacro const  &  __cl_rename_at_116_13 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_13.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5ad54dc005365540 ( Context * __context__, ast_boost::SetupTypeInfoMacro const  &  __cl_rename_at_116_14 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_14.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_763eae1f51cae115 ( Context * __context__, ast_boost::SetupVariantMacro const  &  __cl_rename_at_116_15 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_15.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c247eb3e087743a6 ( Context * __context__, ast_boost::SetupForLoopMacro const  &  __cl_rename_at_116_16 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_16.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_518dfc03923aac9d ( Context * __context__, ast_boost::SetupCaptureMacro const  &  __cl_rename_at_116_17 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_17.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_38a2fc738f0684a7 ( Context * __context__, ast_boost::SetupTypeMacro const  &  __cl_rename_at_116_18 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_18.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b87e7c5f8f5ce3c1 ( Context * __context__, ast_boost::SetupSimulateMacro const  &  __cl_rename_at_116_19 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_19.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cf0d1eb6392a85d3 ( Context * __context__, ast_boost::TagStructureAnnotation const  &  __cl_rename_at_116_20 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_20.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9f95af835ba924bb ( Context * __context__, ast_boost::TagFunctionMacro const  &  __cl_rename_at_116_21 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_21.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c1b034570b4506a1 ( Context * __context__, ast_boost::SetupInferMacro const  &  __cl_rename_at_116_22 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_22.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c4de1f05abb2f4d4 ( Context * __context__, ast_boost::SetupDirtyInferMacro const  &  __cl_rename_at_116_23 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_23.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c4ac869cc1fdebdd ( Context * __context__, ast_boost::SetupOptimizationMacro const  &  __cl_rename_at_116_24 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_24.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_99a4cf4ce09d9435 ( Context * __context__, ast_boost::SetupLintMacro const  &  __cl_rename_at_116_25 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_25.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cb4bdd4ed7a9d0f5 ( Context * __context__, ast_boost::SetupGlobalLintMacro const  &  __cl_rename_at_116_26 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_26.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_46d64d80c21524e3 ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_193_27, ast::AstVariantMacro * __value_rename_at_193_28 )
{
    das_copy(__Arr_rename_at_193_27(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_193_27),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_28);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7534c9a9e0721709 ( Context * __context__, ast_boost::BetterRttiVisitor const  &  __cl_rename_at_116_29 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_29.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline void clone_7f4a34d879fc35f4 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & __dest_rename_at_1121_30, smart_ptr_raw<FunctionAnnotation> const  __src_rename_at_1121_31 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1121_30),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1121_31),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> __clone_src_rename_at_1118_32 )
{
    smart_ptr_raw<FunctionAnnotation> __clone_dest_rename_at_1120_33;das_zero(__clone_dest_rename_at_1120_33);
    clone_7f4a34d879fc35f4(__context__,__clone_dest_rename_at_1120_33,__clone_src_rename_at_1118_32);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(__clone_dest_rename_at_1120_33);
}

inline Module * _FuncastTickfind_moduleTick11101329256228773934_92a5dd08b29912b8 ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_948_34, char * const  __name_rename_at_948_35 ) { das_stack_prologue __prologue(__context__,112,"ast`find_module`11101329256228773934 " DAS_FILE_LINE);
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

inline void _FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f ( Context * __context__, TArray<char *> &  __Arr_rename_at_193_38, char * __value_rename_at_193_39 )
{
    das_copy(__Arr_rename_at_193_38(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_193_38),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_39);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9 ( Context * __context__, TArray<char *> const  &  __it_rename_at_22_40, char * const  __separator_rename_at_22_41 )
{
    char * __st_rename_at_27_42 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_27_43) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_28_44 = true;
        {
            bool __need_loop_29 = true;
            // elem: string const&
            das_iterator<TArray<char *> const > __elem_iterator(__it_rename_at_22_40);
            char * const  * __elem_rename_at_29_45;
            __need_loop_29 = __elem_iterator.first(__context__,(__elem_rename_at_29_45)) && __need_loop_29;
            for ( ; __need_loop_29 ; __need_loop_29 = __elem_iterator.next(__context__,(__elem_rename_at_29_45)) )
            {
                if ( __skip_first_rename_at_28_44 )
                {
                    das_copy(__skip_first_rename_at_28_44,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_27_43),cast<char * const >::from(__separator_rename_at_22_41)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_27_43),cast<char * const >::from((*__elem_rename_at_29_45))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_29_45));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_27_42);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 ( Context * __context__, TArray<char *> &  __Arr_rename_at_177_46, char * const  __value_rename_at_177_47 )
{
    das_copy(__Arr_rename_at_177_46(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_177_46),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_177_47);
}

inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec ( Context * __context__, TArray<smart_ptr_raw<Function>> &  __Arr_rename_at_393_48, smart_ptr_raw<Function> __value_rename_at_393_49 )
{
    clone_e5dbb6c9a183080e(__context__,__Arr_rename_at_393_48(builtin_array_push_back_zero(das_arg<TArray<smart_ptr_raw<Function>>>::pass(__Arr_rename_at_393_48),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_393_49);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2 ( Context * __context__, TArray<smart_ptr_raw<Function>> &  __a_rename_at_1336_50 )
{
    {
        bool __need_loop_1338 = true;
        // aV: smart_ptr<ast::Function> aka TT&
        das_iterator<TArray<smart_ptr_raw<Function>>> __aV_iterator(__a_rename_at_1336_50);
        smart_ptr_raw<Function> * __aV_rename_at_1338_51;
        __need_loop_1338 = __aV_iterator.first(__context__,(__aV_rename_at_1338_51)) && __need_loop_1338;
        for ( ; __need_loop_1338 ; __need_loop_1338 = __aV_iterator.next(__context__,(__aV_rename_at_1338_51)) )
        {
            das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,(*__aV_rename_at_1338_51));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1338_51));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Function>>>::pass(__a_rename_at_1336_50),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_e5dbb6c9a183080e ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_250_52, smart_ptr_raw<Function> const  __src_rename_at_250_53 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_250_52),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_250_53),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_54, bool __extra_rename_at_38_55, bool __contracts_rename_at_38_56, bool __modules_rename_at_38_57 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_54,__extra_rename_at_38_55,__contracts_rename_at_38_56,__modules_rename_at_38_57,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1886_58 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1886_58));
}

inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8 ( Context * __context__ )
{
    AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> __t_rename_at_195_59;das_zero(__t_rename_at_195_59);
    set_variant_index(das_arg<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::pass(__t_rename_at_195_59),8);
    return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(__t_rename_at_195_59);
}

inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_eb77b9b7854f344 ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1892_60 )
{
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__src_rename_at_1892_60));
}

inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1892_61 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1892_61));
}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_1233c5737f91b474 ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_482_62, smart_ptr_raw<Expression> __enull_rename_at_482_63 ) { das_stack_prologue __prologue(__context__,224,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_483_64; memset((void*)&__vdr_rename_at_483_64,0,sizeof(__vdr_rename_at_483_64));
    smart_ptr_raw<ExprConstString> __cna_rename_at_484_66; memset((void*)&__cna_rename_at_484_66,0,sizeof(__cna_rename_at_484_66));
    smart_ptr_raw<ExprOp2> __cond_rename_at_485_68; memset((void*)&__cond_rename_at_485_68,0,sizeof(__cond_rename_at_485_68));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_486_70; memset((void*)&__ctype_rename_at_486_70,0,sizeof(__ctype_rename_at_486_70));
    smart_ptr_raw<Expression> __esub_rename_at_490_72; memset((void*)&__esub_rename_at_490_72,0,sizeof(__esub_rename_at_490_72));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_492_73; memset((void*)&__neqz_rename_at_492_73,0,sizeof(__neqz_rename_at_492_73));
    smart_ptr_raw<ExprOp2> __nand_rename_at_496_75; memset((void*)&__nand_rename_at_496_75,0,sizeof(__nand_rename_at_496_75));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_502_77; memset((void*)&__nsub_rename_at_502_77,0,sizeof(__nsub_rename_at_502_77));
    smart_ptr_raw<ExprCast> __vcast_rename_at_508_78; memset((void*)&__vcast_rename_at_508_78,0,sizeof(__vcast_rename_at_508_78));
    smart_ptr_raw<ExprOp3> __tri_rename_at_514_79; memset((void*)&__tri_rename_at_514_79,0,sizeof(__tri_rename_at_514_79));
    /* finally */ auto __finally_482= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_514_79);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_508_78);
    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__esub_rename_at_490_72);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_486_70);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_485_68);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_484_66);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_483_64);
    /* end finally */ });
    __vdr_rename_at_483_64; das_zero(__vdr_rename_at_483_64); das_move(__vdr_rename_at_483_64, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_483) {
        das_copy((__mks_483.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
        das_move((__mks_483.value /*value*/),(clone_expression(__expr_rename_at_482_62->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_483.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_484_66; das_zero(__cna_rename_at_484_66); das_move(__cna_rename_at_484_66, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_484) {
        das_copy((__mks_484.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_484.text /*value*/,__expr_rename_at_482_62->name /*name*/);
        }    }))));
    __cond_rename_at_485_68; das_zero(__cond_rename_at_485_68); das_move(__cond_rename_at_485_68, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_485) {
        das_copy((__mks_485.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
        das_move((__mks_485.left /*left*/),(__vdr_rename_at_483_64));
        das_move((__mks_485.right /*right*/),(__cna_rename_at_484_66));
        {
            set_das_string(das_arg<das::string>::pass(__mks_485.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_486_70; das_zero(__ctype_rename_at_486_70); das_move(__ctype_rename_at_486_70, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_486) {
        das_copy((__mks_486.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
        das_copy((__mks_486.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_486.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_488) {
            das_copy((__mks_488.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
            das_copy((__mks_488.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_488.alias /*alias*/,__expr_rename_at_482_62->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_490_72; das_zero(__esub_rename_at_490_72); das_move(__esub_rename_at_490_72, clone_expression(__expr_rename_at_482_62->value /*value*/));
    if ( __expr_rename_at_482_62->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_491= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_496_75);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_492_73);
        /* end finally */ });
        __neqz_rename_at_492_73; das_zero(__neqz_rename_at_492_73); das_move(__neqz_rename_at_492_73, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_492) {
            das_copy((__mks_492.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
            das_move((__mks_492.left /*left*/),(clone_expression(__expr_rename_at_482_62->value /*value*/)));
            das_move((__mks_492.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_494) {
                das_copy((__mks_494.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_492.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_496_75; das_zero(__nand_rename_at_496_75); das_move(__nand_rename_at_496_75, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_496) {
            das_copy((__mks_496.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
            das_move((__mks_496.left /*left*/),(__neqz_rename_at_492_73));
            das_move((__mks_496.right /*right*/),(__cond_rename_at_485_68));
            {
                set_das_string(das_arg<das::string>::pass(__mks_496.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_485_68),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_496_75),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_501= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_502_77);
        /* end finally */ });
        __nsub_rename_at_502_77; das_zero(__nsub_rename_at_502_77); das_move(__nsub_rename_at_502_77, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_502) {
            das_copy((__mks_502.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
            das_move((__mks_502.subexpr /*subexpr*/),(__esub_rename_at_490_72));
            das_copy((__mks_502.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_490_72),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_502_77),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_508_78; das_zero(__vcast_rename_at_508_78); das_move(__vcast_rename_at_508_78, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_508) {
        das_copy((__mks_508.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
        das_move((__mks_508.subexpr /*subexpr*/),(__esub_rename_at_490_72));
        das_move((__mks_508.castType /*castType*/),(__ctype_rename_at_486_70));
        das_copy((__mks_508.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_508.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_514_79; das_zero(__tri_rename_at_514_79); das_move(__tri_rename_at_514_79, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_514) {
        das_copy((__mks_514.at /*at*/),(__expr_rename_at_482_62->at /*at*/));
        das_move((__mks_514.subexpr /*subexpr*/),(__cond_rename_at_485_68));
        das_move((__mks_514.left /*left*/),(__vcast_rename_at_508_78));
        das_move((__mks_514.right /*right*/),(__enull_rename_at_482_63));
        {
            set_das_string(das_arg<das::string>::pass(__mks_514.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_514_79->genFlags /*genFlags*/,__expr_rename_at_482_62->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_514_79);
}}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_4aa4fc3752456c49 ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_482_81, smart_ptr_raw<Expression> __enull_rename_at_482_82 ) { das_stack_prologue __prologue(__context__,224,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_483_83; memset((void*)&__vdr_rename_at_483_83,0,sizeof(__vdr_rename_at_483_83));
    smart_ptr_raw<ExprConstString> __cna_rename_at_484_85; memset((void*)&__cna_rename_at_484_85,0,sizeof(__cna_rename_at_484_85));
    smart_ptr_raw<ExprOp2> __cond_rename_at_485_87; memset((void*)&__cond_rename_at_485_87,0,sizeof(__cond_rename_at_485_87));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_486_89; memset((void*)&__ctype_rename_at_486_89,0,sizeof(__ctype_rename_at_486_89));
    smart_ptr_raw<Expression> __esub_rename_at_490_91; memset((void*)&__esub_rename_at_490_91,0,sizeof(__esub_rename_at_490_91));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_492_92; memset((void*)&__neqz_rename_at_492_92,0,sizeof(__neqz_rename_at_492_92));
    smart_ptr_raw<ExprOp2> __nand_rename_at_496_94; memset((void*)&__nand_rename_at_496_94,0,sizeof(__nand_rename_at_496_94));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_502_96; memset((void*)&__nsub_rename_at_502_96,0,sizeof(__nsub_rename_at_502_96));
    smart_ptr_raw<ExprCast> __vcast_rename_at_508_97; memset((void*)&__vcast_rename_at_508_97,0,sizeof(__vcast_rename_at_508_97));
    smart_ptr_raw<ExprOp3> __tri_rename_at_514_98; memset((void*)&__tri_rename_at_514_98,0,sizeof(__tri_rename_at_514_98));
    /* finally */ auto __finally_482= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_514_98);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_508_97);
    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__esub_rename_at_490_91);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_486_89);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_485_87);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_484_85);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_483_83);
    /* end finally */ });
    __vdr_rename_at_483_83; das_zero(__vdr_rename_at_483_83); das_move(__vdr_rename_at_483_83, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_483) {
        das_copy((__mks_483.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
        das_move((__mks_483.value /*value*/),(clone_expression(__expr_rename_at_482_81->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_483.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_484_85; das_zero(__cna_rename_at_484_85); das_move(__cna_rename_at_484_85, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_484) {
        das_copy((__mks_484.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_484.text /*value*/,__expr_rename_at_482_81->name /*name*/);
        }    }))));
    __cond_rename_at_485_87; das_zero(__cond_rename_at_485_87); das_move(__cond_rename_at_485_87, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_485) {
        das_copy((__mks_485.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
        das_move((__mks_485.left /*left*/),(__vdr_rename_at_483_83));
        das_move((__mks_485.right /*right*/),(__cna_rename_at_484_85));
        {
            set_das_string(das_arg<das::string>::pass(__mks_485.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_486_89; das_zero(__ctype_rename_at_486_89); das_move(__ctype_rename_at_486_89, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_486) {
        das_copy((__mks_486.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
        das_copy((__mks_486.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_486.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_488) {
            das_copy((__mks_488.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
            das_copy((__mks_488.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_488.alias /*alias*/,__expr_rename_at_482_81->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_490_91; das_zero(__esub_rename_at_490_91); das_move(__esub_rename_at_490_91, clone_expression(__expr_rename_at_482_81->value /*value*/));
    if ( __expr_rename_at_482_81->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_491= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_496_94);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_492_92);
        /* end finally */ });
        __neqz_rename_at_492_92; das_zero(__neqz_rename_at_492_92); das_move(__neqz_rename_at_492_92, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_492) {
            das_copy((__mks_492.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
            das_move((__mks_492.left /*left*/),(clone_expression(__expr_rename_at_482_81->value /*value*/)));
            das_move((__mks_492.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_494) {
                das_copy((__mks_494.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_492.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_496_94; das_zero(__nand_rename_at_496_94); das_move(__nand_rename_at_496_94, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_496) {
            das_copy((__mks_496.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
            das_move((__mks_496.left /*left*/),(__neqz_rename_at_492_92));
            das_move((__mks_496.right /*right*/),(__cond_rename_at_485_87));
            {
                set_das_string(das_arg<das::string>::pass(__mks_496.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_485_87),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_496_94),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_501= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_502_96);
        /* end finally */ });
        __nsub_rename_at_502_96; das_zero(__nsub_rename_at_502_96); das_move(__nsub_rename_at_502_96, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_502) {
            das_copy((__mks_502.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
            das_move((__mks_502.subexpr /*subexpr*/),(__esub_rename_at_490_91));
            das_copy((__mks_502.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_490_91),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_502_96),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_508_97; das_zero(__vcast_rename_at_508_97); das_move(__vcast_rename_at_508_97, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_508) {
        das_copy((__mks_508.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
        das_move((__mks_508.subexpr /*subexpr*/),(__esub_rename_at_490_91));
        das_move((__mks_508.castType /*castType*/),(__ctype_rename_at_486_89));
        das_copy((__mks_508.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_508.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_514_98; das_zero(__tri_rename_at_514_98); das_move(__tri_rename_at_514_98, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_514) {
        das_copy((__mks_514.at /*at*/),(__expr_rename_at_482_81->at /*at*/));
        das_move((__mks_514.subexpr /*subexpr*/),(__cond_rename_at_485_87));
        das_move((__mks_514.left /*left*/),(__vcast_rename_at_508_97));
        das_move((__mks_514.right /*right*/),(__enull_rename_at_482_82));
        {
            set_das_string(das_arg<das::string>::pass(__mks_514.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_514_98->genFlags /*genFlags*/,__expr_rename_at_482_81->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_514_98);
}}

inline Annotation * _FuncbuiltinTickget_ptrTick8468476673553620226_49b21ea86f1f3e71 ( Context * __context__, smart_ptr_raw<Annotation> const  __src_rename_at_1886_100 )
{
    return das_auto_cast<Annotation *>::cast(das_cast<Annotation *>::cast(__src_rename_at_1886_100));
}

inline void clone_23a052359607d993 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_642_101, smart_ptr_raw<TypeDecl> const  __src_rename_at_642_102 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_642_101),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_642_102),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_a8386ed128f4f0b4 ( Context * __context__, smart_ptr_raw<Enumeration> & __dest_rename_at_794_103, Enumeration * const  __src_rename_at_794_104 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_794_103),das_auto_cast<void * const >::cast(__src_rename_at_794_104),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_584104531b17a816 ( Context * __context__, char * const  __name_rename_at_967_105 )
{
    return das_auto_cast<Module *>::cast(_FuncastTickfind_moduleTick11101329256228773934_92a5dd08b29912b8(__context__,compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_967_105));
}

inline void clone_6e9cdc5c6ca1d1f3 ( Context * __context__, smart_ptr_raw<Annotation> & __dest_rename_at_873_106, smart_ptr_raw<Annotation> const  __src_rename_at_873_107 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_873_106),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_873_107),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick842554968825501494_5630d72624d0f510 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_108 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_108,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1886_109 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1886_109));
}

inline bool isVectorType_34a98665df73cc6f ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_16_110 )
{
    return das_auto_cast<bool>::cast(((((((((((((__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tFloat2)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__typ_rename_at_16_110 == DAS_COMMENT(bound_enum) das::Type::tURange64));
}

inline char * describe_670b9c27dbedaf8f ( Context * __context__, AnnotationArgumentList const  &  __list_rename_at_26_111 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_27_18_rename_at_27_112;das_zero(____acomp_27_18_rename_at_27_112);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_27_18_rename_at_27_112),false);
        {
            bool __need_loop_27 = true;
            // arg: rtti::AnnotationArgument const&
            das_iterator<AnnotationArgumentList const > __arg_iterator(__list_rename_at_26_111);
            AnnotationArgument const  * __arg_rename_at_27_113;
            __need_loop_27 = __arg_iterator.first(__context__,(__arg_rename_at_27_113)) && __need_loop_27;
            for ( ; __need_loop_27 ; __need_loop_27 = __arg_iterator.next(__context__,(__arg_rename_at_27_113)) )
            {
                AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_27_50_0; _temp_make_local_27_50_0;
                _FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f(__context__,das_arg<TArray<char *>>::pass(____acomp_27_18_rename_at_27_112),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_3, cast<das::string const  &>::from((*__arg_rename_at_27_113).name /*name*/), cast<char * const >::from(((char *) "=")), cast<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> &>::from((_temp_make_local_27_50_0 = (rtti_builtin_argument_value((*__arg_rename_at_27_113),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_27_113));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_27_18_rename_at_27_112),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_27_18_rename_at_27_112);
    })),((char *) ",")));
}

inline char * describe_19857d293154313d ( Context * __context__, AnnotationDeclaration const  &  __ann_rename_at_30_114 )
{
    return das_auto_cast<char *>::cast((das_vector_length(__ann_rename_at_30_114.arguments /*arguments*/) != 0) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_4, cast<das::string const  &>::from(__ann_rename_at_30_114.annotation /*annotation*/->name /*name*/), cast<char * const >::from(((char *) "(")), cast<char * const >::from(describe_670b9c27dbedaf8f(__context__,__ann_rename_at_30_114.arguments /*arguments*/)), cast<char * const >::from(((char *) ")"))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<das::string const  &>::from(__ann_rename_at_30_114.annotation /*annotation*/->name /*name*/)))));
}

inline char * describe_d0f1c4ce2af40b5e ( Context * __context__, AnnotationList const  &  __list_rename_at_38_115 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_39_18_rename_at_39_116;das_zero(____acomp_39_18_rename_at_39_116);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_39_18_rename_at_39_116),false);
        {
            bool __need_loop_39 = true;
            // arg: smart_ptr<rtti::AnnotationDeclaration> const&
            das_iterator<AnnotationList const > __arg_iterator(__list_rename_at_38_115);
            smart_ptr_raw<AnnotationDeclaration> const  * __arg_rename_at_39_117;
            __need_loop_39 = __arg_iterator.first(__context__,(__arg_rename_at_39_117)) && __need_loop_39;
            for ( ; __need_loop_39 ; __need_loop_39 = __arg_iterator.next(__context__,(__arg_rename_at_39_117)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49(__context__,das_arg<TArray<char *>>::pass(____acomp_39_18_rename_at_39_116),describe_19857d293154313d(__context__,das_deref(__context__,(*__arg_rename_at_39_117),__FILE__,__LINE__)));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_39_117));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_39_18_rename_at_39_116),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_39_18_rename_at_39_116);
    })),((char *) ",")));
}

inline char * describe_24ebe982465d57fc ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_42_118 )
{
    return das_auto_cast<char *>::cast((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__vvar_rename_at_42_118->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<das::string const  &>::from(__vvar_rename_at_42_118->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,__vvar_rename_at_42_118->type /*_type*/,true,true,true))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_7, cast<das::string const  &>::from(__vvar_rename_at_42_118->name /*name*/), cast<char * const >::from(((char *) ":null"))))));
}

inline bool isExpression_f1fadf50b6070b86 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_63_119, bool __top_rename_at_63_120 )
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__t_rename_at_63_119),das_auto_cast<void * const >::cast(nullptr)) )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( das_vector_length(__t_rename_at_63_119->dim /*dim*/) != 0 )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( __t_rename_at_63_119->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle )
    {
        return das_auto_cast<bool>::cast((eq_dstr_str(__t_rename_at_63_119->annotation /*annotation*/->module /*_module*/->name /*name*/,((char *) "ast"))) ? das_auto_cast<bool>::cast(builtin_string_startswith(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<das::string const  &>::from(__t_rename_at_63_119->annotation /*annotation*/->name /*name*/))),((char *) "Expr"),__context__)) : das_auto_cast<bool>::cast(false));
    } else return das_auto_cast<bool>::cast(((__t_rename_at_63_119->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer) && __top_rename_at_63_120) ? das_auto_cast<bool>::cast(isExpression_f1fadf50b6070b86(__context__,__t_rename_at_63_119->firstType /*firstType*/,false)) : das_auto_cast<bool>::cast(false));
}

inline bool is_same_or_inherited_530410b8a7a25edb ( Context * __context__, Structure const  * const  __parent_rename_at_81_121, Structure const  * const  __child_rename_at_81_122 )
{
    Structure const  * __ch_rename_at_82_123 = __child_rename_at_81_122;
    while ( __ch_rename_at_82_123 != nullptr )
    {
        if ( __parent_rename_at_81_121 == __ch_rename_at_82_123 )
        {
            return das_auto_cast<bool>::cast(true);
        } else {
            das_copy(__ch_rename_at_82_123,__ch_rename_at_82_123->parent /*parent*/);
        };
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool is_class_method_4783315be6e05a7b ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_92_124, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_92_125 )
{
    return das_auto_cast<bool>::cast((__finfo_rename_at_92_125->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tFunction) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_92_125->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_92_125->argTypes /*argTypes*/) == 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_92_125->argTypes /*argTypes*/,0,__context__)->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tStructure) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_92_125->argTypes /*argTypes*/,0,__context__)->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(is_same_or_inherited_530410b8a7a25edb(__context__,das_reinterpret<Structure const  * const >::pass(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_92_125->argTypes /*argTypes*/,0,__context__)->structType /*structType*/),das_reinterpret<Structure const  * const >::pass(_FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc(__context__,__cinfo_rename_at_92_124)))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))))))))));
}

inline void emplace_new_9bef84999dd1e3f4 ( Context * __context__, das::vector<smart_ptr<Expression>> &  __vec_rename_at_114_126, smart_ptr_raw<Expression> __ptr_rename_at_114_127 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__vec_rename_at_114_126),__ptr_rename_at_114_127);
}

inline void emplace_new_7386d366d948387e ( Context * __context__, das::vector<smart_ptr<TypeDecl>> &  __vec_rename_at_118_128, smart_ptr_raw<TypeDecl> __ptr_rename_at_118_129 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__vec_rename_at_118_128),__ptr_rename_at_118_129);
}

inline void emplace_new_2f0cb94c081aada2 ( Context * __context__, das::vector<smart_ptr<Variable>> &  __vec_rename_at_122_130, smart_ptr_raw<Variable> __ptr_rename_at_122_131 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Variable>>>::pass(__vec_rename_at_122_130),__ptr_rename_at_122_131);
}

inline void emplace_new_7782fb9c58748acd ( Context * __context__, MakeStruct &  __vec_rename_at_126_132, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_126_133 )
{
    mks_vector_emplace(das_arg<MakeStruct>::pass(__vec_rename_at_126_132),__ptr_rename_at_126_133);
}

inline bool override_method_49b0943e987177d7 ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_130_134, char * const  __name_rename_at_130_135, char * const  __funcName_rename_at_130_136 ) { das_stack_prologue __prologue(__context__,256,"override_method " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprCast> __vcast_rename_at_133_138; memset((void*)&__vcast_rename_at_133_138,0,sizeof(__vcast_rename_at_133_138));
    {
        bool __need_loop_131 = true;
        // fld: ast::FieldDeclaration&
        das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__str_rename_at_130_134->fields /*fields*/);
        Structure::FieldDeclaration * __fld_rename_at_131_137;
        __need_loop_131 = __fld_iterator.first(__context__,(__fld_rename_at_131_137)) && __need_loop_131;
        for ( ; __need_loop_131 ; __need_loop_131 = __fld_iterator.next(__context__,(__fld_rename_at_131_137)) )
        {
            if ( eq_dstr_str(das_arg<das::string>::pass((*__fld_rename_at_131_137).name /*name*/),__name_rename_at_130_135) )
            {
                /* finally */ auto __finally_132= das_finally([&](){
                das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_133_138);
                /* end finally */ });
                __vcast_rename_at_133_138; das_zero(__vcast_rename_at_133_138); das_move(__vcast_rename_at_133_138, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_133) {
                    das_copy((__mks_133.at /*at*/),(__str_rename_at_130_134->at /*at*/));
                    das_move((__mks_133.subexpr /*subexpr*/),(das_ascend_handle<true,smart_ptr_raw<ExprAddr>>::make(__context__,nullptr,(([&](ExprAddr & __mks_134) {
                        das_copy((__mks_134.at /*at*/),(__str_rename_at_130_134->at /*at*/));
                        {
                            set_das_string(das_arg<das::string>::pass(__mks_134.target /*target*/),__funcName_rename_at_130_136);
                        }                    })))));
                    das_move((__mks_133.castType /*castType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_135) {
                        das_copy((__mks_135.at /*at*/),(__str_rename_at_130_134->at /*at*/));
                        das_copy((__mks_135.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                    })))));
                }))));
                builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast((*__fld_rename_at_131_137).init /*init*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__vcast_rename_at_133_138),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                return das_auto_cast<bool>::cast(true);
            };
        }
        __fld_iterator.close(__context__,(__fld_rename_at_131_137));
    };
    return das_auto_cast<bool>::cast(false);
}}

inline void for_each_tag_function_f441072c0eb499ad ( Context * __context__, Module * const  __mod_rename_at_183_140, char * const  __tag_rename_at_183_141, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __blk_rename_at_183_142 ) { das_stack_prologue __prologue(__context__,224,"for_each_tag_function " DAS_FILE_LINE);
{
    forEachFunction(__mod_rename_at_183_140,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_184_143) -> void{
        {
            bool __need_loop_185 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_184_143->annotations /*annotations*/);
            smart_ptr_raw<AnnotationDeclaration> * __ann_rename_at_185_144;
            __need_loop_185 = __ann_iterator.first(__context__,(__ann_rename_at_185_144)) && __need_loop_185;
            for ( ; __need_loop_185 ; __need_loop_185 = __ann_iterator.next(__context__,(__ann_rename_at_185_144)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_185_144)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_185_144)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_187 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_185_144)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_187_145;
                        __need_loop_187 = __t_iterator.first(__context__,(__t_rename_at_187_145)) && __need_loop_187;
                        for ( ; __need_loop_187 ; __need_loop_187 = __t_iterator.next(__context__,(__t_rename_at_187_145)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_187_145).name /*name*/),__tag_rename_at_183_141) )
                            {
                                das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__blk_rename_at_183_142,__func_rename_at_184_143);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_187_145));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_185_144));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    forEachGenericFunction(__mod_rename_at_183_140,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_195_146) -> void{
        {
            bool __need_loop_196 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_195_146->annotations /*annotations*/);
            smart_ptr_raw<AnnotationDeclaration> * __ann_rename_at_196_147;
            __need_loop_196 = __ann_iterator.first(__context__,(__ann_rename_at_196_147)) && __need_loop_196;
            for ( ; __need_loop_196 ; __need_loop_196 = __ann_iterator.next(__context__,(__ann_rename_at_196_147)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_196_147)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_196_147)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_198 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_196_147)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_198_148;
                        __need_loop_198 = __t_iterator.first(__context__,(__t_rename_at_198_148)) && __need_loop_198;
                        for ( ; __need_loop_198 ; __need_loop_198 = __t_iterator.next(__context__,(__t_rename_at_198_148)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_198_148).name /*name*/),__tag_rename_at_183_141) )
                            {
                                das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__blk_rename_at_183_142,__func_rename_at_195_146);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_198_148));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_196_147));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_a0e87cbd443f10f5 ( Context * __context__, char * const  __argn_rename_at_209_149, AnnotationArgumentList const  &  __args_rename_at_209_150 )
{
    {
        bool __need_loop_210 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_209_150);
        AnnotationArgument const  * __a_rename_at_210_151;
        __need_loop_210 = __a_iterator.first(__context__,(__a_rename_at_210_151)) && __need_loop_210;
        for ( ; __need_loop_210 ; __need_loop_210 = __a_iterator.next(__context__,(__a_rename_at_210_151)) )
        {
            if ( eq_dstr_str((*__a_rename_at_210_151).name /*name*/,__argn_rename_at_209_149) )
            {
                return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(rtti_builtin_argument_value((*__a_rename_at_210_151),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_210_151));
    };
    return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8(__context__));
}

inline AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_eefed45a6ebf3b9 ( Context * __context__, AnnotationArgumentList const  &  __args_rename_at_218_152, char * const  __argn_rename_at_218_153 )
{
    {
        bool __need_loop_219 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_218_152);
        AnnotationArgument const  * __a_rename_at_219_154;
        __need_loop_219 = __a_iterator.first(__context__,(__a_rename_at_219_154)) && __need_loop_219;
        for ( ; __need_loop_219 ; __need_loop_219 = __a_iterator.next(__context__,(__a_rename_at_219_154)) )
        {
            if ( eq_dstr_str((*__a_rename_at_219_154).name /*name*/,__argn_rename_at_218_153) )
            {
                return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(rtti_builtin_argument_value((*__a_rename_at_219_154),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_219_154));
    };
    return das_auto_cast_ref<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8(__context__));
}

inline void apply_tag_annotation_1944b9a89fd11326 ( Context * __context__, char * const  __tag_rename_at_227_155, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_227_156 ) { das_stack_prologue __prologue(__context__,128,"apply_tag_annotation " DAS_FILE_LINE);
{
    TArray<smart_ptr_raw<Function>> __funcs_rename_at_228_157; memset((void*)&__funcs_rename_at_228_157,0,sizeof(__funcs_rename_at_228_157));
    smart_ptr_raw<FunctionAnnotation> __tagAnn_rename_at_234_160; memset((void*)&__tagAnn_rename_at_234_160,0,sizeof(__tagAnn_rename_at_234_160));
    /* finally */ auto __finally_227= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_228_157));
    /* end finally */ });
    das_zero(__funcs_rename_at_228_157);
    for_each_tag_function_f441072c0eb499ad(__context__,thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__tag_rename_at_227_155,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__806bd08fbfbd09f4,[&](smart_ptr_raw<Function> __func_rename_at_229_158) -> void{
        _FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_228_157),__func_rename_at_229_158);
    }));
    {
        bool __need_loop_232 = true;
        // func: smart_ptr<ast::Function> aka FunctionPtr&
        das_iterator<TArray<smart_ptr_raw<Function>>> __func_iterator(__funcs_rename_at_228_157);
        smart_ptr_raw<Function> * __func_rename_at_232_159;
        __need_loop_232 = __func_iterator.first(__context__,(__func_rename_at_232_159)) && __need_loop_232;
        for ( ; __need_loop_232 ; __need_loop_232 = __func_iterator.next(__context__,(__func_rename_at_232_159)) )
        {
            {
                /* finally */ auto __finally_233= das_finally([&](){
                das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__tagAnn_rename_at_234_160);
                /* end finally */ });
                __tagAnn_rename_at_234_160; das_zero(__tagAnn_rename_at_234_160); das_move(__tagAnn_rename_at_234_160, _FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0(__context__,__ann_rename_at_227_156));
                addFunctionFunctionAnnotation((*__func_rename_at_232_159),__tagAnn_rename_at_234_160,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __func_iterator.close(__context__,(__func_rename_at_232_159));
    };
}}

inline smart_ptr_raw<Function> find_unique_function_e26f474c35febe1d ( Context * __context__, Module * const  __mod_rename_at_246_161, char * const  __name_rename_at_246_162, bool __canfail_rename_at_246_163 ) { das_stack_prologue __prologue(__context__,128,"find_unique_function " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_247_164; memset((void*)&__res_rename_at_247_164,0,sizeof(__res_rename_at_247_164));
    int32_t __count_rename_at_248_165; memset((void*)&__count_rename_at_248_165,0,sizeof(__count_rename_at_248_165));
    /* finally */ auto __finally_246= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_247_164);
    /* end finally */ });
    das_zero(__res_rename_at_247_164);
    __count_rename_at_248_165 = 0;
    forEachFunction(__mod_rename_at_246_161,__name_rename_at_246_162,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_249_166) -> void{
        clone_e5dbb6c9a183080e(__context__,__res_rename_at_247_164,__func_rename_at_249_166);
        ++__count_rename_at_248_165;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_248_165 > 1 )
    {
        if ( !__canfail_rename_at_246_163 )
        {
            builtin_throw(((char *) "more than one function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_247_164);
    };
}}

inline smart_ptr_raw<Function> find_unique_generic_22c96d795ba15b85 ( Context * __context__, Module * const  __mod_rename_at_262_167, char * const  __name_rename_at_262_168, bool __canfail_rename_at_262_169 ) { das_stack_prologue __prologue(__context__,128,"find_unique_generic " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_263_170; memset((void*)&__res_rename_at_263_170,0,sizeof(__res_rename_at_263_170));
    int32_t __count_rename_at_264_171; memset((void*)&__count_rename_at_264_171,0,sizeof(__count_rename_at_264_171));
    /* finally */ auto __finally_262= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_263_170);
    /* end finally */ });
    das_zero(__res_rename_at_263_170);
    __count_rename_at_264_171 = 0;
    forEachGenericFunction(__mod_rename_at_262_167,__name_rename_at_262_168,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_265_172) -> void{
        clone_e5dbb6c9a183080e(__context__,__res_rename_at_263_170,__func_rename_at_265_172);
        ++__count_rename_at_264_171;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_264_171 > 1 )
    {
        if ( !__canfail_rename_at_262_169 )
        {
            builtin_throw(((char *) "more than one generic function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_263_170);
    };
}}

inline ExprBlock * setup_call_list_329404b139e4adba ( Context * __context__, char * const  __name_rename_at_278_173, LineInfo const  &  __at_rename_at_278_174, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __subblock_rename_at_278_175 ) { das_stack_prologue __prologue(__context__,144,"setup_call_list " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_279_176; memset((void*)&__fn_rename_at_279_176,0,sizeof(__fn_rename_at_279_176));
    smart_ptr_raw<ExprBlock> __blk_rename_at_285_178; memset((void*)&__blk_rename_at_285_178,0,sizeof(__blk_rename_at_285_178));
    ExprBlock * __res_rename_at_286_179; memset((void*)&__res_rename_at_286_179,0,sizeof(__res_rename_at_286_179));
    /* finally */ auto __finally_278= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_279_176);
    /* end finally */ });
    __fn_rename_at_279_176; das_zero(__fn_rename_at_279_176); das_move(__fn_rename_at_279_176, find_unique_function_e26f474c35febe1d(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_278_173,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_279_176),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_280= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_285_178);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_279_176),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_281) {
            das_copy((__mks_281.at /*at*/),(__at_rename_at_278_174));
            das_copy((__mks_281.atDecl /*atDecl*/),(__at_rename_at_278_174));
            das_copy((__mks_281.flags /*flags*/),(0x200000u));
            das_move((__mks_281.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_283) {
                das_copy((__mks_283.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_283.at /*at*/),(__at_rename_at_278_174));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_281.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_9, cast<char * const >::from(__name_rename_at_278_173))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_285_178; das_zero(__blk_rename_at_285_178); das_move(__blk_rename_at_285_178, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_285) {
            das_copy((__mks_285.at /*at*/),(__at_rename_at_278_174));
        }))));
        __res_rename_at_286_179 = _FuncbuiltinTickget_ptrTick5807679485210906136_eb77b9b7854f344(__context__,__blk_rename_at_285_178);
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_279_176->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_285_178),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__subblock_rename_at_278_175,__fn_rename_at_279_176);
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_279_176,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_10, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_279_176->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(__res_rename_at_286_179);
    } else if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_279_176->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
    {
        builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__fn_rename_at_279_176->body /*body*/));
}}

inline ExprBlock * setup_call_list_8438ddab4dff539b ( Context * __context__, char * const  __name_rename_at_299_180, LineInfo const  &  __at_rename_at_299_181, bool __isInit_rename_at_299_182, bool __isPrivate_rename_at_299_183, bool __isLateInit_rename_at_299_184 ) { das_stack_prologue __prologue(__context__,96,"setup_call_list " DAS_FILE_LINE);
{
    return das_auto_cast<ExprBlock *>::cast(setup_call_list_329404b139e4adba(__context__,__name_rename_at_299_180,__at_rename_at_299_181,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__3b2c4edb933bef4a,[&](smart_ptr_raw<Function> __fn_rename_at_300_185) -> void{
        if ( __isInit_rename_at_299_182 )
        {
            __fn_rename_at_300_185->flags /*flags*/ |= 0x100u;
            if ( __isLateInit_rename_at_299_184 )
            {
                __fn_rename_at_300_185->moreFlags /*moreFlags*/ |= 0x8u;
            };
        };
        if ( __isPrivate_rename_at_299_183 )
        {
            __fn_rename_at_300_185->flags /*flags*/ |= 0x400000u;
        };
    })));
}}

inline ExprBlock * setup_macro_979cb6d902caaebd ( Context * __context__, char * const  __name_rename_at_313_186, LineInfo const  &  __at_rename_at_313_187 ) { das_stack_prologue __prologue(__context__,208,"setup_macro " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_315_188; memset((void*)&__fn_rename_at_315_188,0,sizeof(__fn_rename_at_315_188));
    smart_ptr_raw<ExprBlock> __blk_rename_at_321_190; memset((void*)&__blk_rename_at_321_190,0,sizeof(__blk_rename_at_321_190));
    smart_ptr_raw<ExprBlock> __iblk_rename_at_322_191; memset((void*)&__iblk_rename_at_322_191,0,sizeof(__iblk_rename_at_322_191));
    ExprBlock * __res_rename_at_323_192; memset((void*)&__res_rename_at_323_192,0,sizeof(__res_rename_at_323_192));
    smart_ptr_raw<ExprCall> __ifm_rename_at_324_193; memset((void*)&__ifm_rename_at_324_193,0,sizeof(__ifm_rename_at_324_193));
    smart_ptr_raw<ExprConstString> __ifmn_rename_at_325_195; memset((void*)&__ifmn_rename_at_325_195,0,sizeof(__ifmn_rename_at_325_195));
    smart_ptr_raw<ExprIfThenElse> __ife_rename_at_327_197; memset((void*)&__ife_rename_at_327_197,0,sizeof(__ife_rename_at_327_197));
    ExprBlock * __blk_rename_at_339_198; memset((void*)&__blk_rename_at_339_198,0,sizeof(__blk_rename_at_339_198));
    ExprIfThenElse * __ife_rename_at_343_199; memset((void*)&__ife_rename_at_343_199,0,sizeof(__ife_rename_at_343_199));
    /* finally */ auto __finally_313= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_315_188);
    /* end finally */ });
    compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->flags /*flags*/ |= 0x20u;
    __fn_rename_at_315_188; das_zero(__fn_rename_at_315_188); das_move(__fn_rename_at_315_188, find_unique_function_e26f474c35febe1d(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_313_186,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_315_188),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_316= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprIfThenElse>>::clear(__context__,__ife_rename_at_327_197);
        das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__ifmn_rename_at_325_195);
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__ifm_rename_at_324_193);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__iblk_rename_at_322_191);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_321_190);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_315_188),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_317) {
            das_copy((__mks_317.at /*at*/),(__at_rename_at_313_187));
            das_copy((__mks_317.atDecl /*atDecl*/),(__at_rename_at_313_187));
            das_copy((__mks_317.flags /*flags*/),(0x80600000u));
            das_move((__mks_317.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_319) {
                das_copy((__mks_319.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_319.at /*at*/),(__at_rename_at_313_187));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_317.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_9, cast<char * const >::from(__name_rename_at_313_186))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_321_190; das_zero(__blk_rename_at_321_190); das_move(__blk_rename_at_321_190, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_321) {
            das_copy((__mks_321.at /*at*/),(__at_rename_at_313_187));
        }))));
        __iblk_rename_at_322_191; das_zero(__iblk_rename_at_322_191); das_move(__iblk_rename_at_322_191, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_322) {
            das_copy((__mks_322.at /*at*/),(__at_rename_at_313_187));
        }))));
        __res_rename_at_323_192 = ((ExprBlock *)_FuncbuiltinTickget_ptrTick5807679485210906136_eb77b9b7854f344(__context__,__iblk_rename_at_322_191));
        __ifm_rename_at_324_193; das_zero(__ifm_rename_at_324_193); das_move(__ifm_rename_at_324_193, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_324) {
            das_copy((__mks_324.at /*at*/),(__at_rename_at_313_187));
            {
                set_das_string(das_arg<das::string>::pass(__mks_324.name /*name*/),((char *) "is_compiling_macros_in_module"));
            }        }))));
        __ifmn_rename_at_325_195; das_zero(__ifmn_rename_at_325_195); das_move(__ifmn_rename_at_325_195, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_325) {
            das_copy((__mks_325.at /*at*/),(__at_rename_at_313_187));
            {
                das_clone<das::string,das::string>::clone(__mks_325.text /*value*/,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->name /*name*/);
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__ifm_rename_at_324_193->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__ifmn_rename_at_325_195));
        __ife_rename_at_327_197; das_zero(__ife_rename_at_327_197); das_move(__ife_rename_at_327_197, das_ascend_handle<true,smart_ptr_raw<ExprIfThenElse>>::make(__context__,nullptr,(([&](ExprIfThenElse & __mks_327) {
            das_copy((__mks_327.at /*at*/),(__at_rename_at_313_187));
            das_move((__mks_327.cond /*cond*/),(__ifm_rename_at_324_193));
            das_move((__mks_327.if_true /*if_true*/),(__iblk_rename_at_322_191));
        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_321_190->list /*list*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__ife_rename_at_327_197));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_315_188->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_321_190),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_315_188,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_10, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_315_188->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__res_rename_at_323_192));
    } else {
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_315_188->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __blk_rename_at_339_198 = das_cast<ExprBlock *>::cast(__fn_rename_at_315_188->body /*body*/);
        if ( (das_vector_length(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_339_198->list /*list*/)) != 1) || (SimPolicy<char *>::NotEqu(cast<char *>::from(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_339_198->list /*list*/,0,__context__)->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprIfThenElse")),*__context__,nullptr)) )
        {
            builtin_throw(((char *) "expecting is_compiling_macros"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __ife_rename_at_343_199 = das_cast<ExprIfThenElse *>::cast(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_339_198->list /*list*/,0,__context__));
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__ife_rename_at_343_199->if_true /*if_true*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting ife.if_true.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__ife_rename_at_343_199->if_true /*if_true*/));
    };
}}

inline void * panic_expr_as_2ea20f674c842c9c ( Context * __context__ )
{
    builtin_throw(((char *) "invalid 'as' expression or null pointer dereference"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast<void *>::cast(nullptr);
}

inline bool _FuncTickisTickBuiltInFunction_3d26f4806589821b ( Context * __context__, Function * const  __foo_rename_at_553_200 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__foo_rename_at_553_200->flags /*flags*/,1u << 0));
}

inline BuiltInFunction * _FuncTickasTickBuiltInFunction_c47ad3b03c9bc354 ( Context * __context__, Function * const  __foo_rename_at_557_201 )
{
    DAS_VERIFY((_FuncTickisTickBuiltInFunction_3d26f4806589821b(__context__,__foo_rename_at_557_201)));
    return das_auto_cast<BuiltInFunction *>::cast(das_cast<BuiltInFunction *>::cast(__foo_rename_at_557_201));
}

inline bool _FuncTickisTickExternalFnBase_24b69557e6b14161 ( Context * __context__, Function * const  __foo_rename_at_566_202 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__foo_rename_at_566_202->moreFlags /*moreFlags*/,1u << 12) && das_get_bitfield(__foo_rename_at_566_202->flags /*flags*/,1u << 0));
}

inline ExternalFnBase * _FuncTickasTickExternalFnBase_b840ea32468fb731 ( Context * __context__, Function * const  __foo_rename_at_570_203 )
{
    DAS_VERIFY((_FuncTickisTickExternalFnBase_24b69557e6b14161(__context__,__foo_rename_at_570_203)));
    return das_auto_cast<ExternalFnBase *>::cast(das_cast<ExternalFnBase *>::cast(__foo_rename_at_570_203));
}

inline bool _FuncTickisTickFunctionAnnotation_bf985c1f9a8f8112 ( Context * __context__, Annotation * const  __foo_rename_at_579_204 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_579_204,__FILE__,__LINE__)).rtti_isFunctionAnnotation()));
}

inline bool _FuncTickisTickFunctionAnnotation_88a1558057f6b81f ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_583_205 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_583_205,__FILE__,__LINE__)).rtti_isFunctionAnnotation()));
}

inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_8812613c169a9dba ( Context * __context__, Annotation * const  __foo_rename_at_587_206 )
{
    DAS_VERIFY((_FuncTickisTickFunctionAnnotation_bf985c1f9a8f8112(__context__,__foo_rename_at_587_206)));
    return das_auto_cast<FunctionAnnotation *>::cast(das_cast<FunctionAnnotation *>::cast(__foo_rename_at_587_206));
}

inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_89843e969c415f46 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_592_207 )
{
    DAS_VERIFY((_FuncTickisTickFunctionAnnotation_88a1558057f6b81f(__context__,__foo_rename_at_592_207)));
    return das_auto_cast<FunctionAnnotation *>::cast(das_cast<FunctionAnnotation *>::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_49b21ea86f1f3e71(__context__,__foo_rename_at_592_207)));
}

inline bool _FuncTickisTickStructureAnnotation_a9f69a8ea44dcd1b ( Context * __context__, Annotation * const  __foo_rename_at_602_208 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_602_208,__FILE__,__LINE__)).rtti_isStructureAnnotation()));
}

inline bool _FuncTickisTickStructureAnnotation_25f742c66283203c ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_606_209 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_606_209,__FILE__,__LINE__)).rtti_isStructureAnnotation()));
}

inline StructureAnnotation * _FuncTickasTickStructureAnnotation_dac337e2d083c291 ( Context * __context__, Annotation * const  __foo_rename_at_610_210 )
{
    DAS_VERIFY((_FuncTickisTickStructureAnnotation_a9f69a8ea44dcd1b(__context__,__foo_rename_at_610_210)));
    return das_auto_cast<StructureAnnotation *>::cast(das_cast<StructureAnnotation *>::cast(__foo_rename_at_610_210));
}

inline StructureAnnotation * _FuncTickasTickStructureAnnotation_b43078c41d1f685 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_615_211 )
{
    DAS_VERIFY((_FuncTickisTickStructureAnnotation_25f742c66283203c(__context__,__foo_rename_at_615_211)));
    return das_auto_cast<StructureAnnotation *>::cast(das_cast<StructureAnnotation *>::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_49b21ea86f1f3e71(__context__,__foo_rename_at_615_211)));
}

inline smart_ptr_raw<Expression> walk_and_convert_array_ba1cc70e35611268 ( Context * __context__, uint8_t const  * const  __data_rename_at_620_212, smart_ptr_raw<TypeDecl> const  __info_rename_at_620_213, LineInfo const  &  __at_rename_at_620_214 ) { das_stack_prologue __prologue(__context__,176,"walk_and_convert_array " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_623_216; memset((void*)&__stride_rename_at_623_216,0,sizeof(__stride_rename_at_623_216));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_624_217; memset((void*)&__mkArr_rename_at_624_217,0,sizeof(__mkArr_rename_at_624_217));
    smart_ptr_raw<ExprCall> __mkToArrayMove_rename_at_629_219; memset((void*)&__mkToArrayMove_rename_at_629_219,0,sizeof(__mkToArrayMove_rename_at_629_219));
    int32_t __total_rename_at_621_215 = ((int32_t)any_array_size(das_auto_cast<void * const >::cast(__data_rename_at_620_212)));
    if ( __total_rename_at_621_215 != 0 )
    {
        /* finally */ auto __finally_622= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToArrayMove_rename_at_629_219);
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_624_217);
        /* end finally */ });
        __stride_rename_at_623_216 = ((int32_t)((das_deref(__context__,__info_rename_at_620_213->firstType /*firstType*/,__FILE__,__LINE__)).getSizeOf()));
        __mkArr_rename_at_624_217; das_zero(__mkArr_rename_at_624_217); das_move(__mkArr_rename_at_624_217, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_624) {
            das_copy((__mks_624.at /*at*/),(__at_rename_at_620_214));
            das_move((__mks_624.makeType /*makeType*/),(clone_type(__info_rename_at_620_213->firstType /*firstType*/)));
        }))));
        any_array_foreach(das_auto_cast<void * const >::cast(__data_rename_at_620_212),__stride_rename_at_623_216,das_make_block<void,void *>(__context__,128,0,&__func_info__b28b606ddb7a6760,[&](void * __value_rename_at_625_218) -> void{
            emplace_new_9bef84999dd1e3f4(__context__,das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_624_217->values /*values*/),walk_and_convert_cfdd8313a6a5987(__context__,das_auto_cast<uint8_t const  * const >::cast(__value_rename_at_625_218),__info_rename_at_620_213->firstType /*firstType*/,__at_rename_at_620_214));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_624_217->makeType /*makeType*/->dim /*dim*/),__total_rename_at_621_215);
        __mkToArrayMove_rename_at_629_219; das_zero(__mkToArrayMove_rename_at_629_219); das_move(__mkToArrayMove_rename_at_629_219, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_629) {
            das_copy((__mks_629.at /*at*/),(__at_rename_at_620_214));
            {
                set_das_string(das_arg<das::string>::pass(__mks_629.name /*name*/),((char *) "to_array_move"));
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToArrayMove_rename_at_629_219->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkArr_rename_at_624_217));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkToArrayMove_rename_at_629_219);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_633) {
            das_copy((__mks_633.at /*at*/),(__at_rename_at_620_214));
            das_move((__mks_633.makeType /*makeType*/),(clone_type(__info_rename_at_620_213)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_dim_9001b9148a98cff6 ( Context * __context__, uint8_t const  * const  __data_rename_at_637_221, smart_ptr_raw<TypeDecl> const  __info_rename_at_637_222, LineInfo const  &  __at_rename_at_637_223 ) { das_stack_prologue __prologue(__context__,176,"walk_and_convert_dim " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_638_224; memset((void*)&__stride_rename_at_638_224,0,sizeof(__stride_rename_at_638_224));
    int32_t __total_rename_at_639_225; memset((void*)&__total_rename_at_639_225,0,sizeof(__total_rename_at_639_225));
    smart_ptr_raw<TypeDecl> __einfo_rename_at_640_226; memset((void*)&__einfo_rename_at_640_226,0,sizeof(__einfo_rename_at_640_226));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_642_227; memset((void*)&__mkArr_rename_at_642_227,0,sizeof(__mkArr_rename_at_642_227));
    smart_ptr_raw<Expression> __elem_rename_at_645_230; memset((void*)&__elem_rename_at_645_230,0,sizeof(__elem_rename_at_645_230));
    /* finally */ auto __finally_637= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_642_227);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__einfo_rename_at_640_226);
    /* end finally */ });
    __stride_rename_at_638_224 = ((int32_t)((das_deref(__context__,__info_rename_at_637_222,__FILE__,__LINE__)).getBaseSizeOf()));
    __total_rename_at_639_225 = ((int32_t)((das_deref(__context__,__info_rename_at_637_222,__FILE__,__LINE__)).getCountOf()));
    __einfo_rename_at_640_226; das_zero(__einfo_rename_at_640_226); das_move(__einfo_rename_at_640_226, clone_type(__info_rename_at_637_222));
    das_vector_clear(das_arg<das::vector<int32_t>>::pass(__einfo_rename_at_640_226->dim /*dim*/));
    __mkArr_rename_at_642_227; das_zero(__mkArr_rename_at_642_227); das_move(__mkArr_rename_at_642_227, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_642) {
        das_copy((__mks_642.at /*at*/),(__at_rename_at_637_223));
        {
            clone_23a052359607d993(__context__,__mks_642.makeType /*makeType*/,__info_rename_at_637_222);
        }    }))));
    {
        bool __need_loop_643 = true;
        // x: int const
        das_iterator<range> __x_iterator(mk_range(__total_rename_at_639_225));
        int32_t __x_rename_at_643_229;
        __need_loop_643 = __x_iterator.first(__context__,(__x_rename_at_643_229)) && __need_loop_643;
        for ( ; __need_loop_643 ; __need_loop_643 = __x_iterator.next(__context__,(__x_rename_at_643_229)) )
        {
            __elem_rename_at_645_230; das_zero(__elem_rename_at_645_230); das_move(__elem_rename_at_645_230, walk_and_convert_cfdd8313a6a5987(__context__,das_ptr_add_int32(__data_rename_at_637_221,__stride_rename_at_638_224 * __x_rename_at_643_229,1),__einfo_rename_at_640_226,__at_rename_at_637_223));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_642_227->values /*values*/),__elem_rename_at_645_230);
        }
        __x_iterator.close(__context__,(__x_rename_at_643_229));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_642_227);
}}

inline smart_ptr_raw<Expression> walk_and_convert_pointer_47b794d0dd59337d ( Context * __context__, uint8_t const  * const  __data_rename_at_652_231, smart_ptr_raw<TypeDecl> const  __info_rename_at_652_232, LineInfo const  &  __at_rename_at_652_233 )
{
    uint8_t const  * __pdata_rename_at_654_234 = ((uint8_t const  *)das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_652_231),__FILE__,__LINE__));
    smart_ptr_raw<Expression> __elem_rename_at_655_235; das_zero(__elem_rename_at_655_235); das_move(__elem_rename_at_655_235, walk_and_convert_cfdd8313a6a5987(__context__,__pdata_rename_at_654_234,__info_rename_at_652_232->firstType /*firstType*/,__at_rename_at_652_233));
    smart_ptr_raw<ExprAscend> __mkAsc_rename_at_656_236; das_zero(__mkAsc_rename_at_656_236); das_move(__mkAsc_rename_at_656_236, das_ascend_handle<true,smart_ptr_raw<ExprAscend>>::make(__context__,nullptr,(([&](ExprAscend & __mks_656) {
        das_copy((__mks_656.at /*at*/),(__at_rename_at_652_233));
        das_move((__mks_656.subexpr /*subexpr*/),(__elem_rename_at_655_235));
    }))));
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkAsc_rename_at_656_236);
}

inline smart_ptr_raw<Expression> walk_and_convert_tuple_9ce3b13721ba5daf ( Context * __context__, uint8_t const  * const  __data_rename_at_661_237, smart_ptr_raw<TypeDecl> const  __info_rename_at_661_238, LineInfo const  &  __at_rename_at_661_239 )
{
    smart_ptr_raw<ExprMakeTuple> __mkArr_rename_at_662_240; memset((void*)&__mkArr_rename_at_662_240,0,sizeof(__mkArr_rename_at_662_240));
    int32_t __offset_rename_at_664_242; memset((void*)&__offset_rename_at_664_242,0,sizeof(__offset_rename_at_664_242));
    smart_ptr_raw<Expression> __elem_rename_at_666_243; memset((void*)&__elem_rename_at_666_243,0,sizeof(__elem_rename_at_666_243));
    /* finally */ auto __finally_661= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkArr_rename_at_662_240);
    /* end finally */ });
    __mkArr_rename_at_662_240; das_zero(__mkArr_rename_at_662_240); das_move(__mkArr_rename_at_662_240, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_662) {
        das_copy((__mks_662.at /*at*/),(__at_rename_at_661_239));
        das_move((__mks_662.recordType /*recordType*/),(clone_type(__info_rename_at_661_238)));
    }))));
    {
        bool __need_loop_663 = true;
        // idx: int const
        das_iterator<range> __idx_iterator(mk_range(das_vector_length(__info_rename_at_661_238->argTypes /*argTypes*/)));
        int32_t __idx_rename_at_663_241;
        __need_loop_663 = __idx_iterator.first(__context__,(__idx_rename_at_663_241)) && __need_loop_663;
        for ( ; __need_loop_663 ; __need_loop_663 = __idx_iterator.next(__context__,(__idx_rename_at_663_241)) )
        {
            __offset_rename_at_664_242 = ((int32_t)get_tuple_field_offset(__info_rename_at_661_238,__idx_rename_at_663_241,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            __elem_rename_at_666_243; das_zero(__elem_rename_at_666_243); das_move(__elem_rename_at_666_243, walk_and_convert_cfdd8313a6a5987(__context__,das_ptr_add_int32(__data_rename_at_661_237,__offset_rename_at_664_242,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_661_238->argTypes /*argTypes*/,__idx_rename_at_663_241,__context__),__at_rename_at_661_239));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_662_240->values /*values*/),__elem_rename_at_666_243);
        }
        __idx_iterator.close(__context__,(__idx_rename_at_663_241));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_662_240);
}

inline smart_ptr_raw<Expression> walk_and_convert_structure_106d0a1bf460f63c ( Context * __context__, uint8_t const  * const  __data_rename_at_673_244, smart_ptr_raw<TypeDecl> const  __info_rename_at_673_245, LineInfo const  &  __at_rename_at_673_246 ) { das_stack_prologue __prologue(__context__,352,"walk_and_convert_structure " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprMakeStruct> __mkStruct_rename_at_674_247; memset((void*)&__mkStruct_rename_at_674_247,0,sizeof(__mkStruct_rename_at_674_247));
    Structure * __stype_rename_at_675_248; memset((void*)&__stype_rename_at_675_248,0,sizeof(__stype_rename_at_675_248));
    smart_ptr_raw<MakeStruct> __mkS_rename_at_676_249; memset((void*)&__mkS_rename_at_676_249,0,sizeof(__mkS_rename_at_676_249));
    uint8_t const  * __fdata_rename_at_680_251; memset((void*)&__fdata_rename_at_680_251,0,sizeof(__fdata_rename_at_680_251));
    smart_ptr_raw<Expression> __elem_rename_at_681_252; memset((void*)&__elem_rename_at_681_252,0,sizeof(__elem_rename_at_681_252));
    smart_ptr_raw<MakeFieldDecl> __mkF_rename_at_682_253; memset((void*)&__mkF_rename_at_682_253,0,sizeof(__mkF_rename_at_682_253));
    /* finally */ auto __finally_673= das_finally([&](){
    das_delete_handle<smart_ptr_raw<MakeStruct>>::clear(__context__,__mkS_rename_at_676_249);
    das_delete_handle<smart_ptr_raw<ExprMakeStruct>>::clear(__context__,__mkStruct_rename_at_674_247);
    /* end finally */ });
    __mkStruct_rename_at_674_247; das_zero(__mkStruct_rename_at_674_247); das_move(__mkStruct_rename_at_674_247, das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_674) {
        das_copy((__mks_674.at /*at*/),(__at_rename_at_673_246));
        das_move((__mks_674.makeType /*makeType*/),(clone_type(__info_rename_at_673_245)));
    }))));
    __stype_rename_at_675_248 = ((Structure *)__info_rename_at_673_245->structType /*structType*/);
    __mkS_rename_at_676_249; das_zero(__mkS_rename_at_676_249); das_move(__mkS_rename_at_676_249, das_new_handle<MakeStruct,true>::make(__context__));
    {
        bool __need_loop_677 = true;
        // x: ast::FieldDeclaration const&
        das_iterator<das::vector<Structure::FieldDeclaration> const > __x_iterator(__stype_rename_at_675_248->fields /*fields*/);
        Structure::FieldDeclaration const  * __x_rename_at_677_250;
        __need_loop_677 = __x_iterator.first(__context__,(__x_rename_at_677_250)) && __need_loop_677;
        for ( ; __need_loop_677 ; __need_loop_677 = __x_iterator.next(__context__,(__x_rename_at_677_250)) )
        {
            AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_678_28_1; _temp_make_local_678_28_1;
            if ( das_get_auto_variant_field<vec4f,8,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((_temp_make_local_678_28_1 = (find_arg_eefed45a6ebf3b9(__context__,(*__x_rename_at_677_250).annotation /*annotation*/,((char *) "do_not_convert"))))) )
            {
                __fdata_rename_at_680_251 = ((uint8_t const  *)das_ptr_add_int32(__data_rename_at_673_244,(*__x_rename_at_677_250).offset /*offset*/,1));
                __elem_rename_at_681_252; das_zero(__elem_rename_at_681_252); das_move(__elem_rename_at_681_252, walk_and_convert_cfdd8313a6a5987(__context__,__fdata_rename_at_680_251,(*__x_rename_at_677_250).type /*_type*/,__at_rename_at_673_246));
                __mkF_rename_at_682_253; das_zero(__mkF_rename_at_682_253); das_move(__mkF_rename_at_682_253, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_682) {
                    das_copy((__mks_682.at /*at*/),(__at_rename_at_673_246));
                    das_move((__mks_682.value /*value*/),(__elem_rename_at_681_252));
                    {
                        das_clone<das::string,das::string>::clone(__mks_682.name /*name*/,(*__x_rename_at_677_250).name /*name*/);
                    }                }))));
                if ( !((das_deref(__context__,(*__x_rename_at_677_250).type /*_type*/,__FILE__,__LINE__)).canCopy()) )
                {
                    das_copy(__mkF_rename_at_682_253->flags /*flags*/,0x1u);
                };
                emplace_new_7782fb9c58748acd(__context__,das_arg<MakeStruct>::pass(das_deref(__context__,__mkS_rename_at_676_249,__FILE__,__LINE__)),__mkF_rename_at_682_253);
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_677_250));
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeStruct>>>::pass(__mkStruct_rename_at_674_247->structs /*structs*/),__mkS_rename_at_676_249);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkStruct_rename_at_674_247);
}}

inline smart_ptr_raw<Expression> walk_and_convert_variant_cdeba207e46113d9 ( Context * __context__, uint8_t const  * const  __data_rename_at_694_255, smart_ptr_raw<TypeDecl> const  __info_rename_at_694_256, LineInfo const  &  __at_rename_at_694_257 ) { das_stack_prologue __prologue(__context__,192,"walk_and_convert_variant " DAS_FILE_LINE);
{
    int32_t __vindex_rename_at_696_258 = ((int32_t)das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_694_255),__FILE__,__LINE__));
    int32_t __offset_rename_at_697_259 = ((int32_t)get_variant_field_offset(__info_rename_at_694_256,__vindex_rename_at_696_258,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    smart_ptr_raw<ExprMakeVariant> __mkVariant_rename_at_698_260; das_zero(__mkVariant_rename_at_698_260); das_move(__mkVariant_rename_at_698_260, das_ascend_handle<true,smart_ptr_raw<ExprMakeVariant>>::make(__context__,nullptr,(([&](ExprMakeVariant & __mks_698) {
        das_copy((__mks_698.at /*at*/),(__at_rename_at_694_257));
        das_move((__mks_698.makeType /*makeType*/),(clone_type(__info_rename_at_694_256)));
    }))));
    smart_ptr_raw<Expression> __elem_rename_at_699_261; das_zero(__elem_rename_at_699_261); das_move(__elem_rename_at_699_261, walk_and_convert_cfdd8313a6a5987(__context__,das_ptr_add_int32(__data_rename_at_694_255,__offset_rename_at_697_259,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_694_256->argTypes /*argTypes*/,__vindex_rename_at_696_258,__context__),__at_rename_at_694_257));
    smart_ptr_raw<MakeFieldDecl> __mkS_rename_at_700_262; das_zero(__mkS_rename_at_700_262); das_move(__mkS_rename_at_700_262, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_700) {
        das_copy((__mks_700.at /*at*/),(__at_rename_at_694_257));
        das_move((__mks_700.value /*value*/),(__elem_rename_at_699_261));
        {
            das_clone<das::string,das::string>::clone(__mks_700.name /*name*/,das_index<das::vector<das::string> const >::at(__info_rename_at_694_256->argNames /*argNames*/,__vindex_rename_at_696_258,__context__));
        }    }))));
    if ( !((das_deref(__context__,das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_694_256->argTypes /*argTypes*/,__vindex_rename_at_696_258,__context__),__FILE__,__LINE__)).canCopy()) )
    {
        das_copy(__mkS_rename_at_700_262->flags /*flags*/,0x1u);
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeFieldDecl>>>::pass(__mkVariant_rename_at_698_260->variants /*variants*/),__mkS_rename_at_700_262);
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkVariant_rename_at_698_260);
}}

inline smart_ptr_raw<Expression> walk_and_convert_table_bafed583a64bc3a1 ( Context * __context__, uint8_t const  * const  __data_rename_at_709_264, smart_ptr_raw<TypeDecl> const  __info_rename_at_709_265, LineInfo const  &  __at_rename_at_709_266 ) { das_stack_prologue __prologue(__context__,240,"walk_and_convert_table " DAS_FILE_LINE);
{
    smart_ptr_raw<TypeDecl> __tupT_rename_at_712_268; memset((void*)&__tupT_rename_at_712_268,0,sizeof(__tupT_rename_at_712_268));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_715_269; memset((void*)&__mkArr_rename_at_715_269,0,sizeof(__mkArr_rename_at_715_269));
    int32_t __key_stride_rename_at_716_270; memset((void*)&__key_stride_rename_at_716_270,0,sizeof(__key_stride_rename_at_716_270));
    int32_t __value_stride_rename_at_717_271; memset((void*)&__value_stride_rename_at_717_271,0,sizeof(__value_stride_rename_at_717_271));
    smart_ptr_raw<ExprCall> __mkToTableMove_rename_at_727_277; memset((void*)&__mkToTableMove_rename_at_727_277,0,sizeof(__mkToTableMove_rename_at_727_277));
    int32_t __total_rename_at_710_267 = ((int32_t)any_table_size(das_auto_cast<void * const >::cast(__data_rename_at_709_264)));
    if ( __total_rename_at_710_267 != 0 )
    {
        /* finally */ auto __finally_711= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToTableMove_rename_at_727_277);
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_715_269);
        das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__tupT_rename_at_712_268);
        /* end finally */ });
        __tupT_rename_at_712_268; das_zero(__tupT_rename_at_712_268); das_move(__tupT_rename_at_712_268, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_712) {
            das_copy((__mks_712.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tTuple));
            das_copy((__mks_712.at /*at*/),(__at_rename_at_709_266));
        }))));
        emplace_new_7386d366d948387e(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_712_268->argTypes /*argTypes*/),clone_type(__info_rename_at_709_265->firstType /*firstType*/));
        emplace_new_7386d366d948387e(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_712_268->argTypes /*argTypes*/),clone_type(__info_rename_at_709_265->secondType /*secondType*/));
        __mkArr_rename_at_715_269; das_zero(__mkArr_rename_at_715_269); das_move(__mkArr_rename_at_715_269, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_715) {
            das_copy((__mks_715.at /*at*/),(__at_rename_at_709_266));
            das_move((__mks_715.makeType /*makeType*/),(clone_type(__tupT_rename_at_712_268)));
        }))));
        __key_stride_rename_at_716_270 = ((int32_t)((das_deref(__context__,__info_rename_at_709_265->firstType /*firstType*/,__FILE__,__LINE__)).getSizeOf()));
        __value_stride_rename_at_717_271 = ((int32_t)((das_deref(__context__,__info_rename_at_709_265->secondType /*secondType*/,__FILE__,__LINE__)).getSizeOf()));
        any_table_foreach(das_auto_cast<void * const >::cast(__data_rename_at_709_264),__key_stride_rename_at_716_270,__value_stride_rename_at_717_271,das_make_block<void,void *,void *>(__context__,160,0,&__func_info__b50c88059cb1bcc,[&](void * __pkey_rename_at_718_272, void * __pvalue_rename_at_718_273) -> void{
            smart_ptr_raw<Expression> __key_rename_at_719_274; memset((void*)&__key_rename_at_719_274,0,sizeof(__key_rename_at_719_274));
            smart_ptr_raw<Expression> __value_rename_at_720_275; memset((void*)&__value_rename_at_720_275,0,sizeof(__value_rename_at_720_275));
            smart_ptr_raw<ExprMakeTuple> __mkTup_rename_at_721_276; memset((void*)&__mkTup_rename_at_721_276,0,sizeof(__mkTup_rename_at_721_276));
            /* finally */ auto __finally_718= das_finally([&](){
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__value_rename_at_720_275);
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__key_rename_at_719_274);
            das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkTup_rename_at_721_276);
            /* end finally */ });
            __key_rename_at_719_274; das_zero(__key_rename_at_719_274); das_move(__key_rename_at_719_274, walk_and_convert_cfdd8313a6a5987(__context__,das_auto_cast<uint8_t const  * const >::cast(__pkey_rename_at_718_272),__info_rename_at_709_265->firstType /*firstType*/,__at_rename_at_709_266));
            __value_rename_at_720_275; das_zero(__value_rename_at_720_275); das_move(__value_rename_at_720_275, walk_and_convert_cfdd8313a6a5987(__context__,das_auto_cast<uint8_t const  * const >::cast(__pvalue_rename_at_718_273),__info_rename_at_709_265->secondType /*secondType*/,__at_rename_at_709_266));
            __mkTup_rename_at_721_276; das_zero(__mkTup_rename_at_721_276); das_move(__mkTup_rename_at_721_276, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_721) {
                das_copy((__mks_721.at /*at*/),(__at_rename_at_709_266));
                das_move((__mks_721.recordType /*recordType*/),(clone_type(__tupT_rename_at_712_268)));
                das_copy((__mks_721.isKeyValue /*isKeyValue*/),(true));
            }))));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_721_276->values /*values*/),__key_rename_at_719_274);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_721_276->values /*values*/),__value_rename_at_720_275);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_715_269->values /*values*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkTup_rename_at_721_276));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_715_269->makeType /*makeType*/->dim /*dim*/),__total_rename_at_710_267);
        __mkToTableMove_rename_at_727_277; das_zero(__mkToTableMove_rename_at_727_277); das_move(__mkToTableMove_rename_at_727_277, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_727) {
            das_copy((__mks_727.at /*at*/),(__at_rename_at_709_266));
            {
                set_das_string(das_arg<das::string>::pass(__mks_727.name /*name*/),((char *) "to_table_move"));
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToTableMove_rename_at_727_277->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkArr_rename_at_715_269));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkToTableMove_rename_at_727_277);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_731) {
            das_copy((__mks_731.at /*at*/),(__at_rename_at_709_266));
            das_move((__mks_731.makeType /*makeType*/),(clone_type(__info_rename_at_709_265)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_basic_5a2f4f9254235414 ( Context * __context__, uint8_t const  * const  __data_rename_at_735_279, smart_ptr_raw<TypeDecl> const  __info_rename_at_735_280, LineInfo const  &  __at_rename_at_735_281 ) { das_stack_prologue __prologue(__context__,112,"walk_and_convert_basic " DAS_FILE_LINE);
{
    if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt8 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt8>>::make(__context__,nullptr,(([&](ExprConstInt8 & __mks_738) {
            das_copy((__mks_738.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_738.cvalue<int8_t>() /*value*/),(das_deref(__context__,das_cast<int8_t *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt16 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt16>>::make(__context__,nullptr,(([&](ExprConstInt16 & __mks_740) {
            das_copy((__mks_740.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_740.cvalue<int16_t>() /*value*/),(das_deref(__context__,das_cast<int16_t *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt>>::make(__context__,nullptr,(([&](ExprConstInt & __mks_742) {
            das_copy((__mks_742.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_742.cvalue<int32_t>() /*value*/),(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt64>>::make(__context__,nullptr,(([&](ExprConstInt64 & __mks_744) {
            das_copy((__mks_744.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_744.cvalue<int64_t>() /*value*/),(das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt8 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt8>>::make(__context__,nullptr,(([&](ExprConstUInt8 & __mks_746) {
            das_copy((__mks_746.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_746.cvalue<uint8_t>() /*value*/),(das_deref(__context__,das_cast<uint8_t *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt16 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt16>>::make(__context__,nullptr,(([&](ExprConstUInt16 & __mks_748) {
            das_copy((__mks_748.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_748.cvalue<uint16_t>() /*value*/),(das_deref(__context__,das_cast<uint16_t *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt>>::make(__context__,nullptr,(([&](ExprConstUInt & __mks_750) {
            das_copy((__mks_750.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_750.cvalue<uint32_t>() /*value*/),(das_deref(__context__,das_cast<uint32_t *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt64>>::make(__context__,nullptr,(([&](ExprConstUInt64 & __mks_752) {
            das_copy((__mks_752.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_752.cvalue<uint64_t>() /*value*/),(das_deref(__context__,das_cast<uint64_t *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstFloat>>::make(__context__,nullptr,(([&](ExprConstFloat & __mks_754) {
            das_copy((__mks_754.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_754.cvalue<float>() /*value*/),(das_deref(__context__,das_cast<float *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tDouble )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstDouble>>::make(__context__,nullptr,(([&](ExprConstDouble & __mks_756) {
            das_copy((__mks_756.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_756.cvalue<double>() /*value*/),(das_deref(__context__,das_cast<double *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBool>>::make(__context__,nullptr,(([&](ExprConstBool & __mks_758) {
            das_copy((__mks_758.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_758.cvalue<bool>() /*value*/),(das_deref(__context__,das_cast<bool *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange>>::make(__context__,nullptr,(([&](ExprConstRange & __mks_760) {
            das_copy((__mks_760.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_760.cvalue<range>() /*value*/),(das_deref(__context__,das_cast<range *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange>>::make(__context__,nullptr,(([&](ExprConstURange & __mks_762) {
            das_copy((__mks_762.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_762.cvalue<urange>() /*value*/),(das_deref(__context__,das_cast<urange *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange64>>::make(__context__,nullptr,(([&](ExprConstRange64 & __mks_764) {
            das_copy((__mks_764.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_764.cvalue<range64>() /*value*/),(das_deref(__context__,das_cast<range64 *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange64>>::make(__context__,nullptr,(([&](ExprConstURange64 & __mks_766) {
            das_copy((__mks_766.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_766.cvalue<urange64>() /*value*/),(das_deref(__context__,das_cast<urange64 *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_768) {
            das_copy((__mks_768.at /*at*/),(__at_rename_at_735_281));
            {
                set_das_string(das_arg<das::string>::pass(__mks_768.text /*value*/),das_deref(__context__,das_cast<char * *>::cast(__data_rename_at_735_279),__FILE__,__LINE__));
            }        }))));
    } else if ( __info_rename_at_735_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBitfield>>::make(__context__,nullptr,(([&](ExprConstBitfield & __mks_770) {
            das_copy((__mks_770.at /*at*/),(__at_rename_at_735_281));
            das_copy((__mks_770.cvalue<Bitfield>() /*value*/),(das_deref(__context__,das_cast<Bitfield *>::cast(__data_rename_at_735_279),__FILE__,__LINE__)));
        }))));
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<char * const >::from(((char *) "unsupported ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__info_rename_at_735_280->baseType /*baseType*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(nullptr);
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_enumeration_5bb2d478c4ffeaaa ( Context * __context__, uint8_t const  * const  __data_rename_at_778_283, smart_ptr_raw<TypeDecl> const  __info_rename_at_778_284, LineInfo const  &  __at_rename_at_778_285 ) { das_stack_prologue __prologue(__context__,144,"walk_and_convert_enumeration " DAS_FILE_LINE);
{
    int64_t __eval_rename_at_779_286 = INT64_C(0);
    if ( __info_rename_at_778_284->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration )
    {
        das_copy(__eval_rename_at_779_286,int64_t(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_778_283),__FILE__,__LINE__)));
    } else if ( __info_rename_at_778_284->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8 )
    {
        das_copy(__eval_rename_at_779_286,int64_t(das_deref(__context__,__data_rename_at_778_283,__FILE__,__LINE__)));
    } else if ( __info_rename_at_778_284->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16 )
    {
        das_copy(__eval_rename_at_779_286,int64_t(das_deref(__context__,das_cast<int16_t *>::cast(__data_rename_at_778_283),__FILE__,__LINE__)));
    } else if ( __info_rename_at_778_284->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64 )
    {
        das_copy(__eval_rename_at_779_286,das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_778_283),__FILE__,__LINE__));
    } else {
        builtin_throw(((char *) "unsupported enumeration"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    char * __name_rename_at_791_287 = ((char *)(char *)(((char * const )(ast_find_enum_name(__info_rename_at_778_284->enumType /*enumType*/,__eval_rename_at_779_286,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_791_287),cast<char *>::from(nullptr),*__context__,nullptr) )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_14, cast<char * const >::from(((char *) "can't find name of enum value ")), cast<int64_t>::from(__eval_rename_at_779_286), cast<char * const >::from(((char *) " in ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,__info_rename_at_778_284,true,true,true)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstEnumeration>>::make(__context__,nullptr,(([&](ExprConstEnumeration & __mks_793) {
        {
            set_das_string(das_arg<das::string>::pass(__mks_793.text /*value*/),__name_rename_at_791_287);
            clone_a8386ed128f4f0b4(__context__,__mks_793.enumType /*enumType*/,das_cast<Enumeration *>::cast(__info_rename_at_778_284->enumType /*enumType*/));
        }    }))));
}}

inline smart_ptr_raw<Expression> walk_and_convert_cfdd8313a6a5987 ( Context * __context__, uint8_t const  * const  __data_rename_at_798_289, smart_ptr_raw<TypeDecl> const  __info_rename_at_798_290, LineInfo const  &  __at_rename_at_798_291 )
{
    if ( das_vector_length(__info_rename_at_798_290->dim /*dim*/) != 0 )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_dim_9001b9148a98cff6(__context__,__data_rename_at_798_289,__info_rename_at_798_290,__at_rename_at_798_291));
    } else if ( __info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tArray )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_array_ba1cc70e35611268(__context__,__data_rename_at_798_289,__info_rename_at_798_290,__at_rename_at_798_291));
    } else if ( __info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        if ( das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_798_289),__FILE__,__LINE__) == nullptr )
        {
            return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_807) {
                das_copy((__mks_807.at /*at*/),(__at_rename_at_798_291));
            }))));
        } else return das_auto_cast<smart_ptr_raw<Expression>>::cast(((equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__info_rename_at_798_290->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr))) || ((das_deref(__context__,__info_rename_at_798_290->firstType /*firstType*/,__FILE__,__LINE__)).isVoid())) ? das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_809) {
            das_copy((__mks_809.at /*at*/),(__at_rename_at_798_291));
        })))) : das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(walk_and_convert_pointer_47b794d0dd59337d(__context__,__data_rename_at_798_289,__info_rename_at_798_290,__at_rename_at_798_291)));
    } else if ( __info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tStructure )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_structure_106d0a1bf460f63c(__context__,__data_rename_at_798_289,__info_rename_at_798_290,__at_rename_at_798_291));
    } else if ( __info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tVariant )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_variant_cdeba207e46113d9(__context__,__data_rename_at_798_289,__info_rename_at_798_290,__at_rename_at_798_291));
    } else if ( __info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTuple )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_tuple_9ce3b13721ba5daf(__context__,__data_rename_at_798_289,__info_rename_at_798_290,__at_rename_at_798_291));
    } else if ( __info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTable )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_table_bafed583a64bc3a1(__context__,__data_rename_at_798_289,__info_rename_at_798_290,__at_rename_at_798_291));
    } else return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(((((__info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8) || (__info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16)) || (__info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64)) || (__info_rename_at_798_290->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration)) ? das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_enumeration_5bb2d478c4ffeaaa(__context__,__data_rename_at_798_289,__info_rename_at_798_290,__at_rename_at_798_291)) : das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_basic_5a2f4f9254235414(__context__,__data_rename_at_798_289,__info_rename_at_798_290,__at_rename_at_798_291)));
}

inline Annotation const  * find_annotation_e87941f09ff8d46c ( Context * __context__, char * const  __mod_name_rename_at_853_292, char * const  __ann_name_rename_at_853_293 ) { das_stack_prologue __prologue(__context__,128,"find_annotation " DAS_FILE_LINE);
{
    Module * __mod_rename_at_854_294 = _FuncastTickfind_compiling_moduleTick4523452840392654583_584104531b17a816(__context__,__mod_name_rename_at_853_292);
    if ( __mod_rename_at_854_294 == nullptr )
    {
        return das_auto_cast<Annotation const  *>::cast(nullptr);
    } else {
        Annotation const  * __ann_rename_at_858_295 = 0;
        rtti_builtin_module_for_each_annotation(__mod_rename_at_854_294,das_make_block<void,Annotation const  &>(__context__,112,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_859_296) -> void{
            if ( eq_dstr_str(__value_rename_at_859_296.name /*name*/,__ann_name_rename_at_853_293) )
            {
                das_copy(__ann_rename_at_858_295,das_ref(__context__,__value_rename_at_859_296));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<Annotation const  *>::cast(__ann_rename_at_858_295);
    };
}}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_79e01888a17a2cf8 ( Context * __context__, char * const  __mod_name_rename_at_869_297, char * const  __ann_name_rename_at_869_298, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_869_299 )
{
    Annotation const  * __ann_rename_at_870_300; memset((void*)&__ann_rename_at_870_300,0,sizeof(__ann_rename_at_870_300));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_872_301; memset((void*)&__decl_rename_at_872_301,0,sizeof(__decl_rename_at_872_301));
    char * * __argName_rename_at_875_303; memset((void*)&__argName_rename_at_875_303,0,sizeof(__argName_rename_at_875_303));
    AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> * __arg_rename_at_876_304; memset((void*)&__arg_rename_at_876_304,0,sizeof(__arg_rename_at_876_304));
    /* finally */ auto __finally_869= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_872_301);
    /* end finally */ });
    __ann_rename_at_870_300 = find_annotation_e87941f09ff8d46c(__context__,__mod_name_rename_at_869_297,__ann_name_rename_at_869_298);
    DAS_ASSERT((__ann_rename_at_870_300 != nullptr));
    __decl_rename_at_872_301; das_zero(__decl_rename_at_872_301); das_move(__decl_rename_at_872_301, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_6e9cdc5c6ca1d1f3(__context__,__decl_rename_at_872_301->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_870_300));
    {
        bool __need_loop_874 = true;
        // argP: tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any> aka RttiValue> const&
        das_iterator<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const > __argP_iterator(__args_rename_at_869_299);
        AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>> const  * __argP_rename_at_874_302;
        __need_loop_874 = __argP_iterator.first(__context__,(__argP_rename_at_874_302)) && __need_loop_874;
        for ( ; __need_loop_874 ; __need_loop_874 = __argP_iterator.next(__context__,(__argP_rename_at_874_302)) )
        {
            __argName_rename_at_875_303 = ((char * *)&((char * &)(das_get_auto_tuple_field<char *,0,char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::get((*__argP_rename_at_874_302)))));
            __arg_rename_at_876_304 = ((AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *)&(das_get_auto_tuple_field<AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,1,char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::get((*__argP_rename_at_874_302))));
            if ( das_get_auto_variant_field<bool,0,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_876_304)) )
            {
                add_annotation_argument_ecdc7f01cc8d28cd(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_872_301->arguments /*arguments*/),(*__argName_rename_at_875_303),das_get_auto_variant_field<bool,0,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_876_304),__context__));
            } else if ( das_get_auto_variant_field<int32_t,1,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_876_304)) )
            {
                add_annotation_argument_5d1cfc8d958b1b74(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_872_301->arguments /*arguments*/),(*__argName_rename_at_875_303),das_get_auto_variant_field<int32_t,1,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_876_304),__context__));
            } else if ( das_get_auto_variant_field<float,5,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_876_304)) )
            {
                add_annotation_argument_2ba02ec1a95c40de(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_872_301->arguments /*arguments*/),(*__argName_rename_at_875_303),das_get_auto_variant_field<float,5,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_876_304),__context__));
            } else if ( das_get_auto_variant_field<char *,7,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::is((*__arg_rename_at_876_304)) )
            {
                add_annotation_argument_e01dc466768bdd13(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_872_301->arguments /*arguments*/),(*__argName_rename_at_875_303),das_get_auto_variant_field<char *,7,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>::as((*__arg_rename_at_876_304),__context__));
            } else {
                builtin_throw(((char *) "Invalid rtti value"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __argP_iterator.close(__context__,(__argP_rename_at_874_302));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_872_301);
}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_91779d544c0857a9 ( Context * __context__, char * const  __mod_name_rename_at_892_305, char * const  __ann_name_rename_at_892_306 )
{
    Annotation const  * __ann_rename_at_893_307; memset((void*)&__ann_rename_at_893_307,0,sizeof(__ann_rename_at_893_307));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_895_308; memset((void*)&__decl_rename_at_895_308,0,sizeof(__decl_rename_at_895_308));
    /* finally */ auto __finally_892= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_895_308);
    /* end finally */ });
    __ann_rename_at_893_307 = find_annotation_e87941f09ff8d46c(__context__,__mod_name_rename_at_892_305,__ann_name_rename_at_892_306);
    DAS_ASSERT((__ann_rename_at_893_307 != nullptr));
    __decl_rename_at_895_308; das_zero(__decl_rename_at_895_308); das_move(__decl_rename_at_895_308, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_6e9cdc5c6ca1d1f3(__context__,__decl_rename_at_895_308->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_893_307));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_895_308);
}

inline void append_annotation_9e9720e2bdf176ae ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_900_309, char * const  __mod_name_rename_at_900_310, char * const  __ann_name_rename_at_900_311 )
{
    Annotation const  * __ann_rename_at_901_312; memset((void*)&__ann_rename_at_901_312,0,sizeof(__ann_rename_at_901_312));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_903_313; memset((void*)&__decl_rename_at_903_313,0,sizeof(__decl_rename_at_903_313));
    /* finally */ auto __finally_900= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_903_313);
    /* end finally */ });
    __ann_rename_at_901_312 = find_annotation_e87941f09ff8d46c(__context__,__mod_name_rename_at_900_310,__ann_name_rename_at_900_311);
    DAS_ASSERT((__ann_rename_at_901_312 != nullptr));
    __decl_rename_at_903_313; das_zero(__decl_rename_at_903_313); das_move(__decl_rename_at_903_313, append_annotation_91779d544c0857a9(__context__,__mod_name_rename_at_900_310,__ann_name_rename_at_900_311));
    addAndApplyFunctionAnnotation(__func_rename_at_900_309,__decl_rename_at_903_313,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_4e3ac504625b9e59 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_907_314, char * const  __mod_name_rename_at_907_315, char * const  __ann_name_rename_at_907_316 )
{
    Annotation const  * __ann_rename_at_908_317; memset((void*)&__ann_rename_at_908_317,0,sizeof(__ann_rename_at_908_317));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_910_318; memset((void*)&__decl_rename_at_910_318,0,sizeof(__decl_rename_at_910_318));
    /* finally */ auto __finally_907= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_910_318);
    /* end finally */ });
    __ann_rename_at_908_317 = find_annotation_e87941f09ff8d46c(__context__,__mod_name_rename_at_907_315,__ann_name_rename_at_907_316);
    DAS_ASSERT((__ann_rename_at_908_317 != nullptr));
    __decl_rename_at_910_318; das_zero(__decl_rename_at_910_318); das_move(__decl_rename_at_910_318, append_annotation_91779d544c0857a9(__context__,__mod_name_rename_at_907_315,__ann_name_rename_at_907_316));
    addAndApplyBlockAnnotation(__blk_rename_at_907_314,__decl_rename_at_910_318,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_1d124a466ae78997 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_914_319, char * const  __mod_name_rename_at_914_320, char * const  __ann_name_rename_at_914_321 )
{
    Annotation const  * __ann_rename_at_915_322; memset((void*)&__ann_rename_at_915_322,0,sizeof(__ann_rename_at_915_322));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_917_323; memset((void*)&__decl_rename_at_917_323,0,sizeof(__decl_rename_at_917_323));
    /* finally */ auto __finally_914= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_917_323);
    /* end finally */ });
    __ann_rename_at_915_322 = find_annotation_e87941f09ff8d46c(__context__,__mod_name_rename_at_914_320,__ann_name_rename_at_914_321);
    DAS_ASSERT((__ann_rename_at_915_322 != nullptr));
    __decl_rename_at_917_323; das_zero(__decl_rename_at_917_323); das_move(__decl_rename_at_917_323, append_annotation_91779d544c0857a9(__context__,__mod_name_rename_at_914_320,__ann_name_rename_at_914_321));
    addAndApplyStructAnnotation(__st_rename_at_914_319,__decl_rename_at_917_323,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_d2aadccfb018501d ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_921_324, char * const  __mod_name_rename_at_921_325, char * const  __ann_name_rename_at_921_326, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_921_327 )
{
    Annotation const  * __ann_rename_at_922_328; memset((void*)&__ann_rename_at_922_328,0,sizeof(__ann_rename_at_922_328));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_924_329; memset((void*)&__decl_rename_at_924_329,0,sizeof(__decl_rename_at_924_329));
    /* finally */ auto __finally_921= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_924_329);
    /* end finally */ });
    __ann_rename_at_922_328 = find_annotation_e87941f09ff8d46c(__context__,__mod_name_rename_at_921_325,__ann_name_rename_at_921_326);
    DAS_ASSERT((__ann_rename_at_922_328 != nullptr));
    __decl_rename_at_924_329; das_zero(__decl_rename_at_924_329); das_move(__decl_rename_at_924_329, append_annotation_79e01888a17a2cf8(__context__,__mod_name_rename_at_921_325,__ann_name_rename_at_921_326,__args_rename_at_921_327));
    addAndApplyFunctionAnnotation(__func_rename_at_921_324,__decl_rename_at_924_329,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_4239f8097567c00 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_928_330, char * const  __mod_name_rename_at_928_331, char * const  __ann_name_rename_at_928_332, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_928_333 )
{
    Annotation const  * __ann_rename_at_929_334; memset((void*)&__ann_rename_at_929_334,0,sizeof(__ann_rename_at_929_334));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_931_335; memset((void*)&__decl_rename_at_931_335,0,sizeof(__decl_rename_at_931_335));
    /* finally */ auto __finally_928= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_931_335);
    /* end finally */ });
    __ann_rename_at_929_334 = find_annotation_e87941f09ff8d46c(__context__,__mod_name_rename_at_928_331,__ann_name_rename_at_928_332);
    DAS_ASSERT((__ann_rename_at_929_334 != nullptr));
    __decl_rename_at_931_335; das_zero(__decl_rename_at_931_335); das_move(__decl_rename_at_931_335, append_annotation_79e01888a17a2cf8(__context__,__mod_name_rename_at_928_331,__ann_name_rename_at_928_332,__args_rename_at_928_333));
    addAndApplyBlockAnnotation(__blk_rename_at_928_330,__decl_rename_at_931_335,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_84797c2112b4fd76 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_935_336, char * const  __mod_name_rename_at_935_337, char * const  __ann_name_rename_at_935_338, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_935_339 )
{
    Annotation const  * __ann_rename_at_936_340; memset((void*)&__ann_rename_at_936_340,0,sizeof(__ann_rename_at_936_340));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_938_341; memset((void*)&__decl_rename_at_938_341,0,sizeof(__decl_rename_at_938_341));
    /* finally */ auto __finally_935= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_938_341);
    /* end finally */ });
    __ann_rename_at_936_340 = find_annotation_e87941f09ff8d46c(__context__,__mod_name_rename_at_935_337,__ann_name_rename_at_935_338);
    DAS_ASSERT((__ann_rename_at_936_340 != nullptr));
    __decl_rename_at_938_341; das_zero(__decl_rename_at_938_341); das_move(__decl_rename_at_938_341, append_annotation_79e01888a17a2cf8(__context__,__mod_name_rename_at_935_337,__ann_name_rename_at_935_338,__args_rename_at_935_339));
    addAndApplyStructAnnotation(__st_rename_at_935_336,__decl_rename_at_938_341,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t add_annotation_argument_ecdc7f01cc8d28cd ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_942_342, char * const  __argName_rename_at_942_343, bool __val_rename_at_942_344 )
{
    int32_t __argIdx_rename_at_943_345 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_942_342),__argName_rename_at_942_343));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_942_342,__argIdx_rename_at_943_345,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tBool);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_942_342,__argIdx_rename_at_943_345,__context__).bValue /*bValue*/,__val_rename_at_942_344);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_943_345);
}

inline int32_t add_annotation_argument_5d1cfc8d958b1b74 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_949_346, char * const  __argName_rename_at_949_347, int32_t __val_rename_at_949_348 )
{
    int32_t __argIdx_rename_at_950_349 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_949_346),__argName_rename_at_949_347));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_949_346,__argIdx_rename_at_950_349,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tInt);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_949_346,__argIdx_rename_at_950_349,__context__).iValue /*iValue*/,__val_rename_at_949_348);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_950_349);
}

inline int32_t add_annotation_argument_2ba02ec1a95c40de ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_956_350, char * const  __argName_rename_at_956_351, float __val_rename_at_956_352 )
{
    int32_t __argIdx_rename_at_957_353 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_956_350),__argName_rename_at_956_351));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_956_350,__argIdx_rename_at_957_353,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tFloat);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_956_350,__argIdx_rename_at_957_353,__context__).fValue /*fValue*/,__val_rename_at_956_352);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_957_353);
}

inline int32_t add_annotation_argument_e01dc466768bdd13 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_963_354, char * const  __argName_rename_at_963_355, char * const  __val_rename_at_963_356 )
{
    int32_t __argIdx_rename_at_964_357 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_963_354),__argName_rename_at_963_355));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_963_354,__argIdx_rename_at_964_357,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tString);
    set_das_string(das_arg<das::string>::pass(das_index<AnnotationArgumentList>::at(__arguments_rename_at_963_354,__argIdx_rename_at_964_357,__context__).sValue /*sValue*/),__val_rename_at_963_356);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_964_357);
}

inline int32_t add_annotation_argument_e7f221e58ec46512 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_970_358, AnnotationArgument const  &  __ann_rename_at_970_359 )
{
    int32_t __argIdx_rename_at_971_360 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_970_358),((char * const )(to_das_string(__ann_rename_at_970_359.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_970_358,__argIdx_rename_at_971_360,__context__).type /*basicType*/,__ann_rename_at_970_359.type /*basicType*/);
    if ( __ann_rename_at_970_359.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_970_358,__argIdx_rename_at_971_360,__context__).bValue /*bValue*/,__ann_rename_at_970_359.bValue /*bValue*/);
    } else if ( __ann_rename_at_970_359.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_970_358,__argIdx_rename_at_971_360,__context__).iValue /*iValue*/,__ann_rename_at_970_359.iValue /*iValue*/);
    } else if ( __ann_rename_at_970_359.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_970_358,__argIdx_rename_at_971_360,__context__).fValue /*fValue*/,__ann_rename_at_970_359.fValue /*fValue*/);
    } else if ( __ann_rename_at_970_359.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        das_clone<das::string,das::string>::clone(das_index<AnnotationArgumentList>::at(__arguments_rename_at_970_358,__argIdx_rename_at_971_360,__context__).sValue /*sValue*/,__ann_rename_at_970_359.sValue /*sValue*/);
    } else {
        DAS_ASSERTF((false),(((char *) "unsupported annotation type, add more types")));
    };
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_971_360);
}

inline int32_t get_for_source_index_19e9c390ee4338e2 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_987_361, smart_ptr_raw<Variable> const  __svar_rename_at_987_362 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_988_41_2; _temp_make_local_988_41_2;
    {
        bool __need_loop_988 = true;
        // v: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __v_iterator(__expr_rename_at_987_361->iteratorVariables /*iteratorVariables*/);
        smart_ptr_raw<Variable> const  * __v_rename_at_988_365;
        __need_loop_988 = __v_iterator.first(__context__,(__v_rename_at_988_365)) && __need_loop_988;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_988_41_2 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_988_366;
        __need_loop_988 = __t_iterator.first(__context__,(__t_rename_at_988_366)) && __need_loop_988;
        for ( ; __need_loop_988 ; __need_loop_988 = __v_iterator.next(__context__,(__v_rename_at_988_365)) && __t_iterator.next(__context__,(__t_rename_at_988_366)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__v_rename_at_988_365)),das_auto_cast<smart_ptr_raw<void> const >::cast(__svar_rename_at_987_362)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_988_366);
            };
        }
        __t_iterator.close(__context__,(__t_rename_at_988_366));
        __v_iterator.close(__context__,(__v_rename_at_988_365));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline int32_t get_for_source_index_e425541a2a706c53 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_996_367, smart_ptr_raw<Expression> const  __source_rename_at_996_368 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_997_31_3; _temp_make_local_997_31_3;
    {
        bool __need_loop_997 = true;
        // s: smart_ptr<ast::Expression> const&
        das_iterator<das::vector<smart_ptr<Expression>> const > __s_iterator(__expr_rename_at_996_367->sources /*sources*/);
        smart_ptr_raw<Expression> const  * __s_rename_at_997_371;
        __need_loop_997 = __s_iterator.first(__context__,(__s_rename_at_997_371)) && __need_loop_997;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_997_31_3 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_997_372;
        __need_loop_997 = __t_iterator.first(__context__,(__t_rename_at_997_372)) && __need_loop_997;
        for ( ; __need_loop_997 ; __need_loop_997 = __s_iterator.next(__context__,(__s_rename_at_997_371)) && __t_iterator.next(__context__,(__t_rename_at_997_372)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__s_rename_at_997_371)),das_auto_cast<smart_ptr_raw<void> const >::cast(__source_rename_at_996_368)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_997_372);
            };
        }
        __t_iterator.close(__context__,(__t_rename_at_997_372));
        __s_iterator.close(__context__,(__s_rename_at_997_371));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline smart_ptr_raw<TypeDecl> function_to_type_1f450dac98d6071d ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_1044_373 )
{
    smart_ptr_raw<TypeDecl> __td_rename_at_1045_374; memset((void*)&__td_rename_at_1045_374,0,sizeof(__td_rename_at_1045_374));
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1049_36_4; _temp_make_local_1049_36_4;
    /* finally */ auto __finally_1044= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__td_rename_at_1045_374);
    /* end finally */ });
    __td_rename_at_1045_374; das_zero(__td_rename_at_1045_374); das_move(__td_rename_at_1045_374, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1045) {
        das_copy((__mks_1045.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tFunction));
        das_move((__mks_1045.firstType /*firstType*/),(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_1044_373->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<smart_ptr_raw<TypeDecl>>::cast(clone_type(__fn_rename_at_1044_373->result /*result*/)) : das_auto_cast<smart_ptr_raw<TypeDecl>>::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1046) {
            das_copy((__mks_1046.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
        })))))));
    }))));
    das_vector_resize(das_arg<das::vector<das::string>>::pass(__td_rename_at_1045_374->argNames /*argNames*/),das_vector_length(__fn_rename_at_1044_373->arguments /*arguments*/));
    {
        bool __need_loop_1049 = true;
        // arg: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __arg_iterator(__fn_rename_at_1044_373->arguments /*arguments*/);
        smart_ptr_raw<Variable> const  * __arg_rename_at_1049_377;
        __need_loop_1049 = __arg_iterator.first(__context__,(__arg_rename_at_1049_377)) && __need_loop_1049;
        // argi: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1049_36_4 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __argi_iterator(0,1);
        int32_t __argi_rename_at_1049_378;
        __need_loop_1049 = __argi_iterator.first(__context__,(__argi_rename_at_1049_378)) && __need_loop_1049;
        for ( ; __need_loop_1049 ; __need_loop_1049 = __arg_iterator.next(__context__,(__arg_rename_at_1049_377)) && __argi_iterator.next(__context__,(__argi_rename_at_1049_378)) )
        {
            emplace_new_7386d366d948387e(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__td_rename_at_1045_374->argTypes /*argTypes*/),clone_type((*__arg_rename_at_1049_377)->type /*_type*/));
            das_clone<das::string,das::string>::clone(das_index<das::vector<das::string>>::at(__td_rename_at_1045_374->argNames /*argNames*/,__argi_rename_at_1049_378,__context__),(*__arg_rename_at_1049_377)->name /*name*/);
        }
        __argi_iterator.close(__context__,(__argi_rename_at_1049_378));
        __arg_iterator.close(__context__,(__arg_rename_at_1049_377));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(__td_rename_at_1045_374);
}

inline void visit_finally_d773bb44db182978 ( Context * __context__, ExprBlock * const  __blk_rename_at_1056_379, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1056_380 )
{
    astVisitBlockFinally(das_cast<smart_ptr_raw<ExprBlock>>::cast(__blk_rename_at_1056_379),__adapter_rename_at_1056_380,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool isCMRES_2bb438cbd738593f ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1062_381 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1062_381->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1062_381->flags /*flags*/,1u << 6));
}

inline bool isCMRES_45d360720945cc74 ( Context * __context__, Function * const  __fun_rename_at_1066_382 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1066_382->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1066_382->flags /*flags*/,1u << 6));
}

inline bool isMakeLocal_c461b44592b8facf ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1070_383 )
{
    return das_auto_cast<bool>::cast(((((SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1070_383->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeLocal")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1070_383->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeStruct")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1070_383->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeVariant")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1070_383->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeArray")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1070_383->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeTuple")),*__context__,nullptr)));
}

inline bool isExprCallFunc_aac1b10a17185ef1 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1078_384 )
{
    return das_auto_cast<bool>::cast((((SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1078_384->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCallFunc")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1078_384->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprOp")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1078_384->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprNew")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1078_384->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCall")),*__context__,nullptr)));
}

inline TDim<DAS_COMMENT(bound_enum) das::Type,31> get_workhorse_types_62f1d76706bcc1ad ( Context * __context__ )
{
    return das_auto_cast_ref<TDim<DAS_COMMENT(bound_enum) das::Type,31>>::cast((([&]() -> TDim<DAS_COMMENT(bound_enum) das::Type,31> {
        TDim<DAS_COMMENT(bound_enum) das::Type,31> __mka_1086;
        __mka_1086(0,__context__) = DAS_COMMENT(bound_enum) das::Type::tBool;
        __mka_1086(1,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt8;
        __mka_1086(2,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt8;
        __mka_1086(3,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt16;
        __mka_1086(4,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt16;
        __mka_1086(5,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt64;
        __mka_1086(6,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt64;
        __mka_1086(7,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration;
        __mka_1086(8,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration8;
        __mka_1086(9,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration16;
        __mka_1086(10,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration64;
        __mka_1086(11,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt;
        __mka_1086(12,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt2;
        __mka_1086(13,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt3;
        __mka_1086(14,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt4;
        __mka_1086(15,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt;
        __mka_1086(16,__context__) = DAS_COMMENT(bound_enum) das::Type::tBitfield;
        __mka_1086(17,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt2;
        __mka_1086(18,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt3;
        __mka_1086(19,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt4;
        __mka_1086(20,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat;
        __mka_1086(21,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat2;
        __mka_1086(22,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat3;
        __mka_1086(23,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat4;
        __mka_1086(24,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange;
        __mka_1086(25,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange;
        __mka_1086(26,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange64;
        __mka_1086(27,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange64;
        __mka_1086(28,__context__) = DAS_COMMENT(bound_enum) das::Type::tString;
        __mka_1086(29,__context__) = DAS_COMMENT(bound_enum) das::Type::tDouble;
        __mka_1086(30,__context__) = DAS_COMMENT(bound_enum) das::Type::tPointer;
        return __mka_1086;
    })()));
}

inline int32_t find_argument_index_3d6a951c2870d00f ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1098_385, char * const  __name_rename_at_1098_386 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1099_32_5; _temp_make_local_1099_32_5;
    {
        bool __need_loop_1099 = true;
        // a: $::das_string const&
        das_iterator<das::vector<das::string> const > __a_iterator(__typ_rename_at_1098_385->argNames /*argNames*/);
        das::string const  * __a_rename_at_1099_389;
        __need_loop_1099 = __a_iterator.first(__context__,(__a_rename_at_1099_389)) && __need_loop_1099;
        // ai: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1099_32_5 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __ai_iterator(0,1);
        int32_t __ai_rename_at_1099_390;
        __need_loop_1099 = __ai_iterator.first(__context__,(__ai_rename_at_1099_390)) && __need_loop_1099;
        for ( ; __need_loop_1099 ; __need_loop_1099 = __a_iterator.next(__context__,(__a_rename_at_1099_389)) && __ai_iterator.next(__context__,(__ai_rename_at_1099_390)) )
        {
            if ( eq_dstr_str((*__a_rename_at_1099_389),__name_rename_at_1098_386) )
            {
                return das_auto_cast<int32_t>::cast(__ai_rename_at_1099_390);
            };
        }
        __ai_iterator.close(__context__,(__ai_rename_at_1099_390));
        __a_iterator.close(__context__,(__a_rename_at_1099_389));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline bool isCMRESType_bbf11aadb0354b4d ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1107_391 )
{
    return das_auto_cast<bool>::cast(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__blockT_rename_at_1107_391),das_auto_cast<void * const >::cast(nullptr))) && ((das_deref(__context__,__blockT_rename_at_1107_391,__FILE__,__LINE__)).isRefType())) && !(das_get_bitfield(__blockT_rename_at_1107_391->flags /*flags*/,1u << 0)));
}

inline void debug_expression_impl_203941aeed67e19a ( Context * __context__, StringBuilderWriter &  __writer_rename_at_1115_392, smart_ptr_raw<Expression> const  __expr_rename_at_1115_393, Bitfield __deFlags_rename_at_1115_394, int32_t __tabs_rename_at_1115_395 ) { das_stack_prologue __prologue(__context__,304,"debug_expression_impl " DAS_FILE_LINE);
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1115_393),das_auto_cast<void * const >::cast(nullptr)) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) "null"))));
        return ;
    } else {
        if ( builtin_empty(__expr_rename_at_1115_393->__rtti /*__rtti*/) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) "[NO RTTI]"))));
            return ;
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_18, cast<char * const >::from(((char *) "(")), cast<char * const >::from(__expr_rename_at_1115_393->__rtti /*__rtti*/))))));
            if ( das_get_bitfield(__deFlags_rename_at_1115_394,1u << 0) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_20, cast<char * const >::from(((char *) "<")), cast<uint32_t>::from(builtin_smart_ptr_use_count(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1115_393),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))), cast<char * const >::from(((char *) ">")))))));
            };
            if ( builtin_string_startswith(((char * const )(pass_string(__expr_rename_at_1115_393->__rtti /*__rtti*/))),((char *) "ExprConst"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) " ")), cast<char * const >::from(_FuncastTickdescribeTick842554968825501494_5630d72624d0f510(__context__,__expr_rename_at_1115_393)), cast<char * const >::from(((char *) ")")))))));
                return ;
            } else {
                BasicStructureAnnotation * __ann_rename_at_1132_396 = das_cast<BasicStructureAnnotation *>::cast(get_expression_annotation(_FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16(__context__,__expr_rename_at_1115_393),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
                if ( __ann_rename_at_1132_396 == nullptr )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) ")"))));
                    return ;
                } else {
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1132_396,__FILE__,__LINE__)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,96,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_1137_397, char * __cppName_rename_at_1137_398, smart_ptr_raw<TypeDecl> __xtype_rename_at_1137_399, uint32_t __offset_rename_at_1137_400) -> void{
                        if ( __offset_rename_at_1137_400 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1139_401 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,__xtype_rename_at_1137_399,true,true,true)));
                            int8_t * __p8_rename_at_1140_402 = 0;
                            das_copy(__p8_rename_at_1140_402,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1115_393),int32_t(__offset_rename_at_1137_400),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1139_401),cast<char *>::from(((char *) "smart_ptr<ast::Expression>")),*__context__,nullptr) )
                            {
                                smart_ptr_raw<Expression> * __pv_rename_at_1145_403 = ((smart_ptr_raw<Expression> *)das_cast<smart_ptr_raw<Expression> *>::cast(__p8_rename_at_1140_402));
                                char * __ts_rename_at_1146_404 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1115_395 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_25, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1146_404), cast<char * const >::from(((char *) "(")), cast<char *>::from(__name_rename_at_1137_397), cast<char * const >::from(((char *) " ")))))));
                                debug_expression_impl_203941aeed67e19a(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1115_392),das_deref(__context__,__pv_rename_at_1145_403,__FILE__,__LINE__),__deFlags_rename_at_1115_394,__tabs_rename_at_1115_395 + 2);
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) ")"))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1139_401),cast<char *>::from(((char *) "$::das_string")),*__context__,nullptr) )
                            {
                                das::string * __pv_rename_at_1151_405 = ((das::string *)das_cast<das::string *>::cast(__p8_rename_at_1140_402));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_28, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1137_397), cast<char * const >::from(((char *) "=\"")), cast<das::string const  &>::from(das_deref(__context__,__pv_rename_at_1151_405,__FILE__,__LINE__)), cast<char * const >::from(((char *) "\"")))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1139_401),cast<char *>::from(((char *) "int")),*__context__,nullptr) )
                            {
                                int32_t * __pv_rename_at_1154_406 = ((int32_t *)das_cast<int32_t *>::cast(__p8_rename_at_1140_402));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_30, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1137_397), cast<char * const >::from(((char *) "=")), cast<int32_t>::from(das_deref(__context__,__pv_rename_at_1154_406,__FILE__,__LINE__)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1139_401),cast<char *>::from(((char *) "uint")),*__context__,nullptr) )
                            {
                                uint32_t * __pv_rename_at_1157_407 = ((uint32_t *)das_cast<uint32_t *>::cast(__p8_rename_at_1140_402));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_32, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1137_397), cast<char * const >::from(((char *) "=")), cast<uint32_t>::from(das_deref(__context__,__pv_rename_at_1157_407,__FILE__,__LINE__)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1139_401),cast<char *>::from(((char *) "int64")),*__context__,nullptr) )
                            {
                                int64_t * __pv_rename_at_1160_408 = ((int64_t *)das_cast<int64_t *>::cast(__p8_rename_at_1140_402));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_34, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1137_397), cast<char * const >::from(((char *) "=")), cast<int64_t>::from(das_deref(__context__,__pv_rename_at_1160_408,__FILE__,__LINE__)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1139_401),cast<char *>::from(((char *) "uint64")),*__context__,nullptr) )
                            {
                                uint64_t * __pv_rename_at_1163_409 = ((uint64_t *)das_cast<uint64_t *>::cast(__p8_rename_at_1140_402));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_36, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1137_397), cast<char * const >::from(((char *) "=")), cast<uint64_t>::from(das_deref(__context__,__pv_rename_at_1163_409,__FILE__,__LINE__)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1139_401),cast<char *>::from(((char *) "bool")),*__context__,nullptr) )
                            {
                                bool * __pv_rename_at_1166_410 = ((bool *)das_cast<bool *>::cast(__p8_rename_at_1140_402));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_38, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1137_397), cast<char * const >::from(((char *) "=")), cast<bool>::from(das_deref(__context__,__pv_rename_at_1166_410,__FILE__,__LINE__)))))));
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1132_396,__FILE__,__LINE__)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,96,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_1176_411, char * __cppName_rename_at_1176_412, smart_ptr_raw<TypeDecl> __xtype_rename_at_1176_413, uint32_t __offset_rename_at_1176_414) -> void{
                        if ( __offset_rename_at_1176_414 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1178_415 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,__xtype_rename_at_1176_413,true,true,true)));
                            int8_t * __p8_rename_at_1179_416 = 0;
                            das_copy(__p8_rename_at_1179_416,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1115_393),int32_t(__offset_rename_at_1176_414),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1178_415),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Expression")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Expression>> * __pv_rename_at_1184_417 = ((das::vector<smart_ptr<Expression>> *)das_cast<das::vector<smart_ptr<Expression>> *>::cast(__p8_rename_at_1179_416));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1184_417,__FILE__,__LINE__)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1188_38_6; _temp_make_local_1188_38_6;
                                    char * __ts_rename_at_1186_418 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1115_395 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_25, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1186_418), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1176_411), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1188 = true;
                                        // l: smart_ptr<ast::Expression> const&
                                        das_iterator<das::vector<smart_ptr<Expression>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1184_417,__FILE__,__LINE__));
                                        smart_ptr_raw<Expression> const  * __l_rename_at_1188_421;
                                        __need_loop_1188 = __l_iterator.first(__context__,(__l_rename_at_1188_421)) && __need_loop_1188;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1188_38_6 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1188_422;
                                        __need_loop_1188 = __i_iterator.first(__context__,(__i_rename_at_1188_422)) && __need_loop_1188;
                                        for ( ; __need_loop_1188 ; __need_loop_1188 = __l_iterator.next(__context__,(__l_rename_at_1188_421)) && __i_iterator.next(__context__,(__i_rename_at_1188_422)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_18, cast<char * const >::from(__ts_rename_at_1186_418), cast<char * const >::from(((char *) "  ")))))));
                                            debug_expression_impl_203941aeed67e19a(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1115_392),(*__l_rename_at_1188_421),__deFlags_rename_at_1115_394,__tabs_rename_at_1115_395 + 2);
                                            if ( __i_rename_at_1188_422 != (das_vector_length(das_deref(__context__,__pv_rename_at_1184_417,__FILE__,__LINE__)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __i_iterator.close(__context__,(__i_rename_at_1188_422));
                                        __l_iterator.close(__context__,(__l_rename_at_1188_421));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) "]"))));
                                };
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1178_415),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Variable")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Variable>> * __pv_rename_at_1198_423 = ((das::vector<smart_ptr<Variable>> *)das_cast<das::vector<smart_ptr<Variable>> *>::cast(__p8_rename_at_1179_416));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1198_423,__FILE__,__LINE__)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1202_38_7; _temp_make_local_1202_38_7;
                                    char * __ts_rename_at_1200_424 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1115_395 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_25, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1200_424), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1176_411), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1202 = true;
                                        // l: smart_ptr<ast::Variable> const&
                                        das_iterator<das::vector<smart_ptr<Variable>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1198_423,__FILE__,__LINE__));
                                        smart_ptr_raw<Variable> const  * __l_rename_at_1202_427;
                                        __need_loop_1202 = __l_iterator.first(__context__,(__l_rename_at_1202_427)) && __need_loop_1202;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1202_38_7 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1202_428;
                                        __need_loop_1202 = __i_iterator.first(__context__,(__i_rename_at_1202_428)) && __need_loop_1202;
                                        for ( ; __need_loop_1202 ; __need_loop_1202 = __l_iterator.next(__context__,(__l_rename_at_1202_427)) && __i_iterator.next(__context__,(__i_rename_at_1202_428)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_18, cast<char * const >::from(__ts_rename_at_1200_424), cast<char * const >::from(((char *) "  ")))))));
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<das::string const  &>::from((*__l_rename_at_1202_427)->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,(*__l_rename_at_1202_427)->type /*_type*/,true,true,true)))))));
                                            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__l_rename_at_1202_427)->init /*init*/),das_auto_cast<void * const >::cast(nullptr)) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) " = "))));
                                                debug_expression_impl_203941aeed67e19a(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1115_392),(*__l_rename_at_1202_427)->init /*init*/,__deFlags_rename_at_1115_394,__tabs_rename_at_1115_395 + 2);
                                            };
                                            if ( __i_rename_at_1202_428 != (das_vector_length(das_deref(__context__,__pv_rename_at_1198_423,__FILE__,__LINE__)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __i_iterator.close(__context__,(__i_rename_at_1202_428));
                                        __l_iterator.close(__context__,(__l_rename_at_1202_427));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) "]"))));
                                };
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1115_392),cast<char * const >::from(((char *) ")"))));
                };
            };
        };
    };
}}

inline char * debug_expression_72d312ce8f3ec686 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1221_429, Bitfield __deFlags_rename_at_1221_430 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1222_431) DAS_AOT_INLINE_LAMBDA -> void{
        debug_expression_impl_203941aeed67e19a(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1222_431),__expr_rename_at_1221_429,__deFlags_rename_at_1221_430,0);
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * debug_expression_84991cbecc0ca3af ( Context * __context__, Expression * const  __expr_rename_at_1227_432 )
{
    return das_auto_cast<char *>::cast(debug_expression_72d312ce8f3ec686(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1227_432),0x0u));
}

inline char * describe_d11036fb3823617b ( Context * __context__, Expression * const  __expr_rename_at_1233_433 )
{
    return das_auto_cast<char *>::cast(_FuncastTickdescribeTick842554968825501494_5630d72624d0f510(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1233_433)));
}

inline int32_t getVectorElementCount_6156018073f7b991 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1239_434 )
{
    return das_auto_cast<int32_t>::cast((((((((__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tInt2)) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(2) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tFloat3) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) ? das_auto_cast<int32_t>::cast(3) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tFloat4) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1239_434 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) ? das_auto_cast<int32_t>::cast(4) : das_auto_cast<int32_t>::cast(0))))));
}

inline int32_t getVectorElementSize_7fe52b138a9b94ac ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1246_435 )
{
    return das_auto_cast<int32_t>::cast(((__bt_rename_at_1246_435 == DAS_COMMENT(bound_enum) das::Type::tRange64) || (__bt_rename_at_1246_435 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(8) : das_auto_cast<int32_t>::cast(4));
}

inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_17ab3a94ab7f2835 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1251_436 )
{
    return das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tFloat) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tRange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tUInt2) || (__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tURange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tRange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1251_436 == DAS_COMMENT(bound_enum) das::Type::tURange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tVoid))))))))));
}

inline int32_t getVectorOffset_cb21e53271916e73 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1260_437, char * const  __ident_rename_at_1260_438 )
{
    int32_t __ofs_rename_at_1261_439 = -1;
    if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "x")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "X")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "r")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "R")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1261_439,0);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "y")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "Y")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "g")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "G")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1261_439,1);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "z")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "Z")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "b")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "B")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1261_439,2);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "w")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "W")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "a")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1260_438),cast<char *>::from(((char *) "A")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1261_439,3);
    };
    int32_t __count_rename_at_1271_440 = ((int32_t)getVectorElementCount_6156018073f7b991(__context__,__bt_rename_at_1260_437));
    return das_auto_cast<int32_t>::cast((__ofs_rename_at_1261_439 >= __count_rename_at_1271_440) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(__ofs_rename_at_1261_439));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_532aa57ee748e9 ( Context * __context__ ) {
    TArray<ast::AstFunctionAnnotation *> &  arg_Arr = cast_aot_arg<TArray<ast::AstFunctionAnnotation *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstFunctionAnnotation * arg_value = cast_aot_arg<ast::AstFunctionAnnotation *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_532aa57ee748e9(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_5ea400cd583decff ( Context * __context__ ) {
    ast_boost::MacroMacro const  &  arg_cl = cast_aot_arg<ast_boost::MacroMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_5ea400cd583decff(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_ea422ce758633219 ( Context * __context__ ) {
    ast_boost::TagFunctionAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::TagFunctionAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_ea422ce758633219(__context__, arg_cl));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_8072bcb5f8834d95 ( Context * __context__ ) {
    TArray<ast::AstStructureAnnotation *> &  arg_Arr = cast_aot_arg<TArray<ast::AstStructureAnnotation *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstStructureAnnotation * arg_value = cast_aot_arg<ast::AstStructureAnnotation *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_8072bcb5f8834d95(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_3d92632ea1c06f10 ( Context * __context__ ) {
    ast_boost::SetupFunctionAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupFunctionAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_3d92632ea1c06f10(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_d3ab2c8dbad9d969 ( Context * __context__ ) {
    ast_boost::SetupBlockAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupBlockAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_d3ab2c8dbad9d969(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_3ef2f296a0f69015 ( Context * __context__ ) {
    ast_boost::SetupStructureAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupStructureAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_3ef2f296a0f69015(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_7910128189d4fa7c ( Context * __context__ ) {
    ast_boost::SetupEnumerationAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupEnumerationAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_7910128189d4fa7c(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_7c1880e164f77687 ( Context * __context__ ) {
    ast_boost::SetupContractAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupContractAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_7c1880e164f77687(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_cf248f7c72f6077b ( Context * __context__ ) {
    ast_boost::SetupReaderMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupReaderMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_cf248f7c72f6077b(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_1e3a40dee0adc6b4 ( Context * __context__ ) {
    ast_boost::SetupCommentReader const  &  arg_cl = cast_aot_arg<ast_boost::SetupCommentReader const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_1e3a40dee0adc6b4(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_688d5b5d381c2412 ( Context * __context__ ) {
    ast_boost::SetupCallMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupCallMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_688d5b5d381c2412(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_5ad54dc005365540 ( Context * __context__ ) {
    ast_boost::SetupTypeInfoMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupTypeInfoMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_5ad54dc005365540(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_763eae1f51cae115 ( Context * __context__ ) {
    ast_boost::SetupVariantMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupVariantMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_763eae1f51cae115(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_c247eb3e087743a6 ( Context * __context__ ) {
    ast_boost::SetupForLoopMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupForLoopMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_c247eb3e087743a6(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_518dfc03923aac9d ( Context * __context__ ) {
    ast_boost::SetupCaptureMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupCaptureMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_518dfc03923aac9d(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_38a2fc738f0684a7 ( Context * __context__ ) {
    ast_boost::SetupTypeMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupTypeMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_38a2fc738f0684a7(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_b87e7c5f8f5ce3c1 ( Context * __context__ ) {
    ast_boost::SetupSimulateMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupSimulateMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_b87e7c5f8f5ce3c1(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_cf0d1eb6392a85d3 ( Context * __context__ ) {
    ast_boost::TagStructureAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::TagStructureAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_cf0d1eb6392a85d3(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_9f95af835ba924bb ( Context * __context__ ) {
    ast_boost::TagFunctionMacro const  &  arg_cl = cast_aot_arg<ast_boost::TagFunctionMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_9f95af835ba924bb(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_c1b034570b4506a1 ( Context * __context__ ) {
    ast_boost::SetupInferMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupInferMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_c1b034570b4506a1(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_c4de1f05abb2f4d4 ( Context * __context__ ) {
    ast_boost::SetupDirtyInferMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupDirtyInferMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_c4de1f05abb2f4d4(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_c4ac869cc1fdebdd ( Context * __context__ ) {
    ast_boost::SetupOptimizationMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupOptimizationMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_c4ac869cc1fdebdd(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_99a4cf4ce09d9435 ( Context * __context__ ) {
    ast_boost::SetupLintMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupLintMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_99a4cf4ce09d9435(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_cb4bdd4ed7a9d0f5 ( Context * __context__ ) {
    ast_boost::SetupGlobalLintMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupGlobalLintMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_cb4bdd4ed7a9d0f5(__context__, arg_cl));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_46d64d80c21524e3 ( Context * __context__ ) {
    TArray<ast::AstVariantMacro *> &  arg_Arr = cast_aot_arg<TArray<ast::AstVariantMacro *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstVariantMacro * arg_value = cast_aot_arg<ast::AstVariantMacro *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_46d64d80c21524e3(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_7534c9a9e0721709 ( Context * __context__ ) {
    ast_boost::BetterRttiVisitor const  &  arg_cl = cast_aot_arg<ast_boost::BetterRttiVisitor const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_7534c9a9e0721709(__context__, arg_cl));
}
static vec4f __wrap_clone_7f4a34d879fc35f4 ( Context * __context__ ) {
    smart_ptr_raw<FunctionAnnotation> & arg_dest = cast_aot_arg<smart_ptr_raw<FunctionAnnotation> &>::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<FunctionAnnotation> const  arg_src = cast_aot_arg<smart_ptr_raw<FunctionAnnotation> const >::to(*__context__,__context__->abiArguments()[1]);
    clone_7f4a34d879fc35f4(__context__, arg_dest, arg_src);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0 ( Context * __context__ ) {
    smart_ptr_raw<FunctionAnnotation> arg_clone_src = cast_aot_arg<smart_ptr_raw<FunctionAnnotation>>::to(*__context__,__context__->abiArguments()[0]);
    return cast<smart_ptr_raw<FunctionAnnotation>>::from(_FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0(__context__, arg_clone_src));
}
static vec4f __wrap__FuncastTickfind_moduleTick11101329256228773934_92a5dd08b29912b8 ( Context * __context__ ) {
    smart_ptr_raw<Program> const  arg_prog = cast_aot_arg<smart_ptr_raw<Program> const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<Module *>::from(_FuncastTickfind_moduleTick11101329256228773934_92a5dd08b29912b8(__context__, arg_prog, arg_name));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f ( Context * __context__ ) {
    TArray<char *> &  arg_Arr = cast_aot_arg<TArray<char *> & >::to(*__context__,__context__->abiArguments()[0]);
    char * arg_value = cast_aot_arg<char *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9 ( Context * __context__ ) {
    TArray<char *> const  &  arg_it = cast_aot_arg<TArray<char *> const  & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_separator = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(_Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9(__context__, arg_it, arg_separator));
}
static vec4f __wrap__FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 ( Context * __context__ ) {
    TArray<char *> &  arg_Arr = cast_aot_arg<TArray<char *> & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_value = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec ( Context * __context__ ) {
    TArray<smart_ptr_raw<Function>> &  arg_Arr = cast_aot_arg<TArray<smart_ptr_raw<Function>> & >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Function> arg_value = cast_aot_arg<smart_ptr_raw<Function>>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2 ( Context * __context__ ) {
    TArray<smart_ptr_raw<Function>> &  arg_a = cast_aot_arg<TArray<smart_ptr_raw<Function>> & >::to(*__context__,__context__->abiArguments()[0]);
    _FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap_clone_e5dbb6c9a183080e ( Context * __context__ ) {
    smart_ptr_raw<Function> & arg_dest = cast_aot_arg<smart_ptr_raw<Function> &>::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Function> const  arg_src = cast_aot_arg<smart_ptr_raw<Function> const >::to(*__context__,__context__->abiArguments()[1]);
    clone_e5dbb6c9a183080e(__context__, arg_dest, arg_src);
    return v_zero();
}
static vec4f __wrap__FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5 ( Context * __context__ ) {
    smart_ptr_raw<TypeDecl> const  arg_decl = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[0]);
    bool arg_extra = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[1]);
    bool arg_contracts = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    bool arg_modules = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[3]);
    return cast<char *>::from(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__, arg_decl, arg_extra, arg_contracts, arg_modules));
}
static vec4f __wrap__FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc ( Context * __context__ ) {
    smart_ptr_raw<Structure> const  arg_src = cast_aot_arg<smart_ptr_raw<Structure> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<Structure *>::from(_FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc(__context__, arg_src));
}
static vec4f __wrap__FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8 ( Context * __context__ ) {
    *((AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *) __context__->abiCMRES) = _FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8(__context__);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickget_ptrTick5807679485210906136_eb77b9b7854f344 ( Context * __context__ ) {
    smart_ptr_raw<ExprBlock> arg_src = cast_aot_arg<smart_ptr_raw<ExprBlock>>::to(*__context__,__context__->abiArguments()[0]);
    return cast<ExprBlock *>::from(_FuncbuiltinTickget_ptrTick5807679485210906136_eb77b9b7854f344(__context__, arg_src));
}
static vec4f __wrap__FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259 ( Context * __context__ ) {
    smart_ptr_raw<Structure> arg_src = cast_aot_arg<smart_ptr_raw<Structure>>::to(*__context__,__context__->abiArguments()[0]);
    return cast<Structure *>::from(_FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259(__context__, arg_src));
}
static vec4f __wrap__Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_1233c5737f91b474 ( Context * __context__ ) {
    smart_ptr_raw<ExprAsVariant> const  arg_expr = cast_aot_arg<smart_ptr_raw<ExprAsVariant> const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Expression> arg_enull = cast_aot_arg<smart_ptr_raw<Expression>>::to(*__context__,__context__->abiArguments()[1]);
    return cast<smart_ptr_raw<ExprOp3>>::from(_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_1233c5737f91b474(__context__, arg_expr, arg_enull));
}
static vec4f __wrap__Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_4aa4fc3752456c49 ( Context * __context__ ) {
    smart_ptr_raw<ExprSafeAsVariant> const  arg_expr = cast_aot_arg<smart_ptr_raw<ExprSafeAsVariant> const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Expression> arg_enull = cast_aot_arg<smart_ptr_raw<Expression>>::to(*__context__,__context__->abiArguments()[1]);
    return cast<smart_ptr_raw<ExprOp3>>::from(_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_4aa4fc3752456c49(__context__, arg_expr, arg_enull));
}
static vec4f __wrap__FuncbuiltinTickget_ptrTick8468476673553620226_49b21ea86f1f3e71 ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_src = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<Annotation *>::from(_FuncbuiltinTickget_ptrTick8468476673553620226_49b21ea86f1f3e71(__context__, arg_src));
}
static vec4f __wrap_clone_23a052359607d993 ( Context * __context__ ) {
    smart_ptr_raw<TypeDecl> & arg_dest = cast_aot_arg<smart_ptr_raw<TypeDecl> &>::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_src = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    clone_23a052359607d993(__context__, arg_dest, arg_src);
    return v_zero();
}
static vec4f __wrap_clone_a8386ed128f4f0b4 ( Context * __context__ ) {
    smart_ptr_raw<Enumeration> & arg_dest = cast_aot_arg<smart_ptr_raw<Enumeration> &>::to(*__context__,__context__->abiArguments()[0]);
    Enumeration * const  arg_src = cast_aot_arg<Enumeration * const >::to(*__context__,__context__->abiArguments()[1]);
    clone_a8386ed128f4f0b4(__context__, arg_dest, arg_src);
    return v_zero();
}
static vec4f __wrap__FuncastTickfind_compiling_moduleTick4523452840392654583_584104531b17a816 ( Context * __context__ ) {
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<Module *>::from(_FuncastTickfind_compiling_moduleTick4523452840392654583_584104531b17a816(__context__, arg_name));
}
static vec4f __wrap_clone_6e9cdc5c6ca1d1f3 ( Context * __context__ ) {
    smart_ptr_raw<Annotation> & arg_dest = cast_aot_arg<smart_ptr_raw<Annotation> &>::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Annotation> const  arg_src = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[1]);
    clone_6e9cdc5c6ca1d1f3(__context__, arg_dest, arg_src);
    return v_zero();
}
static vec4f __wrap__FuncastTickdescribeTick842554968825501494_5630d72624d0f510 ( Context * __context__ ) {
    smart_ptr_raw<Expression> const  arg_expr = cast_aot_arg<smart_ptr_raw<Expression> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(_FuncastTickdescribeTick842554968825501494_5630d72624d0f510(__context__, arg_expr));
}
static vec4f __wrap__FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16 ( Context * __context__ ) {
    smart_ptr_raw<Expression> const  arg_src = cast_aot_arg<smart_ptr_raw<Expression> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<Expression *>::from(_FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16(__context__, arg_src));
}
static vec4f __wrap_isVectorType_34a98665df73cc6f ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_typ = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(isVectorType_34a98665df73cc6f(__context__, arg_typ));
}
static vec4f __wrap_describe_670b9c27dbedaf8f ( Context * __context__ ) {
    AnnotationArgumentList const  &  arg_list = cast_aot_arg<AnnotationArgumentList const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(describe_670b9c27dbedaf8f(__context__, arg_list));
}
static vec4f __wrap_describe_19857d293154313d ( Context * __context__ ) {
    AnnotationDeclaration const  &  arg_ann = cast_aot_arg<AnnotationDeclaration const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(describe_19857d293154313d(__context__, arg_ann));
}
static vec4f __wrap_describe_d0f1c4ce2af40b5e ( Context * __context__ ) {
    AnnotationList const  &  arg_list = cast_aot_arg<AnnotationList const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(describe_d0f1c4ce2af40b5e(__context__, arg_list));
}
static vec4f __wrap_describe_24ebe982465d57fc ( Context * __context__ ) {
    smart_ptr_raw<Variable> const  arg_vvar = cast_aot_arg<smart_ptr_raw<Variable> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(describe_24ebe982465d57fc(__context__, arg_vvar));
}
static vec4f __wrap_isExpression_f1fadf50b6070b86 ( Context * __context__ ) {
    smart_ptr_raw<TypeDecl> const  arg_t = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[0]);
    bool arg_top = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(isExpression_f1fadf50b6070b86(__context__, arg_t, arg_top));
}
static vec4f __wrap_is_same_or_inherited_530410b8a7a25edb ( Context * __context__ ) {
    Structure const  * const  arg_parent = cast_aot_arg<Structure const  * const >::to(*__context__,__context__->abiArguments()[0]);
    Structure const  * const  arg_child = cast_aot_arg<Structure const  * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(is_same_or_inherited_530410b8a7a25edb(__context__, arg_parent, arg_child));
}
static vec4f __wrap_is_class_method_4783315be6e05a7b ( Context * __context__ ) {
    smart_ptr_raw<Structure> const  arg_cinfo = cast_aot_arg<smart_ptr_raw<Structure> const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_finfo = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(is_class_method_4783315be6e05a7b(__context__, arg_cinfo, arg_finfo));
}
static vec4f __wrap_emplace_new_9bef84999dd1e3f4 ( Context * __context__ ) {
    das::vector<smart_ptr<Expression>> &  arg_vec = cast_aot_arg<das::vector<smart_ptr<Expression>> & >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Expression> arg_ptr = cast_aot_arg<smart_ptr_raw<Expression>>::to(*__context__,__context__->abiArguments()[1]);
    emplace_new_9bef84999dd1e3f4(__context__, arg_vec, arg_ptr);
    return v_zero();
}
static vec4f __wrap_emplace_new_7386d366d948387e ( Context * __context__ ) {
    das::vector<smart_ptr<TypeDecl>> &  arg_vec = cast_aot_arg<das::vector<smart_ptr<TypeDecl>> & >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> arg_ptr = cast_aot_arg<smart_ptr_raw<TypeDecl>>::to(*__context__,__context__->abiArguments()[1]);
    emplace_new_7386d366d948387e(__context__, arg_vec, arg_ptr);
    return v_zero();
}
static vec4f __wrap_emplace_new_2f0cb94c081aada2 ( Context * __context__ ) {
    das::vector<smart_ptr<Variable>> &  arg_vec = cast_aot_arg<das::vector<smart_ptr<Variable>> & >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Variable> arg_ptr = cast_aot_arg<smart_ptr_raw<Variable>>::to(*__context__,__context__->abiArguments()[1]);
    emplace_new_2f0cb94c081aada2(__context__, arg_vec, arg_ptr);
    return v_zero();
}
static vec4f __wrap_emplace_new_7782fb9c58748acd ( Context * __context__ ) {
    MakeStruct &  arg_vec = cast_aot_arg<MakeStruct & >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<MakeFieldDecl> arg_ptr = cast_aot_arg<smart_ptr_raw<MakeFieldDecl>>::to(*__context__,__context__->abiArguments()[1]);
    emplace_new_7782fb9c58748acd(__context__, arg_vec, arg_ptr);
    return v_zero();
}
static vec4f __wrap_override_method_49b0943e987177d7 ( Context * __context__ ) {
    smart_ptr_raw<Structure> arg_str = cast_aot_arg<smart_ptr_raw<Structure>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_funcName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<bool>::from(override_method_49b0943e987177d7(__context__, arg_str, arg_name, arg_funcName));
}
static vec4f __wrap_for_each_tag_function_f441072c0eb499ad ( Context * __context__ ) {
    Module * const  arg_mod = cast_aot_arg<Module * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_tag = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  arg_blk = cast_aot_arg<Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & >::to(*__context__,__context__->abiArguments()[2]);
    for_each_tag_function_f441072c0eb499ad(__context__, arg_mod, arg_tag, arg_blk);
    return v_zero();
}
static vec4f __wrap_find_arg_a0e87cbd443f10f5 ( Context * __context__ ) {
    char * const  arg_argn = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    AnnotationArgumentList const  &  arg_args = cast_aot_arg<AnnotationArgumentList const  & >::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *) __context__->abiCMRES) = find_arg_a0e87cbd443f10f5(__context__, arg_argn, arg_args);
    return v_zero();
}
static vec4f __wrap_find_arg_eefed45a6ebf3b9 ( Context * __context__ ) {
    AnnotationArgumentList const  &  arg_args = cast_aot_arg<AnnotationArgumentList const  & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argn = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *) __context__->abiCMRES) = find_arg_eefed45a6ebf3b9(__context__, arg_args, arg_argn);
    return v_zero();
}
static vec4f __wrap_apply_tag_annotation_1944b9a89fd11326 ( Context * __context__ ) {
    char * const  arg_tag = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<FunctionAnnotation> arg_ann = cast_aot_arg<smart_ptr_raw<FunctionAnnotation>>::to(*__context__,__context__->abiArguments()[1]);
    apply_tag_annotation_1944b9a89fd11326(__context__, arg_tag, arg_ann);
    return v_zero();
}
static vec4f __wrap_find_unique_function_e26f474c35febe1d ( Context * __context__ ) {
    Module * const  arg_mod = cast_aot_arg<Module * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    bool arg_canfail = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Function>>::from(find_unique_function_e26f474c35febe1d(__context__, arg_mod, arg_name, arg_canfail));
}
static vec4f __wrap_find_unique_generic_22c96d795ba15b85 ( Context * __context__ ) {
    Module * const  arg_mod = cast_aot_arg<Module * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    bool arg_canfail = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Function>>::from(find_unique_generic_22c96d795ba15b85(__context__, arg_mod, arg_name, arg_canfail));
}
static vec4f __wrap_setup_call_list_329404b139e4adba ( Context * __context__ ) {
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[1]);
    Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  arg_subblock = cast_aot_arg<Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<ExprBlock *>::from(setup_call_list_329404b139e4adba(__context__, arg_name, arg_at, arg_subblock));
}
static vec4f __wrap_setup_call_list_8438ddab4dff539b ( Context * __context__ ) {
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[1]);
    bool arg_isInit = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    bool arg_isPrivate = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[3]);
    bool arg_isLateInit = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[4]);
    return cast<ExprBlock *>::from(setup_call_list_8438ddab4dff539b(__context__, arg_name, arg_at, arg_isInit, arg_isPrivate, arg_isLateInit));
}
static vec4f __wrap_setup_macro_979cb6d902caaebd ( Context * __context__ ) {
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<ExprBlock *>::from(setup_macro_979cb6d902caaebd(__context__, arg_name, arg_at));
}
static vec4f __wrap_panic_expr_as_2ea20f674c842c9c ( Context * __context__ ) {
    return cast<void *>::from(panic_expr_as_2ea20f674c842c9c(__context__));
}
static vec4f __wrap__FuncTickisTickBuiltInFunction_3d26f4806589821b ( Context * __context__ ) {
    Function * const  arg_foo = cast_aot_arg<Function * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickBuiltInFunction_3d26f4806589821b(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickBuiltInFunction_c47ad3b03c9bc354 ( Context * __context__ ) {
    Function * const  arg_foo = cast_aot_arg<Function * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<BuiltInFunction *>::from(_FuncTickasTickBuiltInFunction_c47ad3b03c9bc354(__context__, arg_foo));
}
static vec4f __wrap__FuncTickisTickExternalFnBase_24b69557e6b14161 ( Context * __context__ ) {
    Function * const  arg_foo = cast_aot_arg<Function * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickExternalFnBase_24b69557e6b14161(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickExternalFnBase_b840ea32468fb731 ( Context * __context__ ) {
    Function * const  arg_foo = cast_aot_arg<Function * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<ExternalFnBase *>::from(_FuncTickasTickExternalFnBase_b840ea32468fb731(__context__, arg_foo));
}
static vec4f __wrap__FuncTickisTickFunctionAnnotation_bf985c1f9a8f8112 ( Context * __context__ ) {
    Annotation * const  arg_foo = cast_aot_arg<Annotation * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickFunctionAnnotation_bf985c1f9a8f8112(__context__, arg_foo));
}
static vec4f __wrap__FuncTickisTickFunctionAnnotation_88a1558057f6b81f ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_foo = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickFunctionAnnotation_88a1558057f6b81f(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickFunctionAnnotation_8812613c169a9dba ( Context * __context__ ) {
    Annotation * const  arg_foo = cast_aot_arg<Annotation * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<FunctionAnnotation *>::from(_FuncTickasTickFunctionAnnotation_8812613c169a9dba(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickFunctionAnnotation_89843e969c415f46 ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_foo = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<FunctionAnnotation *>::from(_FuncTickasTickFunctionAnnotation_89843e969c415f46(__context__, arg_foo));
}
static vec4f __wrap__FuncTickisTickStructureAnnotation_a9f69a8ea44dcd1b ( Context * __context__ ) {
    Annotation * const  arg_foo = cast_aot_arg<Annotation * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickStructureAnnotation_a9f69a8ea44dcd1b(__context__, arg_foo));
}
static vec4f __wrap__FuncTickisTickStructureAnnotation_25f742c66283203c ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_foo = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickStructureAnnotation_25f742c66283203c(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickStructureAnnotation_dac337e2d083c291 ( Context * __context__ ) {
    Annotation * const  arg_foo = cast_aot_arg<Annotation * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructureAnnotation *>::from(_FuncTickasTickStructureAnnotation_dac337e2d083c291(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickStructureAnnotation_b43078c41d1f685 ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_foo = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructureAnnotation *>::from(_FuncTickasTickStructureAnnotation_b43078c41d1f685(__context__, arg_foo));
}
static vec4f __wrap_walk_and_convert_array_ba1cc70e35611268 ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_array_ba1cc70e35611268(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_walk_and_convert_dim_9001b9148a98cff6 ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_dim_9001b9148a98cff6(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_walk_and_convert_pointer_47b794d0dd59337d ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_pointer_47b794d0dd59337d(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_walk_and_convert_tuple_9ce3b13721ba5daf ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_tuple_9ce3b13721ba5daf(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_walk_and_convert_structure_106d0a1bf460f63c ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_structure_106d0a1bf460f63c(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_walk_and_convert_variant_cdeba207e46113d9 ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_variant_cdeba207e46113d9(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_walk_and_convert_table_bafed583a64bc3a1 ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_table_bafed583a64bc3a1(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_walk_and_convert_basic_5a2f4f9254235414 ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_basic_5a2f4f9254235414(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_walk_and_convert_enumeration_5bb2d478c4ffeaaa ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_enumeration_5bb2d478c4ffeaaa(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_walk_and_convert_cfdd8313a6a5987 ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_cfdd8313a6a5987(__context__, arg_data, arg_info, arg_at));
}
static vec4f __wrap_find_annotation_e87941f09ff8d46c ( Context * __context__ ) {
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<Annotation const  *>::from(find_annotation_e87941f09ff8d46c(__context__, arg_mod_name, arg_ann_name));
}
static vec4f __wrap_append_annotation_79e01888a17a2cf8 ( Context * __context__ ) {
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  arg_args = cast_aot_arg<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<AnnotationDeclaration>>::from(append_annotation_79e01888a17a2cf8(__context__, arg_mod_name, arg_ann_name, arg_args));
}
static vec4f __wrap_append_annotation_91779d544c0857a9 ( Context * __context__ ) {
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<smart_ptr_raw<AnnotationDeclaration>>::from(append_annotation_91779d544c0857a9(__context__, arg_mod_name, arg_ann_name));
}
static vec4f __wrap_append_annotation_9e9720e2bdf176ae ( Context * __context__ ) {
    smart_ptr_raw<Function> arg_func = cast_aot_arg<smart_ptr_raw<Function>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    append_annotation_9e9720e2bdf176ae(__context__, arg_func, arg_mod_name, arg_ann_name);
    return v_zero();
}
static vec4f __wrap_append_annotation_4e3ac504625b9e59 ( Context * __context__ ) {
    smart_ptr_raw<ExprBlock> arg_blk = cast_aot_arg<smart_ptr_raw<ExprBlock>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    append_annotation_4e3ac504625b9e59(__context__, arg_blk, arg_mod_name, arg_ann_name);
    return v_zero();
}
static vec4f __wrap_append_annotation_1d124a466ae78997 ( Context * __context__ ) {
    smart_ptr_raw<Structure> arg_st = cast_aot_arg<smart_ptr_raw<Structure>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    append_annotation_1d124a466ae78997(__context__, arg_st, arg_mod_name, arg_ann_name);
    return v_zero();
}
static vec4f __wrap_append_annotation_d2aadccfb018501d ( Context * __context__ ) {
    smart_ptr_raw<Function> arg_func = cast_aot_arg<smart_ptr_raw<Function>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  arg_args = cast_aot_arg<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & >::to(*__context__,__context__->abiArguments()[3]);
    append_annotation_d2aadccfb018501d(__context__, arg_func, arg_mod_name, arg_ann_name, arg_args);
    return v_zero();
}
static vec4f __wrap_append_annotation_4239f8097567c00 ( Context * __context__ ) {
    smart_ptr_raw<ExprBlock> arg_blk = cast_aot_arg<smart_ptr_raw<ExprBlock>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  arg_args = cast_aot_arg<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & >::to(*__context__,__context__->abiArguments()[3]);
    append_annotation_4239f8097567c00(__context__, arg_blk, arg_mod_name, arg_ann_name, arg_args);
    return v_zero();
}
static vec4f __wrap_append_annotation_84797c2112b4fd76 ( Context * __context__ ) {
    smart_ptr_raw<Structure> arg_st = cast_aot_arg<smart_ptr_raw<Structure>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  arg_args = cast_aot_arg<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & >::to(*__context__,__context__->abiArguments()[3]);
    append_annotation_84797c2112b4fd76(__context__, arg_st, arg_mod_name, arg_ann_name, arg_args);
    return v_zero();
}
static vec4f __wrap_add_annotation_argument_ecdc7f01cc8d28cd ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    bool arg_val = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(add_annotation_argument_ecdc7f01cc8d28cd(__context__, arg_arguments, arg_argName, arg_val));
}
static vec4f __wrap_add_annotation_argument_5d1cfc8d958b1b74 ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_val = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(add_annotation_argument_5d1cfc8d958b1b74(__context__, arg_arguments, arg_argName, arg_val));
}
static vec4f __wrap_add_annotation_argument_2ba02ec1a95c40de ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    float arg_val = cast_aot_arg<float>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(add_annotation_argument_2ba02ec1a95c40de(__context__, arg_arguments, arg_argName, arg_val));
}
static vec4f __wrap_add_annotation_argument_e01dc466768bdd13 ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_val = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(add_annotation_argument_e01dc466768bdd13(__context__, arg_arguments, arg_argName, arg_val));
}
static vec4f __wrap_add_annotation_argument_e7f221e58ec46512 ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    AnnotationArgument const  &  arg_ann = cast_aot_arg<AnnotationArgument const  & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(add_annotation_argument_e7f221e58ec46512(__context__, arg_arguments, arg_ann));
}
static vec4f __wrap_get_for_source_index_19e9c390ee4338e2 ( Context * __context__ ) {
    smart_ptr_raw<ExprFor> const  arg_expr = cast_aot_arg<smart_ptr_raw<ExprFor> const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Variable> const  arg_svar = cast_aot_arg<smart_ptr_raw<Variable> const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(get_for_source_index_19e9c390ee4338e2(__context__, arg_expr, arg_svar));
}
static vec4f __wrap_get_for_source_index_e425541a2a706c53 ( Context * __context__ ) {
    smart_ptr_raw<ExprFor> const  arg_expr = cast_aot_arg<smart_ptr_raw<ExprFor> const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Expression> const  arg_source = cast_aot_arg<smart_ptr_raw<Expression> const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(get_for_source_index_e425541a2a706c53(__context__, arg_expr, arg_source));
}
static vec4f __wrap_function_to_type_1f450dac98d6071d ( Context * __context__ ) {
    smart_ptr_raw<Function> const  arg_fn = cast_aot_arg<smart_ptr_raw<Function> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<smart_ptr_raw<TypeDecl>>::from(function_to_type_1f450dac98d6071d(__context__, arg_fn));
}
static vec4f __wrap_visit_finally_d773bb44db182978 ( Context * __context__ ) {
    ExprBlock * const  arg_blk = cast_aot_arg<ExprBlock * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<VisitorAdapter> const  arg_adapter = cast_aot_arg<smart_ptr_raw<VisitorAdapter> const >::to(*__context__,__context__->abiArguments()[1]);
    visit_finally_d773bb44db182978(__context__, arg_blk, arg_adapter);
    return v_zero();
}
static vec4f __wrap_isCMRES_2bb438cbd738593f ( Context * __context__ ) {
    smart_ptr_raw<Function> const  arg_fun = cast_aot_arg<smart_ptr_raw<Function> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(isCMRES_2bb438cbd738593f(__context__, arg_fun));
}
static vec4f __wrap_isCMRES_45d360720945cc74 ( Context * __context__ ) {
    Function * const  arg_fun = cast_aot_arg<Function * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(isCMRES_45d360720945cc74(__context__, arg_fun));
}
static vec4f __wrap_isMakeLocal_c461b44592b8facf ( Context * __context__ ) {
    smart_ptr_raw<Expression> const  arg_expr = cast_aot_arg<smart_ptr_raw<Expression> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(isMakeLocal_c461b44592b8facf(__context__, arg_expr));
}
static vec4f __wrap_isExprCallFunc_aac1b10a17185ef1 ( Context * __context__ ) {
    smart_ptr_raw<Expression> const  arg_expr = cast_aot_arg<smart_ptr_raw<Expression> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(isExprCallFunc_aac1b10a17185ef1(__context__, arg_expr));
}
static vec4f __wrap_get_workhorse_types_62f1d76706bcc1ad ( Context * __context__ ) {
    *((TDim<DAS_COMMENT(bound_enum) das::Type,31> *) __context__->abiCMRES) = get_workhorse_types_62f1d76706bcc1ad(__context__);
    return v_zero();
}
static vec4f __wrap_find_argument_index_3d6a951c2870d00f ( Context * __context__ ) {
    smart_ptr_raw<TypeDecl> const  arg_typ = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(find_argument_index_3d6a951c2870d00f(__context__, arg_typ, arg_name));
}
static vec4f __wrap_isCMRESType_bbf11aadb0354b4d ( Context * __context__ ) {
    smart_ptr_raw<TypeDecl> const  arg_blockT = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(isCMRESType_bbf11aadb0354b4d(__context__, arg_blockT));
}
static vec4f __wrap_debug_expression_impl_203941aeed67e19a ( Context * __context__ ) {
    StringBuilderWriter &  arg_writer = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Expression> const  arg_expr = cast_aot_arg<smart_ptr_raw<Expression> const >::to(*__context__,__context__->abiArguments()[1]);
    Bitfield arg_deFlags = cast_aot_arg<Bitfield>::to(*__context__,__context__->abiArguments()[2]);
    int32_t arg_tabs = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[3]);
    debug_expression_impl_203941aeed67e19a(__context__, arg_writer, arg_expr, arg_deFlags, arg_tabs);
    return v_zero();
}
static vec4f __wrap_debug_expression_72d312ce8f3ec686 ( Context * __context__ ) {
    smart_ptr_raw<Expression> const  arg_expr = cast_aot_arg<smart_ptr_raw<Expression> const >::to(*__context__,__context__->abiArguments()[0]);
    Bitfield arg_deFlags = cast_aot_arg<Bitfield>::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(debug_expression_72d312ce8f3ec686(__context__, arg_expr, arg_deFlags));
}
static vec4f __wrap_debug_expression_84991cbecc0ca3af ( Context * __context__ ) {
    Expression * const  arg_expr = cast_aot_arg<Expression * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(debug_expression_84991cbecc0ca3af(__context__, arg_expr));
}
static vec4f __wrap_describe_d11036fb3823617b ( Context * __context__ ) {
    Expression * const  arg_expr = cast_aot_arg<Expression * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(describe_d11036fb3823617b(__context__, arg_expr));
}
static vec4f __wrap_getVectorElementCount_6156018073f7b991 ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_bt = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(getVectorElementCount_6156018073f7b991(__context__, arg_bt));
}
static vec4f __wrap_getVectorElementSize_7fe52b138a9b94ac ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_bt = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(getVectorElementSize_7fe52b138a9b94ac(__context__, arg_bt));
}
static vec4f __wrap_getVectorElementType_17ab3a94ab7f2835 ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_bt = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    return cast<DAS_COMMENT(bound_enum) das::Type>::from(getVectorElementType_17ab3a94ab7f2835(__context__, arg_bt));
}
static vec4f __wrap_getVectorOffset_cb21e53271916e73 ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_bt = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_ident = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(getVectorOffset_cb21e53271916e73(__context__, arg_bt, arg_ident));
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0x64a22d29ec30453c, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_532aa57ee748e9, &__wrap__FuncbuiltinTickpushTick10769833213962245646_532aa57ee748e9 },
    { 0xd35f33788bc9102e, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_5ea400cd583decff, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_5ea400cd583decff },
    { 0x9ee3ddbe8168281c, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_ea422ce758633219, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_ea422ce758633219 },
    { 0xf45e989398bfef1c, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_8072bcb5f8834d95, &__wrap__FuncbuiltinTickpushTick10769833213962245646_8072bcb5f8834d95 },
    { 0x551effecf5ab663, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_3d92632ea1c06f10, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_3d92632ea1c06f10 },
    { 0xfa1a41c41161357f, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_d3ab2c8dbad9d969, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_d3ab2c8dbad9d969 },
    { 0xd15d092d70c82d7e, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_3ef2f296a0f69015, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_3ef2f296a0f69015 },
    { 0xda25973bf5792564, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_7910128189d4fa7c, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_7910128189d4fa7c },
    { 0xd0ff25bf11b9ad19, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_7c1880e164f77687, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_7c1880e164f77687 },
    { 0xfebfd7d459158c0a, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_cf248f7c72f6077b, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_cf248f7c72f6077b },
    { 0xd54456cab6397f29, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_1e3a40dee0adc6b4, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_1e3a40dee0adc6b4 },
    { 0xb095f9004180ba72, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_688d5b5d381c2412, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_688d5b5d381c2412 },
    { 0x36f73a3c192599c3, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_5ad54dc005365540, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_5ad54dc005365540 },
    { 0xc4eb46be63bb7acc, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_763eae1f51cae115, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_763eae1f51cae115 },
    { 0xa9006f6cede42def, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_c247eb3e087743a6, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_c247eb3e087743a6 },
    { 0xa86e2a4cb40ed5d0, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_518dfc03923aac9d, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_518dfc03923aac9d },
    { 0x24ec42fd20ba2475, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_38a2fc738f0684a7, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_38a2fc738f0684a7 },
    { 0xaa5c88db8b90caac, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_b87e7c5f8f5ce3c1, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_b87e7c5f8f5ce3c1 },
    { 0x2c28da0a211a84f2, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_cf0d1eb6392a85d3, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_cf0d1eb6392a85d3 },
    { 0x3d95b97169b56e64, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_9f95af835ba924bb, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_9f95af835ba924bb },
    { 0xe2da5828bb0c45de, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_c1b034570b4506a1, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_c1b034570b4506a1 },
    { 0x5cda24eda8b3a753, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_c4de1f05abb2f4d4, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_c4de1f05abb2f4d4 },
    { 0x6e674039d1bcd2ed, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_c4ac869cc1fdebdd, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_c4ac869cc1fdebdd },
    { 0xd17a9ac49f2e5843, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_99a4cf4ce09d9435, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_99a4cf4ce09d9435 },
    { 0x2a38765b9a139f39, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_cb4bdd4ed7a9d0f5, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_cb4bdd4ed7a9d0f5 },
    { 0x2e703291c261d461, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_46d64d80c21524e3, &__wrap__FuncbuiltinTickpushTick10769833213962245646_46d64d80c21524e3 },
    { 0x66f4f827c4d1cbed, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_7534c9a9e0721709, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_7534c9a9e0721709 },
    { 0xdbd3c0b15db539b8, false, (void*)&clone_7f4a34d879fc35f4, &__wrap_clone_7f4a34d879fc35f4 },
    { 0xafbc8ff04895ffe, false, (void*)&_FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0, &__wrap__FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0 },
    { 0x6fdfe306e561605b, false, (void*)&_FuncastTickfind_moduleTick11101329256228773934_92a5dd08b29912b8, &__wrap__FuncastTickfind_moduleTick11101329256228773934_92a5dd08b29912b8 },
    { 0x468a9f289eaed4d0, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f, &__wrap__FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f },
    { 0x61c57fcc3ac50491, false, (void*)&_Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9, &__wrap__Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9 },
    { 0x4a9a547cf73d3b9c, false, (void*)&_FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49, &__wrap__FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 },
    { 0x95370060fd3b30b4, false, (void*)&_FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec, &__wrap__FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec },
    { 0x81ff2acd679f7b49, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2 },
    { 0x820f84ac63af384a, false, (void*)&clone_e5dbb6c9a183080e, &__wrap_clone_e5dbb6c9a183080e },
    { 0x239a2d8977dd3bb6, false, (void*)&_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5, &__wrap__FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5 },
    { 0xf1db16111fe463d3, false, (void*)&_FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc, &__wrap__FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc },
    { 0x251a5d8d979cd19c, true, (void*)&_FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8, &__wrap__FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8 },
    { 0x486be281eea55ccf, false, (void*)&_FuncbuiltinTickget_ptrTick5807679485210906136_eb77b9b7854f344, &__wrap__FuncbuiltinTickget_ptrTick5807679485210906136_eb77b9b7854f344 },
    { 0x78044e70d5c1011e, false, (void*)&_FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259, &__wrap__FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259 },
    { 0x899f0065051b7e2d, false, (void*)&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_1233c5737f91b474, &__wrap__Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_1233c5737f91b474 },
    { 0xcd6021d2e2039a60, false, (void*)&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_4aa4fc3752456c49, &__wrap__Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_4aa4fc3752456c49 },
    { 0x7853985fd2fc806e, false, (void*)&_FuncbuiltinTickget_ptrTick8468476673553620226_49b21ea86f1f3e71, &__wrap__FuncbuiltinTickget_ptrTick8468476673553620226_49b21ea86f1f3e71 },
    { 0x194206eb303c1a9e, false, (void*)&clone_23a052359607d993, &__wrap_clone_23a052359607d993 },
    { 0xe73c490261748131, false, (void*)&clone_a8386ed128f4f0b4, &__wrap_clone_a8386ed128f4f0b4 },
    { 0xb9e623d44f316f31, false, (void*)&_FuncastTickfind_compiling_moduleTick4523452840392654583_584104531b17a816, &__wrap__FuncastTickfind_compiling_moduleTick4523452840392654583_584104531b17a816 },
    { 0x1d4cbeb85198804a, false, (void*)&clone_6e9cdc5c6ca1d1f3, &__wrap_clone_6e9cdc5c6ca1d1f3 },
    { 0x9a6855251932c74e, false, (void*)&_FuncastTickdescribeTick842554968825501494_5630d72624d0f510, &__wrap__FuncastTickdescribeTick842554968825501494_5630d72624d0f510 },
    { 0x53906f3b10d1dcc7, false, (void*)&_FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16, &__wrap__FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16 },
    { 0xab7c1b3930219b1f, false, (void*)&isVectorType_34a98665df73cc6f, &__wrap_isVectorType_34a98665df73cc6f },
    { 0xc6448130ef5e1670, false, (void*)&describe_670b9c27dbedaf8f, &__wrap_describe_670b9c27dbedaf8f },
    { 0x40ec5feb74c36646, false, (void*)&describe_19857d293154313d, &__wrap_describe_19857d293154313d },
    { 0x6fc6e7ea8cf213bc, false, (void*)&describe_d0f1c4ce2af40b5e, &__wrap_describe_d0f1c4ce2af40b5e },
    { 0xf1dac4b5f5ff4237, false, (void*)&describe_24ebe982465d57fc, &__wrap_describe_24ebe982465d57fc },
    { 0xddfd46ce09e77814, false, (void*)&isExpression_f1fadf50b6070b86, &__wrap_isExpression_f1fadf50b6070b86 },
    { 0xbd8ff2ba4682c1d1, false, (void*)&is_same_or_inherited_530410b8a7a25edb, &__wrap_is_same_or_inherited_530410b8a7a25edb },
    { 0xf5f00b9f471ecf5a, false, (void*)&is_class_method_4783315be6e05a7b, &__wrap_is_class_method_4783315be6e05a7b },
    { 0xcffc555531a07b4a, false, (void*)&emplace_new_9bef84999dd1e3f4, &__wrap_emplace_new_9bef84999dd1e3f4 },
    { 0xb8b824dc63822ecd, false, (void*)&emplace_new_7386d366d948387e, &__wrap_emplace_new_7386d366d948387e },
    { 0x11f59ebb24eecdd0, false, (void*)&emplace_new_2f0cb94c081aada2, &__wrap_emplace_new_2f0cb94c081aada2 },
    { 0x21ac2b8ae20cd55d, false, (void*)&emplace_new_7782fb9c58748acd, &__wrap_emplace_new_7782fb9c58748acd },
    { 0x3b0693d191865e81, false, (void*)&override_method_49b0943e987177d7, &__wrap_override_method_49b0943e987177d7 },
    { 0x24a9672b484098b1, false, (void*)&for_each_tag_function_f441072c0eb499ad, &__wrap_for_each_tag_function_f441072c0eb499ad },
    { 0x4f9d4708c18654fb, true, (void*)&find_arg_a0e87cbd443f10f5, &__wrap_find_arg_a0e87cbd443f10f5 },
    { 0x23808c7fceed1880, true, (void*)&find_arg_eefed45a6ebf3b9, &__wrap_find_arg_eefed45a6ebf3b9 },
    { 0x68b5d271ade43c18, false, (void*)&apply_tag_annotation_1944b9a89fd11326, &__wrap_apply_tag_annotation_1944b9a89fd11326 },
    { 0xf88bcde011d31ce2, false, (void*)&find_unique_function_e26f474c35febe1d, &__wrap_find_unique_function_e26f474c35febe1d },
    { 0xa9397b39c4a4cc99, false, (void*)&find_unique_generic_22c96d795ba15b85, &__wrap_find_unique_generic_22c96d795ba15b85 },
    { 0xd67b115fef065026, false, (void*)&setup_call_list_329404b139e4adba, &__wrap_setup_call_list_329404b139e4adba },
    { 0xaa4cb6b79ee38f60, false, (void*)&setup_call_list_8438ddab4dff539b, &__wrap_setup_call_list_8438ddab4dff539b },
    { 0xc8eeb6053d5ec01a, false, (void*)&setup_macro_979cb6d902caaebd, &__wrap_setup_macro_979cb6d902caaebd },
    { 0x5e3abef4280895c3, false, (void*)&panic_expr_as_2ea20f674c842c9c, &__wrap_panic_expr_as_2ea20f674c842c9c },
    { 0x88e42da7cebd28ac, false, (void*)&_FuncTickisTickBuiltInFunction_3d26f4806589821b, &__wrap__FuncTickisTickBuiltInFunction_3d26f4806589821b },
    { 0xf4c92df79648f1cb, false, (void*)&_FuncTickasTickBuiltInFunction_c47ad3b03c9bc354, &__wrap__FuncTickasTickBuiltInFunction_c47ad3b03c9bc354 },
    { 0x1bf1a475e699701a, false, (void*)&_FuncTickisTickExternalFnBase_24b69557e6b14161, &__wrap__FuncTickisTickExternalFnBase_24b69557e6b14161 },
    { 0x55a228b61d8f1c13, false, (void*)&_FuncTickasTickExternalFnBase_b840ea32468fb731, &__wrap__FuncTickasTickExternalFnBase_b840ea32468fb731 },
    { 0x19bfebf1fe9d558a, false, (void*)&_FuncTickisTickFunctionAnnotation_bf985c1f9a8f8112, &__wrap__FuncTickisTickFunctionAnnotation_bf985c1f9a8f8112 },
    { 0x24647544f269ceb6, false, (void*)&_FuncTickisTickFunctionAnnotation_88a1558057f6b81f, &__wrap__FuncTickisTickFunctionAnnotation_88a1558057f6b81f },
    { 0xf6a74038b3a80b3, false, (void*)&_FuncTickasTickFunctionAnnotation_8812613c169a9dba, &__wrap__FuncTickasTickFunctionAnnotation_8812613c169a9dba },
    { 0x2278ae479c0e00e4, false, (void*)&_FuncTickasTickFunctionAnnotation_89843e969c415f46, &__wrap__FuncTickasTickFunctionAnnotation_89843e969c415f46 },
    { 0xd99117159f2f4226, false, (void*)&_FuncTickisTickStructureAnnotation_a9f69a8ea44dcd1b, &__wrap__FuncTickisTickStructureAnnotation_a9f69a8ea44dcd1b },
    { 0xe357bc3c75325fa1, false, (void*)&_FuncTickisTickStructureAnnotation_25f742c66283203c, &__wrap__FuncTickisTickStructureAnnotation_25f742c66283203c },
    { 0x32e4c121b85a5a71, false, (void*)&_FuncTickasTickStructureAnnotation_dac337e2d083c291, &__wrap__FuncTickasTickStructureAnnotation_dac337e2d083c291 },
    { 0x8038e18c674e6b1d, false, (void*)&_FuncTickasTickStructureAnnotation_b43078c41d1f685, &__wrap__FuncTickasTickStructureAnnotation_b43078c41d1f685 },
    { 0xab7649f5835491b4, false, (void*)&walk_and_convert_array_ba1cc70e35611268, &__wrap_walk_and_convert_array_ba1cc70e35611268 },
    { 0xb7d10319c2b53388, false, (void*)&walk_and_convert_dim_9001b9148a98cff6, &__wrap_walk_and_convert_dim_9001b9148a98cff6 },
    { 0x856905e6b62acaa1, false, (void*)&walk_and_convert_pointer_47b794d0dd59337d, &__wrap_walk_and_convert_pointer_47b794d0dd59337d },
    { 0x64a86d4c08dbadf4, false, (void*)&walk_and_convert_tuple_9ce3b13721ba5daf, &__wrap_walk_and_convert_tuple_9ce3b13721ba5daf },
    { 0x72807e195c10bd46, false, (void*)&walk_and_convert_structure_106d0a1bf460f63c, &__wrap_walk_and_convert_structure_106d0a1bf460f63c },
    { 0x5eaba0878f3d0cdd, false, (void*)&walk_and_convert_variant_cdeba207e46113d9, &__wrap_walk_and_convert_variant_cdeba207e46113d9 },
    { 0xb51cde7cf7f5a16b, false, (void*)&walk_and_convert_table_bafed583a64bc3a1, &__wrap_walk_and_convert_table_bafed583a64bc3a1 },
    { 0xcc56a9e7a6fb093d, false, (void*)&walk_and_convert_basic_5a2f4f9254235414, &__wrap_walk_and_convert_basic_5a2f4f9254235414 },
    { 0x4c0a3a42f7d66fd7, false, (void*)&walk_and_convert_enumeration_5bb2d478c4ffeaaa, &__wrap_walk_and_convert_enumeration_5bb2d478c4ffeaaa },
    { 0x39d5a3dc4b740150, false, (void*)&walk_and_convert_cfdd8313a6a5987, &__wrap_walk_and_convert_cfdd8313a6a5987 },
    { 0x6a515cbf1ddb767b, false, (void*)&find_annotation_e87941f09ff8d46c, &__wrap_find_annotation_e87941f09ff8d46c },
    { 0xcd53f5371dfcda79, false, (void*)&append_annotation_79e01888a17a2cf8, &__wrap_append_annotation_79e01888a17a2cf8 },
    { 0x1217abc8ec5a36d8, false, (void*)&append_annotation_91779d544c0857a9, &__wrap_append_annotation_91779d544c0857a9 },
    { 0xa43e9b9d08eddf9b, false, (void*)&append_annotation_9e9720e2bdf176ae, &__wrap_append_annotation_9e9720e2bdf176ae },
    { 0x37bb8551622af90c, false, (void*)&append_annotation_4e3ac504625b9e59, &__wrap_append_annotation_4e3ac504625b9e59 },
    { 0x9a9c8bf45015a224, false, (void*)&append_annotation_1d124a466ae78997, &__wrap_append_annotation_1d124a466ae78997 },
    { 0xb992aaf952dc76cb, false, (void*)&append_annotation_d2aadccfb018501d, &__wrap_append_annotation_d2aadccfb018501d },
    { 0xfbcf8563cec4e727, false, (void*)&append_annotation_4239f8097567c00, &__wrap_append_annotation_4239f8097567c00 },
    { 0x4978a414698efb42, false, (void*)&append_annotation_84797c2112b4fd76, &__wrap_append_annotation_84797c2112b4fd76 },
    { 0x392e1292027c3c8a, false, (void*)&add_annotation_argument_ecdc7f01cc8d28cd, &__wrap_add_annotation_argument_ecdc7f01cc8d28cd },
    { 0x590d84e3ae59d97a, false, (void*)&add_annotation_argument_5d1cfc8d958b1b74, &__wrap_add_annotation_argument_5d1cfc8d958b1b74 },
    { 0xf91b2dbd2b583338, false, (void*)&add_annotation_argument_2ba02ec1a95c40de, &__wrap_add_annotation_argument_2ba02ec1a95c40de },
    { 0xbab7eac1aaaaf0b8, false, (void*)&add_annotation_argument_e01dc466768bdd13, &__wrap_add_annotation_argument_e01dc466768bdd13 },
    { 0x421d82cbd4f678aa, false, (void*)&add_annotation_argument_e7f221e58ec46512, &__wrap_add_annotation_argument_e7f221e58ec46512 },
    { 0x8c94bfb37b3f7184, false, (void*)&get_for_source_index_19e9c390ee4338e2, &__wrap_get_for_source_index_19e9c390ee4338e2 },
    { 0xd76771f0884f6d3f, false, (void*)&get_for_source_index_e425541a2a706c53, &__wrap_get_for_source_index_e425541a2a706c53 },
    { 0xd6ce613dd4ee1ac6, false, (void*)&function_to_type_1f450dac98d6071d, &__wrap_function_to_type_1f450dac98d6071d },
    { 0x744aa02b262f7e34, false, (void*)&visit_finally_d773bb44db182978, &__wrap_visit_finally_d773bb44db182978 },
    { 0xe41cfaa3c698a1de, false, (void*)&isCMRES_2bb438cbd738593f, &__wrap_isCMRES_2bb438cbd738593f },
    { 0x14f45fd804ec1a83, false, (void*)&isCMRES_45d360720945cc74, &__wrap_isCMRES_45d360720945cc74 },
    { 0x74a4840f0a60e533, false, (void*)&isMakeLocal_c461b44592b8facf, &__wrap_isMakeLocal_c461b44592b8facf },
    { 0x7938c433905fde5, false, (void*)&isExprCallFunc_aac1b10a17185ef1, &__wrap_isExprCallFunc_aac1b10a17185ef1 },
    { 0xab31b8629a8394a7, true, (void*)&get_workhorse_types_62f1d76706bcc1ad, &__wrap_get_workhorse_types_62f1d76706bcc1ad },
    { 0xbb1d0dfecde4a96, false, (void*)&find_argument_index_3d6a951c2870d00f, &__wrap_find_argument_index_3d6a951c2870d00f },
    { 0x463b5b3ac935b144, false, (void*)&isCMRESType_bbf11aadb0354b4d, &__wrap_isCMRESType_bbf11aadb0354b4d },
    { 0xdd8a5bd7dd37fff3, false, (void*)&debug_expression_impl_203941aeed67e19a, &__wrap_debug_expression_impl_203941aeed67e19a },
    { 0x79fa533959c1342b, false, (void*)&debug_expression_72d312ce8f3ec686, &__wrap_debug_expression_72d312ce8f3ec686 },
    { 0xd11ee0d6b63cf645, false, (void*)&debug_expression_84991cbecc0ca3af, &__wrap_debug_expression_84991cbecc0ca3af },
    { 0xafc03ec02ad39ebd, false, (void*)&describe_d11036fb3823617b, &__wrap_describe_d11036fb3823617b },
    { 0xdb109b11a1387855, false, (void*)&getVectorElementCount_6156018073f7b991, &__wrap_getVectorElementCount_6156018073f7b991 },
    { 0xf01d5c2c31fa2952, false, (void*)&getVectorElementSize_7fe52b138a9b94ac, &__wrap_getVectorElementSize_7fe52b138a9b94ac },
    { 0xb0205fbcf0493583, false, (void*)&getVectorElementType_17ab3a94ab7f2835, &__wrap_getVectorElementType_17ab3a94ab7f2835 },
    { 0xd803e083fd151ee8, false, (void*)&getVectorOffset_cb21e53271916e73, &__wrap_getVectorOffset_cb21e53271916e73 },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
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
