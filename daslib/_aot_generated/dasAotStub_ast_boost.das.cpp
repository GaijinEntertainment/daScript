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
    tBitfield8 = int32_t(INT64_C(42)),
    tBitfield16 = int32_t(INT64_C(43)),
    tBitfield64 = int32_t(INT64_C(44)),
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
static_assert(sizeof(AstStructureAnnotation)==72,"structure size mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
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
static_assert(sizeof(AstVariantMacro)==40,"structure size mismatch with DAS");
static_assert(offsetof(AstVariantMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprIsVariant)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprAsVariant)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprSafeAsVariant)==32,"structure field offset mismatch with DAS");
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
static_assert(sizeof(MacroMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(MacroMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
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
static_assert(sizeof(TagFunctionAnnotation)==112,"structure size mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,appendToMangledName)==104,"structure field offset mismatch with DAS");
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
static_assert(sizeof(TagStructureAnnotation)==72,"structure size mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupAnyAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupFunctionAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupBlockAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupStructureAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupEnumerationAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupContractAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupReaderMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupCommentReader)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupCommentReader,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupVariantMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupForLoopMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupCaptureMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupTypeMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupSimulateMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupCallMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupCallMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupTypeInfoMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupInferMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupInferMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupDirtyInferMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupLintMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupLintMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupGlobalLintMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(SetupOptimizationMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,setup_call)==88,"structure field offset mismatch with DAS");
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
static_assert(sizeof(TagFunctionMacro)==104,"structure size mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,setup_call)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,tag)==96,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct BetterRttiVisitor {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
static_assert(sizeof(BetterRttiVisitor)==40,"structure size mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,visitExprIsVariant)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,visitExprAsVariant)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,visitExprSafeAsVariant)==32,"structure field offset mismatch with DAS");
}
extern TypeInfo __type_info__6b1c7db4b71a781f;
extern TypeInfo __type_info__af63a74c8601927d;
extern TypeInfo __type_info__3c61146b2bdfb90;
extern TypeInfo __type_info__ec148f64b21547b1;
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
EnumValueInfo __enum_info__c897fe55afe7f727_value_42 = { "tBitfield8", 42 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_43 = { "tBitfield16", 43 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_44 = { "tBitfield64", 44 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_45 = { "tPointer", 45 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_46 = { "tFunction", 46 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_47 = { "tLambda", 47 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_48 = { "tIterator", 48 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_49 = { "tArray", 49 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_50 = { "tTable", 50 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_51 = { "tBlock", 51 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_52 = { "tTuple", 52 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_53 = { "tVariant", 53 };
EnumValueInfo * __enum_info__c897fe55afe7f727_values [] = { &__enum_info__c897fe55afe7f727_value_0, &__enum_info__c897fe55afe7f727_value_1, &__enum_info__c897fe55afe7f727_value_2, &__enum_info__c897fe55afe7f727_value_3, &__enum_info__c897fe55afe7f727_value_4, &__enum_info__c897fe55afe7f727_value_5, &__enum_info__c897fe55afe7f727_value_6, &__enum_info__c897fe55afe7f727_value_7, &__enum_info__c897fe55afe7f727_value_8, &__enum_info__c897fe55afe7f727_value_9, &__enum_info__c897fe55afe7f727_value_10, &__enum_info__c897fe55afe7f727_value_11, &__enum_info__c897fe55afe7f727_value_12, &__enum_info__c897fe55afe7f727_value_13, &__enum_info__c897fe55afe7f727_value_14, &__enum_info__c897fe55afe7f727_value_15, &__enum_info__c897fe55afe7f727_value_16, &__enum_info__c897fe55afe7f727_value_17, &__enum_info__c897fe55afe7f727_value_18, &__enum_info__c897fe55afe7f727_value_19, &__enum_info__c897fe55afe7f727_value_20, &__enum_info__c897fe55afe7f727_value_21, &__enum_info__c897fe55afe7f727_value_22, &__enum_info__c897fe55afe7f727_value_23, &__enum_info__c897fe55afe7f727_value_24, &__enum_info__c897fe55afe7f727_value_25, &__enum_info__c897fe55afe7f727_value_26, &__enum_info__c897fe55afe7f727_value_27, &__enum_info__c897fe55afe7f727_value_28, &__enum_info__c897fe55afe7f727_value_29, &__enum_info__c897fe55afe7f727_value_30, &__enum_info__c897fe55afe7f727_value_31, &__enum_info__c897fe55afe7f727_value_32, &__enum_info__c897fe55afe7f727_value_33, &__enum_info__c897fe55afe7f727_value_34, &__enum_info__c897fe55afe7f727_value_35, &__enum_info__c897fe55afe7f727_value_36, &__enum_info__c897fe55afe7f727_value_37, &__enum_info__c897fe55afe7f727_value_38, &__enum_info__c897fe55afe7f727_value_39, &__enum_info__c897fe55afe7f727_value_40, &__enum_info__c897fe55afe7f727_value_41, &__enum_info__c897fe55afe7f727_value_42, &__enum_info__c897fe55afe7f727_value_43, &__enum_info__c897fe55afe7f727_value_44, &__enum_info__c897fe55afe7f727_value_45, &__enum_info__c897fe55afe7f727_value_46, &__enum_info__c897fe55afe7f727_value_47, &__enum_info__c897fe55afe7f727_value_48, &__enum_info__c897fe55afe7f727_value_49, &__enum_info__c897fe55afe7f727_value_50, &__enum_info__c897fe55afe7f727_value_51, &__enum_info__c897fe55afe7f727_value_52, &__enum_info__c897fe55afe7f727_value_53 };
EnumInfo __enum_info__c897fe55afe7f727 = { "Type", "rtti", __enum_info__c897fe55afe7f727_values, 54, UINT64_C(0xc897fe55afe7f727) };
VarInfo __func_info__f381b060f4dbce7_field_0 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Annotation"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8226, 88, UINT64_C(0x5ac39c9b48fed41d), "value", 0, 0 };
VarInfo * __func_info__f381b060f4dbce7_fields[1] =  { &__func_info__f381b060f4dbce7_field_0 };
FuncInfo __func_info__f381b060f4dbce7 = {"invoke block<(value:rtti::Annotation const):void> const", "", __func_info__f381b060f4dbce7_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xf381b060f4dbce7), 0x0 };
VarInfo __func_info__3b2c4edb933bef4a_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0x4cf955bf86f60534), "fn", 0, 0 };
VarInfo * __func_info__3b2c4edb933bef4a_fields[1] =  { &__func_info__3b2c4edb933bef4a_field_0 };
FuncInfo __func_info__3b2c4edb933bef4a = {"invoke block<(var fn:smart_ptr<ast::Function>):void> const", "", __func_info__3b2c4edb933bef4a_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x3b2c4edb933bef4a), 0x0 };
VarInfo __func_info__806bd08fbfbd09f4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0xec89c0a8da510f28), "func", 0, 0 };
VarInfo * __func_info__806bd08fbfbd09f4_fields[1] =  { &__func_info__806bd08fbfbd09f4_field_0 };
FuncInfo __func_info__806bd08fbfbd09f4 = {"invoke block<(var func:smart_ptr<ast::Function>):void> const", "", __func_info__806bd08fbfbd09f4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x806bd08fbfbd09f4), 0x0 };
VarInfo __func_info__8075d08fbfce07f4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0xec83c0a8da46dd28), "func", 0, 0 };
VarInfo * __func_info__8075d08fbfce07f4_fields[1] =  { &__func_info__8075d08fbfce07f4_field_0 };
FuncInfo __func_info__8075d08fbfce07f4 = {"invoke block<(var func:smart_ptr<ast::Function>):void> const", "", __func_info__8075d08fbfce07f4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8075d08fbfce07f4), 0x0 };
VarInfo __func_info__4dedd3fc50b439aa_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8afce1a80940fc9e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xc9a6e7cc498a17c9), "mod", 0, 0 };
VarInfo * __func_info__4dedd3fc50b439aa_fields[1] =  { &__func_info__4dedd3fc50b439aa_field_0 };
FuncInfo __func_info__4dedd3fc50b439aa = {"invoke block<(var mod:rtti::Module?):void> const", "", __func_info__4dedd3fc50b439aa_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x4dedd3fc50b439aa), 0x0 };
VarInfo __func_info__762f57a17c2dce28_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x2a896b18ac8c5dc), "name", 0, 0 };
VarInfo __func_info__762f57a17c2dce28_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x1b2f537dda20b669), "cppName", 0, 0 };
VarInfo __func_info__762f57a17c2dce28_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ce241e3005cc873b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0x5671bd5411275d07), "xtype", 0, 0 };
VarInfo __func_info__762f57a17c2dce28_field_3 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xc3630b856e2c8315), "offset", 0, 0 };
VarInfo * __func_info__762f57a17c2dce28_fields[4] =  { &__func_info__762f57a17c2dce28_field_0, &__func_info__762f57a17c2dce28_field_1, &__func_info__762f57a17c2dce28_field_2, &__func_info__762f57a17c2dce28_field_3 };
FuncInfo __func_info__762f57a17c2dce28 = {"invoke block<(var name:string;var cppName:string;var xtype:smart_ptr<ast::TypeDecl>;var offset:uint):void> const", "", __func_info__762f57a17c2dce28_fields, 4, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x762f57a17c2dce28), 0x0 };
VarInfo __func_info__b50c88059cb1bcc_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xceb2c4cde2c49d3a), "pkey", 0, 0 };
VarInfo __func_info__b50c88059cb1bcc_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x447579d184dfd9f7), "pvalue", 0, 0 };
VarInfo * __func_info__b50c88059cb1bcc_fields[2] =  { &__func_info__b50c88059cb1bcc_field_0, &__func_info__b50c88059cb1bcc_field_1 };
FuncInfo __func_info__b50c88059cb1bcc = {"invoke block<(var pkey:void?;var pvalue:void?):void> const", "", __func_info__b50c88059cb1bcc_fields, 2, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xb50c88059cb1bcc), 0x0 };
VarInfo __func_info__b28b606ddb7a6760_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x8603fbcef5fdd683), "value", 0, 0 };
VarInfo * __func_info__b28b606ddb7a6760_fields[1] =  { &__func_info__b28b606ddb7a6760_field_0 };
FuncInfo __func_info__b28b606ddb7a6760 = {"invoke block<(var value:void?):void> const", "", __func_info__b28b606ddb7a6760_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xb28b606ddb7a6760), 0x0 };
TypeInfo __type_info__6b1c7db4b71a781f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 258, 32, UINT64_C(0x6b1c7db4b71a781f) };
TypeInfo __type_info__af63a74c8601927d = { Type::anyArgument, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 16, UINT64_C(0xaf63a74c8601927d) };
TypeInfo __type_info__3c61146b2bdfb90 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 290, 32, UINT64_C(0x3c61146b2bdfb90) };
TypeInfo * __type_info__ec148f64b21547b1_arg_types[9] = { &__type_info__af63df4c8601f1a5, &__type_info__af63e44c8601fa24, &__type_info__af63e84c860200f0, &__type_info__d922fe078cefab30, &__type_info__d9307e078cfb0f0c, &__type_info__af63db4c8601ead9, &__type_info__af63d94c8601e773, &__type_info__af63ee4c86020b22, &__type_info__af63a74c8601927d };
const char * __type_info__ec148f64b21547b1_arg_names[9] = { "tBool", "tInt", "tUInt", "tInt64", "tUInt64", "tFloat", "tDouble", "tString", "nothing" };
TypeInfo __type_info__ec148f64b21547b1 = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__ec148f64b21547b1_arg_types, __type_info__ec148f64b21547b1_arg_names, 9, 0, nullptr, 16678, 32, UINT64_C(0xec148f64b21547b1) };
TypeInfo __type_info__4fd1011fade7876f = { Type::tEnumeration, nullptr, &__enum_info__c897fe55afe7f727, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0x4fd1011fade7876f) };
TypeInfo __type_info__fa593d0882a72913 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16548, 8, UINT64_C(0xfa593d0882a72913) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af96fe4c8658cf52 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf96fe4c8658cf52) };
TypeInfo __type_info__b68d800849332aec = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0xb68d800849332aec) };
TypeInfo __type_info__a57bf935c2dd03 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Function"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 464, UINT64_C(0xa57bf935c2dd03) };
TypeInfo __type_info__ce241e3005cc873b = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::TypeDecl"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 232, UINT64_C(0xce241e3005cc873b) };
TypeInfo __type_info__8afce1a80940fc9e = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Module"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 1384, UINT64_C(0x8afce1a80940fc9e) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__38be04c990d4b416 = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__ec148f64b21547b1_arg_types, __type_info__ec148f64b21547b1_arg_names, 9, 0, nullptr, 16390, 32, UINT64_C(0x38be04c990d4b416) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63d94c8601e773 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xaf63d94c8601e773) };
TypeInfo __type_info__af63db4c8601ead9 = { Type::tFloat, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63db4c8601ead9) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__d922fe078cefab30 = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xd922fe078cefab30) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63e84c860200f0 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e84c860200f0) };
TypeInfo __type_info__d9307e078cfb0f0c = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xd9307e078cfb0f0c) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

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
TypeInfo * __tinfo_15[4] = { &__type_info__af90fe4c864e9d52, &__type_info__ec148f64b21547b1, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_18[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_19[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_26[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_29[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_31[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_33[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_35[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec };
TypeInfo * __tinfo_37[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__b68d800849332aec };
TypeInfo * __tinfo_39[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af81fe4c86352052 };

inline void _FuncbuiltinTickpushTick10769833213962245646_6dab624d0a9ec2fa ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_193_0, ast::AstFunctionAnnotation * __value_rename_at_193_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_260ca7b6472dc38b ( Context * __context__, ast_boost::MacroMacro const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2a0858496863a6a8 ( Context * __context__, ast_boost::TagFunctionAnnotation const  & __cl_rename_at_116_3 );
inline void _FuncbuiltinTickpushTick10769833213962245646_9901561b81ea1e93 ( Context * __context__, TArray<ast::AstStructureAnnotation *> & __Arr_rename_at_193_4, ast::AstStructureAnnotation * __value_rename_at_193_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_46f2134b013f7e0f ( Context * __context__, ast_boost::SetupFunctionAnnotation const  & __cl_rename_at_116_6 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_308d52657d5fdb9f ( Context * __context__, ast_boost::SetupBlockAnnotation const  & __cl_rename_at_116_7 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8318dfd00c3831fe ( Context * __context__, ast_boost::SetupStructureAnnotation const  & __cl_rename_at_116_8 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2e8d4d0ba0370750 ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  & __cl_rename_at_116_9 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_dd15ed5b75151430 ( Context * __context__, ast_boost::SetupContractAnnotation const  & __cl_rename_at_116_10 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_554ebee507608082 ( Context * __context__, ast_boost::SetupReaderMacro const  & __cl_rename_at_116_11 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3081f3afed8c109b ( Context * __context__, ast_boost::SetupCommentReader const  & __cl_rename_at_116_12 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_15cdaeb56361f117 ( Context * __context__, ast_boost::SetupCallMacro const  & __cl_rename_at_116_13 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_95c3a006bc4c8db ( Context * __context__, ast_boost::SetupTypeInfoMacro const  & __cl_rename_at_116_14 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1021faef828c12f7 ( Context * __context__, ast_boost::SetupVariantMacro const  & __cl_rename_at_116_15 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e45c871b77e11819 ( Context * __context__, ast_boost::SetupForLoopMacro const  & __cl_rename_at_116_16 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e815a40ffb05ff49 ( Context * __context__, ast_boost::SetupCaptureMacro const  & __cl_rename_at_116_17 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8624b1f3bb6dc40c ( Context * __context__, ast_boost::SetupTypeMacro const  & __cl_rename_at_116_18 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f08eab0b96bf74eb ( Context * __context__, ast_boost::SetupSimulateMacro const  & __cl_rename_at_116_19 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c641e5976eca7cc2 ( Context * __context__, ast_boost::TagStructureAnnotation const  & __cl_rename_at_116_20 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_69b37fb41c547990 ( Context * __context__, ast_boost::TagFunctionMacro const  & __cl_rename_at_116_21 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_940fe04b496ec307 ( Context * __context__, ast_boost::SetupInferMacro const  & __cl_rename_at_116_22 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f4258cbe2a3ca795 ( Context * __context__, ast_boost::SetupDirtyInferMacro const  & __cl_rename_at_116_23 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5c844592baa87df1 ( Context * __context__, ast_boost::SetupOptimizationMacro const  & __cl_rename_at_116_24 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_ff4730001cc28ae9 ( Context * __context__, ast_boost::SetupLintMacro const  & __cl_rename_at_116_25 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f9dc0993d4c46e63 ( Context * __context__, ast_boost::SetupGlobalLintMacro const  & __cl_rename_at_116_26 );
inline void _FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_193_27, ast::AstVariantMacro * __value_rename_at_193_28 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_af3f45d965a765c ( Context * __context__, ast_boost::BetterRttiVisitor const  & __cl_rename_at_116_29 );
inline void clone_7f4a34d879fc35f4 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & __dest_rename_at_1121_30, smart_ptr_raw<FunctionAnnotation> const  __src_rename_at_1121_31 );
inline smart_ptr_raw<FunctionAnnotation> _FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> __clone_src_rename_at_1118_32 );
inline Module * _FuncastTickfind_moduleTick11101329256228773934_b65022313a84d5bd ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_951_34, char * const  __name_rename_at_951_35 );
inline void _FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f ( Context * __context__, TArray<char *> & __Arr_rename_at_193_38, char * __value_rename_at_193_39 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9 ( Context * __context__, TArray<char *> const  & __it_rename_at_29_40, char * const  __separator_rename_at_29_41 );
inline void _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 ( Context * __context__, TArray<char *> & __Arr_rename_at_177_46, char * const  __value_rename_at_177_47 );
inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec ( Context * __context__, TArray<smart_ptr_raw<Function>> & __Arr_rename_at_393_48, smart_ptr_raw<Function> __value_rename_at_393_49 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2 ( Context * __context__, TArray<smart_ptr_raw<Function>> & __a_rename_at_1336_50 );
inline void clone_e5dbb6c9a183080e ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_285_52, smart_ptr_raw<Function> const  __src_rename_at_285_53 );
inline char * _FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_54, bool __extra_rename_at_38_55, bool __contracts_rename_at_38_56, bool __modules_rename_at_38_57 );
inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1907_58 );
inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8 ( Context * __context__ );
inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_c8d3f222daa2e3cc ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1913_60 );
inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1913_61 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_56f1ba3bb9b6d08a ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_547_62, smart_ptr_raw<Expression> __enull_rename_at_547_63 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_69da28483a9e1aa1 ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_547_81, smart_ptr_raw<Expression> __enull_rename_at_547_82 );
inline Annotation * _FuncbuiltinTickget_ptrTick8468476673553620226_b515b7b0b5b85a70 ( Context * __context__, smart_ptr_raw<Annotation> const  __src_rename_at_1907_100 );
inline void clone_23a052359607d993 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_727_101, smart_ptr_raw<TypeDecl> const  __src_rename_at_727_102 );
inline void clone_a8386ed128f4f0b4 ( Context * __context__, smart_ptr_raw<Enumeration> & __dest_rename_at_882_103, Enumeration * const  __src_rename_at_882_104 );
inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_479fe631f4b1cb3 ( Context * __context__, char * const  __name_rename_at_970_105 );
inline void clone_98d3170c0efdddcc ( Context * __context__, smart_ptr_raw<Annotation> & __dest_rename_at_968_106, smart_ptr_raw<Annotation> const  __src_rename_at_968_107 );
inline char * _FuncastTickdescribeTick842554968825501494_5630d72624d0f510 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_108 );
inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1907_109 );
inline bool isVectorType_5a1ccbd394d2d488 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_26_110 );
inline char * describe_a0a74ab06eec48e6 ( Context * __context__, AnnotationArgumentList const  & __list_rename_at_37_111 );
inline char * describe_fcbed85e0402159e ( Context * __context__, AnnotationDeclaration const  & __ann_rename_at_42_114 );
inline char * describe_6a04757d0b7596f7 ( Context * __context__, AnnotationList const  & __list_rename_at_51_115 );
inline char * describe_24ebe982465d57fc ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_56_118 );
inline bool isExpression_f1fadf50b6070b86 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_80_119, bool __top_rename_at_80_120 );
inline bool is_same_or_inherited_66408d7f398767b ( Context * __context__, Structure const  * const  __parent_rename_at_99_121, Structure const  * const  __child_rename_at_99_122 );
inline bool is_class_method_4783315be6e05a7b ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_111_124, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_111_125 );
inline void emplace_new_9bef84999dd1e3f4 ( Context * __context__, das::vector<smart_ptr<Expression>> & __vec_rename_at_134_126, smart_ptr_raw<Expression> __ptr_rename_at_134_127 );
inline void emplace_new_31c417adc7c7987e ( Context * __context__, das::vector<smart_ptr<TypeDecl>> & __vec_rename_at_139_128, smart_ptr_raw<TypeDecl> __ptr_rename_at_139_129 );
inline void emplace_new_2f0cb94c081aada2 ( Context * __context__, das::vector<smart_ptr<Variable>> & __vec_rename_at_144_130, smart_ptr_raw<Variable> __ptr_rename_at_144_131 );
inline void emplace_new_75514f67779c4fa1 ( Context * __context__, MakeStruct & __vec_rename_at_149_132, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_149_133 );
inline bool override_method_8162bb655dd2cd37 ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_154_134, char * const  __name_rename_at_154_135, char * const  __funcName_rename_at_154_136 );
inline void for_each_tag_function_bb49dc3a279a3322 ( Context * __context__, Module * const  __mod_rename_at_214_140, char * const  __tag_rename_at_214_141, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __blk_rename_at_214_142 );
inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_416380068da636fe ( Context * __context__, char * const  __argn_rename_at_240_149, AnnotationArgumentList const  & __args_rename_at_240_150 );
inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_34945b603c757892 ( Context * __context__, AnnotationArgumentList const  & __args_rename_at_250_152, char * const  __argn_rename_at_250_153 );
inline void apply_tag_annotation_1944b9a89fd11326 ( Context * __context__, char * const  __tag_rename_at_260_155, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_260_156 );
inline smart_ptr_raw<Function> find_unique_function_b15ea43980af8102 ( Context * __context__, Module * const  __mod_rename_at_280_161, char * const  __name_rename_at_280_162, bool __canfail_rename_at_280_163 );
inline smart_ptr_raw<Function> find_unique_generic_4054e5bb45ca1054 ( Context * __context__, Module * const  __mod_rename_at_297_167, char * const  __name_rename_at_297_168, bool __canfail_rename_at_297_169 );
inline ExprBlock * setup_call_list_729e5db2cab0e291 ( Context * __context__, char * const  __name_rename_at_314_173, LineInfo const  & __at_rename_at_314_174, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __subblock_rename_at_314_175 );
inline ExprBlock * setup_call_list_7b65a2c40054d8e8 ( Context * __context__, char * const  __name_rename_at_336_180, LineInfo const  & __at_rename_at_336_181, bool __isInit_rename_at_336_182, bool __isPrivate_rename_at_336_183, bool __isLateInit_rename_at_336_184 );
inline ExprBlock * setup_macro_e906336cb4687a9c ( Context * __context__, char * const  __name_rename_at_351_186, LineInfo const  & __at_rename_at_351_187 );
inline void * panic_expr_as_2ea20f674c842c9c ( Context * __context__ );
inline bool _FuncTickisTickBuiltInFunction_3d26f4806589821b ( Context * __context__, Function * const  __foo_rename_at_623_200 );
inline BuiltInFunction * _FuncTickasTickBuiltInFunction_c47ad3b03c9bc354 ( Context * __context__, Function * const  __foo_rename_at_628_201 );
inline bool _FuncTickisTickExternalFnBase_24b69557e6b14161 ( Context * __context__, Function * const  __foo_rename_at_639_202 );
inline ExternalFnBase * _FuncTickasTickExternalFnBase_b840ea32468fb731 ( Context * __context__, Function * const  __foo_rename_at_644_203 );
inline bool _FuncTickisTickFunctionAnnotation_eccaa0a9bcb3796 ( Context * __context__, Annotation * const  __foo_rename_at_655_204 );
inline bool _FuncTickisTickFunctionAnnotation_6d5ce87249ff0eaf ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_660_205 );
inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_8d76f010548d92f6 ( Context * __context__, Annotation * const  __foo_rename_at_665_206 );
inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_6e0707ffcc0c2b76 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_671_207 );
inline bool _FuncTickisTickStructureAnnotation_471f8e2f8678905f ( Context * __context__, Annotation * const  __foo_rename_at_683_208 );
inline bool _FuncTickisTickStructureAnnotation_1883d933e82a8bcc ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_688_209 );
inline StructureAnnotation * _FuncTickasTickStructureAnnotation_1f9c68dfdd32ff55 ( Context * __context__, Annotation * const  __foo_rename_at_693_210 );
inline StructureAnnotation * _FuncTickasTickStructureAnnotation_1a5c23b3eb803935 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_699_211 );
inline smart_ptr_raw<Expression> walk_and_convert_array_ba1cc70e35611268 ( Context * __context__, uint8_t const  * const  __data_rename_at_705_212, smart_ptr_raw<TypeDecl> const  __info_rename_at_705_213, LineInfo const  & __at_rename_at_705_214 );
inline smart_ptr_raw<Expression> walk_and_convert_dim_9001b9148a98cff6 ( Context * __context__, uint8_t const  * const  __data_rename_at_722_221, smart_ptr_raw<TypeDecl> const  __info_rename_at_722_222, LineInfo const  & __at_rename_at_722_223 );
inline smart_ptr_raw<Expression> walk_and_convert_pointer_47b794d0dd59337d ( Context * __context__, uint8_t const  * const  __data_rename_at_737_231, smart_ptr_raw<TypeDecl> const  __info_rename_at_737_232, LineInfo const  & __at_rename_at_737_233 );
inline smart_ptr_raw<Expression> walk_and_convert_tuple_9ce3b13721ba5daf ( Context * __context__, uint8_t const  * const  __data_rename_at_746_237, smart_ptr_raw<TypeDecl> const  __info_rename_at_746_238, LineInfo const  & __at_rename_at_746_239 );
inline smart_ptr_raw<Expression> walk_and_convert_structure_106d0a1bf460f63c ( Context * __context__, uint8_t const  * const  __data_rename_at_758_244, smart_ptr_raw<TypeDecl> const  __info_rename_at_758_245, LineInfo const  & __at_rename_at_758_246 );
inline smart_ptr_raw<Expression> walk_and_convert_variant_cdeba207e46113d9 ( Context * __context__, uint8_t const  * const  __data_rename_at_779_255, smart_ptr_raw<TypeDecl> const  __info_rename_at_779_256, LineInfo const  & __at_rename_at_779_257 );
inline smart_ptr_raw<Expression> walk_and_convert_table_bafed583a64bc3a1 ( Context * __context__, uint8_t const  * const  __data_rename_at_794_264, smart_ptr_raw<TypeDecl> const  __info_rename_at_794_265, LineInfo const  & __at_rename_at_794_266 );
inline smart_ptr_raw<Expression> walk_and_convert_basic_aef53343044f9782 ( Context * __context__, uint8_t const  * const  __data_rename_at_820_279, smart_ptr_raw<TypeDecl> const  __info_rename_at_820_280, LineInfo const  & __at_rename_at_820_281 );
inline smart_ptr_raw<Expression> walk_and_convert_enumeration_5bb2d478c4ffeaaa ( Context * __context__, uint8_t const  * const  __data_rename_at_866_284, smart_ptr_raw<TypeDecl> const  __info_rename_at_866_285, LineInfo const  & __at_rename_at_866_286 );
inline smart_ptr_raw<Expression> walk_and_convert_cfdd8313a6a5987 ( Context * __context__, uint8_t const  * const  __data_rename_at_886_290, smart_ptr_raw<TypeDecl> const  __info_rename_at_886_291, LineInfo const  & __at_rename_at_886_292 );
inline Annotation const  * find_annotation_c7c295d23bae9ab4 ( Context * __context__, char * const  __mod_name_rename_at_946_293, char * const  __ann_name_rename_at_946_294 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_4862c33d2ab9dcd4 ( Context * __context__, char * const  __mod_name_rename_at_963_298, char * const  __ann_name_rename_at_963_299, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_963_300 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_8190e2b8fbb09c12 ( Context * __context__, char * const  __mod_name_rename_at_987_306, char * const  __ann_name_rename_at_987_307 );
inline void append_annotation_9e9720e2bdf176ae ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_996_310, char * const  __mod_name_rename_at_996_311, char * const  __ann_name_rename_at_996_312 );
inline void append_annotation_82649f781a6d03e7 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_1004_315, char * const  __mod_name_rename_at_1004_316, char * const  __ann_name_rename_at_1004_317 );
inline void append_annotation_1d124a466ae78997 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_1012_320, char * const  __mod_name_rename_at_1012_321, char * const  __ann_name_rename_at_1012_322 );
inline void append_annotation_d2aadccfb018501d ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_1020_325, char * const  __mod_name_rename_at_1020_326, char * const  __ann_name_rename_at_1020_327, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_1020_328 );
inline void append_annotation_41a68523ba2c1d9e ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_1028_331, char * const  __mod_name_rename_at_1028_332, char * const  __ann_name_rename_at_1028_333, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_1028_334 );
inline void append_annotation_84797c2112b4fd76 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_1036_337, char * const  __mod_name_rename_at_1036_338, char * const  __ann_name_rename_at_1036_339, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_1036_340 );
inline int32_t add_annotation_argument_a6c4ec43057e73c ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1044_343, char * const  __argName_rename_at_1044_344, bool __val_rename_at_1044_345 );
inline int32_t add_annotation_argument_2ee13ee84bf33b6d ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1052_347, char * const  __argName_rename_at_1052_348, int32_t __val_rename_at_1052_349 );
inline int32_t add_annotation_argument_e4f5a170be5edc8b ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1060_351, char * const  __argName_rename_at_1060_352, float __val_rename_at_1060_353 );
inline int32_t add_annotation_argument_d6722d8e520facbc ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1068_355, char * const  __argName_rename_at_1068_356, char * const  __val_rename_at_1068_357 );
inline int32_t add_annotation_argument_eb9c96b34ef66276 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_1076_359, AnnotationArgument const  & __ann_rename_at_1076_360 );
inline int32_t get_for_source_index_ce1d36791493c0c4 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1094_362, smart_ptr_raw<Variable> const  __svar_rename_at_1094_363 );
inline int32_t get_for_source_index_44e105584529041d ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1104_368, smart_ptr_raw<Expression> const  __source_rename_at_1104_369 );
inline smart_ptr_raw<TypeDecl> function_to_type_1f450dac98d6071d ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_1154_374 );
inline void visit_finally_5653493f01f579fa ( Context * __context__, ExprBlock * const  __blk_rename_at_1167_380, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1167_381 );
inline bool isCMRES_2bb438cbd738593f ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1174_382 );
inline bool isCMRES_45d360720945cc74 ( Context * __context__, Function * const  __fun_rename_at_1179_383 );
inline bool isMakeLocal_c461b44592b8facf ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1184_384 );
inline bool isExprCallFunc_aac1b10a17185ef1 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1193_385 );
inline TDim<DAS_COMMENT(bound_enum) das::Type,34> get_workhorse_types_de3ed4d894cfde0c ( Context * __context__ );
inline int32_t find_argument_index_3d6a951c2870d00f ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1219_386, char * const  __name_rename_at_1219_387 );
inline bool isCMRESType_bbf11aadb0354b4d ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1229_392 );
inline void debug_expression_impl_203941aeed67e19a ( Context * __context__, StringBuilderWriter & __writer_rename_at_1239_393, smart_ptr_raw<Expression> const  __expr_rename_at_1239_394, Bitfield __deFlags_rename_at_1239_395, int32_t __tabs_rename_at_1239_396 );
inline char * debug_expression_72d312ce8f3ec686 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1345_430, Bitfield __deFlags_rename_at_1345_431 );
inline char * debug_expression_84991cbecc0ca3af ( Context * __context__, Expression * const  __expr_rename_at_1352_433 );
inline char * describe_d11036fb3823617b ( Context * __context__, Expression * const  __expr_rename_at_1359_434 );
inline int32_t getVectorElementCount_57af1965ac8593bc ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1366_435 );
inline int32_t getVectorElementSize_d8680eab119bd409 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1374_436 );
inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_3fc0f8eee440e9d1 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1380_437 );
inline int32_t getVectorOffset_ac9cdc53c25ed4ee ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1390_438, char * const  __ident_rename_at_1390_439 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_6dab624d0a9ec2fa ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_193_0, ast::AstFunctionAnnotation * __value_rename_at_193_1 )
{
    das_copy(__Arr_rename_at_193_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_193_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_260ca7b6472dc38b ( Context * __context__, ast_boost::MacroMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2a0858496863a6a8 ( Context * __context__, ast_boost::TagFunctionAnnotation const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_9901561b81ea1e93 ( Context * __context__, TArray<ast::AstStructureAnnotation *> &  __Arr_rename_at_193_4, ast::AstStructureAnnotation * __value_rename_at_193_5 )
{
    das_copy(__Arr_rename_at_193_4(builtin_array_push_back(das_arg<TArray<ast::AstStructureAnnotation *>>::pass(__Arr_rename_at_193_4),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_5);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_46f2134b013f7e0f ( Context * __context__, ast_boost::SetupFunctionAnnotation const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_308d52657d5fdb9f ( Context * __context__, ast_boost::SetupBlockAnnotation const  &  __cl_rename_at_116_7 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_7.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8318dfd00c3831fe ( Context * __context__, ast_boost::SetupStructureAnnotation const  &  __cl_rename_at_116_8 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_8.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2e8d4d0ba0370750 ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  &  __cl_rename_at_116_9 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_9.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_dd15ed5b75151430 ( Context * __context__, ast_boost::SetupContractAnnotation const  &  __cl_rename_at_116_10 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_10.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_554ebee507608082 ( Context * __context__, ast_boost::SetupReaderMacro const  &  __cl_rename_at_116_11 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_11.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3081f3afed8c109b ( Context * __context__, ast_boost::SetupCommentReader const  &  __cl_rename_at_116_12 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_12.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_15cdaeb56361f117 ( Context * __context__, ast_boost::SetupCallMacro const  &  __cl_rename_at_116_13 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_13.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_95c3a006bc4c8db ( Context * __context__, ast_boost::SetupTypeInfoMacro const  &  __cl_rename_at_116_14 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_14.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1021faef828c12f7 ( Context * __context__, ast_boost::SetupVariantMacro const  &  __cl_rename_at_116_15 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_15.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e45c871b77e11819 ( Context * __context__, ast_boost::SetupForLoopMacro const  &  __cl_rename_at_116_16 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_16.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e815a40ffb05ff49 ( Context * __context__, ast_boost::SetupCaptureMacro const  &  __cl_rename_at_116_17 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_17.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8624b1f3bb6dc40c ( Context * __context__, ast_boost::SetupTypeMacro const  &  __cl_rename_at_116_18 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_18.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f08eab0b96bf74eb ( Context * __context__, ast_boost::SetupSimulateMacro const  &  __cl_rename_at_116_19 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_19.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c641e5976eca7cc2 ( Context * __context__, ast_boost::TagStructureAnnotation const  &  __cl_rename_at_116_20 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_20.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_69b37fb41c547990 ( Context * __context__, ast_boost::TagFunctionMacro const  &  __cl_rename_at_116_21 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_21.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_940fe04b496ec307 ( Context * __context__, ast_boost::SetupInferMacro const  &  __cl_rename_at_116_22 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_22.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f4258cbe2a3ca795 ( Context * __context__, ast_boost::SetupDirtyInferMacro const  &  __cl_rename_at_116_23 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_23.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5c844592baa87df1 ( Context * __context__, ast_boost::SetupOptimizationMacro const  &  __cl_rename_at_116_24 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_24.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_ff4730001cc28ae9 ( Context * __context__, ast_boost::SetupLintMacro const  &  __cl_rename_at_116_25 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_25.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f9dc0993d4c46e63 ( Context * __context__, ast_boost::SetupGlobalLintMacro const  &  __cl_rename_at_116_26 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_26.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_193_27, ast::AstVariantMacro * __value_rename_at_193_28 )
{
    das_copy(__Arr_rename_at_193_27(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_193_27),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_28);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_af3f45d965a765c ( Context * __context__, ast_boost::BetterRttiVisitor const  &  __cl_rename_at_116_29 )
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

inline Module * _FuncastTickfind_moduleTick11101329256228773934_b65022313a84d5bd ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_951_34, char * const  __name_rename_at_951_35 ) { das_stack_prologue __prologue(__context__,112,"ast`find_module`11101329256228773934 " DAS_FILE_LINE);
{
    Module * __rm_rename_at_952_36 = 0;
    rtti_builtin_program_for_each_module(__prog_rename_at_951_34,das_make_block<void,Module *>(__context__,96,0,&__func_info__4dedd3fc50b439aa,[&](Module * __mod_rename_at_953_37) -> void{
        if ( SimPolicy<char *>::Equ(cast<char *>::from(((char * const )(to_das_string(das_arg<das::string>::pass(__mod_rename_at_953_37->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),cast<char *>::from(__name_rename_at_951_35),*__context__,nullptr) )
        {
            das_copy(__rm_rename_at_952_36,__mod_rename_at_953_37);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __rm_rename_at_952_36 == nullptr )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) "module '")), cast<char * const >::from(__name_rename_at_951_35), cast<char * const >::from(((char *) "' not found")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<Module *>::cast(__rm_rename_at_952_36);
}}

inline void _FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f ( Context * __context__, TArray<char *> &  __Arr_rename_at_193_38, char * __value_rename_at_193_39 )
{
    das_copy(__Arr_rename_at_193_38(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_193_38),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_39);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9 ( Context * __context__, TArray<char *> const  &  __it_rename_at_29_40, char * const  __separator_rename_at_29_41 )
{
    char * __st_rename_at_35_42 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_35_43) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_36_44 = true;
        {
            bool __need_loop_37 = true;
            // elem: string const&
            das_iterator<TArray<char *> const > __elem_iterator(__it_rename_at_29_40);
            char * const  * __elem_rename_at_37_45;
            __need_loop_37 = __elem_iterator.first(__context__,(__elem_rename_at_37_45)) && __need_loop_37;
            for ( ; __need_loop_37 ; __need_loop_37 = __elem_iterator.next(__context__,(__elem_rename_at_37_45)) )
            {
                if ( __skip_first_rename_at_36_44 )
                {
                    das_copy(__skip_first_rename_at_36_44,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_35_43),cast<char * const >::from(__separator_rename_at_29_41)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_35_43),cast<char * const >::from((*__elem_rename_at_37_45))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_37_45));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_35_42);
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

inline void clone_e5dbb6c9a183080e ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_285_52, smart_ptr_raw<Function> const  __src_rename_at_285_53 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_285_52),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_285_53),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_54, bool __extra_rename_at_38_55, bool __contracts_rename_at_38_56, bool __modules_rename_at_38_57 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_54,__extra_rename_at_38_55,__contracts_rename_at_38_56,__modules_rename_at_38_57,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1907_58 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1907_58));
}

inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8 ( Context * __context__ )
{
    TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> __t_rename_at_195_59;das_zero(__t_rename_at_195_59);
    set_variant_index(das_arg<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::pass(__t_rename_at_195_59),8);
    return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(__t_rename_at_195_59);
}

inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_c8d3f222daa2e3cc ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1913_60 )
{
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__src_rename_at_1913_60));
}

inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1913_61 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1913_61));
}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_56f1ba3bb9b6d08a ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_547_62, smart_ptr_raw<Expression> __enull_rename_at_547_63 ) { das_stack_prologue __prologue(__context__,224,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_548_64; memset((void*)&__vdr_rename_at_548_64,0,sizeof(__vdr_rename_at_548_64));
    smart_ptr_raw<ExprConstString> __cna_rename_at_549_66; memset((void*)&__cna_rename_at_549_66,0,sizeof(__cna_rename_at_549_66));
    smart_ptr_raw<ExprOp2> __cond_rename_at_550_68; memset((void*)&__cond_rename_at_550_68,0,sizeof(__cond_rename_at_550_68));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_551_70; memset((void*)&__ctype_rename_at_551_70,0,sizeof(__ctype_rename_at_551_70));
    smart_ptr_raw<Expression> __esub_rename_at_555_72; memset((void*)&__esub_rename_at_555_72,0,sizeof(__esub_rename_at_555_72));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_557_73; memset((void*)&__neqz_rename_at_557_73,0,sizeof(__neqz_rename_at_557_73));
    smart_ptr_raw<ExprOp2> __nand_rename_at_561_75; memset((void*)&__nand_rename_at_561_75,0,sizeof(__nand_rename_at_561_75));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_567_77; memset((void*)&__nsub_rename_at_567_77,0,sizeof(__nsub_rename_at_567_77));
    smart_ptr_raw<ExprCast> __vcast_rename_at_573_78; memset((void*)&__vcast_rename_at_573_78,0,sizeof(__vcast_rename_at_573_78));
    smart_ptr_raw<ExprOp3> __tri_rename_at_579_79; memset((void*)&__tri_rename_at_579_79,0,sizeof(__tri_rename_at_579_79));
    /* finally */ auto __finally_547= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_579_79);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_573_78);
    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__esub_rename_at_555_72);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_551_70);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_550_68);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_549_66);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_548_64);
    /* end finally */ });
    __vdr_rename_at_548_64; das_zero(__vdr_rename_at_548_64); das_move(__vdr_rename_at_548_64, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_548) {
        das_copy((__mks_548.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
        das_move((__mks_548.value /*value*/),(clone_expression(__expr_rename_at_547_62->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_548.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_549_66; das_zero(__cna_rename_at_549_66); das_move(__cna_rename_at_549_66, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_549) {
        das_copy((__mks_549.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_549.text /*value*/,__expr_rename_at_547_62->name /*name*/);
        }    }))));
    __cond_rename_at_550_68; das_zero(__cond_rename_at_550_68); das_move(__cond_rename_at_550_68, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_550) {
        das_copy((__mks_550.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
        das_move((__mks_550.left /*left*/),(__vdr_rename_at_548_64));
        das_move((__mks_550.right /*right*/),(__cna_rename_at_549_66));
        {
            set_das_string(das_arg<das::string>::pass(__mks_550.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_551_70; das_zero(__ctype_rename_at_551_70); das_move(__ctype_rename_at_551_70, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_551) {
        das_copy((__mks_551.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
        das_copy((__mks_551.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_551.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_553) {
            das_copy((__mks_553.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
            das_copy((__mks_553.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_553.alias /*alias*/,__expr_rename_at_547_62->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_555_72; das_zero(__esub_rename_at_555_72); das_move(__esub_rename_at_555_72, clone_expression(__expr_rename_at_547_62->value /*value*/));
    if ( __expr_rename_at_547_62->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_556= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_561_75);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_557_73);
        /* end finally */ });
        __neqz_rename_at_557_73; das_zero(__neqz_rename_at_557_73); das_move(__neqz_rename_at_557_73, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_557) {
            das_copy((__mks_557.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
            das_move((__mks_557.left /*left*/),(clone_expression(__expr_rename_at_547_62->value /*value*/)));
            das_move((__mks_557.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_559) {
                das_copy((__mks_559.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_557.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_561_75; das_zero(__nand_rename_at_561_75); das_move(__nand_rename_at_561_75, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_561) {
            das_copy((__mks_561.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
            das_move((__mks_561.left /*left*/),(__neqz_rename_at_557_73));
            das_move((__mks_561.right /*right*/),(__cond_rename_at_550_68));
            {
                set_das_string(das_arg<das::string>::pass(__mks_561.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_550_68),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_561_75),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_566= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_567_77);
        /* end finally */ });
        __nsub_rename_at_567_77; das_zero(__nsub_rename_at_567_77); das_move(__nsub_rename_at_567_77, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_567) {
            das_copy((__mks_567.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
            das_move((__mks_567.subexpr /*subexpr*/),(__esub_rename_at_555_72));
            das_copy((__mks_567.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_555_72),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_567_77),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_573_78; das_zero(__vcast_rename_at_573_78); das_move(__vcast_rename_at_573_78, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_573) {
        das_copy((__mks_573.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
        das_move((__mks_573.subexpr /*subexpr*/),(__esub_rename_at_555_72));
        das_move((__mks_573.castType /*castType*/),(__ctype_rename_at_551_70));
        das_copy((__mks_573.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_573.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_579_79; das_zero(__tri_rename_at_579_79); das_move(__tri_rename_at_579_79, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_579) {
        das_copy((__mks_579.at /*at*/),(__expr_rename_at_547_62->at /*at*/));
        das_move((__mks_579.subexpr /*subexpr*/),(__cond_rename_at_550_68));
        das_move((__mks_579.left /*left*/),(__vcast_rename_at_573_78));
        das_move((__mks_579.right /*right*/),(__enull_rename_at_547_63));
        {
            set_das_string(das_arg<das::string>::pass(__mks_579.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_579_79->genFlags /*genFlags*/,__expr_rename_at_547_62->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_579_79);
}}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_69da28483a9e1aa1 ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_547_81, smart_ptr_raw<Expression> __enull_rename_at_547_82 ) { das_stack_prologue __prologue(__context__,224,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_548_83; memset((void*)&__vdr_rename_at_548_83,0,sizeof(__vdr_rename_at_548_83));
    smart_ptr_raw<ExprConstString> __cna_rename_at_549_85; memset((void*)&__cna_rename_at_549_85,0,sizeof(__cna_rename_at_549_85));
    smart_ptr_raw<ExprOp2> __cond_rename_at_550_87; memset((void*)&__cond_rename_at_550_87,0,sizeof(__cond_rename_at_550_87));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_551_89; memset((void*)&__ctype_rename_at_551_89,0,sizeof(__ctype_rename_at_551_89));
    smart_ptr_raw<Expression> __esub_rename_at_555_91; memset((void*)&__esub_rename_at_555_91,0,sizeof(__esub_rename_at_555_91));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_557_92; memset((void*)&__neqz_rename_at_557_92,0,sizeof(__neqz_rename_at_557_92));
    smart_ptr_raw<ExprOp2> __nand_rename_at_561_94; memset((void*)&__nand_rename_at_561_94,0,sizeof(__nand_rename_at_561_94));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_567_96; memset((void*)&__nsub_rename_at_567_96,0,sizeof(__nsub_rename_at_567_96));
    smart_ptr_raw<ExprCast> __vcast_rename_at_573_97; memset((void*)&__vcast_rename_at_573_97,0,sizeof(__vcast_rename_at_573_97));
    smart_ptr_raw<ExprOp3> __tri_rename_at_579_98; memset((void*)&__tri_rename_at_579_98,0,sizeof(__tri_rename_at_579_98));
    /* finally */ auto __finally_547= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_579_98);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_573_97);
    das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__esub_rename_at_555_91);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_551_89);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_550_87);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_549_85);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_548_83);
    /* end finally */ });
    __vdr_rename_at_548_83; das_zero(__vdr_rename_at_548_83); das_move(__vdr_rename_at_548_83, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_548) {
        das_copy((__mks_548.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
        das_move((__mks_548.value /*value*/),(clone_expression(__expr_rename_at_547_81->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_548.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_549_85; das_zero(__cna_rename_at_549_85); das_move(__cna_rename_at_549_85, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_549) {
        das_copy((__mks_549.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_549.text /*value*/,__expr_rename_at_547_81->name /*name*/);
        }    }))));
    __cond_rename_at_550_87; das_zero(__cond_rename_at_550_87); das_move(__cond_rename_at_550_87, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_550) {
        das_copy((__mks_550.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
        das_move((__mks_550.left /*left*/),(__vdr_rename_at_548_83));
        das_move((__mks_550.right /*right*/),(__cna_rename_at_549_85));
        {
            set_das_string(das_arg<das::string>::pass(__mks_550.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_551_89; das_zero(__ctype_rename_at_551_89); das_move(__ctype_rename_at_551_89, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_551) {
        das_copy((__mks_551.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
        das_copy((__mks_551.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_551.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_553) {
            das_copy((__mks_553.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
            das_copy((__mks_553.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_553.alias /*alias*/,__expr_rename_at_547_81->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_555_91; das_zero(__esub_rename_at_555_91); das_move(__esub_rename_at_555_91, clone_expression(__expr_rename_at_547_81->value /*value*/));
    if ( __expr_rename_at_547_81->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_556= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_561_94);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_557_92);
        /* end finally */ });
        __neqz_rename_at_557_92; das_zero(__neqz_rename_at_557_92); das_move(__neqz_rename_at_557_92, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_557) {
            das_copy((__mks_557.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
            das_move((__mks_557.left /*left*/),(clone_expression(__expr_rename_at_547_81->value /*value*/)));
            das_move((__mks_557.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_559) {
                das_copy((__mks_559.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_557.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_561_94; das_zero(__nand_rename_at_561_94); das_move(__nand_rename_at_561_94, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_561) {
            das_copy((__mks_561.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
            das_move((__mks_561.left /*left*/),(__neqz_rename_at_557_92));
            das_move((__mks_561.right /*right*/),(__cond_rename_at_550_87));
            {
                set_das_string(das_arg<das::string>::pass(__mks_561.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_550_87),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_561_94),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_566= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_567_96);
        /* end finally */ });
        __nsub_rename_at_567_96; das_zero(__nsub_rename_at_567_96); das_move(__nsub_rename_at_567_96, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_567) {
            das_copy((__mks_567.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
            das_move((__mks_567.subexpr /*subexpr*/),(__esub_rename_at_555_91));
            das_copy((__mks_567.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_555_91),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_567_96),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_573_97; das_zero(__vcast_rename_at_573_97); das_move(__vcast_rename_at_573_97, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_573) {
        das_copy((__mks_573.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
        das_move((__mks_573.subexpr /*subexpr*/),(__esub_rename_at_555_91));
        das_move((__mks_573.castType /*castType*/),(__ctype_rename_at_551_89));
        das_copy((__mks_573.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_573.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_579_98; das_zero(__tri_rename_at_579_98); das_move(__tri_rename_at_579_98, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_579) {
        das_copy((__mks_579.at /*at*/),(__expr_rename_at_547_81->at /*at*/));
        das_move((__mks_579.subexpr /*subexpr*/),(__cond_rename_at_550_87));
        das_move((__mks_579.left /*left*/),(__vcast_rename_at_573_97));
        das_move((__mks_579.right /*right*/),(__enull_rename_at_547_82));
        {
            set_das_string(das_arg<das::string>::pass(__mks_579.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_579_98->genFlags /*genFlags*/,__expr_rename_at_547_81->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_579_98);
}}

inline Annotation * _FuncbuiltinTickget_ptrTick8468476673553620226_b515b7b0b5b85a70 ( Context * __context__, smart_ptr_raw<Annotation> const  __src_rename_at_1907_100 )
{
    return das_auto_cast<Annotation *>::cast(das_cast<Annotation *>::cast(__src_rename_at_1907_100));
}

inline void clone_23a052359607d993 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_727_101, smart_ptr_raw<TypeDecl> const  __src_rename_at_727_102 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_727_101),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_727_102),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_a8386ed128f4f0b4 ( Context * __context__, smart_ptr_raw<Enumeration> & __dest_rename_at_882_103, Enumeration * const  __src_rename_at_882_104 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_882_103),das_auto_cast<void * const >::cast(__src_rename_at_882_104),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_479fe631f4b1cb3 ( Context * __context__, char * const  __name_rename_at_970_105 )
{
    return das_auto_cast<Module *>::cast(_FuncastTickfind_moduleTick11101329256228773934_b65022313a84d5bd(__context__,compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_970_105));
}

inline void clone_98d3170c0efdddcc ( Context * __context__, smart_ptr_raw<Annotation> & __dest_rename_at_968_106, smart_ptr_raw<Annotation> const  __src_rename_at_968_107 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_968_106),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_968_107),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick842554968825501494_5630d72624d0f510 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_108 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_108,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1907_109 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1907_109));
}

inline bool isVectorType_5a1ccbd394d2d488 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_26_110 )
{
    return das_auto_cast<bool>::cast(((((((((((((__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tFloat2)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__typ_rename_at_26_110 == DAS_COMMENT(bound_enum) das::Type::tURange64));
}

inline char * describe_a0a74ab06eec48e6 ( Context * __context__, AnnotationArgumentList const  &  __list_rename_at_37_111 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_39_18_rename_at_39_112;das_zero(____acomp_39_18_rename_at_39_112);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_39_18_rename_at_39_112),false);
        {
            bool __need_loop_39 = true;
            // arg: rtti::AnnotationArgument const&
            das_iterator<AnnotationArgumentList const > __arg_iterator(__list_rename_at_37_111);
            AnnotationArgument const  * __arg_rename_at_39_113;
            __need_loop_39 = __arg_iterator.first(__context__,(__arg_rename_at_39_113)) && __need_loop_39;
            for ( ; __need_loop_39 ; __need_loop_39 = __arg_iterator.next(__context__,(__arg_rename_at_39_113)) )
            {
                TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_39_50_0; _temp_make_local_39_50_0;
                _FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f(__context__,das_arg<TArray<char *>>::pass(____acomp_39_18_rename_at_39_112),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_3, cast<das::string const  &>::from((*__arg_rename_at_39_113).name /*name*/), cast<char * const >::from(((char *) "=")), cast<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> &>::from((_temp_make_local_39_50_0 = (rtti_builtin_argument_value((*__arg_rename_at_39_113),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_39_113));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_39_18_rename_at_39_112),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_39_18_rename_at_39_112);
    })),((char *) ",")));
}

inline char * describe_fcbed85e0402159e ( Context * __context__, AnnotationDeclaration const  &  __ann_rename_at_42_114 )
{
    return das_auto_cast<char *>::cast((das_vector_length(__ann_rename_at_42_114.arguments /*arguments*/) != 0) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_4, cast<das::string const  &>::from(__ann_rename_at_42_114.annotation /*annotation*/->name /*name*/), cast<char * const >::from(((char *) "(")), cast<char * const >::from(describe_a0a74ab06eec48e6(__context__,__ann_rename_at_42_114.arguments /*arguments*/)), cast<char * const >::from(((char *) ")"))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<das::string const  &>::from(__ann_rename_at_42_114.annotation /*annotation*/->name /*name*/)))));
}

inline char * describe_6a04757d0b7596f7 ( Context * __context__, AnnotationList const  &  __list_rename_at_51_115 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_53_18_rename_at_53_116;das_zero(____acomp_53_18_rename_at_53_116);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_53_18_rename_at_53_116),false);
        {
            bool __need_loop_53 = true;
            // arg: smart_ptr<rtti::AnnotationDeclaration> const&
            das_iterator<AnnotationList const > __arg_iterator(__list_rename_at_51_115);
            smart_ptr_raw<AnnotationDeclaration> const  * __arg_rename_at_53_117;
            __need_loop_53 = __arg_iterator.first(__context__,(__arg_rename_at_53_117)) && __need_loop_53;
            for ( ; __need_loop_53 ; __need_loop_53 = __arg_iterator.next(__context__,(__arg_rename_at_53_117)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49(__context__,das_arg<TArray<char *>>::pass(____acomp_53_18_rename_at_53_116),describe_fcbed85e0402159e(__context__,das_deref(__context__,(*__arg_rename_at_53_117),__FILE__,__LINE__)));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_53_117));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_53_18_rename_at_53_116),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_53_18_rename_at_53_116);
    })),((char *) ",")));
}

inline char * describe_24ebe982465d57fc ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_56_118 )
{
    return das_auto_cast<char *>::cast((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__vvar_rename_at_56_118->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<das::string const  &>::from(__vvar_rename_at_56_118->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,__vvar_rename_at_56_118->type /*_type*/,true,true,true))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_7, cast<das::string const  &>::from(__vvar_rename_at_56_118->name /*name*/), cast<char * const >::from(((char *) ":null"))))));
}

inline bool isExpression_f1fadf50b6070b86 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_80_119, bool __top_rename_at_80_120 )
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__t_rename_at_80_119),das_auto_cast<void * const >::cast(nullptr)) )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( das_vector_length(__t_rename_at_80_119->dim /*dim*/) != 0 )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( __t_rename_at_80_119->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle )
    {
        return das_auto_cast<bool>::cast((eq_dstr_str(__t_rename_at_80_119->annotation /*annotation*/->module /*_module*/->name /*name*/,((char *) "ast"))) ? das_auto_cast<bool>::cast(builtin_string_startswith(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<das::string const  &>::from(__t_rename_at_80_119->annotation /*annotation*/->name /*name*/))),((char *) "Expr"),__context__)) : das_auto_cast<bool>::cast(false));
    } else return das_auto_cast<bool>::cast(((__t_rename_at_80_119->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer) && __top_rename_at_80_120) ? das_auto_cast<bool>::cast(isExpression_f1fadf50b6070b86(__context__,__t_rename_at_80_119->firstType /*firstType*/,false)) : das_auto_cast<bool>::cast(false));
}

inline bool is_same_or_inherited_66408d7f398767b ( Context * __context__, Structure const  * const  __parent_rename_at_99_121, Structure const  * const  __child_rename_at_99_122 )
{
    Structure const  * __ch_rename_at_101_123 = __child_rename_at_99_122;
    while ( __ch_rename_at_101_123 != nullptr )
    {
        if ( __parent_rename_at_99_121 == __ch_rename_at_101_123 )
        {
            return das_auto_cast<bool>::cast(true);
        } else {
            das_copy(__ch_rename_at_101_123,__ch_rename_at_101_123->parent /*parent*/);
        };
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool is_class_method_4783315be6e05a7b ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_111_124, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_111_125 )
{
    return das_auto_cast<bool>::cast((__finfo_rename_at_111_125->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tFunction) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_111_125->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_111_125->argTypes /*argTypes*/) == 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_111_125->argTypes /*argTypes*/,0,__context__)->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tStructure) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_111_125->argTypes /*argTypes*/,0,__context__)->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(is_same_or_inherited_66408d7f398767b(__context__,das_reinterpret<Structure const  * const >::pass(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_111_125->argTypes /*argTypes*/,0,__context__)->structType /*structType*/),das_reinterpret<Structure const  * const >::pass(_FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc(__context__,__cinfo_rename_at_111_124)))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))))))))));
}

inline void emplace_new_9bef84999dd1e3f4 ( Context * __context__, das::vector<smart_ptr<Expression>> &  __vec_rename_at_134_126, smart_ptr_raw<Expression> __ptr_rename_at_134_127 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__vec_rename_at_134_126),__ptr_rename_at_134_127);
}

inline void emplace_new_31c417adc7c7987e ( Context * __context__, das::vector<smart_ptr<TypeDecl>> &  __vec_rename_at_139_128, smart_ptr_raw<TypeDecl> __ptr_rename_at_139_129 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__vec_rename_at_139_128),__ptr_rename_at_139_129);
}

inline void emplace_new_2f0cb94c081aada2 ( Context * __context__, das::vector<smart_ptr<Variable>> &  __vec_rename_at_144_130, smart_ptr_raw<Variable> __ptr_rename_at_144_131 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Variable>>>::pass(__vec_rename_at_144_130),__ptr_rename_at_144_131);
}

inline void emplace_new_75514f67779c4fa1 ( Context * __context__, MakeStruct &  __vec_rename_at_149_132, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_149_133 )
{
    mks_vector_emplace(das_arg<MakeStruct>::pass(__vec_rename_at_149_132),__ptr_rename_at_149_133);
}

inline bool override_method_8162bb655dd2cd37 ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_154_134, char * const  __name_rename_at_154_135, char * const  __funcName_rename_at_154_136 ) { das_stack_prologue __prologue(__context__,256,"override_method " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprCast> __vcast_rename_at_158_138; memset((void*)&__vcast_rename_at_158_138,0,sizeof(__vcast_rename_at_158_138));
    {
        bool __need_loop_156 = true;
        // fld: ast::FieldDeclaration&
        das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__str_rename_at_154_134->fields /*fields*/);
        Structure::FieldDeclaration * __fld_rename_at_156_137;
        __need_loop_156 = __fld_iterator.first(__context__,(__fld_rename_at_156_137)) && __need_loop_156;
        for ( ; __need_loop_156 ; __need_loop_156 = __fld_iterator.next(__context__,(__fld_rename_at_156_137)) )
        {
            if ( eq_dstr_str(das_arg<das::string>::pass((*__fld_rename_at_156_137).name /*name*/),__name_rename_at_154_135) )
            {
                /* finally */ auto __finally_157= das_finally([&](){
                das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_158_138);
                /* end finally */ });
                __vcast_rename_at_158_138; das_zero(__vcast_rename_at_158_138); das_move(__vcast_rename_at_158_138, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_158) {
                    das_copy((__mks_158.at /*at*/),(__str_rename_at_154_134->at /*at*/));
                    das_move((__mks_158.subexpr /*subexpr*/),(das_ascend_handle<true,smart_ptr_raw<ExprAddr>>::make(__context__,nullptr,(([&](ExprAddr & __mks_159) {
                        das_copy((__mks_159.at /*at*/),(__str_rename_at_154_134->at /*at*/));
                        {
                            set_das_string(das_arg<das::string>::pass(__mks_159.target /*target*/),__funcName_rename_at_154_136);
                        }                    })))));
                    das_move((__mks_158.castType /*castType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_160) {
                        das_copy((__mks_160.at /*at*/),(__str_rename_at_154_134->at /*at*/));
                        das_copy((__mks_160.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                    })))));
                }))));
                builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast((*__fld_rename_at_156_137).init /*init*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__vcast_rename_at_158_138),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                return das_auto_cast<bool>::cast(true);
            };
        }
        __fld_iterator.close(__context__,(__fld_rename_at_156_137));
    };
    return das_auto_cast<bool>::cast(false);
}}

inline void for_each_tag_function_bb49dc3a279a3322 ( Context * __context__, Module * const  __mod_rename_at_214_140, char * const  __tag_rename_at_214_141, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __blk_rename_at_214_142 ) { das_stack_prologue __prologue(__context__,224,"for_each_tag_function " DAS_FILE_LINE);
{
    forEachFunction(__mod_rename_at_214_140,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_215_143) -> void{
        {
            bool __need_loop_216 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_215_143->annotations /*annotations*/);
            smart_ptr_raw<AnnotationDeclaration> * __ann_rename_at_216_144;
            __need_loop_216 = __ann_iterator.first(__context__,(__ann_rename_at_216_144)) && __need_loop_216;
            for ( ; __need_loop_216 ; __need_loop_216 = __ann_iterator.next(__context__,(__ann_rename_at_216_144)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_216_144)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_216_144)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_218 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_216_144)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_218_145;
                        __need_loop_218 = __t_iterator.first(__context__,(__t_rename_at_218_145)) && __need_loop_218;
                        for ( ; __need_loop_218 ; __need_loop_218 = __t_iterator.next(__context__,(__t_rename_at_218_145)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_218_145).name /*name*/),__tag_rename_at_214_141) )
                            {
                                das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__blk_rename_at_214_142,__func_rename_at_215_143);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_218_145));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_216_144));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    forEachGenericFunction(__mod_rename_at_214_140,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_226_146) -> void{
        {
            bool __need_loop_227 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_226_146->annotations /*annotations*/);
            smart_ptr_raw<AnnotationDeclaration> * __ann_rename_at_227_147;
            __need_loop_227 = __ann_iterator.first(__context__,(__ann_rename_at_227_147)) && __need_loop_227;
            for ( ; __need_loop_227 ; __need_loop_227 = __ann_iterator.next(__context__,(__ann_rename_at_227_147)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_227_147)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_227_147)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_229 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_227_147)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_229_148;
                        __need_loop_229 = __t_iterator.first(__context__,(__t_rename_at_229_148)) && __need_loop_229;
                        for ( ; __need_loop_229 ; __need_loop_229 = __t_iterator.next(__context__,(__t_rename_at_229_148)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_229_148).name /*name*/),__tag_rename_at_214_141) )
                            {
                                das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__blk_rename_at_214_142,__func_rename_at_226_146);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_229_148));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_227_147));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_416380068da636fe ( Context * __context__, char * const  __argn_rename_at_240_149, AnnotationArgumentList const  &  __args_rename_at_240_150 )
{
    {
        bool __need_loop_242 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_240_150);
        AnnotationArgument const  * __a_rename_at_242_151;
        __need_loop_242 = __a_iterator.first(__context__,(__a_rename_at_242_151)) && __need_loop_242;
        for ( ; __need_loop_242 ; __need_loop_242 = __a_iterator.next(__context__,(__a_rename_at_242_151)) )
        {
            if ( eq_dstr_str((*__a_rename_at_242_151).name /*name*/,__argn_rename_at_240_149) )
            {
                return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(rtti_builtin_argument_value((*__a_rename_at_242_151),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_242_151));
    };
    return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8(__context__));
}

inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_34945b603c757892 ( Context * __context__, AnnotationArgumentList const  &  __args_rename_at_250_152, char * const  __argn_rename_at_250_153 )
{
    {
        bool __need_loop_252 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_250_152);
        AnnotationArgument const  * __a_rename_at_252_154;
        __need_loop_252 = __a_iterator.first(__context__,(__a_rename_at_252_154)) && __need_loop_252;
        for ( ; __need_loop_252 ; __need_loop_252 = __a_iterator.next(__context__,(__a_rename_at_252_154)) )
        {
            if ( eq_dstr_str((*__a_rename_at_252_154).name /*name*/,__argn_rename_at_250_153) )
            {
                return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(rtti_builtin_argument_value((*__a_rename_at_252_154),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_252_154));
    };
    return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8(__context__));
}

inline void apply_tag_annotation_1944b9a89fd11326 ( Context * __context__, char * const  __tag_rename_at_260_155, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_260_156 ) { das_stack_prologue __prologue(__context__,128,"apply_tag_annotation " DAS_FILE_LINE);
{
    TArray<smart_ptr_raw<Function>> __funcs_rename_at_261_157; memset((void*)&__funcs_rename_at_261_157,0,sizeof(__funcs_rename_at_261_157));
    smart_ptr_raw<FunctionAnnotation> __tagAnn_rename_at_267_160; memset((void*)&__tagAnn_rename_at_267_160,0,sizeof(__tagAnn_rename_at_267_160));
    /* finally */ auto __finally_260= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_261_157));
    /* end finally */ });
    das_zero(__funcs_rename_at_261_157);
    for_each_tag_function_bb49dc3a279a3322(__context__,thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__tag_rename_at_260_155,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__806bd08fbfbd09f4,[&](smart_ptr_raw<Function> __func_rename_at_262_158) -> void{
        _FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_261_157),__func_rename_at_262_158);
    }));
    {
        bool __need_loop_265 = true;
        // func: smart_ptr<ast::Function> aka FunctionPtr&
        das_iterator<TArray<smart_ptr_raw<Function>>> __func_iterator(__funcs_rename_at_261_157);
        smart_ptr_raw<Function> * __func_rename_at_265_159;
        __need_loop_265 = __func_iterator.first(__context__,(__func_rename_at_265_159)) && __need_loop_265;
        for ( ; __need_loop_265 ; __need_loop_265 = __func_iterator.next(__context__,(__func_rename_at_265_159)) )
        {
            {
                /* finally */ auto __finally_266= das_finally([&](){
                das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__tagAnn_rename_at_267_160);
                /* end finally */ });
                __tagAnn_rename_at_267_160; das_zero(__tagAnn_rename_at_267_160); das_move(__tagAnn_rename_at_267_160, _FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0(__context__,__ann_rename_at_260_156));
                addFunctionFunctionAnnotation((*__func_rename_at_265_159),__tagAnn_rename_at_267_160,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __func_iterator.close(__context__,(__func_rename_at_265_159));
    };
}}

inline smart_ptr_raw<Function> find_unique_function_b15ea43980af8102 ( Context * __context__, Module * const  __mod_rename_at_280_161, char * const  __name_rename_at_280_162, bool __canfail_rename_at_280_163 ) { das_stack_prologue __prologue(__context__,128,"find_unique_function " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_282_164; memset((void*)&__res_rename_at_282_164,0,sizeof(__res_rename_at_282_164));
    int32_t __count_rename_at_283_165; memset((void*)&__count_rename_at_283_165,0,sizeof(__count_rename_at_283_165));
    /* finally */ auto __finally_280= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_282_164);
    /* end finally */ });
    das_zero(__res_rename_at_282_164);
    __count_rename_at_283_165 = 0;
    forEachFunction(__mod_rename_at_280_161,__name_rename_at_280_162,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_284_166) -> void{
        clone_e5dbb6c9a183080e(__context__,__res_rename_at_282_164,__func_rename_at_284_166);
        ++__count_rename_at_283_165;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_283_165 > 1 )
    {
        if ( !__canfail_rename_at_280_163 )
        {
            builtin_throw(((char *) "more than one function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_282_164);
    };
}}

inline smart_ptr_raw<Function> find_unique_generic_4054e5bb45ca1054 ( Context * __context__, Module * const  __mod_rename_at_297_167, char * const  __name_rename_at_297_168, bool __canfail_rename_at_297_169 ) { das_stack_prologue __prologue(__context__,128,"find_unique_generic " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_299_170; memset((void*)&__res_rename_at_299_170,0,sizeof(__res_rename_at_299_170));
    int32_t __count_rename_at_300_171; memset((void*)&__count_rename_at_300_171,0,sizeof(__count_rename_at_300_171));
    /* finally */ auto __finally_297= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_299_170);
    /* end finally */ });
    das_zero(__res_rename_at_299_170);
    __count_rename_at_300_171 = 0;
    forEachGenericFunction(__mod_rename_at_297_167,__name_rename_at_297_168,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_301_172) -> void{
        clone_e5dbb6c9a183080e(__context__,__res_rename_at_299_170,__func_rename_at_301_172);
        ++__count_rename_at_300_171;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_300_171 > 1 )
    {
        if ( !__canfail_rename_at_297_169 )
        {
            builtin_throw(((char *) "more than one generic function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_299_170);
    };
}}

inline ExprBlock * setup_call_list_729e5db2cab0e291 ( Context * __context__, char * const  __name_rename_at_314_173, LineInfo const  &  __at_rename_at_314_174, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __subblock_rename_at_314_175 ) { das_stack_prologue __prologue(__context__,144,"setup_call_list " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_316_176; memset((void*)&__fn_rename_at_316_176,0,sizeof(__fn_rename_at_316_176));
    smart_ptr_raw<ExprBlock> __blk_rename_at_322_178; memset((void*)&__blk_rename_at_322_178,0,sizeof(__blk_rename_at_322_178));
    ExprBlock * __res_rename_at_323_179; memset((void*)&__res_rename_at_323_179,0,sizeof(__res_rename_at_323_179));
    /* finally */ auto __finally_314= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_316_176);
    /* end finally */ });
    __fn_rename_at_316_176; das_zero(__fn_rename_at_316_176); das_move(__fn_rename_at_316_176, find_unique_function_b15ea43980af8102(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_314_173,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_316_176),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_317= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_322_178);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_316_176),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_318) {
            das_copy((__mks_318.at /*at*/),(__at_rename_at_314_174));
            das_copy((__mks_318.atDecl /*atDecl*/),(__at_rename_at_314_174));
            das_copy((__mks_318.flags /*flags*/),(0x200000u));
            das_move((__mks_318.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_320) {
                das_copy((__mks_320.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_320.at /*at*/),(__at_rename_at_314_174));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_318.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_9, cast<char * const >::from(__name_rename_at_314_173))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_322_178; das_zero(__blk_rename_at_322_178); das_move(__blk_rename_at_322_178, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_322) {
            das_copy((__mks_322.at /*at*/),(__at_rename_at_314_174));
        }))));
        __res_rename_at_323_179 = _FuncbuiltinTickget_ptrTick5807679485210906136_c8d3f222daa2e3cc(__context__,__blk_rename_at_322_178);
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_316_176->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_322_178),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__subblock_rename_at_314_175,__fn_rename_at_316_176);
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_316_176,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_10, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_316_176->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(__res_rename_at_323_179);
    } else if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_316_176->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
    {
        builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__fn_rename_at_316_176->body /*body*/));
}}

