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
 // require json
 // require defer
 // require apply
 // require json_boost
 // require refactor

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
namespace _anon_8840095415379932743 {

namespace refactor { struct ExtractMethodDesc; };
namespace refactor { struct ExtractMethodMacro; };
namespace refactor { struct ExtractVariableDesc; };
namespace refactor { struct ExtractVariableMacro; };
namespace refactor { struct ExtractVariableFunction; };
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
namespace macro_boost { struct MacroVerifyMacro; };
namespace macro_boost { struct CaptureBlock; };
namespace macro_boost { struct CapturedVariable; };
namespace macro_boost { struct ColletFinally; };
namespace macro_boost { struct ColletLabels; };
namespace macro_boost { struct ReturnSkipLockcheck; };
namespace json { struct JsonValue; };
namespace json { struct TokenAt; };
namespace json { struct _lambda_json_99_1; };
namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
namespace apply { struct ApplyMacro; };
namespace json_boost { struct BetterJsonMacro; };
namespace json_boost { struct JsonReader; };
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
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
namespace macro_boost {

struct CapturedVariable {
    Variable * variable;
    ExprVar * expression;
    bool eref;
};
}
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
namespace json {

struct JsonValue {
    AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> value;
};
}
// unused structure TokenAt
// unused structure _lambda_json_99_1
// unused structure DeferMacro
// unused structure DeferDeleteMacro
// unused structure ApplyMacro
// unused structure BetterJsonMacro
// unused structure JsonReader
namespace refactor {

struct ExtractMethodDesc {
    char * call_name;
    char * function_declaration;
    char * call_expression;
    LineInfo call_at;
    LineInfo function_body_at;
};
}
namespace refactor {

struct ExtractMethodMacro {
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
    char * const  method_tag;
};
}
namespace refactor {

struct ExtractVariableDesc {
    char * variable_name;
    char * variable_type;
    char * variable_keyword;
    bool is_reference;
    LineInfo call_at;
    LineInfo variable_init_at;
};
}
namespace refactor {

struct ExtractVariableMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
}
namespace refactor {

struct ExtractVariableFunction {
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
    char * const  method_tag;
};
}
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__bd7cda50d47698b1;
extern VarInfo __var_info__bd7ce950d476b22e;
extern VarInfo __var_info__86ad0551f90082c7;
extern FuncInfo __func_info__d872ffd10ffc59e3;
extern FuncInfo __func_info__f782de3b913ce22c;
extern FuncInfo __func_info__bbb85e1b55dc84f;

VarInfo __func_info__d872ffd10ffc59e3_field_0 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 317, 1, UINT64_C(0xbd7cda50d47698b1), "field", 0, 0 };
VarInfo * __func_info__d872ffd10ffc59e3_fields[1] =  { &__func_info__d872ffd10ffc59e3_field_0 };
FuncInfo __func_info__d872ffd10ffc59e3 = {"invoke block<(field:bool const&):void> const", "", __func_info__d872ffd10ffc59e3_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xd872ffd10ffc59e3), 0x0 };
VarInfo __func_info__f782de3b913ce22c_field_0 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::LineInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8230, 24, UINT64_C(0x86ad0551f90082c7), "field", 0, 0 };
VarInfo * __func_info__f782de3b913ce22c_fields[1] =  { &__func_info__f782de3b913ce22c_field_0 };
FuncInfo __func_info__f782de3b913ce22c = {"invoke block<(field:rtti::LineInfo const):void> const", "", __func_info__f782de3b913ce22c_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xf782de3b913ce22c), 0x0 };
VarInfo __func_info__bbb85e1b55dc84f_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16677, 8, UINT64_C(0xbd7ce950d476b22e), "field", 0, 0 };
VarInfo * __func_info__bbb85e1b55dc84f_fields[1] =  { &__func_info__bbb85e1b55dc84f_field_0 };
FuncInfo __func_info__bbb85e1b55dc84f = {"invoke block<(field:string const&):void> const", "", __func_info__bbb85e1b55dc84f_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xbbb85e1b55dc84f), 0x0 };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_1[1] = { &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_2[1] = { &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_3[1] = { &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_4[1] = { &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_5[1] = { &__type_info__af8afe4c86446b52 };

inline void _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22 ( Context * __context__, TTable<char *,json::JsonValue *> & __Tab_rename_at_939_0, char * const  __at_rename_at_939_1, json::JsonValue * __val_rename_at_939_2 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888 ( Context * __context__, TTable<char *,char *> const  & __a_rename_at_1128_3 );
inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickvaluesTick1935193042646774172_bb2c3e8e5405df3b ( Context * __context__, TTable<char *,char *> const  & __a_rename_at_1143_5 );
inline void _FuncapplyTickstructTickExtractMethodDescTick0x217Tick0Tick5_48404bf757ad1933 ( Context * __context__, refactor::ExtractMethodDesc const  & ___Var_Tick_self_rename_at_62_7, Block DAS_COMMENT((void,char * const  &)) const  & ____arg_call_name_rename_at_62_8, Block DAS_COMMENT((void,char * const  &)) const  & ____arg_function_declaration_rename_at_62_9, Block DAS_COMMENT((void,char * const  &)) const  & ____arg_call_expression_rename_at_62_10, Block DAS_COMMENT((void,LineInfo const )) const  & ____arg_call_at_rename_at_62_11, Block DAS_COMMENT((void,LineInfo const )) const  & ____arg_function_body_at_rename_at_62_12 );
inline void _FuncapplyTickstructTickExtractVariableDescTick0x217Tick0Tick6_d80450c9c7a35438 ( Context * __context__, refactor::ExtractVariableDesc const  & ___Var_Tick_self_rename_at_62_13, Block DAS_COMMENT((void,char * const  &)) const  & ____arg_variable_name_rename_at_62_14, Block DAS_COMMENT((void,char * const  &)) const  & ____arg_variable_type_rename_at_62_15, Block DAS_COMMENT((void,char * const  &)) const  & ____arg_variable_keyword_rename_at_62_16, Block DAS_COMMENT((void,bool const  &)) const  & ____arg_is_reference_rename_at_62_17, Block DAS_COMMENT((void,LineInfo const )) const  & ____arg_call_at_rename_at_62_18, Block DAS_COMMENT((void,LineInfo const )) const  & ____arg_variable_init_at_rename_at_62_19 );
inline Sequence DAS_COMMENT((json::JsonValue * &)) _FuncbuiltinTickvaluesTick1351216622833168869_e4c809c3c6fa4dfd ( Context * __context__, TTable<char *,json::JsonValue *> & __a_rename_at_1150_20 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_4c53369d1a9c0194 ( Context * __context__, TTable<char *,json::JsonValue *> & __a_rename_at_1193_22 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_dc31ee4da771c4 ( Context * __context__, TArray<json::JsonValue *> & __a_rename_at_1182_24 );
inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_26, ast::AstFunctionAnnotation * __value_rename_at_181_27 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f71f3d89fb753b2 ( Context * __context__, refactor::ExtractMethodMacro const  & __cl_rename_at_116_28 );
inline void _FuncbuiltinTickpushTick10769833213962245646_9a94d5ed4f7c5f3d ( Context * __context__, TArray<ast::AstCallMacro *> & __Arr_rename_at_181_29, ast::AstCallMacro * __value_rename_at_181_30 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7fc1c676a0c803b0 ( Context * __context__, refactor::ExtractVariableMacro const  & __cl_rename_at_116_31 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_42f12d0f0ca9b24d ( Context * __context__, refactor::ExtractVariableFunction const  & __cl_rename_at_116_32 );
inline void finalize_435c75a1641e15fa ( Context * __context__, AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> & ____this_rename_at_24_33 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_5df8cb221c94902 ( Context * __context__, TTable<char *,char *> & __a_rename_at_1193_34 );
inline void finalize_23291ba5b15c15ae ( Context * __context__, json::JsonValue & ____this_rename_at_22_35 );
inline json::JsonValue * _Funcjson_boostTickJVTick13966542468881610056_6e31ec00abc1b8a7 ( Context * __context__, TTable<char *,char *> const  & __value_rename_at_517_36 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_95ba3a9d4de4e95b ( Context * __context__, char * const  __name_rename_at_631_42, refactor::ExtractMethodMacro * __someClassPtr_rename_at_631_43 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_e08431098b248a79 ( Context * __context__, char * const  __name_rename_at_631_45, refactor::ExtractVariableFunction * __someClassPtr_rename_at_631_46 );
inline TTable<char *,char *> _FuncbuiltinTickto_table_moveTick5858896087460481804_9bc7a430485b2b12 ( Context * __context__, TDim<AutoTuple<char *,char *>,6> & __a_rename_at_1455_48 );
inline void finalize_f1db2ed1abc2f753 ( Context * __context__, json::JsonValue * & ____this_rename_at_111_51 );
inline void _FuncbuiltinTicksortTick5995501125257068354_37074a96ed1276f6 ( Context * __context__, TArray<macro_boost::CapturedVariable> & __a_rename_at_1576_52, Block DAS_COMMENT((bool,macro_boost::CapturedVariable const ,macro_boost::CapturedVariable const )) const  & __cmp_rename_at_1576_53 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_54, bool __extra_rename_at_38_55, bool __contracts_rename_at_38_56, bool __modules_rename_at_38_57 );
inline json::JsonValue * _Funcjson_boostTickJVTick13966542468881610056_1ada305f850b7c84 ( Context * __context__, refactor::ExtractMethodDesc const  & __value_rename_at_517_58 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_480816f933565535 ( Context * __context__, char * const  __name_rename_at_273_65, char * const  __tag_rename_at_273_66, refactor::ExtractMethodMacro * __classPtr_rename_at_273_67 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_a30263d9bfe747d3 ( Context * __context__, char * const  __name_rename_at_273_69, char * const  __tag_rename_at_273_70, refactor::ExtractVariableFunction * __classPtr_rename_at_273_71 );
inline json::JsonValue * _Funcjson_boostTickJVTick13966542468881610056_802f12bd209d8b61 ( Context * __context__, refactor::ExtractVariableDesc const  & __value_rename_at_517_73 );
inline void extract_method_8e5f4d23667ee4fa ( Context * __context__, char * const  __method_name_rename_at_19_81, Block DAS_COMMENT((void)) const  & __blk_rename_at_19_82 );
inline char * log_range_8671290fdeeface ( Context * __context__, LineInfo const  & __at_rename_at_28_83 );
inline json::JsonValue * JV_b6f7ce5a2e653514 ( Context * __context__, LineInfo const  & __li_rename_at_40_84 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0xcad3b4d085b635b3>(__context__) = 100500;/*LOG_REFACTOR*/
}

inline void _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22 ( Context * __context__, TTable<char *,json::JsonValue *> &  __Tab_rename_at_939_0, char * const  __at_rename_at_939_1, json::JsonValue * __val_rename_at_939_2 )
{
    das_copy(__Tab_rename_at_939_0(__at_rename_at_939_1,__context__),__val_rename_at_939_2);
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888 ( Context * __context__, TTable<char *,char *> const  &  __a_rename_at_1128_3 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1129_4; das_zero(__it_rename_at_1129_4);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1129_4),__a_rename_at_1128_3,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1129_4);
}

inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickvaluesTick1935193042646774172_bb2c3e8e5405df3b ( Context * __context__, TTable<char *,char *> const  &  __a_rename_at_1143_5 )
{
    Sequence DAS_COMMENT((char * *)) __it_rename_at_1144_6; das_zero(__it_rename_at_1144_6);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((char * const ))>::pass(__it_rename_at_1144_6),__a_rename_at_1143_5,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char * &))>::cast(__it_rename_at_1144_6);
}

inline void _FuncapplyTickstructTickExtractMethodDescTick0x217Tick0Tick5_48404bf757ad1933 ( Context * __context__, refactor::ExtractMethodDesc const  &  ___Var_Tick_self_rename_at_62_7, Block DAS_COMMENT((void,char * const  &)) const  &  ____arg_call_name_rename_at_62_8, Block DAS_COMMENT((void,char * const  &)) const  &  ____arg_function_declaration_rename_at_62_9, Block DAS_COMMENT((void,char * const  &)) const  &  ____arg_call_expression_rename_at_62_10, Block DAS_COMMENT((void,LineInfo const )) const  &  ____arg_call_at_rename_at_62_11, Block DAS_COMMENT((void,LineInfo const )) const  &  ____arg_function_body_at_rename_at_62_12 )
{
    das_invoke<void>::invoke<char * const  &>(__context__,nullptr,____arg_call_name_rename_at_62_8,___Var_Tick_self_rename_at_62_7.call_name);
    das_invoke<void>::invoke<char * const  &>(__context__,nullptr,____arg_function_declaration_rename_at_62_9,___Var_Tick_self_rename_at_62_7.function_declaration);
    das_invoke<void>::invoke<char * const  &>(__context__,nullptr,____arg_call_expression_rename_at_62_10,___Var_Tick_self_rename_at_62_7.call_expression);
    das_invoke<void>::invoke<LineInfo const  &>(__context__,nullptr,____arg_call_at_rename_at_62_11,___Var_Tick_self_rename_at_62_7.call_at);
    das_invoke<void>::invoke<LineInfo const  &>(__context__,nullptr,____arg_function_body_at_rename_at_62_12,___Var_Tick_self_rename_at_62_7.function_body_at);
}

inline void _FuncapplyTickstructTickExtractVariableDescTick0x217Tick0Tick6_d80450c9c7a35438 ( Context * __context__, refactor::ExtractVariableDesc const  &  ___Var_Tick_self_rename_at_62_13, Block DAS_COMMENT((void,char * const  &)) const  &  ____arg_variable_name_rename_at_62_14, Block DAS_COMMENT((void,char * const  &)) const  &  ____arg_variable_type_rename_at_62_15, Block DAS_COMMENT((void,char * const  &)) const  &  ____arg_variable_keyword_rename_at_62_16, Block DAS_COMMENT((void,bool const  &)) const  &  ____arg_is_reference_rename_at_62_17, Block DAS_COMMENT((void,LineInfo const )) const  &  ____arg_call_at_rename_at_62_18, Block DAS_COMMENT((void,LineInfo const )) const  &  ____arg_variable_init_at_rename_at_62_19 )
{
    das_invoke<void>::invoke<char * const  &>(__context__,nullptr,____arg_variable_name_rename_at_62_14,___Var_Tick_self_rename_at_62_13.variable_name);
    das_invoke<void>::invoke<char * const  &>(__context__,nullptr,____arg_variable_type_rename_at_62_15,___Var_Tick_self_rename_at_62_13.variable_type);
    das_invoke<void>::invoke<char * const  &>(__context__,nullptr,____arg_variable_keyword_rename_at_62_16,___Var_Tick_self_rename_at_62_13.variable_keyword);
    das_invoke<void>::invoke<bool const  &>(__context__,nullptr,____arg_is_reference_rename_at_62_17,___Var_Tick_self_rename_at_62_13.is_reference);
    das_invoke<void>::invoke<LineInfo const  &>(__context__,nullptr,____arg_call_at_rename_at_62_18,___Var_Tick_self_rename_at_62_13.call_at);
    das_invoke<void>::invoke<LineInfo const  &>(__context__,nullptr,____arg_variable_init_at_rename_at_62_19,___Var_Tick_self_rename_at_62_13.variable_init_at);
}

inline Sequence DAS_COMMENT((json::JsonValue * &)) _FuncbuiltinTickvaluesTick1351216622833168869_e4c809c3c6fa4dfd ( Context * __context__, TTable<char *,json::JsonValue *> &  __a_rename_at_1150_20 )
{
    Sequence DAS_COMMENT((json::JsonValue * *)) __it_rename_at_1151_21; das_zero(__it_rename_at_1151_21);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((json::JsonValue *))>::pass(__it_rename_at_1151_21),das_arg<TTable<char *,json::JsonValue *>>::pass(__a_rename_at_1150_20),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::JsonValue * &))>::cast(__it_rename_at_1151_21);
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_4c53369d1a9c0194 ( Context * __context__, TTable<char *,json::JsonValue *> &  __a_rename_at_1193_22 )
{
    Sequence DAS_COMMENT((json::JsonValue * *)) _temp_make_local_1195_19_32; _temp_make_local_1195_19_32;
    {
        bool __need_loop_1195 = true;
        // aV: json::JsonValue?&
        das_iterator<Sequence DAS_COMMENT((json::JsonValue *))> __aV_iterator((_temp_make_local_1195_19_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_e4c809c3c6fa4dfd(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__a_rename_at_1193_22)))));
        json::JsonValue * * __aV_rename_at_1195_23;
        __need_loop_1195 = __aV_iterator.first(__context__,(__aV_rename_at_1195_23)) && __need_loop_1195;
        for ( ; __need_loop_1195 ; __need_loop_1195 = __aV_iterator.next(__context__,(__aV_rename_at_1195_23)) )
        {
            finalize_f1db2ed1abc2f753(__context__,(*__aV_rename_at_1195_23));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1195_23));
    };
    builtin_table_free(das_arg<TTable<char *,json::JsonValue *>>::pass(__a_rename_at_1193_22),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_dc31ee4da771c4 ( Context * __context__, TArray<json::JsonValue *> &  __a_rename_at_1182_24 )
{
    {
        bool __need_loop_1184 = true;
        // aV: json::JsonValue? aka TT&
        das_iterator<TArray<json::JsonValue *>> __aV_iterator(__a_rename_at_1182_24);
        json::JsonValue * * __aV_rename_at_1184_25;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_25)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_25)) )
        {
            finalize_f1db2ed1abc2f753(__context__,(*__aV_rename_at_1184_25));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_25));
    };
    builtin_array_free(das_arg<TArray<json::JsonValue *>>::pass(__a_rename_at_1182_24),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_26, ast::AstFunctionAnnotation * __value_rename_at_181_27 )
{
    das_copy(__Arr_rename_at_181_26(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_26),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_27);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f71f3d89fb753b2 ( Context * __context__, refactor::ExtractMethodMacro const  &  __cl_rename_at_116_28 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_28.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_9a94d5ed4f7c5f3d ( Context * __context__, TArray<ast::AstCallMacro *> &  __Arr_rename_at_181_29, ast::AstCallMacro * __value_rename_at_181_30 )
{
    das_copy(__Arr_rename_at_181_29(builtin_array_push_back(das_arg<TArray<ast::AstCallMacro *>>::pass(__Arr_rename_at_181_29),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_30);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7fc1c676a0c803b0 ( Context * __context__, refactor::ExtractVariableMacro const  &  __cl_rename_at_116_31 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_31.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_42f12d0f0ca9b24d ( Context * __context__, refactor::ExtractVariableFunction const  &  __cl_rename_at_116_32 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_32.__rtti))).getStructType())));
}

inline void finalize_435c75a1641e15fa ( Context * __context__, AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> &  ____this_rename_at_24_33 )
{
    if ( das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(____this_rename_at_24_33) )
    {
        _FuncbuiltinTickfinalizeTick5454204887383796109_4c53369d1a9c0194(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::get(____this_rename_at_24_33)));
    } else if ( das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(____this_rename_at_24_33) )
    {
        _FuncbuiltinTickfinalizeTick13836114024949725080_dc31ee4da771c4(__context__,das_arg<TArray<json::JsonValue *>>::pass(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::get(____this_rename_at_24_33)));
    };
    memset(&(____this_rename_at_24_33), 0, TypeSize<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>>::size);
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_5df8cb221c94902 ( Context * __context__, TTable<char *,char *> &  __a_rename_at_1193_34 )
{
    builtin_table_free(das_arg<TTable<char *,char *>>::pass(__a_rename_at_1193_34),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_23291ba5b15c15ae ( Context * __context__, json::JsonValue &  ____this_rename_at_22_35 )
{
    finalize_435c75a1641e15fa(__context__,das_arg<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>>::pass(____this_rename_at_22_35.value));
    memset(&(____this_rename_at_22_35), 0, TypeSize<json::JsonValue>::size);
}

inline json::JsonValue * _Funcjson_boostTickJVTick13966542468881610056_6e31ec00abc1b8a7 ( Context * __context__, TTable<char *,char *> const  &  __value_rename_at_517_36 )
{
    Sequence DAS_COMMENT((char *)) _temp_make_local_551_21_80; _temp_make_local_551_21_80;
    Sequence DAS_COMMENT((char * *)) _temp_make_local_551_34_96; _temp_make_local_551_34_96;
    TTable<char *,json::JsonValue *> __map_rename_at_550_37; das_zero(__map_rename_at_550_37);
    {
        bool __need_loop_551 = true;
        // k: string
        das_iterator<Sequence DAS_COMMENT((char *))> __k_iterator((_temp_make_local_551_21_80 = (_FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888(__context__,__value_rename_at_517_36))));
        char * __k_rename_at_551_40;
        __need_loop_551 = __k_iterator.first(__context__,(__k_rename_at_551_40)) && __need_loop_551;
        // v: string const&
        das_iterator<Sequence DAS_COMMENT((char * const ))> __v_iterator((_temp_make_local_551_34_96 = (_FuncbuiltinTickvaluesTick1935193042646774172_bb2c3e8e5405df3b(__context__,__value_rename_at_517_36))));
        char * const  * __v_rename_at_551_41;
        __need_loop_551 = __v_iterator.first(__context__,(__v_rename_at_551_41)) && __need_loop_551;
        for ( ; __need_loop_551 ; __need_loop_551 = __k_iterator.next(__context__,(__k_rename_at_551_40)) && __v_iterator.next(__context__,(__v_rename_at_551_41)) )
        {
            _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_550_37),__k_rename_at_551_40,das_invoke_function<json::JsonValue *>::invoke<char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV Cs*/ 0x7935bbf71114917)),(*__v_rename_at_551_41)));
        }
        __k_iterator.close(__context__,(__k_rename_at_551_40));
        __v_iterator.close(__context__,(__v_rename_at_551_41));
    };
    return das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke<TTable<char *,json::JsonValue *> &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV 1<s>2<1<S<json::JsonValue>>?>T*/ 0xef6bfabcd7a5f481)),das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_550_37)));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_95ba3a9d4de4e95b ( Context * __context__, char * const  __name_rename_at_631_42, refactor::ExtractMethodMacro * __someClassPtr_rename_at_631_43 )
{
    _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_631_43));
    StructInfo const  * __classInfo_rename_at_634_44 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_f71f3d89fb753b2(__context__,das_arg<refactor::ExtractMethodMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_631_43))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_631_42,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_631_43),__classInfo_rename_at_634_44,__context__));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_e08431098b248a79 ( Context * __context__, char * const  __name_rename_at_631_45, refactor::ExtractVariableFunction * __someClassPtr_rename_at_631_46 )
{
    _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_631_46));
    StructInfo const  * __classInfo_rename_at_634_47 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_42f12d0f0ca9b24d(__context__,das_arg<refactor::ExtractVariableFunction>::pass(das_deref(__context__,__someClassPtr_rename_at_631_46))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_631_45,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_631_46),__classInfo_rename_at_634_47,__context__));
}

