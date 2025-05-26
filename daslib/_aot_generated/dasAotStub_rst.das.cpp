#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require fio
#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_fio.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
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
 // require regex
 // require regex_boost
 // require rst

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
namespace _anon_17275503207427487378 {

namespace rst { struct DocGroup; };
namespace rst { struct _lambda_rst_1216_1; };
namespace fio { struct df_header; };
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
namespace regex { struct ReNode; };
namespace regex { struct Regex; };
namespace regex_boost { struct RegexReader; };
// unused enumeration ConversionResult
// unused structure df_header
// unused enumeration CompilationError
#if 0 // external enum
namespace rtti {

enum class ConstMatters : int32_t {
    no = int32_t(INT64_C(0)),
    yes = int32_t(INT64_C(1)),
};
}
#endif // external enum
#if 0 // external enum
namespace rtti {

enum class RefMatters : int32_t {
    no = int32_t(INT64_C(0)),
    yes = int32_t(INT64_C(1)),
};
}
#endif // external enum
#if 0 // external enum
namespace rtti {

enum class TemporaryMatters : int32_t {
    no = int32_t(INT64_C(0)),
    yes = int32_t(INT64_C(1)),
};
}
#endif // external enum
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
namespace regex {

enum class ReOp : int32_t {
    Char = int32_t(0),
    Set = int32_t(1),
    Any = int32_t(2),
    Eos = int32_t(3),
    Group = int32_t(4),
    Plus = int32_t(5),
    Star = int32_t(6),
    Question = int32_t(7),
    Concat = int32_t(8),
    Union = int32_t(9),
};
}
}
template <> struct cast< das::_anon_17275503207427487378::regex::ReOp > : cast_enum < das::_anon_17275503207427487378::regex::ReOp > {};
namespace _anon_17275503207427487378 {
namespace regex {

struct ReNode {
    DAS_COMMENT(enum) regex::ReOp op;
    int32_t id;
    Func DAS_COMMENT((uint8_t const  *,regex::Regex,regex::ReNode *,uint8_t const  * const )) fun2;
    Func DAS_COMMENT((void,regex::ReNode *,Sequence DAS_COMMENT((uint32_t)),StringBuilderWriter)) gen2;
    range at;
    char * text;
    int32_t textLen;
    TArray<regex::ReNode *> all;
    regex::ReNode * left;
    regex::ReNode * right;
    regex::ReNode * subexpr;
    regex::ReNode * next;
    TDim<uint32_t,8> cset;
    int32_t index;
    uint8_t const  * tail;
};
}
namespace regex {

struct Regex {
    regex::ReNode * root;
    uint8_t const  * match;
    TArray<AutoTuple<range,char *>> groups;
    TDim<uint32_t,8> earlyOut;
    bool canEarlyOut;
};
}
// unused structure RegexReader
namespace rst {

struct DocGroup {
    char * name;
    TArray<Function *> func;
    bool hidden;
};
}
namespace rst {

struct _lambda_rst_1216_1 {
    Func DAS_COMMENT((bool,rst::_lambda_rst_1216_1,char * * &)) __lambda;
    Func DAS_COMMENT((void,rst::_lambda_rst_1216_1 *)) __finalize;
    int32_t __yield;
    int32_t index;
    TArray<char *> snp;
    bool _loop_at_1216_35;
    int32_t __i_rename_at_1216_39;
    void * _pvar_0_at_1216_35;
    Sequence DAS_COMMENT((int32_t)) _source_0_at_1216_35;
};
}
extern StructInfo __struct_info__d775c4f039534efe;
extern StructInfo __struct_info__837d1a0a555de4a2;
extern TypeInfo __type_info__774985c53b7f8953;
extern TypeInfo __type_info__6b1c7db4b71a781f;
extern TypeInfo __type_info__f7046ae574272550;
extern TypeInfo __type_info__f08d475fe5d680fc;
extern TypeInfo __type_info__e705b0d7eea4be2c;
extern TypeInfo __type_info__7521b1289fcc2edb;
extern TypeInfo __type_info__3c61146b2bdfb90;
extern TypeInfo __type_info__16d0aa3dd6b69257;
extern TypeInfo __type_info__fa593d0882a72913;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__cd505ad3b1c59cc6;
extern TypeInfo __type_info__a57bf935c2dd03;
extern TypeInfo __type_info__60d16a2d23420951;
extern TypeInfo __type_info__ce241e3005cc873b;
extern TypeInfo __type_info__d0872eb2c1d28358;
extern TypeInfo __type_info__ccd32e474e9a33eb;
extern TypeInfo __type_info__98ed2eed8b7795f1;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__8e26ff93c3acecc5;
extern TypeInfo __type_info__888650585a1a67a;
extern TypeInfo __type_info__f0a0a5963e48bb5f;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__1b2f537dda20b669;
extern VarInfo __var_info__ec89c0a8da510f28;
extern VarInfo __var_info__ec83c0a8da46dd28;
extern VarInfo __var_info__f05dbeaf5dbf985d;
extern VarInfo __var_info__2a896b18ac8c5dc;
extern VarInfo __var_info__c3630b856e2c8315;
extern VarInfo __var_info__882ea441b2e06229;
extern VarInfo __var_info__c15d399143d0bdf0;
extern VarInfo __var_info__5f478141901ca6f8;
extern VarInfo __var_info__ff0b0abb7a32db38;
extern VarInfo __var_info__e668a7c47b738e21;
extern VarInfo __var_info__744f16ce3a0187d7;
extern VarInfo __var_info__94e091f447dc1a54;
extern VarInfo __var_info__2d3c72fd421646d6;
extern VarInfo __var_info__22f7a2c7fa9c2063;
extern VarInfo __var_info__21aa09afb788eee7;
extern VarInfo __var_info__aeeeca2da7dab890;
extern VarInfo __var_info__efb62fe9be79f20e;
extern VarInfo __var_info__48d8485ec58e9054;
extern VarInfo __var_info__7ecfc669c72ec2f6;
extern VarInfo __var_info__85f0f7cef5dd86b7;
extern VarInfo __var_info__f56dcd204992ec23;
extern VarInfo __var_info__932b48247e2ac692;
extern VarInfo __var_info__62310535c768e51e;
extern VarInfo __var_info__c8b6e136feca1e7d;
extern VarInfo __var_info__bdf35432817faabe;
extern VarInfo __var_info__83c4685915b4f96;
extern VarInfo __var_info__5ac39c9b48fed41d;
extern VarInfo __var_info__5671bd5411275d07;
extern FuncInfo __func_info__6edfc1622f67255a;
extern FuncInfo __func_info__4bda63c2c7a42bef;
extern FuncInfo __func_info__f381b060f4dbce7;
extern FuncInfo __func_info__806bd08fbfbd09f4;
extern FuncInfo __func_info__8075d08fbfce07f4;
extern FuncInfo __func_info__aa03b7bcb8f14157;
extern FuncInfo __func_info__762f57a17c2dce28;
extern FuncInfo __func_info__9beb038de389c5b0;
extern FuncInfo __func_info__438466bc7bed9910;
extern FuncInfo __func_info__5f3d7fba1c40074b;
extern FuncInfo __func_info__2701c05c12e53ee0;
extern FuncInfo __func_info__84319ec9af82a642;
extern FuncInfo __func_info__e0afcd004fbe6de3;

VarInfo __struct_info__d775c4f039534efe_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x5f478141901ca6f8), "name", offsetof(rst::DocGroup,name), 1 };
VarInfo __struct_info__d775c4f039534efe_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__f7046ae574272550, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x882ea441b2e06229), "func", offsetof(rst::DocGroup,func), 3 };
VarInfo __struct_info__d775c4f039534efe_field_2 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xc15d399143d0bdf0), "hidden", offsetof(rst::DocGroup,hidden), 0 };
VarInfo * __struct_info__d775c4f039534efe_fields[3] =  { &__struct_info__d775c4f039534efe_field_0, &__struct_info__d775c4f039534efe_field_1, &__struct_info__d775c4f039534efe_field_2 };
StructInfo __struct_info__d775c4f039534efe = {"DocGroup", "rst", 28, __struct_info__d775c4f039534efe_fields, 3, 40, UINT64_C(0x0), nullptr, UINT64_C(0xd775c4f039534efe), 0 };
TypeInfo * __type_info__744f16ce3a0187d7_arg_types_var_9474757822741603490[2] = { &__type_info__8e26ff93c3acecc5, &__type_info__774985c53b7f8953 };
const char * __type_info__744f16ce3a0187d7_arg_names_var_9474757822741603490[2] = { "__this", "_ryield1216" };
VarInfo __struct_info__837d1a0a555de4a2_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__744f16ce3a0187d7_arg_types_var_9474757822741603490, __type_info__744f16ce3a0187d7_arg_names_var_9474757822741603490, 2, 0, nullptr, 12, 8, UINT64_C(0x744f16ce3a0187d7), "__lambda", offsetof(rst::_lambda_rst_1216_1,__lambda), 0 };
TypeInfo * __type_info__ff0b0abb7a32db38_arg_types_var_9474757822741603490[1] = { &__type_info__f08d475fe5d680fc };
const char * __type_info__ff0b0abb7a32db38_arg_names_var_9474757822741603490[1] = { "__this" };
VarInfo __struct_info__837d1a0a555de4a2_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ff0b0abb7a32db38_arg_types_var_9474757822741603490, __type_info__ff0b0abb7a32db38_arg_names_var_9474757822741603490, 1, 0, nullptr, 12, 8, UINT64_C(0xff0b0abb7a32db38), "__finalize", offsetof(rst::_lambda_rst_1216_1,__finalize), 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x94e091f447dc1a54), "__yield", offsetof(rst::_lambda_rst_1216_1,__yield), 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaeeeca2da7dab890), "index", offsetof(rst::_lambda_rst_1216_1,index), 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_4 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xefb62fe9be79f20e), "snp", offsetof(rst::_lambda_rst_1216_1,snp), 7 };
VarInfo __struct_info__837d1a0a555de4a2_field_5 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x2d3c72fd421646d6), "_loop_at_1216_35", offsetof(rst::_lambda_rst_1216_1,_loop_at_1216_35), 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_6 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xe668a7c47b738e21), "__i_rename_at_1216_39", offsetof(rst::_lambda_rst_1216_1,__i_rename_at_1216_39), 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_7 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x22f7a2c7fa9c2063), "_pvar_0_at_1216_35", offsetof(rst::_lambda_rst_1216_1,_pvar_0_at_1216_35), 8 };
VarInfo __struct_info__837d1a0a555de4a2_field_8 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0x21aa09afb788eee7), "_source_0_at_1216_35", offsetof(rst::_lambda_rst_1216_1,_source_0_at_1216_35), 9 };
VarInfo * __struct_info__837d1a0a555de4a2_fields[9] =  { &__struct_info__837d1a0a555de4a2_field_0, &__struct_info__837d1a0a555de4a2_field_1, &__struct_info__837d1a0a555de4a2_field_2, &__struct_info__837d1a0a555de4a2_field_3, &__struct_info__837d1a0a555de4a2_field_4, &__struct_info__837d1a0a555de4a2_field_5, &__struct_info__837d1a0a555de4a2_field_6, &__struct_info__837d1a0a555de4a2_field_7, &__struct_info__837d1a0a555de4a2_field_8 };
StructInfo __struct_info__837d1a0a555de4a2 = {"_lambda_rst_1216_1", "rst", 30, __struct_info__837d1a0a555de4a2_fields, 9, 72, UINT64_C(0x0), nullptr, UINT64_C(0x837d1a0a555de4a2), 4 };
VarInfo __func_info__6edfc1622f67255a_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x48d8485ec58e9054), "stub_file", 0, 0 };
VarInfo * __func_info__6edfc1622f67255a_fields[1] =  { &__func_info__6edfc1622f67255a_field_0 };
FuncInfo __func_info__6edfc1622f67255a = {"invoke block<(stub_file:fio::FILE const? const):void> const", "", __func_info__6edfc1622f67255a_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x6edfc1622f67255a), 0x0 };
VarInfo __func_info__4bda63c2c7a42bef_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x7ecfc669c72ec2f6), "topic_file", 0, 0 };
VarInfo * __func_info__4bda63c2c7a42bef_fields[1] =  { &__func_info__4bda63c2c7a42bef_field_0 };
FuncInfo __func_info__4bda63c2c7a42bef = {"invoke block<(topic_file:fio::FILE const? const):void> const", "", __func_info__4bda63c2c7a42bef_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x4bda63c2c7a42bef), 0x0 };
VarInfo __func_info__f381b060f4dbce7_field_0 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Annotation"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8226, 88, UINT64_C(0x5ac39c9b48fed41d), "value", 0, 0 };
VarInfo * __func_info__f381b060f4dbce7_fields[1] =  { &__func_info__f381b060f4dbce7_field_0 };
FuncInfo __func_info__f381b060f4dbce7 = {"invoke block<(value:rtti::Annotation const):void> const", "", __func_info__f381b060f4dbce7_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xf381b060f4dbce7), 0x0 };
VarInfo __func_info__806bd08fbfbd09f4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0xec89c0a8da510f28), "func", 0, 0 };
VarInfo * __func_info__806bd08fbfbd09f4_fields[1] =  { &__func_info__806bd08fbfbd09f4_field_0 };
FuncInfo __func_info__806bd08fbfbd09f4 = {"invoke block<(var func:smart_ptr<ast::Function>):void> const", "", __func_info__806bd08fbfbd09f4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x806bd08fbfbd09f4), 0x0 };
VarInfo __func_info__8075d08fbfce07f4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0xec83c0a8da46dd28), "func", 0, 0 };
VarInfo * __func_info__8075d08fbfce07f4_fields[1] =  { &__func_info__8075d08fbfce07f4_field_0 };
FuncInfo __func_info__8075d08fbfce07f4 = {"invoke block<(var func:smart_ptr<ast::Function>):void> const", "", __func_info__8075d08fbfce07f4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8075d08fbfce07f4), 0x0 };
VarInfo __func_info__aa03b7bcb8f14157_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16452, 8, UINT64_C(0xf05dbeaf5dbf985d), "name", 0, 0 };
VarInfo __func_info__aa03b7bcb8f14157_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ce241e3005cc873b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0x62310535c768e51e), "value", 0, 0 };
VarInfo * __func_info__aa03b7bcb8f14157_fields[2] =  { &__func_info__aa03b7bcb8f14157_field_0, &__func_info__aa03b7bcb8f14157_field_1 };
FuncInfo __func_info__aa03b7bcb8f14157 = {"invoke block<(var name:string#;var value:smart_ptr<ast::TypeDecl>):void> const", "", __func_info__aa03b7bcb8f14157_fields, 2, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xaa03b7bcb8f14157), 0x0 };
VarInfo __func_info__762f57a17c2dce28_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x2a896b18ac8c5dc), "name", 0, 0 };
VarInfo __func_info__762f57a17c2dce28_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x1b2f537dda20b669), "cppName", 0, 0 };
VarInfo __func_info__762f57a17c2dce28_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ce241e3005cc873b, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0x5671bd5411275d07), "xtype", 0, 0 };
VarInfo __func_info__762f57a17c2dce28_field_3 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xc3630b856e2c8315), "offset", 0, 0 };
VarInfo * __func_info__762f57a17c2dce28_fields[4] =  { &__func_info__762f57a17c2dce28_field_0, &__func_info__762f57a17c2dce28_field_1, &__func_info__762f57a17c2dce28_field_2, &__func_info__762f57a17c2dce28_field_3 };
FuncInfo __func_info__762f57a17c2dce28 = {"invoke block<(var name:string;var cppName:string;var xtype:smart_ptr<ast::TypeDecl>;var offset:uint):void> const", "", __func_info__762f57a17c2dce28_fields, 4, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x762f57a17c2dce28), 0x0 };
VarInfo __func_info__9beb038de389c5b0_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__cd505ad3b1c59cc6, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0xf56dcd204992ec23), "value", 0, 0 };
VarInfo * __func_info__9beb038de389c5b0_fields[1] =  { &__func_info__9beb038de389c5b0_field_0 };
FuncInfo __func_info__9beb038de389c5b0 = {"invoke block<(var value:smart_ptr<ast::Enumeration>):void> const", "", __func_info__9beb038de389c5b0_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x9beb038de389c5b0), 0x0 };
VarInfo __func_info__438466bc7bed9910_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__60d16a2d23420951, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0x932b48247e2ac692), "value", 0, 0 };
VarInfo * __func_info__438466bc7bed9910_fields[1] =  { &__func_info__438466bc7bed9910_field_0 };
FuncInfo __func_info__438466bc7bed9910 = {"invoke block<(var value:smart_ptr<ast::Structure>):void> const", "", __func_info__438466bc7bed9910_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x438466bc7bed9910), 0x0 };
VarInfo __func_info__5f3d7fba1c40074b_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__d0872eb2c1d28358, nullptr, nullptr, nullptr, 0, 0, nullptr, 11264, 8, UINT64_C(0xc8b6e136feca1e7d), "value", 0, 0 };
VarInfo * __func_info__5f3d7fba1c40074b_fields[1] =  { &__func_info__5f3d7fba1c40074b_field_0 };
FuncInfo __func_info__5f3d7fba1c40074b = {"invoke block<(var value:smart_ptr<ast::TypeInfoMacro>):void> const", "", __func_info__5f3d7fba1c40074b_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x5f3d7fba1c40074b), 0x0 };
VarInfo __func_info__2701c05c12e53ee0_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ccd32e474e9a33eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0xbdf35432817faabe), "value", 0, 0 };
VarInfo * __func_info__2701c05c12e53ee0_fields[1] =  { &__func_info__2701c05c12e53ee0_field_0 };
FuncInfo __func_info__2701c05c12e53ee0 = {"invoke block<(var value:smart_ptr<ast::Variable>):void> const", "", __func_info__2701c05c12e53ee0_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x2701c05c12e53ee0), 0x0 };
VarInfo __func_info__84319ec9af82a642_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__98ed2eed8b7795f1, nullptr, nullptr, nullptr, 0, 0, nullptr, 11264, 8, UINT64_C(0x83c4685915b4f96), "value", 0, 0 };
VarInfo * __func_info__84319ec9af82a642_fields[1] =  { &__func_info__84319ec9af82a642_field_0 };
FuncInfo __func_info__84319ec9af82a642 = {"invoke block<(var value:smart_ptr<ast::VariantMacro>):void> const", "", __func_info__84319ec9af82a642_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x84319ec9af82a642), 0x0 };
VarInfo __func_info__e0afcd004fbe6de3_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16452, 8, UINT64_C(0x85f0f7cef5dd86b7), "value", 0, 0 };
VarInfo * __func_info__e0afcd004fbe6de3_fields[1] =  { &__func_info__e0afcd004fbe6de3_field_0 };
FuncInfo __func_info__e0afcd004fbe6de3 = {"invoke block<(var value:string#):void> const", "", __func_info__e0afcd004fbe6de3_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xe0afcd004fbe6de3), 0x0 };
TypeInfo __type_info__774985c53b7f8953 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 24845, 8, UINT64_C(0x774985c53b7f8953) };
TypeInfo __type_info__6b1c7db4b71a781f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 258, 32, UINT64_C(0x6b1c7db4b71a781f) };
TypeInfo __type_info__f7046ae574272550 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xf7046ae574272550) };
TypeInfo __type_info__f08d475fe5d680fc = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8e26ff93c3acecc5, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xf08d475fe5d680fc) };
TypeInfo __type_info__e705b0d7eea4be2c = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__f0a0a5963e48bb5f, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xe705b0d7eea4be2c) };
TypeInfo __type_info__7521b1289fcc2edb = { Type::tStructure, &__struct_info__d775c4f039534efe, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 40, UINT64_C(0x7521b1289fcc2edb) };
TypeInfo __type_info__3c61146b2bdfb90 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 290, 32, UINT64_C(0x3c61146b2bdfb90) };
TypeInfo __type_info__16d0aa3dd6b69257 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~fio::FILE"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 34, 16, UINT64_C(0x16d0aa3dd6b69257) };
TypeInfo __type_info__fa593d0882a72913 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16548, 8, UINT64_C(0xfa593d0882a72913) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__cd505ad3b1c59cc6 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Enumeration"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 176, UINT64_C(0xcd505ad3b1c59cc6) };
TypeInfo __type_info__a57bf935c2dd03 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Function"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 408, UINT64_C(0xa57bf935c2dd03) };
TypeInfo __type_info__60d16a2d23420951 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Structure"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 184, UINT64_C(0x60d16a2d23420951) };
TypeInfo __type_info__ce241e3005cc873b = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::TypeDecl"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 232, UINT64_C(0xce241e3005cc873b) };
TypeInfo __type_info__d0872eb2c1d28358 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::TypeInfoMacro"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8194, 56, UINT64_C(0xd0872eb2c1d28358) };
TypeInfo __type_info__ccd32e474e9a33eb = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Variable"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 256, UINT64_C(0xccd32e474e9a33eb) };
TypeInfo __type_info__98ed2eed8b7795f1 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::VariantMacro"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 48, UINT64_C(0x98ed2eed8b7795f1) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__8e26ff93c3acecc5 = { Type::tStructure, &__struct_info__837d1a0a555de4a2, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0x8e26ff93c3acecc5) };
TypeInfo __type_info__888650585a1a67a = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x888650585a1a67a) };
TypeInfo __type_info__f0a0a5963e48bb5f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xf0a0a5963e48bb5f) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__6b1c7db4b71a781f, __type_info__3c61146b2bdfb90, __type_info__16d0aa3dd6b69257, __type_info__cd505ad3b1c59cc6, __type_info__a57bf935c2dd03, __type_info__60d16a2d23420951, __type_info__ce241e3005cc873b, __type_info__d0872eb2c1d28358, __type_info__ccd32e474e9a33eb, __type_info__98ed2eed8b7795f1, __type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[2] = { &__type_info__37d36026a6078a42, &__type_info__fa593d0882a72913 };
TypeInfo * __tinfo_1[2] = { &__type_info__37d36026a6078a42, &__type_info__888650585a1a67a };
TypeInfo * __tinfo_2[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_3[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_4[2] = { &__type_info__37d36026a6078a42, &__type_info__fa593d0882a72913 };
TypeInfo * __tinfo_5[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_6[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_7[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_8[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f };
TypeInfo * __tinfo_9[4] = { &__type_info__6b1c7db4b71a781f, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f };
TypeInfo * __tinfo_11[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_12[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_13[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_14[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_15[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_16[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_17[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_18[4] = { &__type_info__6b1c7db4b71a781f, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_19[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_20[1] = { &__type_info__7521b1289fcc2edb };
TypeInfo * __tinfo_21[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_22[1] = { &__type_info__e705b0d7eea4be2c };
TypeInfo * __tinfo_23[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__fa593d0882a72913 };
TypeInfo * __tinfo_24[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_25[5] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_26[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_27[4] = { &__type_info__6b1c7db4b71a781f, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_28[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_29[2] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_30[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_31[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_32[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_33[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_34[2] = { &__type_info__37d36026a6078a42, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_35[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_36[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_37[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_38[3] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_39[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_40[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_41[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_42[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_43[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_44[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_45[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_46[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_47[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_48[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_49[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_50[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_51[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_52[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_53[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_54[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_55[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_56[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_57[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_58[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_59[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_60[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_61[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_62[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_63[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_64[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_65[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_66[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_67[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_68[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_69[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_70[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_71[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_72[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_73[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_74[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_75[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_76[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_77[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_78[2] = { &__type_info__37d36026a6078a42, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_79[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_80[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_81[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_82[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_83[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_84[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_85[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_86[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_87[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_88[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_89[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_90[2] = { &__type_info__37d36026a6078a42, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_91[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_92[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_93[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_94[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_95[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_96[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_97[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_98[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_99[2] = { &__type_info__37d36026a6078a42, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_100[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_101[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_102[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_103[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_104[2] = { &__type_info__37d36026a6078a42, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_105[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_106[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_107[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_108[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_109[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_110[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_111[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_112[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_113[6] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_114[9] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_115[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_116[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_117[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_118[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_119[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_120[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_121[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_122[7] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__fa593d0882a72913, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_123[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_124[4] = { &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_125[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_126[2] = { &__type_info__fa593d0882a72913, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_127[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_128[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_129[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_130[2] = { &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f };
TypeInfo * __tinfo_131[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_132[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_133[2] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_134[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_135[2] = { &__type_info__fa593d0882a72913, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_136[2] = { &__type_info__fa593d0882a72913, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_137[2] = { &__type_info__fa593d0882a72913, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_138[2] = { &__type_info__fa593d0882a72913, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_139[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_140[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_141[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_142[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_143[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_144[5] = { &__type_info__af90fe4c864e9d52, &__type_info__fa593d0882a72913, &__type_info__af90fe4c864e9d52, &__type_info__fa593d0882a72913, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_145[3] = { &__type_info__6b1c7db4b71a781f, &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f };
TypeInfo * __tinfo_146[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_147[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_148[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_149[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_150[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_151[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_152[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> const  & __a_rename_at_581_0 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_75e0f8673d04729f ( Context * __context__, TDim<AutoTuple<char *,char *>,13> const  & __a_rename_at_581_1 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_68_2, int32_t __newSize_rename_at_68_3 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_626ed670fc52be6d ( Context * __context__, TDim<AutoTuple<char *,char *>,3> const  & __a_rename_at_581_4 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> & __Arr_rename_at_68_5, int32_t __newSize_rename_at_68_6 );
inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf ( Context * __context__, TDim<AutoTuple<char *,char *>,1> & __a_rename_at_1342_7 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044333107478717573_19fa17f56443c23c ( Context * __context__, range __rng_rename_at_1244_9 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d ( Context * __context__, TDim<char *,3> const  & __a_rename_at_581_11 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b ( Context * __context__, TDim<char *,2> const  & __a_rename_at_581_12 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> & __a_rename_at_1182_13 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66 ( Context * __context__, TArray<TypeDecl *> & __Arr_rename_at_181_14, TypeDecl * __value_rename_at_181_15 );
inline char * _Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9 ( Context * __context__, Sequence DAS_COMMENT((char * &)) & __it_rename_at_46_16, char * const  __separator_rename_at_46_17 );
inline char * _FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7 ( Context * __context__, Annotation const  & __value_rename_at_61_22 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_60d4e412e550e258 ( Context * __context__, TArray<TArray<char *>> const  & __a_rename_at_585_23 );
inline char * _FuncrstTickfunction_label_nameTick4951524166161443384_aafe521f2cb4dc50 ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_77_24 );
inline void finalize_5bf95341c40e5b82 ( Context * __context__, rst::_lambda_rst_1216_1 & ____this_rename_at_1216_26 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1 ( Context * __context__, TArray<char *> & __Arr_rename_at_181_27, char * __value_rename_at_181_28 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835 ( Context * __context__, TArray<char *> const  & __it_rename_at_22_29, char * const  __separator_rename_at_22_30 );
inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f ( Context * __context__, TDim<AutoTuple<char *,char *>,13> & __a_rename_at_1342_35 );
inline void _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50 ( Context * __context__, TArray<Function *> & __Arr_rename_at_181_37, Function * __value_rename_at_181_38 );
inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickeach_refTick16137060296763333387_f3ba366f8c86460e ( Context * __context__, Lambda DAS_COMMENT((bool,char * * &)) const  __lam_rename_at_1299_39 );
inline char * _FuncastTickget_mangled_nameTick8436048561986127392_32c9175ac8d63b0f ( Context * __context__, Function * const  __fn_rename_at_63_41 );
inline TypeDecl * _FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e ( Context * __context__, smart_ptr_raw<TypeDecl> __src_rename_at_1732_42 );
inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_f84a97f3b3efcea2 ( Context * __context__, Function * const  __value_rename_at_69_43 );
inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_2c64031744b1c1f2 ( Context * __context__, TDim<AutoTuple<char *,char *>,3> & __a_rename_at_1342_45 );
inline void _FuncrstTickdocument_typedefTick2903024037696058038_48a0306c1d5a26e9 ( Context * __context__, FILE const  * const  __doc_file_rename_at_419_47, Module * const  __mod_rename_at_419_48, char * const  __name_rename_at_419_49, smart_ptr_raw<TypeDecl> const  __value_rename_at_419_50 );
inline void _FuncrstTickdocument_enumerationTick4211850574665690795_5251d76ee781cebe ( Context * __context__, FILE const  * const  __doc_file_rename_at_496_52, Module * const  __mod_rename_at_496_53, smart_ptr_raw<Enumeration> const  __value_rename_at_496_54 );
inline void _FuncrstTickdocument_class_methodTick12589992380795293570_b41bc23f1e57e06 ( Context * __context__, FILE const  * const  __doc_file_rename_at_573_58, Module * const  __mod_rename_at_573_59, smart_ptr_raw<Structure> const  __value_rename_at_573_60, Structure::FieldDeclaration & __fld_rename_at_573_61 );
inline void _FuncrstTickdocument_structure_annotationTick2734840712723762798_45f2db6d6c9372ba ( Context * __context__, FILE const  * const  __doc_file_rename_at_673_68, Module * const  __mod_rename_at_673_69, Annotation const  & __value_rename_at_673_70 );
inline void _FuncrstTickdocument_function_annotationTick9878220666429632644_62156d6269c1aee6 ( Context * __context__, FILE const  * const  __doc_file_rename_at_762_95, Module * const  __mod_rename_at_762_96, Annotation const  & __value_rename_at_762_97 );
inline void _FuncrstTickdocument_annotationTick7679268031359399902_8b47c8f555ccae04 ( Context * __context__, FILE const  * const  __doc_file_rename_at_789_98, Module * const  __mod_rename_at_789_99, Annotation const  & __value_rename_at_789_100 );
inline void _FuncrstTickdocument_structure_macroTick6088634580083749384_a8cf27101dd30a50 ( Context * __context__, FILE const  * const  __doc_file_rename_at_819_101, Module * const  __mod_rename_at_819_102, Annotation const  & __value_rename_at_819_103 );
inline Function * _FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615 ( Context * __context__, smart_ptr_raw<Function> __src_rename_at_1732_104 );
inline void _FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1008_105, Module * const  __mod_rename_at_1008_106, smart_ptr_raw<Function> __func_rename_at_1008_107 );
inline rst::DocGroup & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_81325f48a6efc9e9 ( Context * __context__, rst::DocGroup & __a_rename_at_50_114 );
inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_69_115 );
inline bool _Func_lambda_rst_1216_1Tickfunction_deb6e09d1773cc6a ( Context * __context__, rst::_lambda_rst_1216_1 & ____this_rename_at_1216_117, char * * & ___ryield1216_rename_at_1216_118 );
inline void _Func_lambda_rst_1216_1Tickfinalizer_d8783cc009d376af ( Context * __context__, rst::_lambda_rst_1216_1 * ____this_rename_at_1216_119 );
inline char * _FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1 ( Context * __context__, Module * const  __mod_rename_at_28_120 );
inline Variable * _FuncbuiltinTickget_ptrTick8468476673553620226_f1503a45e3ae58f3 ( Context * __context__, smart_ptr_raw<Variable> const  __src_rename_at_1726_122 );
inline Enumeration * _FuncbuiltinTickget_ptrTick8468476673553620226_d6dabe47fb1a720 ( Context * __context__, smart_ptr_raw<Enumeration> const  __src_rename_at_1726_123 );
inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1726_124 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_125, bool __extra_rename_at_38_126, bool __contracts_rename_at_38_127, bool __modules_rename_at_38_128 );
inline char * _FuncrstTickhandle_label_nameTick13616998130381234091_7f23dc2205503d09 ( Context * __context__, TypeAnnotation * const  __value_rename_at_61_129 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_68_130, int32_t __newSize_rename_at_68_131 );
inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_132, char * const  __mode_rename_at_12_133, Block DAS_COMMENT((void,FILE const  * const )) const  & __blk_rename_at_12_134 );
inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_649bcd7e51242beb ( Context * __context__, TDim<char *,3> & __a_rename_at_1342_136 );
inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> & __Arr_rename_at_165_138, char * const  __value_rename_at_165_139 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6 ( Context * __context__, TArray<TArray<char *>> & __Arr_rename_at_287_140, TArray<char *> & __value_rename_at_287_141 );
inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_142 );
inline bool _FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6 ( Context * __context__, TypeDecl * const  __tt_rename_at_531_143 );
inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_325c63fe2a174cf6 ( Context * __context__, TDim<char *,2> & __a_rename_at_1342_144 );
inline char * _FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c ( Context * __context__, Module * const  __mod_rename_at_65_146, char * const  __value_rename_at_65_147 );
inline void _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a ( Context * __context__, TTable<Function *,bool> & __Tab_rename_at_947_148, Function * const  __at_rename_at_947_149, bool __val_rename_at_947_150 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_4e6333822225d431 ( Context * __context__, TTable<Function *,bool> const  & __Tab_rename_at_1014_151, Function * const  __at_rename_at_1014_152 );
inline char * _Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f ( Context * __context__, Function * const  __func_rename_at_89_153 );
inline char * _FuncrstTickfunction_label_nameTick4951524166161443384_4422c54db32493f8 ( Context * __context__, Function * const  __value_rename_at_77_158 );
inline bool _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72 ( Context * __context__, TTable<Function *,bool> & __Tab_rename_at_741_160, Function * const  __at_rename_at_741_161 );
inline void _FuncrstTickdocument_functionTick8829733267459955112_767667cb9ac814a7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1008_162, Module * const  __mod_rename_at_1008_163, Function * __func_rename_at_1008_164 );
inline bool _FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45 ( Context * __context__, TTable<char *,bool> & __Tab_rename_at_741_171, char * const  __at_rename_at_741_172 );
inline void _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b ( Context * __context__, TTable<char *,bool> & __Tab_rename_at_947_173, char * const  __at_rename_at_947_174, bool __val_rename_at_947_175 );
inline char * alias_label_name_f0e3a6da4e226a3e ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_33_176 );
inline char * global_label_name_2b7e96918b37c00e ( Context * __context__, Variable * const  __value_rename_at_37_177 );
inline char * global_label_name_b721f72143d35230 ( Context * __context__, smart_ptr_raw<Variable> const  __value_rename_at_41_178 );
inline char * enum_label_name_a32575da767b3ecd ( Context * __context__, Enumeration * const  __value_rename_at_45_179 );
inline char * enum_label_name_768485a24af7de17 ( Context * __context__, smart_ptr_raw<Enumeration> const  __value_rename_at_49_180 );
inline char * struct_label_name_7b7faf8095586da6 ( Context * __context__, Structure * const  __value_rename_at_53_181 );
inline char * struct_label_name_6ee1d24e90557aeb ( Context * __context__, smart_ptr_raw<Structure> const  __value_rename_at_57_182 );
inline char * describe_type_short_56370d8993e381ba ( Context * __context__, TypeDecl * const  __td_rename_at_96_183 );
inline char * describe_type_short_847eafd8ddf6216 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_102_184 );
inline char * describe_type_cf1a091488868462 ( Context * __context__, TypeDecl * const  __td_rename_at_110_186 );
inline char * describe_type_a9bac10014c763b2 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_116_187 );
inline char * make_domain_3e6edb06cf9af7bc ( Context * __context__, char * const  __attr_rename_at_294_209 );
inline char * make_label_3368ba1f933c2731 ( Context * __context__, char * const  __name_rename_at_298_210 );
inline char * make_ref_c995159f43ed1b43 ( Context * __context__, char * const  __name_rename_at_302_211, char * const  __text_rename_at_302_212 );
inline char * make_group_8b304b06bc5b7645 ( Context * __context__, char * const  __name_rename_at_311_214, char * const  __plus_rename_at_311_215 );
inline char * make_header_efad6667e51b3e08 ( Context * __context__, char * const  __name_rename_at_316_217, char * const  __lab_rename_at_316_218 );
inline void write_table_separator_2180d51ced094114 ( Context * __context__, StringBuilderWriter & __writer_rename_at_321_220, TArray<int32_t> const  & __wid_rename_at_321_221, char * const  __sep_rename_at_321_222 );
inline void write_table_line_18bbb8245228f037 ( Context * __context__, StringBuilderWriter & __writer_rename_at_328_224, TArray<char *> const  & __line_rename_at_328_225, TArray<int32_t> const  & __wid_rename_at_328_226 );
inline char * make_table_46d5ce8cf5a30e69 ( Context * __context__, TArray<TArray<char *>> const  & __tab_rename_at_335_231, bool __withHeader_rename_at_335_232 );
inline char * topic_9659659d94ddb29a ( Context * __context__, char * const  __category_rename_at_358_243, Module * const  __mod_rename_at_358_244, char * const  __name_rename_at_358_245 );
inline char * topic_b97c7facf4ccb6a9 ( Context * __context__, Module * const  __mod_rename_at_362_246 );
inline void document_topic_c6e2614dff9f2d3e ( Context * __context__, FILE const  * const  __doc_file_rename_at_366_247, char * const  __topic_rename_at_366_248 );
inline void document_bitfield_4340095c1075f95e ( Context * __context__, FILE const  * const  __doc_file_rename_at_384_253, smart_ptr_raw<TypeDecl> const  __value_rename_at_384_254 );
inline void document_variant_c74548ebfca402e7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_397_261, smart_ptr_raw<TypeDecl> const  __value_rename_at_397_262 );
inline void document_tuple_c74548ebfca402e7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_408_269, smart_ptr_raw<TypeDecl> const  __value_rename_at_408_270 );
inline void document_typedefs_6ca650b4c8738629 ( Context * __context__, FILE const  * const  __doc_file_rename_at_440_277, Module * const  __mod_rename_at_440_278 );
inline void document_global_constant_378f3e1a10c345f3 ( Context * __context__, FILE const  * const  __doc_file_rename_at_454_284, Module * const  __mod_rename_at_454_285, smart_ptr_raw<Variable> const  __value_rename_at_454_286 );
inline bool is_global_constant_320cc2da3d8764f7 ( Context * __context__, smart_ptr_raw<Variable> const  __value_rename_at_462_287 );
inline void document_global_constants_24362fe0342586e0 ( Context * __context__, FILE const  * const  __doc_file_rename_at_478_288, Module * const  __mod_rename_at_478_289 );
inline void document_enumerations_c255f6c4885d0cb5 ( Context * __context__, FILE const  * const  __doc_file_rename_at_511_293, Module * const  __mod_rename_at_511_294 );
inline void document_function_declaration_4c978976b1aaa270 ( Context * __context__, FILE const  * const  __doc_file_rename_at_541_298, char * const  __domain_rename_at_541_299, char * const  __func_name_rename_at_541_300, TArray<char *> const  & __argNames_rename_at_542_301, TArray<TypeDecl *> const  & __argTypes_rename_at_542_302, smart_ptr_raw<TypeDecl> const  __resType_rename_at_542_303 );
inline void document_function_arguments_e7469e75af4ed246 ( Context * __context__, FILE const  * const  __doc_file_rename_at_557_313, TArray<char *> const  & __argNames_rename_at_557_314, TArray<TypeDecl *> const  & __argTypes_rename_at_557_315 );
inline void document_classes_1fc51dae5246eec0 ( Context * __context__, FILE const  * const  __doc_file_rename_at_587_322, Module * const  __mod_rename_at_587_323 );
inline void document_structure_96bd65f4eda31cf3 ( Context * __context__, FILE const  * const  __doc_file_rename_at_633_331, Module * const  __mod_rename_at_633_332, smart_ptr_raw<Structure> const  __value_rename_at_633_333 );
inline void document_structures_5fb20d0dee04ba46 ( Context * __context__, FILE const  * const  __doc_file_rename_at_653_337, Module * const  __mod_rename_at_653_338 );
inline void document_structure_annotations_a53005d9c9a5617 ( Context * __context__, FILE const  * const  __doc_file_rename_at_742_342, Module * const  __mod_rename_at_742_343 );
inline void document_function_annotations_5344d92e9ea569ad ( Context * __context__, FILE const  * const  __doc_file_rename_at_769_347, Module * const  __mod_rename_at_769_348 );
inline void document_annotations_128851a794d46d1a ( Context * __context__, FILE const  * const  __doc_file_rename_at_796_352, Module * const  __mod_rename_at_796_353 );
inline void document_structure_macros_7386ca2820215b3d ( Context * __context__, FILE const  * const  __doc_file_rename_at_826_357, Module * const  __mod_rename_at_826_358 );
inline void document_call_macro_e76fbe68d00e7362 ( Context * __context__, FILE const  * const  __doc_file_rename_at_846_362, Module * const  __mod_rename_at_846_363, char * const  __value_rename_at_846_364 );
inline void document_call_macros_56da413fe6ccb104 ( Context * __context__, FILE const  * const  __doc_file_rename_at_853_365, Module * const  __mod_rename_at_853_366 );
inline void document_reader_macro_4006c26786c1908 ( Context * __context__, FILE const  * const  __doc_file_rename_at_867_370, Module * const  __mod_rename_at_867_371, char * const  __value_rename_at_867_372 );
inline void document_reader_macros_585b57fffa625cd ( Context * __context__, FILE const  * const  __doc_file_rename_at_874_373, Module * const  __mod_rename_at_874_374 );
inline void document_variant_macro_84da1b9ee053cb60 ( Context * __context__, FILE const  * const  __doc_file_rename_at_888_378, Module * const  __mod_rename_at_888_379, char * const  __value_rename_at_888_380 );
inline void document_variant_macros_eab4d095021756ef ( Context * __context__, FILE const  * const  __doc_file_rename_at_895_381, Module * const  __mod_rename_at_895_382 );
inline void document_typeinfo_macro_66a7bf37ca0e4fcd ( Context * __context__, FILE const  * const  __doc_file_rename_at_909_386, Module * const  __mod_rename_at_909_387, char * const  __value_rename_at_909_388 );
inline void document_typeinfo_macros_b4b5371c027f780c ( Context * __context__, FILE const  * const  __doc_file_rename_at_916_389, Module * const  __mod_rename_at_916_390 );
inline bool function_needs_documenting_5ed98b6306abb3d7 ( Context * __context__, smart_ptr_raw<Function> const  __func_rename_at_930_394 );
inline void document_warning_cb981aa521e01a92 ( Context * __context__, FILE const  * const  __doc_file_rename_at_993_405, char * const  __issue_rename_at_993_406 );
inline char * function_name_ee62641d633d27c8 ( Context * __context__, char * const  __name_rename_at_998_407 );
inline void document_functions_c930228364733372 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1027_409, Module * const  __mod_rename_at_1027_410, TArray<rst::DocGroup> & __groups_rename_at_1027_411 );
inline rst::DocGroup debug_group_70ce9e37d0ceaf7a ( Context * __context__, rst::DocGroup & __group_rename_at_1098_424 );
inline void append_to_group_by_regex_fa03c8e61a02da42 ( Context * __context__, rst::DocGroup & __group_rename_at_1106_426, Module * const  __mod_rename_at_1106_427, regex::Regex & __reg_rename_at_1106_428 );
inline rst::DocGroup group_by_regex_3d42ba74b8c888e3 ( Context * __context__, char * const  __name_rename_at_1123_431, Module * const  __mod_rename_at_1123_432, regex::Regex & __reg_rename_at_1123_433 );
inline rst::DocGroup hide_group_72124d6e40656992 ( Context * __context__, rst::DocGroup & __group_rename_at_1131_435 );
inline void generate_topic_stub_2777760f2b77d9f0 ( Context * __context__, TTable<char *,bool> & __tab_rename_at_1137_436, FILE const  * const  __doc_file_rename_at_1137_437, char * const  __topic_rename_at_1137_438 );
inline void generate_substitute_stub_34b873e38788090 ( Context * __context__, Module * const  __mod_rename_at_1144_439, char * const  __substname_rename_at_1144_440 );
inline char * getDetailIncludePath_fb77bda786a398b2 ( Context * __context__, char * const  __fname_rename_at_1205_453, char * const  __substname_rename_at_1205_454 );
inline void document_38c26f0bb9ed7a3e ( Context * __context__, char * const  __name_rename_at_1221_463, Module * const  __mod_rename_at_1221_464, char * const  __fname_rename_at_1221_465, char * const  __substname_rename_at_1221_466, TArray<rst::DocGroup> & __groups_rename_at_1221_467 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<bool,0x7292a0a703a50ed7>(__context__) = true;/*log_documentation*/
    das_global<bool,0xaa5d977586266135>(__context__) = false;/*show_hidden_groups*/
    das_global<char *,0xde937609e38e5c08>(__context__) = nullptr;/*topic_root*/
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> const  &  __a_rename_at_581_0 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_75e0f8673d04729f ( Context * __context__, TDim<AutoTuple<char *,char *>,13> const  &  __a_rename_at_581_1 )
{
    return das_auto_cast<int32_t>::cast(13);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_68_2, int32_t __newSize_rename_at_68_3 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_68_2),__newSize_rename_at_68_3,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_626ed670fc52be6d ( Context * __context__, TDim<AutoTuple<char *,char *>,3> const  &  __a_rename_at_581_4 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_68_5, int32_t __newSize_rename_at_68_6 )
{
    builtin_array_resize(das_arg<TArray<char *>>::pass(__Arr_rename_at_68_5),__newSize_rename_at_68_6,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf ( Context * __context__, TDim<AutoTuple<char *,char *>,1> &  __a_rename_at_1342_7 )
{
    TArray<AutoTuple<char *,char *>> __arr_rename_at_1344_8; das_zero(__arr_rename_at_1344_8);
    _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__arr_rename_at_1344_8),1);
    das_copy(das_cast<TDim<AutoTuple<char *,char *>,1>>::cast(das_ref(__context__,__arr_rename_at_1344_8(0,__context__))),__a_rename_at_1342_7);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<char *,char *>>>::cast(__arr_rename_at_1344_8);
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044333107478717573_19fa17f56443c23c ( Context * __context__, range __rng_rename_at_1244_9 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1245_10; das_zero(__it_rename_at_1245_10);
    builtin_make_range_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1245_10),__rng_rename_at_1244_9,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1245_10);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d ( Context * __context__, TDim<char *,3> const  &  __a_rename_at_581_11 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b ( Context * __context__, TDim<char *,2> const  &  __a_rename_at_581_12 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> &  __a_rename_at_1182_13 )
{
    builtin_array_free(das_arg<TArray<char *>>::pass(__a_rename_at_1182_13),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66 ( Context * __context__, TArray<TypeDecl *> &  __Arr_rename_at_181_14, TypeDecl * __value_rename_at_181_15 )
{
    das_copy(__Arr_rename_at_181_14(builtin_array_push_back(das_arg<TArray<TypeDecl *>>::pass(__Arr_rename_at_181_14),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_15);
}

inline char * _Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9 ( Context * __context__, Sequence DAS_COMMENT((char * &)) &  __it_rename_at_46_16, char * const  __separator_rename_at_46_17 )
{
    char * __st_rename_at_47_18 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_47_19) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_48_20 = true;
        {
            bool __need_loop_49 = true;
            // elem: string aka TT&
            das_iterator<Sequence DAS_COMMENT((char *))> __elem_iterator(__it_rename_at_46_16);
            char * * __elem_rename_at_49_21;
            __need_loop_49 = __elem_iterator.first(__context__,(__elem_rename_at_49_21)) && __need_loop_49;
            for ( ; __need_loop_49 ; __need_loop_49 = __elem_iterator.next(__context__,(__elem_rename_at_49_21)) )
            {
                if ( __skip_first_rename_at_48_20 )
                {
                    das_copy(__skip_first_rename_at_48_20,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_0,cast<StringBuilderWriter &>::from(__writer_rename_at_47_19),cast<char * const >::from(__separator_rename_at_46_17)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_47_19),cast<char *>::from((*__elem_rename_at_49_21))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_49_21));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_47_18);
}

inline char * _FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7 ( Context * __context__, Annotation const  &  __value_rename_at_61_22 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_2, cast<char * const >::from(((char *) "handle-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__value_rename_at_61_22.module /*_module*/)), cast<char * const >::from(((char *) "-")), cast<das::string const  &>::from(__value_rename_at_61_22.name /*name*/))));
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_60d4e412e550e258 ( Context * __context__, TArray<TArray<char *>> const  &  __a_rename_at_585_23 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_585_23) == 0);
}

inline char * _FuncrstTickfunction_label_nameTick4951524166161443384_aafe521f2cb4dc50 ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_77_24 )
{
    TArray<AutoTuple<char *,char *>> _temp_make_local_79_32_48; _temp_make_local_79_32_48;
    TDim<AutoTuple<char *,char *>,13> _temp_make_local_79_32_80; _temp_make_local_79_32_80;
    char * __mn_rename_at_78_25 = (char *)(((char * const )(get_mangled_name(__value_rename_at_77_24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(__mn_rename_at_78_25,das_invoke_function<char * const >::invoke<char * const ,TArray<AutoTuple<char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 0x510d9544ac4d5ab4)),__mn_rename_at_78_25,das_arg<TArray<AutoTuple<char *,char *>>>::pass((_temp_make_local_79_32_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f(__context__,das_arg<TDim<AutoTuple<char *,char *>,13>>::pass((([&]() -> TDim<AutoTuple<char *,char *>,13>& {
        _temp_make_local_79_32_80(0,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_80;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_80) = ((char *) " ");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_80) = ((char *) "_");
            return __mkt_80;
        })());
        _temp_make_local_79_32_80(1,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_81;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_81) = ((char *) "$");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_81) = ((char *) "_builtin_");
            return __mkt_81;
        })());
        _temp_make_local_79_32_80(2,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_82;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_82) = ((char *) "#");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_82) = ((char *) "_hh_");
            return __mkt_82;
        })());
        _temp_make_local_79_32_80(3,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_83;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_83) = ((char *) ":");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_83) = ((char *) "_c_");
            return __mkt_83;
        })());
        _temp_make_local_79_32_80(4,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_84;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_84) = ((char *) "@");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_84) = ((char *) "_at_");
            return __mkt_84;
        })());
        _temp_make_local_79_32_80(5,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_85;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_85) = ((char *) "<");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_85) = ((char *) "_ls_");
            return __mkt_85;
        })());
        _temp_make_local_79_32_80(6,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_86;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_86) = ((char *) ">");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_86) = ((char *) "_gr_");
            return __mkt_86;
        })());
        _temp_make_local_79_32_80(7,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_87;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_87) = ((char *) "`");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_87) = ((char *) "_rq_");
            return __mkt_87;
        })());
        _temp_make_local_79_32_80(8,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_88;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_88) = ((char *) "!");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_88) = ((char *) "_ex_");
            return __mkt_88;
        })());
        _temp_make_local_79_32_80(9,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_89;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_89) = ((char *) "=");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_89) = ((char *) "_eq_");
            return __mkt_89;
        })());
        _temp_make_local_79_32_80(10,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_90;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_90) = ((char *) "?");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_90) = ((char *) "_qm_");
            return __mkt_90;
        })());
        _temp_make_local_79_32_80(11,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_91;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_91) = ((char *) "[");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_91) = ((char *) "_lb_");
            return __mkt_91;
        })());
        _temp_make_local_79_32_80(12,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_92;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_92) = ((char *) "]");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_92) = ((char *) "_rb_");
            return __mkt_92;
        })());
        return _temp_make_local_79_32_80;
    })()))))))));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_3, cast<char * const >::from(((char *) "function-")), cast<char *>::from(__mn_rename_at_78_25))));
}

inline void finalize_5bf95341c40e5b82 ( Context * __context__, rst::_lambda_rst_1216_1 &  ____this_rename_at_1216_26 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(____this_rename_at_1216_26.snp));
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_1216_26._source_0_at_1216_35),__context__);
    memset(&(____this_rename_at_1216_26), 0, TypeSize<rst::_lambda_rst_1216_1>::size);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_181_27, char * __value_rename_at_181_28 )
{
    das_copy(__Arr_rename_at_181_27(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_181_27),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_28);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835 ( Context * __context__, TArray<char *> const  &  __it_rename_at_22_29, char * const  __separator_rename_at_22_30 )
{
    char * __st_rename_at_27_31 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_27_32) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_28_33 = true;
        {
            bool __need_loop_29 = true;
            // elem: string const&
            das_iterator<TArray<char *> const > __elem_iterator(__it_rename_at_22_29);
            char * const  * __elem_rename_at_29_34;
            __need_loop_29 = __elem_iterator.first(__context__,(__elem_rename_at_29_34)) && __need_loop_29;
            for ( ; __need_loop_29 ; __need_loop_29 = __elem_iterator.next(__context__,(__elem_rename_at_29_34)) )
            {
                if ( __skip_first_rename_at_28_33 )
                {
                    das_copy(__skip_first_rename_at_28_33,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_4,cast<StringBuilderWriter &>::from(__writer_rename_at_27_32),cast<char * const >::from(__separator_rename_at_22_30)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_5,cast<StringBuilderWriter &>::from(__writer_rename_at_27_32),cast<char * const >::from((*__elem_rename_at_29_34))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_29_34));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_27_31);
}

inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f ( Context * __context__, TDim<AutoTuple<char *,char *>,13> &  __a_rename_at_1342_35 )
{
    TArray<AutoTuple<char *,char *>> __arr_rename_at_1344_36; das_zero(__arr_rename_at_1344_36);
    _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__arr_rename_at_1344_36),13);
    das_copy(das_cast<TDim<AutoTuple<char *,char *>,13>>::cast(das_ref(__context__,__arr_rename_at_1344_36(0,__context__))),__a_rename_at_1342_35);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<char *,char *>>>::cast(__arr_rename_at_1344_36);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50 ( Context * __context__, TArray<Function *> &  __Arr_rename_at_181_37, Function * __value_rename_at_181_38 )
{
    das_copy(__Arr_rename_at_181_37(builtin_array_push_back(das_arg<TArray<Function *>>::pass(__Arr_rename_at_181_37),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_38);
}

inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickeach_refTick16137060296763333387_f3ba366f8c86460e ( Context * __context__, Lambda DAS_COMMENT((bool,char * * &)) const  __lam_rename_at_1299_39 )
{
    Sequence DAS_COMMENT((char * *)) __it_rename_at_1300_40; das_zero(__it_rename_at_1300_40);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1300_40),__lam_rename_at_1299_39,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char * &))>::cast(__it_rename_at_1300_40);
}

inline char * _FuncastTickget_mangled_nameTick8436048561986127392_32c9175ac8d63b0f ( Context * __context__, Function * const  __fn_rename_at_63_41 )
{
    return das_auto_cast<char *>::cast(((char * const )(get_mangled_name(das_cast<smart_ptr_raw<Function>>::cast(__fn_rename_at_63_41),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline TypeDecl * _FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e ( Context * __context__, smart_ptr_raw<TypeDecl> __src_rename_at_1732_42 )
{
    return das_auto_cast<TypeDecl *>::cast(das_cast<TypeDecl *>::cast(__src_rename_at_1732_42));
}

inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_f84a97f3b3efcea2 ( Context * __context__, Function * const  __value_rename_at_69_43 )
{
    TArray<AutoTuple<char *,char *>> _temp_make_local_72_32_48; _temp_make_local_72_32_48;
    TDim<AutoTuple<char *,char *>,1> _temp_make_local_72_32_80; _temp_make_local_72_32_80;
    char * __mn_rename_at_71_44 = (char *)(((char * const )(to_das_string(__value_rename_at_69_43->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(__mn_rename_at_71_44,das_invoke_function<char * const >::invoke<char * const ,TArray<AutoTuple<char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 0x510d9544ac4d5ab4)),__mn_rename_at_71_44,das_arg<TArray<AutoTuple<char *,char *>>>::pass((_temp_make_local_72_32_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf(__context__,das_arg<TDim<AutoTuple<char *,char *>,1>>::pass((([&]() -> TDim<AutoTuple<char *,char *>,1>& {
        _temp_make_local_72_32_80(0,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_73;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_73) = ((char *) "?");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_73) = ((char *) "_qm_");
            return __mkt_73;
        })());
        return _temp_make_local_72_32_80;
    })()))))))));
    return das_auto_cast<char *>::cast(__mn_rename_at_71_44);
}

inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_2c64031744b1c1f2 ( Context * __context__, TDim<AutoTuple<char *,char *>,3> &  __a_rename_at_1342_45 )
{
    TArray<AutoTuple<char *,char *>> __arr_rename_at_1344_46; das_zero(__arr_rename_at_1344_46);
    _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__arr_rename_at_1344_46),3);
    das_copy(das_cast<TDim<AutoTuple<char *,char *>,3>>::cast(das_ref(__context__,__arr_rename_at_1344_46(0,__context__))),__a_rename_at_1342_45);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<char *,char *>>>::cast(__arr_rename_at_1344_46);
}

inline void _FuncrstTickdocument_typedefTick2903024037696058038_48a0306c1d5a26e9 ( Context * __context__, FILE const  * const  __doc_file_rename_at_419_47, Module * const  __mod_rename_at_419_48, char * const  __name_rename_at_419_49, smart_ptr_raw<TypeDecl> const  __value_rename_at_419_50 )
{
    builtin_fwrite(__doc_file_rename_at_419_47,make_label_3368ba1f933c2731(__context__,alias_label_name_f0e3a6da4e226a3e(__context__,__value_rename_at_419_50)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_419_47,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_419_47,__name_rename_at_419_49,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __value_rename_at_419_50->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield )
    {
        builtin_fwrite(__doc_file_rename_at_419_47,((char *) " is a bitfield\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        document_bitfield_4340095c1075f95e(__context__,__doc_file_rename_at_419_47,__value_rename_at_419_50);
    } else if ( __value_rename_at_419_50->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tVariant )
    {
        builtin_fwrite(__doc_file_rename_at_419_47,((char *) " is a variant type\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        document_variant_c74548ebfca402e7(__context__,__doc_file_rename_at_419_47,__value_rename_at_419_50);
    } else if ( __value_rename_at_419_50->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTuple )
    {
        builtin_fwrite(__doc_file_rename_at_419_47,((char *) " is a tuple type\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        document_tuple_c74548ebfca402e7(__context__,__doc_file_rename_at_419_47,__value_rename_at_419_50);
    } else {
        char * __tp_rename_at_433_51 = (char *)(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__value_rename_at_419_50,false,false,false));
        das_copy(__tp_rename_at_433_51,((char * const )(builtin_string_replace(__tp_rename_at_433_51,((char *) "$::"),nullptr,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        builtin_fwrite(__doc_file_rename_at_419_47,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<char * const >::from(((char *) " = ")), cast<char *>::from(__tp_rename_at_433_51), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_419_47,topic_9659659d94ddb29a(__context__,((char *) "typedef"),__mod_rename_at_419_48,__name_rename_at_419_49));
}

inline void _FuncrstTickdocument_enumerationTick4211850574665690795_5251d76ee781cebe ( Context * __context__, FILE const  * const  __doc_file_rename_at_496_52, Module * const  __mod_rename_at_496_53, smart_ptr_raw<Enumeration> const  __value_rename_at_496_54 )
{
    builtin_fwrite(__doc_file_rename_at_496_52,make_label_3368ba1f933c2731(__context__,enum_label_name_768485a24af7de17(__context__,__value_rename_at_496_54)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_496_52,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_496_52,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_7, cast<das::string const  &>::from(__value_rename_at_496_54->name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    TArray<TArray<char *>> __tab_rename_at_500_55; das_zero(__tab_rename_at_500_55);
    {
        bool __need_loop_501 = true;
        // en: ast::EnumEntry const&
        das_iterator<das::vector<Enumeration::EnumEntry> const > __en_iterator(__value_rename_at_496_54->list /*list*/);
        Enumeration::EnumEntry const  * __en_rename_at_501_56;
        __need_loop_501 = __en_iterator.first(__context__,(__en_rename_at_501_56)) && __need_loop_501;
        for ( ; __need_loop_501 ; __need_loop_501 = __en_iterator.next(__context__,(__en_rename_at_501_56)) )
        {
            TArray<char *> __line_rename_at_502_57; das_zero(__line_rename_at_502_57);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_502_57),((char * const )(to_das_string((*__en_rename_at_501_56).name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_502_57),_FuncastTickdescribeTick842554968825501494_452cfc8955963a18(__context__,(*__en_rename_at_501_56).value /*value*/));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_500_55),das_arg<TArray<char *>>::pass(__line_rename_at_502_57));
        }
        __en_iterator.close(__context__,(__en_rename_at_501_56));
    };
    builtin_fwrite(__doc_file_rename_at_496_52,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_500_55),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_496_52,topic_9659659d94ddb29a(__context__,((char *) "enumeration"),__mod_rename_at_496_53,((char * const )(to_das_string(__value_rename_at_496_54->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void _FuncrstTickdocument_class_methodTick12589992380795293570_b41bc23f1e57e06 ( Context * __context__, FILE const  * const  __doc_file_rename_at_573_58, Module * const  __mod_rename_at_573_59, smart_ptr_raw<Structure> const  __value_rename_at_573_60, Structure::FieldDeclaration &  __fld_rename_at_573_61 )
{
    TArray<char *> __argNames_rename_at_574_62_ConstRef; das_zero(__argNames_rename_at_574_62_ConstRef); das_move(__argNames_rename_at_574_62_ConstRef, ((TArray<char *>)das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_574_21_rename_at_574_63; das_zero(____acomp_574_21_rename_at_574_63);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_574_21_rename_at_574_63),false);
        {
            bool __need_loop_574 = true;
            // arg: $::das_string&
            das_iterator<das::vector<das::string>> __arg_iterator(__fld_rename_at_573_61.type /*_type*/->argNames /*argNames*/);
            das::string * __arg_rename_at_574_64;
            __need_loop_574 = __arg_iterator.first(__context__,(__arg_rename_at_574_64)) && __need_loop_574;
            for ( ; __need_loop_574 ; __need_loop_574 = __arg_iterator.next(__context__,(__arg_rename_at_574_64)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(____acomp_574_21_rename_at_574_63),((char * const )(to_das_string(das_arg<das::string>::pass((*__arg_rename_at_574_64)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_574_64));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_574_21_rename_at_574_63),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_574_21_rename_at_574_63);
    })));
    TArray<char *> const  & __argNames_rename_at_574_62 = __argNames_rename_at_574_62_ConstRef; ;
    TArray<TypeDecl *> __argTypes_rename_at_575_65_ConstRef; das_zero(__argTypes_rename_at_575_65_ConstRef); das_move(__argTypes_rename_at_575_65_ConstRef, ((TArray<TypeDecl *>)das_invoke<TArray<TypeDecl *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<TypeDecl *>{
        TArray<TypeDecl *> ____acomp_575_21_rename_at_575_66; das_zero(____acomp_575_21_rename_at_575_66);
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_575_21_rename_at_575_66),false);
        {
            bool __need_loop_575 = true;
            // arg: smart_ptr<ast::TypeDecl>&
            das_iterator<das::vector<smart_ptr<TypeDecl>>> __arg_iterator(__fld_rename_at_573_61.type /*_type*/->argTypes /*argTypes*/);
            smart_ptr_raw<TypeDecl> * __arg_rename_at_575_67;
            __need_loop_575 = __arg_iterator.first(__context__,(__arg_rename_at_575_67)) && __need_loop_575;
            for ( ; __need_loop_575 ; __need_loop_575 = __arg_iterator.next(__context__,(__arg_rename_at_575_67)) )
            {
                _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66(__context__,das_arg<TArray<TypeDecl *>>::pass(____acomp_575_21_rename_at_575_66),_FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e(__context__,(*__arg_rename_at_575_67)));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_575_67));
        };
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_575_21_rename_at_575_66),true);
        return /* <- */ das_auto_cast_move<TArray<TypeDecl *>>::cast(____acomp_575_21_rename_at_575_66);
    })));
    TArray<TypeDecl *> const  & __argTypes_rename_at_575_65 = __argTypes_rename_at_575_65_ConstRef; ;
    document_function_declaration_4c978976b1aaa270(__context__,__doc_file_rename_at_573_58,((char *) "das:function"),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_8, cast<das::string const  &>::from(__value_rename_at_573_60->name /*name*/), cast<char * const >::from(((char *) ".")), cast<das::string &>::from(__fld_rename_at_573_61.name /*name*/))),__argNames_rename_at_574_62,__argTypes_rename_at_575_65,__fld_rename_at_573_61.type /*_type*/->firstType /*firstType*/);
    if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fld_rename_at_573_61.type /*_type*/->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr))) && !(((das_deref(__context__,__fld_rename_at_573_61.type /*_type*/->firstType /*firstType*/)).isVoid())) )
    {
        builtin_fwrite(__doc_file_rename_at_573_58,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_9, cast<das::string &>::from(__fld_rename_at_573_61.name /*name*/), cast<char * const >::from(((char *) " returns ")), cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__fld_rename_at_573_61.type /*_type*/->firstType /*firstType*/)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( builtin_array_size(__argTypes_rename_at_575_65) > 1 )
    {
        document_function_arguments_e7469e75af4ed246(__context__,__doc_file_rename_at_573_58,__argNames_rename_at_574_62,__argTypes_rename_at_575_65);
    };
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_573_58,topic_9659659d94ddb29a(__context__,((char *) "method"),__mod_rename_at_573_59,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<das::string const  &>::from(__value_rename_at_573_60->name /*name*/), cast<char * const >::from(((char *) ".")), cast<das::string &>::from(__fld_rename_at_573_61.name /*name*/)))));
}

inline void _FuncrstTickdocument_structure_annotationTick2734840712723762798_45f2db6d6c9372ba ( Context * __context__, FILE const  * const  __doc_file_rename_at_673_68, Module * const  __mod_rename_at_673_69, Annotation const  &  __value_rename_at_673_70 ) { das_stack_prologue __prologue(__context__,576,"rst`document_structure_annotation`2734840712723762798 " DAS_FILE_LINE);
{
    bool __any_fields_rename_at_677_71; memset(&__any_fields_rename_at_677_71,0,sizeof(__any_fields_rename_at_677_71));
    bool __any_props_rename_at_678_72; memset(&__any_props_rename_at_678_72,0,sizeof(__any_props_rename_at_678_72));
    BasicStructureAnnotation * __ann_rename_at_679_73; memset(&__ann_rename_at_679_73,0,sizeof(__ann_rename_at_679_73));
    TArray<TArray<char *>> __tab_rename_at_688_78; memset(&__tab_rename_at_688_78,0,sizeof(__tab_rename_at_688_78));
    TArray<TArray<char *>> __tab_rename_at_701_84; memset(&__tab_rename_at_701_84,0,sizeof(__tab_rename_at_701_84));
    TArray<TArray<char *>> __props_rename_at_713_90; memset(&__props_rename_at_713_90,0,sizeof(__props_rename_at_713_90));
    smart_ptr_raw<TypeDecl> __annT_rename_at_714_91; memset(&__annT_rename_at_714_91,0,sizeof(__annT_rename_at_714_91));
    /* finally */ auto __finally_673= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__annT_rename_at_714_91);
    /* end finally */ });
    builtin_fwrite(__doc_file_rename_at_673_68,make_label_3368ba1f933c2731(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7(__context__,__value_rename_at_673_70)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_673_68,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_673_68,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_11, cast<das::string const  &>::from(__value_rename_at_673_70.name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    __any_fields_rename_at_677_71 = false;
    __any_props_rename_at_678_72 = false;
    __ann_rename_at_679_73 = das_cast<BasicStructureAnnotation *>::cast(__value_rename_at_673_70);
    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_679_73)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,128,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_680_74, char * __cppName_rename_at_680_75, smart_ptr_raw<TypeDecl> __xtype_rename_at_680_76, uint32_t __offset_rename_at_680_77) -> void{
        if ( __offset_rename_at_680_77 != 0xffffffffu )
        {
            das_copy(__any_fields_rename_at_677_71,true);
        } else {
            das_copy(__any_props_rename_at_678_72,true);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __any_fields_rename_at_677_71 )
    {
        __tab_rename_at_688_78; das_zero(__tab_rename_at_688_78);
        builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_679_73)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,224,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_689_79, char * __cppName_rename_at_689_80, smart_ptr_raw<TypeDecl> __xtype_rename_at_689_81, uint32_t __offset_rename_at_689_82) -> void{
            if ( __offset_rename_at_689_82 != 0xffffffffu )
            {
                TArray<char *> __line_rename_at_691_83; das_zero(__line_rename_at_691_83);
                _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_691_83),__name_rename_at_689_79);
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_691_83),describe_type_a9bac10014c763b2(__context__,__xtype_rename_at_689_81));
                _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_688_78),das_arg<TArray<char *>>::pass(__line_rename_at_691_83));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_68,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_12, cast<das::string const  &>::from(__value_rename_at_673_70.name /*name*/), cast<char * const >::from(((char *) " fields are\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_68,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_688_78),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( __any_props_rename_at_678_72 )
    {
        __tab_rename_at_701_84; das_zero(__tab_rename_at_701_84);
        builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_679_73)),das_make_block<void,char *,char *,smart_ptr_raw<TypeDecl>,uint32_t>(__context__,352,0,&__func_info__762f57a17c2dce28,[&](char * __name_rename_at_702_85, char * __cppName_rename_at_702_86, smart_ptr_raw<TypeDecl> __xtype_rename_at_702_87, uint32_t __offset_rename_at_702_88) -> void{
            if ( __offset_rename_at_702_88 == 0xffffffffu )
            {
                TArray<char *> __line_rename_at_704_89; das_zero(__line_rename_at_704_89);
                _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_704_89),__name_rename_at_702_85);
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_704_89),describe_type_a9bac10014c763b2(__context__,__xtype_rename_at_702_87));
                _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_701_84),das_arg<TArray<char *>>::pass(__line_rename_at_704_89));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_68,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<das::string const  &>::from(__value_rename_at_673_70.name /*name*/), cast<char * const >::from(((char *) " properties are\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_68,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_701_84),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __props_rename_at_713_90; das_zero(__props_rename_at_713_90);
    __annT_rename_at_714_91; das_zero(__annT_rename_at_714_91); das_move(__annT_rename_at_714_91, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_714) {
        das_copy((__mks_714.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tHandle));
        das_copy((__mks_714.annotation /*annotation*/),(das_cast<TypeAnnotation *>::cast(__value_rename_at_673_70)));
    }))));
    forEachFunction(__mod_rename_at_673_69,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,512,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_716_92) -> void{
        if ( !das_get_bitfield(__func_rename_at_716_92->moreFlags /*moreFlags*/,1u << 12) )
        {
            return ;
        };
        if ( das_vector_length(das_arg<das::vector<smart_ptr<Variable>>>::pass(__func_rename_at_716_92->arguments /*arguments*/)) != 1 )
        {
            return ;
        };
        char * __fna_rename_at_723_93 = ((char *)(char *)(((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_716_92->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
        if ( !(builtin_string_startswith(__fna_rename_at_723_93,((char *) ".`"),__context__)) || (builtin_string_length(__fna_rename_at_723_93,__context__) <= 2) )
        {
            return ;
        };
        if ( !isSameAstType(das_index<das::vector<smart_ptr<Variable>>>::at(__func_rename_at_716_92->arguments /*arguments*/,0,__context__)->type /*_type*/,__annT_rename_at_714_91,DAS_COMMENT(bound_enum) das::RefMatters::no,DAS_COMMENT(bound_enum) das::ConstMatters::no,DAS_COMMENT(bound_enum) das::TemporaryMatters::no,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            return ;
        };
        TArray<char *> __line_rename_at_730_94; das_zero(__line_rename_at_730_94);
        _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_730_94),((char * const )(builtin_string_slice2(__fna_rename_at_723_93,2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_730_94),describe_type_a9bac10014c763b2(__context__,__func_rename_at_716_92->result /*result*/));
        _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__props_rename_at_713_90),das_arg<TArray<char *>>::pass(__line_rename_at_730_94));
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !_FuncbuiltinTickemptyTick15399874715904164783_60d4e412e550e258(__context__,das_arg<TArray<TArray<char *>>>::pass(__props_rename_at_713_90)) )
    {
        builtin_fwrite(__doc_file_rename_at_673_68,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_14, cast<das::string const  &>::from(__value_rename_at_673_70.name /*name*/), cast<char * const >::from(((char *) " property operators are\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_68,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__props_rename_at_713_90),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_673_68,topic_9659659d94ddb29a(__context__,((char *) "structure_annotation"),__mod_rename_at_673_69,((char * const )(to_das_string(__value_rename_at_673_70.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}}

inline void _FuncrstTickdocument_function_annotationTick9878220666429632644_62156d6269c1aee6 ( Context * __context__, FILE const  * const  __doc_file_rename_at_762_95, Module * const  __mod_rename_at_762_96, Annotation const  &  __value_rename_at_762_97 )
{
    builtin_fwrite(__doc_file_rename_at_762_95,make_label_3368ba1f933c2731(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7(__context__,__value_rename_at_762_97)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_762_95,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_762_95,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_15, cast<das::string const  &>::from(__value_rename_at_762_97.name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_762_95,topic_9659659d94ddb29a(__context__,((char *) "function_annotation"),__mod_rename_at_762_96,((char * const )(to_das_string(__value_rename_at_762_97.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void _FuncrstTickdocument_annotationTick7679268031359399902_8b47c8f555ccae04 ( Context * __context__, FILE const  * const  __doc_file_rename_at_789_98, Module * const  __mod_rename_at_789_99, Annotation const  &  __value_rename_at_789_100 )
{
    builtin_fwrite(__doc_file_rename_at_789_98,make_label_3368ba1f933c2731(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7(__context__,__value_rename_at_789_100)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_789_98,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_789_98,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_16, cast<das::string const  &>::from(__value_rename_at_789_100.name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_789_98,topic_9659659d94ddb29a(__context__,((char *) "any_annotation"),__mod_rename_at_789_99,((char * const )(to_das_string(__value_rename_at_789_100.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void _FuncrstTickdocument_structure_macroTick6088634580083749384_a8cf27101dd30a50 ( Context * __context__, FILE const  * const  __doc_file_rename_at_819_101, Module * const  __mod_rename_at_819_102, Annotation const  &  __value_rename_at_819_103 )
{
    builtin_fwrite(__doc_file_rename_at_819_101,make_label_3368ba1f933c2731(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7(__context__,__value_rename_at_819_103)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_819_101,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_819_101,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_17, cast<das::string const  &>::from(__value_rename_at_819_103.name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_819_101,topic_9659659d94ddb29a(__context__,((char *) "structure_macro"),__mod_rename_at_819_102,((char * const )(to_das_string(__value_rename_at_819_103.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline Function * _FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615 ( Context * __context__, smart_ptr_raw<Function> __src_rename_at_1732_104 )
{
    return das_auto_cast<Function *>::cast(das_cast<Function *>::cast(__src_rename_at_1732_104));
}

inline void _FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1008_105, Module * const  __mod_rename_at_1008_106, smart_ptr_raw<Function> __func_rename_at_1008_107 )
{
    TArray<char *> __argNames_rename_at_1009_108_ConstRef; das_zero(__argNames_rename_at_1009_108_ConstRef); das_move(__argNames_rename_at_1009_108_ConstRef, ((TArray<char *>)das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_1009_21_rename_at_1009_109; das_zero(____acomp_1009_21_rename_at_1009_109);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_109),false);
        {
            bool __need_loop_1009 = true;
            // arg: smart_ptr<ast::Variable>&
            das_iterator<das::vector<smart_ptr<Variable>>> __arg_iterator(__func_rename_at_1008_107->arguments /*arguments*/);
            smart_ptr_raw<Variable> * __arg_rename_at_1009_110;
            __need_loop_1009 = __arg_iterator.first(__context__,(__arg_rename_at_1009_110)) && __need_loop_1009;
            for ( ; __need_loop_1009 ; __need_loop_1009 = __arg_iterator.next(__context__,(__arg_rename_at_1009_110)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_109),((char * const )(to_das_string(das_arg<das::string>::pass((*__arg_rename_at_1009_110)->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_1009_110));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_109),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_1009_21_rename_at_1009_109);
    })));
    TArray<char *> const  & __argNames_rename_at_1009_108 = __argNames_rename_at_1009_108_ConstRef; ;
    TArray<TypeDecl *> __argTypes_rename_at_1010_111_ConstRef; das_zero(__argTypes_rename_at_1010_111_ConstRef); das_move(__argTypes_rename_at_1010_111_ConstRef, ((TArray<TypeDecl *>)das_invoke<TArray<TypeDecl *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<TypeDecl *>{
        TArray<TypeDecl *> ____acomp_1010_21_rename_at_1010_112; das_zero(____acomp_1010_21_rename_at_1010_112);
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_112),false);
        {
            bool __need_loop_1010 = true;
            // arg: smart_ptr<ast::Variable>&
            das_iterator<das::vector<smart_ptr<Variable>>> __arg_iterator(__func_rename_at_1008_107->arguments /*arguments*/);
            smart_ptr_raw<Variable> * __arg_rename_at_1010_113;
            __need_loop_1010 = __arg_iterator.first(__context__,(__arg_rename_at_1010_113)) && __need_loop_1010;
            for ( ; __need_loop_1010 ; __need_loop_1010 = __arg_iterator.next(__context__,(__arg_rename_at_1010_113)) )
            {
                _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66(__context__,das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_112),_FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e(__context__,(*__arg_rename_at_1010_113)->type /*_type*/));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_1010_113));
        };
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_112),true);
        return /* <- */ das_auto_cast_move<TArray<TypeDecl *>>::cast(____acomp_1010_21_rename_at_1010_112);
    })));
    TArray<TypeDecl *> const  & __argTypes_rename_at_1010_111 = __argTypes_rename_at_1010_111_ConstRef; ;
    builtin_fwrite(__doc_file_rename_at_1008_105,make_label_3368ba1f933c2731(__context__,_FuncrstTickfunction_label_nameTick4951524166161443384_aafe521f2cb4dc50(__context__,__func_rename_at_1008_107)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_function_declaration_4c978976b1aaa270(__context__,__doc_file_rename_at_1008_105,((char *) "das:function"),function_name_ee62641d633d27c8(__context__,((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_1008_107->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),__argNames_rename_at_1009_108,__argTypes_rename_at_1010_111,__func_rename_at_1008_107->result /*result*/);
    if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__func_rename_at_1008_107->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) && !(((das_deref(__context__,__func_rename_at_1008_107->result /*result*/)).isVoid())) )
    {
        builtin_fwrite(__doc_file_rename_at_1008_105,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_18, cast<das::string &>::from(__func_rename_at_1008_107->name /*name*/), cast<char * const >::from(((char *) " returns ")), cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__func_rename_at_1008_107->result /*result*/)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( das_get_bitfield(__func_rename_at_1008_107->flags /*flags*/,1u << 14) )
    {
        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1008_105,((char *) "This is unsafe operation."));
    };
    if ( das_get_bitfield(__func_rename_at_1008_107->moreFlags /*moreFlags*/,1u << 8) )
    {
        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1008_105,((char *) "This function is deprecated."));
    };
    document_function_arguments_e7469e75af4ed246(__context__,__doc_file_rename_at_1008_105,__argNames_rename_at_1009_108,__argTypes_rename_at_1010_111);
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_1008_105,topic_9659659d94ddb29a(__context__,((char *) "function"),__mod_rename_at_1008_106,das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_19, cast<char * const >::from(_FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b(__context__,__func_rename_at_1008_107))))));
}

inline rst::DocGroup & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_81325f48a6efc9e9 ( Context * __context__, rst::DocGroup &  __a_rename_at_50_114 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_20,cast<rst::DocGroup &>::from(__a_rename_at_50_114)));
    return das_auto_cast_ref<rst::DocGroup &>::cast(__a_rename_at_50_114);
}

inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_69_115 )
{
    TArray<AutoTuple<char *,char *>> _temp_make_local_72_32_48; _temp_make_local_72_32_48;
    TDim<AutoTuple<char *,char *>,1> _temp_make_local_72_32_80; _temp_make_local_72_32_80;
    char * __mn_rename_at_71_116 = (char *)(((char * const )(to_das_string(__value_rename_at_69_115->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(__mn_rename_at_71_116,das_invoke_function<char * const >::invoke<char * const ,TArray<AutoTuple<char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 0x510d9544ac4d5ab4)),__mn_rename_at_71_116,das_arg<TArray<AutoTuple<char *,char *>>>::pass((_temp_make_local_72_32_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf(__context__,das_arg<TDim<AutoTuple<char *,char *>,1>>::pass((([&]() -> TDim<AutoTuple<char *,char *>,1>& {
        _temp_make_local_72_32_80(0,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_73;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_73) = ((char *) "?");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_73) = ((char *) "_qm_");
            return __mkt_73;
        })());
        return _temp_make_local_72_32_80;
    })()))))))));
    return das_auto_cast<char *>::cast(__mn_rename_at_71_116);
}

inline bool _Func_lambda_rst_1216_1Tickfunction_deb6e09d1773cc6a ( Context * __context__, rst::_lambda_rst_1216_1 &  ____this_rename_at_1216_117, char * * & ___ryield1216_rename_at_1216_118 )
{
    switch (____this_rename_at_1216_117.__yield) {
    case 0: goto label_0;
    case 2: goto label_2;
    case 1: goto label_1;
    case 3: goto label_3;
    case 4: goto label_4;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    das_copy(____this_rename_at_1216_117._loop_at_1216_35,true);
    das_move(____this_rename_at_1216_117._source_0_at_1216_35,_FuncbuiltinTickeachTick4044333107478717573_19fa17f56443c23c(__context__,range(____this_rename_at_1216_117.index,builtin_array_size(das_arg<TArray<char *>>::pass(____this_rename_at_1216_117.snp)))));
    memset(&(____this_rename_at_1216_117.__i_rename_at_1216_39), 0, TypeSize<int32_t>::size);
    das_copy(____this_rename_at_1216_117._pvar_0_at_1216_35,das_cast<void *>::cast(das_ref(__context__,____this_rename_at_1216_117.__i_rename_at_1216_39)));
    DAS_SETBOOLAND((____this_rename_at_1216_117._loop_at_1216_35),(builtin_iterator_first(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_1216_117._source_0_at_1216_35),____this_rename_at_1216_117._pvar_0_at_1216_35,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    label_2:;;
    if ( !____this_rename_at_1216_117._loop_at_1216_35 )
    {
        goto label_4;
    };
    das_copy(___ryield1216_rename_at_1216_118,das_ref(__context__,____this_rename_at_1216_117.snp(____this_rename_at_1216_117.__i_rename_at_1216_39,__context__)));
    das_copy(____this_rename_at_1216_117.__yield,1);
    return das_auto_cast<bool>::cast(true);
    label_1:;;
    label_3:;;
    DAS_SETBOOLAND((____this_rename_at_1216_117._loop_at_1216_35),(builtin_iterator_next(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_1216_117._source_0_at_1216_35),____this_rename_at_1216_117._pvar_0_at_1216_35,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    goto label_2;
    label_4:;;
    builtin_iterator_close(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_1216_117._source_0_at_1216_35),____this_rename_at_1216_117._pvar_0_at_1216_35,__context__);
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_rst_1216_1Tickfinalizer_d8783cc009d376af ( Context * __context__, rst::_lambda_rst_1216_1 * ____this_rename_at_1216_119 )
{
    finalize_5bf95341c40e5b82(__context__,das_arg<rst::_lambda_rst_1216_1>::pass(das_deref(__context__,____this_rename_at_1216_119)));
    das_delete_lambda_struct<rst::_lambda_rst_1216_1 *>::clear(__context__,____this_rename_at_1216_119);
}

inline char * _FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1 ( Context * __context__, Module * const  __mod_rename_at_28_120 )
{
    char * __mn_rename_at_29_121 = ((char *)(char *)(((char * const )(to_das_string(__mod_rename_at_28_120->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast((SimPolicy<char *>::Equ(cast<char *>::from(__mn_rename_at_29_121),cast<char *>::from(((char *) "$")),*__context__,nullptr)) ? das_auto_cast<char * const >::cast(((char *) "builtin")) : das_auto_cast<char * const >::cast(__mn_rename_at_29_121));
}

inline Variable * _FuncbuiltinTickget_ptrTick8468476673553620226_f1503a45e3ae58f3 ( Context * __context__, smart_ptr_raw<Variable> const  __src_rename_at_1726_122 )
{
    return das_auto_cast<Variable *>::cast(das_cast<Variable *>::cast(__src_rename_at_1726_122));
}

inline Enumeration * _FuncbuiltinTickget_ptrTick8468476673553620226_d6dabe47fb1a720 ( Context * __context__, smart_ptr_raw<Enumeration> const  __src_rename_at_1726_123 )
{
    return das_auto_cast<Enumeration *>::cast(das_cast<Enumeration *>::cast(__src_rename_at_1726_123));
}

inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1726_124 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1726_124));
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_125, bool __extra_rename_at_38_126, bool __contracts_rename_at_38_127, bool __modules_rename_at_38_128 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_125,__extra_rename_at_38_126,__contracts_rename_at_38_127,__modules_rename_at_38_128,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * _FuncrstTickhandle_label_nameTick13616998130381234091_7f23dc2205503d09 ( Context * __context__, TypeAnnotation * const  __value_rename_at_61_129 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_21, cast<char * const >::from(((char *) "handle-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__value_rename_at_61_129->module /*_module*/)), cast<char * const >::from(((char *) "-")), cast<das::string const  &>::from(__value_rename_at_61_129->name /*name*/))));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_68_130, int32_t __newSize_rename_at_68_131 )
{
    builtin_array_resize(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_68_130),__newSize_rename_at_68_131,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_132, char * const  __mode_rename_at_12_133, Block DAS_COMMENT((void,FILE const  * const )) const  &  __blk_rename_at_12_134 )
{
    FILE const  * __f_rename_at_13_135 = ((FILE const  *)builtin_fopen(__name_rename_at_12_132,__mode_rename_at_12_133));
    das_invoke<void>::invoke<FILE const  * const >(__context__,nullptr,__blk_rename_at_12_134,__f_rename_at_13_135);
    if ( __f_rename_at_13_135 != nullptr )
    {
        builtin_fclose(__f_rename_at_13_135,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_649bcd7e51242beb ( Context * __context__, TDim<char *,3> &  __a_rename_at_1342_136 )
{
    TArray<char *> __arr_rename_at_1344_137; das_zero(__arr_rename_at_1344_137);
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__arr_rename_at_1344_137),3);
    das_copy(das_cast<TDim<char *,3>>::cast(das_ref(__context__,__arr_rename_at_1344_137(0,__context__))),__a_rename_at_1342_136);
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__arr_rename_at_1344_137);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_138, char * const  __value_rename_at_165_139 )
{
    das_copy(__Arr_rename_at_165_138(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_138),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_139);
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6 ( Context * __context__, TArray<TArray<char *>> &  __Arr_rename_at_287_140, TArray<char *> &  __value_rename_at_287_141 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_22,cast<TArray<TArray<char *>> &>::from(__Arr_rename_at_287_140)));
    das_move(__Arr_rename_at_287_140(builtin_array_push_back(das_arg<TArray<TArray<char *>>>::pass(__Arr_rename_at_287_140),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_141);
}

inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_142 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_142,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline bool _FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6 ( Context * __context__, TypeDecl * const  __tt_rename_at_531_143 )
{
    return das_auto_cast<bool>::cast((__tt_rename_at_531_143->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::fakeContext) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((__tt_rename_at_531_143->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::fakeLineInfo) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))));
}

inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_325c63fe2a174cf6 ( Context * __context__, TDim<char *,2> &  __a_rename_at_1342_144 )
{
    TArray<char *> __arr_rename_at_1344_145; das_zero(__arr_rename_at_1344_145);
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__arr_rename_at_1344_145),2);
    das_copy(das_cast<TDim<char *,2>>::cast(das_ref(__context__,__arr_rename_at_1344_145(0,__context__))),__a_rename_at_1342_144);
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__arr_rename_at_1344_145);
}

inline char * _FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c ( Context * __context__, Module * const  __mod_rename_at_65_146, char * const  __value_rename_at_65_147 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_23, cast<char * const >::from(((char *) "call-macro-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__mod_rename_at_65_146)), cast<char * const >::from(((char *) "-")), cast<char * const >::from(__value_rename_at_65_147))));
}

inline void _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a ( Context * __context__, TTable<Function *,bool> &  __Tab_rename_at_947_148, Function * const  __at_rename_at_947_149, bool __val_rename_at_947_150 )
{
    das_copy(__Tab_rename_at_947_148(__at_rename_at_947_149,__context__),__val_rename_at_947_150);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_4e6333822225d431 ( Context * __context__, TTable<Function *,bool> const  &  __Tab_rename_at_1014_151, Function * const  __at_rename_at_1014_152 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_151,__at_rename_at_1014_152));
}

inline char * _Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f ( Context * __context__, Function * const  __func_rename_at_89_153 )
{
    TArray<char *> __args_rename_at_90_154; das_zero(__args_rename_at_90_154); das_move(__args_rename_at_90_154, das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_90_17_rename_at_90_155; das_zero(____acomp_90_17_rename_at_90_155);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_90_17_rename_at_90_155),false);
        {
            bool __need_loop_90 = true;
            // x: smart_ptr<ast::Variable> const&
            das_iterator<das::vector<smart_ptr<Variable>> const > __x_iterator(__func_rename_at_89_153->arguments /*arguments*/);
            smart_ptr_raw<Variable> const  * __x_rename_at_90_156;
            __need_loop_90 = __x_iterator.first(__context__,(__x_rename_at_90_156)) && __need_loop_90;
            for ( ; __need_loop_90 ; __need_loop_90 = __x_iterator.next(__context__,(__x_rename_at_90_156)) )
            {
                _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1(__context__,das_arg<TArray<char *>>::pass(____acomp_90_17_rename_at_90_155),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_24, cast<das::string const  &>::from((*__x_rename_at_90_156)->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,(*__x_rename_at_90_156)->type /*_type*/,false,true,true)))));
            }
            __x_iterator.close(__context__,(__x_rename_at_90_156));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_90_17_rename_at_90_155),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_90_17_rename_at_90_155);
    }));
    char * __res_rename_at_91_157 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_25, cast<das::string const  &>::from(__func_rename_at_89_153->name /*name*/), cast<char * const >::from(((char *) " (")), cast<char * const >::from(_Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835(__context__,das_arg<TArray<char *>>::pass(__args_rename_at_90_154),((char *) ";"))), cast<char * const >::from(((char *) ") : ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__func_rename_at_89_153->result /*result*/,false,true,true))))));
    _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(__args_rename_at_90_154));
    return das_auto_cast<char *>::cast(__res_rename_at_91_157);
}

inline char * _FuncrstTickfunction_label_nameTick4951524166161443384_4422c54db32493f8 ( Context * __context__, Function * const  __value_rename_at_77_158 )
{
    TArray<AutoTuple<char *,char *>> _temp_make_local_79_32_48; _temp_make_local_79_32_48;
    TDim<AutoTuple<char *,char *>,13> _temp_make_local_79_32_80; _temp_make_local_79_32_80;
    char * __mn_rename_at_78_159 = (char *)(_FuncastTickget_mangled_nameTick8436048561986127392_32c9175ac8d63b0f(__context__,__value_rename_at_77_158));
    das_copy(__mn_rename_at_78_159,das_invoke_function<char * const >::invoke<char * const ,TArray<AutoTuple<char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 0x510d9544ac4d5ab4)),__mn_rename_at_78_159,das_arg<TArray<AutoTuple<char *,char *>>>::pass((_temp_make_local_79_32_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f(__context__,das_arg<TDim<AutoTuple<char *,char *>,13>>::pass((([&]() -> TDim<AutoTuple<char *,char *>,13>& {
        _temp_make_local_79_32_80(0,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_80;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_80) = ((char *) " ");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_80) = ((char *) "_");
            return __mkt_80;
        })());
        _temp_make_local_79_32_80(1,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_81;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_81) = ((char *) "$");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_81) = ((char *) "_builtin_");
            return __mkt_81;
        })());
        _temp_make_local_79_32_80(2,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_82;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_82) = ((char *) "#");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_82) = ((char *) "_hh_");
            return __mkt_82;
        })());
        _temp_make_local_79_32_80(3,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_83;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_83) = ((char *) ":");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_83) = ((char *) "_c_");
            return __mkt_83;
        })());
        _temp_make_local_79_32_80(4,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_84;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_84) = ((char *) "@");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_84) = ((char *) "_at_");
            return __mkt_84;
        })());
        _temp_make_local_79_32_80(5,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_85;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_85) = ((char *) "<");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_85) = ((char *) "_ls_");
            return __mkt_85;
        })());
        _temp_make_local_79_32_80(6,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_86;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_86) = ((char *) ">");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_86) = ((char *) "_gr_");
            return __mkt_86;
        })());
        _temp_make_local_79_32_80(7,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_87;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_87) = ((char *) "`");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_87) = ((char *) "_rq_");
            return __mkt_87;
        })());
        _temp_make_local_79_32_80(8,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_88;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_88) = ((char *) "!");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_88) = ((char *) "_ex_");
            return __mkt_88;
        })());
        _temp_make_local_79_32_80(9,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_89;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_89) = ((char *) "=");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_89) = ((char *) "_eq_");
            return __mkt_89;
        })());
        _temp_make_local_79_32_80(10,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_90;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_90) = ((char *) "?");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_90) = ((char *) "_qm_");
            return __mkt_90;
        })());
        _temp_make_local_79_32_80(11,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_91;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_91) = ((char *) "[");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_91) = ((char *) "_lb_");
            return __mkt_91;
        })());
        _temp_make_local_79_32_80(12,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_92;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_92) = ((char *) "]");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_92) = ((char *) "_rb_");
            return __mkt_92;
        })());
        return _temp_make_local_79_32_80;
    })()))))))));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_26, cast<char * const >::from(((char *) "function-")), cast<char *>::from(__mn_rename_at_78_159))));
}

inline bool _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72 ( Context * __context__, TTable<Function *,bool> &  __Tab_rename_at_741_160, Function * const  __at_rename_at_741_161 )
{
    return das_auto_cast<bool>::cast(__Tab_rename_at_741_160(__at_rename_at_741_161,__context__));
}

inline void _FuncrstTickdocument_functionTick8829733267459955112_767667cb9ac814a7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1008_162, Module * const  __mod_rename_at_1008_163, Function * __func_rename_at_1008_164 )
{
    TArray<char *> __argNames_rename_at_1009_165_ConstRef; das_zero(__argNames_rename_at_1009_165_ConstRef); das_move(__argNames_rename_at_1009_165_ConstRef, ((TArray<char *>)das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_1009_21_rename_at_1009_166; das_zero(____acomp_1009_21_rename_at_1009_166);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_166),false);
        {
            bool __need_loop_1009 = true;
            // arg: smart_ptr<ast::Variable>&
            das_iterator<das::vector<smart_ptr<Variable>>> __arg_iterator(__func_rename_at_1008_164->arguments /*arguments*/);
            smart_ptr_raw<Variable> * __arg_rename_at_1009_167;
            __need_loop_1009 = __arg_iterator.first(__context__,(__arg_rename_at_1009_167)) && __need_loop_1009;
            for ( ; __need_loop_1009 ; __need_loop_1009 = __arg_iterator.next(__context__,(__arg_rename_at_1009_167)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_166),((char * const )(to_das_string(das_arg<das::string>::pass((*__arg_rename_at_1009_167)->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_1009_167));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_166),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_1009_21_rename_at_1009_166);
    })));
    TArray<char *> const  & __argNames_rename_at_1009_165 = __argNames_rename_at_1009_165_ConstRef; ;
    TArray<TypeDecl *> __argTypes_rename_at_1010_168_ConstRef; das_zero(__argTypes_rename_at_1010_168_ConstRef); das_move(__argTypes_rename_at_1010_168_ConstRef, ((TArray<TypeDecl *>)das_invoke<TArray<TypeDecl *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<TypeDecl *>{
        TArray<TypeDecl *> ____acomp_1010_21_rename_at_1010_169; das_zero(____acomp_1010_21_rename_at_1010_169);
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_169),false);
        {
            bool __need_loop_1010 = true;
            // arg: smart_ptr<ast::Variable>&
            das_iterator<das::vector<smart_ptr<Variable>>> __arg_iterator(__func_rename_at_1008_164->arguments /*arguments*/);
            smart_ptr_raw<Variable> * __arg_rename_at_1010_170;
            __need_loop_1010 = __arg_iterator.first(__context__,(__arg_rename_at_1010_170)) && __need_loop_1010;
            for ( ; __need_loop_1010 ; __need_loop_1010 = __arg_iterator.next(__context__,(__arg_rename_at_1010_170)) )
            {
                _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66(__context__,das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_169),_FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e(__context__,(*__arg_rename_at_1010_170)->type /*_type*/));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_1010_170));
        };
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_169),true);
        return /* <- */ das_auto_cast_move<TArray<TypeDecl *>>::cast(____acomp_1010_21_rename_at_1010_169);
    })));
    TArray<TypeDecl *> const  & __argTypes_rename_at_1010_168 = __argTypes_rename_at_1010_168_ConstRef; ;
    builtin_fwrite(__doc_file_rename_at_1008_162,make_label_3368ba1f933c2731(__context__,_FuncrstTickfunction_label_nameTick4951524166161443384_4422c54db32493f8(__context__,__func_rename_at_1008_164)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_function_declaration_4c978976b1aaa270(__context__,__doc_file_rename_at_1008_162,((char *) "das:function"),function_name_ee62641d633d27c8(__context__,((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_1008_164->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),__argNames_rename_at_1009_165,__argTypes_rename_at_1010_168,__func_rename_at_1008_164->result /*result*/);
    if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__func_rename_at_1008_164->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) && !(((das_deref(__context__,__func_rename_at_1008_164->result /*result*/)).isVoid())) )
    {
        builtin_fwrite(__doc_file_rename_at_1008_162,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_27, cast<das::string &>::from(__func_rename_at_1008_164->name /*name*/), cast<char * const >::from(((char *) " returns ")), cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__func_rename_at_1008_164->result /*result*/)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( das_get_bitfield(__func_rename_at_1008_164->flags /*flags*/,1u << 14) )
    {
        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1008_162,((char *) "This is unsafe operation."));
    };
    if ( das_get_bitfield(__func_rename_at_1008_164->moreFlags /*moreFlags*/,1u << 8) )
    {
        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1008_162,((char *) "This function is deprecated."));
    };
    document_function_arguments_e7469e75af4ed246(__context__,__doc_file_rename_at_1008_162,__argNames_rename_at_1009_165,__argTypes_rename_at_1010_168);
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_1008_162,topic_9659659d94ddb29a(__context__,((char *) "function"),__mod_rename_at_1008_163,das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_28, cast<char * const >::from(_FuncrstTickfunction_file_nameTick11623859247758201540_f84a97f3b3efcea2(__context__,__func_rename_at_1008_164))))));
}

inline bool _FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45 ( Context * __context__, TTable<char *,bool> &  __Tab_rename_at_741_171, char * const  __at_rename_at_741_172 )
{
    return das_auto_cast<bool>::cast(__Tab_rename_at_741_171(__at_rename_at_741_172,__context__));
}

inline void _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b ( Context * __context__, TTable<char *,bool> &  __Tab_rename_at_947_173, char * const  __at_rename_at_947_174, bool __val_rename_at_947_175 )
{
    das_copy(__Tab_rename_at_947_173(__at_rename_at_947_174,__context__),__val_rename_at_947_175);
}

inline char * alias_label_name_f0e3a6da4e226a3e ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_33_176 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_29, cast<char * const >::from(((char *) "alias-")), cast<das::string const  &>::from(__td_rename_at_33_176->alias /*alias*/))));
}

inline char * global_label_name_2b7e96918b37c00e ( Context * __context__, Variable * const  __value_rename_at_37_177 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_30, cast<char * const >::from(((char *) "global-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__value_rename_at_37_177->module /*_module*/)), cast<char * const >::from(((char *) "-")), cast<das::string const  &>::from(__value_rename_at_37_177->name /*name*/))));
}

inline char * global_label_name_b721f72143d35230 ( Context * __context__, smart_ptr_raw<Variable> const  __value_rename_at_41_178 )
{
    return das_auto_cast<char *>::cast(global_label_name_2b7e96918b37c00e(__context__,_FuncbuiltinTickget_ptrTick8468476673553620226_f1503a45e3ae58f3(__context__,__value_rename_at_41_178)));
}

inline char * enum_label_name_a32575da767b3ecd ( Context * __context__, Enumeration * const  __value_rename_at_45_179 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_31, cast<char * const >::from(((char *) "enum-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__value_rename_at_45_179->module /*_module*/)), cast<char * const >::from(((char *) "-")), cast<das::string const  &>::from(__value_rename_at_45_179->name /*name*/))));
}

inline char * enum_label_name_768485a24af7de17 ( Context * __context__, smart_ptr_raw<Enumeration> const  __value_rename_at_49_180 )
{
    return das_auto_cast<char *>::cast(enum_label_name_a32575da767b3ecd(__context__,_FuncbuiltinTickget_ptrTick8468476673553620226_d6dabe47fb1a720(__context__,__value_rename_at_49_180)));
}

inline char * struct_label_name_7b7faf8095586da6 ( Context * __context__, Structure * const  __value_rename_at_53_181 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_32, cast<char * const >::from(((char *) "struct-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__value_rename_at_53_181->module /*_module*/)), cast<char * const >::from(((char *) "-")), cast<das::string const  &>::from(__value_rename_at_53_181->name /*name*/))));
}

inline char * struct_label_name_6ee1d24e90557aeb ( Context * __context__, smart_ptr_raw<Structure> const  __value_rename_at_57_182 )
{
    return das_auto_cast<char *>::cast(struct_label_name_7b7faf8095586da6(__context__,_FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850(__context__,__value_rename_at_57_182)));
}

inline char * describe_type_short_56370d8993e381ba ( Context * __context__, TypeDecl * const  __td_rename_at_96_183 )
{
    return das_auto_cast<char *>::cast(describe_type_short_847eafd8ddf6216(__context__,das_cast<smart_ptr_raw<TypeDecl>>::cast(__td_rename_at_96_183)));
}

inline char * describe_type_short_847eafd8ddf6216 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_102_184 )
{
    if ( (!(builtin_empty_das_string(__td_rename_at_102_184->alias /*alias*/)) && !(((das_deref(__context__,__td_rename_at_102_184)).isAuto()))) && !(((das_deref(__context__,__td_rename_at_102_184)).isAlias())) )
    {
        return das_auto_cast<char *>::cast(((char * const )(to_das_string(__td_rename_at_102_184->alias /*alias*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    } else {
        char * __tp_rename_at_106_185 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__td_rename_at_102_184,false,false,false)));
        return das_auto_cast<char *>::cast(((char * const )(builtin_string_replace(__tp_rename_at_106_185,((char *) "$::"),nullptr,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    };
}

inline char * describe_type_cf1a091488868462 ( Context * __context__, TypeDecl * const  __td_rename_at_110_186 )
{
    return das_auto_cast<char *>::cast(describe_type_a9bac10014c763b2(__context__,das_cast<smart_ptr_raw<TypeDecl>>::cast(__td_rename_at_110_186)));
}

inline char * describe_type_a9bac10014c763b2 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_116_187 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_117_188) DAS_AOT_INLINE_LAMBDA -> void{
        if ( (!(builtin_empty_das_string(__td_rename_at_116_187->alias /*alias*/)) && !(((das_deref(__context__,__td_rename_at_116_187)).isAuto()))) && !(((das_deref(__context__,__td_rename_at_116_187)).isAlias())) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_33,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,alias_label_name_f0e3a6da4e226a3e(__context__,__td_rename_at_116_187),((char * const )(to_das_string(__td_rename_at_116_187->alias /*alias*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))));
            return ;
        };
        DAS_COMMENT(bound_enum) das::Type __baseType_rename_at_122_189 = ((DAS_COMMENT(bound_enum) das::Type)__td_rename_at_116_187->baseType /*baseType*/);
        if ( __baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::alias )
        {
            if ( !builtin_empty_das_string(__td_rename_at_116_187->alias /*alias*/) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_34,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<das::string const  &>::from(__td_rename_at_116_187->alias /*alias*/)));
            } else {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_35,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "alias"))));
            };
        } else if ( __baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::autoinfer )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_36,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "auto"))));
            if ( !builtin_empty_das_string(__td_rename_at_116_187->alias /*alias*/) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_37,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_38, cast<char * const >::from(((char *) "(")), cast<das::string const  &>::from(__td_rename_at_116_187->alias /*alias*/), cast<char * const >::from(((char *) ")")))))));
            };
        } else if ( __baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tHandle )
        {
            char * __hname_rename_at_135_190 = ((char *)(char *)(((char * const )(to_das_string(__td_rename_at_116_187->annotation /*annotation*/->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
            if ( builtin_string_startswith(__hname_rename_at_135_190,((char *) "dasvector`smart_ptr`"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_39,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "vector<smart_ptr<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_40,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char * const )(builtin_string_slice2(__hname_rename_at_135_190,20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_41,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ">>"))));
            } else if ( builtin_string_startswith(__hname_rename_at_135_190,((char *) "dasvector`"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_42,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "vector<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_43,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char * const )(builtin_string_slice2(__hname_rename_at_135_190,10,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_44,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ">"))));
            } else if ( builtin_string_startswith(__hname_rename_at_135_190,((char *) "pair`"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_45,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "pair<"))));
                TArray<char *> __parts_rename_at_146_191; das_zero(__parts_rename_at_146_191); das_move(__parts_rename_at_146_191, das_invoke_function<TArray<char *>>::invoke_cmres<char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::split CIs CIs*/ 0x427078f3f7fe6bb)),__hname_rename_at_135_190,((char *) "`")));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_46,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char *>::from(__parts_rename_at_146_191(1,__context__))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_47,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ","))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_48,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char *>::from(__parts_rename_at_146_191(2,__context__))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_49,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ">"))));
            } else {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_50,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_7f23dc2205503d09(__context__,__td_rename_at_116_187->annotation /*annotation*/),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_51, cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__td_rename_at_116_187->annotation /*annotation*/->module /*_module*/)), cast<char * const >::from(((char *) "::")), cast<das::string const  &>::from(__td_rename_at_116_187->annotation /*annotation*/->name /*name*/)))))));
            };
        } else if ( __baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tStructure )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_52,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,struct_label_name_7b7faf8095586da6(__context__,__td_rename_at_116_187->structType /*structType*/),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_53, cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__td_rename_at_116_187->structType /*structType*/->module /*_module*/)), cast<char * const >::from(((char *) "::")), cast<das::string const  &>::from(__td_rename_at_116_187->structType /*structType*/->name /*name*/)))))));
        } else if ( (((__baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tEnumeration) || (__baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tEnumeration8)) || (__baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tEnumeration16)) || (__baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tEnumeration64) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_54,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,enum_label_name_a32575da767b3ecd(__context__,__td_rename_at_116_187->enumType /*enumType*/),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_55, cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__td_rename_at_116_187->enumType /*enumType*/->module /*_module*/)), cast<char * const >::from(((char *) "::")), cast<das::string const  &>::from(__td_rename_at_116_187->enumType /*enumType*/->name /*name*/)))))));
        } else if ( __baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tPointer )
        {
            if ( das_get_bitfield(__td_rename_at_116_187->flags /*flags*/,1u << 10) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_56,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "smart_ptr<"))));
            };
            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_187->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_57,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_187->firstType /*firstType*/))));
            } else {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_58,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "void"))));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_59,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(das_get_bitfield(__td_rename_at_116_187->flags /*flags*/,1u << 10) ? das_auto_cast<char * const >::cast(((char *) ">")) : das_auto_cast<char * const >::cast(((char *) "?")))));
        } else if ( __baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tArray )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_60,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "array"))));
            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_187->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_61,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_62,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_187->firstType /*firstType*/))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_63,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ">"))));
            };
        } else if ( __baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tTable )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_64,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "table"))));
            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_187->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_65,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_66,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_187->firstType /*firstType*/))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_67,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ";"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_68,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_187->secondType /*secondType*/))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_69,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ">"))));
            };
        } else if ( __baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tIterator )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_70,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "iterator"))));
            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_187->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_71,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_72,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_187->firstType /*firstType*/))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_73,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ">"))));
            };
        } else if ( ((__baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tBlock) || (__baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tFunction)) || (__baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tLambda) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_74,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char * const )(ast_das_to_string(__baseType_rename_at_122_189,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_75,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "<"))));
            if ( das_vector_length(__td_rename_at_116_187->argTypes /*argTypes*/) != 0 )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_76,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "("))));
                if ( das_vector_length(__td_rename_at_116_187->argNames /*argNames*/) != 0 )
                {
                    bool __first_rename_at_197_192 = true;
                    {
                        bool __need_loop_198 = true;
                        // at: smart_ptr<ast::TypeDecl> const&
                        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__td_rename_at_116_187->argTypes /*argTypes*/);
                        smart_ptr_raw<TypeDecl> const  * __at_rename_at_198_195;
                        __need_loop_198 = __at_iterator.first(__context__,(__at_rename_at_198_195)) && __need_loop_198;
                        // an: $::das_string const&
                        das_iterator<das::vector<das::string> const > __an_iterator(__td_rename_at_116_187->argNames /*argNames*/);
                        das::string const  * __an_rename_at_198_196;
                        __need_loop_198 = __an_iterator.first(__context__,(__an_rename_at_198_196)) && __need_loop_198;
                        for ( ; __need_loop_198 ; __need_loop_198 = __at_iterator.next(__context__,(__at_rename_at_198_195)) && __an_iterator.next(__context__,(__an_rename_at_198_196)) )
                        {
                            if ( __first_rename_at_197_192 )
                            {
                                das_copy(__first_rename_at_197_192,false);
                            } else {
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_77,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ";"))));
                            };
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_78,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<das::string const  &>::from((*__an_rename_at_198_196))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_79,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ":"))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_80,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_198_195)))));
                        }
                        __at_iterator.close(__context__,(__at_rename_at_198_195));
                        __an_iterator.close(__context__,(__an_rename_at_198_196));
                    };
                } else {
                    bool __first_rename_at_209_197 = true;
                    {
                        bool __need_loop_210 = true;
                        // at: smart_ptr<ast::TypeDecl> const&
                        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__td_rename_at_116_187->argTypes /*argTypes*/);
                        smart_ptr_raw<TypeDecl> const  * __at_rename_at_210_198;
                        __need_loop_210 = __at_iterator.first(__context__,(__at_rename_at_210_198)) && __need_loop_210;
                        for ( ; __need_loop_210 ; __need_loop_210 = __at_iterator.next(__context__,(__at_rename_at_210_198)) )
                        {
                            if ( __first_rename_at_209_197 )
                            {
                                das_copy(__first_rename_at_209_197,false);
                            } else {
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_81,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ";"))));
                            };
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_82,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_210_198)))));
                        }
                        __at_iterator.close(__context__,(__at_rename_at_210_198));
                    };
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_83,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ")"))));
                if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_187->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_84,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ":"))));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_85,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_187->firstType /*firstType*/))));
                };
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_86,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ">"))));
        } else if ( (__baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tTuple) || (__baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tVariant) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_87,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char * const )(ast_das_to_string(__baseType_rename_at_122_189,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_88,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "<"))));
            if ( das_vector_length(__td_rename_at_116_187->argTypes /*argTypes*/) != 0 )
            {
                if ( das_vector_length(__td_rename_at_116_187->argNames /*argNames*/) != 0 )
                {
                    bool __first_rename_at_231_199 = true;
                    {
                        bool __need_loop_232 = true;
                        // at: smart_ptr<ast::TypeDecl> const&
                        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__td_rename_at_116_187->argTypes /*argTypes*/);
                        smart_ptr_raw<TypeDecl> const  * __at_rename_at_232_202;
                        __need_loop_232 = __at_iterator.first(__context__,(__at_rename_at_232_202)) && __need_loop_232;
                        // an: $::das_string const&
                        das_iterator<das::vector<das::string> const > __an_iterator(__td_rename_at_116_187->argNames /*argNames*/);
                        das::string const  * __an_rename_at_232_203;
                        __need_loop_232 = __an_iterator.first(__context__,(__an_rename_at_232_203)) && __need_loop_232;
                        for ( ; __need_loop_232 ; __need_loop_232 = __at_iterator.next(__context__,(__at_rename_at_232_202)) && __an_iterator.next(__context__,(__an_rename_at_232_203)) )
                        {
                            if ( __first_rename_at_231_199 )
                            {
                                das_copy(__first_rename_at_231_199,false);
                            } else {
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_89,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ";"))));
                            };
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_90,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<das::string const  &>::from((*__an_rename_at_232_203))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_91,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ":"))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_92,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_232_202)))));
                        }
                        __at_iterator.close(__context__,(__at_rename_at_232_202));
                        __an_iterator.close(__context__,(__an_rename_at_232_203));
                    };
                } else {
                    bool __first_rename_at_243_204 = true;
                    {
                        bool __need_loop_244 = true;
                        // at: smart_ptr<ast::TypeDecl> const&
                        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__td_rename_at_116_187->argTypes /*argTypes*/);
                        smart_ptr_raw<TypeDecl> const  * __at_rename_at_244_205;
                        __need_loop_244 = __at_iterator.first(__context__,(__at_rename_at_244_205)) && __need_loop_244;
                        for ( ; __need_loop_244 ; __need_loop_244 = __at_iterator.next(__context__,(__at_rename_at_244_205)) )
                        {
                            if ( __first_rename_at_243_204 )
                            {
                                das_copy(__first_rename_at_243_204,false);
                            } else {
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_93,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ";"))));
                            };
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_94,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_244_205)))));
                        }
                        __at_iterator.close(__context__,(__at_rename_at_244_205));
                    };
                };
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_95,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ">"))));
        } else if ( __baseType_rename_at_122_189 == DAS_COMMENT(bound_enum) das::Type::tBitfield )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_96,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char * const )(ast_das_to_string(__baseType_rename_at_122_189,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_97,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "<"))));
            bool __first_rename_at_258_206 = true;
            {
                bool __need_loop_259 = true;
                // an: $::das_string const&
                das_iterator<das::vector<das::string> const > __an_iterator(__td_rename_at_116_187->argNames /*argNames*/);
                das::string const  * __an_rename_at_259_207;
                __need_loop_259 = __an_iterator.first(__context__,(__an_rename_at_259_207)) && __need_loop_259;
                for ( ; __need_loop_259 ; __need_loop_259 = __an_iterator.next(__context__,(__an_rename_at_259_207)) )
                {
                    if ( __first_rename_at_258_206 )
                    {
                        das_copy(__first_rename_at_258_206,false);
                    } else {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_98,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ";"))));
                    };
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_99,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<das::string const  &>::from((*__an_rename_at_259_207))));
                }
                __an_iterator.close(__context__,(__an_rename_at_259_207));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_100,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) ">"))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_101,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char * const )(ast_das_to_string(__baseType_rename_at_122_189,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
        };
        if ( das_get_bitfield(__td_rename_at_116_187->flags /*flags*/,1u << 1) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_102,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) " const"))));
        };
        {
            bool __need_loop_274 = true;
            // d: int const&
            das_iterator<das::vector<int32_t> const > __d_iterator(__td_rename_at_116_187->dim /*dim*/);
            int32_t const  * __d_rename_at_274_208;
            __need_loop_274 = __d_iterator.first(__context__,(__d_rename_at_274_208)) && __need_loop_274;
            for ( ; __need_loop_274 ; __need_loop_274 = __d_iterator.next(__context__,(__d_rename_at_274_208)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_103,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "["))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_104,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<int32_t>::from((*__d_rename_at_274_208))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_105,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "]"))));
            }
            __d_iterator.close(__context__,(__d_rename_at_274_208));
        };
        if ( das_get_bitfield(__td_rename_at_116_187->flags /*flags*/,1u << 0) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_106,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "&"))));
        };
        if ( das_get_bitfield(__td_rename_at_116_187->flags /*flags*/,1u << 2) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_107,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "#"))));
        };
        if ( das_get_bitfield(__td_rename_at_116_187->flags /*flags*/,1u << 3) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_108,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) " implicit"))));
        };
        if ( das_get_bitfield(__td_rename_at_116_187->flags /*flags*/,1u << 8) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_109,cast<StringBuilderWriter &>::from(__writer_rename_at_117_188),cast<char * const >::from(((char *) "!"))));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * make_domain_3e6edb06cf9af7bc ( Context * __context__, char * const  __attr_rename_at_294_209 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_110, cast<char * const >::from(((char *) ".. ")), cast<char * const >::from(__attr_rename_at_294_209), cast<char * const >::from(((char *) ":: ")))));
}

