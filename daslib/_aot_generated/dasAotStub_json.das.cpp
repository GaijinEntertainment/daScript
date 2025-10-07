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
namespace json { struct _lambda_json_141_1; };
// unused enumeration ConversionResult
namespace json {

struct JsonValue {
    AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> value;
};
}
namespace json {

struct TokenAt {
    AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> value;
    int32_t line;
    int32_t row;
};
}
namespace json {

struct _lambda_json_141_1 {
    Func DAS_COMMENT((bool,json::_lambda_json_141_1,json::TokenAt)) __lambda;
    Func DAS_COMMENT((void,json::_lambda_json_141_1 *)) __finalize;
    int32_t __yield;
    char * __name_rename_at_243_20;
    int64_t __ival_rename_at_229_24;
    char * __num_rename_at_222_20;
    bool __is_integer_rename_at_201_20;
    bool __anyNumbers_rename_at_196_20;
    bool __is_negative_rename_at_194_20;
    int32_t row;
    int32_t line;
    TArray<uint8_t> str;
    int32_t ahead;
    Sequence DAS_COMMENT((int32_t)) tin;
    char * stext;
};
}
extern StructInfo __struct_info__69e2ef6fbeda51a0;
extern StructInfo __struct_info__6883e30bf583dbcb;
extern StructInfo __struct_info__6957e25f9f0258b6;
extern TypeInfo __type_info__d96037642c78d59b;
extern TypeInfo __type_info__994066f681df7bcd;
extern TypeInfo __type_info__3fb2f51b6ebec3f2;
extern TypeInfo __type_info__24229dbaa5060a1d;
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
extern TypeInfo __type_info__9e24638a2454b67d;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63d94c8601e773;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__d922fe078cefab30;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;

TypeInfo * __type_info__a798e985ff4af814_arg_types_var_7629923981941428640[7] = { &__type_info__e796e1d4af74511c, &__type_info__994066f681df7bcd, &__type_info__af63ee4c86020b22, &__type_info__af63d94c8601e773, &__type_info__d922fe078cefab30, &__type_info__af63df4c8601f1a5, &__type_info__12283e04d98e7c73 };
const char * __type_info__a798e985ff4af814_arg_names_var_7629923981941428640[7] = { "_object", "_array", "_string", "_number", "_longint", "_bool", "_null" };
VarInfo __struct_info__69e2ef6fbeda51a0_field_0 =  { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__a798e985ff4af814_arg_types_var_7629923981941428640, __type_info__a798e985ff4af814_arg_names_var_7629923981941428640, 7, 0, nullptr, 57346, TypeSize<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>>::size, UINT64_C(0xa798e985ff4af814), "value", offsetof(json::JsonValue,value), 1 };
VarInfo * __struct_info__69e2ef6fbeda51a0_fields[1] =  { &__struct_info__69e2ef6fbeda51a0_field_0 };
StructInfo __struct_info__69e2ef6fbeda51a0 = {"JsonValue", "json", 28, __struct_info__69e2ef6fbeda51a0_fields, 1, TypeSize<json::JsonValue>::size, UINT64_C(0x0), nullptr, UINT64_C(0x69e2ef6fbeda51a0), 0 };
TypeInfo * __type_info__fac8b32671fee393_arg_types_var_7531112642396806091[7] = { &__type_info__af63ee4c86020b22, &__type_info__af63d94c8601e773, &__type_info__d922fe078cefab30, &__type_info__af63df4c8601f1a5, &__type_info__12283e04d98e7c73, &__type_info__af63e44c8601fa24, &__type_info__af63ee4c86020b22 };
const char * __type_info__fac8b32671fee393_arg_names_var_7531112642396806091[7] = { "_string", "_number", "_longint", "_bool", "_null", "_symbol", "_error" };
VarInfo __struct_info__6883e30bf583dbcb_field_0 =  { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__fac8b32671fee393_arg_types_var_7531112642396806091, __type_info__fac8b32671fee393_arg_names_var_7531112642396806091, 7, 0, nullptr, 24582, TypeSize<AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *>>::size, UINT64_C(0xfac8b32671fee393), "value", offsetof(json::TokenAt,value), 3 };
VarInfo __struct_info__6883e30bf583dbcb_field_1 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xa981854735365000), "line", offsetof(json::TokenAt,line), 0 };
VarInfo __struct_info__6883e30bf583dbcb_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xbd409e4745b09701), "row", offsetof(json::TokenAt,row), 0 };
VarInfo * __struct_info__6883e30bf583dbcb_fields[3] =  { &__struct_info__6883e30bf583dbcb_field_0, &__struct_info__6883e30bf583dbcb_field_1, &__struct_info__6883e30bf583dbcb_field_2 };
StructInfo __struct_info__6883e30bf583dbcb = {"TokenAt", "json", 12, __struct_info__6883e30bf583dbcb_fields, 3, TypeSize<json::TokenAt>::size, UINT64_C(0x0), nullptr, UINT64_C(0x6883e30bf583dbcb), 0 };
TypeInfo * __type_info__cf689b8c38420e64_arg_types_var_7590784597273761974[2] = { &__type_info__9e24638a2454b67d, &__type_info__fa1aceb92701bc9a };
const char * __type_info__cf689b8c38420e64_arg_names_var_7590784597273761974[2] = { "__this", "_yield_141" };
VarInfo __struct_info__6957e25f9f0258b6_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__cf689b8c38420e64_arg_types_var_7590784597273761974, __type_info__cf689b8c38420e64_arg_names_var_7590784597273761974, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,json::_lambda_json_141_1,json::TokenAt))>::size, UINT64_C(0xcf689b8c38420e64), "__lambda", offsetof(json::_lambda_json_141_1,__lambda), 0 };
TypeInfo * __type_info__8ef576abaa82dace_arg_types_var_7590784597273761974[1] = { &__type_info__24229dbaa5060a1d };
const char * __type_info__8ef576abaa82dace_arg_names_var_7590784597273761974[1] = { "__this" };
VarInfo __struct_info__6957e25f9f0258b6_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8ef576abaa82dace_arg_types_var_7590784597273761974, __type_info__8ef576abaa82dace_arg_names_var_7590784597273761974, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,json::_lambda_json_141_1 *))>::size, UINT64_C(0x8ef576abaa82dace), "__finalize", offsetof(json::_lambda_json_141_1,__finalize), 0 };
VarInfo __struct_info__6957e25f9f0258b6_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xee086d2ee7fad8b), "__yield", offsetof(json::_lambda_json_141_1,__yield), 0 };
VarInfo __struct_info__6957e25f9f0258b6_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xb5a0cab6536b6983), "__name_rename_at_243_20", offsetof(json::_lambda_json_141_1,__name_rename_at_243_20), 5 };
VarInfo __struct_info__6957e25f9f0258b6_field_4 =  { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int64_t>::size, UINT64_C(0x330ef52ca12bafd), "__ival_rename_at_229_24", offsetof(json::_lambda_json_141_1,__ival_rename_at_229_24), 0 };
VarInfo __struct_info__6957e25f9f0258b6_field_5 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xe354d8dac0c958d4), "__num_rename_at_222_20", offsetof(json::_lambda_json_141_1,__num_rename_at_222_20), 11 };
VarInfo __struct_info__6957e25f9f0258b6_field_6 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0x1cf4b984676da036), "__is_integer_rename_at_201_20", offsetof(json::_lambda_json_141_1,__is_integer_rename_at_201_20), 0 };
VarInfo __struct_info__6957e25f9f0258b6_field_7 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xc9db6e0e073b29ae), "__anyNumbers_rename_at_196_20", offsetof(json::_lambda_json_141_1,__anyNumbers_rename_at_196_20), 0 };
VarInfo __struct_info__6957e25f9f0258b6_field_8 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xa8471bd7e486d378), "__is_negative_rename_at_194_20", offsetof(json::_lambda_json_141_1,__is_negative_rename_at_194_20), 0 };
VarInfo __struct_info__6957e25f9f0258b6_field_9 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x8bd506abe9ee0d5e), "row", offsetof(json::_lambda_json_141_1,row), 0 };
VarInfo __struct_info__6957e25f9f0258b6_field_10 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x76765db1bdce1893), "line", offsetof(json::_lambda_json_141_1,line), 0 };
VarInfo __struct_info__6957e25f9f0258b6_field_11 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x88d00dabe7afc343), "str", offsetof(json::_lambda_json_141_1,str), 13 };
VarInfo __struct_info__6957e25f9f0258b6_field_12 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xf5308572e5589a62), "ahead", offsetof(json::_lambda_json_141_1,ahead), 0 };
VarInfo __struct_info__6957e25f9f0258b6_field_13 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, TypeSize<Sequence DAS_COMMENT((int32_t))>::size, UINT64_C(0xa0ce08abfc3e35c4), "tin", offsetof(json::_lambda_json_141_1,tin), 14 };
VarInfo __struct_info__6957e25f9f0258b6_field_14 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16516, TypeSize<char *>::size, UINT64_C(0xa48761a94f4c841), "stext", offsetof(json::_lambda_json_141_1,stext), 15 };
VarInfo * __struct_info__6957e25f9f0258b6_fields[15] =  { &__struct_info__6957e25f9f0258b6_field_0, &__struct_info__6957e25f9f0258b6_field_1, &__struct_info__6957e25f9f0258b6_field_2, &__struct_info__6957e25f9f0258b6_field_3, &__struct_info__6957e25f9f0258b6_field_4, &__struct_info__6957e25f9f0258b6_field_5, &__struct_info__6957e25f9f0258b6_field_6, &__struct_info__6957e25f9f0258b6_field_7, &__struct_info__6957e25f9f0258b6_field_8, &__struct_info__6957e25f9f0258b6_field_9, &__struct_info__6957e25f9f0258b6_field_10, &__struct_info__6957e25f9f0258b6_field_11, &__struct_info__6957e25f9f0258b6_field_12, &__struct_info__6957e25f9f0258b6_field_13, &__struct_info__6957e25f9f0258b6_field_14 };
StructInfo __struct_info__6957e25f9f0258b6 = {"_lambda_json_141_1", "json", 30, __struct_info__6957e25f9f0258b6_fields, 15, TypeSize<json::_lambda_json_141_1>::size, UINT64_C(0x0), nullptr, UINT64_C(0x6957e25f9f0258b6), 3 };
VarInfo __func_info__5a16898e20f97124_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, TypeSize<TArray<uint8_t>>::size, UINT64_C(0xeeae79bd86a73b7b), "str", 0, 0 };
VarInfo * __func_info__5a16898e20f97124_fields[1] =  { &__func_info__5a16898e20f97124_field_0 };
FuncInfo __func_info__5a16898e20f97124 = {"invoke block<(str:array<uint8> const#):void> const", "", __func_info__5a16898e20f97124_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x5a16898e20f97124), 0x0 };
TypeInfo __type_info__d96037642c78d59b = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__fac8b32671fee393_arg_types_var_7531112642396806091, __type_info__fac8b32671fee393_arg_names_var_7531112642396806091, 7, 0, nullptr, 24838, TypeSize<AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *>>::size, UINT64_C(0xd96037642c78d59b) };
TypeInfo __type_info__994066f681df7bcd = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<json::JsonValue *>>::size, UINT64_C(0x994066f681df7bcd) };
TypeInfo __type_info__3fb2f51b6ebec3f2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__790033b1854b444b, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<json::JsonValue *>::size, UINT64_C(0x3fb2f51b6ebec3f2) };
TypeInfo __type_info__24229dbaa5060a1d = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__9e24638a2454b67d, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<json::_lambda_json_141_1 *>::size, UINT64_C(0x24229dbaa5060a1d) };
TypeInfo __type_info__e796e1d4af74511c = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TTable<char *,json::JsonValue *>>::size, UINT64_C(0xe796e1d4af74511c) };
TypeInfo __type_info__12283e04d98e7c73 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0x12283e04d98e7c73) };
TypeInfo __type_info__8dbbd9827382f56f = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__790033b1854b444b, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<json::JsonValue *>::size, UINT64_C(0x8dbbd9827382f56f) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<bool>::size, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af87fe4c863f5252 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<double>::size, UINT64_C(0xaf87fe4c863f5252) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int32_t>::size, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int64_t>::size, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<StringBuilderWriter>::size, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__790033b1854b444b = { Type::tStructure, &__struct_info__69e2ef6fbeda51a0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<json::JsonValue>::size, UINT64_C(0x790033b1854b444b) };
TypeInfo __type_info__fa1aceb92701bc9a = { Type::tStructure, &__struct_info__6883e30bf583dbcb, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24582, TypeSize<json::TokenAt>::size, UINT64_C(0xfa1aceb92701bc9a) };
TypeInfo __type_info__9e24638a2454b67d = { Type::tStructure, &__struct_info__6957e25f9f0258b6, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<json::_lambda_json_141_1>::size, UINT64_C(0x9e24638a2454b67d) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63d94c8601e773 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<double>::size, UINT64_C(0xaf63d94c8601e773) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__d922fe078cefab30 = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int64_t>::size, UINT64_C(0xd922fe078cefab30) };
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

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> & __a_rename_at_1336_0 );
inline void finalize_68a98f179c1b405d ( Context * __context__, json::_lambda_json_141_1 & ____this_rename_at_141_1 );
inline Sequence DAS_COMMENT((json::TokenAt)) _FuncbuiltinTickeachTick9663565701927713696_98de07d056597680 ( Context * __context__, Lambda DAS_COMMENT((bool,json::TokenAt)) const  __lam_rename_at_1443_2 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __it_rename_at_1377_4, int32_t & __value_rename_at_1377_5 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5 ( Context * __context__, char * const  __str_rename_at_1410_6 );
inline void _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_177_8, uint8_t __value_rename_at_177_9 );
inline bool _Func_lambda_json_141_1Tickfunction_10e48de6353d0512 ( Context * __context__, json::_lambda_json_141_1 & ____this_rename_at_141_10, json::TokenAt & ___yield_141_rename_at_141_11 );
inline void _Func_lambda_json_141_1Tickfinalizer_f14d60feb2bb6262 ( Context * __context__, json::_lambda_json_141_1 * ____this_rename_at_141_12 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_697f47037c455603 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __it_rename_at_1377_13, json::TokenAt & __value_rename_at_1377_14 );
inline void _FuncbuiltinTickpushTick10769833213962245646_7e039a4214a3a85d ( Context * __context__, TArray<json::JsonValue *> & __Arr_rename_at_193_15, json::JsonValue * __value_rename_at_193_16 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_ff6c295cb81c75cc ( Context * __context__, TTable<char *,json::JsonValue *> const  & __Tab_rename_at_1040_17, char * const  __at_rename_at_1040_18 );
inline void _FuncbuiltinTickinsertTick4246857231018487965_212d736cb85f8ac ( Context * __context__, TTable<char *,json::JsonValue *> & __Tab_rename_at_860_19, char * const  __at_rename_at_860_20, json::JsonValue * __val_rename_at_860_21 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick3875137551537175793_a1dc436de7d28685 ( Context * __context__, TTable<char *,json::JsonValue *> const  & __a_rename_at_1275_22 );
inline Sequence DAS_COMMENT((json::JsonValue * &)) _FuncbuiltinTickvaluesTick1935193042646774172_bfa1df8f4b975136 ( Context * __context__, TTable<char *,json::JsonValue *> const  & __a_rename_at_1297_24 );
inline json::JsonValue * JVNull_9ff51e1ff95fd846 ( Context * __context__ );
inline json::JsonValue * JV_d50422bd0c926605 ( Context * __context__, char * const  __v_rename_at_55_26 );
inline json::JsonValue * JV_1b649e55c06f723a ( Context * __context__, double __v_rename_at_60_27 );
inline json::JsonValue * JV_ad0db3c995ad7a27 ( Context * __context__, bool __v_rename_at_64_28 );
inline json::JsonValue * JV_753993827c36bf02 ( Context * __context__, TTable<char *,json::JsonValue *> & __v_rename_at_68_29 );
inline json::JsonValue * JV_f6e803bd2d1c03da ( Context * __context__, TArray<json::JsonValue *> & __v_rename_at_72_30 );
inline json::JsonValue * JV_4433ad2ad9d2c346 ( Context * __context__, float __v_rename_at_76_31 );
inline json::JsonValue * JV_d40575edae42e614 ( Context * __context__, int32_t __v_rename_at_80_32 );
inline json::JsonValue * JV_35964a9d95ac5c92 ( Context * __context__, Bitfield __v_rename_at_84_33 );
inline json::JsonValue * JV_95f31162b5b6ffe9 ( Context * __context__, int8_t __val_rename_at_88_34 );
inline json::JsonValue * JV_51967b5b9d9b844b ( Context * __context__, uint8_t __val_rename_at_92_35 );
inline json::JsonValue * JV_bc8ea85e34fd0468 ( Context * __context__, int16_t __val_rename_at_96_36 );
inline json::JsonValue * JV_4d5b9fa150f4f6d5 ( Context * __context__, uint16_t __val_rename_at_100_37 );
inline json::JsonValue * JV_456a700091cf7ee5 ( Context * __context__, uint32_t __val_rename_at_104_38 );
inline json::JsonValue * JV_3cf79a25e4e3f103 ( Context * __context__, int64_t __val_rename_at_108_39 );
inline json::JsonValue * JV_e1a21daba6c9d98b ( Context * __context__, uint64_t __val_rename_at_112_40 );
inline Sequence DAS_COMMENT((json::TokenAt)) lexer_a2420638876b2c8c ( Context * __context__, char * const  __text_rename_at_116_41 );
inline Sequence DAS_COMMENT((json::TokenAt)) lexer_c1e9ca186bdbef79 ( Context * __context__, TArray<uint8_t> const  & __text_rename_at_121_43 );
inline bool next_f56994d6cccfe0db ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __text_rename_at_126_45, int32_t & __character_rename_at_126_46, int32_t & __line_rename_at_126_47, int32_t & __row_rename_at_126_48 );
inline Sequence DAS_COMMENT((json::TokenAt)) _lexer_6c213821d171f98b ( Context * __context__, char * __stext_rename_at_140_49 );
inline bool expect_token_8b3d496f0ac17fc0 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __itv_rename_at_271_50, json::TokenAt & __ahead_rename_at_271_51, int32_t __vindex_rename_at_271_52, char * & __error_rename_at_271_53 );
inline bool expect_symbol_b0ae2af34afb8c2a ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __itv_rename_at_283_54, json::TokenAt & __ahead_rename_at_283_55, int32_t __sym_rename_at_283_56, char * & __error_rename_at_283_57 );
inline json::JsonValue * parse_value_30d0fecba4bd48e6 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __itv_rename_at_298_58, char * & __error_rename_at_298_59 );
inline json::JsonValue * read_json_7766a20eeeca4837 ( Context * __context__, char * const  __text_rename_at_396_69, char * & __error_rename_at_396_70 );
inline json::JsonValue * read_json_c123f70fe0ba5e0d ( Context * __context__, TArray<uint8_t> const  & __text_rename_at_406_73, char * & __error_rename_at_406_74 );
inline bool set_no_trailing_zeros_f1b71a41030849b4 ( Context * __context__, bool __value_rename_at_416_77 );
inline bool set_no_empty_arrays_cb9f007816624484 ( Context * __context__, bool __value_rename_at_425_79 );
inline bool set_allow_duplicate_keys_b6a2085fcfe8cb80 ( Context * __context__, bool __value_rename_at_434_81 );
inline void write_value_6d63393209ed931d ( Context * __context__, StringBuilderWriter & __writer_rename_at_442_83, json::JsonValue * const  __jsv_rename_at_442_84, int32_t __depth_rename_at_442_85 );
inline char * write_json_fce410ed73c6bcb1 ( Context * __context__, json::JsonValue * const  __val_rename_at_518_93 );
inline char * write_json_5aa76509a2243f61 ( Context * __context__, json::JsonValue * const  __val_rename_at_526_96 );
inline char * try_fixing_broken_json_ef0b3a4bda8a0603 ( Context * __context__, char * __bad_rename_at_533_97 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0xc713ddb3e835448d>(__context__) = 0;/*Token_string*/
    das_global<int32_t,0x96ac244c9265d269>(__context__) = 5;/*Token_symbol*/
    das_global<bool,0xd3b8b85b9bf19183>(__context__) = false;/*no_trailing_zeros*/
    das_global<bool,0xab7f08dbdeea5c03>(__context__) = false;/*no_empty_arrays*/
    das_global<bool,0x30e401a8d3210d31>(__context__) = false;/*allow_duplicate_keys*/
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1336_0 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1336_0),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_68a98f179c1b405d ( Context * __context__, json::_lambda_json_141_1 &  ____this_rename_at_141_1 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_1.str));
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_1.tin),__context__);
    memset((void*)&(____this_rename_at_141_1), 0, TypeSize<json::_lambda_json_141_1>::size);
}

