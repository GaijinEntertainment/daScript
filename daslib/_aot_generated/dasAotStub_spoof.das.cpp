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
namespace _anon_10448736428990234716 {

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

VarInfo __struct_info__2040d68d100cd133_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x9a6e42f8b2fb2b62), "text", offsetof(peg::ParsingError,text), 2 };
VarInfo __struct_info__2040d68d100cd133_field_1 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x188fdd7d3dd85eb1), "index", offsetof(peg::ParsingError,index), 0 };
VarInfo * __struct_info__2040d68d100cd133_fields[2] =  { &__struct_info__2040d68d100cd133_field_0, &__struct_info__2040d68d100cd133_field_1 };
StructInfo __struct_info__2040d68d100cd133 = {"ParsingError", "peg", 8, __struct_info__2040d68d100cd133_fields, 2, 16, UINT64_C(0x0), nullptr, UINT64_C(0x2040d68d100cd133), 0 };
VarInfo __struct_info__c993e285a04c692f_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x5a9648338a3b70ee), "varName", offsetof(spoof::SpoofInvocation,varName), 1 };
VarInfo __struct_info__c993e285a04c692f_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x727ed3de16c14aef), "args", offsetof(spoof::SpoofInvocation,args), 2 };
VarInfo * __struct_info__c993e285a04c692f_fields[2] =  { &__struct_info__c993e285a04c692f_field_0, &__struct_info__c993e285a04c692f_field_1 };
StructInfo __struct_info__c993e285a04c692f = {"SpoofInvocation", "spoof", 28, __struct_info__c993e285a04c692f_fields, 2, 32, UINT64_C(0x0), nullptr, UINT64_C(0xc993e285a04c692f), 0 };
VarInfo __func_info__920fb1be533a8af4_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, 24, UINT64_C(0xc149136652fe1c57), "inp", 0, 0 };
VarInfo * __func_info__920fb1be533a8af4_fields[1] =  { &__func_info__920fb1be533a8af4_field_0 };
FuncInfo __func_info__920fb1be533a8af4 = {"invoke block<(inp:array<uint8> const#):void> const", "", __func_info__920fb1be533a8af4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x920fb1be533a8af4), 0x0 };
TypeInfo * __type_info__7d42bfacbd731b56_arg_types[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo __type_info__7d42bfacbd731b56 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__7d42bfacbd731b56_arg_types, nullptr, 2, 0, nullptr, 16646, 16, UINT64_C(0x7d42bfacbd731b56) };
TypeInfo * __type_info__71e1112747ff8db0_arg_types[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
TypeInfo __type_info__71e1112747ff8db0 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__71e1112747ff8db0_arg_types, nullptr, 2, 0, nullptr, 16390, 16, UINT64_C(0x71e1112747ff8db0) };
TypeInfo __type_info__8f14d918b6786dec = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__71e1112747ff8db0, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x8f14d918b6786dec) };
TypeInfo __type_info__12393804d99ce574 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x12393804d99ce574) };
TypeInfo __type_info__a49d33b75bf04f48 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__71e1112747ff8db0, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xa49d33b75bf04f48) };
TypeInfo __type_info__29a1dd55fa61e9e3 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__554236004213764e, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x29a1dd55fa61e9e3) };
TypeInfo __type_info__5eddda99b960ff80 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x5eddda99b960ff80) };
TypeInfo * __type_info__1bcaf53bc9823a03_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__8f14d918b6786dec, &__type_info__af63e44c8601fa24 };
const char * __type_info__1bcaf53bc9823a03_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__1bcaf53bc9823a03 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__1bcaf53bc9823a03_arg_types, __type_info__1bcaf53bc9823a03_arg_names, 3, 0, nullptr, 57346, 40, UINT64_C(0x1bcaf53bc9823a03) };
TypeInfo * __type_info__9293ad0d07b1dbee_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__12393804d99ce574, &__type_info__af63e44c8601fa24 };
const char * __type_info__9293ad0d07b1dbee_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__9293ad0d07b1dbee = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__9293ad0d07b1dbee_arg_types, __type_info__9293ad0d07b1dbee_arg_names, 3, 0, nullptr, 57346, 40, UINT64_C(0x9293ad0d07b1dbee) };
TypeInfo * __type_info__26703dd8492d36b8_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__243e11abf1d63530, &__type_info__af63e44c8601fa24 };
const char * __type_info__26703dd8492d36b8_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__26703dd8492d36b8 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__26703dd8492d36b8_arg_types, __type_info__26703dd8492d36b8_arg_names, 3, 0, nullptr, 57346, 48, UINT64_C(0x26703dd8492d36b8) };
TypeInfo __type_info__a9e49a99cab89d4 = { Type::tStructure, &__struct_info__c993e285a04c692f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 32, UINT64_C(0xa9e49a99cab89d4) };
TypeInfo __type_info__66686027eaf8675d = { Type::tStructure, &__struct_info__2040d68d100cd133, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16678, 16, UINT64_C(0x66686027eaf8675d) };
TypeInfo __type_info__fa593d0882a72913 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16548, 8, UINT64_C(0xfa593d0882a72913) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo * __type_info__6beb6823e5dcfa26_arg_types[2] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
const char * __type_info__6beb6823e5dcfa26_arg_names[2] = { "argName", "init" };
TypeInfo __type_info__6beb6823e5dcfa26 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__6beb6823e5dcfa26_arg_types, __type_info__6beb6823e5dcfa26_arg_names, 2, 0, nullptr, 16390, 16, UINT64_C(0x6beb6823e5dcfa26) };
TypeInfo __type_info__554236004213764e = { Type::tStructure, &__struct_info__2040d68d100cd133, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 16, UINT64_C(0x554236004213764e) };
TypeInfo __type_info__243e11abf1d63530 = { Type::tStructure, &__struct_info__c993e285a04c692f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 32, UINT64_C(0x243e11abf1d63530) };
TypeInfo __type_info__7775f165da7e23c2 = { Type::tStructure, &__struct_info__2040d68d100cd133, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 16, UINT64_C(0x7775f165da7e23c2) };
TypeInfo __type_info__6ad1eba4ab633b52 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__6beb6823e5dcfa26, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x6ad1eba4ab633b52) };
TypeInfo __type_info__9e830744959095a7 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__7775f165da7e23c2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x9e830744959095a7) };
TypeInfo * __type_info__9e169205ce7610ad_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__8f14d918b6786dec, &__type_info__af63e44c8601fa24 };
const char * __type_info__9e169205ce7610ad_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__9e169205ce7610ad = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__9e169205ce7610ad_arg_types, __type_info__9e169205ce7610ad_arg_names, 3, 0, nullptr, 57346, 40, UINT64_C(0x9e169205ce7610ad) };
TypeInfo * __type_info__7480a390cb0fd543_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__243e11abf1d63530, &__type_info__af63e44c8601fa24 };
const char * __type_info__7480a390cb0fd543_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__7480a390cb0fd543 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__7480a390cb0fd543_arg_types, __type_info__7480a390cb0fd543_arg_names, 3, 0, nullptr, 57346, 48, UINT64_C(0x7480a390cb0fd543) };
TypeInfo __type_info__b02bad0282fed564 = { Type::tStructure, &__struct_info__c993e285a04c692f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 32, UINT64_C(0xb02bad0282fed564) };
TypeInfo __type_info__b18f2b15460317a1 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__71e1112747ff8db0, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xb18f2b15460317a1) };
TypeInfo __type_info__df1b7db3f566d157 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__554236004213764e, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xdf1b7db3f566d157) };
TypeInfo * __type_info__35f44e4f93af85ad_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__8f14d918b6786dec, &__type_info__af63e44c8601fa24 };
const char * __type_info__35f44e4f93af85ad_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__35f44e4f93af85ad = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__35f44e4f93af85ad_arg_types, __type_info__35f44e4f93af85ad_arg_names, 3, 0, nullptr, 57346, 40, UINT64_C(0x35f44e4f93af85ad) };
TypeInfo * __type_info__ab24c91193d6fa43_arg_types[3] = { &__type_info__af63df4c8601f1a5, &__type_info__243e11abf1d63530, &__type_info__af63e44c8601fa24 };
const char * __type_info__ab24c91193d6fa43_arg_names[3] = { "success", "value", "endpos" };
TypeInfo __type_info__ab24c91193d6fa43 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__ab24c91193d6fa43_arg_types, __type_info__ab24c91193d6fa43_arg_names, 3, 0, nullptr, 57346, 48, UINT64_C(0xab24c91193d6fa43) };
TypeInfo __type_info__662f8256d51be64 = { Type::tStructure, &__struct_info__c993e285a04c692f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 32, UINT64_C(0x662f8256d51be64) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

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

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_a8fc758b39196373 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_1182_0 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> & __a_rename_at_1182_2 );
inline void finalize_7afa66130e8b5e54 ( Context * __context__, AutoTuple<char *,char *> & ____this_rename_at_1197_3 );
inline void finalize_6ece99d569f84073 ( Context * __context__, spoof::SpoofInvocation & ____this_rename_at_172_4 );
inline void finalize_2218e7aaad09471d ( Context * __context__, AutoTuple<bool,AutoTuple<char *,char *>,int32_t> & ____this_rename_at_1197_5 );
inline void finalize_45dfd694aadf378b ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & ____this_rename_at_1197_6 );
inline void finalize_cfe1ed3b98f2f17 ( Context * __context__, AutoTuple<bool,char *,int32_t> & ____this_rename_at_1197_7 );
inline void finalize_eddf182da4ded640 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & ____this_rename_at_1197_8 );
inline void finalize_78307bdac119941d ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> & ____this_rename_at_1197_9 );
inline void finalize_51b896d19d78efaa ( Context * __context__, peg::ParsingError & ____this_rename_at_18_10 );
inline Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_a373e563dce81dd7 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> & __a_rename_at_1150_11 );
inline Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_fca84cc9cd8fb0e4 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> & __a_rename_at_1150_13 );
inline Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_7358c6086ef94f68 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> & __a_rename_at_1150_15 );
inline Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_862595edb53225a1 ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> & __a_rename_at_1150_17 );
inline Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_e9039b88c532c9ff ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> & __a_rename_at_1150_19 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_21, int32_t __newSize_rename_at_68_22 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_68_23, int32_t __newSize_rename_at_68_24 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> & __Arr_rename_at_68_25, int32_t __newSize_rename_at_68_26 );
inline TArray<peg::ParsingError> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b0722452a1c28717 ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_50_27 );
inline void _FuncbuiltinTickpushTick10769833213962245646_8e4364e894fa22ef ( Context * __context__, TArray<ast::AstReaderMacro *> & __Arr_rename_at_181_28, ast::AstReaderMacro * __value_rename_at_181_29 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9bd0f12f34dd31a3 ( Context * __context__, spoof::SpoofTemplateReader const  & __cl_rename_at_116_30 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f2e4758bc132fae7 ( Context * __context__, spoof::SpoofInstanceReader const  & __cl_rename_at_116_31 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_9d118576d5107c28 ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_32_32, TArray<peg::ParsingError> & __b_rename_at_32_33 );
inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_d452bb76b753397e ( Context * __context__, TArray<peg::ParsingError> & __Arr_rename_at_377_34, peg::ParsingError & __value_rename_at_377_35 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> & __a_rename_at_1182_36 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_e5d8bd461b7dcb17 ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_1182_37 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_40a623f9c95b0cb9 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> & __a_rename_at_1193_39 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_53b9400c23ef0c99 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> & __a_rename_at_1193_41 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> & __a_rename_at_1193_43 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_8872455bc7f9c5ef ( Context * __context__, TArray<bool> & __a_rename_at_1182_45 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_84cf6af8e0041469 ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> & __a_rename_at_1193_46 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_5f01338060689947 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> & __a_rename_at_1193_48 );
inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_d582432df8818d6e ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> const  & __clone_src_rename_at_1056_50 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_f74cc0676a04cd8 ( Context * __context__, AutoTuple<bool,AutoTuple<char *,char *>,int32_t> const  & __clone_src_rename_at_1056_52 );
inline AutoTuple<bool,char *,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245 ( Context * __context__, AutoTuple<bool,char *,int32_t> const  & __clone_src_rename_at_1056_54 );
inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_9dc279ef3efd2937 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> const  & __clone_src_rename_at_1056_56 );
inline AutoTuple<bool,TArray<char *>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_3e547859692e768f ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> const  & __clone_src_rename_at_1056_58 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_a886d0bdf2ebbcc1 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1080_60, TArray<uint8_t> const  & __b_rename_at_1080_61 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_c57c3779e2d803ca ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_1080_63, TArray<AutoTuple<char *,char *>> const  & __b_rename_at_1080_64 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_fd34c08266227f18 ( Context * __context__, TArray<char *> & __a_rename_at_1080_70, TArray<char *> const  & __b_rename_at_1080_71 );
inline void _FuncpegTicktabulateTick3664919181896551618_5938a8f00640e7b0 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_59_77 );
inline char * _FunccolorsTickblue_strTick419715327456344553_ccac9a00db81a9f1 ( Context * __context__, char * const  __str_rename_at_26_78, bool __color_rename_at_26_79 );
inline char * _FunccolorsTickgreen_strTick14807051613123094884_df8dd133b2accbc3 ( Context * __context__, char * const  __str_rename_at_18_80, bool __color_rename_at_18_81 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_aa0264416ea4124a ( Context * __context__, TArray<bool> & __Arr_rename_at_68_82, int32_t __newSize_rename_at_68_83 );
inline bool _FuncpegTickreached_EOFTick12193995001429396764_74c5ca3a73c6ab1f ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_83_84 );
inline void _FuncpegTicktabulateTick3664919181896551618_52ff143bf46df83f ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_59_85 );
inline bool _FuncpegTickreached_EOFTick12193995001429396764_76116504adfe2552 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_83_86 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_8d8767d901055902 ( Context * __context__, TArray<peg::ParsingError> const  & __a_rename_at_585_87 );
inline void _FuncbuiltinTicksortTick14088969635007481283_2b1d2e9c3495b624 ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_1517_88 );
inline TArray<peg::ParsingError> _FuncalgorithmTickuniqueTick8642070526899511001_97dad40955268394 ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_11_91 );
inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ba4ceaf27ff6ff46 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & __a_rename_at_50_98 );
inline TArray<AutoTuple<char *,char *>> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_7ecb0a5d4721f3ce ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_50_99 );
inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e53981a5c0b0dc1 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & __a_rename_at_50_100 );
inline spoof::SpoofInvocation & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8860640a3da483d ( Context * __context__, spoof::SpoofInvocation & __a_rename_at_50_101 );
inline AutoTuple<bool,TArray<char *>,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a887f80fd036fc6a ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> & __a_rename_at_50_102 );
inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_32af3ec8801e47ea ( Context * __context__, TArray<char *> & __a_rename_at_50_103 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_2afaf9cdb69403d4 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & __a_rename_at_32_104, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & __b_rename_at_32_105 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_74d3e26b89ca39c1 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_32_106, TArray<AutoTuple<char *,char *>> & __b_rename_at_32_107 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_721189fff44b0b7c ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & __a_rename_at_32_108, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & __b_rename_at_32_109 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_435b13646b15cb2c ( Context * __context__, spoof::SpoofInvocation & __a_rename_at_32_110, spoof::SpoofInvocation & __b_rename_at_32_111 );
inline void clone_c94fb40a98d27d95 ( Context * __context__, smart_ptr_raw<Variable> & __dest_rename_at_266_112, smart_ptr_raw<Variable> const  __src_rename_at_266_113 );
inline void _FuncpegTicklog_infoTick4618819779667933862_79989224586a0f97 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_78_114, char * __message_rename_at_78_115 );
inline void _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_91_116 );
inline void _FuncpegTicklog_plainTick18061110541707930728_22d44fb5d6ba6b83 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_63_117, char * __message_rename_at_63_118 );
inline bool _FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_34_119, char * const  __template__rename_at_34_120, int32_t __strlen_rename_at_34_121 );
inline void _FuncpegTicklog_failTick11731645277795022368_b3bcabd40e85a2f6 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_68_123, char * __message_rename_at_68_124 );
inline bool & _FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1 ( Context * __context__, TArray<bool> & __a_rename_at_473_125 );
inline char * _FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584 ( Context * __context__, char * const  __str_rename_at_14_127, bool __color_rename_at_14_128 );
inline void _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b ( Context * __context__, TArray<peg::ParsingError> & __Arr_rename_at_181_129, peg::ParsingError & __value_rename_at_181_130 );
inline void _FuncpegTicklog_successTick3645718276917011819_48df15f72c464176 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_73_131, char * __message_rename_at_73_132 );
inline void _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79 ( Context * __context__, TArray<bool> & __Arr_rename_at_165_133, bool __value_rename_at_165_134 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_ca5d53fe6f76d114 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_287_135, AutoTuple<char *,char *> & __value_rename_at_287_136 );
inline void _FuncbuiltinTickpopTick1161079256290593740_da418e66c1d8244e ( Context * __context__, TArray<bool> & __Arr_rename_at_132_137 );
inline void _FuncbuiltinTickpushTick10769833213962245646_39183d2b340e1e79 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_181_138, AutoTuple<char *,char *> & __value_rename_at_181_139 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_57ae76d5fb6c64b9 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> const  & __Tab_rename_at_1014_140, int32_t __at_rename_at_1014_141 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_61012b8bb2fce779 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> const  & __Tab_rename_at_1014_142, int32_t __at_rename_at_1014_143 );
inline int32_t _FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_103_144 );
inline void _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_54_145, int32_t __offset_rename_at_54_146 );
inline AutoTuple<bool,char *> _FuncpegTickmatch_string_literalTick12800953725978677998_f7109d6e87737ab6 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_144_147 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> const  & __Tab_rename_at_1014_150, int32_t __at_rename_at_1014_151 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_664b3b92dd33b520 ( Context * __context__, TArray<char *> & __Arr_rename_at_287_152, char * & __value_rename_at_287_153 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_597abbecf5b26d8 ( Context * __context__, TArray<char *> const  & __it_rename_at_22_154, char * const  __separator_rename_at_22_155 );
inline void _FuncpegTicklog_infoTick4618819779667933862_efdc190cc878d369 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_78_160, char * __message_rename_at_78_161 );
inline void _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_91_162 );
inline void _FuncpegTicklog_plainTick18061110541707930728_8c201494798c4721 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_63_163, char * __message_rename_at_63_164 );
inline bool _FuncpegTickmatchesTick4020677071890555356_83d30930705988a2 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_34_165, char * const  __template__rename_at_34_166, int32_t __strlen_rename_at_34_167 );
inline void _FuncpegTicklog_failTick11731645277795022368_ae7aad9b0b952b34 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_68_169, char * __message_rename_at_68_170 );
inline void _FuncpegTicklog_successTick3645718276917011819_237573808f7a2c5c ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_73_171, char * __message_rename_at_73_172 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_33cfc90a36a139ab ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> const  & __Tab_rename_at_1014_173, int32_t __at_rename_at_1014_174 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_47edd36ebc80950e ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> const  & __Tab_rename_at_1014_175, int32_t __at_rename_at_1014_176 );
inline int32_t _FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_103_177 );
inline void _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_54_178, int32_t __offset_rename_at_54_179 );
inline AutoTuple<bool,char *> _FuncpegTickmatch_string_literalTick12800953725978677998_f6b931b539266e97 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_144_180 );
inline void _FuncalgorithmTicksort_uniqueTick14985796887508933377_fd064d4ca86f9c39 ( Context * __context__, TArray<peg::ParsingError> & __a_rename_at_24_183 );
inline void finalize_b0cebf59e5e51d47 ( Context * __context__, spoof::invocationParserTickid_0x0 & ____this_rename_at_0_185 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_8fb28bab1278e8c2 ( Context * __context__, TArray<peg::ParsingError> const  & __it_rename_at_22_186, char * const  __separator_rename_at_22_187 );
inline void finalize_bf2f98006be610c6 ( Context * __context__, spoof::invocationParserTickid_0x1 & ____this_rename_at_0_192 );
inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _Funcparse_element_list_innerTickid_0x0_1c48b25484abe536 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_193 );
inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _Funcparse_element_listTickid_0x0_d172783e9d6cc855 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_208 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_comma_separated_elements_innerTickid_0x0_7f14badd440f98f ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_212 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_comma_separated_elementsTickid_0x0_84b92b09aca2a83b ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_221 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_element_innerTickid_0x0_70aae0654fd76c21 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_225 );
inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_elementTickid_0x0_85965a755893ca3 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_242 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_element_innerTickid_0x0_4d49965cb6bc1ab8 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_246 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_elementTickid_0x0_c8a6c75b473a3132 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_299 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_element_value_innerTickid_0x0_62f6fb32b8e5e998 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_303 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_element_valueTickid_0x0_4e09996913410390 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_314 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_ident__innerTickid_0x0_79268295f94867d7 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_362_318 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_ident_Tickid_0x0_20b6fcdb44c1b1c0 ( Context * __context__, spoof::invocationParserTickid_0x0 & __parser_rename_at_146_338 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __it_rename_at_1223_342, int32_t & __value_rename_at_1223_343 );
inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_344, uint8_t __value_rename_at_165_345 );
inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> & __Arr_rename_at_165_346, char * const  __value_rename_at_165_347 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1 ( Context * __context__, TArray<char *> & __Arr_rename_at_181_348, char * __value_rename_at_181_349 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5 ( Context * __context__, char * const  __str_rename_at_1256_350 );
inline int32_t _FuncbuiltinTickfind_index_ifTick5144745413123287381_d7241f91c631a1ce ( Context * __context__, TArray<AutoTuple<char *,char *>> const  & __arr_rename_at_1676_352, Block DAS_COMMENT((bool,AutoTuple<char *,char *> const )) const  & __blk_rename_at_1676_353 );
inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _Funcparse_macro_invocation_innerTickid_0x1_fc3224332b02420d ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_355 );
inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _Funcparse_macro_invocationTickid_0x1_e41312d760106bd0 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_378 );
inline AutoTuple<bool,TArray<char *>,int32_t> _Funcparse_element_list_innerTickid_0x1_4ddd8d5780e55324 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_382 );
inline AutoTuple<bool,TArray<char *>,int32_t> _Funcparse_element_listTickid_0x1_f9f95d60cca895e9 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_393 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_comma_separated_elements_innerTickid_0x1_4a764230365b1982 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_397 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_comma_separated_elementsTickid_0x1_a5eb4f00d7b7be ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_406 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_element_innerTickid_0x1_cb2b3527b22dde21 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_410 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_elementTickid_0x1_9dd0aa4f6e723d42 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_463 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_element_innerTickid_0x1_82f0e6f9b68f3345 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_467 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_elementTickid_0x1_609c6e555a4e4af4 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_478 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_class_name_innerTickid_0x1_151c0b46e0f35deb ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_482 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_class_nameTickid_0x1_ecd2c3e791924e42 ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_503 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_ident__innerTickid_0x1_391c4c7b7867934a ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_362_507 );
inline AutoTuple<bool,char *,int32_t> _Funcparse_ident_Tickid_0x1_5767df53d96981e ( Context * __context__, spoof::invocationParserTickid_0x1 & __parser_rename_at_146_527 );
inline Program * _FuncbuiltinTickget_ptrTick8468476673553620226_ae31c60b8d8beb31 ( Context * __context__, smart_ptr_raw<Program> const  __src_rename_at_1726_531 );
inline void instance_args_50995e25354ef5d2 ( Context * __context__, char * const  __invocation_rename_at_177_532, Block DAS_COMMENT((void,spoof::SpoofInvocation,TArray<peg::ParsingError> const )) const  & __blk_rename_at_177_533 );
inline void clone_1d0149763c60891a ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & __dest_rename_at_170_539, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> const  & __src_rename_at_170_540 );
inline void clone_33841e01eec4fe3e ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> & __dest_rename_at_170_541, AutoTuple<bool,spoof::SpoofInvocation,int32_t> const  & __src_rename_at_170_542 );
inline void clone_2562f74cf119ba95 ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> & __dest_rename_at_170_543, AutoTuple<bool,TArray<char *>,int32_t> const  & __src_rename_at_170_544 );
inline void clone_ae0e620d108403a2 ( Context * __context__, spoof::SpoofInvocation & __a_rename_at_172_545, spoof::SpoofInvocation const  & __b_rename_at_172_546 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0x68d557c9065bdca7>(__context__) = 37;/*SPOOF_SPEFICAL*/
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_a8fc758b39196373 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_1182_0 )
{
    {
        bool __need_loop_1184 = true;
        // aV: tuple<string;string> aka TT&
        das_iterator<TArray<AutoTuple<char *,char *>>> __aV_iterator(__a_rename_at_1182_0);
        AutoTuple<char *,char *> * __aV_rename_at_1184_1;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_1)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_1)) )
        {
            finalize_7afa66130e8b5e54(__context__,das_arg<AutoTuple<char *,char *>>::pass((*__aV_rename_at_1184_1)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_1));
    };
    builtin_array_free(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__a_rename_at_1182_0),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> &  __a_rename_at_1182_2 )
{
    builtin_array_free(das_arg<TArray<char *>>::pass(__a_rename_at_1182_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_7afa66130e8b5e54 ( Context * __context__, AutoTuple<char *,char *> &  ____this_rename_at_1197_3 )
{
    memset((void*)&(____this_rename_at_1197_3), 0, TypeSize<AutoTuple<char *,char *>>::size);
}

inline void finalize_6ece99d569f84073 ( Context * __context__, spoof::SpoofInvocation &  ____this_rename_at_172_4 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(____this_rename_at_172_4.args));
    memset((void*)&(____this_rename_at_172_4), 0, TypeSize<spoof::SpoofInvocation>::size);
}

inline void finalize_2218e7aaad09471d ( Context * __context__, AutoTuple<bool,AutoTuple<char *,char *>,int32_t> &  ____this_rename_at_1197_5 )
{
    finalize_7afa66130e8b5e54(__context__,das_arg<AutoTuple<char *,char *>>::pass(das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(____this_rename_at_1197_5)));
    memset((void*)&(____this_rename_at_1197_5), 0, TypeSize<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::size);
}

inline void finalize_45dfd694aadf378b ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  ____this_rename_at_1197_6 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_a8fc758b39196373(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(das_get_auto_tuple_field<TArray<AutoTuple<char *,char *>>,1,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(____this_rename_at_1197_6)));
    memset((void*)&(____this_rename_at_1197_6), 0, TypeSize<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::size);
}

inline void finalize_cfe1ed3b98f2f17 ( Context * __context__, AutoTuple<bool,char *,int32_t> &  ____this_rename_at_1197_7 )
{
    memset((void*)&(____this_rename_at_1197_7), 0, TypeSize<AutoTuple<bool,char *,int32_t>>::size);
}

inline void finalize_eddf182da4ded640 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  ____this_rename_at_1197_8 )
{
    finalize_6ece99d569f84073(__context__,das_arg<spoof::SpoofInvocation>::pass(das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(____this_rename_at_1197_8)));
    memset((void*)&(____this_rename_at_1197_8), 0, TypeSize<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::size);
}

inline void finalize_78307bdac119941d ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> &  ____this_rename_at_1197_9 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(____this_rename_at_1197_9)));
    memset((void*)&(____this_rename_at_1197_9), 0, TypeSize<AutoTuple<bool,TArray<char *>,int32_t>>::size);
}

inline void finalize_51b896d19d78efaa ( Context * __context__, peg::ParsingError &  ____this_rename_at_18_10 )
{
    memset((void*)&(____this_rename_at_18_10), 0, TypeSize<peg::ParsingError>::size);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_a373e563dce81dd7 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> &  __a_rename_at_1150_11 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> *)) __it_rename_at_1151_12;das_zero(__it_rename_at_1151_12);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t>))>::pass(__it_rename_at_1151_12),das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__a_rename_at_1150_11),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> &))>::cast(__it_rename_at_1151_12);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_fca84cc9cd8fb0e4 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> &  __a_rename_at_1150_13 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> *)) __it_rename_at_1151_14;das_zero(__it_rename_at_1151_14);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>))>::pass(__it_rename_at_1151_14),das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(__a_rename_at_1150_13),40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &))>::cast(__it_rename_at_1151_14);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_7358c6086ef94f68 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> &  __a_rename_at_1150_15 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> *)) __it_rename_at_1151_16;das_zero(__it_rename_at_1151_16);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t>))>::pass(__it_rename_at_1151_16),das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__a_rename_at_1150_15),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> &))>::cast(__it_rename_at_1151_16);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_862595edb53225a1 ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> &  __a_rename_at_1150_17 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> *)) __it_rename_at_1151_18;das_zero(__it_rename_at_1151_18);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t>))>::pass(__it_rename_at_1151_18),das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(__a_rename_at_1150_17),48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> &))>::cast(__it_rename_at_1151_18);
}

inline Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> &)) _FuncbuiltinTickvaluesTick1351216622833168869_e9039b88c532c9ff ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> &  __a_rename_at_1150_19 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> *)) __it_rename_at_1151_20;das_zero(__it_rename_at_1151_20);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t>))>::pass(__it_rename_at_1151_20),das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(__a_rename_at_1150_19),40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> &))>::cast(__it_rename_at_1151_20);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_21, int32_t __newSize_rename_at_68_22 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_21),__newSize_rename_at_68_22,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_68_23, int32_t __newSize_rename_at_68_24 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_68_23),__newSize_rename_at_68_24,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_68_25, int32_t __newSize_rename_at_68_26 )
{
    builtin_array_resize(das_arg<TArray<char *>>::pass(__Arr_rename_at_68_25),__newSize_rename_at_68_26,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<peg::ParsingError> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b0722452a1c28717 ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_50_27 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<peg::ParsingError> &>::from(__a_rename_at_50_27)));
    return das_auto_cast_ref<TArray<peg::ParsingError> &>::cast(__a_rename_at_50_27);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_8e4364e894fa22ef ( Context * __context__, TArray<ast::AstReaderMacro *> &  __Arr_rename_at_181_28, ast::AstReaderMacro * __value_rename_at_181_29 )
{
    das_copy(__Arr_rename_at_181_28(builtin_array_push_back(das_arg<TArray<ast::AstReaderMacro *>>::pass(__Arr_rename_at_181_28),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_29);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9bd0f12f34dd31a3 ( Context * __context__, spoof::SpoofTemplateReader const  &  __cl_rename_at_116_30 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_30.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f2e4758bc132fae7 ( Context * __context__, spoof::SpoofInstanceReader const  &  __cl_rename_at_116_31 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_31.__rtti))).getStructType())));
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_9d118576d5107c28 ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_32_32, TArray<peg::ParsingError> &  __b_rename_at_32_33 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TArray<peg::ParsingError> &>::from(__a_rename_at_32_32)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_2,cast<TArray<peg::ParsingError> &>::from(__b_rename_at_32_33)));
    das_move(__a_rename_at_32_32,__b_rename_at_32_33);
}

inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_d452bb76b753397e ( Context * __context__, TArray<peg::ParsingError> &  __Arr_rename_at_377_34, peg::ParsingError &  __value_rename_at_377_35 )
{
    das_copy(__Arr_rename_at_377_34(builtin_array_push_back_zero(das_arg<TArray<peg::ParsingError>>::pass(__Arr_rename_at_377_34),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_377_35);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1182_36 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1182_36),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_e5d8bd461b7dcb17 ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_1182_37 )
{
    {
        bool __need_loop_1184 = true;
        // aV: peg::ParsingError aka TT&
        das_iterator<TArray<peg::ParsingError>> __aV_iterator(__a_rename_at_1182_37);
        peg::ParsingError * __aV_rename_at_1184_38;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_38)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_38)) )
        {
            finalize_51b896d19d78efaa(__context__,das_arg<peg::ParsingError>::pass((*__aV_rename_at_1184_38)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_38));
    };
    builtin_array_free(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_1182_37),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_40a623f9c95b0cb9 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> &  __a_rename_at_1193_39 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t> *)) _temp_make_local_1195_19_32; _temp_make_local_1195_19_32;
    {
        bool __need_loop_1195 = true;
        // aV: tuple<success:bool;value:tuple<string;string> -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,AutoTuple<char *,char *>,int32_t>))> __aV_iterator((_temp_make_local_1195_19_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_a373e563dce81dd7(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__a_rename_at_1193_39)))));
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> * __aV_rename_at_1195_40;
        __need_loop_1195 = __aV_iterator.first(__context__,(__aV_rename_at_1195_40)) && __need_loop_1195;
        for ( ; __need_loop_1195 ; __need_loop_1195 = __aV_iterator.next(__context__,(__aV_rename_at_1195_40)) )
        {
            finalize_2218e7aaad09471d(__context__,das_arg<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::pass((*__aV_rename_at_1195_40)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1195_40));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__a_rename_at_1193_39),4,32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_53b9400c23ef0c99 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> &  __a_rename_at_1193_41 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> *)) _temp_make_local_1195_19_32; _temp_make_local_1195_19_32;
    {
        bool __need_loop_1195 = true;
        // aV: tuple<success:bool;value:array<tuple<string;string>> -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>))> __aV_iterator((_temp_make_local_1195_19_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_fca84cc9cd8fb0e4(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(__a_rename_at_1193_41)))));
        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> * __aV_rename_at_1195_42;
        __need_loop_1195 = __aV_iterator.first(__context__,(__aV_rename_at_1195_42)) && __need_loop_1195;
        for ( ; __need_loop_1195 ; __need_loop_1195 = __aV_iterator.next(__context__,(__aV_rename_at_1195_42)) )
        {
            finalize_45dfd694aadf378b(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass((*__aV_rename_at_1195_42)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1195_42));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(__a_rename_at_1193_41),4,40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> &  __a_rename_at_1193_43 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t> *)) _temp_make_local_1195_19_32; _temp_make_local_1195_19_32;
    {
        bool __need_loop_1195 = true;
        // aV: tuple<success:bool;value:string -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,char *,int32_t>))> __aV_iterator((_temp_make_local_1195_19_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_7358c6086ef94f68(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__a_rename_at_1193_43)))));
        AutoTuple<bool,char *,int32_t> * __aV_rename_at_1195_44;
        __need_loop_1195 = __aV_iterator.first(__context__,(__aV_rename_at_1195_44)) && __need_loop_1195;
        for ( ; __need_loop_1195 ; __need_loop_1195 = __aV_iterator.next(__context__,(__aV_rename_at_1195_44)) )
        {
            finalize_cfe1ed3b98f2f17(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass((*__aV_rename_at_1195_44)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1195_44));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__a_rename_at_1193_43),4,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_8872455bc7f9c5ef ( Context * __context__, TArray<bool> &  __a_rename_at_1182_45 )
{
    builtin_array_free(das_arg<TArray<bool>>::pass(__a_rename_at_1182_45),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_84cf6af8e0041469 ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> &  __a_rename_at_1193_46 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t> *)) _temp_make_local_1195_19_32; _temp_make_local_1195_19_32;
    {
        bool __need_loop_1195 = true;
        // aV: tuple<success:bool;value:spoof::SpoofInvocation -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,spoof::SpoofInvocation,int32_t>))> __aV_iterator((_temp_make_local_1195_19_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_862595edb53225a1(__context__,das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(__a_rename_at_1193_46)))));
        AutoTuple<bool,spoof::SpoofInvocation,int32_t> * __aV_rename_at_1195_47;
        __need_loop_1195 = __aV_iterator.first(__context__,(__aV_rename_at_1195_47)) && __need_loop_1195;
        for ( ; __need_loop_1195 ; __need_loop_1195 = __aV_iterator.next(__context__,(__aV_rename_at_1195_47)) )
        {
            finalize_eddf182da4ded640(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass((*__aV_rename_at_1195_47)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1195_47));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(__a_rename_at_1193_46),4,48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_5f01338060689947 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> &  __a_rename_at_1193_48 )
{
    Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t> *)) _temp_make_local_1195_19_32; _temp_make_local_1195_19_32;
    {
        bool __need_loop_1195 = true;
        // aV: tuple<success:bool;value:array<string> -const;endpos:int>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<bool,TArray<char *>,int32_t>))> __aV_iterator((_temp_make_local_1195_19_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_e9039b88c532c9ff(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(__a_rename_at_1193_48)))));
        AutoTuple<bool,TArray<char *>,int32_t> * __aV_rename_at_1195_49;
        __need_loop_1195 = __aV_iterator.first(__context__,(__aV_rename_at_1195_49)) && __need_loop_1195;
        for ( ; __need_loop_1195 ; __need_loop_1195 = __aV_iterator.next(__context__,(__aV_rename_at_1195_49)) )
        {
            finalize_78307bdac119941d(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass((*__aV_rename_at_1195_49)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1195_49));
    };
    builtin_table_free(das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(__a_rename_at_1193_48),4,40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_d582432df8818d6e ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> const  &  __clone_src_rename_at_1056_50 )
{
    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __clone_dest_rename_at_1058_51;das_zero(__clone_dest_rename_at_1058_51);
    clone_1d0149763c60891a(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass(__clone_dest_rename_at_1058_51),__clone_src_rename_at_1056_50);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(__clone_dest_rename_at_1058_51);
}

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_f74cc0676a04cd8 ( Context * __context__, AutoTuple<bool,AutoTuple<char *,char *>,int32_t> const  &  __clone_src_rename_at_1056_52 )
{
    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __clone_dest_rename_at_1058_53;das_zero(__clone_dest_rename_at_1058_53);
    das_copy(__clone_dest_rename_at_1058_53,__clone_src_rename_at_1056_52);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__clone_dest_rename_at_1058_53);
}

inline AutoTuple<bool,char *,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245 ( Context * __context__, AutoTuple<bool,char *,int32_t> const  &  __clone_src_rename_at_1056_54 )
{
    AutoTuple<bool,char *,int32_t> __clone_dest_rename_at_1058_55;das_zero(__clone_dest_rename_at_1058_55);
    das_copy(__clone_dest_rename_at_1058_55,__clone_src_rename_at_1056_54);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__clone_dest_rename_at_1058_55);
}

inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_9dc279ef3efd2937 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> const  &  __clone_src_rename_at_1056_56 )
{
    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __clone_dest_rename_at_1058_57;das_zero(__clone_dest_rename_at_1058_57);
    clone_33841e01eec4fe3e(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__clone_dest_rename_at_1058_57),__clone_src_rename_at_1056_56);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__clone_dest_rename_at_1058_57);
}

