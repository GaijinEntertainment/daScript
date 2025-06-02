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
namespace _anon_482672747374343583 {

namespace macro_boost { struct MacroVerifyMacro; };
namespace macro_boost { struct CaptureBlock; };
namespace macro_boost { struct CapturedVariable; };
namespace macro_boost { struct ColletFinally; };
namespace macro_boost { struct ColletLabels; };
namespace macro_boost { struct ReturnSkipLockcheck; };
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<Structure> const )) canVisitStructureFieldInit;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) canVisitExprLooksLikeCallArgument;
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
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ,smart_ptr_raw<Expression> const )) preVisitExprSafeAtIndex;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) canVisitExprMakeStructBody;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) canVisitExprMakeStructBlock;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ,smart_ptr_raw<Expression> const )) canVisitExprTypeInfo;
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
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ,smart_ptr_raw<Expression> const )) preVisitExprDeleteSizeExpression;
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
namespace macro_boost {

struct MacroVerifyMacro {
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
namespace macro_boost {

struct CaptureBlock {
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<Structure> const )) canVisitStructureFieldInit;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) canVisitExprLooksLikeCallArgument;
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
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ,smart_ptr_raw<Expression> const )) preVisitExprSafeAtIndex;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) canVisitExprMakeStructBody;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) canVisitExprMakeStructBlock;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ,smart_ptr_raw<Expression> const )) canVisitExprTypeInfo;
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
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ,smart_ptr_raw<Expression> const )) preVisitExprDeleteSizeExpression;
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
    TTable<Variable *,void> scope;
    TTable<Variable *,ExprVar *> captured;
};
}
namespace macro_boost {

struct CapturedVariable {
    Variable * variable;
    ExprVar * expression;
    bool eref;
};
}
namespace macro_boost {

struct ColletFinally {
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<Structure> const )) canVisitStructureFieldInit;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) canVisitExprLooksLikeCallArgument;
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
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ,smart_ptr_raw<Expression> const )) preVisitExprSafeAtIndex;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) canVisitExprMakeStructBody;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) canVisitExprMakeStructBlock;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ,smart_ptr_raw<Expression> const )) canVisitExprTypeInfo;
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
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ,smart_ptr_raw<Expression> const )) preVisitExprDeleteSizeExpression;
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
    TTable<ExprBlock *,void> blocks;
    bool alwaysFor;
};
}
namespace macro_boost {

struct ColletLabels {
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<Structure> const )) canVisitStructureFieldInit;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool)) canVisitExprLooksLikeCallArgument;
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
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ,smart_ptr_raw<Expression> const )) preVisitExprSafeAtIndex;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const )) canVisitExprMakeStructBody;
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const )) canVisitExprMakeStructBlock;
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
    Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ,smart_ptr_raw<Expression> const )) canVisitExprTypeInfo;
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
    Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ,smart_ptr_raw<Expression> const )) preVisitExprDeleteSizeExpression;
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
    TTable<int32_t,void> labels;
};
}
namespace macro_boost {

struct ReturnSkipLockcheck {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
}
extern StructInfo __struct_info__2668415385194db3;
extern TypeInfo __type_info__eca8ada468f4bde9;
extern TypeInfo __type_info__f1e180ed97abebcd;
extern TypeInfo __type_info__4a2b6c4fa080723;
extern TypeInfo __type_info__5e85fe99b916c6d6;
extern TypeInfo __type_info__71c84a7f531ca5bb;
extern TypeInfo __type_info__811c0b03f452ec1;
extern TypeInfo __type_info__ccd32e474e9a33eb;
extern TypeInfo __type_info__baf2de01229b73a2;
extern TypeInfo __type_info__af63e44c8601fa24;
extern VarInfo __var_info__c60c5f46299835fc;
extern VarInfo __var_info__245ccbce6b5ab224;
extern VarInfo __var_info__45766677ab541343;

VarInfo __struct_info__2668415385194db3_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ccd32e474e9a33eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x45766677ab541343), "variable", offsetof(macro_boost::CapturedVariable,variable), 1 };
VarInfo __struct_info__2668415385194db3_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__811c0b03f452ec1, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x245ccbce6b5ab224), "expression", offsetof(macro_boost::CapturedVariable,expression), 3 };
VarInfo __struct_info__2668415385194db3_field_2 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xc60c5f46299835fc), "eref", offsetof(macro_boost::CapturedVariable,eref), 0 };
VarInfo * __struct_info__2668415385194db3_fields[3] =  { &__struct_info__2668415385194db3_field_0, &__struct_info__2668415385194db3_field_1, &__struct_info__2668415385194db3_field_2 };
StructInfo __struct_info__2668415385194db3 = {"CapturedVariable", "macro_boost", 12, __struct_info__2668415385194db3_fields, 3, 24, UINT64_C(0x0), nullptr, UINT64_C(0x2668415385194db3), 0 };
TypeInfo __type_info__eca8ada468f4bde9 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__71c84a7f531ca5bb, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xeca8ada468f4bde9) };
TypeInfo __type_info__f1e180ed97abebcd = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__eca8ada468f4bde9, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xf1e180ed97abebcd) };
TypeInfo __type_info__4a2b6c4fa080723 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__baf2de01229b73a2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x4a2b6c4fa080723) };
TypeInfo __type_info__5e85fe99b916c6d6 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x5e85fe99b916c6d6) };
TypeInfo __type_info__71c84a7f531ca5bb = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprBlock"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 256, UINT64_C(0x71c84a7f531ca5bb) };
TypeInfo __type_info__811c0b03f452ec1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprVar"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 128, UINT64_C(0x811c0b03f452ec1) };
TypeInfo __type_info__ccd32e474e9a33eb = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Variable"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 256, UINT64_C(0xccd32e474e9a33eb) };
TypeInfo __type_info__baf2de01229b73a2 = { Type::tStructure, &__struct_info__2668415385194db3, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 24, UINT64_C(0xbaf2de01229b73a2) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__71c84a7f531ca5bb, __type_info__811c0b03f452ec1, __type_info__ccd32e474e9a33eb, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__f1e180ed97abebcd };
TypeInfo * __tinfo_1[1] = { &__type_info__5e85fe99b916c6d6 };
TypeInfo * __tinfo_2[1] = { &__type_info__4a2b6c4fa080723 };

inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1059_0, smart_ptr_raw<Expression> const  __src_rename_at_1059_1 );
inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_2, ast::AstFunctionAnnotation * __value_rename_at_181_3 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7a86d959f98dbf0f ( Context * __context__, macro_boost::MacroVerifyMacro const  & __cl_rename_at_116_4 );
inline void _FuncbuiltinTickpushTick10769833213962245646_e608f61b6dfcd4ce ( Context * __context__, TArray<ast::AstCallMacro *> & __Arr_rename_at_181_5, ast::AstCallMacro * __value_rename_at_181_6 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e48bce725be85082 ( Context * __context__, macro_boost::ReturnSkipLockcheck const  & __cl_rename_at_116_7 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_8852dcfb910d89aa ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_8 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_73667a6caa7920c1 ( Context * __context__, macro_boost::ColletFinally const  & __cl_rename_at_116_10 );
inline TArray<ExprBlock *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8477891fe74e291a ( Context * __context__, TArray<ExprBlock *> & __a_rename_at_50_11 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_57e23274c7352cb ( Context * __context__, macro_boost::CaptureBlock const  & __cl_rename_at_116_12 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4a213f79ff5ed21b ( Context * __context__, macro_boost::ColletLabels const  & __cl_rename_at_116_13 );
inline void _FuncbuiltinTickpushTick10769833213962245646_f815a1708f9fc21b ( Context * __context__, TArray<ExprBlock *> & __Arr_rename_at_181_14, ExprBlock * __value_rename_at_181_15 );
inline TArray<int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5eb52cff2bdd0eb4 ( Context * __context__, TArray<int32_t> & __a_rename_at_50_16 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_5726e2a60ae65060 ( Context * __context__, TTable<int32_t,void> & __a_rename_at_1193_17 );
inline TArray<macro_boost::CapturedVariable> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_4afe0ef37f3d7d46 ( Context * __context__, TArray<macro_boost::CapturedVariable> & __a_rename_at_50_18 );
inline void _FuncbuiltinTickpushTick10769833213962245646_36bc6cba83fc099e ( Context * __context__, TArray<int32_t> & __Arr_rename_at_181_19, int32_t __value_rename_at_181_20 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_62c4a869e90f1282 ( Context * __context__, char * const  __name_rename_at_631_21, macro_boost::MacroVerifyMacro * __someClassPtr_rename_at_631_22 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_7b8f4ca3a41e72f4 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1744_24 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_6c6f6f459d1ea17c ( Context * __context__, TTable<Variable *,void> const  & __Tab_rename_at_1014_26, Variable * const  __at_rename_at_1014_27 );
inline void _FuncbuiltinTickinsertTick10959621454228962049_21162df62a60dfe5 ( Context * __context__, TTable<Variable *,void> & __Tab_rename_at_895_28, Variable * const  __at_rename_at_895_29 );
inline void _FuncbuiltinTickpushTick10769833213962245646_118a5b29d7d59b09 ( Context * __context__, TArray<macro_boost::CapturedVariable> & __Arr_rename_at_181_30, macro_boost::CapturedVariable & __value_rename_at_181_31 );
inline void _FuncbuiltinTickinsertTick10959621454228962049_fe29507865f40410 ( Context * __context__, TTable<ExprBlock *,void> & __Tab_rename_at_895_32, ExprBlock * const  __at_rename_at_895_33 );
inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_a778d918650a9f1b ( Context * __context__, macro_boost::ColletFinally const  & __someClass_rename_at_684_34 );
inline Sequence DAS_COMMENT((ExprBlock *)) _FuncbuiltinTickkeysTick2205854368403803976_95e7a5c5edb9b4fa ( Context * __context__, TTable<ExprBlock *,void> const  & __a_rename_at_1128_37 );
inline void _FuncbuiltinTickinsertTick10959621454228962049_c74ac84ad6ee5a59 ( Context * __context__, TTable<int32_t,void> & __Tab_rename_at_895_39, int32_t __at_rename_at_895_40 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_4ca660e879c50a40 ( Context * __context__, char * const  __name_rename_at_273_41, char * const  __tag_rename_at_273_42, macro_boost::MacroVerifyMacro * __classPtr_rename_at_273_43 );
inline Variable * _FuncbuiltinTickget_ptrTick8468476673553620226_8841038ce28d3387 ( Context * __context__, smart_ptr_raw<Variable> const  __src_rename_at_1726_45 );
inline ExprVar * _FuncbuiltinTickget_ptrTick8468476673553620226_ebc419a3b3c42a05 ( Context * __context__, smart_ptr_raw<ExprVar> const  __src_rename_at_1726_46 );
inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_ee4ed30e25c091a0 ( Context * __context__, macro_boost::CaptureBlock const  & __someClass_rename_at_684_47 );
inline Sequence DAS_COMMENT((Variable *)) _FuncbuiltinTickkeysTick2205854368403803976_63e640287a08ea51 ( Context * __context__, TTable<Variable *,ExprVar *> const  & __a_rename_at_1128_50 );
inline Sequence DAS_COMMENT((ExprVar * &)) _FuncbuiltinTickvaluesTick1351216622833168869_2c23a274fa76e5d2 ( Context * __context__, TTable<Variable *,ExprVar *> & __a_rename_at_1150_52 );
inline ExprBlock * _FuncbuiltinTickget_ptrTick8468476673553620226_b22ed6a178fb73f0 ( Context * __context__, smart_ptr_raw<ExprBlock> const  __src_rename_at_1726_54 );
inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_1a3b715e243e2e09 ( Context * __context__, macro_boost::ColletLabels const  & __someClass_rename_at_684_55 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickkeysTick2205854368403803976_2aeb949e6a3921a5 ( Context * __context__, TTable<int32_t,void> const  & __a_rename_at_1128_58 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1059_0, smart_ptr_raw<Expression> const  __src_rename_at_1059_1 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1059_0),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1059_1),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_2, ast::AstFunctionAnnotation * __value_rename_at_181_3 )
{
    das_copy(__Arr_rename_at_181_2(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_3);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7a86d959f98dbf0f ( Context * __context__, macro_boost::MacroVerifyMacro const  &  __cl_rename_at_116_4 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_4.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e608f61b6dfcd4ce ( Context * __context__, TArray<ast::AstCallMacro *> &  __Arr_rename_at_181_5, ast::AstCallMacro * __value_rename_at_181_6 )
{
    das_copy(__Arr_rename_at_181_5(builtin_array_push_back(das_arg<TArray<ast::AstCallMacro *>>::pass(__Arr_rename_at_181_5),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_6);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e48bce725be85082 ( Context * __context__, macro_boost::ReturnSkipLockcheck const  &  __cl_rename_at_116_7 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_7.__rtti))).getStructType())));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_8852dcfb910d89aa ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_8 )
{
    smart_ptr_raw<Expression> __clone_dest_rename_at_1058_9;das_zero(__clone_dest_rename_at_1058_9);
    clone_912660ab1e667e4b(__context__,__clone_dest_rename_at_1058_9,__clone_src_rename_at_1056_8);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__clone_dest_rename_at_1058_9);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_73667a6caa7920c1 ( Context * __context__, macro_boost::ColletFinally const  &  __cl_rename_at_116_10 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_10.__rtti))).getStructType())));
}

inline TArray<ExprBlock *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8477891fe74e291a ( Context * __context__, TArray<ExprBlock *> &  __a_rename_at_50_11 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<ExprBlock *> &>::from(__a_rename_at_50_11)));
    return das_auto_cast_ref<TArray<ExprBlock *> &>::cast(__a_rename_at_50_11);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_57e23274c7352cb ( Context * __context__, macro_boost::CaptureBlock const  &  __cl_rename_at_116_12 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_12.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4a213f79ff5ed21b ( Context * __context__, macro_boost::ColletLabels const  &  __cl_rename_at_116_13 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_13.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_f815a1708f9fc21b ( Context * __context__, TArray<ExprBlock *> &  __Arr_rename_at_181_14, ExprBlock * __value_rename_at_181_15 )
{
    das_copy(__Arr_rename_at_181_14(builtin_array_push_back(das_arg<TArray<ExprBlock *>>::pass(__Arr_rename_at_181_14),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_15);
}

inline TArray<int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5eb52cff2bdd0eb4 ( Context * __context__, TArray<int32_t> &  __a_rename_at_50_16 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TArray<int32_t> &>::from(__a_rename_at_50_16)));
    return das_auto_cast_ref<TArray<int32_t> &>::cast(__a_rename_at_50_16);
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_5726e2a60ae65060 ( Context * __context__, TTable<int32_t,void> &  __a_rename_at_1193_17 )
{
    builtin_table_free(das_arg<TTable<int32_t,void>>::pass(__a_rename_at_1193_17),4,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<macro_boost::CapturedVariable> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_4afe0ef37f3d7d46 ( Context * __context__, TArray<macro_boost::CapturedVariable> &  __a_rename_at_50_18 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_2,cast<TArray<macro_boost::CapturedVariable> &>::from(__a_rename_at_50_18)));
    return das_auto_cast_ref<TArray<macro_boost::CapturedVariable> &>::cast(__a_rename_at_50_18);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_36bc6cba83fc099e ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_181_19, int32_t __value_rename_at_181_20 )
{
    das_copy(__Arr_rename_at_181_19(builtin_array_push_back(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_181_19),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_20);
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_62c4a869e90f1282 ( Context * __context__, char * const  __name_rename_at_631_21, macro_boost::MacroVerifyMacro * __someClassPtr_rename_at_631_22 )
{
    _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_631_22));
    StructInfo const  * __classInfo_rename_at_634_23 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_7a86d959f98dbf0f(__context__,das_arg<macro_boost::MacroVerifyMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_631_22))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_631_21,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_631_22),__classInfo_rename_at_634_23,__context__));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_7b8f4ca3a41e72f4 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1744_24 )
{
    smart_ptr_raw<Expression> __dst_rename_at_1746_25; das_zero(__dst_rename_at_1746_25); das_move(__dst_rename_at_1746_25, _FuncbuiltinTickclone_to_moveTick2007252383599261567_8852dcfb910d89aa(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__src_rename_at_1744_24)));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__dst_rename_at_1746_25);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_6c6f6f459d1ea17c ( Context * __context__, TTable<Variable *,void> const  &  __Tab_rename_at_1014_26, Variable * const  __at_rename_at_1014_27 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_26,__at_rename_at_1014_27));
}