inline TTable<char *,char *> _FuncbuiltinTickto_table_moveTick5858896087460481804_9bc7a430485b2b12 ( Context * __context__, TDim<AutoTuple<char *,char *>,6> &  __a_rename_at_1455_48 )
{
    TTable<char *,char *> __tab_rename_at_1456_49; das_zero(__tab_rename_at_1456_49);
    {
        bool __need_loop_1458 = true;
        // x: tuple<string aka keyT;string aka valT>&
        das_iterator<TDim<AutoTuple<char *,char *>,6>> __x_iterator(__a_rename_at_1455_48);
        AutoTuple<char *,char *> * __x_rename_at_1458_50;
        __need_loop_1458 = __x_iterator.first(__context__,(__x_rename_at_1458_50)) && __need_loop_1458;
        for ( ; __need_loop_1458 ; __need_loop_1458 = __x_iterator.next(__context__,(__x_rename_at_1458_50)) )
        {
            das_copy(__tab_rename_at_1456_49(das_get_auto_tuple_field<char *,0,char *,char *>::get((*__x_rename_at_1458_50)),__context__),das_get_auto_tuple_field<char *,1,char *,char *>::get((*__x_rename_at_1458_50)));
        }
        __x_iterator.close(__context__,(__x_rename_at_1458_50));
    };
    return /* <- */ das_auto_cast_move<TTable<char *,char *>>::cast(__tab_rename_at_1456_49);
}