inline AutoTuple<bool,TArray<char *>,int32_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_3e547859692e768f ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> const  &  __clone_src_rename_at_1056_58 )
{
    AutoTuple<bool,TArray<char *>,int32_t> __clone_dest_rename_at_1058_59;das_zero(__clone_dest_rename_at_1058_59);
    clone_2562f74cf119ba95(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass(__clone_dest_rename_at_1058_59),__clone_src_rename_at_1056_58);
    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(__clone_dest_rename_at_1058_59);
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_a886d0bdf2ebbcc1 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1080_60, TArray<uint8_t> const  &  __b_rename_at_1080_61 )
{
    int32_t __ln_rename_at_1081_62 = ((int32_t)builtin_array_size(__b_rename_at_1080_61));
    _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176(__context__,das_arg<TArray<uint8_t>>::pass(__a_rename_at_1080_60),__ln_rename_at_1081_62);
    if ( __ln_rename_at_1081_62 == 0 )
    {
        return ;
    } else {
        das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__a_rename_at_1080_60(0,__context__))),das_auto_cast<void * const >::cast(das_ref(__context__,__b_rename_at_1080_61(0,__context__))),__ln_rename_at_1081_62 * 1);
    };
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_c57c3779e2d803ca ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_1080_63, TArray<AutoTuple<char *,char *>> const  &  __b_rename_at_1080_64 )
{
    int32_t __ln_rename_at_1081_65 = ((int32_t)builtin_array_size(__b_rename_at_1080_64));
    _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__a_rename_at_1080_63),__ln_rename_at_1081_65);
    if ( __ln_rename_at_1081_65 == 0 )
    {
        return ;
    } else {
        {
            bool __need_loop_1091 = true;
            // aV: tuple<string;string> aka TT&
            das_iterator<TArray<AutoTuple<char *,char *>>> __aV_iterator(__a_rename_at_1080_63);
            AutoTuple<char *,char *> * __aV_rename_at_1091_68;
            __need_loop_1091 = __aV_iterator.first(__context__,(__aV_rename_at_1091_68)) && __need_loop_1091;
            // bV: tuple<string;string> const&
            das_iterator<TArray<AutoTuple<char *,char *>> const > __bV_iterator(__b_rename_at_1080_64);
            AutoTuple<char *,char *> const  * __bV_rename_at_1091_69;
            __need_loop_1091 = __bV_iterator.first(__context__,(__bV_rename_at_1091_69)) && __need_loop_1091;
            for ( ; __need_loop_1091 ; __need_loop_1091 = __aV_iterator.next(__context__,(__aV_rename_at_1091_68)) && __bV_iterator.next(__context__,(__bV_rename_at_1091_69)) )
            {
                das_copy((*__aV_rename_at_1091_68),(*__bV_rename_at_1091_69));
            }
            __aV_iterator.close(__context__,(__aV_rename_at_1091_68));
            __bV_iterator.close(__context__,(__bV_rename_at_1091_69));
        };
    };
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_fd34c08266227f18 ( Context * __context__, TArray<char *> &  __a_rename_at_1080_70, TArray<char *> const  &  __b_rename_at_1080_71 )
{
    int32_t __ln_rename_at_1081_72 = ((int32_t)builtin_array_size(__b_rename_at_1080_71));
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__a_rename_at_1080_70),__ln_rename_at_1081_72);
    if ( __ln_rename_at_1081_72 == 0 )
    {
        return ;
    } else {
        {
            bool __need_loop_1091 = true;
            // aV: string aka TT&
            das_iterator<TArray<char *>> __aV_iterator(__a_rename_at_1080_70);
            char * * __aV_rename_at_1091_75;
            __need_loop_1091 = __aV_iterator.first(__context__,(__aV_rename_at_1091_75)) && __need_loop_1091;
            // bV: string const&
            das_iterator<TArray<char *> const > __bV_iterator(__b_rename_at_1080_71);
            char * const  * __bV_rename_at_1091_76;
            __need_loop_1091 = __bV_iterator.first(__context__,(__bV_rename_at_1091_76)) && __need_loop_1091;
            for ( ; __need_loop_1091 ; __need_loop_1091 = __aV_iterator.next(__context__,(__aV_rename_at_1091_75)) && __bV_iterator.next(__context__,(__bV_rename_at_1091_76)) )
            {
                das_copy((*__aV_rename_at_1091_75),(*__bV_rename_at_1091_76));
            }
            __aV_iterator.close(__context__,(__aV_rename_at_1091_75));
            __bV_iterator.close(__context__,(__bV_rename_at_1091_76));
        };
    };
}

inline void _FuncpegTicktabulateTick3664919181896551618_5938a8f00640e7b0 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_59_77 )
{
    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_3, cast<char * const >::from(((char * const )(string_repeat(((char *) "|   "),__parser_rename_at_59_77.tabs,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FunccolorsTickblue_strTick419715327456344553_ccac9a00db81a9f1 ( Context * __context__, char * const  __str_rename_at_26_78, bool __color_rename_at_26_79 )
{
    return das_auto_cast<char *>::cast(__color_rename_at_26_79 ? das_auto_cast<char * const >::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_4, cast<char * const >::from(((char *) "\u001b[34m")), cast<char * const >::from(__str_rename_at_26_78), cast<char * const >::from(((char *) "\u001b[0m"))))) : das_auto_cast<char * const >::cast(__str_rename_at_26_78));
}

inline char * _FunccolorsTickgreen_strTick14807051613123094884_df8dd133b2accbc3 ( Context * __context__, char * const  __str_rename_at_18_80, bool __color_rename_at_18_81 )
{
    return das_auto_cast<char *>::cast(__color_rename_at_18_81 ? das_auto_cast<char * const >::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_5, cast<char * const >::from(((char *) "\u001b[32m")), cast<char * const >::from(__str_rename_at_18_80), cast<char * const >::from(((char *) "\u001b[0m"))))) : das_auto_cast<char * const >::cast(__str_rename_at_18_80));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_aa0264416ea4124a ( Context * __context__, TArray<bool> &  __Arr_rename_at_68_82, int32_t __newSize_rename_at_68_83 )
{
    builtin_array_resize(das_arg<TArray<bool>>::pass(__Arr_rename_at_68_82),__newSize_rename_at_68_83,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncpegTickreached_EOFTick12193995001429396764_74c5ca3a73c6ab1f ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_83_84 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_83_84.input)) <= __parser_rename_at_83_84.index);
}

inline void _FuncpegTicktabulateTick3664919181896551618_52ff143bf46df83f ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_59_85 )
{
    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_6, cast<char * const >::from(((char * const )(string_repeat(((char *) "|   "),__parser_rename_at_59_85.tabs,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncpegTickreached_EOFTick12193995001429396764_76116504adfe2552 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_83_86 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_83_86.input)) <= __parser_rename_at_83_86.index);
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_8d8767d901055902 ( Context * __context__, TArray<peg::ParsingError> const  &  __a_rename_at_585_87 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_585_87) == 0);
}

inline void _FuncbuiltinTicksortTick14088969635007481283_2b1d2e9c3495b624 ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_1517_88 )
{
    if ( builtin_array_size(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_1517_88)) <= 1 )
    {
        return ;
    } else {
        builtin_sort_array_any_cblock_T(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_1517_88),16,builtin_array_size(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_1517_88)),[&](peg::ParsingError const  & __x_rename_at_1538_89, peg::ParsingError const  & __y_rename_at_1538_90) DAS_AOT_INLINE_LAMBDA -> bool{
            return das_auto_cast<bool>::cast(das_invoke_function<bool>::invoke<peg::ParsingError const  &,peg::ParsingError const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@peg::< CS<peg::ParsingError> CS<peg::ParsingError>*/ 0x1448f6afa9f5f32e)),__x_rename_at_1538_89,__y_rename_at_1538_90));
        },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline TArray<peg::ParsingError> _FuncalgorithmTickuniqueTick8642070526899511001_97dad40955268394 ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_11_91 )
{
    int32_t __pidx_rename_at_13_92 = -1;
    TArray<peg::ParsingError> __b_rename_at_14_93;das_zero(__b_rename_at_14_93);
    {
        bool __need_loop_15 = true;
        // e: peg::ParsingError aka TT&
        das_iterator<TArray<peg::ParsingError>> __e_iterator(__a_rename_at_11_91);
        peg::ParsingError * __e_rename_at_15_96;
        __need_loop_15 = __e_iterator.first(__context__,(__e_rename_at_15_96)) && __need_loop_15;
        // eidx: int const
        das_iterator<range> __eidx_iterator(mk_range(builtin_array_size(das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_11_91))));
        int32_t __eidx_rename_at_15_97;
        __need_loop_15 = __eidx_iterator.first(__context__,(__eidx_rename_at_15_97)) && __need_loop_15;
        for ( ; __need_loop_15 ; __need_loop_15 = __e_iterator.next(__context__,(__e_rename_at_15_96)) && __eidx_iterator.next(__context__,(__eidx_rename_at_15_97)) )
        {
            if ( (__pidx_rename_at_13_92 == -1) || (das_invoke_function<bool>::invoke<peg::ParsingError const  &,peg::ParsingError const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@peg::!= CS<peg::ParsingError> CS<peg::ParsingError>*/ 0xe34ed31c8ed3fe95)),das_arg<peg::ParsingError>::pass(__a_rename_at_11_91(__pidx_rename_at_13_92,__context__)),das_arg<peg::ParsingError>::pass((*__e_rename_at_15_96)))) )
            {
                das_copy(__pidx_rename_at_13_92,__eidx_rename_at_15_97);
                _FuncbuiltinTickpush_cloneTick2035469273396957942_d452bb76b753397e(__context__,das_arg<TArray<peg::ParsingError>>::pass(__b_rename_at_14_93),das_arg<peg::ParsingError>::pass((*__e_rename_at_15_96)));
            };
        }
        __e_iterator.close(__context__,(__e_rename_at_15_96));
        __eidx_iterator.close(__context__,(__eidx_rename_at_15_97));
    };
    return /* <- */ das_auto_cast_move<TArray<peg::ParsingError>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b0722452a1c28717(__context__,das_arg<TArray<peg::ParsingError>>::pass(__b_rename_at_14_93)));
}

inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ba4ceaf27ff6ff46 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  __a_rename_at_50_98 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_7,cast<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &>::from(__a_rename_at_50_98)));
    return das_auto_cast_ref<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &>::cast(__a_rename_at_50_98);
}

inline TArray<AutoTuple<char *,char *>> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_7ecb0a5d4721f3ce ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_50_99 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_8,cast<TArray<AutoTuple<char *,char *>> &>::from(__a_rename_at_50_99)));
    return das_auto_cast_ref<TArray<AutoTuple<char *,char *>> &>::cast(__a_rename_at_50_99);
}

inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e53981a5c0b0dc1 ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  __a_rename_at_50_100 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_9,cast<AutoTuple<bool,spoof::SpoofInvocation,int32_t> &>::from(__a_rename_at_50_100)));
    return das_auto_cast_ref<AutoTuple<bool,spoof::SpoofInvocation,int32_t> &>::cast(__a_rename_at_50_100);
}

inline spoof::SpoofInvocation & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8860640a3da483d ( Context * __context__, spoof::SpoofInvocation &  __a_rename_at_50_101 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_10,cast<spoof::SpoofInvocation &>::from(__a_rename_at_50_101)));
    return das_auto_cast_ref<spoof::SpoofInvocation &>::cast(__a_rename_at_50_101);
}

inline AutoTuple<bool,TArray<char *>,int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a887f80fd036fc6a ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> &  __a_rename_at_50_102 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_11,cast<AutoTuple<bool,TArray<char *>,int32_t> &>::from(__a_rename_at_50_102)));
    return das_auto_cast_ref<AutoTuple<bool,TArray<char *>,int32_t> &>::cast(__a_rename_at_50_102);
}

inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_32af3ec8801e47ea ( Context * __context__, TArray<char *> &  __a_rename_at_50_103 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_12,cast<TArray<char *> &>::from(__a_rename_at_50_103)));
    return das_auto_cast_ref<TArray<char *> &>::cast(__a_rename_at_50_103);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_2afaf9cdb69403d4 ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  __a_rename_at_32_104, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  __b_rename_at_32_105 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_13,cast<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &>::from(__a_rename_at_32_104)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_14,cast<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &>::from(__b_rename_at_32_105)));
    das_move(__a_rename_at_32_104,__b_rename_at_32_105);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_74d3e26b89ca39c1 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_32_106, TArray<AutoTuple<char *,char *>> &  __b_rename_at_32_107 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_15,cast<TArray<AutoTuple<char *,char *>> &>::from(__a_rename_at_32_106)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_16,cast<TArray<AutoTuple<char *,char *>> &>::from(__b_rename_at_32_107)));
    das_move(__a_rename_at_32_106,__b_rename_at_32_107);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_721189fff44b0b7c ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  __a_rename_at_32_108, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  __b_rename_at_32_109 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_17,cast<AutoTuple<bool,spoof::SpoofInvocation,int32_t> &>::from(__a_rename_at_32_108)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_18,cast<AutoTuple<bool,spoof::SpoofInvocation,int32_t> &>::from(__b_rename_at_32_109)));
    das_move(__a_rename_at_32_108,__b_rename_at_32_109);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_435b13646b15cb2c ( Context * __context__, spoof::SpoofInvocation &  __a_rename_at_32_110, spoof::SpoofInvocation &  __b_rename_at_32_111 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_19,cast<spoof::SpoofInvocation &>::from(__a_rename_at_32_110)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_20,cast<spoof::SpoofInvocation &>::from(__b_rename_at_32_111)));
    das_move(__a_rename_at_32_110,__b_rename_at_32_111);
}

inline void clone_c94fb40a98d27d95 ( Context * __context__, smart_ptr_raw<Variable> & __dest_rename_at_266_112, smart_ptr_raw<Variable> const  __src_rename_at_266_113 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_266_112),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_266_113),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncpegTicklog_infoTick4618819779667933862_79989224586a0f97 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_78_114, char * __message_rename_at_78_115 )
{
    _FuncpegTicktabulateTick3664919181896551618_5938a8f00640e7b0(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_78_114));
    builtin_print(_FunccolorsTickblue_strTick419715327456344553_ccac9a00db81a9f1(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_21, cast<char * const >::from(((char *) "[.] ")), cast<char *>::from(__message_rename_at_78_115), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_78_114.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_91_116 )
{
    while ( is_white_space(_FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_91_116))) )
    {
        _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_91_116),1);
    };
}

inline void _FuncpegTicklog_plainTick18061110541707930728_22d44fb5d6ba6b83 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_63_117, char * __message_rename_at_63_118 )
{
    _FuncpegTicktabulateTick3664919181896551618_5938a8f00640e7b0(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_63_117));
    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_22, cast<char * const >::from(((char *) "[-] ")), cast<char *>::from(__message_rename_at_63_118), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_34_119, char * const  __template__rename_at_34_120, int32_t __strlen_rename_at_34_121 )
{
    int32_t __mark_rename_at_35_122 = __parser_rename_at_34_119.index;
    if ( builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_34_119.input)) < (__mark_rename_at_35_122 + __strlen_rename_at_34_121) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( das_memcmp(das_auto_cast<void * const >::cast(das_ref(__context__,__parser_rename_at_34_119.input(__mark_rename_at_35_122,__context__))),das_auto_cast<void * const >::cast(das_cast<uint8_t *>::cast(__template__rename_at_34_120)),__strlen_rename_at_34_121) == 0 )
        {
            _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_34_119),__strlen_rename_at_34_121);
            return das_auto_cast<bool>::cast(true);
        } else {
            return das_auto_cast<bool>::cast(false);
        };
    };
}

inline void _FuncpegTicklog_failTick11731645277795022368_b3bcabd40e85a2f6 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_68_123, char * __message_rename_at_68_124 )
{
    _FuncpegTicktabulateTick3664919181896551618_5938a8f00640e7b0(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_68_123));
    builtin_print(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_23, cast<char * const >::from(((char *) "[!] ")), cast<char *>::from(__message_rename_at_68_124), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_68_123.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool & _FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1 ( Context * __context__, TArray<bool> &  __a_rename_at_473_125 )
{
    int32_t __l_rename_at_474_126 = ((int32_t)builtin_array_size(das_arg<TArray<bool>>::pass(__a_rename_at_473_125)));
    if ( __l_rename_at_474_126 == 0 )
    {
        builtin_throw(((char *) "back empty array"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast_ref<bool &>::cast(__a_rename_at_473_125((__l_rename_at_474_126 - 1),__context__));
}

inline char * _FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584 ( Context * __context__, char * const  __str_rename_at_14_127, bool __color_rename_at_14_128 )
{
    return das_auto_cast<char *>::cast(__color_rename_at_14_128 ? das_auto_cast<char * const >::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_24, cast<char * const >::from(((char *) "\u001b[31m")), cast<char * const >::from(__str_rename_at_14_127), cast<char * const >::from(((char *) "\u001b[0m"))))) : das_auto_cast<char * const >::cast(__str_rename_at_14_127));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b ( Context * __context__, TArray<peg::ParsingError> &  __Arr_rename_at_181_129, peg::ParsingError &  __value_rename_at_181_130 )
{
    das_copy(__Arr_rename_at_181_129(builtin_array_push_back(das_arg<TArray<peg::ParsingError>>::pass(__Arr_rename_at_181_129),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_130);
}

inline void _FuncpegTicklog_successTick3645718276917011819_48df15f72c464176 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_73_131, char * __message_rename_at_73_132 )
{
    _FuncpegTicktabulateTick3664919181896551618_5938a8f00640e7b0(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_73_131));
    builtin_print(_FunccolorsTickgreen_strTick14807051613123094884_df8dd133b2accbc3(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_25, cast<char * const >::from(((char *) "[!] ")), cast<char *>::from(__message_rename_at_73_132), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_73_131.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79 ( Context * __context__, TArray<bool> &  __Arr_rename_at_165_133, bool __value_rename_at_165_134 )
{
    das_copy(__Arr_rename_at_165_133(builtin_array_push_back(das_arg<TArray<bool>>::pass(__Arr_rename_at_165_133),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_134);
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_ca5d53fe6f76d114 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_287_135, AutoTuple<char *,char *> &  __value_rename_at_287_136 )
{
    das_move(__Arr_rename_at_287_135(builtin_array_push_back(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_287_135),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_136);
}

inline void _FuncbuiltinTickpopTick1161079256290593740_da418e66c1d8244e ( Context * __context__, TArray<bool> &  __Arr_rename_at_132_137 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_aa0264416ea4124a(__context__,das_arg<TArray<bool>>::pass(__Arr_rename_at_132_137),builtin_array_size(das_arg<TArray<bool>>::pass(__Arr_rename_at_132_137)) - 1);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_39183d2b340e1e79 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_181_138, AutoTuple<char *,char *> &  __value_rename_at_181_139 )
{
    das_copy(__Arr_rename_at_181_138(builtin_array_push_back(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_181_138),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_139);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_57ae76d5fb6c64b9 ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>> const  &  __Tab_rename_at_1014_140, int32_t __at_rename_at_1014_141 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_140,__at_rename_at_1014_141));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_61012b8bb2fce779 ( Context * __context__, TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>> const  &  __Tab_rename_at_1014_142, int32_t __at_rename_at_1014_143 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_142,__at_rename_at_1014_143));
}

inline int32_t _FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_103_144 )
{
    return das_auto_cast<int32_t>::cast((builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_103_144.input)) <= __parser_rename_at_103_144.index) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(int32_t(__parser_rename_at_103_144.input(__parser_rename_at_103_144.index,__context__))));
}

inline void _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_54_145, int32_t __offset_rename_at_54_146 )
{
    __parser_rename_at_54_145.index += __offset_rename_at_54_146;
    das_copy(__parser_rename_at_54_145.longest_prefix,SimPolicy<int32_t>::Max(__parser_rename_at_54_145.longest_prefix,__parser_rename_at_54_145.index,*__context__,nullptr));
}

inline AutoTuple<bool,char *> _FuncpegTickmatch_string_literalTick12800953725978677998_f7109d6e87737ab6 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_144_147 )
{
    TArray<uint8_t> __buffer_rename_at_148_148; memset((void*)&__buffer_rename_at_148_148,0,sizeof(__buffer_rename_at_148_148));
    int32_t __current_char_rename_at_150_149; memset((void*)&__current_char_rename_at_150_149,0,sizeof(__current_char_rename_at_150_149));
    /* finally */ auto __finally_144= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_148));
    /* end finally */ });
    das_zero(__buffer_rename_at_148_148);
    __current_char_rename_at_150_149 = _FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147));
    if ( __current_char_rename_at_150_149 != 34 )
    {
        return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
            AutoTuple<bool,char *> __mkt_153;
            das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_153) = false;
            das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_153) = nullptr;
            return __mkt_153;
        })()));
    } else {
        _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147),1);
        das_copy(__current_char_rename_at_150_149,_FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147)));
        while ( (__current_char_rename_at_150_149 != 34) && !(_FuncpegTickreached_EOFTick12193995001429396764_74c5ca3a73c6ab1f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147))) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_148),uint8_t(__current_char_rename_at_150_149));
            _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147),1);
            das_copy(__current_char_rename_at_150_149,_FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147)));
        };
        if ( _FuncpegTickreached_EOFTick12193995001429396764_74c5ca3a73c6ab1f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147)) )
        {
            return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
                AutoTuple<bool,char *> __mkt_165;
                das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_165) = false;
                das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_165) = nullptr;
                return __mkt_165;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_144_147),1);
            return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
                AutoTuple<bool,char *> __mkt_168;
                das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_168) = true;
                das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_168) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_148),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
                return __mkt_168;
            })()));
        };
    };
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026 ( Context * __context__, TTable<int32_t,AutoTuple<bool,char *,int32_t>> const  &  __Tab_rename_at_1014_150, int32_t __at_rename_at_1014_151 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_150,__at_rename_at_1014_151));
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_664b3b92dd33b520 ( Context * __context__, TArray<char *> &  __Arr_rename_at_287_152, char * & __value_rename_at_287_153 )
{
    das_move(__Arr_rename_at_287_152(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_287_152),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_153);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_597abbecf5b26d8 ( Context * __context__, TArray<char *> const  &  __it_rename_at_22_154, char * const  __separator_rename_at_22_155 )
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

inline void _FuncpegTicklog_infoTick4618819779667933862_efdc190cc878d369 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_78_160, char * __message_rename_at_78_161 )
{
    _FuncpegTicktabulateTick3664919181896551618_52ff143bf46df83f(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_78_160));
    builtin_print(_FunccolorsTickblue_strTick419715327456344553_ccac9a00db81a9f1(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_28, cast<char * const >::from(((char *) "[.] ")), cast<char *>::from(__message_rename_at_78_161), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_78_160.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_91_162 )
{
    while ( is_white_space(_FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_91_162))) )
    {
        _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_91_162),1);
    };
}

inline void _FuncpegTicklog_plainTick18061110541707930728_8c201494798c4721 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_63_163, char * __message_rename_at_63_164 )
{
    _FuncpegTicktabulateTick3664919181896551618_52ff143bf46df83f(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_63_163));
    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_29, cast<char * const >::from(((char *) "[-] ")), cast<char *>::from(__message_rename_at_63_164), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncpegTickmatchesTick4020677071890555356_83d30930705988a2 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_34_165, char * const  __template__rename_at_34_166, int32_t __strlen_rename_at_34_167 )
{
    int32_t __mark_rename_at_35_168 = __parser_rename_at_34_165.index;
    if ( builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_34_165.input)) < (__mark_rename_at_35_168 + __strlen_rename_at_34_167) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( das_memcmp(das_auto_cast<void * const >::cast(das_ref(__context__,__parser_rename_at_34_165.input(__mark_rename_at_35_168,__context__))),das_auto_cast<void * const >::cast(das_cast<uint8_t *>::cast(__template__rename_at_34_166)),__strlen_rename_at_34_167) == 0 )
        {
            _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_34_165),__strlen_rename_at_34_167);
            return das_auto_cast<bool>::cast(true);
        } else {
            return das_auto_cast<bool>::cast(false);
        };
    };
}

inline void _FuncpegTicklog_failTick11731645277795022368_ae7aad9b0b952b34 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_68_169, char * __message_rename_at_68_170 )
{
    _FuncpegTicktabulateTick3664919181896551618_52ff143bf46df83f(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_68_169));
    builtin_print(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_30, cast<char * const >::from(((char *) "[!] ")), cast<char *>::from(__message_rename_at_68_170), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_68_169.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncpegTicklog_successTick3645718276917011819_237573808f7a2c5c ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_73_171, char * __message_rename_at_73_172 )
{
    _FuncpegTicktabulateTick3664919181896551618_52ff143bf46df83f(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_73_171));
    builtin_print(_FunccolorsTickgreen_strTick14807051613123094884_df8dd133b2accbc3(__context__,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_31, cast<char * const >::from(((char *) "[!] ")), cast<char *>::from(__message_rename_at_73_172), cast<char * const >::from(((char *) "\n")))),__parser_rename_at_73_171.color_output),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_33cfc90a36a139ab ( Context * __context__, TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>> const  &  __Tab_rename_at_1014_173, int32_t __at_rename_at_1014_174 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_173,__at_rename_at_1014_174));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_47edd36ebc80950e ( Context * __context__, TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>> const  &  __Tab_rename_at_1014_175, int32_t __at_rename_at_1014_176 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_175,__at_rename_at_1014_176));
}

inline int32_t _FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_103_177 )
{
    return das_auto_cast<int32_t>::cast((builtin_array_size(das_arg<TArray<uint8_t>>::pass(__parser_rename_at_103_177.input)) <= __parser_rename_at_103_177.index) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(int32_t(__parser_rename_at_103_177.input(__parser_rename_at_103_177.index,__context__))));
}

inline void _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_54_178, int32_t __offset_rename_at_54_179 )
{
    __parser_rename_at_54_178.index += __offset_rename_at_54_179;
    das_copy(__parser_rename_at_54_178.longest_prefix,SimPolicy<int32_t>::Max(__parser_rename_at_54_178.longest_prefix,__parser_rename_at_54_178.index,*__context__,nullptr));
}

inline AutoTuple<bool,char *> _FuncpegTickmatch_string_literalTick12800953725978677998_f6b931b539266e97 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_144_180 )
{
    TArray<uint8_t> __buffer_rename_at_148_181; memset((void*)&__buffer_rename_at_148_181,0,sizeof(__buffer_rename_at_148_181));
    int32_t __current_char_rename_at_150_182; memset((void*)&__current_char_rename_at_150_182,0,sizeof(__current_char_rename_at_150_182));
    /* finally */ auto __finally_144= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_181));
    /* end finally */ });
    das_zero(__buffer_rename_at_148_181);
    __current_char_rename_at_150_182 = _FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180));
    if ( __current_char_rename_at_150_182 != 34 )
    {
        return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
            AutoTuple<bool,char *> __mkt_153;
            das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_153) = false;
            das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_153) = nullptr;
            return __mkt_153;
        })()));
    } else {
        _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180),1);
        das_copy(__current_char_rename_at_150_182,_FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180)));
        while ( (__current_char_rename_at_150_182 != 34) && !(_FuncpegTickreached_EOFTick12193995001429396764_76116504adfe2552(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180))) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_181),uint8_t(__current_char_rename_at_150_182));
            _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180),1);
            das_copy(__current_char_rename_at_150_182,_FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180)));
        };
        if ( _FuncpegTickreached_EOFTick12193995001429396764_76116504adfe2552(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180)) )
        {
            return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
                AutoTuple<bool,char *> __mkt_165;
                das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_165) = false;
                das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_165) = nullptr;
                return __mkt_165;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_144_180),1);
            return das_auto_cast_ref<AutoTuple<bool,char *>>::cast((([&]() -> AutoTuple<bool,char *> {
                AutoTuple<bool,char *> __mkt_168;
                das_get_auto_tuple_field<bool,0,bool,char *>::get(__mkt_168) = true;
                das_get_auto_tuple_field<char *,1,bool,char *>::get(__mkt_168) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__buffer_rename_at_148_181),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
                return __mkt_168;
            })()));
        };
    };
}

inline void _FuncalgorithmTicksort_uniqueTick14985796887508933377_fd064d4ca86f9c39 ( Context * __context__, TArray<peg::ParsingError> &  __a_rename_at_24_183 )
{
    if ( _FuncbuiltinTickemptyTick15399874715904164783_8d8767d901055902(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183)) )
    {
        return ;
    } else {
        _FuncbuiltinTicksortTick14088969635007481283_2b1d2e9c3495b624(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183));
        TArray<peg::ParsingError> __b_rename_at_32_184; das_zero(__b_rename_at_32_184); das_move(__b_rename_at_32_184, _FuncalgorithmTickuniqueTick8642070526899511001_97dad40955268394(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183)));
        _FuncbuiltinTickfinalizeTick13836114024949725080_e5d8bd461b7dcb17(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183));
        _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_9d118576d5107c28(__context__,das_arg<TArray<peg::ParsingError>>::pass(__a_rename_at_24_183),das_arg<TArray<peg::ParsingError>>::pass(__b_rename_at_32_184));
    };
}

inline void finalize_b0cebf59e5e51d47 ( Context * __context__, spoof::invocationParserTickid_0x0 &  ____this_rename_at_0_185 )
{
    _FuncbuiltinTickfinalizeTick5454204887383796109_40a623f9c95b0cb9(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(____this_rename_at_0_185.comma_separated_elements_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_53b9400c23ef0c99(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(____this_rename_at_0_185.element_list_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_185.ident__cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_185.element_value_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_40a623f9c95b0cb9(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(____this_rename_at_0_185.element_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_185.sub_element_cache));
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_0_185.input));
    _FuncbuiltinTickfinalizeTick13836114024949725080_8872455bc7f9c5ef(__context__,das_arg<TArray<bool>>::pass(____this_rename_at_0_185.suppress_errors));
    _FuncbuiltinTickfinalizeTick13836114024949725080_e5d8bd461b7dcb17(__context__,das_arg<TArray<peg::ParsingError>>::pass(____this_rename_at_0_185.errors));
    memset((void*)&(____this_rename_at_0_185), 0, TypeSize<spoof::invocationParserTickid_0x0>::size);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_8fb28bab1278e8c2 ( Context * __context__, TArray<peg::ParsingError> const  &  __it_rename_at_22_186, char * const  __separator_rename_at_22_187 )
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

inline void finalize_bf2f98006be610c6 ( Context * __context__, spoof::invocationParserTickid_0x1 &  ____this_rename_at_0_192 )
{
    _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.comma_separated_elements_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.sub_element_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_84cf6af8e0041469(__context__,das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(____this_rename_at_0_192.macro_invocation_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.ident__cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.class_name_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(____this_rename_at_0_192.element_cache));
    _FuncbuiltinTickfinalizeTick5454204887383796109_5f01338060689947(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(____this_rename_at_0_192.element_list_cache));
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_0_192.input));
    _FuncbuiltinTickfinalizeTick13836114024949725080_8872455bc7f9c5ef(__context__,das_arg<TArray<bool>>::pass(____this_rename_at_0_192.suppress_errors));
    _FuncbuiltinTickfinalizeTick13836114024949725080_e5d8bd461b7dcb17(__context__,das_arg<TArray<peg::ParsingError>>::pass(____this_rename_at_0_192.errors));
    memset((void*)&(____this_rename_at_0_192), 0, TypeSize<spoof::invocationParserTickid_0x1>::size);
}

inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _Funcparse_element_list_innerTickid_0x0_1c48b25484abe536 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_193 )
{
    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _temp_make_local_364_18_656; _temp_make_local_364_18_656;
    int32_t __parse_pos_rename_at_1076_194 = __parser_rename_at_362_193.index;
    ++__parser_rename_at_362_193.tabs;
    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __res_1_rename_at_1086_195; das_zero(__res_1_rename_at_1086_195); das_move(__res_1_rename_at_1086_195, das_invoke<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193));
        if ( !_FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193),((char *) "("),1) )
        {
            if ( (__parser_rename_at_362_193.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_193.suppress_errors)))) && (__parser_rename_at_362_193.index == __parser_rename_at_362_193.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1171_49_144; _temp_make_local_1171_49_144;
                char * __rest_rename_at_1169_196 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_34, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_193.index))),false));
                char * __complete_message_rename_at_1170_197 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_35, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_196))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_193.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1171_49_144.text),(__complete_message_rename_at_1170_197));
                    das_copy((_temp_make_local_1171_49_144.index),(__parser_rename_at_362_193.index));
                    return _temp_make_local_1171_49_144;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast((([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> {
                AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __mks_1175;
                das_zero(__mks_1175);
                return __mks_1175;
            })()));
        } else {
            _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193));
            TArray<AutoTuple<char *,char *>> __els_rename_at_671_198;das_zero(__els_rename_at_671_198);
            _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_193.suppress_errors),true);
            while ( true )
            {
                int32_t __sz_rename_at_674_199 = builtin_array_size(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__els_rename_at_671_198));
                das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __comma_separated_elements0_rename_at_546_200; das_zero(__comma_separated_elements0_rename_at_546_200); das_move(__comma_separated_elements0_rename_at_546_200, _Funcparse_comma_separated_elementsTickid_0x0_84b92b09aca2a83b(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193)));
                    if ( !das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__comma_separated_elements0_rename_at_546_200) )
                    {
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                            AutoTuple<bool,int32_t,int32_t> __mks_548;
                            das_zero(__mks_548);
                            return __mks_548;
                        })()));
                    } else {
                        AutoTuple<char *,char *> __temp_name_rename_at_551_201; das_zero(__temp_name_rename_at_551_201); das_move(__temp_name_rename_at_551_201, das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__comma_separated_elements0_rename_at_546_200));
                        _FuncbuiltinTickemplaceTick13923705686753630697_ca5d53fe6f76d114(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__els_rename_at_671_198),das_arg<AutoTuple<char *,char *>>::pass(__temp_name_rename_at_551_201));
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
            _FuncbuiltinTickpopTick1161079256290593740_da418e66c1d8244e(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_193.suppress_errors));
            _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193));
            AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __element0_rename_at_546_202; das_zero(__element0_rename_at_546_202); das_move(__element0_rename_at_546_202, _Funcparse_elementTickid_0x0_85965a755893ca3(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193)));
            if ( !das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__element0_rename_at_546_202) )
            {
                return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast((([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> {
                    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __mks_548;
                    das_zero(__mks_548);
                    return __mks_548;
                })()));
            } else {
                AutoTuple<char *,char *> __last_rename_at_551_203; das_zero(__last_rename_at_551_203); das_move(__last_rename_at_551_203, das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__element0_rename_at_546_202));
                _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193));
                if ( !_FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_193),((char *) ")"),1) )
                {
                    if ( (__parser_rename_at_362_193.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_193.suppress_errors)))) && (__parser_rename_at_362_193.index == __parser_rename_at_362_193.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1171_49_480; _temp_make_local_1171_49_480;
                        char * __rest_rename_at_1169_204 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_36, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_193.index))),false));
                        char * __complete_message_rename_at_1170_205 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_37, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_204))));
                        _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_193.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1171_49_480.text),(__complete_message_rename_at_1170_205));
                            das_copy((_temp_make_local_1171_49_480.index),(__parser_rename_at_362_193.index));
                            return _temp_make_local_1171_49_480;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast((([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> {
                        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __mks_1175;
                        das_zero(__mks_1175);
                        return __mks_1175;
                    })()));
                } else {
                    TArray<AutoTuple<char *,char *>> __val_rename_at_1129_206; das_zero(__val_rename_at_1129_206); das_move(__val_rename_at_1129_206, das_invoke<TArray<AutoTuple<char *,char *>>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<AutoTuple<char *,char *>>{
                        _FuncbuiltinTickpushTick10769833213962245646_39183d2b340e1e79(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__els_rename_at_671_198),das_arg<AutoTuple<char *,char *>>::pass(__last_rename_at_551_203));
                        return /* <- */ das_auto_cast_move<TArray<AutoTuple<char *,char *>>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_7ecb0a5d4721f3ce(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__els_rename_at_671_198)));
                    }));
                    AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __result_rename_at_1137_207; das_zero(__result_rename_at_1137_207); das_move(__result_rename_at_1137_207, (([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> {
                        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __mkt_1137;
                        das_get_auto_tuple_field<bool,0,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__mkt_1137) = true;
                        das_get_auto_tuple_field<TArray<AutoTuple<char *,char *>>,1,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__mkt_1137) = __val_rename_at_1129_206;
                        das_get_auto_tuple_field<int32_t,2,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__mkt_1137) = __parser_rename_at_362_193.index;
                        return __mkt_1137;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(__result_rename_at_1137_207);
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
    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ba4ceaf27ff6ff46(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass((([&]() -> AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>& {
        das_zero(_temp_make_local_364_18_656);
        return _temp_make_local_364_18_656;
    })()))));
}

