#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require strings_boost
 // require ast_boost
 // require lint

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
namespace _anon_12570130816669471540 {

namespace lint { struct LintEverything; };
namespace lint { struct LintVisitor; };
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
// unused enumeration ConversionResult
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
// unused enumeration CaptureMode
// unused enumeration SideEffects
// unused structure AstFunctionAnnotation
// unused structure AstBlockAnnotation
// unused structure AstStructureAnnotation
namespace ast {

struct AstPassMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstPassMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstPassMacro,smart_ptr_raw<Program> const ,Module * const )) apply;
};
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
namespace lint {

struct LintEverything {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstPassMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstPassMacro,smart_ptr_raw<Program> const ,Module * const )) apply;
};
}
namespace lint {

struct LintVisitor {
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
    smart_ptr_raw<VisitorAdapter> astVisitorAdapter;
    TArray<uint64_t> exprForTerminator;
    bool compile_time_errors;
    bool noLint;
    Func DAS_COMMENT((void,lint::LintVisitor,char * const ,LineInfo const )) lint_error;
    Func DAS_COMMENT((void,lint::LintVisitor,smart_ptr_raw<Variable> const ,bool)) validate_var;
};
}
extern StructInfo __struct_info__1e8db4ddc1444e12;
extern StructInfo __struct_info__9a6c40d2ef34a474;
extern TypeInfo __type_info__3c61146b2bdfb90;
extern TypeInfo __type_info__7c61f7ae88617bb2;
extern TypeInfo __type_info__c2f4bc15903e1610;
extern TypeInfo __type_info__defdb920e82da0f4;
extern TypeInfo __type_info__229aabe2f8bef1d9;
extern TypeInfo __type_info__6bdd529063b3dbeb;
extern TypeInfo __type_info__d551858bc6d43037;
extern TypeInfo __type_info__83c768ad9b3f81ea;
extern TypeInfo __type_info__241df6ccda394202;
extern TypeInfo __type_info__4dee28f2a93bbef7;
extern TypeInfo __type_info__5047b5dbcc2127cd;
extern TypeInfo __type_info__7e04c1d12891d606;
extern TypeInfo __type_info__7dd1c1d1283b2d06;
extern TypeInfo __type_info__636dc1714c171367;
extern TypeInfo __type_info__bc5b346893db35b;
extern TypeInfo __type_info__93546827b32c5422;
extern TypeInfo __type_info__118bfa23ce6c000c;
extern TypeInfo __type_info__9c92a72bb3a64bfa;
extern TypeInfo __type_info__ea252439573ea197;
extern TypeInfo __type_info__7f81cc8503986a86;
extern TypeInfo __type_info__b6d18d4b3fadccd4;
extern TypeInfo __type_info__4f4cc10892c6c61;
extern TypeInfo __type_info__65f51082d9833a;
extern TypeInfo __type_info__9089610918ba11f;
extern TypeInfo __type_info__50c7808637778d65;
extern TypeInfo __type_info__87ae85b131d91f57;
extern TypeInfo __type_info__2d27aed7dd587c30;
extern TypeInfo __type_info__31b7a5d7e3ad4eb7;
extern TypeInfo __type_info__2914e4d7d4fafa72;
extern TypeInfo __type_info__773524bb75b61932;
extern TypeInfo __type_info__4a0758d80e688a0e;
extern TypeInfo __type_info__a28688c7ffe035ce;
extern TypeInfo __type_info__a45346c81e6a9b80;
extern TypeInfo __type_info__4f02b717be42f032;
extern TypeInfo __type_info__214ca0a8404236ce;
extern TypeInfo __type_info__e9cb7c9300717d9e;
extern TypeInfo __type_info__f108ab47d962e793;
extern TypeInfo __type_info__514742689af99de7;
extern TypeInfo __type_info__563e42636db280e7;
extern TypeInfo __type_info__a17d42718ebadfe7;
extern TypeInfo __type_info__63e1b8a29ad93469;
extern TypeInfo __type_info__f6614284ea50cbe7;
extern TypeInfo __type_info__518dd4a2ad91defd;
extern TypeInfo __type_info__b8524aede8fd2575;
extern TypeInfo __type_info__d2fee6b26665c989;
extern TypeInfo __type_info__5b91ede0508873e;
extern TypeInfo __type_info__350b375c34e0c48a;
extern TypeInfo __type_info__c84cf5ded2cd1cd8;
extern TypeInfo __type_info__120723ecb6510065;
extern TypeInfo __type_info__e672712e93e236ba;
extern TypeInfo __type_info__3229d47464f4ad50;
extern TypeInfo __type_info__511818eae83f8137;
extern TypeInfo __type_info__849126a4e3db3268;
extern TypeInfo __type_info__ee83d76e6f9a3c81;
extern TypeInfo __type_info__9745884abdafbe87;
extern TypeInfo __type_info__41023c185ec41d2;
extern TypeInfo __type_info__88db72c3eb8c93b1;
extern TypeInfo __type_info__c3c8c780df6c5865;
extern TypeInfo __type_info__8a5e2edb26418a2a;
extern TypeInfo __type_info__837624c70f8f1fa1;
extern TypeInfo __type_info__7f9fc2c601e28df1;
extern TypeInfo __type_info__6ad276912e16c445;
extern TypeInfo __type_info__349161eed600549f;
extern TypeInfo __type_info__8faf3ae8c5ebe47a;
extern TypeInfo __type_info__f44650fbe99befd9;
extern TypeInfo __type_info__e0b574ceb6c8c70f;
extern TypeInfo __type_info__6c1a6b092c78a88;
extern TypeInfo __type_info__5276a743108434eb;
extern TypeInfo __type_info__ea03eef331aabf4;
extern TypeInfo __type_info__4191dbf23146a87e;
extern TypeInfo __type_info__f66cc598ea369f61;
extern TypeInfo __type_info__c19751d6d5da74e2;
extern TypeInfo __type_info__44cd26f4cb3df7e1;
extern TypeInfo __type_info__5d6138f13e1e88c4;
extern TypeInfo __type_info__e7e2063b91ac55a1;
extern TypeInfo __type_info__c7c0e4fba3dcbfcf;
extern TypeInfo __type_info__c5915ffba474f7fe;
extern TypeInfo __type_info__ca2136fbaac99425;
extern TypeInfo __type_info__7d9fd489616ae8d;
extern TypeInfo __type_info__9dfe8a83730428c8;
extern TypeInfo __type_info__6628bcbce7db6a7d;
extern TypeInfo __type_info__1151bc4127672205;
extern TypeInfo __type_info__bba83b75d4855b7e;
extern TypeInfo __type_info__1a5b7f11cf3fb5b5;
extern TypeInfo __type_info__45d77ccae958b9de;
extern TypeInfo __type_info__b5e62a55ec68b6b5;
extern TypeInfo __type_info__a7adf4b0a367d897;
extern TypeInfo __type_info__9c37565e66334661;
extern TypeInfo __type_info__74372feec5a81686;
extern TypeInfo __type_info__4e7dff8bb14f539;
extern TypeInfo __type_info__6bb94e24ea14ce9a;
extern TypeInfo __type_info__631c9e15ba7d5036;
extern TypeInfo __type_info__c1c6f9bc0741f232;
extern TypeInfo __type_info__3b037c8d587730b0;
extern TypeInfo __type_info__563543a880fdcea2;
extern TypeInfo __type_info__eb22258b16c8c6df;
extern TypeInfo __type_info__f5c1d1c41d788f7;
extern TypeInfo __type_info__9cac32b4050a2fb8;
extern TypeInfo __type_info__c758d466d1a06ae2;
extern TypeInfo __type_info__60501e84f49c29e1;
extern TypeInfo __type_info__2dd484863625d80;
extern TypeInfo __type_info__34b7c04894c15d5;
extern TypeInfo __type_info__f9220d94c6b964b5;
extern TypeInfo __type_info__79c6e4b278757551;
extern TypeInfo __type_info__e266b5ccef058802;
extern TypeInfo __type_info__624d371c76b25aa4;
extern TypeInfo __type_info__29c0090cdbf7525c;
extern TypeInfo __type_info__246dda13a8a4b104;
extern TypeInfo __type_info__586f0da79a6e613d;
extern TypeInfo __type_info__98064c57b4bcca5a;
extern TypeInfo __type_info__9d10785eb07580e0;
extern TypeInfo __type_info__9a5e492166d49949;
extern TypeInfo __type_info__e57b0f261f47b890;
extern TypeInfo __type_info__d6b8ed05d16e9f27;
extern TypeInfo __type_info__defb2f7795e0cf8c;
extern TypeInfo __type_info__6c2b5208df908cfa;
extern TypeInfo __type_info__a3a6bcfebaf8fcd8;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__cd505ad3b1c59cc6;
extern TypeInfo __type_info__7329fadda4ca251c;
extern TypeInfo __type_info__3693bdfd1150bb56;
extern TypeInfo __type_info__71ff6f045d2186f1;
extern TypeInfo __type_info__acc5cdadba98f68e;
extern TypeInfo __type_info__2055bdfdcee6bf5e;
extern TypeInfo __type_info__fb56aefdaf9de951;
extern TypeInfo __type_info__cb4a7f89a13eab36;
extern TypeInfo __type_info__71c84a7f531ca5bb;
extern TypeInfo __type_info__afd7e462d2caeebb;
extern TypeInfo __type_info__54fceee561bff5eb;
extern TypeInfo __type_info__a0219258cb3926ee;
extern TypeInfo __type_info__1afef6e5304b2283;
extern TypeInfo __type_info__898a3dd26b376c6a;
extern TypeInfo __type_info__699f14ced40c8382;
extern TypeInfo __type_info__ad18450df661455f;
extern TypeInfo __type_info__91bbd69210f68e07;
extern TypeInfo __type_info__3ee228fe47602659;
extern TypeInfo __type_info__365a0d74b6e3ae27;
extern TypeInfo __type_info__72bac02d9b0c1dd;
extern TypeInfo __type_info__3c5ac02d6cd98dd;
extern TypeInfo __type_info__f2c7ac02c85dcbdd;
extern TypeInfo __type_info__bfbf448dd60c6211;
extern TypeInfo __type_info__9307dd967ffe2b49;
extern TypeInfo __type_info__8903e59677a2e7e1;
extern TypeInfo __type_info__8c69e5967a8610e1;
extern TypeInfo __type_info__8237e59671dc95e1;
extern TypeInfo __type_info__7b85db966c4260e3;
extern TypeInfo __type_info__aaffe596948281e1;
extern TypeInfo __type_info__108c5371ed782a25;
extern TypeInfo __type_info__ef0e5b71d12c4f0e;
extern TypeInfo __type_info__6dfe2527715392d1;
extern TypeInfo __type_info__44e10b9c0b7ea95f;
extern TypeInfo __type_info__81d4b6e4402ada81;
extern TypeInfo __type_info__32209cf3725705b0;
extern TypeInfo __type_info__50a064d75d4cc1fb;
extern TypeInfo __type_info__50a063d75d4cc048;
extern TypeInfo __type_info__50a06ad75d4ccc2d;
extern TypeInfo __type_info__2b5f1ef36c99e51b;
extern TypeInfo __type_info__50a05ed75d4cb7c9;
extern TypeInfo __type_info__50ca60d75d94192f;
extern TypeInfo __type_info__ee05ad47ac112e5f;
extern TypeInfo __type_info__a1fe7a142c668903;
extern TypeInfo __type_info__37bd8d7fdf3c5374;
extern TypeInfo __type_info__10fefde527f0e316;
extern TypeInfo __type_info__8ea2bb6c84fe54ae;
extern TypeInfo __type_info__643b022638807dc3;
extern TypeInfo __type_info__e166b9c4a79e779;
extern TypeInfo __type_info__a3d5bceeff53f155;
extern TypeInfo __type_info__f85f434a5cfa7cf9;
extern TypeInfo __type_info__c1ab66e04afa3a7;
extern TypeInfo __type_info__b8cb16fdfafa869b;
extern TypeInfo __type_info__598840fdaa05c3ef;
extern TypeInfo __type_info__aa2eff9e8711b4c;
extern TypeInfo __type_info__833e12e4dcd8153d;
extern TypeInfo __type_info__f1f05ee81890b310;
extern TypeInfo __type_info__afca8289899d784f;
extern TypeInfo __type_info__725600cc59f9ef1;
extern TypeInfo __type_info__5a876ec502d05cd5;
extern TypeInfo __type_info__a8d3190cd853597a;
extern TypeInfo __type_info__b3d9c0cc943b4165;
extern TypeInfo __type_info__d07c067a5c7b8ff4;
extern TypeInfo __type_info__7260bd93a15a7ff1;
extern TypeInfo __type_info__e9813cd85e320ce1;
extern TypeInfo __type_info__644a49dea6863e78;
extern TypeInfo __type_info__b6c344d07fc80acd;
extern TypeInfo __type_info__94e15ebe6d2ac6c5;
extern TypeInfo __type_info__749bdb083606521a;
extern TypeInfo __type_info__bbedea2da76c1cbd;
extern TypeInfo __type_info__6508f9c8d2b82c4a;
extern TypeInfo __type_info__c88d1b35f2ffa823;
extern TypeInfo __type_info__f14dc0d72b72a465;
extern TypeInfo __type_info__e4bc23ea0da25d79;
extern TypeInfo __type_info__b4d3bed2a010acf4;
extern TypeInfo __type_info__b839bed2a2f3d5f4;
extern TypeInfo __type_info__bb9fbed2a5d6fef4;
extern TypeInfo __type_info__b0f72e776d005eaf;
extern TypeInfo __type_info__29261b9b611e6f1b;
extern TypeInfo __type_info__64b15f9df38db54f;
extern TypeInfo __type_info__d27a1f910d191ab3;
extern TypeInfo __type_info__6dc5617548466ef3;
extern TypeInfo __type_info__15a45142a97c9b3e;
extern TypeInfo __type_info__cfc6c6515483a76b;
extern TypeInfo __type_info__a87ef47d40240d3c;
extern TypeInfo __type_info__10ddfd98f14d71c1;
extern TypeInfo __type_info__4396458b6cca487d;
extern TypeInfo __type_info__7a94f4cc4bcf20e5;
extern TypeInfo __type_info__278bc6d46dadffa8;
extern TypeInfo __type_info__da0e82cafc1e70b1;
extern TypeInfo __type_info__7b55c0a63e321fc1;
extern TypeInfo __type_info__5e2809979d5f78c9;
extern TypeInfo __type_info__60a144dd7cf1ba91;
extern TypeInfo __type_info__c15e41a8ee5ebf97;
extern TypeInfo __type_info__a88454b76bb549ba;
extern TypeInfo __type_info__811c0b03f452ec1;
extern TypeInfo __type_info__51f018132be6c64;
extern TypeInfo __type_info__b99012ac7e42bacf;
extern TypeInfo __type_info__8873b51c25d24aa7;
extern TypeInfo __type_info__960dd6428887a234;
extern TypeInfo __type_info__a57bf935c2dd03;
extern TypeInfo __type_info__acd33335f9c1e498;
extern TypeInfo __type_info__60d16a2d23420951;
extern TypeInfo __type_info__ce241e3005cc873b;
extern TypeInfo __type_info__ccd32e474e9a33eb;
extern TypeInfo __type_info__4200353d82fda873;
extern TypeInfo __type_info__8afce1a80940fc9e;
extern TypeInfo __type_info__125855d9cd771ead;
extern TypeInfo __type_info__21586ce84f433a21;
extern TypeInfo __type_info__f5340b6734d7b2c1;
extern TypeInfo __type_info__e4765bc563f255e;
extern TypeInfo __type_info__6636442e03391ebf;
extern TypeInfo __type_info__4cdbed951d30a5d1;
extern TypeInfo __type_info__c52835f1e7c9ab84;
extern TypeInfo __type_info__7e104fcf0cd430e4;
extern TypeInfo __type_info__afcf203e0d7d50d;
extern TypeInfo __type_info__4d5fdda373bcfbd1;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__d9307e078cfb0f0c;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__404fbd8651779228;
extern VarInfo __var_info__fb898b509e28d9c8;
extern VarInfo __var_info__802b484819ed0c2d;
extern VarInfo __var_info__7c71c3b025ab6c55;
extern VarInfo __var_info__73ec7f5588dcbccc;
extern VarInfo __var_info__6df9772d1def04b5;
extern VarInfo __var_info__afd5e182c47c834d;
extern VarInfo __var_info__f722ae5ea36a9c48;
extern VarInfo __var_info__9bf7e6bedd4bf03b;
extern VarInfo __var_info__4d2aa20c479aea6a;
extern VarInfo __var_info__b8106945f6e3fc3a;
extern VarInfo __var_info__717cd02cdb41bb2a;
extern VarInfo __var_info__b9386f8f015bfb1e;
extern VarInfo __var_info__8941a79636fd28ed;
extern VarInfo __var_info__591a1931dbf740b7;
extern VarInfo __var_info__e8e0d09fd41dec39;
extern VarInfo __var_info__2d81bedb489517e7;
extern VarInfo __var_info__2c95acdb472c618f;
extern VarInfo __var_info__dfaf2e5a156cac5b;
extern VarInfo __var_info__139f4dd326162ccb;
extern VarInfo __var_info__7c24884234a37391;
extern VarInfo __var_info__58ee4c02dcf52dbc;
extern VarInfo __var_info__d793210438da73b2;
extern VarInfo __var_info__4380a05405d544a9;
extern VarInfo __var_info__1dd9197835443a11;
extern VarInfo __var_info__7d0d2be93f052825;
extern VarInfo __var_info__2a88dc897f736b05;
extern VarInfo __var_info__2a79f0897f884291;
extern VarInfo __var_info__f36af989502c64dc;
extern VarInfo __var_info__363d68bc53f7e730;
extern VarInfo __var_info__6caf569fc5ada20f;
extern VarInfo __var_info__e4da73044655d61b;
extern VarInfo __var_info__fad133f08b433e3b;
extern VarInfo __var_info__883c3e97b305c7e1;
extern VarInfo __var_info__3eb766e159e2db49;
extern VarInfo __var_info__9f8350e9c19f8255;
extern VarInfo __var_info__1fbffaa00541e191;
extern VarInfo __var_info__81245a85698ce65d;
extern VarInfo __var_info__4ee15802d484dfce;
extern VarInfo __var_info__6a62a96f37358e75;
extern VarInfo __var_info__9e31d981d2f6e393;
extern VarInfo __var_info__4ee94302d492541f;
extern VarInfo __var_info__f5c258045726d71a;
extern VarInfo __var_info__6e8908b5e9b654b3;
extern VarInfo __var_info__22e143125efe0694;
extern VarInfo __var_info__3575e938698ac7f2;
extern VarInfo __var_info__68cb32f89791abb7;
extern VarInfo __var_info__2cab45e6e29dabd4;
extern VarInfo __var_info__63e2d4927d6419f0;
extern VarInfo __var_info__619835e3cb9cb14b;
extern VarInfo __var_info__72550b12fb41909b;
extern VarInfo __var_info__72550a12fb418ee8;
extern VarInfo __var_info__72551112fb419acd;
extern VarInfo __var_info__4a49f2369627a5ce;
extern VarInfo __var_info__633a91c125407a4d;
extern VarInfo __var_info__634c8ec1255f0b34;
extern VarInfo __var_info__634c8fc1255f0ce7;
extern VarInfo __var_info__634c8cc1255f07ce;
extern VarInfo __var_info__63388ac1253d0868;
extern VarInfo __var_info__634c88c1255f0102;
extern VarInfo __var_info__9f39d836de4c48a0;
extern VarInfo __var_info__23dd3c465c5aacf8;
extern VarInfo __var_info__4b86398eedf1a60a;
extern VarInfo __var_info__6c8186fb9d866b04;
extern VarInfo __var_info__d01a3f236d5d52e9;
extern VarInfo __var_info__5a3228324ddf49ed;
extern VarInfo __var_info__cfe83f236d085ce9;
extern VarInfo __var_info__cfe93f236d0a0fe9;
extern VarInfo __var_info__cfee3f236d128ee9;
extern VarInfo __var_info__5d98263250c26f87;
extern VarInfo __var_info__cff23f236d195ae9;
extern VarInfo __var_info__aaa036240f9f1383;
extern VarInfo __var_info__af4bb646a82e19f5;
extern VarInfo __var_info__7363dd5a3a53e072;
extern VarInfo __var_info__7df04402fc0efcd2;
extern VarInfo __var_info__cd5d0594bbd31183;
extern VarInfo __var_info__4b98c0e117b36490;
extern VarInfo __var_info__81bbbfe1459b2b77;
extern VarInfo __var_info__e6914f2eefeace48;
extern VarInfo __var_info__b1b57985905c2546;
extern VarInfo __var_info__6b597ff2a170677e;
extern VarInfo __var_info__5790fbaa5aa10da3;
extern VarInfo __var_info__2b510ff5bd61305c;
extern VarInfo __var_info__69da5802eb324301;
extern VarInfo __var_info__7e1a4402fc47eb05;
extern VarInfo __var_info__b6a2068ef526c805;
extern VarInfo __var_info__ce17076d9f8cf753;
extern VarInfo __var_info__bea7e44555360f20;
extern VarInfo __var_info__bb6c0827af8b5346;
extern VarInfo __var_info__7e014002fc204c62;
extern VarInfo __var_info__c51c17bf11ebed33;
extern VarInfo __var_info__fb77ceef0b6231cd;
extern VarInfo __var_info__c099ede2b5923b6e;
extern VarInfo __var_info__7f07400e46e3e0c3;
extern VarInfo __var_info__363c70bc53f641c8;
extern VarInfo __var_info__eda32c89381565ec;
extern VarInfo __var_info__c9b7983395b4b39d;
extern VarInfo __var_info__e32df143ca82380d;
extern VarInfo __var_info__b84f13cf26eda329;
extern VarInfo __var_info__5c324202df78f6a9;
extern VarInfo __var_info__b5f04820b8d5b502;
extern VarInfo __var_info__86d2782a6bf7b7d0;
extern VarInfo __var_info__bbbb051551a66a0a;
extern VarInfo __var_info__29a0d7a9e4b9e5f6;
extern VarInfo __var_info__92682c1a46d2a5e1;
extern VarInfo __var_info__18a580c033b89448;
extern VarInfo __var_info__ff9cbce939b8bbe7;
extern VarInfo __var_info__aaec8cf0fd2ec5ac;
extern VarInfo __var_info__6c47240f97a09f01;
extern VarInfo __var_info__8173a7f23281b52c;
extern VarInfo __var_info__3694ecd9abede4e4;
extern VarInfo __var_info__f1f74913939e753c;
extern VarInfo __var_info__4bdfc151dedc4da9;
extern VarInfo __var_info__305131e8c4628f64;
extern VarInfo __var_info__bab2808884e1a301;
extern VarInfo __var_info__4fa475cf1e4911cf;
extern VarInfo __var_info__7e083e02fc1adb06;
extern VarInfo __var_info__eca633024932f86f;
extern VarInfo __var_info__54a28bc44192b9c1;
extern VarInfo __var_info__9d289c0e202c475;
extern VarInfo __var_info__5c394f02df7f2b6e;
extern VarInfo __var_info__ae3ae56613d11d42;
extern VarInfo __var_info__f2f0e741960e1177;
extern VarInfo __var_info__be686e26bffa9144;
extern VarInfo __var_info__14d78d02a2d94ef1;
extern VarInfo __var_info__14d78a02a2d949d8;
extern VarInfo __var_info__2bce2565f2557691;
extern VarInfo __var_info__14d78b02a2d94b8b;
extern VarInfo __var_info__fab868f914151ffa;
extern VarInfo __var_info__28a45465ed4640ea;
extern VarInfo __var_info__4585e1e3fd2a1c4f;
extern VarInfo __var_info__6ff71915ee4dc68;
extern VarInfo __var_info__a1e2cce18d36fa97;
extern VarInfo __var_info__e5a725d92e24b68f;
extern VarInfo __var_info__8bd3f7a275b2fdb6;
extern VarInfo __var_info__6ecbd3e161d61b3f;
extern VarInfo __var_info__648af9cb3fe807e2;
extern VarInfo __var_info__415c6fcc77e22573;
extern VarInfo __var_info__452510e5117043d2;
extern VarInfo __var_info__a13178df83f85383;
extern VarInfo __var_info__64c3392746be1d39;
extern VarInfo __var_info__97956f84758cbd42;
extern VarInfo __var_info__89002db3d2a382d9;
extern VarInfo __var_info__dcd62ab4848d54b8;
extern VarInfo __var_info__68bdfe50f2934580;
extern VarInfo __var_info__4e485f02d3a679c8;
extern VarInfo __var_info__2c13e88b48ba86ba;
extern VarInfo __var_info__6c007322603a901e;
extern VarInfo __var_info__60013666243ecb3;
extern VarInfo __var_info__7c1c3f2d00ff5aa8;
extern VarInfo __var_info__5cfa422ce6237a8e;
extern VarInfo __var_info__893bf90e80994551;
extern VarInfo __var_info__4e4f4402d3ac6a95;
extern VarInfo __var_info__146b37efc8a0690c;
extern VarInfo __var_info__1aa99d5f0bdf07c0;
extern VarInfo __var_info__69e44002eb742df2;
extern VarInfo __var_info__f4d0322851dbbd1c;
extern VarInfo __var_info__4ca48ef5fe7d5999;
extern VarInfo __var_info__d087bf88dadfc976;
extern VarInfo __var_info__9cd5a85ccfeceec9;
extern VarInfo __var_info__5c978d0137240e4a;
extern VarInfo __var_info__eec6a05fd6476150;
extern VarInfo __var_info__a3c16915074de26f;
extern VarInfo __var_info__a27d86cb8c142849;
extern VarInfo __var_info__5e30630df7d456a2;
extern VarInfo __var_info__8341a2400990c170;
extern VarInfo __var_info__a8c08fb6b3ae9ad9;
extern VarInfo __var_info__47802d686ce7346b;
extern VarInfo __var_info__571af9efe83812fc;
extern VarInfo __var_info__5c630a811e350e10;
extern VarInfo __var_info__41fcc1ce9520d068;
extern VarInfo __var_info__b391f5eabc24c0ed;
extern VarInfo __var_info__d80a645b6da8c535;
extern VarInfo __var_info__36c4426c738ddad;
extern VarInfo __var_info__6cad416db0286952;
extern VarInfo __var_info__eaf6264c8c2101ff;
extern VarInfo __var_info__98a9ffc4527873f2;
extern VarInfo __var_info__5935677610234a58;
extern VarInfo __var_info__8e3ef7346246d5cf;
extern VarInfo __var_info__862b83af69f35e28;
extern VarInfo __var_info__d5937361ae5f76f8;
extern VarInfo __var_info__bd93646199fbaf0b;
extern VarInfo __var_info__9dedb59394916004;
extern VarInfo __var_info__29320d51a2c5f4d;
extern VarInfo __var_info__e68dd1c7036b46b6;
extern VarInfo __var_info__7ece88bde3d9c42c;
extern VarInfo __var_info__4625bd7717ba4469;
extern VarInfo __var_info__8209de0cf0162633;
extern VarInfo __var_info__f6130cfee51f9aa1;
extern VarInfo __var_info__249b22d5373d404d;
extern VarInfo __var_info__c51617cedb37de35;
extern VarInfo __var_info__e52aaee50baa0521;
extern VarInfo __var_info__8517df657f4530a;
extern VarInfo __var_info__921c0fceafe8699d;
extern VarInfo __var_info__ba7813ced1e54836;
extern VarInfo __var_info__be5700ced58d4272;
extern VarInfo __var_info__f9ae7a6ab1908649;
extern VarInfo __var_info__6ab1c296bcd15d4d;
extern VarInfo __var_info__566ddd22f95b7cfb;
extern VarInfo __var_info__c8ee8b2e94f52665;
extern VarInfo __var_info__ab85832951adf487;
extern VarInfo __var_info__abb3832951fc1e87;
extern VarInfo __var_info__abb2832951fa6b87;
extern VarInfo __var_info__abb1832951f8b887;
extern VarInfo __var_info__6794a996ba4a520f;
extern VarInfo __var_info__18f4641e60fa4fdb;
extern VarInfo __var_info__6786a996ba32880f;
extern VarInfo __var_info__6787a996ba343b0f;
extern VarInfo __var_info__6788a996ba35ee0f;
extern VarInfo __var_info__29f2661e6f6a2041;
extern VarInfo __var_info__677ca996ba218a0f;
extern VarInfo __var_info__893bad96d6e1b2dc;
extern VarInfo __var_info__c0c341f22742465d;
extern VarInfo __var_info__834b777876181e6b;
extern VarInfo __var_info__ac4a824df3a23f27;
extern VarInfo __var_info__f6bea3965a10ae11;
extern VarInfo __var_info__5688187b0a327e60;
extern VarInfo __var_info__569e1b7b0a57e579;
extern VarInfo __var_info__569e1a7b0a57e3c6;
extern VarInfo __var_info__569e1d7b0a57e8df;
extern VarInfo __var_info__568a1f7b0a35f045;
extern VarInfo __var_info__569e217b0a57efab;
extern VarInfo __var_info__f0945e93d1915b71;
extern VarInfo __var_info__5d8bfc2d1a2015a5;
extern VarInfo __var_info__6ed4cffd10453646;
extern VarInfo __var_info__9c4706ceb88bb6a4;
extern VarInfo __var_info__fcd0fedcec5e3016;
extern VarInfo __var_info__8f62646d23f25b99;
extern VarInfo __var_info__21a610d8b307aab7;
extern VarInfo __var_info__635ac63675961b5b;
extern VarInfo __var_info__44f160a6dbf6c463;
extern VarInfo __var_info__3313f7e77d463c1d;
extern VarInfo __var_info__d95efe75d53d785;
extern VarInfo __var_info__b585c093a8729bb5;
extern VarInfo __var_info__bcee3052ac4d0412;
extern VarInfo __var_info__2f6c89bb2174e0d4;
extern VarInfo __var_info__7adb14e36229366e;
extern VarInfo __var_info__38c0b8b3a0035f4b;
extern VarInfo __var_info__70eaf9dc12ebfe56;
extern VarInfo __var_info__82bdb4937d781651;
extern VarInfo __var_info__a6c5a0d476401dcf;
extern VarInfo __var_info__803a9565358b9c2b;
extern VarInfo __var_info__8a7f1eb665490d1a;
extern VarInfo __var_info__9387c6938b8fa5e7;
extern VarInfo __var_info__692a2ee8bd34e45e;
extern VarInfo __var_info__2aa1ee74f855f5b4;
extern VarInfo __var_info__166357f10c39f6fc;
extern VarInfo __var_info__13e0e5f72f8217fb;
extern VarInfo __var_info__3b849f4fb88473bb;
extern VarInfo __var_info__f1bb59011ce51e2a;
extern VarInfo __var_info__ba19715e054fd353;
extern VarInfo __var_info__160e1334ac8f8bfc;
extern VarInfo __var_info__ac390a5ffb883c3b;
extern VarInfo __var_info__87544b21d0bdc68e;
extern VarInfo __var_info__467b3460de5445fe;
extern VarInfo __var_info__3999c8ecc927783;
extern VarInfo __var_info__7e590097bace95e2;
extern VarInfo __var_info__e1490f83f62810a2;
extern VarInfo __var_info__2f5a09d014210edd;
extern VarInfo __var_info__89db2eaecd06de6f;
extern VarInfo __var_info__fe090ab26f161170;
extern VarInfo __var_info__b015b3f7761553d5;
extern VarInfo __var_info__ec7be13e5039487e;
extern VarInfo __var_info__9a7ac69391983ce7;
extern VarInfo __var_info__999aa82fd12dcf8;
extern VarInfo __var_info__c851ce6ee7f3502b;
extern VarInfo __var_info__9750b1938f1ae438;
extern VarInfo __var_info__974fb1938f193138;
extern VarInfo __var_info__974eb1938f177e38;
extern VarInfo __var_info__10fa045a7f20e7c5;
extern VarInfo __var_info__f881ff76a48e7a59;
extern VarInfo __var_info__5992ea8a5e268051;
extern VarInfo __var_info__2a5ff066b4c53561;
extern VarInfo __var_info__eb17a67d73d1450d;
extern VarInfo __var_info__6ac4f424293494e8;
extern VarInfo __var_info__9c27b1a81ccde81;
extern VarInfo __var_info__f3484371ddf5f3ea;
extern VarInfo __var_info__671c1e58b35b487f;
extern VarInfo __var_info__1a6629989f7a578b;
extern VarInfo __var_info__b369ba5a9fd3f6bf;
extern VarInfo __var_info__7cb15833a89cbb44;
extern VarInfo __var_info__50599c4cfbbbf1a3;
extern VarInfo __var_info__4c448f3c87aebe6b;
extern VarInfo __var_info__786ac2937496a01b;
extern VarInfo __var_info__ee20777da5e5bc33;
extern VarInfo __var_info__ae2e6c616c88a62b;
extern VarInfo __var_info__35446d2c24c68711;
extern VarInfo __var_info__436fe705f6119254;
extern VarInfo __var_info__7f85c2937ae32f1b;
extern VarInfo __var_info__1f78119362cddb74;
extern VarInfo __var_info__c356f39314340551;
extern VarInfo __var_info__643af750008e451d;
extern VarInfo __var_info__4782749160e23df2;
extern VarInfo __var_info__c86ad23f08ba4d61;
extern VarInfo __var_info__af15ebc7492e3415;
extern VarInfo __var_info__101482393405a9cf;
extern VarInfo __var_info__2d7207ab2cb8029;
extern VarInfo __var_info__6c71ddec41aaaf17;
extern VarInfo __var_info__ab31c0bc9573a50e;
extern VarInfo __var_info__8edfb2a132d8a1a4;
extern VarInfo __var_info__b3b4b1a5de3de0c7;
extern VarInfo __var_info__5ccfb39084cfbf8d;
extern VarInfo __var_info__ebde9918bcd35380;
extern VarInfo __var_info__c66a4f95fccc7a5f;
extern VarInfo __var_info__7e3ddf73033bf97a;
extern VarInfo __var_info__7c4e6040573c64f1;
extern VarInfo __var_info__d882593a02577e1e;
extern VarInfo __var_info__9e64c6f0cab96bde;
extern VarInfo __var_info__2aff4c764d300b71;
extern VarInfo __var_info__317dd97cc75b2d63;
extern VarInfo __var_info__833b891d4a8b6f3b;
extern VarInfo __var_info__933eb58cc40d3f86;
extern VarInfo __var_info__ba3dedc70c041d9b;
extern VarInfo __var_info__9114fb7f9b71cef7;
extern VarInfo __var_info__be77a21cf6c882be;
extern VarInfo __var_info__4777ccaf68ef0cb9;
extern VarInfo __var_info__647a3ae14f674180;
extern VarInfo __var_info__2a804375e6080a20;
extern VarInfo __var_info__6838d74b653b1de4;
extern VarInfo __var_info__d0bef68cc9ed3938;
extern VarInfo __var_info__5f7eac88f2e9604f;
extern VarInfo __var_info__3cc519cae3bd693d;
extern VarInfo __var_info__ac5e73a504cb41ed;
extern VarInfo __var_info__a78a6e81bc3fbb28;
extern VarInfo __var_info__af4f0ab9d53a367b;
extern VarInfo __var_info__1e5bec5fc1899531;
extern VarInfo __var_info__1e47de5fc16d47b9;
extern VarInfo __var_info__d97ffc052624b889;
extern VarInfo __var_info__c7ea840626f63c1;
extern VarInfo __var_info__e1b49e90afbc1255;
extern VarInfo __var_info__63c1cccc6cee5499;
extern VarInfo __var_info__c77126596c8747;
extern VarInfo __var_info__25d71217b4b05b4e;
extern VarInfo __var_info__d4a5339397eac5e0;
extern VarInfo __var_info__d1ed822fbff03bdb;
extern VarInfo __var_info__9bee77cb7d793cef;
extern VarInfo __var_info__9af8664b168e269b;
extern VarInfo __var_info__1b53dfe1ddf2e94b;
extern VarInfo __var_info__1b45ebe1de09663f;
extern VarInfo __var_info__513500e20b7c16ee;
extern VarInfo __var_info__bb4514f284b9061e;
extern VarInfo __var_info__9e5eb39ccbbc5fc1;
extern VarInfo __var_info__ab44ba6ec45d54c5;
extern VarInfo __var_info__10391d132c60dad1;
extern VarInfo __var_info__7e408614dc19391b;
extern VarInfo __var_info__d7a4159cdfc723cb;
extern VarInfo __var_info__9bae04658879e2df;
extern VarInfo __var_info__2abfc3db9ec2782f;
extern VarInfo __var_info__c830b1db8edbd193;
extern VarInfo __var_info__1bec0617ac85373c;
extern VarInfo __var_info__e67d8f86e92f853b;
extern VarInfo __var_info__4673d8a661158f3d;
extern VarInfo __var_info__1be3fd17ac778ff1;
extern VarInfo __var_info__85cca36ea6fbcc90;
extern VarInfo __var_info__9c0927b69bc07969;
extern VarInfo __var_info__6320966b0b0640a2;
extern VarInfo __var_info__693e4aab4203cbdc;
extern VarInfo __var_info__b633a8b508277541;
extern VarInfo __var_info__92ced17140df3d1e;
extern VarInfo __var_info__787a20e2af0b5236;
extern VarInfo __var_info__41af8b9fdecb34d1;
extern VarInfo __var_info__687f23a7934addb9;
extern VarInfo __var_info__687f22a7934adc06;
extern VarInfo __var_info__687f25a7934ae11f;
extern VarInfo __var_info__b49663e3167d3da4;
extern VarInfo __var_info__58974fdf367aae2f;
extern VarInfo __var_info__58c950df36cfa5e2;
extern VarInfo __var_info__58c951df36cfa795;
extern VarInfo __var_info__58c94adf36cf9bb0;
extern VarInfo __var_info__58954cdf36774316;
extern VarInfo __var_info__58c956df36cfb014;
extern VarInfo __var_info__66926de2d45531a2;
extern VarInfo __var_info__8ed6ad645b789f5a;
extern VarInfo __var_info__2f740d876e0b4884;
extern VarInfo __var_info__c90cd369f9edeae2;
extern VarInfo __var_info__b00a8c5695ded2df;
extern VarInfo __var_info__3a856020dab61aeb;
extern VarInfo __var_info__b01c8c5695fd68df;
extern VarInfo __var_info__b01d8c5695ff1bdf;
extern VarInfo __var_info__b0168c5695f336df;
extern VarInfo __var_info__30536220d20ca351;
extern VarInfo __var_info__b0128c5695ec6adf;
extern VarInfo __var_info__9327fdd772b62a8d;
extern VarInfo __var_info__6d0b0e187be89a5b;
extern VarInfo __var_info__4e806adf41d6c94c;
extern VarInfo __var_info__3de2fa17c95c3dd8;
extern VarInfo __var_info__97ad6f4fff6d757d;
extern VarInfo __var_info__2fabb44ba51e2e5e;
extern VarInfo __var_info__6637b74bd3c3f3b5;
extern VarInfo __var_info__7ae6556effbccba2;
extern VarInfo __var_info__b64d98db7365a3bc;
extern VarInfo __var_info__8fedab75f45f3548;
extern VarInfo __var_info__909eed032d467225;
extern VarInfo __var_info__94e97360c60ccbf6;
extern VarInfo __var_info__36f50a17c34dd13b;
extern VarInfo __var_info__3d9cf617c8d6d53f;
extern VarInfo __var_info__8a858cd61a32c4cb;
extern VarInfo __var_info__ae2c7c525b576599;
extern VarInfo __var_info__9b48972a32c92396;
extern VarInfo __var_info__3f94f19fe8e70c5c;
extern VarInfo __var_info__3decee17c9619ad0;
extern VarInfo __var_info__ded7c99ebbf651b5;
extern VarInfo __var_info__8f8258cd44bd5ff3;
extern VarInfo __var_info__d901f373b80ee7d4;
extern VarInfo __var_info__35fb364bd7b96c1;
extern VarInfo __var_info__bb400cf284b07986;
extern VarInfo __var_info__508a3df51e5cd12;
extern VarInfo __var_info__56118e87f8a05c63;
extern VarInfo __var_info__fa613307725ae7f7;
extern VarInfo __var_info__e20633a3c3c146b;
extern VarInfo __var_info__2924f017b7732c03;
extern VarInfo __var_info__8013d1dae5465d78;
extern VarInfo __var_info__12d8f6d9d5dead62;
extern VarInfo __var_info__de85442bf10db318;
extern VarInfo __var_info__a131fea67520578;
extern VarInfo __var_info__ecd0ba0f4b18b3f7;
extern VarInfo __var_info__5e248c54071cc77a;
extern VarInfo __var_info__714ea2a2960ed115;
extern VarInfo __var_info__fba56563aec92e36;
extern VarInfo __var_info__802af21a577dc767;
extern VarInfo __var_info__5a547486d6141cd2;
extern VarInfo __var_info__5c1c9d6e73306336;
extern VarInfo __var_info__659cff134b375236;
extern VarInfo __var_info__ae4dffd12fb114b7;
extern VarInfo __var_info__d9a5db0dc73fa5ea;
extern VarInfo __var_info__bf2468461ba4704f;
extern VarInfo __var_info__ef9dcb85e6a70655;
extern VarInfo __var_info__3de1f017c960402c;
extern VarInfo __var_info__719cd81d827ead49;
extern VarInfo __var_info__54cf37e64cce5f7b;
extern VarInfo __var_info__1c5d80edd6565c9b;
extern VarInfo __var_info__291e0117b76d2a38;
extern VarInfo __var_info__76f6080637f13298;
extern VarInfo __var_info__935c60ae663c7fbd;
extern VarInfo __var_info__875dd885be78b606;
extern VarInfo __var_info__e1bc43177ac75487;
extern VarInfo __var_info__e1bc44177ac7563a;
extern VarInfo __var_info__83ad1aadc60656d3;
extern VarInfo __var_info__e1bc45177ac757ed;
extern VarInfo __var_info__f69a5d2f9d815984;
extern VarInfo __var_info__849049adc4f60f2c;
extern VarInfo __var_info__24fe72f324ea6945;
extern VarInfo __var_info__b71488fdfe239082;
extern VarInfo __var_info__4c14504e30d83a91;
extern VarInfo __var_info__24d0d9d58a135865;
extern VarInfo __var_info__913260380c2b4728;
extern VarInfo __var_info__8601634e62501b9d;
extern VarInfo __var_info__f75f24c28f1b3458;
extern VarInfo __var_info__aa7e0339be21b2b1;
extern VarInfo __var_info__610ec238d014a794;
extern VarInfo __var_info__7d33e7a7e5224f55;
extern VarInfo __var_info__c265e47ae7854c9f;
extern VarInfo __var_info__1cb076b2ecd4d8b0;
extern VarInfo __var_info__705cf6b886595b4b;
extern VarInfo __var_info__3be14e162d5565e2;
extern VarInfo __var_info__1f22b71f3af1edea;
extern VarInfo __var_info__1bddf117ac2ae0de;
extern VarInfo __var_info__7e4376fd81e49668;
extern VarInfo __var_info__acd5581d0da14f8c;
extern VarInfo __var_info__3050151b981a6ab9;
extern VarInfo __var_info__840e4ecfb5fc51ee;
extern VarInfo __var_info__6c2c31cfa17d76e4;
extern VarInfo __var_info__bd371c66c9eb6817;
extern VarInfo __var_info__1bd6f617ac24caaf;
extern VarInfo __var_info__a3ecbb5c5601e106;
extern VarInfo __var_info__8bf20c41cb9dcb96;
extern VarInfo __var_info__36ebee17c3130740;
extern VarInfo __var_info__cf9521ef4364a966;
extern VarInfo __var_info__1d202e605403c2b7;
extern VarInfo __var_info__9f5382e006be621c;
extern VarInfo __var_info__f42cff0ada36ad6f;
extern VarInfo __var_info__ff10808a7d9c2c20;
extern VarInfo __var_info__8638cbc831704766;
extern VarInfo __var_info__16eeb6a9049d5149;
extern VarInfo __var_info__75401d533b922a37;
extern VarInfo __var_info__c9097b5866a3e0c4;
extern VarInfo __var_info__6e537ad87a3c52de;
extern VarInfo __var_info__d98c9c486eb62e7b;
extern VarInfo __var_info__9ff398c70462259;
extern VarInfo __var_info__2e2b343561c670f6;
extern VarInfo __var_info__ac26f19d59c637e6;
extern VarInfo __var_info__53e236a279c289ba;
extern VarInfo __var_info__7151322babbded1f;
extern VarInfo __var_info__4708f19442da76f3;
extern VarInfo __var_info__a1f9f6d18fd4d7d5;
extern VarInfo __var_info__b4668103b07b5523;
extern VarInfo __var_info__a65992bc741b8af8;
extern VarInfo __var_info__5c209af46fdd2c15;
extern VarInfo __var_info__cb1c45c3d6c55cf8;
extern VarInfo __var_info__36db993ec7ea7a;
extern VarInfo __var_info__54415956ccfccbc1;
extern VarInfo __var_info__cb8eaeac168eb2ea;
extern VarInfo __var_info__87b6be5eb760221a;
extern VarInfo __var_info__e3aecf5f0583216d;
extern VarInfo __var_info__a83dd595ed95fc2a;
extern VarInfo __var_info__ed0f26b53887bb93;
extern VarInfo __var_info__ed1cae0006658d0c;
extern VarInfo __var_info__66a857925ac8022;
extern VarInfo __var_info__8e75b6de22c6f733;
extern VarInfo __var_info__a4bc79fe7060d06d;
extern VarInfo __var_info__1fcc98c439158013;
extern VarInfo __var_info__cb0f54b51bca0ffb;
extern VarInfo __var_info__9e9c31b94df55077;
extern VarInfo __var_info__39fa3c745babf5af;
extern VarInfo __var_info__db1a5348466c1d30;
extern VarInfo __var_info__64d639b91cdfa50f;
extern VarInfo __var_info__a85a31b955e81f44;
extern VarInfo __var_info__a5484eb95396c6b4;
extern VarInfo __var_info__c626e1678ad826a7;
extern VarInfo __var_info__394406fa4f784c37;
extern VarInfo __var_info__b28e4055068d1da9;
extern VarInfo __var_info__81b654d5a9b1a63;
extern VarInfo __var_info__98a68a5943f0f33d;
extern VarInfo __var_info__98b48a594408bd3d;
extern VarInfo __var_info__98b38a5944070a3d;
extern VarInfo __var_info__98b28a594405573d;
extern VarInfo __var_info__362111fa4cf2d8c9;
extern VarInfo __var_info__b3448b508f91004d;
extern VarInfo __var_info__361311fa4cdb0ec9;
extern VarInfo __var_info__361211fa4cd95bc9;
extern VarInfo __var_info__360d11fa4cd0dcc9;
extern VarInfo __var_info__afde89508cadd3e7;
extern VarInfo __var_info__361911fa4ce540c9;
extern VarInfo __var_info__57c821fa698a4dfa;
extern VarInfo __var_info__13cd994b0f0035e3;
extern VarInfo __var_info__507d518b0db97e55;
extern VarInfo __var_info__7205cd803dc1e121;
extern VarInfo __var_info__e06027fa03bbe4a7;
extern VarInfo __var_info__ff3683d45826d4e2;
extern VarInfo __var_info__ff4882d45845692f;
extern VarInfo __var_info__ff4881d45845677c;
extern VarInfo __var_info__ff4880d4584565c9;
extern VarInfo __var_info__ff347ed458236663;
extern VarInfo __var_info__ff488cd458457a2d;
extern VarInfo __var_info__62d2cdecf1438563;
extern VarInfo __var_info__2fe53e9df606536f;
extern VarInfo __var_info__99df037537935268;
extern VarInfo __var_info__5a9048b914259a82;
extern VarInfo __var_info__da0848d1c542b6e8;
extern VarInfo __var_info__388469b55a519387;
extern VarInfo __var_info__da3046d543b6c331;
extern VarInfo __var_info__f83d84d58244effd;
extern VarInfo __var_info__b5f928184266f21d;
extern VarInfo __var_info__a2b831c732d6acbf;
extern VarInfo __var_info__91de39c724a41957;
extern VarInfo __var_info__ab95dc95f061670f;
extern VarInfo __var_info__f168c75805c4fb50;
extern VarInfo __var_info__651f4fab660392b6;
extern VarInfo __var_info__545432cdd4dc2520;
extern VarInfo __var_info__531fb2668c589df9;
extern VarInfo __var_info__de632c57cd0373a4;
extern VarInfo __var_info__8d0dd095d67cabab;
extern VarInfo __var_info__75a913e04fde93c1;
extern VarInfo __var_info__4363e653f254a75;
extern VarInfo __var_info__66db28ab3cb9062c;
extern VarInfo __var_info__9793e695dfb5080d;
extern VarInfo __var_info__6c8f59f18418b3bc;
extern VarInfo __var_info__b6140d01f1004992;
extern VarInfo __var_info__873b34d6be33396e;
extern VarInfo __var_info__53a75570ca4525d5;
extern VarInfo __var_info__7d442e914c89b521;
extern VarInfo __var_info__8f0ca4d12f4ac744;
extern VarInfo __var_info__2428fc8af625caf9;
extern VarInfo __var_info__249adeffe7dcc8ba;
extern VarInfo __var_info__772cf63b3763b9c9;
extern VarInfo __var_info__db4dcb4495d6d4e4;
extern VarInfo __var_info__e5b0c4527f7128e0;
extern VarInfo __var_info__24b82bf744c33bd5;
extern VarInfo __var_info__d5eb41fc6778c2f8;
extern VarInfo __var_info__291e70e95a4218dc;
extern VarInfo __var_info__279f9f292b04892f;
extern VarInfo __var_info__9c214ae6b6cc6c5;
extern VarInfo __var_info__86763caed6701a1e;
extern VarInfo __var_info__a3ff2104b7343583;
extern VarInfo __var_info__89e2f19de6bd36fc;
extern VarInfo __var_info__9080e695d976110d;
extern VarInfo __var_info__6eef9939275ad076;
extern VarInfo __var_info__e8030adc2c0c141d;
extern VarInfo __var_info__93cad195dc29825e;
extern VarInfo __var_info__93cbd195dc2b355e;
extern VarInfo __var_info__93ccd195dc2ce85e;
extern VarInfo __var_info__df88f56654c4aef7;
extern VarInfo __var_info__91204a10cb25102b;
extern VarInfo __var_info__9afaabd3d7120e7b;
extern VarInfo __var_info__fc85adf5f839389b;
extern VarInfo __var_info__aa5e7310d89ef683;
extern VarInfo __var_info__51c6959eb7d8ecb2;
extern VarInfo __var_info__c8ae6f2d983e7f9b;
extern VarInfo __var_info__96df0db0ae527f88;
extern VarInfo __var_info__d0144d3d83af2c09;
extern VarInfo __var_info__abb18b0074bbd10d;
extern VarInfo __var_info__f62d36f76c2e2809;
extern VarInfo __var_info__727047bcfbe30c92;
extern VarInfo __var_info__f934d2ee83d4bb41;
extern VarInfo __var_info__3fdedde6419b6d01;
extern VarInfo __var_info__e870e29623d0b041;
extern VarInfo __var_info__da0852cf1cefef85;
extern VarInfo __var_info__ae7b494b958cfd1d;
extern VarInfo __var_info__cc26468d7f49e993;
extern VarInfo __var_info__e3bf0919c4d907a6;
extern VarInfo __var_info__e195e2961df0e141;
extern VarInfo __var_info__ed72541b7ccaa3ae;
extern VarInfo __var_info__a5bd2e1b3f914df3;
extern VarInfo __var_info__40d931ea60f18bbf;
extern VarInfo __var_info__a77ccea6ca1951d8;
extern VarInfo __var_info__22cdbe72e3c2d697;
extern VarInfo __var_info__8e0815d127579fcf;
extern VarInfo __var_info__d436bbf954ba2085;
extern VarInfo __var_info__46a59fb09c5853e3;
extern VarInfo __var_info__17e6083f17fb7dd1;
extern VarInfo __var_info__f0ce9e1e38a98150;
extern VarInfo __var_info__86031d69a049c796;
extern VarInfo __var_info__fe693f20b8ceb469;
extern VarInfo __var_info__ada4f71ee74db4e3;
extern VarInfo __var_info__e9b1cddb87221d62;
extern VarInfo __var_info__dcd63514f66ab091;
extern VarInfo __var_info__81c83a50daf2259c;
extern VarInfo __var_info__5bb638a4cd1f16c7;

VarInfo __struct_info__1e8db4ddc1444e12_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0xfb898b509e28d9c8), "__rtti", offsetof(ast::AstVisitor,__rtti), 306 };
TypeInfo * __type_info__404fbd8651779228_arg_types_var_2201614657377291794[1] = { &__type_info__21586ce84f433a21 };
const char * __type_info__404fbd8651779228_arg_names_var_2201614657377291794[1] = { "self" };
VarInfo __struct_info__1e8db4ddc1444e12_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__404fbd8651779228_arg_types_var_2201614657377291794, __type_info__404fbd8651779228_arg_names_var_2201614657377291794, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor))>::size, UINT64_C(0x404fbd8651779228), "__finalize", offsetof(ast::AstVisitor,__finalize), 0 };
TypeInfo * __type_info__571af9efe83812fc_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890 };
const char * __type_info__571af9efe83812fc_arg_names_var_2201614657377291794[2] = { "self", "prog" };
VarInfo __struct_info__1e8db4ddc1444e12_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__571af9efe83812fc_arg_types_var_2201614657377291794, __type_info__571af9efe83812fc_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ))>::size, UINT64_C(0x571af9efe83812fc), "preVisitProgram", offsetof(ast::AstVisitor,preVisitProgram), 0 };
TypeInfo * __type_info__ebde9918bcd35380_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890 };
const char * __type_info__ebde9918bcd35380_arg_names_var_2201614657377291794[2] = { "self", "porg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ebde9918bcd35380_arg_types_var_2201614657377291794, __type_info__ebde9918bcd35380_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ))>::size, UINT64_C(0xebde9918bcd35380), "visitProgram", offsetof(ast::AstVisitor,visitProgram), 0 };
TypeInfo * __type_info__5c630a811e350e10_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802 };
const char * __type_info__5c630a811e350e10_arg_names_var_2201614657377291794[3] = { "self", "prog", "mod" };
VarInfo __struct_info__1e8db4ddc1444e12_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5c630a811e350e10_arg_types_var_2201614657377291794, __type_info__5c630a811e350e10_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ,Module * const ))>::size, UINT64_C(0x5c630a811e350e10), "preVisitProgramBody", offsetof(ast::AstVisitor,preVisitProgramBody), 0 };
TypeInfo * __type_info__47802d686ce7346b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e266b5ccef058802 };
const char * __type_info__47802d686ce7346b_arg_names_var_2201614657377291794[2] = { "self", "mod" };
VarInfo __struct_info__1e8db4ddc1444e12_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__47802d686ce7346b_arg_types_var_2201614657377291794, __type_info__47802d686ce7346b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,Module * const ))>::size, UINT64_C(0x47802d686ce7346b), "preVisitModule", offsetof(ast::AstVisitor,preVisitModule), 0 };
TypeInfo * __type_info__5ccfb39084cfbf8d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e266b5ccef058802 };
const char * __type_info__5ccfb39084cfbf8d_arg_names_var_2201614657377291794[2] = { "self", "mod" };
VarInfo __struct_info__1e8db4ddc1444e12_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5ccfb39084cfbf8d_arg_types_var_2201614657377291794, __type_info__5ccfb39084cfbf8d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,Module * const ))>::size, UINT64_C(0x5ccfb39084cfbf8d), "visitModule", offsetof(ast::AstVisitor,visitModule), 0 };
TypeInfo * __type_info__7c1c3f2d00ff5aa8_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__3b037c8d587730b0 };
const char * __type_info__7c1c3f2d00ff5aa8_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7c1c3f2d00ff5aa8_arg_types_var_2201614657377291794, __type_info__7c1c3f2d00ff5aa8_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const ))>::size, UINT64_C(0x7c1c3f2d00ff5aa8), "preVisitExprTypeDecl", offsetof(ast::AstVisitor,preVisitExprTypeDecl), 0 };
TypeInfo * __type_info__ae2e6c616c88a62b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__3b037c8d587730b0 };
const char * __type_info__ae2e6c616c88a62b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ae2e6c616c88a62b_arg_types_var_2201614657377291794, __type_info__ae2e6c616c88a62b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const ))>::size, UINT64_C(0xae2e6c616c88a62b), "visitExprTypeDecl", offsetof(ast::AstVisitor,visitExprTypeDecl), 0 };
TypeInfo * __type_info__d80a645b6da8c535_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__defb2f7795e0cf8c };
const char * __type_info__d80a645b6da8c535_arg_names_var_2201614657377291794[2] = { "self", "typ" };
VarInfo __struct_info__1e8db4ddc1444e12_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d80a645b6da8c535_arg_types_var_2201614657377291794, __type_info__d80a645b6da8c535_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ))>::size, UINT64_C(0xd80a645b6da8c535), "preVisitTypeDecl", offsetof(ast::AstVisitor,preVisitTypeDecl), 0 };
TypeInfo * __type_info__7c4e6040573c64f1_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__defb2f7795e0cf8c };
const char * __type_info__7c4e6040573c64f1_arg_names_var_2201614657377291794[2] = { "self", "typ" };
VarInfo __struct_info__1e8db4ddc1444e12_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__afcf203e0d7d50d, nullptr, (TypeInfo **)__type_info__7c4e6040573c64f1_arg_types_var_2201614657377291794, __type_info__7c4e6040573c64f1_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ))>::size, UINT64_C(0x7c4e6040573c64f1), "visitTypeDecl", offsetof(ast::AstVisitor,visitTypeDecl), 0 };
TypeInfo * __type_info__dfaf2e5a156cac5b_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__defb2f7795e0cf8c, &__type_info__624d371c76b25aa4 };
const char * __type_info__dfaf2e5a156cac5b_arg_names_var_2201614657377291794[3] = { "self", "typ", "name" };
VarInfo __struct_info__1e8db4ddc1444e12_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__dfaf2e5a156cac5b_arg_types_var_2201614657377291794, __type_info__dfaf2e5a156cac5b_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const ))>::size, UINT64_C(0xdfaf2e5a156cac5b), "preVisitAlias", offsetof(ast::AstVisitor,preVisitAlias), 0 };
TypeInfo * __type_info__36c4426c738ddad_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__defb2f7795e0cf8c, &__type_info__624d371c76b25aa4 };
const char * __type_info__36c4426c738ddad_arg_names_var_2201614657377291794[3] = { "self", "typ", "name" };
VarInfo __struct_info__1e8db4ddc1444e12_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__afcf203e0d7d50d, nullptr, (TypeInfo **)__type_info__36c4426c738ddad_arg_types_var_2201614657377291794, __type_info__36c4426c738ddad_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const ))>::size, UINT64_C(0x36c4426c738ddad), "visitAlias", offsetof(ast::AstVisitor,visitAlias), 0 };
TypeInfo * __type_info__7c71c3b025ab6c55_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7c61f7ae88617bb2 };
const char * __type_info__7c71c3b025ab6c55_arg_names_var_2201614657377291794[2] = { "self", "arg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__7c71c3b025ab6c55_arg_types_var_2201614657377291794, __type_info__7c71c3b025ab6c55_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Enumeration * const ))>::size, UINT64_C(0x7c71c3b025ab6c55), "canVisitEnumeration", offsetof(ast::AstVisitor,canVisitEnumeration), 0 };
TypeInfo * __type_info__139f4dd326162ccb_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__586f0da79a6e613d };
const char * __type_info__139f4dd326162ccb_arg_names_var_2201614657377291794[2] = { "self", "enu" };
VarInfo __struct_info__1e8db4ddc1444e12_field_14 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__139f4dd326162ccb_arg_types_var_2201614657377291794, __type_info__139f4dd326162ccb_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const ))>::size, UINT64_C(0x139f4dd326162ccb), "preVisitEnumeration", offsetof(ast::AstVisitor,preVisitEnumeration), 0 };
TypeInfo * __type_info__7c24884234a37391_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__586f0da79a6e613d, &__type_info__624d371c76b25aa4, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__7c24884234a37391_arg_names_var_2201614657377291794[5] = { "self", "enu", "name", "value", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_15 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7c24884234a37391_arg_types_var_2201614657377291794, __type_info__7c24884234a37391_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x7c24884234a37391), "preVisitEnumerationValue", offsetof(ast::AstVisitor,preVisitEnumerationValue), 0 };
TypeInfo * __type_info__eaf6264c8c2101ff_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__586f0da79a6e613d, &__type_info__624d371c76b25aa4, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__eaf6264c8c2101ff_arg_names_var_2201614657377291794[5] = { "self", "enu", "name", "value", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_16 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__eaf6264c8c2101ff_arg_types_var_2201614657377291794, __type_info__eaf6264c8c2101ff_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xeaf6264c8c2101ff), "visitEnumerationValue", offsetof(ast::AstVisitor,visitEnumerationValue), 0 };
TypeInfo * __type_info__6cad416db0286952_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__586f0da79a6e613d };
const char * __type_info__6cad416db0286952_arg_names_var_2201614657377291794[2] = { "self", "enu" };
VarInfo __struct_info__1e8db4ddc1444e12_field_17 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__e4765bc563f255e, nullptr, (TypeInfo **)__type_info__6cad416db0286952_arg_types_var_2201614657377291794, __type_info__6cad416db0286952_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Enumeration>,ast::AstVisitor,smart_ptr_raw<Enumeration> const ))>::size, UINT64_C(0x6cad416db0286952), "visitEnumeration", offsetof(ast::AstVisitor,visitEnumeration), 0 };
TypeInfo * __type_info__b9386f8f015bfb1e_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f9220d94c6b964b5 };
const char * __type_info__b9386f8f015bfb1e_arg_names_var_2201614657377291794[2] = { "self", "arg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_18 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__b9386f8f015bfb1e_arg_types_var_2201614657377291794, __type_info__b9386f8f015bfb1e_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Structure * const ))>::size, UINT64_C(0xb9386f8f015bfb1e), "canVisitStructure", offsetof(ast::AstVisitor,canVisitStructure), 0 };
TypeInfo * __type_info__41fcc1ce9520d068_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27 };
const char * __type_info__41fcc1ce9520d068_arg_names_var_2201614657377291794[2] = { "self", "str" };
VarInfo __struct_info__1e8db4ddc1444e12_field_19 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__41fcc1ce9520d068_arg_types_var_2201614657377291794, __type_info__41fcc1ce9520d068_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ))>::size, UINT64_C(0x41fcc1ce9520d068), "preVisitStructure", offsetof(ast::AstVisitor,preVisitStructure), 0 };
TypeInfo * __type_info__b391f5eabc24c0ed_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27, &__type_info__29c0090cdbf7525c, &__type_info__af81fe4c86352052 };
const char * __type_info__b391f5eabc24c0ed_arg_names_var_2201614657377291794[4] = { "self", "str", "decl", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_20 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b391f5eabc24c0ed_arg_types_var_2201614657377291794, __type_info__b391f5eabc24c0ed_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool))>::size, UINT64_C(0xb391f5eabc24c0ed), "preVisitStructureField", offsetof(ast::AstVisitor,preVisitStructureField), 0 };
TypeInfo * __type_info__8941a79636fd28ed_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27 };
const char * __type_info__8941a79636fd28ed_arg_names_var_2201614657377291794[2] = { "self", "st" };
VarInfo __struct_info__1e8db4ddc1444e12_field_21 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__8941a79636fd28ed_arg_types_var_2201614657377291794, __type_info__8941a79636fd28ed_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<Structure> const ))>::size, UINT64_C(0x8941a79636fd28ed), "canVisitStructureFieldInit", offsetof(ast::AstVisitor,canVisitStructureFieldInit), 0 };
TypeInfo * __type_info__7e3ddf73033bf97a_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27, &__type_info__29c0090cdbf7525c, &__type_info__af81fe4c86352052 };
const char * __type_info__7e3ddf73033bf97a_arg_names_var_2201614657377291794[4] = { "self", "str", "decl", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_22 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7e3ddf73033bf97a_arg_types_var_2201614657377291794, __type_info__7e3ddf73033bf97a_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool))>::size, UINT64_C(0x7e3ddf73033bf97a), "visitStructureField", offsetof(ast::AstVisitor,visitStructureField), 0 };
TypeInfo * __type_info__c66a4f95fccc7a5f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27 };
const char * __type_info__c66a4f95fccc7a5f_arg_names_var_2201614657377291794[2] = { "self", "str" };
VarInfo __struct_info__1e8db4ddc1444e12_field_23 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__7e104fcf0cd430e4, nullptr, (TypeInfo **)__type_info__c66a4f95fccc7a5f_arg_types_var_2201614657377291794, __type_info__c66a4f95fccc7a5f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Structure>,ast::AstVisitor,smart_ptr_raw<Structure> const ))>::size, UINT64_C(0xc66a4f95fccc7a5f), "visitStructure", offsetof(ast::AstVisitor,visitStructure), 0 };
TypeInfo * __type_info__9bf7e6bedd4bf03b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__34b7c04894c15d5 };
const char * __type_info__9bf7e6bedd4bf03b_arg_names_var_2201614657377291794[2] = { "self", "fun" };
VarInfo __struct_info__1e8db4ddc1444e12_field_24 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__9bf7e6bedd4bf03b_arg_types_var_2201614657377291794, __type_info__9bf7e6bedd4bf03b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Function * const ))>::size, UINT64_C(0x9bf7e6bedd4bf03b), "canVisitFunction", offsetof(ast::AstVisitor,canVisitFunction), 0 };
TypeInfo * __type_info__4d2aa20c479aea6a_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__34b7c04894c15d5, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__4d2aa20c479aea6a_arg_names_var_2201614657377291794[4] = { "self", "fun", "arg", "value" };
VarInfo __struct_info__1e8db4ddc1444e12_field_25 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__4d2aa20c479aea6a_arg_types_var_2201614657377291794, __type_info__4d2aa20c479aea6a_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Function * const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x4d2aa20c479aea6a), "canVisitFunctionArgumentInit", offsetof(ast::AstVisitor,canVisitFunctionArgumentInit), 0 };
TypeInfo * __type_info__9cd5a85ccfeceec9_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0 };
const char * __type_info__9cd5a85ccfeceec9_arg_names_var_2201614657377291794[2] = { "self", "fun" };
VarInfo __struct_info__1e8db4ddc1444e12_field_26 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9cd5a85ccfeceec9_arg_types_var_2201614657377291794, __type_info__9cd5a85ccfeceec9_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ))>::size, UINT64_C(0x9cd5a85ccfeceec9), "preVisitFunction", offsetof(ast::AstVisitor,preVisitFunction), 0 };
TypeInfo * __type_info__c86ad23f08ba4d61_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0 };
const char * __type_info__c86ad23f08ba4d61_arg_names_var_2201614657377291794[2] = { "self", "fun" };
VarInfo __struct_info__1e8db4ddc1444e12_field_27 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4cdbed951d30a5d1, nullptr, (TypeInfo **)__type_info__c86ad23f08ba4d61_arg_types_var_2201614657377291794, __type_info__c86ad23f08ba4d61_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Function>,ast::AstVisitor,smart_ptr_raw<Function> const ))>::size, UINT64_C(0xc86ad23f08ba4d61), "visitFunction", offsetof(ast::AstVisitor,visitFunction), 0 };
TypeInfo * __type_info__5c978d0137240e4a_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__5c978d0137240e4a_arg_names_var_2201614657377291794[4] = { "self", "fun", "arg", "lastArg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_28 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5c978d0137240e4a_arg_types_var_2201614657377291794, __type_info__5c978d0137240e4a_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x5c978d0137240e4a), "preVisitFunctionArgument", offsetof(ast::AstVisitor,preVisitFunctionArgument), 0 };
TypeInfo * __type_info__af15ebc7492e3415_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__af15ebc7492e3415_arg_names_var_2201614657377291794[4] = { "self", "fun", "arg", "lastArg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_29 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__af15ebc7492e3415_arg_types_var_2201614657377291794, __type_info__af15ebc7492e3415_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xaf15ebc7492e3415), "visitFunctionArgument", offsetof(ast::AstVisitor,visitFunctionArgument), 0 };
TypeInfo * __type_info__eec6a05fd6476150_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__eec6a05fd6476150_arg_names_var_2201614657377291794[4] = { "self", "fun", "arg", "value" };
VarInfo __struct_info__1e8db4ddc1444e12_field_30 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__eec6a05fd6476150_arg_types_var_2201614657377291794, __type_info__eec6a05fd6476150_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xeec6a05fd6476150), "preVisitFunctionArgumentInit", offsetof(ast::AstVisitor,preVisitFunctionArgumentInit), 0 };
TypeInfo * __type_info__101482393405a9cf_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__101482393405a9cf_arg_names_var_2201614657377291794[4] = { "self", "fun", "arg", "value" };
VarInfo __struct_info__1e8db4ddc1444e12_field_31 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__101482393405a9cf_arg_types_var_2201614657377291794, __type_info__101482393405a9cf_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x101482393405a9cf), "visitFunctionArgumentInit", offsetof(ast::AstVisitor,visitFunctionArgumentInit), 0 };
TypeInfo * __type_info__a3c16915074de26f_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__98064c57b4bcca5a };
const char * __type_info__a3c16915074de26f_arg_names_var_2201614657377291794[3] = { "self", "fun", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_32 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a3c16915074de26f_arg_types_var_2201614657377291794, __type_info__a3c16915074de26f_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xa3c16915074de26f), "preVisitFunctionBody", offsetof(ast::AstVisitor,preVisitFunctionBody), 0 };
TypeInfo * __type_info__2d7207ab2cb8029_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__98064c57b4bcca5a };
const char * __type_info__2d7207ab2cb8029_arg_names_var_2201614657377291794[3] = { "self", "fun", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_33 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__2d7207ab2cb8029_arg_types_var_2201614657377291794, __type_info__2d7207ab2cb8029_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x2d7207ab2cb8029), "visitFunctionBody", offsetof(ast::AstVisitor,visitFunctionBody), 0 };
TypeInfo * __type_info__d087bf88dadfc976_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__98064c57b4bcca5a };
const char * __type_info__d087bf88dadfc976_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_34 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d087bf88dadfc976_arg_types_var_2201614657377291794, __type_info__d087bf88dadfc976_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xd087bf88dadfc976), "preVisitExpression", offsetof(ast::AstVisitor,preVisitExpression), 0 };
TypeInfo * __type_info__4782749160e23df2_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__98064c57b4bcca5a };
const char * __type_info__4782749160e23df2_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_35 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__4782749160e23df2_arg_types_var_2201614657377291794, __type_info__4782749160e23df2_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x4782749160e23df2), "visitExpression", offsetof(ast::AstVisitor,visitExpression), 0 };
TypeInfo * __type_info__e4da73044655d61b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7 };
const char * __type_info__e4da73044655d61b_arg_names_var_2201614657377291794[2] = { "self", "blk" };
VarInfo __struct_info__1e8db4ddc1444e12_field_36 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e4da73044655d61b_arg_types_var_2201614657377291794, __type_info__e4da73044655d61b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::size, UINT64_C(0xe4da73044655d61b), "preVisitExprBlock", offsetof(ast::AstVisitor,preVisitExprBlock), 0 };
TypeInfo * __type_info__29320d51a2c5f4d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7 };
const char * __type_info__29320d51a2c5f4d_arg_names_var_2201614657377291794[2] = { "self", "blk" };
VarInfo __struct_info__1e8db4ddc1444e12_field_37 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__29320d51a2c5f4d_arg_types_var_2201614657377291794, __type_info__29320d51a2c5f4d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::size, UINT64_C(0x29320d51a2c5f4d), "visitExprBlock", offsetof(ast::AstVisitor,visitExprBlock), 0 };
TypeInfo * __type_info__fad133f08b433e3b_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__fad133f08b433e3b_arg_names_var_2201614657377291794[4] = { "self", "blk", "arg", "lastArg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_38 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__fad133f08b433e3b_arg_types_var_2201614657377291794, __type_info__fad133f08b433e3b_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xfad133f08b433e3b), "preVisitExprBlockArgument", offsetof(ast::AstVisitor,preVisitExprBlockArgument), 0 };
TypeInfo * __type_info__e68dd1c7036b46b6_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__e68dd1c7036b46b6_arg_names_var_2201614657377291794[4] = { "self", "blk", "arg", "lastArg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_39 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__e68dd1c7036b46b6_arg_types_var_2201614657377291794, __type_info__e68dd1c7036b46b6_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xe68dd1c7036b46b6), "visitExprBlockArgument", offsetof(ast::AstVisitor,visitExprBlockArgument), 0 };
TypeInfo * __type_info__883c3e97b305c7e1_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__883c3e97b305c7e1_arg_names_var_2201614657377291794[4] = { "self", "blk", "arg", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_40 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__883c3e97b305c7e1_arg_types_var_2201614657377291794, __type_info__883c3e97b305c7e1_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x883c3e97b305c7e1), "preVisitExprBlockArgumentInit", offsetof(ast::AstVisitor,preVisitExprBlockArgumentInit), 0 };
TypeInfo * __type_info__7ece88bde3d9c42c_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__7ece88bde3d9c42c_arg_names_var_2201614657377291794[4] = { "self", "blk", "arg", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_41 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__7ece88bde3d9c42c_arg_types_var_2201614657377291794, __type_info__7ece88bde3d9c42c_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x7ece88bde3d9c42c), "visitExprBlockArgumentInit", offsetof(ast::AstVisitor,visitExprBlockArgumentInit), 0 };
TypeInfo * __type_info__3eb766e159e2db49_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__98064c57b4bcca5a };
const char * __type_info__3eb766e159e2db49_arg_names_var_2201614657377291794[3] = { "self", "blk", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_42 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3eb766e159e2db49_arg_types_var_2201614657377291794, __type_info__3eb766e159e2db49_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x3eb766e159e2db49), "preVisitExprBlockExpression", offsetof(ast::AstVisitor,preVisitExprBlockExpression), 0 };
TypeInfo * __type_info__4625bd7717ba4469_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__98064c57b4bcca5a };
const char * __type_info__4625bd7717ba4469_arg_names_var_2201614657377291794[3] = { "self", "blk", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_43 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__4625bd7717ba4469_arg_types_var_2201614657377291794, __type_info__4625bd7717ba4469_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x4625bd7717ba4469), "visitExprBlockExpression", offsetof(ast::AstVisitor,visitExprBlockExpression), 0 };
TypeInfo * __type_info__9f8350e9c19f8255_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7 };
const char * __type_info__9f8350e9c19f8255_arg_names_var_2201614657377291794[2] = { "self", "blk" };
VarInfo __struct_info__1e8db4ddc1444e12_field_44 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9f8350e9c19f8255_arg_types_var_2201614657377291794, __type_info__9f8350e9c19f8255_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::size, UINT64_C(0x9f8350e9c19f8255), "preVisitExprBlockFinal", offsetof(ast::AstVisitor,preVisitExprBlockFinal), 0 };
TypeInfo * __type_info__8209de0cf0162633_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7 };
const char * __type_info__8209de0cf0162633_arg_names_var_2201614657377291794[2] = { "self", "blk" };
VarInfo __struct_info__1e8db4ddc1444e12_field_45 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8209de0cf0162633_arg_types_var_2201614657377291794, __type_info__8209de0cf0162633_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::size, UINT64_C(0x8209de0cf0162633), "visitExprBlockFinal", offsetof(ast::AstVisitor,visitExprBlockFinal), 0 };
TypeInfo * __type_info__1fbffaa00541e191_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__98064c57b4bcca5a };
const char * __type_info__1fbffaa00541e191_arg_names_var_2201614657377291794[3] = { "self", "blk", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_46 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1fbffaa00541e191_arg_types_var_2201614657377291794, __type_info__1fbffaa00541e191_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x1fbffaa00541e191), "preVisitExprBlockFinalExpression", offsetof(ast::AstVisitor,preVisitExprBlockFinalExpression), 0 };
TypeInfo * __type_info__f6130cfee51f9aa1_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__98064c57b4bcca5a };
const char * __type_info__f6130cfee51f9aa1_arg_names_var_2201614657377291794[3] = { "self", "blk", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_47 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f6130cfee51f9aa1_arg_types_var_2201614657377291794, __type_info__f6130cfee51f9aa1_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xf6130cfee51f9aa1), "visitExprBlockFinalExpression", offsetof(ast::AstVisitor,visitExprBlockFinalExpression), 0 };
TypeInfo * __type_info__5c324202df78f6a9_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f };
const char * __type_info__5c324202df78f6a9_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_48 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5c324202df78f6a9_arg_types_var_2201614657377291794, __type_info__5c324202df78f6a9_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ))>::size, UINT64_C(0x5c324202df78f6a9), "preVisitExprLet", offsetof(ast::AstVisitor,preVisitExprLet), 0 };
TypeInfo * __type_info__9387c6938b8fa5e7_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f };
const char * __type_info__9387c6938b8fa5e7_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_49 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9387c6938b8fa5e7_arg_types_var_2201614657377291794, __type_info__9387c6938b8fa5e7_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ))>::size, UINT64_C(0x9387c6938b8fa5e7), "visitExprLet", offsetof(ast::AstVisitor,visitExprLet), 0 };
TypeInfo * __type_info__b5f04820b8d5b502_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__b5f04820b8d5b502_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "lastArg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_50 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b5f04820b8d5b502_arg_types_var_2201614657377291794, __type_info__b5f04820b8d5b502_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xb5f04820b8d5b502), "preVisitExprLetVariable", offsetof(ast::AstVisitor,preVisitExprLetVariable), 0 };
TypeInfo * __type_info__692a2ee8bd34e45e_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__692a2ee8bd34e45e_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "lastArg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_51 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__692a2ee8bd34e45e_arg_types_var_2201614657377291794, __type_info__692a2ee8bd34e45e_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x692a2ee8bd34e45e), "visitExprLetVariable", offsetof(ast::AstVisitor,visitExprLetVariable), 0 };
TypeInfo * __type_info__86d2782a6bf7b7d0_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__86d2782a6bf7b7d0_arg_names_var_2201614657377291794[4] = { "self", "blk", "arg", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_52 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__86d2782a6bf7b7d0_arg_types_var_2201614657377291794, __type_info__86d2782a6bf7b7d0_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x86d2782a6bf7b7d0), "preVisitExprLetVariableInit", offsetof(ast::AstVisitor,preVisitExprLetVariableInit), 0 };
TypeInfo * __type_info__2aa1ee74f855f5b4_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__2aa1ee74f855f5b4_arg_names_var_2201614657377291794[4] = { "self", "blk", "arg", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_53 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__2aa1ee74f855f5b4_arg_types_var_2201614657377291794, __type_info__2aa1ee74f855f5b4_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x2aa1ee74f855f5b4), "visitExprLetVariableInit", offsetof(ast::AstVisitor,visitExprLetVariableInit), 0 };
TypeInfo * __type_info__b8106945f6e3fc3a_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__79c6e4b278757551 };
const char * __type_info__b8106945f6e3fc3a_arg_names_var_2201614657377291794[2] = { "self", "arg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_54 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__b8106945f6e3fc3a_arg_types_var_2201614657377291794, __type_info__b8106945f6e3fc3a_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Variable * const ))>::size, UINT64_C(0xb8106945f6e3fc3a), "canVisitGlobalVariable", offsetof(ast::AstVisitor,canVisitGlobalVariable), 0 };
TypeInfo * __type_info__a27d86cb8c142849_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890 };
const char * __type_info__a27d86cb8c142849_arg_names_var_2201614657377291794[2] = { "self", "prog" };
VarInfo __struct_info__1e8db4ddc1444e12_field_55 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a27d86cb8c142849_arg_types_var_2201614657377291794, __type_info__a27d86cb8c142849_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ))>::size, UINT64_C(0xa27d86cb8c142849), "preVisitGlobalLet", offsetof(ast::AstVisitor,preVisitGlobalLet), 0 };
TypeInfo * __type_info__6c71ddec41aaaf17_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890 };
const char * __type_info__6c71ddec41aaaf17_arg_names_var_2201614657377291794[2] = { "self", "prog" };
VarInfo __struct_info__1e8db4ddc1444e12_field_56 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6c71ddec41aaaf17_arg_types_var_2201614657377291794, __type_info__6c71ddec41aaaf17_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ))>::size, UINT64_C(0x6c71ddec41aaaf17), "visitGlobalLet", offsetof(ast::AstVisitor,visitGlobalLet), 0 };
TypeInfo * __type_info__5e30630df7d456a2_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__5e30630df7d456a2_arg_names_var_2201614657377291794[3] = { "self", "arg", "lastArg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_57 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5e30630df7d456a2_arg_types_var_2201614657377291794, __type_info__5e30630df7d456a2_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x5e30630df7d456a2), "preVisitGlobalLetVariable", offsetof(ast::AstVisitor,preVisitGlobalLetVariable), 0 };
TypeInfo * __type_info__ab31c0bc9573a50e_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__ab31c0bc9573a50e_arg_names_var_2201614657377291794[3] = { "self", "arg", "lastArg" };
VarInfo __struct_info__1e8db4ddc1444e12_field_58 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__ab31c0bc9573a50e_arg_types_var_2201614657377291794, __type_info__ab31c0bc9573a50e_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xab31c0bc9573a50e), "visitGlobalLetVariable", offsetof(ast::AstVisitor,visitGlobalLetVariable), 0 };
TypeInfo * __type_info__8341a2400990c170_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__8341a2400990c170_arg_names_var_2201614657377291794[3] = { "self", "arg", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_59 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8341a2400990c170_arg_types_var_2201614657377291794, __type_info__8341a2400990c170_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x8341a2400990c170), "preVisitGlobalLetVariableInit", offsetof(ast::AstVisitor,preVisitGlobalLetVariableInit), 0 };
TypeInfo * __type_info__8edfb2a132d8a1a4_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__8edfb2a132d8a1a4_arg_names_var_2201614657377291794[3] = { "self", "arg", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_60 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__8edfb2a132d8a1a4_arg_types_var_2201614657377291794, __type_info__8edfb2a132d8a1a4_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x8edfb2a132d8a1a4), "visitGlobalLetVariableInit", offsetof(ast::AstVisitor,visitGlobalLetVariableInit), 0 };
TypeInfo * __type_info__89002db3d2a382d9_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4e7dff8bb14f539 };
const char * __type_info__89002db3d2a382d9_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_61 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__89002db3d2a382d9_arg_types_var_2201614657377291794, __type_info__89002db3d2a382d9_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ))>::size, UINT64_C(0x89002db3d2a382d9), "preVisitExprStringBuilder", offsetof(ast::AstVisitor,preVisitExprStringBuilder), 0 };
TypeInfo * __type_info__7cb15833a89cbb44_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4e7dff8bb14f539 };
const char * __type_info__7cb15833a89cbb44_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_62 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__7cb15833a89cbb44_arg_types_var_2201614657377291794, __type_info__7cb15833a89cbb44_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ))>::size, UINT64_C(0x7cb15833a89cbb44), "visitExprStringBuilder", offsetof(ast::AstVisitor,visitExprStringBuilder), 0 };
TypeInfo * __type_info__dcd62ab4848d54b8_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__4e7dff8bb14f539, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__dcd62ab4848d54b8_arg_names_var_2201614657377291794[4] = { "self", "expr", "elem", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_63 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__dcd62ab4848d54b8_arg_types_var_2201614657377291794, __type_info__dcd62ab4848d54b8_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xdcd62ab4848d54b8), "preVisitExprStringBuilderElement", offsetof(ast::AstVisitor,preVisitExprStringBuilderElement), 0 };
TypeInfo * __type_info__50599c4cfbbbf1a3_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__4e7dff8bb14f539, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__50599c4cfbbbf1a3_arg_names_var_2201614657377291794[4] = { "self", "expr", "elem", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_64 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__50599c4cfbbbf1a3_arg_types_var_2201614657377291794, __type_info__50599c4cfbbbf1a3_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x50599c4cfbbbf1a3), "visitExprStringBuilderElement", offsetof(ast::AstVisitor,visitExprStringBuilderElement), 0 };
TypeInfo * __type_info__5c394f02df7f2b6e_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__5d6138f13e1e88c4 };
const char * __type_info__5c394f02df7f2b6e_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_65 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5c394f02df7f2b6e_arg_types_var_2201614657377291794, __type_info__5c394f02df7f2b6e_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const ))>::size, UINT64_C(0x5c394f02df7f2b6e), "preVisitExprNew", offsetof(ast::AstVisitor,preVisitExprNew), 0 };
TypeInfo * __type_info__9a7ac69391983ce7_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__5d6138f13e1e88c4 };
const char * __type_info__9a7ac69391983ce7_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_66 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9a7ac69391983ce7_arg_types_var_2201614657377291794, __type_info__9a7ac69391983ce7_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const ))>::size, UINT64_C(0x9a7ac69391983ce7), "visitExprNew", offsetof(ast::AstVisitor,visitExprNew), 0 };
TypeInfo * __type_info__ae3ae56613d11d42_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__5d6138f13e1e88c4, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__ae3ae56613d11d42_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_67 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ae3ae56613d11d42_arg_types_var_2201614657377291794, __type_info__ae3ae56613d11d42_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xae3ae56613d11d42), "preVisitExprNewArgument", offsetof(ast::AstVisitor,preVisitExprNewArgument), 0 };
TypeInfo * __type_info__999aa82fd12dcf8_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__5d6138f13e1e88c4, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__999aa82fd12dcf8_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_68 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__999aa82fd12dcf8_arg_types_var_2201614657377291794, __type_info__999aa82fd12dcf8_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x999aa82fd12dcf8), "visitExprNewArgument", offsetof(ast::AstVisitor,visitExprNewArgument), 0 };
TypeInfo * __type_info__54a28bc44192b9c1_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__44cd26f4cb3df7e1 };
const char * __type_info__54a28bc44192b9c1_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_69 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__54a28bc44192b9c1_arg_types_var_2201614657377291794, __type_info__54a28bc44192b9c1_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ))>::size, UINT64_C(0x54a28bc44192b9c1), "preVisitExprNamedCall", offsetof(ast::AstVisitor,preVisitExprNamedCall), 0 };
TypeInfo * __type_info__b015b3f7761553d5_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__44cd26f4cb3df7e1 };
const char * __type_info__b015b3f7761553d5_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_70 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__b015b3f7761553d5_arg_types_var_2201614657377291794, __type_info__b015b3f7761553d5_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ))>::size, UINT64_C(0xb015b3f7761553d5), "visitExprNamedCall", offsetof(ast::AstVisitor,visitExprNamedCall), 0 };
TypeInfo * __type_info__9d289c0e202c475_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__44cd26f4cb3df7e1, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__9d289c0e202c475_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_71 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9d289c0e202c475_arg_types_var_2201614657377291794, __type_info__9d289c0e202c475_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0x9d289c0e202c475), "preVisitExprNamedCallArgument", offsetof(ast::AstVisitor,preVisitExprNamedCallArgument), 0 };
TypeInfo * __type_info__ec7be13e5039487e_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__44cd26f4cb3df7e1, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__ec7be13e5039487e_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_72 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__c52835f1e7c9ab84, nullptr, (TypeInfo **)__type_info__ec7be13e5039487e_arg_types_var_2201614657377291794, __type_info__ec7be13e5039487e_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0xec7be13e5039487e), "visitExprNamedCallArgument", offsetof(ast::AstVisitor,visitExprNamedCallArgument), 0 };
TypeInfo * __type_info__bbbb051551a66a0a_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a };
const char * __type_info__bbbb051551a66a0a_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_73 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bbbb051551a66a0a_arg_types_var_2201614657377291794, __type_info__bbbb051551a66a0a_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ))>::size, UINT64_C(0xbbbb051551a66a0a), "preVisitExprLooksLikeCall", offsetof(ast::AstVisitor,preVisitExprLooksLikeCall), 0 };
TypeInfo * __type_info__166357f10c39f6fc_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a };
const char * __type_info__166357f10c39f6fc_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_74 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__166357f10c39f6fc_arg_types_var_2201614657377291794, __type_info__166357f10c39f6fc_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ))>::size, UINT64_C(0x166357f10c39f6fc), "visitExprLooksLikeCall", offsetof(ast::AstVisitor,visitExprLooksLikeCall), 0 };
TypeInfo * __type_info__73ec7f5588dcbccc_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__73ec7f5588dcbccc_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_75 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__73ec7f5588dcbccc_arg_types_var_2201614657377291794, __type_info__73ec7f5588dcbccc_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x73ec7f5588dcbccc), "canVisitExprLooksLikeCallArgument", offsetof(ast::AstVisitor,canVisitExprLooksLikeCallArgument), 0 };
TypeInfo * __type_info__29a0d7a9e4b9e5f6_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__29a0d7a9e4b9e5f6_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_76 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__29a0d7a9e4b9e5f6_arg_types_var_2201614657377291794, __type_info__29a0d7a9e4b9e5f6_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x29a0d7a9e4b9e5f6), "preVisitExprLooksLikeCallArgument", offsetof(ast::AstVisitor,preVisitExprLooksLikeCallArgument), 0 };
TypeInfo * __type_info__13e0e5f72f8217fb_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__13e0e5f72f8217fb_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_77 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__13e0e5f72f8217fb_arg_types_var_2201614657377291794, __type_info__13e0e5f72f8217fb_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x13e0e5f72f8217fb), "visitExprLooksLikeCallArgument", offsetof(ast::AstVisitor,visitExprLooksLikeCallArgument), 0 };
TypeInfo * __type_info__802b484819ed0c2d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7e04c1d12891d606 };
const char * __type_info__802b484819ed0c2d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_78 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__802b484819ed0c2d_arg_types_var_2201614657377291794, __type_info__802b484819ed0c2d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,ExprCall * const ))>::size, UINT64_C(0x802b484819ed0c2d), "canVisitCall", offsetof(ast::AstVisitor,canVisitCall), 0 };
TypeInfo * __type_info__4ee15802d484dfce_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7dd1c1d1283b2d06 };
const char * __type_info__4ee15802d484dfce_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_79 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4ee15802d484dfce_arg_types_var_2201614657377291794, __type_info__4ee15802d484dfce_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const ))>::size, UINT64_C(0x4ee15802d484dfce), "preVisitExprCall", offsetof(ast::AstVisitor,preVisitExprCall), 0 };
TypeInfo * __type_info__c51617cedb37de35_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7dd1c1d1283b2d06 };
const char * __type_info__c51617cedb37de35_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_80 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__c51617cedb37de35_arg_types_var_2201614657377291794, __type_info__c51617cedb37de35_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const ))>::size, UINT64_C(0xc51617cedb37de35), "visitExprCall", offsetof(ast::AstVisitor,visitExprCall), 0 };
TypeInfo * __type_info__6a62a96f37358e75_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__7dd1c1d1283b2d06, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__6a62a96f37358e75_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_81 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6a62a96f37358e75_arg_types_var_2201614657377291794, __type_info__6a62a96f37358e75_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x6a62a96f37358e75), "preVisitExprCallArgument", offsetof(ast::AstVisitor,preVisitExprCallArgument), 0 };
TypeInfo * __type_info__e52aaee50baa0521_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__7dd1c1d1283b2d06, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__e52aaee50baa0521_arg_names_var_2201614657377291794[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_82 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__e52aaee50baa0521_arg_types_var_2201614657377291794, __type_info__e52aaee50baa0521_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xe52aaee50baa0521), "visitExprCallArgument", offsetof(ast::AstVisitor,visitExprCallArgument), 0 };
TypeInfo * __type_info__f2f0e741960e1177_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e7e2063b91ac55a1 };
const char * __type_info__f2f0e741960e1177_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_83 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f2f0e741960e1177_arg_types_var_2201614657377291794, __type_info__f2f0e741960e1177_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const ))>::size, UINT64_C(0xf2f0e741960e1177), "preVisitExprNullCoalescing", offsetof(ast::AstVisitor,preVisitExprNullCoalescing), 0 };
TypeInfo * __type_info__c851ce6ee7f3502b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e7e2063b91ac55a1 };
const char * __type_info__c851ce6ee7f3502b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_84 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__c851ce6ee7f3502b_arg_types_var_2201614657377291794, __type_info__c851ce6ee7f3502b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const ))>::size, UINT64_C(0xc851ce6ee7f3502b), "visitExprNullCoalescing", offsetof(ast::AstVisitor,visitExprNullCoalescing), 0 };
TypeInfo * __type_info__be686e26bffa9144_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__e7e2063b91ac55a1, &__type_info__98064c57b4bcca5a };
const char * __type_info__be686e26bffa9144_arg_names_var_2201614657377291794[3] = { "self", "expr", "defval" };
VarInfo __struct_info__1e8db4ddc1444e12_field_85 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__be686e26bffa9144_arg_types_var_2201614657377291794, __type_info__be686e26bffa9144_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xbe686e26bffa9144), "preVisitExprNullCoalescingDefault", offsetof(ast::AstVisitor,preVisitExprNullCoalescingDefault), 0 };
TypeInfo * __type_info__363d68bc53f7e730_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__241df6ccda394202 };
const char * __type_info__363d68bc53f7e730_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_86 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__363d68bc53f7e730_arg_types_var_2201614657377291794, __type_info__363d68bc53f7e730_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ))>::size, UINT64_C(0x363d68bc53f7e730), "preVisitExprAt", offsetof(ast::AstVisitor,preVisitExprAt), 0 };
TypeInfo * __type_info__9dedb59394916004_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__241df6ccda394202 };
const char * __type_info__9dedb59394916004_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_87 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9dedb59394916004_arg_types_var_2201614657377291794, __type_info__9dedb59394916004_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAt> const ))>::size, UINT64_C(0x9dedb59394916004), "visitExprAt", offsetof(ast::AstVisitor,visitExprAt), 0 };
TypeInfo * __type_info__6caf569fc5ada20f_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__241df6ccda394202, &__type_info__98064c57b4bcca5a };
const char * __type_info__6caf569fc5ada20f_arg_names_var_2201614657377291794[3] = { "self", "expr", "index" };
VarInfo __struct_info__1e8db4ddc1444e12_field_88 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6caf569fc5ada20f_arg_types_var_2201614657377291794, __type_info__6caf569fc5ada20f_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x6caf569fc5ada20f), "preVisitExprAtIndex", offsetof(ast::AstVisitor,preVisitExprAtIndex), 0 };
TypeInfo * __type_info__415c6fcc77e22573_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__b5e62a55ec68b6b5 };
const char * __type_info__415c6fcc77e22573_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_89 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__415c6fcc77e22573_arg_types_var_2201614657377291794, __type_info__415c6fcc77e22573_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ))>::size, UINT64_C(0x415c6fcc77e22573), "preVisitExprSafeAt", offsetof(ast::AstVisitor,preVisitExprSafeAt), 0 };
TypeInfo * __type_info__f3484371ddf5f3ea_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__b5e62a55ec68b6b5 };
const char * __type_info__f3484371ddf5f3ea_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_90 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f3484371ddf5f3ea_arg_types_var_2201614657377291794, __type_info__f3484371ddf5f3ea_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ))>::size, UINT64_C(0xf3484371ddf5f3ea), "visitExprSafeAt", offsetof(ast::AstVisitor,visitExprSafeAt), 0 };
TypeInfo * __type_info__452510e5117043d2_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__b5e62a55ec68b6b5, &__type_info__98064c57b4bcca5a };
const char * __type_info__452510e5117043d2_arg_names_var_2201614657377291794[3] = { "self", "expr", "index" };
VarInfo __struct_info__1e8db4ddc1444e12_field_91 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__452510e5117043d2_arg_types_var_2201614657377291794, __type_info__452510e5117043d2_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x452510e5117043d2), "preVisitExprSafeAtIndex", offsetof(ast::AstVisitor,preVisitExprSafeAtIndex), 0 };
TypeInfo * __type_info__363c70bc53f641c8_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__8a5e2edb26418a2a };
const char * __type_info__363c70bc53f641c8_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_92 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__363c70bc53f641c8_arg_types_var_2201614657377291794, __type_info__363c70bc53f641c8_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const ))>::size, UINT64_C(0x363c70bc53f641c8), "preVisitExprIs", offsetof(ast::AstVisitor,preVisitExprIs), 0 };
TypeInfo * __type_info__82bdb4937d781651_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__8a5e2edb26418a2a };
const char * __type_info__82bdb4937d781651_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_93 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__82bdb4937d781651_arg_types_var_2201614657377291794, __type_info__82bdb4937d781651_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIs> const ))>::size, UINT64_C(0x82bdb4937d781651), "visitExprIs", offsetof(ast::AstVisitor,visitExprIs), 0 };
TypeInfo * __type_info__eda32c89381565ec_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__8a5e2edb26418a2a, &__type_info__defb2f7795e0cf8c };
const char * __type_info__eda32c89381565ec_arg_names_var_2201614657377291794[3] = { "self", "expr", "typeDecl" };
VarInfo __struct_info__1e8db4ddc1444e12_field_94 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__eda32c89381565ec_arg_types_var_2201614657377291794, __type_info__eda32c89381565ec_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const ,smart_ptr_raw<TypeDecl> const ))>::size, UINT64_C(0xeda32c89381565ec), "preVisitExprIsType", offsetof(ast::AstVisitor,preVisitExprIsType), 0 };
TypeInfo * __type_info__14d78a02a2d949d8_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c5915ffba474f7fe };
const char * __type_info__14d78a02a2d949d8_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_95 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__14d78a02a2d949d8_arg_types_var_2201614657377291794, __type_info__14d78a02a2d949d8_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const ))>::size, UINT64_C(0x14d78a02a2d949d8), "preVisitExprOp2", offsetof(ast::AstVisitor,preVisitExprOp2), 0 };
TypeInfo * __type_info__974fb1938f193138_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c5915ffba474f7fe };
const char * __type_info__974fb1938f193138_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_96 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__974fb1938f193138_arg_types_var_2201614657377291794, __type_info__974fb1938f193138_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp2> const ))>::size, UINT64_C(0x974fb1938f193138), "visitExprOp2", offsetof(ast::AstVisitor,visitExprOp2), 0 };
TypeInfo * __type_info__2bce2565f2557691_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__c5915ffba474f7fe, &__type_info__98064c57b4bcca5a };
const char * __type_info__2bce2565f2557691_arg_names_var_2201614657377291794[3] = { "self", "expr", "right" };
VarInfo __struct_info__1e8db4ddc1444e12_field_97 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2bce2565f2557691_arg_types_var_2201614657377291794, __type_info__2bce2565f2557691_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x2bce2565f2557691), "preVisitExprOp2Right", offsetof(ast::AstVisitor,preVisitExprOp2Right), 0 };
TypeInfo * __type_info__14d78b02a2d94b8b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__ca2136fbaac99425 };
const char * __type_info__14d78b02a2d94b8b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_98 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__14d78b02a2d94b8b_arg_types_var_2201614657377291794, __type_info__14d78b02a2d94b8b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ))>::size, UINT64_C(0x14d78b02a2d94b8b), "preVisitExprOp3", offsetof(ast::AstVisitor,preVisitExprOp3), 0 };
TypeInfo * __type_info__974eb1938f177e38_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__ca2136fbaac99425 };
const char * __type_info__974eb1938f177e38_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_99 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__974eb1938f177e38_arg_types_var_2201614657377291794, __type_info__974eb1938f177e38_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ))>::size, UINT64_C(0x974eb1938f177e38), "visitExprOp3", offsetof(ast::AstVisitor,visitExprOp3), 0 };
TypeInfo * __type_info__fab868f914151ffa_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__ca2136fbaac99425, &__type_info__98064c57b4bcca5a };
const char * __type_info__fab868f914151ffa_arg_names_var_2201614657377291794[3] = { "self", "expr", "left" };
VarInfo __struct_info__1e8db4ddc1444e12_field_100 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__fab868f914151ffa_arg_types_var_2201614657377291794, __type_info__fab868f914151ffa_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xfab868f914151ffa), "preVisitExprOp3Left", offsetof(ast::AstVisitor,preVisitExprOp3Left), 0 };
TypeInfo * __type_info__28a45465ed4640ea_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__ca2136fbaac99425, &__type_info__98064c57b4bcca5a };
const char * __type_info__28a45465ed4640ea_arg_names_var_2201614657377291794[3] = { "self", "expr", "right" };
VarInfo __struct_info__1e8db4ddc1444e12_field_101 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__28a45465ed4640ea_arg_types_var_2201614657377291794, __type_info__28a45465ed4640ea_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x28a45465ed4640ea), "preVisitExprOp3Right", offsetof(ast::AstVisitor,preVisitExprOp3Right), 0 };
TypeInfo * __type_info__2d81bedb489517e7_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__350b375c34e0c48a };
const char * __type_info__2d81bedb489517e7_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_102 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__2d81bedb489517e7_arg_types_var_2201614657377291794, __type_info__2d81bedb489517e7_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ))>::size, UINT64_C(0x2d81bedb489517e7), "isRightFirstExprCopy", offsetof(ast::AstVisitor,isRightFirstExprCopy), 0 };
TypeInfo * __type_info__7df04402fc0efcd2_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__350b375c34e0c48a };
const char * __type_info__7df04402fc0efcd2_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_103 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7df04402fc0efcd2_arg_types_var_2201614657377291794, __type_info__7df04402fc0efcd2_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ))>::size, UINT64_C(0x7df04402fc0efcd2), "preVisitExprCopy", offsetof(ast::AstVisitor,preVisitExprCopy), 0 };
TypeInfo * __type_info__9c4706ceb88bb6a4_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__350b375c34e0c48a };
const char * __type_info__9c4706ceb88bb6a4_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_104 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9c4706ceb88bb6a4_arg_types_var_2201614657377291794, __type_info__9c4706ceb88bb6a4_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ))>::size, UINT64_C(0x9c4706ceb88bb6a4), "visitExprCopy", offsetof(ast::AstVisitor,visitExprCopy), 0 };
TypeInfo * __type_info__cd5d0594bbd31183_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__350b375c34e0c48a, &__type_info__98064c57b4bcca5a };
const char * __type_info__cd5d0594bbd31183_arg_names_var_2201614657377291794[3] = { "self", "expr", "right" };
VarInfo __struct_info__1e8db4ddc1444e12_field_105 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__cd5d0594bbd31183_arg_types_var_2201614657377291794, __type_info__cd5d0594bbd31183_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xcd5d0594bbd31183), "preVisitExprCopyRight", offsetof(ast::AstVisitor,preVisitExprCopyRight), 0 };
TypeInfo * __type_info__2c95acdb472c618f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c19751d6d5da74e2 };
const char * __type_info__2c95acdb472c618f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_106 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__2c95acdb472c618f_arg_types_var_2201614657377291794, __type_info__2c95acdb472c618f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMove> const ))>::size, UINT64_C(0x2c95acdb472c618f), "isRightFirstExprMove", offsetof(ast::AstVisitor,isRightFirstExprMove), 0 };
TypeInfo * __type_info__7e083e02fc1adb06_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c19751d6d5da74e2 };
const char * __type_info__7e083e02fc1adb06_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_107 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7e083e02fc1adb06_arg_types_var_2201614657377291794, __type_info__7e083e02fc1adb06_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const ))>::size, UINT64_C(0x7e083e02fc1adb06), "preVisitExprMove", offsetof(ast::AstVisitor,preVisitExprMove), 0 };
TypeInfo * __type_info__fe090ab26f161170_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c19751d6d5da74e2 };
const char * __type_info__fe090ab26f161170_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_108 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__fe090ab26f161170_arg_types_var_2201614657377291794, __type_info__fe090ab26f161170_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMove> const ))>::size, UINT64_C(0xfe090ab26f161170), "visitExprMove", offsetof(ast::AstVisitor,visitExprMove), 0 };
TypeInfo * __type_info__eca633024932f86f_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__c19751d6d5da74e2, &__type_info__98064c57b4bcca5a };
const char * __type_info__eca633024932f86f_arg_names_var_2201614657377291794[3] = { "self", "expr", "right" };
VarInfo __struct_info__1e8db4ddc1444e12_field_109 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__eca633024932f86f_arg_types_var_2201614657377291794, __type_info__eca633024932f86f_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xeca633024932f86f), "preVisitExprMoveRight", offsetof(ast::AstVisitor,preVisitExprMoveRight), 0 };
TypeInfo * __type_info__e8e0d09fd41dec39_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__93546827b32c5422 };
const char * __type_info__e8e0d09fd41dec39_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_110 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__e8e0d09fd41dec39_arg_types_var_2201614657377291794, __type_info__e8e0d09fd41dec39_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprClone> const ))>::size, UINT64_C(0xe8e0d09fd41dec39), "isRightFirstExprClone", offsetof(ast::AstVisitor,isRightFirstExprClone), 0 };
TypeInfo * __type_info__f5c258045726d71a_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__93546827b32c5422 };
const char * __type_info__f5c258045726d71a_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_111 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f5c258045726d71a_arg_types_var_2201614657377291794, __type_info__f5c258045726d71a_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const ))>::size, UINT64_C(0xf5c258045726d71a), "preVisitExprClone", offsetof(ast::AstVisitor,preVisitExprClone), 0 };
TypeInfo * __type_info__ba7813ced1e54836_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__93546827b32c5422 };
const char * __type_info__ba7813ced1e54836_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_112 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ba7813ced1e54836_arg_types_var_2201614657377291794, __type_info__ba7813ced1e54836_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprClone> const ))>::size, UINT64_C(0xba7813ced1e54836), "visitExprClone", offsetof(ast::AstVisitor,visitExprClone), 0 };
TypeInfo * __type_info__6e8908b5e9b654b3_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__93546827b32c5422, &__type_info__98064c57b4bcca5a };
const char * __type_info__6e8908b5e9b654b3_arg_names_var_2201614657377291794[3] = { "self", "expr", "right" };
VarInfo __struct_info__1e8db4ddc1444e12_field_113 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6e8908b5e9b654b3_arg_types_var_2201614657377291794, __type_info__6e8908b5e9b654b3_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x6e8908b5e9b654b3), "preVisitExprCloneRight", offsetof(ast::AstVisitor,preVisitExprCloneRight), 0 };
TypeInfo * __type_info__591a1931dbf740b7_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__83c768ad9b3f81ea };
const char * __type_info__591a1931dbf740b7_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_114 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__591a1931dbf740b7_arg_types_var_2201614657377291794, __type_info__591a1931dbf740b7_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprAssume> const ))>::size, UINT64_C(0x591a1931dbf740b7), "canVisitWithAliasSubexpression", offsetof(ast::AstVisitor,canVisitWithAliasSubexpression), 0 };
TypeInfo * __type_info__f36af989502c64dc_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__83c768ad9b3f81ea };
const char * __type_info__f36af989502c64dc_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_115 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f36af989502c64dc_arg_types_var_2201614657377291794, __type_info__f36af989502c64dc_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssume> const ))>::size, UINT64_C(0xf36af989502c64dc), "preVisitExprAssume", offsetof(ast::AstVisitor,preVisitExprAssume), 0 };
TypeInfo * __type_info__bd93646199fbaf0b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__83c768ad9b3f81ea };
const char * __type_info__bd93646199fbaf0b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_116 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__bd93646199fbaf0b_arg_types_var_2201614657377291794, __type_info__bd93646199fbaf0b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssume> const ))>::size, UINT64_C(0xbd93646199fbaf0b), "visitExprAssume", offsetof(ast::AstVisitor,visitExprAssume), 0 };
TypeInfo * __type_info__69e44002eb742df2_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c758d466d1a06ae2 };
const char * __type_info__69e44002eb742df2_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_117 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__69e44002eb742df2_arg_types_var_2201614657377291794, __type_info__69e44002eb742df2_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const ))>::size, UINT64_C(0x69e44002eb742df2), "preVisitExprWith", offsetof(ast::AstVisitor,preVisitExprWith), 0 };
TypeInfo * __type_info__c356f39314340551_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c758d466d1a06ae2 };
const char * __type_info__c356f39314340551_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_118 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__c356f39314340551_arg_types_var_2201614657377291794, __type_info__c356f39314340551_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWith> const ))>::size, UINT64_C(0xc356f39314340551), "visitExprWith", offsetof(ast::AstVisitor,visitExprWith), 0 };
TypeInfo * __type_info__f4d0322851dbbd1c_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__c758d466d1a06ae2, &__type_info__98064c57b4bcca5a };
const char * __type_info__f4d0322851dbbd1c_arg_names_var_2201614657377291794[3] = { "self", "expr", "right" };
VarInfo __struct_info__1e8db4ddc1444e12_field_119 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f4d0322851dbbd1c_arg_types_var_2201614657377291794, __type_info__f4d0322851dbbd1c_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xf4d0322851dbbd1c), "preVisitExprWithBody", offsetof(ast::AstVisitor,preVisitExprWithBody), 0 };
TypeInfo * __type_info__146b37efc8a0690c_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9cac32b4050a2fb8 };
const char * __type_info__146b37efc8a0690c_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_120 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__146b37efc8a0690c_arg_types_var_2201614657377291794, __type_info__146b37efc8a0690c_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const ))>::size, UINT64_C(0x146b37efc8a0690c), "preVisitExprWhile", offsetof(ast::AstVisitor,preVisitExprWhile), 0 };
TypeInfo * __type_info__1f78119362cddb74_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9cac32b4050a2fb8 };
const char * __type_info__1f78119362cddb74_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_121 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__1f78119362cddb74_arg_types_var_2201614657377291794, __type_info__1f78119362cddb74_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWhile> const ))>::size, UINT64_C(0x1f78119362cddb74), "visitExprWhile", offsetof(ast::AstVisitor,visitExprWhile), 0 };
TypeInfo * __type_info__1aa99d5f0bdf07c0_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__9cac32b4050a2fb8, &__type_info__98064c57b4bcca5a };
const char * __type_info__1aa99d5f0bdf07c0_arg_names_var_2201614657377291794[3] = { "self", "expr", "right" };
VarInfo __struct_info__1e8db4ddc1444e12_field_122 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1aa99d5f0bdf07c0_arg_types_var_2201614657377291794, __type_info__1aa99d5f0bdf07c0_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x1aa99d5f0bdf07c0), "preVisitExprWhileBody", offsetof(ast::AstVisitor,preVisitExprWhileBody), 0 };
TypeInfo * __type_info__6c007322603a901e_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c1c6f9bc0741f232 };
const char * __type_info__6c007322603a901e_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_123 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6c007322603a901e_arg_types_var_2201614657377291794, __type_info__6c007322603a901e_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const ))>::size, UINT64_C(0x6c007322603a901e), "preVisitExprTryCatch", offsetof(ast::AstVisitor,preVisitExprTryCatch), 0 };
TypeInfo * __type_info__ee20777da5e5bc33_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c1c6f9bc0741f232 };
const char * __type_info__ee20777da5e5bc33_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_124 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ee20777da5e5bc33_arg_types_var_2201614657377291794, __type_info__ee20777da5e5bc33_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const ))>::size, UINT64_C(0xee20777da5e5bc33), "visitExprTryCatch", offsetof(ast::AstVisitor,visitExprTryCatch), 0 };
TypeInfo * __type_info__60013666243ecb3_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__c1c6f9bc0741f232, &__type_info__98064c57b4bcca5a };
const char * __type_info__60013666243ecb3_arg_names_var_2201614657377291794[3] = { "self", "expr", "right" };
VarInfo __struct_info__1e8db4ddc1444e12_field_125 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__60013666243ecb3_arg_types_var_2201614657377291794, __type_info__60013666243ecb3_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x60013666243ecb3), "preVisitExprTryCatchCatch", offsetof(ast::AstVisitor,preVisitExprTryCatchCatch), 0 };
TypeInfo * __type_info__c51c17bf11ebed33_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__88db72c3eb8c93b1 };
const char * __type_info__c51c17bf11ebed33_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_126 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c51c17bf11ebed33_arg_types_var_2201614657377291794, __type_info__c51c17bf11ebed33_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ))>::size, UINT64_C(0xc51c17bf11ebed33), "preVisitExprIfThenElse", offsetof(ast::AstVisitor,preVisitExprIfThenElse), 0 };
TypeInfo * __type_info__38c0b8b3a0035f4b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__88db72c3eb8c93b1 };
const char * __type_info__38c0b8b3a0035f4b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_127 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__38c0b8b3a0035f4b_arg_types_var_2201614657377291794, __type_info__38c0b8b3a0035f4b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ))>::size, UINT64_C(0x38c0b8b3a0035f4b), "visitExprIfThenElse", offsetof(ast::AstVisitor,visitExprIfThenElse), 0 };
TypeInfo * __type_info__c099ede2b5923b6e_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__88db72c3eb8c93b1, &__type_info__98064c57b4bcca5a };
const char * __type_info__c099ede2b5923b6e_arg_names_var_2201614657377291794[3] = { "self", "expr", "ifBlock" };
VarInfo __struct_info__1e8db4ddc1444e12_field_128 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c099ede2b5923b6e_arg_types_var_2201614657377291794, __type_info__c099ede2b5923b6e_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xc099ede2b5923b6e), "preVisitExprIfThenElseIfBlock", offsetof(ast::AstVisitor,preVisitExprIfThenElseIfBlock), 0 };
TypeInfo * __type_info__fb77ceef0b6231cd_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__88db72c3eb8c93b1, &__type_info__98064c57b4bcca5a };
const char * __type_info__fb77ceef0b6231cd_arg_names_var_2201614657377291794[3] = { "self", "expr", "elseBlock" };
VarInfo __struct_info__1e8db4ddc1444e12_field_129 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__fb77ceef0b6231cd_arg_types_var_2201614657377291794, __type_info__fb77ceef0b6231cd_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xfb77ceef0b6231cd), "preVisitExprIfThenElseElseBlock", offsetof(ast::AstVisitor,preVisitExprIfThenElseElseBlock), 0 };
TypeInfo * __type_info__7e1a4402fc47eb05_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87 };
const char * __type_info__7e1a4402fc47eb05_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_130 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7e1a4402fc47eb05_arg_types_var_2201614657377291794, __type_info__7e1a4402fc47eb05_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ))>::size, UINT64_C(0x7e1a4402fc47eb05), "preVisitExprFor", offsetof(ast::AstVisitor,preVisitExprFor), 0 };
TypeInfo * __type_info__b585c093a8729bb5_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87 };
const char * __type_info__b585c093a8729bb5_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_131 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__b585c093a8729bb5_arg_types_var_2201614657377291794, __type_info__b585c093a8729bb5_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ))>::size, UINT64_C(0xb585c093a8729bb5), "visitExprFor", offsetof(ast::AstVisitor,visitExprFor), 0 };
TypeInfo * __type_info__bb6c0827af8b5346_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__bb6c0827af8b5346_arg_names_var_2201614657377291794[4] = { "self", "expr", "svar", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_132 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bb6c0827af8b5346_arg_types_var_2201614657377291794, __type_info__bb6c0827af8b5346_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xbb6c0827af8b5346), "preVisitExprForVariable", offsetof(ast::AstVisitor,preVisitExprForVariable), 0 };
TypeInfo * __type_info__2f6c89bb2174e0d4_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__2f6c89bb2174e0d4_arg_names_var_2201614657377291794[4] = { "self", "expr", "svar", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_133 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__2f6c89bb2174e0d4_arg_types_var_2201614657377291794, __type_info__2f6c89bb2174e0d4_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x2f6c89bb2174e0d4), "visitExprForVariable", offsetof(ast::AstVisitor,visitExprForVariable), 0 };
TypeInfo * __type_info__ce17076d9f8cf753_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__ce17076d9f8cf753_arg_names_var_2201614657377291794[4] = { "self", "expr", "source", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_134 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ce17076d9f8cf753_arg_types_var_2201614657377291794, __type_info__ce17076d9f8cf753_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xce17076d9f8cf753), "preVisitExprForSource", offsetof(ast::AstVisitor,preVisitExprForSource), 0 };
TypeInfo * __type_info__bcee3052ac4d0412_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__bcee3052ac4d0412_arg_names_var_2201614657377291794[4] = { "self", "expr", "source", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_135 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__bcee3052ac4d0412_arg_types_var_2201614657377291794, __type_info__bcee3052ac4d0412_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xbcee3052ac4d0412), "visitExprForSource", offsetof(ast::AstVisitor,visitExprForSource), 0 };
TypeInfo * __type_info__bea7e44555360f20_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87 };
const char * __type_info__bea7e44555360f20_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_136 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bea7e44555360f20_arg_types_var_2201614657377291794, __type_info__bea7e44555360f20_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ))>::size, UINT64_C(0xbea7e44555360f20), "preVisitExprForStack", offsetof(ast::AstVisitor,preVisitExprForStack), 0 };
TypeInfo * __type_info__b6a2068ef526c805_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87 };
const char * __type_info__b6a2068ef526c805_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_137 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b6a2068ef526c805_arg_types_var_2201614657377291794, __type_info__b6a2068ef526c805_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ))>::size, UINT64_C(0xb6a2068ef526c805), "preVisitExprForBody", offsetof(ast::AstVisitor,preVisitExprForBody), 0 };
TypeInfo * __type_info__305131e8c4628f64_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4191dbf23146a87e };
const char * __type_info__305131e8c4628f64_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_138 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__305131e8c4628f64_arg_types_var_2201614657377291794, __type_info__305131e8c4628f64_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ))>::size, UINT64_C(0x305131e8c4628f64), "preVisitExprMakeVariant", offsetof(ast::AstVisitor,preVisitExprMakeVariant), 0 };
TypeInfo * __type_info__e1490f83f62810a2_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4191dbf23146a87e };
const char * __type_info__e1490f83f62810a2_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_139 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__e1490f83f62810a2_arg_types_var_2201614657377291794, __type_info__e1490f83f62810a2_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ))>::size, UINT64_C(0xe1490f83f62810a2), "visitExprMakeVariant", offsetof(ast::AstVisitor,visitExprMakeVariant), 0 };
TypeInfo * __type_info__bab2808884e1a301_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__4191dbf23146a87e, &__type_info__af8afe4c86446b52, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__bab2808884e1a301_arg_names_var_2201614657377291794[5] = { "self", "expr", "index", "decl", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_140 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bab2808884e1a301_arg_types_var_2201614657377291794, __type_info__bab2808884e1a301_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0xbab2808884e1a301), "preVisitExprMakeVariantField", offsetof(ast::AstVisitor,preVisitExprMakeVariantField), 0 };
TypeInfo * __type_info__2f5a09d014210edd_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__4191dbf23146a87e, &__type_info__af8afe4c86446b52, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__2f5a09d014210edd_arg_names_var_2201614657377291794[5] = { "self", "expr", "index", "decl", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_141 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__c52835f1e7c9ab84, nullptr, (TypeInfo **)__type_info__2f5a09d014210edd_arg_types_var_2201614657377291794, __type_info__2f5a09d014210edd_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0x2f5a09d014210edd), "visitExprMakeVariantField", offsetof(ast::AstVisitor,visitExprMakeVariantField), 0 };
TypeInfo * __type_info__afd5e182c47c834d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb };
const char * __type_info__afd5e182c47c834d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_142 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__afd5e182c47c834d_arg_types_var_2201614657377291794, __type_info__afd5e182c47c834d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ))>::size, UINT64_C(0xafd5e182c47c834d), "canVisitExprMakeStructBody", offsetof(ast::AstVisitor,canVisitExprMakeStructBody), 0 };
TypeInfo * __type_info__6df9772d1def04b5_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__98064c57b4bcca5a };
const char * __type_info__6df9772d1def04b5_arg_names_var_2201614657377291794[3] = { "self", "expr", "blk" };
VarInfo __struct_info__1e8db4ddc1444e12_field_143 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__6df9772d1def04b5_arg_types_var_2201614657377291794, __type_info__6df9772d1def04b5_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x6df9772d1def04b5), "canVisitExprMakeStructBlock", offsetof(ast::AstVisitor,canVisitExprMakeStructBlock), 0 };
TypeInfo * __type_info__6c47240f97a09f01_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb };
const char * __type_info__6c47240f97a09f01_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_144 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6c47240f97a09f01_arg_types_var_2201614657377291794, __type_info__6c47240f97a09f01_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ))>::size, UINT64_C(0x6c47240f97a09f01), "preVisitExprMakeStruct", offsetof(ast::AstVisitor,preVisitExprMakeStruct), 0 };
TypeInfo * __type_info__ac390a5ffb883c3b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb };
const char * __type_info__ac390a5ffb883c3b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_145 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ac390a5ffb883c3b_arg_types_var_2201614657377291794, __type_info__ac390a5ffb883c3b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ))>::size, UINT64_C(0xac390a5ffb883c3b), "visitExprMakeStruct", offsetof(ast::AstVisitor,visitExprMakeStruct), 0 };
TypeInfo * __type_info__3694ecd9abede4e4_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__af8afe4c86446b52, &__type_info__af81fe4c86352052 };
const char * __type_info__3694ecd9abede4e4_arg_names_var_2201614657377291794[4] = { "self", "expr", "index", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_146 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3694ecd9abede4e4_arg_types_var_2201614657377291794, __type_info__3694ecd9abede4e4_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool))>::size, UINT64_C(0x3694ecd9abede4e4), "preVisitExprMakeStructIndex", offsetof(ast::AstVisitor,preVisitExprMakeStructIndex), 0 };
TypeInfo * __type_info__467b3460de5445fe_arg_types_var_2201614657377291794[4] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__af8afe4c86446b52, &__type_info__af81fe4c86352052 };
const char * __type_info__467b3460de5445fe_arg_names_var_2201614657377291794[4] = { "self", "expr", "index", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_147 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__467b3460de5445fe_arg_types_var_2201614657377291794, __type_info__467b3460de5445fe_arg_names_var_2201614657377291794, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool))>::size, UINT64_C(0x467b3460de5445fe), "visitExprMakeStructIndex", offsetof(ast::AstVisitor,visitExprMakeStructIndex), 0 };
TypeInfo * __type_info__8173a7f23281b52c_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__af8afe4c86446b52, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__8173a7f23281b52c_arg_names_var_2201614657377291794[5] = { "self", "expr", "index", "decl", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_148 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8173a7f23281b52c_arg_types_var_2201614657377291794, __type_info__8173a7f23281b52c_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0x8173a7f23281b52c), "preVisitExprMakeStructField", offsetof(ast::AstVisitor,preVisitExprMakeStructField), 0 };
TypeInfo * __type_info__87544b21d0bdc68e_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__af8afe4c86446b52, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__87544b21d0bdc68e_arg_names_var_2201614657377291794[5] = { "self", "expr", "index", "decl", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_149 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__c52835f1e7c9ab84, nullptr, (TypeInfo **)__type_info__87544b21d0bdc68e_arg_types_var_2201614657377291794, __type_info__87544b21d0bdc68e_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0x87544b21d0bdc68e), "visitExprMakeStructField", offsetof(ast::AstVisitor,visitExprMakeStructField), 0 };
TypeInfo * __type_info__a8c08fb6b3ae9ad9_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__2dd484863625d80 };
const char * __type_info__a8c08fb6b3ae9ad9_arg_names_var_2201614657377291794[3] = { "self", "expr", "blk" };
VarInfo __struct_info__1e8db4ddc1444e12_field_150 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a8c08fb6b3ae9ad9_arg_types_var_2201614657377291794, __type_info__a8c08fb6b3ae9ad9_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xa8c08fb6b3ae9ad9), "preVisitMakeStructureBlock", offsetof(ast::AstVisitor,preVisitMakeStructureBlock), 0 };
TypeInfo * __type_info__b3b4b1a5de3de0c7_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__2dd484863625d80 };
const char * __type_info__b3b4b1a5de3de0c7_arg_names_var_2201614657377291794[3] = { "self", "expr", "blk" };
VarInfo __struct_info__1e8db4ddc1444e12_field_151 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__b3b4b1a5de3de0c7_arg_types_var_2201614657377291794, __type_info__b3b4b1a5de3de0c7_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xb3b4b1a5de3de0c7), "visitMakeStructureBlock", offsetof(ast::AstVisitor,visitMakeStructureBlock), 0 };
TypeInfo * __type_info__92682c1a46d2a5e1_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f44650fbe99befd9 };
const char * __type_info__92682c1a46d2a5e1_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_152 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__92682c1a46d2a5e1_arg_types_var_2201614657377291794, __type_info__92682c1a46d2a5e1_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ))>::size, UINT64_C(0x92682c1a46d2a5e1), "preVisitExprMakeArray", offsetof(ast::AstVisitor,preVisitExprMakeArray), 0 };
TypeInfo * __type_info__3b849f4fb88473bb_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f44650fbe99befd9 };
const char * __type_info__3b849f4fb88473bb_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_153 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__3b849f4fb88473bb_arg_types_var_2201614657377291794, __type_info__3b849f4fb88473bb_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ))>::size, UINT64_C(0x3b849f4fb88473bb), "visitExprMakeArray", offsetof(ast::AstVisitor,visitExprMakeArray), 0 };
TypeInfo * __type_info__18a580c033b89448_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__f44650fbe99befd9, &__type_info__af8afe4c86446b52, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__18a580c033b89448_arg_names_var_2201614657377291794[5] = { "self", "expr", "index", "init", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_154 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__18a580c033b89448_arg_types_var_2201614657377291794, __type_info__18a580c033b89448_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x18a580c033b89448), "preVisitExprMakeArrayIndex", offsetof(ast::AstVisitor,preVisitExprMakeArrayIndex), 0 };
TypeInfo * __type_info__f1bb59011ce51e2a_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__f44650fbe99befd9, &__type_info__af8afe4c86446b52, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__f1bb59011ce51e2a_arg_names_var_2201614657377291794[5] = { "self", "expr", "index", "init", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_155 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f1bb59011ce51e2a_arg_types_var_2201614657377291794, __type_info__f1bb59011ce51e2a_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xf1bb59011ce51e2a), "visitExprMakeArrayIndex", offsetof(ast::AstVisitor,visitExprMakeArrayIndex), 0 };
TypeInfo * __type_info__f1f74913939e753c_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__ea03eef331aabf4 };
const char * __type_info__f1f74913939e753c_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_156 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f1f74913939e753c_arg_types_var_2201614657377291794, __type_info__f1f74913939e753c_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ))>::size, UINT64_C(0xf1f74913939e753c), "preVisitExprMakeTuple", offsetof(ast::AstVisitor,preVisitExprMakeTuple), 0 };
TypeInfo * __type_info__3999c8ecc927783_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__ea03eef331aabf4 };
const char * __type_info__3999c8ecc927783_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_157 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__3999c8ecc927783_arg_types_var_2201614657377291794, __type_info__3999c8ecc927783_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ))>::size, UINT64_C(0x3999c8ecc927783), "visitExprMakeTuple", offsetof(ast::AstVisitor,visitExprMakeTuple), 0 };
TypeInfo * __type_info__4bdfc151dedc4da9_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__ea03eef331aabf4, &__type_info__af8afe4c86446b52, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__4bdfc151dedc4da9_arg_names_var_2201614657377291794[5] = { "self", "expr", "index", "init", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_158 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4bdfc151dedc4da9_arg_types_var_2201614657377291794, __type_info__4bdfc151dedc4da9_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x4bdfc151dedc4da9), "preVisitExprMakeTupleIndex", offsetof(ast::AstVisitor,preVisitExprMakeTupleIndex), 0 };
TypeInfo * __type_info__7e590097bace95e2_arg_types_var_2201614657377291794[5] = { &__type_info__21586ce84f433a21, &__type_info__ea03eef331aabf4, &__type_info__af8afe4c86446b52, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__7e590097bace95e2_arg_names_var_2201614657377291794[5] = { "self", "expr", "index", "init", "last" };
VarInfo __struct_info__1e8db4ddc1444e12_field_159 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__7e590097bace95e2_arg_types_var_2201614657377291794, __type_info__7e590097bace95e2_arg_names_var_2201614657377291794, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x7e590097bace95e2), "visitExprMakeTupleIndex", offsetof(ast::AstVisitor,visitExprMakeTupleIndex), 0 };
TypeInfo * __type_info__d793210438da73b2_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__defdb920e82da0f4 };
const char * __type_info__d793210438da73b2_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_160 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d793210438da73b2_arg_types_var_2201614657377291794, __type_info__d793210438da73b2_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ))>::size, UINT64_C(0xd793210438da73b2), "preVisitExprArrayComprehension", offsetof(ast::AstVisitor,preVisitExprArrayComprehension), 0 };
TypeInfo * __type_info__5935677610234a58_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__defdb920e82da0f4 };
const char * __type_info__5935677610234a58_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_161 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__5935677610234a58_arg_types_var_2201614657377291794, __type_info__5935677610234a58_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ))>::size, UINT64_C(0x5935677610234a58), "visitExprArrayComprehension", offsetof(ast::AstVisitor,visitExprArrayComprehension), 0 };
TypeInfo * __type_info__4380a05405d544a9_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__defdb920e82da0f4, &__type_info__98064c57b4bcca5a };
const char * __type_info__4380a05405d544a9_arg_names_var_2201614657377291794[3] = { "self", "expr", "subexrp" };
VarInfo __struct_info__1e8db4ddc1444e12_field_162 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4380a05405d544a9_arg_types_var_2201614657377291794, __type_info__4380a05405d544a9_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x4380a05405d544a9), "preVisitExprArrayComprehensionSubexpr", offsetof(ast::AstVisitor,preVisitExprArrayComprehensionSubexpr), 0 };
TypeInfo * __type_info__1dd9197835443a11_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__defdb920e82da0f4, &__type_info__98064c57b4bcca5a };
const char * __type_info__1dd9197835443a11_arg_names_var_2201614657377291794[3] = { "self", "expr", "filter" };
VarInfo __struct_info__1e8db4ddc1444e12_field_163 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1dd9197835443a11_arg_types_var_2201614657377291794, __type_info__1dd9197835443a11_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x1dd9197835443a11), "preVisitExprArrayComprehensionWhere", offsetof(ast::AstVisitor,preVisitExprArrayComprehensionWhere), 0 };
TypeInfo * __type_info__f722ae5ea36a9c48_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__563543a880fdcea2, &__type_info__98064c57b4bcca5a };
const char * __type_info__f722ae5ea36a9c48_arg_names_var_2201614657377291794[3] = { "self", "expr", "expr_" };
VarInfo __struct_info__1e8db4ddc1444e12_field_164 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__f722ae5ea36a9c48_arg_types_var_2201614657377291794, __type_info__f722ae5ea36a9c48_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xf722ae5ea36a9c48), "canVisitExprTypeInfo", offsetof(ast::AstVisitor,canVisitExprTypeInfo), 0 };
TypeInfo * __type_info__5cfa422ce6237a8e_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__563543a880fdcea2 };
const char * __type_info__5cfa422ce6237a8e_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_165 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5cfa422ce6237a8e_arg_types_var_2201614657377291794, __type_info__5cfa422ce6237a8e_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ))>::size, UINT64_C(0x5cfa422ce6237a8e), "preVisitExprTypeInfo", offsetof(ast::AstVisitor,preVisitExprTypeInfo), 0 };
TypeInfo * __type_info__35446d2c24c68711_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__563543a880fdcea2 };
const char * __type_info__35446d2c24c68711_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_166 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__35446d2c24c68711_arg_types_var_2201614657377291794, __type_info__35446d2c24c68711_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ))>::size, UINT64_C(0x35446d2c24c68711), "visitExprTypeInfo", offsetof(ast::AstVisitor,visitExprTypeInfo), 0 };
TypeInfo * __type_info__4585e1e3fd2a1c4f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7d9fd489616ae8d };
const char * __type_info__4585e1e3fd2a1c4f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_167 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4585e1e3fd2a1c4f_arg_types_var_2201614657377291794, __type_info__4585e1e3fd2a1c4f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const ))>::size, UINT64_C(0x4585e1e3fd2a1c4f), "preVisitExprPtr2Ref", offsetof(ast::AstVisitor,preVisitExprPtr2Ref), 0 };
TypeInfo * __type_info__10fa045a7f20e7c5_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7d9fd489616ae8d };
const char * __type_info__10fa045a7f20e7c5_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_168 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__10fa045a7f20e7c5_arg_types_var_2201614657377291794, __type_info__10fa045a7f20e7c5_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const ))>::size, UINT64_C(0x10fa045a7f20e7c5), "visitExprPtr2Ref", offsetof(ast::AstVisitor,visitExprPtr2Ref), 0 };
TypeInfo * __type_info__b84f13cf26eda329_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6ad276912e16c445 };
const char * __type_info__b84f13cf26eda329_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_169 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b84f13cf26eda329_arg_types_var_2201614657377291794, __type_info__b84f13cf26eda329_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLabel> const ))>::size, UINT64_C(0xb84f13cf26eda329), "preVisitExprLabel", offsetof(ast::AstVisitor,preVisitExprLabel), 0 };
TypeInfo * __type_info__8a7f1eb665490d1a_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6ad276912e16c445 };
const char * __type_info__8a7f1eb665490d1a_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_170 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__8a7f1eb665490d1a_arg_types_var_2201614657377291794, __type_info__8a7f1eb665490d1a_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLabel> const ))>::size, UINT64_C(0x8a7f1eb665490d1a), "visitExprLabel", offsetof(ast::AstVisitor,visitExprLabel), 0 };
TypeInfo * __type_info__7e014002fc204c62_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__41023c185ec41d2 };
const char * __type_info__7e014002fc204c62_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_171 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7e014002fc204c62_arg_types_var_2201614657377291794, __type_info__7e014002fc204c62_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprGoto> const ))>::size, UINT64_C(0x7e014002fc204c62), "preVisitExprGoto", offsetof(ast::AstVisitor,preVisitExprGoto), 0 };
TypeInfo * __type_info__7adb14e36229366e_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__41023c185ec41d2 };
const char * __type_info__7adb14e36229366e_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_172 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__7adb14e36229366e_arg_types_var_2201614657377291794, __type_info__7adb14e36229366e_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprGoto> const ))>::size, UINT64_C(0x7adb14e36229366e), "visitExprGoto", offsetof(ast::AstVisitor,visitExprGoto), 0 };
TypeInfo * __type_info__8bd3f7a275b2fdb6_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__bba83b75d4855b7e };
const char * __type_info__8bd3f7a275b2fdb6_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_173 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8bd3f7a275b2fdb6_arg_types_var_2201614657377291794, __type_info__8bd3f7a275b2fdb6_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const ))>::size, UINT64_C(0x8bd3f7a275b2fdb6), "preVisitExprRef2Value", offsetof(ast::AstVisitor,preVisitExprRef2Value), 0 };
TypeInfo * __type_info__eb17a67d73d1450d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__bba83b75d4855b7e };
const char * __type_info__eb17a67d73d1450d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_174 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__eb17a67d73d1450d_arg_types_var_2201614657377291794, __type_info__eb17a67d73d1450d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const ))>::size, UINT64_C(0xeb17a67d73d1450d), "visitExprRef2Value", offsetof(ast::AstVisitor,visitExprRef2Value), 0 };
TypeInfo * __type_info__e5a725d92e24b68f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__1151bc4127672205 };
const char * __type_info__e5a725d92e24b68f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_175 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e5a725d92e24b68f_arg_types_var_2201614657377291794, __type_info__e5a725d92e24b68f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const ))>::size, UINT64_C(0xe5a725d92e24b68f), "preVisitExprRef2Ptr", offsetof(ast::AstVisitor,preVisitExprRef2Ptr), 0 };
TypeInfo * __type_info__2a5ff066b4c53561_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__1151bc4127672205 };
const char * __type_info__2a5ff066b4c53561_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_176 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__2a5ff066b4c53561_arg_types_var_2201614657377291794, __type_info__2a5ff066b4c53561_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const ))>::size, UINT64_C(0x2a5ff066b4c53561), "visitExprRef2Ptr", offsetof(ast::AstVisitor,visitExprRef2Ptr), 0 };
TypeInfo * __type_info__58ee4c02dcf52dbc_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c2f4bc15903e1610 };
const char * __type_info__58ee4c02dcf52dbc_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_177 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__58ee4c02dcf52dbc_arg_types_var_2201614657377291794, __type_info__58ee4c02dcf52dbc_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAddr> const ))>::size, UINT64_C(0x58ee4c02dcf52dbc), "preVisitExprAddr", offsetof(ast::AstVisitor,preVisitExprAddr), 0 };
TypeInfo * __type_info__98a9ffc4527873f2_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c2f4bc15903e1610 };
const char * __type_info__98a9ffc4527873f2_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_178 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__98a9ffc4527873f2_arg_types_var_2201614657377291794, __type_info__98a9ffc4527873f2_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAddr> const ))>::size, UINT64_C(0x98a9ffc4527873f2), "visitExprAddr", offsetof(ast::AstVisitor,visitExprAddr), 0 };
TypeInfo * __type_info__2a79f0897f884291_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__d551858bc6d43037 };
const char * __type_info__2a79f0897f884291_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_179 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2a79f0897f884291_arg_types_var_2201614657377291794, __type_info__2a79f0897f884291_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssert> const ))>::size, UINT64_C(0x2a79f0897f884291), "preVisitExprAssert", offsetof(ast::AstVisitor,preVisitExprAssert), 0 };
TypeInfo * __type_info__d5937361ae5f76f8_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__d551858bc6d43037 };
const char * __type_info__d5937361ae5f76f8_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_180 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__d5937361ae5f76f8_arg_types_var_2201614657377291794, __type_info__d5937361ae5f76f8_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssert> const ))>::size, UINT64_C(0xd5937361ae5f76f8), "visitExprAssert", offsetof(ast::AstVisitor,visitExprAssert), 0 };
TypeInfo * __type_info__97956f84758cbd42_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__74372feec5a81686 };
const char * __type_info__97956f84758cbd42_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_181 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__97956f84758cbd42_arg_types_var_2201614657377291794, __type_info__97956f84758cbd42_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const ))>::size, UINT64_C(0x97956f84758cbd42), "preVisitExprStaticAssert", offsetof(ast::AstVisitor,preVisitExprStaticAssert), 0 };
TypeInfo * __type_info__b369ba5a9fd3f6bf_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__74372feec5a81686 };
const char * __type_info__b369ba5a9fd3f6bf_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_182 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__b369ba5a9fd3f6bf_arg_types_var_2201614657377291794, __type_info__b369ba5a9fd3f6bf_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const ))>::size, UINT64_C(0xb369ba5a9fd3f6bf), "visitExprStaticAssert", offsetof(ast::AstVisitor,visitExprStaticAssert), 0 };
TypeInfo * __type_info__6ff71915ee4dc68_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9dfe8a83730428c8 };
const char * __type_info__6ff71915ee4dc68_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_183 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6ff71915ee4dc68_arg_types_var_2201614657377291794, __type_info__6ff71915ee4dc68_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprQuote> const ))>::size, UINT64_C(0x6ff71915ee4dc68), "preVisitExprQuote", offsetof(ast::AstVisitor,preVisitExprQuote), 0 };
TypeInfo * __type_info__f881ff76a48e7a59_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9dfe8a83730428c8 };
const char * __type_info__f881ff76a48e7a59_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_184 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f881ff76a48e7a59_arg_types_var_2201614657377291794, __type_info__f881ff76a48e7a59_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprQuote> const ))>::size, UINT64_C(0xf881ff76a48e7a59), "visitExprQuote", offsetof(ast::AstVisitor,visitExprQuote), 0 };
TypeInfo * __type_info__4b98c0e117b36490_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c84cf5ded2cd1cd8 };
const char * __type_info__4b98c0e117b36490_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_185 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4b98c0e117b36490_arg_types_var_2201614657377291794, __type_info__4b98c0e117b36490_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDebug> const ))>::size, UINT64_C(0x4b98c0e117b36490), "preVisitExprDebug", offsetof(ast::AstVisitor,preVisitExprDebug), 0 };
TypeInfo * __type_info__fcd0fedcec5e3016_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c84cf5ded2cd1cd8 };
const char * __type_info__fcd0fedcec5e3016_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_186 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__fcd0fedcec5e3016_arg_types_var_2201614657377291794, __type_info__fcd0fedcec5e3016_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDebug> const ))>::size, UINT64_C(0xfcd0fedcec5e3016), "visitExprDebug", offsetof(ast::AstVisitor,visitExprDebug), 0 };
TypeInfo * __type_info__7f07400e46e3e0c3_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c3c8c780df6c5865 };
const char * __type_info__7f07400e46e3e0c3_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_187 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7f07400e46e3e0c3_arg_types_var_2201614657377291794, __type_info__7f07400e46e3e0c3_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const ))>::size, UINT64_C(0x7f07400e46e3e0c3), "preVisitExprInvoke", offsetof(ast::AstVisitor,preVisitExprInvoke), 0 };
TypeInfo * __type_info__70eaf9dc12ebfe56_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c3c8c780df6c5865 };
const char * __type_info__70eaf9dc12ebfe56_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_188 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__70eaf9dc12ebfe56_arg_types_var_2201614657377291794, __type_info__70eaf9dc12ebfe56_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const ))>::size, UINT64_C(0x70eaf9dc12ebfe56), "visitExprInvoke", offsetof(ast::AstVisitor,visitExprInvoke), 0 };
TypeInfo * __type_info__b1b57985905c2546_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e672712e93e236ba };
const char * __type_info__b1b57985905c2546_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_189 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b1b57985905c2546_arg_types_var_2201614657377291794, __type_info__b1b57985905c2546_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprErase> const ))>::size, UINT64_C(0xb1b57985905c2546), "preVisitExprErase", offsetof(ast::AstVisitor,preVisitExprErase), 0 };
TypeInfo * __type_info__21a610d8b307aab7_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e672712e93e236ba };
const char * __type_info__21a610d8b307aab7_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_190 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__21a610d8b307aab7_arg_types_var_2201614657377291794, __type_info__21a610d8b307aab7_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprErase> const ))>::size, UINT64_C(0x21a610d8b307aab7), "visitExprErase", offsetof(ast::AstVisitor,visitExprErase), 0 };
TypeInfo * __type_info__64c3392746be1d39_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9c37565e66334661 };
const char * __type_info__64c3392746be1d39_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_191 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__64c3392746be1d39_arg_types_var_2201614657377291794, __type_info__64c3392746be1d39_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const ))>::size, UINT64_C(0x64c3392746be1d39), "preVisitExprSetInsert", offsetof(ast::AstVisitor,preVisitExprSetInsert), 0 };
TypeInfo * __type_info__1a6629989f7a578b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9c37565e66334661 };
const char * __type_info__1a6629989f7a578b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_192 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__1a6629989f7a578b_arg_types_var_2201614657377291794, __type_info__1a6629989f7a578b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const ))>::size, UINT64_C(0x1a6629989f7a578b), "visitExprSetInsert", offsetof(ast::AstVisitor,visitExprSetInsert), 0 };
TypeInfo * __type_info__69da5802eb324301_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__ee83d76e6f9a3c81 };
const char * __type_info__69da5802eb324301_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_193 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__69da5802eb324301_arg_types_var_2201614657377291794, __type_info__69da5802eb324301_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFind> const ))>::size, UINT64_C(0x69da5802eb324301), "preVisitExprFind", offsetof(ast::AstVisitor,preVisitExprFind), 0 };
TypeInfo * __type_info__d95efe75d53d785_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__ee83d76e6f9a3c81 };
const char * __type_info__d95efe75d53d785_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_194 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__d95efe75d53d785_arg_types_var_2201614657377291794, __type_info__d95efe75d53d785_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFind> const ))>::size, UINT64_C(0xd95efe75d53d785), "visitExprFind", offsetof(ast::AstVisitor,visitExprFind), 0 };
TypeInfo * __type_info__e32df143ca82380d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7f9fc2c601e28df1 };
const char * __type_info__e32df143ca82380d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_195 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e32df143ca82380d_arg_types_var_2201614657377291794, __type_info__e32df143ca82380d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const ))>::size, UINT64_C(0xe32df143ca82380d), "preVisitExprKeyExists", offsetof(ast::AstVisitor,preVisitExprKeyExists), 0 };
TypeInfo * __type_info__803a9565358b9c2b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7f9fc2c601e28df1 };
const char * __type_info__803a9565358b9c2b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_196 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__803a9565358b9c2b_arg_types_var_2201614657377291794, __type_info__803a9565358b9c2b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const ))>::size, UINT64_C(0x803a9565358b9c2b), "visitExprKeyExists", offsetof(ast::AstVisitor,visitExprKeyExists), 0 };
TypeInfo * __type_info__2a88dc897f736b05_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6bdd529063b3dbeb };
const char * __type_info__2a88dc897f736b05_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_197 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2a88dc897f736b05_arg_types_var_2201614657377291794, __type_info__2a88dc897f736b05_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAscend> const ))>::size, UINT64_C(0x2a88dc897f736b05), "preVisitExprAscend", offsetof(ast::AstVisitor,preVisitExprAscend), 0 };
TypeInfo * __type_info__862b83af69f35e28_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6bdd529063b3dbeb };
const char * __type_info__862b83af69f35e28_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_198 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__862b83af69f35e28_arg_types_var_2201614657377291794, __type_info__862b83af69f35e28_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAscend> const ))>::size, UINT64_C(0x862b83af69f35e28), "visitExprAscend", offsetof(ast::AstVisitor,visitExprAscend), 0 };
TypeInfo * __type_info__4ee94302d492541f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__bc5b346893db35b };
const char * __type_info__4ee94302d492541f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_199 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4ee94302d492541f_arg_types_var_2201614657377291794, __type_info__4ee94302d492541f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCast> const ))>::size, UINT64_C(0x4ee94302d492541f), "preVisitExprCast", offsetof(ast::AstVisitor,preVisitExprCast), 0 };
TypeInfo * __type_info__921c0fceafe8699d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__bc5b346893db35b };
const char * __type_info__921c0fceafe8699d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_200 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__921c0fceafe8699d_arg_types_var_2201614657377291794, __type_info__921c0fceafe8699d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCast> const ))>::size, UINT64_C(0x921c0fceafe8699d), "visitExprCast", offsetof(ast::AstVisitor,visitExprCast), 0 };
TypeInfo * __type_info__e6914f2eefeace48_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__120723ecb6510065, &__type_info__98064c57b4bcca5a };
const char * __type_info__e6914f2eefeace48_arg_names_var_2201614657377291794[3] = { "self", "del", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_201 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e6914f2eefeace48_arg_types_var_2201614657377291794, __type_info__e6914f2eefeace48_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xe6914f2eefeace48), "preVisitExprDeleteSizeExpression", offsetof(ast::AstVisitor,preVisitExprDeleteSizeExpression), 0 };
TypeInfo * __type_info__81bbbfe1459b2b77_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__120723ecb6510065 };
const char * __type_info__81bbbfe1459b2b77_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_202 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__81bbbfe1459b2b77_arg_types_var_2201614657377291794, __type_info__81bbbfe1459b2b77_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ))>::size, UINT64_C(0x81bbbfe1459b2b77), "preVisitExprDelete", offsetof(ast::AstVisitor,preVisitExprDelete), 0 };
TypeInfo * __type_info__8f62646d23f25b99_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__120723ecb6510065 };
const char * __type_info__8f62646d23f25b99_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_203 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__8f62646d23f25b99_arg_types_var_2201614657377291794, __type_info__8f62646d23f25b99_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ))>::size, UINT64_C(0x8f62646d23f25b99), "visitExprDelete", offsetof(ast::AstVisitor,visitExprDelete), 0 };
TypeInfo * __type_info__4e4f4402d3ac6a95_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f5c1d1c41d788f7 };
const char * __type_info__4e4f4402d3ac6a95_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_204 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4e4f4402d3ac6a95_arg_types_var_2201614657377291794, __type_info__4e4f4402d3ac6a95_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprVar> const ))>::size, UINT64_C(0x4e4f4402d3ac6a95), "preVisitExprVar", offsetof(ast::AstVisitor,preVisitExprVar), 0 };
TypeInfo * __type_info__7f85c2937ae32f1b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f5c1d1c41d788f7 };
const char * __type_info__7f85c2937ae32f1b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_205 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__7f85c2937ae32f1b_arg_types_var_2201614657377291794, __type_info__7f85c2937ae32f1b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprVar> const ))>::size, UINT64_C(0x7f85c2937ae32f1b), "visitExprVar", offsetof(ast::AstVisitor,visitExprVar), 0 };
TypeInfo * __type_info__4e485f02d3a679c8_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__631c9e15ba7d5036 };
const char * __type_info__4e485f02d3a679c8_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_206 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4e485f02d3a679c8_arg_types_var_2201614657377291794, __type_info__4e485f02d3a679c8_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const ))>::size, UINT64_C(0x4e485f02d3a679c8), "preVisitExprTag", offsetof(ast::AstVisitor,preVisitExprTag), 0 };
TypeInfo * __type_info__2c13e88b48ba86ba_arg_types_var_2201614657377291794[3] = { &__type_info__21586ce84f433a21, &__type_info__631c9e15ba7d5036, &__type_info__98064c57b4bcca5a };
const char * __type_info__2c13e88b48ba86ba_arg_names_var_2201614657377291794[3] = { "self", "expr", "value" };
VarInfo __struct_info__1e8db4ddc1444e12_field_207 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2c13e88b48ba86ba_arg_types_var_2201614657377291794, __type_info__2c13e88b48ba86ba_arg_names_var_2201614657377291794, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x2c13e88b48ba86ba), "preVisitExprTagValue", offsetof(ast::AstVisitor,preVisitExprTagValue), 0 };
TypeInfo * __type_info__786ac2937496a01b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__631c9e15ba7d5036 };
const char * __type_info__786ac2937496a01b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_208 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__786ac2937496a01b_arg_types_var_2201614657377291794, __type_info__786ac2937496a01b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTag> const ))>::size, UINT64_C(0x786ac2937496a01b), "visitExprTag", offsetof(ast::AstVisitor,visitExprTag), 0 };
TypeInfo * __type_info__2b510ff5bd61305c_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__849126a4e3db3268 };
const char * __type_info__2b510ff5bd61305c_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_209 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2b510ff5bd61305c_arg_types_var_2201614657377291794, __type_info__2b510ff5bd61305c_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprField> const ))>::size, UINT64_C(0x2b510ff5bd61305c), "preVisitExprField", offsetof(ast::AstVisitor,preVisitExprField), 0 };
TypeInfo * __type_info__3313f7e77d463c1d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__849126a4e3db3268 };
const char * __type_info__3313f7e77d463c1d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_210 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__3313f7e77d463c1d_arg_types_var_2201614657377291794, __type_info__3313f7e77d463c1d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprField> const ))>::size, UINT64_C(0x3313f7e77d463c1d), "visitExprField", offsetof(ast::AstVisitor,visitExprField), 0 };
TypeInfo * __type_info__a13178df83f85383_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__a7adf4b0a367d897 };
const char * __type_info__a13178df83f85383_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_211 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a13178df83f85383_arg_types_var_2201614657377291794, __type_info__a13178df83f85383_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const ))>::size, UINT64_C(0xa13178df83f85383), "preVisitExprSafeField", offsetof(ast::AstVisitor,preVisitExprSafeField), 0 };
TypeInfo * __type_info__671c1e58b35b487f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__a7adf4b0a367d897 };
const char * __type_info__671c1e58b35b487f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_212 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__671c1e58b35b487f_arg_types_var_2201614657377291794, __type_info__671c1e58b35b487f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const ))>::size, UINT64_C(0x671c1e58b35b487f), "visitExprSafeField", offsetof(ast::AstVisitor,visitExprSafeField), 0 };
TypeInfo * __type_info__68bdfe50f2934580_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6bb94e24ea14ce9a };
const char * __type_info__68bdfe50f2934580_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_213 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__68bdfe50f2934580_arg_types_var_2201614657377291794, __type_info__68bdfe50f2934580_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const ))>::size, UINT64_C(0x68bdfe50f2934580), "preVisitExprSwizzle", offsetof(ast::AstVisitor,preVisitExprSwizzle), 0 };
TypeInfo * __type_info__4c448f3c87aebe6b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6bb94e24ea14ce9a };
const char * __type_info__4c448f3c87aebe6b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_214 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__4c448f3c87aebe6b_arg_types_var_2201614657377291794, __type_info__4c448f3c87aebe6b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const ))>::size, UINT64_C(0x4c448f3c87aebe6b), "visitExprSwizzle", offsetof(ast::AstVisitor,visitExprSwizzle), 0 };
TypeInfo * __type_info__c9b7983395b4b39d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__837624c70f8f1fa1 };
const char * __type_info__c9b7983395b4b39d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_215 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c9b7983395b4b39d_arg_types_var_2201614657377291794, __type_info__c9b7983395b4b39d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const ))>::size, UINT64_C(0xc9b7983395b4b39d), "preVisitExprIsVariant", offsetof(ast::AstVisitor,preVisitExprIsVariant), 0 };
TypeInfo * __type_info__a6c5a0d476401dcf_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__837624c70f8f1fa1 };
const char * __type_info__a6c5a0d476401dcf_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_216 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__a6c5a0d476401dcf_arg_types_var_2201614657377291794, __type_info__a6c5a0d476401dcf_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const ))>::size, UINT64_C(0xa6c5a0d476401dcf), "visitExprIsVariant", offsetof(ast::AstVisitor,visitExprIsVariant), 0 };
TypeInfo * __type_info__7d0d2be93f052825_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__229aabe2f8bef1d9 };
const char * __type_info__7d0d2be93f052825_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_217 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7d0d2be93f052825_arg_types_var_2201614657377291794, __type_info__7d0d2be93f052825_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const ))>::size, UINT64_C(0x7d0d2be93f052825), "preVisitExprAsVariant", offsetof(ast::AstVisitor,preVisitExprAsVariant), 0 };
TypeInfo * __type_info__8e3ef7346246d5cf_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__229aabe2f8bef1d9 };
const char * __type_info__8e3ef7346246d5cf_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_218 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__8e3ef7346246d5cf_arg_types_var_2201614657377291794, __type_info__8e3ef7346246d5cf_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const ))>::size, UINT64_C(0x8e3ef7346246d5cf), "visitExprAsVariant", offsetof(ast::AstVisitor,visitExprAsVariant), 0 };
TypeInfo * __type_info__648af9cb3fe807e2_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__45d77ccae958b9de };
const char * __type_info__648af9cb3fe807e2_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_219 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__648af9cb3fe807e2_arg_types_var_2201614657377291794, __type_info__648af9cb3fe807e2_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const ))>::size, UINT64_C(0x648af9cb3fe807e2), "preVisitExprSafeAsVariant", offsetof(ast::AstVisitor,preVisitExprSafeAsVariant), 0 };
TypeInfo * __type_info__9c27b1a81ccde81_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__45d77ccae958b9de };
const char * __type_info__9c27b1a81ccde81_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_220 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9c27b1a81ccde81_arg_types_var_2201614657377291794, __type_info__9c27b1a81ccde81_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const ))>::size, UINT64_C(0x9c27b1a81ccde81), "visitExprSafeAsVariant", offsetof(ast::AstVisitor,visitExprSafeAsVariant), 0 };
TypeInfo * __type_info__14d78d02a2d94ef1_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c7c0e4fba3dcbfcf };
const char * __type_info__14d78d02a2d94ef1_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_221 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__14d78d02a2d94ef1_arg_types_var_2201614657377291794, __type_info__14d78d02a2d94ef1_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp1> const ))>::size, UINT64_C(0x14d78d02a2d94ef1), "preVisitExprOp1", offsetof(ast::AstVisitor,preVisitExprOp1), 0 };
TypeInfo * __type_info__9750b1938f1ae438_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__c7c0e4fba3dcbfcf };
const char * __type_info__9750b1938f1ae438_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_222 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9750b1938f1ae438_arg_types_var_2201614657377291794, __type_info__9750b1938f1ae438_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp1> const ))>::size, UINT64_C(0x9750b1938f1ae438), "visitExprOp1", offsetof(ast::AstVisitor,visitExprOp1), 0 };
TypeInfo * __type_info__6ecbd3e161d61b3f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__1a5b7f11cf3fb5b5 };
const char * __type_info__6ecbd3e161d61b3f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_223 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6ecbd3e161d61b3f_arg_types_var_2201614657377291794, __type_info__6ecbd3e161d61b3f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReturn> const ))>::size, UINT64_C(0x6ecbd3e161d61b3f), "preVisitExprReturn", offsetof(ast::AstVisitor,preVisitExprReturn), 0 };
TypeInfo * __type_info__6ac4f424293494e8_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__1a5b7f11cf3fb5b5 };
const char * __type_info__6ac4f424293494e8_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_224 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__6ac4f424293494e8_arg_types_var_2201614657377291794, __type_info__6ac4f424293494e8_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReturn> const ))>::size, UINT64_C(0x6ac4f424293494e8), "visitExprReturn", offsetof(ast::AstVisitor,visitExprReturn), 0 };
TypeInfo * __type_info__4ca48ef5fe7d5999_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__60501e84f49c29e1 };
const char * __type_info__4ca48ef5fe7d5999_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_225 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4ca48ef5fe7d5999_arg_types_var_2201614657377291794, __type_info__4ca48ef5fe7d5999_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprYield> const ))>::size, UINT64_C(0x4ca48ef5fe7d5999), "preVisitExprYield", offsetof(ast::AstVisitor,preVisitExprYield), 0 };
TypeInfo * __type_info__643af750008e451d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__60501e84f49c29e1 };
const char * __type_info__643af750008e451d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_226 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__643af750008e451d_arg_types_var_2201614657377291794, __type_info__643af750008e451d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprYield> const ))>::size, UINT64_C(0x643af750008e451d), "visitExprYield", offsetof(ast::AstVisitor,visitExprYield), 0 };
TypeInfo * __type_info__81245a85698ce65d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__5047b5dbcc2127cd };
const char * __type_info__81245a85698ce65d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_227 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__81245a85698ce65d_arg_types_var_2201614657377291794, __type_info__81245a85698ce65d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBreak> const ))>::size, UINT64_C(0x81245a85698ce65d), "preVisitExprBreak", offsetof(ast::AstVisitor,preVisitExprBreak), 0 };
TypeInfo * __type_info__249b22d5373d404d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__5047b5dbcc2127cd };
const char * __type_info__249b22d5373d404d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_228 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__249b22d5373d404d_arg_types_var_2201614657377291794, __type_info__249b22d5373d404d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBreak> const ))>::size, UINT64_C(0x249b22d5373d404d), "visitExprBreak", offsetof(ast::AstVisitor,visitExprBreak), 0 };
TypeInfo * __type_info__7363dd5a3a53e072_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__5b91ede0508873e };
const char * __type_info__7363dd5a3a53e072_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_229 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7363dd5a3a53e072_arg_types_var_2201614657377291794, __type_info__7363dd5a3a53e072_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprContinue> const ))>::size, UINT64_C(0x7363dd5a3a53e072), "preVisitExprContinue", offsetof(ast::AstVisitor,preVisitExprContinue), 0 };
TypeInfo * __type_info__6ed4cffd10453646_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__5b91ede0508873e };
const char * __type_info__6ed4cffd10453646_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_230 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__6ed4cffd10453646_arg_types_var_2201614657377291794, __type_info__6ed4cffd10453646_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprContinue> const ))>::size, UINT64_C(0x6ed4cffd10453646), "visitExprContinue", offsetof(ast::AstVisitor,visitExprContinue), 0 };
TypeInfo * __type_info__717cd02cdb41bb2a_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e0b574ceb6c8c70f };
const char * __type_info__717cd02cdb41bb2a_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_231 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__717cd02cdb41bb2a_arg_types_var_2201614657377291794, __type_info__717cd02cdb41bb2a_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const ))>::size, UINT64_C(0x717cd02cdb41bb2a), "canVisitMakeBlockBody", offsetof(ast::AstVisitor,canVisitMakeBlockBody), 0 };
TypeInfo * __type_info__ff9cbce939b8bbe7_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e0b574ceb6c8c70f };
const char * __type_info__ff9cbce939b8bbe7_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_232 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ff9cbce939b8bbe7_arg_types_var_2201614657377291794, __type_info__ff9cbce939b8bbe7_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const ))>::size, UINT64_C(0xff9cbce939b8bbe7), "preVisitExprMakeBlock", offsetof(ast::AstVisitor,preVisitExprMakeBlock), 0 };
TypeInfo * __type_info__ba19715e054fd353_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e0b574ceb6c8c70f };
const char * __type_info__ba19715e054fd353_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_233 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ba19715e054fd353_arg_types_var_2201614657377291794, __type_info__ba19715e054fd353_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const ))>::size, UINT64_C(0xba19715e054fd353), "visitExprMakeBlock", offsetof(ast::AstVisitor,visitExprMakeBlock), 0 };
TypeInfo * __type_info__aaec8cf0fd2ec5ac_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6c1a6b092c78a88 };
const char * __type_info__aaec8cf0fd2ec5ac_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_234 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__aaec8cf0fd2ec5ac_arg_types_var_2201614657377291794, __type_info__aaec8cf0fd2ec5ac_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const ))>::size, UINT64_C(0xaaec8cf0fd2ec5ac), "preVisitExprMakeGenerator", offsetof(ast::AstVisitor,preVisitExprMakeGenerator), 0 };
TypeInfo * __type_info__160e1334ac8f8bfc_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6c1a6b092c78a88 };
const char * __type_info__160e1334ac8f8bfc_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_235 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__160e1334ac8f8bfc_arg_types_var_2201614657377291794, __type_info__160e1334ac8f8bfc_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const ))>::size, UINT64_C(0x160e1334ac8f8bfc), "visitExprMakeGenerator", offsetof(ast::AstVisitor,visitExprMakeGenerator), 0 };
TypeInfo * __type_info__4fa475cf1e4911cf_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f66cc598ea369f61 };
const char * __type_info__4fa475cf1e4911cf_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_236 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4fa475cf1e4911cf_arg_types_var_2201614657377291794, __type_info__4fa475cf1e4911cf_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const ))>::size, UINT64_C(0x4fa475cf1e4911cf), "preVisitExprMemZero", offsetof(ast::AstVisitor,preVisitExprMemZero), 0 };
TypeInfo * __type_info__89db2eaecd06de6f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f66cc598ea369f61 };
const char * __type_info__89db2eaecd06de6f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_237 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__89db2eaecd06de6f_arg_types_var_2201614657377291794, __type_info__89db2eaecd06de6f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const ))>::size, UINT64_C(0x89db2eaecd06de6f), "visitExprMemZero", offsetof(ast::AstVisitor,visitExprMemZero), 0 };
TypeInfo * __type_info__22e143125efe0694_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__118bfa23ce6c000c };
const char * __type_info__22e143125efe0694_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_238 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__22e143125efe0694_arg_types_var_2201614657377291794, __type_info__22e143125efe0694_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConst> const ))>::size, UINT64_C(0x22e143125efe0694), "preVisitExprConst", offsetof(ast::AstVisitor,preVisitExprConst), 0 };
TypeInfo * __type_info__be5700ced58d4272_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__118bfa23ce6c000c };
const char * __type_info__be5700ced58d4272_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_239 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__be5700ced58d4272_arg_types_var_2201614657377291794, __type_info__be5700ced58d4272_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConst> const ))>::size, UINT64_C(0xbe5700ced58d4272), "visitExprConst", offsetof(ast::AstVisitor,visitExprConst), 0 };
TypeInfo * __type_info__9f39d836de4c48a0_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__a45346c81e6a9b80 };
const char * __type_info__9f39d836de4c48a0_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_240 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9f39d836de4c48a0_arg_types_var_2201614657377291794, __type_info__9f39d836de4c48a0_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const ))>::size, UINT64_C(0x9f39d836de4c48a0), "preVisitExprConstPtr", offsetof(ast::AstVisitor,preVisitExprConstPtr), 0 };
TypeInfo * __type_info__893bad96d6e1b2dc_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__a45346c81e6a9b80 };
const char * __type_info__893bad96d6e1b2dc_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_241 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__893bad96d6e1b2dc_arg_types_var_2201614657377291794, __type_info__893bad96d6e1b2dc_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const ))>::size, UINT64_C(0x893bad96d6e1b2dc), "visitExprConstPtr", offsetof(ast::AstVisitor,visitExprConstPtr), 0 };
TypeInfo * __type_info__63e2d4927d6419f0_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__b6d18d4b3fadccd4 };
const char * __type_info__63e2d4927d6419f0_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_242 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__63e2d4927d6419f0_arg_types_var_2201614657377291794, __type_info__63e2d4927d6419f0_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const ))>::size, UINT64_C(0x63e2d4927d6419f0), "preVisitExprConstEnumeration", offsetof(ast::AstVisitor,preVisitExprConstEnumeration), 0 };
TypeInfo * __type_info__c8ee8b2e94f52665_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__b6d18d4b3fadccd4 };
const char * __type_info__c8ee8b2e94f52665_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_243 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__c8ee8b2e94f52665_arg_types_var_2201614657377291794, __type_info__c8ee8b2e94f52665_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const ))>::size, UINT64_C(0xc8ee8b2e94f52665), "visitExprConstEnumeration", offsetof(ast::AstVisitor,visitExprConstEnumeration), 0 };
TypeInfo * __type_info__3575e938698ac7f2_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9c92a72bb3a64bfa };
const char * __type_info__3575e938698ac7f2_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_244 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3575e938698ac7f2_arg_types_var_2201614657377291794, __type_info__3575e938698ac7f2_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const ))>::size, UINT64_C(0x3575e938698ac7f2), "preVisitExprConstBitfield", offsetof(ast::AstVisitor,preVisitExprConstBitfield), 0 };
TypeInfo * __type_info__f9ae7a6ab1908649_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9c92a72bb3a64bfa };
const char * __type_info__f9ae7a6ab1908649_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_245 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f9ae7a6ab1908649_arg_types_var_2201614657377291794, __type_info__f9ae7a6ab1908649_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const ))>::size, UINT64_C(0xf9ae7a6ab1908649), "visitExprConstBitfield", offsetof(ast::AstVisitor,visitExprConstBitfield), 0 };
TypeInfo * __type_info__634c88c1255f0102_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4a0758d80e688a0e };
const char * __type_info__634c88c1255f0102_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_246 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__634c88c1255f0102_arg_types_var_2201614657377291794, __type_info__634c88c1255f0102_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const ))>::size, UINT64_C(0x634c88c1255f0102), "preVisitExprConstInt8", offsetof(ast::AstVisitor,preVisitExprConstInt8), 0 };
TypeInfo * __type_info__677ca996ba218a0f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4a0758d80e688a0e };
const char * __type_info__677ca996ba218a0f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_247 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__677ca996ba218a0f_arg_types_var_2201614657377291794, __type_info__677ca996ba218a0f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const ))>::size, UINT64_C(0x677ca996ba218a0f), "visitExprConstInt8", offsetof(ast::AstVisitor,visitExprConstInt8), 0 };
TypeInfo * __type_info__633a91c125407a4d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__87ae85b131d91f57 };
const char * __type_info__633a91c125407a4d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_248 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__633a91c125407a4d_arg_types_var_2201614657377291794, __type_info__633a91c125407a4d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const ))>::size, UINT64_C(0x633a91c125407a4d), "preVisitExprConstInt16", offsetof(ast::AstVisitor,preVisitExprConstInt16), 0 };
TypeInfo * __type_info__18f4641e60fa4fdb_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__87ae85b131d91f57 };
const char * __type_info__18f4641e60fa4fdb_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_249 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__18f4641e60fa4fdb_arg_types_var_2201614657377291794, __type_info__18f4641e60fa4fdb_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const ))>::size, UINT64_C(0x18f4641e60fa4fdb), "visitExprConstInt16", offsetof(ast::AstVisitor,visitExprConstInt16), 0 };
TypeInfo * __type_info__63388ac1253d0868_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__773524bb75b61932 };
const char * __type_info__63388ac1253d0868_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_250 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__63388ac1253d0868_arg_types_var_2201614657377291794, __type_info__63388ac1253d0868_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const ))>::size, UINT64_C(0x63388ac1253d0868), "preVisitExprConstInt64", offsetof(ast::AstVisitor,preVisitExprConstInt64), 0 };
TypeInfo * __type_info__29f2661e6f6a2041_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__773524bb75b61932 };
const char * __type_info__29f2661e6f6a2041_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_251 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__29f2661e6f6a2041_arg_types_var_2201614657377291794, __type_info__29f2661e6f6a2041_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const ))>::size, UINT64_C(0x29f2661e6f6a2041), "visitExprConstInt64", offsetof(ast::AstVisitor,visitExprConstInt64), 0 };
TypeInfo * __type_info__4a49f2369627a5ce_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__a28688c7ffe035ce };
const char * __type_info__4a49f2369627a5ce_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_252 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4a49f2369627a5ce_arg_types_var_2201614657377291794, __type_info__4a49f2369627a5ce_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const ))>::size, UINT64_C(0x4a49f2369627a5ce), "preVisitExprConstInt", offsetof(ast::AstVisitor,preVisitExprConstInt), 0 };
TypeInfo * __type_info__6794a996ba4a520f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__a28688c7ffe035ce };
const char * __type_info__6794a996ba4a520f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_253 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__6794a996ba4a520f_arg_types_var_2201614657377291794, __type_info__6794a996ba4a520f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const ))>::size, UINT64_C(0x6794a996ba4a520f), "visitExprConstInt", offsetof(ast::AstVisitor,visitExprConstInt), 0 };
TypeInfo * __type_info__634c8ec1255f0b34_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__2d27aed7dd587c30 };
const char * __type_info__634c8ec1255f0b34_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_254 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__634c8ec1255f0b34_arg_types_var_2201614657377291794, __type_info__634c8ec1255f0b34_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const ))>::size, UINT64_C(0x634c8ec1255f0b34), "preVisitExprConstInt2", offsetof(ast::AstVisitor,preVisitExprConstInt2), 0 };
TypeInfo * __type_info__6786a996ba32880f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__2d27aed7dd587c30 };
const char * __type_info__6786a996ba32880f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_255 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__6786a996ba32880f_arg_types_var_2201614657377291794, __type_info__6786a996ba32880f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const ))>::size, UINT64_C(0x6786a996ba32880f), "visitExprConstInt2", offsetof(ast::AstVisitor,visitExprConstInt2), 0 };
TypeInfo * __type_info__634c8fc1255f0ce7_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__31b7a5d7e3ad4eb7 };
const char * __type_info__634c8fc1255f0ce7_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_256 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__634c8fc1255f0ce7_arg_types_var_2201614657377291794, __type_info__634c8fc1255f0ce7_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const ))>::size, UINT64_C(0x634c8fc1255f0ce7), "preVisitExprConstInt3", offsetof(ast::AstVisitor,preVisitExprConstInt3), 0 };
TypeInfo * __type_info__6787a996ba343b0f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__31b7a5d7e3ad4eb7 };
const char * __type_info__6787a996ba343b0f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_257 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__6787a996ba343b0f_arg_types_var_2201614657377291794, __type_info__6787a996ba343b0f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const ))>::size, UINT64_C(0x6787a996ba343b0f), "visitExprConstInt3", offsetof(ast::AstVisitor,visitExprConstInt3), 0 };
TypeInfo * __type_info__634c8cc1255f07ce_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__2914e4d7d4fafa72 };
const char * __type_info__634c8cc1255f07ce_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_258 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__634c8cc1255f07ce_arg_types_var_2201614657377291794, __type_info__634c8cc1255f07ce_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const ))>::size, UINT64_C(0x634c8cc1255f07ce), "preVisitExprConstInt4", offsetof(ast::AstVisitor,preVisitExprConstInt4), 0 };
TypeInfo * __type_info__6788a996ba35ee0f_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__2914e4d7d4fafa72 };
const char * __type_info__6788a996ba35ee0f_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_259 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__6788a996ba35ee0f_arg_types_var_2201614657377291794, __type_info__6788a996ba35ee0f_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const ))>::size, UINT64_C(0x6788a996ba35ee0f), "visitExprConstInt4", offsetof(ast::AstVisitor,visitExprConstInt4), 0 };
TypeInfo * __type_info__cff23f236d195ae9_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f6614284ea50cbe7 };
const char * __type_info__cff23f236d195ae9_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_260 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__cff23f236d195ae9_arg_types_var_2201614657377291794, __type_info__cff23f236d195ae9_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const ))>::size, UINT64_C(0xcff23f236d195ae9), "preVisitExprConstUInt8", offsetof(ast::AstVisitor,preVisitExprConstUInt8), 0 };
TypeInfo * __type_info__569e217b0a57efab_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f6614284ea50cbe7 };
const char * __type_info__569e217b0a57efab_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_261 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__569e217b0a57efab_arg_types_var_2201614657377291794, __type_info__569e217b0a57efab_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const ))>::size, UINT64_C(0x569e217b0a57efab), "visitExprConstUInt8", offsetof(ast::AstVisitor,visitExprConstUInt8), 0 };
TypeInfo * __type_info__5a3228324ddf49ed_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f108ab47d962e793 };
const char * __type_info__5a3228324ddf49ed_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_262 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5a3228324ddf49ed_arg_types_var_2201614657377291794, __type_info__5a3228324ddf49ed_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const ))>::size, UINT64_C(0x5a3228324ddf49ed), "preVisitExprConstUInt16", offsetof(ast::AstVisitor,preVisitExprConstUInt16), 0 };
TypeInfo * __type_info__5688187b0a327e60_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__f108ab47d962e793 };
const char * __type_info__5688187b0a327e60_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_263 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__5688187b0a327e60_arg_types_var_2201614657377291794, __type_info__5688187b0a327e60_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const ))>::size, UINT64_C(0x5688187b0a327e60), "visitExprConstUInt16", offsetof(ast::AstVisitor,visitExprConstUInt16), 0 };
TypeInfo * __type_info__5d98263250c26f87_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__63e1b8a29ad93469 };
const char * __type_info__5d98263250c26f87_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_264 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5d98263250c26f87_arg_types_var_2201614657377291794, __type_info__5d98263250c26f87_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const ))>::size, UINT64_C(0x5d98263250c26f87), "preVisitExprConstUInt64", offsetof(ast::AstVisitor,preVisitExprConstUInt64), 0 };
TypeInfo * __type_info__568a1f7b0a35f045_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__63e1b8a29ad93469 };
const char * __type_info__568a1f7b0a35f045_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_265 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__568a1f7b0a35f045_arg_types_var_2201614657377291794, __type_info__568a1f7b0a35f045_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const ))>::size, UINT64_C(0x568a1f7b0a35f045), "visitExprConstUInt64", offsetof(ast::AstVisitor,visitExprConstUInt64), 0 };
TypeInfo * __type_info__d01a3f236d5d52e9_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__518dd4a2ad91defd };
const char * __type_info__d01a3f236d5d52e9_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_266 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d01a3f236d5d52e9_arg_types_var_2201614657377291794, __type_info__d01a3f236d5d52e9_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const ))>::size, UINT64_C(0xd01a3f236d5d52e9), "preVisitExprConstUInt", offsetof(ast::AstVisitor,preVisitExprConstUInt), 0 };
TypeInfo * __type_info__f6bea3965a10ae11_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__518dd4a2ad91defd };
const char * __type_info__f6bea3965a10ae11_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_267 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f6bea3965a10ae11_arg_types_var_2201614657377291794, __type_info__f6bea3965a10ae11_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const ))>::size, UINT64_C(0xf6bea3965a10ae11), "visitExprConstUInt", offsetof(ast::AstVisitor,visitExprConstUInt), 0 };
TypeInfo * __type_info__cfe83f236d085ce9_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__514742689af99de7 };
const char * __type_info__cfe83f236d085ce9_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_268 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__cfe83f236d085ce9_arg_types_var_2201614657377291794, __type_info__cfe83f236d085ce9_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const ))>::size, UINT64_C(0xcfe83f236d085ce9), "preVisitExprConstUInt2", offsetof(ast::AstVisitor,preVisitExprConstUInt2), 0 };
TypeInfo * __type_info__569e1b7b0a57e579_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__514742689af99de7 };
const char * __type_info__569e1b7b0a57e579_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_269 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__569e1b7b0a57e579_arg_types_var_2201614657377291794, __type_info__569e1b7b0a57e579_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const ))>::size, UINT64_C(0x569e1b7b0a57e579), "visitExprConstUInt2", offsetof(ast::AstVisitor,visitExprConstUInt2), 0 };
TypeInfo * __type_info__cfe93f236d0a0fe9_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__563e42636db280e7 };
const char * __type_info__cfe93f236d0a0fe9_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_270 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__cfe93f236d0a0fe9_arg_types_var_2201614657377291794, __type_info__cfe93f236d0a0fe9_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const ))>::size, UINT64_C(0xcfe93f236d0a0fe9), "preVisitExprConstUInt3", offsetof(ast::AstVisitor,preVisitExprConstUInt3), 0 };
TypeInfo * __type_info__569e1a7b0a57e3c6_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__563e42636db280e7 };
const char * __type_info__569e1a7b0a57e3c6_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_271 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__569e1a7b0a57e3c6_arg_types_var_2201614657377291794, __type_info__569e1a7b0a57e3c6_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const ))>::size, UINT64_C(0x569e1a7b0a57e3c6), "visitExprConstUInt3", offsetof(ast::AstVisitor,visitExprConstUInt3), 0 };
TypeInfo * __type_info__cfee3f236d128ee9_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__a17d42718ebadfe7 };
const char * __type_info__cfee3f236d128ee9_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_272 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__cfee3f236d128ee9_arg_types_var_2201614657377291794, __type_info__cfee3f236d128ee9_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const ))>::size, UINT64_C(0xcfee3f236d128ee9), "preVisitExprConstUInt4", offsetof(ast::AstVisitor,preVisitExprConstUInt4), 0 };
TypeInfo * __type_info__569e1d7b0a57e8df_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__a17d42718ebadfe7 };
const char * __type_info__569e1d7b0a57e8df_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_273 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__569e1d7b0a57e8df_arg_types_var_2201614657377291794, __type_info__569e1d7b0a57e8df_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const ))>::size, UINT64_C(0x569e1d7b0a57e8df), "visitExprConstUInt4", offsetof(ast::AstVisitor,visitExprConstUInt4), 0 };
TypeInfo * __type_info__23dd3c465c5aacf8_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__214ca0a8404236ce };
const char * __type_info__23dd3c465c5aacf8_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_274 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__23dd3c465c5aacf8_arg_types_var_2201614657377291794, __type_info__23dd3c465c5aacf8_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const ))>::size, UINT64_C(0x23dd3c465c5aacf8), "preVisitExprConstRange", offsetof(ast::AstVisitor,preVisitExprConstRange), 0 };
TypeInfo * __type_info__c0c341f22742465d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__214ca0a8404236ce };
const char * __type_info__c0c341f22742465d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_275 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__c0c341f22742465d_arg_types_var_2201614657377291794, __type_info__c0c341f22742465d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const ))>::size, UINT64_C(0xc0c341f22742465d), "visitExprConstRange", offsetof(ast::AstVisitor,visitExprConstRange), 0 };
TypeInfo * __type_info__aaa036240f9f1383_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__d2fee6b26665c989 };
const char * __type_info__aaa036240f9f1383_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_276 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__aaa036240f9f1383_arg_types_var_2201614657377291794, __type_info__aaa036240f9f1383_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const ))>::size, UINT64_C(0xaaa036240f9f1383), "preVisitExprConstURange", offsetof(ast::AstVisitor,preVisitExprConstURange), 0 };
TypeInfo * __type_info__f0945e93d1915b71_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__d2fee6b26665c989 };
const char * __type_info__f0945e93d1915b71_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_277 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f0945e93d1915b71_arg_types_var_2201614657377291794, __type_info__f0945e93d1915b71_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const ))>::size, UINT64_C(0xf0945e93d1915b71), "visitExprConstURange", offsetof(ast::AstVisitor,visitExprConstURange), 0 };
TypeInfo * __type_info__4b86398eedf1a60a_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4f02b717be42f032 };
const char * __type_info__4b86398eedf1a60a_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_278 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4b86398eedf1a60a_arg_types_var_2201614657377291794, __type_info__4b86398eedf1a60a_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const ))>::size, UINT64_C(0x4b86398eedf1a60a), "preVisitExprConstRange64", offsetof(ast::AstVisitor,preVisitExprConstRange64), 0 };
TypeInfo * __type_info__834b777876181e6b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4f02b717be42f032 };
const char * __type_info__834b777876181e6b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_279 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__834b777876181e6b_arg_types_var_2201614657377291794, __type_info__834b777876181e6b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const ))>::size, UINT64_C(0x834b777876181e6b), "visitExprConstRange64", offsetof(ast::AstVisitor,visitExprConstRange64), 0 };
TypeInfo * __type_info__af4bb646a82e19f5_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__b8524aede8fd2575 };
const char * __type_info__af4bb646a82e19f5_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_280 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__af4bb646a82e19f5_arg_types_var_2201614657377291794, __type_info__af4bb646a82e19f5_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const ))>::size, UINT64_C(0xaf4bb646a82e19f5), "preVisitExprConstURange64", offsetof(ast::AstVisitor,preVisitExprConstURange64), 0 };
TypeInfo * __type_info__5d8bfc2d1a2015a5_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__b8524aede8fd2575 };
const char * __type_info__5d8bfc2d1a2015a5_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_281 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__5d8bfc2d1a2015a5_arg_types_var_2201614657377291794, __type_info__5d8bfc2d1a2015a5_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const ))>::size, UINT64_C(0x5d8bfc2d1a2015a5), "visitExprConstURange64", offsetof(ast::AstVisitor,visitExprConstURange64), 0 };
TypeInfo * __type_info__68cb32f89791abb7_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__ea252439573ea197 };
const char * __type_info__68cb32f89791abb7_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_282 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__68cb32f89791abb7_arg_types_var_2201614657377291794, __type_info__68cb32f89791abb7_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const ))>::size, UINT64_C(0x68cb32f89791abb7), "preVisitExprConstBool", offsetof(ast::AstVisitor,preVisitExprConstBool), 0 };
TypeInfo * __type_info__6ab1c296bcd15d4d_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__ea252439573ea197 };
const char * __type_info__6ab1c296bcd15d4d_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_283 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__6ab1c296bcd15d4d_arg_types_var_2201614657377291794, __type_info__6ab1c296bcd15d4d_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const ))>::size, UINT64_C(0x6ab1c296bcd15d4d), "visitExprConstBool", offsetof(ast::AstVisitor,visitExprConstBool), 0 };
TypeInfo * __type_info__619835e3cb9cb14b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__50c7808637778d65 };
const char * __type_info__619835e3cb9cb14b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_284 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__619835e3cb9cb14b_arg_types_var_2201614657377291794, __type_info__619835e3cb9cb14b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const ))>::size, UINT64_C(0x619835e3cb9cb14b), "preVisitExprConstFloat", offsetof(ast::AstVisitor,preVisitExprConstFloat), 0 };
TypeInfo * __type_info__ab85832951adf487_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__50c7808637778d65 };
const char * __type_info__ab85832951adf487_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_285 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ab85832951adf487_arg_types_var_2201614657377291794, __type_info__ab85832951adf487_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const ))>::size, UINT64_C(0xab85832951adf487), "visitExprConstFloat", offsetof(ast::AstVisitor,visitExprConstFloat), 0 };
TypeInfo * __type_info__72550b12fb41909b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4f4cc10892c6c61 };
const char * __type_info__72550b12fb41909b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_286 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__72550b12fb41909b_arg_types_var_2201614657377291794, __type_info__72550b12fb41909b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const ))>::size, UINT64_C(0x72550b12fb41909b), "preVisitExprConstFloat2", offsetof(ast::AstVisitor,preVisitExprConstFloat2), 0 };
TypeInfo * __type_info__abb3832951fc1e87_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__4f4cc10892c6c61 };
const char * __type_info__abb3832951fc1e87_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_287 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__abb3832951fc1e87_arg_types_var_2201614657377291794, __type_info__abb3832951fc1e87_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const ))>::size, UINT64_C(0xabb3832951fc1e87), "visitExprConstFloat2", offsetof(ast::AstVisitor,visitExprConstFloat2), 0 };
TypeInfo * __type_info__72550a12fb418ee8_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__65f51082d9833a };
const char * __type_info__72550a12fb418ee8_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_288 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__72550a12fb418ee8_arg_types_var_2201614657377291794, __type_info__72550a12fb418ee8_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const ))>::size, UINT64_C(0x72550a12fb418ee8), "preVisitExprConstFloat3", offsetof(ast::AstVisitor,preVisitExprConstFloat3), 0 };
TypeInfo * __type_info__abb2832951fa6b87_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__65f51082d9833a };
const char * __type_info__abb2832951fa6b87_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_289 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__abb2832951fa6b87_arg_types_var_2201614657377291794, __type_info__abb2832951fa6b87_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const ))>::size, UINT64_C(0xabb2832951fa6b87), "visitExprConstFloat3", offsetof(ast::AstVisitor,visitExprConstFloat3), 0 };
TypeInfo * __type_info__72551112fb419acd_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9089610918ba11f };
const char * __type_info__72551112fb419acd_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_290 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__72551112fb419acd_arg_types_var_2201614657377291794, __type_info__72551112fb419acd_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const ))>::size, UINT64_C(0x72551112fb419acd), "preVisitExprConstFloat4", offsetof(ast::AstVisitor,preVisitExprConstFloat4), 0 };
TypeInfo * __type_info__abb1832951f8b887_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__9089610918ba11f };
const char * __type_info__abb1832951f8b887_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_291 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__abb1832951f8b887_arg_types_var_2201614657377291794, __type_info__abb1832951f8b887_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const ))>::size, UINT64_C(0xabb1832951f8b887), "visitExprConstFloat4", offsetof(ast::AstVisitor,visitExprConstFloat4), 0 };
TypeInfo * __type_info__6c8186fb9d866b04_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e9cb7c9300717d9e };
const char * __type_info__6c8186fb9d866b04_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_292 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6c8186fb9d866b04_arg_types_var_2201614657377291794, __type_info__6c8186fb9d866b04_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstString> const ))>::size, UINT64_C(0x6c8186fb9d866b04), "preVisitExprConstString", offsetof(ast::AstVisitor,preVisitExprConstString), 0 };
TypeInfo * __type_info__ac4a824df3a23f27_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__e9cb7c9300717d9e };
const char * __type_info__ac4a824df3a23f27_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_293 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ac4a824df3a23f27_arg_types_var_2201614657377291794, __type_info__ac4a824df3a23f27_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstString> const ))>::size, UINT64_C(0xac4a824df3a23f27), "visitExprConstString", offsetof(ast::AstVisitor,visitExprConstString), 0 };
TypeInfo * __type_info__2cab45e6e29dabd4_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7f81cc8503986a86 };
const char * __type_info__2cab45e6e29dabd4_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_294 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2cab45e6e29dabd4_arg_types_var_2201614657377291794, __type_info__2cab45e6e29dabd4_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const ))>::size, UINT64_C(0x2cab45e6e29dabd4), "preVisitExprConstDouble", offsetof(ast::AstVisitor,preVisitExprConstDouble), 0 };
TypeInfo * __type_info__566ddd22f95b7cfb_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__7f81cc8503986a86 };
const char * __type_info__566ddd22f95b7cfb_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_295 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__566ddd22f95b7cfb_arg_types_var_2201614657377291794, __type_info__566ddd22f95b7cfb_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const ))>::size, UINT64_C(0x566ddd22f95b7cfb), "visitExprConstDouble", offsetof(ast::AstVisitor,visitExprConstDouble), 0 };
TypeInfo * __type_info__6b597ff2a170677e_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__3229d47464f4ad50 };
const char * __type_info__6b597ff2a170677e_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_296 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6b597ff2a170677e_arg_types_var_2201614657377291794, __type_info__6b597ff2a170677e_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const ))>::size, UINT64_C(0x6b597ff2a170677e), "preVisitExprFakeContext", offsetof(ast::AstVisitor,preVisitExprFakeContext), 0 };
TypeInfo * __type_info__635ac63675961b5b_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__3229d47464f4ad50 };
const char * __type_info__635ac63675961b5b_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_297 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__635ac63675961b5b_arg_types_var_2201614657377291794, __type_info__635ac63675961b5b_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const ))>::size, UINT64_C(0x635ac63675961b5b), "visitExprFakeContext", offsetof(ast::AstVisitor,visitExprFakeContext), 0 };
TypeInfo * __type_info__5790fbaa5aa10da3_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__511818eae83f8137 };
const char * __type_info__5790fbaa5aa10da3_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_298 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5790fbaa5aa10da3_arg_types_var_2201614657377291794, __type_info__5790fbaa5aa10da3_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const ))>::size, UINT64_C(0x5790fbaa5aa10da3), "preVisitExprFakeLineInfo", offsetof(ast::AstVisitor,preVisitExprFakeLineInfo), 0 };
TypeInfo * __type_info__44f160a6dbf6c463_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__511818eae83f8137 };
const char * __type_info__44f160a6dbf6c463_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_299 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__44f160a6dbf6c463_arg_types_var_2201614657377291794, __type_info__44f160a6dbf6c463_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const ))>::size, UINT64_C(0x44f160a6dbf6c463), "visitExprFakeLineInfo", offsetof(ast::AstVisitor,visitExprFakeLineInfo), 0 };
TypeInfo * __type_info__a1e2cce18d36fa97_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6628bcbce7db6a7d };
const char * __type_info__a1e2cce18d36fa97_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_300 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a1e2cce18d36fa97_arg_types_var_2201614657377291794, __type_info__a1e2cce18d36fa97_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReader> const ))>::size, UINT64_C(0xa1e2cce18d36fa97), "preVisitExprReader", offsetof(ast::AstVisitor,preVisitExprReader), 0 };
TypeInfo * __type_info__5992ea8a5e268051_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__6628bcbce7db6a7d };
const char * __type_info__5992ea8a5e268051_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_301 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__5992ea8a5e268051_arg_types_var_2201614657377291794, __type_info__5992ea8a5e268051_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReader> const ))>::size, UINT64_C(0x5992ea8a5e268051), "visitExprReader", offsetof(ast::AstVisitor,visitExprReader), 0 };
TypeInfo * __type_info__893bf90e80994551_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__eb22258b16c8c6df };
const char * __type_info__893bf90e80994551_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_302 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__893bf90e80994551_arg_types_var_2201614657377291794, __type_info__893bf90e80994551_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const ))>::size, UINT64_C(0x893bf90e80994551), "preVisitExprUnsafe", offsetof(ast::AstVisitor,preVisitExprUnsafe), 0 };
TypeInfo * __type_info__436fe705f6119254_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__eb22258b16c8c6df };
const char * __type_info__436fe705f6119254_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_303 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__436fe705f6119254_arg_types_var_2201614657377291794, __type_info__436fe705f6119254_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const ))>::size, UINT64_C(0x436fe705f6119254), "visitExprUnsafe", offsetof(ast::AstVisitor,visitExprUnsafe), 0 };
TypeInfo * __type_info__9e31d981d2f6e393_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__636dc1714c171367 };
const char * __type_info__9e31d981d2f6e393_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_304 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9e31d981d2f6e393_arg_types_var_2201614657377291794, __type_info__9e31d981d2f6e393_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const ))>::size, UINT64_C(0x9e31d981d2f6e393), "preVisitExprCallMacro", offsetof(ast::AstVisitor,preVisitExprCallMacro), 0 };
TypeInfo * __type_info__8517df657f4530a_arg_types_var_2201614657377291794[2] = { &__type_info__21586ce84f433a21, &__type_info__636dc1714c171367 };
const char * __type_info__8517df657f4530a_arg_names_var_2201614657377291794[2] = { "self", "expr" };
VarInfo __struct_info__1e8db4ddc1444e12_field_305 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__8517df657f4530a_arg_types_var_2201614657377291794, __type_info__8517df657f4530a_arg_names_var_2201614657377291794, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const ))>::size, UINT64_C(0x8517df657f4530a), "visitExprCallMacro", offsetof(ast::AstVisitor,visitExprCallMacro), 0 };
VarInfo * __struct_info__1e8db4ddc1444e12_fields[306] =  { &__struct_info__1e8db4ddc1444e12_field_0, &__struct_info__1e8db4ddc1444e12_field_1, &__struct_info__1e8db4ddc1444e12_field_2, &__struct_info__1e8db4ddc1444e12_field_3, &__struct_info__1e8db4ddc1444e12_field_4, &__struct_info__1e8db4ddc1444e12_field_5, &__struct_info__1e8db4ddc1444e12_field_6, &__struct_info__1e8db4ddc1444e12_field_7, &__struct_info__1e8db4ddc1444e12_field_8, &__struct_info__1e8db4ddc1444e12_field_9, &__struct_info__1e8db4ddc1444e12_field_10, &__struct_info__1e8db4ddc1444e12_field_11, &__struct_info__1e8db4ddc1444e12_field_12, &__struct_info__1e8db4ddc1444e12_field_13, &__struct_info__1e8db4ddc1444e12_field_14, &__struct_info__1e8db4ddc1444e12_field_15, &__struct_info__1e8db4ddc1444e12_field_16, &__struct_info__1e8db4ddc1444e12_field_17, &__struct_info__1e8db4ddc1444e12_field_18, &__struct_info__1e8db4ddc1444e12_field_19, &__struct_info__1e8db4ddc1444e12_field_20, &__struct_info__1e8db4ddc1444e12_field_21, &__struct_info__1e8db4ddc1444e12_field_22, &__struct_info__1e8db4ddc1444e12_field_23, &__struct_info__1e8db4ddc1444e12_field_24, &__struct_info__1e8db4ddc1444e12_field_25, &__struct_info__1e8db4ddc1444e12_field_26, &__struct_info__1e8db4ddc1444e12_field_27, &__struct_info__1e8db4ddc1444e12_field_28, &__struct_info__1e8db4ddc1444e12_field_29, &__struct_info__1e8db4ddc1444e12_field_30, &__struct_info__1e8db4ddc1444e12_field_31, &__struct_info__1e8db4ddc1444e12_field_32, &__struct_info__1e8db4ddc1444e12_field_33, &__struct_info__1e8db4ddc1444e12_field_34, &__struct_info__1e8db4ddc1444e12_field_35, &__struct_info__1e8db4ddc1444e12_field_36, &__struct_info__1e8db4ddc1444e12_field_37, &__struct_info__1e8db4ddc1444e12_field_38, &__struct_info__1e8db4ddc1444e12_field_39, &__struct_info__1e8db4ddc1444e12_field_40, &__struct_info__1e8db4ddc1444e12_field_41, &__struct_info__1e8db4ddc1444e12_field_42, &__struct_info__1e8db4ddc1444e12_field_43, &__struct_info__1e8db4ddc1444e12_field_44, &__struct_info__1e8db4ddc1444e12_field_45, &__struct_info__1e8db4ddc1444e12_field_46, &__struct_info__1e8db4ddc1444e12_field_47, &__struct_info__1e8db4ddc1444e12_field_48, &__struct_info__1e8db4ddc1444e12_field_49, &__struct_info__1e8db4ddc1444e12_field_50, &__struct_info__1e8db4ddc1444e12_field_51, &__struct_info__1e8db4ddc1444e12_field_52, &__struct_info__1e8db4ddc1444e12_field_53, &__struct_info__1e8db4ddc1444e12_field_54, &__struct_info__1e8db4ddc1444e12_field_55, &__struct_info__1e8db4ddc1444e12_field_56, &__struct_info__1e8db4ddc1444e12_field_57, &__struct_info__1e8db4ddc1444e12_field_58, &__struct_info__1e8db4ddc1444e12_field_59, &__struct_info__1e8db4ddc1444e12_field_60, &__struct_info__1e8db4ddc1444e12_field_61, &__struct_info__1e8db4ddc1444e12_field_62, &__struct_info__1e8db4ddc1444e12_field_63, &__struct_info__1e8db4ddc1444e12_field_64, &__struct_info__1e8db4ddc1444e12_field_65, &__struct_info__1e8db4ddc1444e12_field_66, &__struct_info__1e8db4ddc1444e12_field_67, &__struct_info__1e8db4ddc1444e12_field_68, &__struct_info__1e8db4ddc1444e12_field_69, &__struct_info__1e8db4ddc1444e12_field_70, &__struct_info__1e8db4ddc1444e12_field_71, &__struct_info__1e8db4ddc1444e12_field_72, &__struct_info__1e8db4ddc1444e12_field_73, &__struct_info__1e8db4ddc1444e12_field_74, &__struct_info__1e8db4ddc1444e12_field_75, &__struct_info__1e8db4ddc1444e12_field_76, &__struct_info__1e8db4ddc1444e12_field_77, &__struct_info__1e8db4ddc1444e12_field_78, &__struct_info__1e8db4ddc1444e12_field_79, &__struct_info__1e8db4ddc1444e12_field_80, &__struct_info__1e8db4ddc1444e12_field_81, &__struct_info__1e8db4ddc1444e12_field_82, &__struct_info__1e8db4ddc1444e12_field_83, &__struct_info__1e8db4ddc1444e12_field_84, &__struct_info__1e8db4ddc1444e12_field_85, &__struct_info__1e8db4ddc1444e12_field_86, &__struct_info__1e8db4ddc1444e12_field_87, &__struct_info__1e8db4ddc1444e12_field_88, &__struct_info__1e8db4ddc1444e12_field_89, &__struct_info__1e8db4ddc1444e12_field_90, &__struct_info__1e8db4ddc1444e12_field_91, &__struct_info__1e8db4ddc1444e12_field_92, &__struct_info__1e8db4ddc1444e12_field_93, &__struct_info__1e8db4ddc1444e12_field_94, &__struct_info__1e8db4ddc1444e12_field_95, &__struct_info__1e8db4ddc1444e12_field_96, &__struct_info__1e8db4ddc1444e12_field_97, &__struct_info__1e8db4ddc1444e12_field_98, &__struct_info__1e8db4ddc1444e12_field_99, &__struct_info__1e8db4ddc1444e12_field_100, &__struct_info__1e8db4ddc1444e12_field_101, &__struct_info__1e8db4ddc1444e12_field_102, &__struct_info__1e8db4ddc1444e12_field_103, &__struct_info__1e8db4ddc1444e12_field_104, &__struct_info__1e8db4ddc1444e12_field_105, &__struct_info__1e8db4ddc1444e12_field_106, &__struct_info__1e8db4ddc1444e12_field_107, &__struct_info__1e8db4ddc1444e12_field_108, &__struct_info__1e8db4ddc1444e12_field_109, &__struct_info__1e8db4ddc1444e12_field_110, &__struct_info__1e8db4ddc1444e12_field_111, &__struct_info__1e8db4ddc1444e12_field_112, &__struct_info__1e8db4ddc1444e12_field_113, &__struct_info__1e8db4ddc1444e12_field_114, &__struct_info__1e8db4ddc1444e12_field_115, &__struct_info__1e8db4ddc1444e12_field_116, &__struct_info__1e8db4ddc1444e12_field_117, &__struct_info__1e8db4ddc1444e12_field_118, &__struct_info__1e8db4ddc1444e12_field_119, &__struct_info__1e8db4ddc1444e12_field_120, &__struct_info__1e8db4ddc1444e12_field_121, &__struct_info__1e8db4ddc1444e12_field_122, &__struct_info__1e8db4ddc1444e12_field_123, &__struct_info__1e8db4ddc1444e12_field_124, &__struct_info__1e8db4ddc1444e12_field_125, &__struct_info__1e8db4ddc1444e12_field_126, &__struct_info__1e8db4ddc1444e12_field_127, &__struct_info__1e8db4ddc1444e12_field_128, &__struct_info__1e8db4ddc1444e12_field_129, &__struct_info__1e8db4ddc1444e12_field_130, &__struct_info__1e8db4ddc1444e12_field_131, &__struct_info__1e8db4ddc1444e12_field_132, &__struct_info__1e8db4ddc1444e12_field_133, &__struct_info__1e8db4ddc1444e12_field_134, &__struct_info__1e8db4ddc1444e12_field_135, &__struct_info__1e8db4ddc1444e12_field_136, &__struct_info__1e8db4ddc1444e12_field_137, &__struct_info__1e8db4ddc1444e12_field_138, &__struct_info__1e8db4ddc1444e12_field_139, &__struct_info__1e8db4ddc1444e12_field_140, &__struct_info__1e8db4ddc1444e12_field_141, &__struct_info__1e8db4ddc1444e12_field_142, &__struct_info__1e8db4ddc1444e12_field_143, &__struct_info__1e8db4ddc1444e12_field_144, &__struct_info__1e8db4ddc1444e12_field_145, &__struct_info__1e8db4ddc1444e12_field_146, &__struct_info__1e8db4ddc1444e12_field_147, &__struct_info__1e8db4ddc1444e12_field_148, &__struct_info__1e8db4ddc1444e12_field_149, &__struct_info__1e8db4ddc1444e12_field_150, &__struct_info__1e8db4ddc1444e12_field_151, &__struct_info__1e8db4ddc1444e12_field_152, &__struct_info__1e8db4ddc1444e12_field_153, &__struct_info__1e8db4ddc1444e12_field_154, &__struct_info__1e8db4ddc1444e12_field_155, &__struct_info__1e8db4ddc1444e12_field_156, &__struct_info__1e8db4ddc1444e12_field_157, &__struct_info__1e8db4ddc1444e12_field_158, &__struct_info__1e8db4ddc1444e12_field_159, &__struct_info__1e8db4ddc1444e12_field_160, &__struct_info__1e8db4ddc1444e12_field_161, &__struct_info__1e8db4ddc1444e12_field_162, &__struct_info__1e8db4ddc1444e12_field_163, &__struct_info__1e8db4ddc1444e12_field_164, &__struct_info__1e8db4ddc1444e12_field_165, &__struct_info__1e8db4ddc1444e12_field_166, &__struct_info__1e8db4ddc1444e12_field_167, &__struct_info__1e8db4ddc1444e12_field_168, &__struct_info__1e8db4ddc1444e12_field_169, &__struct_info__1e8db4ddc1444e12_field_170, &__struct_info__1e8db4ddc1444e12_field_171, &__struct_info__1e8db4ddc1444e12_field_172, &__struct_info__1e8db4ddc1444e12_field_173, &__struct_info__1e8db4ddc1444e12_field_174, &__struct_info__1e8db4ddc1444e12_field_175, &__struct_info__1e8db4ddc1444e12_field_176, &__struct_info__1e8db4ddc1444e12_field_177, &__struct_info__1e8db4ddc1444e12_field_178, &__struct_info__1e8db4ddc1444e12_field_179, &__struct_info__1e8db4ddc1444e12_field_180, &__struct_info__1e8db4ddc1444e12_field_181, &__struct_info__1e8db4ddc1444e12_field_182, &__struct_info__1e8db4ddc1444e12_field_183, &__struct_info__1e8db4ddc1444e12_field_184, &__struct_info__1e8db4ddc1444e12_field_185, &__struct_info__1e8db4ddc1444e12_field_186, &__struct_info__1e8db4ddc1444e12_field_187, &__struct_info__1e8db4ddc1444e12_field_188, &__struct_info__1e8db4ddc1444e12_field_189, &__struct_info__1e8db4ddc1444e12_field_190, &__struct_info__1e8db4ddc1444e12_field_191, &__struct_info__1e8db4ddc1444e12_field_192, &__struct_info__1e8db4ddc1444e12_field_193, &__struct_info__1e8db4ddc1444e12_field_194, &__struct_info__1e8db4ddc1444e12_field_195, &__struct_info__1e8db4ddc1444e12_field_196, &__struct_info__1e8db4ddc1444e12_field_197, &__struct_info__1e8db4ddc1444e12_field_198, &__struct_info__1e8db4ddc1444e12_field_199, &__struct_info__1e8db4ddc1444e12_field_200, &__struct_info__1e8db4ddc1444e12_field_201, &__struct_info__1e8db4ddc1444e12_field_202, &__struct_info__1e8db4ddc1444e12_field_203, &__struct_info__1e8db4ddc1444e12_field_204, &__struct_info__1e8db4ddc1444e12_field_205, &__struct_info__1e8db4ddc1444e12_field_206, &__struct_info__1e8db4ddc1444e12_field_207, &__struct_info__1e8db4ddc1444e12_field_208, &__struct_info__1e8db4ddc1444e12_field_209, &__struct_info__1e8db4ddc1444e12_field_210, &__struct_info__1e8db4ddc1444e12_field_211, &__struct_info__1e8db4ddc1444e12_field_212, &__struct_info__1e8db4ddc1444e12_field_213, &__struct_info__1e8db4ddc1444e12_field_214, &__struct_info__1e8db4ddc1444e12_field_215, &__struct_info__1e8db4ddc1444e12_field_216, &__struct_info__1e8db4ddc1444e12_field_217, &__struct_info__1e8db4ddc1444e12_field_218, &__struct_info__1e8db4ddc1444e12_field_219, &__struct_info__1e8db4ddc1444e12_field_220, &__struct_info__1e8db4ddc1444e12_field_221, &__struct_info__1e8db4ddc1444e12_field_222, &__struct_info__1e8db4ddc1444e12_field_223, &__struct_info__1e8db4ddc1444e12_field_224, &__struct_info__1e8db4ddc1444e12_field_225, &__struct_info__1e8db4ddc1444e12_field_226, &__struct_info__1e8db4ddc1444e12_field_227, &__struct_info__1e8db4ddc1444e12_field_228, &__struct_info__1e8db4ddc1444e12_field_229, &__struct_info__1e8db4ddc1444e12_field_230, &__struct_info__1e8db4ddc1444e12_field_231, &__struct_info__1e8db4ddc1444e12_field_232, &__struct_info__1e8db4ddc1444e12_field_233, &__struct_info__1e8db4ddc1444e12_field_234, &__struct_info__1e8db4ddc1444e12_field_235, &__struct_info__1e8db4ddc1444e12_field_236, &__struct_info__1e8db4ddc1444e12_field_237, &__struct_info__1e8db4ddc1444e12_field_238, &__struct_info__1e8db4ddc1444e12_field_239, &__struct_info__1e8db4ddc1444e12_field_240, &__struct_info__1e8db4ddc1444e12_field_241, &__struct_info__1e8db4ddc1444e12_field_242, &__struct_info__1e8db4ddc1444e12_field_243, &__struct_info__1e8db4ddc1444e12_field_244, &__struct_info__1e8db4ddc1444e12_field_245, &__struct_info__1e8db4ddc1444e12_field_246, &__struct_info__1e8db4ddc1444e12_field_247, &__struct_info__1e8db4ddc1444e12_field_248, &__struct_info__1e8db4ddc1444e12_field_249, &__struct_info__1e8db4ddc1444e12_field_250, &__struct_info__1e8db4ddc1444e12_field_251, &__struct_info__1e8db4ddc1444e12_field_252, &__struct_info__1e8db4ddc1444e12_field_253, &__struct_info__1e8db4ddc1444e12_field_254, &__struct_info__1e8db4ddc1444e12_field_255, &__struct_info__1e8db4ddc1444e12_field_256, &__struct_info__1e8db4ddc1444e12_field_257, &__struct_info__1e8db4ddc1444e12_field_258, &__struct_info__1e8db4ddc1444e12_field_259, &__struct_info__1e8db4ddc1444e12_field_260, &__struct_info__1e8db4ddc1444e12_field_261, &__struct_info__1e8db4ddc1444e12_field_262, &__struct_info__1e8db4ddc1444e12_field_263, &__struct_info__1e8db4ddc1444e12_field_264, &__struct_info__1e8db4ddc1444e12_field_265, &__struct_info__1e8db4ddc1444e12_field_266, &__struct_info__1e8db4ddc1444e12_field_267, &__struct_info__1e8db4ddc1444e12_field_268, &__struct_info__1e8db4ddc1444e12_field_269, &__struct_info__1e8db4ddc1444e12_field_270, &__struct_info__1e8db4ddc1444e12_field_271, &__struct_info__1e8db4ddc1444e12_field_272, &__struct_info__1e8db4ddc1444e12_field_273, &__struct_info__1e8db4ddc1444e12_field_274, &__struct_info__1e8db4ddc1444e12_field_275, &__struct_info__1e8db4ddc1444e12_field_276, &__struct_info__1e8db4ddc1444e12_field_277, &__struct_info__1e8db4ddc1444e12_field_278, &__struct_info__1e8db4ddc1444e12_field_279, &__struct_info__1e8db4ddc1444e12_field_280, &__struct_info__1e8db4ddc1444e12_field_281, &__struct_info__1e8db4ddc1444e12_field_282, &__struct_info__1e8db4ddc1444e12_field_283, &__struct_info__1e8db4ddc1444e12_field_284, &__struct_info__1e8db4ddc1444e12_field_285, &__struct_info__1e8db4ddc1444e12_field_286, &__struct_info__1e8db4ddc1444e12_field_287, &__struct_info__1e8db4ddc1444e12_field_288, &__struct_info__1e8db4ddc1444e12_field_289, &__struct_info__1e8db4ddc1444e12_field_290, &__struct_info__1e8db4ddc1444e12_field_291, &__struct_info__1e8db4ddc1444e12_field_292, &__struct_info__1e8db4ddc1444e12_field_293, &__struct_info__1e8db4ddc1444e12_field_294, &__struct_info__1e8db4ddc1444e12_field_295, &__struct_info__1e8db4ddc1444e12_field_296, &__struct_info__1e8db4ddc1444e12_field_297, &__struct_info__1e8db4ddc1444e12_field_298, &__struct_info__1e8db4ddc1444e12_field_299, &__struct_info__1e8db4ddc1444e12_field_300, &__struct_info__1e8db4ddc1444e12_field_301, &__struct_info__1e8db4ddc1444e12_field_302, &__struct_info__1e8db4ddc1444e12_field_303, &__struct_info__1e8db4ddc1444e12_field_304, &__struct_info__1e8db4ddc1444e12_field_305 };
StructInfo __struct_info__1e8db4ddc1444e12 = {"AstVisitor", "ast", 13, __struct_info__1e8db4ddc1444e12_fields, 306, TypeSize<ast::AstVisitor>::size, UINT64_C(0x0), nullptr, UINT64_C(0x1e8db4ddc1444e12), 0 };
VarInfo __struct_info__9a6c40d2ef34a474_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0x9e64c6f0cab96bde), "__rtti", offsetof(lint::LintVisitor,__rtti), 306 };
TypeInfo * __type_info__d882593a02577e1e_arg_types_var_11127340054026167412[1] = { &__type_info__21586ce84f433a21 };
const char * __type_info__d882593a02577e1e_arg_names_var_11127340054026167412[1] = { "self" };
VarInfo __struct_info__9a6c40d2ef34a474_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d882593a02577e1e_arg_types_var_11127340054026167412, __type_info__d882593a02577e1e_arg_names_var_11127340054026167412, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor))>::size, UINT64_C(0xd882593a02577e1e), "__finalize", offsetof(lint::LintVisitor,__finalize), 0 };
TypeInfo * __type_info__2e2b343561c670f6_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890 };
const char * __type_info__2e2b343561c670f6_arg_names_var_11127340054026167412[2] = { "self", "prog" };
VarInfo __struct_info__9a6c40d2ef34a474_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2e2b343561c670f6_arg_types_var_11127340054026167412, __type_info__2e2b343561c670f6_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ))>::size, UINT64_C(0x2e2b343561c670f6), "preVisitProgram", offsetof(lint::LintVisitor,preVisitProgram), 0 };
TypeInfo * __type_info__e9b1cddb87221d62_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890 };
const char * __type_info__e9b1cddb87221d62_arg_names_var_11127340054026167412[2] = { "self", "porg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e9b1cddb87221d62_arg_types_var_11127340054026167412, __type_info__e9b1cddb87221d62_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ))>::size, UINT64_C(0xe9b1cddb87221d62), "visitProgram", offsetof(lint::LintVisitor,visitProgram), 0 };
TypeInfo * __type_info__ac26f19d59c637e6_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802 };
const char * __type_info__ac26f19d59c637e6_arg_names_var_11127340054026167412[3] = { "self", "prog", "mod" };
VarInfo __struct_info__9a6c40d2ef34a474_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ac26f19d59c637e6_arg_types_var_11127340054026167412, __type_info__ac26f19d59c637e6_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ,Module * const ))>::size, UINT64_C(0xac26f19d59c637e6), "preVisitProgramBody", offsetof(lint::LintVisitor,preVisitProgramBody), 0 };
TypeInfo * __type_info__9ff398c70462259_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e266b5ccef058802 };
const char * __type_info__9ff398c70462259_arg_names_var_11127340054026167412[2] = { "self", "mod" };
VarInfo __struct_info__9a6c40d2ef34a474_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9ff398c70462259_arg_types_var_11127340054026167412, __type_info__9ff398c70462259_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,Module * const ))>::size, UINT64_C(0x9ff398c70462259), "preVisitModule", offsetof(lint::LintVisitor,preVisitModule), 0 };
TypeInfo * __type_info__ada4f71ee74db4e3_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e266b5ccef058802 };
const char * __type_info__ada4f71ee74db4e3_arg_names_var_11127340054026167412[2] = { "self", "mod" };
VarInfo __struct_info__9a6c40d2ef34a474_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ada4f71ee74db4e3_arg_types_var_11127340054026167412, __type_info__ada4f71ee74db4e3_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,Module * const ))>::size, UINT64_C(0xada4f71ee74db4e3), "visitModule", offsetof(lint::LintVisitor,visitModule), 0 };
TypeInfo * __type_info__840e4ecfb5fc51ee_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__3b037c8d587730b0 };
const char * __type_info__840e4ecfb5fc51ee_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__840e4ecfb5fc51ee_arg_types_var_11127340054026167412, __type_info__840e4ecfb5fc51ee_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const ))>::size, UINT64_C(0x840e4ecfb5fc51ee), "preVisitExprTypeDecl", offsetof(lint::LintVisitor,preVisitExprTypeDecl), 0 };
TypeInfo * __type_info__ae7b494b958cfd1d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__3b037c8d587730b0 };
const char * __type_info__ae7b494b958cfd1d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ae7b494b958cfd1d_arg_types_var_11127340054026167412, __type_info__ae7b494b958cfd1d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeDecl> const ))>::size, UINT64_C(0xae7b494b958cfd1d), "visitExprTypeDecl", offsetof(lint::LintVisitor,visitExprTypeDecl), 0 };
TypeInfo * __type_info__4708f19442da76f3_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__defb2f7795e0cf8c };
const char * __type_info__4708f19442da76f3_arg_names_var_11127340054026167412[2] = { "self", "typ" };
VarInfo __struct_info__9a6c40d2ef34a474_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4708f19442da76f3_arg_types_var_11127340054026167412, __type_info__4708f19442da76f3_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ))>::size, UINT64_C(0x4708f19442da76f3), "preVisitTypeDecl", offsetof(lint::LintVisitor,preVisitTypeDecl), 0 };
TypeInfo * __type_info__5bb638a4cd1f16c7_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__defb2f7795e0cf8c };
const char * __type_info__5bb638a4cd1f16c7_arg_names_var_11127340054026167412[2] = { "self", "typ" };
VarInfo __struct_info__9a6c40d2ef34a474_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__afcf203e0d7d50d, nullptr, (TypeInfo **)__type_info__5bb638a4cd1f16c7_arg_types_var_11127340054026167412, __type_info__5bb638a4cd1f16c7_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ))>::size, UINT64_C(0x5bb638a4cd1f16c7), "visitTypeDecl", offsetof(lint::LintVisitor,visitTypeDecl), 0 };
TypeInfo * __type_info__e1b49e90afbc1255_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__defb2f7795e0cf8c, &__type_info__624d371c76b25aa4 };
const char * __type_info__e1b49e90afbc1255_arg_names_var_11127340054026167412[3] = { "self", "typ", "name" };
VarInfo __struct_info__9a6c40d2ef34a474_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e1b49e90afbc1255_arg_types_var_11127340054026167412, __type_info__e1b49e90afbc1255_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const ))>::size, UINT64_C(0xe1b49e90afbc1255), "preVisitAlias", offsetof(lint::LintVisitor,preVisitAlias), 0 };
TypeInfo * __type_info__b4668103b07b5523_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__defb2f7795e0cf8c, &__type_info__624d371c76b25aa4 };
const char * __type_info__b4668103b07b5523_arg_names_var_11127340054026167412[3] = { "self", "typ", "name" };
VarInfo __struct_info__9a6c40d2ef34a474_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__afcf203e0d7d50d, nullptr, (TypeInfo **)__type_info__b4668103b07b5523_arg_types_var_11127340054026167412, __type_info__b4668103b07b5523_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<TypeDecl>,ast::AstVisitor,smart_ptr_raw<TypeDecl> const ,das::string const ))>::size, UINT64_C(0xb4668103b07b5523), "visitAlias", offsetof(lint::LintVisitor,visitAlias), 0 };
TypeInfo * __type_info__833b891d4a8b6f3b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7c61f7ae88617bb2 };
const char * __type_info__833b891d4a8b6f3b_arg_names_var_11127340054026167412[2] = { "self", "arg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__833b891d4a8b6f3b_arg_types_var_11127340054026167412, __type_info__833b891d4a8b6f3b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Enumeration * const ))>::size, UINT64_C(0x833b891d4a8b6f3b), "canVisitEnumeration", offsetof(lint::LintVisitor,canVisitEnumeration), 0 };
TypeInfo * __type_info__63c1cccc6cee5499_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__586f0da79a6e613d };
const char * __type_info__63c1cccc6cee5499_arg_names_var_11127340054026167412[2] = { "self", "enu" };
VarInfo __struct_info__9a6c40d2ef34a474_field_14 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__63c1cccc6cee5499_arg_types_var_11127340054026167412, __type_info__63c1cccc6cee5499_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const ))>::size, UINT64_C(0x63c1cccc6cee5499), "preVisitEnumeration", offsetof(lint::LintVisitor,preVisitEnumeration), 0 };
TypeInfo * __type_info__c77126596c8747_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__586f0da79a6e613d, &__type_info__624d371c76b25aa4, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__c77126596c8747_arg_names_var_11127340054026167412[5] = { "self", "enu", "name", "value", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_15 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c77126596c8747_arg_types_var_11127340054026167412, __type_info__c77126596c8747_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xc77126596c8747), "preVisitEnumerationValue", offsetof(lint::LintVisitor,preVisitEnumerationValue), 0 };
TypeInfo * __type_info__5c209af46fdd2c15_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__586f0da79a6e613d, &__type_info__624d371c76b25aa4, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__5c209af46fdd2c15_arg_names_var_11127340054026167412[5] = { "self", "enu", "name", "value", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_16 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__5c209af46fdd2c15_arg_types_var_11127340054026167412, __type_info__5c209af46fdd2c15_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Enumeration> const ,das::string const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x5c209af46fdd2c15), "visitEnumerationValue", offsetof(lint::LintVisitor,visitEnumerationValue), 0 };
TypeInfo * __type_info__a65992bc741b8af8_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__586f0da79a6e613d };
const char * __type_info__a65992bc741b8af8_arg_names_var_11127340054026167412[2] = { "self", "enu" };
VarInfo __struct_info__9a6c40d2ef34a474_field_17 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__e4765bc563f255e, nullptr, (TypeInfo **)__type_info__a65992bc741b8af8_arg_types_var_11127340054026167412, __type_info__a65992bc741b8af8_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Enumeration>,ast::AstVisitor,smart_ptr_raw<Enumeration> const ))>::size, UINT64_C(0xa65992bc741b8af8), "visitEnumeration", offsetof(lint::LintVisitor,visitEnumeration), 0 };
TypeInfo * __type_info__d0bef68cc9ed3938_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f9220d94c6b964b5 };
const char * __type_info__d0bef68cc9ed3938_arg_names_var_11127340054026167412[2] = { "self", "arg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_18 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__d0bef68cc9ed3938_arg_types_var_11127340054026167412, __type_info__d0bef68cc9ed3938_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Structure * const ))>::size, UINT64_C(0xd0bef68cc9ed3938), "canVisitStructure", offsetof(lint::LintVisitor,canVisitStructure), 0 };
TypeInfo * __type_info__53e236a279c289ba_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27 };
const char * __type_info__53e236a279c289ba_arg_names_var_11127340054026167412[2] = { "self", "str" };
VarInfo __struct_info__9a6c40d2ef34a474_field_19 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__53e236a279c289ba_arg_types_var_11127340054026167412, __type_info__53e236a279c289ba_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ))>::size, UINT64_C(0x53e236a279c289ba), "preVisitStructure", offsetof(lint::LintVisitor,preVisitStructure), 0 };
TypeInfo * __type_info__7151322babbded1f_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27, &__type_info__29c0090cdbf7525c, &__type_info__af81fe4c86352052 };
const char * __type_info__7151322babbded1f_arg_names_var_11127340054026167412[4] = { "self", "str", "decl", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_20 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7151322babbded1f_arg_types_var_11127340054026167412, __type_info__7151322babbded1f_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool))>::size, UINT64_C(0x7151322babbded1f), "preVisitStructureField", offsetof(lint::LintVisitor,preVisitStructureField), 0 };
TypeInfo * __type_info__5f7eac88f2e9604f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27 };
const char * __type_info__5f7eac88f2e9604f_arg_names_var_11127340054026167412[2] = { "self", "st" };
VarInfo __struct_info__9a6c40d2ef34a474_field_21 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__5f7eac88f2e9604f_arg_types_var_11127340054026167412, __type_info__5f7eac88f2e9604f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<Structure> const ))>::size, UINT64_C(0x5f7eac88f2e9604f), "canVisitStructureFieldInit", offsetof(lint::LintVisitor,canVisitStructureFieldInit), 0 };
TypeInfo * __type_info__81c83a50daf2259c_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27, &__type_info__29c0090cdbf7525c, &__type_info__af81fe4c86352052 };
const char * __type_info__81c83a50daf2259c_arg_names_var_11127340054026167412[4] = { "self", "str", "decl", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_22 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__81c83a50daf2259c_arg_types_var_11127340054026167412, __type_info__81c83a50daf2259c_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const ,bool))>::size, UINT64_C(0x81c83a50daf2259c), "visitStructureField", offsetof(lint::LintVisitor,visitStructureField), 0 };
TypeInfo * __type_info__dcd63514f66ab091_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__d6b8ed05d16e9f27 };
const char * __type_info__dcd63514f66ab091_arg_names_var_11127340054026167412[2] = { "self", "str" };
VarInfo __struct_info__9a6c40d2ef34a474_field_23 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__7e104fcf0cd430e4, nullptr, (TypeInfo **)__type_info__dcd63514f66ab091_arg_types_var_11127340054026167412, __type_info__dcd63514f66ab091_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Structure>,ast::AstVisitor,smart_ptr_raw<Structure> const ))>::size, UINT64_C(0xdcd63514f66ab091), "visitStructure", offsetof(lint::LintVisitor,visitStructure), 0 };
TypeInfo * __type_info__4777ccaf68ef0cb9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__34b7c04894c15d5 };
const char * __type_info__4777ccaf68ef0cb9_arg_names_var_11127340054026167412[2] = { "self", "fun" };
VarInfo __struct_info__9a6c40d2ef34a474_field_24 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__4777ccaf68ef0cb9_arg_types_var_11127340054026167412, __type_info__4777ccaf68ef0cb9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Function * const ))>::size, UINT64_C(0x4777ccaf68ef0cb9), "canVisitFunction", offsetof(lint::LintVisitor,canVisitFunction), 0 };
TypeInfo * __type_info__647a3ae14f674180_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__34b7c04894c15d5, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__647a3ae14f674180_arg_names_var_11127340054026167412[4] = { "self", "fun", "arg", "value" };
VarInfo __struct_info__9a6c40d2ef34a474_field_25 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__647a3ae14f674180_arg_types_var_11127340054026167412, __type_info__647a3ae14f674180_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Function * const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x647a3ae14f674180), "canVisitFunctionArgumentInit", offsetof(lint::LintVisitor,canVisitFunctionArgumentInit), 0 };
TypeInfo * __type_info__f42cff0ada36ad6f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0 };
const char * __type_info__f42cff0ada36ad6f_arg_names_var_11127340054026167412[2] = { "self", "fun" };
VarInfo __struct_info__9a6c40d2ef34a474_field_26 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f42cff0ada36ad6f_arg_types_var_11127340054026167412, __type_info__f42cff0ada36ad6f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ))>::size, UINT64_C(0xf42cff0ada36ad6f), "preVisitFunction", offsetof(lint::LintVisitor,preVisitFunction), 0 };
TypeInfo * __type_info__22cdbe72e3c2d697_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0 };
const char * __type_info__22cdbe72e3c2d697_arg_names_var_11127340054026167412[2] = { "self", "fun" };
VarInfo __struct_info__9a6c40d2ef34a474_field_27 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4cdbed951d30a5d1, nullptr, (TypeInfo **)__type_info__22cdbe72e3c2d697_arg_types_var_11127340054026167412, __type_info__22cdbe72e3c2d697_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Function>,ast::AstVisitor,smart_ptr_raw<Function> const ))>::size, UINT64_C(0x22cdbe72e3c2d697), "visitFunction", offsetof(lint::LintVisitor,visitFunction), 0 };
TypeInfo * __type_info__ff10808a7d9c2c20_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__ff10808a7d9c2c20_arg_names_var_11127340054026167412[4] = { "self", "fun", "arg", "lastArg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_28 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ff10808a7d9c2c20_arg_types_var_11127340054026167412, __type_info__ff10808a7d9c2c20_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xff10808a7d9c2c20), "preVisitFunctionArgument", offsetof(lint::LintVisitor,preVisitFunctionArgument), 0 };
TypeInfo * __type_info__8e0815d127579fcf_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__8e0815d127579fcf_arg_names_var_11127340054026167412[4] = { "self", "fun", "arg", "lastArg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_29 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__8e0815d127579fcf_arg_types_var_11127340054026167412, __type_info__8e0815d127579fcf_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x8e0815d127579fcf), "visitFunctionArgument", offsetof(lint::LintVisitor,visitFunctionArgument), 0 };
TypeInfo * __type_info__8638cbc831704766_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__8638cbc831704766_arg_names_var_11127340054026167412[4] = { "self", "fun", "arg", "value" };
VarInfo __struct_info__9a6c40d2ef34a474_field_30 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8638cbc831704766_arg_types_var_11127340054026167412, __type_info__8638cbc831704766_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x8638cbc831704766), "preVisitFunctionArgumentInit", offsetof(lint::LintVisitor,preVisitFunctionArgumentInit), 0 };
TypeInfo * __type_info__d436bbf954ba2085_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__d436bbf954ba2085_arg_names_var_11127340054026167412[4] = { "self", "fun", "arg", "value" };
VarInfo __struct_info__9a6c40d2ef34a474_field_31 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__d436bbf954ba2085_arg_types_var_11127340054026167412, __type_info__d436bbf954ba2085_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xd436bbf954ba2085), "visitFunctionArgumentInit", offsetof(lint::LintVisitor,visitFunctionArgumentInit), 0 };
TypeInfo * __type_info__16eeb6a9049d5149_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__98064c57b4bcca5a };
const char * __type_info__16eeb6a9049d5149_arg_names_var_11127340054026167412[3] = { "self", "fun", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_32 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__16eeb6a9049d5149_arg_types_var_11127340054026167412, __type_info__16eeb6a9049d5149_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x16eeb6a9049d5149), "preVisitFunctionBody", offsetof(lint::LintVisitor,preVisitFunctionBody), 0 };
TypeInfo * __type_info__46a59fb09c5853e3_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__9d10785eb07580e0, &__type_info__98064c57b4bcca5a };
const char * __type_info__46a59fb09c5853e3_arg_names_var_11127340054026167412[3] = { "self", "fun", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_33 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__46a59fb09c5853e3_arg_types_var_11127340054026167412, __type_info__46a59fb09c5853e3_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x46a59fb09c5853e3), "visitFunctionBody", offsetof(lint::LintVisitor,visitFunctionBody), 0 };
TypeInfo * __type_info__9f5382e006be621c_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__98064c57b4bcca5a };
const char * __type_info__9f5382e006be621c_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_34 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9f5382e006be621c_arg_types_var_11127340054026167412, __type_info__9f5382e006be621c_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x9f5382e006be621c), "preVisitExpression", offsetof(lint::LintVisitor,preVisitExpression), 0 };
TypeInfo * __type_info__a77ccea6ca1951d8_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__98064c57b4bcca5a };
const char * __type_info__a77ccea6ca1951d8_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_35 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__a77ccea6ca1951d8_arg_types_var_11127340054026167412, __type_info__a77ccea6ca1951d8_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xa77ccea6ca1951d8), "visitExpression", offsetof(lint::LintVisitor,visitExpression), 0 };
TypeInfo * __type_info__ab44ba6ec45d54c5_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7 };
const char * __type_info__ab44ba6ec45d54c5_arg_names_var_11127340054026167412[2] = { "self", "blk" };
VarInfo __struct_info__9a6c40d2ef34a474_field_36 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ab44ba6ec45d54c5_arg_types_var_11127340054026167412, __type_info__ab44ba6ec45d54c5_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::size, UINT64_C(0xab44ba6ec45d54c5), "preVisitExprBlock", offsetof(lint::LintVisitor,preVisitExprBlock), 0 };
TypeInfo * __type_info__ed0f26b53887bb93_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7 };
const char * __type_info__ed0f26b53887bb93_arg_names_var_11127340054026167412[2] = { "self", "blk" };
VarInfo __struct_info__9a6c40d2ef34a474_field_37 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ed0f26b53887bb93_arg_types_var_11127340054026167412, __type_info__ed0f26b53887bb93_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::size, UINT64_C(0xed0f26b53887bb93), "visitExprBlock", offsetof(lint::LintVisitor,visitExprBlock), 0 };
TypeInfo * __type_info__10391d132c60dad1_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__10391d132c60dad1_arg_names_var_11127340054026167412[4] = { "self", "blk", "arg", "lastArg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_38 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__10391d132c60dad1_arg_types_var_11127340054026167412, __type_info__10391d132c60dad1_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x10391d132c60dad1), "preVisitExprBlockArgument", offsetof(lint::LintVisitor,preVisitExprBlockArgument), 0 };
TypeInfo * __type_info__ed1cae0006658d0c_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__ed1cae0006658d0c_arg_names_var_11127340054026167412[4] = { "self", "blk", "arg", "lastArg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_39 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__ed1cae0006658d0c_arg_types_var_11127340054026167412, __type_info__ed1cae0006658d0c_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xed1cae0006658d0c), "visitExprBlockArgument", offsetof(lint::LintVisitor,visitExprBlockArgument), 0 };
TypeInfo * __type_info__7e408614dc19391b_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__7e408614dc19391b_arg_names_var_11127340054026167412[4] = { "self", "blk", "arg", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_40 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7e408614dc19391b_arg_types_var_11127340054026167412, __type_info__7e408614dc19391b_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x7e408614dc19391b), "preVisitExprBlockArgumentInit", offsetof(lint::LintVisitor,preVisitExprBlockArgumentInit), 0 };
TypeInfo * __type_info__66a857925ac8022_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__66a857925ac8022_arg_names_var_11127340054026167412[4] = { "self", "blk", "arg", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_41 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__66a857925ac8022_arg_types_var_11127340054026167412, __type_info__66a857925ac8022_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x66a857925ac8022), "visitExprBlockArgumentInit", offsetof(lint::LintVisitor,visitExprBlockArgumentInit), 0 };
TypeInfo * __type_info__d7a4159cdfc723cb_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__98064c57b4bcca5a };
const char * __type_info__d7a4159cdfc723cb_arg_names_var_11127340054026167412[3] = { "self", "blk", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_42 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d7a4159cdfc723cb_arg_types_var_11127340054026167412, __type_info__d7a4159cdfc723cb_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xd7a4159cdfc723cb), "preVisitExprBlockExpression", offsetof(lint::LintVisitor,preVisitExprBlockExpression), 0 };
TypeInfo * __type_info__8e75b6de22c6f733_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__98064c57b4bcca5a };
const char * __type_info__8e75b6de22c6f733_arg_names_var_11127340054026167412[3] = { "self", "blk", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_43 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__8e75b6de22c6f733_arg_types_var_11127340054026167412, __type_info__8e75b6de22c6f733_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x8e75b6de22c6f733), "visitExprBlockExpression", offsetof(lint::LintVisitor,visitExprBlockExpression), 0 };
TypeInfo * __type_info__9bae04658879e2df_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7 };
const char * __type_info__9bae04658879e2df_arg_names_var_11127340054026167412[2] = { "self", "blk" };
VarInfo __struct_info__9a6c40d2ef34a474_field_44 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9bae04658879e2df_arg_types_var_11127340054026167412, __type_info__9bae04658879e2df_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::size, UINT64_C(0x9bae04658879e2df), "preVisitExprBlockFinal", offsetof(lint::LintVisitor,preVisitExprBlockFinal), 0 };
TypeInfo * __type_info__a4bc79fe7060d06d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7 };
const char * __type_info__a4bc79fe7060d06d_arg_names_var_11127340054026167412[2] = { "self", "blk" };
VarInfo __struct_info__9a6c40d2ef34a474_field_45 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a4bc79fe7060d06d_arg_types_var_11127340054026167412, __type_info__a4bc79fe7060d06d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::size, UINT64_C(0xa4bc79fe7060d06d), "visitExprBlockFinal", offsetof(lint::LintVisitor,visitExprBlockFinal), 0 };
TypeInfo * __type_info__2abfc3db9ec2782f_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__98064c57b4bcca5a };
const char * __type_info__2abfc3db9ec2782f_arg_names_var_11127340054026167412[3] = { "self", "blk", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_46 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2abfc3db9ec2782f_arg_types_var_11127340054026167412, __type_info__2abfc3db9ec2782f_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x2abfc3db9ec2782f), "preVisitExprBlockFinalExpression", offsetof(lint::LintVisitor,preVisitExprBlockFinalExpression), 0 };
TypeInfo * __type_info__1fcc98c439158013_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__4dee28f2a93bbef7, &__type_info__98064c57b4bcca5a };
const char * __type_info__1fcc98c439158013_arg_names_var_11127340054026167412[3] = { "self", "blk", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_47 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__1fcc98c439158013_arg_types_var_11127340054026167412, __type_info__1fcc98c439158013_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x1fcc98c439158013), "visitExprBlockFinalExpression", offsetof(lint::LintVisitor,visitExprBlockFinalExpression), 0 };
TypeInfo * __type_info__2924f017b7732c03_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f };
const char * __type_info__2924f017b7732c03_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_48 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2924f017b7732c03_arg_types_var_11127340054026167412, __type_info__2924f017b7732c03_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ))>::size, UINT64_C(0x2924f017b7732c03), "preVisitExprLet", offsetof(lint::LintVisitor,preVisitExprLet), 0 };
TypeInfo * __type_info__9793e695dfb5080d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f };
const char * __type_info__9793e695dfb5080d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_49 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9793e695dfb5080d_arg_types_var_11127340054026167412, __type_info__9793e695dfb5080d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ))>::size, UINT64_C(0x9793e695dfb5080d), "visitExprLet", offsetof(lint::LintVisitor,visitExprLet), 0 };
TypeInfo * __type_info__8013d1dae5465d78_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__8013d1dae5465d78_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "lastArg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_50 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8013d1dae5465d78_arg_types_var_11127340054026167412, __type_info__8013d1dae5465d78_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x8013d1dae5465d78), "preVisitExprLetVariable", offsetof(lint::LintVisitor,preVisitExprLetVariable), 0 };
TypeInfo * __type_info__6c8f59f18418b3bc_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__6c8f59f18418b3bc_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "lastArg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_51 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__6c8f59f18418b3bc_arg_types_var_11127340054026167412, __type_info__6c8f59f18418b3bc_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x6c8f59f18418b3bc), "visitExprLetVariable", offsetof(lint::LintVisitor,visitExprLetVariable), 0 };
TypeInfo * __type_info__12d8f6d9d5dead62_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__12d8f6d9d5dead62_arg_names_var_11127340054026167412[4] = { "self", "blk", "arg", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_52 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__12d8f6d9d5dead62_arg_types_var_11127340054026167412, __type_info__12d8f6d9d5dead62_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x12d8f6d9d5dead62), "preVisitExprLetVariableInit", offsetof(lint::LintVisitor,preVisitExprLetVariableInit), 0 };
TypeInfo * __type_info__b6140d01f1004992_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__349161eed600549f, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__b6140d01f1004992_arg_names_var_11127340054026167412[4] = { "self", "blk", "arg", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_53 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__b6140d01f1004992_arg_types_var_11127340054026167412, __type_info__b6140d01f1004992_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xb6140d01f1004992), "visitExprLetVariableInit", offsetof(lint::LintVisitor,visitExprLetVariableInit), 0 };
TypeInfo * __type_info__2a804375e6080a20_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__79c6e4b278757551 };
const char * __type_info__2a804375e6080a20_arg_names_var_11127340054026167412[2] = { "self", "arg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_54 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__2a804375e6080a20_arg_types_var_11127340054026167412, __type_info__2a804375e6080a20_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,Variable * const ))>::size, UINT64_C(0x2a804375e6080a20), "canVisitGlobalVariable", offsetof(lint::LintVisitor,canVisitGlobalVariable), 0 };
TypeInfo * __type_info__75401d533b922a37_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890 };
const char * __type_info__75401d533b922a37_arg_names_var_11127340054026167412[2] = { "self", "prog" };
VarInfo __struct_info__9a6c40d2ef34a474_field_55 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__75401d533b922a37_arg_types_var_11127340054026167412, __type_info__75401d533b922a37_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ))>::size, UINT64_C(0x75401d533b922a37), "preVisitGlobalLet", offsetof(lint::LintVisitor,preVisitGlobalLet), 0 };
TypeInfo * __type_info__17e6083f17fb7dd1_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e57b0f261f47b890 };
const char * __type_info__17e6083f17fb7dd1_arg_names_var_11127340054026167412[2] = { "self", "prog" };
VarInfo __struct_info__9a6c40d2ef34a474_field_56 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__17e6083f17fb7dd1_arg_types_var_11127340054026167412, __type_info__17e6083f17fb7dd1_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Program> const ))>::size, UINT64_C(0x17e6083f17fb7dd1), "visitGlobalLet", offsetof(lint::LintVisitor,visitGlobalLet), 0 };
TypeInfo * __type_info__c9097b5866a3e0c4_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__c9097b5866a3e0c4_arg_names_var_11127340054026167412[3] = { "self", "arg", "lastArg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_57 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c9097b5866a3e0c4_arg_types_var_11127340054026167412, __type_info__c9097b5866a3e0c4_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xc9097b5866a3e0c4), "preVisitGlobalLetVariable", offsetof(lint::LintVisitor,preVisitGlobalLetVariable), 0 };
TypeInfo * __type_info__f0ce9e1e38a98150_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__f0ce9e1e38a98150_arg_names_var_11127340054026167412[3] = { "self", "arg", "lastArg" };
VarInfo __struct_info__9a6c40d2ef34a474_field_58 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__f0ce9e1e38a98150_arg_types_var_11127340054026167412, __type_info__f0ce9e1e38a98150_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xf0ce9e1e38a98150), "visitGlobalLetVariable", offsetof(lint::LintVisitor,visitGlobalLetVariable), 0 };
TypeInfo * __type_info__6e537ad87a3c52de_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__6e537ad87a3c52de_arg_names_var_11127340054026167412[3] = { "self", "arg", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_59 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6e537ad87a3c52de_arg_types_var_11127340054026167412, __type_info__6e537ad87a3c52de_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x6e537ad87a3c52de), "preVisitGlobalLetVariableInit", offsetof(lint::LintVisitor,preVisitGlobalLetVariableInit), 0 };
TypeInfo * __type_info__86031d69a049c796_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__a3a6bcfebaf8fcd8, &__type_info__98064c57b4bcca5a };
const char * __type_info__86031d69a049c796_arg_names_var_11127340054026167412[3] = { "self", "arg", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_60 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__86031d69a049c796_arg_types_var_11127340054026167412, __type_info__86031d69a049c796_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x86031d69a049c796), "visitGlobalLetVariableInit", offsetof(lint::LintVisitor,visitGlobalLetVariableInit), 0 };
TypeInfo * __type_info__705cf6b886595b4b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4e7dff8bb14f539 };
const char * __type_info__705cf6b886595b4b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_61 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__705cf6b886595b4b_arg_types_var_11127340054026167412, __type_info__705cf6b886595b4b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ))>::size, UINT64_C(0x705cf6b886595b4b), "preVisitExprStringBuilder", offsetof(lint::LintVisitor,preVisitExprStringBuilder), 0 };
TypeInfo * __type_info__727047bcfbe30c92_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4e7dff8bb14f539 };
const char * __type_info__727047bcfbe30c92_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_62 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__727047bcfbe30c92_arg_types_var_11127340054026167412, __type_info__727047bcfbe30c92_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ))>::size, UINT64_C(0x727047bcfbe30c92), "visitExprStringBuilder", offsetof(lint::LintVisitor,visitExprStringBuilder), 0 };
TypeInfo * __type_info__3be14e162d5565e2_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__4e7dff8bb14f539, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__3be14e162d5565e2_arg_names_var_11127340054026167412[4] = { "self", "expr", "elem", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_63 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3be14e162d5565e2_arg_types_var_11127340054026167412, __type_info__3be14e162d5565e2_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x3be14e162d5565e2), "preVisitExprStringBuilderElement", offsetof(lint::LintVisitor,preVisitExprStringBuilderElement), 0 };
TypeInfo * __type_info__f934d2ee83d4bb41_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__4e7dff8bb14f539, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__f934d2ee83d4bb41_arg_names_var_11127340054026167412[4] = { "self", "expr", "elem", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_64 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f934d2ee83d4bb41_arg_types_var_11127340054026167412, __type_info__f934d2ee83d4bb41_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xf934d2ee83d4bb41), "visitExprStringBuilderElement", offsetof(lint::LintVisitor,visitExprStringBuilderElement), 0 };
TypeInfo * __type_info__291e0117b76d2a38_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__5d6138f13e1e88c4 };
const char * __type_info__291e0117b76d2a38_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_65 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__291e0117b76d2a38_arg_types_var_11127340054026167412, __type_info__291e0117b76d2a38_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const ))>::size, UINT64_C(0x291e0117b76d2a38), "preVisitExprNew", offsetof(lint::LintVisitor,preVisitExprNew), 0 };
TypeInfo * __type_info__9080e695d976110d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__5d6138f13e1e88c4 };
const char * __type_info__9080e695d976110d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_66 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9080e695d976110d_arg_types_var_11127340054026167412, __type_info__9080e695d976110d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const ))>::size, UINT64_C(0x9080e695d976110d), "visitExprNew", offsetof(lint::LintVisitor,visitExprNew), 0 };
TypeInfo * __type_info__76f6080637f13298_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__5d6138f13e1e88c4, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__76f6080637f13298_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_67 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__76f6080637f13298_arg_types_var_11127340054026167412, __type_info__76f6080637f13298_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x76f6080637f13298), "preVisitExprNewArgument", offsetof(lint::LintVisitor,preVisitExprNewArgument), 0 };
TypeInfo * __type_info__6eef9939275ad076_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__5d6138f13e1e88c4, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__6eef9939275ad076_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_68 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__6eef9939275ad076_arg_types_var_11127340054026167412, __type_info__6eef9939275ad076_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x6eef9939275ad076), "visitExprNewArgument", offsetof(lint::LintVisitor,visitExprNewArgument), 0 };
TypeInfo * __type_info__54cf37e64cce5f7b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__44cd26f4cb3df7e1 };
const char * __type_info__54cf37e64cce5f7b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_69 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__54cf37e64cce5f7b_arg_types_var_11127340054026167412, __type_info__54cf37e64cce5f7b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ))>::size, UINT64_C(0x54cf37e64cce5f7b), "preVisitExprNamedCall", offsetof(lint::LintVisitor,preVisitExprNamedCall), 0 };
TypeInfo * __type_info__a3ff2104b7343583_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__44cd26f4cb3df7e1 };
const char * __type_info__a3ff2104b7343583_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_70 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__a3ff2104b7343583_arg_types_var_11127340054026167412, __type_info__a3ff2104b7343583_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ))>::size, UINT64_C(0xa3ff2104b7343583), "visitExprNamedCall", offsetof(lint::LintVisitor,visitExprNamedCall), 0 };
TypeInfo * __type_info__1c5d80edd6565c9b_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__44cd26f4cb3df7e1, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__1c5d80edd6565c9b_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_71 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1c5d80edd6565c9b_arg_types_var_11127340054026167412, __type_info__1c5d80edd6565c9b_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0x1c5d80edd6565c9b), "preVisitExprNamedCallArgument", offsetof(lint::LintVisitor,preVisitExprNamedCallArgument), 0 };
TypeInfo * __type_info__89e2f19de6bd36fc_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__44cd26f4cb3df7e1, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__89e2f19de6bd36fc_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_72 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__c52835f1e7c9ab84, nullptr, (TypeInfo **)__type_info__89e2f19de6bd36fc_arg_types_var_11127340054026167412, __type_info__89e2f19de6bd36fc_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0x89e2f19de6bd36fc), "visitExprNamedCallArgument", offsetof(lint::LintVisitor,visitExprNamedCallArgument), 0 };
TypeInfo * __type_info__de85442bf10db318_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a };
const char * __type_info__de85442bf10db318_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_73 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__de85442bf10db318_arg_types_var_11127340054026167412, __type_info__de85442bf10db318_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ))>::size, UINT64_C(0xde85442bf10db318), "preVisitExprLooksLikeCall", offsetof(lint::LintVisitor,preVisitExprLooksLikeCall), 0 };
TypeInfo * __type_info__873b34d6be33396e_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a };
const char * __type_info__873b34d6be33396e_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_74 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__873b34d6be33396e_arg_types_var_11127340054026167412, __type_info__873b34d6be33396e_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ))>::size, UINT64_C(0x873b34d6be33396e), "visitExprLooksLikeCall", offsetof(lint::LintVisitor,visitExprLooksLikeCall), 0 };
TypeInfo * __type_info__933eb58cc40d3f86_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__933eb58cc40d3f86_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_75 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__933eb58cc40d3f86_arg_types_var_11127340054026167412, __type_info__933eb58cc40d3f86_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x933eb58cc40d3f86), "canVisitExprLooksLikeCallArgument", offsetof(lint::LintVisitor,canVisitExprLooksLikeCallArgument), 0 };
TypeInfo * __type_info__a131fea67520578_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__a131fea67520578_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_76 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a131fea67520578_arg_types_var_11127340054026167412, __type_info__a131fea67520578_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xa131fea67520578), "preVisitExprLooksLikeCallArgument", offsetof(lint::LintVisitor,preVisitExprLooksLikeCallArgument), 0 };
TypeInfo * __type_info__53a75570ca4525d5_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__8faf3ae8c5ebe47a, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__53a75570ca4525d5_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_77 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__53a75570ca4525d5_arg_types_var_11127340054026167412, __type_info__53a75570ca4525d5_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x53a75570ca4525d5), "visitExprLooksLikeCallArgument", offsetof(lint::LintVisitor,visitExprLooksLikeCallArgument), 0 };
TypeInfo * __type_info__317dd97cc75b2d63_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7e04c1d12891d606 };
const char * __type_info__317dd97cc75b2d63_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_78 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__317dd97cc75b2d63_arg_types_var_11127340054026167412, __type_info__317dd97cc75b2d63_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,ExprCall * const ))>::size, UINT64_C(0x317dd97cc75b2d63), "canVisitCall", offsetof(lint::LintVisitor,canVisitCall), 0 };
TypeInfo * __type_info__1bec0617ac85373c_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7dd1c1d1283b2d06 };
const char * __type_info__1bec0617ac85373c_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_79 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1bec0617ac85373c_arg_types_var_11127340054026167412, __type_info__1bec0617ac85373c_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const ))>::size, UINT64_C(0x1bec0617ac85373c), "preVisitExprCall", offsetof(lint::LintVisitor,preVisitExprCall), 0 };
TypeInfo * __type_info__9e9c31b94df55077_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7dd1c1d1283b2d06 };
const char * __type_info__9e9c31b94df55077_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_80 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9e9c31b94df55077_arg_types_var_11127340054026167412, __type_info__9e9c31b94df55077_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const ))>::size, UINT64_C(0x9e9c31b94df55077), "visitExprCall", offsetof(lint::LintVisitor,visitExprCall), 0 };
TypeInfo * __type_info__e67d8f86e92f853b_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__7dd1c1d1283b2d06, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__e67d8f86e92f853b_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_81 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e67d8f86e92f853b_arg_types_var_11127340054026167412, __type_info__e67d8f86e92f853b_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xe67d8f86e92f853b), "preVisitExprCallArgument", offsetof(lint::LintVisitor,preVisitExprCallArgument), 0 };
TypeInfo * __type_info__39fa3c745babf5af_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__7dd1c1d1283b2d06, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__39fa3c745babf5af_arg_names_var_11127340054026167412[4] = { "self", "expr", "arg", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_82 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__39fa3c745babf5af_arg_types_var_11127340054026167412, __type_info__39fa3c745babf5af_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x39fa3c745babf5af), "visitExprCallArgument", offsetof(lint::LintVisitor,visitExprCallArgument), 0 };
TypeInfo * __type_info__935c60ae663c7fbd_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e7e2063b91ac55a1 };
const char * __type_info__935c60ae663c7fbd_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_83 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__935c60ae663c7fbd_arg_types_var_11127340054026167412, __type_info__935c60ae663c7fbd_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const ))>::size, UINT64_C(0x935c60ae663c7fbd), "preVisitExprNullCoalescing", offsetof(lint::LintVisitor,preVisitExprNullCoalescing), 0 };
TypeInfo * __type_info__e8030adc2c0c141d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e7e2063b91ac55a1 };
const char * __type_info__e8030adc2c0c141d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_84 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__e8030adc2c0c141d_arg_types_var_11127340054026167412, __type_info__e8030adc2c0c141d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const ))>::size, UINT64_C(0xe8030adc2c0c141d), "visitExprNullCoalescing", offsetof(lint::LintVisitor,visitExprNullCoalescing), 0 };
TypeInfo * __type_info__875dd885be78b606_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__e7e2063b91ac55a1, &__type_info__98064c57b4bcca5a };
const char * __type_info__875dd885be78b606_arg_names_var_11127340054026167412[3] = { "self", "expr", "defval" };
VarInfo __struct_info__9a6c40d2ef34a474_field_85 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__875dd885be78b606_arg_types_var_11127340054026167412, __type_info__875dd885be78b606_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprNullCoalescing> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x875dd885be78b606), "preVisitExprNullCoalescingDefault", offsetof(lint::LintVisitor,preVisitExprNullCoalescingDefault), 0 };
TypeInfo * __type_info__bb4514f284b9061e_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__241df6ccda394202 };
const char * __type_info__bb4514f284b9061e_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_86 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bb4514f284b9061e_arg_types_var_11127340054026167412, __type_info__bb4514f284b9061e_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ))>::size, UINT64_C(0xbb4514f284b9061e), "preVisitExprAt", offsetof(lint::LintVisitor,preVisitExprAt), 0 };
TypeInfo * __type_info__a83dd595ed95fc2a_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__241df6ccda394202 };
const char * __type_info__a83dd595ed95fc2a_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_87 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__a83dd595ed95fc2a_arg_types_var_11127340054026167412, __type_info__a83dd595ed95fc2a_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAt> const ))>::size, UINT64_C(0xa83dd595ed95fc2a), "visitExprAt", offsetof(lint::LintVisitor,visitExprAt), 0 };
TypeInfo * __type_info__9e5eb39ccbbc5fc1_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__241df6ccda394202, &__type_info__98064c57b4bcca5a };
const char * __type_info__9e5eb39ccbbc5fc1_arg_names_var_11127340054026167412[3] = { "self", "expr", "index" };
VarInfo __struct_info__9a6c40d2ef34a474_field_88 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9e5eb39ccbbc5fc1_arg_types_var_11127340054026167412, __type_info__9e5eb39ccbbc5fc1_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x9e5eb39ccbbc5fc1), "preVisitExprAtIndex", offsetof(lint::LintVisitor,preVisitExprAtIndex), 0 };
TypeInfo * __type_info__aa7e0339be21b2b1_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__b5e62a55ec68b6b5 };
const char * __type_info__aa7e0339be21b2b1_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_89 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__aa7e0339be21b2b1_arg_types_var_11127340054026167412, __type_info__aa7e0339be21b2b1_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ))>::size, UINT64_C(0xaa7e0339be21b2b1), "preVisitExprSafeAt", offsetof(lint::LintVisitor,preVisitExprSafeAt), 0 };
TypeInfo * __type_info__96df0db0ae527f88_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__b5e62a55ec68b6b5 };
const char * __type_info__96df0db0ae527f88_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_90 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__96df0db0ae527f88_arg_types_var_11127340054026167412, __type_info__96df0db0ae527f88_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ))>::size, UINT64_C(0x96df0db0ae527f88), "visitExprSafeAt", offsetof(lint::LintVisitor,visitExprSafeAt), 0 };
TypeInfo * __type_info__610ec238d014a794_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__b5e62a55ec68b6b5, &__type_info__98064c57b4bcca5a };
const char * __type_info__610ec238d014a794_arg_names_var_11127340054026167412[3] = { "self", "expr", "index" };
VarInfo __struct_info__9a6c40d2ef34a474_field_91 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__610ec238d014a794_arg_types_var_11127340054026167412, __type_info__610ec238d014a794_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAt> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x610ec238d014a794), "preVisitExprSafeAtIndex", offsetof(lint::LintVisitor,preVisitExprSafeAtIndex), 0 };
TypeInfo * __type_info__bb400cf284b07986_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__8a5e2edb26418a2a };
const char * __type_info__bb400cf284b07986_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_92 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bb400cf284b07986_arg_types_var_11127340054026167412, __type_info__bb400cf284b07986_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const ))>::size, UINT64_C(0xbb400cf284b07986), "preVisitExprIs", offsetof(lint::LintVisitor,preVisitExprIs), 0 };
TypeInfo * __type_info__8d0dd095d67cabab_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__8a5e2edb26418a2a };
const char * __type_info__8d0dd095d67cabab_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_93 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__8d0dd095d67cabab_arg_types_var_11127340054026167412, __type_info__8d0dd095d67cabab_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIs> const ))>::size, UINT64_C(0x8d0dd095d67cabab), "visitExprIs", offsetof(lint::LintVisitor,visitExprIs), 0 };
TypeInfo * __type_info__508a3df51e5cd12_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__8a5e2edb26418a2a, &__type_info__defb2f7795e0cf8c };
const char * __type_info__508a3df51e5cd12_arg_names_var_11127340054026167412[3] = { "self", "expr", "typeDecl" };
VarInfo __struct_info__9a6c40d2ef34a474_field_94 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__508a3df51e5cd12_arg_types_var_11127340054026167412, __type_info__508a3df51e5cd12_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIs> const ,smart_ptr_raw<TypeDecl> const ))>::size, UINT64_C(0x508a3df51e5cd12), "preVisitExprIsType", offsetof(lint::LintVisitor,preVisitExprIsType), 0 };
TypeInfo * __type_info__e1bc44177ac7563a_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c5915ffba474f7fe };
const char * __type_info__e1bc44177ac7563a_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_95 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e1bc44177ac7563a_arg_types_var_11127340054026167412, __type_info__e1bc44177ac7563a_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const ))>::size, UINT64_C(0xe1bc44177ac7563a), "preVisitExprOp2", offsetof(lint::LintVisitor,preVisitExprOp2), 0 };
TypeInfo * __type_info__93cbd195dc2b355e_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c5915ffba474f7fe };
const char * __type_info__93cbd195dc2b355e_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_96 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__93cbd195dc2b355e_arg_types_var_11127340054026167412, __type_info__93cbd195dc2b355e_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp2> const ))>::size, UINT64_C(0x93cbd195dc2b355e), "visitExprOp2", offsetof(lint::LintVisitor,visitExprOp2), 0 };
TypeInfo * __type_info__83ad1aadc60656d3_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__c5915ffba474f7fe, &__type_info__98064c57b4bcca5a };
const char * __type_info__83ad1aadc60656d3_arg_names_var_11127340054026167412[3] = { "self", "expr", "right" };
VarInfo __struct_info__9a6c40d2ef34a474_field_97 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__83ad1aadc60656d3_arg_types_var_11127340054026167412, __type_info__83ad1aadc60656d3_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp2> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x83ad1aadc60656d3), "preVisitExprOp2Right", offsetof(lint::LintVisitor,preVisitExprOp2Right), 0 };
TypeInfo * __type_info__e1bc45177ac757ed_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__ca2136fbaac99425 };
const char * __type_info__e1bc45177ac757ed_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_98 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e1bc45177ac757ed_arg_types_var_11127340054026167412, __type_info__e1bc45177ac757ed_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ))>::size, UINT64_C(0xe1bc45177ac757ed), "preVisitExprOp3", offsetof(lint::LintVisitor,preVisitExprOp3), 0 };
TypeInfo * __type_info__93ccd195dc2ce85e_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__ca2136fbaac99425 };
const char * __type_info__93ccd195dc2ce85e_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_99 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__93ccd195dc2ce85e_arg_types_var_11127340054026167412, __type_info__93ccd195dc2ce85e_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ))>::size, UINT64_C(0x93ccd195dc2ce85e), "visitExprOp3", offsetof(lint::LintVisitor,visitExprOp3), 0 };
TypeInfo * __type_info__f69a5d2f9d815984_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__ca2136fbaac99425, &__type_info__98064c57b4bcca5a };
const char * __type_info__f69a5d2f9d815984_arg_names_var_11127340054026167412[3] = { "self", "expr", "left" };
VarInfo __struct_info__9a6c40d2ef34a474_field_100 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f69a5d2f9d815984_arg_types_var_11127340054026167412, __type_info__f69a5d2f9d815984_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xf69a5d2f9d815984), "preVisitExprOp3Left", offsetof(lint::LintVisitor,preVisitExprOp3Left), 0 };
TypeInfo * __type_info__849049adc4f60f2c_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__ca2136fbaac99425, &__type_info__98064c57b4bcca5a };
const char * __type_info__849049adc4f60f2c_arg_names_var_11127340054026167412[3] = { "self", "expr", "right" };
VarInfo __struct_info__9a6c40d2ef34a474_field_101 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__849049adc4f60f2c_arg_types_var_11127340054026167412, __type_info__849049adc4f60f2c_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x849049adc4f60f2c), "preVisitExprOp3Right", offsetof(lint::LintVisitor,preVisitExprOp3Right), 0 };
TypeInfo * __type_info__1e5bec5fc1899531_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__350b375c34e0c48a };
const char * __type_info__1e5bec5fc1899531_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_102 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__1e5bec5fc1899531_arg_types_var_11127340054026167412, __type_info__1e5bec5fc1899531_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ))>::size, UINT64_C(0x1e5bec5fc1899531), "isRightFirstExprCopy", offsetof(lint::LintVisitor,isRightFirstExprCopy), 0 };
TypeInfo * __type_info__3de2fa17c95c3dd8_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__350b375c34e0c48a };
const char * __type_info__3de2fa17c95c3dd8_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_103 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3de2fa17c95c3dd8_arg_types_var_11127340054026167412, __type_info__3de2fa17c95c3dd8_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ))>::size, UINT64_C(0x3de2fa17c95c3dd8), "preVisitExprCopy", offsetof(lint::LintVisitor,preVisitExprCopy), 0 };
TypeInfo * __type_info__5a9048b914259a82_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__350b375c34e0c48a };
const char * __type_info__5a9048b914259a82_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_104 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__5a9048b914259a82_arg_types_var_11127340054026167412, __type_info__5a9048b914259a82_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ))>::size, UINT64_C(0x5a9048b914259a82), "visitExprCopy", offsetof(lint::LintVisitor,visitExprCopy), 0 };
TypeInfo * __type_info__97ad6f4fff6d757d_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__350b375c34e0c48a, &__type_info__98064c57b4bcca5a };
const char * __type_info__97ad6f4fff6d757d_arg_names_var_11127340054026167412[3] = { "self", "expr", "right" };
VarInfo __struct_info__9a6c40d2ef34a474_field_105 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__97ad6f4fff6d757d_arg_types_var_11127340054026167412, __type_info__97ad6f4fff6d757d_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCopy> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x97ad6f4fff6d757d), "preVisitExprCopyRight", offsetof(lint::LintVisitor,preVisitExprCopyRight), 0 };
TypeInfo * __type_info__1e47de5fc16d47b9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c19751d6d5da74e2 };
const char * __type_info__1e47de5fc16d47b9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_106 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__1e47de5fc16d47b9_arg_types_var_11127340054026167412, __type_info__1e47de5fc16d47b9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMove> const ))>::size, UINT64_C(0x1e47de5fc16d47b9), "isRightFirstExprMove", offsetof(lint::LintVisitor,isRightFirstExprMove), 0 };
TypeInfo * __type_info__3de1f017c960402c_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c19751d6d5da74e2 };
const char * __type_info__3de1f017c960402c_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_107 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3de1f017c960402c_arg_types_var_11127340054026167412, __type_info__3de1f017c960402c_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const ))>::size, UINT64_C(0x3de1f017c960402c), "preVisitExprMove", offsetof(lint::LintVisitor,preVisitExprMove), 0 };
TypeInfo * __type_info__86763caed6701a1e_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c19751d6d5da74e2 };
const char * __type_info__86763caed6701a1e_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_108 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__86763caed6701a1e_arg_types_var_11127340054026167412, __type_info__86763caed6701a1e_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMove> const ))>::size, UINT64_C(0x86763caed6701a1e), "visitExprMove", offsetof(lint::LintVisitor,visitExprMove), 0 };
TypeInfo * __type_info__719cd81d827ead49_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__c19751d6d5da74e2, &__type_info__98064c57b4bcca5a };
const char * __type_info__719cd81d827ead49_arg_names_var_11127340054026167412[3] = { "self", "expr", "right" };
VarInfo __struct_info__9a6c40d2ef34a474_field_109 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__719cd81d827ead49_arg_types_var_11127340054026167412, __type_info__719cd81d827ead49_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMove> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x719cd81d827ead49), "preVisitExprMoveRight", offsetof(lint::LintVisitor,preVisitExprMoveRight), 0 };
TypeInfo * __type_info__af4f0ab9d53a367b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__93546827b32c5422 };
const char * __type_info__af4f0ab9d53a367b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_110 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__af4f0ab9d53a367b_arg_types_var_11127340054026167412, __type_info__af4f0ab9d53a367b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprClone> const ))>::size, UINT64_C(0xaf4f0ab9d53a367b), "isRightFirstExprClone", offsetof(lint::LintVisitor,isRightFirstExprClone), 0 };
TypeInfo * __type_info__85cca36ea6fbcc90_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__93546827b32c5422 };
const char * __type_info__85cca36ea6fbcc90_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_111 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__85cca36ea6fbcc90_arg_types_var_11127340054026167412, __type_info__85cca36ea6fbcc90_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const ))>::size, UINT64_C(0x85cca36ea6fbcc90), "preVisitExprClone", offsetof(lint::LintVisitor,preVisitExprClone), 0 };
TypeInfo * __type_info__a85a31b955e81f44_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__93546827b32c5422 };
const char * __type_info__a85a31b955e81f44_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_112 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__a85a31b955e81f44_arg_types_var_11127340054026167412, __type_info__a85a31b955e81f44_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprClone> const ))>::size, UINT64_C(0xa85a31b955e81f44), "visitExprClone", offsetof(lint::LintVisitor,visitExprClone), 0 };
TypeInfo * __type_info__9c0927b69bc07969_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__93546827b32c5422, &__type_info__98064c57b4bcca5a };
const char * __type_info__9c0927b69bc07969_arg_names_var_11127340054026167412[3] = { "self", "expr", "right" };
VarInfo __struct_info__9a6c40d2ef34a474_field_113 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9c0927b69bc07969_arg_types_var_11127340054026167412, __type_info__9c0927b69bc07969_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprClone> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x9c0927b69bc07969), "preVisitExprCloneRight", offsetof(lint::LintVisitor,preVisitExprCloneRight), 0 };
TypeInfo * __type_info__3cc519cae3bd693d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__83c768ad9b3f81ea };
const char * __type_info__3cc519cae3bd693d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_114 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__3cc519cae3bd693d_arg_types_var_11127340054026167412, __type_info__3cc519cae3bd693d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprAssume> const ))>::size, UINT64_C(0x3cc519cae3bd693d), "canVisitWithAliasSubexpression", offsetof(lint::LintVisitor,canVisitWithAliasSubexpression), 0 };
TypeInfo * __type_info__513500e20b7c16ee_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__83c768ad9b3f81ea };
const char * __type_info__513500e20b7c16ee_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_115 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__513500e20b7c16ee_arg_types_var_11127340054026167412, __type_info__513500e20b7c16ee_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssume> const ))>::size, UINT64_C(0x513500e20b7c16ee), "preVisitExprAssume", offsetof(lint::LintVisitor,preVisitExprAssume), 0 };
TypeInfo * __type_info__e3aecf5f0583216d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__83c768ad9b3f81ea };
const char * __type_info__e3aecf5f0583216d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_116 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__e3aecf5f0583216d_arg_types_var_11127340054026167412, __type_info__e3aecf5f0583216d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssume> const ))>::size, UINT64_C(0xe3aecf5f0583216d), "visitExprAssume", offsetof(lint::LintVisitor,visitExprAssume), 0 };
TypeInfo * __type_info__36ebee17c3130740_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c758d466d1a06ae2 };
const char * __type_info__36ebee17c3130740_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_117 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__36ebee17c3130740_arg_types_var_11127340054026167412, __type_info__36ebee17c3130740_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const ))>::size, UINT64_C(0x36ebee17c3130740), "preVisitExprWith", offsetof(lint::LintVisitor,preVisitExprWith), 0 };
TypeInfo * __type_info__a5bd2e1b3f914df3_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c758d466d1a06ae2 };
const char * __type_info__a5bd2e1b3f914df3_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_118 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__a5bd2e1b3f914df3_arg_types_var_11127340054026167412, __type_info__a5bd2e1b3f914df3_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWith> const ))>::size, UINT64_C(0xa5bd2e1b3f914df3), "visitExprWith", offsetof(lint::LintVisitor,visitExprWith), 0 };
TypeInfo * __type_info__cf9521ef4364a966_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__c758d466d1a06ae2, &__type_info__98064c57b4bcca5a };
const char * __type_info__cf9521ef4364a966_arg_names_var_11127340054026167412[3] = { "self", "expr", "right" };
VarInfo __struct_info__9a6c40d2ef34a474_field_119 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__cf9521ef4364a966_arg_types_var_11127340054026167412, __type_info__cf9521ef4364a966_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWith> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xcf9521ef4364a966), "preVisitExprWithBody", offsetof(lint::LintVisitor,preVisitExprWithBody), 0 };
TypeInfo * __type_info__a3ecbb5c5601e106_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9cac32b4050a2fb8 };
const char * __type_info__a3ecbb5c5601e106_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_120 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a3ecbb5c5601e106_arg_types_var_11127340054026167412, __type_info__a3ecbb5c5601e106_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const ))>::size, UINT64_C(0xa3ecbb5c5601e106), "preVisitExprWhile", offsetof(lint::LintVisitor,preVisitExprWhile), 0 };
TypeInfo * __type_info__ed72541b7ccaa3ae_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9cac32b4050a2fb8 };
const char * __type_info__ed72541b7ccaa3ae_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_121 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ed72541b7ccaa3ae_arg_types_var_11127340054026167412, __type_info__ed72541b7ccaa3ae_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprWhile> const ))>::size, UINT64_C(0xed72541b7ccaa3ae), "visitExprWhile", offsetof(lint::LintVisitor,visitExprWhile), 0 };
TypeInfo * __type_info__8bf20c41cb9dcb96_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__9cac32b4050a2fb8, &__type_info__98064c57b4bcca5a };
const char * __type_info__8bf20c41cb9dcb96_arg_names_var_11127340054026167412[3] = { "self", "expr", "right" };
VarInfo __struct_info__9a6c40d2ef34a474_field_122 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8bf20c41cb9dcb96_arg_types_var_11127340054026167412, __type_info__8bf20c41cb9dcb96_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprWhile> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x8bf20c41cb9dcb96), "preVisitExprWhileBody", offsetof(lint::LintVisitor,preVisitExprWhileBody), 0 };
TypeInfo * __type_info__acd5581d0da14f8c_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c1c6f9bc0741f232 };
const char * __type_info__acd5581d0da14f8c_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_123 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__acd5581d0da14f8c_arg_types_var_11127340054026167412, __type_info__acd5581d0da14f8c_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const ))>::size, UINT64_C(0xacd5581d0da14f8c), "preVisitExprTryCatch", offsetof(lint::LintVisitor,preVisitExprTryCatch), 0 };
TypeInfo * __type_info__da0852cf1cefef85_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c1c6f9bc0741f232 };
const char * __type_info__da0852cf1cefef85_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_124 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__da0852cf1cefef85_arg_types_var_11127340054026167412, __type_info__da0852cf1cefef85_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const ))>::size, UINT64_C(0xda0852cf1cefef85), "visitExprTryCatch", offsetof(lint::LintVisitor,visitExprTryCatch), 0 };
TypeInfo * __type_info__3050151b981a6ab9_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__c1c6f9bc0741f232, &__type_info__98064c57b4bcca5a };
const char * __type_info__3050151b981a6ab9_arg_names_var_11127340054026167412[3] = { "self", "expr", "right" };
VarInfo __struct_info__9a6c40d2ef34a474_field_125 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3050151b981a6ab9_arg_types_var_11127340054026167412, __type_info__3050151b981a6ab9_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTryCatch> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x3050151b981a6ab9), "preVisitExprTryCatchCatch", offsetof(lint::LintVisitor,preVisitExprTryCatchCatch), 0 };
TypeInfo * __type_info__ded7c99ebbf651b5_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__88db72c3eb8c93b1 };
const char * __type_info__ded7c99ebbf651b5_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_126 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ded7c99ebbf651b5_arg_types_var_11127340054026167412, __type_info__ded7c99ebbf651b5_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ))>::size, UINT64_C(0xded7c99ebbf651b5), "preVisitExprIfThenElse", offsetof(lint::LintVisitor,preVisitExprIfThenElse), 0 };
TypeInfo * __type_info__531fb2668c589df9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__88db72c3eb8c93b1 };
const char * __type_info__531fb2668c589df9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_127 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__531fb2668c589df9_arg_types_var_11127340054026167412, __type_info__531fb2668c589df9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ))>::size, UINT64_C(0x531fb2668c589df9), "visitExprIfThenElse", offsetof(lint::LintVisitor,visitExprIfThenElse), 0 };
TypeInfo * __type_info__d901f373b80ee7d4_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__88db72c3eb8c93b1, &__type_info__98064c57b4bcca5a };
const char * __type_info__d901f373b80ee7d4_arg_names_var_11127340054026167412[3] = { "self", "expr", "ifBlock" };
VarInfo __struct_info__9a6c40d2ef34a474_field_128 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d901f373b80ee7d4_arg_types_var_11127340054026167412, __type_info__d901f373b80ee7d4_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xd901f373b80ee7d4), "preVisitExprIfThenElseIfBlock", offsetof(lint::LintVisitor,preVisitExprIfThenElseIfBlock), 0 };
TypeInfo * __type_info__8f8258cd44bd5ff3_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__88db72c3eb8c93b1, &__type_info__98064c57b4bcca5a };
const char * __type_info__8f8258cd44bd5ff3_arg_names_var_11127340054026167412[3] = { "self", "expr", "elseBlock" };
VarInfo __struct_info__9a6c40d2ef34a474_field_129 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8f8258cd44bd5ff3_arg_types_var_11127340054026167412, __type_info__8f8258cd44bd5ff3_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x8f8258cd44bd5ff3), "preVisitExprIfThenElseElseBlock", offsetof(lint::LintVisitor,preVisitExprIfThenElseElseBlock), 0 };
TypeInfo * __type_info__3d9cf617c8d6d53f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87 };
const char * __type_info__3d9cf617c8d6d53f_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_130 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3d9cf617c8d6d53f_arg_types_var_11127340054026167412, __type_info__3d9cf617c8d6d53f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ))>::size, UINT64_C(0x3d9cf617c8d6d53f), "preVisitExprFor", offsetof(lint::LintVisitor,preVisitExprFor), 0 };
TypeInfo * __type_info__ab95dc95f061670f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87 };
const char * __type_info__ab95dc95f061670f_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_131 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ab95dc95f061670f_arg_types_var_11127340054026167412, __type_info__ab95dc95f061670f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ))>::size, UINT64_C(0xab95dc95f061670f), "visitExprFor", offsetof(lint::LintVisitor,visitExprFor), 0 };
TypeInfo * __type_info__3f94f19fe8e70c5c_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__3f94f19fe8e70c5c_arg_names_var_11127340054026167412[4] = { "self", "expr", "svar", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_132 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3f94f19fe8e70c5c_arg_types_var_11127340054026167412, __type_info__3f94f19fe8e70c5c_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x3f94f19fe8e70c5c), "preVisitExprForVariable", offsetof(lint::LintVisitor,preVisitExprForVariable), 0 };
TypeInfo * __type_info__651f4fab660392b6_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__651f4fab660392b6_arg_names_var_11127340054026167412[4] = { "self", "expr", "svar", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_133 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__4d5fdda373bcfbd1, nullptr, (TypeInfo **)__type_info__651f4fab660392b6_arg_types_var_11127340054026167412, __type_info__651f4fab660392b6_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Variable>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0x651f4fab660392b6), "visitExprForVariable", offsetof(lint::LintVisitor,visitExprForVariable), 0 };
TypeInfo * __type_info__ae2c7c525b576599_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__ae2c7c525b576599_arg_names_var_11127340054026167412[4] = { "self", "expr", "source", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_134 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ae2c7c525b576599_arg_types_var_11127340054026167412, __type_info__ae2c7c525b576599_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xae2c7c525b576599), "preVisitExprForSource", offsetof(lint::LintVisitor,preVisitExprForSource), 0 };
TypeInfo * __type_info__f168c75805c4fb50_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__f168c75805c4fb50_arg_names_var_11127340054026167412[4] = { "self", "expr", "source", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_135 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f168c75805c4fb50_arg_types_var_11127340054026167412, __type_info__f168c75805c4fb50_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xf168c75805c4fb50), "visitExprForSource", offsetof(lint::LintVisitor,visitExprForSource), 0 };
TypeInfo * __type_info__9b48972a32c92396_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87 };
const char * __type_info__9b48972a32c92396_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_136 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9b48972a32c92396_arg_types_var_11127340054026167412, __type_info__9b48972a32c92396_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ))>::size, UINT64_C(0x9b48972a32c92396), "preVisitExprForStack", offsetof(lint::LintVisitor,preVisitExprForStack), 0 };
TypeInfo * __type_info__8a858cd61a32c4cb_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9745884abdafbe87 };
const char * __type_info__8a858cd61a32c4cb_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_137 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8a858cd61a32c4cb_arg_types_var_11127340054026167412, __type_info__8a858cd61a32c4cb_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ))>::size, UINT64_C(0x8a858cd61a32c4cb), "preVisitExprForBody", offsetof(lint::LintVisitor,preVisitExprForBody), 0 };
TypeInfo * __type_info__d9a5db0dc73fa5ea_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4191dbf23146a87e };
const char * __type_info__d9a5db0dc73fa5ea_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_138 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d9a5db0dc73fa5ea_arg_types_var_11127340054026167412, __type_info__d9a5db0dc73fa5ea_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ))>::size, UINT64_C(0xd9a5db0dc73fa5ea), "preVisitExprMakeVariant", offsetof(lint::LintVisitor,preVisitExprMakeVariant), 0 };
TypeInfo * __type_info__291e70e95a4218dc_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4191dbf23146a87e };
const char * __type_info__291e70e95a4218dc_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_139 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__291e70e95a4218dc_arg_types_var_11127340054026167412, __type_info__291e70e95a4218dc_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ))>::size, UINT64_C(0x291e70e95a4218dc), "visitExprMakeVariant", offsetof(lint::LintVisitor,visitExprMakeVariant), 0 };
TypeInfo * __type_info__bf2468461ba4704f_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__4191dbf23146a87e, &__type_info__af8afe4c86446b52, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__bf2468461ba4704f_arg_names_var_11127340054026167412[5] = { "self", "expr", "index", "decl", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_140 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bf2468461ba4704f_arg_types_var_11127340054026167412, __type_info__bf2468461ba4704f_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0xbf2468461ba4704f), "preVisitExprMakeVariantField", offsetof(lint::LintVisitor,preVisitExprMakeVariantField), 0 };
TypeInfo * __type_info__279f9f292b04892f_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__4191dbf23146a87e, &__type_info__af8afe4c86446b52, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__279f9f292b04892f_arg_names_var_11127340054026167412[5] = { "self", "expr", "index", "decl", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_141 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__c52835f1e7c9ab84, nullptr, (TypeInfo **)__type_info__279f9f292b04892f_arg_types_var_11127340054026167412, __type_info__279f9f292b04892f_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0x279f9f292b04892f), "visitExprMakeVariantField", offsetof(lint::LintVisitor,visitExprMakeVariantField), 0 };
TypeInfo * __type_info__9114fb7f9b71cef7_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb };
const char * __type_info__9114fb7f9b71cef7_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_142 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__9114fb7f9b71cef7_arg_types_var_11127340054026167412, __type_info__9114fb7f9b71cef7_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ))>::size, UINT64_C(0x9114fb7f9b71cef7), "canVisitExprMakeStructBody", offsetof(lint::LintVisitor,canVisitExprMakeStructBody), 0 };
TypeInfo * __type_info__ba3dedc70c041d9b_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__98064c57b4bcca5a };
const char * __type_info__ba3dedc70c041d9b_arg_names_var_11127340054026167412[3] = { "self", "expr", "blk" };
VarInfo __struct_info__9a6c40d2ef34a474_field_143 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__ba3dedc70c041d9b_arg_types_var_11127340054026167412, __type_info__ba3dedc70c041d9b_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xba3dedc70c041d9b), "canVisitExprMakeStructBlock", offsetof(lint::LintVisitor,canVisitExprMakeStructBlock), 0 };
TypeInfo * __type_info__802af21a577dc767_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb };
const char * __type_info__802af21a577dc767_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_144 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__802af21a577dc767_arg_types_var_11127340054026167412, __type_info__802af21a577dc767_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ))>::size, UINT64_C(0x802af21a577dc767), "preVisitExprMakeStruct", offsetof(lint::LintVisitor,preVisitExprMakeStruct), 0 };
TypeInfo * __type_info__772cf63b3763b9c9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb };
const char * __type_info__772cf63b3763b9c9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_145 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__772cf63b3763b9c9_arg_types_var_11127340054026167412, __type_info__772cf63b3763b9c9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ))>::size, UINT64_C(0x772cf63b3763b9c9), "visitExprMakeStruct", offsetof(lint::LintVisitor,visitExprMakeStruct), 0 };
TypeInfo * __type_info__5c1c9d6e73306336_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__af8afe4c86446b52, &__type_info__af81fe4c86352052 };
const char * __type_info__5c1c9d6e73306336_arg_names_var_11127340054026167412[4] = { "self", "expr", "index", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_146 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5c1c9d6e73306336_arg_types_var_11127340054026167412, __type_info__5c1c9d6e73306336_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool))>::size, UINT64_C(0x5c1c9d6e73306336), "preVisitExprMakeStructIndex", offsetof(lint::LintVisitor,preVisitExprMakeStructIndex), 0 };
TypeInfo * __type_info__e5b0c4527f7128e0_arg_types_var_11127340054026167412[4] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__af8afe4c86446b52, &__type_info__af81fe4c86352052 };
const char * __type_info__e5b0c4527f7128e0_arg_names_var_11127340054026167412[4] = { "self", "expr", "index", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_147 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e5b0c4527f7128e0_arg_types_var_11127340054026167412, __type_info__e5b0c4527f7128e0_arg_names_var_11127340054026167412, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool))>::size, UINT64_C(0xe5b0c4527f7128e0), "visitExprMakeStructIndex", offsetof(lint::LintVisitor,visitExprMakeStructIndex), 0 };
TypeInfo * __type_info__5a547486d6141cd2_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__af8afe4c86446b52, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__5a547486d6141cd2_arg_names_var_11127340054026167412[5] = { "self", "expr", "index", "decl", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_148 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5a547486d6141cd2_arg_types_var_11127340054026167412, __type_info__5a547486d6141cd2_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0x5a547486d6141cd2), "preVisitExprMakeStructField", offsetof(lint::LintVisitor,preVisitExprMakeStructField), 0 };
TypeInfo * __type_info__db4dcb4495d6d4e4_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__af8afe4c86446b52, &__type_info__9a5e492166d49949, &__type_info__af81fe4c86352052 };
const char * __type_info__db4dcb4495d6d4e4_arg_names_var_11127340054026167412[5] = { "self", "expr", "index", "decl", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_149 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__c52835f1e7c9ab84, nullptr, (TypeInfo **)__type_info__db4dcb4495d6d4e4_arg_types_var_11127340054026167412, __type_info__db4dcb4495d6d4e4_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<MakeFieldDecl>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool))>::size, UINT64_C(0xdb4dcb4495d6d4e4), "visitExprMakeStructField", offsetof(lint::LintVisitor,visitExprMakeStructField), 0 };
TypeInfo * __type_info__d98c9c486eb62e7b_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__2dd484863625d80 };
const char * __type_info__d98c9c486eb62e7b_arg_names_var_11127340054026167412[3] = { "self", "expr", "blk" };
VarInfo __struct_info__9a6c40d2ef34a474_field_150 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d98c9c486eb62e7b_arg_types_var_11127340054026167412, __type_info__d98c9c486eb62e7b_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xd98c9c486eb62e7b), "preVisitMakeStructureBlock", offsetof(lint::LintVisitor,preVisitMakeStructureBlock), 0 };
TypeInfo * __type_info__fe693f20b8ceb469_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__5276a743108434eb, &__type_info__2dd484863625d80 };
const char * __type_info__fe693f20b8ceb469_arg_names_var_11127340054026167412[3] = { "self", "expr", "blk" };
VarInfo __struct_info__9a6c40d2ef34a474_field_151 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__fe693f20b8ceb469_arg_types_var_11127340054026167412, __type_info__fe693f20b8ceb469_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xfe693f20b8ceb469), "visitMakeStructureBlock", offsetof(lint::LintVisitor,visitMakeStructureBlock), 0 };
TypeInfo * __type_info__ecd0ba0f4b18b3f7_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f44650fbe99befd9 };
const char * __type_info__ecd0ba0f4b18b3f7_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_152 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ecd0ba0f4b18b3f7_arg_types_var_11127340054026167412, __type_info__ecd0ba0f4b18b3f7_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ))>::size, UINT64_C(0xecd0ba0f4b18b3f7), "preVisitExprMakeArray", offsetof(lint::LintVisitor,preVisitExprMakeArray), 0 };
TypeInfo * __type_info__7d442e914c89b521_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f44650fbe99befd9 };
const char * __type_info__7d442e914c89b521_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_153 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__7d442e914c89b521_arg_types_var_11127340054026167412, __type_info__7d442e914c89b521_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ))>::size, UINT64_C(0x7d442e914c89b521), "visitExprMakeArray", offsetof(lint::LintVisitor,visitExprMakeArray), 0 };
TypeInfo * __type_info__5e248c54071cc77a_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__f44650fbe99befd9, &__type_info__af8afe4c86446b52, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__5e248c54071cc77a_arg_names_var_11127340054026167412[5] = { "self", "expr", "index", "init", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_154 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5e248c54071cc77a_arg_types_var_11127340054026167412, __type_info__5e248c54071cc77a_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x5e248c54071cc77a), "preVisitExprMakeArrayIndex", offsetof(lint::LintVisitor,preVisitExprMakeArrayIndex), 0 };
TypeInfo * __type_info__8f0ca4d12f4ac744_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__f44650fbe99befd9, &__type_info__af8afe4c86446b52, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__8f0ca4d12f4ac744_arg_names_var_11127340054026167412[5] = { "self", "expr", "index", "init", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_155 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__8f0ca4d12f4ac744_arg_types_var_11127340054026167412, __type_info__8f0ca4d12f4ac744_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0x8f0ca4d12f4ac744), "visitExprMakeArrayIndex", offsetof(lint::LintVisitor,visitExprMakeArrayIndex), 0 };
TypeInfo * __type_info__659cff134b375236_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__ea03eef331aabf4 };
const char * __type_info__659cff134b375236_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_156 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__659cff134b375236_arg_types_var_11127340054026167412, __type_info__659cff134b375236_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ))>::size, UINT64_C(0x659cff134b375236), "preVisitExprMakeTuple", offsetof(lint::LintVisitor,preVisitExprMakeTuple), 0 };
TypeInfo * __type_info__24b82bf744c33bd5_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__ea03eef331aabf4 };
const char * __type_info__24b82bf744c33bd5_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_157 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__24b82bf744c33bd5_arg_types_var_11127340054026167412, __type_info__24b82bf744c33bd5_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ))>::size, UINT64_C(0x24b82bf744c33bd5), "visitExprMakeTuple", offsetof(lint::LintVisitor,visitExprMakeTuple), 0 };
TypeInfo * __type_info__ae4dffd12fb114b7_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__ea03eef331aabf4, &__type_info__af8afe4c86446b52, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__ae4dffd12fb114b7_arg_names_var_11127340054026167412[5] = { "self", "expr", "index", "init", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_158 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ae4dffd12fb114b7_arg_types_var_11127340054026167412, __type_info__ae4dffd12fb114b7_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xae4dffd12fb114b7), "preVisitExprMakeTupleIndex", offsetof(lint::LintVisitor,preVisitExprMakeTupleIndex), 0 };
TypeInfo * __type_info__d5eb41fc6778c2f8_arg_types_var_11127340054026167412[5] = { &__type_info__21586ce84f433a21, &__type_info__ea03eef331aabf4, &__type_info__af8afe4c86446b52, &__type_info__98064c57b4bcca5a, &__type_info__af81fe4c86352052 };
const char * __type_info__d5eb41fc6778c2f8_arg_names_var_11127340054026167412[5] = { "self", "expr", "index", "init", "last" };
VarInfo __struct_info__9a6c40d2ef34a474_field_159 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__d5eb41fc6778c2f8_arg_types_var_11127340054026167412, __type_info__d5eb41fc6778c2f8_arg_names_var_11127340054026167412, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool))>::size, UINT64_C(0xd5eb41fc6778c2f8), "visitExprMakeTupleIndex", offsetof(lint::LintVisitor,visitExprMakeTupleIndex), 0 };
TypeInfo * __type_info__d4a5339397eac5e0_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__defdb920e82da0f4 };
const char * __type_info__d4a5339397eac5e0_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_160 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d4a5339397eac5e0_arg_types_var_11127340054026167412, __type_info__d4a5339397eac5e0_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ))>::size, UINT64_C(0xd4a5339397eac5e0), "preVisitExprArrayComprehension", offsetof(lint::LintVisitor,preVisitExprArrayComprehension), 0 };
TypeInfo * __type_info__36db993ec7ea7a_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__defdb920e82da0f4 };
const char * __type_info__36db993ec7ea7a_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_161 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__36db993ec7ea7a_arg_types_var_11127340054026167412, __type_info__36db993ec7ea7a_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ))>::size, UINT64_C(0x36db993ec7ea7a), "visitExprArrayComprehension", offsetof(lint::LintVisitor,visitExprArrayComprehension), 0 };
TypeInfo * __type_info__d1ed822fbff03bdb_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__defdb920e82da0f4, &__type_info__98064c57b4bcca5a };
const char * __type_info__d1ed822fbff03bdb_arg_names_var_11127340054026167412[3] = { "self", "expr", "subexrp" };
VarInfo __struct_info__9a6c40d2ef34a474_field_162 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d1ed822fbff03bdb_arg_types_var_11127340054026167412, __type_info__d1ed822fbff03bdb_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xd1ed822fbff03bdb), "preVisitExprArrayComprehensionSubexpr", offsetof(lint::LintVisitor,preVisitExprArrayComprehensionSubexpr), 0 };
TypeInfo * __type_info__9bee77cb7d793cef_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__defdb920e82da0f4, &__type_info__98064c57b4bcca5a };
const char * __type_info__9bee77cb7d793cef_arg_names_var_11127340054026167412[3] = { "self", "expr", "filter" };
VarInfo __struct_info__9a6c40d2ef34a474_field_163 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9bee77cb7d793cef_arg_types_var_11127340054026167412, __type_info__9bee77cb7d793cef_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x9bee77cb7d793cef), "preVisitExprArrayComprehensionWhere", offsetof(lint::LintVisitor,preVisitExprArrayComprehensionWhere), 0 };
TypeInfo * __type_info__be77a21cf6c882be_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__563543a880fdcea2, &__type_info__98064c57b4bcca5a };
const char * __type_info__be77a21cf6c882be_arg_names_var_11127340054026167412[3] = { "self", "expr", "expr_" };
VarInfo __struct_info__9a6c40d2ef34a474_field_164 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__be77a21cf6c882be_arg_types_var_11127340054026167412, __type_info__be77a21cf6c882be_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0xbe77a21cf6c882be), "canVisitExprTypeInfo", offsetof(lint::LintVisitor,canVisitExprTypeInfo), 0 };
TypeInfo * __type_info__6c2c31cfa17d76e4_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__563543a880fdcea2 };
const char * __type_info__6c2c31cfa17d76e4_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_165 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6c2c31cfa17d76e4_arg_types_var_11127340054026167412, __type_info__6c2c31cfa17d76e4_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ))>::size, UINT64_C(0x6c2c31cfa17d76e4), "preVisitExprTypeInfo", offsetof(lint::LintVisitor,preVisitExprTypeInfo), 0 };
TypeInfo * __type_info__cc26468d7f49e993_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__563543a880fdcea2 };
const char * __type_info__cc26468d7f49e993_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_166 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__cc26468d7f49e993_arg_types_var_11127340054026167412, __type_info__cc26468d7f49e993_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTypeInfo> const ))>::size, UINT64_C(0xcc26468d7f49e993), "visitExprTypeInfo", offsetof(lint::LintVisitor,visitExprTypeInfo), 0 };
TypeInfo * __type_info__24fe72f324ea6945_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7d9fd489616ae8d };
const char * __type_info__24fe72f324ea6945_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_167 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__24fe72f324ea6945_arg_types_var_11127340054026167412, __type_info__24fe72f324ea6945_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const ))>::size, UINT64_C(0x24fe72f324ea6945), "preVisitExprPtr2Ref", offsetof(lint::LintVisitor,preVisitExprPtr2Ref), 0 };
TypeInfo * __type_info__df88f56654c4aef7_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7d9fd489616ae8d };
const char * __type_info__df88f56654c4aef7_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_168 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__df88f56654c4aef7_arg_types_var_11127340054026167412, __type_info__df88f56654c4aef7_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprPtr2Ref> const ))>::size, UINT64_C(0xdf88f56654c4aef7), "visitExprPtr2Ref", offsetof(lint::LintVisitor,visitExprPtr2Ref), 0 };
TypeInfo * __type_info__e20633a3c3c146b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6ad276912e16c445 };
const char * __type_info__e20633a3c3c146b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_169 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e20633a3c3c146b_arg_types_var_11127340054026167412, __type_info__e20633a3c3c146b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLabel> const ))>::size, UINT64_C(0xe20633a3c3c146b), "preVisitExprLabel", offsetof(lint::LintVisitor,preVisitExprLabel), 0 };
TypeInfo * __type_info__66db28ab3cb9062c_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6ad276912e16c445 };
const char * __type_info__66db28ab3cb9062c_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_170 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__66db28ab3cb9062c_arg_types_var_11127340054026167412, __type_info__66db28ab3cb9062c_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprLabel> const ))>::size, UINT64_C(0x66db28ab3cb9062c), "visitExprLabel", offsetof(lint::LintVisitor,visitExprLabel), 0 };
TypeInfo * __type_info__3decee17c9619ad0_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__41023c185ec41d2 };
const char * __type_info__3decee17c9619ad0_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_171 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3decee17c9619ad0_arg_types_var_11127340054026167412, __type_info__3decee17c9619ad0_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprGoto> const ))>::size, UINT64_C(0x3decee17c9619ad0), "preVisitExprGoto", offsetof(lint::LintVisitor,preVisitExprGoto), 0 };
TypeInfo * __type_info__545432cdd4dc2520_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__41023c185ec41d2 };
const char * __type_info__545432cdd4dc2520_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_172 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__545432cdd4dc2520_arg_types_var_11127340054026167412, __type_info__545432cdd4dc2520_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprGoto> const ))>::size, UINT64_C(0x545432cdd4dc2520), "visitExprGoto", offsetof(lint::LintVisitor,visitExprGoto), 0 };
TypeInfo * __type_info__913260380c2b4728_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__bba83b75d4855b7e };
const char * __type_info__913260380c2b4728_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_173 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__913260380c2b4728_arg_types_var_11127340054026167412, __type_info__913260380c2b4728_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const ))>::size, UINT64_C(0x913260380c2b4728), "preVisitExprRef2Value", offsetof(lint::LintVisitor,preVisitExprRef2Value), 0 };
TypeInfo * __type_info__aa5e7310d89ef683_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__bba83b75d4855b7e };
const char * __type_info__aa5e7310d89ef683_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_174 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__aa5e7310d89ef683_arg_types_var_11127340054026167412, __type_info__aa5e7310d89ef683_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Value> const ))>::size, UINT64_C(0xaa5e7310d89ef683), "visitExprRef2Value", offsetof(lint::LintVisitor,visitExprRef2Value), 0 };
TypeInfo * __type_info__24d0d9d58a135865_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__1151bc4127672205 };
const char * __type_info__24d0d9d58a135865_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_175 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__24d0d9d58a135865_arg_types_var_11127340054026167412, __type_info__24d0d9d58a135865_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const ))>::size, UINT64_C(0x24d0d9d58a135865), "preVisitExprRef2Ptr", offsetof(lint::LintVisitor,preVisitExprRef2Ptr), 0 };
TypeInfo * __type_info__fc85adf5f839389b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__1151bc4127672205 };
const char * __type_info__fc85adf5f839389b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_176 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__fc85adf5f839389b_arg_types_var_11127340054026167412, __type_info__fc85adf5f839389b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprRef2Ptr> const ))>::size, UINT64_C(0xfc85adf5f839389b), "visitExprRef2Ptr", offsetof(lint::LintVisitor,visitExprRef2Ptr), 0 };
TypeInfo * __type_info__25d71217b4b05b4e_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c2f4bc15903e1610 };
const char * __type_info__25d71217b4b05b4e_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_177 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__25d71217b4b05b4e_arg_types_var_11127340054026167412, __type_info__25d71217b4b05b4e_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAddr> const ))>::size, UINT64_C(0x25d71217b4b05b4e), "preVisitExprAddr", offsetof(lint::LintVisitor,preVisitExprAddr), 0 };
TypeInfo * __type_info__cb1c45c3d6c55cf8_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c2f4bc15903e1610 };
const char * __type_info__cb1c45c3d6c55cf8_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_178 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__cb1c45c3d6c55cf8_arg_types_var_11127340054026167412, __type_info__cb1c45c3d6c55cf8_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAddr> const ))>::size, UINT64_C(0xcb1c45c3d6c55cf8), "visitExprAddr", offsetof(lint::LintVisitor,visitExprAddr), 0 };
TypeInfo * __type_info__1b45ebe1de09663f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__d551858bc6d43037 };
const char * __type_info__1b45ebe1de09663f_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_179 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1b45ebe1de09663f_arg_types_var_11127340054026167412, __type_info__1b45ebe1de09663f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAssert> const ))>::size, UINT64_C(0x1b45ebe1de09663f), "preVisitExprAssert", offsetof(lint::LintVisitor,preVisitExprAssert), 0 };
TypeInfo * __type_info__87b6be5eb760221a_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__d551858bc6d43037 };
const char * __type_info__87b6be5eb760221a_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_180 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__87b6be5eb760221a_arg_types_var_11127340054026167412, __type_info__87b6be5eb760221a_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAssert> const ))>::size, UINT64_C(0x87b6be5eb760221a), "visitExprAssert", offsetof(lint::LintVisitor,visitExprAssert), 0 };
TypeInfo * __type_info__1cb076b2ecd4d8b0_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__74372feec5a81686 };
const char * __type_info__1cb076b2ecd4d8b0_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_181 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1cb076b2ecd4d8b0_arg_types_var_11127340054026167412, __type_info__1cb076b2ecd4d8b0_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const ))>::size, UINT64_C(0x1cb076b2ecd4d8b0), "preVisitExprStaticAssert", offsetof(lint::LintVisitor,preVisitExprStaticAssert), 0 };
TypeInfo * __type_info__f62d36f76c2e2809_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__74372feec5a81686 };
const char * __type_info__f62d36f76c2e2809_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_182 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f62d36f76c2e2809_arg_types_var_11127340054026167412, __type_info__f62d36f76c2e2809_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprStaticAssert> const ))>::size, UINT64_C(0xf62d36f76c2e2809), "visitExprStaticAssert", offsetof(lint::LintVisitor,visitExprStaticAssert), 0 };
TypeInfo * __type_info__b71488fdfe239082_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9dfe8a83730428c8 };
const char * __type_info__b71488fdfe239082_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_183 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b71488fdfe239082_arg_types_var_11127340054026167412, __type_info__b71488fdfe239082_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprQuote> const ))>::size, UINT64_C(0xb71488fdfe239082), "preVisitExprQuote", offsetof(lint::LintVisitor,preVisitExprQuote), 0 };
TypeInfo * __type_info__91204a10cb25102b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9dfe8a83730428c8 };
const char * __type_info__91204a10cb25102b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_184 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__91204a10cb25102b_arg_types_var_11127340054026167412, __type_info__91204a10cb25102b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprQuote> const ))>::size, UINT64_C(0x91204a10cb25102b), "visitExprQuote", offsetof(lint::LintVisitor,visitExprQuote), 0 };
TypeInfo * __type_info__2fabb44ba51e2e5e_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c84cf5ded2cd1cd8 };
const char * __type_info__2fabb44ba51e2e5e_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_185 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2fabb44ba51e2e5e_arg_types_var_11127340054026167412, __type_info__2fabb44ba51e2e5e_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDebug> const ))>::size, UINT64_C(0x2fabb44ba51e2e5e), "preVisitExprDebug", offsetof(lint::LintVisitor,preVisitExprDebug), 0 };
TypeInfo * __type_info__da0848d1c542b6e8_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c84cf5ded2cd1cd8 };
const char * __type_info__da0848d1c542b6e8_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_186 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__da0848d1c542b6e8_arg_types_var_11127340054026167412, __type_info__da0848d1c542b6e8_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDebug> const ))>::size, UINT64_C(0xda0848d1c542b6e8), "visitExprDebug", offsetof(lint::LintVisitor,visitExprDebug), 0 };
TypeInfo * __type_info__35fb364bd7b96c1_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c3c8c780df6c5865 };
const char * __type_info__35fb364bd7b96c1_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_187 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__35fb364bd7b96c1_arg_types_var_11127340054026167412, __type_info__35fb364bd7b96c1_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const ))>::size, UINT64_C(0x35fb364bd7b96c1), "preVisitExprInvoke", offsetof(lint::LintVisitor,preVisitExprInvoke), 0 };
TypeInfo * __type_info__de632c57cd0373a4_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c3c8c780df6c5865 };
const char * __type_info__de632c57cd0373a4_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_188 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__de632c57cd0373a4_arg_types_var_11127340054026167412, __type_info__de632c57cd0373a4_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprInvoke> const ))>::size, UINT64_C(0xde632c57cd0373a4), "visitExprInvoke", offsetof(lint::LintVisitor,visitExprInvoke), 0 };
TypeInfo * __type_info__b64d98db7365a3bc_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e672712e93e236ba };
const char * __type_info__b64d98db7365a3bc_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_189 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b64d98db7365a3bc_arg_types_var_11127340054026167412, __type_info__b64d98db7365a3bc_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprErase> const ))>::size, UINT64_C(0xb64d98db7365a3bc), "preVisitExprErase", offsetof(lint::LintVisitor,preVisitExprErase), 0 };
TypeInfo * __type_info__da3046d543b6c331_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e672712e93e236ba };
const char * __type_info__da3046d543b6c331_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_190 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__da3046d543b6c331_arg_types_var_11127340054026167412, __type_info__da3046d543b6c331_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprErase> const ))>::size, UINT64_C(0xda3046d543b6c331), "visitExprErase", offsetof(lint::LintVisitor,visitExprErase), 0 };
TypeInfo * __type_info__c265e47ae7854c9f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9c37565e66334661 };
const char * __type_info__c265e47ae7854c9f_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_191 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c265e47ae7854c9f_arg_types_var_11127340054026167412, __type_info__c265e47ae7854c9f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const ))>::size, UINT64_C(0xc265e47ae7854c9f), "preVisitExprSetInsert", offsetof(lint::LintVisitor,preVisitExprSetInsert), 0 };
TypeInfo * __type_info__abb18b0074bbd10d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9c37565e66334661 };
const char * __type_info__abb18b0074bbd10d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_192 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__abb18b0074bbd10d_arg_types_var_11127340054026167412, __type_info__abb18b0074bbd10d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSetInsert> const ))>::size, UINT64_C(0xabb18b0074bbd10d), "visitExprSetInsert", offsetof(lint::LintVisitor,visitExprSetInsert), 0 };
TypeInfo * __type_info__36f50a17c34dd13b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__ee83d76e6f9a3c81 };
const char * __type_info__36f50a17c34dd13b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_193 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__36f50a17c34dd13b_arg_types_var_11127340054026167412, __type_info__36f50a17c34dd13b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFind> const ))>::size, UINT64_C(0x36f50a17c34dd13b), "preVisitExprFind", offsetof(lint::LintVisitor,preVisitExprFind), 0 };
TypeInfo * __type_info__91de39c724a41957_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__ee83d76e6f9a3c81 };
const char * __type_info__91de39c724a41957_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_194 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__91de39c724a41957_arg_types_var_11127340054026167412, __type_info__91de39c724a41957_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFind> const ))>::size, UINT64_C(0x91de39c724a41957), "visitExprFind", offsetof(lint::LintVisitor,visitExprFind), 0 };
TypeInfo * __type_info__fa613307725ae7f7_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7f9fc2c601e28df1 };
const char * __type_info__fa613307725ae7f7_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_195 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__fa613307725ae7f7_arg_types_var_11127340054026167412, __type_info__fa613307725ae7f7_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const ))>::size, UINT64_C(0xfa613307725ae7f7), "preVisitExprKeyExists", offsetof(lint::LintVisitor,preVisitExprKeyExists), 0 };
TypeInfo * __type_info__4363e653f254a75_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7f9fc2c601e28df1 };
const char * __type_info__4363e653f254a75_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_196 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__4363e653f254a75_arg_types_var_11127340054026167412, __type_info__4363e653f254a75_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprKeyExists> const ))>::size, UINT64_C(0x4363e653f254a75), "visitExprKeyExists", offsetof(lint::LintVisitor,visitExprKeyExists), 0 };
TypeInfo * __type_info__1b53dfe1ddf2e94b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6bdd529063b3dbeb };
const char * __type_info__1b53dfe1ddf2e94b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_197 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1b53dfe1ddf2e94b_arg_types_var_11127340054026167412, __type_info__1b53dfe1ddf2e94b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAscend> const ))>::size, UINT64_C(0x1b53dfe1ddf2e94b), "preVisitExprAscend", offsetof(lint::LintVisitor,preVisitExprAscend), 0 };
TypeInfo * __type_info__cb8eaeac168eb2ea_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6bdd529063b3dbeb };
const char * __type_info__cb8eaeac168eb2ea_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_198 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__cb8eaeac168eb2ea_arg_types_var_11127340054026167412, __type_info__cb8eaeac168eb2ea_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAscend> const ))>::size, UINT64_C(0xcb8eaeac168eb2ea), "visitExprAscend", offsetof(lint::LintVisitor,visitExprAscend), 0 };
TypeInfo * __type_info__1be3fd17ac778ff1_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__bc5b346893db35b };
const char * __type_info__1be3fd17ac778ff1_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_199 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1be3fd17ac778ff1_arg_types_var_11127340054026167412, __type_info__1be3fd17ac778ff1_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCast> const ))>::size, UINT64_C(0x1be3fd17ac778ff1), "preVisitExprCast", offsetof(lint::LintVisitor,preVisitExprCast), 0 };
TypeInfo * __type_info__64d639b91cdfa50f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__bc5b346893db35b };
const char * __type_info__64d639b91cdfa50f_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_200 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__64d639b91cdfa50f_arg_types_var_11127340054026167412, __type_info__64d639b91cdfa50f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCast> const ))>::size, UINT64_C(0x64d639b91cdfa50f), "visitExprCast", offsetof(lint::LintVisitor,visitExprCast), 0 };
TypeInfo * __type_info__7ae6556effbccba2_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__120723ecb6510065, &__type_info__98064c57b4bcca5a };
const char * __type_info__7ae6556effbccba2_arg_names_var_11127340054026167412[3] = { "self", "del", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_201 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7ae6556effbccba2_arg_types_var_11127340054026167412, __type_info__7ae6556effbccba2_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x7ae6556effbccba2), "preVisitExprDeleteSizeExpression", offsetof(lint::LintVisitor,preVisitExprDeleteSizeExpression), 0 };
TypeInfo * __type_info__6637b74bd3c3f3b5_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__120723ecb6510065 };
const char * __type_info__6637b74bd3c3f3b5_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_202 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6637b74bd3c3f3b5_arg_types_var_11127340054026167412, __type_info__6637b74bd3c3f3b5_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ))>::size, UINT64_C(0x6637b74bd3c3f3b5), "preVisitExprDelete", offsetof(lint::LintVisitor,preVisitExprDelete), 0 };
TypeInfo * __type_info__388469b55a519387_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__120723ecb6510065 };
const char * __type_info__388469b55a519387_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_203 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__388469b55a519387_arg_types_var_11127340054026167412, __type_info__388469b55a519387_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprDelete> const ))>::size, UINT64_C(0x388469b55a519387), "visitExprDelete", offsetof(lint::LintVisitor,visitExprDelete), 0 };
TypeInfo * __type_info__1bd6f617ac24caaf_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f5c1d1c41d788f7 };
const char * __type_info__1bd6f617ac24caaf_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_204 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1bd6f617ac24caaf_arg_types_var_11127340054026167412, __type_info__1bd6f617ac24caaf_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprVar> const ))>::size, UINT64_C(0x1bd6f617ac24caaf), "preVisitExprVar", offsetof(lint::LintVisitor,preVisitExprVar), 0 };
TypeInfo * __type_info__e195e2961df0e141_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f5c1d1c41d788f7 };
const char * __type_info__e195e2961df0e141_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_205 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__e195e2961df0e141_arg_types_var_11127340054026167412, __type_info__e195e2961df0e141_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprVar> const ))>::size, UINT64_C(0xe195e2961df0e141), "visitExprVar", offsetof(lint::LintVisitor,visitExprVar), 0 };
TypeInfo * __type_info__1bddf117ac2ae0de_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__631c9e15ba7d5036 };
const char * __type_info__1bddf117ac2ae0de_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_206 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1bddf117ac2ae0de_arg_types_var_11127340054026167412, __type_info__1bddf117ac2ae0de_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const ))>::size, UINT64_C(0x1bddf117ac2ae0de), "preVisitExprTag", offsetof(lint::LintVisitor,preVisitExprTag), 0 };
TypeInfo * __type_info__7e4376fd81e49668_arg_types_var_11127340054026167412[3] = { &__type_info__21586ce84f433a21, &__type_info__631c9e15ba7d5036, &__type_info__98064c57b4bcca5a };
const char * __type_info__7e4376fd81e49668_arg_names_var_11127340054026167412[3] = { "self", "expr", "value" };
VarInfo __struct_info__9a6c40d2ef34a474_field_207 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7e4376fd81e49668_arg_types_var_11127340054026167412, __type_info__7e4376fd81e49668_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprTag> const ,smart_ptr_raw<Expression> const ))>::size, UINT64_C(0x7e4376fd81e49668), "preVisitExprTagValue", offsetof(lint::LintVisitor,preVisitExprTagValue), 0 };
TypeInfo * __type_info__e870e29623d0b041_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__631c9e15ba7d5036 };
const char * __type_info__e870e29623d0b041_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_208 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__e870e29623d0b041_arg_types_var_11127340054026167412, __type_info__e870e29623d0b041_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprTag> const ))>::size, UINT64_C(0xe870e29623d0b041), "visitExprTag", offsetof(lint::LintVisitor,visitExprTag), 0 };
TypeInfo * __type_info__94e97360c60ccbf6_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__849126a4e3db3268 };
const char * __type_info__94e97360c60ccbf6_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_209 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__94e97360c60ccbf6_arg_types_var_11127340054026167412, __type_info__94e97360c60ccbf6_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprField> const ))>::size, UINT64_C(0x94e97360c60ccbf6), "preVisitExprField", offsetof(lint::LintVisitor,preVisitExprField), 0 };
TypeInfo * __type_info__a2b831c732d6acbf_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__849126a4e3db3268 };
const char * __type_info__a2b831c732d6acbf_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_210 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__a2b831c732d6acbf_arg_types_var_11127340054026167412, __type_info__a2b831c732d6acbf_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprField> const ))>::size, UINT64_C(0xa2b831c732d6acbf), "visitExprField", offsetof(lint::LintVisitor,visitExprField), 0 };
TypeInfo * __type_info__7d33e7a7e5224f55_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__a7adf4b0a367d897 };
const char * __type_info__7d33e7a7e5224f55_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_211 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7d33e7a7e5224f55_arg_types_var_11127340054026167412, __type_info__7d33e7a7e5224f55_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const ))>::size, UINT64_C(0x7d33e7a7e5224f55), "preVisitExprSafeField", offsetof(lint::LintVisitor,preVisitExprSafeField), 0 };
TypeInfo * __type_info__d0144d3d83af2c09_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__a7adf4b0a367d897 };
const char * __type_info__d0144d3d83af2c09_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_212 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__d0144d3d83af2c09_arg_types_var_11127340054026167412, __type_info__d0144d3d83af2c09_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeField> const ))>::size, UINT64_C(0xd0144d3d83af2c09), "visitExprSafeField", offsetof(lint::LintVisitor,visitExprSafeField), 0 };
TypeInfo * __type_info__1f22b71f3af1edea_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6bb94e24ea14ce9a };
const char * __type_info__1f22b71f3af1edea_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_213 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1f22b71f3af1edea_arg_types_var_11127340054026167412, __type_info__1f22b71f3af1edea_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const ))>::size, UINT64_C(0x1f22b71f3af1edea), "preVisitExprSwizzle", offsetof(lint::LintVisitor,preVisitExprSwizzle), 0 };
TypeInfo * __type_info__3fdedde6419b6d01_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6bb94e24ea14ce9a };
const char * __type_info__3fdedde6419b6d01_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_214 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__3fdedde6419b6d01_arg_types_var_11127340054026167412, __type_info__3fdedde6419b6d01_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSwizzle> const ))>::size, UINT64_C(0x3fdedde6419b6d01), "visitExprSwizzle", offsetof(lint::LintVisitor,visitExprSwizzle), 0 };
TypeInfo * __type_info__56118e87f8a05c63_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__837624c70f8f1fa1 };
const char * __type_info__56118e87f8a05c63_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_215 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__56118e87f8a05c63_arg_types_var_11127340054026167412, __type_info__56118e87f8a05c63_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const ))>::size, UINT64_C(0x56118e87f8a05c63), "preVisitExprIsVariant", offsetof(lint::LintVisitor,preVisitExprIsVariant), 0 };
TypeInfo * __type_info__75a913e04fde93c1_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__837624c70f8f1fa1 };
const char * __type_info__75a913e04fde93c1_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_216 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__75a913e04fde93c1_arg_types_var_11127340054026167412, __type_info__75a913e04fde93c1_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprIsVariant> const ))>::size, UINT64_C(0x75a913e04fde93c1), "visitExprIsVariant", offsetof(lint::LintVisitor,visitExprIsVariant), 0 };
TypeInfo * __type_info__9af8664b168e269b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__229aabe2f8bef1d9 };
const char * __type_info__9af8664b168e269b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_217 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9af8664b168e269b_arg_types_var_11127340054026167412, __type_info__9af8664b168e269b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const ))>::size, UINT64_C(0x9af8664b168e269b), "preVisitExprAsVariant", offsetof(lint::LintVisitor,preVisitExprAsVariant), 0 };
TypeInfo * __type_info__54415956ccfccbc1_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__229aabe2f8bef1d9 };
const char * __type_info__54415956ccfccbc1_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_218 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__54415956ccfccbc1_arg_types_var_11127340054026167412, __type_info__54415956ccfccbc1_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprAsVariant> const ))>::size, UINT64_C(0x54415956ccfccbc1), "visitExprAsVariant", offsetof(lint::LintVisitor,visitExprAsVariant), 0 };
TypeInfo * __type_info__f75f24c28f1b3458_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__45d77ccae958b9de };
const char * __type_info__f75f24c28f1b3458_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_219 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f75f24c28f1b3458_arg_types_var_11127340054026167412, __type_info__f75f24c28f1b3458_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const ))>::size, UINT64_C(0xf75f24c28f1b3458), "preVisitExprSafeAsVariant", offsetof(lint::LintVisitor,preVisitExprSafeAsVariant), 0 };
TypeInfo * __type_info__c8ae6f2d983e7f9b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__45d77ccae958b9de };
const char * __type_info__c8ae6f2d983e7f9b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_220 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__c8ae6f2d983e7f9b_arg_types_var_11127340054026167412, __type_info__c8ae6f2d983e7f9b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprSafeAsVariant> const ))>::size, UINT64_C(0xc8ae6f2d983e7f9b), "visitExprSafeAsVariant", offsetof(lint::LintVisitor,visitExprSafeAsVariant), 0 };
TypeInfo * __type_info__e1bc43177ac75487_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c7c0e4fba3dcbfcf };
const char * __type_info__e1bc43177ac75487_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_221 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e1bc43177ac75487_arg_types_var_11127340054026167412, __type_info__e1bc43177ac75487_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprOp1> const ))>::size, UINT64_C(0xe1bc43177ac75487), "preVisitExprOp1", offsetof(lint::LintVisitor,preVisitExprOp1), 0 };
TypeInfo * __type_info__93cad195dc29825e_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__c7c0e4fba3dcbfcf };
const char * __type_info__93cad195dc29825e_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_222 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__93cad195dc29825e_arg_types_var_11127340054026167412, __type_info__93cad195dc29825e_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprOp1> const ))>::size, UINT64_C(0x93cad195dc29825e), "visitExprOp1", offsetof(lint::LintVisitor,visitExprOp1), 0 };
TypeInfo * __type_info__8601634e62501b9d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__1a5b7f11cf3fb5b5 };
const char * __type_info__8601634e62501b9d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_223 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8601634e62501b9d_arg_types_var_11127340054026167412, __type_info__8601634e62501b9d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReturn> const ))>::size, UINT64_C(0x8601634e62501b9d), "preVisitExprReturn", offsetof(lint::LintVisitor,preVisitExprReturn), 0 };
TypeInfo * __type_info__51c6959eb7d8ecb2_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__1a5b7f11cf3fb5b5 };
const char * __type_info__51c6959eb7d8ecb2_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_224 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__51c6959eb7d8ecb2_arg_types_var_11127340054026167412, __type_info__51c6959eb7d8ecb2_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReturn> const ))>::size, UINT64_C(0x51c6959eb7d8ecb2), "visitExprReturn", offsetof(lint::LintVisitor,visitExprReturn), 0 };
TypeInfo * __type_info__1d202e605403c2b7_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__60501e84f49c29e1 };
const char * __type_info__1d202e605403c2b7_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_225 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1d202e605403c2b7_arg_types_var_11127340054026167412, __type_info__1d202e605403c2b7_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprYield> const ))>::size, UINT64_C(0x1d202e605403c2b7), "preVisitExprYield", offsetof(lint::LintVisitor,preVisitExprYield), 0 };
TypeInfo * __type_info__40d931ea60f18bbf_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__60501e84f49c29e1 };
const char * __type_info__40d931ea60f18bbf_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_226 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__40d931ea60f18bbf_arg_types_var_11127340054026167412, __type_info__40d931ea60f18bbf_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprYield> const ))>::size, UINT64_C(0x40d931ea60f18bbf), "visitExprYield", offsetof(lint::LintVisitor,visitExprYield), 0 };
TypeInfo * __type_info__c830b1db8edbd193_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__5047b5dbcc2127cd };
const char * __type_info__c830b1db8edbd193_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_227 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c830b1db8edbd193_arg_types_var_11127340054026167412, __type_info__c830b1db8edbd193_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBreak> const ))>::size, UINT64_C(0xc830b1db8edbd193), "preVisitExprBreak", offsetof(lint::LintVisitor,preVisitExprBreak), 0 };
TypeInfo * __type_info__cb0f54b51bca0ffb_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__5047b5dbcc2127cd };
const char * __type_info__cb0f54b51bca0ffb_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_228 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__cb0f54b51bca0ffb_arg_types_var_11127340054026167412, __type_info__cb0f54b51bca0ffb_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBreak> const ))>::size, UINT64_C(0xcb0f54b51bca0ffb), "visitExprBreak", offsetof(lint::LintVisitor,visitExprBreak), 0 };
TypeInfo * __type_info__4e806adf41d6c94c_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__5b91ede0508873e };
const char * __type_info__4e806adf41d6c94c_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_229 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4e806adf41d6c94c_arg_types_var_11127340054026167412, __type_info__4e806adf41d6c94c_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprContinue> const ))>::size, UINT64_C(0x4e806adf41d6c94c), "preVisitExprContinue", offsetof(lint::LintVisitor,preVisitExprContinue), 0 };
TypeInfo * __type_info__99df037537935268_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__5b91ede0508873e };
const char * __type_info__99df037537935268_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_230 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__99df037537935268_arg_types_var_11127340054026167412, __type_info__99df037537935268_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprContinue> const ))>::size, UINT64_C(0x99df037537935268), "visitExprContinue", offsetof(lint::LintVisitor,visitExprContinue), 0 };
TypeInfo * __type_info__6838d74b653b1de4_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e0b574ceb6c8c70f };
const char * __type_info__6838d74b653b1de4_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_231 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__6838d74b653b1de4_arg_types_var_11127340054026167412, __type_info__6838d74b653b1de4_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const ))>::size, UINT64_C(0x6838d74b653b1de4), "canVisitMakeBlockBody", offsetof(lint::LintVisitor,canVisitMakeBlockBody), 0 };
TypeInfo * __type_info__714ea2a2960ed115_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e0b574ceb6c8c70f };
const char * __type_info__714ea2a2960ed115_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_232 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__714ea2a2960ed115_arg_types_var_11127340054026167412, __type_info__714ea2a2960ed115_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const ))>::size, UINT64_C(0x714ea2a2960ed115), "preVisitExprMakeBlock", offsetof(lint::LintVisitor,preVisitExprMakeBlock), 0 };
TypeInfo * __type_info__2428fc8af625caf9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e0b574ceb6c8c70f };
const char * __type_info__2428fc8af625caf9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_233 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__2428fc8af625caf9_arg_types_var_11127340054026167412, __type_info__2428fc8af625caf9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeBlock> const ))>::size, UINT64_C(0x2428fc8af625caf9), "visitExprMakeBlock", offsetof(lint::LintVisitor,visitExprMakeBlock), 0 };
TypeInfo * __type_info__fba56563aec92e36_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6c1a6b092c78a88 };
const char * __type_info__fba56563aec92e36_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_234 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__fba56563aec92e36_arg_types_var_11127340054026167412, __type_info__fba56563aec92e36_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const ))>::size, UINT64_C(0xfba56563aec92e36), "preVisitExprMakeGenerator", offsetof(lint::LintVisitor,preVisitExprMakeGenerator), 0 };
TypeInfo * __type_info__249adeffe7dcc8ba_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6c1a6b092c78a88 };
const char * __type_info__249adeffe7dcc8ba_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_235 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__249adeffe7dcc8ba_arg_types_var_11127340054026167412, __type_info__249adeffe7dcc8ba_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMakeGenerator> const ))>::size, UINT64_C(0x249adeffe7dcc8ba), "visitExprMakeGenerator", offsetof(lint::LintVisitor,visitExprMakeGenerator), 0 };
TypeInfo * __type_info__ef9dcb85e6a70655_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f66cc598ea369f61 };
const char * __type_info__ef9dcb85e6a70655_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_236 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ef9dcb85e6a70655_arg_types_var_11127340054026167412, __type_info__ef9dcb85e6a70655_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const ))>::size, UINT64_C(0xef9dcb85e6a70655), "preVisitExprMemZero", offsetof(lint::LintVisitor,preVisitExprMemZero), 0 };
TypeInfo * __type_info__9c214ae6b6cc6c5_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f66cc598ea369f61 };
const char * __type_info__9c214ae6b6cc6c5_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_237 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9c214ae6b6cc6c5_arg_types_var_11127340054026167412, __type_info__9c214ae6b6cc6c5_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprMemZero> const ))>::size, UINT64_C(0x9c214ae6b6cc6c5), "visitExprMemZero", offsetof(lint::LintVisitor,visitExprMemZero), 0 };
TypeInfo * __type_info__6320966b0b0640a2_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__118bfa23ce6c000c };
const char * __type_info__6320966b0b0640a2_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_238 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6320966b0b0640a2_arg_types_var_11127340054026167412, __type_info__6320966b0b0640a2_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConst> const ))>::size, UINT64_C(0x6320966b0b0640a2), "preVisitExprConst", offsetof(lint::LintVisitor,preVisitExprConst), 0 };
TypeInfo * __type_info__a5484eb95396c6b4_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__118bfa23ce6c000c };
const char * __type_info__a5484eb95396c6b4_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_239 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__a5484eb95396c6b4_arg_types_var_11127340054026167412, __type_info__a5484eb95396c6b4_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConst> const ))>::size, UINT64_C(0xa5484eb95396c6b4), "visitExprConst", offsetof(lint::LintVisitor,visitExprConst), 0 };
TypeInfo * __type_info__66926de2d45531a2_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__a45346c81e6a9b80 };
const char * __type_info__66926de2d45531a2_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_240 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__66926de2d45531a2_arg_types_var_11127340054026167412, __type_info__66926de2d45531a2_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const ))>::size, UINT64_C(0x66926de2d45531a2), "preVisitExprConstPtr", offsetof(lint::LintVisitor,preVisitExprConstPtr), 0 };
TypeInfo * __type_info__57c821fa698a4dfa_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__a45346c81e6a9b80 };
const char * __type_info__57c821fa698a4dfa_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_241 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__57c821fa698a4dfa_arg_types_var_11127340054026167412, __type_info__57c821fa698a4dfa_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstPtr> const ))>::size, UINT64_C(0x57c821fa698a4dfa), "visitExprConstPtr", offsetof(lint::LintVisitor,visitExprConstPtr), 0 };
TypeInfo * __type_info__787a20e2af0b5236_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__b6d18d4b3fadccd4 };
const char * __type_info__787a20e2af0b5236_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_242 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__787a20e2af0b5236_arg_types_var_11127340054026167412, __type_info__787a20e2af0b5236_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const ))>::size, UINT64_C(0x787a20e2af0b5236), "preVisitExprConstEnumeration", offsetof(lint::LintVisitor,preVisitExprConstEnumeration), 0 };
TypeInfo * __type_info__81b654d5a9b1a63_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__b6d18d4b3fadccd4 };
const char * __type_info__81b654d5a9b1a63_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_243 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__81b654d5a9b1a63_arg_types_var_11127340054026167412, __type_info__81b654d5a9b1a63_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstEnumeration> const ))>::size, UINT64_C(0x81b654d5a9b1a63), "visitExprConstEnumeration", offsetof(lint::LintVisitor,visitExprConstEnumeration), 0 };
TypeInfo * __type_info__693e4aab4203cbdc_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9c92a72bb3a64bfa };
const char * __type_info__693e4aab4203cbdc_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_244 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__693e4aab4203cbdc_arg_types_var_11127340054026167412, __type_info__693e4aab4203cbdc_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const ))>::size, UINT64_C(0x693e4aab4203cbdc), "preVisitExprConstBitfield", offsetof(lint::LintVisitor,preVisitExprConstBitfield), 0 };
TypeInfo * __type_info__c626e1678ad826a7_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9c92a72bb3a64bfa };
const char * __type_info__c626e1678ad826a7_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_245 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__c626e1678ad826a7_arg_types_var_11127340054026167412, __type_info__c626e1678ad826a7_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBitfield> const ))>::size, UINT64_C(0xc626e1678ad826a7), "visitExprConstBitfield", offsetof(lint::LintVisitor,visitExprConstBitfield), 0 };
TypeInfo * __type_info__58c956df36cfb014_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4a0758d80e688a0e };
const char * __type_info__58c956df36cfb014_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_246 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__58c956df36cfb014_arg_types_var_11127340054026167412, __type_info__58c956df36cfb014_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const ))>::size, UINT64_C(0x58c956df36cfb014), "preVisitExprConstInt8", offsetof(lint::LintVisitor,preVisitExprConstInt8), 0 };
TypeInfo * __type_info__361911fa4ce540c9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4a0758d80e688a0e };
const char * __type_info__361911fa4ce540c9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_247 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__361911fa4ce540c9_arg_types_var_11127340054026167412, __type_info__361911fa4ce540c9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt8> const ))>::size, UINT64_C(0x361911fa4ce540c9), "visitExprConstInt8", offsetof(lint::LintVisitor,visitExprConstInt8), 0 };
TypeInfo * __type_info__58974fdf367aae2f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__87ae85b131d91f57 };
const char * __type_info__58974fdf367aae2f_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_248 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__58974fdf367aae2f_arg_types_var_11127340054026167412, __type_info__58974fdf367aae2f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const ))>::size, UINT64_C(0x58974fdf367aae2f), "preVisitExprConstInt16", offsetof(lint::LintVisitor,preVisitExprConstInt16), 0 };
TypeInfo * __type_info__b3448b508f91004d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__87ae85b131d91f57 };
const char * __type_info__b3448b508f91004d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_249 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__b3448b508f91004d_arg_types_var_11127340054026167412, __type_info__b3448b508f91004d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt16> const ))>::size, UINT64_C(0xb3448b508f91004d), "visitExprConstInt16", offsetof(lint::LintVisitor,visitExprConstInt16), 0 };
TypeInfo * __type_info__58954cdf36774316_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__773524bb75b61932 };
const char * __type_info__58954cdf36774316_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_250 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__58954cdf36774316_arg_types_var_11127340054026167412, __type_info__58954cdf36774316_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const ))>::size, UINT64_C(0x58954cdf36774316), "preVisitExprConstInt64", offsetof(lint::LintVisitor,preVisitExprConstInt64), 0 };
TypeInfo * __type_info__afde89508cadd3e7_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__773524bb75b61932 };
const char * __type_info__afde89508cadd3e7_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_251 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__afde89508cadd3e7_arg_types_var_11127340054026167412, __type_info__afde89508cadd3e7_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt64> const ))>::size, UINT64_C(0xafde89508cadd3e7), "visitExprConstInt64", offsetof(lint::LintVisitor,visitExprConstInt64), 0 };
TypeInfo * __type_info__b49663e3167d3da4_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__a28688c7ffe035ce };
const char * __type_info__b49663e3167d3da4_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_252 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b49663e3167d3da4_arg_types_var_11127340054026167412, __type_info__b49663e3167d3da4_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const ))>::size, UINT64_C(0xb49663e3167d3da4), "preVisitExprConstInt", offsetof(lint::LintVisitor,preVisitExprConstInt), 0 };
TypeInfo * __type_info__362111fa4cf2d8c9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__a28688c7ffe035ce };
const char * __type_info__362111fa4cf2d8c9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_253 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__362111fa4cf2d8c9_arg_types_var_11127340054026167412, __type_info__362111fa4cf2d8c9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt> const ))>::size, UINT64_C(0x362111fa4cf2d8c9), "visitExprConstInt", offsetof(lint::LintVisitor,visitExprConstInt), 0 };
TypeInfo * __type_info__58c950df36cfa5e2_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__2d27aed7dd587c30 };
const char * __type_info__58c950df36cfa5e2_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_254 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__58c950df36cfa5e2_arg_types_var_11127340054026167412, __type_info__58c950df36cfa5e2_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const ))>::size, UINT64_C(0x58c950df36cfa5e2), "preVisitExprConstInt2", offsetof(lint::LintVisitor,preVisitExprConstInt2), 0 };
TypeInfo * __type_info__361311fa4cdb0ec9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__2d27aed7dd587c30 };
const char * __type_info__361311fa4cdb0ec9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_255 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__361311fa4cdb0ec9_arg_types_var_11127340054026167412, __type_info__361311fa4cdb0ec9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt2> const ))>::size, UINT64_C(0x361311fa4cdb0ec9), "visitExprConstInt2", offsetof(lint::LintVisitor,visitExprConstInt2), 0 };
TypeInfo * __type_info__58c951df36cfa795_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__31b7a5d7e3ad4eb7 };
const char * __type_info__58c951df36cfa795_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_256 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__58c951df36cfa795_arg_types_var_11127340054026167412, __type_info__58c951df36cfa795_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const ))>::size, UINT64_C(0x58c951df36cfa795), "preVisitExprConstInt3", offsetof(lint::LintVisitor,preVisitExprConstInt3), 0 };
TypeInfo * __type_info__361211fa4cd95bc9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__31b7a5d7e3ad4eb7 };
const char * __type_info__361211fa4cd95bc9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_257 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__361211fa4cd95bc9_arg_types_var_11127340054026167412, __type_info__361211fa4cd95bc9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt3> const ))>::size, UINT64_C(0x361211fa4cd95bc9), "visitExprConstInt3", offsetof(lint::LintVisitor,visitExprConstInt3), 0 };
TypeInfo * __type_info__58c94adf36cf9bb0_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__2914e4d7d4fafa72 };
const char * __type_info__58c94adf36cf9bb0_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_258 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__58c94adf36cf9bb0_arg_types_var_11127340054026167412, __type_info__58c94adf36cf9bb0_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const ))>::size, UINT64_C(0x58c94adf36cf9bb0), "preVisitExprConstInt4", offsetof(lint::LintVisitor,preVisitExprConstInt4), 0 };
TypeInfo * __type_info__360d11fa4cd0dcc9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__2914e4d7d4fafa72 };
const char * __type_info__360d11fa4cd0dcc9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_259 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__360d11fa4cd0dcc9_arg_types_var_11127340054026167412, __type_info__360d11fa4cd0dcc9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstInt4> const ))>::size, UINT64_C(0x360d11fa4cd0dcc9), "visitExprConstInt4", offsetof(lint::LintVisitor,visitExprConstInt4), 0 };
TypeInfo * __type_info__b0128c5695ec6adf_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f6614284ea50cbe7 };
const char * __type_info__b0128c5695ec6adf_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_260 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b0128c5695ec6adf_arg_types_var_11127340054026167412, __type_info__b0128c5695ec6adf_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const ))>::size, UINT64_C(0xb0128c5695ec6adf), "preVisitExprConstUInt8", offsetof(lint::LintVisitor,preVisitExprConstUInt8), 0 };
TypeInfo * __type_info__ff488cd458457a2d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f6614284ea50cbe7 };
const char * __type_info__ff488cd458457a2d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_261 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ff488cd458457a2d_arg_types_var_11127340054026167412, __type_info__ff488cd458457a2d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt8> const ))>::size, UINT64_C(0xff488cd458457a2d), "visitExprConstUInt8", offsetof(lint::LintVisitor,visitExprConstUInt8), 0 };
TypeInfo * __type_info__3a856020dab61aeb_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f108ab47d962e793 };
const char * __type_info__3a856020dab61aeb_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_262 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3a856020dab61aeb_arg_types_var_11127340054026167412, __type_info__3a856020dab61aeb_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const ))>::size, UINT64_C(0x3a856020dab61aeb), "preVisitExprConstUInt16", offsetof(lint::LintVisitor,preVisitExprConstUInt16), 0 };
TypeInfo * __type_info__ff3683d45826d4e2_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__f108ab47d962e793 };
const char * __type_info__ff3683d45826d4e2_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_263 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ff3683d45826d4e2_arg_types_var_11127340054026167412, __type_info__ff3683d45826d4e2_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt16> const ))>::size, UINT64_C(0xff3683d45826d4e2), "visitExprConstUInt16", offsetof(lint::LintVisitor,visitExprConstUInt16), 0 };
TypeInfo * __type_info__30536220d20ca351_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__63e1b8a29ad93469 };
const char * __type_info__30536220d20ca351_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_264 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__30536220d20ca351_arg_types_var_11127340054026167412, __type_info__30536220d20ca351_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const ))>::size, UINT64_C(0x30536220d20ca351), "preVisitExprConstUInt64", offsetof(lint::LintVisitor,preVisitExprConstUInt64), 0 };
TypeInfo * __type_info__ff347ed458236663_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__63e1b8a29ad93469 };
const char * __type_info__ff347ed458236663_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_265 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ff347ed458236663_arg_types_var_11127340054026167412, __type_info__ff347ed458236663_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt64> const ))>::size, UINT64_C(0xff347ed458236663), "visitExprConstUInt64", offsetof(lint::LintVisitor,visitExprConstUInt64), 0 };
TypeInfo * __type_info__b00a8c5695ded2df_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__518dd4a2ad91defd };
const char * __type_info__b00a8c5695ded2df_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_266 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b00a8c5695ded2df_arg_types_var_11127340054026167412, __type_info__b00a8c5695ded2df_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const ))>::size, UINT64_C(0xb00a8c5695ded2df), "preVisitExprConstUInt", offsetof(lint::LintVisitor,preVisitExprConstUInt), 0 };
TypeInfo * __type_info__e06027fa03bbe4a7_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__518dd4a2ad91defd };
const char * __type_info__e06027fa03bbe4a7_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_267 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__e06027fa03bbe4a7_arg_types_var_11127340054026167412, __type_info__e06027fa03bbe4a7_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt> const ))>::size, UINT64_C(0xe06027fa03bbe4a7), "visitExprConstUInt", offsetof(lint::LintVisitor,visitExprConstUInt), 0 };
TypeInfo * __type_info__b01c8c5695fd68df_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__514742689af99de7 };
const char * __type_info__b01c8c5695fd68df_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_268 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b01c8c5695fd68df_arg_types_var_11127340054026167412, __type_info__b01c8c5695fd68df_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const ))>::size, UINT64_C(0xb01c8c5695fd68df), "preVisitExprConstUInt2", offsetof(lint::LintVisitor,preVisitExprConstUInt2), 0 };
TypeInfo * __type_info__ff4882d45845692f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__514742689af99de7 };
const char * __type_info__ff4882d45845692f_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_269 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ff4882d45845692f_arg_types_var_11127340054026167412, __type_info__ff4882d45845692f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt2> const ))>::size, UINT64_C(0xff4882d45845692f), "visitExprConstUInt2", offsetof(lint::LintVisitor,visitExprConstUInt2), 0 };
TypeInfo * __type_info__b01d8c5695ff1bdf_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__563e42636db280e7 };
const char * __type_info__b01d8c5695ff1bdf_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_270 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b01d8c5695ff1bdf_arg_types_var_11127340054026167412, __type_info__b01d8c5695ff1bdf_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const ))>::size, UINT64_C(0xb01d8c5695ff1bdf), "preVisitExprConstUInt3", offsetof(lint::LintVisitor,preVisitExprConstUInt3), 0 };
TypeInfo * __type_info__ff4881d45845677c_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__563e42636db280e7 };
const char * __type_info__ff4881d45845677c_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_271 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ff4881d45845677c_arg_types_var_11127340054026167412, __type_info__ff4881d45845677c_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt3> const ))>::size, UINT64_C(0xff4881d45845677c), "visitExprConstUInt3", offsetof(lint::LintVisitor,visitExprConstUInt3), 0 };
TypeInfo * __type_info__b0168c5695f336df_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__a17d42718ebadfe7 };
const char * __type_info__b0168c5695f336df_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_272 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b0168c5695f336df_arg_types_var_11127340054026167412, __type_info__b0168c5695f336df_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const ))>::size, UINT64_C(0xb0168c5695f336df), "preVisitExprConstUInt4", offsetof(lint::LintVisitor,preVisitExprConstUInt4), 0 };
TypeInfo * __type_info__ff4880d4584565c9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__a17d42718ebadfe7 };
const char * __type_info__ff4880d4584565c9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_273 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__ff4880d4584565c9_arg_types_var_11127340054026167412, __type_info__ff4880d4584565c9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstUInt4> const ))>::size, UINT64_C(0xff4880d4584565c9), "visitExprConstUInt4", offsetof(lint::LintVisitor,visitExprConstUInt4), 0 };
TypeInfo * __type_info__8ed6ad645b789f5a_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__214ca0a8404236ce };
const char * __type_info__8ed6ad645b789f5a_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_274 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8ed6ad645b789f5a_arg_types_var_11127340054026167412, __type_info__8ed6ad645b789f5a_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const ))>::size, UINT64_C(0x8ed6ad645b789f5a), "preVisitExprConstRange", offsetof(lint::LintVisitor,preVisitExprConstRange), 0 };
TypeInfo * __type_info__13cd994b0f0035e3_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__214ca0a8404236ce };
const char * __type_info__13cd994b0f0035e3_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_275 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__13cd994b0f0035e3_arg_types_var_11127340054026167412, __type_info__13cd994b0f0035e3_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange> const ))>::size, UINT64_C(0x13cd994b0f0035e3), "visitExprConstRange", offsetof(lint::LintVisitor,visitExprConstRange), 0 };
TypeInfo * __type_info__9327fdd772b62a8d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__d2fee6b26665c989 };
const char * __type_info__9327fdd772b62a8d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_276 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9327fdd772b62a8d_arg_types_var_11127340054026167412, __type_info__9327fdd772b62a8d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const ))>::size, UINT64_C(0x9327fdd772b62a8d), "preVisitExprConstURange", offsetof(lint::LintVisitor,preVisitExprConstURange), 0 };
TypeInfo * __type_info__62d2cdecf1438563_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__d2fee6b26665c989 };
const char * __type_info__62d2cdecf1438563_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_277 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__62d2cdecf1438563_arg_types_var_11127340054026167412, __type_info__62d2cdecf1438563_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange> const ))>::size, UINT64_C(0x62d2cdecf1438563), "visitExprConstURange", offsetof(lint::LintVisitor,visitExprConstURange), 0 };
TypeInfo * __type_info__2f740d876e0b4884_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4f02b717be42f032 };
const char * __type_info__2f740d876e0b4884_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_278 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2f740d876e0b4884_arg_types_var_11127340054026167412, __type_info__2f740d876e0b4884_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const ))>::size, UINT64_C(0x2f740d876e0b4884), "preVisitExprConstRange64", offsetof(lint::LintVisitor,preVisitExprConstRange64), 0 };
TypeInfo * __type_info__507d518b0db97e55_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4f02b717be42f032 };
const char * __type_info__507d518b0db97e55_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_279 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__507d518b0db97e55_arg_types_var_11127340054026167412, __type_info__507d518b0db97e55_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstRange64> const ))>::size, UINT64_C(0x507d518b0db97e55), "visitExprConstRange64", offsetof(lint::LintVisitor,visitExprConstRange64), 0 };
TypeInfo * __type_info__6d0b0e187be89a5b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__b8524aede8fd2575 };
const char * __type_info__6d0b0e187be89a5b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_280 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6d0b0e187be89a5b_arg_types_var_11127340054026167412, __type_info__6d0b0e187be89a5b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const ))>::size, UINT64_C(0x6d0b0e187be89a5b), "preVisitExprConstURange64", offsetof(lint::LintVisitor,preVisitExprConstURange64), 0 };
TypeInfo * __type_info__2fe53e9df606536f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__b8524aede8fd2575 };
const char * __type_info__2fe53e9df606536f_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_281 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__2fe53e9df606536f_arg_types_var_11127340054026167412, __type_info__2fe53e9df606536f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstURange64> const ))>::size, UINT64_C(0x2fe53e9df606536f), "visitExprConstURange64", offsetof(lint::LintVisitor,visitExprConstURange64), 0 };
TypeInfo * __type_info__b633a8b508277541_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__ea252439573ea197 };
const char * __type_info__b633a8b508277541_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_282 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b633a8b508277541_arg_types_var_11127340054026167412, __type_info__b633a8b508277541_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const ))>::size, UINT64_C(0xb633a8b508277541), "preVisitExprConstBool", offsetof(lint::LintVisitor,preVisitExprConstBool), 0 };
TypeInfo * __type_info__394406fa4f784c37_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__ea252439573ea197 };
const char * __type_info__394406fa4f784c37_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_283 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__394406fa4f784c37_arg_types_var_11127340054026167412, __type_info__394406fa4f784c37_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstBool> const ))>::size, UINT64_C(0x394406fa4f784c37), "visitExprConstBool", offsetof(lint::LintVisitor,visitExprConstBool), 0 };
TypeInfo * __type_info__41af8b9fdecb34d1_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__50c7808637778d65 };
const char * __type_info__41af8b9fdecb34d1_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_284 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__41af8b9fdecb34d1_arg_types_var_11127340054026167412, __type_info__41af8b9fdecb34d1_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const ))>::size, UINT64_C(0x41af8b9fdecb34d1), "preVisitExprConstFloat", offsetof(lint::LintVisitor,preVisitExprConstFloat), 0 };
TypeInfo * __type_info__98a68a5943f0f33d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__50c7808637778d65 };
const char * __type_info__98a68a5943f0f33d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_285 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__98a68a5943f0f33d_arg_types_var_11127340054026167412, __type_info__98a68a5943f0f33d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat> const ))>::size, UINT64_C(0x98a68a5943f0f33d), "visitExprConstFloat", offsetof(lint::LintVisitor,visitExprConstFloat), 0 };
TypeInfo * __type_info__687f23a7934addb9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4f4cc10892c6c61 };
const char * __type_info__687f23a7934addb9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_286 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__687f23a7934addb9_arg_types_var_11127340054026167412, __type_info__687f23a7934addb9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const ))>::size, UINT64_C(0x687f23a7934addb9), "preVisitExprConstFloat2", offsetof(lint::LintVisitor,preVisitExprConstFloat2), 0 };
TypeInfo * __type_info__98b48a594408bd3d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__4f4cc10892c6c61 };
const char * __type_info__98b48a594408bd3d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_287 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__98b48a594408bd3d_arg_types_var_11127340054026167412, __type_info__98b48a594408bd3d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat2> const ))>::size, UINT64_C(0x98b48a594408bd3d), "visitExprConstFloat2", offsetof(lint::LintVisitor,visitExprConstFloat2), 0 };
TypeInfo * __type_info__687f22a7934adc06_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__65f51082d9833a };
const char * __type_info__687f22a7934adc06_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_288 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__687f22a7934adc06_arg_types_var_11127340054026167412, __type_info__687f22a7934adc06_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const ))>::size, UINT64_C(0x687f22a7934adc06), "preVisitExprConstFloat3", offsetof(lint::LintVisitor,preVisitExprConstFloat3), 0 };
TypeInfo * __type_info__98b38a5944070a3d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__65f51082d9833a };
const char * __type_info__98b38a5944070a3d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_289 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__98b38a5944070a3d_arg_types_var_11127340054026167412, __type_info__98b38a5944070a3d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat3> const ))>::size, UINT64_C(0x98b38a5944070a3d), "visitExprConstFloat3", offsetof(lint::LintVisitor,visitExprConstFloat3), 0 };
TypeInfo * __type_info__687f25a7934ae11f_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9089610918ba11f };
const char * __type_info__687f25a7934ae11f_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_290 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__687f25a7934ae11f_arg_types_var_11127340054026167412, __type_info__687f25a7934ae11f_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const ))>::size, UINT64_C(0x687f25a7934ae11f), "preVisitExprConstFloat4", offsetof(lint::LintVisitor,preVisitExprConstFloat4), 0 };
TypeInfo * __type_info__98b28a594405573d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__9089610918ba11f };
const char * __type_info__98b28a594405573d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_291 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__98b28a594405573d_arg_types_var_11127340054026167412, __type_info__98b28a594405573d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstFloat4> const ))>::size, UINT64_C(0x98b28a594405573d), "visitExprConstFloat4", offsetof(lint::LintVisitor,visitExprConstFloat4), 0 };
TypeInfo * __type_info__c90cd369f9edeae2_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e9cb7c9300717d9e };
const char * __type_info__c90cd369f9edeae2_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_292 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c90cd369f9edeae2_arg_types_var_11127340054026167412, __type_info__c90cd369f9edeae2_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstString> const ))>::size, UINT64_C(0xc90cd369f9edeae2), "preVisitExprConstString", offsetof(lint::LintVisitor,preVisitExprConstString), 0 };
TypeInfo * __type_info__7205cd803dc1e121_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__e9cb7c9300717d9e };
const char * __type_info__7205cd803dc1e121_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_293 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__7205cd803dc1e121_arg_types_var_11127340054026167412, __type_info__7205cd803dc1e121_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstString> const ))>::size, UINT64_C(0x7205cd803dc1e121), "visitExprConstString", offsetof(lint::LintVisitor,visitExprConstString), 0 };
TypeInfo * __type_info__92ced17140df3d1e_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7f81cc8503986a86 };
const char * __type_info__92ced17140df3d1e_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_294 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__92ced17140df3d1e_arg_types_var_11127340054026167412, __type_info__92ced17140df3d1e_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const ))>::size, UINT64_C(0x92ced17140df3d1e), "preVisitExprConstDouble", offsetof(lint::LintVisitor,preVisitExprConstDouble), 0 };
TypeInfo * __type_info__b28e4055068d1da9_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__7f81cc8503986a86 };
const char * __type_info__b28e4055068d1da9_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_295 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__b28e4055068d1da9_arg_types_var_11127340054026167412, __type_info__b28e4055068d1da9_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprConstDouble> const ))>::size, UINT64_C(0xb28e4055068d1da9), "visitExprConstDouble", offsetof(lint::LintVisitor,visitExprConstDouble), 0 };
TypeInfo * __type_info__8fedab75f45f3548_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__3229d47464f4ad50 };
const char * __type_info__8fedab75f45f3548_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_296 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8fedab75f45f3548_arg_types_var_11127340054026167412, __type_info__8fedab75f45f3548_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const ))>::size, UINT64_C(0x8fedab75f45f3548), "preVisitExprFakeContext", offsetof(lint::LintVisitor,preVisitExprFakeContext), 0 };
TypeInfo * __type_info__f83d84d58244effd_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__3229d47464f4ad50 };
const char * __type_info__f83d84d58244effd_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_297 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__f83d84d58244effd_arg_types_var_11127340054026167412, __type_info__f83d84d58244effd_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeContext> const ))>::size, UINT64_C(0xf83d84d58244effd), "visitExprFakeContext", offsetof(lint::LintVisitor,visitExprFakeContext), 0 };
TypeInfo * __type_info__909eed032d467225_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__511818eae83f8137 };
const char * __type_info__909eed032d467225_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_298 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__909eed032d467225_arg_types_var_11127340054026167412, __type_info__909eed032d467225_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const ))>::size, UINT64_C(0x909eed032d467225), "preVisitExprFakeLineInfo", offsetof(lint::LintVisitor,preVisitExprFakeLineInfo), 0 };
TypeInfo * __type_info__b5f928184266f21d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__511818eae83f8137 };
const char * __type_info__b5f928184266f21d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_299 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__b5f928184266f21d_arg_types_var_11127340054026167412, __type_info__b5f928184266f21d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprFakeLineInfo> const ))>::size, UINT64_C(0xb5f928184266f21d), "visitExprFakeLineInfo", offsetof(lint::LintVisitor,visitExprFakeLineInfo), 0 };
TypeInfo * __type_info__4c14504e30d83a91_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6628bcbce7db6a7d };
const char * __type_info__4c14504e30d83a91_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_300 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4c14504e30d83a91_arg_types_var_11127340054026167412, __type_info__4c14504e30d83a91_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReader> const ))>::size, UINT64_C(0x4c14504e30d83a91), "preVisitExprReader", offsetof(lint::LintVisitor,preVisitExprReader), 0 };
TypeInfo * __type_info__9afaabd3d7120e7b_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__6628bcbce7db6a7d };
const char * __type_info__9afaabd3d7120e7b_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_301 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__9afaabd3d7120e7b_arg_types_var_11127340054026167412, __type_info__9afaabd3d7120e7b_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprReader> const ))>::size, UINT64_C(0x9afaabd3d7120e7b), "visitExprReader", offsetof(lint::LintVisitor,visitExprReader), 0 };
TypeInfo * __type_info__bd371c66c9eb6817_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__eb22258b16c8c6df };
const char * __type_info__bd371c66c9eb6817_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_302 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bd371c66c9eb6817_arg_types_var_11127340054026167412, __type_info__bd371c66c9eb6817_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const ))>::size, UINT64_C(0xbd371c66c9eb6817), "preVisitExprUnsafe", offsetof(lint::LintVisitor,preVisitExprUnsafe), 0 };
TypeInfo * __type_info__e3bf0919c4d907a6_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__eb22258b16c8c6df };
const char * __type_info__e3bf0919c4d907a6_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_303 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__e3bf0919c4d907a6_arg_types_var_11127340054026167412, __type_info__e3bf0919c4d907a6_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprUnsafe> const ))>::size, UINT64_C(0xe3bf0919c4d907a6), "visitExprUnsafe", offsetof(lint::LintVisitor,visitExprUnsafe), 0 };
TypeInfo * __type_info__4673d8a661158f3d_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__636dc1714c171367 };
const char * __type_info__4673d8a661158f3d_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_304 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4673d8a661158f3d_arg_types_var_11127340054026167412, __type_info__4673d8a661158f3d_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const ))>::size, UINT64_C(0x4673d8a661158f3d), "preVisitExprCallMacro", offsetof(lint::LintVisitor,preVisitExprCallMacro), 0 };
TypeInfo * __type_info__db1a5348466c1d30_arg_types_var_11127340054026167412[2] = { &__type_info__21586ce84f433a21, &__type_info__636dc1714c171367 };
const char * __type_info__db1a5348466c1d30_arg_names_var_11127340054026167412[2] = { "self", "expr" };
VarInfo __struct_info__9a6c40d2ef34a474_field_305 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__6636442e03391ebf, nullptr, (TypeInfo **)__type_info__db1a5348466c1d30_arg_types_var_11127340054026167412, __type_info__db1a5348466c1d30_arg_names_var_11127340054026167412, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprCallMacro> const ))>::size, UINT64_C(0xdb1a5348466c1d30), "visitExprCallMacro", offsetof(lint::LintVisitor,visitExprCallMacro), 0 };
VarInfo __struct_info__9a6c40d2ef34a474_field_306 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__4200353d82fda873, nullptr, nullptr, nullptr, 0, 0, nullptr, 9216, TypeSize<smart_ptr_raw<VisitorAdapter>>::size, UINT64_C(0x2aff4c764d300b71), "astVisitorAdapter", offsetof(lint::LintVisitor,astVisitorAdapter), 307 };
VarInfo __struct_info__9a6c40d2ef34a474_field_307 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__d9307e078cfb0f0c, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint64_t>>::size, UINT64_C(0xa78a6e81bc3fbb28), "exprForTerminator", offsetof(lint::LintVisitor,exprForTerminator), 312 };
VarInfo __struct_info__9a6c40d2ef34a474_field_308 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xac5e73a504cb41ed), "compile_time_errors", offsetof(lint::LintVisitor,compile_time_errors), 0 };
VarInfo __struct_info__9a6c40d2ef34a474_field_309 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xc7ea840626f63c1), "noLint", offsetof(lint::LintVisitor,noLint), 0 };
TypeInfo * __type_info__d97ffc052624b889_arg_types_var_11127340054026167412[3] = { &__type_info__f5340b6734d7b2c1, &__type_info__af90fe4c864e9d52, &__type_info__246dda13a8a4b104 };
const char * __type_info__d97ffc052624b889_arg_names_var_11127340054026167412[3] = { "self", "text", "at" };
VarInfo __struct_info__9a6c40d2ef34a474_field_310 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d97ffc052624b889_arg_types_var_11127340054026167412, __type_info__d97ffc052624b889_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,lint::LintVisitor,char * const ,LineInfo const ))>::size, UINT64_C(0xd97ffc052624b889), "lint_error", offsetof(lint::LintVisitor,lint_error), 0 };
TypeInfo * __type_info__a1f9f6d18fd4d7d5_arg_types_var_11127340054026167412[3] = { &__type_info__f5340b6734d7b2c1, &__type_info__a3a6bcfebaf8fcd8, &__type_info__af81fe4c86352052 };
const char * __type_info__a1f9f6d18fd4d7d5_arg_names_var_11127340054026167412[3] = { "self", "v", "can_make_const" };
VarInfo __struct_info__9a6c40d2ef34a474_field_311 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a1f9f6d18fd4d7d5_arg_types_var_11127340054026167412, __type_info__a1f9f6d18fd4d7d5_arg_names_var_11127340054026167412, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,lint::LintVisitor,smart_ptr_raw<Variable> const ,bool))>::size, UINT64_C(0xa1f9f6d18fd4d7d5), "validate_var", offsetof(lint::LintVisitor,validate_var), 0 };
VarInfo * __struct_info__9a6c40d2ef34a474_fields[312] =  { &__struct_info__9a6c40d2ef34a474_field_0, &__struct_info__9a6c40d2ef34a474_field_1, &__struct_info__9a6c40d2ef34a474_field_2, &__struct_info__9a6c40d2ef34a474_field_3, &__struct_info__9a6c40d2ef34a474_field_4, &__struct_info__9a6c40d2ef34a474_field_5, &__struct_info__9a6c40d2ef34a474_field_6, &__struct_info__9a6c40d2ef34a474_field_7, &__struct_info__9a6c40d2ef34a474_field_8, &__struct_info__9a6c40d2ef34a474_field_9, &__struct_info__9a6c40d2ef34a474_field_10, &__struct_info__9a6c40d2ef34a474_field_11, &__struct_info__9a6c40d2ef34a474_field_12, &__struct_info__9a6c40d2ef34a474_field_13, &__struct_info__9a6c40d2ef34a474_field_14, &__struct_info__9a6c40d2ef34a474_field_15, &__struct_info__9a6c40d2ef34a474_field_16, &__struct_info__9a6c40d2ef34a474_field_17, &__struct_info__9a6c40d2ef34a474_field_18, &__struct_info__9a6c40d2ef34a474_field_19, &__struct_info__9a6c40d2ef34a474_field_20, &__struct_info__9a6c40d2ef34a474_field_21, &__struct_info__9a6c40d2ef34a474_field_22, &__struct_info__9a6c40d2ef34a474_field_23, &__struct_info__9a6c40d2ef34a474_field_24, &__struct_info__9a6c40d2ef34a474_field_25, &__struct_info__9a6c40d2ef34a474_field_26, &__struct_info__9a6c40d2ef34a474_field_27, &__struct_info__9a6c40d2ef34a474_field_28, &__struct_info__9a6c40d2ef34a474_field_29, &__struct_info__9a6c40d2ef34a474_field_30, &__struct_info__9a6c40d2ef34a474_field_31, &__struct_info__9a6c40d2ef34a474_field_32, &__struct_info__9a6c40d2ef34a474_field_33, &__struct_info__9a6c40d2ef34a474_field_34, &__struct_info__9a6c40d2ef34a474_field_35, &__struct_info__9a6c40d2ef34a474_field_36, &__struct_info__9a6c40d2ef34a474_field_37, &__struct_info__9a6c40d2ef34a474_field_38, &__struct_info__9a6c40d2ef34a474_field_39, &__struct_info__9a6c40d2ef34a474_field_40, &__struct_info__9a6c40d2ef34a474_field_41, &__struct_info__9a6c40d2ef34a474_field_42, &__struct_info__9a6c40d2ef34a474_field_43, &__struct_info__9a6c40d2ef34a474_field_44, &__struct_info__9a6c40d2ef34a474_field_45, &__struct_info__9a6c40d2ef34a474_field_46, &__struct_info__9a6c40d2ef34a474_field_47, &__struct_info__9a6c40d2ef34a474_field_48, &__struct_info__9a6c40d2ef34a474_field_49, &__struct_info__9a6c40d2ef34a474_field_50, &__struct_info__9a6c40d2ef34a474_field_51, &__struct_info__9a6c40d2ef34a474_field_52, &__struct_info__9a6c40d2ef34a474_field_53, &__struct_info__9a6c40d2ef34a474_field_54, &__struct_info__9a6c40d2ef34a474_field_55, &__struct_info__9a6c40d2ef34a474_field_56, &__struct_info__9a6c40d2ef34a474_field_57, &__struct_info__9a6c40d2ef34a474_field_58, &__struct_info__9a6c40d2ef34a474_field_59, &__struct_info__9a6c40d2ef34a474_field_60, &__struct_info__9a6c40d2ef34a474_field_61, &__struct_info__9a6c40d2ef34a474_field_62, &__struct_info__9a6c40d2ef34a474_field_63, &__struct_info__9a6c40d2ef34a474_field_64, &__struct_info__9a6c40d2ef34a474_field_65, &__struct_info__9a6c40d2ef34a474_field_66, &__struct_info__9a6c40d2ef34a474_field_67, &__struct_info__9a6c40d2ef34a474_field_68, &__struct_info__9a6c40d2ef34a474_field_69, &__struct_info__9a6c40d2ef34a474_field_70, &__struct_info__9a6c40d2ef34a474_field_71, &__struct_info__9a6c40d2ef34a474_field_72, &__struct_info__9a6c40d2ef34a474_field_73, &__struct_info__9a6c40d2ef34a474_field_74, &__struct_info__9a6c40d2ef34a474_field_75, &__struct_info__9a6c40d2ef34a474_field_76, &__struct_info__9a6c40d2ef34a474_field_77, &__struct_info__9a6c40d2ef34a474_field_78, &__struct_info__9a6c40d2ef34a474_field_79, &__struct_info__9a6c40d2ef34a474_field_80, &__struct_info__9a6c40d2ef34a474_field_81, &__struct_info__9a6c40d2ef34a474_field_82, &__struct_info__9a6c40d2ef34a474_field_83, &__struct_info__9a6c40d2ef34a474_field_84, &__struct_info__9a6c40d2ef34a474_field_85, &__struct_info__9a6c40d2ef34a474_field_86, &__struct_info__9a6c40d2ef34a474_field_87, &__struct_info__9a6c40d2ef34a474_field_88, &__struct_info__9a6c40d2ef34a474_field_89, &__struct_info__9a6c40d2ef34a474_field_90, &__struct_info__9a6c40d2ef34a474_field_91, &__struct_info__9a6c40d2ef34a474_field_92, &__struct_info__9a6c40d2ef34a474_field_93, &__struct_info__9a6c40d2ef34a474_field_94, &__struct_info__9a6c40d2ef34a474_field_95, &__struct_info__9a6c40d2ef34a474_field_96, &__struct_info__9a6c40d2ef34a474_field_97, &__struct_info__9a6c40d2ef34a474_field_98, &__struct_info__9a6c40d2ef34a474_field_99, &__struct_info__9a6c40d2ef34a474_field_100, &__struct_info__9a6c40d2ef34a474_field_101, &__struct_info__9a6c40d2ef34a474_field_102, &__struct_info__9a6c40d2ef34a474_field_103, &__struct_info__9a6c40d2ef34a474_field_104, &__struct_info__9a6c40d2ef34a474_field_105, &__struct_info__9a6c40d2ef34a474_field_106, &__struct_info__9a6c40d2ef34a474_field_107, &__struct_info__9a6c40d2ef34a474_field_108, &__struct_info__9a6c40d2ef34a474_field_109, &__struct_info__9a6c40d2ef34a474_field_110, &__struct_info__9a6c40d2ef34a474_field_111, &__struct_info__9a6c40d2ef34a474_field_112, &__struct_info__9a6c40d2ef34a474_field_113, &__struct_info__9a6c40d2ef34a474_field_114, &__struct_info__9a6c40d2ef34a474_field_115, &__struct_info__9a6c40d2ef34a474_field_116, &__struct_info__9a6c40d2ef34a474_field_117, &__struct_info__9a6c40d2ef34a474_field_118, &__struct_info__9a6c40d2ef34a474_field_119, &__struct_info__9a6c40d2ef34a474_field_120, &__struct_info__9a6c40d2ef34a474_field_121, &__struct_info__9a6c40d2ef34a474_field_122, &__struct_info__9a6c40d2ef34a474_field_123, &__struct_info__9a6c40d2ef34a474_field_124, &__struct_info__9a6c40d2ef34a474_field_125, &__struct_info__9a6c40d2ef34a474_field_126, &__struct_info__9a6c40d2ef34a474_field_127, &__struct_info__9a6c40d2ef34a474_field_128, &__struct_info__9a6c40d2ef34a474_field_129, &__struct_info__9a6c40d2ef34a474_field_130, &__struct_info__9a6c40d2ef34a474_field_131, &__struct_info__9a6c40d2ef34a474_field_132, &__struct_info__9a6c40d2ef34a474_field_133, &__struct_info__9a6c40d2ef34a474_field_134, &__struct_info__9a6c40d2ef34a474_field_135, &__struct_info__9a6c40d2ef34a474_field_136, &__struct_info__9a6c40d2ef34a474_field_137, &__struct_info__9a6c40d2ef34a474_field_138, &__struct_info__9a6c40d2ef34a474_field_139, &__struct_info__9a6c40d2ef34a474_field_140, &__struct_info__9a6c40d2ef34a474_field_141, &__struct_info__9a6c40d2ef34a474_field_142, &__struct_info__9a6c40d2ef34a474_field_143, &__struct_info__9a6c40d2ef34a474_field_144, &__struct_info__9a6c40d2ef34a474_field_145, &__struct_info__9a6c40d2ef34a474_field_146, &__struct_info__9a6c40d2ef34a474_field_147, &__struct_info__9a6c40d2ef34a474_field_148, &__struct_info__9a6c40d2ef34a474_field_149, &__struct_info__9a6c40d2ef34a474_field_150, &__struct_info__9a6c40d2ef34a474_field_151, &__struct_info__9a6c40d2ef34a474_field_152, &__struct_info__9a6c40d2ef34a474_field_153, &__struct_info__9a6c40d2ef34a474_field_154, &__struct_info__9a6c40d2ef34a474_field_155, &__struct_info__9a6c40d2ef34a474_field_156, &__struct_info__9a6c40d2ef34a474_field_157, &__struct_info__9a6c40d2ef34a474_field_158, &__struct_info__9a6c40d2ef34a474_field_159, &__struct_info__9a6c40d2ef34a474_field_160, &__struct_info__9a6c40d2ef34a474_field_161, &__struct_info__9a6c40d2ef34a474_field_162, &__struct_info__9a6c40d2ef34a474_field_163, &__struct_info__9a6c40d2ef34a474_field_164, &__struct_info__9a6c40d2ef34a474_field_165, &__struct_info__9a6c40d2ef34a474_field_166, &__struct_info__9a6c40d2ef34a474_field_167, &__struct_info__9a6c40d2ef34a474_field_168, &__struct_info__9a6c40d2ef34a474_field_169, &__struct_info__9a6c40d2ef34a474_field_170, &__struct_info__9a6c40d2ef34a474_field_171, &__struct_info__9a6c40d2ef34a474_field_172, &__struct_info__9a6c40d2ef34a474_field_173, &__struct_info__9a6c40d2ef34a474_field_174, &__struct_info__9a6c40d2ef34a474_field_175, &__struct_info__9a6c40d2ef34a474_field_176, &__struct_info__9a6c40d2ef34a474_field_177, &__struct_info__9a6c40d2ef34a474_field_178, &__struct_info__9a6c40d2ef34a474_field_179, &__struct_info__9a6c40d2ef34a474_field_180, &__struct_info__9a6c40d2ef34a474_field_181, &__struct_info__9a6c40d2ef34a474_field_182, &__struct_info__9a6c40d2ef34a474_field_183, &__struct_info__9a6c40d2ef34a474_field_184, &__struct_info__9a6c40d2ef34a474_field_185, &__struct_info__9a6c40d2ef34a474_field_186, &__struct_info__9a6c40d2ef34a474_field_187, &__struct_info__9a6c40d2ef34a474_field_188, &__struct_info__9a6c40d2ef34a474_field_189, &__struct_info__9a6c40d2ef34a474_field_190, &__struct_info__9a6c40d2ef34a474_field_191, &__struct_info__9a6c40d2ef34a474_field_192, &__struct_info__9a6c40d2ef34a474_field_193, &__struct_info__9a6c40d2ef34a474_field_194, &__struct_info__9a6c40d2ef34a474_field_195, &__struct_info__9a6c40d2ef34a474_field_196, &__struct_info__9a6c40d2ef34a474_field_197, &__struct_info__9a6c40d2ef34a474_field_198, &__struct_info__9a6c40d2ef34a474_field_199, &__struct_info__9a6c40d2ef34a474_field_200, &__struct_info__9a6c40d2ef34a474_field_201, &__struct_info__9a6c40d2ef34a474_field_202, &__struct_info__9a6c40d2ef34a474_field_203, &__struct_info__9a6c40d2ef34a474_field_204, &__struct_info__9a6c40d2ef34a474_field_205, &__struct_info__9a6c40d2ef34a474_field_206, &__struct_info__9a6c40d2ef34a474_field_207, &__struct_info__9a6c40d2ef34a474_field_208, &__struct_info__9a6c40d2ef34a474_field_209, &__struct_info__9a6c40d2ef34a474_field_210, &__struct_info__9a6c40d2ef34a474_field_211, &__struct_info__9a6c40d2ef34a474_field_212, &__struct_info__9a6c40d2ef34a474_field_213, &__struct_info__9a6c40d2ef34a474_field_214, &__struct_info__9a6c40d2ef34a474_field_215, &__struct_info__9a6c40d2ef34a474_field_216, &__struct_info__9a6c40d2ef34a474_field_217, &__struct_info__9a6c40d2ef34a474_field_218, &__struct_info__9a6c40d2ef34a474_field_219, &__struct_info__9a6c40d2ef34a474_field_220, &__struct_info__9a6c40d2ef34a474_field_221, &__struct_info__9a6c40d2ef34a474_field_222, &__struct_info__9a6c40d2ef34a474_field_223, &__struct_info__9a6c40d2ef34a474_field_224, &__struct_info__9a6c40d2ef34a474_field_225, &__struct_info__9a6c40d2ef34a474_field_226, &__struct_info__9a6c40d2ef34a474_field_227, &__struct_info__9a6c40d2ef34a474_field_228, &__struct_info__9a6c40d2ef34a474_field_229, &__struct_info__9a6c40d2ef34a474_field_230, &__struct_info__9a6c40d2ef34a474_field_231, &__struct_info__9a6c40d2ef34a474_field_232, &__struct_info__9a6c40d2ef34a474_field_233, &__struct_info__9a6c40d2ef34a474_field_234, &__struct_info__9a6c40d2ef34a474_field_235, &__struct_info__9a6c40d2ef34a474_field_236, &__struct_info__9a6c40d2ef34a474_field_237, &__struct_info__9a6c40d2ef34a474_field_238, &__struct_info__9a6c40d2ef34a474_field_239, &__struct_info__9a6c40d2ef34a474_field_240, &__struct_info__9a6c40d2ef34a474_field_241, &__struct_info__9a6c40d2ef34a474_field_242, &__struct_info__9a6c40d2ef34a474_field_243, &__struct_info__9a6c40d2ef34a474_field_244, &__struct_info__9a6c40d2ef34a474_field_245, &__struct_info__9a6c40d2ef34a474_field_246, &__struct_info__9a6c40d2ef34a474_field_247, &__struct_info__9a6c40d2ef34a474_field_248, &__struct_info__9a6c40d2ef34a474_field_249, &__struct_info__9a6c40d2ef34a474_field_250, &__struct_info__9a6c40d2ef34a474_field_251, &__struct_info__9a6c40d2ef34a474_field_252, &__struct_info__9a6c40d2ef34a474_field_253, &__struct_info__9a6c40d2ef34a474_field_254, &__struct_info__9a6c40d2ef34a474_field_255, &__struct_info__9a6c40d2ef34a474_field_256, &__struct_info__9a6c40d2ef34a474_field_257, &__struct_info__9a6c40d2ef34a474_field_258, &__struct_info__9a6c40d2ef34a474_field_259, &__struct_info__9a6c40d2ef34a474_field_260, &__struct_info__9a6c40d2ef34a474_field_261, &__struct_info__9a6c40d2ef34a474_field_262, &__struct_info__9a6c40d2ef34a474_field_263, &__struct_info__9a6c40d2ef34a474_field_264, &__struct_info__9a6c40d2ef34a474_field_265, &__struct_info__9a6c40d2ef34a474_field_266, &__struct_info__9a6c40d2ef34a474_field_267, &__struct_info__9a6c40d2ef34a474_field_268, &__struct_info__9a6c40d2ef34a474_field_269, &__struct_info__9a6c40d2ef34a474_field_270, &__struct_info__9a6c40d2ef34a474_field_271, &__struct_info__9a6c40d2ef34a474_field_272, &__struct_info__9a6c40d2ef34a474_field_273, &__struct_info__9a6c40d2ef34a474_field_274, &__struct_info__9a6c40d2ef34a474_field_275, &__struct_info__9a6c40d2ef34a474_field_276, &__struct_info__9a6c40d2ef34a474_field_277, &__struct_info__9a6c40d2ef34a474_field_278, &__struct_info__9a6c40d2ef34a474_field_279, &__struct_info__9a6c40d2ef34a474_field_280, &__struct_info__9a6c40d2ef34a474_field_281, &__struct_info__9a6c40d2ef34a474_field_282, &__struct_info__9a6c40d2ef34a474_field_283, &__struct_info__9a6c40d2ef34a474_field_284, &__struct_info__9a6c40d2ef34a474_field_285, &__struct_info__9a6c40d2ef34a474_field_286, &__struct_info__9a6c40d2ef34a474_field_287, &__struct_info__9a6c40d2ef34a474_field_288, &__struct_info__9a6c40d2ef34a474_field_289, &__struct_info__9a6c40d2ef34a474_field_290, &__struct_info__9a6c40d2ef34a474_field_291, &__struct_info__9a6c40d2ef34a474_field_292, &__struct_info__9a6c40d2ef34a474_field_293, &__struct_info__9a6c40d2ef34a474_field_294, &__struct_info__9a6c40d2ef34a474_field_295, &__struct_info__9a6c40d2ef34a474_field_296, &__struct_info__9a6c40d2ef34a474_field_297, &__struct_info__9a6c40d2ef34a474_field_298, &__struct_info__9a6c40d2ef34a474_field_299, &__struct_info__9a6c40d2ef34a474_field_300, &__struct_info__9a6c40d2ef34a474_field_301, &__struct_info__9a6c40d2ef34a474_field_302, &__struct_info__9a6c40d2ef34a474_field_303, &__struct_info__9a6c40d2ef34a474_field_304, &__struct_info__9a6c40d2ef34a474_field_305, &__struct_info__9a6c40d2ef34a474_field_306, &__struct_info__9a6c40d2ef34a474_field_307, &__struct_info__9a6c40d2ef34a474_field_308, &__struct_info__9a6c40d2ef34a474_field_309, &__struct_info__9a6c40d2ef34a474_field_310, &__struct_info__9a6c40d2ef34a474_field_311 };
StructInfo __struct_info__9a6c40d2ef34a474 = {"LintVisitor", "lint", 29, __struct_info__9a6c40d2ef34a474_fields, 312, TypeSize<lint::LintVisitor>::size, UINT64_C(0x0), nullptr, UINT64_C(0x9a6c40d2ef34a474), 0 };
TypeInfo __type_info__3c61146b2bdfb90 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 290, TypeSize<das::string>::size, UINT64_C(0x3c61146b2bdfb90) };
TypeInfo __type_info__7c61f7ae88617bb2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__cd505ad3b1c59cc6, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<Enumeration *>::size, UINT64_C(0x7c61f7ae88617bb2) };
TypeInfo __type_info__c2f4bc15903e1610 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__7329fadda4ca251c, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprAddr>>::size, UINT64_C(0xc2f4bc15903e1610) };
TypeInfo __type_info__defdb920e82da0f4 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__3693bdfd1150bb56, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprArrayComprehension>>::size, UINT64_C(0xdefdb920e82da0f4) };
TypeInfo __type_info__229aabe2f8bef1d9 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__71ff6f045d2186f1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprAsVariant>>::size, UINT64_C(0x229aabe2f8bef1d9) };
TypeInfo __type_info__6bdd529063b3dbeb = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__acc5cdadba98f68e, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprAscend>>::size, UINT64_C(0x6bdd529063b3dbeb) };
TypeInfo __type_info__d551858bc6d43037 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__2055bdfdcee6bf5e, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprAssert>>::size, UINT64_C(0xd551858bc6d43037) };
TypeInfo __type_info__83c768ad9b3f81ea = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__fb56aefdaf9de951, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprAssume>>::size, UINT64_C(0x83c768ad9b3f81ea) };
TypeInfo __type_info__241df6ccda394202 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__cb4a7f89a13eab36, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprAt>>::size, UINT64_C(0x241df6ccda394202) };
TypeInfo __type_info__4dee28f2a93bbef7 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__71c84a7f531ca5bb, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprBlock>>::size, UINT64_C(0x4dee28f2a93bbef7) };
TypeInfo __type_info__5047b5dbcc2127cd = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__afd7e462d2caeebb, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprBreak>>::size, UINT64_C(0x5047b5dbcc2127cd) };
TypeInfo __type_info__7e04c1d12891d606 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__54fceee561bff5eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<ExprCall *>::size, UINT64_C(0x7e04c1d12891d606) };
TypeInfo __type_info__7dd1c1d1283b2d06 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__54fceee561bff5eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprCall>>::size, UINT64_C(0x7dd1c1d1283b2d06) };
TypeInfo __type_info__636dc1714c171367 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a0219258cb3926ee, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprCallMacro>>::size, UINT64_C(0x636dc1714c171367) };
TypeInfo __type_info__bc5b346893db35b = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__1afef6e5304b2283, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprCast>>::size, UINT64_C(0xbc5b346893db35b) };
TypeInfo __type_info__93546827b32c5422 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__898a3dd26b376c6a, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprClone>>::size, UINT64_C(0x93546827b32c5422) };
TypeInfo __type_info__118bfa23ce6c000c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__699f14ced40c8382, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConst>>::size, UINT64_C(0x118bfa23ce6c000c) };
TypeInfo __type_info__9c92a72bb3a64bfa = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ad18450df661455f, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstBitfield>>::size, UINT64_C(0x9c92a72bb3a64bfa) };
TypeInfo __type_info__ea252439573ea197 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__91bbd69210f68e07, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstBool>>::size, UINT64_C(0xea252439573ea197) };
TypeInfo __type_info__7f81cc8503986a86 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__3ee228fe47602659, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstDouble>>::size, UINT64_C(0x7f81cc8503986a86) };
TypeInfo __type_info__b6d18d4b3fadccd4 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__365a0d74b6e3ae27, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstEnumeration>>::size, UINT64_C(0xb6d18d4b3fadccd4) };
TypeInfo __type_info__4f4cc10892c6c61 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__72bac02d9b0c1dd, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstFloat2>>::size, UINT64_C(0x4f4cc10892c6c61) };
TypeInfo __type_info__65f51082d9833a = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__3c5ac02d6cd98dd, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstFloat3>>::size, UINT64_C(0x65f51082d9833a) };
TypeInfo __type_info__9089610918ba11f = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__f2c7ac02c85dcbdd, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstFloat4>>::size, UINT64_C(0x9089610918ba11f) };
TypeInfo __type_info__50c7808637778d65 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__bfbf448dd60c6211, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstFloat>>::size, UINT64_C(0x50c7808637778d65) };
TypeInfo __type_info__87ae85b131d91f57 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__9307dd967ffe2b49, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstInt16>>::size, UINT64_C(0x87ae85b131d91f57) };
TypeInfo __type_info__2d27aed7dd587c30 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8903e59677a2e7e1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstInt2>>::size, UINT64_C(0x2d27aed7dd587c30) };
TypeInfo __type_info__31b7a5d7e3ad4eb7 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8c69e5967a8610e1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstInt3>>::size, UINT64_C(0x31b7a5d7e3ad4eb7) };
TypeInfo __type_info__2914e4d7d4fafa72 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8237e59671dc95e1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstInt4>>::size, UINT64_C(0x2914e4d7d4fafa72) };
TypeInfo __type_info__773524bb75b61932 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__7b85db966c4260e3, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstInt64>>::size, UINT64_C(0x773524bb75b61932) };
TypeInfo __type_info__4a0758d80e688a0e = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__aaffe596948281e1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstInt8>>::size, UINT64_C(0x4a0758d80e688a0e) };
TypeInfo __type_info__a28688c7ffe035ce = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__108c5371ed782a25, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstInt>>::size, UINT64_C(0xa28688c7ffe035ce) };
TypeInfo __type_info__a45346c81e6a9b80 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ef0e5b71d12c4f0e, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstPtr>>::size, UINT64_C(0xa45346c81e6a9b80) };
TypeInfo __type_info__4f02b717be42f032 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__6dfe2527715392d1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstRange64>>::size, UINT64_C(0x4f02b717be42f032) };
TypeInfo __type_info__214ca0a8404236ce = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__44e10b9c0b7ea95f, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstRange>>::size, UINT64_C(0x214ca0a8404236ce) };
TypeInfo __type_info__e9cb7c9300717d9e = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__81d4b6e4402ada81, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstString>>::size, UINT64_C(0xe9cb7c9300717d9e) };
TypeInfo __type_info__f108ab47d962e793 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__32209cf3725705b0, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstUInt16>>::size, UINT64_C(0xf108ab47d962e793) };
TypeInfo __type_info__514742689af99de7 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__50a064d75d4cc1fb, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstUInt2>>::size, UINT64_C(0x514742689af99de7) };
TypeInfo __type_info__563e42636db280e7 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__50a063d75d4cc048, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstUInt3>>::size, UINT64_C(0x563e42636db280e7) };
TypeInfo __type_info__a17d42718ebadfe7 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__50a06ad75d4ccc2d, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstUInt4>>::size, UINT64_C(0xa17d42718ebadfe7) };
TypeInfo __type_info__63e1b8a29ad93469 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__2b5f1ef36c99e51b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstUInt64>>::size, UINT64_C(0x63e1b8a29ad93469) };
TypeInfo __type_info__f6614284ea50cbe7 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__50a05ed75d4cb7c9, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstUInt8>>::size, UINT64_C(0xf6614284ea50cbe7) };
TypeInfo __type_info__518dd4a2ad91defd = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__50ca60d75d94192f, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstUInt>>::size, UINT64_C(0x518dd4a2ad91defd) };
TypeInfo __type_info__b8524aede8fd2575 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ee05ad47ac112e5f, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstURange64>>::size, UINT64_C(0xb8524aede8fd2575) };
TypeInfo __type_info__d2fee6b26665c989 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a1fe7a142c668903, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprConstURange>>::size, UINT64_C(0xd2fee6b26665c989) };
TypeInfo __type_info__5b91ede0508873e = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__37bd8d7fdf3c5374, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprContinue>>::size, UINT64_C(0x5b91ede0508873e) };
TypeInfo __type_info__350b375c34e0c48a = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__10fefde527f0e316, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprCopy>>::size, UINT64_C(0x350b375c34e0c48a) };
TypeInfo __type_info__c84cf5ded2cd1cd8 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8ea2bb6c84fe54ae, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprDebug>>::size, UINT64_C(0xc84cf5ded2cd1cd8) };
TypeInfo __type_info__120723ecb6510065 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__643b022638807dc3, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprDelete>>::size, UINT64_C(0x120723ecb6510065) };
TypeInfo __type_info__e672712e93e236ba = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e166b9c4a79e779, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprErase>>::size, UINT64_C(0xe672712e93e236ba) };
TypeInfo __type_info__3229d47464f4ad50 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a3d5bceeff53f155, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprFakeContext>>::size, UINT64_C(0x3229d47464f4ad50) };
TypeInfo __type_info__511818eae83f8137 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__f85f434a5cfa7cf9, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprFakeLineInfo>>::size, UINT64_C(0x511818eae83f8137) };
TypeInfo __type_info__849126a4e3db3268 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__c1ab66e04afa3a7, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprField>>::size, UINT64_C(0x849126a4e3db3268) };
TypeInfo __type_info__ee83d76e6f9a3c81 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b8cb16fdfafa869b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprFind>>::size, UINT64_C(0xee83d76e6f9a3c81) };
TypeInfo __type_info__9745884abdafbe87 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__598840fdaa05c3ef, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprFor>>::size, UINT64_C(0x9745884abdafbe87) };
TypeInfo __type_info__41023c185ec41d2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__aa2eff9e8711b4c, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprGoto>>::size, UINT64_C(0x41023c185ec41d2) };
TypeInfo __type_info__88db72c3eb8c93b1 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__833e12e4dcd8153d, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprIfThenElse>>::size, UINT64_C(0x88db72c3eb8c93b1) };
TypeInfo __type_info__c3c8c780df6c5865 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__f1f05ee81890b310, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprInvoke>>::size, UINT64_C(0xc3c8c780df6c5865) };
TypeInfo __type_info__8a5e2edb26418a2a = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__afca8289899d784f, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprIs>>::size, UINT64_C(0x8a5e2edb26418a2a) };
TypeInfo __type_info__837624c70f8f1fa1 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__725600cc59f9ef1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprIsVariant>>::size, UINT64_C(0x837624c70f8f1fa1) };
TypeInfo __type_info__7f9fc2c601e28df1 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__5a876ec502d05cd5, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprKeyExists>>::size, UINT64_C(0x7f9fc2c601e28df1) };
TypeInfo __type_info__6ad276912e16c445 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a8d3190cd853597a, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprLabel>>::size, UINT64_C(0x6ad276912e16c445) };
TypeInfo __type_info__349161eed600549f = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b3d9c0cc943b4165, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprLet>>::size, UINT64_C(0x349161eed600549f) };
TypeInfo __type_info__8faf3ae8c5ebe47a = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__d07c067a5c7b8ff4, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprLooksLikeCall>>::size, UINT64_C(0x8faf3ae8c5ebe47a) };
TypeInfo __type_info__f44650fbe99befd9 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__7260bd93a15a7ff1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprMakeArray>>::size, UINT64_C(0xf44650fbe99befd9) };
TypeInfo __type_info__e0b574ceb6c8c70f = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e9813cd85e320ce1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprMakeBlock>>::size, UINT64_C(0xe0b574ceb6c8c70f) };
TypeInfo __type_info__6c1a6b092c78a88 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__644a49dea6863e78, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprMakeGenerator>>::size, UINT64_C(0x6c1a6b092c78a88) };
TypeInfo __type_info__5276a743108434eb = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b6c344d07fc80acd, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprMakeStruct>>::size, UINT64_C(0x5276a743108434eb) };
TypeInfo __type_info__ea03eef331aabf4 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__94e15ebe6d2ac6c5, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprMakeTuple>>::size, UINT64_C(0xea03eef331aabf4) };
TypeInfo __type_info__4191dbf23146a87e = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__749bdb083606521a, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprMakeVariant>>::size, UINT64_C(0x4191dbf23146a87e) };
TypeInfo __type_info__f66cc598ea369f61 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__bbedea2da76c1cbd, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprMemZero>>::size, UINT64_C(0xf66cc598ea369f61) };
TypeInfo __type_info__c19751d6d5da74e2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__6508f9c8d2b82c4a, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprMove>>::size, UINT64_C(0xc19751d6d5da74e2) };
TypeInfo __type_info__44cd26f4cb3df7e1 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__c88d1b35f2ffa823, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprNamedCall>>::size, UINT64_C(0x44cd26f4cb3df7e1) };
TypeInfo __type_info__5d6138f13e1e88c4 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__f14dc0d72b72a465, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprNew>>::size, UINT64_C(0x5d6138f13e1e88c4) };
TypeInfo __type_info__e7e2063b91ac55a1 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e4bc23ea0da25d79, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprNullCoalescing>>::size, UINT64_C(0xe7e2063b91ac55a1) };
TypeInfo __type_info__c7c0e4fba3dcbfcf = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b4d3bed2a010acf4, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprOp1>>::size, UINT64_C(0xc7c0e4fba3dcbfcf) };
TypeInfo __type_info__c5915ffba474f7fe = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b839bed2a2f3d5f4, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprOp2>>::size, UINT64_C(0xc5915ffba474f7fe) };
TypeInfo __type_info__ca2136fbaac99425 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__bb9fbed2a5d6fef4, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprOp3>>::size, UINT64_C(0xca2136fbaac99425) };
TypeInfo __type_info__7d9fd489616ae8d = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b0f72e776d005eaf, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprPtr2Ref>>::size, UINT64_C(0x7d9fd489616ae8d) };
TypeInfo __type_info__9dfe8a83730428c8 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__29261b9b611e6f1b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprQuote>>::size, UINT64_C(0x9dfe8a83730428c8) };
TypeInfo __type_info__6628bcbce7db6a7d = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__64b15f9df38db54f, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprReader>>::size, UINT64_C(0x6628bcbce7db6a7d) };
TypeInfo __type_info__1151bc4127672205 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__d27a1f910d191ab3, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprRef2Ptr>>::size, UINT64_C(0x1151bc4127672205) };
TypeInfo __type_info__bba83b75d4855b7e = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__6dc5617548466ef3, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprRef2Value>>::size, UINT64_C(0xbba83b75d4855b7e) };
TypeInfo __type_info__1a5b7f11cf3fb5b5 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__15a45142a97c9b3e, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprReturn>>::size, UINT64_C(0x1a5b7f11cf3fb5b5) };
TypeInfo __type_info__45d77ccae958b9de = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__cfc6c6515483a76b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprSafeAsVariant>>::size, UINT64_C(0x45d77ccae958b9de) };
TypeInfo __type_info__b5e62a55ec68b6b5 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a87ef47d40240d3c, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprSafeAt>>::size, UINT64_C(0xb5e62a55ec68b6b5) };
TypeInfo __type_info__a7adf4b0a367d897 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__10ddfd98f14d71c1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprSafeField>>::size, UINT64_C(0xa7adf4b0a367d897) };
TypeInfo __type_info__9c37565e66334661 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__4396458b6cca487d, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprSetInsert>>::size, UINT64_C(0x9c37565e66334661) };
TypeInfo __type_info__74372feec5a81686 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__7a94f4cc4bcf20e5, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprStaticAssert>>::size, UINT64_C(0x74372feec5a81686) };
TypeInfo __type_info__4e7dff8bb14f539 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__278bc6d46dadffa8, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprStringBuilder>>::size, UINT64_C(0x4e7dff8bb14f539) };
TypeInfo __type_info__6bb94e24ea14ce9a = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__da0e82cafc1e70b1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprSwizzle>>::size, UINT64_C(0x6bb94e24ea14ce9a) };
TypeInfo __type_info__631c9e15ba7d5036 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__7b55c0a63e321fc1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprTag>>::size, UINT64_C(0x631c9e15ba7d5036) };
TypeInfo __type_info__c1c6f9bc0741f232 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__5e2809979d5f78c9, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprTryCatch>>::size, UINT64_C(0xc1c6f9bc0741f232) };
TypeInfo __type_info__3b037c8d587730b0 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__60a144dd7cf1ba91, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprTypeDecl>>::size, UINT64_C(0x3b037c8d587730b0) };
TypeInfo __type_info__563543a880fdcea2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__c15e41a8ee5ebf97, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprTypeInfo>>::size, UINT64_C(0x563543a880fdcea2) };
TypeInfo __type_info__eb22258b16c8c6df = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a88454b76bb549ba, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprUnsafe>>::size, UINT64_C(0xeb22258b16c8c6df) };
TypeInfo __type_info__f5c1d1c41d788f7 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__811c0b03f452ec1, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprVar>>::size, UINT64_C(0xf5c1d1c41d788f7) };
TypeInfo __type_info__9cac32b4050a2fb8 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__51f018132be6c64, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprWhile>>::size, UINT64_C(0x9cac32b4050a2fb8) };
TypeInfo __type_info__c758d466d1a06ae2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b99012ac7e42bacf, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprWith>>::size, UINT64_C(0xc758d466d1a06ae2) };
TypeInfo __type_info__60501e84f49c29e1 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8873b51c25d24aa7, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<ExprYield>>::size, UINT64_C(0x60501e84f49c29e1) };
TypeInfo __type_info__2dd484863625d80 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__960dd6428887a234, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<Expression>>::size, UINT64_C(0x2dd484863625d80) };
TypeInfo __type_info__34b7c04894c15d5 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<Function *>::size, UINT64_C(0x34b7c04894c15d5) };
TypeInfo __type_info__f9220d94c6b964b5 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__60d16a2d23420951, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<Structure *>::size, UINT64_C(0xf9220d94c6b964b5) };
TypeInfo __type_info__79c6e4b278757551 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ccd32e474e9a33eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<Variable *>::size, UINT64_C(0x79c6e4b278757551) };
TypeInfo __type_info__e266b5ccef058802 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8afce1a80940fc9e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, TypeSize<Module *>::size, UINT64_C(0xe266b5ccef058802) };
TypeInfo __type_info__624d371c76b25aa4 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 34, TypeSize<das::string>::size, UINT64_C(0x624d371c76b25aa4) };
TypeInfo __type_info__29c0090cdbf7525c = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::FieldDeclaration"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24610, TypeSize<Structure::FieldDeclaration>::size, UINT64_C(0x29c0090cdbf7525c) };
TypeInfo __type_info__246dda13a8a4b104 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::LineInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8230, TypeSize<LineInfo>::size, UINT64_C(0x246dda13a8a4b104) };
TypeInfo __type_info__586f0da79a6e613d = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__cd505ad3b1c59cc6, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<Enumeration>>::size, UINT64_C(0x586f0da79a6e613d) };
TypeInfo __type_info__98064c57b4bcca5a = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__960dd6428887a234, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<Expression>>::size, UINT64_C(0x98064c57b4bcca5a) };
TypeInfo __type_info__9d10785eb07580e0 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<Function>>::size, UINT64_C(0x9d10785eb07580e0) };
TypeInfo __type_info__9a5e492166d49949 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__acd33335f9c1e498, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<MakeFieldDecl>>::size, UINT64_C(0x9a5e492166d49949) };
TypeInfo __type_info__e57b0f261f47b890 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__125855d9cd771ead, nullptr, nullptr, nullptr, 0, 0, nullptr, 9248, TypeSize<smart_ptr_raw<Program>>::size, UINT64_C(0xe57b0f261f47b890) };
TypeInfo __type_info__d6b8ed05d16e9f27 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__60d16a2d23420951, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<Structure>>::size, UINT64_C(0xd6b8ed05d16e9f27) };
TypeInfo __type_info__defb2f7795e0cf8c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ce241e3005cc873b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<TypeDecl>>::size, UINT64_C(0xdefb2f7795e0cf8c) };
const char * __type_info__6c2b5208df908cfa_arg_names[6] = { "access_extern", "access_get", "access_ref", "access_init", "access_pass", "access_fold" };
TypeInfo __type_info__6c2b5208df908cfa = { Type::tBitfield, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, __type_info__6c2b5208df908cfa_arg_names, 6, 0, nullptr, 60, TypeSize<Bitfield>::size, UINT64_C(0x6c2b5208df908cfa) };
TypeInfo __type_info__a3a6bcfebaf8fcd8 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ccd32e474e9a33eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, TypeSize<smart_ptr_raw<Variable>>::size, UINT64_C(0xa3a6bcfebaf8fcd8) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<bool>::size, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int32_t>::size, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__cd505ad3b1c59cc6 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Enumeration"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<Enumeration>::size, UINT64_C(0xcd505ad3b1c59cc6) };
TypeInfo __type_info__7329fadda4ca251c = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprAddr"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprAddr>::size, UINT64_C(0x7329fadda4ca251c) };
TypeInfo __type_info__3693bdfd1150bb56 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprArrayComprehension"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprArrayComprehension>::size, UINT64_C(0x3693bdfd1150bb56) };
TypeInfo __type_info__71ff6f045d2186f1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprAsVariant"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprAsVariant>::size, UINT64_C(0x71ff6f045d2186f1) };
TypeInfo __type_info__acc5cdadba98f68e = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprAscend"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprAscend>::size, UINT64_C(0xacc5cdadba98f68e) };
TypeInfo __type_info__2055bdfdcee6bf5e = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprAssert"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprAssert>::size, UINT64_C(0x2055bdfdcee6bf5e) };
TypeInfo __type_info__fb56aefdaf9de951 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprAssume"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprAssume>::size, UINT64_C(0xfb56aefdaf9de951) };
TypeInfo __type_info__cb4a7f89a13eab36 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprAt"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprAt>::size, UINT64_C(0xcb4a7f89a13eab36) };
TypeInfo __type_info__71c84a7f531ca5bb = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprBlock"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprBlock>::size, UINT64_C(0x71c84a7f531ca5bb) };
TypeInfo __type_info__afd7e462d2caeebb = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprBreak"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprBreak>::size, UINT64_C(0xafd7e462d2caeebb) };
TypeInfo __type_info__54fceee561bff5eb = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprCall"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprCall>::size, UINT64_C(0x54fceee561bff5eb) };
TypeInfo __type_info__a0219258cb3926ee = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprCallMacro"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprCallMacro>::size, UINT64_C(0xa0219258cb3926ee) };
TypeInfo __type_info__1afef6e5304b2283 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprCast"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprCast>::size, UINT64_C(0x1afef6e5304b2283) };
TypeInfo __type_info__898a3dd26b376c6a = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprClone"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprClone>::size, UINT64_C(0x898a3dd26b376c6a) };
TypeInfo __type_info__699f14ced40c8382 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConst"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConst>::size, UINT64_C(0x699f14ced40c8382) };
TypeInfo __type_info__ad18450df661455f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstBitfield"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstBitfield>::size, UINT64_C(0xad18450df661455f) };
TypeInfo __type_info__91bbd69210f68e07 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstBool"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstBool>::size, UINT64_C(0x91bbd69210f68e07) };
TypeInfo __type_info__3ee228fe47602659 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstDouble"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstDouble>::size, UINT64_C(0x3ee228fe47602659) };
TypeInfo __type_info__365a0d74b6e3ae27 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstEnumeration"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstEnumeration>::size, UINT64_C(0x365a0d74b6e3ae27) };
TypeInfo __type_info__72bac02d9b0c1dd = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstFloat2"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstFloat2>::size, UINT64_C(0x72bac02d9b0c1dd) };
TypeInfo __type_info__3c5ac02d6cd98dd = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstFloat3"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstFloat3>::size, UINT64_C(0x3c5ac02d6cd98dd) };
TypeInfo __type_info__f2c7ac02c85dcbdd = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstFloat4"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstFloat4>::size, UINT64_C(0xf2c7ac02c85dcbdd) };
TypeInfo __type_info__bfbf448dd60c6211 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstFloat"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstFloat>::size, UINT64_C(0xbfbf448dd60c6211) };
TypeInfo __type_info__9307dd967ffe2b49 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstInt16"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstInt16>::size, UINT64_C(0x9307dd967ffe2b49) };
TypeInfo __type_info__8903e59677a2e7e1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstInt2"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstInt2>::size, UINT64_C(0x8903e59677a2e7e1) };
TypeInfo __type_info__8c69e5967a8610e1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstInt3"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstInt3>::size, UINT64_C(0x8c69e5967a8610e1) };
TypeInfo __type_info__8237e59671dc95e1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstInt4"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstInt4>::size, UINT64_C(0x8237e59671dc95e1) };
TypeInfo __type_info__7b85db966c4260e3 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstInt64"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstInt64>::size, UINT64_C(0x7b85db966c4260e3) };
TypeInfo __type_info__aaffe596948281e1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstInt8"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstInt8>::size, UINT64_C(0xaaffe596948281e1) };
TypeInfo __type_info__108c5371ed782a25 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstInt"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstInt>::size, UINT64_C(0x108c5371ed782a25) };
TypeInfo __type_info__ef0e5b71d12c4f0e = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstPtr"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstPtr>::size, UINT64_C(0xef0e5b71d12c4f0e) };
TypeInfo __type_info__6dfe2527715392d1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstRange64"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstRange64>::size, UINT64_C(0x6dfe2527715392d1) };
TypeInfo __type_info__44e10b9c0b7ea95f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstRange"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstRange>::size, UINT64_C(0x44e10b9c0b7ea95f) };
TypeInfo __type_info__81d4b6e4402ada81 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstString"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstString>::size, UINT64_C(0x81d4b6e4402ada81) };
TypeInfo __type_info__32209cf3725705b0 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstUInt16"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstUInt16>::size, UINT64_C(0x32209cf3725705b0) };
TypeInfo __type_info__50a064d75d4cc1fb = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstUInt2"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstUInt2>::size, UINT64_C(0x50a064d75d4cc1fb) };
TypeInfo __type_info__50a063d75d4cc048 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstUInt3"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstUInt3>::size, UINT64_C(0x50a063d75d4cc048) };
TypeInfo __type_info__50a06ad75d4ccc2d = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstUInt4"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstUInt4>::size, UINT64_C(0x50a06ad75d4ccc2d) };
TypeInfo __type_info__2b5f1ef36c99e51b = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstUInt64"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstUInt64>::size, UINT64_C(0x2b5f1ef36c99e51b) };
TypeInfo __type_info__50a05ed75d4cb7c9 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstUInt8"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstUInt8>::size, UINT64_C(0x50a05ed75d4cb7c9) };
TypeInfo __type_info__50ca60d75d94192f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstUInt"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstUInt>::size, UINT64_C(0x50ca60d75d94192f) };
TypeInfo __type_info__ee05ad47ac112e5f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstURange64"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstURange64>::size, UINT64_C(0xee05ad47ac112e5f) };
TypeInfo __type_info__a1fe7a142c668903 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprConstURange"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprConstURange>::size, UINT64_C(0xa1fe7a142c668903) };
TypeInfo __type_info__37bd8d7fdf3c5374 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprContinue"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprContinue>::size, UINT64_C(0x37bd8d7fdf3c5374) };
TypeInfo __type_info__10fefde527f0e316 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprCopy"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprCopy>::size, UINT64_C(0x10fefde527f0e316) };
TypeInfo __type_info__8ea2bb6c84fe54ae = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprDebug"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprDebug>::size, UINT64_C(0x8ea2bb6c84fe54ae) };
TypeInfo __type_info__643b022638807dc3 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprDelete"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprDelete>::size, UINT64_C(0x643b022638807dc3) };
TypeInfo __type_info__e166b9c4a79e779 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprErase"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprErase>::size, UINT64_C(0xe166b9c4a79e779) };
TypeInfo __type_info__a3d5bceeff53f155 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprFakeContext"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprFakeContext>::size, UINT64_C(0xa3d5bceeff53f155) };
TypeInfo __type_info__f85f434a5cfa7cf9 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprFakeLineInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprFakeLineInfo>::size, UINT64_C(0xf85f434a5cfa7cf9) };
TypeInfo __type_info__c1ab66e04afa3a7 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprField"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprField>::size, UINT64_C(0xc1ab66e04afa3a7) };
TypeInfo __type_info__b8cb16fdfafa869b = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprFind"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprFind>::size, UINT64_C(0xb8cb16fdfafa869b) };
TypeInfo __type_info__598840fdaa05c3ef = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprFor"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprFor>::size, UINT64_C(0x598840fdaa05c3ef) };
TypeInfo __type_info__aa2eff9e8711b4c = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprGoto"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprGoto>::size, UINT64_C(0xaa2eff9e8711b4c) };
TypeInfo __type_info__833e12e4dcd8153d = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprIfThenElse"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprIfThenElse>::size, UINT64_C(0x833e12e4dcd8153d) };
TypeInfo __type_info__f1f05ee81890b310 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprInvoke"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprInvoke>::size, UINT64_C(0xf1f05ee81890b310) };
TypeInfo __type_info__afca8289899d784f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprIs"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprIs>::size, UINT64_C(0xafca8289899d784f) };
TypeInfo __type_info__725600cc59f9ef1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprIsVariant"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprIsVariant>::size, UINT64_C(0x725600cc59f9ef1) };
TypeInfo __type_info__5a876ec502d05cd5 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprKeyExists"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprKeyExists>::size, UINT64_C(0x5a876ec502d05cd5) };
TypeInfo __type_info__a8d3190cd853597a = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprLabel"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprLabel>::size, UINT64_C(0xa8d3190cd853597a) };
TypeInfo __type_info__b3d9c0cc943b4165 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprLet"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprLet>::size, UINT64_C(0xb3d9c0cc943b4165) };
TypeInfo __type_info__d07c067a5c7b8ff4 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprLooksLikeCall"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprLooksLikeCall>::size, UINT64_C(0xd07c067a5c7b8ff4) };
TypeInfo __type_info__7260bd93a15a7ff1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprMakeArray"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprMakeArray>::size, UINT64_C(0x7260bd93a15a7ff1) };
TypeInfo __type_info__e9813cd85e320ce1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprMakeBlock"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprMakeBlock>::size, UINT64_C(0xe9813cd85e320ce1) };
TypeInfo __type_info__644a49dea6863e78 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprMakeGenerator"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprMakeGenerator>::size, UINT64_C(0x644a49dea6863e78) };
TypeInfo __type_info__b6c344d07fc80acd = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprMakeStruct"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprMakeStruct>::size, UINT64_C(0xb6c344d07fc80acd) };
TypeInfo __type_info__94e15ebe6d2ac6c5 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprMakeTuple"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprMakeTuple>::size, UINT64_C(0x94e15ebe6d2ac6c5) };
TypeInfo __type_info__749bdb083606521a = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprMakeVariant"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprMakeVariant>::size, UINT64_C(0x749bdb083606521a) };
TypeInfo __type_info__bbedea2da76c1cbd = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprMemZero"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprMemZero>::size, UINT64_C(0xbbedea2da76c1cbd) };
TypeInfo __type_info__6508f9c8d2b82c4a = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprMove"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprMove>::size, UINT64_C(0x6508f9c8d2b82c4a) };
TypeInfo __type_info__c88d1b35f2ffa823 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprNamedCall"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprNamedCall>::size, UINT64_C(0xc88d1b35f2ffa823) };
TypeInfo __type_info__f14dc0d72b72a465 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprNew"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprNew>::size, UINT64_C(0xf14dc0d72b72a465) };
TypeInfo __type_info__e4bc23ea0da25d79 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprNullCoalescing"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprNullCoalescing>::size, UINT64_C(0xe4bc23ea0da25d79) };
TypeInfo __type_info__b4d3bed2a010acf4 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprOp1"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprOp1>::size, UINT64_C(0xb4d3bed2a010acf4) };
TypeInfo __type_info__b839bed2a2f3d5f4 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprOp2"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprOp2>::size, UINT64_C(0xb839bed2a2f3d5f4) };
TypeInfo __type_info__bb9fbed2a5d6fef4 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprOp3"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprOp3>::size, UINT64_C(0xbb9fbed2a5d6fef4) };
TypeInfo __type_info__b0f72e776d005eaf = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprPtr2Ref"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprPtr2Ref>::size, UINT64_C(0xb0f72e776d005eaf) };
TypeInfo __type_info__29261b9b611e6f1b = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprQuote"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprQuote>::size, UINT64_C(0x29261b9b611e6f1b) };
TypeInfo __type_info__64b15f9df38db54f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprReader"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprReader>::size, UINT64_C(0x64b15f9df38db54f) };
TypeInfo __type_info__d27a1f910d191ab3 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprRef2Ptr"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprRef2Ptr>::size, UINT64_C(0xd27a1f910d191ab3) };
TypeInfo __type_info__6dc5617548466ef3 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprRef2Value"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprRef2Value>::size, UINT64_C(0x6dc5617548466ef3) };
TypeInfo __type_info__15a45142a97c9b3e = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprReturn"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprReturn>::size, UINT64_C(0x15a45142a97c9b3e) };
TypeInfo __type_info__cfc6c6515483a76b = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprSafeAsVariant"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprSafeAsVariant>::size, UINT64_C(0xcfc6c6515483a76b) };
TypeInfo __type_info__a87ef47d40240d3c = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprSafeAt"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprSafeAt>::size, UINT64_C(0xa87ef47d40240d3c) };
TypeInfo __type_info__10ddfd98f14d71c1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprSafeField"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprSafeField>::size, UINT64_C(0x10ddfd98f14d71c1) };
TypeInfo __type_info__4396458b6cca487d = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprSetInsert"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprSetInsert>::size, UINT64_C(0x4396458b6cca487d) };
TypeInfo __type_info__7a94f4cc4bcf20e5 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprStaticAssert"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprStaticAssert>::size, UINT64_C(0x7a94f4cc4bcf20e5) };
TypeInfo __type_info__278bc6d46dadffa8 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprStringBuilder"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprStringBuilder>::size, UINT64_C(0x278bc6d46dadffa8) };
TypeInfo __type_info__da0e82cafc1e70b1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprSwizzle"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprSwizzle>::size, UINT64_C(0xda0e82cafc1e70b1) };
TypeInfo __type_info__7b55c0a63e321fc1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprTag"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprTag>::size, UINT64_C(0x7b55c0a63e321fc1) };
TypeInfo __type_info__5e2809979d5f78c9 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprTryCatch"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprTryCatch>::size, UINT64_C(0x5e2809979d5f78c9) };
TypeInfo __type_info__60a144dd7cf1ba91 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprTypeDecl"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprTypeDecl>::size, UINT64_C(0x60a144dd7cf1ba91) };
TypeInfo __type_info__c15e41a8ee5ebf97 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprTypeInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprTypeInfo>::size, UINT64_C(0xc15e41a8ee5ebf97) };
TypeInfo __type_info__a88454b76bb549ba = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprUnsafe"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprUnsafe>::size, UINT64_C(0xa88454b76bb549ba) };
TypeInfo __type_info__811c0b03f452ec1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprVar"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprVar>::size, UINT64_C(0x811c0b03f452ec1) };
TypeInfo __type_info__51f018132be6c64 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprWhile"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprWhile>::size, UINT64_C(0x51f018132be6c64) };
TypeInfo __type_info__b99012ac7e42bacf = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprWith"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprWith>::size, UINT64_C(0xb99012ac7e42bacf) };
TypeInfo __type_info__8873b51c25d24aa7 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::ExprYield"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<ExprYield>::size, UINT64_C(0x8873b51c25d24aa7) };
TypeInfo __type_info__960dd6428887a234 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Expression"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<Expression>::size, UINT64_C(0x960dd6428887a234) };
TypeInfo __type_info__a57bf935c2dd03 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Function"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<Function>::size, UINT64_C(0xa57bf935c2dd03) };
TypeInfo __type_info__acd33335f9c1e498 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::MakeFieldDecl"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<MakeFieldDecl>::size, UINT64_C(0xacd33335f9c1e498) };
TypeInfo __type_info__60d16a2d23420951 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Structure"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<Structure>::size, UINT64_C(0x60d16a2d23420951) };
TypeInfo __type_info__ce241e3005cc873b = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::TypeDecl"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<TypeDecl>::size, UINT64_C(0xce241e3005cc873b) };
TypeInfo __type_info__ccd32e474e9a33eb = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Variable"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<Variable>::size, UINT64_C(0xccd32e474e9a33eb) };
TypeInfo __type_info__4200353d82fda873 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::VisitorAdapter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<VisitorAdapter>::size, UINT64_C(0x4200353d82fda873) };
TypeInfo __type_info__8afce1a80940fc9e = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Module"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<Module>::size, UINT64_C(0x8afce1a80940fc9e) };
TypeInfo __type_info__125855d9cd771ead = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Program"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8194, TypeSize<Program>::size, UINT64_C(0x125855d9cd771ead) };
TypeInfo __type_info__21586ce84f433a21 = { Type::tStructure, &__struct_info__1e8db4ddc1444e12, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, TypeSize<ast::AstVisitor>::size, UINT64_C(0x21586ce84f433a21) };
TypeInfo __type_info__f5340b6734d7b2c1 = { Type::tStructure, &__struct_info__9a6c40d2ef34a474, nullptr, nullptr, &__type_info__21586ce84f433a21, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<lint::LintVisitor>::size, UINT64_C(0xf5340b6734d7b2c1) };
TypeInfo __type_info__e4765bc563f255e = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__cd505ad3b1c59cc6, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<Enumeration>>::size, UINT64_C(0xe4765bc563f255e) };
TypeInfo __type_info__6636442e03391ebf = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__960dd6428887a234, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<Expression>>::size, UINT64_C(0x6636442e03391ebf) };
TypeInfo __type_info__4cdbed951d30a5d1 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<Function>>::size, UINT64_C(0x4cdbed951d30a5d1) };
TypeInfo __type_info__c52835f1e7c9ab84 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__acd33335f9c1e498, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<MakeFieldDecl>>::size, UINT64_C(0xc52835f1e7c9ab84) };
TypeInfo __type_info__7e104fcf0cd430e4 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__60d16a2d23420951, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<Structure>>::size, UINT64_C(0x7e104fcf0cd430e4) };
TypeInfo __type_info__afcf203e0d7d50d = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ce241e3005cc873b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<TypeDecl>>::size, UINT64_C(0xafcf203e0d7d50d) };
TypeInfo __type_info__4d5fdda373bcfbd1 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ccd32e474e9a33eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, TypeSize<smart_ptr_raw<Variable>>::size, UINT64_C(0x4d5fdda373bcfbd1) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__d9307e078cfb0f0c = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0xd9307e078cfb0f0c) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<void>::size, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__3c61146b2bdfb90, __type_info__624d371c76b25aa4, __type_info__29c0090cdbf7525c, __type_info__246dda13a8a4b104, __type_info__cd505ad3b1c59cc6, __type_info__7329fadda4ca251c, __type_info__3693bdfd1150bb56, __type_info__71ff6f045d2186f1, __type_info__acc5cdadba98f68e, __type_info__2055bdfdcee6bf5e, __type_info__fb56aefdaf9de951, __type_info__cb4a7f89a13eab36, __type_info__71c84a7f531ca5bb, __type_info__afd7e462d2caeebb, __type_info__54fceee561bff5eb, __type_info__a0219258cb3926ee, __type_info__1afef6e5304b2283, __type_info__898a3dd26b376c6a, __type_info__699f14ced40c8382, __type_info__ad18450df661455f, __type_info__91bbd69210f68e07, __type_info__3ee228fe47602659, __type_info__365a0d74b6e3ae27, __type_info__72bac02d9b0c1dd, __type_info__3c5ac02d6cd98dd, __type_info__f2c7ac02c85dcbdd, __type_info__bfbf448dd60c6211, __type_info__9307dd967ffe2b49, __type_info__8903e59677a2e7e1, __type_info__8c69e5967a8610e1, __type_info__8237e59671dc95e1, __type_info__7b85db966c4260e3, __type_info__aaffe596948281e1, __type_info__108c5371ed782a25, __type_info__ef0e5b71d12c4f0e, __type_info__6dfe2527715392d1, __type_info__44e10b9c0b7ea95f, __type_info__81d4b6e4402ada81, __type_info__32209cf3725705b0, __type_info__50a064d75d4cc1fb, __type_info__50a063d75d4cc048, __type_info__50a06ad75d4ccc2d, __type_info__2b5f1ef36c99e51b, __type_info__50a05ed75d4cb7c9, __type_info__50ca60d75d94192f, __type_info__ee05ad47ac112e5f, __type_info__a1fe7a142c668903, __type_info__37bd8d7fdf3c5374, __type_info__10fefde527f0e316, __type_info__8ea2bb6c84fe54ae, __type_info__643b022638807dc3, __type_info__e166b9c4a79e779, __type_info__a3d5bceeff53f155, __type_info__f85f434a5cfa7cf9, __type_info__c1ab66e04afa3a7, __type_info__b8cb16fdfafa869b, __type_info__598840fdaa05c3ef, __type_info__aa2eff9e8711b4c, __type_info__833e12e4dcd8153d, __type_info__f1f05ee81890b310, __type_info__afca8289899d784f, __type_info__725600cc59f9ef1, __type_info__5a876ec502d05cd5, __type_info__a8d3190cd853597a, __type_info__b3d9c0cc943b4165, __type_info__d07c067a5c7b8ff4, __type_info__7260bd93a15a7ff1, __type_info__e9813cd85e320ce1, __type_info__644a49dea6863e78, __type_info__b6c344d07fc80acd, __type_info__94e15ebe6d2ac6c5, __type_info__749bdb083606521a, __type_info__bbedea2da76c1cbd, __type_info__6508f9c8d2b82c4a, __type_info__c88d1b35f2ffa823, __type_info__f14dc0d72b72a465, __type_info__e4bc23ea0da25d79, __type_info__b4d3bed2a010acf4, __type_info__b839bed2a2f3d5f4, __type_info__bb9fbed2a5d6fef4, __type_info__b0f72e776d005eaf, __type_info__29261b9b611e6f1b, __type_info__64b15f9df38db54f, __type_info__d27a1f910d191ab3, __type_info__6dc5617548466ef3, __type_info__15a45142a97c9b3e, __type_info__cfc6c6515483a76b, __type_info__a87ef47d40240d3c, __type_info__10ddfd98f14d71c1, __type_info__4396458b6cca487d, __type_info__7a94f4cc4bcf20e5, __type_info__278bc6d46dadffa8, __type_info__da0e82cafc1e70b1, __type_info__7b55c0a63e321fc1, __type_info__5e2809979d5f78c9, __type_info__60a144dd7cf1ba91, __type_info__c15e41a8ee5ebf97, __type_info__a88454b76bb549ba, __type_info__811c0b03f452ec1, __type_info__51f018132be6c64, __type_info__b99012ac7e42bacf, __type_info__8873b51c25d24aa7, __type_info__960dd6428887a234, __type_info__a57bf935c2dd03, __type_info__acd33335f9c1e498, __type_info__60d16a2d23420951, __type_info__ce241e3005cc873b, __type_info__ccd32e474e9a33eb, __type_info__4200353d82fda873, __type_info__8afce1a80940fc9e, __type_info__125855d9cd771ead, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[3] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_2[5] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_3[6] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__6c2b5208df908cfa };
TypeInfo * __tinfo_4[5] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_5[5] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_6[5] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_7[5] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_8[5] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d47764d7b3c41f15 ( Context * __context__, lint::LintVisitor const  & __cl_rename_at_116_0 );
inline void _FuncbuiltinTickpushTick10769833213962245646_7759240296ddd78a ( Context * __context__, TArray<ast::AstPassMacro *> & __Arr_rename_at_181_1, ast::AstPassMacro * __value_rename_at_181_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1ac5c99ce289e45c ( Context * __context__, lint::LintEverything const  & __cl_rename_at_116_3 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_3ae1960cc30e1fd ( Context * __context__, TArray<uint64_t> & __Arr_rename_at_68_4, int32_t __newSize_rename_at_68_5 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_f43a93ccaab2641f ( Context * __context__, TArray<uint64_t> & __a_rename_at_1234_6 );
inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_7793ddd26dad1560 ( Context * __context__, lint::LintVisitor const  & __someClass_rename_at_684_7 );
inline void clone_3b9defae8930d957 ( Context * __context__, smart_ptr_raw<VisitorAdapter> & __dest_rename_at_154_10, void * const  __src_rename_at_154_11 );
inline void finalize_55065cb84d71e3b7 ( Context * __context__, lint::LintVisitor * & ____this_rename_at_156_12 );
inline void _FuncbuiltinTickpushTick14133213201864676143_ae2011e794a9fb57 ( Context * __context__, TArray<uint64_t> & __Arr_rename_at_165_14, uint64_t __value_rename_at_165_15 );
inline void _FuncbuiltinTickpopTick1161079256290593740_c6129cadfd6fb4a5 ( Context * __context__, TArray<uint64_t> & __Arr_rename_at_132_16 );
inline uint64_t & _FuncbuiltinTickbackTick18296309835877697278_ff0ac4a30478f080 ( Context * __context__, TArray<uint64_t> & __a_rename_at_473_17 );
inline uint64_t _FuncbuiltinTickintptrTick11320822648609276562_9eb7fbdafa7fb812 ( Context * __context__, void * const  __p_rename_at_15_19 );
inline Expression * _FuncbuiltinTickget_ptrTick5807679485210906136_b1f1fde051ec136c ( Context * __context__, smart_ptr_raw<Expression> __src_rename_at_1784_20 );
inline ExprReturn * _FuncbuiltinTickget_ptrTick8468476673553620226_a0a73c860ad64c8d ( Context * __context__, smart_ptr_raw<ExprReturn> const  __src_rename_at_1778_21 );
inline bool _Funcstrings_boostTickeqTick10467641742855300050_6ad0a56a732d869d ( Context * __context__, das::string const  & __b_rename_at_127_22, char * const  __a_rename_at_127_23 );
inline ExprCall * _FuncbuiltinTickget_ptrTick8468476673553620226_ff0a681b03d5e96e ( Context * __context__, smart_ptr_raw<ExprCall> const  __src_rename_at_1778_24 );
inline char * _FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_25, bool __extra_rename_at_38_26, bool __contracts_rename_at_38_27, bool __modules_rename_at_38_28 );
inline void finalize_89dec6ea776f58f4 ( Context * __context__, lint::LintVisitor & ____this_rename_at_21_29 );
inline lint::LintVisitor LintVisitor_c318ca4b22b1ae4d ( Context * __context__ );
inline void _FuncLintVisitorTicklint_error_4e5763a961a7aa74 ( Context * __context__, lint::LintVisitor & __self_rename_at_29_31, char * const  __text_rename_at_29_32, LineInfo const  & __at_rename_at_29_33 );
inline void _FuncLintVisitorTickpreVisitFunction_9639750a23b12f82 ( Context * __context__, lint::LintVisitor & __self_rename_at_39_34, smart_ptr_raw<Function> const  __fun_rename_at_39_35 );
inline smart_ptr_raw<Function> _FuncLintVisitorTickvisitFunction_44fa71cf94fd56d6 ( Context * __context__, lint::LintVisitor & __self_rename_at_48_37, smart_ptr_raw<Function> __fun_rename_at_48_38 );
inline void _FuncLintVisitorTickpreVisitExprBlock_85ebefd711566a91 ( Context * __context__, lint::LintVisitor & __self_rename_at_52_39, smart_ptr_raw<ExprBlock> const  __blk_rename_at_52_40 );
inline smart_ptr_raw<Expression> _FuncLintVisitorTickvisitExprBlock_22a01c59addd8f4 ( Context * __context__, lint::LintVisitor & __self_rename_at_55_41, smart_ptr_raw<ExprBlock> __blk_rename_at_55_42 );
inline smart_ptr_raw<Expression> _FuncLintVisitorTickvisitExprBlockExpression_f757a61fc444fed6 ( Context * __context__, lint::LintVisitor & __self_rename_at_61_43, smart_ptr_raw<ExprBlock> const  __blk_rename_at_61_44, smart_ptr_raw<Expression> __expr_rename_at_61_45 );
inline void _FuncLintVisitorTickpreVisitExprLabel_fbb500e6b71688b6 ( Context * __context__, lint::LintVisitor & __self_rename_at_71_48, smart_ptr_raw<ExprLabel> const  __expr_rename_at_71_49 );
inline void _FuncLintVisitorTickpreVisitExprReturn_d823dfd7056a5c92 ( Context * __context__, lint::LintVisitor & __self_rename_at_77_50, smart_ptr_raw<ExprReturn> const  __expr_rename_at_77_51 );
inline void _FuncLintVisitorTickpreVisitExprCall_f16fcd3917e90c57 ( Context * __context__, lint::LintVisitor & __self_rename_at_83_52, smart_ptr_raw<ExprCall> const  __expr_rename_at_83_53 );
inline void _FuncLintVisitorTickpreVisitExprForVariable_27dc7c7dd02b88fd ( Context * __context__, lint::LintVisitor & __self_rename_at_91_54, smart_ptr_raw<ExprFor> const  __expr_rename_at_91_55, smart_ptr_raw<Variable> const  __v_rename_at_91_56, bool __last_rename_at_91_57 );
inline void _FuncLintVisitorTickvalidate_var_642aa5045edc66b9 ( Context * __context__, lint::LintVisitor & __self_rename_at_95_58, smart_ptr_raw<Variable> const  __v_rename_at_95_59, bool __can_make_const_rename_at_95_60 );
inline void _FuncLintVisitorTickpreVisitExprLet_d0445323d1a7f37 ( Context * __context__, lint::LintVisitor & __self_rename_at_141_63, smart_ptr_raw<ExprLet> const  __expr_rename_at_141_64 );
inline void _FuncLintVisitor_0x27___finalize_51b13a1ae9f10dfa ( Context * __context__, lint::LintVisitor & __self_rename_at_21_66 );
inline void paranoid_e99d462cf42c9878 ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_148_67, bool __compile_time_errors_rename_at_148_68 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d47764d7b3c41f15 ( Context * __context__, lint::LintVisitor const  &  __cl_rename_at_116_0 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_0.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_7759240296ddd78a ( Context * __context__, TArray<ast::AstPassMacro *> &  __Arr_rename_at_181_1, ast::AstPassMacro * __value_rename_at_181_2 )
{
    das_copy(__Arr_rename_at_181_1(builtin_array_push_back(das_arg<TArray<ast::AstPassMacro *>>::pass(__Arr_rename_at_181_1),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_2);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_1ac5c99ce289e45c ( Context * __context__, lint::LintEverything const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_3ae1960cc30e1fd ( Context * __context__, TArray<uint64_t> &  __Arr_rename_at_68_4, int32_t __newSize_rename_at_68_5 )
{
    builtin_array_resize(das_arg<TArray<uint64_t>>::pass(__Arr_rename_at_68_4),__newSize_rename_at_68_5,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_f43a93ccaab2641f ( Context * __context__, TArray<uint64_t> &  __a_rename_at_1234_6 )
{
    builtin_array_free(das_arg<TArray<uint64_t>>::pass(__a_rename_at_1234_6),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<VisitorAdapter> _FuncastTickmake_visitorTick897644165917210720_7793ddd26dad1560 ( Context * __context__, lint::LintVisitor const  &  __someClass_rename_at_684_7 )
{
    lint::LintVisitor const  * __classPtr_rename_at_687_8 = ((lint::LintVisitor const  *)das_ref(__context__,__someClass_rename_at_684_7));
    StructInfo const  * __classInfo_rename_at_688_9 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_d47764d7b3c41f15(__context__,__someClass_rename_at_684_7));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<VisitorAdapter>>::cast(makeVisitor(das_auto_cast<void * const >::cast(__classPtr_rename_at_687_8),__classInfo_rename_at_688_9,__context__));
}

inline void clone_3b9defae8930d957 ( Context * __context__, smart_ptr_raw<VisitorAdapter> & __dest_rename_at_154_10, void * const  __src_rename_at_154_11 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_154_10),__src_rename_at_154_11,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_55065cb84d71e3b7 ( Context * __context__, lint::LintVisitor * & ____this_rename_at_156_12 )
{
    if ( ____this_rename_at_156_12 != nullptr )
    {
        int32_t ____size_rename_at_156_13 = ((int32_t)class_rtti_size(das_auto_cast<void * const >::cast(____this_rename_at_156_12)));
        das_invoke_function<void>::invoke<ast::AstVisitor &>(__context__,nullptr,____this_rename_at_156_12->__finalize,das_arg<ast::AstVisitor>::pass(das_cast<ast::AstVisitor>::cast(das_deref(__context__,____this_rename_at_156_12))));
        das_delete<lint::LintVisitor *>::clear(__context__,____this_rename_at_156_12,____size_rename_at_156_13);
        das_copy(____this_rename_at_156_12,nullptr);
    };
}

inline void _FuncbuiltinTickpushTick14133213201864676143_ae2011e794a9fb57 ( Context * __context__, TArray<uint64_t> &  __Arr_rename_at_165_14, uint64_t __value_rename_at_165_15 )
{
    das_copy(__Arr_rename_at_165_14(builtin_array_push_back(das_arg<TArray<uint64_t>>::pass(__Arr_rename_at_165_14),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_15);
}

inline void _FuncbuiltinTickpopTick1161079256290593740_c6129cadfd6fb4a5 ( Context * __context__, TArray<uint64_t> &  __Arr_rename_at_132_16 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_3ae1960cc30e1fd(__context__,das_arg<TArray<uint64_t>>::pass(__Arr_rename_at_132_16),builtin_array_size(das_arg<TArray<uint64_t>>::pass(__Arr_rename_at_132_16)) - 1);
}

inline uint64_t & _FuncbuiltinTickbackTick18296309835877697278_ff0ac4a30478f080 ( Context * __context__, TArray<uint64_t> &  __a_rename_at_473_17 )
{
    int32_t __l_rename_at_474_18 = ((int32_t)builtin_array_size(das_arg<TArray<uint64_t>>::pass(__a_rename_at_473_17)));
    if ( __l_rename_at_474_18 == 0 )
    {
        builtin_throw(((char *) "back empty array"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast_ref<uint64_t &>::cast(__a_rename_at_473_17((__l_rename_at_474_18 - 1),__context__));
}

inline uint64_t _FuncbuiltinTickintptrTick11320822648609276562_9eb7fbdafa7fb812 ( Context * __context__, void * const  __p_rename_at_15_19 )
{
    return das_auto_cast<uint64_t>::cast(das_cast<uint64_t>::cast(__p_rename_at_15_19));
}

inline Expression * _FuncbuiltinTickget_ptrTick5807679485210906136_b1f1fde051ec136c ( Context * __context__, smart_ptr_raw<Expression> __src_rename_at_1784_20 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1784_20));
}

inline ExprReturn * _FuncbuiltinTickget_ptrTick8468476673553620226_a0a73c860ad64c8d ( Context * __context__, smart_ptr_raw<ExprReturn> const  __src_rename_at_1778_21 )
{
    return das_auto_cast<ExprReturn *>::cast(das_cast<ExprReturn *>::cast(__src_rename_at_1778_21));
}

inline bool _Funcstrings_boostTickeqTick10467641742855300050_6ad0a56a732d869d ( Context * __context__, das::string const  &  __b_rename_at_127_22, char * const  __a_rename_at_127_23 )
{
    return das_auto_cast<bool>::cast(eq_dstr_str(__b_rename_at_127_22,__a_rename_at_127_23));
}

inline ExprCall * _FuncbuiltinTickget_ptrTick8468476673553620226_ff0a681b03d5e96e ( Context * __context__, smart_ptr_raw<ExprCall> const  __src_rename_at_1778_24 )
{
    return das_auto_cast<ExprCall *>::cast(das_cast<ExprCall *>::cast(__src_rename_at_1778_24));
}

inline char * _FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_25, bool __extra_rename_at_38_26, bool __contracts_rename_at_38_27, bool __modules_rename_at_38_28 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_25,__extra_rename_at_38_26,__contracts_rename_at_38_27,__modules_rename_at_38_28,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline void finalize_89dec6ea776f58f4 ( Context * __context__, lint::LintVisitor &  ____this_rename_at_21_29 )
{
    das_delete_handle<smart_ptr_raw<VisitorAdapter>>::clear(__context__,____this_rename_at_21_29.astVisitorAdapter);
    _FuncbuiltinTickfinalizeTick13836114024949725080_f43a93ccaab2641f(__context__,das_arg<TArray<uint64_t>>::pass(____this_rename_at_21_29.exprForTerminator));
    memset((void*)&(____this_rename_at_21_29), 0, TypeSize<lint::LintVisitor>::size);
}

inline lint::LintVisitor LintVisitor_c318ca4b22b1ae4d ( Context * __context__ )
{
    lint::LintVisitor __self_rename_at_26_30; das_zero(__self_rename_at_26_30); das_move(__self_rename_at_26_30, (([&]() -> lint::LintVisitor {
        lint::LintVisitor __mks_26;
        das_zero(__mks_26);
        das_copy((__mks_26.__rtti),(((void *)(&__type_info__f5340b6734d7b2c1))));
        das_copy((__mks_26.__finalize),(das_cast<Func DAS_COMMENT((void,ast::AstVisitor))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor'__finalize S<lint::LintVisitor>*/ 0xd1143a07a80f1d8a)))));
        das_copy((__mks_26.preVisitFunction),(das_cast<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<Function> const ))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`preVisitFunction S<lint::LintVisitor> CY<FunctionPtr>1<H<ast::Function>>?M*/ 0x5c2881857a0c6088)))));
        das_copy((__mks_26.visitFunction),(das_cast<Func DAS_COMMENT((smart_ptr_raw<Function>,ast::AstVisitor,smart_ptr_raw<Function> const ))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`visitFunction S<lint::LintVisitor> Y<FunctionPtr>1<H<ast::Function>>?M*/ 0xd461c5e6ce31a37a)))));
        das_copy((__mks_26.preVisitExprBlock),(das_cast<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`preVisitExprBlock S<lint::LintVisitor> C1<H<ast::ExprBlock>>?M*/ 0x8c03128ad49eb489)))));
        das_copy((__mks_26.visitExprBlock),(das_cast<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`visitExprBlock S<lint::LintVisitor> 1<H<ast::ExprBlock>>?M*/ 0x9b381e3679752a44)))));
        das_copy((__mks_26.visitExprBlockExpression),(das_cast<Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVisitor,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const ))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`visitExprBlockExpression S<lint::LintVisitor> C1<H<ast::ExprBlock>>?M Y<ExpressionPtr>1<H<ast::Expression>>?M*/ 0xc7a6a0292cd2f679)))));
        das_copy((__mks_26.preVisitExprLet),(das_cast<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLet> const ))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`preVisitExprLet S<lint::LintVisitor> C1<H<ast::ExprLet>>?M*/ 0xf5068da131b272d1)))));
        das_copy((__mks_26.preVisitExprCall),(das_cast<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprCall> const ))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`preVisitExprCall S<lint::LintVisitor> C1<H<ast::ExprCall>>?M*/ 0x43ccaebbd8dbba88)))));
        das_copy((__mks_26.preVisitExprForVariable),(das_cast<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`preVisitExprForVariable S<lint::LintVisitor> C1<H<ast::ExprFor>>?M CY<VariablePtr>1<H<ast::Variable>>?M Cb*/ 0xecc644cdf351073d)))));
        das_copy((__mks_26.preVisitExprLabel),(das_cast<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprLabel> const ))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`preVisitExprLabel S<lint::LintVisitor> C1<H<ast::ExprLabel>>?M*/ 0x1378ceaae0245516)))));
        das_copy((__mks_26.preVisitExprReturn),(das_cast<Func DAS_COMMENT((void,ast::AstVisitor,smart_ptr_raw<ExprReturn> const ))>::cast(Func(__context__->fnByMangledName(/*@lint::LintVisitor`preVisitExprReturn S<lint::LintVisitor> C1<H<ast::ExprReturn>>?M*/ 0xa7fce4991e635424)))));
        das_copy((__mks_26.noLint),(false));
        das_copy((__mks_26.lint_error),(Func(__context__->fnByMangledName(/*@lint::LintVisitor`lint_error S<lint::LintVisitor> Cs CH<rtti::LineInfo>*/ 0x23c970088cc98054))));
        das_copy((__mks_26.validate_var),(Func(__context__->fnByMangledName(/*@lint::LintVisitor`validate_var S<lint::LintVisitor> CY<VariablePtr>1<H<ast::Variable>>?M Cb*/ 0xa5f533cb9cbf1dbd))));
        return __mks_26;
    })()));
    return /* <- */ das_auto_cast_move<lint::LintVisitor>::cast(__self_rename_at_26_30);
}

inline void _FuncLintVisitorTicklint_error_4e5763a961a7aa74 ( Context * __context__, lint::LintVisitor &  __self_rename_at_29_31, char * const  __text_rename_at_29_32, LineInfo const  &  __at_rename_at_29_33 )
{
    if ( __self_rename_at_29_31.noLint )
    {
        return ;
    } else {
        if ( __self_rename_at_29_31.compile_time_errors )
        {
            ast_error(compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__at_rename_at_29_33,__text_rename_at_29_32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            builtin_error(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_0, cast<char * const >::from(__text_rename_at_29_32), cast<char * const >::from(((char *) " at ")), cast<char * const >::from(((char * const )(builtin_debug_line(__at_rename_at_29_33,false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
    };
}

inline void _FuncLintVisitorTickpreVisitFunction_9639750a23b12f82 ( Context * __context__, lint::LintVisitor &  __self_rename_at_39_34, smart_ptr_raw<Function> const  __fun_rename_at_39_35 )
{
    das_copy(__self_rename_at_39_34.noLint,false);
    {
        bool __need_loop_41 = true;
        // ann: smart_ptr<rtti::AnnotationDeclaration> const&
        das_iterator<AnnotationList const > __ann_iterator(__fun_rename_at_39_35->annotations /*annotations*/);
        smart_ptr_raw<AnnotationDeclaration> const  * __ann_rename_at_41_36;
        __need_loop_41 = __ann_iterator.first(__context__,(__ann_rename_at_41_36)) && __need_loop_41;
        for ( ; __need_loop_41 ; __need_loop_41 = __ann_iterator.next(__context__,(__ann_rename_at_41_36)) )
        {
            if ( eq_dstr_str((*__ann_rename_at_41_36)->annotation /*annotation*/->name /*name*/,((char *) "no_lint")) )
            {
                das_copy(__self_rename_at_39_34.noLint,true);
                break;
            };
        }
        __ann_iterator.close(__context__,(__ann_rename_at_41_36));
    };
}

inline smart_ptr_raw<Function> _FuncLintVisitorTickvisitFunction_44fa71cf94fd56d6 ( Context * __context__, lint::LintVisitor &  __self_rename_at_48_37, smart_ptr_raw<Function> __fun_rename_at_48_38 )
{
    das_copy(__self_rename_at_48_37.noLint,false);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__fun_rename_at_48_38);
}

inline void _FuncLintVisitorTickpreVisitExprBlock_85ebefd711566a91 ( Context * __context__, lint::LintVisitor &  __self_rename_at_52_39, smart_ptr_raw<ExprBlock> const  __blk_rename_at_52_40 )
{
    _FuncbuiltinTickpushTick14133213201864676143_ae2011e794a9fb57(__context__,das_arg<TArray<uint64_t>>::pass(__self_rename_at_52_39.exprForTerminator),UINT64_C(0x0));
}

inline smart_ptr_raw<Expression> _FuncLintVisitorTickvisitExprBlock_22a01c59addd8f4 ( Context * __context__, lint::LintVisitor &  __self_rename_at_55_41, smart_ptr_raw<ExprBlock> __blk_rename_at_55_42 )
{
    if ( builtin_array_size(das_arg<TArray<uint64_t>>::pass(__self_rename_at_55_41.exprForTerminator)) > 0 )
    {
        _FuncbuiltinTickpopTick1161079256290593740_c6129cadfd6fb4a5(__context__,das_arg<TArray<uint64_t>>::pass(__self_rename_at_55_41.exprForTerminator));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__blk_rename_at_55_42);
}

inline smart_ptr_raw<Expression> _FuncLintVisitorTickvisitExprBlockExpression_f757a61fc444fed6 ( Context * __context__, lint::LintVisitor &  __self_rename_at_61_43, smart_ptr_raw<ExprBlock> const  __blk_rename_at_61_44, smart_ptr_raw<Expression> __expr_rename_at_61_45 )
{
    uint64_t __lb_rename_at_62_46 = ((uint64_t)_FuncbuiltinTickbackTick18296309835877697278_ff0ac4a30478f080(__context__,das_arg<TArray<uint64_t>>::pass(__self_rename_at_61_43.exprForTerminator)));
    if ( __lb_rename_at_62_46 != UINT64_C(0x0) )
    {
        uint64_t __eb_rename_at_64_47 = ((uint64_t)_FuncbuiltinTickintptrTick11320822648609276562_9eb7fbdafa7fb812(__context__,das_auto_cast<void * const >::cast(_FuncbuiltinTickget_ptrTick5807679485210906136_b1f1fde051ec136c(__context__,__expr_rename_at_61_45))));
        if ( __lb_rename_at_62_46 != __eb_rename_at_64_47 )
        {
            das_invoke_method<void,offsetof(lint::LintVisitor,lint_error)>::invoke<lint::LintVisitor &,char * const ,LineInfo &>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_61_43),((char *) "unreachable code"),das_arg<LineInfo>::pass(__expr_rename_at_61_45->at /*at*/));
        };
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__expr_rename_at_61_45);
}

inline void _FuncLintVisitorTickpreVisitExprLabel_fbb500e6b71688b6 ( Context * __context__, lint::LintVisitor &  __self_rename_at_71_48, smart_ptr_raw<ExprLabel> const  __expr_rename_at_71_49 )
{
    if ( builtin_array_size(das_arg<TArray<uint64_t>>::pass(__self_rename_at_71_48.exprForTerminator)) > 0 )
    {
        _FuncbuiltinTickpopTick1161079256290593740_c6129cadfd6fb4a5(__context__,das_arg<TArray<uint64_t>>::pass(__self_rename_at_71_48.exprForTerminator));
    };
    _FuncbuiltinTickpushTick14133213201864676143_ae2011e794a9fb57(__context__,das_arg<TArray<uint64_t>>::pass(__self_rename_at_71_48.exprForTerminator),UINT64_C(0x0));
}

inline void _FuncLintVisitorTickpreVisitExprReturn_d823dfd7056a5c92 ( Context * __context__, lint::LintVisitor &  __self_rename_at_77_50, smart_ptr_raw<ExprReturn> const  __expr_rename_at_77_51 )
{
    if ( builtin_array_size(das_arg<TArray<uint64_t>>::pass(__self_rename_at_77_50.exprForTerminator)) > 0 )
    {
        _FuncbuiltinTickpopTick1161079256290593740_c6129cadfd6fb4a5(__context__,das_arg<TArray<uint64_t>>::pass(__self_rename_at_77_50.exprForTerminator));
    };
    _FuncbuiltinTickpushTick14133213201864676143_ae2011e794a9fb57(__context__,das_arg<TArray<uint64_t>>::pass(__self_rename_at_77_50.exprForTerminator),_FuncbuiltinTickintptrTick11320822648609276562_9eb7fbdafa7fb812(__context__,das_auto_cast<void * const >::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_a0a73c860ad64c8d(__context__,__expr_rename_at_77_51))));
}

inline void _FuncLintVisitorTickpreVisitExprCall_f16fcd3917e90c57 ( Context * __context__, lint::LintVisitor &  __self_rename_at_83_52, smart_ptr_raw<ExprCall> const  __expr_rename_at_83_53 )
{
    if ( _Funcstrings_boostTickeqTick10467641742855300050_6ad0a56a732d869d(__context__,__expr_rename_at_83_53->name /*name*/,((char *) "panic")) && _Funcstrings_boostTickeqTick10467641742855300050_6ad0a56a732d869d(__context__,__expr_rename_at_83_53->func /*func*/->module /*_module*/->name /*name*/,((char *) "$")) )
    {
        if ( builtin_array_size(das_arg<TArray<uint64_t>>::pass(__self_rename_at_83_52.exprForTerminator)) > 0 )
        {
            _FuncbuiltinTickpopTick1161079256290593740_c6129cadfd6fb4a5(__context__,das_arg<TArray<uint64_t>>::pass(__self_rename_at_83_52.exprForTerminator));
        };
        _FuncbuiltinTickpushTick14133213201864676143_ae2011e794a9fb57(__context__,das_arg<TArray<uint64_t>>::pass(__self_rename_at_83_52.exprForTerminator),_FuncbuiltinTickintptrTick11320822648609276562_9eb7fbdafa7fb812(__context__,das_auto_cast<void * const >::cast(_FuncbuiltinTickget_ptrTick8468476673553620226_ff0a681b03d5e96e(__context__,__expr_rename_at_83_53))));
    };
}

inline void _FuncLintVisitorTickpreVisitExprForVariable_27dc7c7dd02b88fd ( Context * __context__, lint::LintVisitor &  __self_rename_at_91_54, smart_ptr_raw<ExprFor> const  __expr_rename_at_91_55, smart_ptr_raw<Variable> const  __v_rename_at_91_56, bool __last_rename_at_91_57 )
{
    das_invoke_method<void,offsetof(lint::LintVisitor,validate_var)>::invoke<lint::LintVisitor &,smart_ptr_raw<Variable> const ,bool>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_91_54),__v_rename_at_91_56,false);
}

inline void _FuncLintVisitorTickvalidate_var_642aa5045edc66b9 ( Context * __context__, lint::LintVisitor &  __self_rename_at_95_58, smart_ptr_raw<Variable> const  __v_rename_at_95_59, bool __can_make_const_rename_at_95_60 )
{
    char * __name_rename_at_96_61 = ((char *)(char *)(((char * const )(to_das_string(__v_rename_at_95_59->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    if ( builtin_string_startswith(__name_rename_at_96_61,((char *) "__"),__context__) )
    {
        return ;
    } else {
        if ( builtin_string_startswith(__name_rename_at_96_61,((char *) "_"),__context__) )
        {
            if ( builtin_string_endswith(__name_rename_at_96_61,((char *) "_"),__context__) )
            {
                return ;
            } else {
                if ( ((das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 2) || das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 4)) || das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 1)) || (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__v_rename_at_95_59->init /*init*/),das_auto_cast<void * const >::cast(nullptr))) )
                {
                    das_invoke_method<void,offsetof(lint::LintVisitor,lint_error)>::invoke<lint::LintVisitor &,char *,LineInfo const  &>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_95_58),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_1, cast<char * const >::from(((char *) "variable '")), cast<das::string const  &>::from(__v_rename_at_95_59->name /*name*/), cast<char * const >::from(((char *) "' is used and should be named without underscode prefix")))),__v_rename_at_95_59->at /*at*/);
                    return ;
                } else {
                    return ;
                };
            };
        };
        if ( ((das_deref(__context__,__v_rename_at_95_59)).isAccessUnused()) )
        {
            das_invoke_method<void,offsetof(lint::LintVisitor,lint_error)>::invoke<lint::LintVisitor &,char *,LineInfo const  &>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_95_58),das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_2, cast<char * const >::from(((char *) "unused variable ")), cast<das::string const  &>::from(__v_rename_at_95_59->name /*name*/), cast<char * const >::from(((char *) ": ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6(__context__,__v_rename_at_95_59->type /*_type*/,true,true,true)), cast<char * const >::from(((char *) " (add an underscore prefix if you really need it)")))),__v_rename_at_95_59->at /*at*/);
            return ;
        } else {
            if ( (((!(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 2)) && !(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 4))) && (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__v_rename_at_95_59->init /*init*/),das_auto_cast<void * const >::cast(nullptr)))) && isExprConst(__v_rename_at_95_59->init /*init*/)) && !(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 5)) )
            {
                das_invoke_method<void,offsetof(lint::LintVisitor,lint_error)>::invoke<lint::LintVisitor &,char *,LineInfo const  &>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_95_58),das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_3, cast<char * const >::from(((char *) "unused variable ")), cast<das::string const  &>::from(__v_rename_at_95_59->name /*name*/), cast<char * const >::from(((char *) ": ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6(__context__,__v_rename_at_95_59->type /*_type*/,true,true,true)), cast<char * const >::from(((char *) " (add an underscore prefix if you really need it) ")), cast<Bitfield>::from(__v_rename_at_95_59->access_flags /*access_flags*/))),__v_rename_at_95_59->at /*at*/);
                return ;
            } else {
                if ( (((!(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 2)) && !(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 4))) && !(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 1))) && ((equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__v_rename_at_95_59->init /*init*/),das_auto_cast<void * const >::cast(nullptr))) || das_get_bitfield(__v_rename_at_95_59->init /*init*/->flags /*flags*/,1u << 1))) && !(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 5)) )
                {
                    das_invoke_method<void,offsetof(lint::LintVisitor,lint_error)>::invoke<lint::LintVisitor &,char *,LineInfo const  &>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_95_58),das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_4, cast<char * const >::from(((char *) "unused variable ")), cast<das::string const  &>::from(__v_rename_at_95_59->name /*name*/), cast<char * const >::from(((char *) ": ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6(__context__,__v_rename_at_95_59->type /*_type*/,true,true,true)), cast<char * const >::from(((char *) " (add an underscore prefix if you really need it)")))),__v_rename_at_95_59->at /*at*/);
                    return ;
                } else {
                    if ( (!(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 1)) && !(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 2))) && (equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__v_rename_at_95_59->init /*init*/),das_auto_cast<void * const >::cast(nullptr))) )
                    {
                        bool __sideEffects_rename_at_123_62 = ((bool)((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__v_rename_at_95_59->init /*init*/),das_auto_cast<void * const >::cast(nullptr))) && !(das_get_bitfield(__v_rename_at_95_59->init /*init*/->flags /*flags*/,1u << 1))));
                        if ( !__sideEffects_rename_at_123_62 )
                        {
                            das_invoke_method<void,offsetof(lint::LintVisitor,lint_error)>::invoke<lint::LintVisitor &,char *,LineInfo const  &>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_95_58),das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_5, cast<char * const >::from(((char *) "variable ")), cast<das::string const  &>::from(__v_rename_at_95_59->name /*name*/), cast<char * const >::from(((char *) ": ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6(__context__,__v_rename_at_95_59->type /*_type*/,true,true,true)), cast<char * const >::from(((char *) " is never used")))),__v_rename_at_95_59->at /*at*/);
                        } else {
                            das_invoke_method<void,offsetof(lint::LintVisitor,lint_error)>::invoke<lint::LintVisitor &,char *,LineInfo const  &>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_95_58),das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_6, cast<char * const >::from(((char *) "variable ")), cast<das::string const  &>::from(__v_rename_at_95_59->name /*name*/), cast<char * const >::from(((char *) ": ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6(__context__,__v_rename_at_95_59->type /*_type*/,true,true,true)), cast<char * const >::from(((char *) " is never used (be careful, initializer has side effects)")))),__v_rename_at_95_59->at /*at*/);
                        };
                        return ;
                    } else {
                        if ( (((__can_make_const_rename_at_95_60 && (__v_rename_at_95_59->type /*_type*/->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tPointer)) && !(das_get_bitfield(__v_rename_at_95_59->type /*_type*/->flags /*flags*/,1u << 1))) && !(das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 2))) && ((das_deref(__context__,__v_rename_at_95_59->type /*_type*/)).canCloneFromConst()) )
                        {
                            das_invoke_method<void,offsetof(lint::LintVisitor,lint_error)>::invoke<lint::LintVisitor &,char *,LineInfo const  &>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_95_58),das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_7, cast<char * const >::from(((char *) "variable ")), cast<das::string const  &>::from(__v_rename_at_95_59->name /*name*/), cast<char * const >::from(((char *) ": ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6(__context__,__v_rename_at_95_59->type /*_type*/,true,true,true)), cast<char * const >::from(((char *) " can be made const (declare with 'let')")))),__v_rename_at_95_59->at /*at*/);
                            return ;
                        } else {
                            if ( (((__can_make_const_rename_at_95_60 && (__v_rename_at_95_59->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer)) && !(das_get_bitfield(__v_rename_at_95_59->type /*_type*/->flags /*flags*/,1u << 1))) && !((das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 2) || das_get_bitfield(__v_rename_at_95_59->access_flags /*access_flags*/,1u << 4)))) && ((das_deref(__context__,__v_rename_at_95_59->type /*_type*/)).canCloneFromConst()) )
                            {
                                das_invoke_method<void,offsetof(lint::LintVisitor,lint_error)>::invoke<lint::LintVisitor &,char *,LineInfo const  &>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_95_58),das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_8, cast<char * const >::from(((char *) "variable ")), cast<das::string const  &>::from(__v_rename_at_95_59->name /*name*/), cast<char * const >::from(((char *) ": ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6(__context__,__v_rename_at_95_59->type /*_type*/,true,true,true)), cast<char * const >::from(((char *) " can be made const (declare with 'let')")))),__v_rename_at_95_59->at /*at*/);
                                return ;
                            };
                        };
                    };
                };
            };
        };
    };
}

inline void _FuncLintVisitorTickpreVisitExprLet_d0445323d1a7f37 ( Context * __context__, lint::LintVisitor &  __self_rename_at_141_63, smart_ptr_raw<ExprLet> const  __expr_rename_at_141_64 )
{
    {
        bool __need_loop_142 = true;
        // v: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __v_iterator(__expr_rename_at_141_64->variables /*variables*/);
        smart_ptr_raw<Variable> const  * __v_rename_at_142_65;
        __need_loop_142 = __v_iterator.first(__context__,(__v_rename_at_142_65)) && __need_loop_142;
        for ( ; __need_loop_142 ; __need_loop_142 = __v_iterator.next(__context__,(__v_rename_at_142_65)) )
        {
            das_invoke_method<void,offsetof(lint::LintVisitor,validate_var)>::invoke<lint::LintVisitor &,smart_ptr_raw<Variable> const ,bool>(__context__,nullptr,das_arg<lint::LintVisitor>::pass(__self_rename_at_141_63),(*__v_rename_at_142_65),true);
        }
        __v_iterator.close(__context__,(__v_rename_at_142_65));
    };
}

inline void _FuncLintVisitor_0x27___finalize_51b13a1ae9f10dfa ( Context * __context__, lint::LintVisitor &  __self_rename_at_21_66 )
{
    finalize_89dec6ea776f58f4(__context__,das_arg<lint::LintVisitor>::pass(__self_rename_at_21_66));
}

inline void paranoid_e99d462cf42c9878 ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_148_67, bool __compile_time_errors_rename_at_148_68 )
{
    lint::LintVisitor * __astVisitor_rename_at_149_69 = das_ascend<lint::LintVisitor,lint::LintVisitor>::make(__context__,nullptr,(([&]() -> lint::LintVisitor {
        lint::LintVisitor __mks_149 = LintVisitor_c318ca4b22b1ae4d(__context__);
        das_copy((__mks_149.compile_time_errors),(__compile_time_errors_rename_at_148_68));
        return __mks_149;
    })()));
    das_move(__astVisitor_rename_at_149_69->astVisitorAdapter,_FuncastTickmake_visitorTick897644165917210720_7793ddd26dad1560(__context__,das_arg<lint::LintVisitor>::pass(das_deref(__context__,__astVisitor_rename_at_149_69))));
    astVisit(__prog_rename_at_148_67,__astVisitor_rename_at_149_69->astVisitorAdapter,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    clone_3b9defae8930d957(__context__,__astVisitor_rename_at_149_69->astVisitorAdapter,das_auto_cast<void * const >::cast(nullptr));
    finalize_55065cb84d71e3b7(__context__,__astVisitor_rename_at_149_69);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x14af1a6a319cedb7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_d47764d7b3c41f15>>();
    };
    aotLib[0x8282b618a6d69348] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_7759240296ddd78a>>();
    };
    aotLib[0xc5c5d8dee2d3ca4c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_1ac5c99ce289e45c>>();
    };
    aotLib[0x5fbb1c37bc30a975] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_3ae1960cc30e1fd>>();
    };
    aotLib[0xe0e03905650cb645] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_f43a93ccaab2641f>>();
    };
    aotLib[0x591537144cdde233] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_visitorTick897644165917210720_7793ddd26dad1560>>();
    };
    aotLib[0xa6a82d1245f119d3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_3b9defae8930d957>>();
    };
    aotLib[0x3d2f95bfbe6ae480] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_55065cb84d71e3b7>>();
    };
    aotLib[0x8013d0153c798435] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_ae2011e794a9fb57>>();
    };
    aotLib[0xda593b8cc96aefa4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_c6129cadfd6fb4a5>>();
    };
    aotLib[0x82b5a63efa6c6440] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickbackTick18296309835877697278_ff0ac4a30478f080>>();
    };
    aotLib[0x3ba2ac00fe65dda] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickintptrTick11320822648609276562_9eb7fbdafa7fb812>>();
    };
    aotLib[0x3de78d8fa0dda3ca] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_b1f1fde051ec136c>>();
    };
    aotLib[0xbaae77e6a859ea49] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_a0a73c860ad64c8d>>();
    };
    aotLib[0xc1aae4775f362427] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickeqTick10467641742855300050_6ad0a56a732d869d>>();
    };
    aotLib[0x2364bef1cc030651] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_ff0a681b03d5e96e>>();
    };
    aotLib[0xb6bf4468b074675b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_38f9e41e41e94da6>>();
    };
    aotLib[0x245713fd69ee840f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_89dec6ea776f58f4>>();
    };
    aotLib[0xed778bb834777cf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&LintVisitor_c318ca4b22b1ae4d>>();
    };
    aotLib[0xd680b4822696791c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTicklint_error_4e5763a961a7aa74>>();
    };
    aotLib[0x64650fd97e24900] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickpreVisitFunction_9639750a23b12f82>>();
    };
    aotLib[0x251a46e361f46907] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickvisitFunction_44fa71cf94fd56d6>>();
    };
    aotLib[0xa3250ebf61a9f2d9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickpreVisitExprBlock_85ebefd711566a91>>();
    };
    aotLib[0xddd1e7e25f4f2fad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickvisitExprBlock_22a01c59addd8f4>>();
    };
    aotLib[0xf083f78468b862e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickvisitExprBlockExpression_f757a61fc444fed6>>();
    };
    aotLib[0xc93a6a38ca337bed] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickpreVisitExprLabel_fbb500e6b71688b6>>();
    };
    aotLib[0x51efefa1fe623b57] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickpreVisitExprReturn_d823dfd7056a5c92>>();
    };
    aotLib[0x695877aeffc69f9d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickpreVisitExprCall_f16fcd3917e90c57>>();
    };
    aotLib[0xa2e0888d5a52426] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickpreVisitExprForVariable_27dc7c7dd02b88fd>>();
    };
    aotLib[0xf78ef85e60d66f62] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickvalidate_var_642aa5045edc66b9>>();
    };
    aotLib[0x195b0245196231d7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitorTickpreVisitExprLet_d0445323d1a7f37>>();
    };
    aotLib[0x3a7ffa742dd005a2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncLintVisitor_0x27___finalize_51b13a1ae9f10dfa>>();
    };
    aotLib[0x8628ff2404883d5a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&paranoid_e99d462cf42c9878>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_12570130816669471540
AotListBase impl_aot_lint(_anon_12570130816669471540::registerAotFunctions);
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