inline void _FuncbuiltinTickinsertTick10959621454228962049_21162df62a60dfe5 ( Context * __context__, TTable<Variable *,void> &  __Tab_rename_at_895_28, Variable * const  __at_rename_at_895_29 )
{
    __builtin_table_set_insert(__context__,__Tab_rename_at_895_28,__at_rename_at_895_29);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_118a5b29d7d59b09 ( Context * __context__, TArray<macro_boost::CapturedVariable> &  __Arr_rename_at_181_30, macro_boost::CapturedVariable &  __value_rename_at_181_31 )
{
    das_copy(__Arr_rename_at_181_30(builtin_array_push_back(das_arg<TArray<macro_boost::CapturedVariable>>::pass(__Arr_rename_at_181_30),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_31);
}

inline void _FuncbuiltinTickinsertTick10959621454228962049_fe29507865f40410 ( Context * __context__, TTable<ExprBlock *,void> &  __Tab_rename_at_895_32, ExprBlock * const  __at_rename_at_895_33 )
{
    __builtin_table_set_insert(__context__,__Tab_rename_at_895_32,__at_rename_at_895_33);
}

inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_a778d918650a9f1b ( Context * __context__, macro_boost::ColletFinally const  &  __someClass_rename_at_684_34 )
{
    macro_boost::ColletFinally const  * __classPtr_rename_at_687_35 = ((macro_boost::ColletFinally const  *)das_ref(__context__,__someClass_rename_at_684_34));
    StructInfo const  * __classInfo_rename_at_688_36 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_73667a6caa7920c1(__context__,__someClass_rename_at_684_34));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<VisitorAdapter>>::cast(makeVisitor(das_auto_cast<void * const >::cast(__classPtr_rename_at_687_35),__classInfo_rename_at_688_36,__context__));
}

inline Sequence DAS_COMMENT((ExprBlock *)) _FuncbuiltinTickkeysTick2205854368403803976_95e7a5c5edb9b4fa ( Context * __context__, TTable<ExprBlock *,void> const  &  __a_rename_at_1128_37 )
{
    Sequence DAS_COMMENT((ExprBlock *)) __it_rename_at_1129_38;das_zero(__it_rename_at_1129_38);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((ExprBlock *))>::pass(__it_rename_at_1129_38),__a_rename_at_1128_37,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((ExprBlock *))>::cast(__it_rename_at_1129_38);
}

inline void _FuncbuiltinTickinsertTick10959621454228962049_c74ac84ad6ee5a59 ( Context * __context__, TTable<int32_t,void> &  __Tab_rename_at_895_39, int32_t __at_rename_at_895_40 )
{
    __builtin_table_set_insert(__context__,__Tab_rename_at_895_39,__at_rename_at_895_40);
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_4ca660e879c50a40 ( Context * __context__, char * const  __name_rename_at_273_41, char * const  __tag_rename_at_273_42, macro_boost::MacroVerifyMacro * __classPtr_rename_at_273_43 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_274_44; memset((void*)&__ann_rename_at_274_44,0,sizeof(__ann_rename_at_274_44));
    /* finally */ auto __finally_273= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_274_44);
    /* end finally */ });
    __ann_rename_at_274_44; das_zero(__ann_rename_at_274_44); das_move(__ann_rename_at_274_44, _FuncastTickmake_function_annotationTick3074191368936885601_62c4a869e90f1282(__context__,__name_rename_at_273_41,__classPtr_rename_at_273_43));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 0x7678364e276db9c2)),__tag_rename_at_273_42,__ann_rename_at_274_44);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_274_44,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Variable * _FuncbuiltinTickget_ptrTick8468476673553620226_8841038ce28d3387 ( Context * __context__, smart_ptr_raw<Variable> const  __src_rename_at_1726_45 )
{
    return das_auto_cast<Variable *>::cast(das_cast<Variable *>::cast(__src_rename_at_1726_45));
}