inline ExprBlock * setup_call_list_7b65a2c40054d8e8 ( Context * __context__, char * const  __name_rename_at_336_180, LineInfo const  &  __at_rename_at_336_181, bool __isInit_rename_at_336_182, bool __isPrivate_rename_at_336_183, bool __isLateInit_rename_at_336_184 ) { das_stack_prologue __prologue(__context__,96,"setup_call_list " DAS_FILE_LINE);
{
    return das_auto_cast<ExprBlock *>::cast(setup_call_list_729e5db2cab0e291(__context__,__name_rename_at_336_180,__at_rename_at_336_181,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__3b2c4edb933bef4a,[&](smart_ptr_raw<Function> __fn_rename_at_338_185) -> void{
        if ( __isInit_rename_at_336_182 )
        {
            __fn_rename_at_338_185->flags /*flags*/ |= 0x100u;
            if ( __isLateInit_rename_at_336_184 )
            {
                __fn_rename_at_338_185->moreFlags /*moreFlags*/ |= 0x8u;
            };
        };
        if ( __isPrivate_rename_at_336_183 )
        {
            __fn_rename_at_338_185->flags /*flags*/ |= 0x400000u;
        };
    })));
}}

inline ExprBlock * setup_macro_e906336cb4687a9c ( Context * __context__, char * const  __name_rename_at_351_186, LineInfo const  &  __at_rename_at_351_187 ) { das_stack_prologue __prologue(__context__,208,"setup_macro " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_354_188; memset((void*)&__fn_rename_at_354_188,0,sizeof(__fn_rename_at_354_188));
    smart_ptr_raw<ExprBlock> __blk_rename_at_360_190; memset((void*)&__blk_rename_at_360_190,0,sizeof(__blk_rename_at_360_190));
    smart_ptr_raw<ExprBlock> __iblk_rename_at_361_191; memset((void*)&__iblk_rename_at_361_191,0,sizeof(__iblk_rename_at_361_191));
    ExprBlock * __res_rename_at_362_192; memset((void*)&__res_rename_at_362_192,0,sizeof(__res_rename_at_362_192));
    smart_ptr_raw<ExprCall> __ifm_rename_at_363_193; memset((void*)&__ifm_rename_at_363_193,0,sizeof(__ifm_rename_at_363_193));
    smart_ptr_raw<ExprConstString> __ifmn_rename_at_364_195; memset((void*)&__ifmn_rename_at_364_195,0,sizeof(__ifmn_rename_at_364_195));
    smart_ptr_raw<ExprIfThenElse> __ife_rename_at_366_197; memset((void*)&__ife_rename_at_366_197,0,sizeof(__ife_rename_at_366_197));
    ExprBlock * __blk_rename_at_378_198; memset((void*)&__blk_rename_at_378_198,0,sizeof(__blk_rename_at_378_198));
    ExprIfThenElse * __ife_rename_at_382_199; memset((void*)&__ife_rename_at_382_199,0,sizeof(__ife_rename_at_382_199));
    /* finally */ auto __finally_351= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_354_188);
    /* end finally */ });
    compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->flags /*flags*/ |= 0x20u;
    __fn_rename_at_354_188; das_zero(__fn_rename_at_354_188); das_move(__fn_rename_at_354_188, find_unique_function_b15ea43980af8102(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_351_186,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_354_188),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_355= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprIfThenElse>>::clear(__context__,__ife_rename_at_366_197);
        das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__ifmn_rename_at_364_195);
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__ifm_rename_at_363_193);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__iblk_rename_at_361_191);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_360_190);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_354_188),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_356) {
            das_copy((__mks_356.at /*at*/),(__at_rename_at_351_187));
            das_copy((__mks_356.atDecl /*atDecl*/),(__at_rename_at_351_187));
            das_copy((__mks_356.flags /*flags*/),(0x80600000u));
            das_move((__mks_356.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_358) {
                das_copy((__mks_358.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_358.at /*at*/),(__at_rename_at_351_187));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_356.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_9, cast<char * const >::from(__name_rename_at_351_186))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_360_190; das_zero(__blk_rename_at_360_190); das_move(__blk_rename_at_360_190, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_360) {
            das_copy((__mks_360.at /*at*/),(__at_rename_at_351_187));
        }))));
        __iblk_rename_at_361_191; das_zero(__iblk_rename_at_361_191); das_move(__iblk_rename_at_361_191, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_361) {
            das_copy((__mks_361.at /*at*/),(__at_rename_at_351_187));
        }))));
        __res_rename_at_362_192 = ((ExprBlock *)_FuncbuiltinTickget_ptrTick5807679485210906136_c8d3f222daa2e3cc(__context__,__iblk_rename_at_361_191));
        __ifm_rename_at_363_193; das_zero(__ifm_rename_at_363_193); das_move(__ifm_rename_at_363_193, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_363) {
            das_copy((__mks_363.at /*at*/),(__at_rename_at_351_187));
            {
                set_das_string(das_arg<das::string>::pass(__mks_363.name /*name*/),((char *) "is_compiling_macros_in_module"));
            }        }))));
        __ifmn_rename_at_364_195; das_zero(__ifmn_rename_at_364_195); das_move(__ifmn_rename_at_364_195, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_364) {
            das_copy((__mks_364.at /*at*/),(__at_rename_at_351_187));
            {
                das_clone<das::string,das::string>::clone(__mks_364.text /*value*/,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->name /*name*/);
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__ifm_rename_at_363_193->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__ifmn_rename_at_364_195));
        __ife_rename_at_366_197; das_zero(__ife_rename_at_366_197); das_move(__ife_rename_at_366_197, das_ascend_handle<true,smart_ptr_raw<ExprIfThenElse>>::make(__context__,nullptr,(([&](ExprIfThenElse & __mks_366) {
            das_copy((__mks_366.at /*at*/),(__at_rename_at_351_187));
            das_move((__mks_366.cond /*cond*/),(__ifm_rename_at_363_193));
            das_move((__mks_366.if_true /*if_true*/),(__iblk_rename_at_361_191));
        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_360_190->list /*list*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__ife_rename_at_366_197));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_354_188->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_360_190),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_354_188,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_10, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_354_188->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__res_rename_at_362_192));
    } else {
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_354_188->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __blk_rename_at_378_198 = das_cast<ExprBlock *>::cast(__fn_rename_at_354_188->body /*body*/);
        if ( (das_vector_length(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_378_198->list /*list*/)) != 1) || (SimPolicy<char *>::NotEqu(cast<char *>::from(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_378_198->list /*list*/,0,__context__)->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprIfThenElse")),*__context__,nullptr)) )
        {
            builtin_throw(((char *) "expecting is_compiling_macros"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __ife_rename_at_382_199 = das_cast<ExprIfThenElse *>::cast(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_378_198->list /*list*/,0,__context__));
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__ife_rename_at_382_199->if_true /*if_true*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting ife.if_true.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__ife_rename_at_382_199->if_true /*if_true*/));
    };
}}

inline void * panic_expr_as_2ea20f674c842c9c ( Context * __context__ )
{
    builtin_throw(((char *) "invalid 'as' expression or null pointer dereference"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast<void *>::cast(nullptr);
}

inline bool _FuncTickisTickBuiltInFunction_3d26f4806589821b ( Context * __context__, Function * const  __foo_rename_at_623_200 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__foo_rename_at_623_200->flags /*flags*/,1u << 0));
}

inline BuiltInFunction * _FuncTickasTickBuiltInFunction_c47ad3b03c9bc354 ( Context * __context__, Function * const  __foo_rename_at_628_201 )
{
    DAS_VERIFY((_FuncTickisTickBuiltInFunction_3d26f4806589821b(__context__,__foo_rename_at_628_201)));
    return das_auto_cast<BuiltInFunction *>::cast(das_cast<BuiltInFunction *>::cast(__foo_rename_at_628_201));
}

inline bool _FuncTickisTickExternalFnBase_24b69557e6b14161 ( Context * __context__, Function * const  __foo_rename_at_639_202 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__foo_rename_at_639_202->moreFlags /*moreFlags*/,1u << 12) && das_get_bitfield(__foo_rename_at_639_202->flags /*flags*/,1u << 0));
}

inline ExternalFnBase * _FuncTickasTickExternalFnBase_b840ea32468fb731 ( Context * __context__, Function * const  __foo_rename_at_644_203 )
{
    DAS_VERIFY((_FuncTickisTickExternalFnBase_24b69557e6b14161(__context__,__foo_rename_at_644_203)));
    return das_auto_cast<ExternalFnBase *>::cast(das_cast<ExternalFnBase *>::cast(__foo_rename_at_644_203));
}

inline bool _FuncTickisTickFunctionAnnotation_eccaa0a9bcb3796 ( Context * __context__, Annotation * const  __foo_rename_at_655_204 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_655_204,__FILE__,__LINE__)).rtti_isFunctionAnnotation()));
}

