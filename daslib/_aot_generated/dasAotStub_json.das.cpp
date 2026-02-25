#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/runtime_profile.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require strings_boost
 // require json

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
namespace _anon_8411258461380451932 {

namespace json { struct JsonValue; };
namespace json { struct TokenAt; };
namespace json { struct _lambda_json_180_1; };
// unused enumeration ConversionResult
namespace json {

struct JsonValue {
    TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> value;
};
static_assert(sizeof(JsonValue)==56,"structure size mismatch with DAS");
static_assert(offsetof(JsonValue,value)==0,"structure field offset mismatch with DAS");
}
namespace json {

struct TokenAt {
    TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> value;
    int32_t line;
    int32_t row;
};
static_assert(sizeof(TokenAt)==24,"structure size mismatch with DAS");
static_assert(offsetof(TokenAt,value)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(TokenAt,line)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(TokenAt,row)==20,"structure field offset mismatch with DAS");
}
namespace json {

struct _lambda_json_180_1 {
    Func DAS_COMMENT((bool,json::_lambda_json_180_1,json::TokenAt)) __lambda;
    Func DAS_COMMENT((void,json::_lambda_json_180_1 *)) __finalize;
    int32_t __yield;
    char * stext;
    Sequence DAS_COMMENT((int32_t)) tin;
    int32_t ahead;
    TArray<uint8_t> str;
    int32_t line;
    int32_t row;
    TArray<uint8_t> __hex_str_rename_at_216_36;
    bool __valid_hex_rename_at_217_36;
    bool ___loop_at_218_32_rename_at_218_32;
    int32_t ____rename_at_218_37;
    void * ___pvar_0_at_218_32_rename_at_218_32;
    bool __is_negative_rename_at_283_20;
    bool __anyNumbers_rename_at_285_20;
    bool __is_integer_rename_at_290_20;
    char * __num_rename_at_311_20;
    int64_t __ival_rename_at_318_24;
    char * __name_rename_at_332_20;
    Sequence DAS_COMMENT((int32_t)) ___source_0_at_218_32_rename_at_218_32;
};
static_assert(sizeof(_lambda_json_180_1)==160,"structure size mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__lambda)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__yield)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,stext)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,tin)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,ahead)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,str)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,line)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,row)==76,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__hex_str_rename_at_216_36)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__valid_hex_rename_at_217_36)==104,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,___loop_at_218_32_rename_at_218_32)==105,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,____rename_at_218_37)==108,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,___pvar_0_at_218_32_rename_at_218_32)==112,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__is_negative_rename_at_283_20)==120,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__anyNumbers_rename_at_285_20)==121,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__is_integer_rename_at_290_20)==122,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__num_rename_at_311_20)==128,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__ival_rename_at_318_24)==136,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,__name_rename_at_332_20)==144,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_json_180_1,___source_0_at_218_32_rename_at_218_32)==152,"structure field offset mismatch with DAS");
}
extern StructInfo __struct_info__69e2ef6fbeda51a0;
extern StructInfo __struct_info__6883e30bf583dbcb;
extern StructInfo __struct_info__6cd4a25fa22ecfe2;
extern TypeInfo __type_info__d96037642c78d59b;
extern TypeInfo __type_info__994066f681df7bcd;
extern TypeInfo __type_info__3fb2f51b6ebec3f2;
extern TypeInfo __type_info__cae9a62f8e83e959;
extern TypeInfo __type_info__e796e1d4af74511c;
extern TypeInfo __type_info__12283e04d98e7c73;
extern TypeInfo __type_info__8dbbd9827382f56f;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af87fe4c863f5252;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__8d8d8008262e16ec;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__790033b1854b444b;
extern TypeInfo __type_info__fa1aceb92701bc9a;
extern TypeInfo __type_info__3fc6f783e697c6b9;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63d94c8601e773;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__d922fe078cefab30;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;

TypeInfo * __type_info__a798e985ff4af814_arg_types_var_7629923981941428640[7] = { &__type_info__e796e1d4af74511c, &__type_info__994066f681df7bcd, &__type_info__af63ee4c86020b22, &__type_info__af63d94c8601e773, &__type_info__d922fe078cefab30, &__type_info__af63df4c8601f1a5, &__type_info__12283e04d98e7c73 };
const char * __type_info__a798e985ff4af814_arg_names_var_7629923981941428640[7] = { "_object", "_array", "_string", "_number", "_longint", "_bool", "_null" };
VarInfo __struct_info__69e2ef6fbeda51a0_field_0 =  { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__a798e985ff4af814_arg_types_var_7629923981941428640, __type_info__a798e985ff4af814_arg_names_var_7629923981941428640, 7, 0, nullptr, 57346, 56, UINT64_C(0xa798e985ff4af814), "value", 0, 1 };
VarInfo * __struct_info__69e2ef6fbeda51a0_fields[1] =  { &__struct_info__69e2ef6fbeda51a0_field_0 };
StructInfo __struct_info__69e2ef6fbeda51a0 = {"JsonValue", "json", 28, __struct_info__69e2ef6fbeda51a0_fields, 1, 56, UINT64_C(0x0), nullptr, UINT64_C(0x69e2ef6fbeda51a0), 0 };
TypeInfo * __type_info__fac8b32671fee393_arg_types_var_7531112642396806091[7] = { &__type_info__af63ee4c86020b22, &__type_info__af63d94c8601e773, &__type_info__d922fe078cefab30, &__type_info__af63df4c8601f1a5, &__type_info__12283e04d98e7c73, &__type_info__af63e44c8601fa24, &__type_info__af63ee4c86020b22 };
const char * __type_info__fac8b32671fee393_arg_names_var_7531112642396806091[7] = { "_string", "_number", "_longint", "_bool", "_null", "_symbol", "_error" };
VarInfo __struct_info__6883e30bf583dbcb_field_0 =  { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__fac8b32671fee393_arg_types_var_7531112642396806091, __type_info__fac8b32671fee393_arg_names_var_7531112642396806091, 7, 0, nullptr, 24582, 16, UINT64_C(0xfac8b32671fee393), "value", 0, 3 };
VarInfo __struct_info__6883e30bf583dbcb_field_1 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xa981854735365000), "line", 16, 0 };
VarInfo __struct_info__6883e30bf583dbcb_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xbd409e4745b09701), "row", 20, 0 };
VarInfo * __struct_info__6883e30bf583dbcb_fields[3] =  { &__struct_info__6883e30bf583dbcb_field_0, &__struct_info__6883e30bf583dbcb_field_1, &__struct_info__6883e30bf583dbcb_field_2 };
StructInfo __struct_info__6883e30bf583dbcb = {"TokenAt", "json", 12, __struct_info__6883e30bf583dbcb_fields, 3, 24, UINT64_C(0x0), nullptr, UINT64_C(0x6883e30bf583dbcb), 0 };
TypeInfo * __type_info__22a890337ea68cd0_arg_types_var_7842071382785445858[2] = { &__type_info__3fc6f783e697c6b9, &__type_info__fa1aceb92701bc9a };
const char * __type_info__22a890337ea68cd0_arg_names_var_7842071382785445858[2] = { "__this", "_yield_180" };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__22a890337ea68cd0_arg_types_var_7842071382785445858, __type_info__22a890337ea68cd0_arg_names_var_7842071382785445858, 2, 0, nullptr, 12, 8, UINT64_C(0x22a890337ea68cd0), "__lambda", 0, 0 };
TypeInfo * __type_info__f5bdd305f7c233ca_arg_types_var_7842071382785445858[1] = { &__type_info__cae9a62f8e83e959 };
const char * __type_info__f5bdd305f7c233ca_arg_names_var_7842071382785445858[1] = { "__this" };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f5bdd305f7c233ca_arg_types_var_7842071382785445858, __type_info__f5bdd305f7c233ca_arg_names_var_7842071382785445858, 1, 0, nullptr, 12, 8, UINT64_C(0xf5bdd305f7c233ca), "__finalize", 8, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x4c45a4a98b0b1017), "__yield", 16, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16516, 8, UINT64_C(0x73e89825efac073d), "stext", 24, 4 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_4 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0xad169fb5e110e050), "tin", 32, 6 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_5 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xad10776b6f00fe86), "ahead", 40, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_6 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xbe049cb5ef657837), "str", 48, 9 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_7 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x21b997841689d20f), "line", 72, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_8 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xc10fa5b5f1ae0f82), "row", 76, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_9 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xff52d15f88223576), "__hex_str_rename_at_216_36", 80, 13 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_10 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xa5169e56fe27f0cc), "__valid_hex_rename_at_217_36", 104, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_11 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x5823a747ac6adf2d), "___loop_at_218_32_rename_at_218_32", 105, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_12 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xd5ff24ec1375a895), "____rename_at_218_37", 108, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_13 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x3187cef00b753b54), "___pvar_0_at_218_32_rename_at_218_32", 112, 17 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_14 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x7b1350c97307efaf), "__is_negative_rename_at_283_20", 120, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_15 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x86e6ac39c080ccc), "__anyNumbers_rename_at_285_20", 121, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_16 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x2a76b2107d127cef), "__is_integer_rename_at_290_20", 122, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_17 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xe3d2412079a50afd), "__num_rename_at_311_20", 128, 19 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_18 =  { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xc1a0621f16fdf757), "__ival_rename_at_318_24", 136, 0 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_19 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xab77720135039591), "__name_rename_at_332_20", 144, 20 };
VarInfo __struct_info__6cd4a25fa22ecfe2_field_20 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0xaf96f3b69467d0ee), "___source_0_at_218_32_rename_at_218_32", 152, 21 };
VarInfo * __struct_info__6cd4a25fa22ecfe2_fields[21] =  { &__struct_info__6cd4a25fa22ecfe2_field_0, &__struct_info__6cd4a25fa22ecfe2_field_1, &__struct_info__6cd4a25fa22ecfe2_field_2, &__struct_info__6cd4a25fa22ecfe2_field_3, &__struct_info__6cd4a25fa22ecfe2_field_4, &__struct_info__6cd4a25fa22ecfe2_field_5, &__struct_info__6cd4a25fa22ecfe2_field_6, &__struct_info__6cd4a25fa22ecfe2_field_7, &__struct_info__6cd4a25fa22ecfe2_field_8, &__struct_info__6cd4a25fa22ecfe2_field_9, &__struct_info__6cd4a25fa22ecfe2_field_10, &__struct_info__6cd4a25fa22ecfe2_field_11, &__struct_info__6cd4a25fa22ecfe2_field_12, &__struct_info__6cd4a25fa22ecfe2_field_13, &__struct_info__6cd4a25fa22ecfe2_field_14, &__struct_info__6cd4a25fa22ecfe2_field_15, &__struct_info__6cd4a25fa22ecfe2_field_16, &__struct_info__6cd4a25fa22ecfe2_field_17, &__struct_info__6cd4a25fa22ecfe2_field_18, &__struct_info__6cd4a25fa22ecfe2_field_19, &__struct_info__6cd4a25fa22ecfe2_field_20 };
StructInfo __struct_info__6cd4a25fa22ecfe2 = {"_lambda_json_180_1", "json", 30, __struct_info__6cd4a25fa22ecfe2_fields, 21, 160, UINT64_C(0x0), nullptr, UINT64_C(0x6cd4a25fa22ecfe2), 3 };
VarInfo __func_info__5a16898e20f97124_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, 24, UINT64_C(0xeeae79bd86a73b7b), "str", 0, 0 };
VarInfo * __func_info__5a16898e20f97124_fields[1] =  { &__func_info__5a16898e20f97124_field_0 };
FuncInfo __func_info__5a16898e20f97124 = {"invoke block<(str:array<uint8> const#):void> const", "", __func_info__5a16898e20f97124_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x5a16898e20f97124), 0x0 };
TypeInfo __type_info__d96037642c78d59b = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__fac8b32671fee393_arg_types_var_7531112642396806091, __type_info__fac8b32671fee393_arg_names_var_7531112642396806091, 7, 0, nullptr, 24838, 16, UINT64_C(0xd96037642c78d59b) };
TypeInfo __type_info__994066f681df7bcd = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x994066f681df7bcd) };
TypeInfo __type_info__3fb2f51b6ebec3f2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__790033b1854b444b, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x3fb2f51b6ebec3f2) };
TypeInfo __type_info__cae9a62f8e83e959 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__3fc6f783e697c6b9, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xcae9a62f8e83e959) };
TypeInfo __type_info__e796e1d4af74511c = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0xe796e1d4af74511c) };
TypeInfo __type_info__12283e04d98e7c73 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x12283e04d98e7c73) };
TypeInfo __type_info__8dbbd9827382f56f = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__790033b1854b444b, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, 8, UINT64_C(0x8dbbd9827382f56f) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af87fe4c863f5252 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0xaf87fe4c863f5252) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__790033b1854b444b = { Type::tStructure, &__struct_info__69e2ef6fbeda51a0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 56, UINT64_C(0x790033b1854b444b) };
TypeInfo __type_info__fa1aceb92701bc9a = { Type::tStructure, &__struct_info__6883e30bf583dbcb, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24582, 24, UINT64_C(0xfa1aceb92701bc9a) };
TypeInfo __type_info__3fc6f783e697c6b9 = { Type::tStructure, &__struct_info__6cd4a25fa22ecfe2, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 160, UINT64_C(0x3fc6f783e697c6b9) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63d94c8601e773 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xaf63d94c8601e773) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__d922fe078cefab30 = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xd922fe078cefab30) };
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

