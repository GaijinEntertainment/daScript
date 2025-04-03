#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require contracts
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
namespace _anon_2463735248708067149 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

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
namespace contracts {

struct IsAnyType {
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
static_assert(sizeof(IsAnyType)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyType,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyType,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyArrayMacro {
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
static_assert(sizeof(IsAnyArrayMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyArrayMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyEnumMacro {
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
static_assert(sizeof(IsAnyEnumMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyEnumMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyBitfieldMacro {
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
static_assert(sizeof(IsAnyBitfieldMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyBitfieldMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyVectorType {
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
static_assert(sizeof(IsAnyVectorType)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVectorType,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyStructMacro {
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
static_assert(sizeof(IsAnyStructMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyStructMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyNumericMacro {
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
static_assert(sizeof(IsAnyNumericMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyNumericMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyWorkhorse {
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
static_assert(sizeof(IsAnyWorkhorse)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorse,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyWorkhorseNonPtrMacro {
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
static_assert(sizeof(IsAnyWorkhorseNonPtrMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyWorkhorseNonPtrMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyTupleNonPtrMacro {
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
static_assert(sizeof(IsAnyTupleNonPtrMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyTupleNonPtrMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyVariantNonPtrMacro {
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
static_assert(sizeof(IsAnyVariantNonPtrMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyVariantNonPtrMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyFunctionNonPtrMacro {
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
static_assert(sizeof(IsAnyFunctionNonPtrMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyFunctionNonPtrMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsAnyLambdaMacro {
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
static_assert(sizeof(IsAnyLambdaMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsAnyLambdaMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsRefMacro {
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
static_assert(sizeof(IsRefMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsRefMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsRefMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsPointer {
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
static_assert(sizeof(IsPointer)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsPointer,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsPointer,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsClass {
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
static_assert(sizeof(IsClass)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsClass,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsClass,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace contracts {

struct IsValueHandle {
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
static_assert(sizeof(IsValueHandle)==112,"structure size mismatch with DAS");
static_assert(offsetof(IsValueHandle,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(IsValueHandle,appendToMangledName)==104,"structure field offset mismatch with DAS");
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e2d6467c527f3892 ( Context * __context__, contracts::IsAnyArrayMacro const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8ff4a2699ee4a0c5 ( Context * __context__, contracts::IsAnyEnumMacro const  & __cl_rename_at_116_3 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_454ead443fb4c032 ( Context * __context__, contracts::IsAnyBitfieldMacro const  & __cl_rename_at_116_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b685d2e65f42da2 ( Context * __context__, contracts::IsAnyVectorType const  & __cl_rename_at_116_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_eabdcec7c93f4b15 ( Context * __context__, contracts::IsAnyStructMacro const  & __cl_rename_at_116_6 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d76c4e9b31825364 ( Context * __context__, contracts::IsAnyNumericMacro const  & __cl_rename_at_116_7 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a97aaf905b3522b5 ( Context * __context__, contracts::IsAnyWorkhorse const  & __cl_rename_at_116_8 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_115e7ae3efe17f22 ( Context * __context__, contracts::IsAnyWorkhorseNonPtrMacro const  & __cl_rename_at_116_9 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8bf96d25c51c8832 ( Context * __context__, contracts::IsAnyTupleNonPtrMacro const  & __cl_rename_at_116_10 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bcb8d945730c3cc3 ( Context * __context__, contracts::IsAnyVariantNonPtrMacro const  & __cl_rename_at_116_11 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1b34dd61bced008e ( Context * __context__, contracts::IsAnyFunctionNonPtrMacro const  & __cl_rename_at_116_12 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5f6353bef777d7de ( Context * __context__, contracts::IsAnyLambdaMacro const  & __cl_rename_at_116_13 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d4ee3eafe5fe8da4 ( Context * __context__, contracts::IsRefMacro const  & __cl_rename_at_116_14 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cc5272df618559d1 ( Context * __context__, contracts::IsPointer const  & __cl_rename_at_116_15 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a58089fb4358a0e1 ( Context * __context__, contracts::IsClass const  & __cl_rename_at_116_16 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c114be6b82743afe ( Context * __context__, contracts::IsValueHandle const  & __cl_rename_at_116_17 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3a30a7d605bea5f ( Context * __context__, contracts::IsAnyType const  & __cl_rename_at_116_18 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_37_19, bool __extra_rename_at_37_20, bool __contracts_rename_at_37_21, bool __modules_rename_at_37_22 );
inline bool isYetAnotherVectorTemplate_a260a54ef8aae7f7 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_12_23 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e2d6467c527f3892 ( Context * __context__, contracts::IsAnyArrayMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8ff4a2699ee4a0c5 ( Context * __context__, contracts::IsAnyEnumMacro const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_454ead443fb4c032 ( Context * __context__, contracts::IsAnyBitfieldMacro const  &  __cl_rename_at_116_4 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_4.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b685d2e65f42da2 ( Context * __context__, contracts::IsAnyVectorType const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_eabdcec7c93f4b15 ( Context * __context__, contracts::IsAnyStructMacro const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d76c4e9b31825364 ( Context * __context__, contracts::IsAnyNumericMacro const  &  __cl_rename_at_116_7 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_7.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a97aaf905b3522b5 ( Context * __context__, contracts::IsAnyWorkhorse const  &  __cl_rename_at_116_8 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_8.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_115e7ae3efe17f22 ( Context * __context__, contracts::IsAnyWorkhorseNonPtrMacro const  &  __cl_rename_at_116_9 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_9.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_8bf96d25c51c8832 ( Context * __context__, contracts::IsAnyTupleNonPtrMacro const  &  __cl_rename_at_116_10 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_10.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bcb8d945730c3cc3 ( Context * __context__, contracts::IsAnyVariantNonPtrMacro const  &  __cl_rename_at_116_11 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_11.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1b34dd61bced008e ( Context * __context__, contracts::IsAnyFunctionNonPtrMacro const  &  __cl_rename_at_116_12 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_12.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5f6353bef777d7de ( Context * __context__, contracts::IsAnyLambdaMacro const  &  __cl_rename_at_116_13 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_13.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d4ee3eafe5fe8da4 ( Context * __context__, contracts::IsRefMacro const  &  __cl_rename_at_116_14 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_14.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cc5272df618559d1 ( Context * __context__, contracts::IsPointer const  &  __cl_rename_at_116_15 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_15.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a58089fb4358a0e1 ( Context * __context__, contracts::IsClass const  &  __cl_rename_at_116_16 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_16.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c114be6b82743afe ( Context * __context__, contracts::IsValueHandle const  &  __cl_rename_at_116_17 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_17.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_3a30a7d605bea5f ( Context * __context__, contracts::IsAnyType const  &  __cl_rename_at_116_18 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_18.__rtti))).getStructType())));
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_37_19, bool __extra_rename_at_37_20, bool __contracts_rename_at_37_21, bool __modules_rename_at_37_22 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_37_19,__extra_rename_at_37_20,__contracts_rename_at_37_21,__modules_rename_at_37_22,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline bool isYetAnotherVectorTemplate_a260a54ef8aae7f7 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_12_23 )
{
    return das_auto_cast<bool>::cast((((das_deref(__context__,__td_rename_at_12_23)).isHandle()) && (__td_rename_at_12_23->annotation /*annotation*/ != nullptr)) && ((das_deref(__context__,__td_rename_at_12_23->annotation /*annotation*/)).isYetAnotherVectorTemplate()));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x17cd053e5d46f3ad] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & , ast::AstFunctionAnnotation *  ),&_FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a>>();
    };
    aotLib[0x34325a4de4f05d44] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_e2d6467c527f3892
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyArrayMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_e2d6467c527f3892>>();
    };
    aotLib[0xba4a0d09c0594ba9] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_8ff4a2699ee4a0c5
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyEnumMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_8ff4a2699ee4a0c5>>();
    };
    aotLib[0x4ea2aec5d8a5ec53] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_454ead443fb4c032
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyBitfieldMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_454ead443fb4c032>>();
    };
    aotLib[0x143e1e4b2a79019] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_b685d2e65f42da2
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyVectorType const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_b685d2e65f42da2>>();
    };
    aotLib[0xe91630cccb138f57] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_eabdcec7c93f4b15
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyStructMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_eabdcec7c93f4b15>>();
    };
    aotLib[0xb563d8d41918ff81] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_d76c4e9b31825364
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyNumericMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_d76c4e9b31825364>>();
    };
    aotLib[0x71553e8f274580d3] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_a97aaf905b3522b5
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyWorkhorse const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_a97aaf905b3522b5>>();
    };
    aotLib[0x9bf67859265d6d90] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_115e7ae3efe17f22
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyWorkhorseNonPtrMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_115e7ae3efe17f22>>();
    };
    aotLib[0x6857e8442904ba2d] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_8bf96d25c51c8832
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyTupleNonPtrMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_8bf96d25c51c8832>>();
    };
    aotLib[0xc7a8c7712ee6ef6c] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_bcb8d945730c3cc3
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyVariantNonPtrMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_bcb8d945730c3cc3>>();
    };
    aotLib[0xb6d942212a9a81c] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_1b34dd61bced008e
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyFunctionNonPtrMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_1b34dd61bced008e>>();
    };
    aotLib[0x58f3d9b572add2d3] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_5f6353bef777d7de
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyLambdaMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_5f6353bef777d7de>>();
    };
    aotLib[0xbd378e3f35b44d66] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_d4ee3eafe5fe8da4
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsRefMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_d4ee3eafe5fe8da4>>();
    };
    aotLib[0xa1521992ccfc4fb8] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_cc5272df618559d1
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsPointer const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_cc5272df618559d1>>();
    };
    aotLib[0x2c75d201caf9e3c5] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_a58089fb4358a0e1
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsClass const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_a58089fb4358a0e1>>();
    };
    aotLib[0x3ad42324348028be] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_c114be6b82743afe
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsValueHandle const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_c114be6b82743afe>>();
    };
    aotLib[0xb63e4b5373b658a5] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_3a30a7d605bea5f
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, contracts::IsAnyType const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_3a30a7d605bea5f>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* { // _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const  , bool , bool , bool  ),&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    aotLib[0xcc511aef0177b5ef] = +[](Context & ctx) -> SimNode* { // isYetAnotherVectorTemplate_a260a54ef8aae7f7
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const   ),&isYetAnotherVectorTemplate_a260a54ef8aae7f7>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_2463735248708067149
AotListBase impl_aot_contracts(_anon_2463735248708067149::registerAotFunctions);
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