inline char * make_label_3368ba1f933c2731 ( Context * __context__, char * const  __name_rename_at_298_210 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_111, cast<char * const >::from(((char *) ".. _")), cast<char * const >::from(__name_rename_at_298_210), cast<char * const >::from(((char *) ":\n\n")))));
}

inline char * make_ref_c995159f43ed1b43 ( Context * __context__, char * const  __name_rename_at_302_211, char * const  __text_rename_at_302_212 )
{
    TArray<AutoTuple<char *,char *>> _temp_make_local_303_37_48; _temp_make_local_303_37_48;
    TDim<AutoTuple<char *,char *>,3> _temp_make_local_303_37_80; _temp_make_local_303_37_80;
    char * __t_rename_at_303_213 = ((char *)(char *)(das_invoke_function<char * const >::invoke<char * const ,TArray<AutoTuple<char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 0x510d9544ac4d5ab4)),__text_rename_at_302_212,das_arg<TArray<AutoTuple<char *,char *>>>::pass((_temp_make_local_303_37_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_2c64031744b1c1f2(__context__,das_arg<TDim<AutoTuple<char *,char *>,3>>::pass((([&]() -> TDim<AutoTuple<char *,char *>,3>& {
        _temp_make_local_303_37_80(0,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_304;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_304) = ((char *) "<");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_304) = ((char *) "\\<");
            return __mkt_304;
        })());
        _temp_make_local_303_37_80(1,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_305;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_305) = ((char *) ">");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_305) = ((char *) "\\>");
            return __mkt_305;
        })());
        _temp_make_local_303_37_80(2,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_306;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_306) = ((char *) "!");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_306) = ((char *) "\\!");
            return __mkt_306;
        })());
        return _temp_make_local_303_37_80;
    })())))))))));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_112, cast<char * const >::from(((char *) " :ref:`")), cast<char * const >::from(__t_rename_at_303_213), cast<char * const >::from(((char *) " <")), cast<char * const >::from(__name_rename_at_302_211), cast<char * const >::from(((char *) ">` ")))));
}

