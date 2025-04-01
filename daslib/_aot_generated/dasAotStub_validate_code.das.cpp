#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require validate_code
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
 // require contracts

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
namespace _anon_6360572917569013088 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

namespace validate_code { struct ValidateCompletionVisitor; };
namespace validate_code { struct VerifyCompletion; };
namespace validate_code { struct ValidateShaderVisitor; };
namespace validate_code { struct JIT_LLVM; };
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
namespace ast {

struct AstSimulateMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstSimulateMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstSimulateMacro,Program * const ,Context * const )) preSimulate;
    Func DAS_COMMENT((bool,ast::AstSimulateMacro,Program * const ,Context * const )) simulate;
};
static_assert(sizeof(AstSimulateMacro)==32,"structure size mismatch with DAS");
static_assert(offsetof(AstSimulateMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstSimulateMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstSimulateMacro,preSimulate)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstSimulateMacro,simulate)==24,"structure field offset mismatch with DAS");
}
// unused structure AstReaderMacro
// unused structure AstCommentReader
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
namespace ast {

struct AstVisitor {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVisitor)) __finalize;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) preVisitProgram;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) visitProgram;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ,Module * const )) preVisitProgramBody;
    Func DAS_COMMENT((void,ast::AstVisitor,Module * const )) preVisitModule;
    Func DAS_COMMENT((void,ast::AstVisitor,Module * const )) visitModule;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const )) preVisitExprTypeDecl;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const )) visitExprTypeDecl;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const )) preVisitTypeDecl;
    Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const )) visitTypeDecl;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const )) preVisitAlias;
    Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const )) visitAlias;
    Func DAS_COMMENT((bool,ast::AstVisitor,Enumeration * const )) canVisitEnumeration;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const )) preVisitEnumeration;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool)) preVisitEnumerationValue;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool)) visitEnumerationValue;
    Func DAS_COMMENT((smart_ptr_raw<Enumeration>,ast::AstVisitor,smart_ptr_raw<Enumeration> const )) visitEnumeration;
    Func DAS_COMMENT((bool,ast::AstVisitor,Structure * const )) canVisitStructure;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const )) preVisitStructure;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool)) preVisitStructureField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool)) visitStructureField;
    Func DAS_COMMENT((smart_ptr_raw<Structure>,ast::AstVisitor,smart_ptr_raw<Structure> const )) visitStructure;
    Func DAS_COMMENT((bool,ast::AstVisitor,Function * const )) canVisitFunction;
    Func DAS_COMMENT((bool,ast::AstVisitor,Function * const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) canVisitFunctionArgumentInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const )) preVisitFunction;
    Func DAS_COMMENT((smart_ptr_raw<Function>,ast::AstVisitor,smart_ptr_raw<Function> const )) visitFunction;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool)) preVisitFunctionArgument;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool)) visitFunctionArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitFunctionArgumentInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitFunctionArgumentInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const )) preVisitFunctionBody;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const )) visitFunctionBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Expression> const )) preVisitExpression;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Expression> const )) visitExpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) preVisitExprBlock;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) visitExprBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool)) preVisitExprBlockArgument;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool)) visitExprBlockArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitExprBlockArgumentInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitExprBlockArgumentInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) preVisitExprBlockExpression;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) visitExprBlockExpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) preVisitExprBlockFinal;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) visitExprBlockFinal;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) preVisitExprBlockFinalExpression;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) visitExprBlockFinalExpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const )) preVisitExprLet;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const )) visitExprLet;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool)) preVisitExprLetVariable;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool)) visitExprLetVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitExprLetVariableInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitExprLetVariableInit;
    Func DAS_COMMENT((bool,ast::AstVisitor,Variable * const )) canVisitGlobalVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) preVisitGlobalLet;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) visitGlobalLet;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool)) preVisitGlobalLetVariable;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool)) visitGlobalLetVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitGlobalLetVariableInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitGlobalLetVariableInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const )) preVisitExprStringBuilder;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const )) visitExprStringBuilder;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprStringBuilderElement;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool)) visitExprStringBuilderElement;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const )) preVisitExprNew;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const )) visitExprNew;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprNewArgument;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool)) visitExprNewArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const )) preVisitExprNamedCall;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const )) visitExprNamedCall;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool)) preVisitExprNamedCallArgument;
    Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool)) visitExprNamedCallArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const )) preVisitExprLooksLikeCall;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const )) visitExprLooksLikeCall;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) canVisitLooksLikeCallArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprLooksLikeCallArgument;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) visitExprLooksLikeCallArgument;
    Func DAS_COMMENT((bool,ast::AstVisitor,ExprCall * const )) canVisitCall;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const )) preVisitExprCall;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const )) visitExprCall;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprCallArgument;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool)) visitExprCallArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const )) preVisitExprNullCoalescing;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const )) visitExprNullCoalescing;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const ,smart_ptr_raw<Expression> const )) preVisitExprNullCoalescingDefault;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const )) preVisitExprAt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAt> const )) visitExprAt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const )) preVisitExprAtIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const )) preVisitExprSafeAt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const )) visitExprSafeAt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const )) preVisitExprSafeAtIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const )) preVisitExprIs;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIs> const )) visitExprIs;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const ,smart_ptr_raw<TypeDecl> const )) preVisitExprIsType;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const )) preVisitExprOp2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp2> const )) visitExprOp2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const ,smart_ptr_raw<Expression> const )) preVisitExprOp2Right;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const )) preVisitExprOp3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp3> const )) visitExprOp3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const )) preVisitExprOp3Left;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const )) preVisitExprOp3Right;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprCopy> const )) isRightFirstExprCopy;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const )) preVisitExprCopy;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCopy> const )) visitExprCopy;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ,smart_ptr_raw<Expression> const )) preVisitExprCopyRight;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMove> const )) isRightFirstExprMove;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const )) preVisitExprMove;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMove> const )) visitExprMove;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const ,smart_ptr_raw<Expression> const )) preVisitExprMoveRight;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprClone> const )) isRightFirstExprClone;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const )) preVisitExprClone;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprClone> const )) visitExprClone;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const ,smart_ptr_raw<Expression> const )) preVisitExprCloneRight;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprAssume> const )) canVisitWithAliasSubexpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssume> const )) preVisitExprAssume;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssume> const )) visitExprAssume;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const )) preVisitExprWith;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWith> const )) visitExprWith;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const ,smart_ptr_raw<Expression> const )) preVisitExprWithBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const )) preVisitExprWhile;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWhile> const )) visitExprWhile;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const ,smart_ptr_raw<Expression> const )) preVisitExprWhileBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const )) preVisitExprTryCatch;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const )) visitExprTryCatch;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const ,smart_ptr_raw<Expression> const )) preVisitExprTryCatchCatch;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const )) preVisitExprIfThenElse;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const )) visitExprIfThenElse;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const )) preVisitExprIfThenElseIfBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const )) preVisitExprIfThenElseElseBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) preVisitExprFor;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) visitExprFor;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool)) preVisitExprForVariable;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool)) visitExprForVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprForSource;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool)) visitExprForSource;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) preVisitExprForStack;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) preVisitExprForBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const )) preVisitExprMakeVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const )) visitExprMakeVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) preVisitExprMakeVariantField;
    Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) visitExprMakeVariantField;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) canVisitMakeStructBody;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) canVisitMakeStructBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) preVisitExprMakeStruct;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) visitExprMakeStruct;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool)) preVisitExprMakeStructIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool)) visitExprMakeStructIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) preVisitExprMakeStructField;
    Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) visitExprMakeStructField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) preVisitMakeStructureBlock;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) visitMakeStructureBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const )) preVisitExprMakeArray;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const )) visitExprMakeArray;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) preVisitExprMakeArrayIndex;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) visitExprMakeArrayIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const )) preVisitExprMakeTuple;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const )) visitExprMakeTuple;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) preVisitExprMakeTupleIndex;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) visitExprMakeTupleIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const )) preVisitExprArrayComprehension;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const )) visitExprArrayComprehension;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const )) preVisitExprArrayComprehensionSubexpr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const )) preVisitExprArrayComprehensionWhere;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const )) preVisitExprTypeInfo;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const )) visitExprTypeInfo;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const )) preVisitExprPtr2Ref;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const )) visitExprPtr2Ref;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLabel> const )) preVisitExprLabel;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLabel> const )) visitExprLabel;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprGoto> const )) preVisitExprGoto;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprGoto> const )) visitExprGoto;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const )) preVisitExprRef2Value;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const )) visitExprRef2Value;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const )) preVisitExprRef2Ptr;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const )) visitExprRef2Ptr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAddr> const )) preVisitExprAddr;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAddr> const )) visitExprAddr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssert> const )) preVisitExprAssert;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssert> const )) visitExprAssert;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const )) preVisitExprStaticAssert;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const )) visitExprStaticAssert;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprQuote> const )) preVisitExprQuote;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprQuote> const )) visitExprQuote;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDebug> const )) preVisitExprDebug;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDebug> const )) visitExprDebug;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const )) preVisitExprInvoke;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const )) visitExprInvoke;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprErase> const )) preVisitExprErase;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprErase> const )) visitExprErase;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const )) preVisitExprSetInsert;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const )) visitExprSetInsert;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFind> const )) preVisitExprFind;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFind> const )) visitExprFind;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const )) preVisitExprKeyExists;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const )) visitExprKeyExists;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAscend> const )) preVisitExprAscend;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAscend> const )) visitExprAscend;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCast> const )) preVisitExprCast;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCast> const )) visitExprCast;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const )) preVisitExprDelete;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDelete> const )) visitExprDelete;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprVar> const )) preVisitExprVar;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprVar> const )) visitExprVar;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const )) preVisitExprTag;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const ,smart_ptr_raw<Expression> const )) preVisitExprTagValue;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTag> const )) visitExprTag;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprField> const )) preVisitExprField;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprField> const )) visitExprField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const )) preVisitExprSafeField;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const )) visitExprSafeField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const )) preVisitExprSwizzle;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const )) visitExprSwizzle;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const )) preVisitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const )) preVisitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const )) preVisitExprSafeAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp1> const )) preVisitExprOp1;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp1> const )) visitExprOp1;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReturn> const )) preVisitExprReturn;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReturn> const )) visitExprReturn;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprYield> const )) preVisitExprYield;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprYield> const )) visitExprYield;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBreak> const )) preVisitExprBreak;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBreak> const )) visitExprBreak;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprContinue> const )) preVisitExprContinue;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprContinue> const )) visitExprContinue;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const )) canVisitMakeBlockBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const )) preVisitExprMakeBlock;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const )) visitExprMakeBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const )) preVisitExprMakeGenerator;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const )) visitExprMakeGenerator;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const )) preVisitExprMemZero;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const )) visitExprMemZero;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConst> const )) preVisitExprConst;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConst> const )) visitExprConst;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const )) preVisitExprConstPtr;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const )) visitExprConstPtr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const )) preVisitExprConstEnumeration;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const )) visitExprConstEnumeration;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const )) preVisitExprConstBitfield;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const )) visitExprConstBitfield;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const )) preVisitExprConstInt8;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const )) visitExprConstInt8;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const )) preVisitExprConstInt16;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const )) visitExprConstInt16;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const )) preVisitExprConstInt64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const )) visitExprConstInt64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const )) preVisitExprConstInt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const )) visitExprConstInt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const )) preVisitExprConstInt2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const )) visitExprConstInt2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const )) preVisitExprConstInt3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const )) visitExprConstInt3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const )) preVisitExprConstInt4;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const )) visitExprConstInt4;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const )) preVisitExprConstUInt8;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const )) visitExprConstUInt8;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const )) preVisitExprConstUInt16;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const )) visitExprConstUInt16;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const )) preVisitExprConstUInt64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const )) visitExprConstUInt64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const )) preVisitExprConstUInt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const )) visitExprConstUInt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const )) preVisitExprConstUInt2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const )) visitExprConstUInt2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const )) preVisitExprConstUInt3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const )) visitExprConstUInt3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const )) preVisitExprConstUInt4;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const )) visitExprConstUInt4;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const )) preVisitExprConstRange;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const )) visitExprConstRange;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const )) preVisitExprConstURange;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const )) visitExprConstURange;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const )) preVisitExprConstRange64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const )) visitExprConstRange64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const )) preVisitExprConstURange64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const )) visitExprConstURange64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const )) preVisitExprConstBool;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const )) visitExprConstBool;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const )) preVisitExprConstFloat;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const )) visitExprConstFloat;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const )) preVisitExprConstFloat2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const )) visitExprConstFloat2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const )) preVisitExprConstFloat3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const )) visitExprConstFloat3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const )) preVisitExprConstFloat4;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const )) visitExprConstFloat4;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstString> const )) preVisitExprConstString;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstString> const )) visitExprConstString;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const )) preVisitExprConstDouble;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const )) visitExprConstDouble;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const )) preVisitExprFakeContext;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const )) visitExprFakeContext;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const )) preVisitExprFakeLineInfo;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const )) visitExprFakeLineInfo;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReader> const )) preVisitExprReader;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReader> const )) visitExprReader;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const )) preVisitExprUnsafe;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const )) visitExprUnsafe;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const )) preVisitExprCallMacro;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const )) visitExprCallMacro;
};
static_assert(sizeof(AstVisitor)==2424,"structure size mismatch with DAS");
static_assert(offsetof(AstVisitor,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitProgram)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitProgram)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitProgramBody)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitModule)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitModule)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprTypeDecl)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprTypeDecl)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitTypeDecl)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitTypeDecl)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitAlias)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitAlias)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitEnumeration)==104,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitEnumeration)==112,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitEnumerationValue)==120,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitEnumerationValue)==128,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitEnumeration)==136,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitStructure)==144,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitStructure)==152,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitStructureField)==160,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitStructureField)==168,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitStructure)==176,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitFunction)==184,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitFunctionArgumentInit)==192,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitFunction)==200,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitFunction)==208,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitFunctionArgument)==216,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitFunctionArgument)==224,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitFunctionArgumentInit)==232,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitFunctionArgumentInit)==240,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitFunctionBody)==248,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitFunctionBody)==256,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExpression)==264,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExpression)==272,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprBlock)==280,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprBlock)==288,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprBlockArgument)==296,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprBlockArgument)==304,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprBlockArgumentInit)==312,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprBlockArgumentInit)==320,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprBlockExpression)==328,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprBlockExpression)==336,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprBlockFinal)==344,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprBlockFinal)==352,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprBlockFinalExpression)==360,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprBlockFinalExpression)==368,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprLet)==376,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprLet)==384,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprLetVariable)==392,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprLetVariable)==400,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprLetVariableInit)==408,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprLetVariableInit)==416,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitGlobalVariable)==424,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitGlobalLet)==432,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitGlobalLet)==440,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitGlobalLetVariable)==448,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitGlobalLetVariable)==456,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitGlobalLetVariableInit)==464,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitGlobalLetVariableInit)==472,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprStringBuilder)==480,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprStringBuilder)==488,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprStringBuilderElement)==496,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprStringBuilderElement)==504,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprNew)==512,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprNew)==520,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprNewArgument)==528,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprNewArgument)==536,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprNamedCall)==544,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprNamedCall)==552,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprNamedCallArgument)==560,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprNamedCallArgument)==568,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprLooksLikeCall)==576,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprLooksLikeCall)==584,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitLooksLikeCallArgument)==592,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprLooksLikeCallArgument)==600,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprLooksLikeCallArgument)==608,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitCall)==616,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprCall)==624,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprCall)==632,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprCallArgument)==640,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprCallArgument)==648,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprNullCoalescing)==656,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprNullCoalescing)==664,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprNullCoalescingDefault)==672,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprAt)==680,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprAt)==688,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprAtIndex)==696,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprSafeAt)==704,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprSafeAt)==712,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprSafeAtIndex)==720,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprIs)==728,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprIs)==736,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprIsType)==744,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprOp2)==752,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprOp2)==760,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprOp2Right)==768,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprOp3)==776,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprOp3)==784,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprOp3Left)==792,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprOp3Right)==800,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,isRightFirstExprCopy)==808,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprCopy)==816,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprCopy)==824,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprCopyRight)==832,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,isRightFirstExprMove)==840,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMove)==848,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMove)==856,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMoveRight)==864,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,isRightFirstExprClone)==872,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprClone)==880,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprClone)==888,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprCloneRight)==896,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitWithAliasSubexpression)==904,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprAssume)==912,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprAssume)==920,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprWith)==928,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprWith)==936,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprWithBody)==944,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprWhile)==952,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprWhile)==960,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprWhileBody)==968,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprTryCatch)==976,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprTryCatch)==984,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprTryCatchCatch)==992,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprIfThenElse)==1000,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprIfThenElse)==1008,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprIfThenElseIfBlock)==1016,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprIfThenElseElseBlock)==1024,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprFor)==1032,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprFor)==1040,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprForVariable)==1048,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprForVariable)==1056,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprForSource)==1064,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprForSource)==1072,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprForStack)==1080,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprForBody)==1088,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeVariant)==1096,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeVariant)==1104,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeVariantField)==1112,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeVariantField)==1120,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitMakeStructBody)==1128,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitMakeStructBlock)==1136,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeStruct)==1144,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeStruct)==1152,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeStructIndex)==1160,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeStructIndex)==1168,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeStructField)==1176,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeStructField)==1184,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitMakeStructureBlock)==1192,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitMakeStructureBlock)==1200,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeArray)==1208,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeArray)==1216,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeArrayIndex)==1224,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeArrayIndex)==1232,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeTuple)==1240,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeTuple)==1248,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeTupleIndex)==1256,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeTupleIndex)==1264,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprArrayComprehension)==1272,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprArrayComprehension)==1280,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprArrayComprehensionSubexpr)==1288,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprArrayComprehensionWhere)==1296,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprTypeInfo)==1304,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprTypeInfo)==1312,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprPtr2Ref)==1320,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprPtr2Ref)==1328,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprLabel)==1336,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprLabel)==1344,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprGoto)==1352,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprGoto)==1360,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprRef2Value)==1368,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprRef2Value)==1376,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprRef2Ptr)==1384,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprRef2Ptr)==1392,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprAddr)==1400,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprAddr)==1408,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprAssert)==1416,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprAssert)==1424,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprStaticAssert)==1432,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprStaticAssert)==1440,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprQuote)==1448,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprQuote)==1456,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprDebug)==1464,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprDebug)==1472,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprInvoke)==1480,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprInvoke)==1488,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprErase)==1496,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprErase)==1504,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprSetInsert)==1512,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprSetInsert)==1520,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprFind)==1528,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprFind)==1536,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprKeyExists)==1544,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprKeyExists)==1552,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprAscend)==1560,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprAscend)==1568,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprCast)==1576,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprCast)==1584,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprDelete)==1592,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprDelete)==1600,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprVar)==1608,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprVar)==1616,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprTag)==1624,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprTagValue)==1632,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprTag)==1640,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprField)==1648,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprField)==1656,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprSafeField)==1664,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprSafeField)==1672,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprSwizzle)==1680,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprSwizzle)==1688,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprIsVariant)==1696,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprIsVariant)==1704,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprAsVariant)==1712,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprAsVariant)==1720,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprSafeAsVariant)==1728,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprSafeAsVariant)==1736,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprOp1)==1744,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprOp1)==1752,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprReturn)==1760,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprReturn)==1768,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprYield)==1776,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprYield)==1784,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprBreak)==1792,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprBreak)==1800,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprContinue)==1808,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprContinue)==1816,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,canVisitMakeBlockBody)==1824,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeBlock)==1832,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeBlock)==1840,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMakeGenerator)==1848,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMakeGenerator)==1856,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprMemZero)==1864,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprMemZero)==1872,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConst)==1880,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConst)==1888,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstPtr)==1896,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstPtr)==1904,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstEnumeration)==1912,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstEnumeration)==1920,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstBitfield)==1928,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstBitfield)==1936,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstInt8)==1944,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstInt8)==1952,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstInt16)==1960,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstInt16)==1968,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstInt64)==1976,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstInt64)==1984,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstInt)==1992,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstInt)==2000,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstInt2)==2008,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstInt2)==2016,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstInt3)==2024,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstInt3)==2032,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstInt4)==2040,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstInt4)==2048,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstUInt8)==2056,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstUInt8)==2064,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstUInt16)==2072,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstUInt16)==2080,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstUInt64)==2088,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstUInt64)==2096,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstUInt)==2104,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstUInt)==2112,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstUInt2)==2120,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstUInt2)==2128,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstUInt3)==2136,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstUInt3)==2144,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstUInt4)==2152,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstUInt4)==2160,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstRange)==2168,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstRange)==2176,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstURange)==2184,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstURange)==2192,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstRange64)==2200,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstRange64)==2208,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstURange64)==2216,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstURange64)==2224,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstBool)==2232,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstBool)==2240,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstFloat)==2248,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstFloat)==2256,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstFloat2)==2264,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstFloat2)==2272,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstFloat3)==2280,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstFloat3)==2288,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstFloat4)==2296,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstFloat4)==2304,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstString)==2312,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstString)==2320,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprConstDouble)==2328,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprConstDouble)==2336,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprFakeContext)==2344,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprFakeContext)==2352,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprFakeLineInfo)==2360,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprFakeLineInfo)==2368,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprReader)==2376,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprReader)==2384,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprUnsafe)==2392,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprUnsafe)==2400,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,preVisitExprCallMacro)==2408,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVisitor,visitExprCallMacro)==2416,"structure field offset mismatch with DAS");
}
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
namespace validate_code {

struct ValidateCompletionVisitor {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVisitor)) __finalize;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) preVisitProgram;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) visitProgram;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ,Module * const )) preVisitProgramBody;
    Func DAS_COMMENT((void,ast::AstVisitor,Module * const )) preVisitModule;
    Func DAS_COMMENT((void,ast::AstVisitor,Module * const )) visitModule;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const )) preVisitExprTypeDecl;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const )) visitExprTypeDecl;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const )) preVisitTypeDecl;
    Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const )) visitTypeDecl;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const )) preVisitAlias;
    Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const )) visitAlias;
    Func DAS_COMMENT((bool,ast::AstVisitor,Enumeration * const )) canVisitEnumeration;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const )) preVisitEnumeration;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool)) preVisitEnumerationValue;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool)) visitEnumerationValue;
    Func DAS_COMMENT((smart_ptr_raw<Enumeration>,ast::AstVisitor,smart_ptr_raw<Enumeration> const )) visitEnumeration;
    Func DAS_COMMENT((bool,ast::AstVisitor,Structure * const )) canVisitStructure;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const )) preVisitStructure;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool)) preVisitStructureField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool)) visitStructureField;
    Func DAS_COMMENT((smart_ptr_raw<Structure>,ast::AstVisitor,smart_ptr_raw<Structure> const )) visitStructure;
    Func DAS_COMMENT((bool,ast::AstVisitor,Function * const )) canVisitFunction;
    Func DAS_COMMENT((bool,ast::AstVisitor,Function * const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) canVisitFunctionArgumentInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const )) preVisitFunction;
    Func DAS_COMMENT((smart_ptr_raw<Function>,ast::AstVisitor,smart_ptr_raw<Function> const )) visitFunction;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool)) preVisitFunctionArgument;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool)) visitFunctionArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitFunctionArgumentInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitFunctionArgumentInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const )) preVisitFunctionBody;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const )) visitFunctionBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Expression> const )) preVisitExpression;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Expression> const )) visitExpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) preVisitExprBlock;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) visitExprBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool)) preVisitExprBlockArgument;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool)) visitExprBlockArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitExprBlockArgumentInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitExprBlockArgumentInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) preVisitExprBlockExpression;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) visitExprBlockExpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) preVisitExprBlockFinal;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) visitExprBlockFinal;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) preVisitExprBlockFinalExpression;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) visitExprBlockFinalExpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const )) preVisitExprLet;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const )) visitExprLet;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool)) preVisitExprLetVariable;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool)) visitExprLetVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitExprLetVariableInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitExprLetVariableInit;
    Func DAS_COMMENT((bool,ast::AstVisitor,Variable * const )) canVisitGlobalVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) preVisitGlobalLet;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) visitGlobalLet;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool)) preVisitGlobalLetVariable;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool)) visitGlobalLetVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitGlobalLetVariableInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitGlobalLetVariableInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const )) preVisitExprStringBuilder;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const )) visitExprStringBuilder;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprStringBuilderElement;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool)) visitExprStringBuilderElement;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const )) preVisitExprNew;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const )) visitExprNew;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprNewArgument;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool)) visitExprNewArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const )) preVisitExprNamedCall;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const )) visitExprNamedCall;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool)) preVisitExprNamedCallArgument;
    Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool)) visitExprNamedCallArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const )) preVisitExprLooksLikeCall;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const )) visitExprLooksLikeCall;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) canVisitLooksLikeCallArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprLooksLikeCallArgument;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) visitExprLooksLikeCallArgument;
    Func DAS_COMMENT((bool,ast::AstVisitor,ExprCall * const )) canVisitCall;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const )) preVisitExprCall;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const )) visitExprCall;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprCallArgument;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool)) visitExprCallArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const )) preVisitExprNullCoalescing;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const )) visitExprNullCoalescing;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const ,smart_ptr_raw<Expression> const )) preVisitExprNullCoalescingDefault;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const )) preVisitExprAt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAt> const )) visitExprAt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const )) preVisitExprAtIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const )) preVisitExprSafeAt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const )) visitExprSafeAt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const )) preVisitExprSafeAtIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const )) preVisitExprIs;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIs> const )) visitExprIs;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const ,smart_ptr_raw<TypeDecl> const )) preVisitExprIsType;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const )) preVisitExprOp2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp2> const )) visitExprOp2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const ,smart_ptr_raw<Expression> const )) preVisitExprOp2Right;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const )) preVisitExprOp3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp3> const )) visitExprOp3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const )) preVisitExprOp3Left;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const )) preVisitExprOp3Right;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprCopy> const )) isRightFirstExprCopy;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const )) preVisitExprCopy;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCopy> const )) visitExprCopy;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ,smart_ptr_raw<Expression> const )) preVisitExprCopyRight;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMove> const )) isRightFirstExprMove;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const )) preVisitExprMove;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMove> const )) visitExprMove;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const ,smart_ptr_raw<Expression> const )) preVisitExprMoveRight;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprClone> const )) isRightFirstExprClone;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const )) preVisitExprClone;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprClone> const )) visitExprClone;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const ,smart_ptr_raw<Expression> const )) preVisitExprCloneRight;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprAssume> const )) canVisitWithAliasSubexpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssume> const )) preVisitExprAssume;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssume> const )) visitExprAssume;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const )) preVisitExprWith;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWith> const )) visitExprWith;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const ,smart_ptr_raw<Expression> const )) preVisitExprWithBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const )) preVisitExprWhile;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWhile> const )) visitExprWhile;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const ,smart_ptr_raw<Expression> const )) preVisitExprWhileBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const )) preVisitExprTryCatch;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const )) visitExprTryCatch;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const ,smart_ptr_raw<Expression> const )) preVisitExprTryCatchCatch;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const )) preVisitExprIfThenElse;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const )) visitExprIfThenElse;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const )) preVisitExprIfThenElseIfBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const )) preVisitExprIfThenElseElseBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) preVisitExprFor;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) visitExprFor;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool)) preVisitExprForVariable;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool)) visitExprForVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprForSource;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool)) visitExprForSource;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) preVisitExprForStack;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) preVisitExprForBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const )) preVisitExprMakeVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const )) visitExprMakeVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) preVisitExprMakeVariantField;
    Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) visitExprMakeVariantField;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) canVisitMakeStructBody;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) canVisitMakeStructBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) preVisitExprMakeStruct;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) visitExprMakeStruct;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool)) preVisitExprMakeStructIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool)) visitExprMakeStructIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) preVisitExprMakeStructField;
    Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) visitExprMakeStructField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) preVisitMakeStructureBlock;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) visitMakeStructureBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const )) preVisitExprMakeArray;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const )) visitExprMakeArray;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) preVisitExprMakeArrayIndex;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) visitExprMakeArrayIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const )) preVisitExprMakeTuple;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const )) visitExprMakeTuple;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) preVisitExprMakeTupleIndex;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) visitExprMakeTupleIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const )) preVisitExprArrayComprehension;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const )) visitExprArrayComprehension;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const )) preVisitExprArrayComprehensionSubexpr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const )) preVisitExprArrayComprehensionWhere;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const )) preVisitExprTypeInfo;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const )) visitExprTypeInfo;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const )) preVisitExprPtr2Ref;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const )) visitExprPtr2Ref;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLabel> const )) preVisitExprLabel;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLabel> const )) visitExprLabel;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprGoto> const )) preVisitExprGoto;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprGoto> const )) visitExprGoto;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const )) preVisitExprRef2Value;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const )) visitExprRef2Value;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const )) preVisitExprRef2Ptr;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const )) visitExprRef2Ptr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAddr> const )) preVisitExprAddr;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAddr> const )) visitExprAddr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssert> const )) preVisitExprAssert;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssert> const )) visitExprAssert;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const )) preVisitExprStaticAssert;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const )) visitExprStaticAssert;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprQuote> const )) preVisitExprQuote;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprQuote> const )) visitExprQuote;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDebug> const )) preVisitExprDebug;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDebug> const )) visitExprDebug;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const )) preVisitExprInvoke;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const )) visitExprInvoke;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprErase> const )) preVisitExprErase;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprErase> const )) visitExprErase;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const )) preVisitExprSetInsert;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const )) visitExprSetInsert;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFind> const )) preVisitExprFind;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFind> const )) visitExprFind;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const )) preVisitExprKeyExists;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const )) visitExprKeyExists;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAscend> const )) preVisitExprAscend;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAscend> const )) visitExprAscend;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCast> const )) preVisitExprCast;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCast> const )) visitExprCast;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const )) preVisitExprDelete;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDelete> const )) visitExprDelete;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprVar> const )) preVisitExprVar;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprVar> const )) visitExprVar;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const )) preVisitExprTag;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const ,smart_ptr_raw<Expression> const )) preVisitExprTagValue;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTag> const )) visitExprTag;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprField> const )) preVisitExprField;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprField> const )) visitExprField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const )) preVisitExprSafeField;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const )) visitExprSafeField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const )) preVisitExprSwizzle;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const )) visitExprSwizzle;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const )) preVisitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const )) preVisitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const )) preVisitExprSafeAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp1> const )) preVisitExprOp1;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp1> const )) visitExprOp1;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReturn> const )) preVisitExprReturn;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReturn> const )) visitExprReturn;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprYield> const )) preVisitExprYield;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprYield> const )) visitExprYield;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBreak> const )) preVisitExprBreak;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBreak> const )) visitExprBreak;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprContinue> const )) preVisitExprContinue;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprContinue> const )) visitExprContinue;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const )) canVisitMakeBlockBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const )) preVisitExprMakeBlock;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const )) visitExprMakeBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const )) preVisitExprMakeGenerator;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const )) visitExprMakeGenerator;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const )) preVisitExprMemZero;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const )) visitExprMemZero;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConst> const )) preVisitExprConst;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConst> const )) visitExprConst;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const )) preVisitExprConstPtr;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const )) visitExprConstPtr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const )) preVisitExprConstEnumeration;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const )) visitExprConstEnumeration;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const )) preVisitExprConstBitfield;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const )) visitExprConstBitfield;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const )) preVisitExprConstInt8;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const )) visitExprConstInt8;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const )) preVisitExprConstInt16;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const )) visitExprConstInt16;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const )) preVisitExprConstInt64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const )) visitExprConstInt64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const )) preVisitExprConstInt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const )) visitExprConstInt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const )) preVisitExprConstInt2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const )) visitExprConstInt2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const )) preVisitExprConstInt3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const )) visitExprConstInt3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const )) preVisitExprConstInt4;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const )) visitExprConstInt4;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const )) preVisitExprConstUInt8;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const )) visitExprConstUInt8;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const )) preVisitExprConstUInt16;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const )) visitExprConstUInt16;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const )) preVisitExprConstUInt64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const )) visitExprConstUInt64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const )) preVisitExprConstUInt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const )) visitExprConstUInt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const )) preVisitExprConstUInt2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const )) visitExprConstUInt2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const )) preVisitExprConstUInt3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const )) visitExprConstUInt3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const )) preVisitExprConstUInt4;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const )) visitExprConstUInt4;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const )) preVisitExprConstRange;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const )) visitExprConstRange;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const )) preVisitExprConstURange;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const )) visitExprConstURange;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const )) preVisitExprConstRange64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const )) visitExprConstRange64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const )) preVisitExprConstURange64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const )) visitExprConstURange64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const )) preVisitExprConstBool;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const )) visitExprConstBool;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const )) preVisitExprConstFloat;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const )) visitExprConstFloat;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const )) preVisitExprConstFloat2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const )) visitExprConstFloat2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const )) preVisitExprConstFloat3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const )) visitExprConstFloat3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const )) preVisitExprConstFloat4;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const )) visitExprConstFloat4;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstString> const )) preVisitExprConstString;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstString> const )) visitExprConstString;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const )) preVisitExprConstDouble;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const )) visitExprConstDouble;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const )) preVisitExprFakeContext;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const )) visitExprFakeContext;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const )) preVisitExprFakeLineInfo;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const )) visitExprFakeLineInfo;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReader> const )) preVisitExprReader;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReader> const )) visitExprReader;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const )) preVisitExprUnsafe;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const )) visitExprUnsafe;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const )) preVisitExprCallMacro;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const )) visitExprCallMacro;
    Function * thisFunc;
};
static_assert(sizeof(ValidateCompletionVisitor)==2432,"structure size mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitProgram)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitProgram)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitProgramBody)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitModule)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitModule)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprTypeDecl)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprTypeDecl)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitTypeDecl)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitTypeDecl)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitAlias)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitAlias)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitEnumeration)==104,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitEnumeration)==112,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitEnumerationValue)==120,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitEnumerationValue)==128,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitEnumeration)==136,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitStructure)==144,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitStructure)==152,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitStructureField)==160,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitStructureField)==168,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitStructure)==176,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitFunction)==184,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitFunctionArgumentInit)==192,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitFunction)==200,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitFunction)==208,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitFunctionArgument)==216,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitFunctionArgument)==224,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitFunctionArgumentInit)==232,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitFunctionArgumentInit)==240,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitFunctionBody)==248,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitFunctionBody)==256,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExpression)==264,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExpression)==272,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprBlock)==280,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprBlock)==288,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprBlockArgument)==296,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprBlockArgument)==304,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprBlockArgumentInit)==312,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprBlockArgumentInit)==320,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprBlockExpression)==328,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprBlockExpression)==336,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprBlockFinal)==344,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprBlockFinal)==352,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprBlockFinalExpression)==360,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprBlockFinalExpression)==368,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprLet)==376,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprLet)==384,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprLetVariable)==392,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprLetVariable)==400,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprLetVariableInit)==408,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprLetVariableInit)==416,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitGlobalVariable)==424,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitGlobalLet)==432,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitGlobalLet)==440,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitGlobalLetVariable)==448,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitGlobalLetVariable)==456,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitGlobalLetVariableInit)==464,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitGlobalLetVariableInit)==472,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprStringBuilder)==480,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprStringBuilder)==488,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprStringBuilderElement)==496,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprStringBuilderElement)==504,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprNew)==512,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprNew)==520,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprNewArgument)==528,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprNewArgument)==536,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprNamedCall)==544,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprNamedCall)==552,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprNamedCallArgument)==560,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprNamedCallArgument)==568,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprLooksLikeCall)==576,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprLooksLikeCall)==584,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitLooksLikeCallArgument)==592,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprLooksLikeCallArgument)==600,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprLooksLikeCallArgument)==608,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitCall)==616,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprCall)==624,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprCall)==632,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprCallArgument)==640,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprCallArgument)==648,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprNullCoalescing)==656,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprNullCoalescing)==664,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprNullCoalescingDefault)==672,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprAt)==680,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprAt)==688,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprAtIndex)==696,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprSafeAt)==704,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprSafeAt)==712,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprSafeAtIndex)==720,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprIs)==728,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprIs)==736,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprIsType)==744,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprOp2)==752,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprOp2)==760,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprOp2Right)==768,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprOp3)==776,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprOp3)==784,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprOp3Left)==792,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprOp3Right)==800,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,isRightFirstExprCopy)==808,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprCopy)==816,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprCopy)==824,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprCopyRight)==832,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,isRightFirstExprMove)==840,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMove)==848,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMove)==856,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMoveRight)==864,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,isRightFirstExprClone)==872,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprClone)==880,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprClone)==888,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprCloneRight)==896,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitWithAliasSubexpression)==904,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprAssume)==912,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprAssume)==920,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprWith)==928,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprWith)==936,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprWithBody)==944,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprWhile)==952,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprWhile)==960,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprWhileBody)==968,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprTryCatch)==976,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprTryCatch)==984,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprTryCatchCatch)==992,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprIfThenElse)==1000,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprIfThenElse)==1008,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprIfThenElseIfBlock)==1016,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprIfThenElseElseBlock)==1024,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprFor)==1032,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprFor)==1040,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprForVariable)==1048,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprForVariable)==1056,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprForSource)==1064,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprForSource)==1072,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprForStack)==1080,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprForBody)==1088,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeVariant)==1096,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeVariant)==1104,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeVariantField)==1112,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeVariantField)==1120,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitMakeStructBody)==1128,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitMakeStructBlock)==1136,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeStruct)==1144,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeStruct)==1152,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeStructIndex)==1160,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeStructIndex)==1168,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeStructField)==1176,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeStructField)==1184,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitMakeStructureBlock)==1192,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitMakeStructureBlock)==1200,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeArray)==1208,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeArray)==1216,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeArrayIndex)==1224,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeArrayIndex)==1232,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeTuple)==1240,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeTuple)==1248,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeTupleIndex)==1256,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeTupleIndex)==1264,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprArrayComprehension)==1272,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprArrayComprehension)==1280,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprArrayComprehensionSubexpr)==1288,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprArrayComprehensionWhere)==1296,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprTypeInfo)==1304,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprTypeInfo)==1312,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprPtr2Ref)==1320,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprPtr2Ref)==1328,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprLabel)==1336,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprLabel)==1344,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprGoto)==1352,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprGoto)==1360,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprRef2Value)==1368,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprRef2Value)==1376,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprRef2Ptr)==1384,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprRef2Ptr)==1392,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprAddr)==1400,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprAddr)==1408,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprAssert)==1416,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprAssert)==1424,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprStaticAssert)==1432,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprStaticAssert)==1440,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprQuote)==1448,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprQuote)==1456,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprDebug)==1464,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprDebug)==1472,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprInvoke)==1480,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprInvoke)==1488,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprErase)==1496,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprErase)==1504,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprSetInsert)==1512,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprSetInsert)==1520,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprFind)==1528,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprFind)==1536,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprKeyExists)==1544,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprKeyExists)==1552,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprAscend)==1560,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprAscend)==1568,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprCast)==1576,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprCast)==1584,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprDelete)==1592,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprDelete)==1600,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprVar)==1608,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprVar)==1616,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprTag)==1624,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprTagValue)==1632,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprTag)==1640,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprField)==1648,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprField)==1656,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprSafeField)==1664,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprSafeField)==1672,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprSwizzle)==1680,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprSwizzle)==1688,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprIsVariant)==1696,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprIsVariant)==1704,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprAsVariant)==1712,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprAsVariant)==1720,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprSafeAsVariant)==1728,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprSafeAsVariant)==1736,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprOp1)==1744,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprOp1)==1752,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprReturn)==1760,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprReturn)==1768,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprYield)==1776,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprYield)==1784,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprBreak)==1792,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprBreak)==1800,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprContinue)==1808,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprContinue)==1816,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,canVisitMakeBlockBody)==1824,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeBlock)==1832,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeBlock)==1840,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMakeGenerator)==1848,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMakeGenerator)==1856,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprMemZero)==1864,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprMemZero)==1872,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConst)==1880,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConst)==1888,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstPtr)==1896,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstPtr)==1904,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstEnumeration)==1912,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstEnumeration)==1920,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstBitfield)==1928,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstBitfield)==1936,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstInt8)==1944,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstInt8)==1952,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstInt16)==1960,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstInt16)==1968,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstInt64)==1976,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstInt64)==1984,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstInt)==1992,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstInt)==2000,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstInt2)==2008,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstInt2)==2016,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstInt3)==2024,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstInt3)==2032,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstInt4)==2040,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstInt4)==2048,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstUInt8)==2056,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstUInt8)==2064,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstUInt16)==2072,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstUInt16)==2080,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstUInt64)==2088,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstUInt64)==2096,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstUInt)==2104,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstUInt)==2112,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstUInt2)==2120,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstUInt2)==2128,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstUInt3)==2136,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstUInt3)==2144,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstUInt4)==2152,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstUInt4)==2160,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstRange)==2168,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstRange)==2176,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstURange)==2184,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstURange)==2192,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstRange64)==2200,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstRange64)==2208,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstURange64)==2216,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstURange64)==2224,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstBool)==2232,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstBool)==2240,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstFloat)==2248,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstFloat)==2256,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstFloat2)==2264,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstFloat2)==2272,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstFloat3)==2280,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstFloat3)==2288,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstFloat4)==2296,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstFloat4)==2304,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstString)==2312,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstString)==2320,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprConstDouble)==2328,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprConstDouble)==2336,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprFakeContext)==2344,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprFakeContext)==2352,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprFakeLineInfo)==2360,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprFakeLineInfo)==2368,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprReader)==2376,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprReader)==2384,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprUnsafe)==2392,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprUnsafe)==2400,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,preVisitExprCallMacro)==2408,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,visitExprCallMacro)==2416,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateCompletionVisitor,thisFunc)==2424,"structure field offset mismatch with DAS");
}
namespace validate_code {

struct VerifyCompletion {
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
static_assert(sizeof(VerifyCompletion)==112,"structure size mismatch with DAS");
static_assert(offsetof(VerifyCompletion,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(VerifyCompletion,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace validate_code {

struct ValidateShaderVisitor {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVisitor)) __finalize;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) preVisitProgram;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) visitProgram;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ,Module * const )) preVisitProgramBody;
    Func DAS_COMMENT((void,ast::AstVisitor,Module * const )) preVisitModule;
    Func DAS_COMMENT((void,ast::AstVisitor,Module * const )) visitModule;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const )) preVisitExprTypeDecl;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const )) visitExprTypeDecl;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const )) preVisitTypeDecl;
    Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const )) visitTypeDecl;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const )) preVisitAlias;
    Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const )) visitAlias;
    Func DAS_COMMENT((bool,ast::AstVisitor,Enumeration * const )) canVisitEnumeration;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const )) preVisitEnumeration;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool)) preVisitEnumerationValue;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool)) visitEnumerationValue;
    Func DAS_COMMENT((smart_ptr_raw<Enumeration>,ast::AstVisitor,smart_ptr_raw<Enumeration> const )) visitEnumeration;
    Func DAS_COMMENT((bool,ast::AstVisitor,Structure * const )) canVisitStructure;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const )) preVisitStructure;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool)) preVisitStructureField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool)) visitStructureField;
    Func DAS_COMMENT((smart_ptr_raw<Structure>,ast::AstVisitor,smart_ptr_raw<Structure> const )) visitStructure;
    Func DAS_COMMENT((bool,ast::AstVisitor,Function * const )) canVisitFunction;
    Func DAS_COMMENT((bool,ast::AstVisitor,Function * const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) canVisitFunctionArgumentInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const )) preVisitFunction;
    Func DAS_COMMENT((smart_ptr_raw<Function>,ast::AstVisitor,smart_ptr_raw<Function> const )) visitFunction;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool)) preVisitFunctionArgument;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool)) visitFunctionArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitFunctionArgumentInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitFunctionArgumentInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const )) preVisitFunctionBody;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const )) visitFunctionBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Expression> const )) preVisitExpression;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Expression> const )) visitExpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) preVisitExprBlock;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) visitExprBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool)) preVisitExprBlockArgument;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool)) visitExprBlockArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitExprBlockArgumentInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitExprBlockArgumentInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) preVisitExprBlockExpression;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) visitExprBlockExpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) preVisitExprBlockFinal;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const )) visitExprBlockFinal;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) preVisitExprBlockFinalExpression;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const )) visitExprBlockFinalExpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const )) preVisitExprLet;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const )) visitExprLet;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool)) preVisitExprLetVariable;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool)) visitExprLetVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitExprLetVariableInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitExprLetVariableInit;
    Func DAS_COMMENT((bool,ast::AstVisitor,Variable * const )) canVisitGlobalVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) preVisitGlobalLet;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const )) visitGlobalLet;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool)) preVisitGlobalLetVariable;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool)) visitGlobalLetVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) preVisitGlobalLetVariableInit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const )) visitGlobalLetVariableInit;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const )) preVisitExprStringBuilder;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const )) visitExprStringBuilder;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprStringBuilderElement;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool)) visitExprStringBuilderElement;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const )) preVisitExprNew;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const )) visitExprNew;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprNewArgument;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool)) visitExprNewArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const )) preVisitExprNamedCall;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const )) visitExprNamedCall;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool)) preVisitExprNamedCallArgument;
    Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool)) visitExprNamedCallArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const )) preVisitExprLooksLikeCall;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const )) visitExprLooksLikeCall;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) canVisitLooksLikeCallArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprLooksLikeCallArgument;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) visitExprLooksLikeCallArgument;
    Func DAS_COMMENT((bool,ast::AstVisitor,ExprCall * const )) canVisitCall;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const )) preVisitExprCall;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const )) visitExprCall;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprCallArgument;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool)) visitExprCallArgument;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const )) preVisitExprNullCoalescing;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const )) visitExprNullCoalescing;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const ,smart_ptr_raw<Expression> const )) preVisitExprNullCoalescingDefault;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const )) preVisitExprAt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAt> const )) visitExprAt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const )) preVisitExprAtIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const )) preVisitExprSafeAt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const )) visitExprSafeAt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const )) preVisitExprSafeAtIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const )) preVisitExprIs;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIs> const )) visitExprIs;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const ,smart_ptr_raw<TypeDecl> const )) preVisitExprIsType;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const )) preVisitExprOp2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp2> const )) visitExprOp2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const ,smart_ptr_raw<Expression> const )) preVisitExprOp2Right;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const )) preVisitExprOp3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp3> const )) visitExprOp3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const )) preVisitExprOp3Left;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const )) preVisitExprOp3Right;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprCopy> const )) isRightFirstExprCopy;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const )) preVisitExprCopy;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCopy> const )) visitExprCopy;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ,smart_ptr_raw<Expression> const )) preVisitExprCopyRight;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMove> const )) isRightFirstExprMove;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const )) preVisitExprMove;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMove> const )) visitExprMove;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const ,smart_ptr_raw<Expression> const )) preVisitExprMoveRight;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprClone> const )) isRightFirstExprClone;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const )) preVisitExprClone;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprClone> const )) visitExprClone;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const ,smart_ptr_raw<Expression> const )) preVisitExprCloneRight;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprAssume> const )) canVisitWithAliasSubexpression;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssume> const )) preVisitExprAssume;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssume> const )) visitExprAssume;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const )) preVisitExprWith;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWith> const )) visitExprWith;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const ,smart_ptr_raw<Expression> const )) preVisitExprWithBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const )) preVisitExprWhile;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWhile> const )) visitExprWhile;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const ,smart_ptr_raw<Expression> const )) preVisitExprWhileBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const )) preVisitExprTryCatch;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const )) visitExprTryCatch;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const ,smart_ptr_raw<Expression> const )) preVisitExprTryCatchCatch;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const )) preVisitExprIfThenElse;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const )) visitExprIfThenElse;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const )) preVisitExprIfThenElseIfBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const )) preVisitExprIfThenElseElseBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) preVisitExprFor;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) visitExprFor;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool)) preVisitExprForVariable;
    Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool)) visitExprForVariable;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool)) preVisitExprForSource;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool)) visitExprForSource;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) preVisitExprForStack;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const )) preVisitExprForBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const )) preVisitExprMakeVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const )) visitExprMakeVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) preVisitExprMakeVariantField;
    Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) visitExprMakeVariantField;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) canVisitMakeStructBody;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) canVisitMakeStructBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) preVisitExprMakeStruct;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) visitExprMakeStruct;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool)) preVisitExprMakeStructIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool)) visitExprMakeStructIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) preVisitExprMakeStructField;
    Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool)) visitExprMakeStructField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) preVisitMakeStructureBlock;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) visitMakeStructureBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const )) preVisitExprMakeArray;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const )) visitExprMakeArray;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) preVisitExprMakeArrayIndex;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) visitExprMakeArrayIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const )) preVisitExprMakeTuple;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const )) visitExprMakeTuple;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) preVisitExprMakeTupleIndex;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool)) visitExprMakeTupleIndex;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const )) preVisitExprArrayComprehension;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const )) visitExprArrayComprehension;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const )) preVisitExprArrayComprehensionSubexpr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const )) preVisitExprArrayComprehensionWhere;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const )) preVisitExprTypeInfo;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const )) visitExprTypeInfo;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const )) preVisitExprPtr2Ref;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const )) visitExprPtr2Ref;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLabel> const )) preVisitExprLabel;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLabel> const )) visitExprLabel;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprGoto> const )) preVisitExprGoto;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprGoto> const )) visitExprGoto;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const )) preVisitExprRef2Value;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const )) visitExprRef2Value;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const )) preVisitExprRef2Ptr;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const )) visitExprRef2Ptr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAddr> const )) preVisitExprAddr;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAddr> const )) visitExprAddr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssert> const )) preVisitExprAssert;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssert> const )) visitExprAssert;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const )) preVisitExprStaticAssert;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const )) visitExprStaticAssert;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprQuote> const )) preVisitExprQuote;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprQuote> const )) visitExprQuote;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDebug> const )) preVisitExprDebug;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDebug> const )) visitExprDebug;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const )) preVisitExprInvoke;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const )) visitExprInvoke;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprErase> const )) preVisitExprErase;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprErase> const )) visitExprErase;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const )) preVisitExprSetInsert;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const )) visitExprSetInsert;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFind> const )) preVisitExprFind;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFind> const )) visitExprFind;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const )) preVisitExprKeyExists;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const )) visitExprKeyExists;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAscend> const )) preVisitExprAscend;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAscend> const )) visitExprAscend;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCast> const )) preVisitExprCast;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCast> const )) visitExprCast;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const )) preVisitExprDelete;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDelete> const )) visitExprDelete;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprVar> const )) preVisitExprVar;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprVar> const )) visitExprVar;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const )) preVisitExprTag;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const ,smart_ptr_raw<Expression> const )) preVisitExprTagValue;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTag> const )) visitExprTag;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprField> const )) preVisitExprField;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprField> const )) visitExprField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const )) preVisitExprSafeField;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const )) visitExprSafeField;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const )) preVisitExprSwizzle;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const )) visitExprSwizzle;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const )) preVisitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const )) preVisitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const )) preVisitExprSafeAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp1> const )) preVisitExprOp1;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp1> const )) visitExprOp1;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReturn> const )) preVisitExprReturn;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReturn> const )) visitExprReturn;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprYield> const )) preVisitExprYield;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprYield> const )) visitExprYield;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBreak> const )) preVisitExprBreak;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBreak> const )) visitExprBreak;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprContinue> const )) preVisitExprContinue;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprContinue> const )) visitExprContinue;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const )) canVisitMakeBlockBody;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const )) preVisitExprMakeBlock;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const )) visitExprMakeBlock;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const )) preVisitExprMakeGenerator;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const )) visitExprMakeGenerator;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const )) preVisitExprMemZero;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const )) visitExprMemZero;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConst> const )) preVisitExprConst;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConst> const )) visitExprConst;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const )) preVisitExprConstPtr;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const )) visitExprConstPtr;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const )) preVisitExprConstEnumeration;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const )) visitExprConstEnumeration;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const )) preVisitExprConstBitfield;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const )) visitExprConstBitfield;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const )) preVisitExprConstInt8;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const )) visitExprConstInt8;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const )) preVisitExprConstInt16;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const )) visitExprConstInt16;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const )) preVisitExprConstInt64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const )) visitExprConstInt64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const )) preVisitExprConstInt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const )) visitExprConstInt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const )) preVisitExprConstInt2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const )) visitExprConstInt2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const )) preVisitExprConstInt3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const )) visitExprConstInt3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const )) preVisitExprConstInt4;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const )) visitExprConstInt4;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const )) preVisitExprConstUInt8;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const )) visitExprConstUInt8;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const )) preVisitExprConstUInt16;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const )) visitExprConstUInt16;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const )) preVisitExprConstUInt64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const )) visitExprConstUInt64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const )) preVisitExprConstUInt;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const )) visitExprConstUInt;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const )) preVisitExprConstUInt2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const )) visitExprConstUInt2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const )) preVisitExprConstUInt3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const )) visitExprConstUInt3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const )) preVisitExprConstUInt4;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const )) visitExprConstUInt4;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const )) preVisitExprConstRange;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const )) visitExprConstRange;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const )) preVisitExprConstURange;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const )) visitExprConstURange;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const )) preVisitExprConstRange64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const )) visitExprConstRange64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const )) preVisitExprConstURange64;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const )) visitExprConstURange64;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const )) preVisitExprConstBool;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const )) visitExprConstBool;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const )) preVisitExprConstFloat;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const )) visitExprConstFloat;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const )) preVisitExprConstFloat2;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const )) visitExprConstFloat2;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const )) preVisitExprConstFloat3;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const )) visitExprConstFloat3;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const )) preVisitExprConstFloat4;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const )) visitExprConstFloat4;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstString> const )) preVisitExprConstString;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstString> const )) visitExprConstString;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const )) preVisitExprConstDouble;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const )) visitExprConstDouble;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const )) preVisitExprFakeContext;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const )) visitExprFakeContext;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const )) preVisitExprFakeLineInfo;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const )) visitExprFakeLineInfo;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReader> const )) preVisitExprReader;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReader> const )) visitExprReader;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const )) preVisitExprUnsafe;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const )) visitExprUnsafe;
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const )) preVisitExprCallMacro;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const )) visitExprCallMacro;
};
static_assert(sizeof(ValidateShaderVisitor)==2424,"structure size mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitProgram)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitProgram)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitProgramBody)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitModule)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitModule)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprTypeDecl)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprTypeDecl)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitTypeDecl)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitTypeDecl)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitAlias)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitAlias)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitEnumeration)==104,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitEnumeration)==112,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitEnumerationValue)==120,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitEnumerationValue)==128,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitEnumeration)==136,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitStructure)==144,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitStructure)==152,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitStructureField)==160,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitStructureField)==168,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitStructure)==176,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitFunction)==184,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitFunctionArgumentInit)==192,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitFunction)==200,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitFunction)==208,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitFunctionArgument)==216,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitFunctionArgument)==224,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitFunctionArgumentInit)==232,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitFunctionArgumentInit)==240,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitFunctionBody)==248,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitFunctionBody)==256,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExpression)==264,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExpression)==272,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprBlock)==280,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprBlock)==288,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprBlockArgument)==296,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprBlockArgument)==304,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprBlockArgumentInit)==312,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprBlockArgumentInit)==320,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprBlockExpression)==328,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprBlockExpression)==336,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprBlockFinal)==344,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprBlockFinal)==352,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprBlockFinalExpression)==360,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprBlockFinalExpression)==368,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprLet)==376,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprLet)==384,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprLetVariable)==392,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprLetVariable)==400,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprLetVariableInit)==408,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprLetVariableInit)==416,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitGlobalVariable)==424,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitGlobalLet)==432,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitGlobalLet)==440,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitGlobalLetVariable)==448,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitGlobalLetVariable)==456,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitGlobalLetVariableInit)==464,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitGlobalLetVariableInit)==472,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprStringBuilder)==480,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprStringBuilder)==488,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprStringBuilderElement)==496,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprStringBuilderElement)==504,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprNew)==512,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprNew)==520,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprNewArgument)==528,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprNewArgument)==536,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprNamedCall)==544,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprNamedCall)==552,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprNamedCallArgument)==560,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprNamedCallArgument)==568,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprLooksLikeCall)==576,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprLooksLikeCall)==584,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitLooksLikeCallArgument)==592,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprLooksLikeCallArgument)==600,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprLooksLikeCallArgument)==608,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitCall)==616,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprCall)==624,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprCall)==632,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprCallArgument)==640,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprCallArgument)==648,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprNullCoalescing)==656,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprNullCoalescing)==664,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprNullCoalescingDefault)==672,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprAt)==680,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprAt)==688,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprAtIndex)==696,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprSafeAt)==704,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprSafeAt)==712,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprSafeAtIndex)==720,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprIs)==728,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprIs)==736,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprIsType)==744,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprOp2)==752,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprOp2)==760,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprOp2Right)==768,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprOp3)==776,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprOp3)==784,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprOp3Left)==792,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprOp3Right)==800,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,isRightFirstExprCopy)==808,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprCopy)==816,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprCopy)==824,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprCopyRight)==832,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,isRightFirstExprMove)==840,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMove)==848,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMove)==856,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMoveRight)==864,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,isRightFirstExprClone)==872,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprClone)==880,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprClone)==888,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprCloneRight)==896,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitWithAliasSubexpression)==904,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprAssume)==912,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprAssume)==920,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprWith)==928,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprWith)==936,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprWithBody)==944,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprWhile)==952,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprWhile)==960,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprWhileBody)==968,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprTryCatch)==976,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprTryCatch)==984,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprTryCatchCatch)==992,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprIfThenElse)==1000,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprIfThenElse)==1008,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprIfThenElseIfBlock)==1016,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprIfThenElseElseBlock)==1024,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprFor)==1032,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprFor)==1040,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprForVariable)==1048,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprForVariable)==1056,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprForSource)==1064,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprForSource)==1072,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprForStack)==1080,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprForBody)==1088,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeVariant)==1096,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeVariant)==1104,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeVariantField)==1112,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeVariantField)==1120,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitMakeStructBody)==1128,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitMakeStructBlock)==1136,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeStruct)==1144,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeStruct)==1152,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeStructIndex)==1160,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeStructIndex)==1168,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeStructField)==1176,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeStructField)==1184,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitMakeStructureBlock)==1192,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitMakeStructureBlock)==1200,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeArray)==1208,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeArray)==1216,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeArrayIndex)==1224,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeArrayIndex)==1232,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeTuple)==1240,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeTuple)==1248,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeTupleIndex)==1256,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeTupleIndex)==1264,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprArrayComprehension)==1272,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprArrayComprehension)==1280,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprArrayComprehensionSubexpr)==1288,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprArrayComprehensionWhere)==1296,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprTypeInfo)==1304,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprTypeInfo)==1312,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprPtr2Ref)==1320,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprPtr2Ref)==1328,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprLabel)==1336,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprLabel)==1344,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprGoto)==1352,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprGoto)==1360,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprRef2Value)==1368,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprRef2Value)==1376,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprRef2Ptr)==1384,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprRef2Ptr)==1392,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprAddr)==1400,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprAddr)==1408,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprAssert)==1416,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprAssert)==1424,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprStaticAssert)==1432,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprStaticAssert)==1440,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprQuote)==1448,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprQuote)==1456,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprDebug)==1464,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprDebug)==1472,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprInvoke)==1480,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprInvoke)==1488,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprErase)==1496,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprErase)==1504,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprSetInsert)==1512,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprSetInsert)==1520,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprFind)==1528,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprFind)==1536,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprKeyExists)==1544,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprKeyExists)==1552,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprAscend)==1560,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprAscend)==1568,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprCast)==1576,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprCast)==1584,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprDelete)==1592,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprDelete)==1600,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprVar)==1608,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprVar)==1616,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprTag)==1624,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprTagValue)==1632,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprTag)==1640,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprField)==1648,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprField)==1656,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprSafeField)==1664,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprSafeField)==1672,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprSwizzle)==1680,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprSwizzle)==1688,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprIsVariant)==1696,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprIsVariant)==1704,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprAsVariant)==1712,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprAsVariant)==1720,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprSafeAsVariant)==1728,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprSafeAsVariant)==1736,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprOp1)==1744,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprOp1)==1752,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprReturn)==1760,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprReturn)==1768,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprYield)==1776,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprYield)==1784,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprBreak)==1792,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprBreak)==1800,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprContinue)==1808,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprContinue)==1816,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,canVisitMakeBlockBody)==1824,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeBlock)==1832,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeBlock)==1840,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMakeGenerator)==1848,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMakeGenerator)==1856,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprMemZero)==1864,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprMemZero)==1872,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConst)==1880,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConst)==1888,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstPtr)==1896,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstPtr)==1904,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstEnumeration)==1912,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstEnumeration)==1920,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstBitfield)==1928,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstBitfield)==1936,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstInt8)==1944,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstInt8)==1952,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstInt16)==1960,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstInt16)==1968,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstInt64)==1976,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstInt64)==1984,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstInt)==1992,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstInt)==2000,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstInt2)==2008,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstInt2)==2016,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstInt3)==2024,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstInt3)==2032,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstInt4)==2040,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstInt4)==2048,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstUInt8)==2056,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstUInt8)==2064,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstUInt16)==2072,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstUInt16)==2080,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstUInt64)==2088,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstUInt64)==2096,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstUInt)==2104,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstUInt)==2112,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstUInt2)==2120,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstUInt2)==2128,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstUInt3)==2136,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstUInt3)==2144,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstUInt4)==2152,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstUInt4)==2160,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstRange)==2168,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstRange)==2176,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstURange)==2184,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstURange)==2192,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstRange64)==2200,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstRange64)==2208,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstURange64)==2216,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstURange64)==2224,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstBool)==2232,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstBool)==2240,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstFloat)==2248,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstFloat)==2256,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstFloat2)==2264,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstFloat2)==2272,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstFloat3)==2280,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstFloat3)==2288,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstFloat4)==2296,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstFloat4)==2304,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstString)==2312,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstString)==2320,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprConstDouble)==2328,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprConstDouble)==2336,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprFakeContext)==2344,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprFakeContext)==2352,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprFakeLineInfo)==2360,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprFakeLineInfo)==2368,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprReader)==2376,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprReader)==2384,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprUnsafe)==2392,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprUnsafe)==2400,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,preVisitExprCallMacro)==2408,"structure field offset mismatch with DAS");
static_assert(offsetof(ValidateShaderVisitor,visitExprCallMacro)==2416,"structure field offset mismatch with DAS");
}
namespace validate_code {

struct JIT_LLVM {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstSimulateMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstSimulateMacro,Program * const ,Context * const )) preSimulate;
    Func DAS_COMMENT((bool,ast::AstSimulateMacro,Program * const ,Context * const )) simulate;
    Func DAS_COMMENT((void,validate_code::JIT_LLVM,Program * const ,Context *)) lint_module;
};
static_assert(sizeof(JIT_LLVM)==40,"structure size mismatch with DAS");
static_assert(offsetof(JIT_LLVM,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(JIT_LLVM,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(JIT_LLVM,preSimulate)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(JIT_LLVM,simulate)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(JIT_LLVM,lint_module)==32,"structure field offset mismatch with DAS");
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_395c35bc9503cbdf ( Context * __context__, validate_code::VerifyCompletion const  & __cl_rename_at_116_2 );
inline void _FuncbuiltinTickpushTick10769833213962245646_3d49d5ae99d6fd68 ( Context * __context__, TArray<ast::AstSimulateMacro *> & __Arr_rename_at_181_3, ast::AstSimulateMacro * __value_rename_at_181_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6ba608af13ad58de ( Context * __context__, validate_code::JIT_LLVM const  & __cl_rename_at_116_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_92e2ae527e156a55 ( Context * __context__, validate_code::ValidateCompletionVisitor const  & __cl_rename_at_116_6 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_795ae830a297a556 ( Context * __context__, validate_code::ValidateShaderVisitor const  & __cl_rename_at_116_7 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_f84c987f8921e410 ( Context * __context__, TArray<Function *> & __Arr_rename_at_68_8, int32_t __newSize_rename_at_68_9 );
inline int32_t _FuncbuiltinTickfind_indexTick791716935952699529_91bcfa77f44fc12a ( Context * __context__, TArray<Function *> const  & __arr_rename_at_1650_10, Function * const  __key_rename_at_1650_11 );
inline void _FuncbuiltinTickinsertTick10959621454228962049_c11818675b11b85 ( Context * __context__, TTable<Function *,void> & __Tab_rename_at_896_13, Function * const  __at_rename_at_896_14 );
inline void _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50 ( Context * __context__, TArray<Function *> & __Arr_rename_at_181_15, Function * __value_rename_at_181_16 );
inline void _FuncbuiltinTickpopTick1161079256290593740_65c208b40850450a ( Context * __context__, TArray<Function *> & __Arr_rename_at_132_17 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_b33d375e46ad04f4 ( Context * __context__, TTable<Function *,void> const  & __Tab_rename_at_1015_18, Function * const  __at_rename_at_1015_19 );
inline Function * _FuncbuiltinTickget_ptrTick5807679485210906136_4bc3bbefec247e32 ( Context * __context__, smart_ptr_raw<Function> __src_rename_at_1733_20 );
inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_f1737f09fda7df67 ( Context * __context__, validate_code::ValidateCompletionVisitor const  & __someClass_rename_at_675_21 );
inline Sequence DAS_COMMENT((Function *)) _FuncbuiltinTickkeysTick2205854368403803976_fc2951f6f9734e1f ( Context * __context__, TTable<Function *,void> const  & __a_rename_at_1129_24 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_37_26, bool __extra_rename_at_37_27, bool __contracts_rename_at_37_28, bool __modules_rename_at_37_29 );
inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_95647a629b0d5183 ( Context * __context__, validate_code::ValidateShaderVisitor const  & __someClass_rename_at_675_30 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_395c35bc9503cbdf ( Context * __context__, validate_code::VerifyCompletion const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_3d49d5ae99d6fd68 ( Context * __context__, TArray<ast::AstSimulateMacro *> &  __Arr_rename_at_181_3, ast::AstSimulateMacro * __value_rename_at_181_4 )
{
    das_copy(__Arr_rename_at_181_3(builtin_array_push_back(das_arg<TArray<ast::AstSimulateMacro *>>::pass(__Arr_rename_at_181_3),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_4);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6ba608af13ad58de ( Context * __context__, validate_code::JIT_LLVM const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_92e2ae527e156a55 ( Context * __context__, validate_code::ValidateCompletionVisitor const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_795ae830a297a556 ( Context * __context__, validate_code::ValidateShaderVisitor const  &  __cl_rename_at_116_7 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_7.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_f84c987f8921e410 ( Context * __context__, TArray<Function *> &  __Arr_rename_at_68_8, int32_t __newSize_rename_at_68_9 )
{
    builtin_array_resize(das_arg<TArray<Function *>>::pass(__Arr_rename_at_68_8),__newSize_rename_at_68_9,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTickfind_indexTick791716935952699529_91bcfa77f44fc12a ( Context * __context__, TArray<Function *> const  &  __arr_rename_at_1650_10, Function * const  __key_rename_at_1650_11 )
{
    {
        bool __need_loop_1651 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(builtin_array_size(__arr_rename_at_1650_10)));
        int32_t __i_rename_at_1651_12;
        __need_loop_1651 = __i_iterator.first(__context__,(__i_rename_at_1651_12)) && __need_loop_1651;
        for ( ; __need_loop_1651 ; __need_loop_1651 = __i_iterator.next(__context__,(__i_rename_at_1651_12)) )
        {
            if ( __arr_rename_at_1650_10(__i_rename_at_1651_12,__context__) == __key_rename_at_1650_11 )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1651_12);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1651_12));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline void _FuncbuiltinTickinsertTick10959621454228962049_c11818675b11b85 ( Context * __context__, TTable<Function *,void> &  __Tab_rename_at_896_13, Function * const  __at_rename_at_896_14 )
{
    __builtin_table_set_insert(__context__,__Tab_rename_at_896_13,__at_rename_at_896_14);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50 ( Context * __context__, TArray<Function *> &  __Arr_rename_at_181_15, Function * __value_rename_at_181_16 )
{
    das_copy(__Arr_rename_at_181_15(builtin_array_push_back(das_arg<TArray<Function *>>::pass(__Arr_rename_at_181_15),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_16);
}

inline void _FuncbuiltinTickpopTick1161079256290593740_65c208b40850450a ( Context * __context__, TArray<Function *> &  __Arr_rename_at_132_17 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_f84c987f8921e410(__context__,das_arg<TArray<Function *>>::pass(__Arr_rename_at_132_17),builtin_array_size(das_arg<TArray<Function *>>::pass(__Arr_rename_at_132_17)) - 1);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_b33d375e46ad04f4 ( Context * __context__, TTable<Function *,void> const  &  __Tab_rename_at_1015_18, Function * const  __at_rename_at_1015_19 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1015_18,__at_rename_at_1015_19));
}

inline Function * _FuncbuiltinTickget_ptrTick5807679485210906136_4bc3bbefec247e32 ( Context * __context__, smart_ptr_raw<Function> __src_rename_at_1733_20 )
{
    return das_auto_cast<Function *>::cast(das_cast<Function *>::cast(__src_rename_at_1733_20));
}

inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_f1737f09fda7df67 ( Context * __context__, validate_code::ValidateCompletionVisitor const  &  __someClass_rename_at_675_21 )
{
    validate_code::ValidateCompletionVisitor const  * __classPtr_rename_at_678_22 = ((validate_code::ValidateCompletionVisitor const  *)das_ref(__context__,__someClass_rename_at_675_21));
    StructInfo const  * __classInfo_rename_at_679_23 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_92e2ae527e156a55(__context__,__someClass_rename_at_675_21));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<VisitorAdapter>>::cast(makeVisitor(das_auto_cast<void * const >::cast(__classPtr_rename_at_678_22),__classInfo_rename_at_679_23,__context__));
}

inline Sequence DAS_COMMENT((Function *)) _FuncbuiltinTickkeysTick2205854368403803976_fc2951f6f9734e1f ( Context * __context__, TTable<Function *,void> const  &  __a_rename_at_1129_24 )
{
    Sequence DAS_COMMENT((Function *)) __it_rename_at_1130_25; das_zero(__it_rename_at_1130_25);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((Function *))>::pass(__it_rename_at_1130_25),__a_rename_at_1129_24,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((Function *))>::cast(__it_rename_at_1130_25);
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_37_26, bool __extra_rename_at_37_27, bool __contracts_rename_at_37_28, bool __modules_rename_at_37_29 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_37_26,__extra_rename_at_37_27,__contracts_rename_at_37_28,__modules_rename_at_37_29,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_95647a629b0d5183 ( Context * __context__, validate_code::ValidateShaderVisitor const  &  __someClass_rename_at_675_30 )
{
    validate_code::ValidateShaderVisitor const  * __classPtr_rename_at_678_31 = ((validate_code::ValidateShaderVisitor const  *)das_ref(__context__,__someClass_rename_at_675_30));
    StructInfo const  * __classInfo_rename_at_679_32 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_795ae830a297a556(__context__,__someClass_rename_at_675_30));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<VisitorAdapter>>::cast(makeVisitor(das_auto_cast<void * const >::cast(__classPtr_rename_at_678_31),__classInfo_rename_at_679_32,__context__));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x17cd053e5d46f3ad] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & , ast::AstFunctionAnnotation *  ),&_FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a>>();
    };
    aotLib[0x2c1e04dc063043e7] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_395c35bc9503cbdf
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, validate_code::VerifyCompletion const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_395c35bc9503cbdf>>();
    };
    aotLib[0x8663006b25a5d2a0] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_3d49d5ae99d6fd68
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstSimulateMacro *> & , ast::AstSimulateMacro *  ),&_FuncbuiltinTickpushTick10769833213962245646_3d49d5ae99d6fd68>>();
    };
    aotLib[0x1f05253b4a723b1b] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_6ba608af13ad58de
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, validate_code::JIT_LLVM const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_6ba608af13ad58de>>();
    };
    aotLib[0xc192d1b45ea33d82] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_92e2ae527e156a55
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, validate_code::ValidateCompletionVisitor const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_92e2ae527e156a55>>();
    };
    aotLib[0xcec27fd84f41f73e] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_795ae830a297a556
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, validate_code::ValidateShaderVisitor const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_795ae830a297a556>>();
    };
    aotLib[0x207d22df43522200] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickresizeTick4811697762258667383_f84c987f8921e410
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<Function *> & , int32_t  ),&_FuncbuiltinTickresizeTick4811697762258667383_f84c987f8921e410>>();
    };
    aotLib[0xe9a30a2c01d0c71b] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickfind_indexTick791716935952699529_91bcfa77f44fc12a
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, TArray<Function *> const  & , Function * const   ),&_FuncbuiltinTickfind_indexTick791716935952699529_91bcfa77f44fc12a>>();
    };
    aotLib[0xbe378df9a42ae36c] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickinsertTick10959621454228962049_c11818675b11b85
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TTable<Function *,void> & , Function * const   ),&_FuncbuiltinTickinsertTick10959621454228962049_c11818675b11b85>>();
    };
    aotLib[0x15ac5ce5ae27e745] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<Function *> & , Function *  ),&_FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50>>();
    };
    aotLib[0x12d79e660cb8485] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpopTick1161079256290593740_65c208b40850450a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<Function *> &  ),&_FuncbuiltinTickpopTick1161079256290593740_65c208b40850450a>>();
    };
    aotLib[0x33ec1c19d93483a] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickkey_existsTick16808803843923989214_b33d375e46ad04f4
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, TTable<Function *,void> const  & , Function * const   ),&_FuncbuiltinTickkey_existsTick16808803843923989214_b33d375e46ad04f4>>();
    };
    aotLib[0x2581bc46befbbcd0] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick5807679485210906136_4bc3bbefec247e32
        return ctx.code->makeNode<SimNode_Aot<Function * (*) ( Context * __context__, smart_ptr_raw<Function>  ),&_FuncbuiltinTickget_ptrTick5807679485210906136_4bc3bbefec247e32>>();
    };
    aotLib[0xbf0c7adc78cbd5c3] = +[](Context & ctx) -> SimNode* { // _FuncastTickmake_visitorTick897644165917210720_f1737f09fda7df67
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<VisitorAdapter> (*) ( Context * __context__, validate_code::ValidateCompletionVisitor const  &  ),&_FuncastTickmake_visitorTick897644165917210720_f1737f09fda7df67>>();
    };
    aotLib[0x73c98109c6352aa1] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickkeysTick2205854368403803976_fc2951f6f9734e1f
        return ctx.code->makeNode<SimNode_AotCMRES<Sequence DAS_COMMENT((Function *)) (*) ( Context * __context__, TTable<Function *,void> const  &  ),&_FuncbuiltinTickkeysTick2205854368403803976_fc2951f6f9734e1f>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* { // _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const  , bool , bool , bool  ),&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    aotLib[0x5ccb4e0b126a3f4c] = +[](Context & ctx) -> SimNode* { // _FuncastTickmake_visitorTick897644165917210720_95647a629b0d5183
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<VisitorAdapter> (*) ( Context * __context__, validate_code::ValidateShaderVisitor const  &  ),&_FuncastTickmake_visitorTick897644165917210720_95647a629b0d5183>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_6360572917569013088
AotListBase impl_aot_validate_code(_anon_6360572917569013088::registerAotFunctions);
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