inline bool _FuncTickisTickFunctionAnnotation_6d5ce87249ff0eaf ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_660_205 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_660_205,__FILE__,__LINE__)).rtti_isFunctionAnnotation()));
}

inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_8d76f010548d92f6 ( Context * __context__, Annotation * const  __foo_rename_at_665_206 )
{
    DAS_VERIFY((_FuncTickisTickFunctionAnnotation_eccaa0a9bcb3796(__context__,__foo_rename_at_665_206)));
    return das_auto_cast<FunctionAnnotation *>::cast(das_cast<FunctionAnnotation *>::cast(__foo_rename_at_665_206));
}

inline FunctionAnnotation * _FuncTickasTickFunctionAnnotation_6e0707ffcc0c2b76 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_671_207 )
{
    DAS_VERIFY((_FuncTickisTickFunctionAnnotation_6d5ce87249ff0eaf(__context__,__foo_rename_at_671_207)));
    return das_auto_cast<FunctionAnnotation *>::cast(das_cast<FunctionAnnotation *>::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_b515b7b0b5b85a70(__context__,__foo_rename_at_671_207)));
}

inline bool _FuncTickisTickStructureAnnotation_471f8e2f8678905f ( Context * __context__, Annotation * const  __foo_rename_at_683_208 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_683_208,__FILE__,__LINE__)).rtti_isStructureAnnotation()));
}