inline char * make_group_8b304b06bc5b7645 ( Context * __context__, char * const  __name_rename_at_311_214, char * const  __plus_rename_at_311_215 )
{
    int32_t __len_rename_at_312_216 = ((int32_t)builtin_string_length(__name_rename_at_311_214,__context__));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_113, cast<char * const >::from(((char * const )(string_repeat(__plus_rename_at_311_215,__len_rename_at_312_216,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__name_rename_at_311_214), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(__plus_rename_at_311_215,__len_rename_at_312_216,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n\n")))));
}

inline char * make_header_efad6667e51b3e08 ( Context * __context__, char * const  __name_rename_at_316_217, char * const  __lab_rename_at_316_218 )
{
    int32_t __wide_rename_at_317_219 = ((int32_t)builtin_string_length(__name_rename_at_316_217,__context__));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<9>(__tinfo_114, cast<char * const >::from(((char *) ".. _stdlib_")), cast<char * const >::from(__lab_rename_at_316_218), cast<char * const >::from(((char *) ":\n\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "="),__wide_rename_at_317_219,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__name_rename_at_316_217), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "="),__wide_rename_at_317_219,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n\n")))));
}

inline void write_table_separator_2180d51ced094114 ( Context * __context__, StringBuilderWriter &  __writer_rename_at_321_220, TArray<int32_t> const  &  __wid_rename_at_321_221, char * const  __sep_rename_at_321_222 )
{
    {
        bool __need_loop_322 = true;
        // x: int const&
        das_iterator<TArray<int32_t> const > __x_iterator(__wid_rename_at_321_221);
        int32_t const  * __x_rename_at_322_223;
        __need_loop_322 = __x_iterator.first(__context__,(__x_rename_at_322_223)) && __need_loop_322;
        for ( ; __need_loop_322 ; __need_loop_322 = __x_iterator.next(__context__,(__x_rename_at_322_223)) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_115,cast<StringBuilderWriter &>::from(__writer_rename_at_321_220),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_116, cast<char * const >::from(((char *) "+")), cast<char * const >::from(((char * const )(string_repeat(__sep_rename_at_321_222,(*__x_rename_at_322_223),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))))));
        }
        __x_iterator.close(__context__,(__x_rename_at_322_223));
    };
    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_117,cast<StringBuilderWriter &>::from(__writer_rename_at_321_220),cast<char * const >::from(((char *) "+\n"))));
}