TypeInfo * __tinfo_0[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_2[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_3[8] = { &__type_info__af90fe4c864e9d52, &__type_info__d96037642c78d59b, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_4[6] = { &__type_info__af90fe4c864e9d52, &__type_info__d96037642c78d59b, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_5[10] = { &__type_info__af90fe4c864e9d52, &__type_info__d96037642c78d59b, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_6[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_7[8] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_8[6] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_11[5] = { &__type_info__d96037642c78d59b, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_12[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_16[2] = { &__type_info__37d36026a6078a42, &__type_info__af87fe4c863f5252 };
TypeInfo * __tinfo_17[2] = { &__type_info__37d36026a6078a42, &__type_info__8d8d8008262e16ec };
TypeInfo * __tinfo_33[2] = { &__type_info__af90fe4c864e9d52, &__type_info__8dbbd9827382f56f };

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5db697d6f0792e58 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1336_0 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044333107478717573_6f02beb659b38015 ( Context * __context__, range __rng_rename_at_1398_1 );
inline void finalize_25878f262ceb6fd ( Context * __context__, json::_lambda_json_180_1 & ____this_rename_at_180_3 );
inline Sequence DAS_COMMENT((json::TokenAt)) _FuncbuiltinTickeachTick9663565701927713696_6ea716cdee8c36ed ( Context * __context__, Lambda DAS_COMMENT((bool,json::TokenAt)) const  __lam_rename_at_1464_4 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_c6c276e75c57546 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __it_rename_at_1377_6, int32_t & __value_rename_at_1377_7 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_3807b33f4b4a44b2 ( Context * __context__, char * const  __str_rename_at_1431_8 );
inline void _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_177_10, uint8_t __value_rename_at_177_11 );
inline void _FuncbuiltinTickpushTick10769833213962245646_89eb839cbd84f567 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_193_12, uint8_t __value_rename_at_193_13 );
inline bool _Func_lambda_json_180_1Tickfunction_3fa1de895736605c ( Context * __context__, json::_lambda_json_180_1 & ____this_rename_at_180_14, json::TokenAt & ___yield_180_rename_at_180_15 );
inline void _Func_lambda_json_180_1Tickfinalizer_caa1114ef8726b27 ( Context * __context__, json::_lambda_json_180_1 * ____this_rename_at_180_20 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_6a504e7a405e82ab ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __it_rename_at_1377_21, json::TokenAt & __value_rename_at_1377_22 );
inline void _FuncbuiltinTickpushTick10769833213962245646_64687faa0a9f918a ( Context * __context__, TArray<json::JsonValue *> & __Arr_rename_at_193_23, json::JsonValue * __value_rename_at_193_24 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_57fe3f8d782e30a7 ( Context * __context__, TTable<char *,json::JsonValue *> const  & __Tab_rename_at_1040_25, char * const  __at_rename_at_1040_26 );
inline void _FuncbuiltinTickinsertTick4246857231018487965_146aa59a1c117ef2 ( Context * __context__, TTable<char *,json::JsonValue *> & __Tab_rename_at_860_27, char * const  __at_rename_at_860_28, json::JsonValue * __val_rename_at_860_29 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick3875137551537175793_d3a3aed2f5c8bde1 ( Context * __context__, TTable<char *,json::JsonValue *> const  & __a_rename_at_1275_30 );
inline Sequence DAS_COMMENT((json::JsonValue * &)) _FuncbuiltinTickvaluesTick1935193042646774172_1c966248b122cc69 ( Context * __context__, TTable<char *,json::JsonValue *> const  & __a_rename_at_1297_32 );
inline json::JsonValue * JVNull_e01b92c701ceb486 ( Context * __context__ );
inline json::JsonValue * JV_b2c605dae25f2a45 ( Context * __context__, char * const  __v_rename_at_62_34 );
inline json::JsonValue * JV_28ecc49916ce157a ( Context * __context__, double __v_rename_at_67_35 );
inline json::JsonValue * JV_3aa84b5b46540567 ( Context * __context__, bool __v_rename_at_72_36 );
inline json::JsonValue * JV_6a8f99ad18c71d8b ( Context * __context__, TTable<char *,json::JsonValue *> & __v_rename_at_77_37 );
inline json::JsonValue * JV_c828edc46b7e6b3f ( Context * __context__, TArray<json::JsonValue *> & __v_rename_at_82_38 );
inline json::JsonValue * JV_d7eb81af2b7b4a86 ( Context * __context__, float __v_rename_at_87_39 );
inline json::JsonValue * JV_636c78630f7810d4 ( Context * __context__, int32_t __v_rename_at_92_40 );
inline json::JsonValue * JV_d5ea1665ba780d52 ( Context * __context__, Bitfield __v_rename_at_97_41 );
inline json::JsonValue * JV_7fe888aac9cdba7b ( Context * __context__, Bitfield8 __v_rename_at_102_42 );
inline json::JsonValue * JV_f9b00e1a90cb16e0 ( Context * __context__, Bitfield16 __v_rename_at_107_43 );
inline json::JsonValue * JV_22b8a5c944277732 ( Context * __context__, Bitfield64 __v_rename_at_112_44 );
inline json::JsonValue * JV_8a82057e10483a29 ( Context * __context__, int8_t __val_rename_at_117_45 );
inline json::JsonValue * JV_4bb097533696a50b ( Context * __context__, uint8_t __val_rename_at_122_46 );
inline json::JsonValue * JV_623fdb61479cb328 ( Context * __context__, int16_t __val_rename_at_127_47 );
inline json::JsonValue * JV_e94e08dbede97c15 ( Context * __context__, uint16_t __val_rename_at_132_48 );
inline json::JsonValue * JV_5d29b737db96a9a5 ( Context * __context__, uint32_t __val_rename_at_137_49 );
inline json::JsonValue * JV_89570eb3601bc343 ( Context * __context__, int64_t __val_rename_at_142_50 );
inline json::JsonValue * JV_f13c60740e17c9cb ( Context * __context__, uint64_t __val_rename_at_147_51 );
inline Sequence DAS_COMMENT((json::TokenAt)) lexer_b6e69875fdd15de2 ( Context * __context__, char * const  __text_rename_at_152_52 );
inline Sequence DAS_COMMENT((json::TokenAt)) lexer_7b01766a6e1e52b1 ( Context * __context__, TArray<uint8_t> const  & __text_rename_at_158_54 );
inline bool next_f03600350289290 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __text_rename_at_164_56, int32_t & __character_rename_at_164_57, int32_t & __line_rename_at_164_58, int32_t & __row_rename_at_164_59 );
inline Sequence DAS_COMMENT((json::TokenAt)) _lexer_d7025220389b96e7 ( Context * __context__, char * __stext_rename_at_178_60 );
inline bool expect_token_92ddcaa262ab835a ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __itv_rename_at_360_61, json::TokenAt & __ahead_rename_at_360_62, int32_t __vindex_rename_at_360_63, char * & __error_rename_at_360_64 );
inline bool expect_symbol_25ecda935f91d894 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __itv_rename_at_373_65, json::TokenAt & __ahead_rename_at_373_66, int32_t __sym_rename_at_373_67, char * & __error_rename_at_373_68 );
inline json::JsonValue * parse_value_ee908ba679ac2739 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __itv_rename_at_389_69, char * & __error_rename_at_389_70 );
inline json::JsonValue * read_json_1cf2494477255e77 ( Context * __context__, char * const  __text_rename_at_488_80, char * & __error_rename_at_488_81 );
inline json::JsonValue * read_json_f5237b438b70759b ( Context * __context__, TArray<uint8_t> const  & __text_rename_at_498_84, char * & __error_rename_at_498_85 );
inline bool set_no_trailing_zeros_f1b71a41030849b4 ( Context * __context__, bool __value_rename_at_510_88 );
inline bool set_no_empty_arrays_cb9f007816624484 ( Context * __context__, bool __value_rename_at_519_90 );
inline bool set_allow_duplicate_keys_b6a2085fcfe8cb80 ( Context * __context__, bool __value_rename_at_528_92 );
inline void write_value_171bdd0fdeefc56b ( Context * __context__, StringBuilderWriter & __writer_rename_at_536_94, json::JsonValue * const  __jsv_rename_at_536_95, int32_t __depth_rename_at_536_96 );
inline char * write_json_571c90b858deacc ( Context * __context__, json::JsonValue * const  __val_rename_at_613_104 );
inline char * write_json_e91db05e9e89ab6f ( Context * __context__, json::JsonValue * const  __val_rename_at_621_107 );
inline char * try_fixing_broken_json_ef0b3a4bda8a0603 ( Context * __context__, char * __bad_rename_at_628_108 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0xc713ddb3e835448d>(__context__) = 0;/*Token_string*/
    das_global<int32_t,0x96ac244c9265d269>(__context__) = 5;/*Token_symbol*/
    das_global<bool,0xd3b8b85b9bf19183>(__context__) = false;/*no_trailing_zeros*/
    das_global<bool,0xab7f08dbdeea5c03>(__context__) = false;/*no_empty_arrays*/
    das_global<bool,0x30e401a8d3210d31>(__context__) = false;/*allow_duplicate_keys*/
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5db697d6f0792e58 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1336_0 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1336_0),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044333107478717573_6f02beb659b38015 ( Context * __context__, range __rng_rename_at_1398_1 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1399_2;das_zero(__it_rename_at_1399_2);
    builtin_make_range_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1399_2),__rng_rename_at_1398_1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1399_2);
}

inline void finalize_25878f262ceb6fd ( Context * __context__, json::_lambda_json_180_1 &  ____this_rename_at_180_3 )
{
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_3.tin),__context__);
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_3.___source_0_at_218_32_rename_at_218_32),__context__);
    _FuncbuiltinTickfinalizeTick13836114024949725080_5db697d6f0792e58(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_3.str));
    _FuncbuiltinTickfinalizeTick13836114024949725080_5db697d6f0792e58(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_3.__hex_str_rename_at_216_36));
    memset((void*)&(____this_rename_at_180_3), 0, 160);
}

inline Sequence DAS_COMMENT((json::TokenAt)) _FuncbuiltinTickeachTick9663565701927713696_6ea716cdee8c36ed ( Context * __context__, Lambda DAS_COMMENT((bool,json::TokenAt)) const  __lam_rename_at_1464_4 )
{
    Sequence DAS_COMMENT((json::TokenAt)) __it_rename_at_1466_5;das_zero(__it_rename_at_1466_5);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__it_rename_at_1466_5),__lam_rename_at_1464_4,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(__it_rename_at_1466_5);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_c6c276e75c57546 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __it_rename_at_1377_6, int32_t & __value_rename_at_1377_7 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1377_6),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1377_7)),__context__));
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_3807b33f4b4a44b2 ( Context * __context__, char * const  __str_rename_at_1431_8 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1432_9;das_zero(__it_rename_at_1432_9);
    builtin_make_string_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1432_9),__str_rename_at_1431_8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1432_9);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_177_10, uint8_t __value_rename_at_177_11 )
{
    das_copy(__Arr_rename_at_177_10(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_177_10),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_177_11);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_89eb839cbd84f567 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_193_12, uint8_t __value_rename_at_193_13 )
{
    das_copy(__Arr_rename_at_193_12(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_193_12),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_13);
}