inline AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> _Funcparse_element_listTickid_0x0_d172783e9d6cc855 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_208 )
{
    int32_t __mark_rename_at_147_209 = __parser_rename_at_146_208.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_57ae76d5fb6c64b9(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>>::pass(__parser_rename_at_146_208.element_list_cache),__parser_rename_at_146_208.index) && !(__parser_rename_at_146_208.error_reporting) )
    {
        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __result_rename_at_155_210; das_zero(__result_rename_at_155_210); das_move(__result_rename_at_155_210, _FuncbuiltinTickclone_to_moveTick2007252383599261567_d582432df8818d6e(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass(__parser_rename_at_146_208.element_list_cache(__parser_rename_at_146_208.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__result_rename_at_155_210) )
        {
            das_copy(__parser_rename_at_146_208.index,das_get_auto_tuple_field<int32_t,2,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__result_rename_at_155_210));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(__result_rename_at_155_210);
    } else {
        AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> __result_rename_at_169_211; das_zero(__result_rename_at_169_211); das_move(__result_rename_at_169_211, _Funcparse_element_list_innerTickid_0x0_1c48b25484abe536(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_208)));
        clone_1d0149763c60891a(__context__,das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass(__parser_rename_at_146_208.element_list_cache(__mark_rename_at_147_209,__context__)),das_arg<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::pass(__result_rename_at_169_211));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t>>::cast(__result_rename_at_169_211);
    };
}

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_comma_separated_elements_innerTickid_0x0_7f14badd440f98f ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_212 )
{
    int32_t __parse_pos_rename_at_1076_213 = __parser_rename_at_362_212.index;
    ++__parser_rename_at_362_212.tabs;
    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __res_1_rename_at_1086_214; das_zero(__res_1_rename_at_1086_214); das_move(__res_1_rename_at_1086_214, das_invoke<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_212));
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __element1_rename_at_546_215; das_zero(__element1_rename_at_546_215); das_move(__element1_rename_at_546_215, _Funcparse_elementTickid_0x0_85965a755893ca3(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_212)));
        if ( !das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__element1_rename_at_546_215) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            AutoTuple<char *,char *> __e_rename_at_551_216; das_zero(__e_rename_at_551_216); das_move(__e_rename_at_551_216, das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__element1_rename_at_546_215));
            _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_212));
            if ( !_FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_212),((char *) ","),1) )
            {
                if ( (__parser_rename_at_362_212.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_212.suppress_errors)))) && (__parser_rename_at_362_212.index == __parser_rename_at_362_212.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1171_49_208; _temp_make_local_1171_49_208;
                    char * __rest_rename_at_1169_217 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_38, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_212.index))),false));
                    char * __complete_message_rename_at_1170_218 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_39, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m,\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_217))));
                    _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_212.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1171_49_208.text),(__complete_message_rename_at_1170_218));
                        das_copy((_temp_make_local_1171_49_208.index),(__parser_rename_at_362_212.index));
                        return _temp_make_local_1171_49_208;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_1175;
                    das_zero(__mks_1175);
                    return __mks_1175;
                })()));
            } else {
                AutoTuple<char *,char *> __val_rename_at_1129_219; das_zero(__val_rename_at_1129_219); das_move(__val_rename_at_1129_219, das_invoke<AutoTuple<char *,char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<char *,char *>{
                    return das_auto_cast_ref<AutoTuple<char *,char *>>::cast(__e_rename_at_551_216);
                }));
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_1137_220; das_zero(__result_rename_at_1137_220); das_move(__result_rename_at_1137_220, (([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mkt_1137;
                    das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1137) = true;
                    das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1137) = __val_rename_at_1129_219;
                    das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1137) = __parser_rename_at_362_212.index;
                    return __mkt_1137;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_1137_220);
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

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_comma_separated_elementsTickid_0x0_84b92b09aca2a83b ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_221 )
{
    int32_t __mark_rename_at_147_222 = __parser_rename_at_146_221.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_61012b8bb2fce779(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__parser_rename_at_146_221.comma_separated_elements_cache),__parser_rename_at_146_221.index) && !(__parser_rename_at_146_221.error_reporting) )
    {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_155_223; das_zero(__result_rename_at_155_223); das_move(__result_rename_at_155_223, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f74cc0676a04cd8(__context__,das_arg<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::pass(__parser_rename_at_146_221.comma_separated_elements_cache(__parser_rename_at_146_221.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__result_rename_at_155_223) )
        {
            das_copy(__parser_rename_at_146_221.index,das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__result_rename_at_155_223));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_155_223);
    } else {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_169_224; das_zero(__result_rename_at_169_224); das_move(__result_rename_at_169_224, _Funcparse_comma_separated_elements_innerTickid_0x0_7f14badd440f98f(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_221)));
        das_copy(__parser_rename_at_146_221.comma_separated_elements_cache(__mark_rename_at_147_222,__context__),__result_rename_at_169_224);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_169_224);
    };
}

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_element_innerTickid_0x0_70aae0654fd76c21 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_225 )
{
    int32_t __parse_pos_rename_at_1076_226 = __parser_rename_at_362_225.index;
    ++__parser_rename_at_362_225.tabs;
    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __res_1_rename_at_1086_227; das_zero(__res_1_rename_at_1086_227); das_move(__res_1_rename_at_1086_227, das_invoke<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t>{
        AutoTuple<bool,char *,int32_t> __ident_0_rename_at_546_228; das_zero(__ident_0_rename_at_546_228); das_move(__ident_0_rename_at_546_228, _Funcparse_ident_Tickid_0x0_20b6fcdb44c1b1c0(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__ident_0_rename_at_546_228) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __e_rename_at_551_229; das_zero(__e_rename_at_551_229); das_move(__e_rename_at_551_229, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__ident_0_rename_at_546_228)));
            _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225));
            if ( !_FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225),((char *) "="),1) )
            {
                if ( (__parser_rename_at_362_225.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_225.suppress_errors)))) && (__parser_rename_at_362_225.index == __parser_rename_at_362_225.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1171_49_240; _temp_make_local_1171_49_240;
                    char * __rest_rename_at_1169_230 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_40, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_225.index))),false));
                    char * __complete_message_rename_at_1170_231 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_41, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m=\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_230))));
                    _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_225.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1171_49_240.text),(__complete_message_rename_at_1170_231));
                        das_copy((_temp_make_local_1171_49_240.index),(__parser_rename_at_362_225.index));
                        return _temp_make_local_1171_49_240;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_1175;
                    das_zero(__mks_1175);
                    return __mks_1175;
                })()));
            } else {
                _FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225));
                AutoTuple<bool,char *,int32_t> __element_value0_rename_at_546_232; das_zero(__element_value0_rename_at_546_232); das_move(__element_value0_rename_at_546_232, _Funcparse_element_valueTickid_0x0_4e09996913410390(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225)));
                if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__element_value0_rename_at_546_232) )
                {
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_548;
                        das_zero(__mks_548);
                        return __mks_548;
                    })()));
                } else {
                    char * __v_rename_at_551_233; das_zero(__v_rename_at_551_233); das_move(__v_rename_at_551_233, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__element_value0_rename_at_546_232)));
                    AutoTuple<char *,char *> __val_rename_at_1129_234; das_zero(__val_rename_at_1129_234); das_move(__val_rename_at_1129_234, das_invoke<AutoTuple<char *,char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<char *,char *>{
                        return das_auto_cast_ref<AutoTuple<char *,char *>>::cast((([&]() -> AutoTuple<char *,char *> {
                            AutoTuple<char *,char *> __mkt_29;
                            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_29) = __e_rename_at_551_229;
                            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_29) = __v_rename_at_551_233;
                            return __mkt_29;
                        })()));
                    }));
                    AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_1137_235; das_zero(__result_rename_at_1137_235); das_move(__result_rename_at_1137_235, (([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mkt_1137;
                        das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1137) = true;
                        das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1137) = __val_rename_at_1129_234;
                        das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1137) = __parser_rename_at_362_225.index;
                        return __mkt_1137;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_1137_235);
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
        AutoTuple<bool,char *,int32_t> __ident_1_rename_at_546_238; das_zero(__ident_1_rename_at_546_238); das_move(__ident_1_rename_at_546_238, _Funcparse_ident_Tickid_0x0_20b6fcdb44c1b1c0(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_225)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__ident_1_rename_at_546_238) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __e_rename_at_551_239; das_zero(__e_rename_at_551_239); das_move(__e_rename_at_551_239, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__ident_1_rename_at_546_238)));
            AutoTuple<char *,char *> __val_rename_at_1129_240; das_zero(__val_rename_at_1129_240); das_move(__val_rename_at_1129_240, das_invoke<AutoTuple<char *,char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<char *,char *>{
                return das_auto_cast_ref<AutoTuple<char *,char *>>::cast((([&]() -> AutoTuple<char *,char *> {
                    AutoTuple<char *,char *> __mkt_32;
                    das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_32) = __e_rename_at_551_239;
                    das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_32) = nullptr;
                    return __mkt_32;
                })()));
            }));
            AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_1137_241; das_zero(__result_rename_at_1137_241); das_move(__result_rename_at_1137_241, (([&]() -> AutoTuple<bool,AutoTuple<char *,char *>,int32_t> {
                AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __mkt_1137;
                das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1137) = true;
                das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1137) = __val_rename_at_1129_240;
                das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__mkt_1137) = __parser_rename_at_362_225.index;
                return __mkt_1137;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_1137_241);
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

inline AutoTuple<bool,AutoTuple<char *,char *>,int32_t> _Funcparse_elementTickid_0x0_85965a755893ca3 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_242 )
{
    int32_t __mark_rename_at_147_243 = __parser_rename_at_146_242.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_61012b8bb2fce779(__context__,das_arg<TTable<int32_t,AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>>::pass(__parser_rename_at_146_242.element_cache),__parser_rename_at_146_242.index) && !(__parser_rename_at_146_242.error_reporting) )
    {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_155_244; das_zero(__result_rename_at_155_244); das_move(__result_rename_at_155_244, _FuncbuiltinTickclone_to_moveTick2007252383599261567_f74cc0676a04cd8(__context__,das_arg<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::pass(__parser_rename_at_146_242.element_cache(__parser_rename_at_146_242.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__result_rename_at_155_244) )
        {
            das_copy(__parser_rename_at_146_242.index,das_get_auto_tuple_field<int32_t,2,bool,AutoTuple<char *,char *>,int32_t>::get(__result_rename_at_155_244));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_155_244);
    } else {
        AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __result_rename_at_169_245; das_zero(__result_rename_at_169_245); das_move(__result_rename_at_169_245, _Funcparse_element_innerTickid_0x0_70aae0654fd76c21(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_242)));
        das_copy(__parser_rename_at_146_242.element_cache(__mark_rename_at_147_243,__context__),__result_rename_at_169_245);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,AutoTuple<char *,char *>,int32_t>>::cast(__result_rename_at_169_245);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_element_innerTickid_0x0_4d49965cb6bc1ab8 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_246 )
{
    int32_t __parse_pos_rename_at_1076_247 = __parser_rename_at_362_246.index;
    ++__parser_rename_at_362_246.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_248; das_zero(__res_1_rename_at_1086_248); das_move(__res_1_rename_at_1086_248, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        if ( !_FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),((char *) "\\"),1) )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1171_49_176; _temp_make_local_1171_49_176;
                char * __rest_rename_at_1169_249 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_42, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                char * __complete_message_rename_at_1170_250 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_43, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m\\\\\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_249))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1171_49_176.text),(__complete_message_rename_at_1170_250));
                    das_copy((_temp_make_local_1171_49_176.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1171_49_176;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1175;
                das_zero(__mks_1175);
                return __mks_1175;
            })()));
        } else {
            int32_t __ext_pos_0_rename_at_564_251 = __parser_rename_at_362_246.index;
            bool ___terminal_result_13_rename_at_1195_252 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
                TDim<bool,256> _temp_make_local_1191_39_304; _temp_make_local_1191_39_304;
                int32_t __ch_rename_at_1190_253 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
                return das_auto_cast<bool>::cast((__ch_rename_at_1190_253 != -1) && (([&]() -> TDim<bool,256>& {
                    _temp_make_local_1191_39_304(0,__context__) = true;
                    _temp_make_local_1191_39_304(1,__context__) = true;
                    _temp_make_local_1191_39_304(2,__context__) = true;
                    _temp_make_local_1191_39_304(3,__context__) = true;
                    _temp_make_local_1191_39_304(4,__context__) = true;
                    _temp_make_local_1191_39_304(5,__context__) = true;
                    _temp_make_local_1191_39_304(6,__context__) = true;
                    _temp_make_local_1191_39_304(7,__context__) = true;
                    _temp_make_local_1191_39_304(8,__context__) = true;
                    _temp_make_local_1191_39_304(9,__context__) = true;
                    _temp_make_local_1191_39_304(10,__context__) = true;
                    _temp_make_local_1191_39_304(11,__context__) = true;
                    _temp_make_local_1191_39_304(12,__context__) = true;
                    _temp_make_local_1191_39_304(13,__context__) = true;
                    _temp_make_local_1191_39_304(14,__context__) = true;
                    _temp_make_local_1191_39_304(15,__context__) = true;
                    _temp_make_local_1191_39_304(16,__context__) = true;
                    _temp_make_local_1191_39_304(17,__context__) = true;
                    _temp_make_local_1191_39_304(18,__context__) = true;
                    _temp_make_local_1191_39_304(19,__context__) = true;
                    _temp_make_local_1191_39_304(20,__context__) = true;
                    _temp_make_local_1191_39_304(21,__context__) = true;
                    _temp_make_local_1191_39_304(22,__context__) = true;
                    _temp_make_local_1191_39_304(23,__context__) = true;
                    _temp_make_local_1191_39_304(24,__context__) = true;
                    _temp_make_local_1191_39_304(25,__context__) = true;
                    _temp_make_local_1191_39_304(26,__context__) = true;
                    _temp_make_local_1191_39_304(27,__context__) = true;
                    _temp_make_local_1191_39_304(28,__context__) = true;
                    _temp_make_local_1191_39_304(29,__context__) = true;
                    _temp_make_local_1191_39_304(30,__context__) = true;
                    _temp_make_local_1191_39_304(31,__context__) = true;
                    _temp_make_local_1191_39_304(32,__context__) = true;
                    _temp_make_local_1191_39_304(33,__context__) = true;
                    _temp_make_local_1191_39_304(34,__context__) = true;
                    _temp_make_local_1191_39_304(35,__context__) = true;
                    _temp_make_local_1191_39_304(36,__context__) = true;
                    _temp_make_local_1191_39_304(37,__context__) = true;
                    _temp_make_local_1191_39_304(38,__context__) = true;
                    _temp_make_local_1191_39_304(39,__context__) = true;
                    _temp_make_local_1191_39_304(40,__context__) = true;
                    _temp_make_local_1191_39_304(41,__context__) = true;
                    _temp_make_local_1191_39_304(42,__context__) = true;
                    _temp_make_local_1191_39_304(43,__context__) = true;
                    _temp_make_local_1191_39_304(44,__context__) = true;
                    _temp_make_local_1191_39_304(45,__context__) = true;
                    _temp_make_local_1191_39_304(46,__context__) = true;
                    _temp_make_local_1191_39_304(47,__context__) = true;
                    _temp_make_local_1191_39_304(48,__context__) = true;
                    _temp_make_local_1191_39_304(49,__context__) = true;
                    _temp_make_local_1191_39_304(50,__context__) = true;
                    _temp_make_local_1191_39_304(51,__context__) = true;
                    _temp_make_local_1191_39_304(52,__context__) = true;
                    _temp_make_local_1191_39_304(53,__context__) = true;
                    _temp_make_local_1191_39_304(54,__context__) = true;
                    _temp_make_local_1191_39_304(55,__context__) = true;
                    _temp_make_local_1191_39_304(56,__context__) = true;
                    _temp_make_local_1191_39_304(57,__context__) = true;
                    _temp_make_local_1191_39_304(58,__context__) = true;
                    _temp_make_local_1191_39_304(59,__context__) = true;
                    _temp_make_local_1191_39_304(60,__context__) = true;
                    _temp_make_local_1191_39_304(61,__context__) = true;
                    _temp_make_local_1191_39_304(62,__context__) = true;
                    _temp_make_local_1191_39_304(63,__context__) = true;
                    _temp_make_local_1191_39_304(64,__context__) = true;
                    _temp_make_local_1191_39_304(65,__context__) = true;
                    _temp_make_local_1191_39_304(66,__context__) = true;
                    _temp_make_local_1191_39_304(67,__context__) = true;
                    _temp_make_local_1191_39_304(68,__context__) = true;
                    _temp_make_local_1191_39_304(69,__context__) = true;
                    _temp_make_local_1191_39_304(70,__context__) = true;
                    _temp_make_local_1191_39_304(71,__context__) = true;
                    _temp_make_local_1191_39_304(72,__context__) = true;
                    _temp_make_local_1191_39_304(73,__context__) = true;
                    _temp_make_local_1191_39_304(74,__context__) = true;
                    _temp_make_local_1191_39_304(75,__context__) = true;
                    _temp_make_local_1191_39_304(76,__context__) = true;
                    _temp_make_local_1191_39_304(77,__context__) = true;
                    _temp_make_local_1191_39_304(78,__context__) = true;
                    _temp_make_local_1191_39_304(79,__context__) = true;
                    _temp_make_local_1191_39_304(80,__context__) = true;
                    _temp_make_local_1191_39_304(81,__context__) = true;
                    _temp_make_local_1191_39_304(82,__context__) = true;
                    _temp_make_local_1191_39_304(83,__context__) = true;
                    _temp_make_local_1191_39_304(84,__context__) = true;
                    _temp_make_local_1191_39_304(85,__context__) = true;
                    _temp_make_local_1191_39_304(86,__context__) = true;
                    _temp_make_local_1191_39_304(87,__context__) = true;
                    _temp_make_local_1191_39_304(88,__context__) = true;
                    _temp_make_local_1191_39_304(89,__context__) = true;
                    _temp_make_local_1191_39_304(90,__context__) = true;
                    _temp_make_local_1191_39_304(91,__context__) = true;
                    _temp_make_local_1191_39_304(92,__context__) = true;
                    _temp_make_local_1191_39_304(93,__context__) = true;
                    _temp_make_local_1191_39_304(94,__context__) = true;
                    _temp_make_local_1191_39_304(95,__context__) = true;
                    _temp_make_local_1191_39_304(96,__context__) = true;
                    _temp_make_local_1191_39_304(97,__context__) = true;
                    _temp_make_local_1191_39_304(98,__context__) = true;
                    _temp_make_local_1191_39_304(99,__context__) = true;
                    _temp_make_local_1191_39_304(100,__context__) = true;
                    _temp_make_local_1191_39_304(101,__context__) = true;
                    _temp_make_local_1191_39_304(102,__context__) = true;
                    _temp_make_local_1191_39_304(103,__context__) = true;
                    _temp_make_local_1191_39_304(104,__context__) = true;
                    _temp_make_local_1191_39_304(105,__context__) = true;
                    _temp_make_local_1191_39_304(106,__context__) = true;
                    _temp_make_local_1191_39_304(107,__context__) = true;
                    _temp_make_local_1191_39_304(108,__context__) = true;
                    _temp_make_local_1191_39_304(109,__context__) = true;
                    _temp_make_local_1191_39_304(110,__context__) = true;
                    _temp_make_local_1191_39_304(111,__context__) = true;
                    _temp_make_local_1191_39_304(112,__context__) = true;
                    _temp_make_local_1191_39_304(113,__context__) = true;
                    _temp_make_local_1191_39_304(114,__context__) = true;
                    _temp_make_local_1191_39_304(115,__context__) = true;
                    _temp_make_local_1191_39_304(116,__context__) = true;
                    _temp_make_local_1191_39_304(117,__context__) = true;
                    _temp_make_local_1191_39_304(118,__context__) = true;
                    _temp_make_local_1191_39_304(119,__context__) = true;
                    _temp_make_local_1191_39_304(120,__context__) = true;
                    _temp_make_local_1191_39_304(121,__context__) = true;
                    _temp_make_local_1191_39_304(122,__context__) = true;
                    _temp_make_local_1191_39_304(123,__context__) = true;
                    _temp_make_local_1191_39_304(124,__context__) = true;
                    _temp_make_local_1191_39_304(125,__context__) = true;
                    _temp_make_local_1191_39_304(126,__context__) = true;
                    _temp_make_local_1191_39_304(127,__context__) = true;
                    _temp_make_local_1191_39_304(128,__context__) = true;
                    _temp_make_local_1191_39_304(129,__context__) = true;
                    _temp_make_local_1191_39_304(130,__context__) = true;
                    _temp_make_local_1191_39_304(131,__context__) = true;
                    _temp_make_local_1191_39_304(132,__context__) = true;
                    _temp_make_local_1191_39_304(133,__context__) = true;
                    _temp_make_local_1191_39_304(134,__context__) = true;
                    _temp_make_local_1191_39_304(135,__context__) = true;
                    _temp_make_local_1191_39_304(136,__context__) = true;
                    _temp_make_local_1191_39_304(137,__context__) = true;
                    _temp_make_local_1191_39_304(138,__context__) = true;
                    _temp_make_local_1191_39_304(139,__context__) = true;
                    _temp_make_local_1191_39_304(140,__context__) = true;
                    _temp_make_local_1191_39_304(141,__context__) = true;
                    _temp_make_local_1191_39_304(142,__context__) = true;
                    _temp_make_local_1191_39_304(143,__context__) = true;
                    _temp_make_local_1191_39_304(144,__context__) = true;
                    _temp_make_local_1191_39_304(145,__context__) = true;
                    _temp_make_local_1191_39_304(146,__context__) = true;
                    _temp_make_local_1191_39_304(147,__context__) = true;
                    _temp_make_local_1191_39_304(148,__context__) = true;
                    _temp_make_local_1191_39_304(149,__context__) = true;
                    _temp_make_local_1191_39_304(150,__context__) = true;
                    _temp_make_local_1191_39_304(151,__context__) = true;
                    _temp_make_local_1191_39_304(152,__context__) = true;
                    _temp_make_local_1191_39_304(153,__context__) = true;
                    _temp_make_local_1191_39_304(154,__context__) = true;
                    _temp_make_local_1191_39_304(155,__context__) = true;
                    _temp_make_local_1191_39_304(156,__context__) = true;
                    _temp_make_local_1191_39_304(157,__context__) = true;
                    _temp_make_local_1191_39_304(158,__context__) = true;
                    _temp_make_local_1191_39_304(159,__context__) = true;
                    _temp_make_local_1191_39_304(160,__context__) = true;
                    _temp_make_local_1191_39_304(161,__context__) = true;
                    _temp_make_local_1191_39_304(162,__context__) = true;
                    _temp_make_local_1191_39_304(163,__context__) = true;
                    _temp_make_local_1191_39_304(164,__context__) = true;
                    _temp_make_local_1191_39_304(165,__context__) = true;
                    _temp_make_local_1191_39_304(166,__context__) = true;
                    _temp_make_local_1191_39_304(167,__context__) = true;
                    _temp_make_local_1191_39_304(168,__context__) = true;
                    _temp_make_local_1191_39_304(169,__context__) = true;
                    _temp_make_local_1191_39_304(170,__context__) = true;
                    _temp_make_local_1191_39_304(171,__context__) = true;
                    _temp_make_local_1191_39_304(172,__context__) = true;
                    _temp_make_local_1191_39_304(173,__context__) = true;
                    _temp_make_local_1191_39_304(174,__context__) = true;
                    _temp_make_local_1191_39_304(175,__context__) = true;
                    _temp_make_local_1191_39_304(176,__context__) = true;
                    _temp_make_local_1191_39_304(177,__context__) = true;
                    _temp_make_local_1191_39_304(178,__context__) = true;
                    _temp_make_local_1191_39_304(179,__context__) = true;
                    _temp_make_local_1191_39_304(180,__context__) = true;
                    _temp_make_local_1191_39_304(181,__context__) = true;
                    _temp_make_local_1191_39_304(182,__context__) = true;
                    _temp_make_local_1191_39_304(183,__context__) = true;
                    _temp_make_local_1191_39_304(184,__context__) = true;
                    _temp_make_local_1191_39_304(185,__context__) = true;
                    _temp_make_local_1191_39_304(186,__context__) = true;
                    _temp_make_local_1191_39_304(187,__context__) = true;
                    _temp_make_local_1191_39_304(188,__context__) = true;
                    _temp_make_local_1191_39_304(189,__context__) = true;
                    _temp_make_local_1191_39_304(190,__context__) = true;
                    _temp_make_local_1191_39_304(191,__context__) = true;
                    _temp_make_local_1191_39_304(192,__context__) = true;
                    _temp_make_local_1191_39_304(193,__context__) = true;
                    _temp_make_local_1191_39_304(194,__context__) = true;
                    _temp_make_local_1191_39_304(195,__context__) = true;
                    _temp_make_local_1191_39_304(196,__context__) = true;
                    _temp_make_local_1191_39_304(197,__context__) = true;
                    _temp_make_local_1191_39_304(198,__context__) = true;
                    _temp_make_local_1191_39_304(199,__context__) = true;
                    _temp_make_local_1191_39_304(200,__context__) = true;
                    _temp_make_local_1191_39_304(201,__context__) = true;
                    _temp_make_local_1191_39_304(202,__context__) = true;
                    _temp_make_local_1191_39_304(203,__context__) = true;
                    _temp_make_local_1191_39_304(204,__context__) = true;
                    _temp_make_local_1191_39_304(205,__context__) = true;
                    _temp_make_local_1191_39_304(206,__context__) = true;
                    _temp_make_local_1191_39_304(207,__context__) = true;
                    _temp_make_local_1191_39_304(208,__context__) = true;
                    _temp_make_local_1191_39_304(209,__context__) = true;
                    _temp_make_local_1191_39_304(210,__context__) = true;
                    _temp_make_local_1191_39_304(211,__context__) = true;
                    _temp_make_local_1191_39_304(212,__context__) = true;
                    _temp_make_local_1191_39_304(213,__context__) = true;
                    _temp_make_local_1191_39_304(214,__context__) = true;
                    _temp_make_local_1191_39_304(215,__context__) = true;
                    _temp_make_local_1191_39_304(216,__context__) = true;
                    _temp_make_local_1191_39_304(217,__context__) = true;
                    _temp_make_local_1191_39_304(218,__context__) = true;
                    _temp_make_local_1191_39_304(219,__context__) = true;
                    _temp_make_local_1191_39_304(220,__context__) = true;
                    _temp_make_local_1191_39_304(221,__context__) = true;
                    _temp_make_local_1191_39_304(222,__context__) = true;
                    _temp_make_local_1191_39_304(223,__context__) = true;
                    _temp_make_local_1191_39_304(224,__context__) = true;
                    _temp_make_local_1191_39_304(225,__context__) = true;
                    _temp_make_local_1191_39_304(226,__context__) = true;
                    _temp_make_local_1191_39_304(227,__context__) = true;
                    _temp_make_local_1191_39_304(228,__context__) = true;
                    _temp_make_local_1191_39_304(229,__context__) = true;
                    _temp_make_local_1191_39_304(230,__context__) = true;
                    _temp_make_local_1191_39_304(231,__context__) = true;
                    _temp_make_local_1191_39_304(232,__context__) = true;
                    _temp_make_local_1191_39_304(233,__context__) = true;
                    _temp_make_local_1191_39_304(234,__context__) = true;
                    _temp_make_local_1191_39_304(235,__context__) = true;
                    _temp_make_local_1191_39_304(236,__context__) = true;
                    _temp_make_local_1191_39_304(237,__context__) = true;
                    _temp_make_local_1191_39_304(238,__context__) = true;
                    _temp_make_local_1191_39_304(239,__context__) = true;
                    _temp_make_local_1191_39_304(240,__context__) = true;
                    _temp_make_local_1191_39_304(241,__context__) = true;
                    _temp_make_local_1191_39_304(242,__context__) = true;
                    _temp_make_local_1191_39_304(243,__context__) = true;
                    _temp_make_local_1191_39_304(244,__context__) = true;
                    _temp_make_local_1191_39_304(245,__context__) = true;
                    _temp_make_local_1191_39_304(246,__context__) = true;
                    _temp_make_local_1191_39_304(247,__context__) = true;
                    _temp_make_local_1191_39_304(248,__context__) = true;
                    _temp_make_local_1191_39_304(249,__context__) = true;
                    _temp_make_local_1191_39_304(250,__context__) = true;
                    _temp_make_local_1191_39_304(251,__context__) = true;
                    _temp_make_local_1191_39_304(252,__context__) = true;
                    _temp_make_local_1191_39_304(253,__context__) = true;
                    _temp_make_local_1191_39_304(254,__context__) = true;
                    _temp_make_local_1191_39_304(255,__context__) = true;
                    return _temp_make_local_1191_39_304;
                })())(__ch_rename_at_1190_253,__context__));
            });
            if ( !___terminal_result_13_rename_at_1195_252 )
            {
                if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1213_49_592; _temp_make_local_1213_49_592;
                    char * __rest_rename_at_1211_254 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_44, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                    char * __complete_message_rename_at_1212_255 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_45, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '0'-'256' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1211_254))));
                    _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1213_49_592.text),(__complete_message_rename_at_1212_255));
                        das_copy((_temp_make_local_1213_49_592.index),(__parser_rename_at_362_246.index));
                        return _temp_make_local_1213_49_592;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1216;
                    das_zero(__mks_1216);
                    return __mks_1216;
                })()));
            } else {
                _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),1);
            };
            char * __str_0_rename_at_566_256 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_246.input(0,__context__))));
            char * __Ch_rename_at_567_257 = (char *)(((char * const )(builtin_string_slice1(__str_0_rename_at_566_256,__ext_pos_0_rename_at_564_251,__parser_rename_at_362_246.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            char * __val_rename_at_1129_258; das_zero(__val_rename_at_1129_258); das_move(__val_rename_at_1129_258, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(((char * const )(pass_string(__Ch_rename_at_567_257))));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1137_259; das_zero(__result_rename_at_1137_259); das_move(__result_rename_at_1137_259, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1137;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_258;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_246.index;
                return __mkt_1137;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_259);
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
        if ( !_FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),((char *) "#"),1) )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1171_49_912; _temp_make_local_1171_49_912;
                char * __rest_rename_at_1169_263 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_46, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                char * __complete_message_rename_at_1170_264 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_47, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m#\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_263))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1171_49_912.text),(__complete_message_rename_at_1170_264));
                    das_copy((_temp_make_local_1171_49_912.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1171_49_912;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1175;
                das_zero(__mks_1175);
                return __mks_1175;
            })()));
        } else {
            AutoTuple<bool,char *> ___terminal_result_15_rename_at_1349_265; das_zero(___terminal_result_15_rename_at_1349_265); das_move(___terminal_result_15_rename_at_1349_265, _FuncpegTickmatch_string_literalTick12800953725978677998_f7109d6e87737ab6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
            if ( !das_get_auto_tuple_field<bool,0,bool,char *>::get(___terminal_result_15_rename_at_1349_265) )
            {
                if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1358_49_976; _temp_make_local_1358_49_976;
                    char * __complete_message_rename_at_1357_266 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_48, cast<char * const >::from(((char *) "Error: Expected a string at ")), cast<int32_t>::from(__parser_rename_at_362_246.index)))));
                    _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1358_49_976.text),(__complete_message_rename_at_1357_266));
                        das_copy((_temp_make_local_1358_49_976.index),(__parser_rename_at_362_246.index));
                        return _temp_make_local_1358_49_976;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1361;
                    das_zero(__mks_1361);
                    return __mks_1361;
                })()));
            } else {
                char * __e_rename_at_1368_267 = (char *)(das_get_auto_tuple_field<char *,1,bool,char *>::get(___terminal_result_15_rename_at_1349_265));
                das_copy(__cut_rename_at_1084_261,true);
                char * __val_rename_at_1129_268; das_zero(__val_rename_at_1129_268); das_move(__val_rename_at_1129_268, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                    return das_auto_cast<char *>::cast(__e_rename_at_1368_267);
                })));
                AutoTuple<bool,char *,int32_t> __result_rename_at_1137_269; das_zero(__result_rename_at_1137_269); das_move(__result_rename_at_1137_269, (([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mkt_1137;
                    das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                    das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_268;
                    das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_246.index;
                    return __mkt_1137;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_269);
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
        AutoTuple<bool,char *> ___terminal_result_17_rename_at_1349_273; das_zero(___terminal_result_17_rename_at_1349_273); das_move(___terminal_result_17_rename_at_1349_273, _FuncpegTickmatch_string_literalTick12800953725978677998_f7109d6e87737ab6(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *>::get(___terminal_result_17_rename_at_1349_273) )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1358_49_1296; _temp_make_local_1358_49_1296;
                char * __complete_message_rename_at_1357_274 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_49, cast<char * const >::from(((char *) "Error: Expected a string at ")), cast<int32_t>::from(__parser_rename_at_362_246.index)))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1358_49_1296.text),(__complete_message_rename_at_1357_274));
                    das_copy((_temp_make_local_1358_49_1296.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1358_49_1296;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1361;
                das_zero(__mks_1361);
                return __mks_1361;
            })()));
        } else {
            char * __e_rename_at_1368_275 = (char *)(das_get_auto_tuple_field<char *,1,bool,char *>::get(___terminal_result_17_rename_at_1349_273));
            das_copy(__cut_rename_at_1084_271,true);
            char * __val_rename_at_1129_276; das_zero(__val_rename_at_1129_276); das_move(__val_rename_at_1129_276, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_50, cast<char * const >::from(((char *) "\"")), cast<char *>::from(__e_rename_at_1368_275), cast<char * const >::from(((char *) "\"")))));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1137_277; das_zero(__result_rename_at_1137_277); das_move(__result_rename_at_1137_277, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1137;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_276;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_246.index;
                return __mkt_1137;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_277);
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
        if ( !_FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),((char *) "("),1) )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1171_49_1600; _temp_make_local_1171_49_1600;
                char * __rest_rename_at_1169_280 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_51, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                char * __complete_message_rename_at_1170_281 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_52, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_280))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1171_49_1600.text),(__complete_message_rename_at_1170_281));
                    das_copy((_temp_make_local_1171_49_1600.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1171_49_1600;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1175;
                das_zero(__mks_1175);
                return __mks_1175;
            })()));
        } else {
            AutoTuple<bool,AutoTuple<char *,char *>,int32_t> __element2_rename_at_546_282; das_zero(__element2_rename_at_546_282); das_move(__element2_rename_at_546_282, _Funcparse_elementTickid_0x0_85965a755893ca3(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
            if ( !das_get_auto_tuple_field<bool,0,bool,AutoTuple<char *,char *>,int32_t>::get(__element2_rename_at_546_282) )
            {
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_548;
                    das_zero(__mks_548);
                    return __mks_548;
                })()));
            } else {
                AutoTuple<char *,char *> __e_rename_at_551_283; das_zero(__e_rename_at_551_283); das_move(__e_rename_at_551_283, das_get_auto_tuple_field<AutoTuple<char *,char *>,1,bool,AutoTuple<char *,char *>,int32_t>::get(__element2_rename_at_546_282));
                if ( !_FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),((char *) ")"),1) )
                {
                    if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1171_49_1728; _temp_make_local_1171_49_1728;
                        char * __rest_rename_at_1169_284 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_53, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                        char * __complete_message_rename_at_1170_285 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_54, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_284))));
                        _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1171_49_1728.text),(__complete_message_rename_at_1170_285));
                            das_copy((_temp_make_local_1171_49_1728.index),(__parser_rename_at_362_246.index));
                            return _temp_make_local_1171_49_1728;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                        AutoTuple<bool,char *,int32_t> __mks_1175;
                        das_zero(__mks_1175);
                        return __mks_1175;
                    })()));
                } else {
                    char * __val_rename_at_1129_286; das_zero(__val_rename_at_1129_286); das_move(__val_rename_at_1129_286, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                        return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_55, cast<char * const >::from(((char *) "(")), cast<AutoTuple<char *,char *> &>::from(__e_rename_at_551_283), cast<char * const >::from(((char *) ")")))));
                    })));
                    AutoTuple<bool,char *,int32_t> __result_rename_at_1137_287; das_zero(__result_rename_at_1137_287); das_move(__result_rename_at_1137_287, (([&]() -> AutoTuple<bool,char *,int32_t> {
                        AutoTuple<bool,char *,int32_t> __mkt_1137;
                        das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                        das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_286;
                        das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_246.index;
                        return __mkt_1137;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_287);
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
        bool ___terminal_result_21_rename_at_1195_291 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
            TDim<bool,256> _temp_make_local_1191_39_2064; _temp_make_local_1191_39_2064;
            int32_t __ch_rename_at_1190_292 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246)));
            return das_auto_cast<bool>::cast((__ch_rename_at_1190_292 != -1) && (([&]() -> TDim<bool,256>& {
                _temp_make_local_1191_39_2064(0,__context__) = true;
                _temp_make_local_1191_39_2064(1,__context__) = true;
                _temp_make_local_1191_39_2064(2,__context__) = true;
                _temp_make_local_1191_39_2064(3,__context__) = true;
                _temp_make_local_1191_39_2064(4,__context__) = true;
                _temp_make_local_1191_39_2064(5,__context__) = true;
                _temp_make_local_1191_39_2064(6,__context__) = true;
                _temp_make_local_1191_39_2064(7,__context__) = true;
                _temp_make_local_1191_39_2064(8,__context__) = true;
                _temp_make_local_1191_39_2064(9,__context__) = true;
                _temp_make_local_1191_39_2064(10,__context__) = true;
                _temp_make_local_1191_39_2064(11,__context__) = true;
                _temp_make_local_1191_39_2064(12,__context__) = true;
                _temp_make_local_1191_39_2064(13,__context__) = true;
                _temp_make_local_1191_39_2064(14,__context__) = true;
                _temp_make_local_1191_39_2064(15,__context__) = true;
                _temp_make_local_1191_39_2064(16,__context__) = true;
                _temp_make_local_1191_39_2064(17,__context__) = true;
                _temp_make_local_1191_39_2064(18,__context__) = true;
                _temp_make_local_1191_39_2064(19,__context__) = true;
                _temp_make_local_1191_39_2064(20,__context__) = true;
                _temp_make_local_1191_39_2064(21,__context__) = true;
                _temp_make_local_1191_39_2064(22,__context__) = true;
                _temp_make_local_1191_39_2064(23,__context__) = true;
                _temp_make_local_1191_39_2064(24,__context__) = true;
                _temp_make_local_1191_39_2064(25,__context__) = true;
                _temp_make_local_1191_39_2064(26,__context__) = true;
                _temp_make_local_1191_39_2064(27,__context__) = true;
                _temp_make_local_1191_39_2064(28,__context__) = true;
                _temp_make_local_1191_39_2064(29,__context__) = true;
                _temp_make_local_1191_39_2064(30,__context__) = true;
                _temp_make_local_1191_39_2064(31,__context__) = true;
                _temp_make_local_1191_39_2064(32,__context__) = true;
                _temp_make_local_1191_39_2064(33,__context__) = true;
                _temp_make_local_1191_39_2064(34,__context__) = true;
                _temp_make_local_1191_39_2064(35,__context__) = true;
                _temp_make_local_1191_39_2064(36,__context__) = true;
                _temp_make_local_1191_39_2064(37,__context__) = true;
                _temp_make_local_1191_39_2064(38,__context__) = true;
                _temp_make_local_1191_39_2064(39,__context__) = true;
                _temp_make_local_1191_39_2064(40,__context__) = true;
                _temp_make_local_1191_39_2064(41,__context__) = false;
                _temp_make_local_1191_39_2064(42,__context__) = true;
                _temp_make_local_1191_39_2064(43,__context__) = true;
                _temp_make_local_1191_39_2064(44,__context__) = false;
                _temp_make_local_1191_39_2064(45,__context__) = true;
                _temp_make_local_1191_39_2064(46,__context__) = true;
                _temp_make_local_1191_39_2064(47,__context__) = true;
                _temp_make_local_1191_39_2064(48,__context__) = true;
                _temp_make_local_1191_39_2064(49,__context__) = true;
                _temp_make_local_1191_39_2064(50,__context__) = true;
                _temp_make_local_1191_39_2064(51,__context__) = true;
                _temp_make_local_1191_39_2064(52,__context__) = true;
                _temp_make_local_1191_39_2064(53,__context__) = true;
                _temp_make_local_1191_39_2064(54,__context__) = true;
                _temp_make_local_1191_39_2064(55,__context__) = true;
                _temp_make_local_1191_39_2064(56,__context__) = true;
                _temp_make_local_1191_39_2064(57,__context__) = true;
                _temp_make_local_1191_39_2064(58,__context__) = true;
                _temp_make_local_1191_39_2064(59,__context__) = true;
                _temp_make_local_1191_39_2064(60,__context__) = true;
                _temp_make_local_1191_39_2064(61,__context__) = true;
                _temp_make_local_1191_39_2064(62,__context__) = true;
                _temp_make_local_1191_39_2064(63,__context__) = true;
                _temp_make_local_1191_39_2064(64,__context__) = true;
                _temp_make_local_1191_39_2064(65,__context__) = true;
                _temp_make_local_1191_39_2064(66,__context__) = true;
                _temp_make_local_1191_39_2064(67,__context__) = true;
                _temp_make_local_1191_39_2064(68,__context__) = true;
                _temp_make_local_1191_39_2064(69,__context__) = true;
                _temp_make_local_1191_39_2064(70,__context__) = true;
                _temp_make_local_1191_39_2064(71,__context__) = true;
                _temp_make_local_1191_39_2064(72,__context__) = true;
                _temp_make_local_1191_39_2064(73,__context__) = true;
                _temp_make_local_1191_39_2064(74,__context__) = true;
                _temp_make_local_1191_39_2064(75,__context__) = true;
                _temp_make_local_1191_39_2064(76,__context__) = true;
                _temp_make_local_1191_39_2064(77,__context__) = true;
                _temp_make_local_1191_39_2064(78,__context__) = true;
                _temp_make_local_1191_39_2064(79,__context__) = true;
                _temp_make_local_1191_39_2064(80,__context__) = true;
                _temp_make_local_1191_39_2064(81,__context__) = true;
                _temp_make_local_1191_39_2064(82,__context__) = true;
                _temp_make_local_1191_39_2064(83,__context__) = true;
                _temp_make_local_1191_39_2064(84,__context__) = true;
                _temp_make_local_1191_39_2064(85,__context__) = true;
                _temp_make_local_1191_39_2064(86,__context__) = true;
                _temp_make_local_1191_39_2064(87,__context__) = true;
                _temp_make_local_1191_39_2064(88,__context__) = true;
                _temp_make_local_1191_39_2064(89,__context__) = true;
                _temp_make_local_1191_39_2064(90,__context__) = true;
                _temp_make_local_1191_39_2064(91,__context__) = true;
                _temp_make_local_1191_39_2064(92,__context__) = true;
                _temp_make_local_1191_39_2064(93,__context__) = true;
                _temp_make_local_1191_39_2064(94,__context__) = true;
                _temp_make_local_1191_39_2064(95,__context__) = true;
                _temp_make_local_1191_39_2064(96,__context__) = true;
                _temp_make_local_1191_39_2064(97,__context__) = true;
                _temp_make_local_1191_39_2064(98,__context__) = true;
                _temp_make_local_1191_39_2064(99,__context__) = true;
                _temp_make_local_1191_39_2064(100,__context__) = true;
                _temp_make_local_1191_39_2064(101,__context__) = true;
                _temp_make_local_1191_39_2064(102,__context__) = true;
                _temp_make_local_1191_39_2064(103,__context__) = true;
                _temp_make_local_1191_39_2064(104,__context__) = true;
                _temp_make_local_1191_39_2064(105,__context__) = true;
                _temp_make_local_1191_39_2064(106,__context__) = true;
                _temp_make_local_1191_39_2064(107,__context__) = true;
                _temp_make_local_1191_39_2064(108,__context__) = true;
                _temp_make_local_1191_39_2064(109,__context__) = true;
                _temp_make_local_1191_39_2064(110,__context__) = true;
                _temp_make_local_1191_39_2064(111,__context__) = true;
                _temp_make_local_1191_39_2064(112,__context__) = true;
                _temp_make_local_1191_39_2064(113,__context__) = true;
                _temp_make_local_1191_39_2064(114,__context__) = true;
                _temp_make_local_1191_39_2064(115,__context__) = true;
                _temp_make_local_1191_39_2064(116,__context__) = true;
                _temp_make_local_1191_39_2064(117,__context__) = true;
                _temp_make_local_1191_39_2064(118,__context__) = true;
                _temp_make_local_1191_39_2064(119,__context__) = true;
                _temp_make_local_1191_39_2064(120,__context__) = true;
                _temp_make_local_1191_39_2064(121,__context__) = true;
                _temp_make_local_1191_39_2064(122,__context__) = true;
                _temp_make_local_1191_39_2064(123,__context__) = true;
                _temp_make_local_1191_39_2064(124,__context__) = true;
                _temp_make_local_1191_39_2064(125,__context__) = true;
                _temp_make_local_1191_39_2064(126,__context__) = true;
                _temp_make_local_1191_39_2064(127,__context__) = true;
                _temp_make_local_1191_39_2064(128,__context__) = true;
                _temp_make_local_1191_39_2064(129,__context__) = true;
                _temp_make_local_1191_39_2064(130,__context__) = true;
                _temp_make_local_1191_39_2064(131,__context__) = true;
                _temp_make_local_1191_39_2064(132,__context__) = true;
                _temp_make_local_1191_39_2064(133,__context__) = true;
                _temp_make_local_1191_39_2064(134,__context__) = true;
                _temp_make_local_1191_39_2064(135,__context__) = true;
                _temp_make_local_1191_39_2064(136,__context__) = true;
                _temp_make_local_1191_39_2064(137,__context__) = true;
                _temp_make_local_1191_39_2064(138,__context__) = true;
                _temp_make_local_1191_39_2064(139,__context__) = true;
                _temp_make_local_1191_39_2064(140,__context__) = true;
                _temp_make_local_1191_39_2064(141,__context__) = true;
                _temp_make_local_1191_39_2064(142,__context__) = true;
                _temp_make_local_1191_39_2064(143,__context__) = true;
                _temp_make_local_1191_39_2064(144,__context__) = true;
                _temp_make_local_1191_39_2064(145,__context__) = true;
                _temp_make_local_1191_39_2064(146,__context__) = true;
                _temp_make_local_1191_39_2064(147,__context__) = true;
                _temp_make_local_1191_39_2064(148,__context__) = true;
                _temp_make_local_1191_39_2064(149,__context__) = true;
                _temp_make_local_1191_39_2064(150,__context__) = true;
                _temp_make_local_1191_39_2064(151,__context__) = true;
                _temp_make_local_1191_39_2064(152,__context__) = true;
                _temp_make_local_1191_39_2064(153,__context__) = true;
                _temp_make_local_1191_39_2064(154,__context__) = true;
                _temp_make_local_1191_39_2064(155,__context__) = true;
                _temp_make_local_1191_39_2064(156,__context__) = true;
                _temp_make_local_1191_39_2064(157,__context__) = true;
                _temp_make_local_1191_39_2064(158,__context__) = true;
                _temp_make_local_1191_39_2064(159,__context__) = true;
                _temp_make_local_1191_39_2064(160,__context__) = true;
                _temp_make_local_1191_39_2064(161,__context__) = true;
                _temp_make_local_1191_39_2064(162,__context__) = true;
                _temp_make_local_1191_39_2064(163,__context__) = true;
                _temp_make_local_1191_39_2064(164,__context__) = true;
                _temp_make_local_1191_39_2064(165,__context__) = true;
                _temp_make_local_1191_39_2064(166,__context__) = true;
                _temp_make_local_1191_39_2064(167,__context__) = true;
                _temp_make_local_1191_39_2064(168,__context__) = true;
                _temp_make_local_1191_39_2064(169,__context__) = true;
                _temp_make_local_1191_39_2064(170,__context__) = true;
                _temp_make_local_1191_39_2064(171,__context__) = true;
                _temp_make_local_1191_39_2064(172,__context__) = true;
                _temp_make_local_1191_39_2064(173,__context__) = true;
                _temp_make_local_1191_39_2064(174,__context__) = true;
                _temp_make_local_1191_39_2064(175,__context__) = true;
                _temp_make_local_1191_39_2064(176,__context__) = true;
                _temp_make_local_1191_39_2064(177,__context__) = true;
                _temp_make_local_1191_39_2064(178,__context__) = true;
                _temp_make_local_1191_39_2064(179,__context__) = true;
                _temp_make_local_1191_39_2064(180,__context__) = true;
                _temp_make_local_1191_39_2064(181,__context__) = true;
                _temp_make_local_1191_39_2064(182,__context__) = true;
                _temp_make_local_1191_39_2064(183,__context__) = true;
                _temp_make_local_1191_39_2064(184,__context__) = true;
                _temp_make_local_1191_39_2064(185,__context__) = true;
                _temp_make_local_1191_39_2064(186,__context__) = true;
                _temp_make_local_1191_39_2064(187,__context__) = true;
                _temp_make_local_1191_39_2064(188,__context__) = true;
                _temp_make_local_1191_39_2064(189,__context__) = true;
                _temp_make_local_1191_39_2064(190,__context__) = true;
                _temp_make_local_1191_39_2064(191,__context__) = true;
                _temp_make_local_1191_39_2064(192,__context__) = true;
                _temp_make_local_1191_39_2064(193,__context__) = true;
                _temp_make_local_1191_39_2064(194,__context__) = true;
                _temp_make_local_1191_39_2064(195,__context__) = true;
                _temp_make_local_1191_39_2064(196,__context__) = true;
                _temp_make_local_1191_39_2064(197,__context__) = true;
                _temp_make_local_1191_39_2064(198,__context__) = true;
                _temp_make_local_1191_39_2064(199,__context__) = true;
                _temp_make_local_1191_39_2064(200,__context__) = true;
                _temp_make_local_1191_39_2064(201,__context__) = true;
                _temp_make_local_1191_39_2064(202,__context__) = true;
                _temp_make_local_1191_39_2064(203,__context__) = true;
                _temp_make_local_1191_39_2064(204,__context__) = true;
                _temp_make_local_1191_39_2064(205,__context__) = true;
                _temp_make_local_1191_39_2064(206,__context__) = true;
                _temp_make_local_1191_39_2064(207,__context__) = true;
                _temp_make_local_1191_39_2064(208,__context__) = true;
                _temp_make_local_1191_39_2064(209,__context__) = true;
                _temp_make_local_1191_39_2064(210,__context__) = true;
                _temp_make_local_1191_39_2064(211,__context__) = true;
                _temp_make_local_1191_39_2064(212,__context__) = true;
                _temp_make_local_1191_39_2064(213,__context__) = true;
                _temp_make_local_1191_39_2064(214,__context__) = true;
                _temp_make_local_1191_39_2064(215,__context__) = true;
                _temp_make_local_1191_39_2064(216,__context__) = true;
                _temp_make_local_1191_39_2064(217,__context__) = true;
                _temp_make_local_1191_39_2064(218,__context__) = true;
                _temp_make_local_1191_39_2064(219,__context__) = true;
                _temp_make_local_1191_39_2064(220,__context__) = true;
                _temp_make_local_1191_39_2064(221,__context__) = true;
                _temp_make_local_1191_39_2064(222,__context__) = true;
                _temp_make_local_1191_39_2064(223,__context__) = true;
                _temp_make_local_1191_39_2064(224,__context__) = true;
                _temp_make_local_1191_39_2064(225,__context__) = true;
                _temp_make_local_1191_39_2064(226,__context__) = true;
                _temp_make_local_1191_39_2064(227,__context__) = true;
                _temp_make_local_1191_39_2064(228,__context__) = true;
                _temp_make_local_1191_39_2064(229,__context__) = true;
                _temp_make_local_1191_39_2064(230,__context__) = true;
                _temp_make_local_1191_39_2064(231,__context__) = true;
                _temp_make_local_1191_39_2064(232,__context__) = true;
                _temp_make_local_1191_39_2064(233,__context__) = true;
                _temp_make_local_1191_39_2064(234,__context__) = true;
                _temp_make_local_1191_39_2064(235,__context__) = true;
                _temp_make_local_1191_39_2064(236,__context__) = true;
                _temp_make_local_1191_39_2064(237,__context__) = true;
                _temp_make_local_1191_39_2064(238,__context__) = true;
                _temp_make_local_1191_39_2064(239,__context__) = true;
                _temp_make_local_1191_39_2064(240,__context__) = true;
                _temp_make_local_1191_39_2064(241,__context__) = true;
                _temp_make_local_1191_39_2064(242,__context__) = true;
                _temp_make_local_1191_39_2064(243,__context__) = true;
                _temp_make_local_1191_39_2064(244,__context__) = true;
                _temp_make_local_1191_39_2064(245,__context__) = true;
                _temp_make_local_1191_39_2064(246,__context__) = true;
                _temp_make_local_1191_39_2064(247,__context__) = true;
                _temp_make_local_1191_39_2064(248,__context__) = true;
                _temp_make_local_1191_39_2064(249,__context__) = true;
                _temp_make_local_1191_39_2064(250,__context__) = true;
                _temp_make_local_1191_39_2064(251,__context__) = true;
                _temp_make_local_1191_39_2064(252,__context__) = true;
                _temp_make_local_1191_39_2064(253,__context__) = true;
                _temp_make_local_1191_39_2064(254,__context__) = true;
                _temp_make_local_1191_39_2064(255,__context__) = true;
                return _temp_make_local_1191_39_2064;
            })())(__ch_rename_at_1190_292,__context__));
        });
        if ( !___terminal_result_21_rename_at_1195_291 )
        {
            if ( (__parser_rename_at_362_246.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_246.suppress_errors)))) && (__parser_rename_at_362_246.index == __parser_rename_at_362_246.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1213_49_2352; _temp_make_local_1213_49_2352;
                char * __rest_rename_at_1211_293 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_56, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_246.index))),false));
                char * __complete_message_rename_at_1212_294 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_57, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '0'-'40'  '42'-'43'  '45'-'256' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1211_293))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_246.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1213_49_2352.text),(__complete_message_rename_at_1212_294));
                    das_copy((_temp_make_local_1213_49_2352.index),(__parser_rename_at_362_246.index));
                    return _temp_make_local_1213_49_2352;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1216;
                das_zero(__mks_1216);
                return __mks_1216;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_246),1);
        };
        char * __str_1_rename_at_566_295 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_246.input(0,__context__))));
        char * __e_rename_at_567_296 = (char *)(((char * const )(builtin_string_slice1(__str_1_rename_at_566_295,__ext_pos_1_rename_at_564_290,__parser_rename_at_362_246.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        char * __val_rename_at_1129_297; das_zero(__val_rename_at_1129_297); das_move(__val_rename_at_1129_297, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
            return das_auto_cast<char *>::cast(__e_rename_at_567_296);
        })));
        AutoTuple<bool,char *,int32_t> __result_rename_at_1137_298; das_zero(__result_rename_at_1137_298); das_move(__result_rename_at_1137_298, (([&]() -> AutoTuple<bool,char *,int32_t> {
            AutoTuple<bool,char *,int32_t> __mkt_1137;
            das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
            das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_297;
            das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_246.index;
            return __mkt_1137;
        })()));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_298);
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

inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_elementTickid_0x0_c8a6c75b473a3132 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_299 )
{
    int32_t __mark_rename_at_147_300 = __parser_rename_at_146_299.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_299.sub_element_cache),__parser_rename_at_146_299.index) && !(__parser_rename_at_146_299.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_301; das_zero(__result_rename_at_155_301); das_move(__result_rename_at_155_301, _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_299.sub_element_cache(__parser_rename_at_146_299.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_301) )
        {
            das_copy(__parser_rename_at_146_299.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_301));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_301);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_302; das_zero(__result_rename_at_169_302); das_move(__result_rename_at_169_302, _Funcparse_sub_element_innerTickid_0x0_4d49965cb6bc1ab8(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_299)));
        das_copy(__parser_rename_at_146_299.sub_element_cache(__mark_rename_at_147_300,__context__),__result_rename_at_169_302);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_302);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_element_value_innerTickid_0x0_62f6fb32b8e5e998 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_303 )
{
    int32_t __parse_pos_rename_at_1076_304 = __parser_rename_at_362_303.index;
    ++__parser_rename_at_362_303.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_305; das_zero(__res_1_rename_at_1086_305); das_move(__res_1_rename_at_1086_305, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        AutoTuple<bool,char *,int32_t> __sub_element0_rename_at_546_306; das_zero(__sub_element0_rename_at_546_306); das_move(__sub_element0_rename_at_546_306, _Funcparse_sub_elementTickid_0x0_c8a6c75b473a3132(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_303)));
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
            _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_303.suppress_errors),true);
            while ( true )
            {
                int32_t __sz_rename_at_674_309 = builtin_array_size(das_arg<TArray<char *>>::pass(__tail_rename_at_671_308));
                das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                    AutoTuple<bool,char *,int32_t> __sub_element1_rename_at_546_310; das_zero(__sub_element1_rename_at_546_310); das_move(__sub_element1_rename_at_546_310, _Funcparse_sub_elementTickid_0x0_c8a6c75b473a3132(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_303)));
                    if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__sub_element1_rename_at_546_310) )
                    {
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                            AutoTuple<bool,int32_t,int32_t> __mks_548;
                            das_zero(__mks_548);
                            return __mks_548;
                        })()));
                    } else {
                        char * __temp_name_rename_at_551_311; das_zero(__temp_name_rename_at_551_311); das_move(__temp_name_rename_at_551_311, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__sub_element1_rename_at_546_310)));
                        _FuncbuiltinTickemplaceTick13923705686753630697_664b3b92dd33b520(__context__,das_arg<TArray<char *>>::pass(__tail_rename_at_671_308),__temp_name_rename_at_551_311);
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
            _FuncbuiltinTickpopTick1161079256290593740_da418e66c1d8244e(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_303.suppress_errors));
            char * __val_rename_at_1129_312; das_zero(__val_rename_at_1129_312); das_move(__val_rename_at_1129_312, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__head_rename_at_551_307),cast<char *>::from(_Funcstrings_boostTickjoinTick16475640899284277631_597abbecf5b26d8(__context__,das_arg<TArray<char *>>::pass(__tail_rename_at_671_308),nullptr)),*__context__,nullptr)));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1137_313; das_zero(__result_rename_at_1137_313); das_move(__result_rename_at_1137_313, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1137;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_312;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_303.index;
                return __mkt_1137;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_313);
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

inline AutoTuple<bool,char *,int32_t> _Funcparse_element_valueTickid_0x0_4e09996913410390 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_314 )
{
    int32_t __mark_rename_at_147_315 = __parser_rename_at_146_314.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_314.element_value_cache),__parser_rename_at_146_314.index) && !(__parser_rename_at_146_314.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_316; das_zero(__result_rename_at_155_316); das_move(__result_rename_at_155_316, _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_314.element_value_cache(__parser_rename_at_146_314.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_316) )
        {
            das_copy(__parser_rename_at_146_314.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_316));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_316);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_317; das_zero(__result_rename_at_169_317); das_move(__result_rename_at_169_317, _Funcparse_element_value_innerTickid_0x0_62f6fb32b8e5e998(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_314)));
        das_copy(__parser_rename_at_146_314.element_value_cache(__mark_rename_at_147_315,__context__),__result_rename_at_169_317);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_317);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_ident__innerTickid_0x0_79268295f94867d7 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_362_318 )
{
    int32_t __parse_pos_rename_at_1076_319 = __parser_rename_at_362_318.index;
    ++__parser_rename_at_362_318.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_320; das_zero(__res_1_rename_at_1086_320); das_move(__res_1_rename_at_1086_320, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        int32_t __ext_pos_2_rename_at_564_321 = __parser_rename_at_362_318.index;
        bool ___terminal_result_22_rename_at_1195_322 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
            TDim<bool,256> _temp_make_local_1191_39_208; _temp_make_local_1191_39_208;
            int32_t __ch_rename_at_1190_323 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_318)));
            return das_auto_cast<bool>::cast((__ch_rename_at_1190_323 != -1) && (([&]() -> TDim<bool,256>& {
                _temp_make_local_1191_39_208(0,__context__) = false;
                _temp_make_local_1191_39_208(1,__context__) = false;
                _temp_make_local_1191_39_208(2,__context__) = false;
                _temp_make_local_1191_39_208(3,__context__) = false;
                _temp_make_local_1191_39_208(4,__context__) = false;
                _temp_make_local_1191_39_208(5,__context__) = false;
                _temp_make_local_1191_39_208(6,__context__) = false;
                _temp_make_local_1191_39_208(7,__context__) = false;
                _temp_make_local_1191_39_208(8,__context__) = false;
                _temp_make_local_1191_39_208(9,__context__) = false;
                _temp_make_local_1191_39_208(10,__context__) = false;
                _temp_make_local_1191_39_208(11,__context__) = false;
                _temp_make_local_1191_39_208(12,__context__) = false;
                _temp_make_local_1191_39_208(13,__context__) = false;
                _temp_make_local_1191_39_208(14,__context__) = false;
                _temp_make_local_1191_39_208(15,__context__) = false;
                _temp_make_local_1191_39_208(16,__context__) = false;
                _temp_make_local_1191_39_208(17,__context__) = false;
                _temp_make_local_1191_39_208(18,__context__) = false;
                _temp_make_local_1191_39_208(19,__context__) = false;
                _temp_make_local_1191_39_208(20,__context__) = false;
                _temp_make_local_1191_39_208(21,__context__) = false;
                _temp_make_local_1191_39_208(22,__context__) = false;
                _temp_make_local_1191_39_208(23,__context__) = false;
                _temp_make_local_1191_39_208(24,__context__) = false;
                _temp_make_local_1191_39_208(25,__context__) = false;
                _temp_make_local_1191_39_208(26,__context__) = false;
                _temp_make_local_1191_39_208(27,__context__) = false;
                _temp_make_local_1191_39_208(28,__context__) = false;
                _temp_make_local_1191_39_208(29,__context__) = false;
                _temp_make_local_1191_39_208(30,__context__) = false;
                _temp_make_local_1191_39_208(31,__context__) = false;
                _temp_make_local_1191_39_208(32,__context__) = false;
                _temp_make_local_1191_39_208(33,__context__) = false;
                _temp_make_local_1191_39_208(34,__context__) = false;
                _temp_make_local_1191_39_208(35,__context__) = false;
                _temp_make_local_1191_39_208(36,__context__) = false;
                _temp_make_local_1191_39_208(37,__context__) = false;
                _temp_make_local_1191_39_208(38,__context__) = false;
                _temp_make_local_1191_39_208(39,__context__) = false;
                _temp_make_local_1191_39_208(40,__context__) = false;
                _temp_make_local_1191_39_208(41,__context__) = false;
                _temp_make_local_1191_39_208(42,__context__) = false;
                _temp_make_local_1191_39_208(43,__context__) = false;
                _temp_make_local_1191_39_208(44,__context__) = false;
                _temp_make_local_1191_39_208(45,__context__) = false;
                _temp_make_local_1191_39_208(46,__context__) = false;
                _temp_make_local_1191_39_208(47,__context__) = false;
                _temp_make_local_1191_39_208(48,__context__) = false;
                _temp_make_local_1191_39_208(49,__context__) = false;
                _temp_make_local_1191_39_208(50,__context__) = false;
                _temp_make_local_1191_39_208(51,__context__) = false;
                _temp_make_local_1191_39_208(52,__context__) = false;
                _temp_make_local_1191_39_208(53,__context__) = false;
                _temp_make_local_1191_39_208(54,__context__) = false;
                _temp_make_local_1191_39_208(55,__context__) = false;
                _temp_make_local_1191_39_208(56,__context__) = false;
                _temp_make_local_1191_39_208(57,__context__) = false;
                _temp_make_local_1191_39_208(58,__context__) = false;
                _temp_make_local_1191_39_208(59,__context__) = false;
                _temp_make_local_1191_39_208(60,__context__) = false;
                _temp_make_local_1191_39_208(61,__context__) = false;
                _temp_make_local_1191_39_208(62,__context__) = false;
                _temp_make_local_1191_39_208(63,__context__) = false;
                _temp_make_local_1191_39_208(64,__context__) = false;
                _temp_make_local_1191_39_208(65,__context__) = true;
                _temp_make_local_1191_39_208(66,__context__) = true;
                _temp_make_local_1191_39_208(67,__context__) = true;
                _temp_make_local_1191_39_208(68,__context__) = true;
                _temp_make_local_1191_39_208(69,__context__) = true;
                _temp_make_local_1191_39_208(70,__context__) = true;
                _temp_make_local_1191_39_208(71,__context__) = true;
                _temp_make_local_1191_39_208(72,__context__) = true;
                _temp_make_local_1191_39_208(73,__context__) = true;
                _temp_make_local_1191_39_208(74,__context__) = true;
                _temp_make_local_1191_39_208(75,__context__) = true;
                _temp_make_local_1191_39_208(76,__context__) = true;
                _temp_make_local_1191_39_208(77,__context__) = true;
                _temp_make_local_1191_39_208(78,__context__) = true;
                _temp_make_local_1191_39_208(79,__context__) = true;
                _temp_make_local_1191_39_208(80,__context__) = true;
                _temp_make_local_1191_39_208(81,__context__) = true;
                _temp_make_local_1191_39_208(82,__context__) = true;
                _temp_make_local_1191_39_208(83,__context__) = true;
                _temp_make_local_1191_39_208(84,__context__) = true;
                _temp_make_local_1191_39_208(85,__context__) = true;
                _temp_make_local_1191_39_208(86,__context__) = true;
                _temp_make_local_1191_39_208(87,__context__) = true;
                _temp_make_local_1191_39_208(88,__context__) = true;
                _temp_make_local_1191_39_208(89,__context__) = true;
                _temp_make_local_1191_39_208(90,__context__) = true;
                _temp_make_local_1191_39_208(91,__context__) = false;
                _temp_make_local_1191_39_208(92,__context__) = false;
                _temp_make_local_1191_39_208(93,__context__) = false;
                _temp_make_local_1191_39_208(94,__context__) = false;
                _temp_make_local_1191_39_208(95,__context__) = true;
                _temp_make_local_1191_39_208(96,__context__) = false;
                _temp_make_local_1191_39_208(97,__context__) = true;
                _temp_make_local_1191_39_208(98,__context__) = true;
                _temp_make_local_1191_39_208(99,__context__) = true;
                _temp_make_local_1191_39_208(100,__context__) = true;
                _temp_make_local_1191_39_208(101,__context__) = true;
                _temp_make_local_1191_39_208(102,__context__) = true;
                _temp_make_local_1191_39_208(103,__context__) = true;
                _temp_make_local_1191_39_208(104,__context__) = true;
                _temp_make_local_1191_39_208(105,__context__) = true;
                _temp_make_local_1191_39_208(106,__context__) = true;
                _temp_make_local_1191_39_208(107,__context__) = true;
                _temp_make_local_1191_39_208(108,__context__) = true;
                _temp_make_local_1191_39_208(109,__context__) = true;
                _temp_make_local_1191_39_208(110,__context__) = true;
                _temp_make_local_1191_39_208(111,__context__) = true;
                _temp_make_local_1191_39_208(112,__context__) = true;
                _temp_make_local_1191_39_208(113,__context__) = true;
                _temp_make_local_1191_39_208(114,__context__) = true;
                _temp_make_local_1191_39_208(115,__context__) = true;
                _temp_make_local_1191_39_208(116,__context__) = true;
                _temp_make_local_1191_39_208(117,__context__) = true;
                _temp_make_local_1191_39_208(118,__context__) = true;
                _temp_make_local_1191_39_208(119,__context__) = true;
                _temp_make_local_1191_39_208(120,__context__) = true;
                _temp_make_local_1191_39_208(121,__context__) = true;
                _temp_make_local_1191_39_208(122,__context__) = true;
                _temp_make_local_1191_39_208(123,__context__) = false;
                _temp_make_local_1191_39_208(124,__context__) = false;
                _temp_make_local_1191_39_208(125,__context__) = false;
                _temp_make_local_1191_39_208(126,__context__) = false;
                _temp_make_local_1191_39_208(127,__context__) = false;
                _temp_make_local_1191_39_208(128,__context__) = false;
                _temp_make_local_1191_39_208(129,__context__) = false;
                _temp_make_local_1191_39_208(130,__context__) = false;
                _temp_make_local_1191_39_208(131,__context__) = false;
                _temp_make_local_1191_39_208(132,__context__) = false;
                _temp_make_local_1191_39_208(133,__context__) = false;
                _temp_make_local_1191_39_208(134,__context__) = false;
                _temp_make_local_1191_39_208(135,__context__) = false;
                _temp_make_local_1191_39_208(136,__context__) = false;
                _temp_make_local_1191_39_208(137,__context__) = false;
                _temp_make_local_1191_39_208(138,__context__) = false;
                _temp_make_local_1191_39_208(139,__context__) = false;
                _temp_make_local_1191_39_208(140,__context__) = false;
                _temp_make_local_1191_39_208(141,__context__) = false;
                _temp_make_local_1191_39_208(142,__context__) = false;
                _temp_make_local_1191_39_208(143,__context__) = false;
                _temp_make_local_1191_39_208(144,__context__) = false;
                _temp_make_local_1191_39_208(145,__context__) = false;
                _temp_make_local_1191_39_208(146,__context__) = false;
                _temp_make_local_1191_39_208(147,__context__) = false;
                _temp_make_local_1191_39_208(148,__context__) = false;
                _temp_make_local_1191_39_208(149,__context__) = false;
                _temp_make_local_1191_39_208(150,__context__) = false;
                _temp_make_local_1191_39_208(151,__context__) = false;
                _temp_make_local_1191_39_208(152,__context__) = false;
                _temp_make_local_1191_39_208(153,__context__) = false;
                _temp_make_local_1191_39_208(154,__context__) = false;
                _temp_make_local_1191_39_208(155,__context__) = false;
                _temp_make_local_1191_39_208(156,__context__) = false;
                _temp_make_local_1191_39_208(157,__context__) = false;
                _temp_make_local_1191_39_208(158,__context__) = false;
                _temp_make_local_1191_39_208(159,__context__) = false;
                _temp_make_local_1191_39_208(160,__context__) = false;
                _temp_make_local_1191_39_208(161,__context__) = false;
                _temp_make_local_1191_39_208(162,__context__) = false;
                _temp_make_local_1191_39_208(163,__context__) = false;
                _temp_make_local_1191_39_208(164,__context__) = false;
                _temp_make_local_1191_39_208(165,__context__) = false;
                _temp_make_local_1191_39_208(166,__context__) = false;
                _temp_make_local_1191_39_208(167,__context__) = false;
                _temp_make_local_1191_39_208(168,__context__) = false;
                _temp_make_local_1191_39_208(169,__context__) = false;
                _temp_make_local_1191_39_208(170,__context__) = false;
                _temp_make_local_1191_39_208(171,__context__) = false;
                _temp_make_local_1191_39_208(172,__context__) = false;
                _temp_make_local_1191_39_208(173,__context__) = false;
                _temp_make_local_1191_39_208(174,__context__) = false;
                _temp_make_local_1191_39_208(175,__context__) = false;
                _temp_make_local_1191_39_208(176,__context__) = false;
                _temp_make_local_1191_39_208(177,__context__) = false;
                _temp_make_local_1191_39_208(178,__context__) = false;
                _temp_make_local_1191_39_208(179,__context__) = false;
                _temp_make_local_1191_39_208(180,__context__) = false;
                _temp_make_local_1191_39_208(181,__context__) = false;
                _temp_make_local_1191_39_208(182,__context__) = false;
                _temp_make_local_1191_39_208(183,__context__) = false;
                _temp_make_local_1191_39_208(184,__context__) = false;
                _temp_make_local_1191_39_208(185,__context__) = false;
                _temp_make_local_1191_39_208(186,__context__) = false;
                _temp_make_local_1191_39_208(187,__context__) = false;
                _temp_make_local_1191_39_208(188,__context__) = false;
                _temp_make_local_1191_39_208(189,__context__) = false;
                _temp_make_local_1191_39_208(190,__context__) = false;
                _temp_make_local_1191_39_208(191,__context__) = false;
                _temp_make_local_1191_39_208(192,__context__) = false;
                _temp_make_local_1191_39_208(193,__context__) = false;
                _temp_make_local_1191_39_208(194,__context__) = false;
                _temp_make_local_1191_39_208(195,__context__) = false;
                _temp_make_local_1191_39_208(196,__context__) = false;
                _temp_make_local_1191_39_208(197,__context__) = false;
                _temp_make_local_1191_39_208(198,__context__) = false;
                _temp_make_local_1191_39_208(199,__context__) = false;
                _temp_make_local_1191_39_208(200,__context__) = false;
                _temp_make_local_1191_39_208(201,__context__) = false;
                _temp_make_local_1191_39_208(202,__context__) = false;
                _temp_make_local_1191_39_208(203,__context__) = false;
                _temp_make_local_1191_39_208(204,__context__) = false;
                _temp_make_local_1191_39_208(205,__context__) = false;
                _temp_make_local_1191_39_208(206,__context__) = false;
                _temp_make_local_1191_39_208(207,__context__) = false;
                _temp_make_local_1191_39_208(208,__context__) = false;
                _temp_make_local_1191_39_208(209,__context__) = false;
                _temp_make_local_1191_39_208(210,__context__) = false;
                _temp_make_local_1191_39_208(211,__context__) = false;
                _temp_make_local_1191_39_208(212,__context__) = false;
                _temp_make_local_1191_39_208(213,__context__) = false;
                _temp_make_local_1191_39_208(214,__context__) = false;
                _temp_make_local_1191_39_208(215,__context__) = false;
                _temp_make_local_1191_39_208(216,__context__) = false;
                _temp_make_local_1191_39_208(217,__context__) = false;
                _temp_make_local_1191_39_208(218,__context__) = false;
                _temp_make_local_1191_39_208(219,__context__) = false;
                _temp_make_local_1191_39_208(220,__context__) = false;
                _temp_make_local_1191_39_208(221,__context__) = false;
                _temp_make_local_1191_39_208(222,__context__) = false;
                _temp_make_local_1191_39_208(223,__context__) = false;
                _temp_make_local_1191_39_208(224,__context__) = false;
                _temp_make_local_1191_39_208(225,__context__) = false;
                _temp_make_local_1191_39_208(226,__context__) = false;
                _temp_make_local_1191_39_208(227,__context__) = false;
                _temp_make_local_1191_39_208(228,__context__) = false;
                _temp_make_local_1191_39_208(229,__context__) = false;
                _temp_make_local_1191_39_208(230,__context__) = false;
                _temp_make_local_1191_39_208(231,__context__) = false;
                _temp_make_local_1191_39_208(232,__context__) = false;
                _temp_make_local_1191_39_208(233,__context__) = false;
                _temp_make_local_1191_39_208(234,__context__) = false;
                _temp_make_local_1191_39_208(235,__context__) = false;
                _temp_make_local_1191_39_208(236,__context__) = false;
                _temp_make_local_1191_39_208(237,__context__) = false;
                _temp_make_local_1191_39_208(238,__context__) = false;
                _temp_make_local_1191_39_208(239,__context__) = false;
                _temp_make_local_1191_39_208(240,__context__) = false;
                _temp_make_local_1191_39_208(241,__context__) = false;
                _temp_make_local_1191_39_208(242,__context__) = false;
                _temp_make_local_1191_39_208(243,__context__) = false;
                _temp_make_local_1191_39_208(244,__context__) = false;
                _temp_make_local_1191_39_208(245,__context__) = false;
                _temp_make_local_1191_39_208(246,__context__) = false;
                _temp_make_local_1191_39_208(247,__context__) = false;
                _temp_make_local_1191_39_208(248,__context__) = false;
                _temp_make_local_1191_39_208(249,__context__) = false;
                _temp_make_local_1191_39_208(250,__context__) = false;
                _temp_make_local_1191_39_208(251,__context__) = false;
                _temp_make_local_1191_39_208(252,__context__) = false;
                _temp_make_local_1191_39_208(253,__context__) = false;
                _temp_make_local_1191_39_208(254,__context__) = false;
                _temp_make_local_1191_39_208(255,__context__) = false;
                return _temp_make_local_1191_39_208;
            })())(__ch_rename_at_1190_323,__context__));
        });
        if ( !___terminal_result_22_rename_at_1195_322 )
        {
            if ( (__parser_rename_at_362_318.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_318.suppress_errors)))) && (__parser_rename_at_362_318.index == __parser_rename_at_362_318.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1213_49_496; _temp_make_local_1213_49_496;
                char * __rest_rename_at_1211_324 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_58, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_318.index))),false));
                char * __complete_message_rename_at_1212_325 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_59, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '65'-'90'  '95'  '97'-'122' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1211_324))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_318.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1213_49_496.text),(__complete_message_rename_at_1212_325));
                    das_copy((_temp_make_local_1213_49_496.index),(__parser_rename_at_362_318.index));
                    return _temp_make_local_1213_49_496;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1216;
                das_zero(__mks_1216);
                return __mks_1216;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_318),1);
        };
        char * __str_2_rename_at_566_326 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_318.input(0,__context__))));
        char * __fc_rename_at_567_327 = (char *)(((char * const )(builtin_string_slice1(__str_2_rename_at_566_326,__ext_pos_2_rename_at_564_321,__parser_rename_at_362_318.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        int32_t __ext_pos_3_rename_at_564_328 = __parser_rename_at_362_318.index;
        _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_318.suppress_errors),true);
        while ( true )
        {
            AutoTuple<bool,int32_t,int32_t> __res_rename_at_644_329; das_zero(__res_rename_at_644_329); das_move(__res_rename_at_644_329, das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                bool ___terminal_result_23_rename_at_1195_330 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
                    TDim<bool,256> _temp_make_local_1191_39_736; _temp_make_local_1191_39_736;
                    int32_t __ch_rename_at_1190_331 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_318)));
                    return das_auto_cast<bool>::cast((__ch_rename_at_1190_331 != -1) && (([&]() -> TDim<bool,256>& {
                        _temp_make_local_1191_39_736(0,__context__) = false;
                        _temp_make_local_1191_39_736(1,__context__) = false;
                        _temp_make_local_1191_39_736(2,__context__) = false;
                        _temp_make_local_1191_39_736(3,__context__) = false;
                        _temp_make_local_1191_39_736(4,__context__) = false;
                        _temp_make_local_1191_39_736(5,__context__) = false;
                        _temp_make_local_1191_39_736(6,__context__) = false;
                        _temp_make_local_1191_39_736(7,__context__) = false;
                        _temp_make_local_1191_39_736(8,__context__) = false;
                        _temp_make_local_1191_39_736(9,__context__) = false;
                        _temp_make_local_1191_39_736(10,__context__) = false;
                        _temp_make_local_1191_39_736(11,__context__) = false;
                        _temp_make_local_1191_39_736(12,__context__) = false;
                        _temp_make_local_1191_39_736(13,__context__) = false;
                        _temp_make_local_1191_39_736(14,__context__) = false;
                        _temp_make_local_1191_39_736(15,__context__) = false;
                        _temp_make_local_1191_39_736(16,__context__) = false;
                        _temp_make_local_1191_39_736(17,__context__) = false;
                        _temp_make_local_1191_39_736(18,__context__) = false;
                        _temp_make_local_1191_39_736(19,__context__) = false;
                        _temp_make_local_1191_39_736(20,__context__) = false;
                        _temp_make_local_1191_39_736(21,__context__) = false;
                        _temp_make_local_1191_39_736(22,__context__) = false;
                        _temp_make_local_1191_39_736(23,__context__) = false;
                        _temp_make_local_1191_39_736(24,__context__) = false;
                        _temp_make_local_1191_39_736(25,__context__) = false;
                        _temp_make_local_1191_39_736(26,__context__) = false;
                        _temp_make_local_1191_39_736(27,__context__) = false;
                        _temp_make_local_1191_39_736(28,__context__) = false;
                        _temp_make_local_1191_39_736(29,__context__) = false;
                        _temp_make_local_1191_39_736(30,__context__) = false;
                        _temp_make_local_1191_39_736(31,__context__) = false;
                        _temp_make_local_1191_39_736(32,__context__) = false;
                        _temp_make_local_1191_39_736(33,__context__) = false;
                        _temp_make_local_1191_39_736(34,__context__) = false;
                        _temp_make_local_1191_39_736(35,__context__) = false;
                        _temp_make_local_1191_39_736(36,__context__) = false;
                        _temp_make_local_1191_39_736(37,__context__) = false;
                        _temp_make_local_1191_39_736(38,__context__) = false;
                        _temp_make_local_1191_39_736(39,__context__) = false;
                        _temp_make_local_1191_39_736(40,__context__) = false;
                        _temp_make_local_1191_39_736(41,__context__) = false;
                        _temp_make_local_1191_39_736(42,__context__) = false;
                        _temp_make_local_1191_39_736(43,__context__) = false;
                        _temp_make_local_1191_39_736(44,__context__) = false;
                        _temp_make_local_1191_39_736(45,__context__) = false;
                        _temp_make_local_1191_39_736(46,__context__) = false;
                        _temp_make_local_1191_39_736(47,__context__) = false;
                        _temp_make_local_1191_39_736(48,__context__) = true;
                        _temp_make_local_1191_39_736(49,__context__) = true;
                        _temp_make_local_1191_39_736(50,__context__) = true;
                        _temp_make_local_1191_39_736(51,__context__) = true;
                        _temp_make_local_1191_39_736(52,__context__) = true;
                        _temp_make_local_1191_39_736(53,__context__) = true;
                        _temp_make_local_1191_39_736(54,__context__) = true;
                        _temp_make_local_1191_39_736(55,__context__) = true;
                        _temp_make_local_1191_39_736(56,__context__) = true;
                        _temp_make_local_1191_39_736(57,__context__) = true;
                        _temp_make_local_1191_39_736(58,__context__) = false;
                        _temp_make_local_1191_39_736(59,__context__) = false;
                        _temp_make_local_1191_39_736(60,__context__) = false;
                        _temp_make_local_1191_39_736(61,__context__) = false;
                        _temp_make_local_1191_39_736(62,__context__) = false;
                        _temp_make_local_1191_39_736(63,__context__) = false;
                        _temp_make_local_1191_39_736(64,__context__) = false;
                        _temp_make_local_1191_39_736(65,__context__) = true;
                        _temp_make_local_1191_39_736(66,__context__) = true;
                        _temp_make_local_1191_39_736(67,__context__) = true;
                        _temp_make_local_1191_39_736(68,__context__) = true;
                        _temp_make_local_1191_39_736(69,__context__) = true;
                        _temp_make_local_1191_39_736(70,__context__) = true;
                        _temp_make_local_1191_39_736(71,__context__) = true;
                        _temp_make_local_1191_39_736(72,__context__) = true;
                        _temp_make_local_1191_39_736(73,__context__) = true;
                        _temp_make_local_1191_39_736(74,__context__) = true;
                        _temp_make_local_1191_39_736(75,__context__) = true;
                        _temp_make_local_1191_39_736(76,__context__) = true;
                        _temp_make_local_1191_39_736(77,__context__) = true;
                        _temp_make_local_1191_39_736(78,__context__) = true;
                        _temp_make_local_1191_39_736(79,__context__) = true;
                        _temp_make_local_1191_39_736(80,__context__) = true;
                        _temp_make_local_1191_39_736(81,__context__) = true;
                        _temp_make_local_1191_39_736(82,__context__) = true;
                        _temp_make_local_1191_39_736(83,__context__) = true;
                        _temp_make_local_1191_39_736(84,__context__) = true;
                        _temp_make_local_1191_39_736(85,__context__) = true;
                        _temp_make_local_1191_39_736(86,__context__) = true;
                        _temp_make_local_1191_39_736(87,__context__) = true;
                        _temp_make_local_1191_39_736(88,__context__) = true;
                        _temp_make_local_1191_39_736(89,__context__) = true;
                        _temp_make_local_1191_39_736(90,__context__) = true;
                        _temp_make_local_1191_39_736(91,__context__) = false;
                        _temp_make_local_1191_39_736(92,__context__) = false;
                        _temp_make_local_1191_39_736(93,__context__) = false;
                        _temp_make_local_1191_39_736(94,__context__) = false;
                        _temp_make_local_1191_39_736(95,__context__) = true;
                        _temp_make_local_1191_39_736(96,__context__) = false;
                        _temp_make_local_1191_39_736(97,__context__) = true;
                        _temp_make_local_1191_39_736(98,__context__) = true;
                        _temp_make_local_1191_39_736(99,__context__) = true;
                        _temp_make_local_1191_39_736(100,__context__) = true;
                        _temp_make_local_1191_39_736(101,__context__) = true;
                        _temp_make_local_1191_39_736(102,__context__) = true;
                        _temp_make_local_1191_39_736(103,__context__) = true;
                        _temp_make_local_1191_39_736(104,__context__) = true;
                        _temp_make_local_1191_39_736(105,__context__) = true;
                        _temp_make_local_1191_39_736(106,__context__) = true;
                        _temp_make_local_1191_39_736(107,__context__) = true;
                        _temp_make_local_1191_39_736(108,__context__) = true;
                        _temp_make_local_1191_39_736(109,__context__) = true;
                        _temp_make_local_1191_39_736(110,__context__) = true;
                        _temp_make_local_1191_39_736(111,__context__) = true;
                        _temp_make_local_1191_39_736(112,__context__) = true;
                        _temp_make_local_1191_39_736(113,__context__) = true;
                        _temp_make_local_1191_39_736(114,__context__) = true;
                        _temp_make_local_1191_39_736(115,__context__) = true;
                        _temp_make_local_1191_39_736(116,__context__) = true;
                        _temp_make_local_1191_39_736(117,__context__) = true;
                        _temp_make_local_1191_39_736(118,__context__) = true;
                        _temp_make_local_1191_39_736(119,__context__) = true;
                        _temp_make_local_1191_39_736(120,__context__) = true;
                        _temp_make_local_1191_39_736(121,__context__) = true;
                        _temp_make_local_1191_39_736(122,__context__) = true;
                        _temp_make_local_1191_39_736(123,__context__) = false;
                        _temp_make_local_1191_39_736(124,__context__) = false;
                        _temp_make_local_1191_39_736(125,__context__) = false;
                        _temp_make_local_1191_39_736(126,__context__) = false;
                        _temp_make_local_1191_39_736(127,__context__) = false;
                        _temp_make_local_1191_39_736(128,__context__) = false;
                        _temp_make_local_1191_39_736(129,__context__) = false;
                        _temp_make_local_1191_39_736(130,__context__) = false;
                        _temp_make_local_1191_39_736(131,__context__) = false;
                        _temp_make_local_1191_39_736(132,__context__) = false;
                        _temp_make_local_1191_39_736(133,__context__) = false;
                        _temp_make_local_1191_39_736(134,__context__) = false;
                        _temp_make_local_1191_39_736(135,__context__) = false;
                        _temp_make_local_1191_39_736(136,__context__) = false;
                        _temp_make_local_1191_39_736(137,__context__) = false;
                        _temp_make_local_1191_39_736(138,__context__) = false;
                        _temp_make_local_1191_39_736(139,__context__) = false;
                        _temp_make_local_1191_39_736(140,__context__) = false;
                        _temp_make_local_1191_39_736(141,__context__) = false;
                        _temp_make_local_1191_39_736(142,__context__) = false;
                        _temp_make_local_1191_39_736(143,__context__) = false;
                        _temp_make_local_1191_39_736(144,__context__) = false;
                        _temp_make_local_1191_39_736(145,__context__) = false;
                        _temp_make_local_1191_39_736(146,__context__) = false;
                        _temp_make_local_1191_39_736(147,__context__) = false;
                        _temp_make_local_1191_39_736(148,__context__) = false;
                        _temp_make_local_1191_39_736(149,__context__) = false;
                        _temp_make_local_1191_39_736(150,__context__) = false;
                        _temp_make_local_1191_39_736(151,__context__) = false;
                        _temp_make_local_1191_39_736(152,__context__) = false;
                        _temp_make_local_1191_39_736(153,__context__) = false;
                        _temp_make_local_1191_39_736(154,__context__) = false;
                        _temp_make_local_1191_39_736(155,__context__) = false;
                        _temp_make_local_1191_39_736(156,__context__) = false;
                        _temp_make_local_1191_39_736(157,__context__) = false;
                        _temp_make_local_1191_39_736(158,__context__) = false;
                        _temp_make_local_1191_39_736(159,__context__) = false;
                        _temp_make_local_1191_39_736(160,__context__) = false;
                        _temp_make_local_1191_39_736(161,__context__) = false;
                        _temp_make_local_1191_39_736(162,__context__) = false;
                        _temp_make_local_1191_39_736(163,__context__) = false;
                        _temp_make_local_1191_39_736(164,__context__) = false;
                        _temp_make_local_1191_39_736(165,__context__) = false;
                        _temp_make_local_1191_39_736(166,__context__) = false;
                        _temp_make_local_1191_39_736(167,__context__) = false;
                        _temp_make_local_1191_39_736(168,__context__) = false;
                        _temp_make_local_1191_39_736(169,__context__) = false;
                        _temp_make_local_1191_39_736(170,__context__) = false;
                        _temp_make_local_1191_39_736(171,__context__) = false;
                        _temp_make_local_1191_39_736(172,__context__) = false;
                        _temp_make_local_1191_39_736(173,__context__) = false;
                        _temp_make_local_1191_39_736(174,__context__) = false;
                        _temp_make_local_1191_39_736(175,__context__) = false;
                        _temp_make_local_1191_39_736(176,__context__) = false;
                        _temp_make_local_1191_39_736(177,__context__) = false;
                        _temp_make_local_1191_39_736(178,__context__) = false;
                        _temp_make_local_1191_39_736(179,__context__) = false;
                        _temp_make_local_1191_39_736(180,__context__) = false;
                        _temp_make_local_1191_39_736(181,__context__) = false;
                        _temp_make_local_1191_39_736(182,__context__) = false;
                        _temp_make_local_1191_39_736(183,__context__) = false;
                        _temp_make_local_1191_39_736(184,__context__) = false;
                        _temp_make_local_1191_39_736(185,__context__) = false;
                        _temp_make_local_1191_39_736(186,__context__) = false;
                        _temp_make_local_1191_39_736(187,__context__) = false;
                        _temp_make_local_1191_39_736(188,__context__) = false;
                        _temp_make_local_1191_39_736(189,__context__) = false;
                        _temp_make_local_1191_39_736(190,__context__) = false;
                        _temp_make_local_1191_39_736(191,__context__) = false;
                        _temp_make_local_1191_39_736(192,__context__) = false;
                        _temp_make_local_1191_39_736(193,__context__) = false;
                        _temp_make_local_1191_39_736(194,__context__) = false;
                        _temp_make_local_1191_39_736(195,__context__) = false;
                        _temp_make_local_1191_39_736(196,__context__) = false;
                        _temp_make_local_1191_39_736(197,__context__) = false;
                        _temp_make_local_1191_39_736(198,__context__) = false;
                        _temp_make_local_1191_39_736(199,__context__) = false;
                        _temp_make_local_1191_39_736(200,__context__) = false;
                        _temp_make_local_1191_39_736(201,__context__) = false;
                        _temp_make_local_1191_39_736(202,__context__) = false;
                        _temp_make_local_1191_39_736(203,__context__) = false;
                        _temp_make_local_1191_39_736(204,__context__) = false;
                        _temp_make_local_1191_39_736(205,__context__) = false;
                        _temp_make_local_1191_39_736(206,__context__) = false;
                        _temp_make_local_1191_39_736(207,__context__) = false;
                        _temp_make_local_1191_39_736(208,__context__) = false;
                        _temp_make_local_1191_39_736(209,__context__) = false;
                        _temp_make_local_1191_39_736(210,__context__) = false;
                        _temp_make_local_1191_39_736(211,__context__) = false;
                        _temp_make_local_1191_39_736(212,__context__) = false;
                        _temp_make_local_1191_39_736(213,__context__) = false;
                        _temp_make_local_1191_39_736(214,__context__) = false;
                        _temp_make_local_1191_39_736(215,__context__) = false;
                        _temp_make_local_1191_39_736(216,__context__) = false;
                        _temp_make_local_1191_39_736(217,__context__) = false;
                        _temp_make_local_1191_39_736(218,__context__) = false;
                        _temp_make_local_1191_39_736(219,__context__) = false;
                        _temp_make_local_1191_39_736(220,__context__) = false;
                        _temp_make_local_1191_39_736(221,__context__) = false;
                        _temp_make_local_1191_39_736(222,__context__) = false;
                        _temp_make_local_1191_39_736(223,__context__) = false;
                        _temp_make_local_1191_39_736(224,__context__) = false;
                        _temp_make_local_1191_39_736(225,__context__) = false;
                        _temp_make_local_1191_39_736(226,__context__) = false;
                        _temp_make_local_1191_39_736(227,__context__) = false;
                        _temp_make_local_1191_39_736(228,__context__) = false;
                        _temp_make_local_1191_39_736(229,__context__) = false;
                        _temp_make_local_1191_39_736(230,__context__) = false;
                        _temp_make_local_1191_39_736(231,__context__) = false;
                        _temp_make_local_1191_39_736(232,__context__) = false;
                        _temp_make_local_1191_39_736(233,__context__) = false;
                        _temp_make_local_1191_39_736(234,__context__) = false;
                        _temp_make_local_1191_39_736(235,__context__) = false;
                        _temp_make_local_1191_39_736(236,__context__) = false;
                        _temp_make_local_1191_39_736(237,__context__) = false;
                        _temp_make_local_1191_39_736(238,__context__) = false;
                        _temp_make_local_1191_39_736(239,__context__) = false;
                        _temp_make_local_1191_39_736(240,__context__) = false;
                        _temp_make_local_1191_39_736(241,__context__) = false;
                        _temp_make_local_1191_39_736(242,__context__) = false;
                        _temp_make_local_1191_39_736(243,__context__) = false;
                        _temp_make_local_1191_39_736(244,__context__) = false;
                        _temp_make_local_1191_39_736(245,__context__) = false;
                        _temp_make_local_1191_39_736(246,__context__) = false;
                        _temp_make_local_1191_39_736(247,__context__) = false;
                        _temp_make_local_1191_39_736(248,__context__) = false;
                        _temp_make_local_1191_39_736(249,__context__) = false;
                        _temp_make_local_1191_39_736(250,__context__) = false;
                        _temp_make_local_1191_39_736(251,__context__) = false;
                        _temp_make_local_1191_39_736(252,__context__) = false;
                        _temp_make_local_1191_39_736(253,__context__) = false;
                        _temp_make_local_1191_39_736(254,__context__) = false;
                        _temp_make_local_1191_39_736(255,__context__) = false;
                        return _temp_make_local_1191_39_736;
                    })())(__ch_rename_at_1190_331,__context__));
                });
                if ( !___terminal_result_23_rename_at_1195_330 )
                {
                    if ( (__parser_rename_at_362_318.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_318.suppress_errors)))) && (__parser_rename_at_362_318.index == __parser_rename_at_362_318.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1213_49_1024; _temp_make_local_1213_49_1024;
                        char * __rest_rename_at_1211_332 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_60, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_318.index))),false));
                        char * __complete_message_rename_at_1212_333 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_61, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '48'-'57'  '65'-'90'  '95' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1211_332))));
                        _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_318.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1213_49_1024.text),(__complete_message_rename_at_1212_333));
                            das_copy((_temp_make_local_1213_49_1024.index),(__parser_rename_at_362_318.index));
                            return _temp_make_local_1213_49_1024;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                        AutoTuple<bool,int32_t,int32_t> __mks_1216;
                        das_zero(__mks_1216);
                        return __mks_1216;
                    })()));
                } else {
                    _FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_362_318),1);
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
        _FuncbuiltinTickpopTick1161079256290593740_da418e66c1d8244e(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_318.suppress_errors));
        char * __str_3_rename_at_566_334 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_318.input(0,__context__))));
        char * __tail_rename_at_567_335 = (char *)(((char * const )(builtin_string_slice1(__str_3_rename_at_566_334,__ext_pos_3_rename_at_564_328,__parser_rename_at_362_318.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        char * __val_rename_at_1129_336; das_zero(__val_rename_at_1129_336); das_move(__val_rename_at_1129_336, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
            return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__fc_rename_at_567_327),cast<char *>::from(__tail_rename_at_567_335),*__context__,nullptr)));
        })));
        AutoTuple<bool,char *,int32_t> __result_rename_at_1137_337; das_zero(__result_rename_at_1137_337); das_move(__result_rename_at_1137_337, (([&]() -> AutoTuple<bool,char *,int32_t> {
            AutoTuple<bool,char *,int32_t> __mkt_1137;
            das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
            das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_336;
            das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_318.index;
            return __mkt_1137;
        })()));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_337);
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