inline void write_table_line_18bbb8245228f037 ( Context * __context__, StringBuilderWriter &  __writer_rename_at_328_224, TArray<char *> const  &  __line_rename_at_328_225, TArray<int32_t> const  &  __wid_rename_at_328_226 )
{
    {
        bool __need_loop_329 = true;
        // x: int const&
        das_iterator<TArray<int32_t> const > __x_iterator(__wid_rename_at_328_226);
        int32_t const  * __x_rename_at_329_229;
        __need_loop_329 = __x_iterator.first(__context__,(__x_rename_at_329_229)) && __need_loop_329;
        // l: string const&
        das_iterator<TArray<char *> const > __l_iterator(__line_rename_at_328_225);
        char * const  * __l_rename_at_329_230;
        __need_loop_329 = __l_iterator.first(__context__,(__l_rename_at_329_230)) && __need_loop_329;
        for ( ; __need_loop_329 ; __need_loop_329 = __x_iterator.next(__context__,(__x_rename_at_329_229)) && __l_iterator.next(__context__,(__l_rename_at_329_230)) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_118,cast<StringBuilderWriter &>::from(__writer_rename_at_328_224),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_119, cast<char * const >::from(((char *) "+")), cast<char * const >::from(das_invoke_function<char * const >::invoke<char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::wide CIs Ci*/ 0xc68522a6b4b16a0d)),(*__l_rename_at_329_230),(*__x_rename_at_329_229))))))));
        }
        __x_iterator.close(__context__,(__x_rename_at_329_229));
        __l_iterator.close(__context__,(__l_rename_at_329_230));
    };
    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_120,cast<StringBuilderWriter &>::from(__writer_rename_at_328_224),cast<char * const >::from(((char *) "+\n"))));
}