inline bool _Func_lambda_json_180_1Tickfunction_3fa1de895736605c ( Context * __context__, json::_lambda_json_180_1 &  ____this_rename_at_180_14, json::TokenAt &  ___yield_180_rename_at_180_15 )
{
    switch (____this_rename_at_180_14.__yield) {
    case 0: goto label_0;
    case 55: goto label_55;
    case 1: goto label_1;
    case 53: goto label_53;
    case 25: goto label_25;
    case 18: goto label_18;
    case 16: goto label_16;
    case 14: goto label_14;
    case 12: goto label_12;
    case 10: goto label_10;
    case 8: goto label_8;
    case 4: goto label_4;
    case 2: goto label_2;
    case 3: goto label_3;
    case 5: goto label_5;
    case 6: goto label_6;
    case 7: goto label_7;
    case 9: goto label_9;
    case 11: goto label_11;
    case 13: goto label_13;
    case 15: goto label_15;
    case 17: goto label_17;
    case 19: goto label_19;
    case 21: goto label_21;
    case 20: goto label_20;
    case 22: goto label_22;
    case 23: goto label_23;
    case 24: goto label_24;
    case 26: goto label_26;
    case 27: goto label_27;
    case 28: goto label_28;
    case 29: goto label_29;
    case 51: goto label_51;
    case 30: goto label_30;
    case 31: goto label_31;
    case 32: goto label_32;
    case 34: goto label_34;
    case 33: goto label_33;
    case 35: goto label_35;
    case 49: goto label_49;
    case 36: goto label_36;
    case 44: goto label_44;
    case 37: goto label_37;
    case 42: goto label_42;
    case 38: goto label_38;
    case 40: goto label_40;
    case 39: goto label_39;
    case 41: goto label_41;
    case 43: goto label_43;
    case 45: goto label_45;
    case 47: goto label_47;
    case 46: goto label_46;
    case 48: goto label_48;
    case 50: goto label_50;
    case 52: goto label_52;
    case 54: goto label_54;
    case 56: goto label_56;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    memset((void*)&(____this_rename_at_180_14.tin), 0, 8);
    das_move(____this_rename_at_180_14.tin,_FuncbuiltinTickeachTick4044332007967089362_3807b33f4b4a44b2(__context__,____this_rename_at_180_14.stext));
    das_copy(____this_rename_at_180_14.ahead,32);
    memset((void*)&(____this_rename_at_180_14.str), 0, 24);
    das_copy(____this_rename_at_180_14.line,1);
    das_copy(____this_rename_at_180_14.row,0);
    label_55:;;
    if ( !!(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin))) )
    {
        goto label_56;
    };
    while ( is_white_space(____this_rename_at_180_14.ahead) && next_f03600350289290(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead,____this_rename_at_180_14.line,____this_rename_at_180_14.row) )
    {
    };
    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin)) )
    {
        return das_auto_cast<bool>::cast(false);
    };
    if ( !((((((____this_rename_at_180_14.ahead == 91) || (____this_rename_at_180_14.ahead == 93)) || (____this_rename_at_180_14.ahead == 123)) || (____this_rename_at_180_14.ahead == 125)) || (____this_rename_at_180_14.ahead == 58)) || (____this_rename_at_180_14.ahead == 44)) )
    {
        goto label_53;
    };
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_197;
        das_zero(__mks_197);
        das_copy((__mks_197.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_197;
            das_get_variant_field<int32_t,8,5>::set(__mkv_197) = ____this_rename_at_180_14.ahead;
            return __mkv_197;
        })())));
        das_copy((__mks_197.line),(____this_rename_at_180_14.line));
        das_copy((__mks_197.row),(____this_rename_at_180_14.row));
        return __mks_197;
    })()));
    das_copy(____this_rename_at_180_14.__yield,1);
    return das_auto_cast<bool>::cast(true);
    label_1:;;
    _FuncbuiltinTicknextTick17450348357676149856_c6c276e75c57546(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead);
    goto label_54;
    label_53:;;
    if ( !(____this_rename_at_180_14.ahead == 34) )
    {
        goto label_51;
    };
    label_25:;;
    if ( !(next_f03600350289290(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead,____this_rename_at_180_14.line,____this_rename_at_180_14.row) && (____this_rename_at_180_14.ahead != 34)) )
    {
        goto label_26;
    };
    if ( !(____this_rename_at_180_14.ahead == 92) )
    {
        goto label_23;
    };
    if ( !next_f03600350289290(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead,____this_rename_at_180_14.line,____this_rename_at_180_14.row) )
    {
        goto label_21;
    };
    if ( !(____this_rename_at_180_14.ahead == 98) )
    {
        goto label_18;
    };
    das_copy(____this_rename_at_180_14.ahead,8);
    goto label_19;
    label_18:;;
    if ( !(____this_rename_at_180_14.ahead == 102) )
    {
        goto label_16;
    };
    das_copy(____this_rename_at_180_14.ahead,12);
    goto label_17;
    label_16:;;
    if ( !(____this_rename_at_180_14.ahead == 110) )
    {
        goto label_14;
    };
    das_copy(____this_rename_at_180_14.ahead,10);
    goto label_15;
    label_14:;;
    if ( !(____this_rename_at_180_14.ahead == 114) )
    {
        goto label_12;
    };
    das_copy(____this_rename_at_180_14.ahead,13);
    goto label_13;
    label_12:;;
    if ( !(____this_rename_at_180_14.ahead == 116) )
    {
        goto label_10;
    };
    das_copy(____this_rename_at_180_14.ahead,9);
    goto label_11;
    label_10:;;
    if ( !(____this_rename_at_180_14.ahead == 47) )
    {
        goto label_8;
    };
    das_copy(____this_rename_at_180_14.ahead,47);
    goto label_9;
    label_8:;;
    if ( !(____this_rename_at_180_14.ahead == 117) )
    {
        goto label_7;
    };
    memset((void*)&(____this_rename_at_180_14.__hex_str_rename_at_216_36), 0, 24);
    das_copy(____this_rename_at_180_14.__valid_hex_rename_at_217_36,true);
    das_copy(____this_rename_at_180_14.___loop_at_218_32_rename_at_218_32,true);
    das_move(____this_rename_at_180_14.___source_0_at_218_32_rename_at_218_32,_FuncbuiltinTickeachTick4044333107478717573_6f02beb659b38015(__context__,range(0,4)));
    memset((void*)&(____this_rename_at_180_14.____rename_at_218_37), 0, 4);
    das_copy(____this_rename_at_180_14.___pvar_0_at_218_32_rename_at_218_32,das_cast<void *>::cast(das_ref(__context__,____this_rename_at_180_14.____rename_at_218_37)));
    DAS_SETBOOLAND((____this_rename_at_180_14.___loop_at_218_32_rename_at_218_32),(builtin_iterator_first(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.___source_0_at_218_32_rename_at_218_32),____this_rename_at_180_14.___pvar_0_at_218_32_rename_at_218_32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    label_4:;;
    if ( !____this_rename_at_180_14.___loop_at_218_32_rename_at_218_32 )
    {
        goto label_6;
    };
    if ( !!(next_f03600350289290(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead,____this_rename_at_180_14.line,____this_rename_at_180_14.row)) )
    {
        goto label_3;
    };
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_220;
        das_zero(__mks_220);
        das_copy((__mks_220.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_220;
            das_get_variant_field<char *,8,6>::set(__mkv_220) = ((char *) "incomplete \\u escape");
            return __mkv_220;
        })())));
        das_copy((__mks_220.line),(____this_rename_at_180_14.line));
        das_copy((__mks_220.row),(____this_rename_at_180_14.row));
        return __mks_220;
    })()));
    das_copy(____this_rename_at_180_14.__yield,2);
    return das_auto_cast<bool>::cast(true);
    label_2:;;
    return das_auto_cast<bool>::cast(false);
    label_3:;;
    if ( !is_hex(____this_rename_at_180_14.ahead) )
    {
        das_copy(____this_rename_at_180_14.__valid_hex_rename_at_217_36,false);
        goto label_6;
    };
    _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.__hex_str_rename_at_216_36),uint8_t(____this_rename_at_180_14.ahead));
    label_5:;;
    DAS_SETBOOLAND((____this_rename_at_180_14.___loop_at_218_32_rename_at_218_32),(builtin_iterator_next(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.___source_0_at_218_32_rename_at_218_32),____this_rename_at_180_14.___pvar_0_at_218_32_rename_at_218_32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    goto label_4;
    label_6:;;
    builtin_iterator_close(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.___source_0_at_218_32_rename_at_218_32),____this_rename_at_180_14.___pvar_0_at_218_32_rename_at_218_32,__context__);
    if ( ____this_rename_at_180_14.__valid_hex_rename_at_217_36 && (builtin_array_size(das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.__hex_str_rename_at_216_36)) == 4) )
    {
        uint32_t __codepoint_rename_at_230_16 = 0x0u;
        {
            bool __need_loop_231 = true;
            // hb: uint8&
            das_iterator<TArray<uint8_t>> __hb_iterator(____this_rename_at_180_14.__hex_str_rename_at_216_36);
            uint8_t * __hb_rename_at_231_17;
            __need_loop_231 = __hb_iterator.first(__context__,(__hb_rename_at_231_17)) && __need_loop_231;
            for ( ; __need_loop_231 ; __need_loop_231 = __hb_iterator.next(__context__,(__hb_rename_at_231_17)) )
            {
                das_copy(__codepoint_rename_at_230_16,SimPolicy<uint32_t>::BinShl(__codepoint_rename_at_230_16,0x4u,*__context__,nullptr));
                int32_t __c_rename_at_233_18 = ((int32_t)int32_t((*__hb_rename_at_231_17)));
                if ( (__c_rename_at_233_18 >= 48) && (__c_rename_at_233_18 <= 57) )
                {
                    __codepoint_rename_at_230_16 += uint32_t(__c_rename_at_233_18 - 48);
                } else if ( (__c_rename_at_233_18 >= 97) && (__c_rename_at_233_18 <= 102) )
                {
                    __codepoint_rename_at_230_16 += uint32_t((__c_rename_at_233_18 - 97) + 10);
                } else if ( (__c_rename_at_233_18 >= 65) && (__c_rename_at_233_18 <= 70) )
                {
                    __codepoint_rename_at_230_16 += uint32_t((__c_rename_at_233_18 - 65) + 10);
                };
            }
            __hb_iterator.close(__context__,(__hb_rename_at_231_17));
        };
        if ( __codepoint_rename_at_230_16 <= 0x7fu )
        {
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(__codepoint_rename_at_230_16));
        } else if ( __codepoint_rename_at_230_16 <= 0x7ffu )
        {
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(0xc0u | (SimPolicy<uint32_t>::BinShr(__codepoint_rename_at_230_16,0x6u,*__context__,nullptr))));
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(0x80u | (__codepoint_rename_at_230_16 & 0x3fu)));
        } else if ( __codepoint_rename_at_230_16 <= 0xffffu )
        {
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(0xe0u | (SimPolicy<uint32_t>::BinShr(__codepoint_rename_at_230_16,0xcu,*__context__,nullptr))));
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(0x80u | ((SimPolicy<uint32_t>::BinShr(__codepoint_rename_at_230_16,0x6u,*__context__,nullptr)) & 0x3fu)));
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(0x80u | (__codepoint_rename_at_230_16 & 0x3fu)));
        };
    } else {
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),0x5c);
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),0x75);
        {
            bool __need_loop_257 = true;
            // b: uint8&
            das_iterator<TArray<uint8_t>> __b_iterator(____this_rename_at_180_14.__hex_str_rename_at_216_36);
            uint8_t * __b_rename_at_257_19;
            __need_loop_257 = __b_iterator.first(__context__,(__b_rename_at_257_19)) && __need_loop_257;
            for ( ; __need_loop_257 ; __need_loop_257 = __b_iterator.next(__context__,(__b_rename_at_257_19)) )
            {
                _FuncbuiltinTickpushTick10769833213962245646_89eb839cbd84f567(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),(*__b_rename_at_257_19));
            }
            __b_iterator.close(__context__,(__b_rename_at_257_19));
        };
        if ( !____this_rename_at_180_14.__valid_hex_rename_at_217_36 )
        {
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
        };
    };
    goto label_25;
    label_7:;;
    label_9:;;
    label_11:;;
    label_13:;;
    label_15:;;
    label_17:;;
    label_19:;;
    _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
    goto label_22;
    label_21:;;
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_268;
        das_zero(__mks_268);
        das_copy((__mks_268.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_268;
            das_get_variant_field<char *,8,6>::set(__mkv_268) = ((char *) "string escape sequence exceeds text");
            return __mkv_268;
        })())));
        das_copy((__mks_268.line),(____this_rename_at_180_14.line));
        das_copy((__mks_268.row),(____this_rename_at_180_14.row));
        return __mks_268;
    })()));
    das_copy(____this_rename_at_180_14.__yield,20);
    return das_auto_cast<bool>::cast(true);
    label_20:;;
    return das_auto_cast<bool>::cast(false);
    label_22:;;
    goto label_24;
    label_23:;;
    _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
    label_24:;;
    goto label_25;
    label_26:;;
    if ( !builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin)) )
    {
        goto label_28;
    };
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_276;
        das_zero(__mks_276);
        das_copy((__mks_276.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_276;
            das_get_variant_field<char *,8,6>::set(__mkv_276) = ((char *) "string exceeds text");
            return __mkv_276;
        })())));
        das_copy((__mks_276.line),(____this_rename_at_180_14.line));
        das_copy((__mks_276.row),(____this_rename_at_180_14.row));
        return __mks_276;
    })()));
    das_copy(____this_rename_at_180_14.__yield,27);
    return das_auto_cast<bool>::cast(true);
    label_27:;;
    return das_auto_cast<bool>::cast(false);
    label_28:;;
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_279;
        das_zero(__mks_279);
        das_copy((__mks_279.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_279;
            das_get_variant_field<char *,8,0>::set(__mkv_279) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
            return __mkv_279;
        })())));
        das_copy((__mks_279.line),(____this_rename_at_180_14.line));
        das_copy((__mks_279.row),(____this_rename_at_180_14.row));
        return __mks_279;
    })()));
    das_copy(____this_rename_at_180_14.__yield,29);
    return das_auto_cast<bool>::cast(true);
    label_29:;;
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTicknextTick17450348357676149856_c6c276e75c57546(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead);
    goto label_52;
    label_51:;;
    if ( !(((____this_rename_at_180_14.ahead == 43) || (____this_rename_at_180_14.ahead == 45)) || is_number(____this_rename_at_180_14.ahead)) )
    {
        goto label_49;
    };
    das_copy(____this_rename_at_180_14.__is_negative_rename_at_283_20,____this_rename_at_180_14.ahead == 45);
    _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
    das_copy(____this_rename_at_180_14.__anyNumbers_rename_at_285_20,is_number(____this_rename_at_180_14.ahead));
    while ( next_f03600350289290(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead,____this_rename_at_180_14.line,____this_rename_at_180_14.row) && is_number(____this_rename_at_180_14.ahead) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
        das_copy(____this_rename_at_180_14.__anyNumbers_rename_at_285_20,true);
    };
    das_copy(____this_rename_at_180_14.__is_integer_rename_at_290_20,true);
    if ( !(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin))) && (____this_rename_at_180_14.ahead == 46) )
    {
        das_copy(____this_rename_at_180_14.__is_integer_rename_at_290_20,false);
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
        while ( next_f03600350289290(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead,____this_rename_at_180_14.line,____this_rename_at_180_14.row) && is_number(____this_rename_at_180_14.ahead) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
            das_copy(____this_rename_at_180_14.__anyNumbers_rename_at_285_20,true);
        };
    };
    if ( !(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin))) && ((____this_rename_at_180_14.ahead == 101) || (____this_rename_at_180_14.ahead == 69)) )
    {
        das_copy(____this_rename_at_180_14.__is_integer_rename_at_290_20,false);
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
        next_f03600350289290(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead,____this_rename_at_180_14.line,____this_rename_at_180_14.row);
        if ( ((____this_rename_at_180_14.ahead == 43) || (____this_rename_at_180_14.ahead == 45)) || is_number(____this_rename_at_180_14.ahead) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
        };
        while ( next_f03600350289290(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead,____this_rename_at_180_14.line,____this_rename_at_180_14.row) && is_number(____this_rename_at_180_14.ahead) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
            das_copy(____this_rename_at_180_14.__anyNumbers_rename_at_285_20,true);
        };
    };
    das_copy(____this_rename_at_180_14.__num_rename_at_311_20,((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( !!(____this_rename_at_180_14.__anyNumbers_rename_at_285_20) )
    {
        goto label_31;
    };
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_313;
        das_zero(__mks_313);
        das_copy((__mks_313.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_313;
            das_get_variant_field<char *,8,6>::set(__mkv_313) = das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_0, cast<char * const >::from(((char *) "invalid number ")), cast<char *>::from(____this_rename_at_180_14.__num_rename_at_311_20)));
            return __mkv_313;
        })())));
        das_copy((__mks_313.line),(____this_rename_at_180_14.line));
        das_copy((__mks_313.row),(____this_rename_at_180_14.row));
        return __mks_313;
    })()));
    das_copy(____this_rename_at_180_14.__yield,30);
    return das_auto_cast<bool>::cast(true);
    label_30:;;
    return das_auto_cast<bool>::cast(false);
    label_31:;;
    if ( !____this_rename_at_180_14.__is_integer_rename_at_290_20 )
    {
        goto label_34;
    };
    das_copy(____this_rename_at_180_14.__ival_rename_at_318_24,____this_rename_at_180_14.__is_negative_rename_at_283_20 ? das_auto_cast<int64_t>::cast(string_to_int64(____this_rename_at_180_14.__num_rename_at_311_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))) : das_auto_cast<int64_t>::cast(int64_t(string_to_uint64(____this_rename_at_180_14.__num_rename_at_311_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_319;
        das_zero(__mks_319);
        das_copy((__mks_319.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_319;
            das_get_variant_field<int64_t,8,2>::set(__mkv_319) = ____this_rename_at_180_14.__ival_rename_at_318_24;
            return __mkv_319;
        })())));
        das_copy((__mks_319.line),(____this_rename_at_180_14.line));
        das_copy((__mks_319.row),(____this_rename_at_180_14.row));
        return __mks_319;
    })()));
    das_copy(____this_rename_at_180_14.__yield,32);
    return das_auto_cast<bool>::cast(true);
    label_32:;;
    goto label_35;
    label_34:;;
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_321;
        das_zero(__mks_321);
        das_copy((__mks_321.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_321;
            das_get_variant_field<double,8,1>::set(__mkv_321) = string_to_double(____this_rename_at_180_14.__num_rename_at_311_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            return __mkv_321;
        })())));
        das_copy((__mks_321.line),(____this_rename_at_180_14.line));
        das_copy((__mks_321.row),(____this_rename_at_180_14.row));
        return __mks_321;
    })()));
    das_copy(____this_rename_at_180_14.__yield,33);
    return das_auto_cast<bool>::cast(true);
    label_33:;;
    label_35:;;
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    delete_string(____this_rename_at_180_14.__num_rename_at_311_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    goto label_50;
    label_49:;;
    if ( !is_alpha(____this_rename_at_180_14.ahead) )
    {
        goto label_47;
    };
    _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
    while ( next_f03600350289290(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_180_14.tin),____this_rename_at_180_14.ahead,____this_rename_at_180_14.line,____this_rename_at_180_14.row) && is_alpha(____this_rename_at_180_14.ahead) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),uint8_t(____this_rename_at_180_14.ahead));
    };
    das_copy(____this_rename_at_180_14.__name_rename_at_332_20,((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( !(SimPolicy<char *>::Equ(cast<char *>::from(____this_rename_at_180_14.__name_rename_at_332_20),cast<char *>::from(((char *) "true")),*__context__,nullptr)) )
    {
        goto label_44;
    };
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_334;
        das_zero(__mks_334);
        das_copy((__mks_334.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_334;
            das_get_variant_field<bool,8,3>::set(__mkv_334) = true;
            return __mkv_334;
        })())));
        das_copy((__mks_334.line),(____this_rename_at_180_14.line));
        das_copy((__mks_334.row),(____this_rename_at_180_14.row));
        return __mks_334;
    })()));
    das_copy(____this_rename_at_180_14.__yield,36);
    return das_auto_cast<bool>::cast(true);
    label_36:;;
    goto label_45;
    label_44:;;
    if ( !(SimPolicy<char *>::Equ(cast<char *>::from(____this_rename_at_180_14.__name_rename_at_332_20),cast<char *>::from(((char *) "false")),*__context__,nullptr)) )
    {
        goto label_42;
    };
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_336;
        das_zero(__mks_336);
        das_copy((__mks_336.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_336;
            das_get_variant_field<bool,8,3>::set(__mkv_336) = false;
            return __mkv_336;
        })())));
        das_copy((__mks_336.line),(____this_rename_at_180_14.line));
        das_copy((__mks_336.row),(____this_rename_at_180_14.row));
        return __mks_336;
    })()));
    das_copy(____this_rename_at_180_14.__yield,37);
    return das_auto_cast<bool>::cast(true);
    label_37:;;
    goto label_43;
    label_42:;;
    if ( !(SimPolicy<char *>::Equ(cast<char *>::from(____this_rename_at_180_14.__name_rename_at_332_20),cast<char *>::from(((char *) "null")),*__context__,nullptr)) )
    {
        goto label_40;
    };
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_338;
        das_zero(__mks_338);
        das_copy((__mks_338.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_338;
            das_get_variant_field<void *,8,4>::set(__mkv_338) = nullptr;
            return __mkv_338;
        })())));
        das_copy((__mks_338.line),(____this_rename_at_180_14.line));
        das_copy((__mks_338.row),(____this_rename_at_180_14.row));
        return __mks_338;
    })()));
    das_copy(____this_rename_at_180_14.__yield,38);
    return das_auto_cast<bool>::cast(true);
    label_38:;;
    goto label_41;
    label_40:;;
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_340;
        das_zero(__mks_340);
        das_copy((__mks_340.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_340;
            das_get_variant_field<char *,8,6>::set(__mkv_340) = das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_0, cast<char * const >::from(((char *) "invalid name ")), cast<char *>::from(____this_rename_at_180_14.__name_rename_at_332_20)));
            return __mkv_340;
        })())));
        das_copy((__mks_340.line),(____this_rename_at_180_14.line));
        das_copy((__mks_340.row),(____this_rename_at_180_14.row));
        return __mks_340;
    })()));
    das_copy(____this_rename_at_180_14.__yield,39);
    return das_auto_cast<bool>::cast(true);
    label_39:;;
    return das_auto_cast<bool>::cast(false);
    label_41:;;
    label_43:;;
    label_45:;;
    delete_string(____this_rename_at_180_14.__name_rename_at_332_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(____this_rename_at_180_14.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    goto label_48;
    label_47:;;
    das_copy(___yield_180_rename_at_180_15,(([&]() -> json::TokenAt {
        json::TokenAt __mks_348;
        das_zero(__mks_348);
        das_copy((__mks_348.value),((([&]() -> TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> {
            TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> __mkv_348;
            das_get_variant_field<char *,8,6>::set(__mkv_348) = das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_2, cast<char * const >::from(((char *) "invalid character `")), cast<char * const >::from(((char * const )(to_string_char(____this_rename_at_180_14.ahead,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(____this_rename_at_180_14.ahead)));
            return __mkv_348;
        })())));
        das_copy((__mks_348.line),(____this_rename_at_180_14.line));
        das_copy((__mks_348.row),(____this_rename_at_180_14.row));
        return __mks_348;
    })()));
    das_copy(____this_rename_at_180_14.__yield,46);
    return das_auto_cast<bool>::cast(true);
    label_46:;;
    return das_auto_cast<bool>::cast(false);
    label_48:;;
    label_50:;;
    label_52:;;
    label_54:;;
    goto label_55;
    label_56:;;
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_json_180_1Tickfinalizer_caa1114ef8726b27 ( Context * __context__, json::_lambda_json_180_1 * ____this_rename_at_180_20 )
{
    delete_string(das_deref(__context__,____this_rename_at_180_20,__FILE__,__LINE__).stext,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    finalize_25878f262ceb6fd(__context__,das_arg<json::_lambda_json_180_1>::pass(das_deref(__context__,____this_rename_at_180_20,__FILE__,__LINE__)));
    das_delete_lambda_struct<json::_lambda_json_180_1 *>::clear(__context__,____this_rename_at_180_20);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_6a504e7a405e82ab ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __it_rename_at_1377_21, json::TokenAt &  __value_rename_at_1377_22 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__it_rename_at_1377_21),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1377_22)),__context__));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_64687faa0a9f918a ( Context * __context__, TArray<json::JsonValue *> &  __Arr_rename_at_193_23, json::JsonValue * __value_rename_at_193_24 )
{
    das_copy(__Arr_rename_at_193_23(builtin_array_push_back(das_arg<TArray<json::JsonValue *>>::pass(__Arr_rename_at_193_23),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_24);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_57fe3f8d782e30a7 ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __Tab_rename_at_1040_25, char * const  __at_rename_at_1040_26 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1040_25,__at_rename_at_1040_26));
}

inline void _FuncbuiltinTickinsertTick4246857231018487965_146aa59a1c117ef2 ( Context * __context__, TTable<char *,json::JsonValue *> &  __Tab_rename_at_860_27, char * const  __at_rename_at_860_28, json::JsonValue * __val_rename_at_860_29 )
{
    das_copy(__Tab_rename_at_860_27(__at_rename_at_860_28,__context__),__val_rename_at_860_29);
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick3875137551537175793_d3a3aed2f5c8bde1 ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __a_rename_at_1275_30 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1276_31;das_zero(__it_rename_at_1276_31);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1276_31),__a_rename_at_1275_30,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1276_31);
}