inline Sequence DAS_COMMENT((json::TokenAt)) _FuncbuiltinTickeachTick9663565701927713696_98de07d056597680 ( Context * __context__, Lambda DAS_COMMENT((bool,json::TokenAt)) const  __lam_rename_at_1443_2 )
{
    Sequence DAS_COMMENT((json::TokenAt)) __it_rename_at_1445_3;das_zero(__it_rename_at_1445_3);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__it_rename_at_1445_3),__lam_rename_at_1443_2,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(__it_rename_at_1445_3);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __it_rename_at_1377_4, int32_t & __value_rename_at_1377_5 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1377_4),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1377_5)),__context__));
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5 ( Context * __context__, char * const  __str_rename_at_1410_6 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1411_7;das_zero(__it_rename_at_1411_7);
    builtin_make_string_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1411_7),__str_rename_at_1410_6,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1411_7);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_177_8, uint8_t __value_rename_at_177_9 )
{
    das_copy(__Arr_rename_at_177_8(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_177_8),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_177_9);
}

inline bool _Func_lambda_json_141_1Tickfunction_10e48de6353d0512 ( Context * __context__, json::_lambda_json_141_1 &  ____this_rename_at_141_10, json::TokenAt &  ___yield_141_rename_at_141_11 )
{
    switch (____this_rename_at_141_10.__yield) {
    case 0: goto label_0;
    case 37: goto label_37;
    case 1: goto label_1;
    case 35: goto label_35;
    case 7: goto label_7;
    case 3: goto label_3;
    case 2: goto label_2;
    case 4: goto label_4;
    case 5: goto label_5;
    case 6: goto label_6;
    case 8: goto label_8;
    case 9: goto label_9;
    case 10: goto label_10;
    case 11: goto label_11;
    case 33: goto label_33;
    case 12: goto label_12;
    case 13: goto label_13;
    case 14: goto label_14;
    case 16: goto label_16;
    case 15: goto label_15;
    case 17: goto label_17;
    case 31: goto label_31;
    case 18: goto label_18;
    case 26: goto label_26;
    case 19: goto label_19;
    case 24: goto label_24;
    case 20: goto label_20;
    case 22: goto label_22;
    case 21: goto label_21;
    case 23: goto label_23;
    case 25: goto label_25;
    case 27: goto label_27;
    case 29: goto label_29;
    case 28: goto label_28;
    case 30: goto label_30;
    case 32: goto label_32;
    case 34: goto label_34;
    case 36: goto label_36;
    case 38: goto label_38;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    memset((void*)&(____this_rename_at_141_10.tin), 0, TypeSize<Sequence DAS_COMMENT((int32_t))>::size);
    das_move(____this_rename_at_141_10.tin,_FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5(__context__,____this_rename_at_141_10.stext));
    das_copy(____this_rename_at_141_10.ahead,32);
    memset((void*)&(____this_rename_at_141_10.str), 0, TypeSize<TArray<uint8_t>>::size);
    das_copy(____this_rename_at_141_10.line,1);
    das_copy(____this_rename_at_141_10.row,0);
    label_37:;;
    if ( !!(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin))) )
    {
        goto label_38;
    };
    while ( is_white_space(____this_rename_at_141_10.ahead) && next_f56994d6cccfe0db(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead,____this_rename_at_141_10.line,____this_rename_at_141_10.row) )
    {
    };
    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin)) )
    {
        return das_auto_cast<bool>::cast(false);
    };
    if ( !((((((____this_rename_at_141_10.ahead == 91) || (____this_rename_at_141_10.ahead == 93)) || (____this_rename_at_141_10.ahead == 123)) || (____this_rename_at_141_10.ahead == 125)) || (____this_rename_at_141_10.ahead == 58)) || (____this_rename_at_141_10.ahead == 44)) )
    {
        goto label_35;
    };
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_158;
        das_zero(__mks_158);
        das_copy((__mks_158.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_158;
            das_get_auto_variant_field<int32_t,5,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_158) = ____this_rename_at_141_10.ahead;
            return __mkv_158;
        })())));
        das_copy((__mks_158.line),(____this_rename_at_141_10.line));
        das_copy((__mks_158.row),(____this_rename_at_141_10.row));
        return __mks_158;
    })()));
    das_copy(____this_rename_at_141_10.__yield,1);
    return das_auto_cast<bool>::cast(true);
    label_1:;;
    _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead);
    goto label_36;
    label_35:;;
    if ( !(____this_rename_at_141_10.ahead == 34) )
    {
        goto label_33;
    };
    label_7:;;
    if ( !(next_f56994d6cccfe0db(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead,____this_rename_at_141_10.line,____this_rename_at_141_10.row) && (____this_rename_at_141_10.ahead != 34)) )
    {
        goto label_8;
    };
    if ( !(____this_rename_at_141_10.ahead == 92) )
    {
        goto label_5;
    };
    if ( !next_f56994d6cccfe0db(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead,____this_rename_at_141_10.line,____this_rename_at_141_10.row) )
    {
        goto label_3;
    };
    if ( ____this_rename_at_141_10.ahead == 98 )
    {
        das_copy(____this_rename_at_141_10.ahead,8);
    } else if ( ____this_rename_at_141_10.ahead == 102 )
    {
        das_copy(____this_rename_at_141_10.ahead,12);
    } else if ( ____this_rename_at_141_10.ahead == 110 )
    {
        das_copy(____this_rename_at_141_10.ahead,10);
    } else if ( ____this_rename_at_141_10.ahead == 114 )
    {
        das_copy(____this_rename_at_141_10.ahead,13);
    } else if ( ____this_rename_at_141_10.ahead == 116 )
    {
        das_copy(____this_rename_at_141_10.ahead,9);
    } else if ( ____this_rename_at_141_10.ahead == 117 )
    {
        das_copy(____this_rename_at_141_10.ahead,92);
    };
    _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
    goto label_4;
    label_3:;;
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_179;
        das_zero(__mks_179);
        das_copy((__mks_179.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_179;
            das_get_auto_variant_field<char *,6,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_179) = ((char *) "string escape sequence exceeds text");
            return __mkv_179;
        })())));
        das_copy((__mks_179.line),(____this_rename_at_141_10.line));
        das_copy((__mks_179.row),(____this_rename_at_141_10.row));
        return __mks_179;
    })()));
    das_copy(____this_rename_at_141_10.__yield,2);
    return das_auto_cast<bool>::cast(true);
    label_2:;;
    return das_auto_cast<bool>::cast(false);
    label_4:;;
    goto label_6;
    label_5:;;
    _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
    label_6:;;
    goto label_7;
    label_8:;;
    if ( !builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin)) )
    {
        goto label_10;
    };
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_187;
        das_zero(__mks_187);
        das_copy((__mks_187.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_187;
            das_get_auto_variant_field<char *,6,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_187) = ((char *) "string exceeds text");
            return __mkv_187;
        })())));
        das_copy((__mks_187.line),(____this_rename_at_141_10.line));
        das_copy((__mks_187.row),(____this_rename_at_141_10.row));
        return __mks_187;
    })()));
    das_copy(____this_rename_at_141_10.__yield,9);
    return das_auto_cast<bool>::cast(true);
    label_9:;;
    return das_auto_cast<bool>::cast(false);
    label_10:;;
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_190;
        das_zero(__mks_190);
        das_copy((__mks_190.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_190;
            das_get_auto_variant_field<char *,0,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_190) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
            return __mkv_190;
        })())));
        das_copy((__mks_190.line),(____this_rename_at_141_10.line));
        das_copy((__mks_190.row),(____this_rename_at_141_10.row));
        return __mks_190;
    })()));
    das_copy(____this_rename_at_141_10.__yield,11);
    return das_auto_cast<bool>::cast(true);
    label_11:;;
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead);
    goto label_34;
    label_33:;;
    if ( !(((____this_rename_at_141_10.ahead == 43) || (____this_rename_at_141_10.ahead == 45)) || is_number(____this_rename_at_141_10.ahead)) )
    {
        goto label_31;
    };
    das_copy(____this_rename_at_141_10.__is_negative_rename_at_194_20,____this_rename_at_141_10.ahead == 45);
    _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
    das_copy(____this_rename_at_141_10.__anyNumbers_rename_at_196_20,is_number(____this_rename_at_141_10.ahead));
    while ( next_f56994d6cccfe0db(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead,____this_rename_at_141_10.line,____this_rename_at_141_10.row) && is_number(____this_rename_at_141_10.ahead) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
        das_copy(____this_rename_at_141_10.__anyNumbers_rename_at_196_20,true);
    };
    das_copy(____this_rename_at_141_10.__is_integer_rename_at_201_20,true);
    if ( !(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin))) && (____this_rename_at_141_10.ahead == 46) )
    {
        das_copy(____this_rename_at_141_10.__is_integer_rename_at_201_20,false);
        _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
        while ( next_f56994d6cccfe0db(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead,____this_rename_at_141_10.line,____this_rename_at_141_10.row) && is_number(____this_rename_at_141_10.ahead) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
            das_copy(____this_rename_at_141_10.__anyNumbers_rename_at_196_20,true);
        };
    };
    if ( !(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin))) && ((____this_rename_at_141_10.ahead == 101) || (____this_rename_at_141_10.ahead == 69)) )
    {
        das_copy(____this_rename_at_141_10.__is_integer_rename_at_201_20,false);
        _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
        next_f56994d6cccfe0db(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead,____this_rename_at_141_10.line,____this_rename_at_141_10.row);
        if ( ((____this_rename_at_141_10.ahead == 43) || (____this_rename_at_141_10.ahead == 45)) || is_number(____this_rename_at_141_10.ahead) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
        };
        while ( next_f56994d6cccfe0db(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead,____this_rename_at_141_10.line,____this_rename_at_141_10.row) && is_number(____this_rename_at_141_10.ahead) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
            das_copy(____this_rename_at_141_10.__anyNumbers_rename_at_196_20,true);
        };
    };
    das_copy(____this_rename_at_141_10.__num_rename_at_222_20,((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( !!(____this_rename_at_141_10.__anyNumbers_rename_at_196_20) )
    {
        goto label_13;
    };
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_224;
        das_zero(__mks_224);
        das_copy((__mks_224.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_224;
            das_get_auto_variant_field<char *,6,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_224) = das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_0, cast<char * const >::from(((char *) "invalid number ")), cast<char *>::from(____this_rename_at_141_10.__num_rename_at_222_20)));
            return __mkv_224;
        })())));
        das_copy((__mks_224.line),(____this_rename_at_141_10.line));
        das_copy((__mks_224.row),(____this_rename_at_141_10.row));
        return __mks_224;
    })()));
    das_copy(____this_rename_at_141_10.__yield,12);
    return das_auto_cast<bool>::cast(true);
    label_12:;;
    return das_auto_cast<bool>::cast(false);
    label_13:;;
    if ( !____this_rename_at_141_10.__is_integer_rename_at_201_20 )
    {
        goto label_16;
    };
    das_copy(____this_rename_at_141_10.__ival_rename_at_229_24,____this_rename_at_141_10.__is_negative_rename_at_194_20 ? das_auto_cast<int64_t>::cast(string_to_int64(____this_rename_at_141_10.__num_rename_at_222_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))) : das_auto_cast<int64_t>::cast(int64_t(string_to_uint64(____this_rename_at_141_10.__num_rename_at_222_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_230;
        das_zero(__mks_230);
        das_copy((__mks_230.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_230;
            das_get_auto_variant_field<int64_t,2,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_230) = ____this_rename_at_141_10.__ival_rename_at_229_24;
            return __mkv_230;
        })())));
        das_copy((__mks_230.line),(____this_rename_at_141_10.line));
        das_copy((__mks_230.row),(____this_rename_at_141_10.row));
        return __mks_230;
    })()));
    das_copy(____this_rename_at_141_10.__yield,14);
    return das_auto_cast<bool>::cast(true);
    label_14:;;
    goto label_17;
    label_16:;;
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_232;
        das_zero(__mks_232);
        das_copy((__mks_232.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_232;
            das_get_auto_variant_field<double,1,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_232) = string_to_double(____this_rename_at_141_10.__num_rename_at_222_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            return __mkv_232;
        })())));
        das_copy((__mks_232.line),(____this_rename_at_141_10.line));
        das_copy((__mks_232.row),(____this_rename_at_141_10.row));
        return __mks_232;
    })()));
    das_copy(____this_rename_at_141_10.__yield,15);
    return das_auto_cast<bool>::cast(true);
    label_15:;;
    label_17:;;
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    delete_string(____this_rename_at_141_10.__num_rename_at_222_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    goto label_32;
    label_31:;;
    if ( !is_alpha(____this_rename_at_141_10.ahead) )
    {
        goto label_29;
    };
    _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
    while ( next_f56994d6cccfe0db(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_141_10.tin),____this_rename_at_141_10.ahead,____this_rename_at_141_10.line,____this_rename_at_141_10.row) && is_alpha(____this_rename_at_141_10.ahead) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),uint8_t(____this_rename_at_141_10.ahead));
    };
    das_copy(____this_rename_at_141_10.__name_rename_at_243_20,((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( !(SimPolicy<char *>::Equ(cast<char *>::from(____this_rename_at_141_10.__name_rename_at_243_20),cast<char *>::from(((char *) "true")),*__context__,nullptr)) )
    {
        goto label_26;
    };
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_245;
        das_zero(__mks_245);
        das_copy((__mks_245.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_245;
            das_get_auto_variant_field<bool,3,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_245) = true;
            return __mkv_245;
        })())));
        das_copy((__mks_245.line),(____this_rename_at_141_10.line));
        das_copy((__mks_245.row),(____this_rename_at_141_10.row));
        return __mks_245;
    })()));
    das_copy(____this_rename_at_141_10.__yield,18);
    return das_auto_cast<bool>::cast(true);
    label_18:;;
    goto label_27;
    label_26:;;
    if ( !(SimPolicy<char *>::Equ(cast<char *>::from(____this_rename_at_141_10.__name_rename_at_243_20),cast<char *>::from(((char *) "false")),*__context__,nullptr)) )
    {
        goto label_24;
    };
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_247;
        das_zero(__mks_247);
        das_copy((__mks_247.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_247;
            das_get_auto_variant_field<bool,3,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_247) = false;
            return __mkv_247;
        })())));
        das_copy((__mks_247.line),(____this_rename_at_141_10.line));
        das_copy((__mks_247.row),(____this_rename_at_141_10.row));
        return __mks_247;
    })()));
    das_copy(____this_rename_at_141_10.__yield,19);
    return das_auto_cast<bool>::cast(true);
    label_19:;;
    goto label_25;
    label_24:;;
    if ( !(SimPolicy<char *>::Equ(cast<char *>::from(____this_rename_at_141_10.__name_rename_at_243_20),cast<char *>::from(((char *) "null")),*__context__,nullptr)) )
    {
        goto label_22;
    };
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_249;
        das_zero(__mks_249);
        das_copy((__mks_249.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_249;
            das_get_auto_variant_field<void *,4,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_249) = nullptr;
            return __mkv_249;
        })())));
        das_copy((__mks_249.line),(____this_rename_at_141_10.line));
        das_copy((__mks_249.row),(____this_rename_at_141_10.row));
        return __mks_249;
    })()));
    das_copy(____this_rename_at_141_10.__yield,20);
    return das_auto_cast<bool>::cast(true);
    label_20:;;
    goto label_23;
    label_22:;;
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_251;
        das_zero(__mks_251);
        das_copy((__mks_251.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_251;
            das_get_auto_variant_field<char *,6,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_251) = das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_0, cast<char * const >::from(((char *) "invalid name ")), cast<char *>::from(____this_rename_at_141_10.__name_rename_at_243_20)));
            return __mkv_251;
        })())));
        das_copy((__mks_251.line),(____this_rename_at_141_10.line));
        das_copy((__mks_251.row),(____this_rename_at_141_10.row));
        return __mks_251;
    })()));
    das_copy(____this_rename_at_141_10.__yield,21);
    return das_auto_cast<bool>::cast(true);
    label_21:;;
    return das_auto_cast<bool>::cast(false);
    label_23:;;
    label_25:;;
    label_27:;;
    delete_string(____this_rename_at_141_10.__name_rename_at_243_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(____this_rename_at_141_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    goto label_30;
    label_29:;;
    das_copy(___yield_141_rename_at_141_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_259;
        das_zero(__mks_259);
        das_copy((__mks_259.value),((([&]() -> AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> __mkv_259;
            das_get_auto_variant_field<char *,6,char *,double,int64_t,bool,void *,int32_t,char *>::set(__mkv_259) = das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_2, cast<char * const >::from(((char *) "invalid character `")), cast<char * const >::from(((char * const )(to_string_char(____this_rename_at_141_10.ahead,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(____this_rename_at_141_10.ahead)));
            return __mkv_259;
        })())));
        das_copy((__mks_259.line),(____this_rename_at_141_10.line));
        das_copy((__mks_259.row),(____this_rename_at_141_10.row));
        return __mks_259;
    })()));
    das_copy(____this_rename_at_141_10.__yield,28);
    return das_auto_cast<bool>::cast(true);
    label_28:;;
    return das_auto_cast<bool>::cast(false);
    label_30:;;
    label_32:;;
    label_34:;;
    label_36:;;
    goto label_37;
    label_38:;;
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_json_141_1Tickfinalizer_f14d60feb2bb6262 ( Context * __context__, json::_lambda_json_141_1 * ____this_rename_at_141_12 )
{
    delete_string(das_deref(__context__,____this_rename_at_141_12,__FILE__,__LINE__).stext,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    finalize_68a98f179c1b405d(__context__,das_arg<json::_lambda_json_141_1>::pass(das_deref(__context__,____this_rename_at_141_12,__FILE__,__LINE__)));
    das_delete_lambda_struct<json::_lambda_json_141_1 *>::clear(__context__,____this_rename_at_141_12);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_697f47037c455603 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __it_rename_at_1377_13, json::TokenAt &  __value_rename_at_1377_14 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__it_rename_at_1377_13),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1377_14)),__context__));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_7e039a4214a3a85d ( Context * __context__, TArray<json::JsonValue *> &  __Arr_rename_at_193_15, json::JsonValue * __value_rename_at_193_16 )
{
    das_copy(__Arr_rename_at_193_15(builtin_array_push_back(das_arg<TArray<json::JsonValue *>>::pass(__Arr_rename_at_193_15),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_16);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_ff6c295cb81c75cc ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __Tab_rename_at_1040_17, char * const  __at_rename_at_1040_18 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1040_17,__at_rename_at_1040_18));
}