inline bool _FuncTickisTickStructureAnnotation_1883d933e82a8bcc ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_688_209 )
{
    return das_auto_cast<bool>::cast(((das_deref(__context__,__foo_rename_at_688_209,__FILE__,__LINE__)).rtti_isStructureAnnotation()));
}

inline StructureAnnotation * _FuncTickasTickStructureAnnotation_1f9c68dfdd32ff55 ( Context * __context__, Annotation * const  __foo_rename_at_693_210 )
{
    DAS_VERIFY((_FuncTickisTickStructureAnnotation_471f8e2f8678905f(__context__,__foo_rename_at_693_210)));
    return das_auto_cast<StructureAnnotation *>::cast(das_cast<StructureAnnotation *>::cast(__foo_rename_at_693_210));
}

inline StructureAnnotation * _FuncTickasTickStructureAnnotation_1a5c23b3eb803935 ( Context * __context__, smart_ptr_raw<Annotation> const  __foo_rename_at_699_211 )
{
    DAS_VERIFY((_FuncTickisTickStructureAnnotation_1883d933e82a8bcc(__context__,__foo_rename_at_699_211)));
    return das_auto_cast<StructureAnnotation *>::cast(das_cast<StructureAnnotation *>::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_b515b7b0b5b85a70(__context__,__foo_rename_at_699_211)));
}

inline smart_ptr_raw<Expression> walk_and_convert_array_ba1cc70e35611268 ( Context * __context__, uint8_t const  * const  __data_rename_at_705_212, smart_ptr_raw<TypeDecl> const  __info_rename_at_705_213, LineInfo const  &  __at_rename_at_705_214 ) { das_stack_prologue __prologue(__context__,176,"walk_and_convert_array " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_708_216; memset((void*)&__stride_rename_at_708_216,0,sizeof(__stride_rename_at_708_216));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_709_217; memset((void*)&__mkArr_rename_at_709_217,0,sizeof(__mkArr_rename_at_709_217));
    smart_ptr_raw<ExprCall> __mkToArrayMove_rename_at_714_219; memset((void*)&__mkToArrayMove_rename_at_714_219,0,sizeof(__mkToArrayMove_rename_at_714_219));
    int32_t __total_rename_at_706_215 = ((int32_t)any_array_size(das_auto_cast<void * const >::cast(__data_rename_at_705_212)));
    if ( __total_rename_at_706_215 != 0 )
    {
        /* finally */ auto __finally_707= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToArrayMove_rename_at_714_219);
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_709_217);
        /* end finally */ });
        __stride_rename_at_708_216 = ((int32_t)((das_deref(__context__,__info_rename_at_705_213->firstType /*firstType*/,__FILE__,__LINE__)).getSizeOf()));
        __mkArr_rename_at_709_217; das_zero(__mkArr_rename_at_709_217); das_move(__mkArr_rename_at_709_217, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_709) {
            das_copy((__mks_709.at /*at*/),(__at_rename_at_705_214));
            das_move((__mks_709.makeType /*makeType*/),(clone_type(__info_rename_at_705_213->firstType /*firstType*/)));
        }))));
        any_array_foreach(das_auto_cast<void * const >::cast(__data_rename_at_705_212),__stride_rename_at_708_216,das_make_block<void,void *>(__context__,128,0,&__func_info__b28b606ddb7a6760,[&](void * __value_rename_at_710_218) -> void{
            emplace_new_9bef84999dd1e3f4(__context__,das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_709_217->values /*values*/),walk_and_convert_cfdd8313a6a5987(__context__,das_auto_cast<uint8_t const  * const >::cast(__value_rename_at_710_218),__info_rename_at_705_213->firstType /*firstType*/,__at_rename_at_705_214));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_709_217->makeType /*makeType*/->dim /*dim*/),__total_rename_at_706_215);
        __mkToArrayMove_rename_at_714_219; das_zero(__mkToArrayMove_rename_at_714_219); das_move(__mkToArrayMove_rename_at_714_219, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_714) {
            das_copy((__mks_714.at /*at*/),(__at_rename_at_705_214));
            {
                set_das_string(das_arg<das::string>::pass(__mks_714.name /*name*/),((char *) "to_array_move"));
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToArrayMove_rename_at_714_219->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkArr_rename_at_709_217));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkToArrayMove_rename_at_714_219);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_718) {
            das_copy((__mks_718.at /*at*/),(__at_rename_at_705_214));
            das_move((__mks_718.makeType /*makeType*/),(clone_type(__info_rename_at_705_213)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_dim_9001b9148a98cff6 ( Context * __context__, uint8_t const  * const  __data_rename_at_722_221, smart_ptr_raw<TypeDecl> const  __info_rename_at_722_222, LineInfo const  &  __at_rename_at_722_223 ) { das_stack_prologue __prologue(__context__,176,"walk_and_convert_dim " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_723_224; memset((void*)&__stride_rename_at_723_224,0,sizeof(__stride_rename_at_723_224));
    int32_t __total_rename_at_724_225; memset((void*)&__total_rename_at_724_225,0,sizeof(__total_rename_at_724_225));
    smart_ptr_raw<TypeDecl> __einfo_rename_at_725_226; memset((void*)&__einfo_rename_at_725_226,0,sizeof(__einfo_rename_at_725_226));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_727_227; memset((void*)&__mkArr_rename_at_727_227,0,sizeof(__mkArr_rename_at_727_227));
    smart_ptr_raw<Expression> __elem_rename_at_730_230; memset((void*)&__elem_rename_at_730_230,0,sizeof(__elem_rename_at_730_230));
    /* finally */ auto __finally_722= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_727_227);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__einfo_rename_at_725_226);
    /* end finally */ });
    __stride_rename_at_723_224 = ((int32_t)((das_deref(__context__,__info_rename_at_722_222,__FILE__,__LINE__)).getBaseSizeOf()));
    __total_rename_at_724_225 = ((int32_t)((das_deref(__context__,__info_rename_at_722_222,__FILE__,__LINE__)).getCountOf()));
    __einfo_rename_at_725_226; das_zero(__einfo_rename_at_725_226); das_move(__einfo_rename_at_725_226, clone_type(__info_rename_at_722_222));
    das_vector_clear(das_arg<das::vector<int32_t>>::pass(__einfo_rename_at_725_226->dim /*dim*/));
    __mkArr_rename_at_727_227; das_zero(__mkArr_rename_at_727_227); das_move(__mkArr_rename_at_727_227, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_727) {
        das_copy((__mks_727.at /*at*/),(__at_rename_at_722_223));
        {
            clone_23a052359607d993(__context__,__mks_727.makeType /*makeType*/,__info_rename_at_722_222);
        }    }))));
    {
        bool __need_loop_728 = true;
        // x: int const
        das_iterator<range> __x_iterator(mk_range(__total_rename_at_724_225));
        int32_t __x_rename_at_728_229;
        __need_loop_728 = __x_iterator.first(__context__,(__x_rename_at_728_229)) && __need_loop_728;
        for ( ; __need_loop_728 ; __need_loop_728 = __x_iterator.next(__context__,(__x_rename_at_728_229)) )
        {
            __elem_rename_at_730_230; das_zero(__elem_rename_at_730_230); das_move(__elem_rename_at_730_230, walk_and_convert_cfdd8313a6a5987(__context__,das_ptr_add_int32(__data_rename_at_722_221,__stride_rename_at_723_224 * __x_rename_at_728_229,1),__einfo_rename_at_725_226,__at_rename_at_722_223));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_727_227->values /*values*/),__elem_rename_at_730_230);
        }
        __x_iterator.close(__context__,(__x_rename_at_728_229));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_727_227);
}}

inline smart_ptr_raw<Expression> walk_and_convert_pointer_47b794d0dd59337d ( Context * __context__, uint8_t const  * const  __data_rename_at_737_231, smart_ptr_raw<TypeDecl> const  __info_rename_at_737_232, LineInfo const  &  __at_rename_at_737_233 )
{
    uint8_t const  * __pdata_rename_at_739_234 = ((uint8_t const  *)das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_737_231),__FILE__,__LINE__));
    smart_ptr_raw<Expression> __elem_rename_at_740_235; das_zero(__elem_rename_at_740_235); das_move(__elem_rename_at_740_235, walk_and_convert_cfdd8313a6a5987(__context__,__pdata_rename_at_739_234,__info_rename_at_737_232->firstType /*firstType*/,__at_rename_at_737_233));
    smart_ptr_raw<ExprAscend> __mkAsc_rename_at_741_236; das_zero(__mkAsc_rename_at_741_236); das_move(__mkAsc_rename_at_741_236, das_ascend_handle<true,smart_ptr_raw<ExprAscend>>::make(__context__,nullptr,(([&](ExprAscend & __mks_741) {
        das_copy((__mks_741.at /*at*/),(__at_rename_at_737_233));
        das_move((__mks_741.subexpr /*subexpr*/),(__elem_rename_at_740_235));
    }))));
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkAsc_rename_at_741_236);
}

inline smart_ptr_raw<Expression> walk_and_convert_tuple_9ce3b13721ba5daf ( Context * __context__, uint8_t const  * const  __data_rename_at_746_237, smart_ptr_raw<TypeDecl> const  __info_rename_at_746_238, LineInfo const  &  __at_rename_at_746_239 )
{
    smart_ptr_raw<ExprMakeTuple> __mkArr_rename_at_747_240; memset((void*)&__mkArr_rename_at_747_240,0,sizeof(__mkArr_rename_at_747_240));
    int32_t __offset_rename_at_749_242; memset((void*)&__offset_rename_at_749_242,0,sizeof(__offset_rename_at_749_242));
    smart_ptr_raw<Expression> __elem_rename_at_751_243; memset((void*)&__elem_rename_at_751_243,0,sizeof(__elem_rename_at_751_243));
    /* finally */ auto __finally_746= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkArr_rename_at_747_240);
    /* end finally */ });
    __mkArr_rename_at_747_240; das_zero(__mkArr_rename_at_747_240); das_move(__mkArr_rename_at_747_240, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_747) {
        das_copy((__mks_747.at /*at*/),(__at_rename_at_746_239));
        das_move((__mks_747.recordType /*recordType*/),(clone_type(__info_rename_at_746_238)));
    }))));
    {
        bool __need_loop_748 = true;
        // idx: int const
        das_iterator<range> __idx_iterator(mk_range(das_vector_length(__info_rename_at_746_238->argTypes /*argTypes*/)));
        int32_t __idx_rename_at_748_241;
        __need_loop_748 = __idx_iterator.first(__context__,(__idx_rename_at_748_241)) && __need_loop_748;
        for ( ; __need_loop_748 ; __need_loop_748 = __idx_iterator.next(__context__,(__idx_rename_at_748_241)) )
        {
            __offset_rename_at_749_242 = ((int32_t)get_tuple_field_offset(__info_rename_at_746_238,__idx_rename_at_748_241,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            __elem_rename_at_751_243; das_zero(__elem_rename_at_751_243); das_move(__elem_rename_at_751_243, walk_and_convert_cfdd8313a6a5987(__context__,das_ptr_add_int32(__data_rename_at_746_237,__offset_rename_at_749_242,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_746_238->argTypes /*argTypes*/,__idx_rename_at_748_241,__context__),__at_rename_at_746_239));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_747_240->values /*values*/),__elem_rename_at_751_243);
        }
        __idx_iterator.close(__context__,(__idx_rename_at_748_241));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_747_240);
}

inline smart_ptr_raw<Expression> walk_and_convert_structure_106d0a1bf460f63c ( Context * __context__, uint8_t const  * const  __data_rename_at_758_244, smart_ptr_raw<TypeDecl> const  __info_rename_at_758_245, LineInfo const  &  __at_rename_at_758_246 ) { das_stack_prologue __prologue(__context__,352,"walk_and_convert_structure " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprMakeStruct> __mkStruct_rename_at_759_247; memset((void*)&__mkStruct_rename_at_759_247,0,sizeof(__mkStruct_rename_at_759_247));
    Structure * __stype_rename_at_760_248; memset((void*)&__stype_rename_at_760_248,0,sizeof(__stype_rename_at_760_248));
    smart_ptr_raw<MakeStruct> __mkS_rename_at_761_249; memset((void*)&__mkS_rename_at_761_249,0,sizeof(__mkS_rename_at_761_249));
    uint8_t const  * __fdata_rename_at_765_251; memset((void*)&__fdata_rename_at_765_251,0,sizeof(__fdata_rename_at_765_251));
    smart_ptr_raw<Expression> __elem_rename_at_766_252; memset((void*)&__elem_rename_at_766_252,0,sizeof(__elem_rename_at_766_252));
    smart_ptr_raw<MakeFieldDecl> __mkF_rename_at_767_253; memset((void*)&__mkF_rename_at_767_253,0,sizeof(__mkF_rename_at_767_253));
    /* finally */ auto __finally_758= das_finally([&](){
    das_delete_handle<smart_ptr_raw<MakeStruct>>::clear(__context__,__mkS_rename_at_761_249);
    das_delete_handle<smart_ptr_raw<ExprMakeStruct>>::clear(__context__,__mkStruct_rename_at_759_247);
    /* end finally */ });
    __mkStruct_rename_at_759_247; das_zero(__mkStruct_rename_at_759_247); das_move(__mkStruct_rename_at_759_247, das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_759) {
        das_copy((__mks_759.at /*at*/),(__at_rename_at_758_246));
        das_move((__mks_759.makeType /*makeType*/),(clone_type(__info_rename_at_758_245)));
    }))));
    __stype_rename_at_760_248 = ((Structure *)__info_rename_at_758_245->structType /*structType*/);
    __mkS_rename_at_761_249; das_zero(__mkS_rename_at_761_249); das_move(__mkS_rename_at_761_249, das_new_handle<MakeStruct,true>::make(__context__));
    {
        bool __need_loop_762 = true;
        // x: ast::FieldDeclaration const&
        das_iterator<das::vector<Structure::FieldDeclaration> const > __x_iterator(__stype_rename_at_760_248->fields /*fields*/);
        Structure::FieldDeclaration const  * __x_rename_at_762_250;
        __need_loop_762 = __x_iterator.first(__context__,(__x_rename_at_762_250)) && __need_loop_762;
        for ( ; __need_loop_762 ; __need_loop_762 = __x_iterator.next(__context__,(__x_rename_at_762_250)) )
        {
            TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_763_28_1; _temp_make_local_763_28_1;
            if ( das_get_variant_field<vec4f,16,8>::is((_temp_make_local_763_28_1 = (find_arg_34945b603c757892(__context__,(*__x_rename_at_762_250).annotation /*annotation*/,((char *) "do_not_convert"))))) )
            {
                __fdata_rename_at_765_251 = ((uint8_t const  *)das_ptr_add_int32(__data_rename_at_758_244,(*__x_rename_at_762_250).offset /*offset*/,1));
                __elem_rename_at_766_252; das_zero(__elem_rename_at_766_252); das_move(__elem_rename_at_766_252, walk_and_convert_cfdd8313a6a5987(__context__,__fdata_rename_at_765_251,(*__x_rename_at_762_250).type /*_type*/,__at_rename_at_758_246));
                __mkF_rename_at_767_253; das_zero(__mkF_rename_at_767_253); das_move(__mkF_rename_at_767_253, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_767) {
                    das_copy((__mks_767.at /*at*/),(__at_rename_at_758_246));
                    das_move((__mks_767.value /*value*/),(__elem_rename_at_766_252));
                    {
                        das_clone<das::string,das::string>::clone(__mks_767.name /*name*/,(*__x_rename_at_762_250).name /*name*/);
                    }                }))));
                if ( !((das_deref(__context__,(*__x_rename_at_762_250).type /*_type*/,__FILE__,__LINE__)).canCopy()) )
                {
                    das_copy(__mkF_rename_at_767_253->flags /*flags*/,0x1u);
                };
                emplace_new_75514f67779c4fa1(__context__,das_arg<MakeStruct>::pass(das_deref(__context__,__mkS_rename_at_761_249,__FILE__,__LINE__)),__mkF_rename_at_767_253);
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_762_250));
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeStruct>>>::pass(__mkStruct_rename_at_759_247->structs /*structs*/),__mkS_rename_at_761_249);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkStruct_rename_at_759_247);
}}

inline smart_ptr_raw<Expression> walk_and_convert_variant_cdeba207e46113d9 ( Context * __context__, uint8_t const  * const  __data_rename_at_779_255, smart_ptr_raw<TypeDecl> const  __info_rename_at_779_256, LineInfo const  &  __at_rename_at_779_257 ) { das_stack_prologue __prologue(__context__,192,"walk_and_convert_variant " DAS_FILE_LINE);
{
    int32_t __vindex_rename_at_781_258 = ((int32_t)das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_779_255),__FILE__,__LINE__));
    int32_t __offset_rename_at_782_259 = ((int32_t)get_variant_field_offset(__info_rename_at_779_256,__vindex_rename_at_781_258,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    smart_ptr_raw<ExprMakeVariant> __mkVariant_rename_at_783_260; das_zero(__mkVariant_rename_at_783_260); das_move(__mkVariant_rename_at_783_260, das_ascend_handle<true,smart_ptr_raw<ExprMakeVariant>>::make(__context__,nullptr,(([&](ExprMakeVariant & __mks_783) {
        das_copy((__mks_783.at /*at*/),(__at_rename_at_779_257));
        das_move((__mks_783.makeType /*makeType*/),(clone_type(__info_rename_at_779_256)));
    }))));
    smart_ptr_raw<Expression> __elem_rename_at_784_261; das_zero(__elem_rename_at_784_261); das_move(__elem_rename_at_784_261, walk_and_convert_cfdd8313a6a5987(__context__,das_ptr_add_int32(__data_rename_at_779_255,__offset_rename_at_782_259,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_779_256->argTypes /*argTypes*/,__vindex_rename_at_781_258,__context__),__at_rename_at_779_257));
    smart_ptr_raw<MakeFieldDecl> __mkS_rename_at_785_262; das_zero(__mkS_rename_at_785_262); das_move(__mkS_rename_at_785_262, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_785) {
        das_copy((__mks_785.at /*at*/),(__at_rename_at_779_257));
        das_move((__mks_785.value /*value*/),(__elem_rename_at_784_261));
        {
            das_clone<das::string,das::string>::clone(__mks_785.name /*name*/,das_index<das::vector<das::string> const >::at(__info_rename_at_779_256->argNames /*argNames*/,__vindex_rename_at_781_258,__context__));
        }    }))));
    if ( !((das_deref(__context__,das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_779_256->argTypes /*argTypes*/,__vindex_rename_at_781_258,__context__),__FILE__,__LINE__)).canCopy()) )
    {
        das_copy(__mkS_rename_at_785_262->flags /*flags*/,0x1u);
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeFieldDecl>>>::pass(__mkVariant_rename_at_783_260->variants /*variants*/),__mkS_rename_at_785_262);
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkVariant_rename_at_783_260);
}}