inline Sequence DAS_COMMENT((json::JsonValue * &)) _FuncbuiltinTickvaluesTick1935193042646774172_1c966248b122cc69 ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __a_rename_at_1297_32 )
{
    Sequence DAS_COMMENT((json::JsonValue * *)) __it_rename_at_1298_33;das_zero(__it_rename_at_1298_33);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((json::JsonValue * const ))>::pass(__it_rename_at_1298_33),__a_rename_at_1297_32,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::JsonValue * &))>::cast(__it_rename_at_1298_33);
}

inline json::JsonValue * JVNull_e01b92c701ceb486 ( Context * __context__ )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_59;
        das_zero(__mks_59);
        das_move((__mks_59.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_59;
            das_get_variant_field<void *,8,6>::set(__mkv_59) = nullptr;
            return __mkv_59;
        })())));
        return __mks_59;
    })())));
}

inline json::JsonValue * JV_b2c605dae25f2a45 ( Context * __context__, char * const  __v_rename_at_62_34 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_64;
        das_zero(__mks_64);
        das_move((__mks_64.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_64;
            das_get_variant_field<char *,8,2>::set(__mkv_64) = __v_rename_at_62_34;
            return __mkv_64;
        })())));
        return __mks_64;
    })())));
}

inline json::JsonValue * JV_28ecc49916ce157a ( Context * __context__, double __v_rename_at_67_35 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_69;
        das_zero(__mks_69);
        das_move((__mks_69.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_69;
            das_get_variant_field<double,8,3>::set(__mkv_69) = __v_rename_at_67_35;
            return __mkv_69;
        })())));
        return __mks_69;
    })())));
}

inline json::JsonValue * JV_3aa84b5b46540567 ( Context * __context__, bool __v_rename_at_72_36 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_74;
        das_zero(__mks_74);
        das_move((__mks_74.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_74;
            das_get_variant_field<bool,8,5>::set(__mkv_74) = __v_rename_at_72_36;
            return __mkv_74;
        })())));
        return __mks_74;
    })())));
}

inline json::JsonValue * JV_6a8f99ad18c71d8b ( Context * __context__, TTable<char *,json::JsonValue *> &  __v_rename_at_77_37 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_79;
        das_zero(__mks_79);
        das_move((__mks_79.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_79;
            das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::set(__mkv_79) = __v_rename_at_77_37;
            return __mkv_79;
        })())));
        return __mks_79;
    })())));
}

inline json::JsonValue * JV_c828edc46b7e6b3f ( Context * __context__, TArray<json::JsonValue *> &  __v_rename_at_82_38 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_84;
        das_zero(__mks_84);
        das_move((__mks_84.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_84;
            das_get_variant_field<TArray<json::JsonValue *>,8,1>::set(__mkv_84) = __v_rename_at_82_38;
            return __mkv_84;
        })())));
        return __mks_84;
    })())));
}

inline json::JsonValue * JV_d7eb81af2b7b4a86 ( Context * __context__, float __v_rename_at_87_39 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_89;
        das_zero(__mks_89);
        das_move((__mks_89.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_89;
            das_get_variant_field<double,8,3>::set(__mkv_89) = double(__v_rename_at_87_39);
            return __mkv_89;
        })())));
        return __mks_89;
    })())));
}

inline json::JsonValue * JV_636c78630f7810d4 ( Context * __context__, int32_t __v_rename_at_92_40 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_94;
        das_zero(__mks_94);
        das_move((__mks_94.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_94;
            das_get_variant_field<int64_t,8,4>::set(__mkv_94) = int64_t(__v_rename_at_92_40);
            return __mkv_94;
        })())));
        return __mks_94;
    })())));
}

inline json::JsonValue * JV_d5ea1665ba780d52 ( Context * __context__, Bitfield __v_rename_at_97_41 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_99;
        das_zero(__mks_99);
        das_move((__mks_99.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_99;
            das_get_variant_field<int64_t,8,4>::set(__mkv_99) = int64_t(__v_rename_at_97_41);
            return __mkv_99;
        })())));
        return __mks_99;
    })())));
}

inline json::JsonValue * JV_7fe888aac9cdba7b ( Context * __context__, Bitfield8 __v_rename_at_102_42 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_104;
        das_zero(__mks_104);
        das_move((__mks_104.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_104;
            das_get_variant_field<int64_t,8,4>::set(__mkv_104) = int64_t(__v_rename_at_102_42);
            return __mkv_104;
        })())));
        return __mks_104;
    })())));
}

inline json::JsonValue * JV_f9b00e1a90cb16e0 ( Context * __context__, Bitfield16 __v_rename_at_107_43 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_109;
        das_zero(__mks_109);
        das_move((__mks_109.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_109;
            das_get_variant_field<int64_t,8,4>::set(__mkv_109) = int64_t(__v_rename_at_107_43);
            return __mkv_109;
        })())));
        return __mks_109;
    })())));
}

inline json::JsonValue * JV_22b8a5c944277732 ( Context * __context__, Bitfield64 __v_rename_at_112_44 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_114;
        das_zero(__mks_114);
        das_move((__mks_114.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_114;
            das_get_variant_field<int64_t,8,4>::set(__mkv_114) = int64_t(__v_rename_at_112_44);
            return __mkv_114;
        })())));
        return __mks_114;
    })())));
}

inline json::JsonValue * JV_8a82057e10483a29 ( Context * __context__, int8_t __val_rename_at_117_45 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_119;
        das_zero(__mks_119);
        das_move((__mks_119.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_119;
            das_get_variant_field<int64_t,8,4>::set(__mkv_119) = int64_t(__val_rename_at_117_45);
            return __mkv_119;
        })())));
        return __mks_119;
    })())));
}

inline json::JsonValue * JV_4bb097533696a50b ( Context * __context__, uint8_t __val_rename_at_122_46 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_124;
        das_zero(__mks_124);
        das_move((__mks_124.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_124;
            das_get_variant_field<int64_t,8,4>::set(__mkv_124) = int64_t(__val_rename_at_122_46);
            return __mkv_124;
        })())));
        return __mks_124;
    })())));
}