inline void _FuncbuiltinTickinsertTick4246857231018487965_212d736cb85f8ac ( Context * __context__, TTable<char *,json::JsonValue *> &  __Tab_rename_at_860_19, char * const  __at_rename_at_860_20, json::JsonValue * __val_rename_at_860_21 )
{
    das_copy(__Tab_rename_at_860_19(__at_rename_at_860_20,__context__),__val_rename_at_860_21);
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick3875137551537175793_a1dc436de7d28685 ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __a_rename_at_1275_22 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1276_23;das_zero(__it_rename_at_1276_23);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1276_23),__a_rename_at_1275_22,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1276_23);
}

inline Sequence DAS_COMMENT((json::JsonValue * &)) _FuncbuiltinTickvaluesTick1935193042646774172_bfa1df8f4b975136 ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __a_rename_at_1297_24 )
{
    Sequence DAS_COMMENT((json::JsonValue * *)) __it_rename_at_1298_25;das_zero(__it_rename_at_1298_25);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((json::JsonValue * const ))>::pass(__it_rename_at_1298_25),__a_rename_at_1297_24,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::JsonValue * &))>::cast(__it_rename_at_1298_25);
}

inline json::JsonValue * JVNull_9ff51e1ff95fd846 ( Context * __context__ )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_52;
        das_zero(__mks_52);
        das_move((__mks_52.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_52;
            das_get_auto_variant_field<void *,6,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_52) = nullptr;
            return __mkv_52;
        })())));
        return __mks_52;
    })())));
}

