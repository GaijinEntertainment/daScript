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
 // require fio
#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_fio.h"
 // require strings_boost
 // require ast_boost
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require helpers
 // require colors
 // require match
 // require utf8_utils
 // require meta_ast
 // require parser_generator
 // require parse_macro
 // require peg
 // require spoof

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
namespace _anon_1837871176362534164 {

namespace spoof { struct SpoofTemplateReader; };
namespace spoof { struct SpoofInvocation; };
namespace spoof { struct SpoofInstanceReader; };
namespace spoof { struct invocationParserTickid_0x0; };
namespace spoof { struct invocationParserTickid_0x1; };
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
namespace fio { struct df_header; };
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
namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
namespace helpers { struct _lambda_helpers_26_1; };
namespace match { struct MatchError; };
namespace match { struct MatchTo; };
namespace match { struct MatchMacro; };
namespace match { struct StaticMatchMacro; };
namespace match { struct MultiMatchMacro; };
namespace match { struct StaticMultiMatchMacro; };
namespace match { struct MatchAsIs; };
namespace match { struct MatchCopy; };
namespace meta_ast { struct Rule_; };
namespace meta_ast { struct Definition; };
namespace meta_ast { struct Alternative; };
namespace parser_generator { struct ParserGenerator; };
namespace parse_macro { struct MacroRule; };
namespace parse_macro { struct ParseMacro; };
namespace peg { struct ParsingError; };
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
namespace ast {

struct AstReaderMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
}
// unused structure AstCommentReader
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
// unused structure AstVisitor
// unused structure df_header
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
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
// unused structure DeferMacro
// unused structure DeferDeleteMacro
// unused structure _lambda_helpers_26_1
// unused enumeration MatchType
// unused structure MatchError
// unused structure MatchTo
// unused structure MatchMacro
// unused structure StaticMatchMacro
// unused structure MultiMatchMacro
// unused structure StaticMultiMatchMacro
// unused structure MatchAsIs
// unused structure MatchCopy
// unused structure Rule_
// unused structure Definition
// unused structure Alternative
// unused structure ParserGenerator
// unused structure MacroRule
// unused structure ParseMacro
namespace peg {

struct ParsingError {
    char * text;
    int32_t index;
};
}
namespace spoof {

struct SpoofTemplateReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
}
namespace spoof {

struct SpoofInvocation {
    char * varName;
    TArray<char *> args;
};
}
namespace spoof {

struct SpoofInstanceReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
}
namespace spoof {

struct invocationParserTickid_0x0 {
    TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> comma_separated_elements_cache;
    TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> element_list_cache;
    TTable<int32_t,AutoTuple<bool,char *,int32_t>> ident__cache;
    TTable<int32_t,AutoTuple<bool,char *,int32_t>> element_value_cache;
    TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> element_cache;
    TTable<int32_t,AutoTuple<bool,char *,int32_t>> sub_element_cache;
    TArray<uint8_t> input;
    int32_t index;
    int32_t tabs;
    bool color_output;
    bool error_reporting;
    int32_t longest_prefix;
    TArray<bool> suppress_errors;
    TArray<peg::ParsingError> errors;
};
}
namespace spoof {

struct invocationParserTickid_0x1 {
    TTable<int32_t,AutoTuple<bool,char *,int32_t>> comma_separated_elements_cache;
    TTable<int32_t,AutoTuple<bool,char *,int32_t>> sub_element_cache;
    TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> macro_invocation_cache;
    TTable<int32_t,AutoTuple<bool,char *,int32_t>> ident__cache;
    TTable<int32_t,AutoTuple<bool,char *,int32_t>> class_name_cache;
    TTable<int32_t,AutoTuple<bool,char *,int32_t>> element_cache;
    TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> element_list_cache;
    TArray<uint8_t> input;
    int32_t index;
    int32_t tabs;
    bool color_output;
    bool error_reporting;
    int32_t longest_prefix;
    TArray<bool> suppress_errors;
    TArray<peg::ParsingError> errors;
};
}
extern StructInfo __struct_info__2040d68d100cd133;
extern StructInfo __struct_info__c993e285a04c692f;
extern TypeInfo __type_info__7d42bfacbd731b56;
extern TypeInfo __type_info__71e1112747ff8db0;
extern TypeInfo __type_info__8f14d918b6786dec;
extern TypeInfo __type_info__12393804d99ce574;
extern TypeInfo __type_info__a49d33b75bf04f48;
extern TypeInfo __type_info__29a1dd55fa61e9e3;
extern TypeInfo __type_info__5eddda99b960ff80;
extern TypeInfo __type_info__1bcaf53bc9823a03;
extern TypeInfo __type_info__9293ad0d07b1dbee;
extern TypeInfo __type_info__26703dd8492d36b8;
extern TypeInfo __type_info__a9e49a99cab89d4;
extern TypeInfo __type_info__66686027eaf8675d;
extern TypeInfo __type_info__fa593d0882a72913;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__6beb6823e5dcfa26;
extern TypeInfo __type_info__554236004213764e;
extern TypeInfo __type_info__243e11abf1d63530;
extern TypeInfo __type_info__7775f165da7e23c2;
extern TypeInfo __type_info__6ad1eba4ab633b52;
extern TypeInfo __type_info__9e830744959095a7;
extern TypeInfo __type_info__9e169205ce7610ad;
extern TypeInfo __type_info__7480a390cb0fd543;
extern TypeInfo __type_info__b02bad0282fed564;
extern TypeInfo __type_info__b18f2b15460317a1;
extern TypeInfo __type_info__df1b7db3f566d157;
extern TypeInfo __type_info__35f44e4f93af85ad;
extern TypeInfo __type_info__ab24c91193d6fa43;
extern TypeInfo __type_info__662f8256d51be64;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__c149136652fe1c57;
extern VarInfo __var_info__188fdd7d3dd85eb1;
extern VarInfo __var_info__9a6e42f8b2fb2b62;
extern VarInfo __var_info__727ed3de16c14aef;
extern VarInfo __var_info__5a9648338a3b70ee;
extern FuncInfo __func_info__920fb1be533a8af4;

VarInfo __struct_info__2040d68d100cd133_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x9a6e42f8b2fb2b62), "text", offsetof(peg::ParsingError,text), 2 };
VarInfo __struct_info__2040d68d100cd133_field_1 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x188fdd7d3dd85eb1), "index", offsetof(peg::ParsingError,index), 0 };
VarInfo * __struct_info__2040d68d100cd133_fields[2] =  { &__struct_info__2040d68d100cd133_field_0, &__struct_info__2040d68d100cd133_field_1 };
StructInfo __struct_info__2040d68d100cd133 = {"ParsingError", "peg", 8, __struct_info__2040d68d100cd133_fields, 2, TypeSize<peg::ParsingError>::size, UINT64_C(0x0), nullptr, UINT64_C(0x2040d68d100cd133), 0 };
VarInfo __struct_info__c993e285a04c692f_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x5a9648338a3b70ee), "varName", offsetof(spoof::SpoofInvocation,varName), 1 };
VarInfo __struct_info__c993e285a04c692f_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<char *>>::size, UINT64_C(0x727ed3de16c14aef), "args", offsetof(spoof::SpoofInvocation,args), 2 };
VarInfo * __struct_info__c993e285a04c692f_fields[2] =  { &__struct_info__c993e285a04c692f_field_0, &__struct_info__c993e285a04c692f_field_1 };
StructInfo __struct_info__c993e285a04c692f = {"SpoofInvocation", "spoof", 28, __struct_info__c993e285a04c692f_fields, 2, TypeSize<spoof::SpoofInvocation>::size, UINT64_C(0x0), nullptr, UINT64_C(0xc993e285a04c692f), 0 };
VarInfo __func_info__920fb1be533a8af4_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, TypeSize<TArray<uint8_t>>::size, UINT64_C(0xc149136652fe1c57), "inp", 0, 0 };
VarInfo * __func_info__920fb1be533a8af4_fields[1] =  { &__func_info__920fb1be533a8af4_field_0 };
FuncInfo __func_info__920fb1be533a8af4 = {"invoke block<(inp:array<uint8> const#):void> const", "", __func_info__920fb1be533a8af4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x920fb1be533a8af4), 0x0 };
TypeInfo * __type_info__7d42bfacbd731b56_arg_types[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo __type_info__7d42bfacbd731b56 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__7d42bfacbd731b56_arg_types, nullptr, 2, 0, nullptr, 16646, TypeSize<AutoTuple<char *,char *>>::size, UINT64_C(0x7d42bfacbd731b56) };
TypeInfo * __type_info__71e1112747ff8db0_arg_types[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo __type_info__71e1112747ff8db0 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__71e1112747ff8db0_arg_types, nullptr, 2, 0, nullptr, 16390, TypeSize<AutoTuple<char *,char *>>::size, UINT64_C(0x71e1112747ff8db0) };
TypeInfo __type_info__8f14d918b6786dec = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__71e1112747ff8db0, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<AutoTuple<char *,char *>>>::size, UINT64_C(0x8f14d918b6786dec) };
TypeInfo __type_info__12393804d99ce574 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<char *>>::size, UINT64_C(0x12393804d99ce574) };
TypeInfo __type_info__a49d33b75bf04f48 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__71e1112747ff8db0, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<AutoTuple<char *,char *>>>::size, UINT64_C(0xa49d33b75bf04f48) };
TypeInfo __type_info__29a1dd55fa61e9e3 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__554236004213764e, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<peg::ParsingError>>::size, UINT64_C(0x29a1dd55fa61e9e3) };
TypeInfo __type_info__5eddda99b960ff80 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<char *>>::size, UINT64_C(0x5eddda99b960ff80) };
TypeInfo * __type_info__1bcaf53bc9823a03_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__8f14d918b6786dec, &__type_info__af63e44c8601fa24 };
const char * __type_info__1bcaf53bc9823a03_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__1bcaf53bc9823a03 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__1bcaf53bc9823a03_arg_types, __type_info__1bcaf53bc9823a03_arg_names, 3, 0, nullptr, 57346, TypeSize<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::size, UINT64_C(0x1bcaf53bc9823a03) };
TypeInfo * __type_info__9293ad0d07b1dbee_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__12393804d99ce574, &__type_info__af63e44c8601fa24 };
const char * __type_info__9293ad0d07b1dbee_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__9293ad0d07b1dbee = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__9293ad0d07b1dbee_arg_types, __type_info__9293ad0d07b1dbee_arg_names, 3, 0, nullptr, 57346, TypeSize<AutoTuple<bool,TArray<char *>,int32_t>>::size, UINT64_C(0x9293ad0d07b1dbee) };
TypeInfo * __type_info__26703dd8492d36b8_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__243e11abf1d63530, &__type_info__af63e44c8601fa24 };
const char * __type_info__26703dd8492d36b8_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__26703dd8492d36b8 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__26703dd8492d36b8_arg_types, __type_info__26703dd8492d36b8_arg_names, 3, 0, nullptr, 57346, TypeSize<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::size, UINT64_C(0x26703dd8492d36b8) };
TypeInfo __type_info__a9e49a99cab89d4 = { Type::tStructure, &__struct_info__c993e285a04c692f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<spoof::SpoofInvocation>::size, UINT64_C(0xa9e49a99cab89d4) };
TypeInfo __type_info__66686027eaf8675d = { Type::tStructure, &__struct_info__2040d68d100cd133, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16678, TypeSize<peg::ParsingError>::size, UINT64_C(0x66686027eaf8675d) };
TypeInfo __type_info__fa593d0882a72913 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16548, TypeSize<char *>::size, UINT64_C(0xfa593d0882a72913) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<StringBuilderWriter>::size, UINT64_C(0x37d36026a6078a42) };
TypeInfo * __type_info__6beb6823e5dcfa26_arg_types[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
const char * __type_info__6beb6823e5dcfa26_arg_names[2] = { "argName", "init" };
TypeInfo __type_info__6beb6823e5dcfa26 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__6beb6823e5dcfa26_arg_types, __type_info__6beb6823e5dcfa26_arg_names, 2, 0, nullptr, 16390, TypeSize<AutoTuple<char *,char *>>::size, UINT64_C(0x6beb6823e5dcfa26) };
TypeInfo __type_info__554236004213764e = { Type::tStructure, &__struct_info__2040d68d100cd133, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, TypeSize<peg::ParsingError>::size, UINT64_C(0x554236004213764e) };
TypeInfo __type_info__243e11abf1d63530 = { Type::tStructure, &__struct_info__c993e285a04c692f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<spoof::SpoofInvocation>::size, UINT64_C(0x243e11abf1d63530) };
TypeInfo __type_info__7775f165da7e23c2 = { Type::tStructure, &__struct_info__2040d68d100cd133, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, TypeSize<peg::ParsingError>::size, UINT64_C(0x7775f165da7e23c2) };
TypeInfo __type_info__6ad1eba4ab633b52 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__6beb6823e5dcfa26, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<AutoTuple<char *,char *>>>::size, UINT64_C(0x6ad1eba4ab633b52) };
TypeInfo __type_info__9e830744959095a7 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__7775f165da7e23c2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<peg::ParsingError>>::size, UINT64_C(0x9e830744959095a7) };
TypeInfo * __type_info__9e169205ce7610ad_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__8f14d918b6786dec, &__type_info__af63e44c8601fa24 };
const char * __type_info__9e169205ce7610ad_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__9e169205ce7610ad = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__9e169205ce7610ad_arg_types, __type_info__9e169205ce7610ad_arg_names, 3, 0, nullptr, 57346, TypeSize<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::size, UINT64_C(0x9e169205ce7610ad) };
TypeInfo * __type_info__7480a390cb0fd543_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__243e11abf1d63530, &__type_info__af63e44c8601fa24 };
const char * __type_info__7480a390cb0fd543_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__7480a390cb0fd543 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__7480a390cb0fd543_arg_types, __type_info__7480a390cb0fd543_arg_names, 3, 0, nullptr, 57346, TypeSize<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::size, UINT64_C(0x7480a390cb0fd543) };
TypeInfo __type_info__b02bad0282fed564 = { Type::tStructure, &__struct_info__c993e285a04c692f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<spoof::SpoofInvocation>::size, UINT64_C(0xb02bad0282fed564) };
TypeInfo __type_info__b18f2b15460317a1 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__71e1112747ff8db0, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<AutoTuple<char *,char *>>>::size, UINT64_C(0xb18f2b15460317a1) };
TypeInfo __type_info__df1b7db3f566d157 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__554236004213764e, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<peg::ParsingError>>::size, UINT64_C(0xdf1b7db3f566d157) };
TypeInfo * __type_info__35f44e4f93af85ad_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__8f14d918b6786dec, &__type_info__af63e44c8601fa24 };
const char * __type_info__35f44e4f93af85ad_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__35f44e4f93af85ad = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__35f44e4f93af85ad_arg_types, __type_info__35f44e4f93af85ad_arg_names, 3, 0, nullptr, 57346, TypeSize<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::size, UINT64_C(0x35f44e4f93af85ad) };
TypeInfo * __type_info__ab24c91193d6fa43_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__243e11abf1d63530, &__type_info__af63e44c8601fa24 };
const char * __type_info__ab24c91193d6fa43_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__ab24c91193d6fa43 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__ab24c91193d6fa43_arg_types, __type_info__ab24c91193d6fa43_arg_names, 3, 0, nullptr, 57346, TypeSize<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::size, UINT64_C(0xab24c91193d6fa43) };
TypeInfo __type_info__662f8256d51be64 = { Type::tStructure, &__struct_info__c993e285a04c692f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<spoof::SpoofInvocation>::size, UINT64_C(0x662f8256d51be64) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint8_t>::size, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<void>::size, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__29a1dd55fa61e9e3 };
TypeInfo * __tinfo_1[1] = { &__type_info__9e830744959095a7 };
TypeInfo * __tinfo_2[1] = { &__type_info__df1b7db3f566d157 };
TypeInfo * __tinfo_3[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_4[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_5[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_6[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_7[1] = { &__type_info__1bcaf53bc9823a03 };
TypeInfo * __tinfo_8[1] = { &__type_info__a49d33b75bf04f48 };
TypeInfo * __tinfo_9[1] = { &__type_info__26703dd8492d36b8 };
TypeInfo * __tinfo_10[1] = { &__type_info__a9e49a99cab89d4 };
TypeInfo * __tinfo_11[1] = { &__type_info__9293ad0d07b1dbee };
TypeInfo * __tinfo_12[1] = { &__type_info__5eddda99b960ff80 };
TypeInfo * __tinfo_13[1] = { &__type_info__9e169205ce7610ad };
TypeInfo * __tinfo_14[1] = { &__type_info__35f44e4f93af85ad };
TypeInfo * __tinfo_15[1] = { &__type_info__6ad1eba4ab633b52 };
TypeInfo * __tinfo_16[1] = { &__type_info__b18f2b15460317a1 };
TypeInfo * __tinfo_17[1] = { &__type_info__7480a390cb0fd543 };
TypeInfo * __tinfo_18[1] = { &__type_info__ab24c91193d6fa43 };
TypeInfo * __tinfo_19[1] = { &__type_info__b02bad0282fed564 };
TypeInfo * __tinfo_20[1] = { &__type_info__662f8256d51be64 };
TypeInfo * __tinfo_21[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_22[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_23[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_24[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_25[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_26[2] = { &__type_info__37d36026a6078a42, &__type_info__fa593d0882a72913 };
TypeInfo * __tinfo_27[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_28[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_29[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_30[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_31[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_32[2] = { &__type_info__37d36026a6078a42, &__type_info__fa593d0882a72913 };
TypeInfo * __tinfo_33[2] = { &__type_info__37d36026a6078a42, &__type_info__66686027eaf8675d };
TypeInfo * __tinfo_34[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_35[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_36[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_37[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_38[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_39[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_40[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_41[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_42[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_43[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_44[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_45[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_46[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_47[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_48[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_49[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_50[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_51[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_52[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_53[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_54[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_55[3] = { &__type_info__af90fe4c864e9d52, &__type_info__7d42bfacbd731b56, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_56[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_57[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_58[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_59[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_60[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_61[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_62[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_63[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_64[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_65[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_66[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_67[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_68[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_69[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_70[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_71[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_72[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_73[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_74[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_75[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_76[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_77[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_78[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_79[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_80[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_81[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_82[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_83[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_84[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_85[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_86[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_87[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_88[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_89[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_90[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_91[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_92[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_93[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_a031df4a8cee6ee5 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_1234_0 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_24797d4c671a586b ( Context * __context__, TArray<char *> & __a_rename_at_1234_2 );
inline void finalize_4560c1849acb38a ( Context * __context__, AutoTuple<char *,char *> & ____this_rename_at_1249_3 );
inline void finalize_2467e182b2a5ed ( Context * __context__, spoof::SpoofInvocation & ____this_rename_at_174_4 );
inline void finalize_6eafafc4b7d66260 ( Context * __context__, AutoTuple<bool,AutoTuple<char *,char *>,int32_t> & ____this_rename_at_1249_5 );
inline void finalize_d046fa145241cec3 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & ____this_rename_at_1249_6 );
inline void finalize_2113affcf78b6161 ( Context * __context__, AutoTuple<bool,char *,int32_t> & ____this_rename_at_1249_7 );
inline void finalize_bfd4d91dc00fbaf3 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & ____this_rename_at_1249_8 );
inline void finalize_da6556421276590d ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> & ____this_rename_at_1249_9 );
inline void finalize_3936267e0c2d5eea ( Context * __context__, peg::ParsingError & ____this_rename_at_18_10 );
inline Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_29780fffd82925e8 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> & __a_rename_at_1202_11 );
inline Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_a95cfb4f229339b9 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> & __a_rename_at_1202_13 );
inline Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_b3588b00b43291d7 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> & __a_rename_at_1202_15 );
inline Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_f64f6a4f9b43e8a8 ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> & __a_rename_at_1202_17 );
inline Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_740b5ec56415e23e ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> & __a_rename_at_1202_19 );
inline TArray<peg::ParsingError> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e4c24aaee0c3ba6b ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_50_21 );
inline void _FuncbuiltinTickpushTick10769833213962245646_cddee3e689dc39ad ( Context * __context__, TArray<ast::AstReaderMacro *> & __Arr_rename_at_181_22, ast::AstReaderMacro * __value_rename_at_181_23 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a825660656018503 ( Context * __context__, spoof::SpoofTemplateReader const  & __cl_rename_at_116_24 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_90a84ecbbf0abd76 ( Context * __context__, spoof::SpoofInstanceReader const  & __cl_rename_at_116_25 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_6e9e07c7cf5371d5 ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_32_26, TArray<peg::ParsingError> & __b_rename_at_32_27 );
inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_cee423d5a5783ec0 ( Context * __context__, TArray<peg::ParsingError> & __Arr_rename_at_377_28, peg::ParsingError & __value_rename_at_377_29 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_77565a85d51f915c ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_30, int32_t __newSize_rename_at_68_31 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_55f52f0e88d35c62 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_68_32, int32_t __newSize_rename_at_68_33 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_2bedf3e77ea4c9f3 ( Context * __context__, TArray<char *> & __Arr_rename_at_68_34, int32_t __newSize_rename_at_68_35 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_e963ce4a12221ab9 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1234_36 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_9c1054907b56811d ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_1234_37 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_29d078faaa5f9fe2 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> & __a_rename_at_1245_39 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_fd098149e3857e1c ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> & __a_rename_at_1245_41 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> & __a_rename_at_1245_43 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_faa91cbe8a8ef152 ( Context * __context__, TArray<bool> & __a_rename_at_1234_45 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_13a83f3db63ddadc ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> & __a_rename_at_1245_46 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_2cd7253cd041b44f ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> & __a_rename_at_1245_48 );
inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_5ab31b7ac47de8d4 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> const  & __clone_src_rename_at_1089_50 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_76164b501fa5f2f ( Context * __context__, AutoTuple<bool,AutoTuple<char *,char *>,int32_t> const  & __clone_src_rename_at_1089_52 );
inline AutoTuple<bool,char *,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb ( Context * __context__, AutoTuple<bool,char *,int32_t> const  & __clone_src_rename_at_1089_54 );
inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_d766db4c7472c995 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> const  & __clone_src_rename_at_1089_56 );
inline AutoTuple<bool,TArray<char *>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_85d66a6acd78b35 ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> const  & __clone_src_rename_at_1089_58 );
inline void _FuncpegTicktabulateTick3664919181896551618_e0b31bc236602422 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_59_60 );
inline char * _FunccolorsTickblue_strTick419715327456344553_85edb7c1543a3a15 ( Context * __context__, char * const  __str_rename_at_26_61, bool __color_rename_at_26_62 );
inline char * _FunccolorsTickgreen_strTick14807051613123094884_f38a76efee9c8c27 ( Context * __context__, char * const  __str_rename_at_18_63, bool __color_rename_at_18_64 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_60cc482d7210f02c ( Context * __context__, TArray<bool> & __Arr_rename_at_68_65, int32_t __newSize_rename_at_68_66 );
inline bool _FuncpegTickreached_EOFTick12193995001429396764_6454e47d0dd7b97f ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_83_67 );
inline void _FuncpegTicktabulateTick3664919181896551618_703c893d8b441b44 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_59_68 );
inline bool _FuncpegTickreached_EOFTick12193995001429396764_63a321d796086836 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_83_69 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_d23c2e725f60c80a ( Context * __context__, TArray<peg::ParsingError> const  & __a_rename_at_585_70 );
inline void _FuncbuiltinTicksortTick14088969635007481283_b6071e2909e46233 ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_1569_71 );
inline TArray<peg::ParsingError> _FuncalgorithmTickuniqueTick8642070526899511001_ea2881ba9416889f ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_11_74 );
inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5ebbb8f8a79624b3 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & __a_rename_at_50_81 );
inline TArray<AutoTuple<char *,char *>> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_d086567e061861cd ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_50_82 );
inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1516aad463652b16 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & __a_rename_at_50_83 );
inline spoof::SpoofInvocation & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_eca34382a0b7791d ( Context * __context__, spoof::SpoofInvocation & __a_rename_at_50_84 );
inline AutoTuple<bool,TArray<char *>,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_59843c375840fe45 ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> & __a_rename_at_50_85 );
inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_70ca5f66113685e1 ( Context * __context__, TArray<char *> & __a_rename_at_50_86 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_27d3da7beba8a3da ( Context * __context__, TArray<uint8_t> & __a_rename_at_1113_87, TArray<uint8_t> const  & __b_rename_at_1113_88 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_abfd1b004e2908bd ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & __a_rename_at_32_90, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & __b_rename_at_32_91 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_ce9078775cda6b21 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_32_92, TArray<AutoTuple<char *,char *>> & __b_rename_at_32_93 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_718c134fc6616938 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & __a_rename_at_32_94, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & __b_rename_at_32_95 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_213995c2dfdbcf59 ( Context * __context__, spoof::SpoofInvocation & __a_rename_at_32_96, spoof::SpoofInvocation & __b_rename_at_32_97 );
inline void clone_c4225ec7818fe341 ( Context * __context__, smart_ptr_raw<Variable> & __dest_rename_at_270_98, smart_ptr_raw<Variable> const  __src_rename_at_270_99 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_ae9c07db44db96c6 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_1113_100, TArray<AutoTuple<char *,char *>> const  & __b_rename_at_1113_101 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_7f476b9128488531 ( Context * __context__, TArray<char *> & __a_rename_at_1113_107, TArray<char *> const  & __b_rename_at_1113_108 );
inline void _FuncpegTicklog_infoTick4618819779667933862_ab23a3e65f4f603 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_78_114, char * __message_rename_at_78_115 );
inline void _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_91_116 );
inline void _FuncpegTicklog_plainTick18061110541707930728_ad4b70a81f1d0180 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_63_117, char * __message_rename_at_63_118 );
inline bool _FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_34_119, char * const  __template__rename_at_34_120, int32_t __strlen_rename_at_34_121 );
inline void _FuncpegTicklog_failTick11731645277795022368_67176c2832debc0c ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_68_123, char * __message_rename_at_68_124 );
inline bool & _FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe ( Context * __context__, TArray<bool> & __a_rename_at_473_125 );
inline char * _FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d ( Context * __context__, char * const  __str_rename_at_14_127, bool __color_rename_at_14_128 );
inline void _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad ( Context * __context__, TArray<peg::ParsingError> & __Arr_rename_at_181_129, peg::ParsingError & __value_rename_at_181_130 );
inline void _FuncpegTicklog_successTick3645718276917011819_88fd6ad4e27a61ae ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_73_131, char * __message_rename_at_73_132 );
inline void _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752 ( Context * __context__, TArray<bool> & __Arr_rename_at_165_133, bool __value_rename_at_165_134 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_b7cb402cbd718488 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_287_135, AutoTuple<char *,char *> & __value_rename_at_287_136 );
inline void _FuncbuiltinTickpopTick1161079256290593740_e59f84a1c28ac19 ( Context * __context__, TArray<bool> & __Arr_rename_at_132_137 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5bc44f666511c4f4 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_181_138, AutoTuple<char *,char *> & __value_rename_at_181_139 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_e89937f359ff4105 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> const  & __Tab_rename_at_1047_140, int32_t __at_rename_at_1047_141 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_31e87ae3c763efa9 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> const  & __Tab_rename_at_1047_142, int32_t __at_rename_at_1047_143 );
inline int32_t _FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_103_144 );
inline void _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_54_145, int32_t __offset_rename_at_54_146 );
inline AutoTuple<bool,char *> _FuncpegTickmatch_string_literalTick12800953725978677998_50b323a2328ca30f ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_144_147 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> const  & __Tab_rename_at_1047_150, int32_t __at_rename_at_1047_151 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_2d94a9a555d8bee7 ( Context * __context__, TArray<char *> & __Arr_rename_at_287_152, char * & __value_rename_at_287_153 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_16b92910c086970e ( Context * __context__, TArray<char *> const  & __it_rename_at_22_154, char * const  __separator_rename_at_22_155 );
inline void _FuncpegTicklog_infoTick4618819779667933862_3fb191919d34d5c0 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_78_160, char * __message_rename_at_78_161 );
inline void _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_91_162 );
inline void _FuncpegTicklog_plainTick18061110541707930728_391a37924ab3bff7 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_63_163, char * __message_rename_at_63_164 );
inline bool _FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_34_165, char * const  __template__rename_at_34_166, int32_t __strlen_rename_at_34_167 );
inline void _FuncpegTicklog_failTick11731645277795022368_8369bcfc7f20004f ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_68_169, char * __message_rename_at_68_170 );
inline void _FuncpegTicklog_successTick3645718276917011819_e4f6b21e9f6afcc5 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_73_171, char * __message_rename_at_73_172 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_18bc2a30ed120a5 ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> const  & __Tab_rename_at_1047_173, int32_t __at_rename_at_1047_174 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_c6163711697a67ea ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> const  & __Tab_rename_at_1047_175, int32_t __at_rename_at_1047_176 );
inline int32_t _FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_103_177 );
inline void _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_54_178, int32_t __offset_rename_at_54_179 );
inline AutoTuple<bool,char *> _FuncpegTickmatch_string_literalTick12800953725978677998_5536b27e9ac361f3 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_144_180 );
inline void _FuncalgorithmTicksort_uniqueTick14985796887508933377_bf6db5262aa69fa5 ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_24_183 );
inline void finalize_f896e7308b8984f3 ( Context * __context__, spoof::invocationParserTickid_0x0 & ____this_rename_at_0_185 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_8958ad763c1a8045 ( Context * __context__, TArray<peg::ParsingError> const  & __it_rename_at_22_186, char * const  __separator_rename_at_22_187 );
inline void finalize_593ca97eb449449f ( Context * __context__, spoof::invocationParserTickid_0x1 & ____this_rename_at_0_192 );
inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _Funcparse_element_list_innerTickid_0x0_afef4622973f1478 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_193 );
inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _Funcparse_element_listTickid_0x0_e08aaeacf4bca4a2 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_208 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_comma_separated_elements_innerTickid_0x0_fc0f629550ec98ab ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_212 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_comma_separated_elementsTickid_0x0_3517a5e2f72086c7 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_221 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_element_innerTickid_0x0_c8e5e2bfd35033ea ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_225 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_elementTickid_0x0_a89929d20275ecb4 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_242 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_element_innerTickid_0x0_4416149c422be215 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_246 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_elementTickid_0x0_aac556ebc4770900 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_299 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_element_value_innerTickid_0x0_4a321894fbeec2f ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_303 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_element_valueTickid_0x0_126b58a15c99cc72 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_314 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_ident__innerTickid_0x0_1def850e72a70e16 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_318 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_ident_Tickid_0x0_ac07755a3c8869f1 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_338 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_76bd142a6cc93fe5 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __it_rename_at_1275_342, int32_t & __value_rename_at_1275_343 );
inline void _FuncbuiltinTickpushTick14133213201864676143_b295659320d5aa3e ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_344, uint8_t __value_rename_at_165_345 );
inline void _FuncbuiltinTickpushTick14133213201864676143_b5e1593e39a49db7 ( Context * __context__, TArray<char *> & __Arr_rename_at_165_346, char * const  __value_rename_at_165_347 );
inline void _FuncbuiltinTickpushTick10769833213962245646_86ba7065cbab662e ( Context * __context__, TArray<char *> & __Arr_rename_at_181_348, char * __value_rename_at_181_349 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_67ac71029b5249f8 ( Context * __context__, char * const  __str_rename_at_1308_350 );
inline int32_t _FuncbuiltinTickfind_index_ifTick5144745413123287381_36aadb1599a6d4e8 ( Context * __context__, TArray<AutoTuple<char *,char *>> const  & __arr_rename_at_1728_352, Block DAS_COMMENT((bool,AutoTuple<char *,char *> const )) const  & __blk_rename_at_1728_353 );
inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _Funcparse_macro_invocation_innerTickid_0x1_5bd6ef28378dc241 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_355 );
inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _Funcparse_macro_invocationTickid_0x1_b47e0c17f58865ea ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_378 );
inline AutoTuple<bool,TArray<char *>,int32_t> _Funcparse_element_list_innerTickid_0x1_ee8dc9dce2c72dd4 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_382 );
inline AutoTuple<bool,TArray<char *>,int32_t> _Funcparse_element_listTickid_0x1_7d5844c12f276ee3 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_393 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_comma_separated_elements_innerTickid_0x1_35f6d9d44c93b4da ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_397 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_comma_separated_elementsTickid_0x1_72b43df88b54209c ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_406 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_element_innerTickid_0x1_3ffc02542601bf64 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_410 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_elementTickid_0x1_70d6c8e08871cfa1 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_463 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_element_innerTickid_0x1_ab5d94b9909bbf99 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_467 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_elementTickid_0x1_a4daaa8f79cfa9ed ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_478 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_class_name_innerTickid_0x1_8485552d58457ba0 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_482 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_class_nameTickid_0x1_ea9ce584f39ea618 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_503 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_ident__innerTickid_0x1_2c1a7fef6a8c065e ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_507 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_ident_Tickid_0x1_ba14b1ca3a3dda32 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_527 );
inline Program * _FuncbuiltinTickget_ptrTick8468476673553620226_5f0e0f646d69bfc7 ( Context * __context__, smart_ptr_raw<Program> const  __src_rename_at_1778_531 );
inline void instance_args_f6e03da14c477f63 ( Context * __context__, char * const  __invocation_rename_at_179_532, Block DAS_COMMENT((void,spoof::SpoofInvocation,TArray<peg::ParsingError> const )) const  & __blk_rename_at_179_533 );
inline void clone_e6aaad296469300e ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & __dest_rename_at_170_539, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> const  & __src_rename_at_170_540 );
inline void clone_fd9d2e92ebaff162 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & __dest_rename_at_170_541, AutoTuple<bool,spoof::SpoofInvocation,int32_t> const  & __src_rename_at_170_542 );
inline void clone_7eaba744dd27b4f ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> & __dest_rename_at_170_543, AutoTuple<bool,TArray<char *>,int32_t> const  & __src_rename_at_170_544 );
inline void clone_4208deb8a0b826d6 ( Context * __context__, spoof::SpoofInvocation & __a_rename_at_174_545, spoof::SpoofInvocation const  & __b_rename_at_174_546 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0x68d557c9065bdca7>(__context__) = 37;/*SPOOF_SPEFICAL*/
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_a031df4a8cee6ee5 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_1234_0 )
{
    {
        bool __need_loop_1236 = true;
        // aV: tuple<string;string> aka TT&
        das_iterator<TArray<AutoTuple<char *,char *>>> __aV_iterator(__a_rename_at_1234_0);
        AutoTuple<char *,char *> * __aV_rename_at_1236_1;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_1)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_1)) )
        {
            finalize_4560c1849acb38a(__context__,das_arg<AutoTuple<char *,char *>>::pass((*__aV_rename_at_1236_1)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_1));
    };
    builtin_array_free(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__a_rename_at_1234_0),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_24797d4c671a586b ( Context * __context__, TArray<char *> &  __a_rename_at_1234_2 )
{
    builtin_array_free(das_arg<TArray<char *>>::pass(__a_rename_at_1234_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_4560c1849acb38a ( Context * __context__, AutoTuple<char *,char *> &  ____this_rename_at_1249_3 )
{
    memset((void*)&(____this_rename_at_1249_3), 0, TypeSize<AutoTuple<char *,char *>>::size);
}

inline void finalize_2467e182b2a5ed ( Context * __context__, spoof::SpoofInvocation &  ____this_rename_at_174_4 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_24797d4c671a586b(__context__,das_arg<TArray<char *>>::pass(____this_rename_at_174_4.args));
    memset((void*)&(____this_rename_at_174_4), 0, TypeSize<spoof::SpoofInvocation>::size);
}

inline void finalize_6eafafc4b7d66260 ( Context * __context__, AutoTuple<bool,AutoTuple<char *,char *>,int32_t> &  ____this_rename_at_1249_5 )
{
    finalize_4560c1849acb38a(__context__,das_arg<AutoTuple<char *,char *>>::pass(das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(____this_rename_at_1249_5)));
    memset((void*)&(____this_rename_at_1249_5), 0, TypeSize<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::size);
}

inline void finalize_d046fa145241cec3 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  ____this_rename_at_1249_6 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_a031df4a8cee6ee5(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(das_get_auto_tuple_field<TArray<AutoTuple<char *,char *>>,1,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(____this_rename_at_1249_6)));
    memset((void*)&(____this_rename_at_1249_6), 0, TypeSize<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::size);
}

inline void finalize_2113affcf78b6161 ( Context * __context__, AutoTuple<bool,char *,int32_t> &  ____this_rename_at_1249_7 )
{
    memset((void*)&(____this_rename_at_1249_7), 0, TypeSize<AutoTuple<bool,char *,int32_t>>::size);
}

inline void finalize_bfd4d91dc00fbaf3 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  ____this_rename_at_1249_8 )
{
    finalize_2467e182b2a5ed(__context__,das_arg<spoof::SpoofInvocation>::pass(das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(____this_rename_at_1249_8)));
    memset((void*)&(____this_rename_at_1249_8), 0, TypeSize<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::size);
}

inline void finalize_da6556421276590d ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> &  ____this_rename_at_1249_9 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_24797d4c671a586b(__context__,das_arg<TArray<char *>>::pass(das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(____this_rename_at_1249_9)));
    memset((void*)&(____this_rename_at_1249_9), 0, TypeSize<AutoTuple<bool,TArray<char *>,int32_t>>::size);
}

inline void finalize_3936267e0c2d5eea ( Context * __context__, peg::ParsingError &  ____this_rename_at_18_10 )
{
    memset((void*)&(____this_rename_at_18_10), 0, TypeSize<peg::ParsingError>::size);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_29780fffd82925e8 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> &  __a_rename_at_1202_11 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> *)) __it_rename_at_1203_12;das_zero(__it_rename_at_1203_12);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t>))>::pass(__it_rename_at_1203_12),das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__a_rename_at_1202_11),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> &))>::cast(__it_rename_at_1203_12);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_a95cfb4f229339b9 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> &  __a_rename_at_1202_13 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> *)) __it_rename_at_1203_14;das_zero(__it_rename_at_1203_14);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>))>::pass(__it_rename_at_1203_14),das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(__a_rename_at_1202_13),40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &))>::cast(__it_rename_at_1203_14);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_b3588b00b43291d7 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> &  __a_rename_at_1202_15 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> *)) __it_rename_at_1203_16;das_zero(__it_rename_at_1203_16);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t>))>::pass(__it_rename_at_1203_16),das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__a_rename_at_1202_15),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> &))>::cast(__it_rename_at_1203_16);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_f64f6a4f9b43e8a8 ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> &  __a_rename_at_1202_17 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> *)) __it_rename_at_1203_18;das_zero(__it_rename_at_1203_18);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t>))>::pass(__it_rename_at_1203_18),das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(__a_rename_at_1202_17),48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> &))>::cast(__it_rename_at_1203_18);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_740b5ec56415e23e ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> &  __a_rename_at_1202_19 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> *)) __it_rename_at_1203_20;das_zero(__it_rename_at_1203_20);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t>))>::pass(__it_rename_at_1203_20),das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(__a_rename_at_1202_19),40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> &))>::cast(__it_rename_at_1203_20);
}

inline TArray<peg::ParsingError> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e4c24aaee0c3ba6b ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_50_21 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<peg::ParsingError> &>::from(__a_rename_at_50_21)));
    return das_auto_cast_ref<TArray<peg::ParsingError> &>::cast(__a_rename_at_50_21);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_cddee3e689dc39ad ( Context * __context__, TArray<ast::AstReaderMacro *> &  __Arr_rename_at_181_22, ast::AstReaderMacro * __value_rename_at_181_23 )
{
    das_copy(__Arr_rename_at_181_22(builtin_array_push_back(das_arg<TArray<ast::AstReaderMacro *>>::pass(__Arr_rename_at_181_22),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_23);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a825660656018503 ( Context * __context__, spoof::SpoofTemplateReader const  &  __cl_rename_at_116_24 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_24.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_90a84ecbbf0abd76 ( Context * __context__, spoof::SpoofInstanceReader const  &  __cl_rename_at_116_25 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_25.__rtti))).getStructType())));
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_6e9e07c7cf5371d5 ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_32_26, TArray<peg::ParsingError> &  __b_rename_at_32_27 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TArray<peg::ParsingError> &>::from(__a_rename_at_32_26)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_2,cast<TArray<peg::ParsingError> &>::from(__b_rename_at_32_27)));
    das_move(__a_rename_at_32_26,__b_rename_at_32_27);
}

inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_cee423d5a5783ec0 ( Context * __context__, TArray<peg::ParsingError> &  __Arr_rename_at_377_28, peg::ParsingError &  __value_rename_at_377_29 )
{
    das_copy(__Arr_rename_at_377_28(builtin_array_push_back_zero(das_arg<TArray<peg::ParsingError>>::pass(__Arr_rename_at_377_28),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_377_29);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_77565a85d51f915c ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_30, int32_t __newSize_rename_at_68_31 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_30),__newSize_rename_at_68_31,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_55f52f0e88d35c62 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_68_32, int32_t __newSize_rename_at_68_33 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_68_32),__newSize_rename_at_68_33,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_2bedf3e77ea4c9f3 ( Context * __context__, TArray<char *> &  __Arr_rename_at_68_34, int32_t __newSize_rename_at_68_35 )
{
    builtin_array_resize(das_arg<TArray<char *>>::pass(__Arr_rename_at_68_34),__newSize_rename_at_68_35,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_e963ce4a12221ab9 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1234_36 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1234_36),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_9c1054907b56811d ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_1234_37 )
{
    {
        bool __need_loop_1236 = true;
        // aV: peg::ParsingError aka TT&
        das_iterator<TArray<peg::ParsingError>> __aV_iterator(__a_rename_at_1234_37);
        peg::ParsingError * __aV_rename_at_1236_38;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_38)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_38)) )
        {
            finalize_3936267e0c2d5eea(__context__,das_arg<peg::ParsingError>::pass((*__aV_rename_at_1236_38)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_38));
    };
    builtin_array_free(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_1234_37),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_29d078faaa5f9fe2 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> &  __a_rename_at_1245_39 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> *)) _temp_make_local_1247_19_0; _temp_make_local_1247_19_0;
    {
        bool __need_loop_1247 = true;
        // aV: tuple<success:bool;value:tuple<string;string> -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t>))> __aV_iterator((_temp_make_local_1247_19_0 = (_FuncbuiltinTickvaluesTick1351216622833168869_29780fffd82925e8(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__a_rename_at_1245_39)))));
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> * __aV_rename_at_1247_40;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_40)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_40)) )
        {
            finalize_6eafafc4b7d66260(__context__,das_arg<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::pass((*__aV_rename_at_1247_40)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_40));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__a_rename_at_1245_39),4,32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_fd098149e3857e1c ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> &  __a_rename_at_1245_41 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> *)) _temp_make_local_1247_19_1; _temp_make_local_1247_19_1;
    {
        bool __need_loop_1247 = true;
        // aV: tuple<success:bool;value:array<tuple<string;string>> -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>))> __aV_iterator((_temp_make_local_1247_19_1 = (_FuncbuiltinTickvaluesTick1351216622833168869_a95cfb4f229339b9(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(__a_rename_at_1245_41)))));
        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> * __aV_rename_at_1247_42;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_42)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_42)) )
        {
            finalize_d046fa145241cec3(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass((*__aV_rename_at_1247_42)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_42));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(__a_rename_at_1245_41),4,40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> &  __a_rename_at_1245_43 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> *)) _temp_make_local_1247_19_2; _temp_make_local_1247_19_2;
    {
        bool __need_loop_1247 = true;
        // aV: tuple<success:bool;value:string -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t>))> __aV_iterator((_temp_make_local_1247_19_2 = (_FuncbuiltinTickvaluesTick1351216622833168869_b3588b00b43291d7(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__a_rename_at_1245_43)))));
        AutoTuple<bool,char *,int32_t> * __aV_rename_at_1247_44;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_44)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_44)) )
        {
            finalize_2113affcf78b6161(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass((*__aV_rename_at_1247_44)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_44));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__a_rename_at_1245_43),4,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_faa91cbe8a8ef152 ( Context * __context__, TArray<bool> &  __a_rename_at_1234_45 )
{
    builtin_array_free(das_arg<TArray<bool>>::pass(__a_rename_at_1234_45),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_13a83f3db63ddadc ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> &  __a_rename_at_1245_46 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> *)) _temp_make_local_1247_19_3; _temp_make_local_1247_19_3;
    {
        bool __need_loop_1247 = true;
        // aV: tuple<success:bool;value:spoof::SpoofInvocation -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t>))> __aV_iterator((_temp_make_local_1247_19_3 = (_FuncbuiltinTickvaluesTick1351216622833168869_f64f6a4f9b43e8a8(__context__,das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(__a_rename_at_1245_46)))));
        AutoTuple<bool,spoof::SpoofInvocation,int32_t> * __aV_rename_at_1247_47;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_47)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_47)) )
        {
            finalize_bfd4d91dc00fbaf3(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass((*__aV_rename_at_1247_47)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_47));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(__a_rename_at_1245_46),4,48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_2cd7253cd041b44f ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> &  __a_rename_at_1245_48 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> *)) _temp_make_local_1247_19_4; _temp_make_local_1247_19_4;
    {
        bool __need_loop_1247 = true;
        // aV: tuple<success:bool;value:array<string> -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t>))> __aV_iterator((_temp_make_local_1247_19_4 = (_FuncbuiltinTickvaluesTick1351216622833168869_740b5ec56415e23e(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(__a_rename_at_1245_48)))));
        AutoTuple<bool,TArray<char *>,int32_t> * __aV_rename_at_1247_49;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_49)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_49)) )
        {
            finalize_da6556421276590d(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass((*__aV_rename_at_1247_49)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_49));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(__a_rename_at_1245_48),4,40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_5ab31b7ac47de8d4 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> const  &  __clone_src_rename_at_1089_50 )
{
    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __clone_dest_rename_at_1091_51;das_zero(__clone_dest_rename_at_1091_51);
    clone_e6aaad296469300e(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass(__clone_dest_rename_at_1091_51),__clone_src_rename_at_1089_50);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(__clone_dest_rename_at_1091_51);
}

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_76164b501fa5f2f ( Context * __context__, AutoTuple<bool,AutoTuple<char *,char *>,int32_t> const  &  __clone_src_rename_at_1089_52 )
{
    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __clone_dest_rename_at_1091_53;das_zero(__clone_dest_rename_at_1091_53);
    das_copy(__clone_dest_rename_at_1091_53,__clone_src_rename_at_1089_52);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__clone_dest_rename_at_1091_53);
}

inline AutoTuple<bool,char *,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb ( Context * __context__, AutoTuple<bool,char *,int32_t> const  &  __clone_src_rename_at_1089_54 )
{
    AutoTuple<bool,char *,int32_t> __clone_dest_rename_at_1091_55;das_zero(__clone_dest_rename_at_1091_55);
    das_copy(__clone_dest_rename_at_1091_55,__clone_src_rename_at_1089_54);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__clone_dest_rename_at_1091_55);
}

inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_d766db4c7472c995 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> const  &  __clone_src_rename_at_1089_56 )
{
    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __clone_dest_rename_at_1091_57;das_zero(__clone_dest_rename_at_1091_57);
    clone_fd9d2e92ebaff162(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__clone_dest_rename_at_1091_57),__clone_src_rename_at_1089_56);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__clone_dest_rename_at_1091_57);
}

inline AutoTuple<bool,TArray<char *>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_85d66a6acd78b35 ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> const  &  __clone_src_rename_at_1089_58 )
{
    AutoTuple<bool,TArray<char *>,int32_t> __clone_dest_rename_at_1091_59;das_zero(__clone_dest_rename_at_1091_59);
    clone_7eaba744dd27b4f(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass(__clone_dest_rename_at_1091_59),__clone_src_rename_at_1089_58);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(__clone_dest_rename_at_1091_59);
}

inline void _FuncpegTicktabulateTick3664919181896551618_e0b31bc236602422 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_59_60 )
{
    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_3, cast<char * const >::from(((char * const )(string_repeat(((char *) "|   "),__parser_rename_at_59_60.tabs,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FunccolorsTickblue_strTick419715327456344553_85edb7c1543a3a15 ( Context * __context__, char * const  __str_rename_at_26_61, bool __color_rename_at_26_62 )
{
    return das_auto_cast<char *>::cast(__color_rename_at_26_62 ? das_auto_cast<char * const >::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_4, cast<char * const >::from(((char *) "\u001b[34m")), cast<char * const >::from(__str_rename_at_26_61), cast<char * const >::from(((char *) "\u001b[0m"))))) : das_auto_cast<char * const >::cast(__str_rename_at_26_61));
}

inline char * _FunccolorsTickgreen_strTick14807051613123094884_f38a76efee9c8c27 ( Context * __context__, char * const  __str_rename_at_18_63, bool __color_rename_at_18_64 )
{
    return das_auto_cast<char *>::cast(__color_rename_at_18_64 ? das_auto_cast<char * const >::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_5, cast<char * const >::from(((char *) "\u001b[32m")), cast<char * const >::from(__str_rename_at_18_63), cast<char * const >::from(((char *) "\u001b[0m"))))) : das_auto_cast<char * const >::cast(__str_rename_at_18_63));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_60cc482d7210f02c ( Context * __context__, TArray<bool> &  __Arr_rename_at_68_65, int32_t __newSize_rename_at_68_66 )
{
    builtin_array_resize(das_arg<TArray<bool>>::pass(__Arr_rename_at_68_65),__newSize_rename_at_68_66,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncpegTickreached_EOFTick12193995001429396764_6454e47d0dd7b97f ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_83_67 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_83_67.input)) <= __parser_rename_at_83_67.index);
}

inline void _FuncpegTicktabulateTick3664919181896551618_703c893d8b441b44 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_59_68 )
{
    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_6, cast<char * const >::from(((char * const )(string_repeat(((char *) "|   "),__parser_rename_at_59_68.tabs,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncpegTickreached_EOFTick12193995001429396764_63a321d796086836 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_83_69 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_83_69.input)) <= __parser_rename_at_83_69.index);
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_d23c2e725f60c80a ( Context * __context__, TArray<peg::ParsingError> const  &  __a_rename_at_585_70 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_585_70) == 0);
}

inline void _FuncbuiltinTicksortTick14088969635007481283_b6071e2909e46233 ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_1569_71 )
{
    if ( builtin_array_size(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_1569_71)) <= 1 )
    {
        return ;
    } else {
        builtin_sort_array_any_cblock_T(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_1569_71),16,builtin_array_size(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_1569_71)),[&](peg::ParsingError const  & __x_rename_at_1590_72, peg::ParsingError const  & __y_rename_at_1590_73) DAS_AOT_INLINE_LAMBDA -> bool{
            return das_auto_cast<bool>::cast(das_invoke_function<bool>::invoke<peg::ParsingError const  &,peg::ParsingError const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@peg::< CS<peg::ParsingError> CS<peg::ParsingError>*/ 0x1448f6afa9f5f32e)),__x_rename_at_1590_72,__y_rename_at_1590_73));
        },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline TArray<peg::ParsingError> _FuncalgorithmTickuniqueTick8642070526899511001_ea2881ba9416889f ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_11_74 )
{
    int32_t __pidx_rename_at_13_75 = -1;
    TArray<peg::ParsingError> __b_rename_at_14_76;das_zero(__b_rename_at_14_76);
    {
        bool __need_loop_15 = true;
        // e: peg::ParsingError aka TT&
        das_iterator<TArray<peg::ParsingError>> __e_iterator(__a_rename_at_11_74);
        peg::ParsingError * __e_rename_at_15_79;
        __need_loop_15 = __e_iterator.first(__context__,(__e_rename_at_15_79)) && __need_loop_15;
        // eidx: int const
        das_iterator<range> __eidx_iterator(mk_range(builtin_array_size(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_11_74))));
        int32_t __eidx_rename_at_15_80;
        __need_loop_15 = __eidx_iterator.first(__context__,(__eidx_rename_at_15_80)) && __need_loop_15;
        for ( ; __need_loop_15 ; __need_loop_15 = __e_iterator.next(__context__,(__e_rename_at_15_79)) && __eidx_iterator.next(__context__,(__eidx_rename_at_15_80)) )
        {
            if ( (__pidx_rename_at_13_75 == -1) || (das_invoke_function<bool>::invoke<peg::ParsingError const  &,peg::ParsingError const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@peg::!= CS<peg::ParsingError> CS<peg::ParsingError>*/ 0xe34ed31c8ed3fe95)),das_arg<peg::ParsingError>::pass(__a_rename_at_11_74(__pidx_rename_at_13_75,__context__)),das_arg<peg::ParsingError>::pass((*__e_rename_at_15_79)))) )
            {
                das_copy(__pidx_rename_at_13_75,__eidx_rename_at_15_80);
                _FuncbuiltinTickpush_cloneTick2035469273396957942_cee423d5a5783ec0(__context__,das_arg<TArray<peg::ParsingError>>::pass(__b_rename_at_14_76),das_arg<peg::ParsingError>::pass((*__e_rename_at_15_79)));
            };
        }
        __e_iterator.close(__context__,(__e_rename_at_15_79));
        __eidx_iterator.close(__context__,(__eidx_rename_at_15_80));
    };
    return /* <- */ das_auto_cast_move<TArray<peg::ParsingError>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e4c24aaee0c3ba6b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__b_rename_at_14_76)));
}

inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5ebbb8f8a79624b3 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  __a_rename_at_50_81 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_7,cast<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &>::from(__a_rename_at_50_81)));
    return das_auto_cast_ref<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &>::cast(__a_rename_at_50_81);
}

inline TArray<AutoTuple<char *,char *>> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_d086567e061861cd ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_50_82 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_8,cast<TArray<AutoTuple<char *,char *>> &>::from(__a_rename_at_50_82)));
    return das_auto_cast_ref<TArray<AutoTuple<char *,char *>> &>::cast(__a_rename_at_50_82);
}

inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1516aad463652b16 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  __a_rename_at_50_83 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_9,cast<AutoTuple<bool,spoof::SpoofInvocation,int32_t> &>::from(__a_rename_at_50_83)));
    return das_auto_cast_ref<AutoTuple<bool,spoof::SpoofInvocation,int32_t> &>::cast(__a_rename_at_50_83);
}

inline spoof::SpoofInvocation & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_eca34382a0b7791d ( Context * __context__, spoof::SpoofInvocation &  __a_rename_at_50_84 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_10,cast<spoof::SpoofInvocation &>::from(__a_rename_at_50_84)));
    return das_auto_cast_ref<spoof::SpoofInvocation &>::cast(__a_rename_at_50_84);
}

inline AutoTuple<bool,TArray<char *>,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_59843c375840fe45 ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> &  __a_rename_at_50_85 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_11,cast<AutoTuple<bool,TArray<char *>,int32_t> &>::from(__a_rename_at_50_85)));
    return das_auto_cast_ref<AutoTuple<bool,TArray<char *>,int32_t> &>::cast(__a_rename_at_50_85);
}

inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_70ca5f66113685e1 ( Context * __context__, TArray<char *> &  __a_rename_at_50_86 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_12,cast<TArray<char *> &>::from(__a_rename_at_50_86)));
    return das_auto_cast_ref<TArray<char *> &>::cast(__a_rename_at_50_86);
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_27d3da7beba8a3da ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1113_87, TArray<uint8_t> const  &  __b_rename_at_1113_88 )
{
    int32_t __ln_rename_at_1114_89 = ((int32_t)builtin_array_size(__b_rename_at_1113_88));
    _FuncbuiltinTickresizeTick4811697762258667383_77565a85d51f915c(__context__,das_arg<TArray<uint8_t>>::pass(__a_rename_at_1113_87),__ln_rename_at_1114_89);
    if ( __ln_rename_at_1114_89 == 0 )
    {
        return ;
    } else {
        das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__a_rename_at_1113_87(0,__context__))),das_auto_cast<void * const >::cast(das_ref(__context__,__b_rename_at_1113_88(0,__context__))),__ln_rename_at_1114_89 * 1);
    };
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_abfd1b004e2908bd ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  __a_rename_at_32_90, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  __b_rename_at_32_91 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_13,cast<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &>::from(__a_rename_at_32_90)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_14,cast<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &>::from(__b_rename_at_32_91)));
    das_move(__a_rename_at_32_90,__b_rename_at_32_91);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_ce9078775cda6b21 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_32_92, TArray<AutoTuple<char *,char *>> &  __b_rename_at_32_93 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_15,cast<TArray<AutoTuple<char *,char *>> &>::from(__a_rename_at_32_92)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_16,cast<TArray<AutoTuple<char *,char *>> &>::from(__b_rename_at_32_93)));
    das_move(__a_rename_at_32_92,__b_rename_at_32_93);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_718c134fc6616938 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  __a_rename_at_32_94, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  __b_rename_at_32_95 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_17,cast<AutoTuple<bool,spoof::SpoofInvocation,int32_t> &>::from(__a_rename_at_32_94)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_18,cast<AutoTuple<bool,spoof::SpoofInvocation,int32_t> &>::from(__b_rename_at_32_95)));
    das_move(__a_rename_at_32_94,__b_rename_at_32_95);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_213995c2dfdbcf59 ( Context * __context__, spoof::SpoofInvocation &  __a_rename_at_32_96, spoof::SpoofInvocation &  __b_rename_at_32_97 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_19,cast<spoof::SpoofInvocation &>::from(__a_rename_at_32_96)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_20,cast<spoof::SpoofInvocation &>::from(__b_rename_at_32_97)));
    das_move(__a_rename_at_32_96,__b_rename_at_32_97);
}

inline void clone_c4225ec7818fe341 ( Context * __context__, smart_ptr_raw<Variable> & __dest_rename_at_270_98, smart_ptr_raw<Variable> const  __src_rename_at_270_99 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_270_98),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_270_99),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_ae9c07db44db96c6 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_1113_100, TArray<AutoTuple<char *,char *>> const  &  __b_rename_at_1113_101 )
{
    int32_t __ln_rename_at_1114_102 = ((int32_t)builtin_array_size(__b_rename_at_1113_101));
    _FuncbuiltinTickresizeTick4811697762258667383_55f52f0e88d35c62(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__a_rename_at_1113_100),__ln_rename_at_1114_102);
    if ( __ln_rename_at_1114_102 == 0 )
    {
        return ;
    } else {
        {
            bool __need_loop_1124 = true;
            // aV: tuple<string;string> aka TT&
            das_iterator<TArray<AutoTuple<char *,char *>>> __aV_iterator(__a_rename_at_1113_100);
            AutoTuple<char *,char *> * __aV_rename_at_1124_105;
            __need_loop_1124 = __aV_iterator.first(__context__,(__aV_rename_at_1124_105)) && __need_loop_1124;
            // bV: tuple<string;string> const&
            das_iterator<TArray<AutoTuple<char *,char *>> const > __bV_iterator(__b_rename_at_1113_101);
            AutoTuple<char *,char *> const  * __bV_rename_at_1124_106;
            __need_loop_1124 = __bV_iterator.first(__context__,(__bV_rename_at_1124_106)) && __need_loop_1124;
            for ( ; __need_loop_1124 ; __need_loop_1124 = __aV_iterator.next(__context__,(__aV_rename_at_1124_105)) && __bV_iterator.next(__context__,(__bV_rename_at_1124_106)) )
            {
                das_copy((*__aV_rename_at_1124_105),(*__bV_rename_at_1124_106));
            }
            __aV_iterator.close(__context__,(__aV_rename_at_1124_105));
            __bV_iterator.close(__context__,(__bV_rename_at_1124_106));
        };
    };
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_7f476b9128488531 ( Context * __context__, TArray<char *> &  __a_rename_at_1113_107, TArray<char *> const  &  __b_rename_at_1113_108 )
{
    int32_t __ln_rename_at_1114_109 = ((int32_t)builtin_array_size(__b_rename_at_1113_108));
    _FuncbuiltinTickresizeTick4811697762258667383_2bedf3e77ea4c9f3(__context__,das_arg<TArray<char *>>::pass(__a_rename_at_1113_107),__ln_rename_at_1114_109);
    if ( __ln_rename_at_1114_109 == 0 )
    {
        return ;
    } else {
        {
            bool __need_loop_1124 = true;
            // aV: string aka TT&
            das_iterator<TArray<char *>> __aV_iterator(__a_rename_at_1113_107);
            char * * __aV_rename_at_1124_112;
            __need_loop_1124 = __aV_iterator.first(__context__,(__aV_rename_at_1124_112)) && __need_loop_1124;
            // bV: string const&
            das_iterator<TArray<char *> const > __bV_iterator(__b_rename_at_1113_108);
            char * const  * __bV_rename_at_1124_113;
            __need_loop_1124 = __bV_iterator.first(__context__,(__bV_rename_at_1124_113)) && __need_loop_1124;
            for ( ; __need_loop_1124 ; __need_loop_1124 = __aV_iterator.next(__context__,(__aV_rename_at_1124_112)) && __bV_iterator.next(__context__,(__bV_rename_at_1124_113)) )
            {
                das_copy((*__aV_rename_at_1124_112),(*__bV_rename_at_1124_113));
            }
            __aV_iterator.close(__context__,(__aV_rename_at_1124_112));
            __bV_iterator.close(__context__,(__bV_rename_at_1124_113));
        };
    };
}

inline void _FuncpegTicklog_infoTick4618819779667933862_ab23a3e65f4f603 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_78_114, char * __message_rename_at_78_115 )
{
    _FuncpegTicktabulateTick3664919181896551618_e0b31bc236602422(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_78_114));
    builtin_print(_FunccolorsTickblue_strTick419715327456344553_85edb7c1543a3a15(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_21, cast<char * const >::from(((char *) "[.] ")), cast<char *>::from(__message_rename_at_78_115), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_78_114.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_91_116 )
{
    while ( is_white_space(_FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_91_116))) )
    {
        _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_91_116),1);
    };
}

inline void _FuncpegTicklog_plainTick18061110541707930728_ad4b70a81f1d0180 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_63_117, char * __message_rename_at_63_118 )
{
    _FuncpegTicktabulateTick3664919181896551618_e0b31bc236602422(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_63_117));
    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_22, cast<char * const >::from(((char *) "[-] ")), cast<char *>::from(__message_rename_at_63_118), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_34_119, char * const  __template__rename_at_34_120, int32_t __strlen_rename_at_34_121 )
{
    int32_t __mark_rename_at_35_122 = __parser_rename_at_34_119.index;
    if ( builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_34_119.input)) < (__mark_rename_at_35_122 + __strlen_rename_at_34_121) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( das_memcmp(das_auto_cast<void * const >::cast(das_ref(__context__,__parser_rename_at_34_119.input(__mark_rename_at_35_122,__context__))),das_auto_cast<void * const >::cast(das_cast<uint8_t *>::cast(__template__rename_at_34_120)),__strlen_rename_at_34_121) == 0 )
        {
            _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_34_119),__strlen_rename_at_34_121);
            return das_auto_cast<bool>::cast(true);
        } else {
            return das_auto_cast<bool>::cast(false);
        };
    };
}

inline void _FuncpegTicklog_failTick11731645277795022368_67176c2832debc0c ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_68_123, char * __message_rename_at_68_124 )
{
    _FuncpegTicktabulateTick3664919181896551618_e0b31bc236602422(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_68_123));
    builtin_print(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_23, cast<char * const >::from(((char *) "[!] ")), cast<char *>::from(__message_rename_at_68_124), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_68_123.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool & _FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe ( Context * __context__, TArray<bool> &  __a_rename_at_473_125 )
{
    int32_t __l_rename_at_474_126 = ((int32_t)builtin_array_size(das_arg<TArray<bool>>::pass(__a_rename_at_473_125)));
    if ( __l_rename_at_474_126 == 0 )
    {
        builtin_throw(((char *) "back empty array"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast_ref<bool &>::cast(__a_rename_at_473_125((__l_rename_at_474_126 - 1),__context__));
}

inline char * _FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d ( Context * __context__, char * const  __str_rename_at_14_127, bool __color_rename_at_14_128 )
{
    return das_auto_cast<char *>::cast(__color_rename_at_14_128 ? das_auto_cast<char * const >::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_24, cast<char * const >::from(((char *) "\u001b[31m")), cast<char * const >::from(__str_rename_at_14_127), cast<char * const >::from(((char *) "\u001b[0m"))))) : das_auto_cast<char * const >::cast(__str_rename_at_14_127));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad ( Context * __context__, TArray<peg::ParsingError> &  __Arr_rename_at_181_129, peg::ParsingError &  __value_rename_at_181_130 )
{
    das_copy(__Arr_rename_at_181_129(builtin_array_push_back(das_arg<TArray<peg::ParsingError>>::pass(__Arr_rename_at_181_129),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_130);
}

inline void _FuncpegTicklog_successTick3645718276917011819_88fd6ad4e27a61ae ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_73_131, char * __message_rename_at_73_132 )
{
    _FuncpegTicktabulateTick3664919181896551618_e0b31bc236602422(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_73_131));
    builtin_print(_FunccolorsTickgreen_strTick14807051613123094884_f38a76efee9c8c27(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_25, cast<char * const >::from(((char *) "[!] ")), cast<char *>::from(__message_rename_at_73_132), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_73_131.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752 ( Context * __context__, TArray<bool> &  __Arr_rename_at_165_133, bool __value_rename_at_165_134 )
{
    das_copy(__Arr_rename_at_165_133(builtin_array_push_back(das_arg<TArray<bool>>::pass(__Arr_rename_at_165_133),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_134);
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_b7cb402cbd718488 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_287_135, AutoTuple<char *,char *> &  __value_rename_at_287_136 )
{
    das_move(__Arr_rename_at_287_135(builtin_array_push_back(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_287_135),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_136);
}

inline void _FuncbuiltinTickpopTick1161079256290593740_e59f84a1c28ac19 ( Context * __context__, TArray<bool> &  __Arr_rename_at_132_137 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_60cc482d7210f02c(__context__,das_arg<TArray<bool>>::pass(__Arr_rename_at_132_137),builtin_array_size(das_arg<TArray<bool>>::pass(__Arr_rename_at_132_137)) - 1);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5bc44f666511c4f4 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_181_138, AutoTuple<char *,char *> &  __value_rename_at_181_139 )
{
    das_copy(__Arr_rename_at_181_138(builtin_array_push_back(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_181_138),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_139);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_e89937f359ff4105 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> const  &  __Tab_rename_at_1047_140, int32_t __at_rename_at_1047_141 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_140,__at_rename_at_1047_141));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_31e87ae3c763efa9 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> const  &  __Tab_rename_at_1047_142, int32_t __at_rename_at_1047_143 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_142,__at_rename_at_1047_143));
}

inline int32_t _FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_103_144 )
{
    return das_auto_cast<int32_t>::cast((builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_103_144.input)) <= __parser_rename_at_103_144.index) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(int32_t(__parser_rename_at_103_144.input(__parser_rename_at_103_144.index,__context__))));
}

inline void _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_54_145, int32_t __offset_rename_at_54_146 )
{
    __parser_rename_at_54_145.index += __offset_rename_at_54_146;
    das_copy(__parser_rename_at_54_145.longest_prefix,SimPolicy<int32_t>::Max(__parser_rename_at_54_145.longest_prefix,__parser_rename_at_54_145.index,*__context__,nullptr));
}

inline AutoTuple<bool,char *> _FuncpegTickmatch_string_literalTick12800953725978677998_50b323a2328ca30f ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_144_147 )
{
    TArray<uint8_t> __buffer_rename_at_148_148; memset((void*)&__buffer_rename_at_148_148,0,sizeof(__buffer_rename_at_148_148));
    int32_t __current_char_rename_at_150_149; memset((void*)&__current_char_rename_at_150_149,0,sizeof(__current_char_rename_at_150_149));
    /* finally */ auto __finally_144= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_e963ce4a12221ab9(__context__,das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_148));
    /* end finally */ });
    das_zero(__buffer_rename_at_148_148);
    __current_char_rename_at_150_149 = _FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147));
    if ( __current_char_rename_at_150_149 != 34 )
    {
        return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
            AutoTuple<bool,char *> __mkt_153;
            das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_153) = false;
            das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_153) = nullptr;
            return __mkt_153;
        })()));
    } else {
        _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147),1);
        das_copy(__current_char_rename_at_150_149,_FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147)));
        while ( (__current_char_rename_at_150_149 != 34) && !(_FuncpegTickreached_EOFTick12193995001429396764_6454e47d0dd7b97f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147))) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_b295659320d5aa3e(__context__,das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_148),uint8_t(__current_char_rename_at_150_149));
            _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147),1);
            das_copy(__current_char_rename_at_150_149,_FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147)));
        };
        if ( _FuncpegTickreached_EOFTick12193995001429396764_6454e47d0dd7b97f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147)) )
        {
            return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
                AutoTuple<bool,char *> __mkt_165;
                das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_165) = false;
                das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_165) = nullptr;
                return __mkt_165;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147),1);
            return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
                AutoTuple<bool,char *> __mkt_168;
                das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_168) = true;
                das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_168) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_148),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
                return __mkt_168;
            })()));
        };
    };
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> const  &  __Tab_rename_at_1047_150, int32_t __at_rename_at_1047_151 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_150,__at_rename_at_1047_151));
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_2d94a9a555d8bee7 ( Context * __context__, TArray<char *> &  __Arr_rename_at_287_152, char * & __value_rename_at_287_153 )
{
    das_move(__Arr_rename_at_287_152(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_287_152),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_153);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_16b92910c086970e ( Context * __context__, TArray<char *> const  &  __it_rename_at_22_154, char * const  __separator_rename_at_22_155 )
{
    char * __st_rename_at_27_156 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_27_157) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_28_158 = true;
        {
            bool __need_loop_29 = true;
            // elem: string const& -const
            das_iterator<TArray<char *> const > __elem_iterator(__it_rename_at_22_154);
            char * const  * __elem_rename_at_29_159;
            __need_loop_29 = __elem_iterator.first(__context__,(__elem_rename_at_29_159)) && __need_loop_29;
            for ( ; __need_loop_29 ; __need_loop_29 = __elem_iterator.next(__context__,(__elem_rename_at_29_159)) )
            {
                if ( __skip_first_rename_at_28_158 )
                {
                    das_copy(__skip_first_rename_at_28_158,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_26,cast<StringBuilderWriter &>::from(__writer_rename_at_27_157),cast<char * const >::from(__separator_rename_at_22_155)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_27,cast<StringBuilderWriter &>::from(__writer_rename_at_27_157),cast<char * const >::from((*__elem_rename_at_29_159))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_29_159));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_27_156);
}

inline void _FuncpegTicklog_infoTick4618819779667933862_3fb191919d34d5c0 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_78_160, char * __message_rename_at_78_161 )
{
    _FuncpegTicktabulateTick3664919181896551618_703c893d8b441b44(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_78_160));
    builtin_print(_FunccolorsTickblue_strTick419715327456344553_85edb7c1543a3a15(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_28, cast<char * const >::from(((char *) "[.] ")), cast<char *>::from(__message_rename_at_78_161), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_78_160.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_91_162 )
{
    while ( is_white_space(_FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_91_162))) )
    {
        _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_91_162),1);
    };
}

inline void _FuncpegTicklog_plainTick18061110541707930728_391a37924ab3bff7 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_63_163, char * __message_rename_at_63_164 )
{
    _FuncpegTicktabulateTick3664919181896551618_703c893d8b441b44(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_63_163));
    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_29, cast<char * const >::from(((char *) "[-] ")), cast<char *>::from(__message_rename_at_63_164), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_34_165, char * const  __template__rename_at_34_166, int32_t __strlen_rename_at_34_167 )
{
    int32_t __mark_rename_at_35_168 = __parser_rename_at_34_165.index;
    if ( builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_34_165.input)) < (__mark_rename_at_35_168 + __strlen_rename_at_34_167) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( das_memcmp(das_auto_cast<void * const >::cast(das_ref(__context__,__parser_rename_at_34_165.input(__mark_rename_at_35_168,__context__))),das_auto_cast<void * const >::cast(das_cast<uint8_t *>::cast(__template__rename_at_34_166)),__strlen_rename_at_34_167) == 0 )
        {
            _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_34_165),__strlen_rename_at_34_167);
            return das_auto_cast<bool>::cast(true);
        } else {
            return das_auto_cast<bool>::cast(false);
        };
    };
}

inline void _FuncpegTicklog_failTick11731645277795022368_8369bcfc7f20004f ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_68_169, char * __message_rename_at_68_170 )
{
    _FuncpegTicktabulateTick3664919181896551618_703c893d8b441b44(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_68_169));
    builtin_print(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_30, cast<char * const >::from(((char *) "[!] ")), cast<char *>::from(__message_rename_at_68_170), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_68_169.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncpegTicklog_successTick3645718276917011819_e4f6b21e9f6afcc5 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_73_171, char * __message_rename_at_73_172 )
{
    _FuncpegTicktabulateTick3664919181896551618_703c893d8b441b44(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_73_171));
    builtin_print(_FunccolorsTickgreen_strTick14807051613123094884_f38a76efee9c8c27(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_31, cast<char * const >::from(((char *) "[!] ")), cast<char *>::from(__message_rename_at_73_172), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_73_171.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_18bc2a30ed120a5 ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> const  &  __Tab_rename_at_1047_173, int32_t __at_rename_at_1047_174 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_173,__at_rename_at_1047_174));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_c6163711697a67ea ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> const  &  __Tab_rename_at_1047_175, int32_t __at_rename_at_1047_176 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_175,__at_rename_at_1047_176));
}

inline int32_t _FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_103_177 )
{
    return das_auto_cast<int32_t>::cast((builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_103_177.input)) <= __parser_rename_at_103_177.index) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(int32_t(__parser_rename_at_103_177.input(__parser_rename_at_103_177.index,__context__))));
}

inline void _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_54_178, int32_t __offset_rename_at_54_179 )
{
    __parser_rename_at_54_178.index += __offset_rename_at_54_179;
    das_copy(__parser_rename_at_54_178.longest_prefix,SimPolicy<int32_t>::Max(__parser_rename_at_54_178.longest_prefix,__parser_rename_at_54_178.index,*__context__,nullptr));
}

inline AutoTuple<bool,char *> _FuncpegTickmatch_string_literalTick12800953725978677998_5536b27e9ac361f3 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_144_180 )
{
    TArray<uint8_t> __buffer_rename_at_148_181; memset((void*)&__buffer_rename_at_148_181,0,sizeof(__buffer_rename_at_148_181));
    int32_t __current_char_rename_at_150_182; memset((void*)&__current_char_rename_at_150_182,0,sizeof(__current_char_rename_at_150_182));
    /* finally */ auto __finally_144= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_e963ce4a12221ab9(__context__,das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_181));
    /* end finally */ });
    das_zero(__buffer_rename_at_148_181);
    __current_char_rename_at_150_182 = _FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180));
    if ( __current_char_rename_at_150_182 != 34 )
    {
        return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
            AutoTuple<bool,char *> __mkt_153;
            das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_153) = false;
            das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_153) = nullptr;
            return __mkt_153;
        })()));
    } else {
        _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180),1);
        das_copy(__current_char_rename_at_150_182,_FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180)));
        while ( (__current_char_rename_at_150_182 != 34) && !(_FuncpegTickreached_EOFTick12193995001429396764_63a321d796086836(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180))) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_b295659320d5aa3e(__context__,das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_181),uint8_t(__current_char_rename_at_150_182));
            _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180),1);
            das_copy(__current_char_rename_at_150_182,_FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180)));
        };
        if ( _FuncpegTickreached_EOFTick12193995001429396764_63a321d796086836(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180)) )
        {
            return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
                AutoTuple<bool,char *> __mkt_165;
                das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_165) = false;
                das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_165) = nullptr;
                return __mkt_165;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180),1);
            return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
                AutoTuple<bool,char *> __mkt_168;
                das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_168) = true;
                das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_168) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_181),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
                return __mkt_168;
            })()));
        };
    };
}

inline void _FuncalgorithmTicksort_uniqueTick14985796887508933377_bf6db5262aa69fa5 ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_24_183 )
{
    if ( _FuncbuiltinTickemptyTick15399874715904164783_d23c2e725f60c80a(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183)) )
    {
        return ;
    } else {
        _FuncbuiltinTicksortTick14088969635007481283_b6071e2909e46233(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183));
        TArray<peg::ParsingError> __b_rename_at_32_184; das_zero(__b_rename_at_32_184); das_move(__b_rename_at_32_184, _FuncalgorithmTickuniqueTick8642070526899511001_ea2881ba9416889f(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183)));
        _FuncbuiltinTickfinalizeTick13836114024949725080_9c1054907b56811d(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183));
        _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_6e9e07c7cf5371d5(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183),das_arg<TArray<peg::ParsingError>>::pass(__b_rename_at_32_184));
    };
}

inline void finalize_f896e7308b8984f3 ( Context * __context__, spoof::invocationParserTickid_0x0 &  ____this_rename_at_0_185 )
{
    _FuncbuiltinTickfinalizeTick5454204887383796109_29d078faaa5f9fe2(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(____this_rename_at_0_185.comma_separated_elements_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_fd098149e3857e1c(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(____this_rename_at_0_185.element_list_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_185.ident__cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_185.element_value_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_29d078faaa5f9fe2(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(____this_rename_at_0_185.element_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_185.sub_element_cache));
    _FuncbuiltinTickfinalizeTick13836114024949725080_e963ce4a12221ab9(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_0_185.input));
    _FuncbuiltinTickfinalizeTick13836114024949725080_faa91cbe8a8ef152(__context__,das_arg<TArray<bool>>::pass(____this_rename_at_0_185.suppress_errors));
    _FuncbuiltinTickfinalizeTick13836114024949725080_9c1054907b56811d(__context__,das_arg<TArray<peg::ParsingError>>::pass(____this_rename_at_0_185.errors));
    memset((void*)&(____this_rename_at_0_185), 0, TypeSize<spoof::invocationParserTickid_0x0>::size);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_8958ad763c1a8045 ( Context * __context__, TArray<peg::ParsingError> const  &  __it_rename_at_22_186, char * const  __separator_rename_at_22_187 )
{
    char * __st_rename_at_27_188 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_27_189) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_28_190 = true;
        {
            bool __need_loop_29 = true;
            // elem: peg::ParsingError const&
            das_iterator<TArray<peg::ParsingError> const > __elem_iterator(__it_rename_at_22_186);
            peg::ParsingError const  * __elem_rename_at_29_191;
            __need_loop_29 = __elem_iterator.first(__context__,(__elem_rename_at_29_191)) && __need_loop_29;
            for ( ; __need_loop_29 ; __need_loop_29 = __elem_iterator.next(__context__,(__elem_rename_at_29_191)) )
            {
                if ( __skip_first_rename_at_28_190 )
                {
                    das_copy(__skip_first_rename_at_28_190,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_32,cast<StringBuilderWriter &>::from(__writer_rename_at_27_189),cast<char * const >::from(__separator_rename_at_22_187)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_33,cast<StringBuilderWriter &>::from(__writer_rename_at_27_189),cast<peg::ParsingError const  &>::from((*__elem_rename_at_29_191))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_29_191));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_27_188);
}

inline void finalize_593ca97eb449449f ( Context * __context__, spoof::invocationParserTickid_0x1 &  ____this_rename_at_0_192 )
{
    _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.comma_separated_elements_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.sub_element_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_13a83f3db63ddadc(__context__,das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(____this_rename_at_0_192.macro_invocation_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.ident__cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.class_name_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.element_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_2cd7253cd041b44f(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(____this_rename_at_0_192.element_list_cache));
    _FuncbuiltinTickfinalizeTick13836114024949725080_e963ce4a12221ab9(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_0_192.input));
    _FuncbuiltinTickfinalizeTick13836114024949725080_faa91cbe8a8ef152(__context__,das_arg<TArray<bool>>::pass(____this_rename_at_0_192.suppress_errors));
    _FuncbuiltinTickfinalizeTick13836114024949725080_9c1054907b56811d(__context__,das_arg<TArray<peg::ParsingError>>::pass(____this_rename_at_0_192.errors));
    memset((void*)&(____this_rename_at_0_192), 0, TypeSize<spoof::invocationParserTickid_0x1>::size);
}

inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _Funcparse_element_list_innerTickid_0x0_afef4622973f1478 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_193 )
{
    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _temp_make_local_364_18_5; _temp_make_local_364_18_5;
    int32_t __parse_pos_rename_at_1076_194 = __parser_rename_at_362_193.index;
    ++__parser_rename_at_362_193.tabs;
    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __res_1_rename_at_1086_195; das_zero(__res_1_rename_at_1086_195); das_move(__res_1_rename_at_1086_195, das_invoke<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193));
        if ( !_FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193),((char *) "("),1) )
        {
            if ( (__parser_rename_at_362_193.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_193.suppress_errors)))) && (__parser_rename_at_362_193.index == __parser_rename_at_362_193.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1175_49_6; _temp_make_local_1175_49_6;
                char * __rest_rename_at_1173_196 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_34, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_193.index))),false));
                char * __complete_message_rename_at_1174_197 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_35, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_196))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_193.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1175_49_6.text),(__complete_message_rename_at_1174_197));
                    das_copy((_temp_make_local_1175_49_6.index),(__parser_rename_at_362_193.index));
                    return _temp_make_local_1175_49_6;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast((([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> {
                AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __mks_1179;
                das_zero(__mks_1179);
                return __mks_1179;
            })()));
        } else {
            _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193));
            TArray<AutoTuple<char *,char *>> __els_rename_at_671_198;das_zero(__els_rename_at_671_198);
            _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_193.suppress_errors),true);
            while ( true )
            {
                int32_t __sz_rename_at_674_199 = builtin_array_size(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__els_rename_at_671_198));
                das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __comma_separated_elements0_rename_at_546_200; das_zero(__comma_separated_elements0_rename_at_546_200); das_move(__comma_separated_elements0_rename_at_546_200, _Funcparse_comma_separated_elementsTickid_0x0_3517a5e2f72086c7(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193)));
                    if ( !das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__comma_separated_elements0_rename_at_546_200) )
                    {
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                            AutoTuple<bool,int32_t,int32_t> __mks_548;
                            das_zero(__mks_548);
                            return __mks_548;
                        })()));
                    } else {
                        AutoTuple<char *,char *> __temp_name_rename_at_551_201; das_zero(__temp_name_rename_at_551_201); das_move(__temp_name_rename_at_551_201, das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__comma_separated_elements0_rename_at_546_200));
                        _FuncbuiltinTickemplaceTick13923705686753630697_b7cb402cbd718488(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__els_rename_at_671_198),das_arg<AutoTuple<char *,char *>>::pass(__temp_name_rename_at_551_201));
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                            AutoTuple<bool,int32_t,int32_t> __mkt_666;
                            das_get_auto_tuple_field<bool,0,bool,int32_t,int32_t>::get(__mkt_666) = true;
                            das_get_auto_tuple_field<int32_t,1,bool,int32_t,int32_t>::get(__mkt_666) = 0;
                            das_get_auto_tuple_field<int32_t,2,bool,int32_t,int32_t>::get(__mkt_666) = 0;
                            return __mkt_666;
                        })()));
                    };
                });
                if ( __sz_rename_at_674_199 == builtin_array_size(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__els_rename_at_671_198)) )
                {
                    break;
                };
            };
            _FuncbuiltinTickpopTick1161079256290593740_e59f84a1c28ac19(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_193.suppress_errors));
            _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193));
            AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __element0_rename_at_546_202; das_zero(__element0_rename_at_546_202); das_move(__element0_rename_at_546_202, _Funcparse_elementTickid_0x0_a89929d20275ecb4(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193)));
            if ( !das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__element0_rename_at_546_202) )
            {
                return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast((([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> {
                    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __mks_548;
                    das_zero(__mks_548);
                    return __mks_548;
                })()));
            } else {
                AutoTuple<char *,char *> __last_rename_at_551_203; das_zero(__last_rename_at_551_203); das_move(__last_rename_at_551_203, das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__element0_rename_at_546_202));
                _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193));
                if ( !_FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193),((char *) ")"),1) )
                {
                    if ( (__parser_rename_at_362_193.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_193.suppress_errors)))) && (__parser_rename_at_362_193.index == __parser_rename_at_362_193.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1175_49_7; _temp_make_local_1175_49_7;
                        char * __rest_rename_at_1173_204 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_36, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_193.index))),false));
                        char * __complete_message_rename_at_1174_205 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_37, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_204))));
                        _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_193.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1175_49_7.text),(__complete_message_rename_at_1174_205));
                            das_copy((_temp_make_local_1175_49_7.index),(__parser_rename_at_362_193.index));
                            return _temp_make_local_1175_49_7;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast((([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> {
                        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __mks_1179;
                        das_zero(__mks_1179);
                        return __mks_1179;
                    })()));
                } else {
                    TArray<AutoTuple<char *,char *>> __val_rename_at_1133_206; das_zero(__val_rename_at_1133_206); das_move(__val_rename_at_1133_206, das_invoke<TArray<AutoTuple<char *,char *>>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<AutoTuple<char *,char *>>{
                        _FuncbuiltinTickpushTick10769833213962245646_5bc44f666511c4f4(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__els_rename_at_671_198),das_arg<AutoTuple<char *,char *>>::pass(__last_rename_at_551_203));
                        return /* <- */ das_auto_cast_move<TArray<AutoTuple<char *,char *>>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_d086567e061861cd(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__els_rename_at_671_198)));
                    }));
                    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __result_rename_at_1141_207; das_zero(__result_rename_at_1141_207); das_move(__result_rename_at_1141_207, (([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> {
                        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __mkt_1141;
                        das_get_auto_tuple_field<bool,0,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__mkt_1141) = true;
                        das_get_auto_tuple_field<TArray<AutoTuple<char *,char *>>,1,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__mkt_1141) = __val_rename_at_1133_206;
                        das_get_auto_tuple_field<int32_t,2,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__mkt_1141) = __parser_rename_at_362_193.index;
                        return __mkt_1141;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(__result_rename_at_1141_207);
                };
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__res_1_rename_at_1086_195) )
    {
        --__parser_rename_at_362_193.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(__res_1_rename_at_1086_195);
    } else {
        --__parser_rename_at_362_193.tabs;
        das_copy(__parser_rename_at_362_193.index,__parse_pos_rename_at_1076_194);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5ebbb8f8a79624b3(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass((([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>& {
        das_zero(_temp_make_local_364_18_5);
        return _temp_make_local_364_18_5;
    })()))));
}

inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _Funcparse_element_listTickid_0x0_e08aaeacf4bca4a2 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_208 )
{
    int32_t __mark_rename_at_147_209 = __parser_rename_at_146_208.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_e89937f359ff4105(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(__parser_rename_at_146_208.element_list_cache),__parser_rename_at_146_208.index) && !(__parser_rename_at_146_208.error_reporting) )
    {
        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __result_rename_at_155_210; das_zero(__result_rename_at_155_210); das_move(__result_rename_at_155_210, _FuncbuiltinTickclone_to_moveTick2007252383599261567_5ab31b7ac47de8d4(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass(__parser_rename_at_146_208.element_list_cache(__parser_rename_at_146_208.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__result_rename_at_155_210) )
        {
            das_copy(__parser_rename_at_146_208.index,das_get_auto_tuple_field<int32_t,2,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__result_rename_at_155_210));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(__result_rename_at_155_210);
    } else {
        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __result_rename_at_169_211; das_zero(__result_rename_at_169_211); das_move(__result_rename_at_169_211, _Funcparse_element_list_innerTickid_0x0_afef4622973f1478(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_208)));
        clone_e6aaad296469300e(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass(__parser_rename_at_146_208.element_list_cache(__mark_rename_at_147_209,__context__)),das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass(__result_rename_at_169_211));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(__result_rename_at_169_211);
    };
}

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_comma_separated_elements_innerTickid_0x0_fc0f629550ec98ab ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_212 )
{
    int32_t __parse_pos_rename_at_1076_213 = __parser_rename_at_362_212.index;
    ++__parser_rename_at_362_212.tabs;
    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __res_1_rename_at_1086_214; das_zero(__res_1_rename_at_1086_214); das_move(__res_1_rename_at_1086_214, das_invoke<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_212));
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __element1_rename_at_546_215; das_zero(__element1_rename_at_546_215); das_move(__element1_rename_at_546_215, _Funcparse_elementTickid_0x0_a89929d20275ecb4(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_212)));
        if ( !das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__element1_rename_at_546_215) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            AutoTuple<char *,char *> __e_rename_at_551_216; das_zero(__e_rename_at_551_216); das_move(__e_rename_at_551_216, das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__element1_rename_at_546_215));
            _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_212));
            if ( !_FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_212),((char *) ","),1) )
            {
                if ( (__parser_rename_at_362_212.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_212.suppress_errors)))) && (__parser_rename_at_362_212.index == __parser_rename_at_362_212.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1175_49_8; _temp_make_local_1175_49_8;
                    char * __rest_rename_at_1173_217 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_38, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_212.index))),false));
                    char * __complete_message_rename_at_1174_218 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_39, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m,\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_217))));
                    _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_212.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1175_49_8.text),(__complete_message_rename_at_1174_218));
                        das_copy((_temp_make_local_1175_49_8.index),(__parser_rename_at_362_212.index));
                        return _temp_make_local_1175_49_8;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_1179;
                    das_zero(__mks_1179);
                    return __mks_1179;
                })()));
            } else {
                AutoTuple<char *,char *> __val_rename_at_1133_219; das_zero(__val_rename_at_1133_219); das_move(__val_rename_at_1133_219, das_invoke<AutoTuple<char *,char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<char *,char *>{
                    return das_auto_cast_ref<AutoTuple<char *,char *>>::cast(__e_rename_at_551_216);
                }));
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_1141_220; das_zero(__result_rename_at_1141_220); das_move(__result_rename_at_1141_220, (([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mkt_1141;
                    das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1141) = true;
                    das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1141) = __val_rename_at_1133_219;
                    das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1141) = __parser_rename_at_362_212.index;
                    return __mkt_1141;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_1141_220);
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__res_1_rename_at_1086_214) )
    {
        --__parser_rename_at_362_212.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__res_1_rename_at_1086_214);
    } else {
        --__parser_rename_at_362_212.tabs;
        das_copy(__parser_rename_at_362_212.index,__parse_pos_rename_at_1076_213);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_comma_separated_elementsTickid_0x0_3517a5e2f72086c7 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_221 )
{
    int32_t __mark_rename_at_147_222 = __parser_rename_at_146_221.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_31e87ae3c763efa9(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__parser_rename_at_146_221.comma_separated_elements_cache),__parser_rename_at_146_221.index) && !(__parser_rename_at_146_221.error_reporting) )
    {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_155_223; das_zero(__result_rename_at_155_223); das_move(__result_rename_at_155_223, _FuncbuiltinTickclone_to_moveTick2007252383599261567_76164b501fa5f2f(__context__,das_arg<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::pass(__parser_rename_at_146_221.comma_separated_elements_cache(__parser_rename_at_146_221.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__result_rename_at_155_223) )
        {
            das_copy(__parser_rename_at_146_221.index,das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__result_rename_at_155_223));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_155_223);
    } else {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_169_224; das_zero(__result_rename_at_169_224); das_move(__result_rename_at_169_224, _Funcparse_comma_separated_elements_innerTickid_0x0_fc0f629550ec98ab(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_221)));
        das_copy(__parser_rename_at_146_221.comma_separated_elements_cache(__mark_rename_at_147_222,__context__),__result_rename_at_169_224);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_169_224);
    };
}

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_element_innerTickid_0x0_c8e5e2bfd35033ea ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_225 )
{
    int32_t __parse_pos_rename_at_1076_226 = __parser_rename_at_362_225.index;
    ++__parser_rename_at_362_225.tabs;
    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __res_1_rename_at_1086_227; das_zero(__res_1_rename_at_1086_227); das_move(__res_1_rename_at_1086_227, das_invoke<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t>{
        AutoTuple<bool,char *,int32_t> __ident_0_rename_at_546_228; das_zero(__ident_0_rename_at_546_228); das_move(__ident_0_rename_at_546_228, _Funcparse_ident_Tickid_0x0_ac07755a3c8869f1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__ident_0_rename_at_546_228) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __e_rename_at_551_229; das_zero(__e_rename_at_551_229); das_move(__e_rename_at_551_229, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__ident_0_rename_at_546_228)));
            _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225));
            if ( !_FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225),((char *) "="),1) )
            {
                if ( (__parser_rename_at_362_225.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_225.suppress_errors)))) && (__parser_rename_at_362_225.index == __parser_rename_at_362_225.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1175_49_9; _temp_make_local_1175_49_9;
                    char * __rest_rename_at_1173_230 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_40, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_225.index))),false));
                    char * __complete_message_rename_at_1174_231 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_41, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m=\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_230))));
                    _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_225.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1175_49_9.text),(__complete_message_rename_at_1174_231));
                        das_copy((_temp_make_local_1175_49_9.index),(__parser_rename_at_362_225.index));
                        return _temp_make_local_1175_49_9;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_1179;
                    das_zero(__mks_1179);
                    return __mks_1179;
                })()));
            } else {
                _FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225));
                AutoTuple<bool,char *,int32_t> __element_value0_rename_at_546_232; das_zero(__element_value0_rename_at_546_232); das_move(__element_value0_rename_at_546_232, _Funcparse_element_valueTickid_0x0_126b58a15c99cc72(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225)));
                if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__element_value0_rename_at_546_232) )
                {
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_548;
                        das_zero(__mks_548);
                        return __mks_548;
                    })()));
                } else {
                    char * __v_rename_at_551_233; das_zero(__v_rename_at_551_233); das_move(__v_rename_at_551_233, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__element_value0_rename_at_546_232)));
                    AutoTuple<char *,char *> __val_rename_at_1133_234; das_zero(__val_rename_at_1133_234); das_move(__val_rename_at_1133_234, das_invoke<AutoTuple<char *,char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<char *,char *>{
                        return das_auto_cast_ref<AutoTuple<char *,char *>>::cast((([&]() -> AutoTuple<char *,char *> {
                            AutoTuple<char *,char *> __mkt_29;
                            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_29) = __e_rename_at_551_229;
                            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_29) = __v_rename_at_551_233;
                            return __mkt_29;
                        })()));
                    }));
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_1141_235; das_zero(__result_rename_at_1141_235); das_move(__result_rename_at_1141_235, (([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mkt_1141;
                        das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1141) = true;
                        das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1141) = __val_rename_at_1133_234;
                        das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1141) = __parser_rename_at_362_225.index;
                        return __mkt_1141;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_1141_235);
                };
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__res_1_rename_at_1086_227) )
    {
        --__parser_rename_at_362_225.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__res_1_rename_at_1086_227);
    } else {
        --__parser_rename_at_362_225.tabs;
        das_copy(__parser_rename_at_362_225.index,__parse_pos_rename_at_1076_226);
    };
    int32_t __parse_pos_rename_at_1076_236 = __parser_rename_at_362_225.index;
    ++__parser_rename_at_362_225.tabs;
    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __res_2_rename_at_1086_237; das_zero(__res_2_rename_at_1086_237); das_move(__res_2_rename_at_1086_237, das_invoke<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t>{
        AutoTuple<bool,char *,int32_t> __ident_1_rename_at_546_238; das_zero(__ident_1_rename_at_546_238); das_move(__ident_1_rename_at_546_238, _Funcparse_ident_Tickid_0x0_ac07755a3c8869f1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__ident_1_rename_at_546_238) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __e_rename_at_551_239; das_zero(__e_rename_at_551_239); das_move(__e_rename_at_551_239, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__ident_1_rename_at_546_238)));
            AutoTuple<char *,char *> __val_rename_at_1133_240; das_zero(__val_rename_at_1133_240); das_move(__val_rename_at_1133_240, das_invoke<AutoTuple<char *,char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<char *,char *>{
                return das_auto_cast_ref<AutoTuple<char *,char *>>::cast((([&]() -> AutoTuple<char *,char *> {
                    AutoTuple<char *,char *> __mkt_32;
                    das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_32) = __e_rename_at_551_239;
                    das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_32) = nullptr;
                    return __mkt_32;
                })()));
            }));
            AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_1141_241; das_zero(__result_rename_at_1141_241); das_move(__result_rename_at_1141_241, (([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mkt_1141;
                das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1141) = true;
                das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1141) = __val_rename_at_1133_240;
                das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1141) = __parser_rename_at_362_225.index;
                return __mkt_1141;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_1141_241);
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__res_2_rename_at_1086_237) )
    {
        --__parser_rename_at_362_225.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__res_2_rename_at_1086_237);
    } else {
        --__parser_rename_at_362_225.tabs;
        das_copy(__parser_rename_at_362_225.index,__parse_pos_rename_at_1076_236);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_elementTickid_0x0_a89929d20275ecb4 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_242 )
{
    int32_t __mark_rename_at_147_243 = __parser_rename_at_146_242.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_31e87ae3c763efa9(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__parser_rename_at_146_242.element_cache),__parser_rename_at_146_242.index) && !(__parser_rename_at_146_242.error_reporting) )
    {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_155_244; das_zero(__result_rename_at_155_244); das_move(__result_rename_at_155_244, _FuncbuiltinTickclone_to_moveTick2007252383599261567_76164b501fa5f2f(__context__,das_arg<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::pass(__parser_rename_at_146_242.element_cache(__parser_rename_at_146_242.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__result_rename_at_155_244) )
        {
            das_copy(__parser_rename_at_146_242.index,das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__result_rename_at_155_244));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_155_244);
    } else {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_169_245; das_zero(__result_rename_at_169_245); das_move(__result_rename_at_169_245, _Funcparse_element_innerTickid_0x0_c8e5e2bfd35033ea(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_242)));
        das_copy(__parser_rename_at_146_242.element_cache(__mark_rename_at_147_243,__context__),__result_rename_at_169_245);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_169_245);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_element_innerTickid_0x0_4416149c422be215 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_246 )
{
    int32_t __parse_pos_rename_at_1076_247 = __parser_rename_at_362_246.index;
    ++__parser_rename_at_362_246.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_248; das_zero(__res_1_rename_at_1086_248); das_move(__res_1_rename_at_1086_248, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        if ( !_FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),((char *) "\\"),1) )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1175_49_10; _temp_make_local_1175_49_10;
                char * __rest_rename_at_1173_249 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_42, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                char * __complete_message_rename_at_1174_250 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_43, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m\\\\\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_249))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1175_49_10.text),(__complete_message_rename_at_1174_250));
                    das_copy((_temp_make_local_1175_49_10.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1175_49_10;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1179;
                das_zero(__mks_1179);
                return __mks_1179;
            })()));
        } else {
            int32_t __ext_pos_0_rename_at_564_251 = __parser_rename_at_362_246.index;
            bool ___terminal_result_13_rename_at_1199_252 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
                TDim<bool,256> _temp_make_local_1195_39_11; _temp_make_local_1195_39_11;
                int32_t __ch_rename_at_1194_253 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
                return das_auto_cast<bool>::cast((__ch_rename_at_1194_253 != -1) && (([&]() -> TDim<bool,256>& {
                    _temp_make_local_1195_39_11(0,__context__) = true;
                    _temp_make_local_1195_39_11(1,__context__) = true;
                    _temp_make_local_1195_39_11(2,__context__) = true;
                    _temp_make_local_1195_39_11(3,__context__) = true;
                    _temp_make_local_1195_39_11(4,__context__) = true;
                    _temp_make_local_1195_39_11(5,__context__) = true;
                    _temp_make_local_1195_39_11(6,__context__) = true;
                    _temp_make_local_1195_39_11(7,__context__) = true;
                    _temp_make_local_1195_39_11(8,__context__) = true;
                    _temp_make_local_1195_39_11(9,__context__) = true;
                    _temp_make_local_1195_39_11(10,__context__) = true;
                    _temp_make_local_1195_39_11(11,__context__) = true;
                    _temp_make_local_1195_39_11(12,__context__) = true;
                    _temp_make_local_1195_39_11(13,__context__) = true;
                    _temp_make_local_1195_39_11(14,__context__) = true;
                    _temp_make_local_1195_39_11(15,__context__) = true;
                    _temp_make_local_1195_39_11(16,__context__) = true;
                    _temp_make_local_1195_39_11(17,__context__) = true;
                    _temp_make_local_1195_39_11(18,__context__) = true;
                    _temp_make_local_1195_39_11(19,__context__) = true;
                    _temp_make_local_1195_39_11(20,__context__) = true;
                    _temp_make_local_1195_39_11(21,__context__) = true;
                    _temp_make_local_1195_39_11(22,__context__) = true;
                    _temp_make_local_1195_39_11(23,__context__) = true;
                    _temp_make_local_1195_39_11(24,__context__) = true;
                    _temp_make_local_1195_39_11(25,__context__) = true;
                    _temp_make_local_1195_39_11(26,__context__) = true;
                    _temp_make_local_1195_39_11(27,__context__) = true;
                    _temp_make_local_1195_39_11(28,__context__) = true;
                    _temp_make_local_1195_39_11(29,__context__) = true;
                    _temp_make_local_1195_39_11(30,__context__) = true;
                    _temp_make_local_1195_39_11(31,__context__) = true;
                    _temp_make_local_1195_39_11(32,__context__) = true;
                    _temp_make_local_1195_39_11(33,__context__) = true;
                    _temp_make_local_1195_39_11(34,__context__) = true;
                    _temp_make_local_1195_39_11(35,__context__) = true;
                    _temp_make_local_1195_39_11(36,__context__) = true;
                    _temp_make_local_1195_39_11(37,__context__) = true;
                    _temp_make_local_1195_39_11(38,__context__) = true;
                    _temp_make_local_1195_39_11(39,__context__) = true;
                    _temp_make_local_1195_39_11(40,__context__) = true;
                    _temp_make_local_1195_39_11(41,__context__) = true;
                    _temp_make_local_1195_39_11(42,__context__) = true;
                    _temp_make_local_1195_39_11(43,__context__) = true;
                    _temp_make_local_1195_39_11(44,__context__) = true;
                    _temp_make_local_1195_39_11(45,__context__) = true;
                    _temp_make_local_1195_39_11(46,__context__) = true;
                    _temp_make_local_1195_39_11(47,__context__) = true;
                    _temp_make_local_1195_39_11(48,__context__) = true;
                    _temp_make_local_1195_39_11(49,__context__) = true;
                    _temp_make_local_1195_39_11(50,__context__) = true;
                    _temp_make_local_1195_39_11(51,__context__) = true;
                    _temp_make_local_1195_39_11(52,__context__) = true;
                    _temp_make_local_1195_39_11(53,__context__) = true;
                    _temp_make_local_1195_39_11(54,__context__) = true;
                    _temp_make_local_1195_39_11(55,__context__) = true;
                    _temp_make_local_1195_39_11(56,__context__) = true;
                    _temp_make_local_1195_39_11(57,__context__) = true;
                    _temp_make_local_1195_39_11(58,__context__) = true;
                    _temp_make_local_1195_39_11(59,__context__) = true;
                    _temp_make_local_1195_39_11(60,__context__) = true;
                    _temp_make_local_1195_39_11(61,__context__) = true;
                    _temp_make_local_1195_39_11(62,__context__) = true;
                    _temp_make_local_1195_39_11(63,__context__) = true;
                    _temp_make_local_1195_39_11(64,__context__) = true;
                    _temp_make_local_1195_39_11(65,__context__) = true;
                    _temp_make_local_1195_39_11(66,__context__) = true;
                    _temp_make_local_1195_39_11(67,__context__) = true;
                    _temp_make_local_1195_39_11(68,__context__) = true;
                    _temp_make_local_1195_39_11(69,__context__) = true;
                    _temp_make_local_1195_39_11(70,__context__) = true;
                    _temp_make_local_1195_39_11(71,__context__) = true;
                    _temp_make_local_1195_39_11(72,__context__) = true;
                    _temp_make_local_1195_39_11(73,__context__) = true;
                    _temp_make_local_1195_39_11(74,__context__) = true;
                    _temp_make_local_1195_39_11(75,__context__) = true;
                    _temp_make_local_1195_39_11(76,__context__) = true;
                    _temp_make_local_1195_39_11(77,__context__) = true;
                    _temp_make_local_1195_39_11(78,__context__) = true;
                    _temp_make_local_1195_39_11(79,__context__) = true;
                    _temp_make_local_1195_39_11(80,__context__) = true;
                    _temp_make_local_1195_39_11(81,__context__) = true;
                    _temp_make_local_1195_39_11(82,__context__) = true;
                    _temp_make_local_1195_39_11(83,__context__) = true;
                    _temp_make_local_1195_39_11(84,__context__) = true;
                    _temp_make_local_1195_39_11(85,__context__) = true;
                    _temp_make_local_1195_39_11(86,__context__) = true;
                    _temp_make_local_1195_39_11(87,__context__) = true;
                    _temp_make_local_1195_39_11(88,__context__) = true;
                    _temp_make_local_1195_39_11(89,__context__) = true;
                    _temp_make_local_1195_39_11(90,__context__) = true;
                    _temp_make_local_1195_39_11(91,__context__) = true;
                    _temp_make_local_1195_39_11(92,__context__) = true;
                    _temp_make_local_1195_39_11(93,__context__) = true;
                    _temp_make_local_1195_39_11(94,__context__) = true;
                    _temp_make_local_1195_39_11(95,__context__) = true;
                    _temp_make_local_1195_39_11(96,__context__) = true;
                    _temp_make_local_1195_39_11(97,__context__) = true;
                    _temp_make_local_1195_39_11(98,__context__) = true;
                    _temp_make_local_1195_39_11(99,__context__) = true;
                    _temp_make_local_1195_39_11(100,__context__) = true;
                    _temp_make_local_1195_39_11(101,__context__) = true;
                    _temp_make_local_1195_39_11(102,__context__) = true;
                    _temp_make_local_1195_39_11(103,__context__) = true;
                    _temp_make_local_1195_39_11(104,__context__) = true;
                    _temp_make_local_1195_39_11(105,__context__) = true;
                    _temp_make_local_1195_39_11(106,__context__) = true;
                    _temp_make_local_1195_39_11(107,__context__) = true;
                    _temp_make_local_1195_39_11(108,__context__) = true;
                    _temp_make_local_1195_39_11(109,__context__) = true;
                    _temp_make_local_1195_39_11(110,__context__) = true;
                    _temp_make_local_1195_39_11(111,__context__) = true;
                    _temp_make_local_1195_39_11(112,__context__) = true;
                    _temp_make_local_1195_39_11(113,__context__) = true;
                    _temp_make_local_1195_39_11(114,__context__) = true;
                    _temp_make_local_1195_39_11(115,__context__) = true;
                    _temp_make_local_1195_39_11(116,__context__) = true;
                    _temp_make_local_1195_39_11(117,__context__) = true;
                    _temp_make_local_1195_39_11(118,__context__) = true;
                    _temp_make_local_1195_39_11(119,__context__) = true;
                    _temp_make_local_1195_39_11(120,__context__) = true;
                    _temp_make_local_1195_39_11(121,__context__) = true;
                    _temp_make_local_1195_39_11(122,__context__) = true;
                    _temp_make_local_1195_39_11(123,__context__) = true;
                    _temp_make_local_1195_39_11(124,__context__) = true;
                    _temp_make_local_1195_39_11(125,__context__) = true;
                    _temp_make_local_1195_39_11(126,__context__) = true;
                    _temp_make_local_1195_39_11(127,__context__) = true;
                    _temp_make_local_1195_39_11(128,__context__) = true;
                    _temp_make_local_1195_39_11(129,__context__) = true;
                    _temp_make_local_1195_39_11(130,__context__) = true;
                    _temp_make_local_1195_39_11(131,__context__) = true;
                    _temp_make_local_1195_39_11(132,__context__) = true;
                    _temp_make_local_1195_39_11(133,__context__) = true;
                    _temp_make_local_1195_39_11(134,__context__) = true;
                    _temp_make_local_1195_39_11(135,__context__) = true;
                    _temp_make_local_1195_39_11(136,__context__) = true;
                    _temp_make_local_1195_39_11(137,__context__) = true;
                    _temp_make_local_1195_39_11(138,__context__) = true;
                    _temp_make_local_1195_39_11(139,__context__) = true;
                    _temp_make_local_1195_39_11(140,__context__) = true;
                    _temp_make_local_1195_39_11(141,__context__) = true;
                    _temp_make_local_1195_39_11(142,__context__) = true;
                    _temp_make_local_1195_39_11(143,__context__) = true;
                    _temp_make_local_1195_39_11(144,__context__) = true;
                    _temp_make_local_1195_39_11(145,__context__) = true;
                    _temp_make_local_1195_39_11(146,__context__) = true;
                    _temp_make_local_1195_39_11(147,__context__) = true;
                    _temp_make_local_1195_39_11(148,__context__) = true;
                    _temp_make_local_1195_39_11(149,__context__) = true;
                    _temp_make_local_1195_39_11(150,__context__) = true;
                    _temp_make_local_1195_39_11(151,__context__) = true;
                    _temp_make_local_1195_39_11(152,__context__) = true;
                    _temp_make_local_1195_39_11(153,__context__) = true;
                    _temp_make_local_1195_39_11(154,__context__) = true;
                    _temp_make_local_1195_39_11(155,__context__) = true;
                    _temp_make_local_1195_39_11(156,__context__) = true;
                    _temp_make_local_1195_39_11(157,__context__) = true;
                    _temp_make_local_1195_39_11(158,__context__) = true;
                    _temp_make_local_1195_39_11(159,__context__) = true;
                    _temp_make_local_1195_39_11(160,__context__) = true;
                    _temp_make_local_1195_39_11(161,__context__) = true;
                    _temp_make_local_1195_39_11(162,__context__) = true;
                    _temp_make_local_1195_39_11(163,__context__) = true;
                    _temp_make_local_1195_39_11(164,__context__) = true;
                    _temp_make_local_1195_39_11(165,__context__) = true;
                    _temp_make_local_1195_39_11(166,__context__) = true;
                    _temp_make_local_1195_39_11(167,__context__) = true;
                    _temp_make_local_1195_39_11(168,__context__) = true;
                    _temp_make_local_1195_39_11(169,__context__) = true;
                    _temp_make_local_1195_39_11(170,__context__) = true;
                    _temp_make_local_1195_39_11(171,__context__) = true;
                    _temp_make_local_1195_39_11(172,__context__) = true;
                    _temp_make_local_1195_39_11(173,__context__) = true;
                    _temp_make_local_1195_39_11(174,__context__) = true;
                    _temp_make_local_1195_39_11(175,__context__) = true;
                    _temp_make_local_1195_39_11(176,__context__) = true;
                    _temp_make_local_1195_39_11(177,__context__) = true;
                    _temp_make_local_1195_39_11(178,__context__) = true;
                    _temp_make_local_1195_39_11(179,__context__) = true;
                    _temp_make_local_1195_39_11(180,__context__) = true;
                    _temp_make_local_1195_39_11(181,__context__) = true;
                    _temp_make_local_1195_39_11(182,__context__) = true;
                    _temp_make_local_1195_39_11(183,__context__) = true;
                    _temp_make_local_1195_39_11(184,__context__) = true;
                    _temp_make_local_1195_39_11(185,__context__) = true;
                    _temp_make_local_1195_39_11(186,__context__) = true;
                    _temp_make_local_1195_39_11(187,__context__) = true;
                    _temp_make_local_1195_39_11(188,__context__) = true;
                    _temp_make_local_1195_39_11(189,__context__) = true;
                    _temp_make_local_1195_39_11(190,__context__) = true;
                    _temp_make_local_1195_39_11(191,__context__) = true;
                    _temp_make_local_1195_39_11(192,__context__) = true;
                    _temp_make_local_1195_39_11(193,__context__) = true;
                    _temp_make_local_1195_39_11(194,__context__) = true;
                    _temp_make_local_1195_39_11(195,__context__) = true;
                    _temp_make_local_1195_39_11(196,__context__) = true;
                    _temp_make_local_1195_39_11(197,__context__) = true;
                    _temp_make_local_1195_39_11(198,__context__) = true;
                    _temp_make_local_1195_39_11(199,__context__) = true;
                    _temp_make_local_1195_39_11(200,__context__) = true;
                    _temp_make_local_1195_39_11(201,__context__) = true;
                    _temp_make_local_1195_39_11(202,__context__) = true;
                    _temp_make_local_1195_39_11(203,__context__) = true;
                    _temp_make_local_1195_39_11(204,__context__) = true;
                    _temp_make_local_1195_39_11(205,__context__) = true;
                    _temp_make_local_1195_39_11(206,__context__) = true;
                    _temp_make_local_1195_39_11(207,__context__) = true;
                    _temp_make_local_1195_39_11(208,__context__) = true;
                    _temp_make_local_1195_39_11(209,__context__) = true;
                    _temp_make_local_1195_39_11(210,__context__) = true;
                    _temp_make_local_1195_39_11(211,__context__) = true;
                    _temp_make_local_1195_39_11(212,__context__) = true;
                    _temp_make_local_1195_39_11(213,__context__) = true;
                    _temp_make_local_1195_39_11(214,__context__) = true;
                    _temp_make_local_1195_39_11(215,__context__) = true;
                    _temp_make_local_1195_39_11(216,__context__) = true;
                    _temp_make_local_1195_39_11(217,__context__) = true;
                    _temp_make_local_1195_39_11(218,__context__) = true;
                    _temp_make_local_1195_39_11(219,__context__) = true;
                    _temp_make_local_1195_39_11(220,__context__) = true;
                    _temp_make_local_1195_39_11(221,__context__) = true;
                    _temp_make_local_1195_39_11(222,__context__) = true;
                    _temp_make_local_1195_39_11(223,__context__) = true;
                    _temp_make_local_1195_39_11(224,__context__) = true;
                    _temp_make_local_1195_39_11(225,__context__) = true;
                    _temp_make_local_1195_39_11(226,__context__) = true;
                    _temp_make_local_1195_39_11(227,__context__) = true;
                    _temp_make_local_1195_39_11(228,__context__) = true;
                    _temp_make_local_1195_39_11(229,__context__) = true;
                    _temp_make_local_1195_39_11(230,__context__) = true;
                    _temp_make_local_1195_39_11(231,__context__) = true;
                    _temp_make_local_1195_39_11(232,__context__) = true;
                    _temp_make_local_1195_39_11(233,__context__) = true;
                    _temp_make_local_1195_39_11(234,__context__) = true;
                    _temp_make_local_1195_39_11(235,__context__) = true;
                    _temp_make_local_1195_39_11(236,__context__) = true;
                    _temp_make_local_1195_39_11(237,__context__) = true;
                    _temp_make_local_1195_39_11(238,__context__) = true;
                    _temp_make_local_1195_39_11(239,__context__) = true;
                    _temp_make_local_1195_39_11(240,__context__) = true;
                    _temp_make_local_1195_39_11(241,__context__) = true;
                    _temp_make_local_1195_39_11(242,__context__) = true;
                    _temp_make_local_1195_39_11(243,__context__) = true;
                    _temp_make_local_1195_39_11(244,__context__) = true;
                    _temp_make_local_1195_39_11(245,__context__) = true;
                    _temp_make_local_1195_39_11(246,__context__) = true;
                    _temp_make_local_1195_39_11(247,__context__) = true;
                    _temp_make_local_1195_39_11(248,__context__) = true;
                    _temp_make_local_1195_39_11(249,__context__) = true;
                    _temp_make_local_1195_39_11(250,__context__) = true;
                    _temp_make_local_1195_39_11(251,__context__) = true;
                    _temp_make_local_1195_39_11(252,__context__) = true;
                    _temp_make_local_1195_39_11(253,__context__) = true;
                    _temp_make_local_1195_39_11(254,__context__) = true;
                    _temp_make_local_1195_39_11(255,__context__) = true;
                    return _temp_make_local_1195_39_11;
                })())(__ch_rename_at_1194_253,__context__));
            });
            if ( !___terminal_result_13_rename_at_1199_252 )
            {
                if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1217_49_12; _temp_make_local_1217_49_12;
                    char * __rest_rename_at_1215_254 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_44, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                    char * __complete_message_rename_at_1216_255 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_45, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '0'-'256' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1215_254))));
                    _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1217_49_12.text),(__complete_message_rename_at_1216_255));
                        das_copy((_temp_make_local_1217_49_12.index),(__parser_rename_at_362_246.index));
                        return _temp_make_local_1217_49_12;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1220;
                    das_zero(__mks_1220);
                    return __mks_1220;
                })()));
            } else {
                _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),1);
            };
            char * __str_0_rename_at_566_256 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_246.input(0,__context__))));
            char * __Ch_rename_at_567_257 = (char *)(((char * const )(builtin_string_slice1(__str_0_rename_at_566_256,__ext_pos_0_rename_at_564_251,__parser_rename_at_362_246.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            char * __val_rename_at_1133_258; das_zero(__val_rename_at_1133_258); das_move(__val_rename_at_1133_258, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(((char * const )(pass_string(__Ch_rename_at_567_257))));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1141_259; das_zero(__result_rename_at_1141_259); das_move(__result_rename_at_1141_259, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1141;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_258;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_246.index;
                return __mkt_1141;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_259);
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_1_rename_at_1086_248) )
    {
        --__parser_rename_at_362_246.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_1_rename_at_1086_248);
    } else {
        --__parser_rename_at_362_246.tabs;
        das_copy(__parser_rename_at_362_246.index,__parse_pos_rename_at_1076_247);
    };
    int32_t __parse_pos_rename_at_1076_260 = __parser_rename_at_362_246.index;
    ++__parser_rename_at_362_246.tabs;
    bool __cut_rename_at_1084_261 = false;
    AutoTuple<bool,char *,int32_t> __res_2_rename_at_1086_262; das_zero(__res_2_rename_at_1086_262); das_move(__res_2_rename_at_1086_262, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        if ( !_FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),((char *) "#"),1) )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1175_49_13; _temp_make_local_1175_49_13;
                char * __rest_rename_at_1173_263 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_46, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                char * __complete_message_rename_at_1174_264 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_47, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m#\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_263))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1175_49_13.text),(__complete_message_rename_at_1174_264));
                    das_copy((_temp_make_local_1175_49_13.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1175_49_13;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1179;
                das_zero(__mks_1179);
                return __mks_1179;
            })()));
        } else {
            AutoTuple<bool,char *> ___terminal_result_15_rename_at_1353_265; das_zero(___terminal_result_15_rename_at_1353_265); das_move(___terminal_result_15_rename_at_1353_265, _FuncpegTickmatch_string_literalTick12800953725978677998_50b323a2328ca30f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
            if ( !das_get_auto_tuple_field<bool,0,bool,char *>::get(___terminal_result_15_rename_at_1353_265) )
            {
                if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1362_49_14; _temp_make_local_1362_49_14;
                    char * __complete_message_rename_at_1361_266 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_48, cast<char * const >::from(((char *) "Error: Expected a string at ")), cast<int32_t>::from(__parser_rename_at_362_246.index)))));
                    _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1362_49_14.text),(__complete_message_rename_at_1361_266));
                        das_copy((_temp_make_local_1362_49_14.index),(__parser_rename_at_362_246.index));
                        return _temp_make_local_1362_49_14;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1365;
                    das_zero(__mks_1365);
                    return __mks_1365;
                })()));
            } else {
                char * __e_rename_at_1372_267 = (char *)(das_get_auto_tuple_field<char *,1,bool,char *>::get(___terminal_result_15_rename_at_1353_265));
                das_copy(__cut_rename_at_1084_261,true);
                char * __val_rename_at_1133_268; das_zero(__val_rename_at_1133_268); das_move(__val_rename_at_1133_268, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                    return das_auto_cast<char *>::cast(__e_rename_at_1372_267);
                })));
                AutoTuple<bool,char *,int32_t> __result_rename_at_1141_269; das_zero(__result_rename_at_1141_269); das_move(__result_rename_at_1141_269, (([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mkt_1141;
                    das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                    das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_268;
                    das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_246.index;
                    return __mkt_1141;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_269);
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_2_rename_at_1086_262) )
    {
        --__parser_rename_at_362_246.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_2_rename_at_1086_262);
    } else {
        --__parser_rename_at_362_246.tabs;
        das_copy(__parser_rename_at_362_246.index,__parse_pos_rename_at_1076_260);
        if ( __cut_rename_at_1084_261 )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1110;
                das_zero(__mks_1110);
                return __mks_1110;
            })()));
        };
    };
    int32_t __parse_pos_rename_at_1076_270 = __parser_rename_at_362_246.index;
    ++__parser_rename_at_362_246.tabs;
    bool __cut_rename_at_1084_271 = false;
    AutoTuple<bool,char *,int32_t> __res_3_rename_at_1086_272; das_zero(__res_3_rename_at_1086_272); das_move(__res_3_rename_at_1086_272, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        AutoTuple<bool,char *> ___terminal_result_17_rename_at_1353_273; das_zero(___terminal_result_17_rename_at_1353_273); das_move(___terminal_result_17_rename_at_1353_273, _FuncpegTickmatch_string_literalTick12800953725978677998_50b323a2328ca30f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *>::get(___terminal_result_17_rename_at_1353_273) )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1362_49_15; _temp_make_local_1362_49_15;
                char * __complete_message_rename_at_1361_274 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_49, cast<char * const >::from(((char *) "Error: Expected a string at ")), cast<int32_t>::from(__parser_rename_at_362_246.index)))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1362_49_15.text),(__complete_message_rename_at_1361_274));
                    das_copy((_temp_make_local_1362_49_15.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1362_49_15;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1365;
                das_zero(__mks_1365);
                return __mks_1365;
            })()));
        } else {
            char * __e_rename_at_1372_275 = (char *)(das_get_auto_tuple_field<char *,1,bool,char *>::get(___terminal_result_17_rename_at_1353_273));
            das_copy(__cut_rename_at_1084_271,true);
            char * __val_rename_at_1133_276; das_zero(__val_rename_at_1133_276); das_move(__val_rename_at_1133_276, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_50, cast<char * const >::from(((char *) "\"")), cast<char *>::from(__e_rename_at_1372_275), cast<char * const >::from(((char *) "\"")))));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1141_277; das_zero(__result_rename_at_1141_277); das_move(__result_rename_at_1141_277, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1141;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_276;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_246.index;
                return __mkt_1141;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_277);
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_3_rename_at_1086_272) )
    {
        --__parser_rename_at_362_246.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_3_rename_at_1086_272);
    } else {
        --__parser_rename_at_362_246.tabs;
        das_copy(__parser_rename_at_362_246.index,__parse_pos_rename_at_1076_270);
        if ( __cut_rename_at_1084_271 )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1110;
                das_zero(__mks_1110);
                return __mks_1110;
            })()));
        };
    };
    int32_t __parse_pos_rename_at_1076_278 = __parser_rename_at_362_246.index;
    ++__parser_rename_at_362_246.tabs;
    AutoTuple<bool,char *,int32_t> __res_4_rename_at_1086_279; das_zero(__res_4_rename_at_1086_279); das_move(__res_4_rename_at_1086_279, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        if ( !_FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),((char *) "("),1) )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1175_49_16; _temp_make_local_1175_49_16;
                char * __rest_rename_at_1173_280 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_51, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                char * __complete_message_rename_at_1174_281 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_52, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_280))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1175_49_16.text),(__complete_message_rename_at_1174_281));
                    das_copy((_temp_make_local_1175_49_16.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1175_49_16;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1179;
                das_zero(__mks_1179);
                return __mks_1179;
            })()));
        } else {
            AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __element2_rename_at_546_282; das_zero(__element2_rename_at_546_282); das_move(__element2_rename_at_546_282, _Funcparse_elementTickid_0x0_a89929d20275ecb4(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
            if ( !das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__element2_rename_at_546_282) )
            {
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_548;
                    das_zero(__mks_548);
                    return __mks_548;
                })()));
            } else {
                AutoTuple<char *,char *> __e_rename_at_551_283; das_zero(__e_rename_at_551_283); das_move(__e_rename_at_551_283, das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__element2_rename_at_546_282));
                if ( !_FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),((char *) ")"),1) )
                {
                    if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1175_49_17; _temp_make_local_1175_49_17;
                        char * __rest_rename_at_1173_284 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_53, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                        char * __complete_message_rename_at_1174_285 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_54, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_284))));
                        _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1175_49_17.text),(__complete_message_rename_at_1174_285));
                            das_copy((_temp_make_local_1175_49_17.index),(__parser_rename_at_362_246.index));
                            return _temp_make_local_1175_49_17;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                        AutoTuple<bool,char *,int32_t> __mks_1179;
                        das_zero(__mks_1179);
                        return __mks_1179;
                    })()));
                } else {
                    char * __val_rename_at_1133_286; das_zero(__val_rename_at_1133_286); das_move(__val_rename_at_1133_286, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                        return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_55, cast<char * const >::from(((char *) "(")), cast<AutoTuple<char *,char *> &>::from(__e_rename_at_551_283), cast<char * const >::from(((char *) ")")))));
                    })));
                    AutoTuple<bool,char *,int32_t> __result_rename_at_1141_287; das_zero(__result_rename_at_1141_287); das_move(__result_rename_at_1141_287, (([&]() -> AutoTuple<bool,char *,int32_t> {
                        AutoTuple<bool,char *,int32_t> __mkt_1141;
                        das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                        das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_286;
                        das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_246.index;
                        return __mkt_1141;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_287);
                };
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_4_rename_at_1086_279) )
    {
        --__parser_rename_at_362_246.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_4_rename_at_1086_279);
    } else {
        --__parser_rename_at_362_246.tabs;
        das_copy(__parser_rename_at_362_246.index,__parse_pos_rename_at_1076_278);
    };
    int32_t __parse_pos_rename_at_1076_288 = __parser_rename_at_362_246.index;
    ++__parser_rename_at_362_246.tabs;
    AutoTuple<bool,char *,int32_t> __res_5_rename_at_1086_289; das_zero(__res_5_rename_at_1086_289); das_move(__res_5_rename_at_1086_289, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        int32_t __ext_pos_1_rename_at_564_290 = __parser_rename_at_362_246.index;
        bool ___terminal_result_21_rename_at_1199_291 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
            TDim<bool,256> _temp_make_local_1195_39_18; _temp_make_local_1195_39_18;
            int32_t __ch_rename_at_1194_292 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
            return das_auto_cast<bool>::cast((__ch_rename_at_1194_292 != -1) && (([&]() -> TDim<bool,256>& {
                _temp_make_local_1195_39_18(0,__context__) = true;
                _temp_make_local_1195_39_18(1,__context__) = true;
                _temp_make_local_1195_39_18(2,__context__) = true;
                _temp_make_local_1195_39_18(3,__context__) = true;
                _temp_make_local_1195_39_18(4,__context__) = true;
                _temp_make_local_1195_39_18(5,__context__) = true;
                _temp_make_local_1195_39_18(6,__context__) = true;
                _temp_make_local_1195_39_18(7,__context__) = true;
                _temp_make_local_1195_39_18(8,__context__) = true;
                _temp_make_local_1195_39_18(9,__context__) = true;
                _temp_make_local_1195_39_18(10,__context__) = true;
                _temp_make_local_1195_39_18(11,__context__) = true;
                _temp_make_local_1195_39_18(12,__context__) = true;
                _temp_make_local_1195_39_18(13,__context__) = true;
                _temp_make_local_1195_39_18(14,__context__) = true;
                _temp_make_local_1195_39_18(15,__context__) = true;
                _temp_make_local_1195_39_18(16,__context__) = true;
                _temp_make_local_1195_39_18(17,__context__) = true;
                _temp_make_local_1195_39_18(18,__context__) = true;
                _temp_make_local_1195_39_18(19,__context__) = true;
                _temp_make_local_1195_39_18(20,__context__) = true;
                _temp_make_local_1195_39_18(21,__context__) = true;
                _temp_make_local_1195_39_18(22,__context__) = true;
                _temp_make_local_1195_39_18(23,__context__) = true;
                _temp_make_local_1195_39_18(24,__context__) = true;
                _temp_make_local_1195_39_18(25,__context__) = true;
                _temp_make_local_1195_39_18(26,__context__) = true;
                _temp_make_local_1195_39_18(27,__context__) = true;
                _temp_make_local_1195_39_18(28,__context__) = true;
                _temp_make_local_1195_39_18(29,__context__) = true;
                _temp_make_local_1195_39_18(30,__context__) = true;
                _temp_make_local_1195_39_18(31,__context__) = true;
                _temp_make_local_1195_39_18(32,__context__) = true;
                _temp_make_local_1195_39_18(33,__context__) = true;
                _temp_make_local_1195_39_18(34,__context__) = true;
                _temp_make_local_1195_39_18(35,__context__) = true;
                _temp_make_local_1195_39_18(36,__context__) = true;
                _temp_make_local_1195_39_18(37,__context__) = true;
                _temp_make_local_1195_39_18(38,__context__) = true;
                _temp_make_local_1195_39_18(39,__context__) = true;
                _temp_make_local_1195_39_18(40,__context__) = true;
                _temp_make_local_1195_39_18(41,__context__) = false;
                _temp_make_local_1195_39_18(42,__context__) = true;
                _temp_make_local_1195_39_18(43,__context__) = true;
                _temp_make_local_1195_39_18(44,__context__) = false;
                _temp_make_local_1195_39_18(45,__context__) = true;
                _temp_make_local_1195_39_18(46,__context__) = true;
                _temp_make_local_1195_39_18(47,__context__) = true;
                _temp_make_local_1195_39_18(48,__context__) = true;
                _temp_make_local_1195_39_18(49,__context__) = true;
                _temp_make_local_1195_39_18(50,__context__) = true;
                _temp_make_local_1195_39_18(51,__context__) = true;
                _temp_make_local_1195_39_18(52,__context__) = true;
                _temp_make_local_1195_39_18(53,__context__) = true;
                _temp_make_local_1195_39_18(54,__context__) = true;
                _temp_make_local_1195_39_18(55,__context__) = true;
                _temp_make_local_1195_39_18(56,__context__) = true;
                _temp_make_local_1195_39_18(57,__context__) = true;
                _temp_make_local_1195_39_18(58,__context__) = true;
                _temp_make_local_1195_39_18(59,__context__) = true;
                _temp_make_local_1195_39_18(60,__context__) = true;
                _temp_make_local_1195_39_18(61,__context__) = true;
                _temp_make_local_1195_39_18(62,__context__) = true;
                _temp_make_local_1195_39_18(63,__context__) = true;
                _temp_make_local_1195_39_18(64,__context__) = true;
                _temp_make_local_1195_39_18(65,__context__) = true;
                _temp_make_local_1195_39_18(66,__context__) = true;
                _temp_make_local_1195_39_18(67,__context__) = true;
                _temp_make_local_1195_39_18(68,__context__) = true;
                _temp_make_local_1195_39_18(69,__context__) = true;
                _temp_make_local_1195_39_18(70,__context__) = true;
                _temp_make_local_1195_39_18(71,__context__) = true;
                _temp_make_local_1195_39_18(72,__context__) = true;
                _temp_make_local_1195_39_18(73,__context__) = true;
                _temp_make_local_1195_39_18(74,__context__) = true;
                _temp_make_local_1195_39_18(75,__context__) = true;
                _temp_make_local_1195_39_18(76,__context__) = true;
                _temp_make_local_1195_39_18(77,__context__) = true;
                _temp_make_local_1195_39_18(78,__context__) = true;
                _temp_make_local_1195_39_18(79,__context__) = true;
                _temp_make_local_1195_39_18(80,__context__) = true;
                _temp_make_local_1195_39_18(81,__context__) = true;
                _temp_make_local_1195_39_18(82,__context__) = true;
                _temp_make_local_1195_39_18(83,__context__) = true;
                _temp_make_local_1195_39_18(84,__context__) = true;
                _temp_make_local_1195_39_18(85,__context__) = true;
                _temp_make_local_1195_39_18(86,__context__) = true;
                _temp_make_local_1195_39_18(87,__context__) = true;
                _temp_make_local_1195_39_18(88,__context__) = true;
                _temp_make_local_1195_39_18(89,__context__) = true;
                _temp_make_local_1195_39_18(90,__context__) = true;
                _temp_make_local_1195_39_18(91,__context__) = true;
                _temp_make_local_1195_39_18(92,__context__) = true;
                _temp_make_local_1195_39_18(93,__context__) = true;
                _temp_make_local_1195_39_18(94,__context__) = true;
                _temp_make_local_1195_39_18(95,__context__) = true;
                _temp_make_local_1195_39_18(96,__context__) = true;
                _temp_make_local_1195_39_18(97,__context__) = true;
                _temp_make_local_1195_39_18(98,__context__) = true;
                _temp_make_local_1195_39_18(99,__context__) = true;
                _temp_make_local_1195_39_18(100,__context__) = true;
                _temp_make_local_1195_39_18(101,__context__) = true;
                _temp_make_local_1195_39_18(102,__context__) = true;
                _temp_make_local_1195_39_18(103,__context__) = true;
                _temp_make_local_1195_39_18(104,__context__) = true;
                _temp_make_local_1195_39_18(105,__context__) = true;
                _temp_make_local_1195_39_18(106,__context__) = true;
                _temp_make_local_1195_39_18(107,__context__) = true;
                _temp_make_local_1195_39_18(108,__context__) = true;
                _temp_make_local_1195_39_18(109,__context__) = true;
                _temp_make_local_1195_39_18(110,__context__) = true;
                _temp_make_local_1195_39_18(111,__context__) = true;
                _temp_make_local_1195_39_18(112,__context__) = true;
                _temp_make_local_1195_39_18(113,__context__) = true;
                _temp_make_local_1195_39_18(114,__context__) = true;
                _temp_make_local_1195_39_18(115,__context__) = true;
                _temp_make_local_1195_39_18(116,__context__) = true;
                _temp_make_local_1195_39_18(117,__context__) = true;
                _temp_make_local_1195_39_18(118,__context__) = true;
                _temp_make_local_1195_39_18(119,__context__) = true;
                _temp_make_local_1195_39_18(120,__context__) = true;
                _temp_make_local_1195_39_18(121,__context__) = true;
                _temp_make_local_1195_39_18(122,__context__) = true;
                _temp_make_local_1195_39_18(123,__context__) = true;
                _temp_make_local_1195_39_18(124,__context__) = true;
                _temp_make_local_1195_39_18(125,__context__) = true;
                _temp_make_local_1195_39_18(126,__context__) = true;
                _temp_make_local_1195_39_18(127,__context__) = true;
                _temp_make_local_1195_39_18(128,__context__) = true;
                _temp_make_local_1195_39_18(129,__context__) = true;
                _temp_make_local_1195_39_18(130,__context__) = true;
                _temp_make_local_1195_39_18(131,__context__) = true;
                _temp_make_local_1195_39_18(132,__context__) = true;
                _temp_make_local_1195_39_18(133,__context__) = true;
                _temp_make_local_1195_39_18(134,__context__) = true;
                _temp_make_local_1195_39_18(135,__context__) = true;
                _temp_make_local_1195_39_18(136,__context__) = true;
                _temp_make_local_1195_39_18(137,__context__) = true;
                _temp_make_local_1195_39_18(138,__context__) = true;
                _temp_make_local_1195_39_18(139,__context__) = true;
                _temp_make_local_1195_39_18(140,__context__) = true;
                _temp_make_local_1195_39_18(141,__context__) = true;
                _temp_make_local_1195_39_18(142,__context__) = true;
                _temp_make_local_1195_39_18(143,__context__) = true;
                _temp_make_local_1195_39_18(144,__context__) = true;
                _temp_make_local_1195_39_18(145,__context__) = true;
                _temp_make_local_1195_39_18(146,__context__) = true;
                _temp_make_local_1195_39_18(147,__context__) = true;
                _temp_make_local_1195_39_18(148,__context__) = true;
                _temp_make_local_1195_39_18(149,__context__) = true;
                _temp_make_local_1195_39_18(150,__context__) = true;
                _temp_make_local_1195_39_18(151,__context__) = true;
                _temp_make_local_1195_39_18(152,__context__) = true;
                _temp_make_local_1195_39_18(153,__context__) = true;
                _temp_make_local_1195_39_18(154,__context__) = true;
                _temp_make_local_1195_39_18(155,__context__) = true;
                _temp_make_local_1195_39_18(156,__context__) = true;
                _temp_make_local_1195_39_18(157,__context__) = true;
                _temp_make_local_1195_39_18(158,__context__) = true;
                _temp_make_local_1195_39_18(159,__context__) = true;
                _temp_make_local_1195_39_18(160,__context__) = true;
                _temp_make_local_1195_39_18(161,__context__) = true;
                _temp_make_local_1195_39_18(162,__context__) = true;
                _temp_make_local_1195_39_18(163,__context__) = true;
                _temp_make_local_1195_39_18(164,__context__) = true;
                _temp_make_local_1195_39_18(165,__context__) = true;
                _temp_make_local_1195_39_18(166,__context__) = true;
                _temp_make_local_1195_39_18(167,__context__) = true;
                _temp_make_local_1195_39_18(168,__context__) = true;
                _temp_make_local_1195_39_18(169,__context__) = true;
                _temp_make_local_1195_39_18(170,__context__) = true;
                _temp_make_local_1195_39_18(171,__context__) = true;
                _temp_make_local_1195_39_18(172,__context__) = true;
                _temp_make_local_1195_39_18(173,__context__) = true;
                _temp_make_local_1195_39_18(174,__context__) = true;
                _temp_make_local_1195_39_18(175,__context__) = true;
                _temp_make_local_1195_39_18(176,__context__) = true;
                _temp_make_local_1195_39_18(177,__context__) = true;
                _temp_make_local_1195_39_18(178,__context__) = true;
                _temp_make_local_1195_39_18(179,__context__) = true;
                _temp_make_local_1195_39_18(180,__context__) = true;
                _temp_make_local_1195_39_18(181,__context__) = true;
                _temp_make_local_1195_39_18(182,__context__) = true;
                _temp_make_local_1195_39_18(183,__context__) = true;
                _temp_make_local_1195_39_18(184,__context__) = true;
                _temp_make_local_1195_39_18(185,__context__) = true;
                _temp_make_local_1195_39_18(186,__context__) = true;
                _temp_make_local_1195_39_18(187,__context__) = true;
                _temp_make_local_1195_39_18(188,__context__) = true;
                _temp_make_local_1195_39_18(189,__context__) = true;
                _temp_make_local_1195_39_18(190,__context__) = true;
                _temp_make_local_1195_39_18(191,__context__) = true;
                _temp_make_local_1195_39_18(192,__context__) = true;
                _temp_make_local_1195_39_18(193,__context__) = true;
                _temp_make_local_1195_39_18(194,__context__) = true;
                _temp_make_local_1195_39_18(195,__context__) = true;
                _temp_make_local_1195_39_18(196,__context__) = true;
                _temp_make_local_1195_39_18(197,__context__) = true;
                _temp_make_local_1195_39_18(198,__context__) = true;
                _temp_make_local_1195_39_18(199,__context__) = true;
                _temp_make_local_1195_39_18(200,__context__) = true;
                _temp_make_local_1195_39_18(201,__context__) = true;
                _temp_make_local_1195_39_18(202,__context__) = true;
                _temp_make_local_1195_39_18(203,__context__) = true;
                _temp_make_local_1195_39_18(204,__context__) = true;
                _temp_make_local_1195_39_18(205,__context__) = true;
                _temp_make_local_1195_39_18(206,__context__) = true;
                _temp_make_local_1195_39_18(207,__context__) = true;
                _temp_make_local_1195_39_18(208,__context__) = true;
                _temp_make_local_1195_39_18(209,__context__) = true;
                _temp_make_local_1195_39_18(210,__context__) = true;
                _temp_make_local_1195_39_18(211,__context__) = true;
                _temp_make_local_1195_39_18(212,__context__) = true;
                _temp_make_local_1195_39_18(213,__context__) = true;
                _temp_make_local_1195_39_18(214,__context__) = true;
                _temp_make_local_1195_39_18(215,__context__) = true;
                _temp_make_local_1195_39_18(216,__context__) = true;
                _temp_make_local_1195_39_18(217,__context__) = true;
                _temp_make_local_1195_39_18(218,__context__) = true;
                _temp_make_local_1195_39_18(219,__context__) = true;
                _temp_make_local_1195_39_18(220,__context__) = true;
                _temp_make_local_1195_39_18(221,__context__) = true;
                _temp_make_local_1195_39_18(222,__context__) = true;
                _temp_make_local_1195_39_18(223,__context__) = true;
                _temp_make_local_1195_39_18(224,__context__) = true;
                _temp_make_local_1195_39_18(225,__context__) = true;
                _temp_make_local_1195_39_18(226,__context__) = true;
                _temp_make_local_1195_39_18(227,__context__) = true;
                _temp_make_local_1195_39_18(228,__context__) = true;
                _temp_make_local_1195_39_18(229,__context__) = true;
                _temp_make_local_1195_39_18(230,__context__) = true;
                _temp_make_local_1195_39_18(231,__context__) = true;
                _temp_make_local_1195_39_18(232,__context__) = true;
                _temp_make_local_1195_39_18(233,__context__) = true;
                _temp_make_local_1195_39_18(234,__context__) = true;
                _temp_make_local_1195_39_18(235,__context__) = true;
                _temp_make_local_1195_39_18(236,__context__) = true;
                _temp_make_local_1195_39_18(237,__context__) = true;
                _temp_make_local_1195_39_18(238,__context__) = true;
                _temp_make_local_1195_39_18(239,__context__) = true;
                _temp_make_local_1195_39_18(240,__context__) = true;
                _temp_make_local_1195_39_18(241,__context__) = true;
                _temp_make_local_1195_39_18(242,__context__) = true;
                _temp_make_local_1195_39_18(243,__context__) = true;
                _temp_make_local_1195_39_18(244,__context__) = true;
                _temp_make_local_1195_39_18(245,__context__) = true;
                _temp_make_local_1195_39_18(246,__context__) = true;
                _temp_make_local_1195_39_18(247,__context__) = true;
                _temp_make_local_1195_39_18(248,__context__) = true;
                _temp_make_local_1195_39_18(249,__context__) = true;
                _temp_make_local_1195_39_18(250,__context__) = true;
                _temp_make_local_1195_39_18(251,__context__) = true;
                _temp_make_local_1195_39_18(252,__context__) = true;
                _temp_make_local_1195_39_18(253,__context__) = true;
                _temp_make_local_1195_39_18(254,__context__) = true;
                _temp_make_local_1195_39_18(255,__context__) = true;
                return _temp_make_local_1195_39_18;
            })())(__ch_rename_at_1194_292,__context__));
        });
        if ( !___terminal_result_21_rename_at_1199_291 )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1217_49_19; _temp_make_local_1217_49_19;
                char * __rest_rename_at_1215_293 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_56, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                char * __complete_message_rename_at_1216_294 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_57, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '0'-'40'  '42'-'43'  '45'-'256' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1215_293))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1217_49_19.text),(__complete_message_rename_at_1216_294));
                    das_copy((_temp_make_local_1217_49_19.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1217_49_19;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1220;
                das_zero(__mks_1220);
                return __mks_1220;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),1);
        };
        char * __str_1_rename_at_566_295 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_246.input(0,__context__))));
        char * __e_rename_at_567_296 = (char *)(((char * const )(builtin_string_slice1(__str_1_rename_at_566_295,__ext_pos_1_rename_at_564_290,__parser_rename_at_362_246.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        char * __val_rename_at_1133_297; das_zero(__val_rename_at_1133_297); das_move(__val_rename_at_1133_297, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
            return das_auto_cast<char *>::cast(__e_rename_at_567_296);
        })));
        AutoTuple<bool,char *,int32_t> __result_rename_at_1141_298; das_zero(__result_rename_at_1141_298); das_move(__result_rename_at_1141_298, (([&]() -> AutoTuple<bool,char *,int32_t> {
            AutoTuple<bool,char *,int32_t> __mkt_1141;
            das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
            das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_297;
            das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_246.index;
            return __mkt_1141;
        })()));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_298);
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_5_rename_at_1086_289) )
    {
        --__parser_rename_at_362_246.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_5_rename_at_1086_289);
    } else {
        --__parser_rename_at_362_246.tabs;
        das_copy(__parser_rename_at_362_246.index,__parse_pos_rename_at_1076_288);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
        AutoTuple<bool,char *,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_elementTickid_0x0_aac556ebc4770900 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_299 )
{
    int32_t __mark_rename_at_147_300 = __parser_rename_at_146_299.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_299.sub_element_cache),__parser_rename_at_146_299.index) && !(__parser_rename_at_146_299.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_301; das_zero(__result_rename_at_155_301); das_move(__result_rename_at_155_301, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_299.sub_element_cache(__parser_rename_at_146_299.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_301) )
        {
            das_copy(__parser_rename_at_146_299.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_301));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_301);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_302; das_zero(__result_rename_at_169_302); das_move(__result_rename_at_169_302, _Funcparse_sub_element_innerTickid_0x0_4416149c422be215(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_299)));
        das_copy(__parser_rename_at_146_299.sub_element_cache(__mark_rename_at_147_300,__context__),__result_rename_at_169_302);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_302);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_element_value_innerTickid_0x0_4a321894fbeec2f ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_303 )
{
    int32_t __parse_pos_rename_at_1076_304 = __parser_rename_at_362_303.index;
    ++__parser_rename_at_362_303.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_305; das_zero(__res_1_rename_at_1086_305); das_move(__res_1_rename_at_1086_305, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        AutoTuple<bool,char *,int32_t> __sub_element0_rename_at_546_306; das_zero(__sub_element0_rename_at_546_306); das_move(__sub_element0_rename_at_546_306, _Funcparse_sub_elementTickid_0x0_aac556ebc4770900(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_303)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__sub_element0_rename_at_546_306) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __head_rename_at_551_307; das_zero(__head_rename_at_551_307); das_move(__head_rename_at_551_307, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__sub_element0_rename_at_546_306)));
            TArray<char *> __tail_rename_at_671_308;das_zero(__tail_rename_at_671_308);
            _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_303.suppress_errors),true);
            while ( true )
            {
                int32_t __sz_rename_at_674_309 = builtin_array_size(das_arg<TArray<char *>>::pass(__tail_rename_at_671_308));
                das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                    AutoTuple<bool,char *,int32_t> __sub_element1_rename_at_546_310; das_zero(__sub_element1_rename_at_546_310); das_move(__sub_element1_rename_at_546_310, _Funcparse_sub_elementTickid_0x0_aac556ebc4770900(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_303)));
                    if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__sub_element1_rename_at_546_310) )
                    {
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                            AutoTuple<bool,int32_t,int32_t> __mks_548;
                            das_zero(__mks_548);
                            return __mks_548;
                        })()));
                    } else {
                        char * __temp_name_rename_at_551_311; das_zero(__temp_name_rename_at_551_311); das_move(__temp_name_rename_at_551_311, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__sub_element1_rename_at_546_310)));
                        _FuncbuiltinTickemplaceTick13923705686753630697_2d94a9a555d8bee7(__context__,das_arg<TArray<char *>>::pass(__tail_rename_at_671_308),__temp_name_rename_at_551_311);
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                            AutoTuple<bool,int32_t,int32_t> __mkt_666;
                            das_get_auto_tuple_field<bool,0,bool,int32_t,int32_t>::get(__mkt_666) = true;
                            das_get_auto_tuple_field<int32_t,1,bool,int32_t,int32_t>::get(__mkt_666) = 0;
                            das_get_auto_tuple_field<int32_t,2,bool,int32_t,int32_t>::get(__mkt_666) = 0;
                            return __mkt_666;
                        })()));
                    };
                });
                if ( __sz_rename_at_674_309 == builtin_array_size(das_arg<TArray<char *>>::pass(__tail_rename_at_671_308)) )
                {
                    break;
                };
            };
            _FuncbuiltinTickpopTick1161079256290593740_e59f84a1c28ac19(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_303.suppress_errors));
            char * __val_rename_at_1133_312; das_zero(__val_rename_at_1133_312); das_move(__val_rename_at_1133_312, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__head_rename_at_551_307),cast<char *>::from(_Funcstrings_boostTickjoinTick16475640899284277631_16b92910c086970e(__context__,das_arg<TArray<char *>>::pass(__tail_rename_at_671_308),nullptr)),*__context__,nullptr)));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1141_313; das_zero(__result_rename_at_1141_313); das_move(__result_rename_at_1141_313, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1141;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_312;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_303.index;
                return __mkt_1141;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_313);
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_1_rename_at_1086_305) )
    {
        --__parser_rename_at_362_303.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_1_rename_at_1086_305);
    } else {
        --__parser_rename_at_362_303.tabs;
        das_copy(__parser_rename_at_362_303.index,__parse_pos_rename_at_1076_304);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
        AutoTuple<bool,char *,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_element_valueTickid_0x0_126b58a15c99cc72 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_314 )
{
    int32_t __mark_rename_at_147_315 = __parser_rename_at_146_314.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_314.element_value_cache),__parser_rename_at_146_314.index) && !(__parser_rename_at_146_314.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_316; das_zero(__result_rename_at_155_316); das_move(__result_rename_at_155_316, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_314.element_value_cache(__parser_rename_at_146_314.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_316) )
        {
            das_copy(__parser_rename_at_146_314.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_316));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_316);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_317; das_zero(__result_rename_at_169_317); das_move(__result_rename_at_169_317, _Funcparse_element_value_innerTickid_0x0_4a321894fbeec2f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_314)));
        das_copy(__parser_rename_at_146_314.element_value_cache(__mark_rename_at_147_315,__context__),__result_rename_at_169_317);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_317);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_ident__innerTickid_0x0_1def850e72a70e16 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_318 )
{
    int32_t __parse_pos_rename_at_1076_319 = __parser_rename_at_362_318.index;
    ++__parser_rename_at_362_318.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_320; das_zero(__res_1_rename_at_1086_320); das_move(__res_1_rename_at_1086_320, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        int32_t __ext_pos_2_rename_at_564_321 = __parser_rename_at_362_318.index;
        bool ___terminal_result_22_rename_at_1199_322 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
            TDim<bool,256> _temp_make_local_1195_39_20; _temp_make_local_1195_39_20;
            int32_t __ch_rename_at_1194_323 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_318)));
            return das_auto_cast<bool>::cast((__ch_rename_at_1194_323 != -1) && (([&]() -> TDim<bool,256>& {
                _temp_make_local_1195_39_20(0,__context__) = false;
                _temp_make_local_1195_39_20(1,__context__) = false;
                _temp_make_local_1195_39_20(2,__context__) = false;
                _temp_make_local_1195_39_20(3,__context__) = false;
                _temp_make_local_1195_39_20(4,__context__) = false;
                _temp_make_local_1195_39_20(5,__context__) = false;
                _temp_make_local_1195_39_20(6,__context__) = false;
                _temp_make_local_1195_39_20(7,__context__) = false;
                _temp_make_local_1195_39_20(8,__context__) = false;
                _temp_make_local_1195_39_20(9,__context__) = false;
                _temp_make_local_1195_39_20(10,__context__) = false;
                _temp_make_local_1195_39_20(11,__context__) = false;
                _temp_make_local_1195_39_20(12,__context__) = false;
                _temp_make_local_1195_39_20(13,__context__) = false;
                _temp_make_local_1195_39_20(14,__context__) = false;
                _temp_make_local_1195_39_20(15,__context__) = false;
                _temp_make_local_1195_39_20(16,__context__) = false;
                _temp_make_local_1195_39_20(17,__context__) = false;
                _temp_make_local_1195_39_20(18,__context__) = false;
                _temp_make_local_1195_39_20(19,__context__) = false;
                _temp_make_local_1195_39_20(20,__context__) = false;
                _temp_make_local_1195_39_20(21,__context__) = false;
                _temp_make_local_1195_39_20(22,__context__) = false;
                _temp_make_local_1195_39_20(23,__context__) = false;
                _temp_make_local_1195_39_20(24,__context__) = false;
                _temp_make_local_1195_39_20(25,__context__) = false;
                _temp_make_local_1195_39_20(26,__context__) = false;
                _temp_make_local_1195_39_20(27,__context__) = false;
                _temp_make_local_1195_39_20(28,__context__) = false;
                _temp_make_local_1195_39_20(29,__context__) = false;
                _temp_make_local_1195_39_20(30,__context__) = false;
                _temp_make_local_1195_39_20(31,__context__) = false;
                _temp_make_local_1195_39_20(32,__context__) = false;
                _temp_make_local_1195_39_20(33,__context__) = false;
                _temp_make_local_1195_39_20(34,__context__) = false;
                _temp_make_local_1195_39_20(35,__context__) = false;
                _temp_make_local_1195_39_20(36,__context__) = false;
                _temp_make_local_1195_39_20(37,__context__) = false;
                _temp_make_local_1195_39_20(38,__context__) = false;
                _temp_make_local_1195_39_20(39,__context__) = false;
                _temp_make_local_1195_39_20(40,__context__) = false;
                _temp_make_local_1195_39_20(41,__context__) = false;
                _temp_make_local_1195_39_20(42,__context__) = false;
                _temp_make_local_1195_39_20(43,__context__) = false;
                _temp_make_local_1195_39_20(44,__context__) = false;
                _temp_make_local_1195_39_20(45,__context__) = false;
                _temp_make_local_1195_39_20(46,__context__) = false;
                _temp_make_local_1195_39_20(47,__context__) = false;
                _temp_make_local_1195_39_20(48,__context__) = false;
                _temp_make_local_1195_39_20(49,__context__) = false;
                _temp_make_local_1195_39_20(50,__context__) = false;
                _temp_make_local_1195_39_20(51,__context__) = false;
                _temp_make_local_1195_39_20(52,__context__) = false;
                _temp_make_local_1195_39_20(53,__context__) = false;
                _temp_make_local_1195_39_20(54,__context__) = false;
                _temp_make_local_1195_39_20(55,__context__) = false;
                _temp_make_local_1195_39_20(56,__context__) = false;
                _temp_make_local_1195_39_20(57,__context__) = false;
                _temp_make_local_1195_39_20(58,__context__) = false;
                _temp_make_local_1195_39_20(59,__context__) = false;
                _temp_make_local_1195_39_20(60,__context__) = false;
                _temp_make_local_1195_39_20(61,__context__) = false;
                _temp_make_local_1195_39_20(62,__context__) = false;
                _temp_make_local_1195_39_20(63,__context__) = false;
                _temp_make_local_1195_39_20(64,__context__) = false;
                _temp_make_local_1195_39_20(65,__context__) = true;
                _temp_make_local_1195_39_20(66,__context__) = true;
                _temp_make_local_1195_39_20(67,__context__) = true;
                _temp_make_local_1195_39_20(68,__context__) = true;
                _temp_make_local_1195_39_20(69,__context__) = true;
                _temp_make_local_1195_39_20(70,__context__) = true;
                _temp_make_local_1195_39_20(71,__context__) = true;
                _temp_make_local_1195_39_20(72,__context__) = true;
                _temp_make_local_1195_39_20(73,__context__) = true;
                _temp_make_local_1195_39_20(74,__context__) = true;
                _temp_make_local_1195_39_20(75,__context__) = true;
                _temp_make_local_1195_39_20(76,__context__) = true;
                _temp_make_local_1195_39_20(77,__context__) = true;
                _temp_make_local_1195_39_20(78,__context__) = true;
                _temp_make_local_1195_39_20(79,__context__) = true;
                _temp_make_local_1195_39_20(80,__context__) = true;
                _temp_make_local_1195_39_20(81,__context__) = true;
                _temp_make_local_1195_39_20(82,__context__) = true;
                _temp_make_local_1195_39_20(83,__context__) = true;
                _temp_make_local_1195_39_20(84,__context__) = true;
                _temp_make_local_1195_39_20(85,__context__) = true;
                _temp_make_local_1195_39_20(86,__context__) = true;
                _temp_make_local_1195_39_20(87,__context__) = true;
                _temp_make_local_1195_39_20(88,__context__) = true;
                _temp_make_local_1195_39_20(89,__context__) = true;
                _temp_make_local_1195_39_20(90,__context__) = true;
                _temp_make_local_1195_39_20(91,__context__) = false;
                _temp_make_local_1195_39_20(92,__context__) = false;
                _temp_make_local_1195_39_20(93,__context__) = false;
                _temp_make_local_1195_39_20(94,__context__) = false;
                _temp_make_local_1195_39_20(95,__context__) = true;
                _temp_make_local_1195_39_20(96,__context__) = false;
                _temp_make_local_1195_39_20(97,__context__) = true;
                _temp_make_local_1195_39_20(98,__context__) = true;
                _temp_make_local_1195_39_20(99,__context__) = true;
                _temp_make_local_1195_39_20(100,__context__) = true;
                _temp_make_local_1195_39_20(101,__context__) = true;
                _temp_make_local_1195_39_20(102,__context__) = true;
                _temp_make_local_1195_39_20(103,__context__) = true;
                _temp_make_local_1195_39_20(104,__context__) = true;
                _temp_make_local_1195_39_20(105,__context__) = true;
                _temp_make_local_1195_39_20(106,__context__) = true;
                _temp_make_local_1195_39_20(107,__context__) = true;
                _temp_make_local_1195_39_20(108,__context__) = true;
                _temp_make_local_1195_39_20(109,__context__) = true;
                _temp_make_local_1195_39_20(110,__context__) = true;
                _temp_make_local_1195_39_20(111,__context__) = true;
                _temp_make_local_1195_39_20(112,__context__) = true;
                _temp_make_local_1195_39_20(113,__context__) = true;
                _temp_make_local_1195_39_20(114,__context__) = true;
                _temp_make_local_1195_39_20(115,__context__) = true;
                _temp_make_local_1195_39_20(116,__context__) = true;
                _temp_make_local_1195_39_20(117,__context__) = true;
                _temp_make_local_1195_39_20(118,__context__) = true;
                _temp_make_local_1195_39_20(119,__context__) = true;
                _temp_make_local_1195_39_20(120,__context__) = true;
                _temp_make_local_1195_39_20(121,__context__) = true;
                _temp_make_local_1195_39_20(122,__context__) = true;
                _temp_make_local_1195_39_20(123,__context__) = false;
                _temp_make_local_1195_39_20(124,__context__) = false;
                _temp_make_local_1195_39_20(125,__context__) = false;
                _temp_make_local_1195_39_20(126,__context__) = false;
                _temp_make_local_1195_39_20(127,__context__) = false;
                _temp_make_local_1195_39_20(128,__context__) = false;
                _temp_make_local_1195_39_20(129,__context__) = false;
                _temp_make_local_1195_39_20(130,__context__) = false;
                _temp_make_local_1195_39_20(131,__context__) = false;
                _temp_make_local_1195_39_20(132,__context__) = false;
                _temp_make_local_1195_39_20(133,__context__) = false;
                _temp_make_local_1195_39_20(134,__context__) = false;
                _temp_make_local_1195_39_20(135,__context__) = false;
                _temp_make_local_1195_39_20(136,__context__) = false;
                _temp_make_local_1195_39_20(137,__context__) = false;
                _temp_make_local_1195_39_20(138,__context__) = false;
                _temp_make_local_1195_39_20(139,__context__) = false;
                _temp_make_local_1195_39_20(140,__context__) = false;
                _temp_make_local_1195_39_20(141,__context__) = false;
                _temp_make_local_1195_39_20(142,__context__) = false;
                _temp_make_local_1195_39_20(143,__context__) = false;
                _temp_make_local_1195_39_20(144,__context__) = false;
                _temp_make_local_1195_39_20(145,__context__) = false;
                _temp_make_local_1195_39_20(146,__context__) = false;
                _temp_make_local_1195_39_20(147,__context__) = false;
                _temp_make_local_1195_39_20(148,__context__) = false;
                _temp_make_local_1195_39_20(149,__context__) = false;
                _temp_make_local_1195_39_20(150,__context__) = false;
                _temp_make_local_1195_39_20(151,__context__) = false;
                _temp_make_local_1195_39_20(152,__context__) = false;
                _temp_make_local_1195_39_20(153,__context__) = false;
                _temp_make_local_1195_39_20(154,__context__) = false;
                _temp_make_local_1195_39_20(155,__context__) = false;
                _temp_make_local_1195_39_20(156,__context__) = false;
                _temp_make_local_1195_39_20(157,__context__) = false;
                _temp_make_local_1195_39_20(158,__context__) = false;
                _temp_make_local_1195_39_20(159,__context__) = false;
                _temp_make_local_1195_39_20(160,__context__) = false;
                _temp_make_local_1195_39_20(161,__context__) = false;
                _temp_make_local_1195_39_20(162,__context__) = false;
                _temp_make_local_1195_39_20(163,__context__) = false;
                _temp_make_local_1195_39_20(164,__context__) = false;
                _temp_make_local_1195_39_20(165,__context__) = false;
                _temp_make_local_1195_39_20(166,__context__) = false;
                _temp_make_local_1195_39_20(167,__context__) = false;
                _temp_make_local_1195_39_20(168,__context__) = false;
                _temp_make_local_1195_39_20(169,__context__) = false;
                _temp_make_local_1195_39_20(170,__context__) = false;
                _temp_make_local_1195_39_20(171,__context__) = false;
                _temp_make_local_1195_39_20(172,__context__) = false;
                _temp_make_local_1195_39_20(173,__context__) = false;
                _temp_make_local_1195_39_20(174,__context__) = false;
                _temp_make_local_1195_39_20(175,__context__) = false;
                _temp_make_local_1195_39_20(176,__context__) = false;
                _temp_make_local_1195_39_20(177,__context__) = false;
                _temp_make_local_1195_39_20(178,__context__) = false;
                _temp_make_local_1195_39_20(179,__context__) = false;
                _temp_make_local_1195_39_20(180,__context__) = false;
                _temp_make_local_1195_39_20(181,__context__) = false;
                _temp_make_local_1195_39_20(182,__context__) = false;
                _temp_make_local_1195_39_20(183,__context__) = false;
                _temp_make_local_1195_39_20(184,__context__) = false;
                _temp_make_local_1195_39_20(185,__context__) = false;
                _temp_make_local_1195_39_20(186,__context__) = false;
                _temp_make_local_1195_39_20(187,__context__) = false;
                _temp_make_local_1195_39_20(188,__context__) = false;
                _temp_make_local_1195_39_20(189,__context__) = false;
                _temp_make_local_1195_39_20(190,__context__) = false;
                _temp_make_local_1195_39_20(191,__context__) = false;
                _temp_make_local_1195_39_20(192,__context__) = false;
                _temp_make_local_1195_39_20(193,__context__) = false;
                _temp_make_local_1195_39_20(194,__context__) = false;
                _temp_make_local_1195_39_20(195,__context__) = false;
                _temp_make_local_1195_39_20(196,__context__) = false;
                _temp_make_local_1195_39_20(197,__context__) = false;
                _temp_make_local_1195_39_20(198,__context__) = false;
                _temp_make_local_1195_39_20(199,__context__) = false;
                _temp_make_local_1195_39_20(200,__context__) = false;
                _temp_make_local_1195_39_20(201,__context__) = false;
                _temp_make_local_1195_39_20(202,__context__) = false;
                _temp_make_local_1195_39_20(203,__context__) = false;
                _temp_make_local_1195_39_20(204,__context__) = false;
                _temp_make_local_1195_39_20(205,__context__) = false;
                _temp_make_local_1195_39_20(206,__context__) = false;
                _temp_make_local_1195_39_20(207,__context__) = false;
                _temp_make_local_1195_39_20(208,__context__) = false;
                _temp_make_local_1195_39_20(209,__context__) = false;
                _temp_make_local_1195_39_20(210,__context__) = false;
                _temp_make_local_1195_39_20(211,__context__) = false;
                _temp_make_local_1195_39_20(212,__context__) = false;
                _temp_make_local_1195_39_20(213,__context__) = false;
                _temp_make_local_1195_39_20(214,__context__) = false;
                _temp_make_local_1195_39_20(215,__context__) = false;
                _temp_make_local_1195_39_20(216,__context__) = false;
                _temp_make_local_1195_39_20(217,__context__) = false;
                _temp_make_local_1195_39_20(218,__context__) = false;
                _temp_make_local_1195_39_20(219,__context__) = false;
                _temp_make_local_1195_39_20(220,__context__) = false;
                _temp_make_local_1195_39_20(221,__context__) = false;
                _temp_make_local_1195_39_20(222,__context__) = false;
                _temp_make_local_1195_39_20(223,__context__) = false;
                _temp_make_local_1195_39_20(224,__context__) = false;
                _temp_make_local_1195_39_20(225,__context__) = false;
                _temp_make_local_1195_39_20(226,__context__) = false;
                _temp_make_local_1195_39_20(227,__context__) = false;
                _temp_make_local_1195_39_20(228,__context__) = false;
                _temp_make_local_1195_39_20(229,__context__) = false;
                _temp_make_local_1195_39_20(230,__context__) = false;
                _temp_make_local_1195_39_20(231,__context__) = false;
                _temp_make_local_1195_39_20(232,__context__) = false;
                _temp_make_local_1195_39_20(233,__context__) = false;
                _temp_make_local_1195_39_20(234,__context__) = false;
                _temp_make_local_1195_39_20(235,__context__) = false;
                _temp_make_local_1195_39_20(236,__context__) = false;
                _temp_make_local_1195_39_20(237,__context__) = false;
                _temp_make_local_1195_39_20(238,__context__) = false;
                _temp_make_local_1195_39_20(239,__context__) = false;
                _temp_make_local_1195_39_20(240,__context__) = false;
                _temp_make_local_1195_39_20(241,__context__) = false;
                _temp_make_local_1195_39_20(242,__context__) = false;
                _temp_make_local_1195_39_20(243,__context__) = false;
                _temp_make_local_1195_39_20(244,__context__) = false;
                _temp_make_local_1195_39_20(245,__context__) = false;
                _temp_make_local_1195_39_20(246,__context__) = false;
                _temp_make_local_1195_39_20(247,__context__) = false;
                _temp_make_local_1195_39_20(248,__context__) = false;
                _temp_make_local_1195_39_20(249,__context__) = false;
                _temp_make_local_1195_39_20(250,__context__) = false;
                _temp_make_local_1195_39_20(251,__context__) = false;
                _temp_make_local_1195_39_20(252,__context__) = false;
                _temp_make_local_1195_39_20(253,__context__) = false;
                _temp_make_local_1195_39_20(254,__context__) = false;
                _temp_make_local_1195_39_20(255,__context__) = false;
                return _temp_make_local_1195_39_20;
            })())(__ch_rename_at_1194_323,__context__));
        });
        if ( !___terminal_result_22_rename_at_1199_322 )
        {
            if ( (__parser_rename_at_362_318.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_318.suppress_errors)))) && (__parser_rename_at_362_318.index == __parser_rename_at_362_318.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1217_49_21; _temp_make_local_1217_49_21;
                char * __rest_rename_at_1215_324 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_58, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_318.index))),false));
                char * __complete_message_rename_at_1216_325 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_59, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '65'-'90'  '95'  '97'-'122' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1215_324))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_318.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1217_49_21.text),(__complete_message_rename_at_1216_325));
                    das_copy((_temp_make_local_1217_49_21.index),(__parser_rename_at_362_318.index));
                    return _temp_make_local_1217_49_21;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1220;
                das_zero(__mks_1220);
                return __mks_1220;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_318),1);
        };
        char * __str_2_rename_at_566_326 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_318.input(0,__context__))));
        char * __fc_rename_at_567_327 = (char *)(((char * const )(builtin_string_slice1(__str_2_rename_at_566_326,__ext_pos_2_rename_at_564_321,__parser_rename_at_362_318.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        int32_t __ext_pos_3_rename_at_564_328 = __parser_rename_at_362_318.index;
        _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_318.suppress_errors),true);
        while ( true )
        {
            AutoTuple<bool,int32_t,int32_t> __res_rename_at_644_329; das_zero(__res_rename_at_644_329); das_move(__res_rename_at_644_329, das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                bool ___terminal_result_23_rename_at_1199_330 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
                    TDim<bool,256> _temp_make_local_1195_39_22; _temp_make_local_1195_39_22;
                    int32_t __ch_rename_at_1194_331 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_318)));
                    return das_auto_cast<bool>::cast((__ch_rename_at_1194_331 != -1) && (([&]() -> TDim<bool,256>& {
                        _temp_make_local_1195_39_22(0,__context__) = false;
                        _temp_make_local_1195_39_22(1,__context__) = false;
                        _temp_make_local_1195_39_22(2,__context__) = false;
                        _temp_make_local_1195_39_22(3,__context__) = false;
                        _temp_make_local_1195_39_22(4,__context__) = false;
                        _temp_make_local_1195_39_22(5,__context__) = false;
                        _temp_make_local_1195_39_22(6,__context__) = false;
                        _temp_make_local_1195_39_22(7,__context__) = false;
                        _temp_make_local_1195_39_22(8,__context__) = false;
                        _temp_make_local_1195_39_22(9,__context__) = false;
                        _temp_make_local_1195_39_22(10,__context__) = false;
                        _temp_make_local_1195_39_22(11,__context__) = false;
                        _temp_make_local_1195_39_22(12,__context__) = false;
                        _temp_make_local_1195_39_22(13,__context__) = false;
                        _temp_make_local_1195_39_22(14,__context__) = false;
                        _temp_make_local_1195_39_22(15,__context__) = false;
                        _temp_make_local_1195_39_22(16,__context__) = false;
                        _temp_make_local_1195_39_22(17,__context__) = false;
                        _temp_make_local_1195_39_22(18,__context__) = false;
                        _temp_make_local_1195_39_22(19,__context__) = false;
                        _temp_make_local_1195_39_22(20,__context__) = false;
                        _temp_make_local_1195_39_22(21,__context__) = false;
                        _temp_make_local_1195_39_22(22,__context__) = false;
                        _temp_make_local_1195_39_22(23,__context__) = false;
                        _temp_make_local_1195_39_22(24,__context__) = false;
                        _temp_make_local_1195_39_22(25,__context__) = false;
                        _temp_make_local_1195_39_22(26,__context__) = false;
                        _temp_make_local_1195_39_22(27,__context__) = false;
                        _temp_make_local_1195_39_22(28,__context__) = false;
                        _temp_make_local_1195_39_22(29,__context__) = false;
                        _temp_make_local_1195_39_22(30,__context__) = false;
                        _temp_make_local_1195_39_22(31,__context__) = false;
                        _temp_make_local_1195_39_22(32,__context__) = false;
                        _temp_make_local_1195_39_22(33,__context__) = false;
                        _temp_make_local_1195_39_22(34,__context__) = false;
                        _temp_make_local_1195_39_22(35,__context__) = false;
                        _temp_make_local_1195_39_22(36,__context__) = false;
                        _temp_make_local_1195_39_22(37,__context__) = false;
                        _temp_make_local_1195_39_22(38,__context__) = false;
                        _temp_make_local_1195_39_22(39,__context__) = false;
                        _temp_make_local_1195_39_22(40,__context__) = false;
                        _temp_make_local_1195_39_22(41,__context__) = false;
                        _temp_make_local_1195_39_22(42,__context__) = false;
                        _temp_make_local_1195_39_22(43,__context__) = false;
                        _temp_make_local_1195_39_22(44,__context__) = false;
                        _temp_make_local_1195_39_22(45,__context__) = false;
                        _temp_make_local_1195_39_22(46,__context__) = false;
                        _temp_make_local_1195_39_22(47,__context__) = false;
                        _temp_make_local_1195_39_22(48,__context__) = true;
                        _temp_make_local_1195_39_22(49,__context__) = true;
                        _temp_make_local_1195_39_22(50,__context__) = true;
                        _temp_make_local_1195_39_22(51,__context__) = true;
                        _temp_make_local_1195_39_22(52,__context__) = true;
                        _temp_make_local_1195_39_22(53,__context__) = true;
                        _temp_make_local_1195_39_22(54,__context__) = true;
                        _temp_make_local_1195_39_22(55,__context__) = true;
                        _temp_make_local_1195_39_22(56,__context__) = true;
                        _temp_make_local_1195_39_22(57,__context__) = true;
                        _temp_make_local_1195_39_22(58,__context__) = false;
                        _temp_make_local_1195_39_22(59,__context__) = false;
                        _temp_make_local_1195_39_22(60,__context__) = false;
                        _temp_make_local_1195_39_22(61,__context__) = false;
                        _temp_make_local_1195_39_22(62,__context__) = false;
                        _temp_make_local_1195_39_22(63,__context__) = false;
                        _temp_make_local_1195_39_22(64,__context__) = false;
                        _temp_make_local_1195_39_22(65,__context__) = true;
                        _temp_make_local_1195_39_22(66,__context__) = true;
                        _temp_make_local_1195_39_22(67,__context__) = true;
                        _temp_make_local_1195_39_22(68,__context__) = true;
                        _temp_make_local_1195_39_22(69,__context__) = true;
                        _temp_make_local_1195_39_22(70,__context__) = true;
                        _temp_make_local_1195_39_22(71,__context__) = true;
                        _temp_make_local_1195_39_22(72,__context__) = true;
                        _temp_make_local_1195_39_22(73,__context__) = true;
                        _temp_make_local_1195_39_22(74,__context__) = true;
                        _temp_make_local_1195_39_22(75,__context__) = true;
                        _temp_make_local_1195_39_22(76,__context__) = true;
                        _temp_make_local_1195_39_22(77,__context__) = true;
                        _temp_make_local_1195_39_22(78,__context__) = true;
                        _temp_make_local_1195_39_22(79,__context__) = true;
                        _temp_make_local_1195_39_22(80,__context__) = true;
                        _temp_make_local_1195_39_22(81,__context__) = true;
                        _temp_make_local_1195_39_22(82,__context__) = true;
                        _temp_make_local_1195_39_22(83,__context__) = true;
                        _temp_make_local_1195_39_22(84,__context__) = true;
                        _temp_make_local_1195_39_22(85,__context__) = true;
                        _temp_make_local_1195_39_22(86,__context__) = true;
                        _temp_make_local_1195_39_22(87,__context__) = true;
                        _temp_make_local_1195_39_22(88,__context__) = true;
                        _temp_make_local_1195_39_22(89,__context__) = true;
                        _temp_make_local_1195_39_22(90,__context__) = true;
                        _temp_make_local_1195_39_22(91,__context__) = false;
                        _temp_make_local_1195_39_22(92,__context__) = false;
                        _temp_make_local_1195_39_22(93,__context__) = false;
                        _temp_make_local_1195_39_22(94,__context__) = false;
                        _temp_make_local_1195_39_22(95,__context__) = true;
                        _temp_make_local_1195_39_22(96,__context__) = false;
                        _temp_make_local_1195_39_22(97,__context__) = true;
                        _temp_make_local_1195_39_22(98,__context__) = true;
                        _temp_make_local_1195_39_22(99,__context__) = true;
                        _temp_make_local_1195_39_22(100,__context__) = true;
                        _temp_make_local_1195_39_22(101,__context__) = true;
                        _temp_make_local_1195_39_22(102,__context__) = true;
                        _temp_make_local_1195_39_22(103,__context__) = true;
                        _temp_make_local_1195_39_22(104,__context__) = true;
                        _temp_make_local_1195_39_22(105,__context__) = true;
                        _temp_make_local_1195_39_22(106,__context__) = true;
                        _temp_make_local_1195_39_22(107,__context__) = true;
                        _temp_make_local_1195_39_22(108,__context__) = true;
                        _temp_make_local_1195_39_22(109,__context__) = true;
                        _temp_make_local_1195_39_22(110,__context__) = true;
                        _temp_make_local_1195_39_22(111,__context__) = true;
                        _temp_make_local_1195_39_22(112,__context__) = true;
                        _temp_make_local_1195_39_22(113,__context__) = true;
                        _temp_make_local_1195_39_22(114,__context__) = true;
                        _temp_make_local_1195_39_22(115,__context__) = true;
                        _temp_make_local_1195_39_22(116,__context__) = true;
                        _temp_make_local_1195_39_22(117,__context__) = true;
                        _temp_make_local_1195_39_22(118,__context__) = true;
                        _temp_make_local_1195_39_22(119,__context__) = true;
                        _temp_make_local_1195_39_22(120,__context__) = true;
                        _temp_make_local_1195_39_22(121,__context__) = true;
                        _temp_make_local_1195_39_22(122,__context__) = true;
                        _temp_make_local_1195_39_22(123,__context__) = false;
                        _temp_make_local_1195_39_22(124,__context__) = false;
                        _temp_make_local_1195_39_22(125,__context__) = false;
                        _temp_make_local_1195_39_22(126,__context__) = false;
                        _temp_make_local_1195_39_22(127,__context__) = false;
                        _temp_make_local_1195_39_22(128,__context__) = false;
                        _temp_make_local_1195_39_22(129,__context__) = false;
                        _temp_make_local_1195_39_22(130,__context__) = false;
                        _temp_make_local_1195_39_22(131,__context__) = false;
                        _temp_make_local_1195_39_22(132,__context__) = false;
                        _temp_make_local_1195_39_22(133,__context__) = false;
                        _temp_make_local_1195_39_22(134,__context__) = false;
                        _temp_make_local_1195_39_22(135,__context__) = false;
                        _temp_make_local_1195_39_22(136,__context__) = false;
                        _temp_make_local_1195_39_22(137,__context__) = false;
                        _temp_make_local_1195_39_22(138,__context__) = false;
                        _temp_make_local_1195_39_22(139,__context__) = false;
                        _temp_make_local_1195_39_22(140,__context__) = false;
                        _temp_make_local_1195_39_22(141,__context__) = false;
                        _temp_make_local_1195_39_22(142,__context__) = false;
                        _temp_make_local_1195_39_22(143,__context__) = false;
                        _temp_make_local_1195_39_22(144,__context__) = false;
                        _temp_make_local_1195_39_22(145,__context__) = false;
                        _temp_make_local_1195_39_22(146,__context__) = false;
                        _temp_make_local_1195_39_22(147,__context__) = false;
                        _temp_make_local_1195_39_22(148,__context__) = false;
                        _temp_make_local_1195_39_22(149,__context__) = false;
                        _temp_make_local_1195_39_22(150,__context__) = false;
                        _temp_make_local_1195_39_22(151,__context__) = false;
                        _temp_make_local_1195_39_22(152,__context__) = false;
                        _temp_make_local_1195_39_22(153,__context__) = false;
                        _temp_make_local_1195_39_22(154,__context__) = false;
                        _temp_make_local_1195_39_22(155,__context__) = false;
                        _temp_make_local_1195_39_22(156,__context__) = false;
                        _temp_make_local_1195_39_22(157,__context__) = false;
                        _temp_make_local_1195_39_22(158,__context__) = false;
                        _temp_make_local_1195_39_22(159,__context__) = false;
                        _temp_make_local_1195_39_22(160,__context__) = false;
                        _temp_make_local_1195_39_22(161,__context__) = false;
                        _temp_make_local_1195_39_22(162,__context__) = false;
                        _temp_make_local_1195_39_22(163,__context__) = false;
                        _temp_make_local_1195_39_22(164,__context__) = false;
                        _temp_make_local_1195_39_22(165,__context__) = false;
                        _temp_make_local_1195_39_22(166,__context__) = false;
                        _temp_make_local_1195_39_22(167,__context__) = false;
                        _temp_make_local_1195_39_22(168,__context__) = false;
                        _temp_make_local_1195_39_22(169,__context__) = false;
                        _temp_make_local_1195_39_22(170,__context__) = false;
                        _temp_make_local_1195_39_22(171,__context__) = false;
                        _temp_make_local_1195_39_22(172,__context__) = false;
                        _temp_make_local_1195_39_22(173,__context__) = false;
                        _temp_make_local_1195_39_22(174,__context__) = false;
                        _temp_make_local_1195_39_22(175,__context__) = false;
                        _temp_make_local_1195_39_22(176,__context__) = false;
                        _temp_make_local_1195_39_22(177,__context__) = false;
                        _temp_make_local_1195_39_22(178,__context__) = false;
                        _temp_make_local_1195_39_22(179,__context__) = false;
                        _temp_make_local_1195_39_22(180,__context__) = false;
                        _temp_make_local_1195_39_22(181,__context__) = false;
                        _temp_make_local_1195_39_22(182,__context__) = false;
                        _temp_make_local_1195_39_22(183,__context__) = false;
                        _temp_make_local_1195_39_22(184,__context__) = false;
                        _temp_make_local_1195_39_22(185,__context__) = false;
                        _temp_make_local_1195_39_22(186,__context__) = false;
                        _temp_make_local_1195_39_22(187,__context__) = false;
                        _temp_make_local_1195_39_22(188,__context__) = false;
                        _temp_make_local_1195_39_22(189,__context__) = false;
                        _temp_make_local_1195_39_22(190,__context__) = false;
                        _temp_make_local_1195_39_22(191,__context__) = false;
                        _temp_make_local_1195_39_22(192,__context__) = false;
                        _temp_make_local_1195_39_22(193,__context__) = false;
                        _temp_make_local_1195_39_22(194,__context__) = false;
                        _temp_make_local_1195_39_22(195,__context__) = false;
                        _temp_make_local_1195_39_22(196,__context__) = false;
                        _temp_make_local_1195_39_22(197,__context__) = false;
                        _temp_make_local_1195_39_22(198,__context__) = false;
                        _temp_make_local_1195_39_22(199,__context__) = false;
                        _temp_make_local_1195_39_22(200,__context__) = false;
                        _temp_make_local_1195_39_22(201,__context__) = false;
                        _temp_make_local_1195_39_22(202,__context__) = false;
                        _temp_make_local_1195_39_22(203,__context__) = false;
                        _temp_make_local_1195_39_22(204,__context__) = false;
                        _temp_make_local_1195_39_22(205,__context__) = false;
                        _temp_make_local_1195_39_22(206,__context__) = false;
                        _temp_make_local_1195_39_22(207,__context__) = false;
                        _temp_make_local_1195_39_22(208,__context__) = false;
                        _temp_make_local_1195_39_22(209,__context__) = false;
                        _temp_make_local_1195_39_22(210,__context__) = false;
                        _temp_make_local_1195_39_22(211,__context__) = false;
                        _temp_make_local_1195_39_22(212,__context__) = false;
                        _temp_make_local_1195_39_22(213,__context__) = false;
                        _temp_make_local_1195_39_22(214,__context__) = false;
                        _temp_make_local_1195_39_22(215,__context__) = false;
                        _temp_make_local_1195_39_22(216,__context__) = false;
                        _temp_make_local_1195_39_22(217,__context__) = false;
                        _temp_make_local_1195_39_22(218,__context__) = false;
                        _temp_make_local_1195_39_22(219,__context__) = false;
                        _temp_make_local_1195_39_22(220,__context__) = false;
                        _temp_make_local_1195_39_22(221,__context__) = false;
                        _temp_make_local_1195_39_22(222,__context__) = false;
                        _temp_make_local_1195_39_22(223,__context__) = false;
                        _temp_make_local_1195_39_22(224,__context__) = false;
                        _temp_make_local_1195_39_22(225,__context__) = false;
                        _temp_make_local_1195_39_22(226,__context__) = false;
                        _temp_make_local_1195_39_22(227,__context__) = false;
                        _temp_make_local_1195_39_22(228,__context__) = false;
                        _temp_make_local_1195_39_22(229,__context__) = false;
                        _temp_make_local_1195_39_22(230,__context__) = false;
                        _temp_make_local_1195_39_22(231,__context__) = false;
                        _temp_make_local_1195_39_22(232,__context__) = false;
                        _temp_make_local_1195_39_22(233,__context__) = false;
                        _temp_make_local_1195_39_22(234,__context__) = false;
                        _temp_make_local_1195_39_22(235,__context__) = false;
                        _temp_make_local_1195_39_22(236,__context__) = false;
                        _temp_make_local_1195_39_22(237,__context__) = false;
                        _temp_make_local_1195_39_22(238,__context__) = false;
                        _temp_make_local_1195_39_22(239,__context__) = false;
                        _temp_make_local_1195_39_22(240,__context__) = false;
                        _temp_make_local_1195_39_22(241,__context__) = false;
                        _temp_make_local_1195_39_22(242,__context__) = false;
                        _temp_make_local_1195_39_22(243,__context__) = false;
                        _temp_make_local_1195_39_22(244,__context__) = false;
                        _temp_make_local_1195_39_22(245,__context__) = false;
                        _temp_make_local_1195_39_22(246,__context__) = false;
                        _temp_make_local_1195_39_22(247,__context__) = false;
                        _temp_make_local_1195_39_22(248,__context__) = false;
                        _temp_make_local_1195_39_22(249,__context__) = false;
                        _temp_make_local_1195_39_22(250,__context__) = false;
                        _temp_make_local_1195_39_22(251,__context__) = false;
                        _temp_make_local_1195_39_22(252,__context__) = false;
                        _temp_make_local_1195_39_22(253,__context__) = false;
                        _temp_make_local_1195_39_22(254,__context__) = false;
                        _temp_make_local_1195_39_22(255,__context__) = false;
                        return _temp_make_local_1195_39_22;
                    })())(__ch_rename_at_1194_331,__context__));
                });
                if ( !___terminal_result_23_rename_at_1199_330 )
                {
                    if ( (__parser_rename_at_362_318.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_318.suppress_errors)))) && (__parser_rename_at_362_318.index == __parser_rename_at_362_318.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1217_49_23; _temp_make_local_1217_49_23;
                        char * __rest_rename_at_1215_332 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_60, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_318.index))),false));
                        char * __complete_message_rename_at_1216_333 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_61, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '48'-'57'  '65'-'90'  '95' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1215_332))));
                        _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_318.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1217_49_23.text),(__complete_message_rename_at_1216_333));
                            das_copy((_temp_make_local_1217_49_23.index),(__parser_rename_at_362_318.index));
                            return _temp_make_local_1217_49_23;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                        AutoTuple<bool,int32_t,int32_t> __mks_1220;
                        das_zero(__mks_1220);
                        return __mks_1220;
                    })()));
                } else {
                    _FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_318),1);
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                        AutoTuple<bool,int32_t,int32_t> __mkt_637;
                        das_get_auto_tuple_field<bool,0,bool,int32_t,int32_t>::get(__mkt_637) = true;
                        das_get_auto_tuple_field<int32_t,1,bool,int32_t,int32_t>::get(__mkt_637) = 0;
                        das_get_auto_tuple_field<int32_t,2,bool,int32_t,int32_t>::get(__mkt_637) = __parser_rename_at_362_318.index;
                        return __mkt_637;
                    })()));
                };
            }));
            if ( !das_get_auto_tuple_field<bool,0,bool,int32_t,int32_t>::get(__res_rename_at_644_329) )
            {
                break;
            };
        };
        _FuncbuiltinTickpopTick1161079256290593740_e59f84a1c28ac19(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_318.suppress_errors));
        char * __str_3_rename_at_566_334 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_318.input(0,__context__))));
        char * __tail_rename_at_567_335 = (char *)(((char * const )(builtin_string_slice1(__str_3_rename_at_566_334,__ext_pos_3_rename_at_564_328,__parser_rename_at_362_318.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        char * __val_rename_at_1133_336; das_zero(__val_rename_at_1133_336); das_move(__val_rename_at_1133_336, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
            return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__fc_rename_at_567_327),cast<char *>::from(__tail_rename_at_567_335),*__context__,nullptr)));
        })));
        AutoTuple<bool,char *,int32_t> __result_rename_at_1141_337; das_zero(__result_rename_at_1141_337); das_move(__result_rename_at_1141_337, (([&]() -> AutoTuple<bool,char *,int32_t> {
            AutoTuple<bool,char *,int32_t> __mkt_1141;
            das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
            das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_336;
            das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_318.index;
            return __mkt_1141;
        })()));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_337);
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_1_rename_at_1086_320) )
    {
        --__parser_rename_at_362_318.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_1_rename_at_1086_320);
    } else {
        --__parser_rename_at_362_318.tabs;
        das_copy(__parser_rename_at_362_318.index,__parse_pos_rename_at_1076_319);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
        AutoTuple<bool,char *,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_ident_Tickid_0x0_ac07755a3c8869f1 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_338 )
{
    int32_t __mark_rename_at_147_339 = __parser_rename_at_146_338.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_338.ident__cache),__parser_rename_at_146_338.index) && !(__parser_rename_at_146_338.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_340; das_zero(__result_rename_at_155_340); das_move(__result_rename_at_155_340, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_338.ident__cache(__parser_rename_at_146_338.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_340) )
        {
            das_copy(__parser_rename_at_146_338.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_340));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_340);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_341; das_zero(__result_rename_at_169_341); das_move(__result_rename_at_169_341, _Funcparse_ident__innerTickid_0x0_1def850e72a70e16(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_338)));
        das_copy(__parser_rename_at_146_338.ident__cache(__mark_rename_at_147_339,__context__),__result_rename_at_169_341);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_341);
    };
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_76bd142a6cc93fe5 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __it_rename_at_1275_342, int32_t & __value_rename_at_1275_343 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1275_342),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1275_343)),__context__));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_b295659320d5aa3e ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_344, uint8_t __value_rename_at_165_345 )
{
    das_copy(__Arr_rename_at_165_344(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_344),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_345);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_b5e1593e39a49db7 ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_346, char * const  __value_rename_at_165_347 )
{
    das_copy(__Arr_rename_at_165_346(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_346),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_347);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_86ba7065cbab662e ( Context * __context__, TArray<char *> &  __Arr_rename_at_181_348, char * __value_rename_at_181_349 )
{
    das_copy(__Arr_rename_at_181_348(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_181_348),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_349);
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_67ac71029b5249f8 ( Context * __context__, char * const  __str_rename_at_1308_350 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1309_351;das_zero(__it_rename_at_1309_351);
    builtin_make_string_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1309_351),__str_rename_at_1308_350,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1309_351);
}

inline int32_t _FuncbuiltinTickfind_index_ifTick5144745413123287381_36aadb1599a6d4e8 ( Context * __context__, TArray<AutoTuple<char *,char *>> const  &  __arr_rename_at_1728_352, Block DAS_COMMENT((bool,AutoTuple<char *,char *> const )) const  &  __blk_rename_at_1728_353 )
{
    {
        bool __need_loop_1729 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(builtin_array_size(__arr_rename_at_1728_352)));
        int32_t __i_rename_at_1729_354;
        __need_loop_1729 = __i_iterator.first(__context__,(__i_rename_at_1729_354)) && __need_loop_1729;
        for ( ; __need_loop_1729 ; __need_loop_1729 = __i_iterator.next(__context__,(__i_rename_at_1729_354)) )
        {
            if ( das_invoke<bool>::invoke<AutoTuple<char *,char *> const  &>(__context__,nullptr,__blk_rename_at_1728_353,__arr_rename_at_1728_352(__i_rename_at_1729_354,__context__)) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1729_354);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1729_354));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _Funcparse_macro_invocation_innerTickid_0x1_5bd6ef28378dc241 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_355 )
{
    AutoTuple<bool,spoof::SpoofInvocation,int32_t> _temp_make_local_364_18_24; _temp_make_local_364_18_24;
    int32_t __parse_pos_rename_at_1076_356 = __parser_rename_at_362_355.index;
    ++__parser_rename_at_362_355.tabs;
    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __res_1_rename_at_1086_357; das_zero(__res_1_rename_at_1086_357); das_move(__res_1_rename_at_1086_357, das_invoke<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,spoof::SpoofInvocation,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
        AutoTuple<bool,char *,int32_t> __class_name0_rename_at_546_358; das_zero(__class_name0_rename_at_546_358); das_move(__class_name0_rename_at_546_358, _Funcparse_class_nameTickid_0x1_ea9ce584f39ea618(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__class_name0_rename_at_546_358) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __cn_rename_at_551_359; das_zero(__cn_rename_at_551_359); das_move(__cn_rename_at_551_359, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__class_name0_rename_at_546_358)));
            _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
            if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355),((char *) "("),1) )
            {
                if ( (__parser_rename_at_362_355.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_355.suppress_errors)))) && (__parser_rename_at_362_355.index == __parser_rename_at_362_355.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1175_49_25; _temp_make_local_1175_49_25;
                    char * __rest_rename_at_1173_360 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_62, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_355.index))),false));
                    char * __complete_message_rename_at_1174_361 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_63, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_360))));
                    _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_355.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1175_49_25.text),(__complete_message_rename_at_1174_361));
                        das_copy((_temp_make_local_1175_49_25.index),(__parser_rename_at_362_355.index));
                        return _temp_make_local_1175_49_25;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_1179;
                    das_zero(__mks_1179);
                    return __mks_1179;
                })()));
            } else {
                AutoTuple<bool,TArray<char *>,int32_t> __element_list0_rename_at_546_362; das_zero(__element_list0_rename_at_546_362); das_move(__element_list0_rename_at_546_362, _Funcparse_element_listTickid_0x1_7d5844c12f276ee3(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355)));
                if ( !das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__element_list0_rename_at_546_362) )
                {
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_548;
                        das_zero(__mks_548);
                        return __mks_548;
                    })()));
                } else {
                    TArray<char *> __el_rename_at_551_363; das_zero(__el_rename_at_551_363); das_move(__el_rename_at_551_363, das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(__element_list0_rename_at_546_362));
                    _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
                    if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355),((char *) ")"),1) )
                    {
                        if ( (__parser_rename_at_362_355.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_355.suppress_errors)))) && (__parser_rename_at_362_355.index == __parser_rename_at_362_355.longest_prefix) )
                        {
                            peg::ParsingError _temp_make_local_1175_49_26; _temp_make_local_1175_49_26;
                            char * __rest_rename_at_1173_364 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_64, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_355.index))),false));
                            char * __complete_message_rename_at_1174_365 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_65, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_364))));
                            _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_355.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                                das_copy((_temp_make_local_1175_49_26.text),(__complete_message_rename_at_1174_365));
                                das_copy((_temp_make_local_1175_49_26.index),(__parser_rename_at_362_355.index));
                                return _temp_make_local_1175_49_26;
                            })())));
                        };
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                            AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_1179;
                            das_zero(__mks_1179);
                            return __mks_1179;
                        })()));
                    } else {
                        spoof::SpoofInvocation __val_rename_at_1133_366; das_zero(__val_rename_at_1133_366); das_move(__val_rename_at_1133_366, das_invoke<spoof::SpoofInvocation>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> spoof::SpoofInvocation{
                            spoof::SpoofInvocation _temp_make_local_183_22_27; _temp_make_local_183_22_27;
                            return /* <- */ das_auto_cast_move<spoof::SpoofInvocation>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_eca34382a0b7791d(__context__,das_arg<spoof::SpoofInvocation>::pass((([&]() -> spoof::SpoofInvocation& {
                                das_copy((_temp_make_local_183_22_27.varName),(__cn_rename_at_551_359));
                                das_move((_temp_make_local_183_22_27.args),(__el_rename_at_551_363));
                                return _temp_make_local_183_22_27;
                            })()))));
                        }));
                        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_1141_367; das_zero(__result_rename_at_1141_367); das_move(__result_rename_at_1141_367, (([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                            AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mkt_1141;
                            das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1141) = true;
                            das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1141) = __val_rename_at_1133_366;
                            das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1141) = __parser_rename_at_362_355.index;
                            return __mkt_1141;
                        })()));
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__result_rename_at_1141_367);
                    };
                };
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__res_1_rename_at_1086_357) )
    {
        --__parser_rename_at_362_355.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__res_1_rename_at_1086_357);
    } else {
        --__parser_rename_at_362_355.tabs;
        das_copy(__parser_rename_at_362_355.index,__parse_pos_rename_at_1076_356);
    };
    int32_t __parse_pos_rename_at_1076_368 = __parser_rename_at_362_355.index;
    ++__parser_rename_at_362_355.tabs;
    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __res_2_rename_at_1086_369; das_zero(__res_2_rename_at_1086_369); das_move(__res_2_rename_at_1086_369, das_invoke<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,spoof::SpoofInvocation,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
        AutoTuple<bool,char *,int32_t> __class_name1_rename_at_546_370; das_zero(__class_name1_rename_at_546_370); das_move(__class_name1_rename_at_546_370, _Funcparse_class_nameTickid_0x1_ea9ce584f39ea618(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__class_name1_rename_at_546_370) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __cn_rename_at_551_371; das_zero(__cn_rename_at_551_371); das_move(__cn_rename_at_551_371, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__class_name1_rename_at_546_370)));
            _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
            if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355),((char *) "("),1) )
            {
                if ( (__parser_rename_at_362_355.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_355.suppress_errors)))) && (__parser_rename_at_362_355.index == __parser_rename_at_362_355.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1175_49_28; _temp_make_local_1175_49_28;
                    char * __rest_rename_at_1173_372 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_66, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_355.index))),false));
                    char * __complete_message_rename_at_1174_373 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_67, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_372))));
                    _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_355.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1175_49_28.text),(__complete_message_rename_at_1174_373));
                        das_copy((_temp_make_local_1175_49_28.index),(__parser_rename_at_362_355.index));
                        return _temp_make_local_1175_49_28;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_1179;
                    das_zero(__mks_1179);
                    return __mks_1179;
                })()));
            } else {
                _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
                if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355),((char *) ")"),1) )
                {
                    if ( (__parser_rename_at_362_355.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_355.suppress_errors)))) && (__parser_rename_at_362_355.index == __parser_rename_at_362_355.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1175_49_29; _temp_make_local_1175_49_29;
                        char * __rest_rename_at_1173_374 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_68, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_355.index))),false));
                        char * __complete_message_rename_at_1174_375 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_69, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_374))));
                        _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_355.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1175_49_29.text),(__complete_message_rename_at_1174_375));
                            das_copy((_temp_make_local_1175_49_29.index),(__parser_rename_at_362_355.index));
                            return _temp_make_local_1175_49_29;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_1179;
                        das_zero(__mks_1179);
                        return __mks_1179;
                    })()));
                } else {
                    spoof::SpoofInvocation __val_rename_at_1133_376; das_zero(__val_rename_at_1133_376); das_move(__val_rename_at_1133_376, das_invoke<spoof::SpoofInvocation>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> spoof::SpoofInvocation{
                        spoof::SpoofInvocation _temp_make_local_186_22_30; _temp_make_local_186_22_30;
                        return /* <- */ das_auto_cast_move<spoof::SpoofInvocation>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_eca34382a0b7791d(__context__,das_arg<spoof::SpoofInvocation>::pass((([&]() -> spoof::SpoofInvocation& {
                            das_zero(_temp_make_local_186_22_30);
                            das_copy((_temp_make_local_186_22_30.varName),(__cn_rename_at_551_371));
                            return _temp_make_local_186_22_30;
                        })()))));
                    }));
                    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_1141_377; das_zero(__result_rename_at_1141_377); das_move(__result_rename_at_1141_377, (([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mkt_1141;
                        das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1141) = true;
                        das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1141) = __val_rename_at_1133_376;
                        das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1141) = __parser_rename_at_362_355.index;
                        return __mkt_1141;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__result_rename_at_1141_377);
                };
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__res_2_rename_at_1086_369) )
    {
        --__parser_rename_at_362_355.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__res_2_rename_at_1086_369);
    } else {
        --__parser_rename_at_362_355.tabs;
        das_copy(__parser_rename_at_362_355.index,__parse_pos_rename_at_1076_368);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1516aad463652b16(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t>& {
        das_zero(_temp_make_local_364_18_24);
        return _temp_make_local_364_18_24;
    })()))));
}

inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _Funcparse_macro_invocationTickid_0x1_b47e0c17f58865ea ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_378 )
{
    int32_t __mark_rename_at_147_379 = __parser_rename_at_146_378.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_18bc2a30ed120a5(__context__,das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(__parser_rename_at_146_378.macro_invocation_cache),__parser_rename_at_146_378.index) && !(__parser_rename_at_146_378.error_reporting) )
    {
        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_155_380; das_zero(__result_rename_at_155_380); das_move(__result_rename_at_155_380, _FuncbuiltinTickclone_to_moveTick2007252383599261567_d766db4c7472c995(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__parser_rename_at_146_378.macro_invocation_cache(__parser_rename_at_146_378.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_155_380) )
        {
            das_copy(__parser_rename_at_146_378.index,das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_155_380));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__result_rename_at_155_380);
    } else {
        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_169_381; das_zero(__result_rename_at_169_381); das_move(__result_rename_at_169_381, _Funcparse_macro_invocation_innerTickid_0x1_5bd6ef28378dc241(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_378)));
        clone_fd9d2e92ebaff162(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__parser_rename_at_146_378.macro_invocation_cache(__mark_rename_at_147_379,__context__)),das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__result_rename_at_169_381));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__result_rename_at_169_381);
    };
}

inline AutoTuple<bool,TArray<char *>,int32_t> _Funcparse_element_list_innerTickid_0x1_ee8dc9dce2c72dd4 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_382 )
{
    AutoTuple<bool,TArray<char *>,int32_t> _temp_make_local_364_18_31; _temp_make_local_364_18_31;
    int32_t __parse_pos_rename_at_1076_383 = __parser_rename_at_362_382.index;
    ++__parser_rename_at_362_382.tabs;
    AutoTuple<bool,TArray<char *>,int32_t> __res_1_rename_at_1086_384; das_zero(__res_1_rename_at_1086_384); das_move(__res_1_rename_at_1086_384, das_invoke<AutoTuple<bool,TArray<char *>,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,TArray<char *>,int32_t>{
        TArray<char *> __els_rename_at_671_385;das_zero(__els_rename_at_671_385);
        _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_382.suppress_errors),true);
        while ( true )
        {
            int32_t __sz_rename_at_674_386 = builtin_array_size(das_arg<TArray<char *>>::pass(__els_rename_at_671_385));
            das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                AutoTuple<bool,char *,int32_t> __comma_separated_elements0_rename_at_546_387; das_zero(__comma_separated_elements0_rename_at_546_387); das_move(__comma_separated_elements0_rename_at_546_387, _Funcparse_comma_separated_elementsTickid_0x1_72b43df88b54209c(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_382)));
                if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__comma_separated_elements0_rename_at_546_387) )
                {
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                        AutoTuple<bool,int32_t,int32_t> __mks_548;
                        das_zero(__mks_548);
                        return __mks_548;
                    })()));
                } else {
                    char * __temp_name_rename_at_551_388; das_zero(__temp_name_rename_at_551_388); das_move(__temp_name_rename_at_551_388, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__comma_separated_elements0_rename_at_546_387)));
                    _FuncbuiltinTickemplaceTick13923705686753630697_2d94a9a555d8bee7(__context__,das_arg<TArray<char *>>::pass(__els_rename_at_671_385),__temp_name_rename_at_551_388);
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                        AutoTuple<bool,int32_t,int32_t> __mkt_666;
                        das_get_auto_tuple_field<bool,0,bool,int32_t,int32_t>::get(__mkt_666) = true;
                        das_get_auto_tuple_field<int32_t,1,bool,int32_t,int32_t>::get(__mkt_666) = 0;
                        das_get_auto_tuple_field<int32_t,2,bool,int32_t,int32_t>::get(__mkt_666) = 0;
                        return __mkt_666;
                    })()));
                };
            });
            if ( __sz_rename_at_674_386 == builtin_array_size(das_arg<TArray<char *>>::pass(__els_rename_at_671_385)) )
            {
                break;
            };
        };
        _FuncbuiltinTickpopTick1161079256290593740_e59f84a1c28ac19(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_382.suppress_errors));
        _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_382));
        AutoTuple<bool,char *,int32_t> __element0_rename_at_546_389; das_zero(__element0_rename_at_546_389); das_move(__element0_rename_at_546_389, _Funcparse_elementTickid_0x1_a4daaa8f79cfa9ed(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_382)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__element0_rename_at_546_389) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,TArray<char *>,int32_t> {
                AutoTuple<bool,TArray<char *>,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __last_rename_at_551_390; das_zero(__last_rename_at_551_390); das_move(__last_rename_at_551_390, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__element0_rename_at_546_389)));
            TArray<char *> __val_rename_at_1133_391; das_zero(__val_rename_at_1133_391); das_move(__val_rename_at_1133_391, das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
                _FuncbuiltinTickpushTick10769833213962245646_86ba7065cbab662e(__context__,das_arg<TArray<char *>>::pass(__els_rename_at_671_385),__last_rename_at_551_390);
                return /* <- */ das_auto_cast_move<TArray<char *>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_70ca5f66113685e1(__context__,das_arg<TArray<char *>>::pass(__els_rename_at_671_385)));
            }));
            AutoTuple<bool,TArray<char *>,int32_t> __result_rename_at_1141_392; das_zero(__result_rename_at_1141_392); das_move(__result_rename_at_1141_392, (([&]() -> AutoTuple<bool,TArray<char *>,int32_t> {
                AutoTuple<bool,TArray<char *>,int32_t> __mkt_1141;
                das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__mkt_1141) = true;
                das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(__mkt_1141) = __val_rename_at_1133_391;
                das_get_auto_tuple_field<int32_t,2,bool,TArray<char *>,int32_t>::get(__mkt_1141) = __parser_rename_at_362_382.index;
                return __mkt_1141;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(__result_rename_at_1141_392);
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__res_1_rename_at_1086_384) )
    {
        --__parser_rename_at_362_382.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(__res_1_rename_at_1086_384);
    } else {
        --__parser_rename_at_362_382.tabs;
        das_copy(__parser_rename_at_362_382.index,__parse_pos_rename_at_1076_383);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_59843c375840fe45(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass((([&]() -> AutoTuple<bool,TArray<char *>,int32_t>& {
        das_zero(_temp_make_local_364_18_31);
        return _temp_make_local_364_18_31;
    })()))));
}

inline AutoTuple<bool,TArray<char *>,int32_t> _Funcparse_element_listTickid_0x1_7d5844c12f276ee3 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_393 )
{
    int32_t __mark_rename_at_147_394 = __parser_rename_at_146_393.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_c6163711697a67ea(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(__parser_rename_at_146_393.element_list_cache),__parser_rename_at_146_393.index) && !(__parser_rename_at_146_393.error_reporting) )
    {
        AutoTuple<bool,TArray<char *>,int32_t> __result_rename_at_155_395; das_zero(__result_rename_at_155_395); das_move(__result_rename_at_155_395, _FuncbuiltinTickclone_to_moveTick2007252383599261567_85d66a6acd78b35(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass(__parser_rename_at_146_393.element_list_cache(__parser_rename_at_146_393.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__result_rename_at_155_395) )
        {
            das_copy(__parser_rename_at_146_393.index,das_get_auto_tuple_field<int32_t,2,bool,TArray<char *>,int32_t>::get(__result_rename_at_155_395));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(__result_rename_at_155_395);
    } else {
        AutoTuple<bool,TArray<char *>,int32_t> __result_rename_at_169_396; das_zero(__result_rename_at_169_396); das_move(__result_rename_at_169_396, _Funcparse_element_list_innerTickid_0x1_ee8dc9dce2c72dd4(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_393)));
        clone_7eaba744dd27b4f(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass(__parser_rename_at_146_393.element_list_cache(__mark_rename_at_147_394,__context__)),das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass(__result_rename_at_169_396));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(__result_rename_at_169_396);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_comma_separated_elements_innerTickid_0x1_35f6d9d44c93b4da ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_397 )
{
    int32_t __parse_pos_rename_at_1076_398 = __parser_rename_at_362_397.index;
    ++__parser_rename_at_362_397.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_399; das_zero(__res_1_rename_at_1086_399); das_move(__res_1_rename_at_1086_399, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_397));
        AutoTuple<bool,char *,int32_t> __element1_rename_at_546_400; das_zero(__element1_rename_at_546_400); das_move(__element1_rename_at_546_400, _Funcparse_elementTickid_0x1_a4daaa8f79cfa9ed(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_397)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__element1_rename_at_546_400) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __e_rename_at_551_401; das_zero(__e_rename_at_551_401); das_move(__e_rename_at_551_401, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__element1_rename_at_546_400)));
            if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_397),((char *) ","),1) )
            {
                if ( (__parser_rename_at_362_397.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_397.suppress_errors)))) && (__parser_rename_at_362_397.index == __parser_rename_at_362_397.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1175_49_32; _temp_make_local_1175_49_32;
                    char * __rest_rename_at_1173_402 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_70, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_397.index))),false));
                    char * __complete_message_rename_at_1174_403 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_71, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m,\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_402))));
                    _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_397.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1175_49_32.text),(__complete_message_rename_at_1174_403));
                        das_copy((_temp_make_local_1175_49_32.index),(__parser_rename_at_362_397.index));
                        return _temp_make_local_1175_49_32;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1179;
                    das_zero(__mks_1179);
                    return __mks_1179;
                })()));
            } else {
                char * __val_rename_at_1133_404; das_zero(__val_rename_at_1133_404); das_move(__val_rename_at_1133_404, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                    return das_auto_cast<char *>::cast(__e_rename_at_551_401);
                })));
                AutoTuple<bool,char *,int32_t> __result_rename_at_1141_405; das_zero(__result_rename_at_1141_405); das_move(__result_rename_at_1141_405, (([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mkt_1141;
                    das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                    das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_404;
                    das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_397.index;
                    return __mkt_1141;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_405);
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_1_rename_at_1086_399) )
    {
        --__parser_rename_at_362_397.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_1_rename_at_1086_399);
    } else {
        --__parser_rename_at_362_397.tabs;
        das_copy(__parser_rename_at_362_397.index,__parse_pos_rename_at_1076_398);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
        AutoTuple<bool,char *,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_comma_separated_elementsTickid_0x1_72b43df88b54209c ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_406 )
{
    int32_t __mark_rename_at_147_407 = __parser_rename_at_146_406.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_406.comma_separated_elements_cache),__parser_rename_at_146_406.index) && !(__parser_rename_at_146_406.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_408; das_zero(__result_rename_at_155_408); das_move(__result_rename_at_155_408, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_406.comma_separated_elements_cache(__parser_rename_at_146_406.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_408) )
        {
            das_copy(__parser_rename_at_146_406.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_408));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_408);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_409; das_zero(__result_rename_at_169_409); das_move(__result_rename_at_169_409, _Funcparse_comma_separated_elements_innerTickid_0x1_35f6d9d44c93b4da(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_406)));
        das_copy(__parser_rename_at_146_406.comma_separated_elements_cache(__mark_rename_at_147_407,__context__),__result_rename_at_169_409);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_409);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_element_innerTickid_0x1_3ffc02542601bf64 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_410 )
{
    int32_t __parse_pos_rename_at_1076_411 = __parser_rename_at_362_410.index;
    ++__parser_rename_at_362_410.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_412; das_zero(__res_1_rename_at_1086_412); das_move(__res_1_rename_at_1086_412, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),((char *) "\\"),1) )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1175_49_33; _temp_make_local_1175_49_33;
                char * __rest_rename_at_1173_413 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_72, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                char * __complete_message_rename_at_1174_414 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_73, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m\\\\\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_413))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1175_49_33.text),(__complete_message_rename_at_1174_414));
                    das_copy((_temp_make_local_1175_49_33.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1175_49_33;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1179;
                das_zero(__mks_1179);
                return __mks_1179;
            })()));
        } else {
            int32_t __ext_pos_0_rename_at_564_415 = __parser_rename_at_362_410.index;
            bool ___terminal_result_14_rename_at_1199_416 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
                TDim<bool,256> _temp_make_local_1195_39_34; _temp_make_local_1195_39_34;
                int32_t __ch_rename_at_1194_417 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
                return das_auto_cast<bool>::cast((__ch_rename_at_1194_417 != -1) && (([&]() -> TDim<bool,256>& {
                    _temp_make_local_1195_39_34(0,__context__) = true;
                    _temp_make_local_1195_39_34(1,__context__) = true;
                    _temp_make_local_1195_39_34(2,__context__) = true;
                    _temp_make_local_1195_39_34(3,__context__) = true;
                    _temp_make_local_1195_39_34(4,__context__) = true;
                    _temp_make_local_1195_39_34(5,__context__) = true;
                    _temp_make_local_1195_39_34(6,__context__) = true;
                    _temp_make_local_1195_39_34(7,__context__) = true;
                    _temp_make_local_1195_39_34(8,__context__) = true;
                    _temp_make_local_1195_39_34(9,__context__) = true;
                    _temp_make_local_1195_39_34(10,__context__) = true;
                    _temp_make_local_1195_39_34(11,__context__) = true;
                    _temp_make_local_1195_39_34(12,__context__) = true;
                    _temp_make_local_1195_39_34(13,__context__) = true;
                    _temp_make_local_1195_39_34(14,__context__) = true;
                    _temp_make_local_1195_39_34(15,__context__) = true;
                    _temp_make_local_1195_39_34(16,__context__) = true;
                    _temp_make_local_1195_39_34(17,__context__) = true;
                    _temp_make_local_1195_39_34(18,__context__) = true;
                    _temp_make_local_1195_39_34(19,__context__) = true;
                    _temp_make_local_1195_39_34(20,__context__) = true;
                    _temp_make_local_1195_39_34(21,__context__) = true;
                    _temp_make_local_1195_39_34(22,__context__) = true;
                    _temp_make_local_1195_39_34(23,__context__) = true;
                    _temp_make_local_1195_39_34(24,__context__) = true;
                    _temp_make_local_1195_39_34(25,__context__) = true;
                    _temp_make_local_1195_39_34(26,__context__) = true;
                    _temp_make_local_1195_39_34(27,__context__) = true;
                    _temp_make_local_1195_39_34(28,__context__) = true;
                    _temp_make_local_1195_39_34(29,__context__) = true;
                    _temp_make_local_1195_39_34(30,__context__) = true;
                    _temp_make_local_1195_39_34(31,__context__) = true;
                    _temp_make_local_1195_39_34(32,__context__) = true;
                    _temp_make_local_1195_39_34(33,__context__) = true;
                    _temp_make_local_1195_39_34(34,__context__) = true;
                    _temp_make_local_1195_39_34(35,__context__) = true;
                    _temp_make_local_1195_39_34(36,__context__) = true;
                    _temp_make_local_1195_39_34(37,__context__) = true;
                    _temp_make_local_1195_39_34(38,__context__) = true;
                    _temp_make_local_1195_39_34(39,__context__) = true;
                    _temp_make_local_1195_39_34(40,__context__) = true;
                    _temp_make_local_1195_39_34(41,__context__) = true;
                    _temp_make_local_1195_39_34(42,__context__) = true;
                    _temp_make_local_1195_39_34(43,__context__) = true;
                    _temp_make_local_1195_39_34(44,__context__) = true;
                    _temp_make_local_1195_39_34(45,__context__) = true;
                    _temp_make_local_1195_39_34(46,__context__) = true;
                    _temp_make_local_1195_39_34(47,__context__) = true;
                    _temp_make_local_1195_39_34(48,__context__) = true;
                    _temp_make_local_1195_39_34(49,__context__) = true;
                    _temp_make_local_1195_39_34(50,__context__) = true;
                    _temp_make_local_1195_39_34(51,__context__) = true;
                    _temp_make_local_1195_39_34(52,__context__) = true;
                    _temp_make_local_1195_39_34(53,__context__) = true;
                    _temp_make_local_1195_39_34(54,__context__) = true;
                    _temp_make_local_1195_39_34(55,__context__) = true;
                    _temp_make_local_1195_39_34(56,__context__) = true;
                    _temp_make_local_1195_39_34(57,__context__) = true;
                    _temp_make_local_1195_39_34(58,__context__) = true;
                    _temp_make_local_1195_39_34(59,__context__) = true;
                    _temp_make_local_1195_39_34(60,__context__) = true;
                    _temp_make_local_1195_39_34(61,__context__) = true;
                    _temp_make_local_1195_39_34(62,__context__) = true;
                    _temp_make_local_1195_39_34(63,__context__) = true;
                    _temp_make_local_1195_39_34(64,__context__) = true;
                    _temp_make_local_1195_39_34(65,__context__) = true;
                    _temp_make_local_1195_39_34(66,__context__) = true;
                    _temp_make_local_1195_39_34(67,__context__) = true;
                    _temp_make_local_1195_39_34(68,__context__) = true;
                    _temp_make_local_1195_39_34(69,__context__) = true;
                    _temp_make_local_1195_39_34(70,__context__) = true;
                    _temp_make_local_1195_39_34(71,__context__) = true;
                    _temp_make_local_1195_39_34(72,__context__) = true;
                    _temp_make_local_1195_39_34(73,__context__) = true;
                    _temp_make_local_1195_39_34(74,__context__) = true;
                    _temp_make_local_1195_39_34(75,__context__) = true;
                    _temp_make_local_1195_39_34(76,__context__) = true;
                    _temp_make_local_1195_39_34(77,__context__) = true;
                    _temp_make_local_1195_39_34(78,__context__) = true;
                    _temp_make_local_1195_39_34(79,__context__) = true;
                    _temp_make_local_1195_39_34(80,__context__) = true;
                    _temp_make_local_1195_39_34(81,__context__) = true;
                    _temp_make_local_1195_39_34(82,__context__) = true;
                    _temp_make_local_1195_39_34(83,__context__) = true;
                    _temp_make_local_1195_39_34(84,__context__) = true;
                    _temp_make_local_1195_39_34(85,__context__) = true;
                    _temp_make_local_1195_39_34(86,__context__) = true;
                    _temp_make_local_1195_39_34(87,__context__) = true;
                    _temp_make_local_1195_39_34(88,__context__) = true;
                    _temp_make_local_1195_39_34(89,__context__) = true;
                    _temp_make_local_1195_39_34(90,__context__) = true;
                    _temp_make_local_1195_39_34(91,__context__) = true;
                    _temp_make_local_1195_39_34(92,__context__) = true;
                    _temp_make_local_1195_39_34(93,__context__) = true;
                    _temp_make_local_1195_39_34(94,__context__) = true;
                    _temp_make_local_1195_39_34(95,__context__) = true;
                    _temp_make_local_1195_39_34(96,__context__) = true;
                    _temp_make_local_1195_39_34(97,__context__) = true;
                    _temp_make_local_1195_39_34(98,__context__) = true;
                    _temp_make_local_1195_39_34(99,__context__) = true;
                    _temp_make_local_1195_39_34(100,__context__) = true;
                    _temp_make_local_1195_39_34(101,__context__) = true;
                    _temp_make_local_1195_39_34(102,__context__) = true;
                    _temp_make_local_1195_39_34(103,__context__) = true;
                    _temp_make_local_1195_39_34(104,__context__) = true;
                    _temp_make_local_1195_39_34(105,__context__) = true;
                    _temp_make_local_1195_39_34(106,__context__) = true;
                    _temp_make_local_1195_39_34(107,__context__) = true;
                    _temp_make_local_1195_39_34(108,__context__) = true;
                    _temp_make_local_1195_39_34(109,__context__) = true;
                    _temp_make_local_1195_39_34(110,__context__) = true;
                    _temp_make_local_1195_39_34(111,__context__) = true;
                    _temp_make_local_1195_39_34(112,__context__) = true;
                    _temp_make_local_1195_39_34(113,__context__) = true;
                    _temp_make_local_1195_39_34(114,__context__) = true;
                    _temp_make_local_1195_39_34(115,__context__) = true;
                    _temp_make_local_1195_39_34(116,__context__) = true;
                    _temp_make_local_1195_39_34(117,__context__) = true;
                    _temp_make_local_1195_39_34(118,__context__) = true;
                    _temp_make_local_1195_39_34(119,__context__) = true;
                    _temp_make_local_1195_39_34(120,__context__) = true;
                    _temp_make_local_1195_39_34(121,__context__) = true;
                    _temp_make_local_1195_39_34(122,__context__) = true;
                    _temp_make_local_1195_39_34(123,__context__) = true;
                    _temp_make_local_1195_39_34(124,__context__) = true;
                    _temp_make_local_1195_39_34(125,__context__) = true;
                    _temp_make_local_1195_39_34(126,__context__) = true;
                    _temp_make_local_1195_39_34(127,__context__) = true;
                    _temp_make_local_1195_39_34(128,__context__) = true;
                    _temp_make_local_1195_39_34(129,__context__) = true;
                    _temp_make_local_1195_39_34(130,__context__) = true;
                    _temp_make_local_1195_39_34(131,__context__) = true;
                    _temp_make_local_1195_39_34(132,__context__) = true;
                    _temp_make_local_1195_39_34(133,__context__) = true;
                    _temp_make_local_1195_39_34(134,__context__) = true;
                    _temp_make_local_1195_39_34(135,__context__) = true;
                    _temp_make_local_1195_39_34(136,__context__) = true;
                    _temp_make_local_1195_39_34(137,__context__) = true;
                    _temp_make_local_1195_39_34(138,__context__) = true;
                    _temp_make_local_1195_39_34(139,__context__) = true;
                    _temp_make_local_1195_39_34(140,__context__) = true;
                    _temp_make_local_1195_39_34(141,__context__) = true;
                    _temp_make_local_1195_39_34(142,__context__) = true;
                    _temp_make_local_1195_39_34(143,__context__) = true;
                    _temp_make_local_1195_39_34(144,__context__) = true;
                    _temp_make_local_1195_39_34(145,__context__) = true;
                    _temp_make_local_1195_39_34(146,__context__) = true;
                    _temp_make_local_1195_39_34(147,__context__) = true;
                    _temp_make_local_1195_39_34(148,__context__) = true;
                    _temp_make_local_1195_39_34(149,__context__) = true;
                    _temp_make_local_1195_39_34(150,__context__) = true;
                    _temp_make_local_1195_39_34(151,__context__) = true;
                    _temp_make_local_1195_39_34(152,__context__) = true;
                    _temp_make_local_1195_39_34(153,__context__) = true;
                    _temp_make_local_1195_39_34(154,__context__) = true;
                    _temp_make_local_1195_39_34(155,__context__) = true;
                    _temp_make_local_1195_39_34(156,__context__) = true;
                    _temp_make_local_1195_39_34(157,__context__) = true;
                    _temp_make_local_1195_39_34(158,__context__) = true;
                    _temp_make_local_1195_39_34(159,__context__) = true;
                    _temp_make_local_1195_39_34(160,__context__) = true;
                    _temp_make_local_1195_39_34(161,__context__) = true;
                    _temp_make_local_1195_39_34(162,__context__) = true;
                    _temp_make_local_1195_39_34(163,__context__) = true;
                    _temp_make_local_1195_39_34(164,__context__) = true;
                    _temp_make_local_1195_39_34(165,__context__) = true;
                    _temp_make_local_1195_39_34(166,__context__) = true;
                    _temp_make_local_1195_39_34(167,__context__) = true;
                    _temp_make_local_1195_39_34(168,__context__) = true;
                    _temp_make_local_1195_39_34(169,__context__) = true;
                    _temp_make_local_1195_39_34(170,__context__) = true;
                    _temp_make_local_1195_39_34(171,__context__) = true;
                    _temp_make_local_1195_39_34(172,__context__) = true;
                    _temp_make_local_1195_39_34(173,__context__) = true;
                    _temp_make_local_1195_39_34(174,__context__) = true;
                    _temp_make_local_1195_39_34(175,__context__) = true;
                    _temp_make_local_1195_39_34(176,__context__) = true;
                    _temp_make_local_1195_39_34(177,__context__) = true;
                    _temp_make_local_1195_39_34(178,__context__) = true;
                    _temp_make_local_1195_39_34(179,__context__) = true;
                    _temp_make_local_1195_39_34(180,__context__) = true;
                    _temp_make_local_1195_39_34(181,__context__) = true;
                    _temp_make_local_1195_39_34(182,__context__) = true;
                    _temp_make_local_1195_39_34(183,__context__) = true;
                    _temp_make_local_1195_39_34(184,__context__) = true;
                    _temp_make_local_1195_39_34(185,__context__) = true;
                    _temp_make_local_1195_39_34(186,__context__) = true;
                    _temp_make_local_1195_39_34(187,__context__) = true;
                    _temp_make_local_1195_39_34(188,__context__) = true;
                    _temp_make_local_1195_39_34(189,__context__) = true;
                    _temp_make_local_1195_39_34(190,__context__) = true;
                    _temp_make_local_1195_39_34(191,__context__) = true;
                    _temp_make_local_1195_39_34(192,__context__) = true;
                    _temp_make_local_1195_39_34(193,__context__) = true;
                    _temp_make_local_1195_39_34(194,__context__) = true;
                    _temp_make_local_1195_39_34(195,__context__) = true;
                    _temp_make_local_1195_39_34(196,__context__) = true;
                    _temp_make_local_1195_39_34(197,__context__) = true;
                    _temp_make_local_1195_39_34(198,__context__) = true;
                    _temp_make_local_1195_39_34(199,__context__) = true;
                    _temp_make_local_1195_39_34(200,__context__) = true;
                    _temp_make_local_1195_39_34(201,__context__) = true;
                    _temp_make_local_1195_39_34(202,__context__) = true;
                    _temp_make_local_1195_39_34(203,__context__) = true;
                    _temp_make_local_1195_39_34(204,__context__) = true;
                    _temp_make_local_1195_39_34(205,__context__) = true;
                    _temp_make_local_1195_39_34(206,__context__) = true;
                    _temp_make_local_1195_39_34(207,__context__) = true;
                    _temp_make_local_1195_39_34(208,__context__) = true;
                    _temp_make_local_1195_39_34(209,__context__) = true;
                    _temp_make_local_1195_39_34(210,__context__) = true;
                    _temp_make_local_1195_39_34(211,__context__) = true;
                    _temp_make_local_1195_39_34(212,__context__) = true;
                    _temp_make_local_1195_39_34(213,__context__) = true;
                    _temp_make_local_1195_39_34(214,__context__) = true;
                    _temp_make_local_1195_39_34(215,__context__) = true;
                    _temp_make_local_1195_39_34(216,__context__) = true;
                    _temp_make_local_1195_39_34(217,__context__) = true;
                    _temp_make_local_1195_39_34(218,__context__) = true;
                    _temp_make_local_1195_39_34(219,__context__) = true;
                    _temp_make_local_1195_39_34(220,__context__) = true;
                    _temp_make_local_1195_39_34(221,__context__) = true;
                    _temp_make_local_1195_39_34(222,__context__) = true;
                    _temp_make_local_1195_39_34(223,__context__) = true;
                    _temp_make_local_1195_39_34(224,__context__) = true;
                    _temp_make_local_1195_39_34(225,__context__) = true;
                    _temp_make_local_1195_39_34(226,__context__) = true;
                    _temp_make_local_1195_39_34(227,__context__) = true;
                    _temp_make_local_1195_39_34(228,__context__) = true;
                    _temp_make_local_1195_39_34(229,__context__) = true;
                    _temp_make_local_1195_39_34(230,__context__) = true;
                    _temp_make_local_1195_39_34(231,__context__) = true;
                    _temp_make_local_1195_39_34(232,__context__) = true;
                    _temp_make_local_1195_39_34(233,__context__) = true;
                    _temp_make_local_1195_39_34(234,__context__) = true;
                    _temp_make_local_1195_39_34(235,__context__) = true;
                    _temp_make_local_1195_39_34(236,__context__) = true;
                    _temp_make_local_1195_39_34(237,__context__) = true;
                    _temp_make_local_1195_39_34(238,__context__) = true;
                    _temp_make_local_1195_39_34(239,__context__) = true;
                    _temp_make_local_1195_39_34(240,__context__) = true;
                    _temp_make_local_1195_39_34(241,__context__) = true;
                    _temp_make_local_1195_39_34(242,__context__) = true;
                    _temp_make_local_1195_39_34(243,__context__) = true;
                    _temp_make_local_1195_39_34(244,__context__) = true;
                    _temp_make_local_1195_39_34(245,__context__) = true;
                    _temp_make_local_1195_39_34(246,__context__) = true;
                    _temp_make_local_1195_39_34(247,__context__) = true;
                    _temp_make_local_1195_39_34(248,__context__) = true;
                    _temp_make_local_1195_39_34(249,__context__) = true;
                    _temp_make_local_1195_39_34(250,__context__) = true;
                    _temp_make_local_1195_39_34(251,__context__) = true;
                    _temp_make_local_1195_39_34(252,__context__) = true;
                    _temp_make_local_1195_39_34(253,__context__) = true;
                    _temp_make_local_1195_39_34(254,__context__) = true;
                    _temp_make_local_1195_39_34(255,__context__) = true;
                    return _temp_make_local_1195_39_34;
                })())(__ch_rename_at_1194_417,__context__));
            });
            if ( !___terminal_result_14_rename_at_1199_416 )
            {
                if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1217_49_35; _temp_make_local_1217_49_35;
                    char * __rest_rename_at_1215_418 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_74, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                    char * __complete_message_rename_at_1216_419 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_75, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '0'-'256' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1215_418))));
                    _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1217_49_35.text),(__complete_message_rename_at_1216_419));
                        das_copy((_temp_make_local_1217_49_35.index),(__parser_rename_at_362_410.index));
                        return _temp_make_local_1217_49_35;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1220;
                    das_zero(__mks_1220);
                    return __mks_1220;
                })()));
            } else {
                _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),1);
            };
            char * __str_0_rename_at_566_420 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_410.input(0,__context__))));
            char * __Ch_rename_at_567_421 = (char *)(((char * const )(builtin_string_slice1(__str_0_rename_at_566_420,__ext_pos_0_rename_at_564_415,__parser_rename_at_362_410.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            char * __val_rename_at_1133_422; das_zero(__val_rename_at_1133_422); das_move(__val_rename_at_1133_422, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(((char * const )(pass_string(__Ch_rename_at_567_421))));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1141_423; das_zero(__result_rename_at_1141_423); das_move(__result_rename_at_1141_423, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1141;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_422;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_410.index;
                return __mkt_1141;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_423);
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_1_rename_at_1086_412) )
    {
        --__parser_rename_at_362_410.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_1_rename_at_1086_412);
    } else {
        --__parser_rename_at_362_410.tabs;
        das_copy(__parser_rename_at_362_410.index,__parse_pos_rename_at_1076_411);
    };
    int32_t __parse_pos_rename_at_1076_424 = __parser_rename_at_362_410.index;
    ++__parser_rename_at_362_410.tabs;
    bool __cut_rename_at_1084_425 = false;
    AutoTuple<bool,char *,int32_t> __res_2_rename_at_1086_426; das_zero(__res_2_rename_at_1086_426); das_move(__res_2_rename_at_1086_426, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),((char *) "#"),1) )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1175_49_36; _temp_make_local_1175_49_36;
                char * __rest_rename_at_1173_427 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_76, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                char * __complete_message_rename_at_1174_428 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_77, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m#\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_427))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1175_49_36.text),(__complete_message_rename_at_1174_428));
                    das_copy((_temp_make_local_1175_49_36.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1175_49_36;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1179;
                das_zero(__mks_1179);
                return __mks_1179;
            })()));
        } else {
            AutoTuple<bool,char *> ___terminal_result_16_rename_at_1353_429; das_zero(___terminal_result_16_rename_at_1353_429); das_move(___terminal_result_16_rename_at_1353_429, _FuncpegTickmatch_string_literalTick12800953725978677998_5536b27e9ac361f3(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
            if ( !das_get_auto_tuple_field<bool,0,bool,char *>::get(___terminal_result_16_rename_at_1353_429) )
            {
                if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1362_49_37; _temp_make_local_1362_49_37;
                    char * __complete_message_rename_at_1361_430 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_78, cast<char * const >::from(((char *) "Error: Expected a string at ")), cast<int32_t>::from(__parser_rename_at_362_410.index)))));
                    _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1362_49_37.text),(__complete_message_rename_at_1361_430));
                        das_copy((_temp_make_local_1362_49_37.index),(__parser_rename_at_362_410.index));
                        return _temp_make_local_1362_49_37;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1365;
                    das_zero(__mks_1365);
                    return __mks_1365;
                })()));
            } else {
                char * __e_rename_at_1372_431 = (char *)(das_get_auto_tuple_field<char *,1,bool,char *>::get(___terminal_result_16_rename_at_1353_429));
                das_copy(__cut_rename_at_1084_425,true);
                char * __val_rename_at_1133_432; das_zero(__val_rename_at_1133_432); das_move(__val_rename_at_1133_432, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                    return das_auto_cast<char *>::cast(__e_rename_at_1372_431);
                })));
                AutoTuple<bool,char *,int32_t> __result_rename_at_1141_433; das_zero(__result_rename_at_1141_433); das_move(__result_rename_at_1141_433, (([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mkt_1141;
                    das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                    das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_432;
                    das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_410.index;
                    return __mkt_1141;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_433);
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_2_rename_at_1086_426) )
    {
        --__parser_rename_at_362_410.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_2_rename_at_1086_426);
    } else {
        --__parser_rename_at_362_410.tabs;
        das_copy(__parser_rename_at_362_410.index,__parse_pos_rename_at_1076_424);
        if ( __cut_rename_at_1084_425 )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1110;
                das_zero(__mks_1110);
                return __mks_1110;
            })()));
        };
    };
    int32_t __parse_pos_rename_at_1076_434 = __parser_rename_at_362_410.index;
    ++__parser_rename_at_362_410.tabs;
    bool __cut_rename_at_1084_435 = false;
    AutoTuple<bool,char *,int32_t> __res_3_rename_at_1086_436; das_zero(__res_3_rename_at_1086_436); das_move(__res_3_rename_at_1086_436, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        AutoTuple<bool,char *> ___terminal_result_18_rename_at_1353_437; das_zero(___terminal_result_18_rename_at_1353_437); das_move(___terminal_result_18_rename_at_1353_437, _FuncpegTickmatch_string_literalTick12800953725978677998_5536b27e9ac361f3(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *>::get(___terminal_result_18_rename_at_1353_437) )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1362_49_38; _temp_make_local_1362_49_38;
                char * __complete_message_rename_at_1361_438 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_79, cast<char * const >::from(((char *) "Error: Expected a string at ")), cast<int32_t>::from(__parser_rename_at_362_410.index)))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1362_49_38.text),(__complete_message_rename_at_1361_438));
                    das_copy((_temp_make_local_1362_49_38.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1362_49_38;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1365;
                das_zero(__mks_1365);
                return __mks_1365;
            })()));
        } else {
            char * __e_rename_at_1372_439 = (char *)(das_get_auto_tuple_field<char *,1,bool,char *>::get(___terminal_result_18_rename_at_1353_437));
            das_copy(__cut_rename_at_1084_435,true);
            char * __val_rename_at_1133_440; das_zero(__val_rename_at_1133_440); das_move(__val_rename_at_1133_440, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_80, cast<char * const >::from(((char *) "\"")), cast<char *>::from(__e_rename_at_1372_439), cast<char * const >::from(((char *) "\"")))));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1141_441; das_zero(__result_rename_at_1141_441); das_move(__result_rename_at_1141_441, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1141;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_440;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_410.index;
                return __mkt_1141;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_441);
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_3_rename_at_1086_436) )
    {
        --__parser_rename_at_362_410.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_3_rename_at_1086_436);
    } else {
        --__parser_rename_at_362_410.tabs;
        das_copy(__parser_rename_at_362_410.index,__parse_pos_rename_at_1076_434);
        if ( __cut_rename_at_1084_435 )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1110;
                das_zero(__mks_1110);
                return __mks_1110;
            })()));
        };
    };
    int32_t __parse_pos_rename_at_1076_442 = __parser_rename_at_362_410.index;
    ++__parser_rename_at_362_410.tabs;
    AutoTuple<bool,char *,int32_t> __res_4_rename_at_1086_443; das_zero(__res_4_rename_at_1086_443); das_move(__res_4_rename_at_1086_443, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),((char *) "("),1) )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1175_49_39; _temp_make_local_1175_49_39;
                char * __rest_rename_at_1173_444 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_81, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                char * __complete_message_rename_at_1174_445 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_82, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_444))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1175_49_39.text),(__complete_message_rename_at_1174_445));
                    das_copy((_temp_make_local_1175_49_39.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1175_49_39;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1179;
                das_zero(__mks_1179);
                return __mks_1179;
            })()));
        } else {
            AutoTuple<bool,char *,int32_t> __element2_rename_at_546_446; das_zero(__element2_rename_at_546_446); das_move(__element2_rename_at_546_446, _Funcparse_elementTickid_0x1_a4daaa8f79cfa9ed(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
            if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__element2_rename_at_546_446) )
            {
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_548;
                    das_zero(__mks_548);
                    return __mks_548;
                })()));
            } else {
                char * __e_rename_at_551_447; das_zero(__e_rename_at_551_447); das_move(__e_rename_at_551_447, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__element2_rename_at_546_446)));
                if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),((char *) ")"),1) )
                {
                    if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1175_49_40; _temp_make_local_1175_49_40;
                        char * __rest_rename_at_1173_448 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_83, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                        char * __complete_message_rename_at_1174_449 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_84, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_448))));
                        _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1175_49_40.text),(__complete_message_rename_at_1174_449));
                            das_copy((_temp_make_local_1175_49_40.index),(__parser_rename_at_362_410.index));
                            return _temp_make_local_1175_49_40;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                        AutoTuple<bool,char *,int32_t> __mks_1179;
                        das_zero(__mks_1179);
                        return __mks_1179;
                    })()));
                } else {
                    char * __val_rename_at_1133_450; das_zero(__val_rename_at_1133_450); das_move(__val_rename_at_1133_450, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                        return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_85, cast<char * const >::from(((char *) "(")), cast<char *>::from(__e_rename_at_551_447), cast<char * const >::from(((char *) ")")))));
                    })));
                    AutoTuple<bool,char *,int32_t> __result_rename_at_1141_451; das_zero(__result_rename_at_1141_451); das_move(__result_rename_at_1141_451, (([&]() -> AutoTuple<bool,char *,int32_t> {
                        AutoTuple<bool,char *,int32_t> __mkt_1141;
                        das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                        das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_450;
                        das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_410.index;
                        return __mkt_1141;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_451);
                };
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_4_rename_at_1086_443) )
    {
        --__parser_rename_at_362_410.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_4_rename_at_1086_443);
    } else {
        --__parser_rename_at_362_410.tabs;
        das_copy(__parser_rename_at_362_410.index,__parse_pos_rename_at_1076_442);
    };
    int32_t __parse_pos_rename_at_1076_452 = __parser_rename_at_362_410.index;
    ++__parser_rename_at_362_410.tabs;
    AutoTuple<bool,char *,int32_t> __res_5_rename_at_1086_453; das_zero(__res_5_rename_at_1086_453); das_move(__res_5_rename_at_1086_453, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        int32_t __ext_pos_1_rename_at_564_454 = __parser_rename_at_362_410.index;
        bool ___terminal_result_22_rename_at_1199_455 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
            TDim<bool,256> _temp_make_local_1195_39_41; _temp_make_local_1195_39_41;
            int32_t __ch_rename_at_1194_456 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
            return das_auto_cast<bool>::cast((__ch_rename_at_1194_456 != -1) && (([&]() -> TDim<bool,256>& {
                _temp_make_local_1195_39_41(0,__context__) = true;
                _temp_make_local_1195_39_41(1,__context__) = true;
                _temp_make_local_1195_39_41(2,__context__) = true;
                _temp_make_local_1195_39_41(3,__context__) = true;
                _temp_make_local_1195_39_41(4,__context__) = true;
                _temp_make_local_1195_39_41(5,__context__) = true;
                _temp_make_local_1195_39_41(6,__context__) = true;
                _temp_make_local_1195_39_41(7,__context__) = true;
                _temp_make_local_1195_39_41(8,__context__) = true;
                _temp_make_local_1195_39_41(9,__context__) = true;
                _temp_make_local_1195_39_41(10,__context__) = true;
                _temp_make_local_1195_39_41(11,__context__) = true;
                _temp_make_local_1195_39_41(12,__context__) = true;
                _temp_make_local_1195_39_41(13,__context__) = true;
                _temp_make_local_1195_39_41(14,__context__) = true;
                _temp_make_local_1195_39_41(15,__context__) = true;
                _temp_make_local_1195_39_41(16,__context__) = true;
                _temp_make_local_1195_39_41(17,__context__) = true;
                _temp_make_local_1195_39_41(18,__context__) = true;
                _temp_make_local_1195_39_41(19,__context__) = true;
                _temp_make_local_1195_39_41(20,__context__) = true;
                _temp_make_local_1195_39_41(21,__context__) = true;
                _temp_make_local_1195_39_41(22,__context__) = true;
                _temp_make_local_1195_39_41(23,__context__) = true;
                _temp_make_local_1195_39_41(24,__context__) = true;
                _temp_make_local_1195_39_41(25,__context__) = true;
                _temp_make_local_1195_39_41(26,__context__) = true;
                _temp_make_local_1195_39_41(27,__context__) = true;
                _temp_make_local_1195_39_41(28,__context__) = true;
                _temp_make_local_1195_39_41(29,__context__) = true;
                _temp_make_local_1195_39_41(30,__context__) = true;
                _temp_make_local_1195_39_41(31,__context__) = true;
                _temp_make_local_1195_39_41(32,__context__) = true;
                _temp_make_local_1195_39_41(33,__context__) = true;
                _temp_make_local_1195_39_41(34,__context__) = true;
                _temp_make_local_1195_39_41(35,__context__) = true;
                _temp_make_local_1195_39_41(36,__context__) = true;
                _temp_make_local_1195_39_41(37,__context__) = true;
                _temp_make_local_1195_39_41(38,__context__) = true;
                _temp_make_local_1195_39_41(39,__context__) = true;
                _temp_make_local_1195_39_41(40,__context__) = true;
                _temp_make_local_1195_39_41(41,__context__) = false;
                _temp_make_local_1195_39_41(42,__context__) = true;
                _temp_make_local_1195_39_41(43,__context__) = true;
                _temp_make_local_1195_39_41(44,__context__) = false;
                _temp_make_local_1195_39_41(45,__context__) = true;
                _temp_make_local_1195_39_41(46,__context__) = true;
                _temp_make_local_1195_39_41(47,__context__) = true;
                _temp_make_local_1195_39_41(48,__context__) = true;
                _temp_make_local_1195_39_41(49,__context__) = true;
                _temp_make_local_1195_39_41(50,__context__) = true;
                _temp_make_local_1195_39_41(51,__context__) = true;
                _temp_make_local_1195_39_41(52,__context__) = true;
                _temp_make_local_1195_39_41(53,__context__) = true;
                _temp_make_local_1195_39_41(54,__context__) = true;
                _temp_make_local_1195_39_41(55,__context__) = true;
                _temp_make_local_1195_39_41(56,__context__) = true;
                _temp_make_local_1195_39_41(57,__context__) = true;
                _temp_make_local_1195_39_41(58,__context__) = true;
                _temp_make_local_1195_39_41(59,__context__) = true;
                _temp_make_local_1195_39_41(60,__context__) = true;
                _temp_make_local_1195_39_41(61,__context__) = true;
                _temp_make_local_1195_39_41(62,__context__) = true;
                _temp_make_local_1195_39_41(63,__context__) = true;
                _temp_make_local_1195_39_41(64,__context__) = true;
                _temp_make_local_1195_39_41(65,__context__) = true;
                _temp_make_local_1195_39_41(66,__context__) = true;
                _temp_make_local_1195_39_41(67,__context__) = true;
                _temp_make_local_1195_39_41(68,__context__) = true;
                _temp_make_local_1195_39_41(69,__context__) = true;
                _temp_make_local_1195_39_41(70,__context__) = true;
                _temp_make_local_1195_39_41(71,__context__) = true;
                _temp_make_local_1195_39_41(72,__context__) = true;
                _temp_make_local_1195_39_41(73,__context__) = true;
                _temp_make_local_1195_39_41(74,__context__) = true;
                _temp_make_local_1195_39_41(75,__context__) = true;
                _temp_make_local_1195_39_41(76,__context__) = true;
                _temp_make_local_1195_39_41(77,__context__) = true;
                _temp_make_local_1195_39_41(78,__context__) = true;
                _temp_make_local_1195_39_41(79,__context__) = true;
                _temp_make_local_1195_39_41(80,__context__) = true;
                _temp_make_local_1195_39_41(81,__context__) = true;
                _temp_make_local_1195_39_41(82,__context__) = true;
                _temp_make_local_1195_39_41(83,__context__) = true;
                _temp_make_local_1195_39_41(84,__context__) = true;
                _temp_make_local_1195_39_41(85,__context__) = true;
                _temp_make_local_1195_39_41(86,__context__) = true;
                _temp_make_local_1195_39_41(87,__context__) = true;
                _temp_make_local_1195_39_41(88,__context__) = true;
                _temp_make_local_1195_39_41(89,__context__) = true;
                _temp_make_local_1195_39_41(90,__context__) = true;
                _temp_make_local_1195_39_41(91,__context__) = true;
                _temp_make_local_1195_39_41(92,__context__) = true;
                _temp_make_local_1195_39_41(93,__context__) = true;
                _temp_make_local_1195_39_41(94,__context__) = true;
                _temp_make_local_1195_39_41(95,__context__) = true;
                _temp_make_local_1195_39_41(96,__context__) = true;
                _temp_make_local_1195_39_41(97,__context__) = true;
                _temp_make_local_1195_39_41(98,__context__) = true;
                _temp_make_local_1195_39_41(99,__context__) = true;
                _temp_make_local_1195_39_41(100,__context__) = true;
                _temp_make_local_1195_39_41(101,__context__) = true;
                _temp_make_local_1195_39_41(102,__context__) = true;
                _temp_make_local_1195_39_41(103,__context__) = true;
                _temp_make_local_1195_39_41(104,__context__) = true;
                _temp_make_local_1195_39_41(105,__context__) = true;
                _temp_make_local_1195_39_41(106,__context__) = true;
                _temp_make_local_1195_39_41(107,__context__) = true;
                _temp_make_local_1195_39_41(108,__context__) = true;
                _temp_make_local_1195_39_41(109,__context__) = true;
                _temp_make_local_1195_39_41(110,__context__) = true;
                _temp_make_local_1195_39_41(111,__context__) = true;
                _temp_make_local_1195_39_41(112,__context__) = true;
                _temp_make_local_1195_39_41(113,__context__) = true;
                _temp_make_local_1195_39_41(114,__context__) = true;
                _temp_make_local_1195_39_41(115,__context__) = true;
                _temp_make_local_1195_39_41(116,__context__) = true;
                _temp_make_local_1195_39_41(117,__context__) = true;
                _temp_make_local_1195_39_41(118,__context__) = true;
                _temp_make_local_1195_39_41(119,__context__) = true;
                _temp_make_local_1195_39_41(120,__context__) = true;
                _temp_make_local_1195_39_41(121,__context__) = true;
                _temp_make_local_1195_39_41(122,__context__) = true;
                _temp_make_local_1195_39_41(123,__context__) = true;
                _temp_make_local_1195_39_41(124,__context__) = true;
                _temp_make_local_1195_39_41(125,__context__) = true;
                _temp_make_local_1195_39_41(126,__context__) = true;
                _temp_make_local_1195_39_41(127,__context__) = true;
                _temp_make_local_1195_39_41(128,__context__) = true;
                _temp_make_local_1195_39_41(129,__context__) = true;
                _temp_make_local_1195_39_41(130,__context__) = true;
                _temp_make_local_1195_39_41(131,__context__) = true;
                _temp_make_local_1195_39_41(132,__context__) = true;
                _temp_make_local_1195_39_41(133,__context__) = true;
                _temp_make_local_1195_39_41(134,__context__) = true;
                _temp_make_local_1195_39_41(135,__context__) = true;
                _temp_make_local_1195_39_41(136,__context__) = true;
                _temp_make_local_1195_39_41(137,__context__) = true;
                _temp_make_local_1195_39_41(138,__context__) = true;
                _temp_make_local_1195_39_41(139,__context__) = true;
                _temp_make_local_1195_39_41(140,__context__) = true;
                _temp_make_local_1195_39_41(141,__context__) = true;
                _temp_make_local_1195_39_41(142,__context__) = true;
                _temp_make_local_1195_39_41(143,__context__) = true;
                _temp_make_local_1195_39_41(144,__context__) = true;
                _temp_make_local_1195_39_41(145,__context__) = true;
                _temp_make_local_1195_39_41(146,__context__) = true;
                _temp_make_local_1195_39_41(147,__context__) = true;
                _temp_make_local_1195_39_41(148,__context__) = true;
                _temp_make_local_1195_39_41(149,__context__) = true;
                _temp_make_local_1195_39_41(150,__context__) = true;
                _temp_make_local_1195_39_41(151,__context__) = true;
                _temp_make_local_1195_39_41(152,__context__) = true;
                _temp_make_local_1195_39_41(153,__context__) = true;
                _temp_make_local_1195_39_41(154,__context__) = true;
                _temp_make_local_1195_39_41(155,__context__) = true;
                _temp_make_local_1195_39_41(156,__context__) = true;
                _temp_make_local_1195_39_41(157,__context__) = true;
                _temp_make_local_1195_39_41(158,__context__) = true;
                _temp_make_local_1195_39_41(159,__context__) = true;
                _temp_make_local_1195_39_41(160,__context__) = true;
                _temp_make_local_1195_39_41(161,__context__) = true;
                _temp_make_local_1195_39_41(162,__context__) = true;
                _temp_make_local_1195_39_41(163,__context__) = true;
                _temp_make_local_1195_39_41(164,__context__) = true;
                _temp_make_local_1195_39_41(165,__context__) = true;
                _temp_make_local_1195_39_41(166,__context__) = true;
                _temp_make_local_1195_39_41(167,__context__) = true;
                _temp_make_local_1195_39_41(168,__context__) = true;
                _temp_make_local_1195_39_41(169,__context__) = true;
                _temp_make_local_1195_39_41(170,__context__) = true;
                _temp_make_local_1195_39_41(171,__context__) = true;
                _temp_make_local_1195_39_41(172,__context__) = true;
                _temp_make_local_1195_39_41(173,__context__) = true;
                _temp_make_local_1195_39_41(174,__context__) = true;
                _temp_make_local_1195_39_41(175,__context__) = true;
                _temp_make_local_1195_39_41(176,__context__) = true;
                _temp_make_local_1195_39_41(177,__context__) = true;
                _temp_make_local_1195_39_41(178,__context__) = true;
                _temp_make_local_1195_39_41(179,__context__) = true;
                _temp_make_local_1195_39_41(180,__context__) = true;
                _temp_make_local_1195_39_41(181,__context__) = true;
                _temp_make_local_1195_39_41(182,__context__) = true;
                _temp_make_local_1195_39_41(183,__context__) = true;
                _temp_make_local_1195_39_41(184,__context__) = true;
                _temp_make_local_1195_39_41(185,__context__) = true;
                _temp_make_local_1195_39_41(186,__context__) = true;
                _temp_make_local_1195_39_41(187,__context__) = true;
                _temp_make_local_1195_39_41(188,__context__) = true;
                _temp_make_local_1195_39_41(189,__context__) = true;
                _temp_make_local_1195_39_41(190,__context__) = true;
                _temp_make_local_1195_39_41(191,__context__) = true;
                _temp_make_local_1195_39_41(192,__context__) = true;
                _temp_make_local_1195_39_41(193,__context__) = true;
                _temp_make_local_1195_39_41(194,__context__) = true;
                _temp_make_local_1195_39_41(195,__context__) = true;
                _temp_make_local_1195_39_41(196,__context__) = true;
                _temp_make_local_1195_39_41(197,__context__) = true;
                _temp_make_local_1195_39_41(198,__context__) = true;
                _temp_make_local_1195_39_41(199,__context__) = true;
                _temp_make_local_1195_39_41(200,__context__) = true;
                _temp_make_local_1195_39_41(201,__context__) = true;
                _temp_make_local_1195_39_41(202,__context__) = true;
                _temp_make_local_1195_39_41(203,__context__) = true;
                _temp_make_local_1195_39_41(204,__context__) = true;
                _temp_make_local_1195_39_41(205,__context__) = true;
                _temp_make_local_1195_39_41(206,__context__) = true;
                _temp_make_local_1195_39_41(207,__context__) = true;
                _temp_make_local_1195_39_41(208,__context__) = true;
                _temp_make_local_1195_39_41(209,__context__) = true;
                _temp_make_local_1195_39_41(210,__context__) = true;
                _temp_make_local_1195_39_41(211,__context__) = true;
                _temp_make_local_1195_39_41(212,__context__) = true;
                _temp_make_local_1195_39_41(213,__context__) = true;
                _temp_make_local_1195_39_41(214,__context__) = true;
                _temp_make_local_1195_39_41(215,__context__) = true;
                _temp_make_local_1195_39_41(216,__context__) = true;
                _temp_make_local_1195_39_41(217,__context__) = true;
                _temp_make_local_1195_39_41(218,__context__) = true;
                _temp_make_local_1195_39_41(219,__context__) = true;
                _temp_make_local_1195_39_41(220,__context__) = true;
                _temp_make_local_1195_39_41(221,__context__) = true;
                _temp_make_local_1195_39_41(222,__context__) = true;
                _temp_make_local_1195_39_41(223,__context__) = true;
                _temp_make_local_1195_39_41(224,__context__) = true;
                _temp_make_local_1195_39_41(225,__context__) = true;
                _temp_make_local_1195_39_41(226,__context__) = true;
                _temp_make_local_1195_39_41(227,__context__) = true;
                _temp_make_local_1195_39_41(228,__context__) = true;
                _temp_make_local_1195_39_41(229,__context__) = true;
                _temp_make_local_1195_39_41(230,__context__) = true;
                _temp_make_local_1195_39_41(231,__context__) = true;
                _temp_make_local_1195_39_41(232,__context__) = true;
                _temp_make_local_1195_39_41(233,__context__) = true;
                _temp_make_local_1195_39_41(234,__context__) = true;
                _temp_make_local_1195_39_41(235,__context__) = true;
                _temp_make_local_1195_39_41(236,__context__) = true;
                _temp_make_local_1195_39_41(237,__context__) = true;
                _temp_make_local_1195_39_41(238,__context__) = true;
                _temp_make_local_1195_39_41(239,__context__) = true;
                _temp_make_local_1195_39_41(240,__context__) = true;
                _temp_make_local_1195_39_41(241,__context__) = true;
                _temp_make_local_1195_39_41(242,__context__) = true;
                _temp_make_local_1195_39_41(243,__context__) = true;
                _temp_make_local_1195_39_41(244,__context__) = true;
                _temp_make_local_1195_39_41(245,__context__) = true;
                _temp_make_local_1195_39_41(246,__context__) = true;
                _temp_make_local_1195_39_41(247,__context__) = true;
                _temp_make_local_1195_39_41(248,__context__) = true;
                _temp_make_local_1195_39_41(249,__context__) = true;
                _temp_make_local_1195_39_41(250,__context__) = true;
                _temp_make_local_1195_39_41(251,__context__) = true;
                _temp_make_local_1195_39_41(252,__context__) = true;
                _temp_make_local_1195_39_41(253,__context__) = true;
                _temp_make_local_1195_39_41(254,__context__) = true;
                _temp_make_local_1195_39_41(255,__context__) = true;
                return _temp_make_local_1195_39_41;
            })())(__ch_rename_at_1194_456,__context__));
        });
        if ( !___terminal_result_22_rename_at_1199_455 )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1217_49_42; _temp_make_local_1217_49_42;
                char * __rest_rename_at_1215_457 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_86, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                char * __complete_message_rename_at_1216_458 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_87, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '0'-'40'  '42'-'43'  '45'-'256' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1215_457))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1217_49_42.text),(__complete_message_rename_at_1216_458));
                    das_copy((_temp_make_local_1217_49_42.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1217_49_42;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1220;
                das_zero(__mks_1220);
                return __mks_1220;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),1);
        };
        char * __str_1_rename_at_566_459 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_410.input(0,__context__))));
        char * __e_rename_at_567_460 = (char *)(((char * const )(builtin_string_slice1(__str_1_rename_at_566_459,__ext_pos_1_rename_at_564_454,__parser_rename_at_362_410.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        char * __val_rename_at_1133_461; das_zero(__val_rename_at_1133_461); das_move(__val_rename_at_1133_461, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
            return das_auto_cast<char *>::cast(__e_rename_at_567_460);
        })));
        AutoTuple<bool,char *,int32_t> __result_rename_at_1141_462; das_zero(__result_rename_at_1141_462); das_move(__result_rename_at_1141_462, (([&]() -> AutoTuple<bool,char *,int32_t> {
            AutoTuple<bool,char *,int32_t> __mkt_1141;
            das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
            das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_461;
            das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_410.index;
            return __mkt_1141;
        })()));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_462);
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_5_rename_at_1086_453) )
    {
        --__parser_rename_at_362_410.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_5_rename_at_1086_453);
    } else {
        --__parser_rename_at_362_410.tabs;
        das_copy(__parser_rename_at_362_410.index,__parse_pos_rename_at_1076_452);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
        AutoTuple<bool,char *,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_elementTickid_0x1_70d6c8e08871cfa1 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_463 )
{
    int32_t __mark_rename_at_147_464 = __parser_rename_at_146_463.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_463.sub_element_cache),__parser_rename_at_146_463.index) && !(__parser_rename_at_146_463.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_465; das_zero(__result_rename_at_155_465); das_move(__result_rename_at_155_465, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_463.sub_element_cache(__parser_rename_at_146_463.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_465) )
        {
            das_copy(__parser_rename_at_146_463.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_465));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_465);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_466; das_zero(__result_rename_at_169_466); das_move(__result_rename_at_169_466, _Funcparse_sub_element_innerTickid_0x1_3ffc02542601bf64(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_463)));
        das_copy(__parser_rename_at_146_463.sub_element_cache(__mark_rename_at_147_464,__context__),__result_rename_at_169_466);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_466);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_element_innerTickid_0x1_ab5d94b9909bbf99 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_467 )
{
    int32_t __parse_pos_rename_at_1076_468 = __parser_rename_at_362_467.index;
    ++__parser_rename_at_362_467.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_469; das_zero(__res_1_rename_at_1086_469); das_move(__res_1_rename_at_1086_469, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        AutoTuple<bool,char *,int32_t> __sub_element0_rename_at_546_470; das_zero(__sub_element0_rename_at_546_470); das_move(__sub_element0_rename_at_546_470, _Funcparse_sub_elementTickid_0x1_70d6c8e08871cfa1(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_467)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__sub_element0_rename_at_546_470) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __head_rename_at_551_471; das_zero(__head_rename_at_551_471); das_move(__head_rename_at_551_471, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__sub_element0_rename_at_546_470)));
            TArray<char *> __tail_rename_at_671_472;das_zero(__tail_rename_at_671_472);
            _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_467.suppress_errors),true);
            while ( true )
            {
                int32_t __sz_rename_at_674_473 = builtin_array_size(das_arg<TArray<char *>>::pass(__tail_rename_at_671_472));
                das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                    AutoTuple<bool,char *,int32_t> __sub_element1_rename_at_546_474; das_zero(__sub_element1_rename_at_546_474); das_move(__sub_element1_rename_at_546_474, _Funcparse_sub_elementTickid_0x1_70d6c8e08871cfa1(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_467)));
                    if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__sub_element1_rename_at_546_474) )
                    {
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                            AutoTuple<bool,int32_t,int32_t> __mks_548;
                            das_zero(__mks_548);
                            return __mks_548;
                        })()));
                    } else {
                        char * __temp_name_rename_at_551_475; das_zero(__temp_name_rename_at_551_475); das_move(__temp_name_rename_at_551_475, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__sub_element1_rename_at_546_474)));
                        _FuncbuiltinTickemplaceTick13923705686753630697_2d94a9a555d8bee7(__context__,das_arg<TArray<char *>>::pass(__tail_rename_at_671_472),__temp_name_rename_at_551_475);
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                            AutoTuple<bool,int32_t,int32_t> __mkt_666;
                            das_get_auto_tuple_field<bool,0,bool,int32_t,int32_t>::get(__mkt_666) = true;
                            das_get_auto_tuple_field<int32_t,1,bool,int32_t,int32_t>::get(__mkt_666) = 0;
                            das_get_auto_tuple_field<int32_t,2,bool,int32_t,int32_t>::get(__mkt_666) = 0;
                            return __mkt_666;
                        })()));
                    };
                });
                if ( __sz_rename_at_674_473 == builtin_array_size(das_arg<TArray<char *>>::pass(__tail_rename_at_671_472)) )
                {
                    break;
                };
            };
            _FuncbuiltinTickpopTick1161079256290593740_e59f84a1c28ac19(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_467.suppress_errors));
            char * __val_rename_at_1133_476; das_zero(__val_rename_at_1133_476); das_move(__val_rename_at_1133_476, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__head_rename_at_551_471),cast<char *>::from(_Funcstrings_boostTickjoinTick16475640899284277631_16b92910c086970e(__context__,das_arg<TArray<char *>>::pass(__tail_rename_at_671_472),nullptr)),*__context__,nullptr)));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1141_477; das_zero(__result_rename_at_1141_477); das_move(__result_rename_at_1141_477, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1141;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_476;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_467.index;
                return __mkt_1141;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_477);
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_1_rename_at_1086_469) )
    {
        --__parser_rename_at_362_467.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_1_rename_at_1086_469);
    } else {
        --__parser_rename_at_362_467.tabs;
        das_copy(__parser_rename_at_362_467.index,__parse_pos_rename_at_1076_468);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
        AutoTuple<bool,char *,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_elementTickid_0x1_a4daaa8f79cfa9ed ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_478 )
{
    int32_t __mark_rename_at_147_479 = __parser_rename_at_146_478.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_478.element_cache),__parser_rename_at_146_478.index) && !(__parser_rename_at_146_478.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_480; das_zero(__result_rename_at_155_480); das_move(__result_rename_at_155_480, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_478.element_cache(__parser_rename_at_146_478.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_480) )
        {
            das_copy(__parser_rename_at_146_478.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_480));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_480);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_481; das_zero(__result_rename_at_169_481); das_move(__result_rename_at_169_481, _Funcparse_element_innerTickid_0x1_ab5d94b9909bbf99(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_478)));
        das_copy(__parser_rename_at_146_478.element_cache(__mark_rename_at_147_479,__context__),__result_rename_at_169_481);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_481);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_class_name_innerTickid_0x1_8485552d58457ba0 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_482 )
{
    int32_t __parse_pos_rename_at_1076_483 = __parser_rename_at_362_482.index;
    ++__parser_rename_at_362_482.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_484; das_zero(__res_1_rename_at_1086_484); das_move(__res_1_rename_at_1086_484, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482));
        int32_t __ext_pos_2_rename_at_564_485 = __parser_rename_at_362_482.index;
        AutoTuple<bool,char *,int32_t> __ident_0_rename_at_525_486; das_zero(__ident_0_rename_at_525_486); das_move(__ident_0_rename_at_525_486, _Funcparse_ident_Tickid_0x1_ba14b1ca3a3dda32(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__ident_0_rename_at_525_486) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_527;
                das_zero(__mks_527);
                return __mks_527;
            })()));
        };
        char * __str_2_rename_at_566_487 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_482.input(0,__context__))));
        char * __scope_rename_at_567_488 = (char *)(((char * const )(builtin_string_slice1(__str_2_rename_at_566_487,__ext_pos_2_rename_at_564_485,__parser_rename_at_362_482.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        if ( !_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482),((char *) "::"),2) )
        {
            if ( (__parser_rename_at_362_482.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_482.suppress_errors)))) && (__parser_rename_at_362_482.index == __parser_rename_at_362_482.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1175_49_43; _temp_make_local_1175_49_43;
                char * __rest_rename_at_1173_489 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_88, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_482.index))),false));
                char * __complete_message_rename_at_1174_490 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_89, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m::\u001b[0m")), cast<char *>::from(__rest_rename_at_1173_489))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_482.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1175_49_43.text),(__complete_message_rename_at_1174_490));
                    das_copy((_temp_make_local_1175_49_43.index),(__parser_rename_at_362_482.index));
                    return _temp_make_local_1175_49_43;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1179;
                das_zero(__mks_1179);
                return __mks_1179;
            })()));
        } else {
            int32_t __ext_pos_3_rename_at_564_491 = __parser_rename_at_362_482.index;
            AutoTuple<bool,char *,int32_t> __ident_1_rename_at_525_492; das_zero(__ident_1_rename_at_525_492); das_move(__ident_1_rename_at_525_492, _Funcparse_ident_Tickid_0x1_ba14b1ca3a3dda32(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482)));
            if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__ident_1_rename_at_525_492) )
            {
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_527;
                    das_zero(__mks_527);
                    return __mks_527;
                })()));
            } else {
                char * __str_3_rename_at_566_493 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_482.input(0,__context__))));
                char * __id_rename_at_567_494 = (char *)(((char * const )(builtin_string_slice1(__str_3_rename_at_566_493,__ext_pos_3_rename_at_564_491,__parser_rename_at_362_482.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
                char * __val_rename_at_1133_495; das_zero(__val_rename_at_1133_495); das_move(__val_rename_at_1133_495, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                    return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from((cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__scope_rename_at_567_488),cast<char *>::from(((char *) "::")),*__context__,nullptr)))),cast<char *>::from(__id_rename_at_567_494),*__context__,nullptr)));
                })));
                AutoTuple<bool,char *,int32_t> __result_rename_at_1141_496; das_zero(__result_rename_at_1141_496); das_move(__result_rename_at_1141_496, (([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mkt_1141;
                    das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                    das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_495;
                    das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_482.index;
                    return __mkt_1141;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_496);
            };
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_1_rename_at_1086_484) )
    {
        --__parser_rename_at_362_482.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_1_rename_at_1086_484);
    } else {
        --__parser_rename_at_362_482.tabs;
        das_copy(__parser_rename_at_362_482.index,__parse_pos_rename_at_1076_483);
    };
    int32_t __parse_pos_rename_at_1076_497 = __parser_rename_at_362_482.index;
    ++__parser_rename_at_362_482.tabs;
    AutoTuple<bool,char *,int32_t> __res_2_rename_at_1086_498; das_zero(__res_2_rename_at_1086_498); das_move(__res_2_rename_at_1086_498, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482));
        AutoTuple<bool,char *,int32_t> __ident_2_rename_at_546_499; das_zero(__ident_2_rename_at_546_499); das_move(__ident_2_rename_at_546_499, _Funcparse_ident_Tickid_0x1_ba14b1ca3a3dda32(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__ident_2_rename_at_546_499) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __id_rename_at_551_500; das_zero(__id_rename_at_551_500); das_move(__id_rename_at_551_500, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__ident_2_rename_at_546_499)));
            char * __val_rename_at_1133_501; das_zero(__val_rename_at_1133_501); das_move(__val_rename_at_1133_501, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(__id_rename_at_551_500);
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1141_502; das_zero(__result_rename_at_1141_502); das_move(__result_rename_at_1141_502, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1141;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_501;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_482.index;
                return __mkt_1141;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_502);
        };
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_2_rename_at_1086_498) )
    {
        --__parser_rename_at_362_482.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_2_rename_at_1086_498);
    } else {
        --__parser_rename_at_362_482.tabs;
        das_copy(__parser_rename_at_362_482.index,__parse_pos_rename_at_1076_497);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
        AutoTuple<bool,char *,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_class_nameTickid_0x1_ea9ce584f39ea618 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_503 )
{
    int32_t __mark_rename_at_147_504 = __parser_rename_at_146_503.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_503.class_name_cache),__parser_rename_at_146_503.index) && !(__parser_rename_at_146_503.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_505; das_zero(__result_rename_at_155_505); das_move(__result_rename_at_155_505, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_503.class_name_cache(__parser_rename_at_146_503.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_505) )
        {
            das_copy(__parser_rename_at_146_503.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_505));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_505);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_506; das_zero(__result_rename_at_169_506); das_move(__result_rename_at_169_506, _Funcparse_class_name_innerTickid_0x1_8485552d58457ba0(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_503)));
        das_copy(__parser_rename_at_146_503.class_name_cache(__mark_rename_at_147_504,__context__),__result_rename_at_169_506);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_506);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_ident__innerTickid_0x1_2c1a7fef6a8c065e ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_507 )
{
    int32_t __parse_pos_rename_at_1076_508 = __parser_rename_at_362_507.index;
    ++__parser_rename_at_362_507.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_509; das_zero(__res_1_rename_at_1086_509); das_move(__res_1_rename_at_1086_509, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        int32_t __ext_pos_4_rename_at_564_510 = __parser_rename_at_362_507.index;
        bool ___terminal_result_26_rename_at_1199_511 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
            TDim<bool,256> _temp_make_local_1195_39_44; _temp_make_local_1195_39_44;
            int32_t __ch_rename_at_1194_512 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_507)));
            return das_auto_cast<bool>::cast((__ch_rename_at_1194_512 != -1) && (([&]() -> TDim<bool,256>& {
                _temp_make_local_1195_39_44(0,__context__) = false;
                _temp_make_local_1195_39_44(1,__context__) = false;
                _temp_make_local_1195_39_44(2,__context__) = false;
                _temp_make_local_1195_39_44(3,__context__) = false;
                _temp_make_local_1195_39_44(4,__context__) = false;
                _temp_make_local_1195_39_44(5,__context__) = false;
                _temp_make_local_1195_39_44(6,__context__) = false;
                _temp_make_local_1195_39_44(7,__context__) = false;
                _temp_make_local_1195_39_44(8,__context__) = false;
                _temp_make_local_1195_39_44(9,__context__) = false;
                _temp_make_local_1195_39_44(10,__context__) = false;
                _temp_make_local_1195_39_44(11,__context__) = false;
                _temp_make_local_1195_39_44(12,__context__) = false;
                _temp_make_local_1195_39_44(13,__context__) = false;
                _temp_make_local_1195_39_44(14,__context__) = false;
                _temp_make_local_1195_39_44(15,__context__) = false;
                _temp_make_local_1195_39_44(16,__context__) = false;
                _temp_make_local_1195_39_44(17,__context__) = false;
                _temp_make_local_1195_39_44(18,__context__) = false;
                _temp_make_local_1195_39_44(19,__context__) = false;
                _temp_make_local_1195_39_44(20,__context__) = false;
                _temp_make_local_1195_39_44(21,__context__) = false;
                _temp_make_local_1195_39_44(22,__context__) = false;
                _temp_make_local_1195_39_44(23,__context__) = false;
                _temp_make_local_1195_39_44(24,__context__) = false;
                _temp_make_local_1195_39_44(25,__context__) = false;
                _temp_make_local_1195_39_44(26,__context__) = false;
                _temp_make_local_1195_39_44(27,__context__) = false;
                _temp_make_local_1195_39_44(28,__context__) = false;
                _temp_make_local_1195_39_44(29,__context__) = false;
                _temp_make_local_1195_39_44(30,__context__) = false;
                _temp_make_local_1195_39_44(31,__context__) = false;
                _temp_make_local_1195_39_44(32,__context__) = false;
                _temp_make_local_1195_39_44(33,__context__) = false;
                _temp_make_local_1195_39_44(34,__context__) = false;
                _temp_make_local_1195_39_44(35,__context__) = false;
                _temp_make_local_1195_39_44(36,__context__) = false;
                _temp_make_local_1195_39_44(37,__context__) = false;
                _temp_make_local_1195_39_44(38,__context__) = false;
                _temp_make_local_1195_39_44(39,__context__) = false;
                _temp_make_local_1195_39_44(40,__context__) = false;
                _temp_make_local_1195_39_44(41,__context__) = false;
                _temp_make_local_1195_39_44(42,__context__) = false;
                _temp_make_local_1195_39_44(43,__context__) = false;
                _temp_make_local_1195_39_44(44,__context__) = false;
                _temp_make_local_1195_39_44(45,__context__) = false;
                _temp_make_local_1195_39_44(46,__context__) = false;
                _temp_make_local_1195_39_44(47,__context__) = false;
                _temp_make_local_1195_39_44(48,__context__) = false;
                _temp_make_local_1195_39_44(49,__context__) = false;
                _temp_make_local_1195_39_44(50,__context__) = false;
                _temp_make_local_1195_39_44(51,__context__) = false;
                _temp_make_local_1195_39_44(52,__context__) = false;
                _temp_make_local_1195_39_44(53,__context__) = false;
                _temp_make_local_1195_39_44(54,__context__) = false;
                _temp_make_local_1195_39_44(55,__context__) = false;
                _temp_make_local_1195_39_44(56,__context__) = false;
                _temp_make_local_1195_39_44(57,__context__) = false;
                _temp_make_local_1195_39_44(58,__context__) = false;
                _temp_make_local_1195_39_44(59,__context__) = false;
                _temp_make_local_1195_39_44(60,__context__) = false;
                _temp_make_local_1195_39_44(61,__context__) = false;
                _temp_make_local_1195_39_44(62,__context__) = false;
                _temp_make_local_1195_39_44(63,__context__) = false;
                _temp_make_local_1195_39_44(64,__context__) = false;
                _temp_make_local_1195_39_44(65,__context__) = true;
                _temp_make_local_1195_39_44(66,__context__) = true;
                _temp_make_local_1195_39_44(67,__context__) = true;
                _temp_make_local_1195_39_44(68,__context__) = true;
                _temp_make_local_1195_39_44(69,__context__) = true;
                _temp_make_local_1195_39_44(70,__context__) = true;
                _temp_make_local_1195_39_44(71,__context__) = true;
                _temp_make_local_1195_39_44(72,__context__) = true;
                _temp_make_local_1195_39_44(73,__context__) = true;
                _temp_make_local_1195_39_44(74,__context__) = true;
                _temp_make_local_1195_39_44(75,__context__) = true;
                _temp_make_local_1195_39_44(76,__context__) = true;
                _temp_make_local_1195_39_44(77,__context__) = true;
                _temp_make_local_1195_39_44(78,__context__) = true;
                _temp_make_local_1195_39_44(79,__context__) = true;
                _temp_make_local_1195_39_44(80,__context__) = true;
                _temp_make_local_1195_39_44(81,__context__) = true;
                _temp_make_local_1195_39_44(82,__context__) = true;
                _temp_make_local_1195_39_44(83,__context__) = true;
                _temp_make_local_1195_39_44(84,__context__) = true;
                _temp_make_local_1195_39_44(85,__context__) = true;
                _temp_make_local_1195_39_44(86,__context__) = true;
                _temp_make_local_1195_39_44(87,__context__) = true;
                _temp_make_local_1195_39_44(88,__context__) = true;
                _temp_make_local_1195_39_44(89,__context__) = true;
                _temp_make_local_1195_39_44(90,__context__) = true;
                _temp_make_local_1195_39_44(91,__context__) = false;
                _temp_make_local_1195_39_44(92,__context__) = false;
                _temp_make_local_1195_39_44(93,__context__) = false;
                _temp_make_local_1195_39_44(94,__context__) = false;
                _temp_make_local_1195_39_44(95,__context__) = true;
                _temp_make_local_1195_39_44(96,__context__) = false;
                _temp_make_local_1195_39_44(97,__context__) = true;
                _temp_make_local_1195_39_44(98,__context__) = true;
                _temp_make_local_1195_39_44(99,__context__) = true;
                _temp_make_local_1195_39_44(100,__context__) = true;
                _temp_make_local_1195_39_44(101,__context__) = true;
                _temp_make_local_1195_39_44(102,__context__) = true;
                _temp_make_local_1195_39_44(103,__context__) = true;
                _temp_make_local_1195_39_44(104,__context__) = true;
                _temp_make_local_1195_39_44(105,__context__) = true;
                _temp_make_local_1195_39_44(106,__context__) = true;
                _temp_make_local_1195_39_44(107,__context__) = true;
                _temp_make_local_1195_39_44(108,__context__) = true;
                _temp_make_local_1195_39_44(109,__context__) = true;
                _temp_make_local_1195_39_44(110,__context__) = true;
                _temp_make_local_1195_39_44(111,__context__) = true;
                _temp_make_local_1195_39_44(112,__context__) = true;
                _temp_make_local_1195_39_44(113,__context__) = true;
                _temp_make_local_1195_39_44(114,__context__) = true;
                _temp_make_local_1195_39_44(115,__context__) = true;
                _temp_make_local_1195_39_44(116,__context__) = true;
                _temp_make_local_1195_39_44(117,__context__) = true;
                _temp_make_local_1195_39_44(118,__context__) = true;
                _temp_make_local_1195_39_44(119,__context__) = true;
                _temp_make_local_1195_39_44(120,__context__) = true;
                _temp_make_local_1195_39_44(121,__context__) = true;
                _temp_make_local_1195_39_44(122,__context__) = true;
                _temp_make_local_1195_39_44(123,__context__) = false;
                _temp_make_local_1195_39_44(124,__context__) = false;
                _temp_make_local_1195_39_44(125,__context__) = false;
                _temp_make_local_1195_39_44(126,__context__) = false;
                _temp_make_local_1195_39_44(127,__context__) = false;
                _temp_make_local_1195_39_44(128,__context__) = false;
                _temp_make_local_1195_39_44(129,__context__) = false;
                _temp_make_local_1195_39_44(130,__context__) = false;
                _temp_make_local_1195_39_44(131,__context__) = false;
                _temp_make_local_1195_39_44(132,__context__) = false;
                _temp_make_local_1195_39_44(133,__context__) = false;
                _temp_make_local_1195_39_44(134,__context__) = false;
                _temp_make_local_1195_39_44(135,__context__) = false;
                _temp_make_local_1195_39_44(136,__context__) = false;
                _temp_make_local_1195_39_44(137,__context__) = false;
                _temp_make_local_1195_39_44(138,__context__) = false;
                _temp_make_local_1195_39_44(139,__context__) = false;
                _temp_make_local_1195_39_44(140,__context__) = false;
                _temp_make_local_1195_39_44(141,__context__) = false;
                _temp_make_local_1195_39_44(142,__context__) = false;
                _temp_make_local_1195_39_44(143,__context__) = false;
                _temp_make_local_1195_39_44(144,__context__) = false;
                _temp_make_local_1195_39_44(145,__context__) = false;
                _temp_make_local_1195_39_44(146,__context__) = false;
                _temp_make_local_1195_39_44(147,__context__) = false;
                _temp_make_local_1195_39_44(148,__context__) = false;
                _temp_make_local_1195_39_44(149,__context__) = false;
                _temp_make_local_1195_39_44(150,__context__) = false;
                _temp_make_local_1195_39_44(151,__context__) = false;
                _temp_make_local_1195_39_44(152,__context__) = false;
                _temp_make_local_1195_39_44(153,__context__) = false;
                _temp_make_local_1195_39_44(154,__context__) = false;
                _temp_make_local_1195_39_44(155,__context__) = false;
                _temp_make_local_1195_39_44(156,__context__) = false;
                _temp_make_local_1195_39_44(157,__context__) = false;
                _temp_make_local_1195_39_44(158,__context__) = false;
                _temp_make_local_1195_39_44(159,__context__) = false;
                _temp_make_local_1195_39_44(160,__context__) = false;
                _temp_make_local_1195_39_44(161,__context__) = false;
                _temp_make_local_1195_39_44(162,__context__) = false;
                _temp_make_local_1195_39_44(163,__context__) = false;
                _temp_make_local_1195_39_44(164,__context__) = false;
                _temp_make_local_1195_39_44(165,__context__) = false;
                _temp_make_local_1195_39_44(166,__context__) = false;
                _temp_make_local_1195_39_44(167,__context__) = false;
                _temp_make_local_1195_39_44(168,__context__) = false;
                _temp_make_local_1195_39_44(169,__context__) = false;
                _temp_make_local_1195_39_44(170,__context__) = false;
                _temp_make_local_1195_39_44(171,__context__) = false;
                _temp_make_local_1195_39_44(172,__context__) = false;
                _temp_make_local_1195_39_44(173,__context__) = false;
                _temp_make_local_1195_39_44(174,__context__) = false;
                _temp_make_local_1195_39_44(175,__context__) = false;
                _temp_make_local_1195_39_44(176,__context__) = false;
                _temp_make_local_1195_39_44(177,__context__) = false;
                _temp_make_local_1195_39_44(178,__context__) = false;
                _temp_make_local_1195_39_44(179,__context__) = false;
                _temp_make_local_1195_39_44(180,__context__) = false;
                _temp_make_local_1195_39_44(181,__context__) = false;
                _temp_make_local_1195_39_44(182,__context__) = false;
                _temp_make_local_1195_39_44(183,__context__) = false;
                _temp_make_local_1195_39_44(184,__context__) = false;
                _temp_make_local_1195_39_44(185,__context__) = false;
                _temp_make_local_1195_39_44(186,__context__) = false;
                _temp_make_local_1195_39_44(187,__context__) = false;
                _temp_make_local_1195_39_44(188,__context__) = false;
                _temp_make_local_1195_39_44(189,__context__) = false;
                _temp_make_local_1195_39_44(190,__context__) = false;
                _temp_make_local_1195_39_44(191,__context__) = false;
                _temp_make_local_1195_39_44(192,__context__) = false;
                _temp_make_local_1195_39_44(193,__context__) = false;
                _temp_make_local_1195_39_44(194,__context__) = false;
                _temp_make_local_1195_39_44(195,__context__) = false;
                _temp_make_local_1195_39_44(196,__context__) = false;
                _temp_make_local_1195_39_44(197,__context__) = false;
                _temp_make_local_1195_39_44(198,__context__) = false;
                _temp_make_local_1195_39_44(199,__context__) = false;
                _temp_make_local_1195_39_44(200,__context__) = false;
                _temp_make_local_1195_39_44(201,__context__) = false;
                _temp_make_local_1195_39_44(202,__context__) = false;
                _temp_make_local_1195_39_44(203,__context__) = false;
                _temp_make_local_1195_39_44(204,__context__) = false;
                _temp_make_local_1195_39_44(205,__context__) = false;
                _temp_make_local_1195_39_44(206,__context__) = false;
                _temp_make_local_1195_39_44(207,__context__) = false;
                _temp_make_local_1195_39_44(208,__context__) = false;
                _temp_make_local_1195_39_44(209,__context__) = false;
                _temp_make_local_1195_39_44(210,__context__) = false;
                _temp_make_local_1195_39_44(211,__context__) = false;
                _temp_make_local_1195_39_44(212,__context__) = false;
                _temp_make_local_1195_39_44(213,__context__) = false;
                _temp_make_local_1195_39_44(214,__context__) = false;
                _temp_make_local_1195_39_44(215,__context__) = false;
                _temp_make_local_1195_39_44(216,__context__) = false;
                _temp_make_local_1195_39_44(217,__context__) = false;
                _temp_make_local_1195_39_44(218,__context__) = false;
                _temp_make_local_1195_39_44(219,__context__) = false;
                _temp_make_local_1195_39_44(220,__context__) = false;
                _temp_make_local_1195_39_44(221,__context__) = false;
                _temp_make_local_1195_39_44(222,__context__) = false;
                _temp_make_local_1195_39_44(223,__context__) = false;
                _temp_make_local_1195_39_44(224,__context__) = false;
                _temp_make_local_1195_39_44(225,__context__) = false;
                _temp_make_local_1195_39_44(226,__context__) = false;
                _temp_make_local_1195_39_44(227,__context__) = false;
                _temp_make_local_1195_39_44(228,__context__) = false;
                _temp_make_local_1195_39_44(229,__context__) = false;
                _temp_make_local_1195_39_44(230,__context__) = false;
                _temp_make_local_1195_39_44(231,__context__) = false;
                _temp_make_local_1195_39_44(232,__context__) = false;
                _temp_make_local_1195_39_44(233,__context__) = false;
                _temp_make_local_1195_39_44(234,__context__) = false;
                _temp_make_local_1195_39_44(235,__context__) = false;
                _temp_make_local_1195_39_44(236,__context__) = false;
                _temp_make_local_1195_39_44(237,__context__) = false;
                _temp_make_local_1195_39_44(238,__context__) = false;
                _temp_make_local_1195_39_44(239,__context__) = false;
                _temp_make_local_1195_39_44(240,__context__) = false;
                _temp_make_local_1195_39_44(241,__context__) = false;
                _temp_make_local_1195_39_44(242,__context__) = false;
                _temp_make_local_1195_39_44(243,__context__) = false;
                _temp_make_local_1195_39_44(244,__context__) = false;
                _temp_make_local_1195_39_44(245,__context__) = false;
                _temp_make_local_1195_39_44(246,__context__) = false;
                _temp_make_local_1195_39_44(247,__context__) = false;
                _temp_make_local_1195_39_44(248,__context__) = false;
                _temp_make_local_1195_39_44(249,__context__) = false;
                _temp_make_local_1195_39_44(250,__context__) = false;
                _temp_make_local_1195_39_44(251,__context__) = false;
                _temp_make_local_1195_39_44(252,__context__) = false;
                _temp_make_local_1195_39_44(253,__context__) = false;
                _temp_make_local_1195_39_44(254,__context__) = false;
                _temp_make_local_1195_39_44(255,__context__) = false;
                return _temp_make_local_1195_39_44;
            })())(__ch_rename_at_1194_512,__context__));
        });
        if ( !___terminal_result_26_rename_at_1199_511 )
        {
            if ( (__parser_rename_at_362_507.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_507.suppress_errors)))) && (__parser_rename_at_362_507.index == __parser_rename_at_362_507.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1217_49_45; _temp_make_local_1217_49_45;
                char * __rest_rename_at_1215_513 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_90, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_507.index))),false));
                char * __complete_message_rename_at_1216_514 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_91, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '65'-'90'  '95'  '97'-'122' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1215_513))));
                _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_507.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1217_49_45.text),(__complete_message_rename_at_1216_514));
                    das_copy((_temp_make_local_1217_49_45.index),(__parser_rename_at_362_507.index));
                    return _temp_make_local_1217_49_45;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1220;
                das_zero(__mks_1220);
                return __mks_1220;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_507),1);
        };
        char * __str_4_rename_at_566_515 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_507.input(0,__context__))));
        char * __fc_rename_at_567_516 = (char *)(((char * const )(builtin_string_slice1(__str_4_rename_at_566_515,__ext_pos_4_rename_at_564_510,__parser_rename_at_362_507.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        int32_t __ext_pos_5_rename_at_564_517 = __parser_rename_at_362_507.index;
        _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_507.suppress_errors),true);
        while ( true )
        {
            AutoTuple<bool,int32_t,int32_t> __res_rename_at_644_518; das_zero(__res_rename_at_644_518); das_move(__res_rename_at_644_518, das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                bool ___terminal_result_27_rename_at_1199_519 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
                    TDim<bool,256> _temp_make_local_1195_39_46; _temp_make_local_1195_39_46;
                    int32_t __ch_rename_at_1194_520 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_507)));
                    return das_auto_cast<bool>::cast((__ch_rename_at_1194_520 != -1) && (([&]() -> TDim<bool,256>& {
                        _temp_make_local_1195_39_46(0,__context__) = false;
                        _temp_make_local_1195_39_46(1,__context__) = false;
                        _temp_make_local_1195_39_46(2,__context__) = false;
                        _temp_make_local_1195_39_46(3,__context__) = false;
                        _temp_make_local_1195_39_46(4,__context__) = false;
                        _temp_make_local_1195_39_46(5,__context__) = false;
                        _temp_make_local_1195_39_46(6,__context__) = false;
                        _temp_make_local_1195_39_46(7,__context__) = false;
                        _temp_make_local_1195_39_46(8,__context__) = false;
                        _temp_make_local_1195_39_46(9,__context__) = false;
                        _temp_make_local_1195_39_46(10,__context__) = false;
                        _temp_make_local_1195_39_46(11,__context__) = false;
                        _temp_make_local_1195_39_46(12,__context__) = false;
                        _temp_make_local_1195_39_46(13,__context__) = false;
                        _temp_make_local_1195_39_46(14,__context__) = false;
                        _temp_make_local_1195_39_46(15,__context__) = false;
                        _temp_make_local_1195_39_46(16,__context__) = false;
                        _temp_make_local_1195_39_46(17,__context__) = false;
                        _temp_make_local_1195_39_46(18,__context__) = false;
                        _temp_make_local_1195_39_46(19,__context__) = false;
                        _temp_make_local_1195_39_46(20,__context__) = false;
                        _temp_make_local_1195_39_46(21,__context__) = false;
                        _temp_make_local_1195_39_46(22,__context__) = false;
                        _temp_make_local_1195_39_46(23,__context__) = false;
                        _temp_make_local_1195_39_46(24,__context__) = false;
                        _temp_make_local_1195_39_46(25,__context__) = false;
                        _temp_make_local_1195_39_46(26,__context__) = false;
                        _temp_make_local_1195_39_46(27,__context__) = false;
                        _temp_make_local_1195_39_46(28,__context__) = false;
                        _temp_make_local_1195_39_46(29,__context__) = false;
                        _temp_make_local_1195_39_46(30,__context__) = false;
                        _temp_make_local_1195_39_46(31,__context__) = false;
                        _temp_make_local_1195_39_46(32,__context__) = false;
                        _temp_make_local_1195_39_46(33,__context__) = false;
                        _temp_make_local_1195_39_46(34,__context__) = false;
                        _temp_make_local_1195_39_46(35,__context__) = false;
                        _temp_make_local_1195_39_46(36,__context__) = false;
                        _temp_make_local_1195_39_46(37,__context__) = false;
                        _temp_make_local_1195_39_46(38,__context__) = false;
                        _temp_make_local_1195_39_46(39,__context__) = false;
                        _temp_make_local_1195_39_46(40,__context__) = false;
                        _temp_make_local_1195_39_46(41,__context__) = false;
                        _temp_make_local_1195_39_46(42,__context__) = false;
                        _temp_make_local_1195_39_46(43,__context__) = false;
                        _temp_make_local_1195_39_46(44,__context__) = false;
                        _temp_make_local_1195_39_46(45,__context__) = false;
                        _temp_make_local_1195_39_46(46,__context__) = false;
                        _temp_make_local_1195_39_46(47,__context__) = false;
                        _temp_make_local_1195_39_46(48,__context__) = true;
                        _temp_make_local_1195_39_46(49,__context__) = true;
                        _temp_make_local_1195_39_46(50,__context__) = true;
                        _temp_make_local_1195_39_46(51,__context__) = true;
                        _temp_make_local_1195_39_46(52,__context__) = true;
                        _temp_make_local_1195_39_46(53,__context__) = true;
                        _temp_make_local_1195_39_46(54,__context__) = true;
                        _temp_make_local_1195_39_46(55,__context__) = true;
                        _temp_make_local_1195_39_46(56,__context__) = true;
                        _temp_make_local_1195_39_46(57,__context__) = true;
                        _temp_make_local_1195_39_46(58,__context__) = false;
                        _temp_make_local_1195_39_46(59,__context__) = false;
                        _temp_make_local_1195_39_46(60,__context__) = false;
                        _temp_make_local_1195_39_46(61,__context__) = false;
                        _temp_make_local_1195_39_46(62,__context__) = false;
                        _temp_make_local_1195_39_46(63,__context__) = false;
                        _temp_make_local_1195_39_46(64,__context__) = false;
                        _temp_make_local_1195_39_46(65,__context__) = true;
                        _temp_make_local_1195_39_46(66,__context__) = true;
                        _temp_make_local_1195_39_46(67,__context__) = true;
                        _temp_make_local_1195_39_46(68,__context__) = true;
                        _temp_make_local_1195_39_46(69,__context__) = true;
                        _temp_make_local_1195_39_46(70,__context__) = true;
                        _temp_make_local_1195_39_46(71,__context__) = true;
                        _temp_make_local_1195_39_46(72,__context__) = true;
                        _temp_make_local_1195_39_46(73,__context__) = true;
                        _temp_make_local_1195_39_46(74,__context__) = true;
                        _temp_make_local_1195_39_46(75,__context__) = true;
                        _temp_make_local_1195_39_46(76,__context__) = true;
                        _temp_make_local_1195_39_46(77,__context__) = true;
                        _temp_make_local_1195_39_46(78,__context__) = true;
                        _temp_make_local_1195_39_46(79,__context__) = true;
                        _temp_make_local_1195_39_46(80,__context__) = true;
                        _temp_make_local_1195_39_46(81,__context__) = true;
                        _temp_make_local_1195_39_46(82,__context__) = true;
                        _temp_make_local_1195_39_46(83,__context__) = true;
                        _temp_make_local_1195_39_46(84,__context__) = true;
                        _temp_make_local_1195_39_46(85,__context__) = true;
                        _temp_make_local_1195_39_46(86,__context__) = true;
                        _temp_make_local_1195_39_46(87,__context__) = true;
                        _temp_make_local_1195_39_46(88,__context__) = true;
                        _temp_make_local_1195_39_46(89,__context__) = true;
                        _temp_make_local_1195_39_46(90,__context__) = true;
                        _temp_make_local_1195_39_46(91,__context__) = false;
                        _temp_make_local_1195_39_46(92,__context__) = false;
                        _temp_make_local_1195_39_46(93,__context__) = false;
                        _temp_make_local_1195_39_46(94,__context__) = false;
                        _temp_make_local_1195_39_46(95,__context__) = true;
                        _temp_make_local_1195_39_46(96,__context__) = false;
                        _temp_make_local_1195_39_46(97,__context__) = true;
                        _temp_make_local_1195_39_46(98,__context__) = true;
                        _temp_make_local_1195_39_46(99,__context__) = true;
                        _temp_make_local_1195_39_46(100,__context__) = true;
                        _temp_make_local_1195_39_46(101,__context__) = true;
                        _temp_make_local_1195_39_46(102,__context__) = true;
                        _temp_make_local_1195_39_46(103,__context__) = true;
                        _temp_make_local_1195_39_46(104,__context__) = true;
                        _temp_make_local_1195_39_46(105,__context__) = true;
                        _temp_make_local_1195_39_46(106,__context__) = true;
                        _temp_make_local_1195_39_46(107,__context__) = true;
                        _temp_make_local_1195_39_46(108,__context__) = true;
                        _temp_make_local_1195_39_46(109,__context__) = true;
                        _temp_make_local_1195_39_46(110,__context__) = true;
                        _temp_make_local_1195_39_46(111,__context__) = true;
                        _temp_make_local_1195_39_46(112,__context__) = true;
                        _temp_make_local_1195_39_46(113,__context__) = true;
                        _temp_make_local_1195_39_46(114,__context__) = true;
                        _temp_make_local_1195_39_46(115,__context__) = true;
                        _temp_make_local_1195_39_46(116,__context__) = true;
                        _temp_make_local_1195_39_46(117,__context__) = true;
                        _temp_make_local_1195_39_46(118,__context__) = true;
                        _temp_make_local_1195_39_46(119,__context__) = true;
                        _temp_make_local_1195_39_46(120,__context__) = true;
                        _temp_make_local_1195_39_46(121,__context__) = true;
                        _temp_make_local_1195_39_46(122,__context__) = true;
                        _temp_make_local_1195_39_46(123,__context__) = false;
                        _temp_make_local_1195_39_46(124,__context__) = false;
                        _temp_make_local_1195_39_46(125,__context__) = false;
                        _temp_make_local_1195_39_46(126,__context__) = false;
                        _temp_make_local_1195_39_46(127,__context__) = false;
                        _temp_make_local_1195_39_46(128,__context__) = false;
                        _temp_make_local_1195_39_46(129,__context__) = false;
                        _temp_make_local_1195_39_46(130,__context__) = false;
                        _temp_make_local_1195_39_46(131,__context__) = false;
                        _temp_make_local_1195_39_46(132,__context__) = false;
                        _temp_make_local_1195_39_46(133,__context__) = false;
                        _temp_make_local_1195_39_46(134,__context__) = false;
                        _temp_make_local_1195_39_46(135,__context__) = false;
                        _temp_make_local_1195_39_46(136,__context__) = false;
                        _temp_make_local_1195_39_46(137,__context__) = false;
                        _temp_make_local_1195_39_46(138,__context__) = false;
                        _temp_make_local_1195_39_46(139,__context__) = false;
                        _temp_make_local_1195_39_46(140,__context__) = false;
                        _temp_make_local_1195_39_46(141,__context__) = false;
                        _temp_make_local_1195_39_46(142,__context__) = false;
                        _temp_make_local_1195_39_46(143,__context__) = false;
                        _temp_make_local_1195_39_46(144,__context__) = false;
                        _temp_make_local_1195_39_46(145,__context__) = false;
                        _temp_make_local_1195_39_46(146,__context__) = false;
                        _temp_make_local_1195_39_46(147,__context__) = false;
                        _temp_make_local_1195_39_46(148,__context__) = false;
                        _temp_make_local_1195_39_46(149,__context__) = false;
                        _temp_make_local_1195_39_46(150,__context__) = false;
                        _temp_make_local_1195_39_46(151,__context__) = false;
                        _temp_make_local_1195_39_46(152,__context__) = false;
                        _temp_make_local_1195_39_46(153,__context__) = false;
                        _temp_make_local_1195_39_46(154,__context__) = false;
                        _temp_make_local_1195_39_46(155,__context__) = false;
                        _temp_make_local_1195_39_46(156,__context__) = false;
                        _temp_make_local_1195_39_46(157,__context__) = false;
                        _temp_make_local_1195_39_46(158,__context__) = false;
                        _temp_make_local_1195_39_46(159,__context__) = false;
                        _temp_make_local_1195_39_46(160,__context__) = false;
                        _temp_make_local_1195_39_46(161,__context__) = false;
                        _temp_make_local_1195_39_46(162,__context__) = false;
                        _temp_make_local_1195_39_46(163,__context__) = false;
                        _temp_make_local_1195_39_46(164,__context__) = false;
                        _temp_make_local_1195_39_46(165,__context__) = false;
                        _temp_make_local_1195_39_46(166,__context__) = false;
                        _temp_make_local_1195_39_46(167,__context__) = false;
                        _temp_make_local_1195_39_46(168,__context__) = false;
                        _temp_make_local_1195_39_46(169,__context__) = false;
                        _temp_make_local_1195_39_46(170,__context__) = false;
                        _temp_make_local_1195_39_46(171,__context__) = false;
                        _temp_make_local_1195_39_46(172,__context__) = false;
                        _temp_make_local_1195_39_46(173,__context__) = false;
                        _temp_make_local_1195_39_46(174,__context__) = false;
                        _temp_make_local_1195_39_46(175,__context__) = false;
                        _temp_make_local_1195_39_46(176,__context__) = false;
                        _temp_make_local_1195_39_46(177,__context__) = false;
                        _temp_make_local_1195_39_46(178,__context__) = false;
                        _temp_make_local_1195_39_46(179,__context__) = false;
                        _temp_make_local_1195_39_46(180,__context__) = false;
                        _temp_make_local_1195_39_46(181,__context__) = false;
                        _temp_make_local_1195_39_46(182,__context__) = false;
                        _temp_make_local_1195_39_46(183,__context__) = false;
                        _temp_make_local_1195_39_46(184,__context__) = false;
                        _temp_make_local_1195_39_46(185,__context__) = false;
                        _temp_make_local_1195_39_46(186,__context__) = false;
                        _temp_make_local_1195_39_46(187,__context__) = false;
                        _temp_make_local_1195_39_46(188,__context__) = false;
                        _temp_make_local_1195_39_46(189,__context__) = false;
                        _temp_make_local_1195_39_46(190,__context__) = false;
                        _temp_make_local_1195_39_46(191,__context__) = false;
                        _temp_make_local_1195_39_46(192,__context__) = false;
                        _temp_make_local_1195_39_46(193,__context__) = false;
                        _temp_make_local_1195_39_46(194,__context__) = false;
                        _temp_make_local_1195_39_46(195,__context__) = false;
                        _temp_make_local_1195_39_46(196,__context__) = false;
                        _temp_make_local_1195_39_46(197,__context__) = false;
                        _temp_make_local_1195_39_46(198,__context__) = false;
                        _temp_make_local_1195_39_46(199,__context__) = false;
                        _temp_make_local_1195_39_46(200,__context__) = false;
                        _temp_make_local_1195_39_46(201,__context__) = false;
                        _temp_make_local_1195_39_46(202,__context__) = false;
                        _temp_make_local_1195_39_46(203,__context__) = false;
                        _temp_make_local_1195_39_46(204,__context__) = false;
                        _temp_make_local_1195_39_46(205,__context__) = false;
                        _temp_make_local_1195_39_46(206,__context__) = false;
                        _temp_make_local_1195_39_46(207,__context__) = false;
                        _temp_make_local_1195_39_46(208,__context__) = false;
                        _temp_make_local_1195_39_46(209,__context__) = false;
                        _temp_make_local_1195_39_46(210,__context__) = false;
                        _temp_make_local_1195_39_46(211,__context__) = false;
                        _temp_make_local_1195_39_46(212,__context__) = false;
                        _temp_make_local_1195_39_46(213,__context__) = false;
                        _temp_make_local_1195_39_46(214,__context__) = false;
                        _temp_make_local_1195_39_46(215,__context__) = false;
                        _temp_make_local_1195_39_46(216,__context__) = false;
                        _temp_make_local_1195_39_46(217,__context__) = false;
                        _temp_make_local_1195_39_46(218,__context__) = false;
                        _temp_make_local_1195_39_46(219,__context__) = false;
                        _temp_make_local_1195_39_46(220,__context__) = false;
                        _temp_make_local_1195_39_46(221,__context__) = false;
                        _temp_make_local_1195_39_46(222,__context__) = false;
                        _temp_make_local_1195_39_46(223,__context__) = false;
                        _temp_make_local_1195_39_46(224,__context__) = false;
                        _temp_make_local_1195_39_46(225,__context__) = false;
                        _temp_make_local_1195_39_46(226,__context__) = false;
                        _temp_make_local_1195_39_46(227,__context__) = false;
                        _temp_make_local_1195_39_46(228,__context__) = false;
                        _temp_make_local_1195_39_46(229,__context__) = false;
                        _temp_make_local_1195_39_46(230,__context__) = false;
                        _temp_make_local_1195_39_46(231,__context__) = false;
                        _temp_make_local_1195_39_46(232,__context__) = false;
                        _temp_make_local_1195_39_46(233,__context__) = false;
                        _temp_make_local_1195_39_46(234,__context__) = false;
                        _temp_make_local_1195_39_46(235,__context__) = false;
                        _temp_make_local_1195_39_46(236,__context__) = false;
                        _temp_make_local_1195_39_46(237,__context__) = false;
                        _temp_make_local_1195_39_46(238,__context__) = false;
                        _temp_make_local_1195_39_46(239,__context__) = false;
                        _temp_make_local_1195_39_46(240,__context__) = false;
                        _temp_make_local_1195_39_46(241,__context__) = false;
                        _temp_make_local_1195_39_46(242,__context__) = false;
                        _temp_make_local_1195_39_46(243,__context__) = false;
                        _temp_make_local_1195_39_46(244,__context__) = false;
                        _temp_make_local_1195_39_46(245,__context__) = false;
                        _temp_make_local_1195_39_46(246,__context__) = false;
                        _temp_make_local_1195_39_46(247,__context__) = false;
                        _temp_make_local_1195_39_46(248,__context__) = false;
                        _temp_make_local_1195_39_46(249,__context__) = false;
                        _temp_make_local_1195_39_46(250,__context__) = false;
                        _temp_make_local_1195_39_46(251,__context__) = false;
                        _temp_make_local_1195_39_46(252,__context__) = false;
                        _temp_make_local_1195_39_46(253,__context__) = false;
                        _temp_make_local_1195_39_46(254,__context__) = false;
                        _temp_make_local_1195_39_46(255,__context__) = false;
                        return _temp_make_local_1195_39_46;
                    })())(__ch_rename_at_1194_520,__context__));
                });
                if ( !___terminal_result_27_rename_at_1199_519 )
                {
                    if ( (__parser_rename_at_362_507.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_507.suppress_errors)))) && (__parser_rename_at_362_507.index == __parser_rename_at_362_507.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1217_49_47; _temp_make_local_1217_49_47;
                        char * __rest_rename_at_1215_521 = (char *)(_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_92, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_507.index))),false));
                        char * __complete_message_rename_at_1216_522 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_93, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '48'-'57'  '65'-'90'  '95' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1215_521))));
                        _FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_507.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1217_49_47.text),(__complete_message_rename_at_1216_522));
                            das_copy((_temp_make_local_1217_49_47.index),(__parser_rename_at_362_507.index));
                            return _temp_make_local_1217_49_47;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                        AutoTuple<bool,int32_t,int32_t> __mks_1220;
                        das_zero(__mks_1220);
                        return __mks_1220;
                    })()));
                } else {
                    _FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_507),1);
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                        AutoTuple<bool,int32_t,int32_t> __mkt_637;
                        das_get_auto_tuple_field<bool,0,bool,int32_t,int32_t>::get(__mkt_637) = true;
                        das_get_auto_tuple_field<int32_t,1,bool,int32_t,int32_t>::get(__mkt_637) = 0;
                        das_get_auto_tuple_field<int32_t,2,bool,int32_t,int32_t>::get(__mkt_637) = __parser_rename_at_362_507.index;
                        return __mkt_637;
                    })()));
                };
            }));
            if ( !das_get_auto_tuple_field<bool,0,bool,int32_t,int32_t>::get(__res_rename_at_644_518) )
            {
                break;
            };
        };
        _FuncbuiltinTickpopTick1161079256290593740_e59f84a1c28ac19(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_507.suppress_errors));
        char * __str_5_rename_at_566_523 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_507.input(0,__context__))));
        char * __tail_rename_at_567_524 = (char *)(((char * const )(builtin_string_slice1(__str_5_rename_at_566_523,__ext_pos_5_rename_at_564_517,__parser_rename_at_362_507.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        char * __val_rename_at_1133_525; das_zero(__val_rename_at_1133_525); das_move(__val_rename_at_1133_525, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
            return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__fc_rename_at_567_516),cast<char *>::from(__tail_rename_at_567_524),*__context__,nullptr)));
        })));
        AutoTuple<bool,char *,int32_t> __result_rename_at_1141_526; das_zero(__result_rename_at_1141_526); das_move(__result_rename_at_1141_526, (([&]() -> AutoTuple<bool,char *,int32_t> {
            AutoTuple<bool,char *,int32_t> __mkt_1141;
            das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1141) = true;
            das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1141) = __val_rename_at_1133_525;
            das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1141) = __parser_rename_at_362_507.index;
            return __mkt_1141;
        })()));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1141_526);
    }));
    if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__res_1_rename_at_1086_509) )
    {
        --__parser_rename_at_362_507.tabs;
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__res_1_rename_at_1086_509);
    } else {
        --__parser_rename_at_362_507.tabs;
        das_copy(__parser_rename_at_362_507.index,__parse_pos_rename_at_1076_508);
    };
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
        AutoTuple<bool,char *,int32_t> __mks_364;
        das_zero(__mks_364);
        return __mks_364;
    })()));
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_ident_Tickid_0x1_ba14b1ca3a3dda32 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_527 )
{
    int32_t __mark_rename_at_147_528 = __parser_rename_at_146_527.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_527.ident__cache),__parser_rename_at_146_527.index) && !(__parser_rename_at_146_527.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_529; das_zero(__result_rename_at_155_529); das_move(__result_rename_at_155_529, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_527.ident__cache(__parser_rename_at_146_527.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_529) )
        {
            das_copy(__parser_rename_at_146_527.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_529));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_529);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_530; das_zero(__result_rename_at_169_530); das_move(__result_rename_at_169_530, _Funcparse_ident__innerTickid_0x1_2c1a7fef6a8c065e(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_527)));
        das_copy(__parser_rename_at_146_527.ident__cache(__mark_rename_at_147_528,__context__),__result_rename_at_169_530);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_530);
    };
}

inline Program * _FuncbuiltinTickget_ptrTick8468476673553620226_5f0e0f646d69bfc7 ( Context * __context__, smart_ptr_raw<Program> const  __src_rename_at_1778_531 )
{
    return das_auto_cast<Program *>::cast(das_cast<Program *>::cast(__src_rename_at_1778_531));
}

inline void instance_args_f6e03da14c477f63 ( Context * __context__, char * const  __invocation_rename_at_179_532, Block DAS_COMMENT((void,spoof::SpoofInvocation,TArray<peg::ParsingError> const )) const  &  __blk_rename_at_179_533 ) { das_stack_prologue __prologue(__context__,1200,"instance_args " DAS_FILE_LINE);
{
    spoof::invocationParserTickid_0x1 ___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534; memset((void*)&___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534,0,sizeof(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534));
    spoof::invocationParserTickid_0x1 __parser_rename_at_406_535; memset((void*)&__parser_rename_at_406_535,0,sizeof(__parser_rename_at_406_535));
    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_418_537; memset((void*)&__result_rename_at_418_537,0,sizeof(__result_rename_at_418_537));
    {
        AutoTuple<bool,spoof::SpoofInvocation,int32_t> _temp_make_local_419_22_48; _temp_make_local_419_22_48;
        /* finally */ auto __finally_405= das_finally([&](){
        finalize_593ca97eb449449f(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534));
        finalize_593ca97eb449449f(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_406_535));
        /* end finally */ });
        das_zero(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534);
        das_zero(__parser_rename_at_406_535);
        _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_406_535.suppress_errors),false);
        builtin_string_peek(__invocation_rename_at_179_532,das_make_block<void,TArray<uint8_t> const  &>(__context__,944,0,&__func_info__920fb1be533a8af4,[&](TArray<uint8_t> const  & __inp_rename_at_414_536) -> void{
            _FuncbuiltinTickcloneTick3038771811667655495_27d3da7beba8a3da(__context__,das_arg<TArray<uint8_t>>::pass(__parser_rename_at_406_535.input),__inp_rename_at_414_536);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_zero(__result_rename_at_418_537);
        _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_718c134fc6616938(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__result_rename_at_418_537),das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass((_temp_make_local_419_22_48 = (_Funcparse_macro_invocationTickid_0x1_b47e0c17f58865ea(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_406_535))))));
        if ( das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_418_537) )
        {
            TArray<peg::ParsingError> _temp_make_local_422_48_49; _temp_make_local_422_48_49;
            das_invoke<void>::invoke<spoof::SpoofInvocation &,TArray<peg::ParsingError> &>(__context__,nullptr,__blk_rename_at_179_533,das_arg<spoof::SpoofInvocation>::pass(das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_418_537)),das_arg<TArray<peg::ParsingError>>::pass((([&]() -> TArray<peg::ParsingError>& {
                das_zero(_temp_make_local_422_48_49);
                return _temp_make_local_422_48_49;
            })())));
            return ;
        } else {
            AutoTuple<bool,spoof::SpoofInvocation,int32_t> _temp_make_local_438_22_50; _temp_make_local_438_22_50;
            _FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752(__context__,das_arg<TArray<bool>>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.suppress_errors),false);
            builtin_string_peek(__invocation_rename_at_179_532,das_make_block<void,TArray<uint8_t> const  &>(__context__,1136,0,&__func_info__920fb1be533a8af4,[&](TArray<uint8_t> const  & __inp_rename_at_431_538) -> void{
                _FuncbuiltinTickcloneTick3038771811667655495_27d3da7beba8a3da(__context__,das_arg<TArray<uint8_t>>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.input),__inp_rename_at_431_538);
            }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            das_copy(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.error_reporting,true);
            das_copy(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.longest_prefix,__parser_rename_at_406_535.longest_prefix);
            _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_718c134fc6616938(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__result_rename_at_418_537),das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass((_temp_make_local_438_22_50 = (_Funcparse_macro_invocationTickid_0x1_b47e0c17f58865ea(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534))))));
            _FuncalgorithmTicksort_uniqueTick14985796887508933377_bf6db5262aa69fa5(__context__,das_arg<TArray<peg::ParsingError>>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.errors));
            das_invoke<void>::invoke<spoof::SpoofInvocation &,TArray<peg::ParsingError> &>(__context__,nullptr,__blk_rename_at_179_533,das_arg<spoof::SpoofInvocation>::pass(das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_418_537)),das_arg<TArray<peg::ParsingError>>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.errors));
        };
    };
}}

inline void clone_e6aaad296469300e ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  __dest_rename_at_170_539, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> const  &  __src_rename_at_170_540 )
{
    das_copy(das_get_auto_tuple_field<bool,0,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__dest_rename_at_170_539),das_get_auto_tuple_field<bool,0,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__src_rename_at_170_540));
    _FuncbuiltinTickcloneTick3038771811667655495_ae9c07db44db96c6(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(das_get_auto_tuple_field<TArray<AutoTuple<char *,char *>>,1,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__dest_rename_at_170_539)),das_get_auto_tuple_field<TArray<AutoTuple<char *,char *>>,1,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__src_rename_at_170_540));
    das_copy(das_get_auto_tuple_field<int32_t,2,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__dest_rename_at_170_539),das_get_auto_tuple_field<int32_t,2,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__src_rename_at_170_540));
}

inline void clone_fd9d2e92ebaff162 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  __dest_rename_at_170_541, AutoTuple<bool,spoof::SpoofInvocation,int32_t> const  &  __src_rename_at_170_542 )
{
    das_copy(das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__dest_rename_at_170_541),das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__src_rename_at_170_542));
    clone_4208deb8a0b826d6(__context__,das_arg<spoof::SpoofInvocation>::pass(das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__dest_rename_at_170_541)),das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__src_rename_at_170_542));
    das_copy(das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__dest_rename_at_170_541),das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__src_rename_at_170_542));
}

inline void clone_7eaba744dd27b4f ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> &  __dest_rename_at_170_543, AutoTuple<bool,TArray<char *>,int32_t> const  &  __src_rename_at_170_544 )
{
    das_copy(das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__dest_rename_at_170_543),das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__src_rename_at_170_544));
    _FuncbuiltinTickcloneTick3038771811667655495_7f476b9128488531(__context__,das_arg<TArray<char *>>::pass(das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(__dest_rename_at_170_543)),das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(__src_rename_at_170_544));
    das_copy(das_get_auto_tuple_field<int32_t,2,bool,TArray<char *>,int32_t>::get(__dest_rename_at_170_543),das_get_auto_tuple_field<int32_t,2,bool,TArray<char *>,int32_t>::get(__src_rename_at_170_544));
}

inline void clone_4208deb8a0b826d6 ( Context * __context__, spoof::SpoofInvocation &  __a_rename_at_174_545, spoof::SpoofInvocation const  &  __b_rename_at_174_546 )
{
    das_copy(__a_rename_at_174_545.varName,__b_rename_at_174_546.varName);
    _FuncbuiltinTickcloneTick3038771811667655495_7f476b9128488531(__context__,das_arg<TArray<char *>>::pass(__a_rename_at_174_545.args),__b_rename_at_174_546.args);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x3308153feeba5bd1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_a031df4a8cee6ee5>>();
    };
    aotLib[0x6a449a5af1c279b1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_24797d4c671a586b>>();
    };
    aotLib[0x24d12f08f4a8e1ea] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_4560c1849acb38a>>();
    };
    aotLib[0xb9ce82d1eb5fbb59] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_2467e182b2a5ed>>();
    };
    aotLib[0x9df986300e0b2bb0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_6eafafc4b7d66260>>();
    };
    aotLib[0x5b2a0fe8a1422822] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_d046fa145241cec3>>();
    };
    aotLib[0xbb8703df8aa77855] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_2113affcf78b6161>>();
    };
    aotLib[0x25479985db734dc0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_bfd4d91dc00fbaf3>>();
    };
    aotLib[0x71a491c19e5a2ee4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_da6556421276590d>>();
    };
    aotLib[0x970dc19cab4643ff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_3936267e0c2d5eea>>();
    };
    aotLib[0x116c464af180ba64] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_29780fffd82925e8>>();
    };
    aotLib[0xb4ed9b72693c7cab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_a95cfb4f229339b9>>();
    };
    aotLib[0xffbed205b31a8e69] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_b3588b00b43291d7>>();
    };
    aotLib[0xbe9b759e58c5e270] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_f64f6a4f9b43e8a8>>();
    };
    aotLib[0x74c0077dc8e02f6c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_740b5ec56415e23e>>();
    };
    aotLib[0x9926cfebd7ae72e2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e4c24aaee0c3ba6b>>();
    };
    aotLib[0x48cc4a16bdfa81bb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_cddee3e689dc39ad>>();
    };
    aotLib[0xc71a739544124b11] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_a825660656018503>>();
    };
    aotLib[0xde35172715b09469] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_90a84ecbbf0abd76>>();
    };
    aotLib[0x29f9ffdd08e96bbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_6e9e07c7cf5371d5>>();
    };
    aotLib[0x839eefaa216e2e98] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpush_cloneTick2035469273396957942_cee423d5a5783ec0>>();
    };
    aotLib[0x6af710fdf235f7f6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_77565a85d51f915c>>();
    };
    aotLib[0x153a2f8f2aee8574] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_55f52f0e88d35c62>>();
    };
    aotLib[0x522a583e9e54667d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_2bedf3e77ea4c9f3>>();
    };
    aotLib[0xd3750dbbc5818f0c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_e963ce4a12221ab9>>();
    };
    aotLib[0xe466bb9c90d3d998] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_9c1054907b56811d>>();
    };
    aotLib[0x45024d6089ed7bce] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_29d078faaa5f9fe2>>();
    };
    aotLib[0xfec157ecc8ab6e06] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_fd098149e3857e1c>>();
    };
    aotLib[0xa7f9e518e05d6c2d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_625304db010e36bc>>();
    };
    aotLib[0x5a9689d8ec01e55f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_faa91cbe8a8ef152>>();
    };
    aotLib[0xb4f61d4e20e4e1f9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_13a83f3db63ddadc>>();
    };
    aotLib[0x9e4441d89d005b31] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_2cd7253cd041b44f>>();
    };
    aotLib[0xc8c67016654c8656] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_5ab31b7ac47de8d4>>();
    };
    aotLib[0xe4d1b2481ae4eab3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_76164b501fa5f2f>>();
    };
    aotLib[0xbbf8eecd7d75ec76] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_f73505a010f165fb>>();
    };
    aotLib[0x635eebc6a4d7350b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_d766db4c7472c995>>();
    };
    aotLib[0x90845c960f38bcf8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_85d66a6acd78b35>>();
    };
    aotLib[0x3994914f1d990cd6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicktabulateTick3664919181896551618_e0b31bc236602422>>();
    };
    aotLib[0x58f93bed1c4147d0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FunccolorsTickblue_strTick419715327456344553_85edb7c1543a3a15>>();
    };
    aotLib[0xb7e090cd4b9f490c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FunccolorsTickgreen_strTick14807051613123094884_f38a76efee9c8c27>>();
    };
    aotLib[0x83b245b017826180] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_60cc482d7210f02c>>();
    };
    aotLib[0x8a9f4f86045fcfc6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickreached_EOFTick12193995001429396764_6454e47d0dd7b97f>>();
    };
    aotLib[0xfd4f7e0a94ae0339] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicktabulateTick3664919181896551618_703c893d8b441b44>>();
    };
    aotLib[0xf74e611473ec19d2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickreached_EOFTick12193995001429396764_63a321d796086836>>();
    };
    aotLib[0x5f2696b3d87c8c0e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemptyTick15399874715904164783_d23c2e725f60c80a>>();
    };
    aotLib[0xd5c6f3a37757808a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicksortTick14088969635007481283_b6071e2909e46233>>();
    };
    aotLib[0xfb61da65afd196d4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncalgorithmTickuniqueTick8642070526899511001_ea2881ba9416889f>>();
    };
    aotLib[0x5e39fed7f0e6f0d9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5ebbb8f8a79624b3>>();
    };
    aotLib[0xd1bcdbf64e00b868] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_d086567e061861cd>>();
    };
    aotLib[0xe65c50e70d485ebc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1516aad463652b16>>();
    };
    aotLib[0x41ca6f4a1c837b5f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_eca34382a0b7791d>>();
    };
    aotLib[0xcdfbc4272c7b8d68] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_59843c375840fe45>>();
    };
    aotLib[0x8457c0cbb0dbae4b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_70ca5f66113685e1>>();
    };
    aotLib[0x5cc82a2260b712b7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_27d3da7beba8a3da>>();
    };
    aotLib[0x5738ac76902c902b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_abfd1b004e2908bd>>();
    };
    aotLib[0xa02c33542eb46117] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_ce9078775cda6b21>>();
    };
    aotLib[0x696e09695033ef30] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_718c134fc6616938>>();
    };
    aotLib[0xfb7adb10e51efb0a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_213995c2dfdbcf59>>();
    };
    aotLib[0xf7e5b0117b7dd44a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_c4225ec7818fe341>>();
    };
    aotLib[0x76ec9f6db498c9f7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_ae9c07db44db96c6>>();
    };
    aotLib[0xc58919e0b100510] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_7f476b9128488531>>();
    };
    aotLib[0x75fa62af4ffbdd07] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_infoTick4618819779667933862_ab23a3e65f4f603>>();
    };
    aotLib[0x500788a53d692c9b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickskip_whitespaceTick3657885083884260190_72b18c514876c2d1>>();
    };
    aotLib[0x1701cea8fcde9498] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_plainTick18061110541707930728_ad4b70a81f1d0180>>();
    };
    aotLib[0xd47e5ef5a352bbc3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickmatchesTick4020677071890555356_8060a095191b1db6>>();
    };
    aotLib[0x9913424261853fca] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_failTick11731645277795022368_67176c2832debc0c>>();
    };
    aotLib[0xd097ee0a42d8d82b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickbackTick18296309835877697278_a417f4cc33bcb2fe>>();
    };
    aotLib[0xa2bb37a87e23a3f3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FunccolorsTickred_strTick17114617365443413298_d93e8cf1fd2ad99d>>();
    };
    aotLib[0xe225727f3577476e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_c4dee0604444ebad>>();
    };
    aotLib[0xa8f9ca92a59f0574] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_successTick3645718276917011819_88fd6ad4e27a61ae>>();
    };
    aotLib[0x725dc4599514bd52] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_31fc28cd28b2752>>();
    };
    aotLib[0xdd44a4b74050e324] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_b7cb402cbd718488>>();
    };
    aotLib[0xd3e6218419df2d59] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_e59f84a1c28ac19>>();
    };
    aotLib[0x5085f13dccb65e2e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_5bc44f666511c4f4>>();
    };
    aotLib[0x4cf3896ca3a16488] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_e89937f359ff4105>>();
    };
    aotLib[0xf6d6c87c79e31de1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_31e87ae3c763efa9>>();
    };
    aotLib[0xf9342281b68c34e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickget_current_charTick5927279557575598025_fc039a8bf864e2a1>>();
    };
    aotLib[0xbe2a5f1c3a9be043] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickmoveTick4844919725508203339_2ee0d8ea93b6ff8f>>();
    };
    aotLib[0xdbcb9ae0225aea5b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncpegTickmatch_string_literalTick12800953725978677998_50b323a2328ca30f>>();
    };
    aotLib[0xba4a8bdc274c145d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_53863556d16827b0>>();
    };
    aotLib[0x35e905f7253e228f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_2d94a9a555d8bee7>>();
    };
    aotLib[0x6f17aa40695e804c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick16475640899284277631_16b92910c086970e>>();
    };
    aotLib[0xf2a79ba11d7f7745] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_infoTick4618819779667933862_3fb191919d34d5c0>>();
    };
    aotLib[0x4cbbc6f43a33b4ec] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickskip_whitespaceTick3657885083884260190_eda7387e44c477bd>>();
    };
    aotLib[0x8f52de3020522e3a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_plainTick18061110541707930728_391a37924ab3bff7>>();
    };
    aotLib[0x2e32656d577c37c3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickmatchesTick4020677071890555356_d6851889dd8a0d5b>>();
    };
    aotLib[0x175cc34d609d8a0f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_failTick11731645277795022368_8369bcfc7f20004f>>();
    };
    aotLib[0x6084ab5c205f3204] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_successTick3645718276917011819_e4f6b21e9f6afcc5>>();
    };
    aotLib[0x2324064f6de5a42e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_18bc2a30ed120a5>>();
    };
    aotLib[0x795e18de53af6669] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_c6163711697a67ea>>();
    };
    aotLib[0x39758419de7ced2a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickget_current_charTick5927279557575598025_e1cd69b1a82906df>>();
    };
    aotLib[0x25a1bd4e7dc8c927] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickmoveTick4844919725508203339_30e82fc490b67b15>>();
    };
    aotLib[0x9b9aa589072bf28c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncpegTickmatch_string_literalTick12800953725978677998_5536b27e9ac361f3>>();
    };
    aotLib[0x99ccd271a995d1f5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncalgorithmTicksort_uniqueTick14985796887508933377_bf6db5262aa69fa5>>();
    };
    aotLib[0x535ace29ac5b0033] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_f896e7308b8984f3>>();
    };
    aotLib[0xbeed1e26a1db5a6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick16475640899284277631_8958ad763c1a8045>>();
    };
    aotLib[0xf39d916724e1f2c4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_593ca97eb449449f>>();
    };
    aotLib[0x704b60c028bac45c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_list_innerTickid_0x0_afef4622973f1478>>();
    };
    aotLib[0x367fef9226e37e19] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_listTickid_0x0_e08aaeacf4bca4a2>>();
    };
    aotLib[0x807c77509f2b4ebd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_comma_separated_elements_innerTickid_0x0_fc0f629550ec98ab>>();
    };
    aotLib[0x47d3e5c0657584ce] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_comma_separated_elementsTickid_0x0_3517a5e2f72086c7>>();
    };
    aotLib[0x38f81008ca964a6a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_innerTickid_0x0_c8e5e2bfd35033ea>>();
    };
    aotLib[0x8539e83e028fcbab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_elementTickid_0x0_a89929d20275ecb4>>();
    };
    aotLib[0x97aadb8765b2073] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_sub_element_innerTickid_0x0_4416149c422be215>>();
    };
    aotLib[0x104c6479fd84dfea] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_sub_elementTickid_0x0_aac556ebc4770900>>();
    };
    aotLib[0xcd2415b4cbed5d41] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_value_innerTickid_0x0_4a321894fbeec2f>>();
    };
    aotLib[0x2232833a0c4879c6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_valueTickid_0x0_126b58a15c99cc72>>();
    };
    aotLib[0x5393db2d53fa7852] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_ident__innerTickid_0x0_1def850e72a70e16>>();
    };
    aotLib[0x8b39485970f12bf3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_ident_Tickid_0x0_ac07755a3c8869f1>>();
    };
    aotLib[0xd8f7c60323f8b3ff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_76bd142a6cc93fe5>>();
    };
    aotLib[0x531b3ecb4a502a5f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_b295659320d5aa3e>>();
    };
    aotLib[0xaaeb5e356dad2c27] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_b5e1593e39a49db7>>();
    };
    aotLib[0x899a9a660251d256] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_86ba7065cbab662e>>();
    };
    aotLib[0x87ac6218844db094] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick4044332007967089362_67ac71029b5249f8>>();
    };
    aotLib[0x7da8a3678279f987] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_index_ifTick5144745413123287381_36aadb1599a6d4e8>>();
    };
    aotLib[0x40596a4f2c250cd4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_macro_invocation_innerTickid_0x1_5bd6ef28378dc241>>();
    };
    aotLib[0x98fdd69c9225f55a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_macro_invocationTickid_0x1_b47e0c17f58865ea>>();
    };
    aotLib[0xea561be1f8196575] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_list_innerTickid_0x1_ee8dc9dce2c72dd4>>();
    };
    aotLib[0xd5fc2aa667beb129] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_listTickid_0x1_7d5844c12f276ee3>>();
    };
    aotLib[0xb621f23f9c2b3241] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_comma_separated_elements_innerTickid_0x1_35f6d9d44c93b4da>>();
    };
    aotLib[0x20422d75a5050796] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_comma_separated_elementsTickid_0x1_72b43df88b54209c>>();
    };
    aotLib[0x1b5ca21ddefeaed3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_sub_element_innerTickid_0x1_3ffc02542601bf64>>();
    };
    aotLib[0x22a2acd672aa18a4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_sub_elementTickid_0x1_70d6c8e08871cfa1>>();
    };
    aotLib[0xb4c2a739642c99f3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_innerTickid_0x1_ab5d94b9909bbf99>>();
    };
    aotLib[0x3727205074188378] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_elementTickid_0x1_a4daaa8f79cfa9ed>>();
    };
    aotLib[0x9adb42ea346c1ccb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_class_name_innerTickid_0x1_8485552d58457ba0>>();
    };
    aotLib[0x1a28e9cf86366241] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_class_nameTickid_0x1_ea9ce584f39ea618>>();
    };
    aotLib[0x3811ddb5dccf7067] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_ident__innerTickid_0x1_2c1a7fef6a8c065e>>();
    };
    aotLib[0x6dda21b0062180f2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_ident_Tickid_0x1_ba14b1ca3a3dda32>>();
    };
    aotLib[0x347de45a6b0ec160] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_5f0e0f646d69bfc7>>();
    };
    aotLib[0x3f2acb2a54386d28] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&instance_args_f6e03da14c477f63>>();
    };
    aotLib[0x80eebf892ade5d71] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_e6aaad296469300e>>();
    };
    aotLib[0xcff4e7d94d6c9549] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_fd9d2e92ebaff162>>();
    };
    aotLib[0xfc0f6b743f8d9064] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_7eaba744dd27b4f>>();
    };
    aotLib[0xf2253ad5f604e861] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_4208deb8a0b826d6>>();
    };
    // [[ init script ]]
    aotLib[0x3821f79f7ca33ae3] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_1837871176362534164
AotListBase impl_aot_spoof(_anon_1837871176362534164::registerAotFunctions);
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