inline void finalize_f1db2ed1abc2f753 ( Context * __context__, json::JsonValue * & ____this_rename_at_111_51 )
{
    if ( ____this_rename_at_111_51 != nullptr )
    {
        finalize_23291ba5b15c15ae(__context__,das_arg<json::JsonValue>::pass(das_deref(__context__,____this_rename_at_111_51)));
        das_delete<json::JsonValue *>::clear(__context__,____this_rename_at_111_51);
        das_copy(____this_rename_at_111_51,nullptr);
    };
}

inline void _FuncbuiltinTicksortTick5995501125257068354_37074a96ed1276f6 ( Context * __context__, TArray<macro_boost::CapturedVariable> &  __a_rename_at_1576_52, Block DAS_COMMENT((bool,macro_boost::CapturedVariable const ,macro_boost::CapturedVariable const )) const  &  __cmp_rename_at_1576_53 )
{
    builtin_sort_array_any_cblock_T(das_arg<TArray<macro_boost::CapturedVariable>>::pass(__a_rename_at_1576_52),24,builtin_array_size(das_arg<TArray<macro_boost::CapturedVariable>>::pass(__a_rename_at_1576_52)),__cmp_rename_at_1576_53,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_54, bool __extra_rename_at_38_55, bool __contracts_rename_at_38_56, bool __modules_rename_at_38_57 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_54,__extra_rename_at_38_55,__contracts_rename_at_38_56,__modules_rename_at_38_57,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline json::JsonValue * _Funcjson_boostTickJVTick13966542468881610056_1ada305f850b7c84 ( Context * __context__, refactor::ExtractMethodDesc const  &  __value_rename_at_517_58 ) { das_stack_prologue __prologue(__context__,400,"json_boost`JV`13966542468881610056 " DAS_FILE_LINE);
{
    TTable<char *,json::JsonValue *> __map_rename_at_534_59; das_zero(__map_rename_at_534_59);
    _FuncapplyTickstructTickExtractMethodDescTick0x217Tick0Tick5_48404bf757ad1933(__context__,__value_rename_at_517_58,das_make_block<void,char * const  &>(__context__,128,0,&__func_info__bbb85e1b55dc84f,[&](char * const  & __field_rename_at_535_60) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_59),((char *) "call_name"),das_invoke_function<json::JsonValue *>::invoke<char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV Cs*/ 0x7935bbf71114917)),__field_rename_at_535_60));
    }),das_make_block<void,char * const  &>(__context__,192,0,&__func_info__bbb85e1b55dc84f,[&](char * const  & __field_rename_at_535_61) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_59),((char *) "function_declaration"),das_invoke_function<json::JsonValue *>::invoke<char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV Cs*/ 0x7935bbf71114917)),__field_rename_at_535_61));
    }),das_make_block<void,char * const  &>(__context__,256,0,&__func_info__bbb85e1b55dc84f,[&](char * const  & __field_rename_at_535_62) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_59),((char *) "call_expression"),das_invoke_function<json::JsonValue *>::invoke<char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV Cs*/ 0x7935bbf71114917)),__field_rename_at_535_62));
    }),das_make_block<void,LineInfo const  &>(__context__,320,0,&__func_info__f782de3b913ce22c,[&](LineInfo const  & __field_rename_at_535_63) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_59),((char *) "call_at"),JV_b6f7ce5a2e653514(__context__,__field_rename_at_535_63));
    }),das_make_block<void,LineInfo const  &>(__context__,384,0,&__func_info__f782de3b913ce22c,[&](LineInfo const  & __field_rename_at_535_64) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_59),((char *) "function_body_at"),JV_b6f7ce5a2e653514(__context__,__field_rename_at_535_64));
    }));
    return das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke<TTable<char *,json::JsonValue *> &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV 1<s>2<1<S<json::JsonValue>>?>T*/ 0xef6bfabcd7a5f481)),das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_59)));
}}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_480816f933565535 ( Context * __context__, char * const  __name_rename_at_273_65, char * const  __tag_rename_at_273_66, refactor::ExtractMethodMacro * __classPtr_rename_at_273_67 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_274_68; memset(&__ann_rename_at_274_68,0,sizeof(__ann_rename_at_274_68));
    /* finally */ auto __finally_273= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_274_68);
    /* end finally */ });
    __ann_rename_at_274_68; das_zero(__ann_rename_at_274_68); das_move(__ann_rename_at_274_68, _FuncastTickmake_function_annotationTick3074191368936885601_95ba3a9d4de4e95b(__context__,__name_rename_at_273_65,__classPtr_rename_at_273_67));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 0x7678364e276db9c2)),__tag_rename_at_273_66,__ann_rename_at_274_68);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_274_68,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_a30263d9bfe747d3 ( Context * __context__, char * const  __name_rename_at_273_69, char * const  __tag_rename_at_273_70, refactor::ExtractVariableFunction * __classPtr_rename_at_273_71 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_274_72; memset(&__ann_rename_at_274_72,0,sizeof(__ann_rename_at_274_72));
    /* finally */ auto __finally_273= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_274_72);
    /* end finally */ });
    __ann_rename_at_274_72; das_zero(__ann_rename_at_274_72); das_move(__ann_rename_at_274_72, _FuncastTickmake_function_annotationTick3074191368936885601_e08431098b248a79(__context__,__name_rename_at_273_69,__classPtr_rename_at_273_71));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 0x7678364e276db9c2)),__tag_rename_at_273_70,__ann_rename_at_274_72);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_274_72,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline json::JsonValue * _Funcjson_boostTickJVTick13966542468881610056_802f12bd209d8b61 ( Context * __context__, refactor::ExtractVariableDesc const  &  __value_rename_at_517_73 ) { das_stack_prologue __prologue(__context__,464,"json_boost`JV`13966542468881610056 " DAS_FILE_LINE);
{
    TTable<char *,json::JsonValue *> __map_rename_at_534_74; das_zero(__map_rename_at_534_74);
    _FuncapplyTickstructTickExtractVariableDescTick0x217Tick0Tick6_d80450c9c7a35438(__context__,__value_rename_at_517_73,das_make_block<void,char * const  &>(__context__,128,0,&__func_info__bbb85e1b55dc84f,[&](char * const  & __field_rename_at_535_75) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_74),((char *) "variable_name"),das_invoke_function<json::JsonValue *>::invoke<char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV Cs*/ 0x7935bbf71114917)),__field_rename_at_535_75));
    }),das_make_block<void,char * const  &>(__context__,192,0,&__func_info__bbb85e1b55dc84f,[&](char * const  & __field_rename_at_535_76) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_74),((char *) "variable_type"),das_invoke_function<json::JsonValue *>::invoke<char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV Cs*/ 0x7935bbf71114917)),__field_rename_at_535_76));
    }),das_make_block<void,char * const  &>(__context__,256,0,&__func_info__bbb85e1b55dc84f,[&](char * const  & __field_rename_at_535_77) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_74),((char *) "variable_keyword"),das_invoke_function<json::JsonValue *>::invoke<char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV Cs*/ 0x7935bbf71114917)),__field_rename_at_535_77));
    }),das_make_block<void,bool const  &>(__context__,320,0,&__func_info__d872ffd10ffc59e3,[&](bool const  & __field_rename_at_535_78) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_74),((char *) "is_reference"),das_invoke_function<json::JsonValue *>::invoke<bool>(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV Cb*/ 0x7a25bbf712ac617)),__field_rename_at_535_78));
    }),das_make_block<void,LineInfo const  &>(__context__,384,0,&__func_info__f782de3b913ce22c,[&](LineInfo const  & __field_rename_at_535_79) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_74),((char *) "call_at"),JV_b6f7ce5a2e653514(__context__,__field_rename_at_535_79));
    }),das_make_block<void,LineInfo const  &>(__context__,448,0,&__func_info__f782de3b913ce22c,[&](LineInfo const  & __field_rename_at_535_80) -> void{
        _FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_74),((char *) "variable_init_at"),JV_b6f7ce5a2e653514(__context__,__field_rename_at_535_80));
    }));
    return das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke<TTable<char *,json::JsonValue *> &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JV 1<s>2<1<S<json::JsonValue>>?>T*/ 0xef6bfabcd7a5f481)),das_arg<TTable<char *,json::JsonValue *>>::pass(__map_rename_at_534_74)));
}}