inline json::JsonValue * JV_d50422bd0c926605 ( Context * __context__, char * const  __v_rename_at_55_26 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_57;
        das_zero(__mks_57);
        das_move((__mks_57.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_57;
            das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_57) = __v_rename_at_55_26;
            return __mkv_57;
        })())));
        return __mks_57;
    })())));
}

inline json::JsonValue * JV_1b649e55c06f723a ( Context * __context__, double __v_rename_at_60_27 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_61;
        das_zero(__mks_61);
        das_move((__mks_61.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_61;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_61) = __v_rename_at_60_27;
            return __mkv_61;
        })())));
        return __mks_61;
    })())));
}

inline json::JsonValue * JV_ad0db3c995ad7a27 ( Context * __context__, bool __v_rename_at_64_28 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_65;
        das_zero(__mks_65);
        das_move((__mks_65.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_65;
            das_get_auto_variant_field<bool,5,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_65) = __v_rename_at_64_28;
            return __mkv_65;
        })())));
        return __mks_65;
    })())));
}

inline json::JsonValue * JV_753993827c36bf02 ( Context * __context__, TTable<char *,json::JsonValue *> &  __v_rename_at_68_29 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_69;
        das_zero(__mks_69);
        das_move((__mks_69.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_69;
            das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_69) = __v_rename_at_68_29;
            return __mkv_69;
        })())));
        return __mks_69;
    })())));
}

inline json::JsonValue * JV_f6e803bd2d1c03da ( Context * __context__, TArray<json::JsonValue *> &  __v_rename_at_72_30 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_73;
        das_zero(__mks_73);
        das_move((__mks_73.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_73;
            das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_73) = __v_rename_at_72_30;
            return __mkv_73;
        })())));
        return __mks_73;
    })())));
}

inline json::JsonValue * JV_4433ad2ad9d2c346 ( Context * __context__, float __v_rename_at_76_31 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_77;
        das_zero(__mks_77);
        das_move((__mks_77.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_77;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_77) = double(__v_rename_at_76_31);
            return __mkv_77;
        })())));
        return __mks_77;
    })())));
}

inline json::JsonValue * JV_d40575edae42e614 ( Context * __context__, int32_t __v_rename_at_80_32 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_81;
        das_zero(__mks_81);
        das_move((__mks_81.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_81;
            das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_81) = int64_t(__v_rename_at_80_32);
            return __mkv_81;
        })())));
        return __mks_81;
    })())));
}

inline json::JsonValue * JV_35964a9d95ac5c92 ( Context * __context__, Bitfield __v_rename_at_84_33 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_85;
        das_zero(__mks_85);
        das_move((__mks_85.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_85;
            das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_85) = int64_t(__v_rename_at_84_33);
            return __mkv_85;
        })())));
        return __mks_85;
    })())));
}

inline json::JsonValue * JV_95f31162b5b6ffe9 ( Context * __context__, int8_t __val_rename_at_88_34 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_89;
        das_zero(__mks_89);
        das_move((__mks_89.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_89;
            das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_89) = int64_t(__val_rename_at_88_34);
            return __mkv_89;
        })())));
        return __mks_89;
    })())));
}

inline json::JsonValue * JV_51967b5b9d9b844b ( Context * __context__, uint8_t __val_rename_at_92_35 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_93;
        das_zero(__mks_93);
        das_move((__mks_93.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_93;
            das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_93) = int64_t(__val_rename_at_92_35);
            return __mkv_93;
        })())));
        return __mks_93;
    })())));
}

inline json::JsonValue * JV_bc8ea85e34fd0468 ( Context * __context__, int16_t __val_rename_at_96_36 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_97;
        das_zero(__mks_97);
        das_move((__mks_97.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_97;
            das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_97) = int64_t(__val_rename_at_96_36);
            return __mkv_97;
        })())));
        return __mks_97;
    })())));
}

inline json::JsonValue * JV_4d5b9fa150f4f6d5 ( Context * __context__, uint16_t __val_rename_at_100_37 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_101;
        das_zero(__mks_101);
        das_move((__mks_101.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_101;
            das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_101) = int64_t(__val_rename_at_100_37);
            return __mkv_101;
        })())));
        return __mks_101;
    })())));
}

inline json::JsonValue * JV_456a700091cf7ee5 ( Context * __context__, uint32_t __val_rename_at_104_38 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_105;
        das_zero(__mks_105);
        das_move((__mks_105.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_105;
            das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_105) = int64_t(__val_rename_at_104_38);
            return __mkv_105;
        })())));
        return __mks_105;
    })())));
}

inline json::JsonValue * JV_3cf79a25e4e3f103 ( Context * __context__, int64_t __val_rename_at_108_39 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_109;
        das_zero(__mks_109);
        das_move((__mks_109.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_109;
            das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_109) = __val_rename_at_108_39;
            return __mkv_109;
        })())));
        return __mks_109;
    })())));
}

inline json::JsonValue * JV_e1a21daba6c9d98b ( Context * __context__, uint64_t __val_rename_at_112_40 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_113;
        das_zero(__mks_113);
        das_move((__mks_113.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> __mkv_113;
            das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::set(__mkv_113) = int64_t(__val_rename_at_112_40);
            return __mkv_113;
        })())));
        return __mks_113;
    })())));
}