inline smart_ptr_raw<Expression> walk_and_convert_table_bafed583a64bc3a1 ( Context * __context__, uint8_t const  * const  __data_rename_at_794_264, smart_ptr_raw<TypeDecl> const  __info_rename_at_794_265, LineInfo const  &  __at_rename_at_794_266 ) { das_stack_prologue __prologue(__context__,240,"walk_and_convert_table " DAS_FILE_LINE);
{
    smart_ptr_raw<TypeDecl> __tupT_rename_at_797_268; memset((void*)&__tupT_rename_at_797_268,0,sizeof(__tupT_rename_at_797_268));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_800_269; memset((void*)&__mkArr_rename_at_800_269,0,sizeof(__mkArr_rename_at_800_269));
    int32_t __key_stride_rename_at_801_270; memset((void*)&__key_stride_rename_at_801_270,0,sizeof(__key_stride_rename_at_801_270));
    int32_t __value_stride_rename_at_802_271; memset((void*)&__value_stride_rename_at_802_271,0,sizeof(__value_stride_rename_at_802_271));
    smart_ptr_raw<ExprCall> __mkToTableMove_rename_at_812_277; memset((void*)&__mkToTableMove_rename_at_812_277,0,sizeof(__mkToTableMove_rename_at_812_277));
    int32_t __total_rename_at_795_267 = ((int32_t)any_table_size(das_auto_cast<void * const >::cast(__data_rename_at_794_264)));
    if ( __total_rename_at_795_267 != 0 )
    {
        /* finally */ auto __finally_796= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToTableMove_rename_at_812_277);
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_800_269);
        das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__tupT_rename_at_797_268);
        /* end finally */ });
        __tupT_rename_at_797_268; das_zero(__tupT_rename_at_797_268); das_move(__tupT_rename_at_797_268, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_797) {
            das_copy((__mks_797.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tTuple));
            das_copy((__mks_797.at /*at*/),(__at_rename_at_794_266));
        }))));
        emplace_new_31c417adc7c7987e(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_797_268->argTypes /*argTypes*/),clone_type(__info_rename_at_794_265->firstType /*firstType*/));
        emplace_new_31c417adc7c7987e(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_797_268->argTypes /*argTypes*/),clone_type(__info_rename_at_794_265->secondType /*secondType*/));
        __mkArr_rename_at_800_269; das_zero(__mkArr_rename_at_800_269); das_move(__mkArr_rename_at_800_269, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_800) {
            das_copy((__mks_800.at /*at*/),(__at_rename_at_794_266));
            das_move((__mks_800.makeType /*makeType*/),(clone_type(__tupT_rename_at_797_268)));
        }))));
        __key_stride_rename_at_801_270 = ((int32_t)((das_deref(__context__,__info_rename_at_794_265->firstType /*firstType*/,__FILE__,__LINE__)).getSizeOf()));
        __value_stride_rename_at_802_271 = ((int32_t)((das_deref(__context__,__info_rename_at_794_265->secondType /*secondType*/,__FILE__,__LINE__)).getSizeOf()));
        any_table_foreach(das_auto_cast<void * const >::cast(__data_rename_at_794_264),__key_stride_rename_at_801_270,__value_stride_rename_at_802_271,das_make_block<void,void *,void *>(__context__,160,0,&__func_info__b50c88059cb1bcc,[&](void * __pkey_rename_at_803_272, void * __pvalue_rename_at_803_273) -> void{
            smart_ptr_raw<Expression> __key_rename_at_804_274; memset((void*)&__key_rename_at_804_274,0,sizeof(__key_rename_at_804_274));
            smart_ptr_raw<Expression> __value_rename_at_805_275; memset((void*)&__value_rename_at_805_275,0,sizeof(__value_rename_at_805_275));
            smart_ptr_raw<ExprMakeTuple> __mkTup_rename_at_806_276; memset((void*)&__mkTup_rename_at_806_276,0,sizeof(__mkTup_rename_at_806_276));
            /* finally */ auto __finally_803= das_finally([&](){
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__value_rename_at_805_275);
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__key_rename_at_804_274);
            das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkTup_rename_at_806_276);
            /* end finally */ });
            __key_rename_at_804_274; das_zero(__key_rename_at_804_274); das_move(__key_rename_at_804_274, walk_and_convert_cfdd8313a6a5987(__context__,das_auto_cast<uint8_t const  * const >::cast(__pkey_rename_at_803_272),__info_rename_at_794_265->firstType /*firstType*/,__at_rename_at_794_266));
            __value_rename_at_805_275; das_zero(__value_rename_at_805_275); das_move(__value_rename_at_805_275, walk_and_convert_cfdd8313a6a5987(__context__,das_auto_cast<uint8_t const  * const >::cast(__pvalue_rename_at_803_273),__info_rename_at_794_265->secondType /*secondType*/,__at_rename_at_794_266));
            __mkTup_rename_at_806_276; das_zero(__mkTup_rename_at_806_276); das_move(__mkTup_rename_at_806_276, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_806) {
                das_copy((__mks_806.at /*at*/),(__at_rename_at_794_266));
                das_move((__mks_806.recordType /*recordType*/),(clone_type(__tupT_rename_at_797_268)));
                das_copy((__mks_806.isKeyValue /*isKeyValue*/),(true));
            }))));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_806_276->values /*values*/),__key_rename_at_804_274);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_806_276->values /*values*/),__value_rename_at_805_275);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_800_269->values /*values*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkTup_rename_at_806_276));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_800_269->makeType /*makeType*/->dim /*dim*/),__total_rename_at_795_267);
        __mkToTableMove_rename_at_812_277; das_zero(__mkToTableMove_rename_at_812_277); das_move(__mkToTableMove_rename_at_812_277, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_812) {
            das_copy((__mks_812.at /*at*/),(__at_rename_at_794_266));
            {
                set_das_string(das_arg<das::string>::pass(__mks_812.name /*name*/),((char *) "to_table_move"));
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToTableMove_rename_at_812_277->arguments /*arguments*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__mkArr_rename_at_800_269));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkToTableMove_rename_at_812_277);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_816) {
            das_copy((__mks_816.at /*at*/),(__at_rename_at_794_266));
            das_move((__mks_816.makeType /*makeType*/),(clone_type(__info_rename_at_794_265)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_basic_aef53343044f9782 ( Context * __context__, uint8_t const  * const  __data_rename_at_820_279, smart_ptr_raw<TypeDecl> const  __info_rename_at_820_280, LineInfo const  &  __at_rename_at_820_281 ) { das_stack_prologue __prologue(__context__,112,"walk_and_convert_basic " DAS_FILE_LINE);
{
    if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt8 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt8>>::make(__context__,nullptr,(([&](ExprConstInt8 & __mks_823) {
            das_copy((__mks_823.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_823.cvalue<int8_t>() /*value*/),(das_deref(__context__,das_cast<int8_t *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt16 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt16>>::make(__context__,nullptr,(([&](ExprConstInt16 & __mks_825) {
            das_copy((__mks_825.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_825.cvalue<int16_t>() /*value*/),(das_deref(__context__,das_cast<int16_t *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt>>::make(__context__,nullptr,(([&](ExprConstInt & __mks_827) {
            das_copy((__mks_827.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_827.cvalue<int32_t>() /*value*/),(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt64>>::make(__context__,nullptr,(([&](ExprConstInt64 & __mks_829) {
            das_copy((__mks_829.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_829.cvalue<int64_t>() /*value*/),(das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt8 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt8>>::make(__context__,nullptr,(([&](ExprConstUInt8 & __mks_831) {
            das_copy((__mks_831.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_831.cvalue<uint8_t>() /*value*/),(das_deref(__context__,das_cast<uint8_t *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt16 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt16>>::make(__context__,nullptr,(([&](ExprConstUInt16 & __mks_833) {
            das_copy((__mks_833.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_833.cvalue<uint16_t>() /*value*/),(das_deref(__context__,das_cast<uint16_t *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt>>::make(__context__,nullptr,(([&](ExprConstUInt & __mks_835) {
            das_copy((__mks_835.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_835.cvalue<uint32_t>() /*value*/),(das_deref(__context__,das_cast<uint32_t *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt64>>::make(__context__,nullptr,(([&](ExprConstUInt64 & __mks_837) {
            das_copy((__mks_837.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_837.cvalue<uint64_t>() /*value*/),(das_deref(__context__,das_cast<uint64_t *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstFloat>>::make(__context__,nullptr,(([&](ExprConstFloat & __mks_839) {
            das_copy((__mks_839.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_839.cvalue<float>() /*value*/),(das_deref(__context__,das_cast<float *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tDouble )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstDouble>>::make(__context__,nullptr,(([&](ExprConstDouble & __mks_841) {
            das_copy((__mks_841.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_841.cvalue<double>() /*value*/),(das_deref(__context__,das_cast<double *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBool>>::make(__context__,nullptr,(([&](ExprConstBool & __mks_843) {
            das_copy((__mks_843.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_843.cvalue<bool>() /*value*/),(das_deref(__context__,das_cast<bool *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange>>::make(__context__,nullptr,(([&](ExprConstRange & __mks_845) {
            das_copy((__mks_845.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_845.cvalue<range>() /*value*/),(das_deref(__context__,das_cast<range *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange>>::make(__context__,nullptr,(([&](ExprConstURange & __mks_847) {
            das_copy((__mks_847.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_847.cvalue<urange>() /*value*/),(das_deref(__context__,das_cast<urange *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange64>>::make(__context__,nullptr,(([&](ExprConstRange64 & __mks_849) {
            das_copy((__mks_849.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_849.cvalue<range64>() /*value*/),(das_deref(__context__,das_cast<range64 *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange64>>::make(__context__,nullptr,(([&](ExprConstURange64 & __mks_851) {
            das_copy((__mks_851.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_851.cvalue<urange64>() /*value*/),(das_deref(__context__,das_cast<urange64 *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
        }))));
    } else if ( __info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_853) {
            das_copy((__mks_853.at /*at*/),(__at_rename_at_820_281));
            {
                set_das_string(das_arg<das::string>::pass(__mks_853.text /*value*/),das_deref(__context__,das_cast<char * *>::cast(__data_rename_at_820_279),__FILE__,__LINE__));
            }        }))));
    } else if ( (((__info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield8) || (__info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield16)) || (__info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield)) || (__info_rename_at_820_280->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield64) )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBitfield>>::make(__context__,nullptr,(([&](ExprConstBitfield & __mks_858) {
            das_copy((__mks_858.at /*at*/),(__at_rename_at_820_281));
            das_copy((__mks_858.cvalue<Bitfield>() /*value*/),(das_deref(__context__,das_cast<Bitfield *>::cast(__data_rename_at_820_279),__FILE__,__LINE__)));
            {
                clone_23a052359607d993(__context__,__mks_858.bitfieldType /*bitfieldType*/,__info_rename_at_820_280);
            }        }))));
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<char * const >::from(((char *) "unsupported ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__info_rename_at_820_280->baseType /*baseType*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(nullptr);
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_enumeration_5bb2d478c4ffeaaa ( Context * __context__, uint8_t const  * const  __data_rename_at_866_284, smart_ptr_raw<TypeDecl> const  __info_rename_at_866_285, LineInfo const  &  __at_rename_at_866_286 ) { das_stack_prologue __prologue(__context__,144,"walk_and_convert_enumeration " DAS_FILE_LINE);
{
    int64_t __eval_rename_at_867_287 = INT64_C(0);
    if ( __info_rename_at_866_285->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration )
    {
        das_copy(__eval_rename_at_867_287,int64_t(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_866_284),__FILE__,__LINE__)));
    } else if ( __info_rename_at_866_285->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8 )
    {
        das_copy(__eval_rename_at_867_287,int64_t(das_deref(__context__,__data_rename_at_866_284,__FILE__,__LINE__)));
    } else if ( __info_rename_at_866_285->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16 )
    {
        das_copy(__eval_rename_at_867_287,int64_t(das_deref(__context__,das_cast<int16_t *>::cast(__data_rename_at_866_284),__FILE__,__LINE__)));
    } else if ( __info_rename_at_866_285->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64 )
    {
        das_copy(__eval_rename_at_867_287,das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_866_284),__FILE__,__LINE__));
    } else {
        builtin_throw(((char *) "unsupported enumeration"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    char * __name_rename_at_879_288 = ((char *)(char *)(((char * const )(ast_find_enum_name(__info_rename_at_866_285->enumType /*enumType*/,__eval_rename_at_867_287,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_879_288),cast<char *>::from(nullptr),*__context__,nullptr) )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_14, cast<char * const >::from(((char *) "can't find name of enum value ")), cast<int64_t>::from(__eval_rename_at_867_287), cast<char * const >::from(((char *) " in ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,__info_rename_at_866_285,true,true,true)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstEnumeration>>::make(__context__,nullptr,(([&](ExprConstEnumeration & __mks_881) {
        {
            set_das_string(das_arg<das::string>::pass(__mks_881.text /*value*/),__name_rename_at_879_288);
            clone_a8386ed128f4f0b4(__context__,__mks_881.enumType /*enumType*/,das_cast<Enumeration *>::cast(__info_rename_at_866_285->enumType /*enumType*/));
        }    }))));
}}

inline smart_ptr_raw<Expression> walk_and_convert_cfdd8313a6a5987 ( Context * __context__, uint8_t const  * const  __data_rename_at_886_290, smart_ptr_raw<TypeDecl> const  __info_rename_at_886_291, LineInfo const  &  __at_rename_at_886_292 )
{
    if ( das_vector_length(__info_rename_at_886_291->dim /*dim*/) != 0 )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_dim_9001b9148a98cff6(__context__,__data_rename_at_886_290,__info_rename_at_886_291,__at_rename_at_886_292));
    } else if ( __info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tArray )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_array_ba1cc70e35611268(__context__,__data_rename_at_886_290,__info_rename_at_886_291,__at_rename_at_886_292));
    } else if ( __info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        if ( das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_886_290),__FILE__,__LINE__) == nullptr )
        {
            return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_896) {
                das_copy((__mks_896.at /*at*/),(__at_rename_at_886_292));
            }))));
        } else return das_auto_cast<smart_ptr_raw<Expression>>::cast(((equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__info_rename_at_886_291->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr))) || ((das_deref(__context__,__info_rename_at_886_291->firstType /*firstType*/,__FILE__,__LINE__)).isVoid())) ? das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_898) {
            das_copy((__mks_898.at /*at*/),(__at_rename_at_886_292));
        })))) : das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(walk_and_convert_pointer_47b794d0dd59337d(__context__,__data_rename_at_886_290,__info_rename_at_886_291,__at_rename_at_886_292)));
    } else if ( __info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tStructure )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_structure_106d0a1bf460f63c(__context__,__data_rename_at_886_290,__info_rename_at_886_291,__at_rename_at_886_292));
    } else if ( __info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tVariant )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_variant_cdeba207e46113d9(__context__,__data_rename_at_886_290,__info_rename_at_886_291,__at_rename_at_886_292));
    } else if ( __info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTuple )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_tuple_9ce3b13721ba5daf(__context__,__data_rename_at_886_290,__info_rename_at_886_291,__at_rename_at_886_292));
    } else if ( __info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTable )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_table_bafed583a64bc3a1(__context__,__data_rename_at_886_290,__info_rename_at_886_291,__at_rename_at_886_292));
    } else return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(((((__info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8) || (__info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16)) || (__info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64)) || (__info_rename_at_886_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration)) ? das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_enumeration_5bb2d478c4ffeaaa(__context__,__data_rename_at_886_290,__info_rename_at_886_291,__at_rename_at_886_292)) : das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_basic_aef53343044f9782(__context__,__data_rename_at_886_290,__info_rename_at_886_291,__at_rename_at_886_292)));
}

inline Annotation const  * find_annotation_c7c295d23bae9ab4 ( Context * __context__, char * const  __mod_name_rename_at_946_293, char * const  __ann_name_rename_at_946_294 ) { das_stack_prologue __prologue(__context__,128,"find_annotation " DAS_FILE_LINE);
{
    Module * __mod_rename_at_948_295 = _FuncastTickfind_compiling_moduleTick4523452840392654583_479fe631f4b1cb3(__context__,__mod_name_rename_at_946_293);
    if ( __mod_rename_at_948_295 == nullptr )
    {
        return das_auto_cast<Annotation const  *>::cast(nullptr);
    } else {
        Annotation const  * __ann_rename_at_952_296 = 0;
        rtti_builtin_module_for_each_annotation(__mod_rename_at_948_295,das_make_block<void,Annotation const  &>(__context__,112,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_953_297) -> void{
            if ( eq_dstr_str(__value_rename_at_953_297.name /*name*/,__ann_name_rename_at_946_294) )
            {
                das_copy(__ann_rename_at_952_296,das_ref(__context__,__value_rename_at_953_297));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<Annotation const  *>::cast(__ann_rename_at_952_296);
    };
}}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_4862c33d2ab9dcd4 ( Context * __context__, char * const  __mod_name_rename_at_963_298, char * const  __ann_name_rename_at_963_299, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_963_300 )
{
    Annotation const  * __ann_rename_at_965_301; memset((void*)&__ann_rename_at_965_301,0,sizeof(__ann_rename_at_965_301));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_967_302; memset((void*)&__decl_rename_at_967_302,0,sizeof(__decl_rename_at_967_302));
    char * * __argName_rename_at_970_304; memset((void*)&__argName_rename_at_970_304,0,sizeof(__argName_rename_at_970_304));
    TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> * __arg_rename_at_971_305; memset((void*)&__arg_rename_at_971_305,0,sizeof(__arg_rename_at_971_305));
    /* finally */ auto __finally_963= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_967_302);
    /* end finally */ });
    __ann_rename_at_965_301 = find_annotation_c7c295d23bae9ab4(__context__,__mod_name_rename_at_963_298,__ann_name_rename_at_963_299);
    DAS_ASSERT((__ann_rename_at_965_301 != nullptr));
    __decl_rename_at_967_302; das_zero(__decl_rename_at_967_302); das_move(__decl_rename_at_967_302, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_98d3170c0efdddcc(__context__,__decl_rename_at_967_302->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_965_301));
    {
        bool __need_loop_969 = true;
        // argP: tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any> aka RttiValue> const&
        das_iterator<TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const > __argP_iterator(__args_rename_at_963_300);
        TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>> const  * __argP_rename_at_969_303;
        __need_loop_969 = __argP_iterator.first(__context__,(__argP_rename_at_969_303)) && __need_loop_969;
        for ( ; __need_loop_969 ; __need_loop_969 = __argP_iterator.next(__context__,(__argP_rename_at_969_303)) )
        {
            __argName_rename_at_970_304 = ((char * *)&((char * &)(das_get_tuple_field<char *,0>::get((*__argP_rename_at_969_303)))));
            __arg_rename_at_971_305 = ((TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *)&(das_get_tuple_field<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,16>::get((*__argP_rename_at_969_303))));
            if ( das_get_variant_field<bool,16,0>::is((*__arg_rename_at_971_305)) )
            {
                add_annotation_argument_a6c4ec43057e73c(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_967_302->arguments /*arguments*/),(*__argName_rename_at_970_304),das_get_variant_field<bool,16,0>::as((*__arg_rename_at_971_305),__context__));
            } else if ( das_get_variant_field<int32_t,16,1>::is((*__arg_rename_at_971_305)) )
            {
                add_annotation_argument_2ee13ee84bf33b6d(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_967_302->arguments /*arguments*/),(*__argName_rename_at_970_304),das_get_variant_field<int32_t,16,1>::as((*__arg_rename_at_971_305),__context__));
            } else if ( das_get_variant_field<float,16,5>::is((*__arg_rename_at_971_305)) )
            {
                add_annotation_argument_e4f5a170be5edc8b(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_967_302->arguments /*arguments*/),(*__argName_rename_at_970_304),das_get_variant_field<float,16,5>::as((*__arg_rename_at_971_305),__context__));
            } else if ( das_get_variant_field<char *,16,7>::is((*__arg_rename_at_971_305)) )
            {
                add_annotation_argument_d6722d8e520facbc(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_967_302->arguments /*arguments*/),(*__argName_rename_at_970_304),das_get_variant_field<char *,16,7>::as((*__arg_rename_at_971_305),__context__));
            } else {
                builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_15, cast<char * const >::from(((char *) "Invalid rtti value ")), cast<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> const  &>::from((*__arg_rename_at_971_305)), cast<char * const >::from(((char *) " for annotation argument ")), cast<char * const >::from((*__argName_rename_at_970_304)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __argP_iterator.close(__context__,(__argP_rename_at_969_303));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_967_302);
}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_8190e2b8fbb09c12 ( Context * __context__, char * const  __mod_name_rename_at_987_306, char * const  __ann_name_rename_at_987_307 )
{
    Annotation const  * __ann_rename_at_989_308; memset((void*)&__ann_rename_at_989_308,0,sizeof(__ann_rename_at_989_308));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_991_309; memset((void*)&__decl_rename_at_991_309,0,sizeof(__decl_rename_at_991_309));
    /* finally */ auto __finally_987= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_991_309);
    /* end finally */ });
    __ann_rename_at_989_308 = find_annotation_c7c295d23bae9ab4(__context__,__mod_name_rename_at_987_306,__ann_name_rename_at_987_307);
    DAS_ASSERT((__ann_rename_at_989_308 != nullptr));
    __decl_rename_at_991_309; das_zero(__decl_rename_at_991_309); das_move(__decl_rename_at_991_309, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_98d3170c0efdddcc(__context__,__decl_rename_at_991_309->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_989_308));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_991_309);
}

inline void append_annotation_9e9720e2bdf176ae ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_996_310, char * const  __mod_name_rename_at_996_311, char * const  __ann_name_rename_at_996_312 )
{
    Annotation const  * __ann_rename_at_998_313; memset((void*)&__ann_rename_at_998_313,0,sizeof(__ann_rename_at_998_313));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1000_314; memset((void*)&__decl_rename_at_1000_314,0,sizeof(__decl_rename_at_1000_314));
    /* finally */ auto __finally_996= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1000_314);
    /* end finally */ });
    __ann_rename_at_998_313 = find_annotation_c7c295d23bae9ab4(__context__,__mod_name_rename_at_996_311,__ann_name_rename_at_996_312);
    DAS_ASSERT((__ann_rename_at_998_313 != nullptr));
    __decl_rename_at_1000_314; das_zero(__decl_rename_at_1000_314); das_move(__decl_rename_at_1000_314, append_annotation_8190e2b8fbb09c12(__context__,__mod_name_rename_at_996_311,__ann_name_rename_at_996_312));
    addAndApplyFunctionAnnotation(__func_rename_at_996_310,__decl_rename_at_1000_314,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_82649f781a6d03e7 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_1004_315, char * const  __mod_name_rename_at_1004_316, char * const  __ann_name_rename_at_1004_317 )
{
    Annotation const  * __ann_rename_at_1006_318; memset((void*)&__ann_rename_at_1006_318,0,sizeof(__ann_rename_at_1006_318));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1008_319; memset((void*)&__decl_rename_at_1008_319,0,sizeof(__decl_rename_at_1008_319));
    /* finally */ auto __finally_1004= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1008_319);
    /* end finally */ });
    __ann_rename_at_1006_318 = find_annotation_c7c295d23bae9ab4(__context__,__mod_name_rename_at_1004_316,__ann_name_rename_at_1004_317);
    DAS_ASSERT((__ann_rename_at_1006_318 != nullptr));
    __decl_rename_at_1008_319; das_zero(__decl_rename_at_1008_319); das_move(__decl_rename_at_1008_319, append_annotation_8190e2b8fbb09c12(__context__,__mod_name_rename_at_1004_316,__ann_name_rename_at_1004_317));
    addAndApplyBlockAnnotation(__blk_rename_at_1004_315,__decl_rename_at_1008_319,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_1d124a466ae78997 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_1012_320, char * const  __mod_name_rename_at_1012_321, char * const  __ann_name_rename_at_1012_322 )
{
    Annotation const  * __ann_rename_at_1014_323; memset((void*)&__ann_rename_at_1014_323,0,sizeof(__ann_rename_at_1014_323));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1016_324; memset((void*)&__decl_rename_at_1016_324,0,sizeof(__decl_rename_at_1016_324));
    /* finally */ auto __finally_1012= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1016_324);
    /* end finally */ });
    __ann_rename_at_1014_323 = find_annotation_c7c295d23bae9ab4(__context__,__mod_name_rename_at_1012_321,__ann_name_rename_at_1012_322);
    DAS_ASSERT((__ann_rename_at_1014_323 != nullptr));
    __decl_rename_at_1016_324; das_zero(__decl_rename_at_1016_324); das_move(__decl_rename_at_1016_324, append_annotation_8190e2b8fbb09c12(__context__,__mod_name_rename_at_1012_321,__ann_name_rename_at_1012_322));
    addAndApplyStructAnnotation(__st_rename_at_1012_320,__decl_rename_at_1016_324,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_d2aadccfb018501d ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_1020_325, char * const  __mod_name_rename_at_1020_326, char * const  __ann_name_rename_at_1020_327, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_1020_328 )
{
    Annotation const  * __ann_rename_at_1022_329; memset((void*)&__ann_rename_at_1022_329,0,sizeof(__ann_rename_at_1022_329));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1024_330; memset((void*)&__decl_rename_at_1024_330,0,sizeof(__decl_rename_at_1024_330));
    /* finally */ auto __finally_1020= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1024_330);
    /* end finally */ });
    __ann_rename_at_1022_329 = find_annotation_c7c295d23bae9ab4(__context__,__mod_name_rename_at_1020_326,__ann_name_rename_at_1020_327);
    DAS_ASSERT((__ann_rename_at_1022_329 != nullptr));
    __decl_rename_at_1024_330; das_zero(__decl_rename_at_1024_330); das_move(__decl_rename_at_1024_330, append_annotation_4862c33d2ab9dcd4(__context__,__mod_name_rename_at_1020_326,__ann_name_rename_at_1020_327,__args_rename_at_1020_328));
    addAndApplyFunctionAnnotation(__func_rename_at_1020_325,__decl_rename_at_1024_330,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_41a68523ba2c1d9e ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_1028_331, char * const  __mod_name_rename_at_1028_332, char * const  __ann_name_rename_at_1028_333, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_1028_334 )
{
    Annotation const  * __ann_rename_at_1030_335; memset((void*)&__ann_rename_at_1030_335,0,sizeof(__ann_rename_at_1030_335));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1032_336; memset((void*)&__decl_rename_at_1032_336,0,sizeof(__decl_rename_at_1032_336));
    /* finally */ auto __finally_1028= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1032_336);
    /* end finally */ });
    __ann_rename_at_1030_335 = find_annotation_c7c295d23bae9ab4(__context__,__mod_name_rename_at_1028_332,__ann_name_rename_at_1028_333);
    DAS_ASSERT((__ann_rename_at_1030_335 != nullptr));
    __decl_rename_at_1032_336; das_zero(__decl_rename_at_1032_336); das_move(__decl_rename_at_1032_336, append_annotation_4862c33d2ab9dcd4(__context__,__mod_name_rename_at_1028_332,__ann_name_rename_at_1028_333,__args_rename_at_1028_334));
    addAndApplyBlockAnnotation(__blk_rename_at_1028_331,__decl_rename_at_1032_336,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_84797c2112b4fd76 ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_1036_337, char * const  __mod_name_rename_at_1036_338, char * const  __ann_name_rename_at_1036_339, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_1036_340 )
{
    Annotation const  * __ann_rename_at_1038_341; memset((void*)&__ann_rename_at_1038_341,0,sizeof(__ann_rename_at_1038_341));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_1040_342; memset((void*)&__decl_rename_at_1040_342,0,sizeof(__decl_rename_at_1040_342));
    /* finally */ auto __finally_1036= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_1040_342);
    /* end finally */ });
    __ann_rename_at_1038_341 = find_annotation_c7c295d23bae9ab4(__context__,__mod_name_rename_at_1036_338,__ann_name_rename_at_1036_339);
    DAS_ASSERT((__ann_rename_at_1038_341 != nullptr));
    __decl_rename_at_1040_342; das_zero(__decl_rename_at_1040_342); das_move(__decl_rename_at_1040_342, append_annotation_4862c33d2ab9dcd4(__context__,__mod_name_rename_at_1036_338,__ann_name_rename_at_1036_339,__args_rename_at_1036_340));
    addAndApplyStructAnnotation(__st_rename_at_1036_337,__decl_rename_at_1040_342,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t add_annotation_argument_a6c4ec43057e73c ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1044_343, char * const  __argName_rename_at_1044_344, bool __val_rename_at_1044_345 )
{
    int32_t __argIdx_rename_at_1046_346 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1044_343),__argName_rename_at_1044_344));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1044_343,__argIdx_rename_at_1046_346,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tBool);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1044_343,__argIdx_rename_at_1046_346,__context__).bValue /*bValue*/,__val_rename_at_1044_345);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1046_346);
}

inline int32_t add_annotation_argument_2ee13ee84bf33b6d ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1052_347, char * const  __argName_rename_at_1052_348, int32_t __val_rename_at_1052_349 )
{
    int32_t __argIdx_rename_at_1054_350 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1052_347),__argName_rename_at_1052_348));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1052_347,__argIdx_rename_at_1054_350,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tInt);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1052_347,__argIdx_rename_at_1054_350,__context__).iValue /*iValue*/,__val_rename_at_1052_349);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1054_350);
}

inline int32_t add_annotation_argument_e4f5a170be5edc8b ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1060_351, char * const  __argName_rename_at_1060_352, float __val_rename_at_1060_353 )
{
    int32_t __argIdx_rename_at_1062_354 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1060_351),__argName_rename_at_1060_352));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1060_351,__argIdx_rename_at_1062_354,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tFloat);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1060_351,__argIdx_rename_at_1062_354,__context__).fValue /*fValue*/,__val_rename_at_1060_353);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1062_354);
}

inline int32_t add_annotation_argument_d6722d8e520facbc ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1068_355, char * const  __argName_rename_at_1068_356, char * const  __val_rename_at_1068_357 )
{
    int32_t __argIdx_rename_at_1070_358 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1068_355),__argName_rename_at_1068_356));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1068_355,__argIdx_rename_at_1070_358,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tString);
    set_das_string(das_arg<das::string>::pass(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1068_355,__argIdx_rename_at_1070_358,__context__).sValue /*sValue*/),__val_rename_at_1068_357);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1070_358);
}

inline int32_t add_annotation_argument_eb9c96b34ef66276 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_1076_359, AnnotationArgument const  &  __ann_rename_at_1076_360 )
{
    int32_t __argIdx_rename_at_1078_361 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_1076_359),((char * const )(to_das_string(__ann_rename_at_1076_360.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1076_359,__argIdx_rename_at_1078_361,__context__).type /*basicType*/,__ann_rename_at_1076_360.type /*basicType*/);
    if ( __ann_rename_at_1076_360.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1076_359,__argIdx_rename_at_1078_361,__context__).bValue /*bValue*/,__ann_rename_at_1076_360.bValue /*bValue*/);
    } else if ( __ann_rename_at_1076_360.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1076_359,__argIdx_rename_at_1078_361,__context__).iValue /*iValue*/,__ann_rename_at_1076_360.iValue /*iValue*/);
    } else if ( __ann_rename_at_1076_360.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1076_359,__argIdx_rename_at_1078_361,__context__).fValue /*fValue*/,__ann_rename_at_1076_360.fValue /*fValue*/);
    } else if ( __ann_rename_at_1076_360.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        das_clone<das::string,das::string>::clone(das_index<AnnotationArgumentList>::at(__arguments_rename_at_1076_359,__argIdx_rename_at_1078_361,__context__).sValue /*sValue*/,__ann_rename_at_1076_360.sValue /*sValue*/);
    } else {
        DAS_ASSERTF((false),(((char *) "unsupported annotation type, add more types")));
    };
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_1078_361);
}

inline int32_t get_for_source_index_ce1d36791493c0c4 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1094_362, smart_ptr_raw<Variable> const  __svar_rename_at_1094_363 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1096_41_2; _temp_make_local_1096_41_2;
    {
        bool __need_loop_1096 = true;
        // v: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __v_iterator(__expr_rename_at_1094_362->iteratorVariables /*iteratorVariables*/);
        smart_ptr_raw<Variable> const  * __v_rename_at_1096_366;
        __need_loop_1096 = __v_iterator.first(__context__,(__v_rename_at_1096_366)) && __need_loop_1096;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1096_41_2 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_1096_367;
        __need_loop_1096 = __t_iterator.first(__context__,(__t_rename_at_1096_367)) && __need_loop_1096;
        for ( ; __need_loop_1096 ; __need_loop_1096 = __v_iterator.next(__context__,(__v_rename_at_1096_366)) && __t_iterator.next(__context__,(__t_rename_at_1096_367)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__v_rename_at_1096_366)),das_auto_cast<smart_ptr_raw<void> const >::cast(__svar_rename_at_1094_363)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_1096_367);
            };
        }
        __t_iterator.close(__context__,(__t_rename_at_1096_367));
        __v_iterator.close(__context__,(__v_rename_at_1096_366));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline int32_t get_for_source_index_44e105584529041d ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_1104_368, smart_ptr_raw<Expression> const  __source_rename_at_1104_369 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1106_31_3; _temp_make_local_1106_31_3;
    {
        bool __need_loop_1106 = true;
        // s: smart_ptr<ast::Expression> const&
        das_iterator<das::vector<smart_ptr<Expression>> const > __s_iterator(__expr_rename_at_1104_368->sources /*sources*/);
        smart_ptr_raw<Expression> const  * __s_rename_at_1106_372;
        __need_loop_1106 = __s_iterator.first(__context__,(__s_rename_at_1106_372)) && __need_loop_1106;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1106_31_3 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_1106_373;
        __need_loop_1106 = __t_iterator.first(__context__,(__t_rename_at_1106_373)) && __need_loop_1106;
        for ( ; __need_loop_1106 ; __need_loop_1106 = __s_iterator.next(__context__,(__s_rename_at_1106_372)) && __t_iterator.next(__context__,(__t_rename_at_1106_373)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__s_rename_at_1106_372)),das_auto_cast<smart_ptr_raw<void> const >::cast(__source_rename_at_1104_369)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_1106_373);
            };
        }
        __t_iterator.close(__context__,(__t_rename_at_1106_373));
        __s_iterator.close(__context__,(__s_rename_at_1106_372));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline smart_ptr_raw<TypeDecl> function_to_type_1f450dac98d6071d ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_1154_374 )
{
    smart_ptr_raw<TypeDecl> __td_rename_at_1156_375; memset((void*)&__td_rename_at_1156_375,0,sizeof(__td_rename_at_1156_375));
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1160_36_4; _temp_make_local_1160_36_4;
    /* finally */ auto __finally_1154= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__td_rename_at_1156_375);
    /* end finally */ });
    __td_rename_at_1156_375; das_zero(__td_rename_at_1156_375); das_move(__td_rename_at_1156_375, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1156) {
        das_copy((__mks_1156.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tFunction));
        das_move((__mks_1156.firstType /*firstType*/),(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_1154_374->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<smart_ptr_raw<TypeDecl>>::cast(clone_type(__fn_rename_at_1154_374->result /*result*/)) : das_auto_cast<smart_ptr_raw<TypeDecl>>::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1157) {
            das_copy((__mks_1157.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
        })))))));
    }))));
    das_vector_resize(das_arg<das::vector<das::string>>::pass(__td_rename_at_1156_375->argNames /*argNames*/),das_vector_length(__fn_rename_at_1154_374->arguments /*arguments*/));
    {
        bool __need_loop_1160 = true;
        // arg: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __arg_iterator(__fn_rename_at_1154_374->arguments /*arguments*/);
        smart_ptr_raw<Variable> const  * __arg_rename_at_1160_378;
        __need_loop_1160 = __arg_iterator.first(__context__,(__arg_rename_at_1160_378)) && __need_loop_1160;
        // argi: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1160_36_4 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __argi_iterator(0,1);
        int32_t __argi_rename_at_1160_379;
        __need_loop_1160 = __argi_iterator.first(__context__,(__argi_rename_at_1160_379)) && __need_loop_1160;
        for ( ; __need_loop_1160 ; __need_loop_1160 = __arg_iterator.next(__context__,(__arg_rename_at_1160_378)) && __argi_iterator.next(__context__,(__argi_rename_at_1160_379)) )
        {
            emplace_new_31c417adc7c7987e(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__td_rename_at_1156_375->argTypes /*argTypes*/),clone_type((*__arg_rename_at_1160_378)->type /*_type*/));
            das_clone<das::string,das::string>::clone(das_index<das::vector<das::string>>::at(__td_rename_at_1156_375->argNames /*argNames*/,__argi_rename_at_1160_379,__context__),(*__arg_rename_at_1160_378)->name /*name*/);
        }
        __argi_iterator.close(__context__,(__argi_rename_at_1160_379));
        __arg_iterator.close(__context__,(__arg_rename_at_1160_378));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(__td_rename_at_1156_375);
}

