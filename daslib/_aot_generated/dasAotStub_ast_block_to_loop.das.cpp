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
 // require ast_block_to_loop

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
namespace _anon_9086548006838625885 {

namespace ast_block_to_loop { struct B2LVisitor; };
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
namespace ast_block_to_loop {

struct B2LVisitor {
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
    int32_t inClosure;
    int32_t inArchetype;
    bool failOnReturn;
    bool replaceReturnWithContinue;
    bool requireContinueCond;
    TArray<int32_t> loop_depth;
};
}


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bb674c1ef03d7420 ( Context * __context__, ast_block_to_loop::B2LVisitor const  & __cl_rename_at_116_0 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_68_1, int32_t __newSize_rename_at_68_2 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8 ( Context * __context__, TArray<int32_t> & __a_rename_at_1182_3 );
inline void _FuncbuiltinTickpushTick14133213201864676143_527bd39f2798fbd0 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_165_4, int32_t __value_rename_at_165_5 );
inline int32_t & _FuncbuiltinTickbackTick18296309835877697278_65b498d9361d727c ( Context * __context__, TArray<int32_t> & __a_rename_at_473_6 );
inline void _FuncbuiltinTickpopTick1161079256290593740_1e32e23f49324001 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_132_8 );
inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_d9214d1fc8ae3a62 ( Context * __context__, ast_block_to_loop::B2LVisitor const  & __someClass_rename_at_684_9 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bb674c1ef03d7420 ( Context * __context__, ast_block_to_loop::B2LVisitor const  &  __cl_rename_at_116_0 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_0.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_68_1, int32_t __newSize_rename_at_68_2 )
{
    builtin_array_resize(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_68_1),__newSize_rename_at_68_2,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8 ( Context * __context__, TArray<int32_t> &  __a_rename_at_1182_3 )
{
    builtin_array_free(das_arg<TArray<int32_t>>::pass(__a_rename_at_1182_3),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_527bd39f2798fbd0 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_165_4, int32_t __value_rename_at_165_5 )
{
    das_copy(__Arr_rename_at_165_4(builtin_array_push_back(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_165_4),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_5);
}

inline int32_t & _FuncbuiltinTickbackTick18296309835877697278_65b498d9361d727c ( Context * __context__, TArray<int32_t> &  __a_rename_at_473_6 )
{
    int32_t __l_rename_at_474_7 = ((int32_t)builtin_array_size(das_arg<TArray<int32_t>>::pass(__a_rename_at_473_6)));
    if ( __l_rename_at_474_7 == 0 )
    {
        builtin_throw(((char *) "back empty array"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast_ref<int32_t &>::cast(__a_rename_at_473_6((__l_rename_at_474_7 - 1),__context__));
}

inline void _FuncbuiltinTickpopTick1161079256290593740_1e32e23f49324001 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_132_8 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51(__context__,das_arg<TArray<int32_t>>::pass(__Arr_rename_at_132_8),builtin_array_size(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_132_8)) - 1);
}

inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_d9214d1fc8ae3a62 ( Context * __context__, ast_block_to_loop::B2LVisitor const  &  __someClass_rename_at_684_9 )
{
    ast_block_to_loop::B2LVisitor const  * __classPtr_rename_at_687_10 = ((ast_block_to_loop::B2LVisitor const  *)das_ref(__context__,__someClass_rename_at_684_9));
    StructInfo const  * __classInfo_rename_at_688_11 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_bb674c1ef03d7420(__context__,__someClass_rename_at_684_9));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<VisitorAdapter>>::cast(makeVisitor(das_auto_cast<void * const >::cast(__classPtr_rename_at_687_10),__classInfo_rename_at_688_11,__context__));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x402fad95ddd44d1e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_bb674c1ef03d7420>>();
    };
    aotLib[0xd5b298fc293f15ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51>>();
    };
    aotLib[0x439426c0e62e2f81] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8>>();
    };
    aotLib[0xbf539bde820b092a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_527bd39f2798fbd0>>();
    };
    aotLib[0x9c01eb7b05730e24] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickbackTick18296309835877697278_65b498d9361d727c>>();
    };
    aotLib[0xa6ae6ccf63262232] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_1e32e23f49324001>>();
    };
    aotLib[0xdd91932d305064d5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_visitorTick897644165917210720_d9214d1fc8ae3a62>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_9086548006838625885
AotListBase impl_aot_ast_block_to_loop(_anon_9086548006838625885::registerAotFunctions);
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