inline Sequence DAS_COMMENT((json::TokenAt)) lexer_a2420638876b2c8c ( Context * __context__, char * const  __text_rename_at_116_41 )
{
    char * __stext_rename_at_117_42 = ((char *)(char *)(((char * const )(builtin_string_clone(__text_rename_at_116_41,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(_lexer_6c213821d171f98b(__context__,__stext_rename_at_117_42));
}

inline Sequence DAS_COMMENT((json::TokenAt)) lexer_c1e9ca186bdbef79 ( Context * __context__, TArray<uint8_t> const  &  __text_rename_at_121_43 )
{
    char * __stext_rename_at_122_44 = ((char *)(char *)(((char * const )(builtin_string_from_array(__text_rename_at_121_43,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(_lexer_6c213821d171f98b(__context__,__stext_rename_at_122_44));
}

inline bool next_f56994d6cccfe0db ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __text_rename_at_126_45, int32_t & __character_rename_at_126_46, int32_t & __line_rename_at_126_47, int32_t & __row_rename_at_126_48 )
{
    if ( _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__text_rename_at_126_45),__character_rename_at_126_46) )
    {
        if ( is_new_line(__character_rename_at_126_46) )
        {
            ++__line_rename_at_126_47;
            das_copy(__row_rename_at_126_48,0);
        } else {
            ++__row_rename_at_126_48;
        };
        return das_auto_cast<bool>::cast(true);
    } else {
        return das_auto_cast<bool>::cast(false);
    };
}

inline Sequence DAS_COMMENT((json::TokenAt)) _lexer_6c213821d171f98b ( Context * __context__, char * __stext_rename_at_140_49 )
{
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(_FuncbuiltinTickeachTick9663565701927713696_98de07d056597680(__context__,das_ascend<bool,json::_lambda_json_141_1>::make(__context__,&__type_info__9e24638a2454b67d,(([&]() -> json::_lambda_json_141_1 {
        json::_lambda_json_141_1 __mks_141;
        das_zero(__mks_141);
        das_copy((__mks_141.__lambda),(Func(__context__->fnByMangledName(/*@json::_lambda_json_141_1`function XS<json::_lambda_json_141_1> S<json::TokenAt>*/ 0x8793d6dd22736fa0))));
        das_copy((__mks_141.__finalize),(Func(__context__->fnByMangledName(/*@json::_lambda_json_141_1`finalizer X1<S<json::_lambda_json_141_1>>?*/ 0x5d499bb6ed3ad75f))));
        das_copy((__mks_141.stext),(__stext_rename_at_140_49));
        return __mks_141;
    })()))));
}

inline bool expect_token_8b3d496f0ac17fc0 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __itv_rename_at_271_50, json::TokenAt &  __ahead_rename_at_271_51, int32_t __vindex_rename_at_271_52, char * & __error_rename_at_271_53 )
{
    if ( !_FuncbuiltinTicknextTick17450348357676149856_697f47037c455603(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_271_50),das_arg<json::TokenAt>::pass(__ahead_rename_at_271_51)) )
    {
        das_copy(__error_rename_at_271_53,((char *) "unexected eos"));
        return das_auto_cast<bool>::cast(false);
    } else if ( variant_index(das_arg<AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *>>::pass(__ahead_rename_at_271_51.value)) != __vindex_rename_at_271_52 )
    {
        das_copy(__error_rename_at_271_53,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_3, cast<char * const >::from(((char *) "unexpected ")), cast<AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_271_51.value), cast<char * const >::from(((char *) ", expecting variant ")), cast<int32_t>::from(__vindex_rename_at_271_52), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_271_51.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_271_51.row))));
        return das_auto_cast<bool>::cast(false);
    } else {
        return das_auto_cast<bool>::cast(true);
    };
}

inline bool expect_symbol_b0ae2af34afb8c2a ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __itv_rename_at_283_54, json::TokenAt &  __ahead_rename_at_283_55, int32_t __sym_rename_at_283_56, char * & __error_rename_at_283_57 )
{
    if ( !_FuncbuiltinTicknextTick17450348357676149856_697f47037c455603(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_283_54),das_arg<json::TokenAt>::pass(__ahead_rename_at_283_55)) )
    {
        das_copy(__error_rename_at_283_57,((char *) "unexected eos"));
        return das_auto_cast<bool>::cast(false);
    } else if ( !das_get_auto_variant_field<int32_t,5,char *,double,int64_t,bool,void *,int32_t,char *>::is(__ahead_rename_at_283_55.value) )
    {
        das_copy(__error_rename_at_283_57,das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_4, cast<char * const >::from(((char *) "unexpected ")), cast<AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_283_55.value), cast<char * const >::from(((char *) ", expecting symbol at ")), cast<int32_t>::from(__ahead_rename_at_283_55.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_283_55.row))));
        return das_auto_cast<bool>::cast(false);
    } else if ( !(das_get_auto_variant_field<int32_t,5,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_283_55.value,__context__) == __sym_rename_at_283_56) )
    {
        das_copy(__error_rename_at_283_57,das_string_builder(__context__,SimNode_AotInterop<10>(__tinfo_5, cast<char * const >::from(((char *) "unexpected ")), cast<AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_283_55.value), cast<char * const >::from(((char *) ", expecting symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sym_rename_at_283_56,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sym_rename_at_283_56), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_283_55.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_283_55.row))));
        return das_auto_cast<bool>::cast(true);
    } else {
        return das_auto_cast<bool>::cast(true);
    };
}

inline json::JsonValue * parse_value_30d0fecba4bd48e6 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __itv_rename_at_298_58, char * & __error_rename_at_298_59 )
{
    json::TokenAt __ahead_rename_at_299_60;das_zero(__ahead_rename_at_299_60);
    if ( !_FuncbuiltinTicknextTick17450348357676149856_697f47037c455603(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58),das_arg<json::TokenAt>::pass(__ahead_rename_at_299_60)) )
    {
        return das_auto_cast<json::JsonValue *>::cast(nullptr);
    } else {
        if ( das_get_auto_variant_field<int32_t,5,char *,double,int64_t,bool,void *,int32_t,char *>::is(__ahead_rename_at_299_60.value) )
        {
            int32_t __sym_rename_at_304_61 = ((int32_t)das_get_auto_variant_field<int32_t,5,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_299_60.value,__context__));
            if ( __sym_rename_at_304_61 == 93 )
            {
                das_copy(__error_rename_at_298_59,das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_6, cast<char * const >::from(((char *) "unexpected ] at ")), cast<int32_t>::from(__ahead_rename_at_299_60.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_299_60.row))));
                return das_auto_cast<json::JsonValue *>::cast(nullptr);
            } else {
                if ( __sym_rename_at_304_61 == 91 )
                {
                    TArray<json::JsonValue *> __arr_rename_at_310_62;das_zero(__arr_rename_at_310_62);
                    while ( !builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58)) )
                    {
                        json::JsonValue * __value_rename_at_312_63 = parse_value_30d0fecba4bd48e6(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58),__error_rename_at_298_59);
                        if ( __value_rename_at_312_63 == nullptr )
                        {
                            if ( builtin_string_startswith(__error_rename_at_298_59,((char *) "unexpected ]"),__context__) && (builtin_array_size(das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_310_62)) == 0) )
                            {
                                das_copy(__error_rename_at_298_59,nullptr);
                                return das_auto_cast<json::JsonValue *>::cast(JV_f6e803bd2d1c03da(__context__,das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_310_62)));
                            } else {
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            };
                        };
                        _FuncbuiltinTickpushTick10769833213962245646_7e039a4214a3a85d(__context__,das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_310_62),__value_rename_at_312_63);
                        if ( !expect_token_8b3d496f0ac17fc0(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58),das_arg<json::TokenAt>::pass(__ahead_rename_at_299_60),5,__error_rename_at_298_59) )
                        {
                            return das_auto_cast<json::JsonValue *>::cast(nullptr);
                        } else {
                            int32_t __sepsym_rename_at_324_64 = ((int32_t)das_get_auto_variant_field<int32_t,5,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_299_60.value,__context__));
                            if ( __sepsym_rename_at_324_64 == 93 )
                            {
                                break;
                            } else if ( __sepsym_rename_at_324_64 != 44 )
                            {
                                das_copy(__error_rename_at_298_59,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_7, cast<char * const >::from(((char *) "unsepected array seaprator symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sepsym_rename_at_324_64,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sepsym_rename_at_324_64), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_299_60.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_299_60.row))));
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            };
                        };
                    };
                    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58)) )
                    {
                        das_copy(__error_rename_at_298_59,((char *) "unexpected eos"));
                        return das_auto_cast<json::JsonValue *>::cast(nullptr);
                    } else {
                        return das_auto_cast<json::JsonValue *>::cast(JV_f6e803bd2d1c03da(__context__,das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_310_62)));
                    };
                } else if ( __sym_rename_at_304_61 == 123 )
                {
                    TTable<char *,json::JsonValue *> __tab_rename_at_338_65;das_zero(__tab_rename_at_338_65);
                    while ( !builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58)) )
                    {
                        if ( !expect_token_8b3d496f0ac17fc0(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58),das_arg<json::TokenAt>::pass(__ahead_rename_at_299_60),0,__error_rename_at_298_59) )
                        {
                            if ( (das_get_auto_variant_field<int32_t,5,char *,double,int64_t,bool,void *,int32_t,char *>::is(__ahead_rename_at_299_60.value) && (das_get_auto_variant_field<int32_t,5,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_299_60.value,__context__) == 125)) && (builtin_table_size(das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_338_65)) == 0) )
                            {
                                das_copy(__error_rename_at_298_59,nullptr);
                                return das_auto_cast<json::JsonValue *>::cast(JV_753993827c36bf02(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_338_65)));
                            } else {
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            };
                        };
                        char * __key_rename_at_347_66 = ((char *)(char *)(das_get_auto_variant_field<char *,0,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_299_60.value,__context__)));
                        if ( !expect_symbol_b0ae2af34afb8c2a(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58),das_arg<json::TokenAt>::pass(__ahead_rename_at_299_60),58,__error_rename_at_298_59) )
                        {
                            return das_auto_cast<json::JsonValue *>::cast(nullptr);
                        } else {
                            json::JsonValue * __value_rename_at_351_67 = parse_value_30d0fecba4bd48e6(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58),__error_rename_at_298_59);
                            if ( __value_rename_at_351_67 == nullptr )
                            {
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            } else {
                                if ( !das_global<bool,0x30e401a8d3210d31>(__context__) /*allow_duplicate_keys*/ && _FuncbuiltinTickkey_existsTick16808803843923989214_ff6c295cb81c75cc(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_338_65),__key_rename_at_347_66) )
                                {
                                    das_copy(__error_rename_at_298_59,das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_8, cast<char * const >::from(((char *) "duplicate key ")), cast<char * const >::from(__key_rename_at_347_66), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_299_60.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_299_60.row))));
                                    return das_auto_cast<json::JsonValue *>::cast(nullptr);
                                } else {
                                    _FuncbuiltinTickinsertTick4246857231018487965_212d736cb85f8ac(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_338_65),__key_rename_at_347_66,__value_rename_at_351_67);
                                    if ( !expect_token_8b3d496f0ac17fc0(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58),das_arg<json::TokenAt>::pass(__ahead_rename_at_299_60),5,__error_rename_at_298_59) )
                                    {
                                        return das_auto_cast<json::JsonValue *>::cast(nullptr);
                                    } else {
                                        int32_t __sepsym_rename_at_363_68 = ((int32_t)das_get_auto_variant_field<int32_t,5,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_299_60.value,__context__));
                                        if ( __sepsym_rename_at_363_68 == 125 )
                                        {
                                            break;
                                        } else if ( __sepsym_rename_at_363_68 != 44 )
                                        {
                                            das_copy(__error_rename_at_298_59,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_7, cast<char * const >::from(((char *) "unsepected object seaprator symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sepsym_rename_at_363_68,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sepsym_rename_at_363_68), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_299_60.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_299_60.row))));
                                            return das_auto_cast<json::JsonValue *>::cast(nullptr);
                                        };
                                    };
                                };
                            };
                        };
                    };
                    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_298_58)) )
                    {
                        das_copy(__error_rename_at_298_59,((char *) "unexpected eos"));
                        return das_auto_cast<json::JsonValue *>::cast(nullptr);
                    } else {
                        return das_auto_cast<json::JsonValue *>::cast(JV_753993827c36bf02(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_338_65)));
                    };
                } else {
                    das_copy(__error_rename_at_298_59,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_7, cast<char * const >::from(((char *) "unexpected symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sym_rename_at_304_61,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sym_rename_at_304_61), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_299_60.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_299_60.row))));
                    return das_auto_cast<json::JsonValue *>::cast(nullptr);
                };
            };
        } else if ( das_get_auto_variant_field<char *,0,char *,double,int64_t,bool,void *,int32_t,char *>::is(__ahead_rename_at_299_60.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_d50422bd0c926605(__context__,das_get_auto_variant_field<char *,0,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_299_60.value,__context__)));
        } else if ( das_get_auto_variant_field<double,1,char *,double,int64_t,bool,void *,int32_t,char *>::is(__ahead_rename_at_299_60.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_1b649e55c06f723a(__context__,das_get_auto_variant_field<double,1,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_299_60.value,__context__)));
        } else if ( das_get_auto_variant_field<int64_t,2,char *,double,int64_t,bool,void *,int32_t,char *>::is(__ahead_rename_at_299_60.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_3cf79a25e4e3f103(__context__,das_get_auto_variant_field<int64_t,2,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_299_60.value,__context__)));
        } else if ( das_get_auto_variant_field<bool,3,char *,double,int64_t,bool,void *,int32_t,char *>::is(__ahead_rename_at_299_60.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_ad0db3c995ad7a27(__context__,das_get_auto_variant_field<bool,3,char *,double,int64_t,bool,void *,int32_t,char *>::as(__ahead_rename_at_299_60.value,__context__)));
        } else if ( das_get_auto_variant_field<void *,4,char *,double,int64_t,bool,void *,int32_t,char *>::is(__ahead_rename_at_299_60.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JVNull_9ff51e1ff95fd846(__context__));
        } else {
            das_copy(__error_rename_at_298_59,das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_11, cast<AutoVariant<char *,double,int64_t,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_299_60.value), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_299_60.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_299_60.row))));
            return das_auto_cast<json::JsonValue *>::cast(nullptr);
        };
    };
}