inline json::JsonValue * JV_623fdb61479cb328 ( Context * __context__, int16_t __val_rename_at_127_47 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_129;
        das_zero(__mks_129);
        das_move((__mks_129.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_129;
            das_get_variant_field<int64_t,8,4>::set(__mkv_129) = int64_t(__val_rename_at_127_47);
            return __mkv_129;
        })())));
        return __mks_129;
    })())));
}

inline json::JsonValue * JV_e94e08dbede97c15 ( Context * __context__, uint16_t __val_rename_at_132_48 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_134;
        das_zero(__mks_134);
        das_move((__mks_134.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_134;
            das_get_variant_field<int64_t,8,4>::set(__mkv_134) = int64_t(__val_rename_at_132_48);
            return __mkv_134;
        })())));
        return __mks_134;
    })())));
}

inline json::JsonValue * JV_5d29b737db96a9a5 ( Context * __context__, uint32_t __val_rename_at_137_49 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_139;
        das_zero(__mks_139);
        das_move((__mks_139.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_139;
            das_get_variant_field<int64_t,8,4>::set(__mkv_139) = int64_t(__val_rename_at_137_49);
            return __mkv_139;
        })())));
        return __mks_139;
    })())));
}

inline json::JsonValue * JV_89570eb3601bc343 ( Context * __context__, int64_t __val_rename_at_142_50 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_144;
        das_zero(__mks_144);
        das_move((__mks_144.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_144;
            das_get_variant_field<int64_t,8,4>::set(__mkv_144) = __val_rename_at_142_50;
            return __mkv_144;
        })())));
        return __mks_144;
    })())));
}

inline json::JsonValue * JV_f13c60740e17c9cb ( Context * __context__, uint64_t __val_rename_at_147_51 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_149;
        das_zero(__mks_149);
        das_move((__mks_149.value),((([&]() -> TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_149;
            das_get_variant_field<int64_t,8,4>::set(__mkv_149) = int64_t(__val_rename_at_147_51);
            return __mkv_149;
        })())));
        return __mks_149;
    })())));
}

inline Sequence DAS_COMMENT((json::TokenAt)) lexer_b6e69875fdd15de2 ( Context * __context__, char * const  __text_rename_at_152_52 )
{
    char * __stext_rename_at_154_53 = ((char *)(char *)(((char * const )(builtin_string_clone(__text_rename_at_152_52,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(_lexer_d7025220389b96e7(__context__,__stext_rename_at_154_53));
}

inline Sequence DAS_COMMENT((json::TokenAt)) lexer_7b01766a6e1e52b1 ( Context * __context__, TArray<uint8_t> const  &  __text_rename_at_158_54 )
{
    char * __stext_rename_at_160_55 = ((char *)(char *)(((char * const )(builtin_string_from_array(__text_rename_at_158_54,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(_lexer_d7025220389b96e7(__context__,__stext_rename_at_160_55));
}

inline bool next_f03600350289290 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __text_rename_at_164_56, int32_t & __character_rename_at_164_57, int32_t & __line_rename_at_164_58, int32_t & __row_rename_at_164_59 )
{
    if ( _FuncbuiltinTicknextTick17450348357676149856_c6c276e75c57546(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__text_rename_at_164_56),__character_rename_at_164_57) )
    {
        if ( is_new_line(__character_rename_at_164_57) )
        {
            ++__line_rename_at_164_58;
            das_copy(__row_rename_at_164_59,0);
        } else {
            ++__row_rename_at_164_59;
        };
        return das_auto_cast<bool>::cast(true);
    } else {
        return das_auto_cast<bool>::cast(false);
    };
}

inline Sequence DAS_COMMENT((json::TokenAt)) _lexer_d7025220389b96e7 ( Context * __context__, char * __stext_rename_at_178_60 )
{
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(_FuncbuiltinTickeachTick9663565701927713696_6ea716cdee8c36ed(__context__,das_ascend<bool,json::_lambda_json_180_1>::make(__context__,&__type_info__3fc6f783e697c6b9,(([&]() -> json::_lambda_json_180_1 {
        json::_lambda_json_180_1 __mks_180;
        das_zero(__mks_180);
        das_copy((__mks_180.__lambda),(Func(__context__->fnByMangledName(/*@json::_lambda_json_180_1`function XS<json::_lambda_json_180_1> S<json::TokenAt>*/ 0x4cde46c0e129c1d3))));
        das_copy((__mks_180.__finalize),(Func(__context__->fnByMangledName(/*@json::_lambda_json_180_1`finalizer X1<S<json::_lambda_json_180_1>>?*/ 0x45c648bccbaaa9cb))));
        das_copy((__mks_180.stext),(__stext_rename_at_178_60));
        return __mks_180;
    })()))));
}

inline bool expect_token_92ddcaa262ab835a ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __itv_rename_at_360_61, json::TokenAt &  __ahead_rename_at_360_62, int32_t __vindex_rename_at_360_63, char * & __error_rename_at_360_64 )
{
    if ( !_FuncbuiltinTicknextTick17450348357676149856_6a504e7a405e82ab(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_360_61),das_arg<json::TokenAt>::pass(__ahead_rename_at_360_62)) )
    {
        das_copy(__error_rename_at_360_64,((char *) "unexected eos"));
        return das_auto_cast<bool>::cast(false);
    } else if ( variant_index(das_arg<TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *>>::pass(__ahead_rename_at_360_62.value)) != __vindex_rename_at_360_63 )
    {
        das_copy(__error_rename_at_360_64,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_3, cast<char * const >::from(((char *) "unexpected ")), cast<TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_360_62.value), cast<char * const >::from(((char *) ", expecting variant ")), cast<int32_t>::from(__vindex_rename_at_360_63), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_360_62.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_360_62.row))));
        return das_auto_cast<bool>::cast(false);
    } else {
        return das_auto_cast<bool>::cast(true);
    };
}

inline bool expect_symbol_25ecda935f91d894 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __itv_rename_at_373_65, json::TokenAt &  __ahead_rename_at_373_66, int32_t __sym_rename_at_373_67, char * & __error_rename_at_373_68 )
{
    if ( !_FuncbuiltinTicknextTick17450348357676149856_6a504e7a405e82ab(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_373_65),das_arg<json::TokenAt>::pass(__ahead_rename_at_373_66)) )
    {
        das_copy(__error_rename_at_373_68,((char *) "unexected eos"));
        return das_auto_cast<bool>::cast(false);
    } else if ( !das_get_variant_field<int32_t,8,5>::is(__ahead_rename_at_373_66.value) )
    {
        das_copy(__error_rename_at_373_68,das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_4, cast<char * const >::from(((char *) "unexpected ")), cast<TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_373_66.value), cast<char * const >::from(((char *) ", expecting symbol at ")), cast<int32_t>::from(__ahead_rename_at_373_66.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_373_66.row))));
        return das_auto_cast<bool>::cast(false);
    } else if ( !(das_get_variant_field<int32_t,8,5>::as(__ahead_rename_at_373_66.value,__context__) == __sym_rename_at_373_67) )
    {
        das_copy(__error_rename_at_373_68,das_string_builder(__context__,SimNode_AotInterop<10>(__tinfo_5, cast<char * const >::from(((char *) "unexpected ")), cast<TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_373_66.value), cast<char * const >::from(((char *) ", expecting symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sym_rename_at_373_67,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sym_rename_at_373_67), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_373_66.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_373_66.row))));
        return das_auto_cast<bool>::cast(false);
    } else {
        return das_auto_cast<bool>::cast(true);
    };
}

inline json::JsonValue * parse_value_ee908ba679ac2739 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __itv_rename_at_389_69, char * & __error_rename_at_389_70 )
{
    json::TokenAt __ahead_rename_at_391_71;das_zero(__ahead_rename_at_391_71);
    if ( !_FuncbuiltinTicknextTick17450348357676149856_6a504e7a405e82ab(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69),das_arg<json::TokenAt>::pass(__ahead_rename_at_391_71)) )
    {
        return das_auto_cast<json::JsonValue *>::cast(nullptr);
    } else {
        if ( das_get_variant_field<int32_t,8,5>::is(__ahead_rename_at_391_71.value) )
        {
            int32_t __sym_rename_at_396_72 = ((int32_t)das_get_variant_field<int32_t,8,5>::as(__ahead_rename_at_391_71.value,__context__));
            if ( __sym_rename_at_396_72 == 93 )
            {
                das_copy(__error_rename_at_389_70,das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_6, cast<char * const >::from(((char *) "unexpected ] at ")), cast<int32_t>::from(__ahead_rename_at_391_71.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_391_71.row))));
                return das_auto_cast<json::JsonValue *>::cast(nullptr);
            } else {
                if ( __sym_rename_at_396_72 == 91 )
                {
                    TArray<json::JsonValue *> __arr_rename_at_402_73;das_zero(__arr_rename_at_402_73);
                    while ( !builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69)) )
                    {
                        json::JsonValue * __value_rename_at_404_74 = parse_value_ee908ba679ac2739(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69),__error_rename_at_389_70);
                        if ( __value_rename_at_404_74 == nullptr )
                        {
                            if ( builtin_string_startswith(__error_rename_at_389_70,((char *) "unexpected ]"),__context__) && (builtin_array_size(das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_402_73)) == 0) )
                            {
                                das_copy(__error_rename_at_389_70,nullptr);
                                return das_auto_cast<json::JsonValue *>::cast(JV_c828edc46b7e6b3f(__context__,das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_402_73)));
                            } else {
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            };
                        };
                        _FuncbuiltinTickpushTick10769833213962245646_64687faa0a9f918a(__context__,das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_402_73),__value_rename_at_404_74);
                        if ( !expect_token_92ddcaa262ab835a(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69),das_arg<json::TokenAt>::pass(__ahead_rename_at_391_71),5,__error_rename_at_389_70) )
                        {
                            return das_auto_cast<json::JsonValue *>::cast(nullptr);
                        } else {
                            int32_t __sepsym_rename_at_416_75 = ((int32_t)das_get_variant_field<int32_t,8,5>::as(__ahead_rename_at_391_71.value,__context__));
                            if ( __sepsym_rename_at_416_75 == 93 )
                            {
                                break;
                            } else if ( __sepsym_rename_at_416_75 != 44 )
                            {
                                das_copy(__error_rename_at_389_70,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_7, cast<char * const >::from(((char *) "unexpected array separator symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sepsym_rename_at_416_75,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sepsym_rename_at_416_75), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_391_71.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_391_71.row))));
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            };
                        };
                    };
                    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69)) )
                    {
                        das_copy(__error_rename_at_389_70,((char *) "unexpected eos"));
                        return das_auto_cast<json::JsonValue *>::cast(nullptr);
                    } else {
                        return das_auto_cast<json::JsonValue *>::cast(JV_c828edc46b7e6b3f(__context__,das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_402_73)));
                    };
                } else if ( __sym_rename_at_396_72 == 123 )
                {
                    TTable<char *,json::JsonValue *> __tab_rename_at_430_76;das_zero(__tab_rename_at_430_76);
                    while ( !builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69)) )
                    {
                        if ( !expect_token_92ddcaa262ab835a(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69),das_arg<json::TokenAt>::pass(__ahead_rename_at_391_71),0,__error_rename_at_389_70) )
                        {
                            if ( (das_get_variant_field<int32_t,8,5>::is(__ahead_rename_at_391_71.value) && (das_get_variant_field<int32_t,8,5>::as(__ahead_rename_at_391_71.value,__context__) == 125)) && (builtin_table_size(das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_430_76)) == 0) )
                            {
                                das_copy(__error_rename_at_389_70,nullptr);
                                return das_auto_cast<json::JsonValue *>::cast(JV_6a8f99ad18c71d8b(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_430_76)));
                            } else {
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            };
                        };
                        char * __key_rename_at_439_77 = ((char *)(char *)(das_get_variant_field<char *,8,0>::as(__ahead_rename_at_391_71.value,__context__)));
                        if ( !expect_symbol_25ecda935f91d894(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69),das_arg<json::TokenAt>::pass(__ahead_rename_at_391_71),58,__error_rename_at_389_70) )
                        {
                            return das_auto_cast<json::JsonValue *>::cast(nullptr);
                        } else {
                            json::JsonValue * __value_rename_at_443_78 = parse_value_ee908ba679ac2739(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69),__error_rename_at_389_70);
                            if ( __value_rename_at_443_78 == nullptr )
                            {
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            } else {
                                if ( !das_global<bool,0x30e401a8d3210d31>(__context__) /*allow_duplicate_keys*/ && _FuncbuiltinTickkey_existsTick16808803843923989214_57fe3f8d782e30a7(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_430_76),__key_rename_at_439_77) )
                                {
                                    das_copy(__error_rename_at_389_70,das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_8, cast<char * const >::from(((char *) "duplicate key ")), cast<char * const >::from(__key_rename_at_439_77), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_391_71.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_391_71.row))));
                                    return das_auto_cast<json::JsonValue *>::cast(nullptr);
                                } else {
                                    _FuncbuiltinTickinsertTick4246857231018487965_146aa59a1c117ef2(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_430_76),__key_rename_at_439_77,__value_rename_at_443_78);
                                    if ( !expect_token_92ddcaa262ab835a(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69),das_arg<json::TokenAt>::pass(__ahead_rename_at_391_71),5,__error_rename_at_389_70) )
                                    {
                                        return das_auto_cast<json::JsonValue *>::cast(nullptr);
                                    } else {
                                        int32_t __sepsym_rename_at_455_79 = ((int32_t)das_get_variant_field<int32_t,8,5>::as(__ahead_rename_at_391_71.value,__context__));
                                        if ( __sepsym_rename_at_455_79 == 125 )
                                        {
                                            break;
                                        } else if ( __sepsym_rename_at_455_79 != 44 )
                                        {
                                            das_copy(__error_rename_at_389_70,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_7, cast<char * const >::from(((char *) "unexpected object separator symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sepsym_rename_at_455_79,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sepsym_rename_at_455_79), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_391_71.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_391_71.row))));
                                            return das_auto_cast<json::JsonValue *>::cast(nullptr);
                                        };
                                    };
                                };
                            };
                        };
                    };
                    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_389_69)) )
                    {
                        das_copy(__error_rename_at_389_70,((char *) "unexpected eos"));
                        return das_auto_cast<json::JsonValue *>::cast(nullptr);
                    } else {
                        return das_auto_cast<json::JsonValue *>::cast(JV_6a8f99ad18c71d8b(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_430_76)));
                    };
                } else {
                    das_copy(__error_rename_at_389_70,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_7, cast<char * const >::from(((char *) "unexpected symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sym_rename_at_396_72,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sym_rename_at_396_72), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_391_71.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_391_71.row))));
                    return das_auto_cast<json::JsonValue *>::cast(nullptr);
                };
            };
        } else if ( das_get_variant_field<char *,8,0>::is(__ahead_rename_at_391_71.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_b2c605dae25f2a45(__context__,das_get_variant_field<char *,8,0>::as(__ahead_rename_at_391_71.value,__context__)));
        } else if ( das_get_variant_field<double,8,1>::is(__ahead_rename_at_391_71.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_28ecc49916ce157a(__context__,das_get_variant_field<double,8,1>::as(__ahead_rename_at_391_71.value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,2>::is(__ahead_rename_at_391_71.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_89570eb3601bc343(__context__,das_get_variant_field<int64_t,8,2>::as(__ahead_rename_at_391_71.value,__context__)));
        } else if ( das_get_variant_field<bool,8,3>::is(__ahead_rename_at_391_71.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_3aa84b5b46540567(__context__,das_get_variant_field<bool,8,3>::as(__ahead_rename_at_391_71.value,__context__)));
        } else if ( das_get_variant_field<void *,8,4>::is(__ahead_rename_at_391_71.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JVNull_e01b92c701ceb486(__context__));
        } else {
            das_copy(__error_rename_at_389_70,das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_11, cast<TVariant<16,8,char *,double,int64_t,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_391_71.value), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_391_71.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_391_71.row))));
            return das_auto_cast<json::JsonValue *>::cast(nullptr);
        };
    };
}