inline void extract_method_8e5f4d23667ee4fa ( Context * __context__, char * const  __method_name_rename_at_19_81, Block DAS_COMMENT((void)) const  &  __blk_rename_at_19_82 )
{
    das_invoke<void>::invoke(__context__,nullptr,__blk_rename_at_19_82);
}

inline char * log_range_8671290fdeeface ( Context * __context__, LineInfo const  &  __at_rename_at_28_83 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_0, cast<char * const >::from(((char * const )(builtin_debug_line(__at_rename_at_28_83,false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "-")), cast<int32_t>::from(int32_t(__at_rename_at_28_83.last_line /*last_line*/)), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(int32_t(__at_rename_at_28_83.last_column /*last_column*/)))));
}

inline json::JsonValue * JV_b6f7ce5a2e653514 ( Context * __context__, LineInfo const  &  __li_rename_at_40_84 )
{
    char * __fname_rename_at_41_85; memset(&__fname_rename_at_41_85,0,sizeof(__fname_rename_at_41_85));
    int32_t __tabSize_rename_at_42_86; memset(&__tabSize_rename_at_42_86,0,sizeof(__tabSize_rename_at_42_86));
    TTable<char *,char *> __kv_rename_at_43_87; memset(&__kv_rename_at_43_87,0,sizeof(__kv_rename_at_43_87));
    TDim<AutoTuple<char *,char *>,6> _temp_make_local_43_22_112; _temp_make_local_43_22_112;
    /* finally */ auto __finally_40= das_finally([&](){
    _FuncbuiltinTickfinalizeTick5454204887383796109_5df8cb221c94902(__context__,das_arg<TTable<char *,char *>>::pass(__kv_rename_at_43_87));
    /* end finally */ });
    __fname_rename_at_41_85 = ((char *)(char *)(((__li_rename_at_40_84.fileInfo /*fileInfo*/ != nullptr) ? das_auto_cast<char * const >::cast(((char * const )(to_das_string(__li_rename_at_40_84.fileInfo /*fileInfo*/->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) : das_auto_cast<char * const >::cast(nullptr))));
    __tabSize_rename_at_42_86 = ((int32_t)das_null_coalescing<int32_t>::get(das_safe_navigation_handle<FileInfo,int32_t>::get(__li_rename_at_40_84.fileInfo /*fileInfo*/,([&](const FileInfo * __any) -> auto & {return __any->tabSize /*tabSize*/;})),4));
    __kv_rename_at_43_87; das_zero(__kv_rename_at_43_87); das_move(__kv_rename_at_43_87, _FuncbuiltinTickto_table_moveTick5858896087460481804_9bc7a430485b2b12(__context__,das_arg<TDim<AutoTuple<char *,char *>,6>>::pass((([&]() -> TDim<AutoTuple<char *,char *>,6>& {
        _temp_make_local_43_22_112(0,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_43;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_43) = ((char *) "line");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_43) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_1, cast<int32_t>::from(int32_t(__li_rename_at_40_84.line /*line*/))));
            return __mkt_43;
        })());
        _temp_make_local_43_22_112(1,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_43;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_43) = ((char *) "column");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_43) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_2, cast<int32_t>::from(int32_t(__li_rename_at_40_84.column /*column*/))));
            return __mkt_43;
        })());
        _temp_make_local_43_22_112(2,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_43;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_43) = ((char *) "last_line");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_43) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_3, cast<int32_t>::from(int32_t(__li_rename_at_40_84.last_line /*last_line*/))));
            return __mkt_43;
        })());
        _temp_make_local_43_22_112(3,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_43;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_43) = ((char *) "last_column");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_43) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_4, cast<int32_t>::from(int32_t(__li_rename_at_40_84.last_column /*last_column*/))));
            return __mkt_43;
        })());
        _temp_make_local_43_22_112(4,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_43;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_43) = ((char *) "file");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_43) = __fname_rename_at_41_85;
            return __mkt_43;
        })());
        _temp_make_local_43_22_112(5,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_43;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_43) = ((char *) "tab");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_43) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<int32_t>::from(__tabSize_rename_at_42_86)));
            return __mkt_43;
        })());
        return _temp_make_local_43_22_112;
    })()))));
    return das_auto_cast<json::JsonValue *>::cast(_Funcjson_boostTickJVTick13966542468881610056_6e31ec00abc1b8a7(__context__,das_arg<TTable<char *,char *>>::pass(__kv_rename_at_43_87)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x91de40987b0422b7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick4246857231018487965_29d8ad7ca1533e22>>();
    };
    aotLib[0xeefb3e272bbfb4a4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888>>();
    };
    aotLib[0xc88537e2e7d634e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1935193042646774172_bb2c3e8e5405df3b>>();
    };
    aotLib[0x712f93413b0b505] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncapplyTickstructTickExtractMethodDescTick0x217Tick0Tick5_48404bf757ad1933>>();
    };
    aotLib[0xbde6dfe6f51e78e5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncapplyTickstructTickExtractVariableDescTick0x217Tick0Tick6_d80450c9c7a35438>>();
    };
    aotLib[0x99aff7cd6a09d767] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_e4c809c3c6fa4dfd>>();
    };
    aotLib[0x3c66963ce3602d5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_4c53369d1a9c0194>>();
    };
    aotLib[0x87aeac74e6237029] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_dc31ee4da771c4>>();
    };
    aotLib[0xd09660c6ccc577ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795>>();
    };
    aotLib[0x2af26f08da38ed03] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_f71f3d89fb753b2>>();
    };
    aotLib[0x3fdd65aa3293d0d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_9a94d5ed4f7c5f3d>>();
    };
    aotLib[0xfe124f9bcef0523e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_7fc1c676a0c803b0>>();
    };
    aotLib[0xe1a51901728bc04f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_42f12d0f0ca9b24d>>();
    };
    aotLib[0x5042114dcf9ac7a8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_435c75a1641e15fa>>();
    };
    aotLib[0x4bc162b24a69568f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_5df8cb221c94902>>();
    };
    aotLib[0x92b7bdad1f62a817] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_23291ba5b15c15ae>>();
    };
    aotLib[0xfd5b833add5565f4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcjson_boostTickJVTick13966542468881610056_6e31ec00abc1b8a7>>();
    };
    aotLib[0xaf5f924cc62e5c1e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_function_annotationTick3074191368936885601_95ba3a9d4de4e95b>>();
    };
    aotLib[0x623f20bc72fba5e8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_function_annotationTick3074191368936885601_e08431098b248a79>>();
    };
    aotLib[0x88b5cdec914a1cf1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_table_moveTick5858896087460481804_9bc7a430485b2b12>>();
    };
    aotLib[0x789622b5d5aa146e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_f1db2ed1abc2f753>>();
    };
    aotLib[0x9344d52fd248ddfa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicksortTick5995501125257068354_37074a96ed1276f6>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    aotLib[0xd4192796e0f41fad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcjson_boostTickJVTick13966542468881610056_1ada305f850b7c84>>();
    };
    aotLib[0x67f52afaba677a32] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_480816f933565535>>();
    };
    aotLib[0x602b1ac43b89d11b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_a30263d9bfe747d3>>();
    };
    aotLib[0xae7b5b7e5141af91] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcjson_boostTickJVTick13966542468881610056_802f12bd209d8b61>>();
    };
    aotLib[0x22ed3b80af40eec4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&extract_method_8e5f4d23667ee4fa>>();
    };
    aotLib[0xe104d0f3fe5c7881] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&log_range_8671290fdeeface>>();
    };
    aotLib[0x5686a48ef3d1db6e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_b6f7ce5a2e653514>>();
    };
    // [[ init script ]]
    aotLib[0x8d26cdc807208b6e] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_8840095415379932743
AotListBase impl_aot_refactor(_anon_8840095415379932743::registerAotFunctions);
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