inline json::JsonValue * read_json_7766a20eeeca4837 ( Context * __context__, char * const  __text_rename_at_396_69, char * & __error_rename_at_396_70 )
{
    das_copy(__error_rename_at_396_70,nullptr);
    Sequence DAS_COMMENT((json::TokenAt)) __lex_rename_at_400_71; das_zero(__lex_rename_at_400_71); das_move(__lex_rename_at_400_71, lexer_a2420638876b2c8c(__context__,__text_rename_at_396_69));
    json::JsonValue * __res_rename_at_401_72 = parse_value_30d0fecba4bd48e6(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_400_71),__error_rename_at_396_70);
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_400_71),__context__);
    return das_auto_cast<json::JsonValue *>::cast(__res_rename_at_401_72);
}

inline json::JsonValue * read_json_c123f70fe0ba5e0d ( Context * __context__, TArray<uint8_t> const  &  __text_rename_at_406_73, char * & __error_rename_at_406_74 )
{
    das_copy(__error_rename_at_406_74,nullptr);
    Sequence DAS_COMMENT((json::TokenAt)) __lex_rename_at_408_75; das_zero(__lex_rename_at_408_75); das_move(__lex_rename_at_408_75, lexer_c1e9ca186bdbef79(__context__,__text_rename_at_406_73));
    json::JsonValue * __res_rename_at_409_76 = parse_value_30d0fecba4bd48e6(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_408_75),__error_rename_at_406_74);
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_408_75),__context__);
    return das_auto_cast<json::JsonValue *>::cast(__res_rename_at_409_76);
}

inline bool set_no_trailing_zeros_f1b71a41030849b4 ( Context * __context__, bool __value_rename_at_416_77 )
{
    bool __old_ntz_rename_at_418_78 = ((bool)das_global<bool,0xd3b8b85b9bf19183>(__context__) /*no_trailing_zeros*/);
    das_copy(das_global<bool,0xd3b8b85b9bf19183>(__context__) /*no_trailing_zeros*/,__value_rename_at_416_77);
    return das_auto_cast<bool>::cast(__old_ntz_rename_at_418_78);
}

inline bool set_no_empty_arrays_cb9f007816624484 ( Context * __context__, bool __value_rename_at_425_79 )
{
    bool __old_nea_rename_at_427_80 = ((bool)das_global<bool,0xab7f08dbdeea5c03>(__context__) /*no_empty_arrays*/);
    das_copy(das_global<bool,0xab7f08dbdeea5c03>(__context__) /*no_empty_arrays*/,__value_rename_at_425_79);
    return das_auto_cast<bool>::cast(__old_nea_rename_at_427_80);
}

inline bool set_allow_duplicate_keys_b6a2085fcfe8cb80 ( Context * __context__, bool __value_rename_at_434_81 )
{
    bool __old_adc_rename_at_436_82 = ((bool)das_global<bool,0x30e401a8d3210d31>(__context__) /*allow_duplicate_keys*/);
    das_copy(das_global<bool,0x30e401a8d3210d31>(__context__) /*allow_duplicate_keys*/,__value_rename_at_434_81);
    return das_auto_cast<bool>::cast(__old_adc_rename_at_436_82);
}

inline void write_value_6d63393209ed931d ( Context * __context__, StringBuilderWriter &  __writer_rename_at_442_83, json::JsonValue * const  __jsv_rename_at_442_84, int32_t __depth_rename_at_442_85 )
{
    if ( __jsv_rename_at_442_84 == nullptr )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "null"))));
    } else if ( das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::is(__jsv_rename_at_442_84->value) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "\""))));
        write_escape_string(das_arg<StringBuilderWriter>::pass(__writer_rename_at_442_83),das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__));
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "\""))));
    } else if ( das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::is(__jsv_rename_at_442_84->value) )
    {
        if ( das_global<bool,0xd3b8b85b9bf19183>(__context__) /*no_trailing_zeros*/ )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char * const )(builtin_string_rtrim_ts(((char * const )(builtin_string_rtrim_ts(((char * const )(fmt_d(((char *) ":.17f"),das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "0"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "."),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_16,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<double>::from(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__))));
        };
    } else if ( das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::is(__jsv_rename_at_442_84->value) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<int64_t>::from(das_get_auto_variant_field<int64_t,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__))));
    } else if ( das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::is(__jsv_rename_at_442_84->value) )
    {
        if ( builtin_array_size(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__)) == 0 )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "[]"))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "[\n"))));
            bool __first_rename_at_462_86 = true;
            {
                bool __need_loop_463 = true;
                // elem: json::JsonValue? const&
                das_iterator<TArray<json::JsonValue *> const > __elem_iterator(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__));
                json::JsonValue * const  * __elem_rename_at_463_87;
                __need_loop_463 = __elem_iterator.first(__context__,(__elem_rename_at_463_87)) && __need_loop_463;
                for ( ; __need_loop_463 ; __need_loop_463 = __elem_iterator.next(__context__,(__elem_rename_at_463_87)) )
                {
                    if ( __first_rename_at_462_86 )
                    {
                        das_copy(__first_rename_at_462_86,false);
                    } else {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) ",\n"))));
                    };
                    write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_442_83),9,__depth_rename_at_442_85 + 1);
                    write_value_6d63393209ed931d(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_442_83),(*__elem_rename_at_463_87),__depth_rename_at_442_85 + 1);
                }
                __elem_iterator.close(__context__,(__elem_rename_at_463_87));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "\n"))));
            write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_442_83),9,__depth_rename_at_442_85);
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "]"))));
        };
    } else if ( das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::is(__jsv_rename_at_442_84->value) )
    {
        if ( builtin_table_size(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__)) == 0 )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "{}"))));
        } else {
            Sequence DAS_COMMENT((char *)) _temp_make_local_482_33_0; _temp_make_local_482_33_0;
            Sequence DAS_COMMENT((json::JsonValue * *)) _temp_make_local_482_61_1; _temp_make_local_482_61_1;
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "{\n"))));
            bool __first_rename_at_481_88 = true;
            {
                bool __need_loop_482 = true;
                // elemK: string
                das_iterator<Sequence DAS_COMMENT((char *))> __elemK_iterator((_temp_make_local_482_33_0 = (_FuncbuiltinTickkeysTick3875137551537175793_a1dc436de7d28685(__context__,das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__)))));
                char * __elemK_rename_at_482_91;
                __need_loop_482 = __elemK_iterator.first(__context__,(__elemK_rename_at_482_91)) && __need_loop_482;
                // elemV: json::JsonValue? const&
                das_iterator<Sequence DAS_COMMENT((json::JsonValue * const ))> __elemV_iterator((_temp_make_local_482_61_1 = (_FuncbuiltinTickvaluesTick1935193042646774172_bfa1df8f4b975136(__context__,das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__)))));
                json::JsonValue * const  * __elemV_rename_at_482_92;
                __need_loop_482 = __elemV_iterator.first(__context__,(__elemV_rename_at_482_92)) && __need_loop_482;
                for ( ; __need_loop_482 ; __need_loop_482 = __elemK_iterator.next(__context__,(__elemK_rename_at_482_91)) && __elemV_iterator.next(__context__,(__elemV_rename_at_482_92)) )
                {
                    if ( das_global<bool,0xab7f08dbdeea5c03>(__context__) /*no_empty_arrays*/ )
                    {
                        if ( das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::is((*__elemV_rename_at_482_92)->value) )
                        {
                            if ( builtin_array_size(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as((*__elemV_rename_at_482_92)->value,__context__)) == 0 )
                            {
                                continue;
                            };
                        };
                    };
                    if ( __first_rename_at_481_88 )
                    {
                        das_copy(__first_rename_at_481_88,false);
                    } else {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) ",\n"))));
                    };
                    write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_442_83),9,__depth_rename_at_442_85 + 1);
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "\""))));
                    write_escape_string(das_arg<StringBuilderWriter>::pass(__writer_rename_at_442_83),__elemK_rename_at_482_91);
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "\" : "))));
                    write_value_6d63393209ed931d(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_442_83),(*__elemV_rename_at_482_92),__depth_rename_at_442_85 + 1);
                }
                __elemV_iterator.close(__context__,(__elemV_rename_at_482_92));
                __elemK_iterator.close(__context__,(__elemK_rename_at_482_91));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "\n"))));
            write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_442_83),9,__depth_rename_at_442_85);
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "}"))));
        };
    } else if ( das_get_auto_variant_field<bool,5,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::is(__jsv_rename_at_442_84->value) )
    {
        if ( das_get_auto_variant_field<bool,5,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::as(__jsv_rename_at_442_84->value,__context__) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "true"))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "false"))));
        };
    } else if ( das_get_auto_variant_field<void *,6,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>::is(__jsv_rename_at_442_84->value) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_442_83),cast<char * const >::from(((char *) "null"))));
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_33, cast<char * const >::from(((char *) "unexpected ")), cast<json::JsonValue * const >::from(__jsv_rename_at_442_84))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline char * write_json_fce410ed73c6bcb1 ( Context * __context__, json::JsonValue * const  __val_rename_at_518_93 )
{
    char * __st_rename_at_520_94 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_520_95) DAS_AOT_INLINE_LAMBDA -> void{
        write_value_6d63393209ed931d(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_520_95),__val_rename_at_518_93,0);
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_520_94);
}

inline char * write_json_5aa76509a2243f61 ( Context * __context__, json::JsonValue * const  __val_rename_at_526_96 )
{
    return das_auto_cast<char *>::cast(write_json_fce410ed73c6bcb1(__context__,das_cast<json::JsonValue *>::cast(__val_rename_at_526_96)));
}