inline void visit_finally_5653493f01f579fa ( Context * __context__, ExprBlock * const  __blk_rename_at_1167_380, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1167_381 )
{
    astVisitBlockFinally(das_cast<smart_ptr_raw<ExprBlock>>::cast(__blk_rename_at_1167_380),__adapter_rename_at_1167_381,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool isCMRES_2bb438cbd738593f ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1174_382 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1174_382->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1174_382->flags /*flags*/,1u << 6));
}

inline bool isCMRES_45d360720945cc74 ( Context * __context__, Function * const  __fun_rename_at_1179_383 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1179_383->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1179_383->flags /*flags*/,1u << 6));
}

inline bool isMakeLocal_c461b44592b8facf ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1184_384 )
{
    return das_auto_cast<bool>::cast(((((SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1184_384->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeLocal")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1184_384->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeStruct")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1184_384->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeVariant")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1184_384->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeArray")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1184_384->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeTuple")),*__context__,nullptr)));
}

inline bool isExprCallFunc_aac1b10a17185ef1 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1193_385 )
{
    return das_auto_cast<bool>::cast((((SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1193_385->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCallFunc")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1193_385->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprOp")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1193_385->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprNew")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1193_385->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCall")),*__context__,nullptr)));
}

inline TDim<DAS_COMMENT(bound_enum) das::Type,34> get_workhorse_types_de3ed4d894cfde0c ( Context * __context__ )
{
    return das_auto_cast_ref<TDim<DAS_COMMENT(bound_enum) das::Type,34>>::cast((([&]() -> TDim<DAS_COMMENT(bound_enum) das::Type,34> {
        TDim<DAS_COMMENT(bound_enum) das::Type,34> __mka_1203;
        __mka_1203(0,__context__) = DAS_COMMENT(bound_enum) das::Type::tBool;
        __mka_1203(1,__context__) = DAS_COMMENT(bound_enum) das::Type::tString;
        __mka_1203(2,__context__) = DAS_COMMENT(bound_enum) das::Type::tPointer;
        __mka_1203(3,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat;
        __mka_1203(4,__context__) = DAS_COMMENT(bound_enum) das::Type::tDouble;
        __mka_1203(5,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration;
        __mka_1203(6,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration8;
        __mka_1203(7,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration16;
        __mka_1203(8,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration64;
        __mka_1203(9,__context__) = DAS_COMMENT(bound_enum) das::Type::tBitfield;
        __mka_1203(10,__context__) = DAS_COMMENT(bound_enum) das::Type::tBitfield8;
        __mka_1203(11,__context__) = DAS_COMMENT(bound_enum) das::Type::tBitfield16;
        __mka_1203(12,__context__) = DAS_COMMENT(bound_enum) das::Type::tBitfield64;
        __mka_1203(13,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt;
        __mka_1203(14,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt8;
        __mka_1203(15,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt16;
        __mka_1203(16,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt64;
        __mka_1203(17,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt;
        __mka_1203(18,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt8;
        __mka_1203(19,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt16;
        __mka_1203(20,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt64;
        __mka_1203(21,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt2;
        __mka_1203(22,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt3;
        __mka_1203(23,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt4;
        __mka_1203(24,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt2;
        __mka_1203(25,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt3;
        __mka_1203(26,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt4;
        __mka_1203(27,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat2;
        __mka_1203(28,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat3;
        __mka_1203(29,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat4;
        __mka_1203(30,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange;
        __mka_1203(31,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange64;
        __mka_1203(32,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange;
        __mka_1203(33,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange64;
        return __mka_1203;
    })()));
}

inline int32_t find_argument_index_3d6a951c2870d00f ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1219_386, char * const  __name_rename_at_1219_387 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1221_32_5; _temp_make_local_1221_32_5;
    {
        bool __need_loop_1221 = true;
        // a: $::das_string const&
        das_iterator<das::vector<das::string> const > __a_iterator(__typ_rename_at_1219_386->argNames /*argNames*/);
        das::string const  * __a_rename_at_1221_390;
        __need_loop_1221 = __a_iterator.first(__context__,(__a_rename_at_1221_390)) && __need_loop_1221;
        // ai: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1221_32_5 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __ai_iterator(0,1);
        int32_t __ai_rename_at_1221_391;
        __need_loop_1221 = __ai_iterator.first(__context__,(__ai_rename_at_1221_391)) && __need_loop_1221;
        for ( ; __need_loop_1221 ; __need_loop_1221 = __a_iterator.next(__context__,(__a_rename_at_1221_390)) && __ai_iterator.next(__context__,(__ai_rename_at_1221_391)) )
        {
            if ( eq_dstr_str((*__a_rename_at_1221_390),__name_rename_at_1219_387) )
            {
                return das_auto_cast<int32_t>::cast(__ai_rename_at_1221_391);
            };
        }
        __ai_iterator.close(__context__,(__ai_rename_at_1221_391));
        __a_iterator.close(__context__,(__a_rename_at_1221_390));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline bool isCMRESType_bbf11aadb0354b4d ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1229_392 )
{
    return das_auto_cast<bool>::cast(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__blockT_rename_at_1229_392),das_auto_cast<void * const >::cast(nullptr))) && ((das_deref(__context__,__blockT_rename_at_1229_392,__FILE__,__LINE__)).isRefType())) && !(das_get_bitfield(__blockT_rename_at_1229_392->flags /*flags*/,1u << 0)));
}

inline void debug_expression_impl_203941aeed67e19a ( Context * __context__, StringBuilderWriter &  __writer_rename_at_1239_393, smart_ptr_raw<Expression> const  __expr_rename_at_1239_394, Bitfield __deFlags_rename_at_1239_395, int32_t __tabs_rename_at_1239_396 ) { das_stack_prologue __prologue(__context__,304,"debug_expression_impl " DAS_FILE_LINE);
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1239_394),das_auto_cast<void * const >::cast(nullptr)) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) "null"))));
        return ;
    } else {
        if ( builtin_empty(__expr_rename_at_1239_394->__rtti /*__rtti*/) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) "[NO RTTI]"))));
            return ;
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_19, cast<char * const >::from(((char *) "(")), cast<char * const >::from(__expr_rename_at_1239_394->__rtti /*__rtti*/))))));
            if ( das_get_bitfield(__deFlags_rename_at_1239_395,1u << 0) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_21, cast<char * const >::from(((char *) "<")), cast<uint32_t>::from(builtin_smart_ptr_use_count(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1239_394),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))), cast<char * const >::from(((char *) ">")))))));
            };
            if ( builtin_string_startswith(((char * const )(pass_string(__expr_rename_at_1239_394->__rtti /*__rtti*/))),((char *) "ExprConst"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) " ")), cast<char * const >::from(_FuncastTickdescribeTick842554968825501494_5630d72624d0f510(__context__,__expr_rename_at_1239_394)), cast<char * const >::from(((char *) ")")))))));
                return ;
            } else {
                BasicStructureAnnotation * __ann_rename_at_1256_397 = das_cast<BasicStructureAnnotation *>::cast(get_expression_annotation(_FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16(__context__,__expr_rename_at_1239_394),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
                if ( __ann_rename_at_1256_397 == nullptr )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) ")"))));
                    return ;
                } else {
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1256_397,__FILE__,__LINE__)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,96,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_1261_398, char * __cppName_rename_at_1261_399, smart_ptr_raw<TypeDecl> __xtype_rename_at_1261_400, uint32_t __offset_rename_at_1261_401) -> void{
                        if ( __offset_rename_at_1261_401 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1263_402 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,__xtype_rename_at_1261_400,true,true,true)));
                            int8_t * __p8_rename_at_1264_403 = 0;
                            das_copy(__p8_rename_at_1264_403,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1239_394),int32_t(__offset_rename_at_1261_401),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1263_402),cast<char *>::from(((char *) "smart_ptr<ast::Expression>")),*__context__,nullptr) )
                            {
                                smart_ptr_raw<Expression> * __pv_rename_at_1269_404 = ((smart_ptr_raw<Expression> *)das_cast<smart_ptr_raw<Expression> *>::cast(__p8_rename_at_1264_403));
                                char * __ts_rename_at_1270_405 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1239_396 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_26, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1270_405), cast<char * const >::from(((char *) "(")), cast<char *>::from(__name_rename_at_1261_398), cast<char * const >::from(((char *) " ")))))));
                                debug_expression_impl_203941aeed67e19a(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1239_393),das_deref(__context__,__pv_rename_at_1269_404,__FILE__,__LINE__),__deFlags_rename_at_1239_395,__tabs_rename_at_1239_396 + 2);
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) ")"))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1263_402),cast<char *>::from(((char *) "$::das_string")),*__context__,nullptr) )
                            {
                                das::string * __pv_rename_at_1275_406 = ((das::string *)das_cast<das::string *>::cast(__p8_rename_at_1264_403));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_29, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1261_398), cast<char * const >::from(((char *) "=\"")), cast<das::string const  &>::from(das_deref(__context__,__pv_rename_at_1275_406,__FILE__,__LINE__)), cast<char * const >::from(((char *) "\"")))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1263_402),cast<char *>::from(((char *) "int")),*__context__,nullptr) )
                            {
                                int32_t * __pv_rename_at_1278_407 = ((int32_t *)das_cast<int32_t *>::cast(__p8_rename_at_1264_403));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_31, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1261_398), cast<char * const >::from(((char *) "=")), cast<int32_t>::from(das_deref(__context__,__pv_rename_at_1278_407,__FILE__,__LINE__)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1263_402),cast<char *>::from(((char *) "uint")),*__context__,nullptr) )
                            {
                                uint32_t * __pv_rename_at_1281_408 = ((uint32_t *)das_cast<uint32_t *>::cast(__p8_rename_at_1264_403));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_33, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1261_398), cast<char * const >::from(((char *) "=")), cast<uint32_t>::from(das_deref(__context__,__pv_rename_at_1281_408,__FILE__,__LINE__)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1263_402),cast<char *>::from(((char *) "int64")),*__context__,nullptr) )
                            {
                                int64_t * __pv_rename_at_1284_409 = ((int64_t *)das_cast<int64_t *>::cast(__p8_rename_at_1264_403));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_35, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1261_398), cast<char * const >::from(((char *) "=")), cast<int64_t>::from(das_deref(__context__,__pv_rename_at_1284_409,__FILE__,__LINE__)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1263_402),cast<char *>::from(((char *) "uint64")),*__context__,nullptr) )
                            {
                                uint64_t * __pv_rename_at_1287_410 = ((uint64_t *)das_cast<uint64_t *>::cast(__p8_rename_at_1264_403));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_37, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1261_398), cast<char * const >::from(((char *) "=")), cast<uint64_t>::from(das_deref(__context__,__pv_rename_at_1287_410,__FILE__,__LINE__)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1263_402),cast<char *>::from(((char *) "bool")),*__context__,nullptr) )
                            {
                                bool * __pv_rename_at_1290_411 = ((bool *)das_cast<bool *>::cast(__p8_rename_at_1264_403));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_39, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1261_398), cast<char * const >::from(((char *) "=")), cast<bool>::from(das_deref(__context__,__pv_rename_at_1290_411,__FILE__,__LINE__)))))));
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1256_397,__FILE__,__LINE__)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,96,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_1300_412, char * __cppName_rename_at_1300_413, smart_ptr_raw<TypeDecl> __xtype_rename_at_1300_414, uint32_t __offset_rename_at_1300_415) -> void{
                        if ( __offset_rename_at_1300_415 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1302_416 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,__xtype_rename_at_1300_414,true,true,true)));
                            int8_t * __p8_rename_at_1303_417 = 0;
                            das_copy(__p8_rename_at_1303_417,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1239_394),int32_t(__offset_rename_at_1300_415),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1302_416),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Expression")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Expression>> * __pv_rename_at_1308_418 = ((das::vector<smart_ptr<Expression>> *)das_cast<das::vector<smart_ptr<Expression>> *>::cast(__p8_rename_at_1303_417));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1308_418,__FILE__,__LINE__)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1312_38_6; _temp_make_local_1312_38_6;
                                    char * __ts_rename_at_1310_419 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1239_396 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_26, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1310_419), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1300_412), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1312 = true;
                                        // l: smart_ptr<ast::Expression> const&
                                        das_iterator<das::vector<smart_ptr<Expression>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1308_418,__FILE__,__LINE__));
                                        smart_ptr_raw<Expression> const  * __l_rename_at_1312_422;
                                        __need_loop_1312 = __l_iterator.first(__context__,(__l_rename_at_1312_422)) && __need_loop_1312;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1312_38_6 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1312_423;
                                        __need_loop_1312 = __i_iterator.first(__context__,(__i_rename_at_1312_423)) && __need_loop_1312;
                                        for ( ; __need_loop_1312 ; __need_loop_1312 = __l_iterator.next(__context__,(__l_rename_at_1312_422)) && __i_iterator.next(__context__,(__i_rename_at_1312_423)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_19, cast<char * const >::from(__ts_rename_at_1310_419), cast<char * const >::from(((char *) "  ")))))));
                                            debug_expression_impl_203941aeed67e19a(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1239_393),(*__l_rename_at_1312_422),__deFlags_rename_at_1239_395,__tabs_rename_at_1239_396 + 2);
                                            if ( __i_rename_at_1312_423 != (das_vector_length(das_deref(__context__,__pv_rename_at_1308_418,__FILE__,__LINE__)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __i_iterator.close(__context__,(__i_rename_at_1312_423));
                                        __l_iterator.close(__context__,(__l_rename_at_1312_422));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) "]"))));
                                };
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1302_416),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Variable")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Variable>> * __pv_rename_at_1322_424 = ((das::vector<smart_ptr<Variable>> *)das_cast<das::vector<smart_ptr<Variable>> *>::cast(__p8_rename_at_1303_417));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1322_424,__FILE__,__LINE__)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1326_38_7; _temp_make_local_1326_38_7;
                                    char * __ts_rename_at_1324_425 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1239_396 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_26, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1324_425), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1300_412), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1326 = true;
                                        // l: smart_ptr<ast::Variable> const&
                                        das_iterator<das::vector<smart_ptr<Variable>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1322_424,__FILE__,__LINE__));
                                        smart_ptr_raw<Variable> const  * __l_rename_at_1326_428;
                                        __need_loop_1326 = __l_iterator.first(__context__,(__l_rename_at_1326_428)) && __need_loop_1326;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1326_38_7 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1326_429;
                                        __need_loop_1326 = __i_iterator.first(__context__,(__i_rename_at_1326_429)) && __need_loop_1326;
                                        for ( ; __need_loop_1326 ; __need_loop_1326 = __l_iterator.next(__context__,(__l_rename_at_1326_428)) && __i_iterator.next(__context__,(__i_rename_at_1326_429)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_19, cast<char * const >::from(__ts_rename_at_1324_425), cast<char * const >::from(((char *) "  ")))))));
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<das::string const  &>::from((*__l_rename_at_1326_428)->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5(__context__,(*__l_rename_at_1326_428)->type /*_type*/,true,true,true)))))));
                                            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__l_rename_at_1326_428)->init /*init*/),das_auto_cast<void * const >::cast(nullptr)) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) " = "))));
                                                debug_expression_impl_203941aeed67e19a(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1239_393),(*__l_rename_at_1326_428)->init /*init*/,__deFlags_rename_at_1239_395,__tabs_rename_at_1239_396 + 2);
                                            };
                                            if ( __i_rename_at_1326_429 != (das_vector_length(das_deref(__context__,__pv_rename_at_1322_424,__FILE__,__LINE__)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __i_iterator.close(__context__,(__i_rename_at_1326_429));
                                        __l_iterator.close(__context__,(__l_rename_at_1326_428));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) "]"))));
                                };
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_1239_393),cast<char * const >::from(((char *) ")"))));
                };
            };
        };
    };
}}