inline char * make_table_46d5ce8cf5a30e69 ( Context * __context__, TArray<TArray<char *>> const  &  __tab_rename_at_335_231, bool __withHeader_rename_at_335_232 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_336_233) DAS_AOT_INLINE_LAMBDA -> void{
        Sequence DAS_COMMENT((int32_t)) _temp_make_local_349_30_224; _temp_make_local_349_30_224;
        int32_t __cols_rename_at_338_234 = 0;
        {
            bool __need_loop_339 = true;
            // c: array<string> const&
            das_iterator<TArray<TArray<char *>> const > __c_iterator(__tab_rename_at_335_231);
            TArray<char *> const  * __c_rename_at_339_235;
            __need_loop_339 = __c_iterator.first(__context__,(__c_rename_at_339_235)) && __need_loop_339;
            for ( ; __need_loop_339 ; __need_loop_339 = __c_iterator.next(__context__,(__c_rename_at_339_235)) )
            {
                das_copy(__cols_rename_at_338_234,SimPolicy<int32_t>::Max(__cols_rename_at_338_234,builtin_array_size((*__c_rename_at_339_235)),*__context__,nullptr));
            }
            __c_iterator.close(__context__,(__c_rename_at_339_235));
        };
        TArray<int32_t> __wid_rename_at_342_236; das_zero(__wid_rename_at_342_236);
        _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51(__context__,das_arg<TArray<int32_t>>::pass(__wid_rename_at_342_236),__cols_rename_at_338_234);
        {
            bool __need_loop_344 = true;
            // cc: array<string> const&
            das_iterator<TArray<TArray<char *>> const > __cc_iterator(__tab_rename_at_335_231);
            TArray<char *> const  * __cc_rename_at_344_237;
            __need_loop_344 = __cc_iterator.first(__context__,(__cc_rename_at_344_237)) && __need_loop_344;
            for ( ; __need_loop_344 ; __need_loop_344 = __cc_iterator.next(__context__,(__cc_rename_at_344_237)) )
            {
                {
                    bool __need_loop_345 = true;
                    // x: int const
                    das_iterator<range> __x_iterator(mk_range(__cols_rename_at_338_234));
                    int32_t __x_rename_at_345_238;
                    __need_loop_345 = __x_iterator.first(__context__,(__x_rename_at_345_238)) && __need_loop_345;
                    for ( ; __need_loop_345 ; __need_loop_345 = __x_iterator.next(__context__,(__x_rename_at_345_238)) )
                    {
                        das_copy(__wid_rename_at_342_236(__x_rename_at_345_238,__context__),SimPolicy<int32_t>::Max(__wid_rename_at_342_236(__x_rename_at_345_238,__context__),builtin_string_length((*__cc_rename_at_344_237)(__x_rename_at_345_238,__context__),__context__),*__context__,nullptr));
                    }
                    __x_iterator.close(__context__,(__x_rename_at_345_238));
                };
            }
            __cc_iterator.close(__context__,(__cc_rename_at_344_237));
        };
        {
            bool __need_loop_349 = true;
            // ccc: array<string> const&
            das_iterator<TArray<TArray<char *>> const > __ccc_iterator(__tab_rename_at_335_231);
            TArray<char *> const  * __ccc_rename_at_349_241;
            __need_loop_349 = __ccc_iterator.first(__context__,(__ccc_rename_at_349_241)) && __need_loop_349;
            // idx: int
            das_iterator_count DAS_COMMENT((_temp_make_local_349_30_224 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __idx_iterator(0,1);
            int32_t __idx_rename_at_349_242;
            __need_loop_349 = __idx_iterator.first(__context__,(__idx_rename_at_349_242)) && __need_loop_349;
            for ( ; __need_loop_349 ; __need_loop_349 = __ccc_iterator.next(__context__,(__ccc_rename_at_349_241)) && __idx_iterator.next(__context__,(__idx_rename_at_349_242)) )
            {
                write_table_separator_2180d51ced094114(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_336_233),das_arg<TArray<int32_t>>::pass(__wid_rename_at_342_236),((__idx_rename_at_349_242 == 1) && __withHeader_rename_at_335_232) ? das_auto_cast<char * const >::cast(((char *) "=")) : das_auto_cast<char * const >::cast(((char *) "-")));
                write_table_line_18bbb8245228f037(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_336_233),(*__ccc_rename_at_349_241),das_arg<TArray<int32_t>>::pass(__wid_rename_at_342_236));
            }
            __ccc_iterator.close(__context__,(__ccc_rename_at_349_241));
            __idx_iterator.close(__context__,(__idx_rename_at_349_242));
        };
        write_table_separator_2180d51ced094114(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_336_233),das_arg<TArray<int32_t>>::pass(__wid_rename_at_342_236),((char *) "-"));
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_121,cast<StringBuilderWriter &>::from(__writer_rename_at_336_233),cast<char * const >::from(((char *) "\n\n"))));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * topic_9659659d94ddb29a ( Context * __context__, char * const  __category_rename_at_358_243, Module * const  __mod_rename_at_358_244, char * const  __name_rename_at_358_245 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<7>(__tinfo_122, cast<char * const >::from(((char *) "|")), cast<char * const >::from(__category_rename_at_358_243), cast<char * const >::from(((char *) "-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__mod_rename_at_358_244)), cast<char * const >::from(((char *) "-")), cast<char * const >::from(__name_rename_at_358_245), cast<char * const >::from(((char *) "|")))));
}

inline char * topic_b97c7facf4ccb6a9 ( Context * __context__, Module * const  __mod_rename_at_362_246 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_123, cast<char * const >::from(((char *) "|module-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__mod_rename_at_362_246)), cast<char * const >::from(((char *) "|")))));
}

inline void document_topic_c6e2614dff9f2d3e ( Context * __context__, FILE const  * const  __doc_file_rename_at_366_247, char * const  __topic_rename_at_366_248 ) { das_stack_prologue __prologue(__context__,288,"document_topic " DAS_FILE_LINE);
{
    char * __topic_file_name_rename_at_367_249 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_124, cast<char *>::from(das_global<char *,0xde937609e38e5c08>(__context__) /*topic_root*/), cast<char * const >::from(((char *) "/")), cast<char * const >::from(((char * const )(builtin_string_slice1(__topic_rename_at_366_248,1,-1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ".rst"))))));
    FStat __st_rename_at_368_250; das_zero(__st_rename_at_368_250);
    if ( builtin_stat(__topic_file_name_rename_at_367_249,das_arg<FStat>::pass(__st_rename_at_368_250)) )
    {
        _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,__topic_file_name_rename_at_367_249,((char *) "rb"),das_make_block<void,FILE const  * const >(__context__,256,0,&__func_info__4bda63c2c7a42bef,[&](FILE const  * const  __topic_file_rename_at_370_251) -> void{
            if ( __topic_file_rename_at_370_251 != nullptr )
            {
                char * __instxt_rename_at_372_252 = ((char *)(char *)(((char * const )(builtin_fread(__topic_file_rename_at_370_251,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                builtin_fwrite(__doc_file_rename_at_366_247,__instxt_rename_at_372_252,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                builtin_fwrite(__doc_file_rename_at_366_247,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            } else {
                builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_125, cast<char * const >::from(((char *) "can't open ")), cast<char * const >::from(__topic_file_name_rename_at_367_249))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }));
    } else {
        builtin_fwrite(__doc_file_rename_at_366_247,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_126, cast<char * const >::from(__topic_rename_at_366_248), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_bitfield_4340095c1075f95e ( Context * __context__, FILE const  * const  __doc_file_rename_at_384_253, smart_ptr_raw<TypeDecl> const  __value_rename_at_384_254 )
{
    TArray<char *> _temp_make_local_386_17_64; _temp_make_local_386_17_64;
    TDim<char *,3> _temp_make_local_386_17_96; _temp_make_local_386_17_96;
    TArray<TArray<char *>> __tab_rename_at_385_255; das_zero(__tab_rename_at_385_255);
    _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_385_255),das_arg<TArray<char *>>::pass((_temp_make_local_386_17_64 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_649bcd7e51242beb(__context__,das_arg<TDim<char *,3>>::pass((([&]() -> TDim<char *,3>& {
        _temp_make_local_386_17_96(0,__context__) = ((char *) "field");
        _temp_make_local_386_17_96(1,__context__) = ((char *) "bit");
        _temp_make_local_386_17_96(2,__context__) = ((char *) "value");
        return _temp_make_local_386_17_96;
    })())))))));
    {
        bool __need_loop_387 = true;
        // an: $::das_string const&
        das_iterator<das::vector<das::string> const > __an_iterator(__value_rename_at_384_254->argNames /*argNames*/);
        das::string const  * __an_rename_at_387_258;
        __need_loop_387 = __an_iterator.first(__context__,(__an_rename_at_387_258)) && __need_loop_387;
        // bit: int const
        das_iterator<range> __bit_iterator(range(0,32));
        int32_t __bit_rename_at_387_259;
        __need_loop_387 = __bit_iterator.first(__context__,(__bit_rename_at_387_259)) && __need_loop_387;
        for ( ; __need_loop_387 ; __need_loop_387 = __an_iterator.next(__context__,(__an_rename_at_387_258)) && __bit_iterator.next(__context__,(__bit_rename_at_387_259)) )
        {
            TArray<char *> __line_rename_at_388_260; das_zero(__line_rename_at_388_260);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_388_260),((char * const )(to_das_string((*__an_rename_at_387_258),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_388_260),((char * const )(das_lexical_cast_int_i32(__bit_rename_at_387_259,false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_388_260),((char * const )(das_lexical_cast_int_i32(1 << __bit_rename_at_387_259,false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_385_255),das_arg<TArray<char *>>::pass(__line_rename_at_388_260));
        }
        __an_iterator.close(__context__,(__an_rename_at_387_258));
        __bit_iterator.close(__context__,(__bit_rename_at_387_259));
    };
    builtin_fwrite(__doc_file_rename_at_384_253,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_385_255),true),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void document_variant_c74548ebfca402e7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_397_261, smart_ptr_raw<TypeDecl> const  __value_rename_at_397_262 )
{
    TArray<TArray<char *>> __tab_rename_at_398_263; das_zero(__tab_rename_at_398_263);
    {
        bool __need_loop_399 = true;
        // an: $::das_string const&
        das_iterator<das::vector<das::string> const > __an_iterator(__value_rename_at_397_262->argNames /*argNames*/);
        das::string const  * __an_rename_at_399_266;
        __need_loop_399 = __an_iterator.first(__context__,(__an_rename_at_399_266)) && __need_loop_399;
        // at: smart_ptr<ast::TypeDecl> const&
        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__value_rename_at_397_262->argTypes /*argTypes*/);
        smart_ptr_raw<TypeDecl> const  * __at_rename_at_399_267;
        __need_loop_399 = __at_iterator.first(__context__,(__at_rename_at_399_267)) && __need_loop_399;
        for ( ; __need_loop_399 ; __need_loop_399 = __an_iterator.next(__context__,(__an_rename_at_399_266)) && __at_iterator.next(__context__,(__at_rename_at_399_267)) )
        {
            TArray<char *> __line_rename_at_400_268; das_zero(__line_rename_at_400_268);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_400_268),((char * const )(to_das_string((*__an_rename_at_399_266),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_400_268),describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_399_267)));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_398_263),das_arg<TArray<char *>>::pass(__line_rename_at_400_268));
        }
        __an_iterator.close(__context__,(__an_rename_at_399_266));
        __at_iterator.close(__context__,(__at_rename_at_399_267));
    };
    builtin_fwrite(__doc_file_rename_at_397_261,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_398_263),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void document_tuple_c74548ebfca402e7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_408_269, smart_ptr_raw<TypeDecl> const  __value_rename_at_408_270 )
{
    TArray<TArray<char *>> __tab_rename_at_409_271; das_zero(__tab_rename_at_409_271);
    {
        bool __need_loop_410 = true;
        // an: $::das_string const&
        das_iterator<das::vector<das::string> const > __an_iterator(__value_rename_at_408_270->argNames /*argNames*/);
        das::string const  * __an_rename_at_410_274;
        __need_loop_410 = __an_iterator.first(__context__,(__an_rename_at_410_274)) && __need_loop_410;
        // at: smart_ptr<ast::TypeDecl> const&
        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__value_rename_at_408_270->argTypes /*argTypes*/);
        smart_ptr_raw<TypeDecl> const  * __at_rename_at_410_275;
        __need_loop_410 = __at_iterator.first(__context__,(__at_rename_at_410_275)) && __need_loop_410;
        for ( ; __need_loop_410 ; __need_loop_410 = __an_iterator.next(__context__,(__an_rename_at_410_274)) && __at_iterator.next(__context__,(__at_rename_at_410_275)) )
        {
            TArray<char *> __line_rename_at_411_276; das_zero(__line_rename_at_411_276);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_411_276),((char * const )(to_das_string((*__an_rename_at_410_274),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_411_276),describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_410_275)));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_409_271),das_arg<TArray<char *>>::pass(__line_rename_at_411_276));
        }
        __an_iterator.close(__context__,(__an_rename_at_410_274));
        __at_iterator.close(__context__,(__at_rename_at_410_275));
    };
    builtin_fwrite(__doc_file_rename_at_408_269,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_409_271),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void document_typedefs_6ca650b4c8738629 ( Context * __context__, FILE const  * const  __doc_file_rename_at_440_277, Module * const  __mod_rename_at_440_278 ) { das_stack_prologue __prologue(__context__,176,"document_typedefs " DAS_FILE_LINE);
{
    bool __any_rename_at_441_279 = false;
    for_each_typedef(__mod_rename_at_440_278,das_make_block<void,char *,smart_ptr_raw<TypeDecl>>(__context__,96,0,&__func_info__aa03b7bcb8f14157,[&](char * __name_rename_at_442_280, smart_ptr_raw<TypeDecl> __value_rename_at_442_281) -> void{
        das_copy(__any_rename_at_441_279,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_441_279 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_440_277,((char *) "++++++++++++\nType aliases\n++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_typedef(__mod_rename_at_440_278,das_make_block<void,char *,smart_ptr_raw<TypeDecl>>(__context__,160,0,&__func_info__aa03b7bcb8f14157,[&](char * __name_rename_at_449_282, smart_ptr_raw<TypeDecl> __value_rename_at_449_283) -> void{
            _FuncrstTickdocument_typedefTick2903024037696058038_48a0306c1d5a26e9(__context__,__doc_file_rename_at_440_277,__mod_rename_at_440_278,__name_rename_at_449_282,__value_rename_at_449_283);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_global_constant_378f3e1a10c345f3 ( Context * __context__, FILE const  * const  __doc_file_rename_at_454_284, Module * const  __mod_rename_at_454_285, smart_ptr_raw<Variable> const  __value_rename_at_454_286 )
{
    builtin_fwrite(__doc_file_rename_at_454_284,make_label_3368ba1f933c2731(__context__,global_label_name_b721f72143d35230(__context__,__value_rename_at_454_286)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_454_284,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_454_284,((char * const )(to_das_string(__value_rename_at_454_286->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_454_284,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_127, cast<char * const >::from(((char *) " = ")), cast<char * const >::from(_FuncastTickdescribeTick842554968825501494_452cfc8955963a18(__context__,__value_rename_at_454_286->init /*init*/)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_454_284,topic_9659659d94ddb29a(__context__,((char *) "variable"),__mod_rename_at_454_285,((char * const )(to_das_string(__value_rename_at_454_286->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline bool is_global_constant_320cc2da3d8764f7 ( Context * __context__, smart_ptr_raw<Variable> const  __value_rename_at_462_287 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__value_rename_at_462_287->flags /*flags*/,1u << 10) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(das_get_bitfield(__value_rename_at_462_287->type /*_type*/->flags /*flags*/,1u << 1)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(((das_deref(__context__,__value_rename_at_462_287->type /*_type*/)).isWorkhorseType())) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__value_rename_at_462_287->init /*init*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))))));
}

inline void document_global_constants_24362fe0342586e0 ( Context * __context__, FILE const  * const  __doc_file_rename_at_478_288, Module * const  __mod_rename_at_478_289 ) { das_stack_prologue __prologue(__context__,176,"document_global_constants " DAS_FILE_LINE);
{
    bool __any_rename_at_479_290 = false;
    for_each_global(__mod_rename_at_478_289,das_make_block<void,smart_ptr_raw<Variable>>(__context__,96,0,&__func_info__2701c05c12e53ee0,[&](smart_ptr_raw<Variable> __value_rename_at_480_291) -> void{
        if ( is_global_constant_320cc2da3d8764f7(__context__,__value_rename_at_480_291) )
        {
            das_copy(__any_rename_at_479_290,true);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_479_290 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_478_288,((char *) "+++++++++\nConstants\n+++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_global(__mod_rename_at_478_289,das_make_block<void,smart_ptr_raw<Variable>>(__context__,160,0,&__func_info__2701c05c12e53ee0,[&](smart_ptr_raw<Variable> __value_rename_at_489_292) -> void{
            if ( is_global_constant_320cc2da3d8764f7(__context__,__value_rename_at_489_292) )
            {
                document_global_constant_378f3e1a10c345f3(__context__,__doc_file_rename_at_478_288,__mod_rename_at_478_289,__value_rename_at_489_292);
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_enumerations_c255f6c4885d0cb5 ( Context * __context__, FILE const  * const  __doc_file_rename_at_511_293, Module * const  __mod_rename_at_511_294 ) { das_stack_prologue __prologue(__context__,176,"document_enumerations " DAS_FILE_LINE);
{
    bool __any_rename_at_512_295 = false;
    for_each_enumeration(__mod_rename_at_511_294,das_make_block<void,smart_ptr_raw<Enumeration>>(__context__,96,0,&__func_info__9beb038de389c5b0,[&](smart_ptr_raw<Enumeration> __value_rename_at_513_296) -> void{
        if ( __value_rename_at_513_296->isPrivate /*isPrivate*/ )
        {
            return ;
        };
        das_copy(__any_rename_at_512_295,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_512_295 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_511_293,((char *) "++++++++++++\nEnumerations\n++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_enumeration(__mod_rename_at_511_294,das_make_block<void,smart_ptr_raw<Enumeration>>(__context__,160,0,&__func_info__9beb038de389c5b0,[&](smart_ptr_raw<Enumeration> __value_rename_at_523_297) -> void{
            if ( __value_rename_at_523_297->isPrivate /*isPrivate*/ )
            {
                return ;
            };
            _FuncrstTickdocument_enumerationTick4211850574665690795_5251d76ee781cebe(__context__,__doc_file_rename_at_511_293,__mod_rename_at_511_294,__value_rename_at_523_297);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_function_declaration_4c978976b1aaa270 ( Context * __context__, FILE const  * const  __doc_file_rename_at_541_298, char * const  __domain_rename_at_541_299, char * const  __func_name_rename_at_541_300, TArray<char *> const  &  __argNames_rename_at_542_301, TArray<TypeDecl *> const  &  __argTypes_rename_at_542_302, smart_ptr_raw<TypeDecl> const  __resType_rename_at_542_303 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_545_42_32; _temp_make_local_545_42_32;
    builtin_fwrite(__doc_file_rename_at_541_298,make_domain_3e6edb06cf9af7bc(__context__,__domain_rename_at_541_299),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_541_298,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_128, cast<char * const >::from(__func_name_rename_at_541_300), cast<char * const >::from(((char *) "(")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_545 = true;
        // an: string const&
        das_iterator<TArray<char *> const > __an_iterator(__argNames_rename_at_542_301);
        char * const  * __an_rename_at_545_310;
        __need_loop_545 = __an_iterator.first(__context__,(__an_rename_at_545_310)) && __need_loop_545;
        // at: ast::TypeDecl? const&
        das_iterator<TArray<TypeDecl *> const > __at_iterator(__argTypes_rename_at_542_302);
        TypeDecl * const  * __at_rename_at_545_311;
        __need_loop_545 = __at_iterator.first(__context__,(__at_rename_at_545_311)) && __need_loop_545;
        // i: int
        das_iterator_count DAS_COMMENT((_temp_make_local_545_42_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
        int32_t __i_rename_at_545_312;
        __need_loop_545 = __i_iterator.first(__context__,(__i_rename_at_545_312)) && __need_loop_545;
        for ( ; __need_loop_545 ; __need_loop_545 = __an_iterator.next(__context__,(__an_rename_at_545_310)) && __at_iterator.next(__context__,(__at_rename_at_545_311)) && __i_iterator.next(__context__,(__i_rename_at_545_312)) )
        {
            if ( _FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6(__context__,(*__at_rename_at_545_311)) )
            {
                if ( __i_rename_at_545_312 != 0 )
                {
                    builtin_fwrite(__doc_file_rename_at_541_298,((char *) "; "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
                builtin_fwrite(__doc_file_rename_at_541_298,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_129, cast<char * const >::from((*__an_rename_at_545_310)), cast<char * const >::from(((char *) ": ")), cast<char * const >::from(describe_type_short_56370d8993e381ba(__context__,(*__at_rename_at_545_311))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __an_iterator.close(__context__,(__an_rename_at_545_310));
        __at_iterator.close(__context__,(__at_rename_at_545_311));
        __i_iterator.close(__context__,(__i_rename_at_545_312));
    };
    builtin_fwrite(__doc_file_rename_at_541_298,((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_541_298,((char *) "\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void document_function_arguments_e7469e75af4ed246 ( Context * __context__, FILE const  * const  __doc_file_rename_at_557_313, TArray<char *> const  &  __argNames_rename_at_557_314, TArray<TypeDecl *> const  &  __argTypes_rename_at_557_315 )
{
    TArray<char *> _temp_make_local_559_17_64; _temp_make_local_559_17_64;
    TDim<char *,2> _temp_make_local_559_17_96; _temp_make_local_559_17_96;
    TArray<TArray<char *>> __tab_rename_at_558_316; das_zero(__tab_rename_at_558_316);
    _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_558_316),das_arg<TArray<char *>>::pass((_temp_make_local_559_17_64 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_325c63fe2a174cf6(__context__,das_arg<TDim<char *,2>>::pass((([&]() -> TDim<char *,2>& {
        _temp_make_local_559_17_96(0,__context__) = ((char *) "argument");
        _temp_make_local_559_17_96(1,__context__) = ((char *) "argument type");
        return _temp_make_local_559_17_96;
    })())))))));
    {
        bool __need_loop_560 = true;
        // an: string const&
        das_iterator<TArray<char *> const > __an_iterator(__argNames_rename_at_557_314);
        char * const  * __an_rename_at_560_319;
        __need_loop_560 = __an_iterator.first(__context__,(__an_rename_at_560_319)) && __need_loop_560;
        // at: ast::TypeDecl? const&
        das_iterator<TArray<TypeDecl *> const > __at_iterator(__argTypes_rename_at_557_315);
        TypeDecl * const  * __at_rename_at_560_320;
        __need_loop_560 = __at_iterator.first(__context__,(__at_rename_at_560_320)) && __need_loop_560;
        for ( ; __need_loop_560 ; __need_loop_560 = __an_iterator.next(__context__,(__an_rename_at_560_319)) && __at_iterator.next(__context__,(__at_rename_at_560_320)) )
        {
            if ( _FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6(__context__,(*__at_rename_at_560_320)) )
            {
                TArray<char *> __line_rename_at_562_321; das_zero(__line_rename_at_562_321);
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_562_321),(*__an_rename_at_560_319));
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_562_321),describe_type_cf1a091488868462(__context__,(*__at_rename_at_560_320)));
                _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_558_316),das_arg<TArray<char *>>::pass(__line_rename_at_562_321));
            };
        }
        __an_iterator.close(__context__,(__an_rename_at_560_319));
        __at_iterator.close(__context__,(__at_rename_at_560_320));
    };
    if ( builtin_array_size(das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_558_316)) != 1 )
    {
        builtin_fwrite(__doc_file_rename_at_557_313,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_558_316),true),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void document_classes_1fc51dae5246eec0 ( Context * __context__, FILE const  * const  __doc_file_rename_at_587_322, Module * const  __mod_rename_at_587_323 ) { das_stack_prologue __prologue(__context__,528,"document_classes " DAS_FILE_LINE);
{
    bool __any_rename_at_588_324 = false;
    for_each_structure(__mod_rename_at_587_323,das_make_block<void,smart_ptr_raw<Structure>>(__context__,96,0,&__func_info__438466bc7bed9910,[&](smart_ptr_raw<Structure> __value_rename_at_589_325) -> void{
        if ( das_get_bitfield(__value_rename_at_589_325->flags /*flags*/,1u << 0) && !(das_get_bitfield(__value_rename_at_589_325->flags /*flags*/,1u << 7)) )
        {
            das_copy(__any_rename_at_588_324,true);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_588_324 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_587_322,((char *) "+++++++\nClasses\n+++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_structure(__mod_rename_at_587_323,das_make_block<void,smart_ptr_raw<Structure>>(__context__,160,0,&__func_info__438466bc7bed9910,[&](smart_ptr_raw<Structure> __value_rename_at_598_326) -> void{
            if ( !(das_get_bitfield(__value_rename_at_598_326->flags /*flags*/,1u << 0)) || das_get_bitfield(__value_rename_at_598_326->flags /*flags*/,1u << 7) )
            {
                return ;
            };
            builtin_fwrite(__doc_file_rename_at_587_322,make_label_3368ba1f933c2731(__context__,struct_label_name_6ee1d24e90557aeb(__context__,__value_rename_at_598_326)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            builtin_fwrite(__doc_file_rename_at_587_322,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            builtin_fwrite(__doc_file_rename_at_587_322,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_598_326->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            if ( __value_rename_at_598_326->parent /*parent*/ != nullptr )
            {
                builtin_fwrite(__doc_file_rename_at_587_322,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_130, cast<char * const >::from(((char *) " : ")), cast<das::string &>::from(__value_rename_at_598_326->parent /*parent*/->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
            builtin_fwrite(__doc_file_rename_at_587_322,((char *) "\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_587_322,topic_9659659d94ddb29a(__context__,((char *) "class"),__mod_rename_at_587_323,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_598_326->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
            int32_t __w_rename_at_610_327 = 0;
            {
                bool __need_loop_611 = true;
                // fld: ast::FieldDeclaration&
                das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__value_rename_at_598_326->fields /*fields*/);
                Structure::FieldDeclaration * __fld_rename_at_611_328;
                __need_loop_611 = __fld_iterator.first(__context__,(__fld_rename_at_611_328)) && __need_loop_611;
                for ( ; __need_loop_611 ; __need_loop_611 = __fld_iterator.next(__context__,(__fld_rename_at_611_328)) )
                {
                    if ( (!(das_invoke_function<bool>::invoke<smart_ptr_raw<Structure> const ,smart_ptr_raw<TypeDecl> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::is_class_method CY<StructurePtr>1<H<ast::Structure>>?M CY<TypeDeclPtr>1<H<ast::TypeDecl>>?M*/ 0xa1c63892e7a43312)),__value_rename_at_598_326,(*__fld_rename_at_611_328).type /*_type*/)) && !(das_get_bitfield((*__fld_rename_at_611_328).flags /*flags*/,1u << 3))) && !(das_get_bitfield((*__fld_rename_at_611_328).flags /*flags*/,1u << 1)) )
                    {
                        das_copy(__w_rename_at_610_327,SimPolicy<int32_t>::Max(__w_rename_at_610_327,builtin_ext_string_length(das_arg<das::string>::pass((*__fld_rename_at_611_328).name /*name*/)),*__context__,nullptr));
                    };
                }
                __fld_iterator.close(__context__,(__fld_rename_at_611_328));
            };
            if ( __w_rename_at_610_327 != 0 )
            {
                builtin_fwrite(__doc_file_rename_at_587_322,((char *) "it defines as follows\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                {
                    bool __need_loop_618 = true;
                    // fld: ast::FieldDeclaration&
                    das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__value_rename_at_598_326->fields /*fields*/);
                    Structure::FieldDeclaration * __fld_rename_at_618_329;
                    __need_loop_618 = __fld_iterator.first(__context__,(__fld_rename_at_618_329)) && __need_loop_618;
                    for ( ; __need_loop_618 ; __need_loop_618 = __fld_iterator.next(__context__,(__fld_rename_at_618_329)) )
                    {
                        if ( ((!(das_invoke_function<bool>::invoke<smart_ptr_raw<Structure> const ,smart_ptr_raw<TypeDecl> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::is_class_method CY<StructurePtr>1<H<ast::Structure>>?M CY<TypeDeclPtr>1<H<ast::TypeDecl>>?M*/ 0xa1c63892e7a43312)),__value_rename_at_598_326,(*__fld_rename_at_618_329).type /*_type*/)) && !(das_get_bitfield((*__fld_rename_at_618_329).flags /*flags*/,1u << 3))) && !(das_get_bitfield((*__fld_rename_at_618_329).flags /*flags*/,1u << 1))) && (neq_dstr_str(das_arg<das::string>::pass((*__fld_rename_at_618_329).name /*name*/),((char *) "__rtti"))) )
                        {
                            builtin_fwrite(__doc_file_rename_at_587_322,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_131, cast<char * const >::from(((char *) "  | ")), cast<char * const >::from(das_invoke_function<char * const >::invoke<char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::wide CIs Ci*/ 0xc68522a6b4b16a0d)),((char * const )(to_das_string(das_arg<das::string>::pass((*__fld_rename_at_618_329).name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),__w_rename_at_610_327)), cast<char * const >::from(((char *) " : ")), cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__fld_rename_at_618_329).type /*_type*/)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        };
                    }
                    __fld_iterator.close(__context__,(__fld_rename_at_618_329));
                };
                builtin_fwrite(__doc_file_rename_at_587_322,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
            {
                bool __need_loop_625 = true;
                // fld: ast::FieldDeclaration&
                das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__value_rename_at_598_326->fields /*fields*/);
                Structure::FieldDeclaration * __fld_rename_at_625_330;
                __need_loop_625 = __fld_iterator.first(__context__,(__fld_rename_at_625_330)) && __need_loop_625;
                for ( ; __need_loop_625 ; __need_loop_625 = __fld_iterator.next(__context__,(__fld_rename_at_625_330)) )
                {
                    if ( ((das_invoke_function<bool>::invoke<smart_ptr_raw<Structure> const ,smart_ptr_raw<TypeDecl> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::is_class_method CY<StructurePtr>1<H<ast::Structure>>?M CY<TypeDeclPtr>1<H<ast::TypeDecl>>?M*/ 0xa1c63892e7a43312)),__value_rename_at_598_326,(*__fld_rename_at_625_330).type /*_type*/) && !(das_get_bitfield((*__fld_rename_at_625_330).flags /*flags*/,1u << 3))) && !(das_get_bitfield((*__fld_rename_at_625_330).flags /*flags*/,1u << 1))) && das_get_bitfield((*__fld_rename_at_625_330).flags /*flags*/,1u << 8) )
                    {
                        _FuncrstTickdocument_class_methodTick12589992380795293570_b41bc23f1e57e06(__context__,__doc_file_rename_at_587_322,__mod_rename_at_587_323,__value_rename_at_598_326,das_arg<Structure::FieldDeclaration>::pass((*__fld_rename_at_625_330)));
                    };
                }
                __fld_iterator.close(__context__,(__fld_rename_at_625_330));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_structure_96bd65f4eda31cf3 ( Context * __context__, FILE const  * const  __doc_file_rename_at_633_331, Module * const  __mod_rename_at_633_332, smart_ptr_raw<Structure> const  __value_rename_at_633_333 )
{
    builtin_fwrite(__doc_file_rename_at_633_331,make_label_3368ba1f933c2731(__context__,struct_label_name_6ee1d24e90557aeb(__context__,__value_rename_at_633_333)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_633_331,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_633_331,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_132, cast<das::string const  &>::from(__value_rename_at_633_333->name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __value_rename_at_633_333->parent /*parent*/ != nullptr )
    {
        builtin_fwrite(__doc_file_rename_at_633_331,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_133, cast<char * const >::from(((char *) " : ")), cast<das::string const  &>::from(__value_rename_at_633_333->parent /*parent*/->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    builtin_fwrite(__doc_file_rename_at_633_331,((char *) "\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    TArray<TArray<char *>> __tab_rename_at_641_334; das_zero(__tab_rename_at_641_334);
    {
        bool __need_loop_642 = true;
        // fld: ast::FieldDeclaration const&
        das_iterator<das::vector<Structure::FieldDeclaration> const > __fld_iterator(__value_rename_at_633_333->fields /*fields*/);
        Structure::FieldDeclaration const  * __fld_rename_at_642_335;
        __need_loop_642 = __fld_iterator.first(__context__,(__fld_rename_at_642_335)) && __need_loop_642;
        for ( ; __need_loop_642 ; __need_loop_642 = __fld_iterator.next(__context__,(__fld_rename_at_642_335)) )
        {
            TArray<char *> __line_rename_at_643_336; das_zero(__line_rename_at_643_336);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_643_336),((char * const )(to_das_string((*__fld_rename_at_642_335).name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_643_336),describe_type_a9bac10014c763b2(__context__,(*__fld_rename_at_642_335).type /*_type*/));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_641_334),das_arg<TArray<char *>>::pass(__line_rename_at_643_336));
        }
        __fld_iterator.close(__context__,(__fld_rename_at_642_335));
    };
    builtin_fwrite(__doc_file_rename_at_633_331,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_134, cast<das::string const  &>::from(__value_rename_at_633_333->name /*name*/), cast<char * const >::from(((char *) " fields are\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_633_331,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_641_334),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_633_331,topic_9659659d94ddb29a(__context__,((char *) "structure"),__mod_rename_at_633_332,((char * const )(to_das_string(__value_rename_at_633_333->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void document_structures_5fb20d0dee04ba46 ( Context * __context__, FILE const  * const  __doc_file_rename_at_653_337, Module * const  __mod_rename_at_653_338 ) { das_stack_prologue __prologue(__context__,176,"document_structures " DAS_FILE_LINE);
{
    bool __any_rename_at_654_339 = false;
    for_each_structure(__mod_rename_at_653_338,das_make_block<void,smart_ptr_raw<Structure>>(__context__,96,0,&__func_info__438466bc7bed9910,[&](smart_ptr_raw<Structure> __value_rename_at_655_340) -> void{
        if ( (das_get_bitfield(__value_rename_at_655_340->flags /*flags*/,1u << 0) || das_get_bitfield(__value_rename_at_655_340->flags /*flags*/,1u << 4)) || das_get_bitfield(__value_rename_at_655_340->flags /*flags*/,1u << 7) )
        {
            return ;
        };
        das_copy(__any_rename_at_654_339,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_654_339 )
    {
        return ;
    } else {
        for_each_structure(__mod_rename_at_653_338,das_make_block<void,smart_ptr_raw<Structure>>(__context__,160,0,&__func_info__438466bc7bed9910,[&](smart_ptr_raw<Structure> __value_rename_at_665_341) -> void{
            if ( (das_get_bitfield(__value_rename_at_665_341->flags /*flags*/,1u << 0) || das_get_bitfield(__value_rename_at_665_341->flags /*flags*/,1u << 4)) || das_get_bitfield(__value_rename_at_665_341->flags /*flags*/,1u << 7) )
            {
                return ;
            };
            document_structure_96bd65f4eda31cf3(__context__,__doc_file_rename_at_653_337,__mod_rename_at_653_338,__value_rename_at_665_341);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_structure_annotations_a53005d9c9a5617 ( Context * __context__, FILE const  * const  __doc_file_rename_at_742_342, Module * const  __mod_rename_at_742_343 ) { das_stack_prologue __prologue(__context__,176,"document_structure_annotations " DAS_FILE_LINE);
{
    bool __any_rename_at_743_344 = false;
    rtti_builtin_module_for_each_annotation(__mod_rename_at_742_343,das_make_block<void,Annotation const  &>(__context__,96,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_744_345) -> void{
        if ( !((__value_rename_at_744_345).rtti_isBasicStructureAnnotation()) )
        {
            return ;
        };
        das_copy(__any_rename_at_743_344,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_743_344 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_742_342,((char *) "++++++++++++++++++\nHandled structures\n++++++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        rtti_builtin_module_for_each_annotation(__mod_rename_at_742_343,das_make_block<void,Annotation const  &>(__context__,160,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_754_346) -> void{
            if ( !((__value_rename_at_754_346).rtti_isBasicStructureAnnotation()) )
            {
                return ;
            };
            _FuncrstTickdocument_structure_annotationTick2734840712723762798_45f2db6d6c9372ba(__context__,__doc_file_rename_at_742_342,__mod_rename_at_742_343,__value_rename_at_754_346);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_function_annotations_5344d92e9ea569ad ( Context * __context__, FILE const  * const  __doc_file_rename_at_769_347, Module * const  __mod_rename_at_769_348 ) { das_stack_prologue __prologue(__context__,176,"document_function_annotations " DAS_FILE_LINE);
{
    bool __any_rename_at_770_349 = false;
    rtti_builtin_module_for_each_annotation(__mod_rename_at_769_348,das_make_block<void,Annotation const  &>(__context__,96,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_771_350) -> void{
        if ( !((__value_rename_at_771_350).rtti_isFunctionAnnotation()) )
        {
            return ;
        };
        das_copy(__any_rename_at_770_349,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_770_349 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_769_347,((char *) "++++++++++++++++++++\nFunction annotations\n++++++++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        rtti_builtin_module_for_each_annotation(__mod_rename_at_769_348,das_make_block<void,Annotation const  &>(__context__,160,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_781_351) -> void{
            if ( !((__value_rename_at_781_351).rtti_isFunctionAnnotation()) )
            {
                return ;
            };
            _FuncrstTickdocument_function_annotationTick9878220666429632644_62156d6269c1aee6(__context__,__doc_file_rename_at_769_347,__mod_rename_at_769_348,__value_rename_at_781_351);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_annotations_128851a794d46d1a ( Context * __context__, FILE const  * const  __doc_file_rename_at_796_352, Module * const  __mod_rename_at_796_353 ) { das_stack_prologue __prologue(__context__,176,"document_annotations " DAS_FILE_LINE);
{
    bool __any_rename_at_797_354 = false;
    rtti_builtin_module_for_each_annotation(__mod_rename_at_796_353,das_make_block<void,Annotation const  &>(__context__,96,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_798_355) -> void{
        if ( !(((__value_rename_at_798_355).rtti_isHandledTypeAnnotation())) || ((__value_rename_at_798_355).rtti_isBasicStructureAnnotation()) )
        {
            return ;
        };
        das_copy(__any_rename_at_797_354,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_797_354 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_796_352,((char *) "+++++++++++++\nHandled types\n+++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        rtti_builtin_module_for_each_annotation(__mod_rename_at_796_353,das_make_block<void,Annotation const  &>(__context__,160,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_808_356) -> void{
            if ( !(((__value_rename_at_808_356).rtti_isHandledTypeAnnotation())) || ((__value_rename_at_808_356).rtti_isBasicStructureAnnotation()) )
            {
                return ;
            };
            if ( builtin_string_startswith(((char * const )(to_das_string(__value_rename_at_808_356.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "dasvector`"),__context__) )
            {
                return ;
            };
            _FuncrstTickdocument_annotationTick7679268031359399902_8b47c8f555ccae04(__context__,__doc_file_rename_at_796_352,__mod_rename_at_796_353,__value_rename_at_808_356);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_structure_macros_7386ca2820215b3d ( Context * __context__, FILE const  * const  __doc_file_rename_at_826_357, Module * const  __mod_rename_at_826_358 ) { das_stack_prologue __prologue(__context__,176,"document_structure_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_827_359 = false;
    rtti_builtin_module_for_each_annotation(__mod_rename_at_826_358,das_make_block<void,Annotation const  &>(__context__,96,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_828_360) -> void{
        if ( (((__value_rename_at_828_360).rtti_isHandledTypeAnnotation()) || ((__value_rename_at_828_360).rtti_isBasicStructureAnnotation())) || ((__value_rename_at_828_360).rtti_isFunctionAnnotation()) )
        {
            return ;
        };
        das_copy(__any_rename_at_827_359,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_827_359 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_826_357,((char *) "++++++++++++++++\nStructure macros\n++++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        rtti_builtin_module_for_each_annotation(__mod_rename_at_826_358,das_make_block<void,Annotation const  &>(__context__,160,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_838_361) -> void{
            if ( (((__value_rename_at_838_361).rtti_isHandledTypeAnnotation()) || ((__value_rename_at_838_361).rtti_isBasicStructureAnnotation())) || ((__value_rename_at_838_361).rtti_isFunctionAnnotation()) )
            {
                return ;
            };
            _FuncrstTickdocument_structure_macroTick6088634580083749384_a8cf27101dd30a50(__context__,__doc_file_rename_at_826_357,__mod_rename_at_826_358,__value_rename_at_838_361);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_call_macro_e76fbe68d00e7362 ( Context * __context__, FILE const  * const  __doc_file_rename_at_846_362, Module * const  __mod_rename_at_846_363, char * const  __value_rename_at_846_364 )
{
    builtin_fwrite(__doc_file_rename_at_846_362,make_label_3368ba1f933c2731(__context__,_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c(__context__,__mod_rename_at_846_363,__value_rename_at_846_364)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_846_362,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_846_362,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_135, cast<char * const >::from(__value_rename_at_846_364), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_846_362,topic_9659659d94ddb29a(__context__,((char *) "function_annotation"),__mod_rename_at_846_363,__value_rename_at_846_364));
}

inline void document_call_macros_56da413fe6ccb104 ( Context * __context__, FILE const  * const  __doc_file_rename_at_853_365, Module * const  __mod_rename_at_853_366 ) { das_stack_prologue __prologue(__context__,176,"document_call_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_854_367 = false;
    for_each_call_macro(__mod_rename_at_853_366,das_make_block<void,char *>(__context__,96,0,&__func_info__e0afcd004fbe6de3,[&](char * __value_rename_at_855_368) -> void{
        das_copy(__any_rename_at_854_367,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_854_367 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_853_365,((char *) "+++++++++++\nCall macros\n+++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_call_macro(__mod_rename_at_853_366,das_make_block<void,char *>(__context__,160,0,&__func_info__e0afcd004fbe6de3,[&](char * __value_rename_at_862_369) -> void{
            document_call_macro_e76fbe68d00e7362(__context__,__doc_file_rename_at_853_365,__mod_rename_at_853_366,__value_rename_at_862_369);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_reader_macro_4006c26786c1908 ( Context * __context__, FILE const  * const  __doc_file_rename_at_867_370, Module * const  __mod_rename_at_867_371, char * const  __value_rename_at_867_372 )
{
    builtin_fwrite(__doc_file_rename_at_867_370,make_label_3368ba1f933c2731(__context__,_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c(__context__,__mod_rename_at_867_371,__value_rename_at_867_372)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_867_370,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_867_370,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_136, cast<char * const >::from(__value_rename_at_867_372), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_867_370,topic_9659659d94ddb29a(__context__,((char *) "reader_macro"),__mod_rename_at_867_371,__value_rename_at_867_372));
}

inline void document_reader_macros_585b57fffa625cd ( Context * __context__, FILE const  * const  __doc_file_rename_at_874_373, Module * const  __mod_rename_at_874_374 ) { das_stack_prologue __prologue(__context__,176,"document_reader_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_875_375 = false;
    for_each_reader_macro(__mod_rename_at_874_374,das_make_block<void,char *>(__context__,96,0,&__func_info__e0afcd004fbe6de3,[&](char * __value_rename_at_876_376) -> void{
        das_copy(__any_rename_at_875_375,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_875_375 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_874_373,((char *) "+++++++++++++\nReader macros\n+++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_reader_macro(__mod_rename_at_874_374,das_make_block<void,char *>(__context__,160,0,&__func_info__e0afcd004fbe6de3,[&](char * __value_rename_at_883_377) -> void{
            document_reader_macro_4006c26786c1908(__context__,__doc_file_rename_at_874_373,__mod_rename_at_874_374,__value_rename_at_883_377);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_variant_macro_84da1b9ee053cb60 ( Context * __context__, FILE const  * const  __doc_file_rename_at_888_378, Module * const  __mod_rename_at_888_379, char * const  __value_rename_at_888_380 )
{
    builtin_fwrite(__doc_file_rename_at_888_378,make_label_3368ba1f933c2731(__context__,_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c(__context__,__mod_rename_at_888_379,__value_rename_at_888_380)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_888_378,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_888_378,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_137, cast<char * const >::from(__value_rename_at_888_380), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_888_378,topic_9659659d94ddb29a(__context__,((char *) "variant_macro"),__mod_rename_at_888_379,__value_rename_at_888_380));
}

inline void document_variant_macros_eab4d095021756ef ( Context * __context__, FILE const  * const  __doc_file_rename_at_895_381, Module * const  __mod_rename_at_895_382 ) { das_stack_prologue __prologue(__context__,176,"document_variant_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_896_383 = false;
    for_each_variant_macro(__mod_rename_at_895_382,das_make_block<void,smart_ptr_raw<VariantMacro>>(__context__,96,0,&__func_info__84319ec9af82a642,[&](smart_ptr_raw<VariantMacro> __value_rename_at_897_384) -> void{
        das_copy(__any_rename_at_896_383,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_896_383 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_895_381,((char *) "++++++++++++++\nVariant macros\n++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_variant_macro(__mod_rename_at_895_382,das_make_block<void,smart_ptr_raw<VariantMacro>>(__context__,160,0,&__func_info__84319ec9af82a642,[&](smart_ptr_raw<VariantMacro> __value_rename_at_904_385) -> void{
            document_variant_macro_84da1b9ee053cb60(__context__,__doc_file_rename_at_895_381,__mod_rename_at_895_382,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_904_385->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_typeinfo_macro_66a7bf37ca0e4fcd ( Context * __context__, FILE const  * const  __doc_file_rename_at_909_386, Module * const  __mod_rename_at_909_387, char * const  __value_rename_at_909_388 )
{
    builtin_fwrite(__doc_file_rename_at_909_386,make_label_3368ba1f933c2731(__context__,_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c(__context__,__mod_rename_at_909_387,__value_rename_at_909_388)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_909_386,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_909_386,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_138, cast<char * const >::from(__value_rename_at_909_388), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_909_386,topic_9659659d94ddb29a(__context__,((char *) "typeinfo_macro"),__mod_rename_at_909_387,__value_rename_at_909_388));
}

inline void document_typeinfo_macros_b4b5371c027f780c ( Context * __context__, FILE const  * const  __doc_file_rename_at_916_389, Module * const  __mod_rename_at_916_390 ) { das_stack_prologue __prologue(__context__,176,"document_typeinfo_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_917_391 = false;
    for_each_typeinfo_macro(__mod_rename_at_916_390,das_make_block<void,smart_ptr_raw<TypeInfoMacro>>(__context__,96,0,&__func_info__5f3d7fba1c40074b,[&](smart_ptr_raw<TypeInfoMacro> __value_rename_at_918_392) -> void{
        das_copy(__any_rename_at_917_391,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_917_391 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_916_389,((char *) "+++++++++++++++\nTypeinfo macros\n+++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_typeinfo_macro(__mod_rename_at_916_390,das_make_block<void,smart_ptr_raw<TypeInfoMacro>>(__context__,160,0,&__func_info__5f3d7fba1c40074b,[&](smart_ptr_raw<TypeInfoMacro> __value_rename_at_925_393) -> void{
            document_typeinfo_macro_66a7bf37ca0e4fcd(__context__,__doc_file_rename_at_916_389,__mod_rename_at_916_390,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_925_393->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline bool function_needs_documenting_5ed98b6306abb3d7 ( Context * __context__, smart_ptr_raw<Function> const  __func_rename_at_930_394 )
{
    if ( das_get_bitfield(__func_rename_at_930_394->flags /*flags*/,1u << 21) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( das_get_bitfield(__func_rename_at_930_394->flags /*flags*/,1u << 27) )
        {
            return das_auto_cast<bool>::cast(false);
        } else {
            if ( das_get_bitfield(__func_rename_at_930_394->flags /*flags*/,1u << 22) )
            {
                return das_auto_cast<bool>::cast(false);
            } else {
                if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__func_rename_at_930_394->fromGeneric /*fromGeneric*/),das_auto_cast<void * const >::cast(nullptr)) )
                {
                    return das_auto_cast<bool>::cast(false);
                } else {
                    if ( das_get_bitfield(__func_rename_at_930_394->moreFlags /*moreFlags*/,1u << 12) )
                    {
                        return das_auto_cast<bool>::cast(false);
                    } else {
                        bool __isBuiltin_rename_at_946_395 = false;
                        peek_das_string_T(__func_rename_at_930_394->module /*_module*/->name /*name*/,[&](char * const  __name_rename_at_947_396) DAS_AOT_INLINE_LAMBDA -> void{
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_947_396),cast<char *>::from(((char *) "$")),*__context__,nullptr) )
                            {
                                das_copy(__isBuiltin_rename_at_946_395,true);
                            };
                        },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        bool __isOperator_rename_at_953_397 = false;
                        if ( __isBuiltin_rename_at_946_395 )
                        {
                            peek_das_string_T(__func_rename_at_930_394->name /*name*/,[&](char * const  __name_rename_at_955_398) DAS_AOT_INLINE_LAMBDA -> void{
                                int32_t __ch_rename_at_956_399 = ((int32_t)get_character_at(__name_rename_at_955_398,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
                                if ( (!(is_alpha(__ch_rename_at_956_399)) && !(is_number(__ch_rename_at_956_399))) && (__ch_rename_at_956_399 != 95) )
                                {
                                    das_copy(__isOperator_rename_at_953_397,true);
                                };
                            },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        };
                        if ( __isOperator_rename_at_953_397 )
                        {
                            return das_auto_cast<bool>::cast(false);
                        } else {
                            bool __isTypeConstructor_rename_at_966_400 = false;
                            if ( __isBuiltin_rename_at_946_395 )
                            {
                                peek_das_string_T(__func_rename_at_930_394->name /*name*/,[&](char * const  __name_rename_at_968_401) DAS_AOT_INLINE_LAMBDA -> void{
                                    if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__func_rename_at_930_394->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) && ((das_deref(__context__,__func_rename_at_930_394->result /*result*/)).isCtorType()) )
                                    {
                                        char * __tname_rename_at_970_402 = ((char *)(char *)(((char * const )(ast_das_to_string(__func_rename_at_930_394->result /*result*/->baseType /*baseType*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                        if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_968_401),cast<char *>::from(__tname_rename_at_970_402),*__context__,nullptr) )
                                        {
                                            das_copy(__isTypeConstructor_rename_at_966_400,true);
                                        };
                                    };
                                },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                            };
                            if ( __isTypeConstructor_rename_at_966_400 )
                            {
                                return das_auto_cast<bool>::cast(false);
                            } else {
                                bool __isBuiltinName_rename_at_981_403 = false;
                                peek_das_string_T(__func_rename_at_930_394->name /*name*/,[&](char * const  __name_rename_at_982_404) DAS_AOT_INLINE_LAMBDA -> void{
                                    if ( builtin_string_startswith(__name_rename_at_982_404,((char *) "_builtin"),__context__) || builtin_string_startswith(__name_rename_at_982_404,((char *) "__builtin"),__context__) )
                                    {
                                        das_copy(__isBuiltinName_rename_at_981_403,true);
                                    };
                                },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                                return das_auto_cast<bool>::cast(__isBuiltinName_rename_at_981_403 ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true));
                            };
                        };
                    };
                };
            };
        };
    };
}

inline void document_warning_cb981aa521e01a92 ( Context * __context__, FILE const  * const  __doc_file_rename_at_993_405, char * const  __issue_rename_at_993_406 )
{
    builtin_fwrite(__doc_file_rename_at_993_405,((char *) ".. warning:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_993_405,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_139, cast<char * const >::from(((char *) "\n  ")), cast<char * const >::from(__issue_rename_at_993_406), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * function_name_ee62641d633d27c8 ( Context * __context__, char * const  __name_rename_at_998_407 )
{
    if ( !builtin_empty(__name_rename_at_998_407) )
    {
        int32_t __ch_rename_at_1000_408 = ((int32_t)get_character_at(__name_rename_at_998_407,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        if ( (__ch_rename_at_1000_408 != 95) && !(is_alpha(__ch_rename_at_1000_408)) )
        {
            return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_140, cast<char * const >::from(((char *) "operator ")), cast<char * const >::from(__name_rename_at_998_407))));
        };
    };
    return das_auto_cast<char *>::cast(__name_rename_at_998_407);
}

inline void document_functions_c930228364733372 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1027_409, Module * const  __mod_rename_at_1027_410, TArray<rst::DocGroup> &  __groups_rename_at_1027_411 ) { das_stack_prologue __prologue(__context__,384,"document_functions " DAS_FILE_LINE);
{
    TTable<Function *,bool> __tab_rename_at_1028_412; das_zero(__tab_rename_at_1028_412);
    {
        bool __need_loop_1029 = true;
        // grp: rst::DocGroup&
        das_iterator<TArray<rst::DocGroup>> __grp_iterator(__groups_rename_at_1027_411);
        rst::DocGroup * __grp_rename_at_1029_413;
        __need_loop_1029 = __grp_iterator.first(__context__,(__grp_rename_at_1029_413)) && __need_loop_1029;
        for ( ; __need_loop_1029 ; __need_loop_1029 = __grp_iterator.next(__context__,(__grp_rename_at_1029_413)) )
        {
            if ( builtin_array_size(das_arg<TArray<Function *>>::pass((*__grp_rename_at_1029_413).func)) != 0 )
            {
                if ( true && (*__grp_rename_at_1029_413).hidden )
                {
                    {
                        bool __need_loop_1032 = true;
                        // func: ast::Function?&
                        das_iterator<TArray<Function *>> __func_iterator((*__grp_rename_at_1029_413).func);
                        Function * * __func_rename_at_1032_414;
                        __need_loop_1032 = __func_iterator.first(__context__,(__func_rename_at_1032_414)) && __need_loop_1032;
                        for ( ; __need_loop_1032 ; __need_loop_1032 = __func_iterator.next(__context__,(__func_rename_at_1032_414)) )
                        {
                            _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_412),(*__func_rename_at_1032_414),true);
                        }
                        __func_iterator.close(__context__,(__func_rename_at_1032_414));
                    };
                } else {
                    builtin_fwrite(__doc_file_rename_at_1027_409,make_group_8b304b06bc5b7645(__context__,(*__grp_rename_at_1029_413).name,((char *) "+")),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    if ( (*__grp_rename_at_1029_413).hidden )
                    {
                        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1027_409,((char *) "This group of functions is hidden. It will not be in the final documentation."));
                    };
                    {
                        bool __need_loop_1040 = true;
                        // func: ast::Function?&
                        das_iterator<TArray<Function *>> __func_iterator((*__grp_rename_at_1029_413).func);
                        Function * * __func_rename_at_1040_415;
                        __need_loop_1040 = __func_iterator.first(__context__,(__func_rename_at_1040_415)) && __need_loop_1040;
                        for ( ; __need_loop_1040 ; __need_loop_1040 = __func_iterator.next(__context__,(__func_rename_at_1040_415)) )
                        {
                            if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_4e6333822225d431(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_412),(*__func_rename_at_1040_415)) )
                            {
                                char * __descr_rename_at_1042_416 = ((char *)(char *)(_Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f(__context__,(*__func_rename_at_1040_415))));
                                char * __labn_rename_at_1043_417 = (char *)(_FuncrstTickfunction_label_nameTick4951524166161443384_4422c54db32493f8(__context__,(*__func_rename_at_1040_415)));
                                builtin_fwrite(__doc_file_rename_at_1027_409,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_141, cast<char * const >::from(((char *) "  * ")), cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,__labn_rename_at_1043_417,__descr_rename_at_1042_416)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                            };
                        }
                        __func_iterator.close(__context__,(__func_rename_at_1040_415));
                    };
                    builtin_fwrite(__doc_file_rename_at_1027_409,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    {
                        bool __need_loop_1048 = true;
                        // func: ast::Function?&
                        das_iterator<TArray<Function *>> __func_iterator((*__grp_rename_at_1029_413).func);
                        Function * * __func_rename_at_1048_418;
                        __need_loop_1048 = __func_iterator.first(__context__,(__func_rename_at_1048_418)) && __need_loop_1048;
                        for ( ; __need_loop_1048 ; __need_loop_1048 = __func_iterator.next(__context__,(__func_rename_at_1048_418)) )
                        {
                            if ( _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_412),(*__func_rename_at_1048_418)) )
                            {
                                continue;
                            } else {
                                _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_412),(*__func_rename_at_1048_418),true);
                                _FuncrstTickdocument_functionTick8829733267459955112_767667cb9ac814a7(__context__,__doc_file_rename_at_1027_409,__mod_rename_at_1027_410,(*__func_rename_at_1048_418));
                            };
                        }
                        __func_iterator.close(__context__,(__func_rename_at_1048_418));
                    };
                };
            };
        }
        __grp_iterator.close(__context__,(__grp_rename_at_1029_413));
    };
    bool __first_function_rename_at_1058_419 = true;
    forEachFunction(__mod_rename_at_1027_410,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,272,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_1059_420) -> void{
        if ( !function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1059_420) )
        {
            return ;
        };
        Function * __pfunc_rename_at_1063_421 = _FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615(__context__,__func_rename_at_1059_420);
        if ( _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_412),__pfunc_rename_at_1063_421) )
        {
            return ;
        };
        _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_412),__pfunc_rename_at_1063_421,true);
        if ( __first_function_rename_at_1058_419 )
        {
            builtin_fwrite(__doc_file_rename_at_1027_409,((char *) "+++++++++++++\nUncategorized\n+++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            das_copy(__first_function_rename_at_1058_419,false);
        };
        _FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00(__context__,__doc_file_rename_at_1027_409,__mod_rename_at_1027_410,__func_rename_at_1059_420);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    for_each_generic(__mod_rename_at_1027_410,das_make_block<void,smart_ptr_raw<Function>>(__context__,352,0,&__func_info__806bd08fbfbd09f4,[&](smart_ptr_raw<Function> __func_rename_at_1074_422) -> void{
        if ( !function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1074_422) )
        {
            return ;
        };
        Function * __pfunc_rename_at_1078_423 = _FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615(__context__,__func_rename_at_1074_422);
        if ( _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_412),__pfunc_rename_at_1078_423) )
        {
            return ;
        };
        _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_412),__pfunc_rename_at_1078_423,true);
        if ( __first_function_rename_at_1058_419 )
        {
            builtin_fwrite(__doc_file_rename_at_1027_409,((char *) "+++++++++++++\nUncategorized\n+++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            das_copy(__first_function_rename_at_1058_419,false);
        };
        _FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00(__context__,__doc_file_rename_at_1027_409,__mod_rename_at_1027_410,__func_rename_at_1074_422);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline rst::DocGroup debug_group_70ce9e37d0ceaf7a ( Context * __context__, rst::DocGroup &  __group_rename_at_1098_424 )
{
    toLog(10000,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_142, cast<char * const >::from(((char *) "Group ")), cast<char *>::from(__group_rename_at_1098_424.name), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_1100 = true;
        // func: ast::Function?&
        das_iterator<TArray<Function *>> __func_iterator(__group_rename_at_1098_424.func);
        Function * * __func_rename_at_1100_425;
        __need_loop_1100 = __func_iterator.first(__context__,(__func_rename_at_1100_425)) && __need_loop_1100;
        for ( ; __need_loop_1100 ; __need_loop_1100 = __func_iterator.next(__context__,(__func_rename_at_1100_425)) )
        {
            toLog(10000,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_143, cast<char * const >::from(((char *) "\t")), cast<char * const >::from(_Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f(__context__,(*__func_rename_at_1100_425))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }
        __func_iterator.close(__context__,(__func_rename_at_1100_425));
    };
    return /* <- */ das_auto_cast_move<rst::DocGroup>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_81325f48a6efc9e9(__context__,das_arg<rst::DocGroup>::pass(__group_rename_at_1098_424)));
}

inline void append_to_group_by_regex_fa03c8e61a02da42 ( Context * __context__, rst::DocGroup &  __group_rename_at_1106_426, Module * const  __mod_rename_at_1106_427, regex::Regex &  __reg_rename_at_1106_428 ) { das_stack_prologue __prologue(__context__,160,"append_to_group_by_regex " DAS_FILE_LINE);
{
    forEachFunction(__mod_rename_at_1106_427,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_1107_429) -> void{
        if ( function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1107_429) )
        {
            if ( das_invoke_function<int32_t>::invoke<regex::Regex &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_match S<regex::Regex> Cs Ci*/ 0x297b408d218ed2d)),das_arg<regex::Regex>::pass(__reg_rename_at_1106_428),((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_1107_429->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),0) != -1 )
            {
                _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50(__context__,das_arg<TArray<Function *>>::pass(__group_rename_at_1106_426.func),_FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615(__context__,__func_rename_at_1107_429));
            };
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    for_each_generic(__mod_rename_at_1106_427,das_make_block<void,smart_ptr_raw<Function>>(__context__,144,0,&__func_info__806bd08fbfbd09f4,[&](smart_ptr_raw<Function> __func_rename_at_1114_430) -> void{
        if ( function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1114_430) )
        {
            if ( das_invoke_function<int32_t>::invoke<regex::Regex &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_match S<regex::Regex> Cs Ci*/ 0x297b408d218ed2d)),das_arg<regex::Regex>::pass(__reg_rename_at_1106_428),((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_1114_430->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),0) != -1 )
            {
                _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50(__context__,das_arg<TArray<Function *>>::pass(__group_rename_at_1106_426.func),_FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615(__context__,__func_rename_at_1114_430));
            };
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline rst::DocGroup group_by_regex_3d42ba74b8c888e3 ( Context * __context__, char * const  __name_rename_at_1123_431, Module * const  __mod_rename_at_1123_432, regex::Regex &  __reg_rename_at_1123_433 )
{
    rst::DocGroup __group_rename_at_1126_434; das_zero(__group_rename_at_1126_434); das_move(__group_rename_at_1126_434, (([&]() -> rst::DocGroup {
        rst::DocGroup __mks_1126;
        das_zero(__mks_1126);
        das_copy((__mks_1126.name),(__name_rename_at_1123_431));
        das_copy((__mks_1126.hidden),(false));
        return __mks_1126;
    })()));
    append_to_group_by_regex_fa03c8e61a02da42(__context__,das_arg<rst::DocGroup>::pass(__group_rename_at_1126_434),__mod_rename_at_1123_432,das_arg<regex::Regex>::pass(__reg_rename_at_1123_433));
    return /* <- */ das_auto_cast_move<rst::DocGroup>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_81325f48a6efc9e9(__context__,das_arg<rst::DocGroup>::pass(__group_rename_at_1126_434)));
}

inline rst::DocGroup hide_group_72124d6e40656992 ( Context * __context__, rst::DocGroup &  __group_rename_at_1131_435 )
{
    das_copy(__group_rename_at_1131_435.hidden,true);
    return /* <- */ das_auto_cast_move<rst::DocGroup>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_81325f48a6efc9e9(__context__,das_arg<rst::DocGroup>::pass(__group_rename_at_1131_435)));
}

inline void generate_topic_stub_2777760f2b77d9f0 ( Context * __context__, TTable<char *,bool> &  __tab_rename_at_1137_436, FILE const  * const  __doc_file_rename_at_1137_437, char * const  __topic_rename_at_1137_438 )
{
    if ( !_FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1137_436),__topic_rename_at_1137_438) )
    {
        _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1137_436),__topic_rename_at_1137_438,true);
        builtin_fwrite(__doc_file_rename_at_1137_437,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_144, cast<char * const >::from(((char *) ".. ")), cast<char * const >::from(__topic_rename_at_1137_438), cast<char * const >::from(((char *) " replace:: to be documented in ")), cast<char * const >::from(__topic_rename_at_1137_438), cast<char * const >::from(((char *) ".rst\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void generate_substitute_stub_34b873e38788090 ( Context * __context__, Module * const  __mod_rename_at_1144_439, char * const  __substname_rename_at_1144_440 ) { das_stack_prologue __prologue(__context__,864,"generate_substitute_stub " DAS_FILE_LINE);
{
    FStat __st_rename_at_1145_441; das_zero(__st_rename_at_1145_441);
    if ( builtin_stat(__substname_rename_at_1144_440,das_arg<FStat>::pass(__st_rename_at_1145_441)) )
    {
        return ;
    } else {
        _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,__substname_rename_at_1144_440,((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,240,0,&__func_info__6edfc1622f67255a,[&](FILE const  * const  __stub_file_rename_at_1149_442) -> void{
            if ( __stub_file_rename_at_1149_442 != nullptr )
            {
                TTable<char *,bool> __tab_rename_at_1151_443; das_zero(__tab_rename_at_1151_443);
                for_each_typedef(__mod_rename_at_1144_439,das_make_block<void,char *,smart_ptr_raw<TypeDecl>>(__context__,352,0,&__func_info__aa03b7bcb8f14157,[&](char * __name_rename_at_1153_444, smart_ptr_raw<TypeDecl> __value_rename_at_1153_445) -> void{
                    generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "typedef"),__mod_rename_at_1144_439,__name_rename_at_1153_444));
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                for_each_enumeration(__mod_rename_at_1144_439,das_make_block<void,smart_ptr_raw<Enumeration>>(__context__,416,0,&__func_info__9beb038de389c5b0,[&](smart_ptr_raw<Enumeration> __value_rename_at_1157_446) -> void{
                    generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "enumeration"),__mod_rename_at_1144_439,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_1157_446->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                for_each_structure(__mod_rename_at_1144_439,das_make_block<void,smart_ptr_raw<Structure>>(__context__,480,0,&__func_info__438466bc7bed9910,[&](smart_ptr_raw<Structure> __value_rename_at_1161_447) -> void{
                    if ( das_get_bitfield(__value_rename_at_1161_447->flags /*flags*/,1u << 0) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "class"),__mod_rename_at_1144_439,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_1161_447->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                        {
                            bool __need_loop_1165 = true;
                            // fld: ast::FieldDeclaration&
                            das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__value_rename_at_1161_447->fields /*fields*/);
                            Structure::FieldDeclaration * __fld_rename_at_1165_448;
                            __need_loop_1165 = __fld_iterator.first(__context__,(__fld_rename_at_1165_448)) && __need_loop_1165;
                            for ( ; __need_loop_1165 ; __need_loop_1165 = __fld_iterator.next(__context__,(__fld_rename_at_1165_448)) )
                            {
                                if ( ((das_invoke_function<bool>::invoke<smart_ptr_raw<Structure> const ,smart_ptr_raw<TypeDecl> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::is_class_method CY<StructurePtr>1<H<ast::Structure>>?M CY<TypeDeclPtr>1<H<ast::TypeDecl>>?M*/ 0xa1c63892e7a43312)),__value_rename_at_1161_447,(*__fld_rename_at_1165_448).type /*_type*/) && !(das_get_bitfield((*__fld_rename_at_1165_448).flags /*flags*/,1u << 3))) && !(das_get_bitfield((*__fld_rename_at_1165_448).flags /*flags*/,1u << 1))) && das_get_bitfield((*__fld_rename_at_1165_448).flags /*flags*/,1u << 8) )
                                {
                                    generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "method"),__mod_rename_at_1144_439,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_145, cast<das::string &>::from(__value_rename_at_1161_447->name /*name*/), cast<char * const >::from(((char *) ".")), cast<das::string &>::from((*__fld_rename_at_1165_448).name /*name*/)))));
                                };
                            }
                            __fld_iterator.close(__context__,(__fld_rename_at_1165_448));
                        };
                    } else if ( !das_get_bitfield(__value_rename_at_1161_447->flags /*flags*/,1u << 4) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "structure"),__mod_rename_at_1144_439,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_1161_447->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                forEachFunction(__mod_rename_at_1144_439,nullptr,das_make_block<void,smart_ptr_raw<Function>>(__context__,656,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr_raw<Function> __func_rename_at_1175_449) -> void{
                    if ( function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1175_449) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "function"),__mod_rename_at_1144_439,das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_146, cast<char * const >::from(_FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b(__context__,__func_rename_at_1175_449))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                for_each_generic(__mod_rename_at_1144_439,das_make_block<void,smart_ptr_raw<Function>>(__context__,720,0,&__func_info__806bd08fbfbd09f4,[&](smart_ptr_raw<Function> __func_rename_at_1180_450) -> void{
                    if ( function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1180_450) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "function"),__mod_rename_at_1144_439,das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_147, cast<char * const >::from(_FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b(__context__,__func_rename_at_1180_450))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                rtti_builtin_module_for_each_annotation(__mod_rename_at_1144_439,das_make_block<void,Annotation const  &>(__context__,784,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_1186_451) -> void{
                    if ( ((__value_rename_at_1186_451).rtti_isBasicStructureAnnotation()) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "structure_annotation"),__mod_rename_at_1144_439,((char * const )(to_das_string(__value_rename_at_1186_451.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                    } else if ( ((__value_rename_at_1186_451).rtti_isHandledTypeAnnotation()) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "any_annotation"),__mod_rename_at_1144_439,((char * const )(to_das_string(__value_rename_at_1186_451.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                for_each_global(__mod_rename_at_1144_439,das_make_block<void,smart_ptr_raw<Variable>>(__context__,848,0,&__func_info__2701c05c12e53ee0,[&](smart_ptr_raw<Variable> __value_rename_at_1194_452) -> void{
                    if ( is_global_constant_320cc2da3d8764f7(__context__,__value_rename_at_1194_452) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_443),__stub_file_rename_at_1149_442,topic_9659659d94ddb29a(__context__,((char *) "variable"),__mod_rename_at_1144_439,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_1194_452->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            } else {
                builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_148, cast<char * const >::from(((char *) "can't generate stub at ")), cast<char * const >::from(__substname_rename_at_1144_440), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }));
    };
}}

inline char * getDetailIncludePath_fb77bda786a398b2 ( Context * __context__, char * const  __fname_rename_at_1205_453, char * const  __substname_rename_at_1205_454 )
{
    Sequence DAS_COMMENT((char * *)) _temp_make_local_1216_35_160; _temp_make_local_1216_35_160;
    TArray<char *> __fnp_rename_at_1206_455; das_zero(__fnp_rename_at_1206_455); das_move(__fnp_rename_at_1206_455, das_invoke_function<TArray<char *>>::invoke_cmres<char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::split_by_chars CIs CIs*/ 0xaf8429a94caa79c6)),__fname_rename_at_1205_453,((char *) "\\/")));
    TArray<char *> __snp_rename_at_1207_456; das_zero(__snp_rename_at_1207_456); das_move(__snp_rename_at_1207_456, das_invoke_function<TArray<char *>>::invoke_cmres<char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::split_by_chars CIs CIs*/ 0xaf8429a94caa79c6)),__substname_rename_at_1205_454,((char *) "\\/")));
    int32_t __index_rename_at_1208_457 = 0;
    {
        bool __need_loop_1209 = true;
        // f: string&
        das_iterator<TArray<char *>> __f_iterator(__fnp_rename_at_1206_455);
        char * * __f_rename_at_1209_460;
        __need_loop_1209 = __f_iterator.first(__context__,(__f_rename_at_1209_460)) && __need_loop_1209;
        // s: string&
        das_iterator<TArray<char *>> __s_iterator(__snp_rename_at_1207_456);
        char * * __s_rename_at_1209_461;
        __need_loop_1209 = __s_iterator.first(__context__,(__s_rename_at_1209_461)) && __need_loop_1209;
        for ( ; __need_loop_1209 ; __need_loop_1209 = __f_iterator.next(__context__,(__f_rename_at_1209_460)) && __s_iterator.next(__context__,(__s_rename_at_1209_461)) )
        {
            if ( SimPolicy<char *>::NotEqu(cast<char *>::from((*__f_rename_at_1209_460)),cast<char *>::from((*__s_rename_at_1209_461)),*__context__,nullptr) )
            {
                break;
            } else {
                ++__index_rename_at_1208_457;
            };
        }
        __f_iterator.close(__context__,(__f_rename_at_1209_460));
        __s_iterator.close(__context__,(__s_rename_at_1209_461));
    };
    char * __ipath_rename_at_1216_462 = (char *)(_Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9(__context__,das_arg<Sequence DAS_COMMENT((char *))>::pass((_temp_make_local_1216_35_160 = (_FuncbuiltinTickeach_refTick16137060296763333387_f3ba366f8c86460e(__context__,das_ascend<bool,rst::_lambda_rst_1216_1>::make(__context__,&__type_info__8e26ff93c3acecc5,(([&]() -> rst::_lambda_rst_1216_1 {
        rst::_lambda_rst_1216_1 __mks_1216;
        das_zero(__mks_1216);
        das_copy((__mks_1216.__lambda),(Func(__context__->fnByMangledName(/*@rst::_lambda_rst_1216_1`function XS<rst::_lambda_rst_1216_1> &1<s>?*/ 0x2072edc7d4124152))));
        das_copy((__mks_1216.__finalize),(Func(__context__->fnByMangledName(/*@rst::_lambda_rst_1216_1`finalizer X1<S<rst::_lambda_rst_1216_1>>?*/ 0x6e343e2e320192e3))));
        das_copy((__mks_1216.index),(__index_rename_at_1208_457));
        das_move((__mks_1216.snp),(__snp_rename_at_1207_456));
        return __mks_1216;
    })())))))),((char *) "/")));
    return das_auto_cast<char *>::cast(__ipath_rename_at_1216_462);
}

inline void document_38c26f0bb9ed7a3e ( Context * __context__, char * const  __name_rename_at_1221_463, Module * const  __mod_rename_at_1221_464, char * const  __fname_rename_at_1221_465, char * const  __substname_rename_at_1221_466, TArray<rst::DocGroup> &  __groups_rename_at_1221_467 )
{
    toLog(20000,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_149, cast<char * const >::from(((char *) "Documenting ")), cast<char * const >::from(__name_rename_at_1221_463), cast<char * const >::from(((char *) " into ")), cast<char * const >::from(__fname_rename_at_1221_465), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __mod_rename_at_1221_464 == nullptr )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_150, cast<char * const >::from(((char *) "missing module ")), cast<char * const >::from(__name_rename_at_1221_463))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    FILE const  * __doc_file_rename_at_1231_468 = builtin_fopen(__fname_rename_at_1221_465,((char *) "wb"));
    if ( __doc_file_rename_at_1231_468 == nullptr )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_151, cast<char * const >::from(((char *) "can't open ")), cast<char * const >::from(__fname_rename_at_1221_465))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    builtin_fwrite(__doc_file_rename_at_1231_468,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    char * __mod_name_rename_at_1236_469 = (char *)(((char * const )(to_das_string(__mod_rename_at_1221_464->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__mod_name_rename_at_1236_469),cast<char *>::from(((char *) "$")),*__context__,nullptr) )
    {
        das_copy(__mod_name_rename_at_1236_469,((char *) "_builtin"));
    };
    builtin_fwrite(__doc_file_rename_at_1231_468,make_header_efad6667e51b3e08(__context__,__name_rename_at_1221_463,__mod_name_rename_at_1236_469),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( builtin_string_length(__substname_rename_at_1221_466,__context__) != 0 )
    {
        builtin_fwrite(__doc_file_rename_at_1231_468,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_152, cast<char * const >::from(((char *) ".. include:: ")), cast<char * const >::from(getDetailIncludePath_fb77bda786a398b2(__context__,__fname_rename_at_1221_465,__substname_rename_at_1221_466)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        generate_substitute_stub_34b873e38788090(__context__,__mod_rename_at_1221_464,__substname_rename_at_1221_466);
    };
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_1231_468,topic_b97c7facf4ccb6a9(__context__,__mod_rename_at_1221_464));
    document_typedefs_6ca650b4c8738629(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_global_constants_24362fe0342586e0(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_enumerations_c255f6c4885d0cb5(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_structures_5fb20d0dee04ba46(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_structure_annotations_a53005d9c9a5617(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_function_annotations_5344d92e9ea569ad(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_call_macros_56da413fe6ccb104(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_reader_macros_585b57fffa625cd(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_variant_macros_eab4d095021756ef(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_typeinfo_macros_b4b5371c027f780c(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_annotations_128851a794d46d1a(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_structure_macros_7386ca2820215b3d(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_classes_1fc51dae5246eec0(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464);
    document_functions_c930228364733372(__context__,__doc_file_rename_at_1231_468,__mod_rename_at_1221_464,das_arg<TArray<rst::DocGroup>>::pass(__groups_rename_at_1221_467));
    builtin_fwrite(__doc_file_rename_at_1231_468,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fclose(__doc_file_rename_at_1231_468,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x393f34d50cd6136b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9>>();
    };
    aotLib[0x1a7ae42b3df9f4ba] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_75e0f8673d04729f>>();
    };
    aotLib[0x734e45a36711660] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e>>();
    };
    aotLib[0x8287ab4f1cdf8595] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_626ed670fc52be6d>>();
    };
    aotLib[0x380b7455b0a2f9f5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1>>();
    };
    aotLib[0x615f2ea0adab2f0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf>>();
    };
    aotLib[0x863c4893da830593] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick4044333107478717573_19fa17f56443c23c>>();
    };
    aotLib[0x913a8f2e9b96def6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d>>();
    };
    aotLib[0x1d18c10ccd1f35e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b>>();
    };
    aotLib[0x78948a08f0d98f51] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8>>();
    };
    aotLib[0xc6d010c722352347] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66>>();
    };
    aotLib[0xe473f497a092ecde] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9>>();
    };
    aotLib[0xc8c9111ff7052fb0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7>>();
    };
    aotLib[0x47ee94f223768edd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemptyTick15399874715904164783_60d4e412e550e258>>();
    };
    aotLib[0x39421012d4f6b3c0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickfunction_label_nameTick4951524166161443384_aafe521f2cb4dc50>>();
    };
    aotLib[0xa7c81ec8c5749cc0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_5bf95341c40e5b82>>();
    };
    aotLib[0xde6e54abcc6b70c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1>>();
    };
    aotLib[0x17c076c38e276651] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835>>();
    };
    aotLib[0x6bfd914a4a3cd5f6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f>>();
    };
    aotLib[0x15ac5ce5ae27e745] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50>>();
    };
    aotLib[0xa930d5b7ad34076e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeach_refTick16137060296763333387_f3ba366f8c86460e>>();
    };
    aotLib[0xb59d57e65ab89251] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickget_mangled_nameTick8436048561986127392_32c9175ac8d63b0f>>();
    };
    aotLib[0x680e3616c07f88fa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e>>();
    };
    aotLib[0xb0b31de6adf6ab5a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickfunction_file_nameTick11623859247758201540_f84a97f3b3efcea2>>();
    };
    aotLib[0x12491fe0302ad886] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_2c64031744b1c1f2>>();
    };
    aotLib[0x42da21885f868775] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickdocument_typedefTick2903024037696058038_48a0306c1d5a26e9>>();
    };
    aotLib[0x4b01355ea2d6ae0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickdocument_enumerationTick4211850574665690795_5251d76ee781cebe>>();
    };
    aotLib[0x8d68046e898e308b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickdocument_class_methodTick12589992380795293570_b41bc23f1e57e06>>();
    };
    aotLib[0xcb52f85a549cbb9a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickdocument_structure_annotationTick2734840712723762798_45f2db6d6c9372ba>>();
    };
    aotLib[0x2ecb0d6b1a1807b4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickdocument_function_annotationTick9878220666429632644_62156d6269c1aee6>>();
    };
    aotLib[0x499f06d37f5cdbfb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickdocument_annotationTick7679268031359399902_8b47c8f555ccae04>>();
    };
    aotLib[0x7359db810e0a9362] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickdocument_structure_macroTick6088634580083749384_a8cf27101dd30a50>>();
    };
    aotLib[0x611e4c1295d672ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615>>();
    };
    aotLib[0xa4b642a3a8289fc7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00>>();
    };
    aotLib[0x1be422f5f0bcf31f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_81325f48a6efc9e9>>();
    };
    aotLib[0x29b001e592d236a5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b>>();
    };
    aotLib[0xf812fb454f3c6150] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_rst_1216_1Tickfunction_deb6e09d1773cc6a>>();
    };
    aotLib[0xc5b67635f73c9aaf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_rst_1216_1Tickfinalizer_d8783cc009d376af>>();
    };
    aotLib[0xdb83ef5e07f54fc9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1>>();
    };
    aotLib[0xdb4c66fdb62e6c3d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_f1503a45e3ae58f3>>();
    };
    aotLib[0xa47525acff80991] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_d6dabe47fb1a720>>();
    };
    aotLib[0x398d67528cabe5c6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    aotLib[0x6701dc1abcd033d2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickhandle_label_nameTick13616998130381234091_7f23dc2205503d09>>();
    };
    aotLib[0xd5b298fc293f15ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51>>();
    };
    aotLib[0x7c2e30b4091b7513] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac>>();
    };
    aotLib[0x47ab915efca2326e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_649bcd7e51242beb>>();
    };
    aotLib[0x3c395992954dd60d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_d626258b591934af>>();
    };
    aotLib[0x70f9e95328fb7cf5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6>>();
    };
    aotLib[0xfbfe800d5700e08f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick842554968825501494_452cfc8955963a18>>();
    };
    aotLib[0x6e7a2bd0539ae9f5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6>>();
    };
    aotLib[0x611db07d61b26349] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_325c63fe2a174cf6>>();
    };
    aotLib[0x1e8ffe44dcbaaa69] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c>>();
    };
    aotLib[0x4d18bbfb8b74944a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a>>();
    };
    aotLib[0x9ce6017dea85c52e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_4e6333822225d431>>();
    };
    aotLib[0x8fd1c19f4baf74f7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f>>();
    };
    aotLib[0x617fe4ced76f5482] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickfunction_label_nameTick4951524166161443384_4422c54db32493f8>>();
    };
    aotLib[0x6a48f146592c6948] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72>>();
    };
    aotLib[0x8de39723336ffbf5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickdocument_functionTick8829733267459955112_767667cb9ac814a7>>();
    };
    aotLib[0xf7214cd218a61fdf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45>>();
    };
    aotLib[0xec1fda1174e8c717] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b>>();
    };
    aotLib[0x90571d6b86291165] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&alias_label_name_f0e3a6da4e226a3e>>();
    };
    aotLib[0x47063727e0446768] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&global_label_name_2b7e96918b37c00e>>();
    };
    aotLib[0xe4f1e61c41734632] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&global_label_name_b721f72143d35230>>();
    };
    aotLib[0x8adbc84bf1b50840] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&enum_label_name_a32575da767b3ecd>>();
    };
    aotLib[0x337720e26d36d7f5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&enum_label_name_768485a24af7de17>>();
    };
    aotLib[0x9ec1f091cdac453c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&struct_label_name_7b7faf8095586da6>>();
    };
    aotLib[0x92785b64e1a53e97] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&struct_label_name_6ee1d24e90557aeb>>();
    };
    aotLib[0xf0e8710aab95d4e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_type_short_56370d8993e381ba>>();
    };
    aotLib[0x4d3013b6e4e564bd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_type_short_847eafd8ddf6216>>();
    };
    aotLib[0x6e43b5e5f4f54c08] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_type_cf1a091488868462>>();
    };
    aotLib[0xd4983f60ed4583f0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&describe_type_a9bac10014c763b2>>();
    };
    aotLib[0x8b7847da4d370b82] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&make_domain_3e6edb06cf9af7bc>>();
    };
    aotLib[0xf09d70dabfadabf7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&make_label_3368ba1f933c2731>>();
    };
    aotLib[0xf05b4a0a1b199255] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&make_ref_c995159f43ed1b43>>();
    };
    aotLib[0xa4007e96ab229d2e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&make_group_8b304b06bc5b7645>>();
    };
    aotLib[0x4d2bdb499c09940e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&make_header_efad6667e51b3e08>>();
    };
    aotLib[0x71a6dd85e12a4e1b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&write_table_separator_2180d51ced094114>>();
    };
    aotLib[0x139b75dbca9873b9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&write_table_line_18bbb8245228f037>>();
    };
    aotLib[0xbd5d6d3969b4183a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&make_table_46d5ce8cf5a30e69>>();
    };
    aotLib[0x3701c1118ea995a2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&topic_9659659d94ddb29a>>();
    };
    aotLib[0x222ad1b8211a6a8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&topic_b97c7facf4ccb6a9>>();
    };
    aotLib[0xf589aea3422e4045] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_topic_c6e2614dff9f2d3e>>();
    };
    aotLib[0x50085a99d8fdfcdb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_bitfield_4340095c1075f95e>>();
    };
    aotLib[0xbe303b3ce156301] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_variant_c74548ebfca402e7>>();
    };
    aotLib[0xbe303b3ce156301] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_tuple_c74548ebfca402e7>>();
    };
    aotLib[0x296a535cbe2892b3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_typedefs_6ca650b4c8738629>>();
    };
    aotLib[0xb7d01a42ca3c9835] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_global_constant_378f3e1a10c345f3>>();
    };
    aotLib[0x1044e0d4b1e31a84] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_global_constant_320cc2da3d8764f7>>();
    };
    aotLib[0x4c660ac033acfc94] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_global_constants_24362fe0342586e0>>();
    };
    aotLib[0xc0b5a40a4992e444] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_enumerations_c255f6c4885d0cb5>>();
    };
    aotLib[0x50b89e7ee73e968f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_function_declaration_4c978976b1aaa270>>();
    };
    aotLib[0x2dbc5e90fa262740] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_function_arguments_e7469e75af4ed246>>();
    };
    aotLib[0x771511dec0de2047] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_classes_1fc51dae5246eec0>>();
    };
    aotLib[0xf3c9f2972bb5d9a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_structure_96bd65f4eda31cf3>>();
    };
    aotLib[0x8f31513dfda2e2f4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_structures_5fb20d0dee04ba46>>();
    };
    aotLib[0xbee53c52788bd560] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_structure_annotations_a53005d9c9a5617>>();
    };
    aotLib[0x86e8180286ceee80] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_function_annotations_5344d92e9ea569ad>>();
    };
    aotLib[0x78128c402cb8ae85] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_annotations_128851a794d46d1a>>();
    };
    aotLib[0x3713eeac0dfd25e4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_structure_macros_7386ca2820215b3d>>();
    };
    aotLib[0x65115697bb66c2b6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_call_macro_e76fbe68d00e7362>>();
    };
    aotLib[0xe145b4236e0a830b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_call_macros_56da413fe6ccb104>>();
    };
    aotLib[0x9aa9dd6dd19ca370] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_reader_macro_4006c26786c1908>>();
    };
    aotLib[0x8f59e0fcfcceb77] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_reader_macros_585b57fffa625cd>>();
    };
    aotLib[0x71d153c31909d9ec] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_variant_macro_84da1b9ee053cb60>>();
    };
    aotLib[0x8b34cba7048615c0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_variant_macros_eab4d095021756ef>>();
    };
    aotLib[0xe0eead8c010234a5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_typeinfo_macro_66a7bf37ca0e4fcd>>();
    };
    aotLib[0x44a03a7023e1ec22] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_typeinfo_macros_b4b5371c027f780c>>();
    };
    aotLib[0xcfe2328fbcdd4da3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&function_needs_documenting_5ed98b6306abb3d7>>();
    };
    aotLib[0xaa891474801b90df] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_warning_cb981aa521e01a92>>();
    };
    aotLib[0x7eccf71cb654622d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&function_name_ee62641d633d27c8>>();
    };
    aotLib[0x9b64c51684737c65] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_functions_c930228364733372>>();
    };
    aotLib[0x42bae5fa8bd63949] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&debug_group_70ce9e37d0ceaf7a>>();
    };
    aotLib[0x8755542dc41e8943] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&append_to_group_by_regex_fa03c8e61a02da42>>();
    };
    aotLib[0xcffc7c038d9f7f49] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&group_by_regex_3d42ba74b8c888e3>>();
    };
    aotLib[0x6d1f6494afc596f4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&hide_group_72124d6e40656992>>();
    };
    aotLib[0x92f6119b810e330f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&generate_topic_stub_2777760f2b77d9f0>>();
    };
    aotLib[0xae3f3d29d97152f7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&generate_substitute_stub_34b873e38788090>>();
    };
    aotLib[0xfe4a4edf9d1c0a9e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getDetailIncludePath_fb77bda786a398b2>>();
    };
    aotLib[0xa44d252d90792f20] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&document_38c26f0bb9ed7a3e>>();
    };
    // [[ init script ]]
    aotLib[0x185482f89db39a9] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_17275503207427487378
AotListBase impl_aot_rst(_anon_17275503207427487378::registerAotFunctions);
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