inline char * try_fixing_broken_json_ef0b3a4bda8a0603 ( Context * __context__, char * __bad_rename_at_533_97 ) { das_stack_prologue __prologue(__context__,272,"try_fixing_broken_json " DAS_FILE_LINE);
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_539_98) DAS_AOT_INLINE_LAMBDA -> void{
        builtin_string_peek(__bad_rename_at_533_97,das_make_block<void,TArray<uint8_t> const  &>(__context__,144,0,&__func_info__5a16898e20f97124,[&](TArray<uint8_t> const  & __str_rename_at_540_99) -> void{
            int32_t __i_rename_at_541_100 = 0;
            int32_t __lstr_rename_at_542_101 = ((int32_t)builtin_array_size(__str_rename_at_540_99));
            while ( __i_rename_at_541_100 < __lstr_rename_at_542_101 )
            {
                while ( (__i_rename_at_541_100 < __lstr_rename_at_542_101) && (das_nequ_val(__str_rename_at_540_99(__i_rename_at_541_100,__context__),0x22)) )
                {
                    if ( das_equ_val(__str_rename_at_540_99(__i_rename_at_541_100,__context__),0x2c) )
                    {
                        int32_t __j_rename_at_547_102 = (__i_rename_at_541_100 + 1);
                        while ( (__j_rename_at_547_102 < __lstr_rename_at_542_101) && is_white_space(int32_t(__str_rename_at_540_99(__j_rename_at_547_102,__context__))) )
                        {
                            ++__j_rename_at_547_102;
                        };
                        if ( (__j_rename_at_547_102 < __lstr_rename_at_542_101) && ((das_equ_val(__str_rename_at_540_99(__j_rename_at_547_102,__context__),0x7d)) || (das_equ_val(__str_rename_at_540_99(__j_rename_at_547_102,__context__),0x5d))) )
                        {
                            das_copy(__i_rename_at_541_100,__j_rename_at_547_102);
                            continue;
                        };
                    };
                    if ( (das_equ_val(__str_rename_at_540_99(__i_rename_at_541_100,__context__),0x5c)) && (das_equ_val(__str_rename_at_540_99((__i_rename_at_541_100 + 1),__context__),0x75)) )
                    {
                        __i_rename_at_541_100 += 2;
                        while ( (__i_rename_at_541_100 < __lstr_rename_at_542_101) && is_hex(int32_t(__str_rename_at_540_99(__i_rename_at_541_100,__context__))) )
                        {
                            ++__i_rename_at_541_100;
                        };
                        continue;
                    } else {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),int32_t(__str_rename_at_540_99(__i_rename_at_541_100,__context__)));
                        ++__i_rename_at_541_100;
                    };
                };
                if ( __i_rename_at_541_100 >= __lstr_rename_at_542_101 )
                {
                    break;
                } else {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),34);
                    ++__i_rename_at_541_100;
                    label_0:;;
                    while ( (__i_rename_at_541_100 < __lstr_rename_at_542_101) && (das_nequ_val(__str_rename_at_540_99(__i_rename_at_541_100,__context__),0x22)) )
                    {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),int32_t(__str_rename_at_540_99(__i_rename_at_541_100,__context__)));
                        ++__i_rename_at_541_100;
                    };
                    if ( __i_rename_at_541_100 >= __lstr_rename_at_542_101 )
                    {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),34);
                        break;
                    } else {
                        int32_t __nested_rename_at_583_103 = ((int32_t)__i_rename_at_541_100);
                        int32_t __j_rename_at_584_104 = (__i_rename_at_541_100 + 1);
                        bool __any_white_space_rename_at_585_105 = ((bool)((__j_rename_at_584_104 < __lstr_rename_at_542_101) && is_white_space(int32_t(__str_rename_at_540_99(__j_rename_at_584_104,__context__)))));
                        while ( (__j_rename_at_584_104 < __lstr_rename_at_542_101) && is_white_space(int32_t(__str_rename_at_540_99(__j_rename_at_584_104,__context__))) )
                        {
                            ++__j_rename_at_584_104;
                        };
                        if ( __j_rename_at_584_104 >= __lstr_rename_at_542_101 )
                        {
                            write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),34);
                            break;
                        } else {
                            int32_t __strj_rename_at_594_106 = ((int32_t)int32_t(__str_rename_at_540_99(__j_rename_at_584_104,__context__)));
                            if ( (((__strj_rename_at_594_106 == 58) || (__strj_rename_at_594_106 == 125)) || (__strj_rename_at_594_106 == 93)) || (__strj_rename_at_594_106 == 44) )
                            {
                                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),34);
                                ++__i_rename_at_541_100;
                                continue;
                            } else {
                                if ( __strj_rename_at_594_106 == 43 )
                                {
                                    ++__j_rename_at_584_104;
                                    while ( (__j_rename_at_584_104 < __lstr_rename_at_542_101) && is_white_space(int32_t(__str_rename_at_540_99(__j_rename_at_584_104,__context__))) )
                                    {
                                        ++__j_rename_at_584_104;
                                    };
                                    if ( (__j_rename_at_584_104 < __lstr_rename_at_542_101) && (das_equ_val(__str_rename_at_540_99(__j_rename_at_584_104,__context__),0x22)) )
                                    {
                                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),32);
                                        das_copy(__i_rename_at_541_100,__j_rename_at_584_104 + 1);
                                        goto label_0;
                                    };
                                };
                                if ( __any_white_space_rename_at_585_105 )
                                {
                                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),34);
                                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),44);
                                    das_copy(__i_rename_at_541_100,__nested_rename_at_583_103 + 1);
                                    continue;
                                } else {
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_539_98),cast<char * const >::from(((char *) "`"))));
                                    das_copy(__i_rename_at_541_100,__nested_rename_at_583_103 + 1);
                                    while ( (__i_rename_at_541_100 < __lstr_rename_at_542_101) && (das_nequ_val(__str_rename_at_540_99(__i_rename_at_541_100,__context__),0x22)) )
                                    {
                                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),int32_t(__str_rename_at_540_99(__i_rename_at_541_100,__context__)));
                                        ++__i_rename_at_541_100;
                                    };
                                    if ( __i_rename_at_541_100 >= __lstr_rename_at_542_101 )
                                    {
                                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_539_98),cast<char * const >::from(((char *) "`"))));
                                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_539_98),34);
                                        break;
                                    } else {
                                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_539_98),cast<char * const >::from(((char *) "`"))));
                                        ++__i_rename_at_541_100;
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
static vec4f __wrap__FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__ ) {
    TArray<uint8_t> &  arg_a = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap_finalize_68a98f179c1b405d ( Context * __context__ ) {
    json::_lambda_json_141_1 &  arg___this = cast_aot_arg<json::_lambda_json_141_1 & >::to(*__context__,__context__->abiArguments()[0]);
    finalize_68a98f179c1b405d(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickeachTick9663565701927713696_98de07d056597680 ( Context * __context__ ) {
    Lambda DAS_COMMENT((bool,json::TokenAt)) const  arg_lam = cast_aot_arg<Lambda DAS_COMMENT((bool,json::TokenAt)) const >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::TokenAt)) *) __context__->abiCMRES) = _FuncbuiltinTickeachTick9663565701927713696_98de07d056597680(__context__, arg_lam);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57 ( Context * __context__ ) {
    Sequence DAS_COMMENT((int32_t)) &  arg_it = cast_aot_arg<Sequence DAS_COMMENT((int32_t)) & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t & arg_value = cast_aot_arg<int32_t &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57(__context__, arg_it, arg_value));
}
static vec4f __wrap__FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5 ( Context * __context__ ) {
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((int32_t)) *) __context__->abiCMRES) = _FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5(__context__, arg_str);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d ( Context * __context__ ) {
    TArray<uint8_t> &  arg_Arr = cast_aot_arg<TArray<uint8_t> & >::to(*__context__,__context__->abiArguments()[0]);
    uint8_t arg_value = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__Func_lambda_json_141_1Tickfunction_10e48de6353d0512 ( Context * __context__ ) {
    json::_lambda_json_141_1 &  arg___this = cast_aot_arg<json::_lambda_json_141_1 & >::to(*__context__,__context__->abiArguments()[0]);
    json::TokenAt &  arg__yield_141 = cast_aot_arg<json::TokenAt & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_Func_lambda_json_141_1Tickfunction_10e48de6353d0512(__context__, arg___this, arg__yield_141));
}
static vec4f __wrap__Func_lambda_json_141_1Tickfinalizer_f14d60feb2bb6262 ( Context * __context__ ) {
    json::_lambda_json_141_1 * arg___this = cast_aot_arg<json::_lambda_json_141_1 *>::to(*__context__,__context__->abiArguments()[0]);
    _Func_lambda_json_141_1Tickfinalizer_f14d60feb2bb6262(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTicknextTick17450348357676149856_697f47037c455603 ( Context * __context__ ) {
    Sequence DAS_COMMENT((json::TokenAt)) &  arg_it = cast_aot_arg<Sequence DAS_COMMENT((json::TokenAt)) & >::to(*__context__,__context__->abiArguments()[0]);
    json::TokenAt &  arg_value = cast_aot_arg<json::TokenAt & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncbuiltinTicknextTick17450348357676149856_697f47037c455603(__context__, arg_it, arg_value));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_7e039a4214a3a85d ( Context * __context__ ) {
    TArray<json::JsonValue *> &  arg_Arr = cast_aot_arg<TArray<json::JsonValue *> & >::to(*__context__,__context__->abiArguments()[0]);
    json::JsonValue * arg_value = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_7e039a4214a3a85d(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickkey_existsTick16808803843923989214_ff6c295cb81c75cc ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> const  &  arg_Tab = cast_aot_arg<TTable<char *,json::JsonValue *> const  & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_at = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncbuiltinTickkey_existsTick16808803843923989214_ff6c295cb81c75cc(__context__, arg_Tab, arg_at));
}
static vec4f __wrap__FuncbuiltinTickinsertTick4246857231018487965_212d736cb85f8ac ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> &  arg_Tab = cast_aot_arg<TTable<char *,json::JsonValue *> & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_at = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    json::JsonValue * arg_val = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[2]);
    _FuncbuiltinTickinsertTick4246857231018487965_212d736cb85f8ac(__context__, arg_Tab, arg_at, arg_val);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickkeysTick3875137551537175793_a1dc436de7d28685 ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> const  &  arg_a = cast_aot_arg<TTable<char *,json::JsonValue *> const  & >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((char *)) *) __context__->abiCMRES) = _FuncbuiltinTickkeysTick3875137551537175793_a1dc436de7d28685(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickvaluesTick1935193042646774172_bfa1df8f4b975136 ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> const  &  arg_a = cast_aot_arg<TTable<char *,json::JsonValue *> const  & >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::JsonValue * &)) *) __context__->abiCMRES) = _FuncbuiltinTickvaluesTick1935193042646774172_bfa1df8f4b975136(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap_JVNull_9ff51e1ff95fd846 ( Context * __context__ ) {
    return cast<json::JsonValue *>::from(JVNull_9ff51e1ff95fd846(__context__));
}
static vec4f __wrap_JV_d50422bd0c926605 ( Context * __context__ ) {
    char * const  arg_v = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_d50422bd0c926605(__context__, arg_v));
}
static vec4f __wrap_JV_1b649e55c06f723a ( Context * __context__ ) {
    double arg_v = cast_aot_arg<double>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_1b649e55c06f723a(__context__, arg_v));
}
static vec4f __wrap_JV_ad0db3c995ad7a27 ( Context * __context__ ) {
    bool arg_v = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_ad0db3c995ad7a27(__context__, arg_v));
}
static vec4f __wrap_JV_753993827c36bf02 ( Context * __context__ ) {
    TTable<char *,json::JsonValue *> &  arg_v = cast_aot_arg<TTable<char *,json::JsonValue *> & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_753993827c36bf02(__context__, arg_v));
}
static vec4f __wrap_JV_f6e803bd2d1c03da ( Context * __context__ ) {
    TArray<json::JsonValue *> &  arg_v = cast_aot_arg<TArray<json::JsonValue *> & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_f6e803bd2d1c03da(__context__, arg_v));
}
static vec4f __wrap_JV_4433ad2ad9d2c346 ( Context * __context__ ) {
    float arg_v = cast_aot_arg<float>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_4433ad2ad9d2c346(__context__, arg_v));
}
static vec4f __wrap_JV_d40575edae42e614 ( Context * __context__ ) {
    int32_t arg_v = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_d40575edae42e614(__context__, arg_v));
}
static vec4f __wrap_JV_35964a9d95ac5c92 ( Context * __context__ ) {
    Bitfield arg_v = cast_aot_arg<Bitfield>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_35964a9d95ac5c92(__context__, arg_v));
}
static vec4f __wrap_JV_95f31162b5b6ffe9 ( Context * __context__ ) {
    int8_t arg_val = cast_aot_arg<int8_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_95f31162b5b6ffe9(__context__, arg_val));
}
static vec4f __wrap_JV_51967b5b9d9b844b ( Context * __context__ ) {
    uint8_t arg_val = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_51967b5b9d9b844b(__context__, arg_val));
}
static vec4f __wrap_JV_bc8ea85e34fd0468 ( Context * __context__ ) {
    int16_t arg_val = cast_aot_arg<int16_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_bc8ea85e34fd0468(__context__, arg_val));
}
static vec4f __wrap_JV_4d5b9fa150f4f6d5 ( Context * __context__ ) {
    uint16_t arg_val = cast_aot_arg<uint16_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_4d5b9fa150f4f6d5(__context__, arg_val));
}
static vec4f __wrap_JV_456a700091cf7ee5 ( Context * __context__ ) {
    uint32_t arg_val = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_456a700091cf7ee5(__context__, arg_val));
}
static vec4f __wrap_JV_3cf79a25e4e3f103 ( Context * __context__ ) {
    int64_t arg_val = cast_aot_arg<int64_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_3cf79a25e4e3f103(__context__, arg_val));
}
static vec4f __wrap_JV_e1a21daba6c9d98b ( Context * __context__ ) {
    uint64_t arg_val = cast_aot_arg<uint64_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<json::JsonValue *>::from(JV_e1a21daba6c9d98b(__context__, arg_val));
}
static vec4f __wrap_lexer_a2420638876b2c8c ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::TokenAt)) *) __context__->abiCMRES) = lexer_a2420638876b2c8c(__context__, arg_text);
    return v_zero();
}
static vec4f __wrap_lexer_c1e9ca186bdbef79 ( Context * __context__ ) {
    TArray<uint8_t> const  &  arg_text = cast_aot_arg<TArray<uint8_t> const  & >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::TokenAt)) *) __context__->abiCMRES) = lexer_c1e9ca186bdbef79(__context__, arg_text);
    return v_zero();
}
static vec4f __wrap_next_f56994d6cccfe0db ( Context * __context__ ) {
    Sequence DAS_COMMENT((int32_t)) &  arg_text = cast_aot_arg<Sequence DAS_COMMENT((int32_t)) & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t & arg_character = cast_aot_arg<int32_t &>::to(*__context__,__context__->abiArguments()[1]);
    int32_t & arg_line = cast_aot_arg<int32_t &>::to(*__context__,__context__->abiArguments()[2]);
    int32_t & arg_row = cast_aot_arg<int32_t &>::to(*__context__,__context__->abiArguments()[3]);
    return cast<bool>::from(next_f56994d6cccfe0db(__context__, arg_text, arg_character, arg_line, arg_row));
}
static vec4f __wrap__lexer_6c213821d171f98b ( Context * __context__ ) {
    char * arg_stext = cast_aot_arg<char *>::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((json::TokenAt)) *) __context__->abiCMRES) = _lexer_6c213821d171f98b(__context__, arg_stext);
    return v_zero();
}
static vec4f __wrap_expect_token_8b3d496f0ac17fc0 ( Context * __context__ ) {
    Sequence DAS_COMMENT((json::TokenAt)) &  arg_itv = cast_aot_arg<Sequence DAS_COMMENT((json::TokenAt)) & >::to(*__context__,__context__->abiArguments()[0]);
    json::TokenAt &  arg_ahead = cast_aot_arg<json::TokenAt & >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_vindex = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[3]);
    return cast<bool>::from(expect_token_8b3d496f0ac17fc0(__context__, arg_itv, arg_ahead, arg_vindex, arg_error));
}
static vec4f __wrap_expect_symbol_b0ae2af34afb8c2a ( Context * __context__ ) {
    Sequence DAS_COMMENT((json::TokenAt)) &  arg_itv = cast_aot_arg<Sequence DAS_COMMENT((json::TokenAt)) & >::to(*__context__,__context__->abiArguments()[0]);
    json::TokenAt &  arg_ahead = cast_aot_arg<json::TokenAt & >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_sym = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[3]);
    return cast<bool>::from(expect_symbol_b0ae2af34afb8c2a(__context__, arg_itv, arg_ahead, arg_sym, arg_error));
}
static vec4f __wrap_parse_value_30d0fecba4bd48e6 ( Context * __context__ ) {
    Sequence DAS_COMMENT((json::TokenAt)) &  arg_itv = cast_aot_arg<Sequence DAS_COMMENT((json::TokenAt)) & >::to(*__context__,__context__->abiArguments()[0]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(parse_value_30d0fecba4bd48e6(__context__, arg_itv, arg_error));
}
static vec4f __wrap_read_json_7766a20eeeca4837 ( Context * __context__ ) {
    char * const  arg_text = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(read_json_7766a20eeeca4837(__context__, arg_text, arg_error));
}
static vec4f __wrap_read_json_c123f70fe0ba5e0d ( Context * __context__ ) {
    TArray<uint8_t> const  &  arg_text = cast_aot_arg<TArray<uint8_t> const  & >::to(*__context__,__context__->abiArguments()[0]);
    char * & arg_error = cast_aot_arg<char * &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(read_json_c123f70fe0ba5e0d(__context__, arg_text, arg_error));
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
static vec4f __wrap_write_value_6d63393209ed931d ( Context * __context__ ) {
    StringBuilderWriter &  arg_writer = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[0]);
    json::JsonValue * const  arg_jsv = cast_aot_arg<json::JsonValue * const >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_depth = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    write_value_6d63393209ed931d(__context__, arg_writer, arg_jsv, arg_depth);
    return v_zero();
}
static vec4f __wrap_write_json_fce410ed73c6bcb1 ( Context * __context__ ) {
    json::JsonValue * const  arg_val = cast_aot_arg<json::JsonValue * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(write_json_fce410ed73c6bcb1(__context__, arg_val));
}
static vec4f __wrap_write_json_5aa76509a2243f61 ( Context * __context__ ) {
    json::JsonValue * const  arg_val = cast_aot_arg<json::JsonValue * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(write_json_5aa76509a2243f61(__context__, arg_val));
}
static vec4f __wrap_try_fixing_broken_json_ef0b3a4bda8a0603 ( Context * __context__ ) {
    char * arg_bad = cast_aot_arg<char *>::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(try_fixing_broken_json_ef0b3a4bda8a0603(__context__, arg_bad));
}
static vec4f __wrap___init_script ( Context * __context__ ) {
    __init_script(__context__, cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[0]));
    return v_zero();
};

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0x1f8eb5798352d074, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb },
    { 0xd8cf5466b2b5ac54, false, (void*)&finalize_68a98f179c1b405d, &__wrap_finalize_68a98f179c1b405d },
    { 0xb0f9c87c496f9282, true, (void*)&_FuncbuiltinTickeachTick9663565701927713696_98de07d056597680, &__wrap__FuncbuiltinTickeachTick9663565701927713696_98de07d056597680 },
    { 0x56aed9f0fde0dd26, false, (void*)&_FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57, &__wrap__FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57 },
    { 0x9e683f3840fb4e50, true, (void*)&_FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5, &__wrap__FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5 },
    { 0x58266f7d6c4bcaae, false, (void*)&_FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d, &__wrap__FuncbuiltinTickpushTick14133213201864676143_9dcaf07ee39dc13d },
    { 0xfeeb98479ce56e0f, false, (void*)&_Func_lambda_json_141_1Tickfunction_10e48de6353d0512, &__wrap__Func_lambda_json_141_1Tickfunction_10e48de6353d0512 },
    { 0xb3890ab45e716086, false, (void*)&_Func_lambda_json_141_1Tickfinalizer_f14d60feb2bb6262, &__wrap__Func_lambda_json_141_1Tickfinalizer_f14d60feb2bb6262 },
    { 0xe5bbc9d40955117b, false, (void*)&_FuncbuiltinTicknextTick17450348357676149856_697f47037c455603, &__wrap__FuncbuiltinTicknextTick17450348357676149856_697f47037c455603 },
    { 0xff836eaddb1c0715, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_7e039a4214a3a85d, &__wrap__FuncbuiltinTickpushTick10769833213962245646_7e039a4214a3a85d },
    { 0xf5465995318ff500, false, (void*)&_FuncbuiltinTickkey_existsTick16808803843923989214_ff6c295cb81c75cc, &__wrap__FuncbuiltinTickkey_existsTick16808803843923989214_ff6c295cb81c75cc },
    { 0x2681235935198424, false, (void*)&_FuncbuiltinTickinsertTick4246857231018487965_212d736cb85f8ac, &__wrap__FuncbuiltinTickinsertTick4246857231018487965_212d736cb85f8ac },
    { 0x40d5933441a059cd, true, (void*)&_FuncbuiltinTickkeysTick3875137551537175793_a1dc436de7d28685, &__wrap__FuncbuiltinTickkeysTick3875137551537175793_a1dc436de7d28685 },
    { 0x63bf3ec314a37701, true, (void*)&_FuncbuiltinTickvaluesTick1935193042646774172_bfa1df8f4b975136, &__wrap__FuncbuiltinTickvaluesTick1935193042646774172_bfa1df8f4b975136 },
    { 0x63760f80e531ed28, false, (void*)&JVNull_9ff51e1ff95fd846, &__wrap_JVNull_9ff51e1ff95fd846 },
    { 0x1726bc4005a3c3dc, false, (void*)&JV_d50422bd0c926605, &__wrap_JV_d50422bd0c926605 },
    { 0x17364f767983fd15, false, (void*)&JV_1b649e55c06f723a, &__wrap_JV_1b649e55c06f723a },
    { 0x227c200c3daec274, false, (void*)&JV_ad0db3c995ad7a27, &__wrap_JV_ad0db3c995ad7a27 },
    { 0x70ca1dd152d84bcd, false, (void*)&JV_753993827c36bf02, &__wrap_JV_753993827c36bf02 },
    { 0x859e61e78fbbf750, false, (void*)&JV_f6e803bd2d1c03da, &__wrap_JV_f6e803bd2d1c03da },
    { 0xf5a1450bcfca4319, false, (void*)&JV_4433ad2ad9d2c346, &__wrap_JV_4433ad2ad9d2c346 },
    { 0xa8f7d9f588da8df6, false, (void*)&JV_d40575edae42e614, &__wrap_JV_d40575edae42e614 },
    { 0xeaee8f62ab47cef, false, (void*)&JV_35964a9d95ac5c92, &__wrap_JV_35964a9d95ac5c92 },
    { 0x5dd6cb0d5c8c4a06, false, (void*)&JV_95f31162b5b6ffe9, &__wrap_JV_95f31162b5b6ffe9 },
    { 0x22bc330fa2bf0692, false, (void*)&JV_51967b5b9d9b844b, &__wrap_JV_51967b5b9d9b844b },
    { 0x53bc0ae6ebeb8b7a, false, (void*)&JV_bc8ea85e34fd0468, &__wrap_JV_bc8ea85e34fd0468 },
    { 0xb84429e7b6ea9359, false, (void*)&JV_4d5b9fa150f4f6d5, &__wrap_JV_4d5b9fa150f4f6d5 },
    { 0x918e655b05cfe8c9, false, (void*)&JV_456a700091cf7ee5, &__wrap_JV_456a700091cf7ee5 },
    { 0x4581e2f459e94a69, false, (void*)&JV_3cf79a25e4e3f103, &__wrap_JV_3cf79a25e4e3f103 },
    { 0xfc71a55d83d91757, false, (void*)&JV_e1a21daba6c9d98b, &__wrap_JV_e1a21daba6c9d98b },
    { 0x6b51ad18dc347190, true, (void*)&lexer_a2420638876b2c8c, &__wrap_lexer_a2420638876b2c8c },
    { 0x7f43664bf8a674eb, true, (void*)&lexer_c1e9ca186bdbef79, &__wrap_lexer_c1e9ca186bdbef79 },
    { 0x7e36fa532ac3713a, false, (void*)&next_f56994d6cccfe0db, &__wrap_next_f56994d6cccfe0db },
    { 0xeb5871af398530bc, true, (void*)&_lexer_6c213821d171f98b, &__wrap__lexer_6c213821d171f98b },
    { 0x343213338cff7df9, false, (void*)&expect_token_8b3d496f0ac17fc0, &__wrap_expect_token_8b3d496f0ac17fc0 },
    { 0xb34ef6ede2c4489e, false, (void*)&expect_symbol_b0ae2af34afb8c2a, &__wrap_expect_symbol_b0ae2af34afb8c2a },
    { 0x33beab4827eca892, false, (void*)&parse_value_30d0fecba4bd48e6, &__wrap_parse_value_30d0fecba4bd48e6 },
    { 0xb2224d0c100200ab, false, (void*)&read_json_7766a20eeeca4837, &__wrap_read_json_7766a20eeeca4837 },
    { 0x6e0605089f66a01a, false, (void*)&read_json_c123f70fe0ba5e0d, &__wrap_read_json_c123f70fe0ba5e0d },
    { 0xbedae22c23a5e9df, false, (void*)&set_no_trailing_zeros_f1b71a41030849b4, &__wrap_set_no_trailing_zeros_f1b71a41030849b4 },
    { 0x5a66bc39dfca9478, false, (void*)&set_no_empty_arrays_cb9f007816624484, &__wrap_set_no_empty_arrays_cb9f007816624484 },
    { 0x7f23b5cd01bd670c, false, (void*)&set_allow_duplicate_keys_b6a2085fcfe8cb80, &__wrap_set_allow_duplicate_keys_b6a2085fcfe8cb80 },
    { 0xd2a3261dc853e5b3, false, (void*)&write_value_6d63393209ed931d, &__wrap_write_value_6d63393209ed931d },
    { 0x9e30486955fb7ddb, false, (void*)&write_json_fce410ed73c6bcb1, &__wrap_write_json_fce410ed73c6bcb1 },
    { 0xb90366510ade6a64, false, (void*)&write_json_5aa76509a2243f61, &__wrap_write_json_5aa76509a2243f61 },
    { 0xd4ef299eadab4c81, false, (void*)&try_fixing_broken_json_ef0b3a4bda8a0603, &__wrap_try_fixing_broken_json_ef0b3a4bda8a0603 },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
    // [[ init script ]]
    aotLib.emplace(0x2d8b34dbf172be8f, AotFactory(false, (void*)&__init_script, &__wrap___init_script));
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