inline AutoTuple<bool,char *,int32_t> _Funcparse_ident_Tickid_0x0_20b6fcdb44c1b1c0 ( Context * __context__, spoof::invocationParserTickid_0x0 &  __parser_rename_at_146_338 )
{
    int32_t __mark_rename_at_147_339 = __parser_rename_at_146_338.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_338.ident__cache),__parser_rename_at_146_338.index) && !(__parser_rename_at_146_338.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_340; das_zero(__result_rename_at_155_340); das_move(__result_rename_at_155_340, _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_338.ident__cache(__parser_rename_at_146_338.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_340) )
        {
            das_copy(__parser_rename_at_146_338.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_340));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_340);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_341; das_zero(__result_rename_at_169_341); das_move(__result_rename_at_169_341, _Funcparse_ident__innerTickid_0x0_79268295f94867d7(__context__,das_arg<spoof::invocationParserTickid_0x0>::pass(__parser_rename_at_146_338)));
        das_copy(__parser_rename_at_146_338.ident__cache(__mark_rename_at_147_339,__context__),__result_rename_at_169_341);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_341);
    };
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __it_rename_at_1223_342, int32_t & __value_rename_at_1223_343 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1223_342),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1223_343)),__context__));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_344, uint8_t __value_rename_at_165_345 )
{
    das_copy(__Arr_rename_at_165_344(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_344),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_345);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_346, char * const  __value_rename_at_165_347 )
{
    das_copy(__Arr_rename_at_165_346(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_346),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_347);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_181_348, char * __value_rename_at_181_349 )
{
    das_copy(__Arr_rename_at_181_348(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_181_348),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_349);
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5 ( Context * __context__, char * const  __str_rename_at_1256_350 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1257_351;das_zero(__it_rename_at_1257_351);
    builtin_make_string_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1257_351),__str_rename_at_1256_350,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1257_351);
}

inline int32_t _FuncbuiltinTickfind_index_ifTick5144745413123287381_d7241f91c631a1ce ( Context * __context__, TArray<AutoTuple<char *,char *>> const  &  __arr_rename_at_1676_352, Block DAS_COMMENT((bool,AutoTuple<char *,char *> const )) const  &  __blk_rename_at_1676_353 )
{
    {
        bool __need_loop_1677 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(builtin_array_size(__arr_rename_at_1676_352)));
        int32_t __i_rename_at_1677_354;
        __need_loop_1677 = __i_iterator.first(__context__,(__i_rename_at_1677_354)) && __need_loop_1677;
        for ( ; __need_loop_1677 ; __need_loop_1677 = __i_iterator.next(__context__,(__i_rename_at_1677_354)) )
        {
            if ( das_invoke<bool>::invoke<AutoTuple<char *,char *> const  &>(__context__,nullptr,__blk_rename_at_1676_353,__arr_rename_at_1676_352(__i_rename_at_1677_354,__context__)) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1677_354);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1677_354));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _Funcparse_macro_invocation_innerTickid_0x1_fc3224332b02420d ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_355 )
{
    AutoTuple<bool,spoof::SpoofInvocation,int32_t> _temp_make_local_364_18_1120; _temp_make_local_364_18_1120;
    int32_t __parse_pos_rename_at_1076_356 = __parser_rename_at_362_355.index;
    ++__parser_rename_at_362_355.tabs;
    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __res_1_rename_at_1086_357; das_zero(__res_1_rename_at_1086_357); das_move(__res_1_rename_at_1086_357, das_invoke<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,spoof::SpoofInvocation,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
        AutoTuple<bool,char *,int32_t> __class_name0_rename_at_546_358; das_zero(__class_name0_rename_at_546_358); das_move(__class_name0_rename_at_546_358, _Funcparse_class_nameTickid_0x1_ecd2c3e791924e42(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__class_name0_rename_at_546_358) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __cn_rename_at_551_359; das_zero(__cn_rename_at_551_359); das_move(__cn_rename_at_551_359, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__class_name0_rename_at_546_358)));
            _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
            if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355),((char *) "("),1) )
            {
                if ( (__parser_rename_at_362_355.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_355.suppress_errors)))) && (__parser_rename_at_362_355.index == __parser_rename_at_362_355.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1171_49_256; _temp_make_local_1171_49_256;
                    char * __rest_rename_at_1169_360 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_62, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_355.index))),false));
                    char * __complete_message_rename_at_1170_361 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_63, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_360))));
                    _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_355.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1171_49_256.text),(__complete_message_rename_at_1170_361));
                        das_copy((_temp_make_local_1171_49_256.index),(__parser_rename_at_362_355.index));
                        return _temp_make_local_1171_49_256;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_1175;
                    das_zero(__mks_1175);
                    return __mks_1175;
                })()));
            } else {
                AutoTuple<bool,TArray<char *>,int32_t> __element_list0_rename_at_546_362; das_zero(__element_list0_rename_at_546_362); das_move(__element_list0_rename_at_546_362, _Funcparse_element_listTickid_0x1_f9f95d60cca895e9(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355)));
                if ( !das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__element_list0_rename_at_546_362) )
                {
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_548;
                        das_zero(__mks_548);
                        return __mks_548;
                    })()));
                } else {
                    TArray<char *> __el_rename_at_551_363; das_zero(__el_rename_at_551_363); das_move(__el_rename_at_551_363, das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(__element_list0_rename_at_546_362));
                    _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
                    if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355),((char *) ")"),1) )
                    {
                        if ( (__parser_rename_at_362_355.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_355.suppress_errors)))) && (__parser_rename_at_362_355.index == __parser_rename_at_362_355.longest_prefix) )
                        {
                            peg::ParsingError _temp_make_local_1171_49_416; _temp_make_local_1171_49_416;
                            char * __rest_rename_at_1169_364 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_64, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_355.index))),false));
                            char * __complete_message_rename_at_1170_365 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_65, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_364))));
                            _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_355.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                                das_copy((_temp_make_local_1171_49_416.text),(__complete_message_rename_at_1170_365));
                                das_copy((_temp_make_local_1171_49_416.index),(__parser_rename_at_362_355.index));
                                return _temp_make_local_1171_49_416;
                            })())));
                        };
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                            AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_1175;
                            das_zero(__mks_1175);
                            return __mks_1175;
                        })()));
                    } else {
                        spoof::SpoofInvocation __val_rename_at_1129_366; das_zero(__val_rename_at_1129_366); das_move(__val_rename_at_1129_366, das_invoke<spoof::SpoofInvocation>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> spoof::SpoofInvocation{
                            spoof::SpoofInvocation _temp_make_local_181_22_544; _temp_make_local_181_22_544;
                            return /* <- */ das_auto_cast_move<spoof::SpoofInvocation>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8860640a3da483d(__context__,das_arg<spoof::SpoofInvocation>::pass((([&]() -> spoof::SpoofInvocation& {
                                das_copy((_temp_make_local_181_22_544.varName),(__cn_rename_at_551_359));
                                das_move((_temp_make_local_181_22_544.args),(__el_rename_at_551_363));
                                return _temp_make_local_181_22_544;
                            })()))));
                        }));
                        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_1137_367; das_zero(__result_rename_at_1137_367); das_move(__result_rename_at_1137_367, (([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                            AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mkt_1137;
                            das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1137) = true;
                            das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1137) = __val_rename_at_1129_366;
                            das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1137) = __parser_rename_at_362_355.index;
                            return __mkt_1137;
                        })()));
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__result_rename_at_1137_367);
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
        _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
        AutoTuple<bool,char *,int32_t> __class_name1_rename_at_546_370; das_zero(__class_name1_rename_at_546_370); das_move(__class_name1_rename_at_546_370, _Funcparse_class_nameTickid_0x1_ecd2c3e791924e42(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__class_name1_rename_at_546_370) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __cn_rename_at_551_371; das_zero(__cn_rename_at_551_371); das_move(__cn_rename_at_551_371, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__class_name1_rename_at_546_370)));
            _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
            if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355),((char *) "("),1) )
            {
                if ( (__parser_rename_at_362_355.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_355.suppress_errors)))) && (__parser_rename_at_362_355.index == __parser_rename_at_362_355.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1171_49_848; _temp_make_local_1171_49_848;
                    char * __rest_rename_at_1169_372 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_66, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_355.index))),false));
                    char * __complete_message_rename_at_1170_373 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_67, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_372))));
                    _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_355.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1171_49_848.text),(__complete_message_rename_at_1170_373));
                        das_copy((_temp_make_local_1171_49_848.index),(__parser_rename_at_362_355.index));
                        return _temp_make_local_1171_49_848;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_1175;
                    das_zero(__mks_1175);
                    return __mks_1175;
                })()));
            } else {
                _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355));
                if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_355),((char *) ")"),1) )
                {
                    if ( (__parser_rename_at_362_355.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_355.suppress_errors)))) && (__parser_rename_at_362_355.index == __parser_rename_at_362_355.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1171_49_912; _temp_make_local_1171_49_912;
                        char * __rest_rename_at_1169_374 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_68, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_355.index))),false));
                        char * __complete_message_rename_at_1170_375 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_69, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_374))));
                        _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_355.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1171_49_912.text),(__complete_message_rename_at_1170_375));
                            das_copy((_temp_make_local_1171_49_912.index),(__parser_rename_at_362_355.index));
                            return _temp_make_local_1171_49_912;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mks_1175;
                        das_zero(__mks_1175);
                        return __mks_1175;
                    })()));
                } else {
                    spoof::SpoofInvocation __val_rename_at_1129_376; das_zero(__val_rename_at_1129_376); das_move(__val_rename_at_1129_376, das_invoke<spoof::SpoofInvocation>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> spoof::SpoofInvocation{
                        spoof::SpoofInvocation _temp_make_local_184_22_1040; _temp_make_local_184_22_1040;
                        return /* <- */ das_auto_cast_move<spoof::SpoofInvocation>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8860640a3da483d(__context__,das_arg<spoof::SpoofInvocation>::pass((([&]() -> spoof::SpoofInvocation& {
                            das_zero(_temp_make_local_184_22_1040);
                            das_copy((_temp_make_local_184_22_1040.varName),(__cn_rename_at_551_371));
                            return _temp_make_local_184_22_1040;
                        })()))));
                    }));
                    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_1137_377; das_zero(__result_rename_at_1137_377); das_move(__result_rename_at_1137_377, (([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t> {
                        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __mkt_1137;
                        das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1137) = true;
                        das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1137) = __val_rename_at_1129_376;
                        das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__mkt_1137) = __parser_rename_at_362_355.index;
                        return __mkt_1137;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__result_rename_at_1137_377);
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
    return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e53981a5c0b0dc1(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass((([&]() -> AutoTuple<bool,spoof::SpoofInvocation,int32_t>& {
        das_zero(_temp_make_local_364_18_1120);
        return _temp_make_local_364_18_1120;
    })()))));
}

inline AutoTuple<bool,spoof::SpoofInvocation,int32_t> _Funcparse_macro_invocationTickid_0x1_e41312d760106bd0 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_378 )
{
    int32_t __mark_rename_at_147_379 = __parser_rename_at_146_378.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_33cfc90a36a139ab(__context__,das_arg<TTable<int32_t,AutoTuple<bool,spoof::SpoofInvocation,int32_t>>>::pass(__parser_rename_at_146_378.macro_invocation_cache),__parser_rename_at_146_378.index) && !(__parser_rename_at_146_378.error_reporting) )
    {
        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_155_380; das_zero(__result_rename_at_155_380); das_move(__result_rename_at_155_380, _FuncbuiltinTickclone_to_moveTick2007252383599261567_9dc279ef3efd2937(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__parser_rename_at_146_378.macro_invocation_cache(__parser_rename_at_146_378.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_155_380) )
        {
            das_copy(__parser_rename_at_146_378.index,das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_155_380));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__result_rename_at_155_380);
    } else {
        AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_169_381; das_zero(__result_rename_at_169_381); das_move(__result_rename_at_169_381, _Funcparse_macro_invocation_innerTickid_0x1_fc3224332b02420d(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_378)));
        clone_33841e01eec4fe3e(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__parser_rename_at_146_378.macro_invocation_cache(__mark_rename_at_147_379,__context__)),das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__result_rename_at_169_381));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::cast(__result_rename_at_169_381);
    };
}