inline char * debug_expression_72d312ce8f3ec686 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1345_430, Bitfield __deFlags_rename_at_1345_431 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1347_432) DAS_AOT_INLINE_LAMBDA -> void{
        debug_expression_impl_203941aeed67e19a(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1347_432),__expr_rename_at_1345_430,__deFlags_rename_at_1345_431,0);
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * debug_expression_84991cbecc0ca3af ( Context * __context__, Expression * const  __expr_rename_at_1352_433 )
{
    return das_auto_cast<char *>::cast(debug_expression_72d312ce8f3ec686(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1352_433),0x0u));
}

inline char * describe_d11036fb3823617b ( Context * __context__, Expression * const  __expr_rename_at_1359_434 )
{
    return das_auto_cast<char *>::cast(_FuncastTickdescribeTick842554968825501494_5630d72624d0f510(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1359_434)));
}

inline int32_t getVectorElementCount_57af1965ac8593bc ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1366_435 )
{
    return das_auto_cast<int32_t>::cast((((((((__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tInt2)) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(2) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tFloat3) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) ? das_auto_cast<int32_t>::cast(3) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tFloat4) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1366_435 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) ? das_auto_cast<int32_t>::cast(4) : das_auto_cast<int32_t>::cast(0))))));
}

inline int32_t getVectorElementSize_d8680eab119bd409 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1374_436 )
{
    return das_auto_cast<int32_t>::cast(((__bt_rename_at_1374_436 == DAS_COMMENT(bound_enum) das::Type::tRange64) || (__bt_rename_at_1374_436 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(8) : das_auto_cast<int32_t>::cast(4));
}

inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_3fc0f8eee440e9d1 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1380_437 )
{
    return das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tFloat) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tRange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tUInt2) || (__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tURange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tRange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1380_437 == DAS_COMMENT(bound_enum) das::Type::tURange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tVoid))))))))));
}