inline json::JsonValue * read_json_1cf2494477255e77 ( Context * __context__, char * const  __text_rename_at_488_80, char * & __error_rename_at_488_81 )
{
    das_copy(__error_rename_at_488_81,nullptr);
    Sequence DAS_COMMENT((json::TokenAt)) __lex_rename_at_492_82; das_zero(__lex_rename_at_492_82); das_move(__lex_rename_at_492_82, lexer_b6e69875fdd15de2(__context__,__text_rename_at_488_80));
    json::JsonValue * __res_rename_at_493_83 = parse_value_ee908ba679ac2739(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_492_82),__error_rename_at_488_81);
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_492_82),__context__);
    return das_auto_cast<json::JsonValue *>::cast(__res_rename_at_493_83);
}

inline json::JsonValue * read_json_f5237b438b70759b ( Context * __context__, TArray<uint8_t> const  &  __text_rename_at_498_84, char * & __error_rename_at_498_85 )
{
    das_copy(__error_rename_at_498_85,nullptr);
    Sequence DAS_COMMENT((json::TokenAt)) __lex_rename_at_502_86; das_zero(__lex_rename_at_502_86); das_move(__lex_rename_at_502_86, lexer_7b01766a6e1e52b1(__context__,__text_rename_at_498_84));
    json::JsonValue * __res_rename_at_503_87 = parse_value_ee908ba679ac2739(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_502_86),__error_rename_at_498_85);
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_502_86),__context__);
    return das_auto_cast<json::JsonValue *>::cast(__res_rename_at_503_87);
}

inline bool set_no_trailing_zeros_f1b71a41030849b4 ( Context * __context__, bool __value_rename_at_510_88 )
{
    bool __old_ntz_rename_at_512_89 = ((bool)das_global<bool,0xd3b8b85b9bf19183>(__context__) /*no_trailing_zeros*/);
    das_copy(das_global<bool,0xd3b8b85b9bf19183>(__context__) /*no_trailing_zeros*/,__value_rename_at_510_88);
    return das_auto_cast<bool>::cast(__old_ntz_rename_at_512_89);
}

inline bool set_no_empty_arrays_cb9f007816624484 ( Context * __context__, bool __value_rename_at_519_90 )
{
    bool __old_nea_rename_at_521_91 = ((bool)das_global<bool,0xab7f08dbdeea5c03>(__context__) /*no_empty_arrays*/);
    das_copy(das_global<bool,0xab7f08dbdeea5c03>(__context__) /*no_empty_arrays*/,__value_rename_at_519_90);
    return das_auto_cast<bool>::cast(__old_nea_rename_at_521_91);
}

inline bool set_allow_duplicate_keys_b6a2085fcfe8cb80 ( Context * __context__, bool __value_rename_at_528_92 )
{
    bool __old_adc_rename_at_530_93 = ((bool)das_global<bool,0x30e401a8d3210d31>(__context__) /*allow_duplicate_keys*/);
    das_copy(das_global<bool,0x30e401a8d3210d31>(__context__) /*allow_duplicate_keys*/,__value_rename_at_528_92);
    return das_auto_cast<bool>::cast(__old_adc_rename_at_530_93);
}

inline void write_value_171bdd0fdeefc56b ( Context * __context__, StringBuilderWriter &  __writer_rename_at_536_94, json::JsonValue * const  __jsv_rename_at_536_95, int32_t __depth_rename_at_536_96 )
{
    if ( __jsv_rename_at_536_95 == nullptr )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "null"))));
    } else if ( das_get_variant_field<char *,8,2>::is(__jsv_rename_at_536_95->value) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "\""))));
        write_escape_string(das_arg<StringBuilderWriter>::pass(__writer_rename_at_536_94),das_get_variant_field<char *,8,2>::as(__jsv_rename_at_536_95->value,__context__));
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "\""))));
    } else if ( das_get_variant_field<double,8,3>::is(__jsv_rename_at_536_95->value) )
    {
        if ( das_global<bool,0xd3b8b85b9bf19183>(__context__) /*no_trailing_zeros*/ )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char * const )(builtin_string_rtrim_ts(((char * const )(builtin_string_rtrim_ts(((char * const )(fmt_d(((char *) ":.17f"),das_get_variant_field<double,8,3>::as(__jsv_rename_at_536_95->value,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "0"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "."),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_16,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<double>::from(das_get_variant_field<double,8,3>::as(__jsv_rename_at_536_95->value,__context__))));
        };
    } else if ( das_get_variant_field<int64_t,8,4>::is(__jsv_rename_at_536_95->value) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<int64_t>::from(das_get_variant_field<int64_t,8,4>::as(__jsv_rename_at_536_95->value,__context__))));
    } else if ( das_get_variant_field<TArray<json::JsonValue *>,8,1>::is(__jsv_rename_at_536_95->value) )
    {
        if ( builtin_array_size(das_get_variant_field<TArray<json::JsonValue *>,8,1>::as(__jsv_rename_at_536_95->value,__context__)) == 0 )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "[]"))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "[\n"))));
            bool __first_rename_at_557_97 = true;
            {
                bool __need_loop_558 = true;
                // elem: json::JsonValue? const&
                das_iterator<TArray<json::JsonValue *> const > __elem_iterator(das_get_variant_field<TArray<json::JsonValue *>,8,1>::as(__jsv_rename_at_536_95->value,__context__));
                json::JsonValue * const  * __elem_rename_at_558_98;
                __need_loop_558 = __elem_iterator.first(__context__,(__elem_rename_at_558_98)) && __need_loop_558;
                for ( ; __need_loop_558 ; __need_loop_558 = __elem_iterator.next(__context__,(__elem_rename_at_558_98)) )
                {
                    if ( __first_rename_at_557_97 )
                    {
                        das_copy(__first_rename_at_557_97,false);
                    } else {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) ",\n"))));
                    };
                    write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_536_94),9,__depth_rename_at_536_96 + 1);
                    write_value_171bdd0fdeefc56b(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_536_94),(*__elem_rename_at_558_98),__depth_rename_at_536_96 + 1);
                }
                __elem_iterator.close(__context__,(__elem_rename_at_558_98));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "\n"))));
            write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_536_94),9,__depth_rename_at_536_96);
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "]"))));
        };
    } else if ( das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::is(__jsv_rename_at_536_95->value) )
    {
        if ( builtin_table_size(das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__jsv_rename_at_536_95->value,__context__)) == 0 )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "{}"))));
        } else {
            Sequence DAS_COMMENT((char *)) _temp_make_local_577_33_0; _temp_make_local_577_33_0;
            Sequence DAS_COMMENT((json::JsonValue * *)) _temp_make_local_577_61_1; _temp_make_local_577_61_1;
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "{\n"))));
            bool __first_rename_at_576_99 = true;
            {
                bool __need_loop_577 = true;
                // elemK: string
                das_iterator<Sequence DAS_COMMENT((char *))> __elemK_iterator((_temp_make_local_577_33_0 = (_FuncbuiltinTickkeysTick3875137551537175793_d3a3aed2f5c8bde1(__context__,das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__jsv_rename_at_536_95->value,__context__)))));
                char * __elemK_rename_at_577_102;
                __need_loop_577 = __elemK_iterator.first(__context__,(__elemK_rename_at_577_102)) && __need_loop_577;
                // elemV: json::JsonValue? const&
                das_iterator<Sequence DAS_COMMENT((json::JsonValue * const ))> __elemV_iterator((_temp_make_local_577_61_1 = (_FuncbuiltinTickvaluesTick1935193042646774172_1c966248b122cc69(__context__,das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__jsv_rename_at_536_95->value,__context__)))));
                json::JsonValue * const  * __elemV_rename_at_577_103;
                __need_loop_577 = __elemV_iterator.first(__context__,(__elemV_rename_at_577_103)) && __need_loop_577;
                for ( ; __need_loop_577 ; __need_loop_577 = __elemK_iterator.next(__context__,(__elemK_rename_at_577_102)) && __elemV_iterator.next(__context__,(__elemV_rename_at_577_103)) )
                {
                    if ( das_global<bool,0xab7f08dbdeea5c03>(__context__) /*no_empty_arrays*/ )
                    {
                        if ( das_get_variant_field<TArray<json::JsonValue *>,8,1>::is((*__elemV_rename_at_577_103)->value) )
                        {
                            if ( builtin_array_size(das_get_variant_field<TArray<json::JsonValue *>,8,1>::as((*__elemV_rename_at_577_103)->value,__context__)) == 0 )
                            {
                                continue;
                            };
                        };
                    };
                    if ( __first_rename_at_576_99 )
                    {
                        das_copy(__first_rename_at_576_99,false);
                    } else {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) ",\n"))));
                    };
                    write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_536_94),9,__depth_rename_at_536_96 + 1);
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "\""))));
                    write_escape_string(das_arg<StringBuilderWriter>::pass(__writer_rename_at_536_94),__elemK_rename_at_577_102);
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "\" : "))));
                    write_value_171bdd0fdeefc56b(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_536_94),(*__elemV_rename_at_577_103),__depth_rename_at_536_96 + 1);
                }
                __elemV_iterator.close(__context__,(__elemV_rename_at_577_103));
                __elemK_iterator.close(__context__,(__elemK_rename_at_577_102));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "\n"))));
            write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_536_94),9,__depth_rename_at_536_96);
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "}"))));
        };
    } else if ( das_get_variant_field<bool,8,5>::is(__jsv_rename_at_536_95->value) )
    {
        if ( das_get_variant_field<bool,8,5>::as(__jsv_rename_at_536_95->value,__context__) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "true"))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "false"))));
        };
    } else if ( das_get_variant_field<void *,8,6>::is(__jsv_rename_at_536_95->value) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_536_94),cast<char * const >::from(((char *) "null"))));
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_33, cast<char * const >::from(((char *) "unexpected ")), cast<json::JsonValue * const >::from(__jsv_rename_at_536_95))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline char * write_json_571c90b858deacc ( Context * __context__, json::JsonValue * const  __val_rename_at_613_104 )
{
    char * __st_rename_at_615_105 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_615_106) DAS_AOT_INLINE_LAMBDA -> void{
        write_value_171bdd0fdeefc56b(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_615_106),__val_rename_at_613_104,0);
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_615_105);
}

inline char * write_json_e91db05e9e89ab6f ( Context * __context__, json::JsonValue * const  __val_rename_at_621_107 )
{
    return das_auto_cast<char *>::cast(write_json_571c90b858deacc(__context__,das_cast<json::JsonValue *>::cast(__val_rename_at_621_107)));
}