inline AutoTuple<bool,TArray<char *>,int32_t> _Funcparse_element_list_innerTickid_0x1_4ddd8d5780e55324 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_382 )
{
    AutoTuple<bool,TArray<char *>,int32_t> _temp_make_local_364_18_528; _temp_make_local_364_18_528;
    int32_t __parse_pos_rename_at_1076_383 = __parser_rename_at_362_382.index;
    ++__parser_rename_at_362_382.tabs;
    AutoTuple<bool,TArray<char *>,int32_t> __res_1_rename_at_1086_384; das_zero(__res_1_rename_at_1086_384); das_move(__res_1_rename_at_1086_384, das_invoke<AutoTuple<bool,TArray<char *>,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,TArray<char *>,int32_t>{
        TArray<char *> __els_rename_at_671_385;das_zero(__els_rename_at_671_385);
        _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_382.suppress_errors),true);
        while ( true )
        {
            int32_t __sz_rename_at_674_386 = builtin_array_size(das_arg<TArray<char *>>::pass(__els_rename_at_671_385));
            das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                AutoTuple<bool,char *,int32_t> __comma_separated_elements0_rename_at_546_387; das_zero(__comma_separated_elements0_rename_at_546_387); das_move(__comma_separated_elements0_rename_at_546_387, _Funcparse_comma_separated_elementsTickid_0x1_a5eb4f00d7b7be(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_382)));
                if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__comma_separated_elements0_rename_at_546_387) )
                {
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                        AutoTuple<bool,int32_t,int32_t> __mks_548;
                        das_zero(__mks_548);
                        return __mks_548;
                    })()));
                } else {
                    char * __temp_name_rename_at_551_388; das_zero(__temp_name_rename_at_551_388); das_move(__temp_name_rename_at_551_388, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__comma_separated_elements0_rename_at_546_387)));
                    _FuncbuiltinTickemplaceTick13923705686753630697_664b3b92dd33b520(__context__,das_arg<TArray<char *>>::pass(__els_rename_at_671_385),__temp_name_rename_at_551_388);
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
        _FuncbuiltinTickpopTick1161079256290593740_da418e66c1d8244e(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_382.suppress_errors));
        _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_382));
        AutoTuple<bool,char *,int32_t> __element0_rename_at_546_389; das_zero(__element0_rename_at_546_389); das_move(__element0_rename_at_546_389, _Funcparse_elementTickid_0x1_609c6e555a4e4af4(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_382)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__element0_rename_at_546_389) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast((([&]() -> AutoTuple<bool,TArray<char *>,int32_t> {
                AutoTuple<bool,TArray<char *>,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __last_rename_at_551_390; das_zero(__last_rename_at_551_390); das_move(__last_rename_at_551_390, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__element0_rename_at_546_389)));
            TArray<char *> __val_rename_at_1129_391; das_zero(__val_rename_at_1129_391); das_move(__val_rename_at_1129_391, das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
                _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1(__context__,das_arg<TArray<char *>>::pass(__els_rename_at_671_385),__last_rename_at_551_390);
                return /* <- */ das_auto_cast_move<TArray<char *>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_32af3ec8801e47ea(__context__,das_arg<TArray<char *>>::pass(__els_rename_at_671_385)));
            }));
            AutoTuple<bool,TArray<char *>,int32_t> __result_rename_at_1137_392; das_zero(__result_rename_at_1137_392); das_move(__result_rename_at_1137_392, (([&]() -> AutoTuple<bool,TArray<char *>,int32_t> {
                AutoTuple<bool,TArray<char *>,int32_t> __mkt_1137;
                das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__mkt_1137) = true;
                das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(__mkt_1137) = __val_rename_at_1129_391;
                das_get_auto_tuple_field<int32_t,2,bool,TArray<char *>,int32_t>::get(__mkt_1137) = __parser_rename_at_362_382.index;
                return __mkt_1137;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(__result_rename_at_1137_392);
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
    return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a887f80fd036fc6a(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass((([&]() -> AutoTuple<bool,TArray<char *>,int32_t>& {
        das_zero(_temp_make_local_364_18_528);
        return _temp_make_local_364_18_528;
    })()))));
}

inline AutoTuple<bool,TArray<char *>,int32_t> _Funcparse_element_listTickid_0x1_f9f95d60cca895e9 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_393 )
{
    int32_t __mark_rename_at_147_394 = __parser_rename_at_146_393.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_47edd36ebc80950e(__context__,das_arg<TTable<int32_t,AutoTuple<bool,TArray<char *>,int32_t>>>::pass(__parser_rename_at_146_393.element_list_cache),__parser_rename_at_146_393.index) && !(__parser_rename_at_146_393.error_reporting) )
    {
        AutoTuple<bool,TArray<char *>,int32_t> __result_rename_at_155_395; das_zero(__result_rename_at_155_395); das_move(__result_rename_at_155_395, _FuncbuiltinTickclone_to_moveTick2007252383599261567_3e547859692e768f(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass(__parser_rename_at_146_393.element_list_cache(__parser_rename_at_146_393.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__result_rename_at_155_395) )
        {
            das_copy(__parser_rename_at_146_393.index,das_get_auto_tuple_field<int32_t,2,bool,TArray<char *>,int32_t>::get(__result_rename_at_155_395));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(__result_rename_at_155_395);
    } else {
        AutoTuple<bool,TArray<char *>,int32_t> __result_rename_at_169_396; das_zero(__result_rename_at_169_396); das_move(__result_rename_at_169_396, _Funcparse_element_list_innerTickid_0x1_4ddd8d5780e55324(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_393)));
        clone_2562f74cf119ba95(__context__,das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass(__parser_rename_at_146_393.element_list_cache(__mark_rename_at_147_394,__context__)),das_arg<AutoTuple<bool,TArray<char *>,int32_t>>::pass(__result_rename_at_169_396));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,TArray<char *>,int32_t>>::cast(__result_rename_at_169_396);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_comma_separated_elements_innerTickid_0x1_4a764230365b1982 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_397 )
{
    int32_t __parse_pos_rename_at_1076_398 = __parser_rename_at_362_397.index;
    ++__parser_rename_at_362_397.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_399; das_zero(__res_1_rename_at_1086_399); das_move(__res_1_rename_at_1086_399, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_397));
        AutoTuple<bool,char *,int32_t> __element1_rename_at_546_400; das_zero(__element1_rename_at_546_400); das_move(__element1_rename_at_546_400, _Funcparse_elementTickid_0x1_609c6e555a4e4af4(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_397)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__element1_rename_at_546_400) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __e_rename_at_551_401; das_zero(__e_rename_at_551_401); das_move(__e_rename_at_551_401, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__element1_rename_at_546_400)));
            if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_397),((char *) ","),1) )
            {
                if ( (__parser_rename_at_362_397.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_397.suppress_errors)))) && (__parser_rename_at_362_397.index == __parser_rename_at_362_397.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1171_49_208; _temp_make_local_1171_49_208;
                    char * __rest_rename_at_1169_402 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_70, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_397.index))),false));
                    char * __complete_message_rename_at_1170_403 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_71, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m,\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_402))));
                    _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_397.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1171_49_208.text),(__complete_message_rename_at_1170_403));
                        das_copy((_temp_make_local_1171_49_208.index),(__parser_rename_at_362_397.index));
                        return _temp_make_local_1171_49_208;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1175;
                    das_zero(__mks_1175);
                    return __mks_1175;
                })()));
            } else {
                char * __val_rename_at_1129_404; das_zero(__val_rename_at_1129_404); das_move(__val_rename_at_1129_404, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                    return das_auto_cast<char *>::cast(__e_rename_at_551_401);
                })));
                AutoTuple<bool,char *,int32_t> __result_rename_at_1137_405; das_zero(__result_rename_at_1137_405); das_move(__result_rename_at_1137_405, (([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mkt_1137;
                    das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                    das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_404;
                    das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_397.index;
                    return __mkt_1137;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_405);
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

inline AutoTuple<bool,char *,int32_t> _Funcparse_comma_separated_elementsTickid_0x1_a5eb4f00d7b7be ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_406 )
{
    int32_t __mark_rename_at_147_407 = __parser_rename_at_146_406.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_406.comma_separated_elements_cache),__parser_rename_at_146_406.index) && !(__parser_rename_at_146_406.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_408; das_zero(__result_rename_at_155_408); das_move(__result_rename_at_155_408, _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_406.comma_separated_elements_cache(__parser_rename_at_146_406.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_408) )
        {
            das_copy(__parser_rename_at_146_406.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_408));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_408);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_409; das_zero(__result_rename_at_169_409); das_move(__result_rename_at_169_409, _Funcparse_comma_separated_elements_innerTickid_0x1_4a764230365b1982(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_406)));
        das_copy(__parser_rename_at_146_406.comma_separated_elements_cache(__mark_rename_at_147_407,__context__),__result_rename_at_169_409);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_409);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_element_innerTickid_0x1_cb2b3527b22dde21 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_410 )
{
    int32_t __parse_pos_rename_at_1076_411 = __parser_rename_at_362_410.index;
    ++__parser_rename_at_362_410.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_412; das_zero(__res_1_rename_at_1086_412); das_move(__res_1_rename_at_1086_412, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),((char *) "\\"),1) )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1171_49_176; _temp_make_local_1171_49_176;
                char * __rest_rename_at_1169_413 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_72, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                char * __complete_message_rename_at_1170_414 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_73, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m\\\\\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_413))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1171_49_176.text),(__complete_message_rename_at_1170_414));
                    das_copy((_temp_make_local_1171_49_176.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1171_49_176;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1175;
                das_zero(__mks_1175);
                return __mks_1175;
            })()));
        } else {
            int32_t __ext_pos_0_rename_at_564_415 = __parser_rename_at_362_410.index;
            bool ___terminal_result_14_rename_at_1195_416 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
                TDim<bool,256> _temp_make_local_1191_39_304; _temp_make_local_1191_39_304;
                int32_t __ch_rename_at_1190_417 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
                return das_auto_cast<bool>::cast((__ch_rename_at_1190_417 != -1) && (([&]() -> TDim<bool,256>& {
                    _temp_make_local_1191_39_304(0,__context__) = true;
                    _temp_make_local_1191_39_304(1,__context__) = true;
                    _temp_make_local_1191_39_304(2,__context__) = true;
                    _temp_make_local_1191_39_304(3,__context__) = true;
                    _temp_make_local_1191_39_304(4,__context__) = true;
                    _temp_make_local_1191_39_304(5,__context__) = true;
                    _temp_make_local_1191_39_304(6,__context__) = true;
                    _temp_make_local_1191_39_304(7,__context__) = true;
                    _temp_make_local_1191_39_304(8,__context__) = true;
                    _temp_make_local_1191_39_304(9,__context__) = true;
                    _temp_make_local_1191_39_304(10,__context__) = true;
                    _temp_make_local_1191_39_304(11,__context__) = true;
                    _temp_make_local_1191_39_304(12,__context__) = true;
                    _temp_make_local_1191_39_304(13,__context__) = true;
                    _temp_make_local_1191_39_304(14,__context__) = true;
                    _temp_make_local_1191_39_304(15,__context__) = true;
                    _temp_make_local_1191_39_304(16,__context__) = true;
                    _temp_make_local_1191_39_304(17,__context__) = true;
                    _temp_make_local_1191_39_304(18,__context__) = true;
                    _temp_make_local_1191_39_304(19,__context__) = true;
                    _temp_make_local_1191_39_304(20,__context__) = true;
                    _temp_make_local_1191_39_304(21,__context__) = true;
                    _temp_make_local_1191_39_304(22,__context__) = true;
                    _temp_make_local_1191_39_304(23,__context__) = true;
                    _temp_make_local_1191_39_304(24,__context__) = true;
                    _temp_make_local_1191_39_304(25,__context__) = true;
                    _temp_make_local_1191_39_304(26,__context__) = true;
                    _temp_make_local_1191_39_304(27,__context__) = true;
                    _temp_make_local_1191_39_304(28,__context__) = true;
                    _temp_make_local_1191_39_304(29,__context__) = true;
                    _temp_make_local_1191_39_304(30,__context__) = true;
                    _temp_make_local_1191_39_304(31,__context__) = true;
                    _temp_make_local_1191_39_304(32,__context__) = true;
                    _temp_make_local_1191_39_304(33,__context__) = true;
                    _temp_make_local_1191_39_304(34,__context__) = true;
                    _temp_make_local_1191_39_304(35,__context__) = true;
                    _temp_make_local_1191_39_304(36,__context__) = true;
                    _temp_make_local_1191_39_304(37,__context__) = true;
                    _temp_make_local_1191_39_304(38,__context__) = true;
                    _temp_make_local_1191_39_304(39,__context__) = true;
                    _temp_make_local_1191_39_304(40,__context__) = true;
                    _temp_make_local_1191_39_304(41,__context__) = true;
                    _temp_make_local_1191_39_304(42,__context__) = true;
                    _temp_make_local_1191_39_304(43,__context__) = true;
                    _temp_make_local_1191_39_304(44,__context__) = true;
                    _temp_make_local_1191_39_304(45,__context__) = true;
                    _temp_make_local_1191_39_304(46,__context__) = true;
                    _temp_make_local_1191_39_304(47,__context__) = true;
                    _temp_make_local_1191_39_304(48,__context__) = true;
                    _temp_make_local_1191_39_304(49,__context__) = true;
                    _temp_make_local_1191_39_304(50,__context__) = true;
                    _temp_make_local_1191_39_304(51,__context__) = true;
                    _temp_make_local_1191_39_304(52,__context__) = true;
                    _temp_make_local_1191_39_304(53,__context__) = true;
                    _temp_make_local_1191_39_304(54,__context__) = true;
                    _temp_make_local_1191_39_304(55,__context__) = true;
                    _temp_make_local_1191_39_304(56,__context__) = true;
                    _temp_make_local_1191_39_304(57,__context__) = true;
                    _temp_make_local_1191_39_304(58,__context__) = true;
                    _temp_make_local_1191_39_304(59,__context__) = true;
                    _temp_make_local_1191_39_304(60,__context__) = true;
                    _temp_make_local_1191_39_304(61,__context__) = true;
                    _temp_make_local_1191_39_304(62,__context__) = true;
                    _temp_make_local_1191_39_304(63,__context__) = true;
                    _temp_make_local_1191_39_304(64,__context__) = true;
                    _temp_make_local_1191_39_304(65,__context__) = true;
                    _temp_make_local_1191_39_304(66,__context__) = true;
                    _temp_make_local_1191_39_304(67,__context__) = true;
                    _temp_make_local_1191_39_304(68,__context__) = true;
                    _temp_make_local_1191_39_304(69,__context__) = true;
                    _temp_make_local_1191_39_304(70,__context__) = true;
                    _temp_make_local_1191_39_304(71,__context__) = true;
                    _temp_make_local_1191_39_304(72,__context__) = true;
                    _temp_make_local_1191_39_304(73,__context__) = true;
                    _temp_make_local_1191_39_304(74,__context__) = true;
                    _temp_make_local_1191_39_304(75,__context__) = true;
                    _temp_make_local_1191_39_304(76,__context__) = true;
                    _temp_make_local_1191_39_304(77,__context__) = true;
                    _temp_make_local_1191_39_304(78,__context__) = true;
                    _temp_make_local_1191_39_304(79,__context__) = true;
                    _temp_make_local_1191_39_304(80,__context__) = true;
                    _temp_make_local_1191_39_304(81,__context__) = true;
                    _temp_make_local_1191_39_304(82,__context__) = true;
                    _temp_make_local_1191_39_304(83,__context__) = true;
                    _temp_make_local_1191_39_304(84,__context__) = true;
                    _temp_make_local_1191_39_304(85,__context__) = true;
                    _temp_make_local_1191_39_304(86,__context__) = true;
                    _temp_make_local_1191_39_304(87,__context__) = true;
                    _temp_make_local_1191_39_304(88,__context__) = true;
                    _temp_make_local_1191_39_304(89,__context__) = true;
                    _temp_make_local_1191_39_304(90,__context__) = true;
                    _temp_make_local_1191_39_304(91,__context__) = true;
                    _temp_make_local_1191_39_304(92,__context__) = true;
                    _temp_make_local_1191_39_304(93,__context__) = true;
                    _temp_make_local_1191_39_304(94,__context__) = true;
                    _temp_make_local_1191_39_304(95,__context__) = true;
                    _temp_make_local_1191_39_304(96,__context__) = true;
                    _temp_make_local_1191_39_304(97,__context__) = true;
                    _temp_make_local_1191_39_304(98,__context__) = true;
                    _temp_make_local_1191_39_304(99,__context__) = true;
                    _temp_make_local_1191_39_304(100,__context__) = true;
                    _temp_make_local_1191_39_304(101,__context__) = true;
                    _temp_make_local_1191_39_304(102,__context__) = true;
                    _temp_make_local_1191_39_304(103,__context__) = true;
                    _temp_make_local_1191_39_304(104,__context__) = true;
                    _temp_make_local_1191_39_304(105,__context__) = true;
                    _temp_make_local_1191_39_304(106,__context__) = true;
                    _temp_make_local_1191_39_304(107,__context__) = true;
                    _temp_make_local_1191_39_304(108,__context__) = true;
                    _temp_make_local_1191_39_304(109,__context__) = true;
                    _temp_make_local_1191_39_304(110,__context__) = true;
                    _temp_make_local_1191_39_304(111,__context__) = true;
                    _temp_make_local_1191_39_304(112,__context__) = true;
                    _temp_make_local_1191_39_304(113,__context__) = true;
                    _temp_make_local_1191_39_304(114,__context__) = true;
                    _temp_make_local_1191_39_304(115,__context__) = true;
                    _temp_make_local_1191_39_304(116,__context__) = true;
                    _temp_make_local_1191_39_304(117,__context__) = true;
                    _temp_make_local_1191_39_304(118,__context__) = true;
                    _temp_make_local_1191_39_304(119,__context__) = true;
                    _temp_make_local_1191_39_304(120,__context__) = true;
                    _temp_make_local_1191_39_304(121,__context__) = true;
                    _temp_make_local_1191_39_304(122,__context__) = true;
                    _temp_make_local_1191_39_304(123,__context__) = true;
                    _temp_make_local_1191_39_304(124,__context__) = true;
                    _temp_make_local_1191_39_304(125,__context__) = true;
                    _temp_make_local_1191_39_304(126,__context__) = true;
                    _temp_make_local_1191_39_304(127,__context__) = true;
                    _temp_make_local_1191_39_304(128,__context__) = true;
                    _temp_make_local_1191_39_304(129,__context__) = true;
                    _temp_make_local_1191_39_304(130,__context__) = true;
                    _temp_make_local_1191_39_304(131,__context__) = true;
                    _temp_make_local_1191_39_304(132,__context__) = true;
                    _temp_make_local_1191_39_304(133,__context__) = true;
                    _temp_make_local_1191_39_304(134,__context__) = true;
                    _temp_make_local_1191_39_304(135,__context__) = true;
                    _temp_make_local_1191_39_304(136,__context__) = true;
                    _temp_make_local_1191_39_304(137,__context__) = true;
                    _temp_make_local_1191_39_304(138,__context__) = true;
                    _temp_make_local_1191_39_304(139,__context__) = true;
                    _temp_make_local_1191_39_304(140,__context__) = true;
                    _temp_make_local_1191_39_304(141,__context__) = true;
                    _temp_make_local_1191_39_304(142,__context__) = true;
                    _temp_make_local_1191_39_304(143,__context__) = true;
                    _temp_make_local_1191_39_304(144,__context__) = true;
                    _temp_make_local_1191_39_304(145,__context__) = true;
                    _temp_make_local_1191_39_304(146,__context__) = true;
                    _temp_make_local_1191_39_304(147,__context__) = true;
                    _temp_make_local_1191_39_304(148,__context__) = true;
                    _temp_make_local_1191_39_304(149,__context__) = true;
                    _temp_make_local_1191_39_304(150,__context__) = true;
                    _temp_make_local_1191_39_304(151,__context__) = true;
                    _temp_make_local_1191_39_304(152,__context__) = true;
                    _temp_make_local_1191_39_304(153,__context__) = true;
                    _temp_make_local_1191_39_304(154,__context__) = true;
                    _temp_make_local_1191_39_304(155,__context__) = true;
                    _temp_make_local_1191_39_304(156,__context__) = true;
                    _temp_make_local_1191_39_304(157,__context__) = true;
                    _temp_make_local_1191_39_304(158,__context__) = true;
                    _temp_make_local_1191_39_304(159,__context__) = true;
                    _temp_make_local_1191_39_304(160,__context__) = true;
                    _temp_make_local_1191_39_304(161,__context__) = true;
                    _temp_make_local_1191_39_304(162,__context__) = true;
                    _temp_make_local_1191_39_304(163,__context__) = true;
                    _temp_make_local_1191_39_304(164,__context__) = true;
                    _temp_make_local_1191_39_304(165,__context__) = true;
                    _temp_make_local_1191_39_304(166,__context__) = true;
                    _temp_make_local_1191_39_304(167,__context__) = true;
                    _temp_make_local_1191_39_304(168,__context__) = true;
                    _temp_make_local_1191_39_304(169,__context__) = true;
                    _temp_make_local_1191_39_304(170,__context__) = true;
                    _temp_make_local_1191_39_304(171,__context__) = true;
                    _temp_make_local_1191_39_304(172,__context__) = true;
                    _temp_make_local_1191_39_304(173,__context__) = true;
                    _temp_make_local_1191_39_304(174,__context__) = true;
                    _temp_make_local_1191_39_304(175,__context__) = true;
                    _temp_make_local_1191_39_304(176,__context__) = true;
                    _temp_make_local_1191_39_304(177,__context__) = true;
                    _temp_make_local_1191_39_304(178,__context__) = true;
                    _temp_make_local_1191_39_304(179,__context__) = true;
                    _temp_make_local_1191_39_304(180,__context__) = true;
                    _temp_make_local_1191_39_304(181,__context__) = true;
                    _temp_make_local_1191_39_304(182,__context__) = true;
                    _temp_make_local_1191_39_304(183,__context__) = true;
                    _temp_make_local_1191_39_304(184,__context__) = true;
                    _temp_make_local_1191_39_304(185,__context__) = true;
                    _temp_make_local_1191_39_304(186,__context__) = true;
                    _temp_make_local_1191_39_304(187,__context__) = true;
                    _temp_make_local_1191_39_304(188,__context__) = true;
                    _temp_make_local_1191_39_304(189,__context__) = true;
                    _temp_make_local_1191_39_304(190,__context__) = true;
                    _temp_make_local_1191_39_304(191,__context__) = true;
                    _temp_make_local_1191_39_304(192,__context__) = true;
                    _temp_make_local_1191_39_304(193,__context__) = true;
                    _temp_make_local_1191_39_304(194,__context__) = true;
                    _temp_make_local_1191_39_304(195,__context__) = true;
                    _temp_make_local_1191_39_304(196,__context__) = true;
                    _temp_make_local_1191_39_304(197,__context__) = true;
                    _temp_make_local_1191_39_304(198,__context__) = true;
                    _temp_make_local_1191_39_304(199,__context__) = true;
                    _temp_make_local_1191_39_304(200,__context__) = true;
                    _temp_make_local_1191_39_304(201,__context__) = true;
                    _temp_make_local_1191_39_304(202,__context__) = true;
                    _temp_make_local_1191_39_304(203,__context__) = true;
                    _temp_make_local_1191_39_304(204,__context__) = true;
                    _temp_make_local_1191_39_304(205,__context__) = true;
                    _temp_make_local_1191_39_304(206,__context__) = true;
                    _temp_make_local_1191_39_304(207,__context__) = true;
                    _temp_make_local_1191_39_304(208,__context__) = true;
                    _temp_make_local_1191_39_304(209,__context__) = true;
                    _temp_make_local_1191_39_304(210,__context__) = true;
                    _temp_make_local_1191_39_304(211,__context__) = true;
                    _temp_make_local_1191_39_304(212,__context__) = true;
                    _temp_make_local_1191_39_304(213,__context__) = true;
                    _temp_make_local_1191_39_304(214,__context__) = true;
                    _temp_make_local_1191_39_304(215,__context__) = true;
                    _temp_make_local_1191_39_304(216,__context__) = true;
                    _temp_make_local_1191_39_304(217,__context__) = true;
                    _temp_make_local_1191_39_304(218,__context__) = true;
                    _temp_make_local_1191_39_304(219,__context__) = true;
                    _temp_make_local_1191_39_304(220,__context__) = true;
                    _temp_make_local_1191_39_304(221,__context__) = true;
                    _temp_make_local_1191_39_304(222,__context__) = true;
                    _temp_make_local_1191_39_304(223,__context__) = true;
                    _temp_make_local_1191_39_304(224,__context__) = true;
                    _temp_make_local_1191_39_304(225,__context__) = true;
                    _temp_make_local_1191_39_304(226,__context__) = true;
                    _temp_make_local_1191_39_304(227,__context__) = true;
                    _temp_make_local_1191_39_304(228,__context__) = true;
                    _temp_make_local_1191_39_304(229,__context__) = true;
                    _temp_make_local_1191_39_304(230,__context__) = true;
                    _temp_make_local_1191_39_304(231,__context__) = true;
                    _temp_make_local_1191_39_304(232,__context__) = true;
                    _temp_make_local_1191_39_304(233,__context__) = true;
                    _temp_make_local_1191_39_304(234,__context__) = true;
                    _temp_make_local_1191_39_304(235,__context__) = true;
                    _temp_make_local_1191_39_304(236,__context__) = true;
                    _temp_make_local_1191_39_304(237,__context__) = true;
                    _temp_make_local_1191_39_304(238,__context__) = true;
                    _temp_make_local_1191_39_304(239,__context__) = true;
                    _temp_make_local_1191_39_304(240,__context__) = true;
                    _temp_make_local_1191_39_304(241,__context__) = true;
                    _temp_make_local_1191_39_304(242,__context__) = true;
                    _temp_make_local_1191_39_304(243,__context__) = true;
                    _temp_make_local_1191_39_304(244,__context__) = true;
                    _temp_make_local_1191_39_304(245,__context__) = true;
                    _temp_make_local_1191_39_304(246,__context__) = true;
                    _temp_make_local_1191_39_304(247,__context__) = true;
                    _temp_make_local_1191_39_304(248,__context__) = true;
                    _temp_make_local_1191_39_304(249,__context__) = true;
                    _temp_make_local_1191_39_304(250,__context__) = true;
                    _temp_make_local_1191_39_304(251,__context__) = true;
                    _temp_make_local_1191_39_304(252,__context__) = true;
                    _temp_make_local_1191_39_304(253,__context__) = true;
                    _temp_make_local_1191_39_304(254,__context__) = true;
                    _temp_make_local_1191_39_304(255,__context__) = true;
                    return _temp_make_local_1191_39_304;
                })())(__ch_rename_at_1190_417,__context__));
            });
            if ( !___terminal_result_14_rename_at_1195_416 )
            {
                if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1213_49_592; _temp_make_local_1213_49_592;
                    char * __rest_rename_at_1211_418 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_74, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                    char * __complete_message_rename_at_1212_419 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_75, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '0'-'256' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1211_418))));
                    _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1213_49_592.text),(__complete_message_rename_at_1212_419));
                        das_copy((_temp_make_local_1213_49_592.index),(__parser_rename_at_362_410.index));
                        return _temp_make_local_1213_49_592;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1216;
                    das_zero(__mks_1216);
                    return __mks_1216;
                })()));
            } else {
                _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),1);
            };
            char * __str_0_rename_at_566_420 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_410.input(0,__context__))));
            char * __Ch_rename_at_567_421 = (char *)(((char * const )(builtin_string_slice1(__str_0_rename_at_566_420,__ext_pos_0_rename_at_564_415,__parser_rename_at_362_410.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            char * __val_rename_at_1129_422; das_zero(__val_rename_at_1129_422); das_move(__val_rename_at_1129_422, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(((char * const )(pass_string(__Ch_rename_at_567_421))));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1137_423; das_zero(__result_rename_at_1137_423); das_move(__result_rename_at_1137_423, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1137;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_422;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_410.index;
                return __mkt_1137;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_423);
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
        if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),((char *) "#"),1) )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1171_49_912; _temp_make_local_1171_49_912;
                char * __rest_rename_at_1169_427 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_76, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                char * __complete_message_rename_at_1170_428 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_77, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m#\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_427))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1171_49_912.text),(__complete_message_rename_at_1170_428));
                    das_copy((_temp_make_local_1171_49_912.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1171_49_912;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1175;
                das_zero(__mks_1175);
                return __mks_1175;
            })()));
        } else {
            AutoTuple<bool,char *> ___terminal_result_16_rename_at_1349_429; das_zero(___terminal_result_16_rename_at_1349_429); das_move(___terminal_result_16_rename_at_1349_429, _FuncpegTickmatch_string_literalTick12800953725978677998_f6b931b539266e97(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
            if ( !das_get_auto_tuple_field<bool,0,bool,char *>::get(___terminal_result_16_rename_at_1349_429) )
            {
                if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
                {
                    peg::ParsingError _temp_make_local_1358_49_976; _temp_make_local_1358_49_976;
                    char * __complete_message_rename_at_1357_430 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_78, cast<char * const >::from(((char *) "Error: Expected a string at ")), cast<int32_t>::from(__parser_rename_at_362_410.index)))));
                    _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                        das_copy((_temp_make_local_1358_49_976.text),(__complete_message_rename_at_1357_430));
                        das_copy((_temp_make_local_1358_49_976.index),(__parser_rename_at_362_410.index));
                        return _temp_make_local_1358_49_976;
                    })())));
                };
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_1361;
                    das_zero(__mks_1361);
                    return __mks_1361;
                })()));
            } else {
                char * __e_rename_at_1368_431 = (char *)(das_get_auto_tuple_field<char *,1,bool,char *>::get(___terminal_result_16_rename_at_1349_429));
                das_copy(__cut_rename_at_1084_425,true);
                char * __val_rename_at_1129_432; das_zero(__val_rename_at_1129_432); das_move(__val_rename_at_1129_432, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                    return das_auto_cast<char *>::cast(__e_rename_at_1368_431);
                })));
                AutoTuple<bool,char *,int32_t> __result_rename_at_1137_433; das_zero(__result_rename_at_1137_433); das_move(__result_rename_at_1137_433, (([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mkt_1137;
                    das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                    das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_432;
                    das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_410.index;
                    return __mkt_1137;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_433);
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
        AutoTuple<bool,char *> ___terminal_result_18_rename_at_1349_437; das_zero(___terminal_result_18_rename_at_1349_437); das_move(___terminal_result_18_rename_at_1349_437, _FuncpegTickmatch_string_literalTick12800953725978677998_f6b931b539266e97(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *>::get(___terminal_result_18_rename_at_1349_437) )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1358_49_1296; _temp_make_local_1358_49_1296;
                char * __complete_message_rename_at_1357_438 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_79, cast<char * const >::from(((char *) "Error: Expected a string at ")), cast<int32_t>::from(__parser_rename_at_362_410.index)))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1358_49_1296.text),(__complete_message_rename_at_1357_438));
                    das_copy((_temp_make_local_1358_49_1296.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1358_49_1296;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1361;
                das_zero(__mks_1361);
                return __mks_1361;
            })()));
        } else {
            char * __e_rename_at_1368_439 = (char *)(das_get_auto_tuple_field<char *,1,bool,char *>::get(___terminal_result_18_rename_at_1349_437));
            das_copy(__cut_rename_at_1084_435,true);
            char * __val_rename_at_1129_440; das_zero(__val_rename_at_1129_440); das_move(__val_rename_at_1129_440, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_80, cast<char * const >::from(((char *) "\"")), cast<char *>::from(__e_rename_at_1368_439), cast<char * const >::from(((char *) "\"")))));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1137_441; das_zero(__result_rename_at_1137_441); das_move(__result_rename_at_1137_441, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1137;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_440;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_410.index;
                return __mkt_1137;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_441);
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
        if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),((char *) "("),1) )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1171_49_1600; _temp_make_local_1171_49_1600;
                char * __rest_rename_at_1169_444 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_81, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                char * __complete_message_rename_at_1170_445 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_82, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m(\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_444))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1171_49_1600.text),(__complete_message_rename_at_1170_445));
                    das_copy((_temp_make_local_1171_49_1600.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1171_49_1600;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1175;
                das_zero(__mks_1175);
                return __mks_1175;
            })()));
        } else {
            AutoTuple<bool,char *,int32_t> __element2_rename_at_546_446; das_zero(__element2_rename_at_546_446); das_move(__element2_rename_at_546_446, _Funcparse_elementTickid_0x1_609c6e555a4e4af4(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
            if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__element2_rename_at_546_446) )
            {
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mks_548;
                    das_zero(__mks_548);
                    return __mks_548;
                })()));
            } else {
                char * __e_rename_at_551_447; das_zero(__e_rename_at_551_447); das_move(__e_rename_at_551_447, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__element2_rename_at_546_446)));
                if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),((char *) ")"),1) )
                {
                    if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1171_49_1728; _temp_make_local_1171_49_1728;
                        char * __rest_rename_at_1169_448 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_83, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                        char * __complete_message_rename_at_1170_449 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_84, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m)\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_448))));
                        _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1171_49_1728.text),(__complete_message_rename_at_1170_449));
                            das_copy((_temp_make_local_1171_49_1728.index),(__parser_rename_at_362_410.index));
                            return _temp_make_local_1171_49_1728;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                        AutoTuple<bool,char *,int32_t> __mks_1175;
                        das_zero(__mks_1175);
                        return __mks_1175;
                    })()));
                } else {
                    char * __val_rename_at_1129_450; das_zero(__val_rename_at_1129_450); das_move(__val_rename_at_1129_450, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                        return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_85, cast<char * const >::from(((char *) "(")), cast<char *>::from(__e_rename_at_551_447), cast<char * const >::from(((char *) ")")))));
                    })));
                    AutoTuple<bool,char *,int32_t> __result_rename_at_1137_451; das_zero(__result_rename_at_1137_451); das_move(__result_rename_at_1137_451, (([&]() -> AutoTuple<bool,char *,int32_t> {
                        AutoTuple<bool,char *,int32_t> __mkt_1137;
                        das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                        das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_450;
                        das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_410.index;
                        return __mkt_1137;
                    })()));
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_451);
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
        bool ___terminal_result_22_rename_at_1195_455 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
            TDim<bool,256> _temp_make_local_1191_39_2064; _temp_make_local_1191_39_2064;
            int32_t __ch_rename_at_1190_456 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410)));
            return das_auto_cast<bool>::cast((__ch_rename_at_1190_456 != -1) && (([&]() -> TDim<bool,256>& {
                _temp_make_local_1191_39_2064(0,__context__) = true;
                _temp_make_local_1191_39_2064(1,__context__) = true;
                _temp_make_local_1191_39_2064(2,__context__) = true;
                _temp_make_local_1191_39_2064(3,__context__) = true;
                _temp_make_local_1191_39_2064(4,__context__) = true;
                _temp_make_local_1191_39_2064(5,__context__) = true;
                _temp_make_local_1191_39_2064(6,__context__) = true;
                _temp_make_local_1191_39_2064(7,__context__) = true;
                _temp_make_local_1191_39_2064(8,__context__) = true;
                _temp_make_local_1191_39_2064(9,__context__) = true;
                _temp_make_local_1191_39_2064(10,__context__) = true;
                _temp_make_local_1191_39_2064(11,__context__) = true;
                _temp_make_local_1191_39_2064(12,__context__) = true;
                _temp_make_local_1191_39_2064(13,__context__) = true;
                _temp_make_local_1191_39_2064(14,__context__) = true;
                _temp_make_local_1191_39_2064(15,__context__) = true;
                _temp_make_local_1191_39_2064(16,__context__) = true;
                _temp_make_local_1191_39_2064(17,__context__) = true;
                _temp_make_local_1191_39_2064(18,__context__) = true;
                _temp_make_local_1191_39_2064(19,__context__) = true;
                _temp_make_local_1191_39_2064(20,__context__) = true;
                _temp_make_local_1191_39_2064(21,__context__) = true;
                _temp_make_local_1191_39_2064(22,__context__) = true;
                _temp_make_local_1191_39_2064(23,__context__) = true;
                _temp_make_local_1191_39_2064(24,__context__) = true;
                _temp_make_local_1191_39_2064(25,__context__) = true;
                _temp_make_local_1191_39_2064(26,__context__) = true;
                _temp_make_local_1191_39_2064(27,__context__) = true;
                _temp_make_local_1191_39_2064(28,__context__) = true;
                _temp_make_local_1191_39_2064(29,__context__) = true;
                _temp_make_local_1191_39_2064(30,__context__) = true;
                _temp_make_local_1191_39_2064(31,__context__) = true;
                _temp_make_local_1191_39_2064(32,__context__) = true;
                _temp_make_local_1191_39_2064(33,__context__) = true;
                _temp_make_local_1191_39_2064(34,__context__) = true;
                _temp_make_local_1191_39_2064(35,__context__) = true;
                _temp_make_local_1191_39_2064(36,__context__) = true;
                _temp_make_local_1191_39_2064(37,__context__) = true;
                _temp_make_local_1191_39_2064(38,__context__) = true;
                _temp_make_local_1191_39_2064(39,__context__) = true;
                _temp_make_local_1191_39_2064(40,__context__) = true;
                _temp_make_local_1191_39_2064(41,__context__) = false;
                _temp_make_local_1191_39_2064(42,__context__) = true;
                _temp_make_local_1191_39_2064(43,__context__) = true;
                _temp_make_local_1191_39_2064(44,__context__) = false;
                _temp_make_local_1191_39_2064(45,__context__) = true;
                _temp_make_local_1191_39_2064(46,__context__) = true;
                _temp_make_local_1191_39_2064(47,__context__) = true;
                _temp_make_local_1191_39_2064(48,__context__) = true;
                _temp_make_local_1191_39_2064(49,__context__) = true;
                _temp_make_local_1191_39_2064(50,__context__) = true;
                _temp_make_local_1191_39_2064(51,__context__) = true;
                _temp_make_local_1191_39_2064(52,__context__) = true;
                _temp_make_local_1191_39_2064(53,__context__) = true;
                _temp_make_local_1191_39_2064(54,__context__) = true;
                _temp_make_local_1191_39_2064(55,__context__) = true;
                _temp_make_local_1191_39_2064(56,__context__) = true;
                _temp_make_local_1191_39_2064(57,__context__) = true;
                _temp_make_local_1191_39_2064(58,__context__) = true;
                _temp_make_local_1191_39_2064(59,__context__) = true;
                _temp_make_local_1191_39_2064(60,__context__) = true;
                _temp_make_local_1191_39_2064(61,__context__) = true;
                _temp_make_local_1191_39_2064(62,__context__) = true;
                _temp_make_local_1191_39_2064(63,__context__) = true;
                _temp_make_local_1191_39_2064(64,__context__) = true;
                _temp_make_local_1191_39_2064(65,__context__) = true;
                _temp_make_local_1191_39_2064(66,__context__) = true;
                _temp_make_local_1191_39_2064(67,__context__) = true;
                _temp_make_local_1191_39_2064(68,__context__) = true;
                _temp_make_local_1191_39_2064(69,__context__) = true;
                _temp_make_local_1191_39_2064(70,__context__) = true;
                _temp_make_local_1191_39_2064(71,__context__) = true;
                _temp_make_local_1191_39_2064(72,__context__) = true;
                _temp_make_local_1191_39_2064(73,__context__) = true;
                _temp_make_local_1191_39_2064(74,__context__) = true;
                _temp_make_local_1191_39_2064(75,__context__) = true;
                _temp_make_local_1191_39_2064(76,__context__) = true;
                _temp_make_local_1191_39_2064(77,__context__) = true;
                _temp_make_local_1191_39_2064(78,__context__) = true;
                _temp_make_local_1191_39_2064(79,__context__) = true;
                _temp_make_local_1191_39_2064(80,__context__) = true;
                _temp_make_local_1191_39_2064(81,__context__) = true;
                _temp_make_local_1191_39_2064(82,__context__) = true;
                _temp_make_local_1191_39_2064(83,__context__) = true;
                _temp_make_local_1191_39_2064(84,__context__) = true;
                _temp_make_local_1191_39_2064(85,__context__) = true;
                _temp_make_local_1191_39_2064(86,__context__) = true;
                _temp_make_local_1191_39_2064(87,__context__) = true;
                _temp_make_local_1191_39_2064(88,__context__) = true;
                _temp_make_local_1191_39_2064(89,__context__) = true;
                _temp_make_local_1191_39_2064(90,__context__) = true;
                _temp_make_local_1191_39_2064(91,__context__) = true;
                _temp_make_local_1191_39_2064(92,__context__) = true;
                _temp_make_local_1191_39_2064(93,__context__) = true;
                _temp_make_local_1191_39_2064(94,__context__) = true;
                _temp_make_local_1191_39_2064(95,__context__) = true;
                _temp_make_local_1191_39_2064(96,__context__) = true;
                _temp_make_local_1191_39_2064(97,__context__) = true;
                _temp_make_local_1191_39_2064(98,__context__) = true;
                _temp_make_local_1191_39_2064(99,__context__) = true;
                _temp_make_local_1191_39_2064(100,__context__) = true;
                _temp_make_local_1191_39_2064(101,__context__) = true;
                _temp_make_local_1191_39_2064(102,__context__) = true;
                _temp_make_local_1191_39_2064(103,__context__) = true;
                _temp_make_local_1191_39_2064(104,__context__) = true;
                _temp_make_local_1191_39_2064(105,__context__) = true;
                _temp_make_local_1191_39_2064(106,__context__) = true;
                _temp_make_local_1191_39_2064(107,__context__) = true;
                _temp_make_local_1191_39_2064(108,__context__) = true;
                _temp_make_local_1191_39_2064(109,__context__) = true;
                _temp_make_local_1191_39_2064(110,__context__) = true;
                _temp_make_local_1191_39_2064(111,__context__) = true;
                _temp_make_local_1191_39_2064(112,__context__) = true;
                _temp_make_local_1191_39_2064(113,__context__) = true;
                _temp_make_local_1191_39_2064(114,__context__) = true;
                _temp_make_local_1191_39_2064(115,__context__) = true;
                _temp_make_local_1191_39_2064(116,__context__) = true;
                _temp_make_local_1191_39_2064(117,__context__) = true;
                _temp_make_local_1191_39_2064(118,__context__) = true;
                _temp_make_local_1191_39_2064(119,__context__) = true;
                _temp_make_local_1191_39_2064(120,__context__) = true;
                _temp_make_local_1191_39_2064(121,__context__) = true;
                _temp_make_local_1191_39_2064(122,__context__) = true;
                _temp_make_local_1191_39_2064(123,__context__) = true;
                _temp_make_local_1191_39_2064(124,__context__) = true;
                _temp_make_local_1191_39_2064(125,__context__) = true;
                _temp_make_local_1191_39_2064(126,__context__) = true;
                _temp_make_local_1191_39_2064(127,__context__) = true;
                _temp_make_local_1191_39_2064(128,__context__) = true;
                _temp_make_local_1191_39_2064(129,__context__) = true;
                _temp_make_local_1191_39_2064(130,__context__) = true;
                _temp_make_local_1191_39_2064(131,__context__) = true;
                _temp_make_local_1191_39_2064(132,__context__) = true;
                _temp_make_local_1191_39_2064(133,__context__) = true;
                _temp_make_local_1191_39_2064(134,__context__) = true;
                _temp_make_local_1191_39_2064(135,__context__) = true;
                _temp_make_local_1191_39_2064(136,__context__) = true;
                _temp_make_local_1191_39_2064(137,__context__) = true;
                _temp_make_local_1191_39_2064(138,__context__) = true;
                _temp_make_local_1191_39_2064(139,__context__) = true;
                _temp_make_local_1191_39_2064(140,__context__) = true;
                _temp_make_local_1191_39_2064(141,__context__) = true;
                _temp_make_local_1191_39_2064(142,__context__) = true;
                _temp_make_local_1191_39_2064(143,__context__) = true;
                _temp_make_local_1191_39_2064(144,__context__) = true;
                _temp_make_local_1191_39_2064(145,__context__) = true;
                _temp_make_local_1191_39_2064(146,__context__) = true;
                _temp_make_local_1191_39_2064(147,__context__) = true;
                _temp_make_local_1191_39_2064(148,__context__) = true;
                _temp_make_local_1191_39_2064(149,__context__) = true;
                _temp_make_local_1191_39_2064(150,__context__) = true;
                _temp_make_local_1191_39_2064(151,__context__) = true;
                _temp_make_local_1191_39_2064(152,__context__) = true;
                _temp_make_local_1191_39_2064(153,__context__) = true;
                _temp_make_local_1191_39_2064(154,__context__) = true;
                _temp_make_local_1191_39_2064(155,__context__) = true;
                _temp_make_local_1191_39_2064(156,__context__) = true;
                _temp_make_local_1191_39_2064(157,__context__) = true;
                _temp_make_local_1191_39_2064(158,__context__) = true;
                _temp_make_local_1191_39_2064(159,__context__) = true;
                _temp_make_local_1191_39_2064(160,__context__) = true;
                _temp_make_local_1191_39_2064(161,__context__) = true;
                _temp_make_local_1191_39_2064(162,__context__) = true;
                _temp_make_local_1191_39_2064(163,__context__) = true;
                _temp_make_local_1191_39_2064(164,__context__) = true;
                _temp_make_local_1191_39_2064(165,__context__) = true;
                _temp_make_local_1191_39_2064(166,__context__) = true;
                _temp_make_local_1191_39_2064(167,__context__) = true;
                _temp_make_local_1191_39_2064(168,__context__) = true;
                _temp_make_local_1191_39_2064(169,__context__) = true;
                _temp_make_local_1191_39_2064(170,__context__) = true;
                _temp_make_local_1191_39_2064(171,__context__) = true;
                _temp_make_local_1191_39_2064(172,__context__) = true;
                _temp_make_local_1191_39_2064(173,__context__) = true;
                _temp_make_local_1191_39_2064(174,__context__) = true;
                _temp_make_local_1191_39_2064(175,__context__) = true;
                _temp_make_local_1191_39_2064(176,__context__) = true;
                _temp_make_local_1191_39_2064(177,__context__) = true;
                _temp_make_local_1191_39_2064(178,__context__) = true;
                _temp_make_local_1191_39_2064(179,__context__) = true;
                _temp_make_local_1191_39_2064(180,__context__) = true;
                _temp_make_local_1191_39_2064(181,__context__) = true;
                _temp_make_local_1191_39_2064(182,__context__) = true;
                _temp_make_local_1191_39_2064(183,__context__) = true;
                _temp_make_local_1191_39_2064(184,__context__) = true;
                _temp_make_local_1191_39_2064(185,__context__) = true;
                _temp_make_local_1191_39_2064(186,__context__) = true;
                _temp_make_local_1191_39_2064(187,__context__) = true;
                _temp_make_local_1191_39_2064(188,__context__) = true;
                _temp_make_local_1191_39_2064(189,__context__) = true;
                _temp_make_local_1191_39_2064(190,__context__) = true;
                _temp_make_local_1191_39_2064(191,__context__) = true;
                _temp_make_local_1191_39_2064(192,__context__) = true;
                _temp_make_local_1191_39_2064(193,__context__) = true;
                _temp_make_local_1191_39_2064(194,__context__) = true;
                _temp_make_local_1191_39_2064(195,__context__) = true;
                _temp_make_local_1191_39_2064(196,__context__) = true;
                _temp_make_local_1191_39_2064(197,__context__) = true;
                _temp_make_local_1191_39_2064(198,__context__) = true;
                _temp_make_local_1191_39_2064(199,__context__) = true;
                _temp_make_local_1191_39_2064(200,__context__) = true;
                _temp_make_local_1191_39_2064(201,__context__) = true;
                _temp_make_local_1191_39_2064(202,__context__) = true;
                _temp_make_local_1191_39_2064(203,__context__) = true;
                _temp_make_local_1191_39_2064(204,__context__) = true;
                _temp_make_local_1191_39_2064(205,__context__) = true;
                _temp_make_local_1191_39_2064(206,__context__) = true;
                _temp_make_local_1191_39_2064(207,__context__) = true;
                _temp_make_local_1191_39_2064(208,__context__) = true;
                _temp_make_local_1191_39_2064(209,__context__) = true;
                _temp_make_local_1191_39_2064(210,__context__) = true;
                _temp_make_local_1191_39_2064(211,__context__) = true;
                _temp_make_local_1191_39_2064(212,__context__) = true;
                _temp_make_local_1191_39_2064(213,__context__) = true;
                _temp_make_local_1191_39_2064(214,__context__) = true;
                _temp_make_local_1191_39_2064(215,__context__) = true;
                _temp_make_local_1191_39_2064(216,__context__) = true;
                _temp_make_local_1191_39_2064(217,__context__) = true;
                _temp_make_local_1191_39_2064(218,__context__) = true;
                _temp_make_local_1191_39_2064(219,__context__) = true;
                _temp_make_local_1191_39_2064(220,__context__) = true;
                _temp_make_local_1191_39_2064(221,__context__) = true;
                _temp_make_local_1191_39_2064(222,__context__) = true;
                _temp_make_local_1191_39_2064(223,__context__) = true;
                _temp_make_local_1191_39_2064(224,__context__) = true;
                _temp_make_local_1191_39_2064(225,__context__) = true;
                _temp_make_local_1191_39_2064(226,__context__) = true;
                _temp_make_local_1191_39_2064(227,__context__) = true;
                _temp_make_local_1191_39_2064(228,__context__) = true;
                _temp_make_local_1191_39_2064(229,__context__) = true;
                _temp_make_local_1191_39_2064(230,__context__) = true;
                _temp_make_local_1191_39_2064(231,__context__) = true;
                _temp_make_local_1191_39_2064(232,__context__) = true;
                _temp_make_local_1191_39_2064(233,__context__) = true;
                _temp_make_local_1191_39_2064(234,__context__) = true;
                _temp_make_local_1191_39_2064(235,__context__) = true;
                _temp_make_local_1191_39_2064(236,__context__) = true;
                _temp_make_local_1191_39_2064(237,__context__) = true;
                _temp_make_local_1191_39_2064(238,__context__) = true;
                _temp_make_local_1191_39_2064(239,__context__) = true;
                _temp_make_local_1191_39_2064(240,__context__) = true;
                _temp_make_local_1191_39_2064(241,__context__) = true;
                _temp_make_local_1191_39_2064(242,__context__) = true;
                _temp_make_local_1191_39_2064(243,__context__) = true;
                _temp_make_local_1191_39_2064(244,__context__) = true;
                _temp_make_local_1191_39_2064(245,__context__) = true;
                _temp_make_local_1191_39_2064(246,__context__) = true;
                _temp_make_local_1191_39_2064(247,__context__) = true;
                _temp_make_local_1191_39_2064(248,__context__) = true;
                _temp_make_local_1191_39_2064(249,__context__) = true;
                _temp_make_local_1191_39_2064(250,__context__) = true;
                _temp_make_local_1191_39_2064(251,__context__) = true;
                _temp_make_local_1191_39_2064(252,__context__) = true;
                _temp_make_local_1191_39_2064(253,__context__) = true;
                _temp_make_local_1191_39_2064(254,__context__) = true;
                _temp_make_local_1191_39_2064(255,__context__) = true;
                return _temp_make_local_1191_39_2064;
            })())(__ch_rename_at_1190_456,__context__));
        });
        if ( !___terminal_result_22_rename_at_1195_455 )
        {
            if ( (__parser_rename_at_362_410.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_410.suppress_errors)))) && (__parser_rename_at_362_410.index == __parser_rename_at_362_410.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1213_49_2352; _temp_make_local_1213_49_2352;
                char * __rest_rename_at_1211_457 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_86, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_410.index))),false));
                char * __complete_message_rename_at_1212_458 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_87, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '0'-'40'  '42'-'43'  '45'-'256' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1211_457))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_410.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1213_49_2352.text),(__complete_message_rename_at_1212_458));
                    das_copy((_temp_make_local_1213_49_2352.index),(__parser_rename_at_362_410.index));
                    return _temp_make_local_1213_49_2352;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1216;
                das_zero(__mks_1216);
                return __mks_1216;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_410),1);
        };
        char * __str_1_rename_at_566_459 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_410.input(0,__context__))));
        char * __e_rename_at_567_460 = (char *)(((char * const )(builtin_string_slice1(__str_1_rename_at_566_459,__ext_pos_1_rename_at_564_454,__parser_rename_at_362_410.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        char * __val_rename_at_1129_461; das_zero(__val_rename_at_1129_461); das_move(__val_rename_at_1129_461, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
            return das_auto_cast<char *>::cast(__e_rename_at_567_460);
        })));
        AutoTuple<bool,char *,int32_t> __result_rename_at_1137_462; das_zero(__result_rename_at_1137_462); das_move(__result_rename_at_1137_462, (([&]() -> AutoTuple<bool,char *,int32_t> {
            AutoTuple<bool,char *,int32_t> __mkt_1137;
            das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
            das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_461;
            das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_410.index;
            return __mkt_1137;
        })()));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_462);
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