inline int32_t getVectorOffset_ac9cdc53c25ed4ee ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1390_438, char * const  __ident_rename_at_1390_439 )
{
    int32_t __ofs_rename_at_1392_440 = -1;
    if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "x")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "X")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "r")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "R")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1392_440,0);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "y")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "Y")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "g")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "G")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1392_440,1);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "z")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "Z")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "b")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "B")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1392_440,2);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "w")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "W")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "a")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1390_439),cast<char *>::from(((char *) "A")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1392_440,3);
    };
    int32_t __count_rename_at_1402_441 = ((int32_t)getVectorElementCount_57af1965ac8593bc(__context__,__bt_rename_at_1390_438));
    return das_auto_cast<int32_t>::cast((__ofs_rename_at_1392_440 >= __count_rename_at_1402_441) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(__ofs_rename_at_1392_440));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_6dab624d0a9ec2fa ( Context * __context__ ) {
    TArray<ast::AstFunctionAnnotation *> &  arg_Arr = cast_aot_arg<TArray<ast::AstFunctionAnnotation *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstFunctionAnnotation * arg_value = cast_aot_arg<ast::AstFunctionAnnotation *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_6dab624d0a9ec2fa(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_260ca7b6472dc38b ( Context * __context__ ) {
    ast_boost::MacroMacro const  &  arg_cl = cast_aot_arg<ast_boost::MacroMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_260ca7b6472dc38b(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_2a0858496863a6a8 ( Context * __context__ ) {
    ast_boost::TagFunctionAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::TagFunctionAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_2a0858496863a6a8(__context__, arg_cl));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_9901561b81ea1e93 ( Context * __context__ ) {
    TArray<ast::AstStructureAnnotation *> &  arg_Arr = cast_aot_arg<TArray<ast::AstStructureAnnotation *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstStructureAnnotation * arg_value = cast_aot_arg<ast::AstStructureAnnotation *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_9901561b81ea1e93(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_46f2134b013f7e0f ( Context * __context__ ) {
    ast_boost::SetupFunctionAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupFunctionAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_46f2134b013f7e0f(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_308d52657d5fdb9f ( Context * __context__ ) {
    ast_boost::SetupBlockAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupBlockAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_308d52657d5fdb9f(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_8318dfd00c3831fe ( Context * __context__ ) {
    ast_boost::SetupStructureAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupStructureAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_8318dfd00c3831fe(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_2e8d4d0ba0370750 ( Context * __context__ ) {
    ast_boost::SetupEnumerationAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupEnumerationAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_2e8d4d0ba0370750(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_dd15ed5b75151430 ( Context * __context__ ) {
    ast_boost::SetupContractAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::SetupContractAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_dd15ed5b75151430(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_554ebee507608082 ( Context * __context__ ) {
    ast_boost::SetupReaderMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupReaderMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_554ebee507608082(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_3081f3afed8c109b ( Context * __context__ ) {
    ast_boost::SetupCommentReader const  &  arg_cl = cast_aot_arg<ast_boost::SetupCommentReader const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_3081f3afed8c109b(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_15cdaeb56361f117 ( Context * __context__ ) {
    ast_boost::SetupCallMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupCallMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_15cdaeb56361f117(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_95c3a006bc4c8db ( Context * __context__ ) {
    ast_boost::SetupTypeInfoMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupTypeInfoMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_95c3a006bc4c8db(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_1021faef828c12f7 ( Context * __context__ ) {
    ast_boost::SetupVariantMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupVariantMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_1021faef828c12f7(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_e45c871b77e11819 ( Context * __context__ ) {
    ast_boost::SetupForLoopMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupForLoopMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_e45c871b77e11819(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_e815a40ffb05ff49 ( Context * __context__ ) {
    ast_boost::SetupCaptureMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupCaptureMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_e815a40ffb05ff49(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_8624b1f3bb6dc40c ( Context * __context__ ) {
    ast_boost::SetupTypeMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupTypeMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_8624b1f3bb6dc40c(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_f08eab0b96bf74eb ( Context * __context__ ) {
    ast_boost::SetupSimulateMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupSimulateMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_f08eab0b96bf74eb(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_c641e5976eca7cc2 ( Context * __context__ ) {
    ast_boost::TagStructureAnnotation const  &  arg_cl = cast_aot_arg<ast_boost::TagStructureAnnotation const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_c641e5976eca7cc2(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_69b37fb41c547990 ( Context * __context__ ) {
    ast_boost::TagFunctionMacro const  &  arg_cl = cast_aot_arg<ast_boost::TagFunctionMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_69b37fb41c547990(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_940fe04b496ec307 ( Context * __context__ ) {
    ast_boost::SetupInferMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupInferMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_940fe04b496ec307(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_f4258cbe2a3ca795 ( Context * __context__ ) {
    ast_boost::SetupDirtyInferMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupDirtyInferMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_f4258cbe2a3ca795(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_5c844592baa87df1 ( Context * __context__ ) {
    ast_boost::SetupOptimizationMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupOptimizationMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_5c844592baa87df1(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_ff4730001cc28ae9 ( Context * __context__ ) {
    ast_boost::SetupLintMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupLintMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_ff4730001cc28ae9(__context__, arg_cl));
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_f9dc0993d4c46e63 ( Context * __context__ ) {
    ast_boost::SetupGlobalLintMacro const  &  arg_cl = cast_aot_arg<ast_boost::SetupGlobalLintMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_f9dc0993d4c46e63(__context__, arg_cl));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e ( Context * __context__ ) {
    TArray<ast::AstVariantMacro *> &  arg_Arr = cast_aot_arg<TArray<ast::AstVariantMacro *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstVariantMacro * arg_value = cast_aot_arg<ast::AstVariantMacro *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_af3f45d965a765c ( Context * __context__ ) {
    ast_boost::BetterRttiVisitor const  &  arg_cl = cast_aot_arg<ast_boost::BetterRttiVisitor const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_af3f45d965a765c(__context__, arg_cl));
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
static vec4f __wrap__FuncastTickfind_moduleTick11101329256228773934_b65022313a84d5bd ( Context * __context__ ) {
    smart_ptr_raw<Program> const  arg_prog = cast_aot_arg<smart_ptr_raw<Program> const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<Module *>::from(_FuncastTickfind_moduleTick11101329256228773934_b65022313a84d5bd(__context__, arg_prog, arg_name));
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
    *((TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *) __context__->abiCMRES) = _FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8(__context__);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickget_ptrTick5807679485210906136_c8d3f222daa2e3cc ( Context * __context__ ) {
    smart_ptr_raw<ExprBlock> arg_src = cast_aot_arg<smart_ptr_raw<ExprBlock>>::to(*__context__,__context__->abiArguments()[0]);
    return cast<ExprBlock *>::from(_FuncbuiltinTickget_ptrTick5807679485210906136_c8d3f222daa2e3cc(__context__, arg_src));
}
static vec4f __wrap__FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259 ( Context * __context__ ) {
    smart_ptr_raw<Structure> arg_src = cast_aot_arg<smart_ptr_raw<Structure>>::to(*__context__,__context__->abiArguments()[0]);
    return cast<Structure *>::from(_FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259(__context__, arg_src));
}
static vec4f __wrap__Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_56f1ba3bb9b6d08a ( Context * __context__ ) {
    smart_ptr_raw<ExprAsVariant> const  arg_expr = cast_aot_arg<smart_ptr_raw<ExprAsVariant> const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Expression> arg_enull = cast_aot_arg<smart_ptr_raw<Expression>>::to(*__context__,__context__->abiArguments()[1]);
    return cast<smart_ptr_raw<ExprOp3>>::from(_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_56f1ba3bb9b6d08a(__context__, arg_expr, arg_enull));
}
static vec4f __wrap__Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_69da28483a9e1aa1 ( Context * __context__ ) {
    smart_ptr_raw<ExprSafeAsVariant> const  arg_expr = cast_aot_arg<smart_ptr_raw<ExprSafeAsVariant> const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Expression> arg_enull = cast_aot_arg<smart_ptr_raw<Expression>>::to(*__context__,__context__->abiArguments()[1]);
    return cast<smart_ptr_raw<ExprOp3>>::from(_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_69da28483a9e1aa1(__context__, arg_expr, arg_enull));
}
static vec4f __wrap__FuncbuiltinTickget_ptrTick8468476673553620226_b515b7b0b5b85a70 ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_src = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<Annotation *>::from(_FuncbuiltinTickget_ptrTick8468476673553620226_b515b7b0b5b85a70(__context__, arg_src));
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
static vec4f __wrap__FuncastTickfind_compiling_moduleTick4523452840392654583_479fe631f4b1cb3 ( Context * __context__ ) {
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<Module *>::from(_FuncastTickfind_compiling_moduleTick4523452840392654583_479fe631f4b1cb3(__context__, arg_name));
}
static vec4f __wrap_clone_98d3170c0efdddcc ( Context * __context__ ) {
    smart_ptr_raw<Annotation> & arg_dest = cast_aot_arg<smart_ptr_raw<Annotation> &>::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Annotation> const  arg_src = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[1]);
    clone_98d3170c0efdddcc(__context__, arg_dest, arg_src);
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
static vec4f __wrap_isVectorType_5a1ccbd394d2d488 ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_typ = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(isVectorType_5a1ccbd394d2d488(__context__, arg_typ));
}
static vec4f __wrap_describe_a0a74ab06eec48e6 ( Context * __context__ ) {
    AnnotationArgumentList const  &  arg_list = cast_aot_arg<AnnotationArgumentList const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(describe_a0a74ab06eec48e6(__context__, arg_list));
}
static vec4f __wrap_describe_fcbed85e0402159e ( Context * __context__ ) {
    AnnotationDeclaration const  &  arg_ann = cast_aot_arg<AnnotationDeclaration const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(describe_fcbed85e0402159e(__context__, arg_ann));
}
static vec4f __wrap_describe_6a04757d0b7596f7 ( Context * __context__ ) {
    AnnotationList const  &  arg_list = cast_aot_arg<AnnotationList const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(describe_6a04757d0b7596f7(__context__, arg_list));
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
static vec4f __wrap_is_same_or_inherited_66408d7f398767b ( Context * __context__ ) {
    Structure const  * const  arg_parent = cast_aot_arg<Structure const  * const >::to(*__context__,__context__->abiArguments()[0]);
    Structure const  * const  arg_child = cast_aot_arg<Structure const  * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(is_same_or_inherited_66408d7f398767b(__context__, arg_parent, arg_child));
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
static vec4f __wrap_emplace_new_31c417adc7c7987e ( Context * __context__ ) {
    das::vector<smart_ptr<TypeDecl>> &  arg_vec = cast_aot_arg<das::vector<smart_ptr<TypeDecl>> & >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> arg_ptr = cast_aot_arg<smart_ptr_raw<TypeDecl>>::to(*__context__,__context__->abiArguments()[1]);
    emplace_new_31c417adc7c7987e(__context__, arg_vec, arg_ptr);
    return v_zero();
}
static vec4f __wrap_emplace_new_2f0cb94c081aada2 ( Context * __context__ ) {
    das::vector<smart_ptr<Variable>> &  arg_vec = cast_aot_arg<das::vector<smart_ptr<Variable>> & >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Variable> arg_ptr = cast_aot_arg<smart_ptr_raw<Variable>>::to(*__context__,__context__->abiArguments()[1]);
    emplace_new_2f0cb94c081aada2(__context__, arg_vec, arg_ptr);
    return v_zero();
}
static vec4f __wrap_emplace_new_75514f67779c4fa1 ( Context * __context__ ) {
    MakeStruct &  arg_vec = cast_aot_arg<MakeStruct & >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<MakeFieldDecl> arg_ptr = cast_aot_arg<smart_ptr_raw<MakeFieldDecl>>::to(*__context__,__context__->abiArguments()[1]);
    emplace_new_75514f67779c4fa1(__context__, arg_vec, arg_ptr);
    return v_zero();
}
static vec4f __wrap_override_method_8162bb655dd2cd37 ( Context * __context__ ) {
    smart_ptr_raw<Structure> arg_str = cast_aot_arg<smart_ptr_raw<Structure>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_funcName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<bool>::from(override_method_8162bb655dd2cd37(__context__, arg_str, arg_name, arg_funcName));
}
static vec4f __wrap_for_each_tag_function_bb49dc3a279a3322 ( Context * __context__ ) {
    Module * const  arg_mod = cast_aot_arg<Module * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_tag = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  arg_blk = cast_aot_arg<Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & >::to(*__context__,__context__->abiArguments()[2]);
    for_each_tag_function_bb49dc3a279a3322(__context__, arg_mod, arg_tag, arg_blk);
    return v_zero();
}
static vec4f __wrap_find_arg_416380068da636fe ( Context * __context__ ) {
    char * const  arg_argn = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    AnnotationArgumentList const  &  arg_args = cast_aot_arg<AnnotationArgumentList const  & >::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *) __context__->abiCMRES) = find_arg_416380068da636fe(__context__, arg_argn, arg_args);
    return v_zero();
}
static vec4f __wrap_find_arg_34945b603c757892 ( Context * __context__ ) {
    AnnotationArgumentList const  &  arg_args = cast_aot_arg<AnnotationArgumentList const  & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argn = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *) __context__->abiCMRES) = find_arg_34945b603c757892(__context__, arg_args, arg_argn);
    return v_zero();
}
static vec4f __wrap_apply_tag_annotation_1944b9a89fd11326 ( Context * __context__ ) {
    char * const  arg_tag = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<FunctionAnnotation> arg_ann = cast_aot_arg<smart_ptr_raw<FunctionAnnotation>>::to(*__context__,__context__->abiArguments()[1]);
    apply_tag_annotation_1944b9a89fd11326(__context__, arg_tag, arg_ann);
    return v_zero();
}
static vec4f __wrap_find_unique_function_b15ea43980af8102 ( Context * __context__ ) {
    Module * const  arg_mod = cast_aot_arg<Module * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    bool arg_canfail = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Function>>::from(find_unique_function_b15ea43980af8102(__context__, arg_mod, arg_name, arg_canfail));
}
static vec4f __wrap_find_unique_generic_4054e5bb45ca1054 ( Context * __context__ ) {
    Module * const  arg_mod = cast_aot_arg<Module * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    bool arg_canfail = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Function>>::from(find_unique_generic_4054e5bb45ca1054(__context__, arg_mod, arg_name, arg_canfail));
}
static vec4f __wrap_setup_call_list_729e5db2cab0e291 ( Context * __context__ ) {
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[1]);
    Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  arg_subblock = cast_aot_arg<Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<ExprBlock *>::from(setup_call_list_729e5db2cab0e291(__context__, arg_name, arg_at, arg_subblock));
}
static vec4f __wrap_setup_call_list_7b65a2c40054d8e8 ( Context * __context__ ) {
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[1]);
    bool arg_isInit = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    bool arg_isPrivate = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[3]);
    bool arg_isLateInit = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[4]);
    return cast<ExprBlock *>::from(setup_call_list_7b65a2c40054d8e8(__context__, arg_name, arg_at, arg_isInit, arg_isPrivate, arg_isLateInit));
}
static vec4f __wrap_setup_macro_e906336cb4687a9c ( Context * __context__ ) {
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<ExprBlock *>::from(setup_macro_e906336cb4687a9c(__context__, arg_name, arg_at));
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
static vec4f __wrap__FuncTickisTickFunctionAnnotation_eccaa0a9bcb3796 ( Context * __context__ ) {
    Annotation * const  arg_foo = cast_aot_arg<Annotation * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickFunctionAnnotation_eccaa0a9bcb3796(__context__, arg_foo));
}
static vec4f __wrap__FuncTickisTickFunctionAnnotation_6d5ce87249ff0eaf ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_foo = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickFunctionAnnotation_6d5ce87249ff0eaf(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickFunctionAnnotation_8d76f010548d92f6 ( Context * __context__ ) {
    Annotation * const  arg_foo = cast_aot_arg<Annotation * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<FunctionAnnotation *>::from(_FuncTickasTickFunctionAnnotation_8d76f010548d92f6(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickFunctionAnnotation_6e0707ffcc0c2b76 ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_foo = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<FunctionAnnotation *>::from(_FuncTickasTickFunctionAnnotation_6e0707ffcc0c2b76(__context__, arg_foo));
}
static vec4f __wrap__FuncTickisTickStructureAnnotation_471f8e2f8678905f ( Context * __context__ ) {
    Annotation * const  arg_foo = cast_aot_arg<Annotation * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickStructureAnnotation_471f8e2f8678905f(__context__, arg_foo));
}
static vec4f __wrap__FuncTickisTickStructureAnnotation_1883d933e82a8bcc ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_foo = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(_FuncTickisTickStructureAnnotation_1883d933e82a8bcc(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickStructureAnnotation_1f9c68dfdd32ff55 ( Context * __context__ ) {
    Annotation * const  arg_foo = cast_aot_arg<Annotation * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructureAnnotation *>::from(_FuncTickasTickStructureAnnotation_1f9c68dfdd32ff55(__context__, arg_foo));
}
static vec4f __wrap__FuncTickasTickStructureAnnotation_1a5c23b3eb803935 ( Context * __context__ ) {
    smart_ptr_raw<Annotation> const  arg_foo = cast_aot_arg<smart_ptr_raw<Annotation> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructureAnnotation *>::from(_FuncTickasTickStructureAnnotation_1a5c23b3eb803935(__context__, arg_foo));
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
static vec4f __wrap_walk_and_convert_basic_aef53343044f9782 ( Context * __context__ ) {
    uint8_t const  * const  arg_data = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<TypeDecl> const  arg_info = cast_aot_arg<smart_ptr_raw<TypeDecl> const >::to(*__context__,__context__->abiArguments()[1]);
    LineInfo const  &  arg_at = cast_aot_arg<LineInfo const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<Expression>>::from(walk_and_convert_basic_aef53343044f9782(__context__, arg_data, arg_info, arg_at));
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
static vec4f __wrap_find_annotation_c7c295d23bae9ab4 ( Context * __context__ ) {
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<Annotation const  *>::from(find_annotation_c7c295d23bae9ab4(__context__, arg_mod_name, arg_ann_name));
}
static vec4f __wrap_append_annotation_4862c33d2ab9dcd4 ( Context * __context__ ) {
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  arg_args = cast_aot_arg<TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<smart_ptr_raw<AnnotationDeclaration>>::from(append_annotation_4862c33d2ab9dcd4(__context__, arg_mod_name, arg_ann_name, arg_args));
}
static vec4f __wrap_append_annotation_8190e2b8fbb09c12 ( Context * __context__ ) {
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<smart_ptr_raw<AnnotationDeclaration>>::from(append_annotation_8190e2b8fbb09c12(__context__, arg_mod_name, arg_ann_name));
}
static vec4f __wrap_append_annotation_9e9720e2bdf176ae ( Context * __context__ ) {
    smart_ptr_raw<Function> arg_func = cast_aot_arg<smart_ptr_raw<Function>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    append_annotation_9e9720e2bdf176ae(__context__, arg_func, arg_mod_name, arg_ann_name);
    return v_zero();
}
static vec4f __wrap_append_annotation_82649f781a6d03e7 ( Context * __context__ ) {
    smart_ptr_raw<ExprBlock> arg_blk = cast_aot_arg<smart_ptr_raw<ExprBlock>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    append_annotation_82649f781a6d03e7(__context__, arg_blk, arg_mod_name, arg_ann_name);
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
    TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  arg_args = cast_aot_arg<TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & >::to(*__context__,__context__->abiArguments()[3]);
    append_annotation_d2aadccfb018501d(__context__, arg_func, arg_mod_name, arg_ann_name, arg_args);
    return v_zero();
}
static vec4f __wrap_append_annotation_41a68523ba2c1d9e ( Context * __context__ ) {
    smart_ptr_raw<ExprBlock> arg_blk = cast_aot_arg<smart_ptr_raw<ExprBlock>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  arg_args = cast_aot_arg<TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & >::to(*__context__,__context__->abiArguments()[3]);
    append_annotation_41a68523ba2c1d9e(__context__, arg_blk, arg_mod_name, arg_ann_name, arg_args);
    return v_zero();
}
static vec4f __wrap_append_annotation_84797c2112b4fd76 ( Context * __context__ ) {
    smart_ptr_raw<Structure> arg_st = cast_aot_arg<smart_ptr_raw<Structure>>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_mod_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_ann_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  arg_args = cast_aot_arg<TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & >::to(*__context__,__context__->abiArguments()[3]);
    append_annotation_84797c2112b4fd76(__context__, arg_st, arg_mod_name, arg_ann_name, arg_args);
    return v_zero();
}
static vec4f __wrap_add_annotation_argument_a6c4ec43057e73c ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    bool arg_val = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(add_annotation_argument_a6c4ec43057e73c(__context__, arg_arguments, arg_argName, arg_val));
}
static vec4f __wrap_add_annotation_argument_2ee13ee84bf33b6d ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_val = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(add_annotation_argument_2ee13ee84bf33b6d(__context__, arg_arguments, arg_argName, arg_val));
}
static vec4f __wrap_add_annotation_argument_e4f5a170be5edc8b ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    float arg_val = cast_aot_arg<float>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(add_annotation_argument_e4f5a170be5edc8b(__context__, arg_arguments, arg_argName, arg_val));
}
static vec4f __wrap_add_annotation_argument_d6722d8e520facbc ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_argName = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_val = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(add_annotation_argument_d6722d8e520facbc(__context__, arg_arguments, arg_argName, arg_val));
}
static vec4f __wrap_add_annotation_argument_eb9c96b34ef66276 ( Context * __context__ ) {
    AnnotationArgumentList &  arg_arguments = cast_aot_arg<AnnotationArgumentList & >::to(*__context__,__context__->abiArguments()[0]);
    AnnotationArgument const  &  arg_ann = cast_aot_arg<AnnotationArgument const  & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(add_annotation_argument_eb9c96b34ef66276(__context__, arg_arguments, arg_ann));
}
static vec4f __wrap_get_for_source_index_ce1d36791493c0c4 ( Context * __context__ ) {
    smart_ptr_raw<ExprFor> const  arg_expr = cast_aot_arg<smart_ptr_raw<ExprFor> const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Variable> const  arg_svar = cast_aot_arg<smart_ptr_raw<Variable> const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(get_for_source_index_ce1d36791493c0c4(__context__, arg_expr, arg_svar));
}
static vec4f __wrap_get_for_source_index_44e105584529041d ( Context * __context__ ) {
    smart_ptr_raw<ExprFor> const  arg_expr = cast_aot_arg<smart_ptr_raw<ExprFor> const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<Expression> const  arg_source = cast_aot_arg<smart_ptr_raw<Expression> const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(get_for_source_index_44e105584529041d(__context__, arg_expr, arg_source));
}
static vec4f __wrap_function_to_type_1f450dac98d6071d ( Context * __context__ ) {
    smart_ptr_raw<Function> const  arg_fn = cast_aot_arg<smart_ptr_raw<Function> const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<smart_ptr_raw<TypeDecl>>::from(function_to_type_1f450dac98d6071d(__context__, arg_fn));
}
static vec4f __wrap_visit_finally_5653493f01f579fa ( Context * __context__ ) {
    ExprBlock * const  arg_blk = cast_aot_arg<ExprBlock * const >::to(*__context__,__context__->abiArguments()[0]);
    smart_ptr_raw<VisitorAdapter> const  arg_adapter = cast_aot_arg<smart_ptr_raw<VisitorAdapter> const >::to(*__context__,__context__->abiArguments()[1]);
    visit_finally_5653493f01f579fa(__context__, arg_blk, arg_adapter);
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
static vec4f __wrap_get_workhorse_types_de3ed4d894cfde0c ( Context * __context__ ) {
    *((TDim<DAS_COMMENT(bound_enum) das::Type,34> *) __context__->abiCMRES) = get_workhorse_types_de3ed4d894cfde0c(__context__);
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
static vec4f __wrap_getVectorElementCount_57af1965ac8593bc ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_bt = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(getVectorElementCount_57af1965ac8593bc(__context__, arg_bt));
}
static vec4f __wrap_getVectorElementSize_d8680eab119bd409 ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_bt = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(getVectorElementSize_d8680eab119bd409(__context__, arg_bt));
}
static vec4f __wrap_getVectorElementType_3fc0f8eee440e9d1 ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_bt = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    return cast<DAS_COMMENT(bound_enum) das::Type>::from(getVectorElementType_3fc0f8eee440e9d1(__context__, arg_bt));
}
static vec4f __wrap_getVectorOffset_ac9cdc53c25ed4ee ( Context * __context__ ) {
    DAS_COMMENT(bound_enum) das::Type arg_bt = cast_aot_arg<DAS_COMMENT(bound_enum) das::Type>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_ident = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(getVectorOffset_ac9cdc53c25ed4ee(__context__, arg_bt, arg_ident));
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0x2432b6c15deebc39, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_6dab624d0a9ec2fa, &__wrap__FuncbuiltinTickpushTick10769833213962245646_6dab624d0a9ec2fa },
    { 0x558e1153dfe402f, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_260ca7b6472dc38b, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_260ca7b6472dc38b },
    { 0x37558da8cb35855, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_2a0858496863a6a8, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_2a0858496863a6a8 },
    { 0xc53320672be4e18f, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_9901561b81ea1e93, &__wrap__FuncbuiltinTickpushTick10769833213962245646_9901561b81ea1e93 },
    { 0xec97fda44fc31d33, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_46f2134b013f7e0f, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_46f2134b013f7e0f },
    { 0xced3c306b089e4eb, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_308d52657d5fdb9f, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_308d52657d5fdb9f },
    { 0xe1cb5e3d622b0226, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_8318dfd00c3831fe, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_8318dfd00c3831fe },
    { 0x96bf4da705286c2c, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_2e8d4d0ba0370750, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_2e8d4d0ba0370750 },
    { 0xbe0738cb7a7cb22a, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_dd15ed5b75151430, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_dd15ed5b75151430 },
    { 0xa6ae857396c8ef, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_554ebee507608082, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_554ebee507608082 },
    { 0x1687f8a9e431676c, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_3081f3afed8c109b, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_3081f3afed8c109b },
    { 0xb7662eea63317b1d, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_15cdaeb56361f117, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_15cdaeb56361f117 },
    { 0xddcac865d047d6f6, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_95c3a006bc4c8db, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_95c3a006bc4c8db },
    { 0x857cc46f3d02d311, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_1021faef828c12f7, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_1021faef828c12f7 },
    { 0x972637212c08e511, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_e45c871b77e11819, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_e45c871b77e11819 },
    { 0x2560e8db72993a32, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_e815a40ffb05ff49, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_e815a40ffb05ff49 },
    { 0x4b02be0e6c90c837, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_8624b1f3bb6dc40c, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_8624b1f3bb6dc40c },
    { 0xb5ba3420738a320d, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_f08eab0b96bf74eb, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_f08eab0b96bf74eb },
    { 0x4b8290e9ddf62a67, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_c641e5976eca7cc2, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_c641e5976eca7cc2 },
    { 0x97092019c3792a88, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_69b37fb41c547990, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_69b37fb41c547990 },
    { 0xb97ea176e7d95510, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_940fe04b496ec307, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_940fe04b496ec307 },
    { 0xeb4c143a93ec9169, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_f4258cbe2a3ca795, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_f4258cbe2a3ca795 },
    { 0xf214343eef58ee50, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_5c844592baa87df1, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_5c844592baa87df1 },
    { 0xf42a16aec1147fd6, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_ff4730001cc28ae9, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_ff4730001cc28ae9 },
    { 0xbe6198726dad2e24, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_f9dc0993d4c46e63, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_f9dc0993d4c46e63 },
    { 0x4bc10cea9c6c5a94, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e, &__wrap__FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e },
    { 0xc224717590b5e9c6, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_af3f45d965a765c, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_af3f45d965a765c },
    { 0xdbd3c0b15db539b8, false, (void*)&clone_7f4a34d879fc35f4, &__wrap_clone_7f4a34d879fc35f4 },
    { 0xafbc8ff04895ffe, false, (void*)&_FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0, &__wrap__FuncbuiltinTickclone_to_moveTick17180087396728260876_eb0b7db8566e75c0 },
    { 0x820d2672259b2af2, false, (void*)&_FuncastTickfind_moduleTick11101329256228773934_b65022313a84d5bd, &__wrap__FuncastTickfind_moduleTick11101329256228773934_b65022313a84d5bd },
    { 0x468a9f289eaed4d0, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f, &__wrap__FuncbuiltinTickpushTick10769833213962245646_a54a33ebec8df23f },
    { 0x61c57fcc3ac50491, false, (void*)&_Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9, &__wrap__Funcstrings_boostTickjoinTick16475640899284277631_2d62f8289a864db9 },
    { 0x4a9a547cf73d3b9c, false, (void*)&_FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49, &__wrap__FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 },
    { 0x95370060fd3b30b4, false, (void*)&_FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec, &__wrap__FuncbuiltinTickpush_cloneTick2035469273396957942_a41815ce051f5fec },
    { 0x81ff2acd679f7b49, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_4a458787bd7c95c2 },
    { 0x820f84ac63af384a, false, (void*)&clone_e5dbb6c9a183080e, &__wrap_clone_e5dbb6c9a183080e },
    { 0x239a2d8977dd3bb6, false, (void*)&_FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5, &__wrap__FuncastTickdescribeTick2562845734617055679_4284af1259fd4ca5 },
    { 0xf1db16111fe463d3, false, (void*)&_FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc, &__wrap__FuncbuiltinTickget_ptrTick8468476673553620226_184a9b5c7d12e8bc },
    { 0x251a5d8d979cd19c, true, (void*)&_FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8, &__wrap__FuncrttiTickRttiValue_nothingTick4715542659269841615_917a547d9456fca8 },
    { 0x5946d633f9c7be31, false, (void*)&_FuncbuiltinTickget_ptrTick5807679485210906136_c8d3f222daa2e3cc, &__wrap__FuncbuiltinTickget_ptrTick5807679485210906136_c8d3f222daa2e3cc },
    { 0x78044e70d5c1011e, false, (void*)&_FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259, &__wrap__FuncbuiltinTickget_ptrTick5807679485210906136_1929edf53d43b259 },
    { 0x96440be49875a847, false, (void*)&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_56f1ba3bb9b6d08a, &__wrap__Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_56f1ba3bb9b6d08a },
    { 0xebe759d60497c9bf, false, (void*)&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_69da28483a9e1aa1, &__wrap__Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_69da28483a9e1aa1 },
    { 0x5e5cf684885e7f1c, false, (void*)&_FuncbuiltinTickget_ptrTick8468476673553620226_b515b7b0b5b85a70, &__wrap__FuncbuiltinTickget_ptrTick8468476673553620226_b515b7b0b5b85a70 },
    { 0x194206eb303c1a9e, false, (void*)&clone_23a052359607d993, &__wrap_clone_23a052359607d993 },
    { 0xe73c490261748131, false, (void*)&clone_a8386ed128f4f0b4, &__wrap_clone_a8386ed128f4f0b4 },
    { 0x993c9ab4c0f8d81e, false, (void*)&_FuncastTickfind_compiling_moduleTick4523452840392654583_479fe631f4b1cb3, &__wrap__FuncastTickfind_compiling_moduleTick4523452840392654583_479fe631f4b1cb3 },
    { 0xc128cc0e85044d7c, false, (void*)&clone_98d3170c0efdddcc, &__wrap_clone_98d3170c0efdddcc },
    { 0x9a6855251932c74e, false, (void*)&_FuncastTickdescribeTick842554968825501494_5630d72624d0f510, &__wrap__FuncastTickdescribeTick842554968825501494_5630d72624d0f510 },
    { 0x53906f3b10d1dcc7, false, (void*)&_FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16, &__wrap__FuncbuiltinTickget_ptrTick8468476673553620226_4654cded882c4a16 },
    { 0x2463ef01a1540cf0, false, (void*)&isVectorType_5a1ccbd394d2d488, &__wrap_isVectorType_5a1ccbd394d2d488 },
    { 0x702c8d22572f9c2a, false, (void*)&describe_a0a74ab06eec48e6, &__wrap_describe_a0a74ab06eec48e6 },
    { 0x3e4c4140fb3843b8, false, (void*)&describe_fcbed85e0402159e, &__wrap_describe_fcbed85e0402159e },
    { 0x8f5153ada7c59c5c, false, (void*)&describe_6a04757d0b7596f7, &__wrap_describe_6a04757d0b7596f7 },
    { 0xf1dac4b5f5ff4237, false, (void*)&describe_24ebe982465d57fc, &__wrap_describe_24ebe982465d57fc },
    { 0xddfd46ce09e77814, false, (void*)&isExpression_f1fadf50b6070b86, &__wrap_isExpression_f1fadf50b6070b86 },
    { 0xad2349c08c656829, false, (void*)&is_same_or_inherited_66408d7f398767b, &__wrap_is_same_or_inherited_66408d7f398767b },
    { 0xcaa5238e7ff944db, false, (void*)&is_class_method_4783315be6e05a7b, &__wrap_is_class_method_4783315be6e05a7b },
    { 0xcffc555531a07b4a, false, (void*)&emplace_new_9bef84999dd1e3f4, &__wrap_emplace_new_9bef84999dd1e3f4 },
    { 0xc148da7001d50d7f, false, (void*)&emplace_new_31c417adc7c7987e, &__wrap_emplace_new_31c417adc7c7987e },
    { 0x11f59ebb24eecdd0, false, (void*)&emplace_new_2f0cb94c081aada2, &__wrap_emplace_new_2f0cb94c081aada2 },
    { 0xdb16b8bbdf7bc12, false, (void*)&emplace_new_75514f67779c4fa1, &__wrap_emplace_new_75514f67779c4fa1 },
    { 0xe55947caf7c304f, false, (void*)&override_method_8162bb655dd2cd37, &__wrap_override_method_8162bb655dd2cd37 },
    { 0x599d7dd74137dda7, false, (void*)&for_each_tag_function_bb49dc3a279a3322, &__wrap_for_each_tag_function_bb49dc3a279a3322 },
    { 0xb743f694178665ee, true, (void*)&find_arg_416380068da636fe, &__wrap_find_arg_416380068da636fe },
    { 0xc098843cbaad00f4, true, (void*)&find_arg_34945b603c757892, &__wrap_find_arg_34945b603c757892 },
    { 0xa4cdf4d2a4a6cb55, false, (void*)&apply_tag_annotation_1944b9a89fd11326, &__wrap_apply_tag_annotation_1944b9a89fd11326 },
    { 0x10276113422517c2, false, (void*)&find_unique_function_b15ea43980af8102, &__wrap_find_unique_function_b15ea43980af8102 },
    { 0x38e229760d49aa65, false, (void*)&find_unique_generic_4054e5bb45ca1054, &__wrap_find_unique_generic_4054e5bb45ca1054 },
    { 0xcfc3fed432510a6c, false, (void*)&setup_call_list_729e5db2cab0e291, &__wrap_setup_call_list_729e5db2cab0e291 },
    { 0x23d43138ba4312ee, false, (void*)&setup_call_list_7b65a2c40054d8e8, &__wrap_setup_call_list_7b65a2c40054d8e8 },
    { 0xe6addf68967c033b, false, (void*)&setup_macro_e906336cb4687a9c, &__wrap_setup_macro_e906336cb4687a9c },
    { 0x5e3abef4280895c3, false, (void*)&panic_expr_as_2ea20f674c842c9c, &__wrap_panic_expr_as_2ea20f674c842c9c },
    { 0x88e42da7cebd28ac, false, (void*)&_FuncTickisTickBuiltInFunction_3d26f4806589821b, &__wrap__FuncTickisTickBuiltInFunction_3d26f4806589821b },
    { 0xf4c92df79648f1cb, false, (void*)&_FuncTickasTickBuiltInFunction_c47ad3b03c9bc354, &__wrap__FuncTickasTickBuiltInFunction_c47ad3b03c9bc354 },
    { 0x1bf1a475e699701a, false, (void*)&_FuncTickisTickExternalFnBase_24b69557e6b14161, &__wrap__FuncTickisTickExternalFnBase_24b69557e6b14161 },
    { 0x55a228b61d8f1c13, false, (void*)&_FuncTickasTickExternalFnBase_b840ea32468fb731, &__wrap__FuncTickasTickExternalFnBase_b840ea32468fb731 },
    { 0xb96afd03eede8dfb, false, (void*)&_FuncTickisTickFunctionAnnotation_eccaa0a9bcb3796, &__wrap__FuncTickisTickFunctionAnnotation_eccaa0a9bcb3796 },
    { 0x35968e6e56a2a4ba, false, (void*)&_FuncTickisTickFunctionAnnotation_6d5ce87249ff0eaf, &__wrap__FuncTickisTickFunctionAnnotation_6d5ce87249ff0eaf },
    { 0x89233fa2ca3cd5f, false, (void*)&_FuncTickasTickFunctionAnnotation_8d76f010548d92f6, &__wrap__FuncTickasTickFunctionAnnotation_8d76f010548d92f6 },
    { 0xe3b1396c691390af, false, (void*)&_FuncTickasTickFunctionAnnotation_6e0707ffcc0c2b76, &__wrap__FuncTickasTickFunctionAnnotation_6e0707ffcc0c2b76 },
    { 0x2697152c3ec05194, false, (void*)&_FuncTickisTickStructureAnnotation_471f8e2f8678905f, &__wrap__FuncTickisTickStructureAnnotation_471f8e2f8678905f },
    { 0xbd1b82d6ba56a9e6, false, (void*)&_FuncTickisTickStructureAnnotation_1883d933e82a8bcc, &__wrap__FuncTickisTickStructureAnnotation_1883d933e82a8bcc },
    { 0xa723f14b3811d4d5, false, (void*)&_FuncTickasTickStructureAnnotation_1f9c68dfdd32ff55, &__wrap__FuncTickasTickStructureAnnotation_1f9c68dfdd32ff55 },
    { 0x777004b70ed3f9f3, false, (void*)&_FuncTickasTickStructureAnnotation_1a5c23b3eb803935, &__wrap__FuncTickasTickStructureAnnotation_1a5c23b3eb803935 },
    { 0x4d239bf4843863f9, false, (void*)&walk_and_convert_array_ba1cc70e35611268, &__wrap_walk_and_convert_array_ba1cc70e35611268 },
    { 0x8d73bca5ce04d0ce, false, (void*)&walk_and_convert_dim_9001b9148a98cff6, &__wrap_walk_and_convert_dim_9001b9148a98cff6 },
    { 0x93ad6d3f58f111c7, false, (void*)&walk_and_convert_pointer_47b794d0dd59337d, &__wrap_walk_and_convert_pointer_47b794d0dd59337d },
    { 0x64a165722d974ea0, false, (void*)&walk_and_convert_tuple_9ce3b13721ba5daf, &__wrap_walk_and_convert_tuple_9ce3b13721ba5daf },
    { 0xc768ad11e392f9b5, false, (void*)&walk_and_convert_structure_106d0a1bf460f63c, &__wrap_walk_and_convert_structure_106d0a1bf460f63c },
    { 0x9bdd9ac39a594d17, false, (void*)&walk_and_convert_variant_cdeba207e46113d9, &__wrap_walk_and_convert_variant_cdeba207e46113d9 },
    { 0xcfbfe1a8f0851409, false, (void*)&walk_and_convert_table_bafed583a64bc3a1, &__wrap_walk_and_convert_table_bafed583a64bc3a1 },
    { 0x422d5ed2dc60958, false, (void*)&walk_and_convert_basic_aef53343044f9782, &__wrap_walk_and_convert_basic_aef53343044f9782 },
    { 0x4c0a3a42f7d66fd7, false, (void*)&walk_and_convert_enumeration_5bb2d478c4ffeaaa, &__wrap_walk_and_convert_enumeration_5bb2d478c4ffeaaa },
    { 0xc32e39a4c12758b2, false, (void*)&walk_and_convert_cfdd8313a6a5987, &__wrap_walk_and_convert_cfdd8313a6a5987 },
    { 0x59f96878622318dd, false, (void*)&find_annotation_c7c295d23bae9ab4, &__wrap_find_annotation_c7c295d23bae9ab4 },
    { 0x70f7b9d9049b5963, false, (void*)&append_annotation_4862c33d2ab9dcd4, &__wrap_append_annotation_4862c33d2ab9dcd4 },
    { 0xc5c5c1bede6c1279, false, (void*)&append_annotation_8190e2b8fbb09c12, &__wrap_append_annotation_8190e2b8fbb09c12 },
    { 0xb0146b58dcf6718, false, (void*)&append_annotation_9e9720e2bdf176ae, &__wrap_append_annotation_9e9720e2bdf176ae },
    { 0xe035519b3c003250, false, (void*)&append_annotation_82649f781a6d03e7, &__wrap_append_annotation_82649f781a6d03e7 },
    { 0xd5eaa703f277f196, false, (void*)&append_annotation_1d124a466ae78997, &__wrap_append_annotation_1d124a466ae78997 },
    { 0x551583ffde7da9fa, false, (void*)&append_annotation_d2aadccfb018501d, &__wrap_append_annotation_d2aadccfb018501d },
    { 0x3c96bc0c90b51764, false, (void*)&append_annotation_41a68523ba2c1d9e, &__wrap_append_annotation_41a68523ba2c1d9e },
    { 0x14217de8a71a7f0b, false, (void*)&append_annotation_84797c2112b4fd76, &__wrap_append_annotation_84797c2112b4fd76 },
    { 0xc56728211ed1d43e, false, (void*)&add_annotation_argument_a6c4ec43057e73c, &__wrap_add_annotation_argument_a6c4ec43057e73c },
    { 0x2bcc700b909e7209, false, (void*)&add_annotation_argument_2ee13ee84bf33b6d, &__wrap_add_annotation_argument_2ee13ee84bf33b6d },
    { 0x244dbaa7434b7cab, false, (void*)&add_annotation_argument_e4f5a170be5edc8b, &__wrap_add_annotation_argument_e4f5a170be5edc8b },
    { 0xd09b8a9b7815e8a8, false, (void*)&add_annotation_argument_d6722d8e520facbc, &__wrap_add_annotation_argument_d6722d8e520facbc },
    { 0x3b67223ead226351, false, (void*)&add_annotation_argument_eb9c96b34ef66276, &__wrap_add_annotation_argument_eb9c96b34ef66276 },
    { 0xfe069916369755a7, false, (void*)&get_for_source_index_ce1d36791493c0c4, &__wrap_get_for_source_index_ce1d36791493c0c4 },
    { 0xc1e1a7a6b9e1759a, false, (void*)&get_for_source_index_44e105584529041d, &__wrap_get_for_source_index_44e105584529041d },
    { 0x957af87d653818a3, false, (void*)&function_to_type_1f450dac98d6071d, &__wrap_function_to_type_1f450dac98d6071d },
    { 0x162da8897420fde8, false, (void*)&visit_finally_5653493f01f579fa, &__wrap_visit_finally_5653493f01f579fa },
    { 0xe41cfaa3c698a1de, false, (void*)&isCMRES_2bb438cbd738593f, &__wrap_isCMRES_2bb438cbd738593f },
    { 0x14f45fd804ec1a83, false, (void*)&isCMRES_45d360720945cc74, &__wrap_isCMRES_45d360720945cc74 },
    { 0x74a4840f0a60e533, false, (void*)&isMakeLocal_c461b44592b8facf, &__wrap_isMakeLocal_c461b44592b8facf },
    { 0x7938c433905fde5, false, (void*)&isExprCallFunc_aac1b10a17185ef1, &__wrap_isExprCallFunc_aac1b10a17185ef1 },
    { 0xa5b58081958049c4, true, (void*)&get_workhorse_types_de3ed4d894cfde0c, &__wrap_get_workhorse_types_de3ed4d894cfde0c },
    { 0xbb1d0dfecde4a96, false, (void*)&find_argument_index_3d6a951c2870d00f, &__wrap_find_argument_index_3d6a951c2870d00f },
    { 0x463b5b3ac935b144, false, (void*)&isCMRESType_bbf11aadb0354b4d, &__wrap_isCMRESType_bbf11aadb0354b4d },
    { 0xdd8a5bd7dd37fff3, false, (void*)&debug_expression_impl_203941aeed67e19a, &__wrap_debug_expression_impl_203941aeed67e19a },
    { 0x79fa533959c1342b, false, (void*)&debug_expression_72d312ce8f3ec686, &__wrap_debug_expression_72d312ce8f3ec686 },
    { 0xd11ee0d6b63cf645, false, (void*)&debug_expression_84991cbecc0ca3af, &__wrap_debug_expression_84991cbecc0ca3af },
    { 0xafc03ec02ad39ebd, false, (void*)&describe_d11036fb3823617b, &__wrap_describe_d11036fb3823617b },
    { 0xd8fec034a29109d8, false, (void*)&getVectorElementCount_57af1965ac8593bc, &__wrap_getVectorElementCount_57af1965ac8593bc },
    { 0x65a5cd10a142fc64, false, (void*)&getVectorElementSize_d8680eab119bd409, &__wrap_getVectorElementSize_d8680eab119bd409 },
    { 0xb08e10e11a6b3e3, false, (void*)&getVectorElementType_3fc0f8eee440e9d1, &__wrap_getVectorElementType_3fc0f8eee440e9d1 },
    { 0xb053391479e3a256, false, (void*)&getVectorOffset_ac9cdc53c25ed4ee, &__wrap_getVectorOffset_ac9cdc53c25ed4ee },
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