inline ExprVar * _FuncbuiltinTickget_ptrTick8468476673553620226_ebc419a3b3c42a05 ( Context * __context__, smart_ptr_raw<ExprVar> const  __src_rename_at_1726_46 )
{
    return das_auto_cast<ExprVar *>::cast(das_cast<ExprVar *>::cast(__src_rename_at_1726_46));
}

inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_ee4ed30e25c091a0 ( Context * __context__, macro_boost::CaptureBlock const  &  __someClass_rename_at_684_47 )
{
    macro_boost::CaptureBlock const  * __classPtr_rename_at_687_48 = ((macro_boost::CaptureBlock const  *)das_ref(__context__,__someClass_rename_at_684_47));
    StructInfo const  * __classInfo_rename_at_688_49 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_57e23274c7352cb(__context__,__someClass_rename_at_684_47));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<VisitorAdapter>>::cast(makeVisitor(das_auto_cast<void * const >::cast(__classPtr_rename_at_687_48),__classInfo_rename_at_688_49,__context__));
}

inline Sequence DAS_COMMENT((Variable *)) _FuncbuiltinTickkeysTick2205854368403803976_63e640287a08ea51 ( Context * __context__, TTable<Variable *,ExprVar *> const  &  __a_rename_at_1128_50 )
{
    Sequence DAS_COMMENT((Variable *)) __it_rename_at_1129_51;das_zero(__it_rename_at_1129_51);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((Variable *))>::pass(__it_rename_at_1129_51),__a_rename_at_1128_50,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((Variable *))>::cast(__it_rename_at_1129_51);
}