inline AutoTuple<bool,char *,int32_t> _Funcparse_sub_elementTickid_0x1_9dd0aa4f6e723d42 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_463 )
{
    int32_t __mark_rename_at_147_464 = __parser_rename_at_146_463.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_463.sub_element_cache),__parser_rename_at_146_463.index) && !(__parser_rename_at_146_463.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_465; das_zero(__result_rename_at_155_465); das_move(__result_rename_at_155_465, _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_463.sub_element_cache(__parser_rename_at_146_463.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_465) )
        {
            das_copy(__parser_rename_at_146_463.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_465));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_465);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_466; das_zero(__result_rename_at_169_466); das_move(__result_rename_at_169_466, _Funcparse_sub_element_innerTickid_0x1_cb2b3527b22dde21(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_463)));
        das_copy(__parser_rename_at_146_463.sub_element_cache(__mark_rename_at_147_464,__context__),__result_rename_at_169_466);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_466);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_element_innerTickid_0x1_82f0e6f9b68f3345 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_467 )
{
    int32_t __parse_pos_rename_at_1076_468 = __parser_rename_at_362_467.index;
    ++__parser_rename_at_362_467.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_469; das_zero(__res_1_rename_at_1086_469); das_move(__res_1_rename_at_1086_469, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        AutoTuple<bool,char *,int32_t> __sub_element0_rename_at_546_470; das_zero(__sub_element0_rename_at_546_470); das_move(__sub_element0_rename_at_546_470, _Funcparse_sub_elementTickid_0x1_9dd0aa4f6e723d42(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_467)));
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
            _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_467.suppress_errors),true);
            while ( true )
            {
                int32_t __sz_rename_at_674_473 = builtin_array_size(das_arg<TArray<char *>>::pass(__tail_rename_at_671_472));
                das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                    AutoTuple<bool,char *,int32_t> __sub_element1_rename_at_546_474; das_zero(__sub_element1_rename_at_546_474); das_move(__sub_element1_rename_at_546_474, _Funcparse_sub_elementTickid_0x1_9dd0aa4f6e723d42(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_467)));
                    if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__sub_element1_rename_at_546_474) )
                    {
                        return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                            AutoTuple<bool,int32_t,int32_t> __mks_548;
                            das_zero(__mks_548);
                            return __mks_548;
                        })()));
                    } else {
                        char * __temp_name_rename_at_551_475; das_zero(__temp_name_rename_at_551_475); das_move(__temp_name_rename_at_551_475, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__sub_element1_rename_at_546_474)));
                        _FuncbuiltinTickemplaceTick13923705686753630697_664b3b92dd33b520(__context__,das_arg<TArray<char *>>::pass(__tail_rename_at_671_472),__temp_name_rename_at_551_475);
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
            _FuncbuiltinTickpopTick1161079256290593740_da418e66c1d8244e(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_467.suppress_errors));
            char * __val_rename_at_1129_476; das_zero(__val_rename_at_1129_476); das_move(__val_rename_at_1129_476, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__head_rename_at_551_471),cast<char *>::from(_Funcstrings_boostTickjoinTick16475640899284277631_597abbecf5b26d8(__context__,das_arg<TArray<char *>>::pass(__tail_rename_at_671_472),nullptr)),*__context__,nullptr)));
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1137_477; das_zero(__result_rename_at_1137_477); das_move(__result_rename_at_1137_477, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1137;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_476;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_467.index;
                return __mkt_1137;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_477);
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

inline AutoTuple<bool,char *,int32_t> _Funcparse_elementTickid_0x1_609c6e555a4e4af4 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_478 )
{
    int32_t __mark_rename_at_147_479 = __parser_rename_at_146_478.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_478.element_cache),__parser_rename_at_146_478.index) && !(__parser_rename_at_146_478.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_480; das_zero(__result_rename_at_155_480); das_move(__result_rename_at_155_480, _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_478.element_cache(__parser_rename_at_146_478.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_480) )
        {
            das_copy(__parser_rename_at_146_478.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_480));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_480);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_481; das_zero(__result_rename_at_169_481); das_move(__result_rename_at_169_481, _Funcparse_element_innerTickid_0x1_82f0e6f9b68f3345(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_478)));
        das_copy(__parser_rename_at_146_478.element_cache(__mark_rename_at_147_479,__context__),__result_rename_at_169_481);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_481);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_class_name_innerTickid_0x1_151c0b46e0f35deb ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_482 )
{
    int32_t __parse_pos_rename_at_1076_483 = __parser_rename_at_362_482.index;
    ++__parser_rename_at_362_482.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_484; das_zero(__res_1_rename_at_1086_484); das_move(__res_1_rename_at_1086_484, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482));
        int32_t __ext_pos_2_rename_at_564_485 = __parser_rename_at_362_482.index;
        AutoTuple<bool,char *,int32_t> __ident_0_rename_at_525_486; das_zero(__ident_0_rename_at_525_486); das_move(__ident_0_rename_at_525_486, _Funcparse_ident_Tickid_0x1_5767df53d96981e(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482)));
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
        if ( !_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482),((char *) "::"),2) )
        {
            if ( (__parser_rename_at_362_482.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_482.suppress_errors)))) && (__parser_rename_at_362_482.index == __parser_rename_at_362_482.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1171_49_272; _temp_make_local_1171_49_272;
                char * __rest_rename_at_1169_489 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_88, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_482.index))),false));
                char * __complete_message_rename_at_1170_490 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_89, cast<char *>::from(((char *) "Error: Expected the terminal `\u001b[1m::\u001b[0m")), cast<char *>::from(__rest_rename_at_1169_489))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_482.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1171_49_272.text),(__complete_message_rename_at_1170_490));
                    das_copy((_temp_make_local_1171_49_272.index),(__parser_rename_at_362_482.index));
                    return _temp_make_local_1171_49_272;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1175;
                das_zero(__mks_1175);
                return __mks_1175;
            })()));
        } else {
            int32_t __ext_pos_3_rename_at_564_491 = __parser_rename_at_362_482.index;
            AutoTuple<bool,char *,int32_t> __ident_1_rename_at_525_492; das_zero(__ident_1_rename_at_525_492); das_move(__ident_1_rename_at_525_492, _Funcparse_ident_Tickid_0x1_5767df53d96981e(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482)));
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
                char * __val_rename_at_1129_495; das_zero(__val_rename_at_1129_495); das_move(__val_rename_at_1129_495, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                    return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from((cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__scope_rename_at_567_488),cast<char *>::from(((char *) "::")),*__context__,nullptr)))),cast<char *>::from(__id_rename_at_567_494),*__context__,nullptr)));
                })));
                AutoTuple<bool,char *,int32_t> __result_rename_at_1137_496; das_zero(__result_rename_at_1137_496); das_move(__result_rename_at_1137_496, (([&]() -> AutoTuple<bool,char *,int32_t> {
                    AutoTuple<bool,char *,int32_t> __mkt_1137;
                    das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                    das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_495;
                    das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_482.index;
                    return __mkt_1137;
                })()));
                return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_496);
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
        _FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482));
        AutoTuple<bool,char *,int32_t> __ident_2_rename_at_546_499; das_zero(__ident_2_rename_at_546_499); das_move(__ident_2_rename_at_546_499, _Funcparse_ident_Tickid_0x1_5767df53d96981e(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_482)));
        if ( !das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__ident_2_rename_at_546_499) )
        {
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_548;
                das_zero(__mks_548);
                return __mks_548;
            })()));
        } else {
            char * __id_rename_at_551_500; das_zero(__id_rename_at_551_500); das_move(__id_rename_at_551_500, (char *)(das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__ident_2_rename_at_546_499)));
            char * __val_rename_at_1129_501; das_zero(__val_rename_at_1129_501); das_move(__val_rename_at_1129_501, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
                return das_auto_cast<char *>::cast(__id_rename_at_551_500);
            })));
            AutoTuple<bool,char *,int32_t> __result_rename_at_1137_502; das_zero(__result_rename_at_1137_502); das_move(__result_rename_at_1137_502, (([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mkt_1137;
                das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
                das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_501;
                das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_482.index;
                return __mkt_1137;
            })()));
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_502);
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

inline AutoTuple<bool,char *,int32_t> _Funcparse_class_nameTickid_0x1_ecd2c3e791924e42 ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_503 )
{
    int32_t __mark_rename_at_147_504 = __parser_rename_at_146_503.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_503.class_name_cache),__parser_rename_at_146_503.index) && !(__parser_rename_at_146_503.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_505; das_zero(__result_rename_at_155_505); das_move(__result_rename_at_155_505, _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_503.class_name_cache(__parser_rename_at_146_503.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_505) )
        {
            das_copy(__parser_rename_at_146_503.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_505));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_505);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_506; das_zero(__result_rename_at_169_506); das_move(__result_rename_at_169_506, _Funcparse_class_name_innerTickid_0x1_151c0b46e0f35deb(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_503)));
        das_copy(__parser_rename_at_146_503.class_name_cache(__mark_rename_at_147_504,__context__),__result_rename_at_169_506);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_506);
    };
}

inline AutoTuple<bool,char *,int32_t> _Funcparse_ident__innerTickid_0x1_391c4c7b7867934a ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_362_507 )
{
    int32_t __parse_pos_rename_at_1076_508 = __parser_rename_at_362_507.index;
    ++__parser_rename_at_362_507.tabs;
    AutoTuple<bool,char *,int32_t> __res_1_rename_at_1086_509; das_zero(__res_1_rename_at_1086_509); das_move(__res_1_rename_at_1086_509, das_invoke<AutoTuple<bool,char *,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,char *,int32_t>{
        int32_t __ext_pos_4_rename_at_564_510 = __parser_rename_at_362_507.index;
        bool ___terminal_result_26_rename_at_1195_511 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
            TDim<bool,256> _temp_make_local_1191_39_208; _temp_make_local_1191_39_208;
            int32_t __ch_rename_at_1190_512 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_507)));
            return das_auto_cast<bool>::cast((__ch_rename_at_1190_512 != -1) && (([&]() -> TDim<bool,256>& {
                _temp_make_local_1191_39_208(0,__context__) = false;
                _temp_make_local_1191_39_208(1,__context__) = false;
                _temp_make_local_1191_39_208(2,__context__) = false;
                _temp_make_local_1191_39_208(3,__context__) = false;
                _temp_make_local_1191_39_208(4,__context__) = false;
                _temp_make_local_1191_39_208(5,__context__) = false;
                _temp_make_local_1191_39_208(6,__context__) = false;
                _temp_make_local_1191_39_208(7,__context__) = false;
                _temp_make_local_1191_39_208(8,__context__) = false;
                _temp_make_local_1191_39_208(9,__context__) = false;
                _temp_make_local_1191_39_208(10,__context__) = false;
                _temp_make_local_1191_39_208(11,__context__) = false;
                _temp_make_local_1191_39_208(12,__context__) = false;
                _temp_make_local_1191_39_208(13,__context__) = false;
                _temp_make_local_1191_39_208(14,__context__) = false;
                _temp_make_local_1191_39_208(15,__context__) = false;
                _temp_make_local_1191_39_208(16,__context__) = false;
                _temp_make_local_1191_39_208(17,__context__) = false;
                _temp_make_local_1191_39_208(18,__context__) = false;
                _temp_make_local_1191_39_208(19,__context__) = false;
                _temp_make_local_1191_39_208(20,__context__) = false;
                _temp_make_local_1191_39_208(21,__context__) = false;
                _temp_make_local_1191_39_208(22,__context__) = false;
                _temp_make_local_1191_39_208(23,__context__) = false;
                _temp_make_local_1191_39_208(24,__context__) = false;
                _temp_make_local_1191_39_208(25,__context__) = false;
                _temp_make_local_1191_39_208(26,__context__) = false;
                _temp_make_local_1191_39_208(27,__context__) = false;
                _temp_make_local_1191_39_208(28,__context__) = false;
                _temp_make_local_1191_39_208(29,__context__) = false;
                _temp_make_local_1191_39_208(30,__context__) = false;
                _temp_make_local_1191_39_208(31,__context__) = false;
                _temp_make_local_1191_39_208(32,__context__) = false;
                _temp_make_local_1191_39_208(33,__context__) = false;
                _temp_make_local_1191_39_208(34,__context__) = false;
                _temp_make_local_1191_39_208(35,__context__) = false;
                _temp_make_local_1191_39_208(36,__context__) = false;
                _temp_make_local_1191_39_208(37,__context__) = false;
                _temp_make_local_1191_39_208(38,__context__) = false;
                _temp_make_local_1191_39_208(39,__context__) = false;
                _temp_make_local_1191_39_208(40,__context__) = false;
                _temp_make_local_1191_39_208(41,__context__) = false;
                _temp_make_local_1191_39_208(42,__context__) = false;
                _temp_make_local_1191_39_208(43,__context__) = false;
                _temp_make_local_1191_39_208(44,__context__) = false;
                _temp_make_local_1191_39_208(45,__context__) = false;
                _temp_make_local_1191_39_208(46,__context__) = false;
                _temp_make_local_1191_39_208(47,__context__) = false;
                _temp_make_local_1191_39_208(48,__context__) = false;
                _temp_make_local_1191_39_208(49,__context__) = false;
                _temp_make_local_1191_39_208(50,__context__) = false;
                _temp_make_local_1191_39_208(51,__context__) = false;
                _temp_make_local_1191_39_208(52,__context__) = false;
                _temp_make_local_1191_39_208(53,__context__) = false;
                _temp_make_local_1191_39_208(54,__context__) = false;
                _temp_make_local_1191_39_208(55,__context__) = false;
                _temp_make_local_1191_39_208(56,__context__) = false;
                _temp_make_local_1191_39_208(57,__context__) = false;
                _temp_make_local_1191_39_208(58,__context__) = false;
                _temp_make_local_1191_39_208(59,__context__) = false;
                _temp_make_local_1191_39_208(60,__context__) = false;
                _temp_make_local_1191_39_208(61,__context__) = false;
                _temp_make_local_1191_39_208(62,__context__) = false;
                _temp_make_local_1191_39_208(63,__context__) = false;
                _temp_make_local_1191_39_208(64,__context__) = false;
                _temp_make_local_1191_39_208(65,__context__) = true;
                _temp_make_local_1191_39_208(66,__context__) = true;
                _temp_make_local_1191_39_208(67,__context__) = true;
                _temp_make_local_1191_39_208(68,__context__) = true;
                _temp_make_local_1191_39_208(69,__context__) = true;
                _temp_make_local_1191_39_208(70,__context__) = true;
                _temp_make_local_1191_39_208(71,__context__) = true;
                _temp_make_local_1191_39_208(72,__context__) = true;
                _temp_make_local_1191_39_208(73,__context__) = true;
                _temp_make_local_1191_39_208(74,__context__) = true;
                _temp_make_local_1191_39_208(75,__context__) = true;
                _temp_make_local_1191_39_208(76,__context__) = true;
                _temp_make_local_1191_39_208(77,__context__) = true;
                _temp_make_local_1191_39_208(78,__context__) = true;
                _temp_make_local_1191_39_208(79,__context__) = true;
                _temp_make_local_1191_39_208(80,__context__) = true;
                _temp_make_local_1191_39_208(81,__context__) = true;
                _temp_make_local_1191_39_208(82,__context__) = true;
                _temp_make_local_1191_39_208(83,__context__) = true;
                _temp_make_local_1191_39_208(84,__context__) = true;
                _temp_make_local_1191_39_208(85,__context__) = true;
                _temp_make_local_1191_39_208(86,__context__) = true;
                _temp_make_local_1191_39_208(87,__context__) = true;
                _temp_make_local_1191_39_208(88,__context__) = true;
                _temp_make_local_1191_39_208(89,__context__) = true;
                _temp_make_local_1191_39_208(90,__context__) = true;
                _temp_make_local_1191_39_208(91,__context__) = false;
                _temp_make_local_1191_39_208(92,__context__) = false;
                _temp_make_local_1191_39_208(93,__context__) = false;
                _temp_make_local_1191_39_208(94,__context__) = false;
                _temp_make_local_1191_39_208(95,__context__) = true;
                _temp_make_local_1191_39_208(96,__context__) = false;
                _temp_make_local_1191_39_208(97,__context__) = true;
                _temp_make_local_1191_39_208(98,__context__) = true;
                _temp_make_local_1191_39_208(99,__context__) = true;
                _temp_make_local_1191_39_208(100,__context__) = true;
                _temp_make_local_1191_39_208(101,__context__) = true;
                _temp_make_local_1191_39_208(102,__context__) = true;
                _temp_make_local_1191_39_208(103,__context__) = true;
                _temp_make_local_1191_39_208(104,__context__) = true;
                _temp_make_local_1191_39_208(105,__context__) = true;
                _temp_make_local_1191_39_208(106,__context__) = true;
                _temp_make_local_1191_39_208(107,__context__) = true;
                _temp_make_local_1191_39_208(108,__context__) = true;
                _temp_make_local_1191_39_208(109,__context__) = true;
                _temp_make_local_1191_39_208(110,__context__) = true;
                _temp_make_local_1191_39_208(111,__context__) = true;
                _temp_make_local_1191_39_208(112,__context__) = true;
                _temp_make_local_1191_39_208(113,__context__) = true;
                _temp_make_local_1191_39_208(114,__context__) = true;
                _temp_make_local_1191_39_208(115,__context__) = true;
                _temp_make_local_1191_39_208(116,__context__) = true;
                _temp_make_local_1191_39_208(117,__context__) = true;
                _temp_make_local_1191_39_208(118,__context__) = true;
                _temp_make_local_1191_39_208(119,__context__) = true;
                _temp_make_local_1191_39_208(120,__context__) = true;
                _temp_make_local_1191_39_208(121,__context__) = true;
                _temp_make_local_1191_39_208(122,__context__) = true;
                _temp_make_local_1191_39_208(123,__context__) = false;
                _temp_make_local_1191_39_208(124,__context__) = false;
                _temp_make_local_1191_39_208(125,__context__) = false;
                _temp_make_local_1191_39_208(126,__context__) = false;
                _temp_make_local_1191_39_208(127,__context__) = false;
                _temp_make_local_1191_39_208(128,__context__) = false;
                _temp_make_local_1191_39_208(129,__context__) = false;
                _temp_make_local_1191_39_208(130,__context__) = false;
                _temp_make_local_1191_39_208(131,__context__) = false;
                _temp_make_local_1191_39_208(132,__context__) = false;
                _temp_make_local_1191_39_208(133,__context__) = false;
                _temp_make_local_1191_39_208(134,__context__) = false;
                _temp_make_local_1191_39_208(135,__context__) = false;
                _temp_make_local_1191_39_208(136,__context__) = false;
                _temp_make_local_1191_39_208(137,__context__) = false;
                _temp_make_local_1191_39_208(138,__context__) = false;
                _temp_make_local_1191_39_208(139,__context__) = false;
                _temp_make_local_1191_39_208(140,__context__) = false;
                _temp_make_local_1191_39_208(141,__context__) = false;
                _temp_make_local_1191_39_208(142,__context__) = false;
                _temp_make_local_1191_39_208(143,__context__) = false;
                _temp_make_local_1191_39_208(144,__context__) = false;
                _temp_make_local_1191_39_208(145,__context__) = false;
                _temp_make_local_1191_39_208(146,__context__) = false;
                _temp_make_local_1191_39_208(147,__context__) = false;
                _temp_make_local_1191_39_208(148,__context__) = false;
                _temp_make_local_1191_39_208(149,__context__) = false;
                _temp_make_local_1191_39_208(150,__context__) = false;
                _temp_make_local_1191_39_208(151,__context__) = false;
                _temp_make_local_1191_39_208(152,__context__) = false;
                _temp_make_local_1191_39_208(153,__context__) = false;
                _temp_make_local_1191_39_208(154,__context__) = false;
                _temp_make_local_1191_39_208(155,__context__) = false;
                _temp_make_local_1191_39_208(156,__context__) = false;
                _temp_make_local_1191_39_208(157,__context__) = false;
                _temp_make_local_1191_39_208(158,__context__) = false;
                _temp_make_local_1191_39_208(159,__context__) = false;
                _temp_make_local_1191_39_208(160,__context__) = false;
                _temp_make_local_1191_39_208(161,__context__) = false;
                _temp_make_local_1191_39_208(162,__context__) = false;
                _temp_make_local_1191_39_208(163,__context__) = false;
                _temp_make_local_1191_39_208(164,__context__) = false;
                _temp_make_local_1191_39_208(165,__context__) = false;
                _temp_make_local_1191_39_208(166,__context__) = false;
                _temp_make_local_1191_39_208(167,__context__) = false;
                _temp_make_local_1191_39_208(168,__context__) = false;
                _temp_make_local_1191_39_208(169,__context__) = false;
                _temp_make_local_1191_39_208(170,__context__) = false;
                _temp_make_local_1191_39_208(171,__context__) = false;
                _temp_make_local_1191_39_208(172,__context__) = false;
                _temp_make_local_1191_39_208(173,__context__) = false;
                _temp_make_local_1191_39_208(174,__context__) = false;
                _temp_make_local_1191_39_208(175,__context__) = false;
                _temp_make_local_1191_39_208(176,__context__) = false;
                _temp_make_local_1191_39_208(177,__context__) = false;
                _temp_make_local_1191_39_208(178,__context__) = false;
                _temp_make_local_1191_39_208(179,__context__) = false;
                _temp_make_local_1191_39_208(180,__context__) = false;
                _temp_make_local_1191_39_208(181,__context__) = false;
                _temp_make_local_1191_39_208(182,__context__) = false;
                _temp_make_local_1191_39_208(183,__context__) = false;
                _temp_make_local_1191_39_208(184,__context__) = false;
                _temp_make_local_1191_39_208(185,__context__) = false;
                _temp_make_local_1191_39_208(186,__context__) = false;
                _temp_make_local_1191_39_208(187,__context__) = false;
                _temp_make_local_1191_39_208(188,__context__) = false;
                _temp_make_local_1191_39_208(189,__context__) = false;
                _temp_make_local_1191_39_208(190,__context__) = false;
                _temp_make_local_1191_39_208(191,__context__) = false;
                _temp_make_local_1191_39_208(192,__context__) = false;
                _temp_make_local_1191_39_208(193,__context__) = false;
                _temp_make_local_1191_39_208(194,__context__) = false;
                _temp_make_local_1191_39_208(195,__context__) = false;
                _temp_make_local_1191_39_208(196,__context__) = false;
                _temp_make_local_1191_39_208(197,__context__) = false;
                _temp_make_local_1191_39_208(198,__context__) = false;
                _temp_make_local_1191_39_208(199,__context__) = false;
                _temp_make_local_1191_39_208(200,__context__) = false;
                _temp_make_local_1191_39_208(201,__context__) = false;
                _temp_make_local_1191_39_208(202,__context__) = false;
                _temp_make_local_1191_39_208(203,__context__) = false;
                _temp_make_local_1191_39_208(204,__context__) = false;
                _temp_make_local_1191_39_208(205,__context__) = false;
                _temp_make_local_1191_39_208(206,__context__) = false;
                _temp_make_local_1191_39_208(207,__context__) = false;
                _temp_make_local_1191_39_208(208,__context__) = false;
                _temp_make_local_1191_39_208(209,__context__) = false;
                _temp_make_local_1191_39_208(210,__context__) = false;
                _temp_make_local_1191_39_208(211,__context__) = false;
                _temp_make_local_1191_39_208(212,__context__) = false;
                _temp_make_local_1191_39_208(213,__context__) = false;
                _temp_make_local_1191_39_208(214,__context__) = false;
                _temp_make_local_1191_39_208(215,__context__) = false;
                _temp_make_local_1191_39_208(216,__context__) = false;
                _temp_make_local_1191_39_208(217,__context__) = false;
                _temp_make_local_1191_39_208(218,__context__) = false;
                _temp_make_local_1191_39_208(219,__context__) = false;
                _temp_make_local_1191_39_208(220,__context__) = false;
                _temp_make_local_1191_39_208(221,__context__) = false;
                _temp_make_local_1191_39_208(222,__context__) = false;
                _temp_make_local_1191_39_208(223,__context__) = false;
                _temp_make_local_1191_39_208(224,__context__) = false;
                _temp_make_local_1191_39_208(225,__context__) = false;
                _temp_make_local_1191_39_208(226,__context__) = false;
                _temp_make_local_1191_39_208(227,__context__) = false;
                _temp_make_local_1191_39_208(228,__context__) = false;
                _temp_make_local_1191_39_208(229,__context__) = false;
                _temp_make_local_1191_39_208(230,__context__) = false;
                _temp_make_local_1191_39_208(231,__context__) = false;
                _temp_make_local_1191_39_208(232,__context__) = false;
                _temp_make_local_1191_39_208(233,__context__) = false;
                _temp_make_local_1191_39_208(234,__context__) = false;
                _temp_make_local_1191_39_208(235,__context__) = false;
                _temp_make_local_1191_39_208(236,__context__) = false;
                _temp_make_local_1191_39_208(237,__context__) = false;
                _temp_make_local_1191_39_208(238,__context__) = false;
                _temp_make_local_1191_39_208(239,__context__) = false;
                _temp_make_local_1191_39_208(240,__context__) = false;
                _temp_make_local_1191_39_208(241,__context__) = false;
                _temp_make_local_1191_39_208(242,__context__) = false;
                _temp_make_local_1191_39_208(243,__context__) = false;
                _temp_make_local_1191_39_208(244,__context__) = false;
                _temp_make_local_1191_39_208(245,__context__) = false;
                _temp_make_local_1191_39_208(246,__context__) = false;
                _temp_make_local_1191_39_208(247,__context__) = false;
                _temp_make_local_1191_39_208(248,__context__) = false;
                _temp_make_local_1191_39_208(249,__context__) = false;
                _temp_make_local_1191_39_208(250,__context__) = false;
                _temp_make_local_1191_39_208(251,__context__) = false;
                _temp_make_local_1191_39_208(252,__context__) = false;
                _temp_make_local_1191_39_208(253,__context__) = false;
                _temp_make_local_1191_39_208(254,__context__) = false;
                _temp_make_local_1191_39_208(255,__context__) = false;
                return _temp_make_local_1191_39_208;
            })())(__ch_rename_at_1190_512,__context__));
        });
        if ( !___terminal_result_26_rename_at_1195_511 )
        {
            if ( (__parser_rename_at_362_507.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_507.suppress_errors)))) && (__parser_rename_at_362_507.index == __parser_rename_at_362_507.longest_prefix) )
            {
                peg::ParsingError _temp_make_local_1213_49_496; _temp_make_local_1213_49_496;
                char * __rest_rename_at_1211_513 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_90, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_507.index))),false));
                char * __complete_message_rename_at_1212_514 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_91, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '65'-'90'  '95'  '97'-'122' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1211_513))));
                _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_507.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                    das_copy((_temp_make_local_1213_49_496.text),(__complete_message_rename_at_1212_514));
                    das_copy((_temp_make_local_1213_49_496.index),(__parser_rename_at_362_507.index));
                    return _temp_make_local_1213_49_496;
                })())));
            };
            return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast((([&]() -> AutoTuple<bool,char *,int32_t> {
                AutoTuple<bool,char *,int32_t> __mks_1216;
                das_zero(__mks_1216);
                return __mks_1216;
            })()));
        } else {
            _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_507),1);
        };
        char * __str_4_rename_at_566_515 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_507.input(0,__context__))));
        char * __fc_rename_at_567_516 = (char *)(((char * const )(builtin_string_slice1(__str_4_rename_at_566_515,__ext_pos_4_rename_at_564_510,__parser_rename_at_362_507.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        int32_t __ext_pos_5_rename_at_564_517 = __parser_rename_at_362_507.index;
        _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_507.suppress_errors),true);
        while ( true )
        {
            AutoTuple<bool,int32_t,int32_t> __res_rename_at_644_518; das_zero(__res_rename_at_644_518); das_move(__res_rename_at_644_518, das_invoke<AutoTuple<bool,int32_t,int32_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> AutoTuple<bool,int32_t,int32_t>{
                bool ___terminal_result_27_rename_at_1195_519 = das_invoke<bool>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> bool{
                    TDim<bool,256> _temp_make_local_1191_39_736; _temp_make_local_1191_39_736;
                    int32_t __ch_rename_at_1190_520 = ((int32_t)_FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_507)));
                    return das_auto_cast<bool>::cast((__ch_rename_at_1190_520 != -1) && (([&]() -> TDim<bool,256>& {
                        _temp_make_local_1191_39_736(0,__context__) = false;
                        _temp_make_local_1191_39_736(1,__context__) = false;
                        _temp_make_local_1191_39_736(2,__context__) = false;
                        _temp_make_local_1191_39_736(3,__context__) = false;
                        _temp_make_local_1191_39_736(4,__context__) = false;
                        _temp_make_local_1191_39_736(5,__context__) = false;
                        _temp_make_local_1191_39_736(6,__context__) = false;
                        _temp_make_local_1191_39_736(7,__context__) = false;
                        _temp_make_local_1191_39_736(8,__context__) = false;
                        _temp_make_local_1191_39_736(9,__context__) = false;
                        _temp_make_local_1191_39_736(10,__context__) = false;
                        _temp_make_local_1191_39_736(11,__context__) = false;
                        _temp_make_local_1191_39_736(12,__context__) = false;
                        _temp_make_local_1191_39_736(13,__context__) = false;
                        _temp_make_local_1191_39_736(14,__context__) = false;
                        _temp_make_local_1191_39_736(15,__context__) = false;
                        _temp_make_local_1191_39_736(16,__context__) = false;
                        _temp_make_local_1191_39_736(17,__context__) = false;
                        _temp_make_local_1191_39_736(18,__context__) = false;
                        _temp_make_local_1191_39_736(19,__context__) = false;
                        _temp_make_local_1191_39_736(20,__context__) = false;
                        _temp_make_local_1191_39_736(21,__context__) = false;
                        _temp_make_local_1191_39_736(22,__context__) = false;
                        _temp_make_local_1191_39_736(23,__context__) = false;
                        _temp_make_local_1191_39_736(24,__context__) = false;
                        _temp_make_local_1191_39_736(25,__context__) = false;
                        _temp_make_local_1191_39_736(26,__context__) = false;
                        _temp_make_local_1191_39_736(27,__context__) = false;
                        _temp_make_local_1191_39_736(28,__context__) = false;
                        _temp_make_local_1191_39_736(29,__context__) = false;
                        _temp_make_local_1191_39_736(30,__context__) = false;
                        _temp_make_local_1191_39_736(31,__context__) = false;
                        _temp_make_local_1191_39_736(32,__context__) = false;
                        _temp_make_local_1191_39_736(33,__context__) = false;
                        _temp_make_local_1191_39_736(34,__context__) = false;
                        _temp_make_local_1191_39_736(35,__context__) = false;
                        _temp_make_local_1191_39_736(36,__context__) = false;
                        _temp_make_local_1191_39_736(37,__context__) = false;
                        _temp_make_local_1191_39_736(38,__context__) = false;
                        _temp_make_local_1191_39_736(39,__context__) = false;
                        _temp_make_local_1191_39_736(40,__context__) = false;
                        _temp_make_local_1191_39_736(41,__context__) = false;
                        _temp_make_local_1191_39_736(42,__context__) = false;
                        _temp_make_local_1191_39_736(43,__context__) = false;
                        _temp_make_local_1191_39_736(44,__context__) = false;
                        _temp_make_local_1191_39_736(45,__context__) = false;
                        _temp_make_local_1191_39_736(46,__context__) = false;
                        _temp_make_local_1191_39_736(47,__context__) = false;
                        _temp_make_local_1191_39_736(48,__context__) = true;
                        _temp_make_local_1191_39_736(49,__context__) = true;
                        _temp_make_local_1191_39_736(50,__context__) = true;
                        _temp_make_local_1191_39_736(51,__context__) = true;
                        _temp_make_local_1191_39_736(52,__context__) = true;
                        _temp_make_local_1191_39_736(53,__context__) = true;
                        _temp_make_local_1191_39_736(54,__context__) = true;
                        _temp_make_local_1191_39_736(55,__context__) = true;
                        _temp_make_local_1191_39_736(56,__context__) = true;
                        _temp_make_local_1191_39_736(57,__context__) = true;
                        _temp_make_local_1191_39_736(58,__context__) = false;
                        _temp_make_local_1191_39_736(59,__context__) = false;
                        _temp_make_local_1191_39_736(60,__context__) = false;
                        _temp_make_local_1191_39_736(61,__context__) = false;
                        _temp_make_local_1191_39_736(62,__context__) = false;
                        _temp_make_local_1191_39_736(63,__context__) = false;
                        _temp_make_local_1191_39_736(64,__context__) = false;
                        _temp_make_local_1191_39_736(65,__context__) = true;
                        _temp_make_local_1191_39_736(66,__context__) = true;
                        _temp_make_local_1191_39_736(67,__context__) = true;
                        _temp_make_local_1191_39_736(68,__context__) = true;
                        _temp_make_local_1191_39_736(69,__context__) = true;
                        _temp_make_local_1191_39_736(70,__context__) = true;
                        _temp_make_local_1191_39_736(71,__context__) = true;
                        _temp_make_local_1191_39_736(72,__context__) = true;
                        _temp_make_local_1191_39_736(73,__context__) = true;
                        _temp_make_local_1191_39_736(74,__context__) = true;
                        _temp_make_local_1191_39_736(75,__context__) = true;
                        _temp_make_local_1191_39_736(76,__context__) = true;
                        _temp_make_local_1191_39_736(77,__context__) = true;
                        _temp_make_local_1191_39_736(78,__context__) = true;
                        _temp_make_local_1191_39_736(79,__context__) = true;
                        _temp_make_local_1191_39_736(80,__context__) = true;
                        _temp_make_local_1191_39_736(81,__context__) = true;
                        _temp_make_local_1191_39_736(82,__context__) = true;
                        _temp_make_local_1191_39_736(83,__context__) = true;
                        _temp_make_local_1191_39_736(84,__context__) = true;
                        _temp_make_local_1191_39_736(85,__context__) = true;
                        _temp_make_local_1191_39_736(86,__context__) = true;
                        _temp_make_local_1191_39_736(87,__context__) = true;
                        _temp_make_local_1191_39_736(88,__context__) = true;
                        _temp_make_local_1191_39_736(89,__context__) = true;
                        _temp_make_local_1191_39_736(90,__context__) = true;
                        _temp_make_local_1191_39_736(91,__context__) = false;
                        _temp_make_local_1191_39_736(92,__context__) = false;
                        _temp_make_local_1191_39_736(93,__context__) = false;
                        _temp_make_local_1191_39_736(94,__context__) = false;
                        _temp_make_local_1191_39_736(95,__context__) = true;
                        _temp_make_local_1191_39_736(96,__context__) = false;
                        _temp_make_local_1191_39_736(97,__context__) = true;
                        _temp_make_local_1191_39_736(98,__context__) = true;
                        _temp_make_local_1191_39_736(99,__context__) = true;
                        _temp_make_local_1191_39_736(100,__context__) = true;
                        _temp_make_local_1191_39_736(101,__context__) = true;
                        _temp_make_local_1191_39_736(102,__context__) = true;
                        _temp_make_local_1191_39_736(103,__context__) = true;
                        _temp_make_local_1191_39_736(104,__context__) = true;
                        _temp_make_local_1191_39_736(105,__context__) = true;
                        _temp_make_local_1191_39_736(106,__context__) = true;
                        _temp_make_local_1191_39_736(107,__context__) = true;
                        _temp_make_local_1191_39_736(108,__context__) = true;
                        _temp_make_local_1191_39_736(109,__context__) = true;
                        _temp_make_local_1191_39_736(110,__context__) = true;
                        _temp_make_local_1191_39_736(111,__context__) = true;
                        _temp_make_local_1191_39_736(112,__context__) = true;
                        _temp_make_local_1191_39_736(113,__context__) = true;
                        _temp_make_local_1191_39_736(114,__context__) = true;
                        _temp_make_local_1191_39_736(115,__context__) = true;
                        _temp_make_local_1191_39_736(116,__context__) = true;
                        _temp_make_local_1191_39_736(117,__context__) = true;
                        _temp_make_local_1191_39_736(118,__context__) = true;
                        _temp_make_local_1191_39_736(119,__context__) = true;
                        _temp_make_local_1191_39_736(120,__context__) = true;
                        _temp_make_local_1191_39_736(121,__context__) = true;
                        _temp_make_local_1191_39_736(122,__context__) = true;
                        _temp_make_local_1191_39_736(123,__context__) = false;
                        _temp_make_local_1191_39_736(124,__context__) = false;
                        _temp_make_local_1191_39_736(125,__context__) = false;
                        _temp_make_local_1191_39_736(126,__context__) = false;
                        _temp_make_local_1191_39_736(127,__context__) = false;
                        _temp_make_local_1191_39_736(128,__context__) = false;
                        _temp_make_local_1191_39_736(129,__context__) = false;
                        _temp_make_local_1191_39_736(130,__context__) = false;
                        _temp_make_local_1191_39_736(131,__context__) = false;
                        _temp_make_local_1191_39_736(132,__context__) = false;
                        _temp_make_local_1191_39_736(133,__context__) = false;
                        _temp_make_local_1191_39_736(134,__context__) = false;
                        _temp_make_local_1191_39_736(135,__context__) = false;
                        _temp_make_local_1191_39_736(136,__context__) = false;
                        _temp_make_local_1191_39_736(137,__context__) = false;
                        _temp_make_local_1191_39_736(138,__context__) = false;
                        _temp_make_local_1191_39_736(139,__context__) = false;
                        _temp_make_local_1191_39_736(140,__context__) = false;
                        _temp_make_local_1191_39_736(141,__context__) = false;
                        _temp_make_local_1191_39_736(142,__context__) = false;
                        _temp_make_local_1191_39_736(143,__context__) = false;
                        _temp_make_local_1191_39_736(144,__context__) = false;
                        _temp_make_local_1191_39_736(145,__context__) = false;
                        _temp_make_local_1191_39_736(146,__context__) = false;
                        _temp_make_local_1191_39_736(147,__context__) = false;
                        _temp_make_local_1191_39_736(148,__context__) = false;
                        _temp_make_local_1191_39_736(149,__context__) = false;
                        _temp_make_local_1191_39_736(150,__context__) = false;
                        _temp_make_local_1191_39_736(151,__context__) = false;
                        _temp_make_local_1191_39_736(152,__context__) = false;
                        _temp_make_local_1191_39_736(153,__context__) = false;
                        _temp_make_local_1191_39_736(154,__context__) = false;
                        _temp_make_local_1191_39_736(155,__context__) = false;
                        _temp_make_local_1191_39_736(156,__context__) = false;
                        _temp_make_local_1191_39_736(157,__context__) = false;
                        _temp_make_local_1191_39_736(158,__context__) = false;
                        _temp_make_local_1191_39_736(159,__context__) = false;
                        _temp_make_local_1191_39_736(160,__context__) = false;
                        _temp_make_local_1191_39_736(161,__context__) = false;
                        _temp_make_local_1191_39_736(162,__context__) = false;
                        _temp_make_local_1191_39_736(163,__context__) = false;
                        _temp_make_local_1191_39_736(164,__context__) = false;
                        _temp_make_local_1191_39_736(165,__context__) = false;
                        _temp_make_local_1191_39_736(166,__context__) = false;
                        _temp_make_local_1191_39_736(167,__context__) = false;
                        _temp_make_local_1191_39_736(168,__context__) = false;
                        _temp_make_local_1191_39_736(169,__context__) = false;
                        _temp_make_local_1191_39_736(170,__context__) = false;
                        _temp_make_local_1191_39_736(171,__context__) = false;
                        _temp_make_local_1191_39_736(172,__context__) = false;
                        _temp_make_local_1191_39_736(173,__context__) = false;
                        _temp_make_local_1191_39_736(174,__context__) = false;
                        _temp_make_local_1191_39_736(175,__context__) = false;
                        _temp_make_local_1191_39_736(176,__context__) = false;
                        _temp_make_local_1191_39_736(177,__context__) = false;
                        _temp_make_local_1191_39_736(178,__context__) = false;
                        _temp_make_local_1191_39_736(179,__context__) = false;
                        _temp_make_local_1191_39_736(180,__context__) = false;
                        _temp_make_local_1191_39_736(181,__context__) = false;
                        _temp_make_local_1191_39_736(182,__context__) = false;
                        _temp_make_local_1191_39_736(183,__context__) = false;
                        _temp_make_local_1191_39_736(184,__context__) = false;
                        _temp_make_local_1191_39_736(185,__context__) = false;
                        _temp_make_local_1191_39_736(186,__context__) = false;
                        _temp_make_local_1191_39_736(187,__context__) = false;
                        _temp_make_local_1191_39_736(188,__context__) = false;
                        _temp_make_local_1191_39_736(189,__context__) = false;
                        _temp_make_local_1191_39_736(190,__context__) = false;
                        _temp_make_local_1191_39_736(191,__context__) = false;
                        _temp_make_local_1191_39_736(192,__context__) = false;
                        _temp_make_local_1191_39_736(193,__context__) = false;
                        _temp_make_local_1191_39_736(194,__context__) = false;
                        _temp_make_local_1191_39_736(195,__context__) = false;
                        _temp_make_local_1191_39_736(196,__context__) = false;
                        _temp_make_local_1191_39_736(197,__context__) = false;
                        _temp_make_local_1191_39_736(198,__context__) = false;
                        _temp_make_local_1191_39_736(199,__context__) = false;
                        _temp_make_local_1191_39_736(200,__context__) = false;
                        _temp_make_local_1191_39_736(201,__context__) = false;
                        _temp_make_local_1191_39_736(202,__context__) = false;
                        _temp_make_local_1191_39_736(203,__context__) = false;
                        _temp_make_local_1191_39_736(204,__context__) = false;
                        _temp_make_local_1191_39_736(205,__context__) = false;
                        _temp_make_local_1191_39_736(206,__context__) = false;
                        _temp_make_local_1191_39_736(207,__context__) = false;
                        _temp_make_local_1191_39_736(208,__context__) = false;
                        _temp_make_local_1191_39_736(209,__context__) = false;
                        _temp_make_local_1191_39_736(210,__context__) = false;
                        _temp_make_local_1191_39_736(211,__context__) = false;
                        _temp_make_local_1191_39_736(212,__context__) = false;
                        _temp_make_local_1191_39_736(213,__context__) = false;
                        _temp_make_local_1191_39_736(214,__context__) = false;
                        _temp_make_local_1191_39_736(215,__context__) = false;
                        _temp_make_local_1191_39_736(216,__context__) = false;
                        _temp_make_local_1191_39_736(217,__context__) = false;
                        _temp_make_local_1191_39_736(218,__context__) = false;
                        _temp_make_local_1191_39_736(219,__context__) = false;
                        _temp_make_local_1191_39_736(220,__context__) = false;
                        _temp_make_local_1191_39_736(221,__context__) = false;
                        _temp_make_local_1191_39_736(222,__context__) = false;
                        _temp_make_local_1191_39_736(223,__context__) = false;
                        _temp_make_local_1191_39_736(224,__context__) = false;
                        _temp_make_local_1191_39_736(225,__context__) = false;
                        _temp_make_local_1191_39_736(226,__context__) = false;
                        _temp_make_local_1191_39_736(227,__context__) = false;
                        _temp_make_local_1191_39_736(228,__context__) = false;
                        _temp_make_local_1191_39_736(229,__context__) = false;
                        _temp_make_local_1191_39_736(230,__context__) = false;
                        _temp_make_local_1191_39_736(231,__context__) = false;
                        _temp_make_local_1191_39_736(232,__context__) = false;
                        _temp_make_local_1191_39_736(233,__context__) = false;
                        _temp_make_local_1191_39_736(234,__context__) = false;
                        _temp_make_local_1191_39_736(235,__context__) = false;
                        _temp_make_local_1191_39_736(236,__context__) = false;
                        _temp_make_local_1191_39_736(237,__context__) = false;
                        _temp_make_local_1191_39_736(238,__context__) = false;
                        _temp_make_local_1191_39_736(239,__context__) = false;
                        _temp_make_local_1191_39_736(240,__context__) = false;
                        _temp_make_local_1191_39_736(241,__context__) = false;
                        _temp_make_local_1191_39_736(242,__context__) = false;
                        _temp_make_local_1191_39_736(243,__context__) = false;
                        _temp_make_local_1191_39_736(244,__context__) = false;
                        _temp_make_local_1191_39_736(245,__context__) = false;
                        _temp_make_local_1191_39_736(246,__context__) = false;
                        _temp_make_local_1191_39_736(247,__context__) = false;
                        _temp_make_local_1191_39_736(248,__context__) = false;
                        _temp_make_local_1191_39_736(249,__context__) = false;
                        _temp_make_local_1191_39_736(250,__context__) = false;
                        _temp_make_local_1191_39_736(251,__context__) = false;
                        _temp_make_local_1191_39_736(252,__context__) = false;
                        _temp_make_local_1191_39_736(253,__context__) = false;
                        _temp_make_local_1191_39_736(254,__context__) = false;
                        _temp_make_local_1191_39_736(255,__context__) = false;
                        return _temp_make_local_1191_39_736;
                    })())(__ch_rename_at_1190_520,__context__));
                });
                if ( !___terminal_result_27_rename_at_1195_519 )
                {
                    if ( (__parser_rename_at_362_507.error_reporting && !(_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_507.suppress_errors)))) && (__parser_rename_at_362_507.index == __parser_rename_at_362_507.longest_prefix) )
                    {
                        peg::ParsingError _temp_make_local_1213_49_1024; _temp_make_local_1213_49_1024;
                        char * __rest_rename_at_1211_521 = (char *)(_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_92, cast<char * const >::from(((char *) "` at ")), cast<int32_t>::from(__parser_rename_at_362_507.index))),false));
                        char * __complete_message_rename_at_1212_522 = (char *)(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_93, cast<char *>::from(((char *) "Error: Expected charset item `\u001b[1mset( '48'-'57'  '65'-'90'  '95' )\u001b[0m")), cast<char *>::from(__rest_rename_at_1211_521))));
                        _FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b(__context__,das_arg<TArray<peg::ParsingError>>::pass(__parser_rename_at_362_507.errors),das_arg<peg::ParsingError>::pass((([&]() -> peg::ParsingError& {
                            das_copy((_temp_make_local_1213_49_1024.text),(__complete_message_rename_at_1212_522));
                            das_copy((_temp_make_local_1213_49_1024.index),(__parser_rename_at_362_507.index));
                            return _temp_make_local_1213_49_1024;
                        })())));
                    };
                    return /* <- */ das_auto_cast_move<AutoTuple<bool,int32_t,int32_t>>::cast((([&]() -> AutoTuple<bool,int32_t,int32_t> {
                        AutoTuple<bool,int32_t,int32_t> __mks_1216;
                        das_zero(__mks_1216);
                        return __mks_1216;
                    })()));
                } else {
                    _FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_362_507),1);
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
        _FuncbuiltinTickpopTick1161079256290593740_da418e66c1d8244e(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_362_507.suppress_errors));
        char * __str_5_rename_at_566_523 = (char *)(das_cast<char *>::cast(das_ref(__context__,__parser_rename_at_362_507.input(0,__context__))));
        char * __tail_rename_at_567_524 = (char *)(((char * const )(builtin_string_slice1(__str_5_rename_at_566_523,__ext_pos_5_rename_at_564_517,__parser_rename_at_362_507.index,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        char * __val_rename_at_1129_525; das_zero(__val_rename_at_1129_525); das_move(__val_rename_at_1129_525, (char *)(das_invoke<char *>::invoke(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> char *{
            return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__fc_rename_at_567_516),cast<char *>::from(__tail_rename_at_567_524),*__context__,nullptr)));
        })));
        AutoTuple<bool,char *,int32_t> __result_rename_at_1137_526; das_zero(__result_rename_at_1137_526); das_move(__result_rename_at_1137_526, (([&]() -> AutoTuple<bool,char *,int32_t> {
            AutoTuple<bool,char *,int32_t> __mkt_1137;
            das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__mkt_1137) = true;
            das_get_auto_tuple_field<char *,1,bool,char *,int32_t>::get(__mkt_1137) = __val_rename_at_1129_525;
            das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__mkt_1137) = __parser_rename_at_362_507.index;
            return __mkt_1137;
        })()));
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_1137_526);
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

