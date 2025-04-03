#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require heartbeat
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
namespace _anon_10909611696801807781 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

namespace heartbeat { struct AddHeartbeat; };
namespace heartbeat { struct HeartbeatPass; };
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
// unused enumeration Type
// unused enumeration RefMatters
// unused enumeration ConstMatters
// unused enumeration TemporaryMatters
// unused enumeration ConversionResult
// unused enumeration SideEffects
// unused enumeration CaptureMode
// unused structure AstFunctionAnnotation
// unused structure AstBlockAnnotation
// unused structure AstStructureAnnotation
namespace ast {

struct AstPassMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstPassMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstPassMacro,smart_ptr_raw<Program> const ,Module * const )) apply;
};
static_assert(sizeof(AstPassMacro)==24,"structure size mismatch with DAS");
static_assert(offsetof(AstPassMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstPassMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstPassMacro,apply)==16,"structure field offset mismatch with DAS");
}
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
namespace heartbeat {

struct AddHeartbeat {
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
    bool astChanged;
    Func DAS_COMMENT((void,heartbeat::AddHeartbeat,ExprBlock *)) heartbeatBlock;
};
static_assert(sizeof(AddHeartbeat)==2440,"structure size mismatch with DAS");
static_assert(offsetof(AddHeartbeat,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitProgram)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitProgram)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitProgramBody)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitModule)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitModule)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprTypeDecl)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprTypeDecl)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitTypeDecl)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitTypeDecl)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitAlias)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitAlias)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitEnumeration)==104,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitEnumeration)==112,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitEnumerationValue)==120,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitEnumerationValue)==128,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitEnumeration)==136,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitStructure)==144,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitStructure)==152,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitStructureField)==160,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitStructureField)==168,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitStructure)==176,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitFunction)==184,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitFunctionArgumentInit)==192,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitFunction)==200,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitFunction)==208,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitFunctionArgument)==216,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitFunctionArgument)==224,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitFunctionArgumentInit)==232,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitFunctionArgumentInit)==240,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitFunctionBody)==248,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitFunctionBody)==256,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExpression)==264,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExpression)==272,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprBlock)==280,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprBlock)==288,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprBlockArgument)==296,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprBlockArgument)==304,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprBlockArgumentInit)==312,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprBlockArgumentInit)==320,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprBlockExpression)==328,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprBlockExpression)==336,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprBlockFinal)==344,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprBlockFinal)==352,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprBlockFinalExpression)==360,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprBlockFinalExpression)==368,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprLet)==376,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprLet)==384,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprLetVariable)==392,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprLetVariable)==400,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprLetVariableInit)==408,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprLetVariableInit)==416,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitGlobalVariable)==424,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitGlobalLet)==432,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitGlobalLet)==440,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitGlobalLetVariable)==448,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitGlobalLetVariable)==456,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitGlobalLetVariableInit)==464,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitGlobalLetVariableInit)==472,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprStringBuilder)==480,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprStringBuilder)==488,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprStringBuilderElement)==496,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprStringBuilderElement)==504,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprNew)==512,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprNew)==520,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprNewArgument)==528,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprNewArgument)==536,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprNamedCall)==544,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprNamedCall)==552,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprNamedCallArgument)==560,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprNamedCallArgument)==568,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprLooksLikeCall)==576,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprLooksLikeCall)==584,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitLooksLikeCallArgument)==592,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprLooksLikeCallArgument)==600,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprLooksLikeCallArgument)==608,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitCall)==616,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprCall)==624,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprCall)==632,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprCallArgument)==640,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprCallArgument)==648,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprNullCoalescing)==656,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprNullCoalescing)==664,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprNullCoalescingDefault)==672,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprAt)==680,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprAt)==688,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprAtIndex)==696,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprSafeAt)==704,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprSafeAt)==712,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprSafeAtIndex)==720,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprIs)==728,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprIs)==736,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprIsType)==744,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprOp2)==752,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprOp2)==760,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprOp2Right)==768,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprOp3)==776,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprOp3)==784,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprOp3Left)==792,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprOp3Right)==800,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,isRightFirstExprCopy)==808,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprCopy)==816,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprCopy)==824,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprCopyRight)==832,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,isRightFirstExprMove)==840,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMove)==848,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMove)==856,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMoveRight)==864,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,isRightFirstExprClone)==872,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprClone)==880,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprClone)==888,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprCloneRight)==896,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitWithAliasSubexpression)==904,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprAssume)==912,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprAssume)==920,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprWith)==928,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprWith)==936,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprWithBody)==944,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprWhile)==952,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprWhile)==960,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprWhileBody)==968,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprTryCatch)==976,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprTryCatch)==984,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprTryCatchCatch)==992,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprIfThenElse)==1000,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprIfThenElse)==1008,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprIfThenElseIfBlock)==1016,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprIfThenElseElseBlock)==1024,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprFor)==1032,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprFor)==1040,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprForVariable)==1048,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprForVariable)==1056,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprForSource)==1064,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprForSource)==1072,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprForStack)==1080,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprForBody)==1088,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeVariant)==1096,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeVariant)==1104,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeVariantField)==1112,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeVariantField)==1120,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitMakeStructBody)==1128,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitMakeStructBlock)==1136,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeStruct)==1144,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeStruct)==1152,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeStructIndex)==1160,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeStructIndex)==1168,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeStructField)==1176,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeStructField)==1184,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitMakeStructureBlock)==1192,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitMakeStructureBlock)==1200,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeArray)==1208,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeArray)==1216,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeArrayIndex)==1224,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeArrayIndex)==1232,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeTuple)==1240,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeTuple)==1248,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeTupleIndex)==1256,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeTupleIndex)==1264,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprArrayComprehension)==1272,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprArrayComprehension)==1280,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprArrayComprehensionSubexpr)==1288,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprArrayComprehensionWhere)==1296,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprTypeInfo)==1304,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprTypeInfo)==1312,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprPtr2Ref)==1320,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprPtr2Ref)==1328,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprLabel)==1336,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprLabel)==1344,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprGoto)==1352,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprGoto)==1360,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprRef2Value)==1368,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprRef2Value)==1376,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprRef2Ptr)==1384,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprRef2Ptr)==1392,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprAddr)==1400,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprAddr)==1408,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprAssert)==1416,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprAssert)==1424,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprStaticAssert)==1432,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprStaticAssert)==1440,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprQuote)==1448,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprQuote)==1456,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprDebug)==1464,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprDebug)==1472,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprInvoke)==1480,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprInvoke)==1488,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprErase)==1496,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprErase)==1504,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprSetInsert)==1512,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprSetInsert)==1520,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprFind)==1528,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprFind)==1536,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprKeyExists)==1544,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprKeyExists)==1552,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprAscend)==1560,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprAscend)==1568,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprCast)==1576,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprCast)==1584,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprDelete)==1592,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprDelete)==1600,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprVar)==1608,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprVar)==1616,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprTag)==1624,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprTagValue)==1632,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprTag)==1640,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprField)==1648,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprField)==1656,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprSafeField)==1664,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprSafeField)==1672,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprSwizzle)==1680,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprSwizzle)==1688,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprIsVariant)==1696,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprIsVariant)==1704,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprAsVariant)==1712,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprAsVariant)==1720,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprSafeAsVariant)==1728,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprSafeAsVariant)==1736,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprOp1)==1744,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprOp1)==1752,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprReturn)==1760,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprReturn)==1768,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprYield)==1776,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprYield)==1784,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprBreak)==1792,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprBreak)==1800,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprContinue)==1808,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprContinue)==1816,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,canVisitMakeBlockBody)==1824,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeBlock)==1832,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeBlock)==1840,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMakeGenerator)==1848,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMakeGenerator)==1856,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprMemZero)==1864,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprMemZero)==1872,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConst)==1880,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConst)==1888,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstPtr)==1896,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstPtr)==1904,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstEnumeration)==1912,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstEnumeration)==1920,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstBitfield)==1928,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstBitfield)==1936,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstInt8)==1944,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstInt8)==1952,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstInt16)==1960,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstInt16)==1968,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstInt64)==1976,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstInt64)==1984,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstInt)==1992,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstInt)==2000,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstInt2)==2008,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstInt2)==2016,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstInt3)==2024,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstInt3)==2032,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstInt4)==2040,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstInt4)==2048,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstUInt8)==2056,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstUInt8)==2064,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstUInt16)==2072,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstUInt16)==2080,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstUInt64)==2088,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstUInt64)==2096,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstUInt)==2104,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstUInt)==2112,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstUInt2)==2120,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstUInt2)==2128,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstUInt3)==2136,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstUInt3)==2144,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstUInt4)==2152,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstUInt4)==2160,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstRange)==2168,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstRange)==2176,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstURange)==2184,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstURange)==2192,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstRange64)==2200,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstRange64)==2208,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstURange64)==2216,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstURange64)==2224,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstBool)==2232,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstBool)==2240,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstFloat)==2248,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstFloat)==2256,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstFloat2)==2264,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstFloat2)==2272,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstFloat3)==2280,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstFloat3)==2288,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstFloat4)==2296,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstFloat4)==2304,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstString)==2312,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstString)==2320,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprConstDouble)==2328,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprConstDouble)==2336,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprFakeContext)==2344,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprFakeContext)==2352,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprFakeLineInfo)==2360,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprFakeLineInfo)==2368,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprReader)==2376,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprReader)==2384,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprUnsafe)==2392,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprUnsafe)==2400,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,preVisitExprCallMacro)==2408,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,visitExprCallMacro)==2416,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,astChanged)==2424,"structure field offset mismatch with DAS");
static_assert(offsetof(AddHeartbeat,heartbeatBlock)==2432,"structure field offset mismatch with DAS");
}
namespace heartbeat {

struct HeartbeatPass {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstPassMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstPassMacro,smart_ptr_raw<Program> const ,Module * const )) apply;
};
static_assert(sizeof(HeartbeatPass)==24,"structure size mismatch with DAS");
static_assert(offsetof(HeartbeatPass,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(HeartbeatPass,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(HeartbeatPass,apply)==16,"structure field offset mismatch with DAS");
}

inline void _FuncbuiltinTickpushTick10769833213962245646_6cf9a5e82d90a8ac ( Context * __context__, TArray<ast::AstPassMacro *> & __Arr_rename_at_181_0, ast::AstPassMacro * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2952435867441271 ( Context * __context__, heartbeat::HeartbeatPass const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_379d57396f054877 ( Context * __context__, heartbeat::AddHeartbeat const  & __cl_rename_at_116_3 );
inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_428146f860a5b3b2 ( Context * __context__, heartbeat::AddHeartbeat const  & __someClass_rename_at_675_4 );
inline void set_heartbeat_b037c51092251447 ( Context * __context__, Lambda DAS_COMMENT((void)) __cb_rename_at_17_7 );
inline void heartbeat_dd9ab94cfe6ac86e ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global_zero<Lambda DAS_COMMENT((void)),0xfd15697e41823168>(__context__);/*g_hbCallback*/
    das_global<bool,0x3ee07d3118c2016a>(__context__) = false;/*g_inHB*/
}

inline void _FuncbuiltinTickpushTick10769833213962245646_6cf9a5e82d90a8ac ( Context * __context__, TArray<ast::AstPassMacro *> &  __Arr_rename_at_181_0, ast::AstPassMacro * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstPassMacro *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2952435867441271 ( Context * __context__, heartbeat::HeartbeatPass const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_379d57396f054877 ( Context * __context__, heartbeat::AddHeartbeat const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_428146f860a5b3b2 ( Context * __context__, heartbeat::AddHeartbeat const  &  __someClass_rename_at_675_4 )
{
    heartbeat::AddHeartbeat const  * __classPtr_rename_at_678_5 = ((heartbeat::AddHeartbeat const  *)das_ref(__context__,__someClass_rename_at_675_4));
    StructInfo const  * __classInfo_rename_at_679_6 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_379d57396f054877(__context__,__someClass_rename_at_675_4));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<VisitorAdapter>>::cast(makeVisitor(das_auto_cast<void * const >::cast(__classPtr_rename_at_678_5),__classInfo_rename_at_679_6,__context__));
}

inline void set_heartbeat_b037c51092251447 ( Context * __context__, Lambda DAS_COMMENT((void)) __cb_rename_at_17_7 )
{
    das_delete<Lambda DAS_COMMENT((void))>::clear(__context__,das_global<Lambda DAS_COMMENT((void)),0xfd15697e41823168>(__context__) /*g_hbCallback*/);
    das_move(das_global<Lambda DAS_COMMENT((void)),0xfd15697e41823168>(__context__) /*g_hbCallback*/,__cb_rename_at_17_7);
}

inline void heartbeat_dd9ab94cfe6ac86e ( Context * __context__ )
{
    if ( das_global<bool,0x3ee07d3118c2016a>(__context__) /*g_inHB*/ )
    {
        return ;
    } else {
        if ( das_global<Lambda DAS_COMMENT((void)),0xfd15697e41823168>(__context__) /*g_hbCallback*/ != nullptr )
        {
            das_copy(das_global<bool,0x3ee07d3118c2016a>(__context__) /*g_inHB*/,true);
            das_invoke_lambda<void>::invoke(__context__,nullptr,das_global<Lambda DAS_COMMENT((void)),0xfd15697e41823168>(__context__) /*g_hbCallback*/);
            das_copy(das_global<bool,0x3ee07d3118c2016a>(__context__) /*g_inHB*/,false);
        };
    };
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x8a14f32d6241d5d5] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_6cf9a5e82d90a8ac
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstPassMacro *> & , ast::AstPassMacro *  ),&_FuncbuiltinTickpushTick10769833213962245646_6cf9a5e82d90a8ac>>();
    };
    aotLib[0xb7acde6fd31cb585] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_2952435867441271
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, heartbeat::HeartbeatPass const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_2952435867441271>>();
    };
    aotLib[0x21b12a623b9e5af8] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_379d57396f054877
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, heartbeat::AddHeartbeat const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_379d57396f054877>>();
    };
    aotLib[0xa6f9f1dff779d049] = +[](Context & ctx) -> SimNode* { // _FuncastTickmake_visitorTick897644165917210720_428146f860a5b3b2
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<VisitorAdapter> (*) ( Context * __context__, heartbeat::AddHeartbeat const  &  ),&_FuncastTickmake_visitorTick897644165917210720_428146f860a5b3b2>>();
    };
    aotLib[0x244fc38ec5d850cd] = +[](Context & ctx) -> SimNode* { // set_heartbeat_b037c51092251447
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, Lambda DAS_COMMENT((void))  ),&set_heartbeat_b037c51092251447>>();
    };
    aotLib[0x53719bc08a6d3b2f] = +[](Context & ctx) -> SimNode* { // heartbeat_dd9ab94cfe6ac86e
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__ ),&heartbeat_dd9ab94cfe6ac86e>>();
    };
    // [[ init script ]]
    aotLib[0x4f35b1c002401dff] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<SimNode_Aot<void (*)(Context *, bool),&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_10909611696801807781
AotListBase impl_aot_heartbeat(_anon_10909611696801807781::registerAotFunctions);
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