inline char * try_fixing_broken_json_ef0b3a4bda8a0603 ( Context * __context__, char * __bad_rename_at_628_108 ) { das_stack_prologue __prologue(__context__,272,"try_fixing_broken_json " DAS_FILE_LINE);
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_634_109) DAS_AOT_INLINE_LAMBDA -> void{
        builtin_string_peek(__bad_rename_at_628_108,das_make_block<void,TArray<uint8_t> const  &>(__context__,144,0,&__func_info__5a16898e20f97124,[&](TArray<uint8_t> const  & __str_rename_at_635_110) -> void{
            int32_t __i_rename_at_636_111 = 0;
            int32_t __lstr_rename_at_637_112 = ((int32_t)builtin_array_size(__str_rename_at_635_110));
            while ( __i_rename_at_636_111 < __lstr_rename_at_637_112 )
            {
                while ( (__i_rename_at_636_111 < __lstr_rename_at_637_112) && (das_nequ_val(__str_rename_at_635_110(__i_rename_at_636_111,__context__),0x22)) )
                {
                    if ( das_equ_val(__str_rename_at_635_110(__i_rename_at_636_111,__context__),0x2c) )
                    {
                        int32_t __j_rename_at_642_113 = (__i_rename_at_636_111 + 1);
                        while ( (__j_rename_at_642_113 < __lstr_rename_at_637_112) && is_white_space(int32_t(__str_rename_at_635_110(__j_rename_at_642_113,__context__))) )
                        {
                            ++__j_rename_at_642_113;
                        };
                        if ( (__j_rename_at_642_113 < __lstr_rename_at_637_112) && ((das_equ_val(__str_rename_at_635_110(__j_rename_at_642_113,__context__),0x7d)) || (das_equ_val(__str_rename_at_635_110(__j_rename_at_642_113,__context__),0x5d))) )
                        {
                            das_copy(__i_rename_at_636_111,__j_rename_at_642_113);
                            continue;
                        };
                    };
                    if ( (das_equ_val(__str_rename_at_635_110(__i_rename_at_636_111,__context__),0x5c)) && (das_equ_val(__str_rename_at_635_110((__i_rename_at_636_111 + 1),__context__),0x75)) )
                    {
                        __i_rename_at_636_111 += 2;
                        while ( (__i_rename_at_636_111 < __lstr_rename_at_637_112) && is_hex(int32_t(__str_rename_at_635_110(__i_rename_at_636_111,__context__))) )
                        {
                            ++__i_rename_at_636_111;
                        };
                        continue;
                    } else {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),int32_t(__str_rename_at_635_110(__i_rename_at_636_111,__context__)));
                        ++__i_rename_at_636_111;
                    };
                };
                if ( __i_rename_at_636_111 >= __lstr_rename_at_637_112 )
                {
                    break;
                } else {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),34);
                    ++__i_rename_at_636_111;
                    label_0:;;
                    while ( (__i_rename_at_636_111 < __lstr_rename_at_637_112) && (das_nequ_val(__str_rename_at_635_110(__i_rename_at_636_111,__context__),0x22)) )
                    {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),int32_t(__str_rename_at_635_110(__i_rename_at_636_111,__context__)));
                        ++__i_rename_at_636_111;
                    };
                    if ( __i_rename_at_636_111 >= __lstr_rename_at_637_112 )
                    {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),34);
                        break;
                    } else {
                        int32_t __nested_rename_at_678_114 = ((int32_t)__i_rename_at_636_111);
                        int32_t __j_rename_at_679_115 = (__i_rename_at_636_111 + 1);
                        bool __any_white_space_rename_at_680_116 = ((bool)((__j_rename_at_679_115 < __lstr_rename_at_637_112) && is_white_space(int32_t(__str_rename_at_635_110(__j_rename_at_679_115,__context__)))));
                        while ( (__j_rename_at_679_115 < __lstr_rename_at_637_112) && is_white_space(int32_t(__str_rename_at_635_110(__j_rename_at_679_115,__context__))) )
                        {
                            ++__j_rename_at_679_115;
                        };
                        if ( __j_rename_at_679_115 >= __lstr_rename_at_637_112 )
                        {
                            write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),34);
                            break;
                        } else {
                            int32_t __strj_rename_at_689_117 = ((int32_t)int32_t(__str_rename_at_635_110(__j_rename_at_679_115,__context__)));
                            if ( (((__strj_rename_at_689_117 == 58) || (__strj_rename_at_689_117 == 125)) || (__strj_rename_at_689_117 == 93)) || (__strj_rename_at_689_117 == 44) )
                            {
                                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),34);
                                ++__i_rename_at_636_111;
                                continue;
                            } else {
                                if ( __strj_rename_at_689_117 == 43 )
                                {
                                    ++__j_rename_at_679_115;
                                    while ( (__j_rename_at_679_115 < __lstr_rename_at_637_112) && is_white_space(int32_t(__str_rename_at_635_110(__j_rename_at_679_115,__context__))) )
                                    {
                                        ++__j_rename_at_679_115;
                                    };
                                    if ( (__j_rename_at_679_115 < __lstr_rename_at_637_112) && (das_equ_val(__str_rename_at_635_110(__j_rename_at_679_115,__context__),0x22)) )
                                    {
                                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),32);
                                        das_copy(__i_rename_at_636_111,__j_rename_at_679_115 + 1);
                                        goto label_0;
                                    };
                                };
                                if ( __any_white_space_rename_at_680_116 )
                                {
                                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),34);
                                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),44);
                                    das_copy(__i_rename_at_636_111,__nested_rename_at_678_114 + 1);
                                    continue;
                                } else {
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_634_109),cast<char * const >::from(((char *) "`"))));
                                    das_copy(__i_rename_at_636_111,__nested_rename_at_678_114 + 1);
                                    while ( (__i_rename_at_636_111 < __lstr_rename_at_637_112) && (das_nequ_val(__str_rename_at_635_110(__i_rename_at_636_111,__context__),0x22)) )
                                    {
                                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),int32_t(__str_rename_at_635_110(__i_rename_at_636_111,__context__)));
                                        ++__i_rename_at_636_111;
                                    };
                                    if ( __i_rename_at_636_111 >= __lstr_rename_at_637_112 )
                                    {
                                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_634_109),cast<char * const >::from(((char *) "`"))));
                                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_634_109),34);
                                        break;
                                    } else {
                                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_634_109),cast<char * const >::from(((char *) "`"))));
                                        ++__i_rename_at_636_111;
                                        goto label_0;
                                    };
                                };
                            };
                        };
                    };
                };
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}}
static vec4f __wrap__FuncbuiltinTickfinalizeTick13836114024949725080_5db697d6f0792e58 ( Context * __context__ ) {
    TArray<uint8_t> &  arg_a = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    _FuncbuiltinTickfinalizeTick13836114024949725080_5db697d6f0792e58(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickeachTick4044333107478717573_6f02beb659b38015 ( Context * __context__ ) {
    range arg_rng = cast_aot_arg<range>::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((int32_t)) *) __context__->abiCMRES) = _FuncbuiltinTickeachTick4044333107478717573_6f02beb659b38015(__context__, arg_rng);
    return v_zero();
}
static vec4f __wrap_finalize_25878f262ceb6fd ( Context * __context__ ) {
    json::_lambda_json_180_1 &  arg___this = cast_aot_arg<json::_lambda_json_180_1 & >::to(*__context__,__context__->abiArguments()[0]);
    finalize_25878f262ceb6fd(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickeachTick9663565701927713696_6ea716cdee8c36ed ( Context * __context__ ) {
    Lambda DAS_COMMENT((bool,json::TokenAt)) const  arg_lam = cast_aot_arg<Lambda DAS_COMMENT((bool,json::TokenAt)) const >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::TokenAt)) *) __context__->abiCMRES) = _FuncbuiltinTickeachTick9663565701927713696_6ea716cdee8c36ed(__context__, arg_lam);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTicknextTick17450348357676149856_c6c276e75c57546 ( Context * __context__ ) {
    Sequence DAS_COMMENT((int32_t)) &  arg_it = cast_aot_arg<Sequence DAS_COMMENT((int32_t)) & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t & arg_value = cast_aot_arg<int32_t &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncbuiltinTicknextTick17450348357676149856_c6c276e75c57546(__context__, arg_it, arg_value));
}
static vec4f __wrap__FuncbuiltinTickeachTick4044332007967089362_3807b33f4b4a44b2 ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((int32_t)) *) __context__->abiCMRES) = _FuncbuiltinTickeachTick4044332007967089362_3807b33f4b4a44b2(__context__, arg_str);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8 ( Context * __context__ ) {
    TArray<uint8_t> &  arg_Arr = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    uint8_t arg_value = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_89eb839cbd84f567 ( Context * __context__ ) {
    TArray<uint8_t> &  arg_Arr = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    uint8_t arg_value = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_89eb839cbd84f567(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__Func_lambda_json_180_1Tickfunction_3fa1de895736605c ( Context * __context__ ) {
    json::_lambda_json_180_1 &  arg___this = cast_aot_arg<json::_lambda_json_180_1 & >::to(*__context__,__context__->abiArguments()[0]);
    json::TokenAt &  arg__yield_180 = cast_aot_arg<json::TokenAt & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_Func_lambda_json_180_1Tickfunction_3fa1de895736605c(__context__, arg___this, arg__yield_180));
}
static vec4f __wrap__Func_lambda_json_180_1Tickfinalizer_caa1114ef8726b27 ( Context * __context__ ) {
    json::_lambda_json_180_1 * arg___this = cast_aot_arg<json::_lambda_json_180_1 *>::to(*__context__,__context__->abiArguments()[0]);
    _Func_lambda_json_180_1Tickfinalizer_caa1114ef8726b27(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTicknextTick17450348357676149856_6a504e7a405e82ab ( Context * __context__ ) {
    Sequence DAS_COMMENT((json::TokenAt)) &  arg_it = cast_aot_arg<Sequence DAS_COMMENT((json::TokenAt)) & >::to(*__context__,__context__->abiArguments()[0]);
    json::TokenAt &  arg_value = cast_aot_arg<json::TokenAt & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncbuiltinTicknextTick17450348357676149856_6a504e7a405e82ab(__context__, arg_it, arg_value));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_64687faa0a9f918a ( Context * __context__ ) {
    TArray<json::JsonValue *> &  arg_Arr = cast_aot_arg<TArray<json::JsonValue *> & >::to(*__context__,__context__->abiArguments()[0]);
    json::JsonValue * arg_value = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_64687faa0a9f918a(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickkey_existsTick16808803843923989214_57fe3f8d782e30a7 ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> const  &  arg_Tab = cast_aot_arg<TTable<char *,json::JsonValue *> const  & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_at = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncbuiltinTickkey_existsTick16808803843923989214_57fe3f8d782e30a7(__context__, arg_Tab, arg_at));
}
static vec4f __wrap__FuncbuiltinTickinsertTick4246857231018487965_146aa59a1c117ef2 ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> &  arg_Tab = cast_aot_arg<TTable<char *,json::JsonValue *> & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_at = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    json::JsonValue * arg_val = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[2]);
    _FuncbuiltinTickinsertTick4246857231018487965_146aa59a1c117ef2(__context__, arg_Tab, arg_at, arg_val);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickkeysTick3875137551537175793_d3a3aed2f5c8bde1 ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> const  &  arg_a = cast_aot_arg<TTable<char *,json::JsonValue *> const  & >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((char *)) *) __context__->abiCMRES) = _FuncbuiltinTickkeysTick3875137551537175793_d3a3aed2f5c8bde1(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickvaluesTick1935193042646774172_1c966248b122cc69 ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> const  &  arg_a = cast_aot_arg<TTable<char *,json::JsonValue *> const  & >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::JsonValue * &)) *) __context__->abiCMRES) = _FuncbuiltinTickvaluesTick1935193042646774172_1c966248b122cc69(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap_JVNull_e01b92c701ceb486 ( Context * __context__ ) {
    return cast<json::JsonValue *>::from(JVNull_e01b92c701ceb486(__context__));
}
static vec4f __wrap_JV_b2c605dae25f2a45 ( Context * __context__ ) {
    char * const  arg_v = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_b2c605dae25f2a45(__context__, arg_v));
}
static vec4f __wrap_JV_28ecc49916ce157a ( Context * __context__ ) {
    double arg_v = cast_aot_arg<double>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_28ecc49916ce157a(__context__, arg_v));
}
static vec4f __wrap_JV_3aa84b5b46540567 ( Context * __context__ ) {
    bool arg_v = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_3aa84b5b46540567(__context__, arg_v));
}
static vec4f __wrap_JV_6a8f99ad18c71d8b ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> &  arg_v = cast_aot_arg<TTable<char *,json::JsonValue *> & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_6a8f99ad18c71d8b(__context__, arg_v));
}
static vec4f __wrap_JV_c828edc46b7e6b3f ( Context * __context__ ) {
    TArray<json::JsonValue *> &  arg_v = cast_aot_arg<TArray<json::JsonValue *> & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_c828edc46b7e6b3f(__context__, arg_v));
}
static vec4f __wrap_JV_d7eb81af2b7b4a86 ( Context * __context__ ) {
    float arg_v = cast_aot_arg<float>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_d7eb81af2b7b4a86(__context__, arg_v));
}
static vec4f __wrap_JV_636c78630f7810d4 ( Context * __context__ ) {
    int32_t arg_v = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_636c78630f7810d4(__context__, arg_v));
}
static vec4f __wrap_JV_d5ea1665ba780d52 ( Context * __context__ ) {
    Bitfield arg_v = cast_aot_arg<Bitfield>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_d5ea1665ba780d52(__context__, arg_v));
}
static vec4f __wrap_JV_7fe888aac9cdba7b ( Context * __context__ ) {
    Bitfield8 arg_v = cast_aot_arg<Bitfield8>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_7fe888aac9cdba7b(__context__, arg_v));
}
static vec4f __wrap_JV_f9b00e1a90cb16e0 ( Context * __context__ ) {
    Bitfield16 arg_v = cast_aot_arg<Bitfield16>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_f9b00e1a90cb16e0(__context__, arg_v));
}
static vec4f __wrap_JV_22b8a5c944277732 ( Context * __context__ ) {
    Bitfield64 arg_v = cast_aot_arg<Bitfield64>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_22b8a5c944277732(__context__, arg_v));
}
static vec4f __wrap_JV_8a82057e10483a29 ( Context * __context__ ) {
    int8_t arg_val = cast_aot_arg<int8_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_8a82057e10483a29(__context__, arg_val));
}
static vec4f __wrap_JV_4bb097533696a50b ( Context * __context__ ) {
    uint8_t arg_val = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_4bb097533696a50b(__context__, arg_val));
}
static vec4f __wrap_JV_623fdb61479cb328 ( Context * __context__ ) {
    int16_t arg_val = cast_aot_arg<int16_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_623fdb61479cb328(__context__, arg_val));
}
static vec4f __wrap_JV_e94e08dbede97c15 ( Context * __context__ ) {
    uint16_t arg_val = cast_aot_arg<uint16_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_e94e08dbede97c15(__context__, arg_val));
}
static vec4f __wrap_JV_5d29b737db96a9a5 ( Context * __context__ ) {
    uint32_t arg_val = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_5d29b737db96a9a5(__context__, arg_val));
}
static vec4f __wrap_JV_89570eb3601bc343 ( Context * __context__ ) {
    int64_t arg_val = cast_aot_arg<int64_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_89570eb3601bc343(__context__, arg_val));
}
static vec4f __wrap_JV_f13c60740e17c9cb ( Context * __context__ ) {
    uint64_t arg_val = cast_aot_arg<uint64_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_f13c60740e17c9cb(__context__, arg_val));
}
static vec4f __wrap_lexer_b6e69875fdd15de2 ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::TokenAt)) *) __context__->abiCMRES) = lexer_b6e69875fdd15de2(__context__, arg_text);
    return v_zero();
}
static vec4f __wrap_lexer_7b01766a6e1e52b1 ( Context * __context__ ) {
    TArray<uint8_t> const  &  arg_text = cast_aot_arg<TArray<uint8_t> const  & >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::TokenAt)) *) __context__->abiCMRES) = lexer_7b01766a6e1e52b1(__context__, arg_text);
    return v_zero();
}
static vec4f __wrap_next_f03600350289290 ( Context * __context__ ) {
    Sequence DAS_COMMENT((int32_t)) &  arg_text = cast_aot_arg<Sequence DAS_COMMENT((int32_t)) & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t & arg_character = cast_aot_arg<int32_t &>::to(*__context__,__context__->abiArguments()[1]);
    int32_t & arg_line = cast_aot_arg<int32_t &>::to(*__context__,__context__->abiArguments()[2]);
    int32_t & arg_row = cast_aot_arg<int32_t &>::to(*__context__,__context__->abiArguments()[3]);
    return cast<bool>::from(next_f03600350289290(__context__, arg_text, arg_character, arg_line, arg_row));
}
static vec4f __wrap__lexer_d7025220389b96e7 ( Context * __context__ ) {
    char * arg_stext = cast_aot_arg<char *>::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::TokenAt)) *) __context__->abiCMRES) = _lexer_d7025220389b96e7(__context__, arg_stext);
    return v_zero();
}
static vec4f __wrap_expect_token_92ddcaa262ab835a ( Context * __context__ ) {
    Sequence DAS_COMMENT((json::TokenAt)) &  arg_itv = cast_aot_arg<Sequence DAS_COMMENT((json::TokenAt)) & >::to(*__context__,__context__->abiArguments()[0]);
    json::TokenAt &  arg_ahead = cast_aot_arg<json::TokenAt & >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_vindex = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[3]);
    return cast<bool>::from(expect_token_92ddcaa262ab835a(__context__, arg_itv, arg_ahead, arg_vindex, arg_error));
}
static vec4f __wrap_expect_symbol_25ecda935f91d894 ( Context * __context__ ) {
    Sequence DAS_COMMENT((json::TokenAt)) &  arg_itv = cast_aot_arg<Sequence DAS_COMMENT((json::TokenAt)) & >::to(*__context__,__context__->abiArguments()[0]);
    json::TokenAt &  arg_ahead = cast_aot_arg<json::TokenAt & >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_sym = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[3]);
    return cast<bool>::from(expect_symbol_25ecda935f91d894(__context__, arg_itv, arg_ahead, arg_sym, arg_error));
}
static vec4f __wrap_parse_value_ee908ba679ac2739 ( Context * __context__ ) {
    Sequence DAS_COMMENT((json::TokenAt)) &  arg_itv = cast_aot_arg<Sequence DAS_COMMENT((json::TokenAt)) & >::to(*__context__,__context__->abiArguments()[0]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(parse_value_ee908ba679ac2739(__context__, arg_itv, arg_error));
}
static vec4f __wrap_read_json_1cf2494477255e77 ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(read_json_1cf2494477255e77(__context__, arg_text, arg_error));
}
static vec4f __wrap_read_json_f5237b438b70759b ( Context * __context__ ) {
    TArray<uint8_t> const  &  arg_text = cast_aot_arg<TArray<uint8_t> const  & >::to(*__context__,__context__->abiArguments()[0]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(read_json_f5237b438b70759b(__context__, arg_text, arg_error));
}
static vec4f __wrap_set_no_trailing_zeros_f1b71a41030849b4 ( Context * __context__ ) {
    bool arg_value = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(set_no_trailing_zeros_f1b71a41030849b4(__context__, arg_value));
}
static vec4f __wrap_set_no_empty_arrays_cb9f007816624484 ( Context * __context__ ) {
    bool arg_value = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(set_no_empty_arrays_cb9f007816624484(__context__, arg_value));
}
static vec4f __wrap_set_allow_duplicate_keys_b6a2085fcfe8cb80 ( Context * __context__ ) {
    bool arg_value = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(set_allow_duplicate_keys_b6a2085fcfe8cb80(__context__, arg_value));
}
static vec4f __wrap_write_value_171bdd0fdeefc56b ( Context * __context__ ) {
    StringBuilderWriter &  arg_writer = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[0]);
    json::JsonValue * const  arg_jsv = cast_aot_arg<json::JsonValue * const >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_depth = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    write_value_171bdd0fdeefc56b(__context__, arg_writer, arg_jsv, arg_depth);
    return v_zero();
}
static vec4f __wrap_write_json_571c90b858deacc ( Context * __context__ ) {
    json::JsonValue * const  arg_val = cast_aot_arg<json::JsonValue * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(write_json_571c90b858deacc(__context__, arg_val));
}
static vec4f __wrap_write_json_e91db05e9e89ab6f ( Context * __context__ ) {
    json::JsonValue * const  arg_val = cast_aot_arg<json::JsonValue * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(write_json_e91db05e9e89ab6f(__context__, arg_val));
}
static vec4f __wrap_try_fixing_broken_json_ef0b3a4bda8a0603 ( Context * __context__ ) {
    char * arg_bad = cast_aot_arg<char *>::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(try_fixing_broken_json_ef0b3a4bda8a0603(__context__, arg_bad));
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0x18b4eceb8d41d1bd, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_5db697d6f0792e58, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_5db697d6f0792e58 },
    { 0xf426d9532c77b0a4, true, (void*)&_FuncbuiltinTickeachTick4044333107478717573_6f02beb659b38015, &__wrap__FuncbuiltinTickeachTick4044333107478717573_6f02beb659b38015 },
    { 0xdd44a48e4e3faac7, false, (void*)&finalize_25878f262ceb6fd, &__wrap_finalize_25878f262ceb6fd },
    { 0x8f3d3c45209b5453, true, (void*)&_FuncbuiltinTickeachTick9663565701927713696_6ea716cdee8c36ed, &__wrap__FuncbuiltinTickeachTick9663565701927713696_6ea716cdee8c36ed },
    { 0x510ab8f8c291da6e, false, (void*)&_FuncbuiltinTicknextTick17450348357676149856_c6c276e75c57546, &__wrap__FuncbuiltinTicknextTick17450348357676149856_c6c276e75c57546 },
    { 0x7a2016b87f6f7c12, true, (void*)&_FuncbuiltinTickeachTick4044332007967089362_3807b33f4b4a44b2, &__wrap__FuncbuiltinTickeachTick4044332007967089362_3807b33f4b4a44b2 },
    { 0x89792d67359db3ab, false, (void*)&_FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8, &__wrap__FuncbuiltinTickpushTick14133213201864676143_a295abf396ef30f8 },
    { 0x6895eeb45ac7fb7d, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_89eb839cbd84f567, &__wrap__FuncbuiltinTickpushTick10769833213962245646_89eb839cbd84f567 },
    { 0xca6d81346e38141c, false, (void*)&_Func_lambda_json_180_1Tickfunction_3fa1de895736605c, &__wrap__Func_lambda_json_180_1Tickfunction_3fa1de895736605c },
    { 0x2bb42f348e5e7919, false, (void*)&_Func_lambda_json_180_1Tickfinalizer_caa1114ef8726b27, &__wrap__Func_lambda_json_180_1Tickfinalizer_caa1114ef8726b27 },
    { 0x2eb9466837740670, false, (void*)&_FuncbuiltinTicknextTick17450348357676149856_6a504e7a405e82ab, &__wrap__FuncbuiltinTicknextTick17450348357676149856_6a504e7a405e82ab },
    { 0x613cdd6160ba8c54, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_64687faa0a9f918a, &__wrap__FuncbuiltinTickpushTick10769833213962245646_64687faa0a9f918a },
    { 0x3ce69c2067c0d50b, false, (void*)&_FuncbuiltinTickkey_existsTick16808803843923989214_57fe3f8d782e30a7, &__wrap__FuncbuiltinTickkey_existsTick16808803843923989214_57fe3f8d782e30a7 },
    { 0xe697c01c5e24b2ec, false, (void*)&_FuncbuiltinTickinsertTick4246857231018487965_146aa59a1c117ef2, &__wrap__FuncbuiltinTickinsertTick4246857231018487965_146aa59a1c117ef2 },
    { 0xc7322764b58ccb97, true, (void*)&_FuncbuiltinTickkeysTick3875137551537175793_d3a3aed2f5c8bde1, &__wrap__FuncbuiltinTickkeysTick3875137551537175793_d3a3aed2f5c8bde1 },
    { 0x9b740b47e079ae69, true, (void*)&_FuncbuiltinTickvaluesTick1935193042646774172_1c966248b122cc69, &__wrap__FuncbuiltinTickvaluesTick1935193042646774172_1c966248b122cc69 },
    { 0xd6514e9d8acb515b, false, (void*)&JVNull_e01b92c701ceb486, &__wrap_JVNull_e01b92c701ceb486 },
    { 0xd6e507a200a5db2e, false, (void*)&JV_b2c605dae25f2a45, &__wrap_JV_b2c605dae25f2a45 },
    { 0x39c3fd3257310da1, false, (void*)&JV_28ecc49916ce157a, &__wrap_JV_28ecc49916ce157a },
    { 0x3533afad0885ff41, false, (void*)&JV_3aa84b5b46540567, &__wrap_JV_3aa84b5b46540567 },
    { 0xd3511687e409d26e, false, (void*)&JV_6a8f99ad18c71d8b, &__wrap_JV_6a8f99ad18c71d8b },
    { 0xb0fd257822f17ed2, false, (void*)&JV_c828edc46b7e6b3f, &__wrap_JV_c828edc46b7e6b3f },
    { 0x3237a0e6f37a7e22, false, (void*)&JV_d7eb81af2b7b4a86, &__wrap_JV_d7eb81af2b7b4a86 },
    { 0x23c89760c22dba8e, false, (void*)&JV_636c78630f7810d4, &__wrap_JV_636c78630f7810d4 },
    { 0x4aecf1d83f30694f, false, (void*)&JV_d5ea1665ba780d52, &__wrap_JV_d5ea1665ba780d52 },
    { 0x6d4516a7632b4208, false, (void*)&JV_7fe888aac9cdba7b, &__wrap_JV_7fe888aac9cdba7b },
    { 0xdda21b981aacf6b8, false, (void*)&JV_f9b00e1a90cb16e0, &__wrap_JV_f9b00e1a90cb16e0 },
    { 0xfddfb0f49d0a491b, false, (void*)&JV_22b8a5c944277732, &__wrap_JV_22b8a5c944277732 },
    { 0x689a7d3c97766ab0, false, (void*)&JV_8a82057e10483a29, &__wrap_JV_8a82057e10483a29 },
    { 0xeec406a9645ff0e1, false, (void*)&JV_4bb097533696a50b, &__wrap_JV_4bb097533696a50b },
    { 0x1ab2859285510619, false, (void*)&JV_623fdb61479cb328, &__wrap_JV_623fdb61479cb328 },
    { 0xb472d6c37a51b6c7, false, (void*)&JV_e94e08dbede97c15, &__wrap_JV_e94e08dbede97c15 },
    { 0xcc2956899f6e3a28, false, (void*)&JV_5d29b737db96a9a5, &__wrap_JV_5d29b737db96a9a5 },
    { 0xebc54b3c01b55d0f, false, (void*)&JV_89570eb3601bc343, &__wrap_JV_89570eb3601bc343 },
    { 0x7270a19d02c563db, false, (void*)&JV_f13c60740e17c9cb, &__wrap_JV_f13c60740e17c9cb },
    { 0x1cbf2c81e84ed20c, true, (void*)&lexer_b6e69875fdd15de2, &__wrap_lexer_b6e69875fdd15de2 },
    { 0xb97c217fcffeae6c, true, (void*)&lexer_7b01766a6e1e52b1, &__wrap_lexer_7b01766a6e1e52b1 },
    { 0x3ab0399a3176467, false, (void*)&next_f03600350289290, &__wrap_next_f03600350289290 },
    { 0x24e41dcbcbb71907, true, (void*)&_lexer_d7025220389b96e7, &__wrap__lexer_d7025220389b96e7 },
    { 0x6403388df8b12fb5, false, (void*)&expect_token_92ddcaa262ab835a, &__wrap_expect_token_92ddcaa262ab835a },
    { 0x967c653d8a6c3666, false, (void*)&expect_symbol_25ecda935f91d894, &__wrap_expect_symbol_25ecda935f91d894 },
    { 0xba21b31978646981, false, (void*)&parse_value_ee908ba679ac2739, &__wrap_parse_value_ee908ba679ac2739 },
    { 0x2560e22a0e05e773, false, (void*)&read_json_1cf2494477255e77, &__wrap_read_json_1cf2494477255e77 },
    { 0x148c219be570a84e, false, (void*)&read_json_f5237b438b70759b, &__wrap_read_json_f5237b438b70759b },
    { 0xbedae22c23a5e9df, false, (void*)&set_no_trailing_zeros_f1b71a41030849b4, &__wrap_set_no_trailing_zeros_f1b71a41030849b4 },
    { 0x5a66bc39dfca9478, false, (void*)&set_no_empty_arrays_cb9f007816624484, &__wrap_set_no_empty_arrays_cb9f007816624484 },
    { 0x7f23b5cd01bd670c, false, (void*)&set_allow_duplicate_keys_b6a2085fcfe8cb80, &__wrap_set_allow_duplicate_keys_b6a2085fcfe8cb80 },
    { 0xecc1bd7bff388b78, false, (void*)&write_value_171bdd0fdeefc56b, &__wrap_write_value_171bdd0fdeefc56b },
    { 0xc59d211ccad269fb, false, (void*)&write_json_571c90b858deacc, &__wrap_write_json_571c90b858deacc },
    { 0x4591766036dbe1b6, false, (void*)&write_json_e91db05e9e89ab6f, &__wrap_write_json_e91db05e9e89ab6f },
    { 0xd4ef299eadab4c81, false, (void*)&try_fixing_broken_json_ef0b3a4bda8a0603, &__wrap_try_fixing_broken_json_ef0b3a4bda8a0603 },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
    resolveTypeInfoAnnotations();
}

} // namespace _anon_8411258461380451932
AotListBase impl_aot_json(_anon_8411258461380451932::registerAotFunctions);
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