inline AutoTuple<bool,char *,int32_t> _Funcparse_ident_Tickid_0x1_5767df53d96981e ( Context * __context__, spoof::invocationParserTickid_0x1 &  __parser_rename_at_146_527 )
{
    int32_t __mark_rename_at_147_528 = __parser_rename_at_146_527.index;
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026(__context__,das_arg<TTable<int32_t,AutoTuple<bool,char *,int32_t>>>::pass(__parser_rename_at_146_527.ident__cache),__parser_rename_at_146_527.index) && !(__parser_rename_at_146_527.error_reporting) )
    {
        AutoTuple<bool,char *,int32_t> __result_rename_at_155_529; das_zero(__result_rename_at_155_529); das_move(__result_rename_at_155_529, _FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245(__context__,das_arg<AutoTuple<bool,char *,int32_t>>::pass(__parser_rename_at_146_527.ident__cache(__parser_rename_at_146_527.index,__context__))));
        if ( das_get_auto_tuple_field<bool,0,bool,char *,int32_t>::get(__result_rename_at_155_529) )
        {
            das_copy(__parser_rename_at_146_527.index,das_get_auto_tuple_field<int32_t,2,bool,char *,int32_t>::get(__result_rename_at_155_529));
        };
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_155_529);
    } else {
        AutoTuple<bool,char *,int32_t> __result_rename_at_169_530; das_zero(__result_rename_at_169_530); das_move(__result_rename_at_169_530, _Funcparse_ident__innerTickid_0x1_391c4c7b7867934a(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_146_527)));
        das_copy(__parser_rename_at_146_527.ident__cache(__mark_rename_at_147_528,__context__),__result_rename_at_169_530);
        return /* <- */ das_auto_cast_move<AutoTuple<bool,char *,int32_t>>::cast(__result_rename_at_169_530);
    };
}

inline Program * _FuncbuiltinTickget_ptrTick8468476673553620226_ae31c60b8d8beb31 ( Context * __context__, smart_ptr_raw<Program> const  __src_rename_at_1726_531 )
{
    return das_auto_cast<Program *>::cast(das_cast<Program *>::cast(__src_rename_at_1726_531));
}

inline void instance_args_50995e25354ef5d2 ( Context * __context__, char * const  __invocation_rename_at_177_532, Block DAS_COMMENT((void,spoof::SpoofInvocation,TArray<peg::ParsingError> const )) const  &  __blk_rename_at_177_533 ) { das_stack_prologue __prologue(__context__,1200,"instance_args " DAS_FILE_LINE);
{
    spoof::invocationParserTickid_0x1 ___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534; memset((void*)&___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534,0,sizeof(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534));
    spoof::invocationParserTickid_0x1 __parser_rename_at_406_535; memset((void*)&__parser_rename_at_406_535,0,sizeof(__parser_rename_at_406_535));
    AutoTuple<bool,spoof::SpoofInvocation,int32_t> __result_rename_at_418_537; memset((void*)&__result_rename_at_418_537,0,sizeof(__result_rename_at_418_537));
    {
        AutoTuple<bool,spoof::SpoofInvocation,int32_t> _temp_make_local_419_22_1008; _temp_make_local_419_22_1008;
        /* finally */ auto __finally_405= das_finally([&](){
        finalize_bf2f98006be610c6(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534));
        finalize_bf2f98006be610c6(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_406_535));
        /* end finally */ });
        das_zero(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534);
        das_zero(__parser_rename_at_406_535);
        _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79(__context__,das_arg<TArray<bool>>::pass(__parser_rename_at_406_535.suppress_errors),false);
        builtin_string_peek(__invocation_rename_at_177_532,das_make_block<void,TArray<uint8_t> const  &>(__context__,944,0,&__func_info__920fb1be533a8af4,[&](TArray<uint8_t> const  & __inp_rename_at_414_536) -> void{
            _FuncbuiltinTickcloneTick3038771811667655495_a886d0bdf2ebbcc1(__context__,das_arg<TArray<uint8_t>>::pass(__parser_rename_at_406_535.input),__inp_rename_at_414_536);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_zero(__result_rename_at_418_537);
        _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_721189fff44b0b7c(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__result_rename_at_418_537),das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass((_temp_make_local_419_22_1008 = (_Funcparse_macro_invocationTickid_0x1_e41312d760106bd0(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(__parser_rename_at_406_535))))));
        if ( das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_418_537) )
        {
            TArray<peg::ParsingError> _temp_make_local_422_48_1056; _temp_make_local_422_48_1056;
            das_invoke<void>::invoke<spoof::SpoofInvocation &,TArray<peg::ParsingError> &>(__context__,nullptr,__blk_rename_at_177_533,das_arg<spoof::SpoofInvocation>::pass(das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_418_537)),das_arg<TArray<peg::ParsingError>>::pass((([&]() -> TArray<peg::ParsingError>& {
                das_zero(_temp_make_local_422_48_1056);
                return _temp_make_local_422_48_1056;
            })())));
            return ;
        } else {
            AutoTuple<bool,spoof::SpoofInvocation,int32_t> _temp_make_local_438_22_1152; _temp_make_local_438_22_1152;
            _FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79(__context__,das_arg<TArray<bool>>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.suppress_errors),false);
            builtin_string_peek(__invocation_rename_at_177_532,das_make_block<void,TArray<uint8_t> const  &>(__context__,1136,0,&__func_info__920fb1be533a8af4,[&](TArray<uint8_t> const  & __inp_rename_at_431_538) -> void{
                _FuncbuiltinTickcloneTick3038771811667655495_a886d0bdf2ebbcc1(__context__,das_arg<TArray<uint8_t>>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.input),__inp_rename_at_431_538);
            }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            das_copy(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.error_reporting,true);
            das_copy(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.longest_prefix,__parser_rename_at_406_535.longest_prefix);
            _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_721189fff44b0b7c(__context__,das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass(__result_rename_at_418_537),das_arg<AutoTuple<bool,spoof::SpoofInvocation,int32_t>>::pass((_temp_make_local_438_22_1152 = (_Funcparse_macro_invocationTickid_0x1_e41312d760106bd0(__context__,das_arg<spoof::invocationParserTickid_0x1>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534))))));
            _FuncalgorithmTicksort_uniqueTick14985796887508933377_fd064d4ca86f9c39(__context__,das_arg<TArray<peg::ParsingError>>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.errors));
            das_invoke<void>::invoke<spoof::SpoofInvocation &,TArray<peg::ParsingError> &>(__context__,nullptr,__blk_rename_at_177_533,das_arg<spoof::SpoofInvocation>::pass(das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__result_rename_at_418_537)),das_arg<TArray<peg::ParsingError>>::pass(___Varerror_reporting_parserTickatTick428Tick24_rename_at_428_534.errors));
        };
    };
}}

inline void clone_1d0149763c60891a ( Context * __context__, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> &  __dest_rename_at_170_539, AutoTuple<bool,TArray<AutoTuple<char *,char *>>,int32_t> const  &  __src_rename_at_170_540 )
{
    das_copy(das_get_auto_tuple_field<bool,0,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__dest_rename_at_170_539),das_get_auto_tuple_field<bool,0,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__src_rename_at_170_540));
    _FuncbuiltinTickcloneTick3038771811667655495_c57c3779e2d803ca(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(das_get_auto_tuple_field<TArray<AutoTuple<char *,char *>>,1,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__dest_rename_at_170_539)),das_get_auto_tuple_field<TArray<AutoTuple<char *,char *>>,1,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__src_rename_at_170_540));
    das_copy(das_get_auto_tuple_field<int32_t,2,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__dest_rename_at_170_539),das_get_auto_tuple_field<int32_t,2,bool,TArray<AutoTuple<char *,char *>>,int32_t>::get(__src_rename_at_170_540));
}

inline void clone_33841e01eec4fe3e ( Context * __context__, AutoTuple<bool,spoof::SpoofInvocation,int32_t> &  __dest_rename_at_170_541, AutoTuple<bool,spoof::SpoofInvocation,int32_t> const  &  __src_rename_at_170_542 )
{
    das_copy(das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__dest_rename_at_170_541),das_get_auto_tuple_field<bool,0,bool,spoof::SpoofInvocation,int32_t>::get(__src_rename_at_170_542));
    clone_ae0e620d108403a2(__context__,das_arg<spoof::SpoofInvocation>::pass(das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__dest_rename_at_170_541)),das_get_auto_tuple_field<spoof::SpoofInvocation,1,bool,spoof::SpoofInvocation,int32_t>::get(__src_rename_at_170_542));
    das_copy(das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__dest_rename_at_170_541),das_get_auto_tuple_field<int32_t,2,bool,spoof::SpoofInvocation,int32_t>::get(__src_rename_at_170_542));
}

inline void clone_2562f74cf119ba95 ( Context * __context__, AutoTuple<bool,TArray<char *>,int32_t> &  __dest_rename_at_170_543, AutoTuple<bool,TArray<char *>,int32_t> const  &  __src_rename_at_170_544 )
{
    das_copy(das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__dest_rename_at_170_543),das_get_auto_tuple_field<bool,0,bool,TArray<char *>,int32_t>::get(__src_rename_at_170_544));
    _FuncbuiltinTickcloneTick3038771811667655495_fd34c08266227f18(__context__,das_arg<TArray<char *>>::pass(das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(__dest_rename_at_170_543)),das_get_auto_tuple_field<TArray<char *>,1,bool,TArray<char *>,int32_t>::get(__src_rename_at_170_544));
    das_copy(das_get_auto_tuple_field<int32_t,2,bool,TArray<char *>,int32_t>::get(__dest_rename_at_170_543),das_get_auto_tuple_field<int32_t,2,bool,TArray<char *>,int32_t>::get(__src_rename_at_170_544));
}

inline void clone_ae0e620d108403a2 ( Context * __context__, spoof::SpoofInvocation &  __a_rename_at_172_545, spoof::SpoofInvocation const  &  __b_rename_at_172_546 )
{
    das_copy(__a_rename_at_172_545.varName,__b_rename_at_172_546.varName);
    _FuncbuiltinTickcloneTick3038771811667655495_fd34c08266227f18(__context__,das_arg<TArray<char *>>::pass(__a_rename_at_172_545.args),__b_rename_at_172_546.args);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xe511e0fbab95040c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_a8fc758b39196373>>();
    };
    aotLib[0x78948a08f0d98f51] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8>>();
    };
    aotLib[0xb6e97b3a0934a282] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_7afa66130e8b5e54>>();
    };
    aotLib[0x8d7b855a3f42d88] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_6ece99d569f84073>>();
    };
    aotLib[0xa6ee4522901c12ce] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_2218e7aaad09471d>>();
    };
    aotLib[0xc0c04de3f4c76d15] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_45dfd694aadf378b>>();
    };
    aotLib[0xb59a75091585764a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_cfe1ed3b98f2f17>>();
    };
    aotLib[0x2d377846e2d94fe5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_eddf182da4ded640>>();
    };
    aotLib[0x8c33c18ee728d399] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_78307bdac119941d>>();
    };
    aotLib[0x96b4a1cd82d5c3d9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_51b896d19d78efaa>>();
    };
    aotLib[0x15341e2be776b55e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_a373e563dce81dd7>>();
    };
    aotLib[0xb9547032d852923c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_fca84cc9cd8fb0e4>>();
    };
    aotLib[0xc6cb2f5df5d9f72c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_7358c6086ef94f68>>();
    };
    aotLib[0x2479b6fb1886fcf5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_862595edb53225a1>>();
    };
    aotLib[0x4ec778d1a5b9771d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_e9039b88c532c9ff>>();
    };
    aotLib[0xc502128bbf7c5d21] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176>>();
    };
    aotLib[0x734e45a36711660] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e>>();
    };
    aotLib[0x380b7455b0a2f9f5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1>>();
    };
    aotLib[0xf7a71d8d7a342eb3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b0722452a1c28717>>();
    };
    aotLib[0x92d5b2bdb4b14a67] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_8e4364e894fa22ef>>();
    };
    aotLib[0x1b9aa991c9d71d5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_9bd0f12f34dd31a3>>();
    };
    aotLib[0x36f654787083381] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_f2e4758bc132fae7>>();
    };
    aotLib[0x132733c8099193e6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_9d118576d5107c28>>();
    };
    aotLib[0x1c0fe1f251eb269c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpush_cloneTick2035469273396957942_d452bb76b753397e>>();
    };
    aotLib[0x1f8eb5798352d074] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb>>();
    };
    aotLib[0x216c58576015519e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_e5d8bd461b7dcb17>>();
    };
    aotLib[0xbce9b7e0c95a24ba] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_40a623f9c95b0cb9>>();
    };
    aotLib[0x40f852a6ba31c742] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_53b9400c23ef0c99>>();
    };
    aotLib[0xd01f1351a57c0f69] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_d1c7a131ecc21c31>>();
    };
    aotLib[0x570a46511e69fdd5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_8872455bc7f9c5ef>>();
    };
    aotLib[0x9920d08e13cf5d4d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_84cf6af8e0041469>>();
    };
    aotLib[0x8707409c4c4d54ce] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_5f01338060689947>>();
    };
    aotLib[0x320f4500530d081e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_d582432df8818d6e>>();
    };
    aotLib[0x8e81afc715bbd140] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_f74cc0676a04cd8>>();
    };
    aotLib[0x415ecf28208e07b0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_77cc2981fc884245>>();
    };
    aotLib[0xa3f3a5fa8d6ac798] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_9dc279ef3efd2937>>();
    };
    aotLib[0x9b307f7167bbec5a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_3e547859692e768f>>();
    };
    aotLib[0xb9c682910c711f21] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_a886d0bdf2ebbcc1>>();
    };
    aotLib[0xe0be72b18f4f052d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_c57c3779e2d803ca>>();
    };
    aotLib[0xbcd4c25d5b8b9b7d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_fd34c08266227f18>>();
    };
    aotLib[0xc1afa0c45e3c0e6d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicktabulateTick3664919181896551618_5938a8f00640e7b0>>();
    };
    aotLib[0xae258387a0c634d6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FunccolorsTickblue_strTick419715327456344553_ccac9a00db81a9f1>>();
    };
    aotLib[0x74d54f4ac4eb4727] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FunccolorsTickgreen_strTick14807051613123094884_df8dd133b2accbc3>>();
    };
    aotLib[0x23d1fc4074fb65ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_aa0264416ea4124a>>();
    };
    aotLib[0xfd12020bee0e8fc3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickreached_EOFTick12193995001429396764_74c5ca3a73c6ab1f>>();
    };
    aotLib[0xd4711fd1c134f5e7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicktabulateTick3664919181896551618_52ff143bf46df83f>>();
    };
    aotLib[0xe7e6168937c24e2a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickreached_EOFTick12193995001429396764_76116504adfe2552>>();
    };
    aotLib[0xedb5b47d288fe915] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemptyTick15399874715904164783_8d8767d901055902>>();
    };
    aotLib[0x45dc0e0c4f87093c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicksortTick14088969635007481283_2b1d2e9c3495b624>>();
    };
    aotLib[0xa3bc959af4693b28] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncalgorithmTickuniqueTick8642070526899511001_97dad40955268394>>();
    };
    aotLib[0x102e7c0c7a4cfe9b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ba4ceaf27ff6ff46>>();
    };
    aotLib[0xfa13e8480165404b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_7ecb0a5d4721f3ce>>();
    };
    aotLib[0x6c738e340178c2c4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e53981a5c0b0dc1>>();
    };
    aotLib[0x748d053b6ddd177b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_e8860640a3da483d>>();
    };
    aotLib[0xc8e74fdd926d36c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a887f80fd036fc6a>>();
    };
    aotLib[0x3800c5f97f161217] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_32af3ec8801e47ea>>();
    };
    aotLib[0xd60296124b9caf45] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_2afaf9cdb69403d4>>();
    };
    aotLib[0x2dfbdc0340c38f96] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_74d3e26b89ca39c1>>();
    };
    aotLib[0xe97ac22b8a181839] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_721189fff44b0b7c>>();
    };
    aotLib[0xcb74b73c039e3dd6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_435b13646b15cb2c>>();
    };
    aotLib[0xf623fb8ef2eacf51] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_c94fb40a98d27d95>>();
    };
    aotLib[0x64700d557207cf22] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_infoTick4618819779667933862_79989224586a0f97>>();
    };
    aotLib[0x9fb4568129a4b6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickskip_whitespaceTick3657885083884260190_3fb7288c42068266>>();
    };
    aotLib[0x4871c46e6756f50a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_plainTick18061110541707930728_22d44fb5d6ba6b83>>();
    };
    aotLib[0x9809dab4fed3560b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickmatchesTick4020677071890555356_9e1f950256966e0c>>();
    };
    aotLib[0x7ea9f73f79a370f7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_failTick11731645277795022368_b3bcabd40e85a2f6>>();
    };
    aotLib[0x7a7807bfb4554c8c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickbackTick18296309835877697278_7e6879d98672cf1>>();
    };
    aotLib[0x11417bed914982b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FunccolorsTickred_strTick17114617365443413298_5bf8a5556296c584>>();
    };
    aotLib[0xd7a02d8400ef10ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_9a9bd56128d77e7b>>();
    };
    aotLib[0x63acab79d7aea3c4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_successTick3645718276917011819_48df15f72c464176>>();
    };
    aotLib[0xde5e529e24b24e42] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_3cd12c1f7e0e3a79>>();
    };
    aotLib[0xc6b2f008007a935] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_ca5d53fe6f76d114>>();
    };
    aotLib[0xe295497dc857473d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_da418e66c1d8244e>>();
    };
    aotLib[0x853ebbce39c5f6ff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_39183d2b340e1e79>>();
    };
    aotLib[0x41ddbb3344863172] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_57ae76d5fb6c64b9>>();
    };
    aotLib[0x76e749f15754d85f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_61012b8bb2fce779>>();
    };
    aotLib[0xba714b17cce76d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickget_current_charTick5927279557575598025_e3ef590842b05710>>();
    };
    aotLib[0xdcd8029b9dc740ca] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickmoveTick4844919725508203339_10d0d871a12fe16d>>();
    };
    aotLib[0xaaa96287c646cada] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncpegTickmatch_string_literalTick12800953725978677998_f7109d6e87737ab6>>();
    };
    aotLib[0xda45d90f47e091ff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_97f64505aaf18026>>();
    };
    aotLib[0x5211b09ded1ce193] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_664b3b92dd33b520>>();
    };
    aotLib[0xbbe654d0ae175f60] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick16475640899284277631_597abbecf5b26d8>>();
    };
    aotLib[0xd386f8646b7541a3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_infoTick4618819779667933862_efdc190cc878d369>>();
    };
    aotLib[0xb7d5f102d2852b3b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickskip_whitespaceTick3657885083884260190_1a04dd008fff4b65>>();
    };
    aotLib[0x2d2056465dddef8c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_plainTick18061110541707930728_8c201494798c4721>>();
    };
    aotLib[0xd327117324d1807e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickmatchesTick4020677071890555356_83d30930705988a2>>();
    };
    aotLib[0x9d5a95c93e42b0f6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_failTick11731645277795022368_ae7aad9b0b952b34>>();
    };
    aotLib[0x94a09e36b4a439e4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTicklog_successTick3645718276917011819_237573808f7a2c5c>>();
    };
    aotLib[0x6d70fe0458aa4377] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_33cfc90a36a139ab>>();
    };
    aotLib[0x43b9711d3a877643] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_47edd36ebc80950e>>();
    };
    aotLib[0xebc8de4458770db0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickget_current_charTick5927279557575598025_cd70b30469027b07>>();
    };
    aotLib[0xa9135dac98278079] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncpegTickmoveTick4844919725508203339_cc17fdbb42048d3a>>();
    };
    aotLib[0xcd3a590a3f884d82] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncpegTickmatch_string_literalTick12800953725978677998_f6b931b539266e97>>();
    };
    aotLib[0xd180e844bd619f81] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncalgorithmTicksort_uniqueTick14985796887508933377_fd064d4ca86f9c39>>();
    };
    aotLib[0x3a4f3e6320471b69] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_b0cebf59e5e51d47>>();
    };
    aotLib[0x750201432f310f2e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick16475640899284277631_8fb28bab1278e8c2>>();
    };
    aotLib[0x2970592eb43c096f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_bf2f98006be610c6>>();
    };
    aotLib[0xb936b537957084c3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_list_innerTickid_0x0_1c48b25484abe536>>();
    };
    aotLib[0x1b21cfa4fa744a85] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_listTickid_0x0_d172783e9d6cc855>>();
    };
    aotLib[0x9d8ad22d89a40035] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_comma_separated_elements_innerTickid_0x0_7f14badd440f98f>>();
    };
    aotLib[0x679e83cab188f399] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_comma_separated_elementsTickid_0x0_84b92b09aca2a83b>>();
    };
    aotLib[0x628921fb26335afa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_innerTickid_0x0_70aae0654fd76c21>>();
    };
    aotLib[0x23361c64c243bc2d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_elementTickid_0x0_85965a755893ca3>>();
    };
    aotLib[0xb4653b9bec639b7f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_sub_element_innerTickid_0x0_4d49965cb6bc1ab8>>();
    };
    aotLib[0xfba29faeb63d1a66] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_sub_elementTickid_0x0_c8a6c75b473a3132>>();
    };
    aotLib[0x76cb0bf3e6e21d68] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_value_innerTickid_0x0_62f6fb32b8e5e998>>();
    };
    aotLib[0x26092beb128a383b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_valueTickid_0x0_4e09996913410390>>();
    };
    aotLib[0x5889ed4ceaefd90e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_ident__innerTickid_0x0_79268295f94867d7>>();
    };
    aotLib[0x2023200ced3fc350] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_ident_Tickid_0x0_20b6fcdb44c1b1c0>>();
    };
    aotLib[0x56aed9f0fde0dd26] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57>>();
    };
    aotLib[0xcba4b4c693ad27e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67>>();
    };
    aotLib[0x3c395992954dd60d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_d626258b591934af>>();
    };
    aotLib[0xde6e54abcc6b70c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1>>();
    };
    aotLib[0x9e683f3840fb4e50] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5>>();
    };
    aotLib[0x171c354280f250f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_index_ifTick5144745413123287381_d7241f91c631a1ce>>();
    };
    aotLib[0xc5b7281748a897d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_macro_invocation_innerTickid_0x1_fc3224332b02420d>>();
    };
    aotLib[0x1fbaa2ec1cb42c3e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_macro_invocationTickid_0x1_e41312d760106bd0>>();
    };
    aotLib[0x6b2fbc37976cb636] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_list_innerTickid_0x1_4ddd8d5780e55324>>();
    };
    aotLib[0xf98d02d67963ca9e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_listTickid_0x1_f9f95d60cca895e9>>();
    };
    aotLib[0x5511e1dae9d4e4d5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_comma_separated_elements_innerTickid_0x1_4a764230365b1982>>();
    };
    aotLib[0x9d0ea8c11ab76edb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_comma_separated_elementsTickid_0x1_a5eb4f00d7b7be>>();
    };
    aotLib[0x894f5c53f3dab51d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_sub_element_innerTickid_0x1_cb2b3527b22dde21>>();
    };
    aotLib[0x8a6d5b8703b37d82] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_sub_elementTickid_0x1_9dd0aa4f6e723d42>>();
    };
    aotLib[0xfed3a4b5ce1f5fc9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_element_innerTickid_0x1_82f0e6f9b68f3345>>();
    };
    aotLib[0x51f35b38c5d7613b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_elementTickid_0x1_609c6e555a4e4af4>>();
    };
    aotLib[0x94686c1362138b65] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_class_name_innerTickid_0x1_151c0b46e0f35deb>>();
    };
    aotLib[0x30c3f92d15a96bc7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_class_nameTickid_0x1_ecd2c3e791924e42>>();
    };
    aotLib[0x9ac8337da837a6dd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_ident__innerTickid_0x1_391c4c7b7867934a>>();
    };
    aotLib[0xbcfe7c5cc48bd7db] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funcparse_ident_Tickid_0x1_5767df53d96981e>>();
    };
    aotLib[0x911c6cf074983819] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_ae31c60b8d8beb31>>();
    };
    aotLib[0x2e20ee6fc5a03a8c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&instance_args_50995e25354ef5d2>>();
    };
    aotLib[0x4703e055ada6b929] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_1d0149763c60891a>>();
    };
    aotLib[0x7b189f1b6b94654c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_33841e01eec4fe3e>>();
    };
    aotLib[0xfcbfee38a50aef53] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_2562f74cf119ba95>>();
    };
    aotLib[0x9f304953134e9467] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_ae0e620d108403a2>>();
    };
    // [[ init script ]]
    aotLib[0xcc9e8cb9e146a9fe] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_10448736428990234716
AotListBase impl_aot_spoof(_anon_10448736428990234716::registerAotFunctions);
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