inline Sequence DAS_COMMENT((ExprVar * &)) _FuncbuiltinTickvaluesTick1351216622833168869_2c23a274fa76e5d2 ( Context * __context__, TTable<Variable *,ExprVar *> &  __a_rename_at_1150_52 )
{
    Sequence DAS_COMMENT((ExprVar * *)) __it_rename_at_1151_53;das_zero(__it_rename_at_1151_53);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((ExprVar *))>::pass(__it_rename_at_1151_53),das_arg<TTable<Variable *,ExprVar *>>::pass(__a_rename_at_1150_52),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((ExprVar * &))>::cast(__it_rename_at_1151_53);
}

inline ExprBlock * _FuncbuiltinTickget_ptrTick8468476673553620226_b22ed6a178fb73f0 ( Context * __context__, smart_ptr_raw<ExprBlock> const  __src_rename_at_1726_54 )
{
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__src_rename_at_1726_54));
}

inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_1a3b715e243e2e09 ( Context * __context__, macro_boost::ColletLabels const  &  __someClass_rename_at_684_55 )
{
    macro_boost::ColletLabels const  * __classPtr_rename_at_687_56 = ((macro_boost::ColletLabels const  *)das_ref(__context__,__someClass_rename_at_684_55));
    StructInfo const  * __classInfo_rename_at_688_57 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_4a213f79ff5ed21b(__context__,__someClass_rename_at_684_55));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<VisitorAdapter>>::cast(makeVisitor(das_auto_cast<void * const >::cast(__classPtr_rename_at_687_56),__classInfo_rename_at_688_57,__context__));
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickkeysTick2205854368403803976_2aeb949e6a3921a5 ( Context * __context__, TTable<int32_t,void> const  &  __a_rename_at_1128_58 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1129_59;das_zero(__it_rename_at_1129_59);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1129_59),__a_rename_at_1128_58,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1129_59);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x74b4210f5379bdbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_912660ab1e667e4b>>();
    };
    aotLib[0xd09660c6ccc577ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795>>();
    };
    aotLib[0xaa0c767f7a614f15] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_7a86d959f98dbf0f>>();
    };
    aotLib[0xe553d306a044fc24] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e608f61b6dfcd4ce>>();
    };
    aotLib[0x12172bd85eb0aa4c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_e48bce725be85082>>();
    };
    aotLib[0xb070103dd2cac518] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_8852dcfb910d89aa>>();
    };
    aotLib[0x870b3f6709b46d1d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_73667a6caa7920c1>>();
    };
    aotLib[0xa4d644936e56f2d4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8477891fe74e291a>>();
    };
    aotLib[0xecfb29ad33fca4b2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_57e23274c7352cb>>();
    };
    aotLib[0xecf942368c658974] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_4a213f79ff5ed21b>>();
    };
    aotLib[0xd57d561550b41b67] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_f815a1708f9fc21b>>();
    };
    aotLib[0x2d35f7e916a2d737] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5eb52cff2bdd0eb4>>();
    };
    aotLib[0x221849cc2c2a5028] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_5726e2a60ae65060>>();
    };
    aotLib[0xd72d456c520ab109] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_4afe0ef37f3d7d46>>();
    };
    aotLib[0xb06bed94af39c78f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_36bc6cba83fc099e>>();
    };
    aotLib[0x502ce87403863c7f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_function_annotationTick3074191368936885601_62c4a869e90f1282>>();
    };
    aotLib[0x80e1870c49481f68] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickadd_ptr_refTick918185754185293024_7b8f4ca3a41e72f4>>();
    };
    aotLib[0x36fc9e1a19baf744] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_6c6f6f459d1ea17c>>();
    };
    aotLib[0x5d83807cd256fbb0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick10959621454228962049_21162df62a60dfe5>>();
    };
    aotLib[0x631fc9ff94e6b639] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_118a5b29d7d59b09>>();
    };
    aotLib[0xbd978cb2d593c43e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick10959621454228962049_fe29507865f40410>>();
    };
    aotLib[0xdc5ef862661ebbe4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_visitorTick897644165917210720_a778d918650a9f1b>>();
    };
    aotLib[0xcc2994356a618695] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_95e7a5c5edb9b4fa>>();
    };
    aotLib[0xd3740a535aff1008] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick10959621454228962049_c74ac84ad6ee5a59>>();
    };
    aotLib[0x22e2eaee027cb7ce] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_4ca660e879c50a40>>();
    };
    aotLib[0xa151d20018f876af] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_8841038ce28d3387>>();
    };
    aotLib[0x1524e3fe8688eddb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_ebc419a3b3c42a05>>();
    };
    aotLib[0x119d9fd98d06210] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_visitorTick897644165917210720_ee4ed30e25c091a0>>();
    };
    aotLib[0xf663a50c5cf495bb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_63e640287a08ea51>>();
    };
    aotLib[0xab9dd9b84a4933ef] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_2c23a274fa76e5d2>>();
    };
    aotLib[0xd5bc22da8d336d95] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_b22ed6a178fb73f0>>();
    };
    aotLib[0x450504417543d4c3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_visitorTick897644165917210720_1a3b715e243e2e09>>();
    };
    aotLib[0x76ab1e6031b7ec2a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_2aeb949e6a3921a5>>();
    };
    // [[ init script ]]
    aotLib[0x5464bbe131808aa5] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_482672747374343583
AotListBase impl_aot_macro_boost(_anon_482672747374343583::registerAotFunctions);
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
