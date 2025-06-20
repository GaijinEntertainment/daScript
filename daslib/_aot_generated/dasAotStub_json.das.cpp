#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
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
namespace json { struct _lambda_json_99_1; };
// unused enumeration ConversionResult
namespace json {

struct JsonValue {
    AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> value;
};
}
namespace json {

struct TokenAt {
    AutoVariant<char *,double,bool,void *,int32_t,char *> value;
    int32_t line;
    int32_t row;
};
}
namespace json {

struct _lambda_json_99_1 {
    Func DAS_COMMENT((bool,json::_lambda_json_99_1,json::TokenAt)) __lambda;
    Func DAS_COMMENT((void,json::_lambda_json_99_1 *)) __finalize;
    int32_t __yield;
    char * stext;
    Sequence DAS_COMMENT((int32_t)) tin;
    int32_t ahead;
    TArray<uint8_t> str;
    int32_t line;
    int32_t row;
    bool __anyNumbers_rename_at_153_20;
    char * __num_rename_at_176_20;
    char * __name_rename_at_191_20;
};
}
extern StructInfo __struct_info__69e2ef6fbeda51a0;
extern StructInfo __struct_info__6883e30bf583dbcb;
extern StructInfo __struct_info__6a6e80869f9c2645;
extern TypeInfo __type_info__2012934da3706857;
extern TypeInfo __type_info__994066f681df7bcd;
extern TypeInfo __type_info__3fb2f51b6ebec3f2;
extern TypeInfo __type_info__c8285ff343ef0759;
extern TypeInfo __type_info__e796e1d4af74511c;
extern TypeInfo __type_info__12283e04d98e7c73;
extern TypeInfo __type_info__8dbbd9827382f56f;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af87fe4c863f5252;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__790033b1854b444b;
extern TypeInfo __type_info__fa1aceb92701bc9a;
extern TypeInfo __type_info__2a52f700eb676896;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63d94c8601e773;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__a798e985ff4af814;
extern VarInfo __var_info__a981854735365000;
extern VarInfo __var_info__bd409e4745b09701;
extern VarInfo __var_info__fac8b32671fee393;
extern VarInfo __var_info__3d73996c5681e844;
extern VarInfo __var_info__9a90b9f9a04c1efb;
extern VarInfo __var_info__1ad3feb31b5dc3aa;
extern VarInfo __var_info__78ea3f2e2d01cd52;
extern VarInfo __var_info__45cb83eb461416a7;
extern VarInfo __var_info__e1f7ebda0a2794bd;
extern VarInfo __var_info__94d62a8e591a8b82;
extern VarInfo __var_info__4f373f4f33c2812e;
extern VarInfo __var_info__631e284f448bfb43;
extern VarInfo __var_info__bbf5d91787c27e20;
extern VarInfo __var_info__aa7f354f8130a431;
extern VarInfo __var_info__4ec11f4f333f14a6;
extern VarInfo __var_info__eeae79bd86a73b7b;
extern FuncInfo __func_info__5a16898e20f97124;

TypeInfo * __type_info__a798e985ff4af814_arg_types_var_7629923981941428640[6] = { &__type_info__e796e1d4af74511c, &__type_info__994066f681df7bcd, &__type_info__af63ee4c86020b22, &__type_info__af63d94c8601e773, &__type_info__af63df4c8601f1a5, &__type_info__12283e04d98e7c73 };
const char * __type_info__a798e985ff4af814_arg_names_var_7629923981941428640[6] = { "_object", "_array", "_string", "_number", "_bool", "_null" };
VarInfo __struct_info__69e2ef6fbeda51a0_field_0 =  { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__a798e985ff4af814_arg_types_var_7629923981941428640, __type_info__a798e985ff4af814_arg_names_var_7629923981941428640, 6, 0, nullptr, 57346, TypeSize<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>>::size, UINT64_C(0xa798e985ff4af814), "value", offsetof(json::JsonValue,value), 1 };
VarInfo * __struct_info__69e2ef6fbeda51a0_fields[1] =  { &__struct_info__69e2ef6fbeda51a0_field_0 };
StructInfo __struct_info__69e2ef6fbeda51a0 = {"JsonValue", "json", 28, __struct_info__69e2ef6fbeda51a0_fields, 1, TypeSize<json::JsonValue>::size, UINT64_C(0x0), nullptr, UINT64_C(0x69e2ef6fbeda51a0), 0 };
TypeInfo * __type_info__fac8b32671fee393_arg_types_var_7531112642396806091[6] = { &__type_info__af63ee4c86020b22, &__type_info__af63d94c8601e773, &__type_info__af63df4c8601f1a5, &__type_info__12283e04d98e7c73, &__type_info__af63e44c8601fa24, &__type_info__af63ee4c86020b22 };
const char * __type_info__fac8b32671fee393_arg_names_var_7531112642396806091[6] = { "_string", "_number", "_bool", "_null", "_symbol", "_error" };
VarInfo __struct_info__6883e30bf583dbcb_field_0 =  { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__fac8b32671fee393_arg_types_var_7531112642396806091, __type_info__fac8b32671fee393_arg_names_var_7531112642396806091, 6, 0, nullptr, 24582, TypeSize<AutoVariant<char *,double,bool,void *,int32_t,char *>>::size, UINT64_C(0xfac8b32671fee393), "value", offsetof(json::TokenAt,value), 3 };
VarInfo __struct_info__6883e30bf583dbcb_field_1 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xa981854735365000), "line", offsetof(json::TokenAt,line), 0 };
VarInfo __struct_info__6883e30bf583dbcb_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xbd409e4745b09701), "row", offsetof(json::TokenAt,row), 0 };
VarInfo * __struct_info__6883e30bf583dbcb_fields[3] =  { &__struct_info__6883e30bf583dbcb_field_0, &__struct_info__6883e30bf583dbcb_field_1, &__struct_info__6883e30bf583dbcb_field_2 };
StructInfo __struct_info__6883e30bf583dbcb = {"TokenAt", "json", 12, __struct_info__6883e30bf583dbcb_fields, 3, TypeSize<json::TokenAt>::size, UINT64_C(0x0), nullptr, UINT64_C(0x6883e30bf583dbcb), 0 };
TypeInfo * __type_info__1ad3feb31b5dc3aa_arg_types_var_7669208531150317125[2] = { &__type_info__2a52f700eb676896, &__type_info__fa1aceb92701bc9a };
const char * __type_info__1ad3feb31b5dc3aa_arg_names_var_7669208531150317125[2] = { "__this", "_yield_99" };
VarInfo __struct_info__6a6e80869f9c2645_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__1ad3feb31b5dc3aa_arg_types_var_7669208531150317125, __type_info__1ad3feb31b5dc3aa_arg_names_var_7669208531150317125, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,json::_lambda_json_99_1,json::TokenAt))>::size, UINT64_C(0x1ad3feb31b5dc3aa), "__lambda", offsetof(json::_lambda_json_99_1,__lambda), 0 };
TypeInfo * __type_info__9a90b9f9a04c1efb_arg_types_var_7669208531150317125[1] = { &__type_info__c8285ff343ef0759 };
const char * __type_info__9a90b9f9a04c1efb_arg_names_var_7669208531150317125[1] = { "__this" };
VarInfo __struct_info__6a6e80869f9c2645_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__9a90b9f9a04c1efb_arg_types_var_7669208531150317125, __type_info__9a90b9f9a04c1efb_arg_names_var_7669208531150317125, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,json::_lambda_json_99_1 *))>::size, UINT64_C(0x9a90b9f9a04c1efb), "__finalize", offsetof(json::_lambda_json_99_1,__finalize), 0 };
VarInfo __struct_info__6a6e80869f9c2645_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xe1f7ebda0a2794bd), "__yield", offsetof(json::_lambda_json_99_1,__yield), 0 };
VarInfo __struct_info__6a6e80869f9c2645_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16516, TypeSize<char *>::size, UINT64_C(0xbbf5d91787c27e20), "stext", offsetof(json::_lambda_json_99_1,stext), 4 };
VarInfo __struct_info__6a6e80869f9c2645_field_4 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, TypeSize<Sequence DAS_COMMENT((int32_t))>::size, UINT64_C(0x4ec11f4f333f14a6), "tin", offsetof(json::_lambda_json_99_1,tin), 6 };
VarInfo __struct_info__6a6e80869f9c2645_field_5 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x94d62a8e591a8b82), "ahead", offsetof(json::_lambda_json_99_1,ahead), 0 };
VarInfo __struct_info__6a6e80869f9c2645_field_6 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0xaa7f354f8130a431), "str", offsetof(json::_lambda_json_99_1,str), 10 };
VarInfo __struct_info__6a6e80869f9c2645_field_7 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x4f373f4f33c2812e), "line", offsetof(json::_lambda_json_99_1,line), 0 };
VarInfo __struct_info__6a6e80869f9c2645_field_8 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x631e284f448bfb43), "row", offsetof(json::_lambda_json_99_1,row), 0 };
VarInfo __struct_info__6a6e80869f9c2645_field_9 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0x3d73996c5681e844), "__anyNumbers_rename_at_153_20", offsetof(json::_lambda_json_99_1,__anyNumbers_rename_at_153_20), 0 };
VarInfo __struct_info__6a6e80869f9c2645_field_10 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x45cb83eb461416a7), "__num_rename_at_176_20", offsetof(json::_lambda_json_99_1,__num_rename_at_176_20), 11 };
VarInfo __struct_info__6a6e80869f9c2645_field_11 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x78ea3f2e2d01cd52), "__name_rename_at_191_20", offsetof(json::_lambda_json_99_1,__name_rename_at_191_20), 12 };
VarInfo * __struct_info__6a6e80869f9c2645_fields[12] =  { &__struct_info__6a6e80869f9c2645_field_0, &__struct_info__6a6e80869f9c2645_field_1, &__struct_info__6a6e80869f9c2645_field_2, &__struct_info__6a6e80869f9c2645_field_3, &__struct_info__6a6e80869f9c2645_field_4, &__struct_info__6a6e80869f9c2645_field_5, &__struct_info__6a6e80869f9c2645_field_6, &__struct_info__6a6e80869f9c2645_field_7, &__struct_info__6a6e80869f9c2645_field_8, &__struct_info__6a6e80869f9c2645_field_9, &__struct_info__6a6e80869f9c2645_field_10, &__struct_info__6a6e80869f9c2645_field_11 };
StructInfo __struct_info__6a6e80869f9c2645 = {"_lambda_json_99_1", "json", 30, __struct_info__6a6e80869f9c2645_fields, 12, TypeSize<json::_lambda_json_99_1>::size, UINT64_C(0x0), nullptr, UINT64_C(0x6a6e80869f9c2645), 3 };
VarInfo __func_info__5a16898e20f97124_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, TypeSize<TArray<uint8_t>>::size, UINT64_C(0xeeae79bd86a73b7b), "str", 0, 0 };
VarInfo * __func_info__5a16898e20f97124_fields[1] =  { &__func_info__5a16898e20f97124_field_0 };
FuncInfo __func_info__5a16898e20f97124 = {"invoke block<(str:array<uint8> const#):void> const", "", __func_info__5a16898e20f97124_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x5a16898e20f97124), 0x0 };
TypeInfo * __type_info__2012934da3706857_arg_types[6] = { &__type_info__af63ee4c86020b22, &__type_info__af63d94c8601e773, &__type_info__af63df4c8601f1a5, &__type_info__12283e04d98e7c73, &__type_info__af63e44c8601fa24, &__type_info__af63ee4c86020b22 };
const char * __type_info__2012934da3706857_arg_names[6] = { "_string", "_number", "_bool", "_null", "_symbol", "_error" };
TypeInfo __type_info__2012934da3706857 = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__2012934da3706857_arg_types, __type_info__2012934da3706857_arg_names, 6, 0, nullptr, 24838, TypeSize<AutoVariant<char *,double,bool,void *,int32_t,char *>>::size, UINT64_C(0x2012934da3706857) };
TypeInfo __type_info__994066f681df7bcd = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<json::JsonValue *>>::size, UINT64_C(0x994066f681df7bcd) };
TypeInfo __type_info__3fb2f51b6ebec3f2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__790033b1854b444b, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<json::JsonValue *>::size, UINT64_C(0x3fb2f51b6ebec3f2) };
TypeInfo __type_info__c8285ff343ef0759 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__2a52f700eb676896, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<json::_lambda_json_99_1 *>::size, UINT64_C(0xc8285ff343ef0759) };
TypeInfo __type_info__e796e1d4af74511c = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TTable<char *,json::JsonValue *>>::size, UINT64_C(0xe796e1d4af74511c) };
TypeInfo __type_info__12283e04d98e7c73 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0x12283e04d98e7c73) };
TypeInfo __type_info__8dbbd9827382f56f = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__790033b1854b444b, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<json::JsonValue *>::size, UINT64_C(0x8dbbd9827382f56f) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<bool>::size, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af87fe4c863f5252 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<double>::size, UINT64_C(0xaf87fe4c863f5252) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int32_t>::size, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<StringBuilderWriter>::size, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__790033b1854b444b = { Type::tStructure, &__struct_info__69e2ef6fbeda51a0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<json::JsonValue>::size, UINT64_C(0x790033b1854b444b) };
TypeInfo __type_info__fa1aceb92701bc9a = { Type::tStructure, &__struct_info__6883e30bf583dbcb, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24582, TypeSize<json::TokenAt>::size, UINT64_C(0xfa1aceb92701bc9a) };
TypeInfo __type_info__2a52f700eb676896 = { Type::tStructure, &__struct_info__6a6e80869f9c2645, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<json::_lambda_json_99_1>::size, UINT64_C(0x2a52f700eb676896) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63d94c8601e773 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<double>::size, UINT64_C(0xaf63d94c8601e773) };
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

TypeInfo * __tinfo_0[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_1[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_2[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_3[8] = { &__type_info__af90fe4c864e9d52, &__type_info__2012934da3706857, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_4[6] = { &__type_info__af90fe4c864e9d52, &__type_info__2012934da3706857, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_5[10] = { &__type_info__af90fe4c864e9d52, &__type_info__2012934da3706857, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_6[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_7[8] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_8[6] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_9[8] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_10[8] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_11[5] = { &__type_info__2012934da3706857, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_12[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_13[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_14[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_15[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_16[2] = { &__type_info__37d36026a6078a42, &__type_info__af87fe4c863f5252 };
TypeInfo * __tinfo_17[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_18[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_19[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_20[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_22[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_23[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_24[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_25[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_26[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_27[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_28[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_29[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_30[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_31[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_32[2] = { &__type_info__af90fe4c864e9d52, &__type_info__8dbbd9827382f56f };
TypeInfo * __tinfo_33[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_34[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_35[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_118143448cef53b4 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1192_0 );
inline void finalize_ad8689b2c3216ff3 ( Context * __context__, json::_lambda_json_99_1 & ____this_rename_at_99_1 );
inline Sequence DAS_COMMENT((json::TokenAt)) _FuncbuiltinTickeachTick9663565701927713696_f82670fc7e714dfa ( Context * __context__, Lambda DAS_COMMENT((bool,json::TokenAt)) const  __lam_rename_at_1299_2 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_80eac3dd8d64249e ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __it_rename_at_1233_4, int32_t & __value_rename_at_1233_5 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_b9a088e792341ef2 ( Context * __context__, char * const  __str_rename_at_1266_6 );
inline void _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_8, uint8_t __value_rename_at_165_9 );
inline bool _Func_lambda_json_99_1Tickfunction_fbd838db6f25aa6d ( Context * __context__, json::_lambda_json_99_1 & ____this_rename_at_99_10, json::TokenAt & ___yield_99_rename_at_99_11 );
inline void _Func_lambda_json_99_1Tickfinalizer_dd59807cf54b4451 ( Context * __context__, json::_lambda_json_99_1 * ____this_rename_at_99_12 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_e247a8d469c511cc ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __it_rename_at_1233_13, json::TokenAt & __value_rename_at_1233_14 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5302216a93726f8a ( Context * __context__, TArray<json::JsonValue *> & __Arr_rename_at_181_15, json::JsonValue * __value_rename_at_181_16 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_f90c5a9ce3ca9524 ( Context * __context__, TTable<char *,json::JsonValue *> const  & __Tab_rename_at_1005_17, char * const  __at_rename_at_1005_18 );
inline void _FuncbuiltinTickinsertTick4246857231018487965_30a7dd4f934af59c ( Context * __context__, TTable<char *,json::JsonValue *> & __Tab_rename_at_841_19, char * const  __at_rename_at_841_20, json::JsonValue * __val_rename_at_841_21 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_5d08049ff7d797e2 ( Context * __context__, TTable<char *,json::JsonValue *> const  & __a_rename_at_1138_22 );
inline Sequence DAS_COMMENT((json::JsonValue * &)) _FuncbuiltinTickvaluesTick1935193042646774172_e8953d3b3cbab6d5 ( Context * __context__, TTable<char *,json::JsonValue *> const  & __a_rename_at_1153_24 );
inline json::JsonValue * JV_35aac71ff80df937 ( Context * __context__, char * const  __v_rename_at_48_26 );
inline json::JsonValue * JV_411a9817ab566332 ( Context * __context__, double __v_rename_at_53_27 );
inline json::JsonValue * JV_b2f8c005ec090a85 ( Context * __context__, bool __v_rename_at_57_28 );
inline json::JsonValue * JVNull_7972f25b86b1842d ( Context * __context__ );
inline json::JsonValue * JV_eda1cfc37ee5473f ( Context * __context__, TTable<char *,json::JsonValue *> & __v_rename_at_66_29 );
inline json::JsonValue * JV_eb50e491960f713b ( Context * __context__, TArray<json::JsonValue *> & __v_rename_at_70_30 );
inline Sequence DAS_COMMENT((json::TokenAt)) lexer_e4c23188cefd4e94 ( Context * __context__, char * const  __text_rename_at_74_31 );
inline Sequence DAS_COMMENT((json::TokenAt)) lexer_6e4965f12d35e1fb ( Context * __context__, TArray<uint8_t> const  & __text_rename_at_79_33 );
inline bool next_971a4a5b86ed11b3 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __text_rename_at_84_35, int32_t & __character_rename_at_84_36, int32_t & __line_rename_at_84_37, int32_t & __row_rename_at_84_38 );
inline Sequence DAS_COMMENT((json::TokenAt)) _lexer_a97174d736e96fde ( Context * __context__, char * __stext_rename_at_98_39 );
inline bool expect_token_4c6c072b3f5fa12b ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __itv_rename_at_219_40, json::TokenAt & __ahead_rename_at_219_41, int32_t __vindex_rename_at_219_42, char * & __error_rename_at_219_43 );
inline bool expect_symbol_21380f29090e81b6 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __itv_rename_at_231_44, json::TokenAt & __ahead_rename_at_231_45, int32_t __sym_rename_at_231_46, char * & __error_rename_at_231_47 );
inline json::JsonValue * parse_value_1716bf8ce4d4c65a ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) & __itv_rename_at_246_48, char * & __error_rename_at_246_49 );
inline json::JsonValue * read_json_809f579b141dd044 ( Context * __context__, char * const  __text_rename_at_342_59, char * & __error_rename_at_342_60 );
inline json::JsonValue * read_json_4ba85080204587fb ( Context * __context__, TArray<uint8_t> const  & __text_rename_at_352_63, char * & __error_rename_at_352_64 );
inline bool set_no_trailing_zeros_995e7aa57f8a54c3 ( Context * __context__, bool __value_rename_at_362_67 );
inline bool set_no_empty_arrays_42235cc25e5283a4 ( Context * __context__, bool __value_rename_at_371_69 );
inline bool set_allow_duplicate_keys_b0d075b8cdc0fb30 ( Context * __context__, bool __value_rename_at_380_71 );
inline void write_value_b3cf9236674b49df ( Context * __context__, StringBuilderWriter & __writer_rename_at_388_73, json::JsonValue * const  __jsv_rename_at_388_74, int32_t __depth_rename_at_388_75 );
inline char * write_json_b2bd5d3e5bfc13b7 ( Context * __context__, json::JsonValue * const  __val_rename_at_462_83 );
inline char * write_json_746b512bcec82883 ( Context * __context__, json::JsonValue * const  __val_rename_at_470_86 );
inline char * try_fixing_broken_json_1c995197dfb2261e ( Context * __context__, char * __bad_rename_at_477_87 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0xc713ddb3e835448d>(__context__) = 0;/*Token_string*/
    das_global<int32_t,0x96ac244c9265d269>(__context__) = 4;/*Token_symbol*/
    das_global<bool,0xd3b8b85b9bf19183>(__context__) = false;/*no_trailing_zeros*/
    das_global<bool,0xab7f08dbdeea5c03>(__context__) = false;/*no_empty_arrays*/
    das_global<bool,0x30e401a8d3210d31>(__context__) = false;/*allow_duplicate_keys*/
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_118143448cef53b4 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1192_0 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1192_0),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_ad8689b2c3216ff3 ( Context * __context__, json::_lambda_json_99_1 &  ____this_rename_at_99_1 )
{
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_1.tin),__context__);
    _FuncbuiltinTickfinalizeTick13836114024949725080_118143448cef53b4(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_1.str));
    memset((void*)&(____this_rename_at_99_1), 0, TypeSize<json::_lambda_json_99_1>::size);
}

inline Sequence DAS_COMMENT((json::TokenAt)) _FuncbuiltinTickeachTick9663565701927713696_f82670fc7e714dfa ( Context * __context__, Lambda DAS_COMMENT((bool,json::TokenAt)) const  __lam_rename_at_1299_2 )
{
    Sequence DAS_COMMENT((json::TokenAt)) __it_rename_at_1301_3;das_zero(__it_rename_at_1301_3);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__it_rename_at_1301_3),__lam_rename_at_1299_2,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(__it_rename_at_1301_3);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_80eac3dd8d64249e ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __it_rename_at_1233_4, int32_t & __value_rename_at_1233_5 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1233_4),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1233_5)),__context__));
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_b9a088e792341ef2 ( Context * __context__, char * const  __str_rename_at_1266_6 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1267_7;das_zero(__it_rename_at_1267_7);
    builtin_make_string_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1267_7),__str_rename_at_1266_6,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1267_7);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_8, uint8_t __value_rename_at_165_9 )
{
    das_copy(__Arr_rename_at_165_8(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_8),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_9);
}

inline bool _Func_lambda_json_99_1Tickfunction_fbd838db6f25aa6d ( Context * __context__, json::_lambda_json_99_1 &  ____this_rename_at_99_10, json::TokenAt &  ___yield_99_rename_at_99_11 )
{
    switch (____this_rename_at_99_10.__yield) {
    case 0: goto label_0;
    case 34: goto label_34;
    case 1: goto label_1;
    case 32: goto label_32;
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
    case 30: goto label_30;
    case 12: goto label_12;
    case 13: goto label_13;
    case 14: goto label_14;
    case 28: goto label_28;
    case 15: goto label_15;
    case 23: goto label_23;
    case 16: goto label_16;
    case 21: goto label_21;
    case 17: goto label_17;
    case 19: goto label_19;
    case 18: goto label_18;
    case 20: goto label_20;
    case 22: goto label_22;
    case 24: goto label_24;
    case 26: goto label_26;
    case 25: goto label_25;
    case 27: goto label_27;
    case 29: goto label_29;
    case 31: goto label_31;
    case 33: goto label_33;
    case 35: goto label_35;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    memset((void*)&(____this_rename_at_99_10.tin), 0, TypeSize<Sequence DAS_COMMENT((int32_t))>::size);
    das_move(____this_rename_at_99_10.tin,_FuncbuiltinTickeachTick4044332007967089362_b9a088e792341ef2(__context__,____this_rename_at_99_10.stext));
    das_copy(____this_rename_at_99_10.ahead,32);
    memset((void*)&(____this_rename_at_99_10.str), 0, TypeSize<TArray<uint8_t>>::size);
    das_copy(____this_rename_at_99_10.line,1);
    das_copy(____this_rename_at_99_10.row,0);
    label_34:;;
    if ( !!(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin))) )
    {
        goto label_35;
    };
    while ( is_white_space(____this_rename_at_99_10.ahead) && next_971a4a5b86ed11b3(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead,____this_rename_at_99_10.line,____this_rename_at_99_10.row) )
    {
    };
    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin)) )
    {
        return das_auto_cast<bool>::cast(false);
    };
    if ( !((((((____this_rename_at_99_10.ahead == 91) || (____this_rename_at_99_10.ahead == 93)) || (____this_rename_at_99_10.ahead == 123)) || (____this_rename_at_99_10.ahead == 125)) || (____this_rename_at_99_10.ahead == 58)) || (____this_rename_at_99_10.ahead == 44)) )
    {
        goto label_32;
    };
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_116;
        das_zero(__mks_116);
        das_copy((__mks_116.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_116;
            das_get_auto_variant_field<int32_t,4,char *,double,bool,void *,int32_t,char *>::set(__mkv_116) = ____this_rename_at_99_10.ahead;
            return __mkv_116;
        })())));
        das_copy((__mks_116.line),(____this_rename_at_99_10.line));
        das_copy((__mks_116.row),(____this_rename_at_99_10.row));
        return __mks_116;
    })()));
    das_copy(____this_rename_at_99_10.__yield,1);
    return das_auto_cast<bool>::cast(true);
    label_1:;;
    _FuncbuiltinTicknextTick17450348357676149856_80eac3dd8d64249e(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead);
    goto label_33;
    label_32:;;
    if ( !(____this_rename_at_99_10.ahead == 34) )
    {
        goto label_30;
    };
    label_7:;;
    if ( !(next_971a4a5b86ed11b3(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead,____this_rename_at_99_10.line,____this_rename_at_99_10.row) && (____this_rename_at_99_10.ahead != 34)) )
    {
        goto label_8;
    };
    if ( !(____this_rename_at_99_10.ahead == 92) )
    {
        goto label_5;
    };
    if ( !next_971a4a5b86ed11b3(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead,____this_rename_at_99_10.line,____this_rename_at_99_10.row) )
    {
        goto label_3;
    };
    if ( ____this_rename_at_99_10.ahead == 98 )
    {
        das_copy(____this_rename_at_99_10.ahead,8);
    } else if ( ____this_rename_at_99_10.ahead == 102 )
    {
        das_copy(____this_rename_at_99_10.ahead,12);
    } else if ( ____this_rename_at_99_10.ahead == 110 )
    {
        das_copy(____this_rename_at_99_10.ahead,10);
    } else if ( ____this_rename_at_99_10.ahead == 114 )
    {
        das_copy(____this_rename_at_99_10.ahead,13);
    } else if ( ____this_rename_at_99_10.ahead == 116 )
    {
        das_copy(____this_rename_at_99_10.ahead,9);
    } else if ( ____this_rename_at_99_10.ahead == 117 )
    {
        das_copy(____this_rename_at_99_10.ahead,92);
    };
    _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
    goto label_4;
    label_3:;;
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_137;
        das_zero(__mks_137);
        das_copy((__mks_137.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_137;
            das_get_auto_variant_field<char *,5,char *,double,bool,void *,int32_t,char *>::set(__mkv_137) = ((char *) "string escape sequence exceeds text");
            return __mkv_137;
        })())));
        das_copy((__mks_137.line),(____this_rename_at_99_10.line));
        das_copy((__mks_137.row),(____this_rename_at_99_10.row));
        return __mks_137;
    })()));
    das_copy(____this_rename_at_99_10.__yield,2);
    return das_auto_cast<bool>::cast(true);
    label_2:;;
    return das_auto_cast<bool>::cast(false);
    label_4:;;
    goto label_6;
    label_5:;;
    _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
    label_6:;;
    goto label_7;
    label_8:;;
    if ( !builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin)) )
    {
        goto label_10;
    };
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_145;
        das_zero(__mks_145);
        das_copy((__mks_145.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_145;
            das_get_auto_variant_field<char *,5,char *,double,bool,void *,int32_t,char *>::set(__mkv_145) = ((char *) "string exceeds text");
            return __mkv_145;
        })())));
        das_copy((__mks_145.line),(____this_rename_at_99_10.line));
        das_copy((__mks_145.row),(____this_rename_at_99_10.row));
        return __mks_145;
    })()));
    das_copy(____this_rename_at_99_10.__yield,9);
    return das_auto_cast<bool>::cast(true);
    label_9:;;
    return das_auto_cast<bool>::cast(false);
    label_10:;;
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_148;
        das_zero(__mks_148);
        das_copy((__mks_148.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_148;
            das_get_auto_variant_field<char *,0,char *,double,bool,void *,int32_t,char *>::set(__mkv_148) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
            return __mkv_148;
        })())));
        das_copy((__mks_148.line),(____this_rename_at_99_10.line));
        das_copy((__mks_148.row),(____this_rename_at_99_10.row));
        return __mks_148;
    })()));
    das_copy(____this_rename_at_99_10.__yield,11);
    return das_auto_cast<bool>::cast(true);
    label_11:;;
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTicknextTick17450348357676149856_80eac3dd8d64249e(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead);
    goto label_31;
    label_30:;;
    if ( !(((____this_rename_at_99_10.ahead == 43) || (____this_rename_at_99_10.ahead == 45)) || is_number(____this_rename_at_99_10.ahead)) )
    {
        goto label_28;
    };
    _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
    das_copy(____this_rename_at_99_10.__anyNumbers_rename_at_153_20,is_number(____this_rename_at_99_10.ahead));
    while ( next_971a4a5b86ed11b3(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead,____this_rename_at_99_10.line,____this_rename_at_99_10.row) && is_number(____this_rename_at_99_10.ahead) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
        das_copy(____this_rename_at_99_10.__anyNumbers_rename_at_153_20,true);
    };
    if ( !(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin))) && (____this_rename_at_99_10.ahead == 46) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
        while ( next_971a4a5b86ed11b3(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead,____this_rename_at_99_10.line,____this_rename_at_99_10.row) && is_number(____this_rename_at_99_10.ahead) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
            das_copy(____this_rename_at_99_10.__anyNumbers_rename_at_153_20,true);
        };
    };
    if ( !(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin))) && ((____this_rename_at_99_10.ahead == 101) || (____this_rename_at_99_10.ahead == 69)) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
        next_971a4a5b86ed11b3(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead,____this_rename_at_99_10.line,____this_rename_at_99_10.row);
        if ( ((____this_rename_at_99_10.ahead == 43) || (____this_rename_at_99_10.ahead == 45)) || is_number(____this_rename_at_99_10.ahead) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
        };
        while ( next_971a4a5b86ed11b3(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead,____this_rename_at_99_10.line,____this_rename_at_99_10.row) && is_number(____this_rename_at_99_10.ahead) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
            das_copy(____this_rename_at_99_10.__anyNumbers_rename_at_153_20,true);
        };
    };
    das_copy(____this_rename_at_99_10.__num_rename_at_176_20,((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( !!(____this_rename_at_99_10.__anyNumbers_rename_at_153_20) )
    {
        goto label_13;
    };
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_178;
        das_zero(__mks_178);
        das_copy((__mks_178.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_178;
            das_get_auto_variant_field<char *,5,char *,double,bool,void *,int32_t,char *>::set(__mkv_178) = das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_0, cast<char * const >::from(((char *) "invalid number ")), cast<char *>::from(____this_rename_at_99_10.__num_rename_at_176_20)));
            return __mkv_178;
        })())));
        das_copy((__mks_178.line),(____this_rename_at_99_10.line));
        das_copy((__mks_178.row),(____this_rename_at_99_10.row));
        return __mks_178;
    })()));
    das_copy(____this_rename_at_99_10.__yield,12);
    return das_auto_cast<bool>::cast(true);
    label_12:;;
    return das_auto_cast<bool>::cast(false);
    label_13:;;
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_181;
        das_zero(__mks_181);
        das_copy((__mks_181.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_181;
            das_get_auto_variant_field<double,1,char *,double,bool,void *,int32_t,char *>::set(__mkv_181) = string_to_double(____this_rename_at_99_10.__num_rename_at_176_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            return __mkv_181;
        })())));
        das_copy((__mks_181.line),(____this_rename_at_99_10.line));
        das_copy((__mks_181.row),(____this_rename_at_99_10.row));
        return __mks_181;
    })()));
    das_copy(____this_rename_at_99_10.__yield,14);
    return das_auto_cast<bool>::cast(true);
    label_14:;;
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    delete_string(____this_rename_at_99_10.__num_rename_at_176_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    goto label_29;
    label_28:;;
    if ( !is_alpha(____this_rename_at_99_10.ahead) )
    {
        goto label_26;
    };
    _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
    while ( next_971a4a5b86ed11b3(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_99_10.tin),____this_rename_at_99_10.ahead,____this_rename_at_99_10.line,____this_rename_at_99_10.row) && is_alpha(____this_rename_at_99_10.ahead) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),uint8_t(____this_rename_at_99_10.ahead));
    };
    das_copy(____this_rename_at_99_10.__name_rename_at_191_20,((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( !(SimPolicy<char *>::Equ(cast<char *>::from(____this_rename_at_99_10.__name_rename_at_191_20),cast<char *>::from(((char *) "true")),*__context__,nullptr)) )
    {
        goto label_23;
    };
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_193;
        das_zero(__mks_193);
        das_copy((__mks_193.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_193;
            das_get_auto_variant_field<bool,2,char *,double,bool,void *,int32_t,char *>::set(__mkv_193) = true;
            return __mkv_193;
        })())));
        das_copy((__mks_193.line),(____this_rename_at_99_10.line));
        das_copy((__mks_193.row),(____this_rename_at_99_10.row));
        return __mks_193;
    })()));
    das_copy(____this_rename_at_99_10.__yield,15);
    return das_auto_cast<bool>::cast(true);
    label_15:;;
    goto label_24;
    label_23:;;
    if ( !(SimPolicy<char *>::Equ(cast<char *>::from(____this_rename_at_99_10.__name_rename_at_191_20),cast<char *>::from(((char *) "false")),*__context__,nullptr)) )
    {
        goto label_21;
    };
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_195;
        das_zero(__mks_195);
        das_copy((__mks_195.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_195;
            das_get_auto_variant_field<bool,2,char *,double,bool,void *,int32_t,char *>::set(__mkv_195) = false;
            return __mkv_195;
        })())));
        das_copy((__mks_195.line),(____this_rename_at_99_10.line));
        das_copy((__mks_195.row),(____this_rename_at_99_10.row));
        return __mks_195;
    })()));
    das_copy(____this_rename_at_99_10.__yield,16);
    return das_auto_cast<bool>::cast(true);
    label_16:;;
    goto label_22;
    label_21:;;
    if ( !(SimPolicy<char *>::Equ(cast<char *>::from(____this_rename_at_99_10.__name_rename_at_191_20),cast<char *>::from(((char *) "null")),*__context__,nullptr)) )
    {
        goto label_19;
    };
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_197;
        das_zero(__mks_197);
        das_copy((__mks_197.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_197;
            das_get_auto_variant_field<void *,3,char *,double,bool,void *,int32_t,char *>::set(__mkv_197) = nullptr;
            return __mkv_197;
        })())));
        das_copy((__mks_197.line),(____this_rename_at_99_10.line));
        das_copy((__mks_197.row),(____this_rename_at_99_10.row));
        return __mks_197;
    })()));
    das_copy(____this_rename_at_99_10.__yield,17);
    return das_auto_cast<bool>::cast(true);
    label_17:;;
    goto label_20;
    label_19:;;
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_199;
        das_zero(__mks_199);
        das_copy((__mks_199.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_199;
            das_get_auto_variant_field<char *,5,char *,double,bool,void *,int32_t,char *>::set(__mkv_199) = das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_1, cast<char * const >::from(((char *) "invalid name ")), cast<char *>::from(____this_rename_at_99_10.__name_rename_at_191_20)));
            return __mkv_199;
        })())));
        das_copy((__mks_199.line),(____this_rename_at_99_10.line));
        das_copy((__mks_199.row),(____this_rename_at_99_10.row));
        return __mks_199;
    })()));
    das_copy(____this_rename_at_99_10.__yield,18);
    return das_auto_cast<bool>::cast(true);
    label_18:;;
    return das_auto_cast<bool>::cast(false);
    label_20:;;
    label_22:;;
    label_24:;;
    delete_string(____this_rename_at_99_10.__name_rename_at_191_20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_array_clear(das_arg<TArray<uint8_t>>::pass(____this_rename_at_99_10.str),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    goto label_27;
    label_26:;;
    das_copy(___yield_99_rename_at_99_11,(([&]() -> json::TokenAt {
        json::TokenAt __mks_207;
        das_zero(__mks_207);
        das_copy((__mks_207.value),((([&]() -> AutoVariant<char *,double,bool,void *,int32_t,char *> {
            AutoVariant<char *,double,bool,void *,int32_t,char *> __mkv_207;
            das_get_auto_variant_field<char *,5,char *,double,bool,void *,int32_t,char *>::set(__mkv_207) = das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_2, cast<char * const >::from(((char *) "invalid character `")), cast<char * const >::from(((char * const )(to_string_char(____this_rename_at_99_10.ahead,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(____this_rename_at_99_10.ahead)));
            return __mkv_207;
        })())));
        das_copy((__mks_207.line),(____this_rename_at_99_10.line));
        das_copy((__mks_207.row),(____this_rename_at_99_10.row));
        return __mks_207;
    })()));
    das_copy(____this_rename_at_99_10.__yield,25);
    return das_auto_cast<bool>::cast(true);
    label_25:;;
    return das_auto_cast<bool>::cast(false);
    label_27:;;
    label_29:;;
    label_31:;;
    label_33:;;
    goto label_34;
    label_35:;;
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_json_99_1Tickfinalizer_dd59807cf54b4451 ( Context * __context__, json::_lambda_json_99_1 * ____this_rename_at_99_12 )
{
    delete_string(das_deref(__context__,____this_rename_at_99_12).stext,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    finalize_ad8689b2c3216ff3(__context__,das_arg<json::_lambda_json_99_1>::pass(das_deref(__context__,____this_rename_at_99_12)));
    das_delete_lambda_struct<json::_lambda_json_99_1 *>::clear(__context__,____this_rename_at_99_12);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_e247a8d469c511cc ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __it_rename_at_1233_13, json::TokenAt &  __value_rename_at_1233_14 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__it_rename_at_1233_13),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1233_14)),__context__));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5302216a93726f8a ( Context * __context__, TArray<json::JsonValue *> &  __Arr_rename_at_181_15, json::JsonValue * __value_rename_at_181_16 )
{
    das_copy(__Arr_rename_at_181_15(builtin_array_push_back(das_arg<TArray<json::JsonValue *>>::pass(__Arr_rename_at_181_15),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_16);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_f90c5a9ce3ca9524 ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __Tab_rename_at_1005_17, char * const  __at_rename_at_1005_18 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1005_17,__at_rename_at_1005_18));
}

inline void _FuncbuiltinTickinsertTick4246857231018487965_30a7dd4f934af59c ( Context * __context__, TTable<char *,json::JsonValue *> &  __Tab_rename_at_841_19, char * const  __at_rename_at_841_20, json::JsonValue * __val_rename_at_841_21 )
{
    das_copy(__Tab_rename_at_841_19(__at_rename_at_841_20,__context__),__val_rename_at_841_21);
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_5d08049ff7d797e2 ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __a_rename_at_1138_22 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1139_23;das_zero(__it_rename_at_1139_23);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1139_23),__a_rename_at_1138_22,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1139_23);
}

inline Sequence DAS_COMMENT((json::JsonValue * &)) _FuncbuiltinTickvaluesTick1935193042646774172_e8953d3b3cbab6d5 ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __a_rename_at_1153_24 )
{
    Sequence DAS_COMMENT((json::JsonValue * *)) __it_rename_at_1154_25;das_zero(__it_rename_at_1154_25);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((json::JsonValue * const ))>::pass(__it_rename_at_1154_25),__a_rename_at_1153_24,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::JsonValue * &))>::cast(__it_rename_at_1154_25);
}

inline json::JsonValue * JV_35aac71ff80df937 ( Context * __context__, char * const  __v_rename_at_48_26 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_50;
        das_zero(__mks_50);
        das_move((__mks_50.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_50;
            das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_50) = __v_rename_at_48_26;
            return __mkv_50;
        })())));
        return __mks_50;
    })())));
}

inline json::JsonValue * JV_411a9817ab566332 ( Context * __context__, double __v_rename_at_53_27 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_54;
        das_zero(__mks_54);
        das_move((__mks_54.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_54;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_54) = __v_rename_at_53_27;
            return __mkv_54;
        })())));
        return __mks_54;
    })())));
}

inline json::JsonValue * JV_b2f8c005ec090a85 ( Context * __context__, bool __v_rename_at_57_28 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_58;
        das_zero(__mks_58);
        das_move((__mks_58.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_58;
            das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_58) = __v_rename_at_57_28;
            return __mkv_58;
        })())));
        return __mks_58;
    })())));
}

inline json::JsonValue * JVNull_7972f25b86b1842d ( Context * __context__ )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_63;
        das_zero(__mks_63);
        das_move((__mks_63.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_63;
            das_get_auto_variant_field<void *,5,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_63) = nullptr;
            return __mkv_63;
        })())));
        return __mks_63;
    })())));
}

inline json::JsonValue * JV_eda1cfc37ee5473f ( Context * __context__, TTable<char *,json::JsonValue *> &  __v_rename_at_66_29 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_67;
        das_zero(__mks_67);
        das_move((__mks_67.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_67;
            das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_67) = __v_rename_at_66_29;
            return __mkv_67;
        })())));
        return __mks_67;
    })())));
}

inline json::JsonValue * JV_eb50e491960f713b ( Context * __context__, TArray<json::JsonValue *> &  __v_rename_at_70_30 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_71;
        das_zero(__mks_71);
        das_move((__mks_71.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_71;
            das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_71) = __v_rename_at_70_30;
            return __mkv_71;
        })())));
        return __mks_71;
    })())));
}

inline Sequence DAS_COMMENT((json::TokenAt)) lexer_e4c23188cefd4e94 ( Context * __context__, char * const  __text_rename_at_74_31 )
{
    char * __stext_rename_at_75_32 = ((char *)(char *)(((char * const )(builtin_string_clone(__text_rename_at_74_31,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(_lexer_a97174d736e96fde(__context__,__stext_rename_at_75_32));
}

inline Sequence DAS_COMMENT((json::TokenAt)) lexer_6e4965f12d35e1fb ( Context * __context__, TArray<uint8_t> const  &  __text_rename_at_79_33 )
{
    char * __stext_rename_at_80_34 = ((char *)(char *)(((char * const )(builtin_string_from_array(__text_rename_at_79_33,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(_lexer_a97174d736e96fde(__context__,__stext_rename_at_80_34));
}

inline bool next_971a4a5b86ed11b3 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __text_rename_at_84_35, int32_t & __character_rename_at_84_36, int32_t & __line_rename_at_84_37, int32_t & __row_rename_at_84_38 )
{
    if ( _FuncbuiltinTicknextTick17450348357676149856_80eac3dd8d64249e(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__text_rename_at_84_35),__character_rename_at_84_36) )
    {
        if ( is_new_line(__character_rename_at_84_36) )
        {
            ++__line_rename_at_84_37;
            das_copy(__row_rename_at_84_38,0);
        } else {
            ++__row_rename_at_84_38;
        };
        return das_auto_cast<bool>::cast(true);
    } else {
        return das_auto_cast<bool>::cast(false);
    };
}

inline Sequence DAS_COMMENT((json::TokenAt)) _lexer_a97174d736e96fde ( Context * __context__, char * __stext_rename_at_98_39 )
{
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((json::TokenAt))>::cast(_FuncbuiltinTickeachTick9663565701927713696_f82670fc7e714dfa(__context__,das_ascend<bool,json::_lambda_json_99_1>::make(__context__,&__type_info__2a52f700eb676896,(([&]() -> json::_lambda_json_99_1 {
        json::_lambda_json_99_1 __mks_99;
        das_zero(__mks_99);
        das_copy((__mks_99.__lambda),(Func(__context__->fnByMangledName(/*@json::_lambda_json_99_1`function XS<json::_lambda_json_99_1> S<json::TokenAt>*/ 0x9b5a662e30a6b76d))));
        das_copy((__mks_99.__finalize),(Func(__context__->fnByMangledName(/*@json::_lambda_json_99_1`finalizer X1<S<json::_lambda_json_99_1>>?*/ 0x7378f83302232f7))));
        das_copy((__mks_99.stext),(__stext_rename_at_98_39));
        return __mks_99;
    })()))));
}

inline bool expect_token_4c6c072b3f5fa12b ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __itv_rename_at_219_40, json::TokenAt &  __ahead_rename_at_219_41, int32_t __vindex_rename_at_219_42, char * & __error_rename_at_219_43 )
{
    if ( !_FuncbuiltinTicknextTick17450348357676149856_e247a8d469c511cc(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_219_40),das_arg<json::TokenAt>::pass(__ahead_rename_at_219_41)) )
    {
        das_copy(__error_rename_at_219_43,((char *) "unexected eos"));
        return das_auto_cast<bool>::cast(false);
    } else if ( variant_index(das_arg<AutoVariant<char *,double,bool,void *,int32_t,char *>>::pass(__ahead_rename_at_219_41.value)) != __vindex_rename_at_219_42 )
    {
        das_copy(__error_rename_at_219_43,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_3, cast<char * const >::from(((char *) "unexpected ")), cast<AutoVariant<char *,double,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_219_41.value), cast<char * const >::from(((char *) ", expecting variant ")), cast<int32_t>::from(__vindex_rename_at_219_42), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_219_41.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_219_41.row))));
        return das_auto_cast<bool>::cast(false);
    } else {
        return das_auto_cast<bool>::cast(true);
    };
}

inline bool expect_symbol_21380f29090e81b6 ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __itv_rename_at_231_44, json::TokenAt &  __ahead_rename_at_231_45, int32_t __sym_rename_at_231_46, char * & __error_rename_at_231_47 )
{
    if ( !_FuncbuiltinTicknextTick17450348357676149856_e247a8d469c511cc(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_231_44),das_arg<json::TokenAt>::pass(__ahead_rename_at_231_45)) )
    {
        das_copy(__error_rename_at_231_47,((char *) "unexected eos"));
        return das_auto_cast<bool>::cast(false);
    } else if ( !das_get_auto_variant_field<int32_t,4,char *,double,bool,void *,int32_t,char *>::is(__ahead_rename_at_231_45.value) )
    {
        das_copy(__error_rename_at_231_47,das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_4, cast<char * const >::from(((char *) "unexpected ")), cast<AutoVariant<char *,double,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_231_45.value), cast<char * const >::from(((char *) ", expecting symbol at ")), cast<int32_t>::from(__ahead_rename_at_231_45.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_231_45.row))));
        return das_auto_cast<bool>::cast(false);
    } else if ( !(das_get_auto_variant_field<int32_t,4,char *,double,bool,void *,int32_t,char *>::as(__ahead_rename_at_231_45.value,__context__) == __sym_rename_at_231_46) )
    {
        das_copy(__error_rename_at_231_47,das_string_builder(__context__,SimNode_AotInterop<10>(__tinfo_5, cast<char * const >::from(((char *) "unexpected ")), cast<AutoVariant<char *,double,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_231_45.value), cast<char * const >::from(((char *) ", expecting symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sym_rename_at_231_46,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sym_rename_at_231_46), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_231_45.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_231_45.row))));
        return das_auto_cast<bool>::cast(true);
    } else {
        return das_auto_cast<bool>::cast(true);
    };
}

inline json::JsonValue * parse_value_1716bf8ce4d4c65a ( Context * __context__, Sequence DAS_COMMENT((json::TokenAt)) &  __itv_rename_at_246_48, char * & __error_rename_at_246_49 )
{
    json::TokenAt __ahead_rename_at_247_50;das_zero(__ahead_rename_at_247_50);
    if ( !_FuncbuiltinTicknextTick17450348357676149856_e247a8d469c511cc(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48),das_arg<json::TokenAt>::pass(__ahead_rename_at_247_50)) )
    {
        return das_auto_cast<json::JsonValue *>::cast(nullptr);
    } else {
        if ( das_get_auto_variant_field<int32_t,4,char *,double,bool,void *,int32_t,char *>::is(__ahead_rename_at_247_50.value) )
        {
            int32_t __sym_rename_at_252_51 = ((int32_t)das_get_auto_variant_field<int32_t,4,char *,double,bool,void *,int32_t,char *>::as(__ahead_rename_at_247_50.value,__context__));
            if ( __sym_rename_at_252_51 == 93 )
            {
                das_copy(__error_rename_at_246_49,das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_6, cast<char * const >::from(((char *) "unexpected ] at ")), cast<int32_t>::from(__ahead_rename_at_247_50.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_247_50.row))));
                return das_auto_cast<json::JsonValue *>::cast(nullptr);
            } else {
                if ( __sym_rename_at_252_51 == 91 )
                {
                    TArray<json::JsonValue *> __arr_rename_at_258_52;das_zero(__arr_rename_at_258_52);
                    while ( !builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48)) )
                    {
                        json::JsonValue * __value_rename_at_260_53 = parse_value_1716bf8ce4d4c65a(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48),__error_rename_at_246_49);
                        if ( __value_rename_at_260_53 == nullptr )
                        {
                            if ( builtin_string_startswith(__error_rename_at_246_49,((char *) "unexpected ]"),__context__) && (builtin_array_size(das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_258_52)) == 0) )
                            {
                                das_copy(__error_rename_at_246_49,nullptr);
                                return das_auto_cast<json::JsonValue *>::cast(JV_eb50e491960f713b(__context__,das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_258_52)));
                            } else {
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            };
                        };
                        _FuncbuiltinTickpushTick10769833213962245646_5302216a93726f8a(__context__,das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_258_52),__value_rename_at_260_53);
                        if ( !expect_token_4c6c072b3f5fa12b(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48),das_arg<json::TokenAt>::pass(__ahead_rename_at_247_50),4,__error_rename_at_246_49) )
                        {
                            return das_auto_cast<json::JsonValue *>::cast(nullptr);
                        } else {
                            int32_t __sepsym_rename_at_272_54 = ((int32_t)das_get_auto_variant_field<int32_t,4,char *,double,bool,void *,int32_t,char *>::as(__ahead_rename_at_247_50.value,__context__));
                            if ( __sepsym_rename_at_272_54 == 93 )
                            {
                                break;
                            } else if ( __sepsym_rename_at_272_54 != 44 )
                            {
                                das_copy(__error_rename_at_246_49,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_7, cast<char * const >::from(((char *) "unsepected array seaprator symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sepsym_rename_at_272_54,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sepsym_rename_at_272_54), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_247_50.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_247_50.row))));
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            };
                        };
                    };
                    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48)) )
                    {
                        das_copy(__error_rename_at_246_49,((char *) "unexpected eos"));
                        return das_auto_cast<json::JsonValue *>::cast(nullptr);
                    } else {
                        return das_auto_cast<json::JsonValue *>::cast(JV_eb50e491960f713b(__context__,das_arg<TArray<json::JsonValue *>>::pass(__arr_rename_at_258_52)));
                    };
                } else if ( __sym_rename_at_252_51 == 123 )
                {
                    TTable<char *,json::JsonValue *> __tab_rename_at_286_55;das_zero(__tab_rename_at_286_55);
                    while ( !builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48)) )
                    {
                        if ( !expect_token_4c6c072b3f5fa12b(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48),das_arg<json::TokenAt>::pass(__ahead_rename_at_247_50),0,__error_rename_at_246_49) )
                        {
                            if ( (das_get_auto_variant_field<int32_t,4,char *,double,bool,void *,int32_t,char *>::is(__ahead_rename_at_247_50.value) && (das_get_auto_variant_field<int32_t,4,char *,double,bool,void *,int32_t,char *>::as(__ahead_rename_at_247_50.value,__context__) == 125)) && (builtin_table_size(das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_286_55)) == 0) )
                            {
                                das_copy(__error_rename_at_246_49,nullptr);
                                return das_auto_cast<json::JsonValue *>::cast(JV_eda1cfc37ee5473f(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_286_55)));
                            } else {
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            };
                        };
                        char * __key_rename_at_295_56 = ((char *)(char *)(das_get_auto_variant_field<char *,0,char *,double,bool,void *,int32_t,char *>::as(__ahead_rename_at_247_50.value,__context__)));
                        if ( !expect_symbol_21380f29090e81b6(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48),das_arg<json::TokenAt>::pass(__ahead_rename_at_247_50),58,__error_rename_at_246_49) )
                        {
                            return das_auto_cast<json::JsonValue *>::cast(nullptr);
                        } else {
                            json::JsonValue * __value_rename_at_299_57 = parse_value_1716bf8ce4d4c65a(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48),__error_rename_at_246_49);
                            if ( __value_rename_at_299_57 == nullptr )
                            {
                                return das_auto_cast<json::JsonValue *>::cast(nullptr);
                            } else {
                                if ( !das_global<bool,0x30e401a8d3210d31>(__context__) /*allow_duplicate_keys*/ && _FuncbuiltinTickkey_existsTick16808803843923989214_f90c5a9ce3ca9524(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_286_55),__key_rename_at_295_56) )
                                {
                                    das_copy(__error_rename_at_246_49,das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_8, cast<char * const >::from(((char *) "duplicate key ")), cast<char * const >::from(__key_rename_at_295_56), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_247_50.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_247_50.row))));
                                    return das_auto_cast<json::JsonValue *>::cast(nullptr);
                                } else {
                                    _FuncbuiltinTickinsertTick4246857231018487965_30a7dd4f934af59c(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_286_55),__key_rename_at_295_56,__value_rename_at_299_57);
                                    if ( !expect_token_4c6c072b3f5fa12b(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48),das_arg<json::TokenAt>::pass(__ahead_rename_at_247_50),4,__error_rename_at_246_49) )
                                    {
                                        return das_auto_cast<json::JsonValue *>::cast(nullptr);
                                    } else {
                                        int32_t __sepsym_rename_at_311_58 = ((int32_t)das_get_auto_variant_field<int32_t,4,char *,double,bool,void *,int32_t,char *>::as(__ahead_rename_at_247_50.value,__context__));
                                        if ( __sepsym_rename_at_311_58 == 125 )
                                        {
                                            break;
                                        } else if ( __sepsym_rename_at_311_58 != 44 )
                                        {
                                            das_copy(__error_rename_at_246_49,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_9, cast<char * const >::from(((char *) "unsepected object seaprator symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sepsym_rename_at_311_58,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sepsym_rename_at_311_58), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_247_50.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_247_50.row))));
                                            return das_auto_cast<json::JsonValue *>::cast(nullptr);
                                        };
                                    };
                                };
                            };
                        };
                    };
                    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__itv_rename_at_246_48)) )
                    {
                        das_copy(__error_rename_at_246_49,((char *) "unexpected eos"));
                        return das_auto_cast<json::JsonValue *>::cast(nullptr);
                    } else {
                        return das_auto_cast<json::JsonValue *>::cast(JV_eda1cfc37ee5473f(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__tab_rename_at_286_55)));
                    };
                } else {
                    das_copy(__error_rename_at_246_49,das_string_builder(__context__,SimNode_AotInterop<8>(__tinfo_10, cast<char * const >::from(((char *) "unexpected symbol `")), cast<char * const >::from(((char * const )(to_string_char(__sym_rename_at_252_51,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "` aka ASCII ")), cast<int32_t>::from(__sym_rename_at_252_51), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_247_50.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_247_50.row))));
                    return das_auto_cast<json::JsonValue *>::cast(nullptr);
                };
            };
        } else if ( das_get_auto_variant_field<char *,0,char *,double,bool,void *,int32_t,char *>::is(__ahead_rename_at_247_50.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_35aac71ff80df937(__context__,das_get_auto_variant_field<char *,0,char *,double,bool,void *,int32_t,char *>::as(__ahead_rename_at_247_50.value,__context__)));
        } else if ( das_get_auto_variant_field<double,1,char *,double,bool,void *,int32_t,char *>::is(__ahead_rename_at_247_50.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_411a9817ab566332(__context__,das_get_auto_variant_field<double,1,char *,double,bool,void *,int32_t,char *>::as(__ahead_rename_at_247_50.value,__context__)));
        } else if ( das_get_auto_variant_field<bool,2,char *,double,bool,void *,int32_t,char *>::is(__ahead_rename_at_247_50.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JV_b2f8c005ec090a85(__context__,das_get_auto_variant_field<bool,2,char *,double,bool,void *,int32_t,char *>::as(__ahead_rename_at_247_50.value,__context__)));
        } else if ( das_get_auto_variant_field<void *,3,char *,double,bool,void *,int32_t,char *>::is(__ahead_rename_at_247_50.value) )
        {
            return das_auto_cast<json::JsonValue *>::cast(JVNull_7972f25b86b1842d(__context__));
        } else {
            das_copy(__error_rename_at_246_49,das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_11, cast<AutoVariant<char *,double,bool,void *,int32_t,char *> &>::from(__ahead_rename_at_247_50.value), cast<char * const >::from(((char *) " at ")), cast<int32_t>::from(__ahead_rename_at_247_50.line), cast<char * const >::from(((char *) ":")), cast<int32_t>::from(__ahead_rename_at_247_50.row))));
            return das_auto_cast<json::JsonValue *>::cast(nullptr);
        };
    };
}

inline json::JsonValue * read_json_809f579b141dd044 ( Context * __context__, char * const  __text_rename_at_342_59, char * & __error_rename_at_342_60 )
{
    das_copy(__error_rename_at_342_60,nullptr);
    Sequence DAS_COMMENT((json::TokenAt)) __lex_rename_at_346_61; das_zero(__lex_rename_at_346_61); das_move(__lex_rename_at_346_61, lexer_e4c23188cefd4e94(__context__,__text_rename_at_342_59));
    json::JsonValue * __res_rename_at_347_62 = parse_value_1716bf8ce4d4c65a(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_346_61),__error_rename_at_342_60);
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_346_61),__context__);
    return das_auto_cast<json::JsonValue *>::cast(__res_rename_at_347_62);
}

inline json::JsonValue * read_json_4ba85080204587fb ( Context * __context__, TArray<uint8_t> const  &  __text_rename_at_352_63, char * & __error_rename_at_352_64 )
{
    das_copy(__error_rename_at_352_64,nullptr);
    Sequence DAS_COMMENT((json::TokenAt)) __lex_rename_at_354_65; das_zero(__lex_rename_at_354_65); das_move(__lex_rename_at_354_65, lexer_6e4965f12d35e1fb(__context__,__text_rename_at_352_63));
    json::JsonValue * __res_rename_at_355_66 = parse_value_1716bf8ce4d4c65a(__context__,das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_354_65),__error_rename_at_352_64);
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((json::TokenAt))>::pass(__lex_rename_at_354_65),__context__);
    return das_auto_cast<json::JsonValue *>::cast(__res_rename_at_355_66);
}

inline bool set_no_trailing_zeros_995e7aa57f8a54c3 ( Context * __context__, bool __value_rename_at_362_67 )
{
    bool __old_ntz_rename_at_364_68 = ((bool)das_global<bool,0xd3b8b85b9bf19183>(__context__) /*no_trailing_zeros*/);
    das_copy(das_global<bool,0xd3b8b85b9bf19183>(__context__) /*no_trailing_zeros*/,__value_rename_at_362_67);
    return das_auto_cast<bool>::cast(__old_ntz_rename_at_364_68);
}

inline bool set_no_empty_arrays_42235cc25e5283a4 ( Context * __context__, bool __value_rename_at_371_69 )
{
    bool __old_nea_rename_at_373_70 = ((bool)das_global<bool,0xab7f08dbdeea5c03>(__context__) /*no_empty_arrays*/);
    das_copy(das_global<bool,0xab7f08dbdeea5c03>(__context__) /*no_empty_arrays*/,__value_rename_at_371_69);
    return das_auto_cast<bool>::cast(__old_nea_rename_at_373_70);
}

inline bool set_allow_duplicate_keys_b0d075b8cdc0fb30 ( Context * __context__, bool __value_rename_at_380_71 )
{
    bool __old_adc_rename_at_382_72 = ((bool)das_global<bool,0x30e401a8d3210d31>(__context__) /*allow_duplicate_keys*/);
    das_copy(das_global<bool,0x30e401a8d3210d31>(__context__) /*allow_duplicate_keys*/,__value_rename_at_380_71);
    return das_auto_cast<bool>::cast(__old_adc_rename_at_382_72);
}

inline void write_value_b3cf9236674b49df ( Context * __context__, StringBuilderWriter &  __writer_rename_at_388_73, json::JsonValue * const  __jsv_rename_at_388_74, int32_t __depth_rename_at_388_75 )
{
    if ( __jsv_rename_at_388_74 == nullptr )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "null"))));
    } else if ( das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__jsv_rename_at_388_74->value) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_13,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "\""))));
        write_escape_string(das_arg<StringBuilderWriter>::pass(__writer_rename_at_388_73),das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jsv_rename_at_388_74->value,__context__));
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_14,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "\""))));
    } else if ( das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__jsv_rename_at_388_74->value) )
    {
        if ( das_global<bool,0xd3b8b85b9bf19183>(__context__) /*no_trailing_zeros*/ )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_15,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char * const )(builtin_string_rtrim_ts(((char * const )(builtin_string_rtrim_ts(((char * const )(fmt_d(((char *) ":.17f"),das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jsv_rename_at_388_74->value,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "0"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "."),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_16,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<double>::from(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jsv_rename_at_388_74->value,__context__))));
        };
    } else if ( das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__jsv_rename_at_388_74->value) )
    {
        if ( builtin_array_size(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jsv_rename_at_388_74->value,__context__)) == 0 )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "[]"))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "[\n"))));
            bool __first_rename_at_406_76 = true;
            {
                bool __need_loop_407 = true;
                // elem: json::JsonValue? const&
                das_iterator<TArray<json::JsonValue *> const > __elem_iterator(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jsv_rename_at_388_74->value,__context__));
                json::JsonValue * const  * __elem_rename_at_407_77;
                __need_loop_407 = __elem_iterator.first(__context__,(__elem_rename_at_407_77)) && __need_loop_407;
                for ( ; __need_loop_407 ; __need_loop_407 = __elem_iterator.next(__context__,(__elem_rename_at_407_77)) )
                {
                    if ( __first_rename_at_406_76 )
                    {
                        das_copy(__first_rename_at_406_76,false);
                    } else {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_19,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) ",\n"))));
                    };
                    write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_388_73),9,__depth_rename_at_388_75 + 1);
                    write_value_b3cf9236674b49df(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_388_73),(*__elem_rename_at_407_77),__depth_rename_at_388_75 + 1);
                }
                __elem_iterator.close(__context__,(__elem_rename_at_407_77));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_20,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "\n"))));
            write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_388_73),9,__depth_rename_at_388_75);
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_21,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "]"))));
        };
    } else if ( das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__jsv_rename_at_388_74->value) )
    {
        if ( builtin_table_size(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jsv_rename_at_388_74->value,__context__)) == 0 )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_22,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "{}"))));
        } else {
            Sequence DAS_COMMENT((char *)) _temp_make_local_426_33_0; _temp_make_local_426_33_0;
            Sequence DAS_COMMENT((json::JsonValue * *)) _temp_make_local_426_61_1; _temp_make_local_426_61_1;
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_23,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "{\n"))));
            bool __first_rename_at_425_78 = true;
            {
                bool __need_loop_426 = true;
                // elemK: string
                das_iterator<Sequence DAS_COMMENT((char *))> __elemK_iterator((_temp_make_local_426_33_0 = (_FuncbuiltinTickkeysTick2205854368403803976_5d08049ff7d797e2(__context__,das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jsv_rename_at_388_74->value,__context__)))));
                char * __elemK_rename_at_426_81;
                __need_loop_426 = __elemK_iterator.first(__context__,(__elemK_rename_at_426_81)) && __need_loop_426;
                // elemV: json::JsonValue? const&
                das_iterator<Sequence DAS_COMMENT((json::JsonValue * const ))> __elemV_iterator((_temp_make_local_426_61_1 = (_FuncbuiltinTickvaluesTick1935193042646774172_e8953d3b3cbab6d5(__context__,das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jsv_rename_at_388_74->value,__context__)))));
                json::JsonValue * const  * __elemV_rename_at_426_82;
                __need_loop_426 = __elemV_iterator.first(__context__,(__elemV_rename_at_426_82)) && __need_loop_426;
                for ( ; __need_loop_426 ; __need_loop_426 = __elemK_iterator.next(__context__,(__elemK_rename_at_426_81)) && __elemV_iterator.next(__context__,(__elemV_rename_at_426_82)) )
                {
                    if ( das_global<bool,0xab7f08dbdeea5c03>(__context__) /*no_empty_arrays*/ )
                    {
                        if ( das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is((*__elemV_rename_at_426_82)->value) )
                        {
                            if ( builtin_array_size(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__elemV_rename_at_426_82)->value,__context__)) == 0 )
                            {
                                continue;
                            };
                        };
                    };
                    if ( __first_rename_at_425_78 )
                    {
                        das_copy(__first_rename_at_425_78,false);
                    } else {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_24,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) ",\n"))));
                    };
                    write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_388_73),9,__depth_rename_at_388_75 + 1);
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_25,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "\""))));
                    write_escape_string(das_arg<StringBuilderWriter>::pass(__writer_rename_at_388_73),__elemK_rename_at_426_81);
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_26,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "\" : "))));
                    write_value_b3cf9236674b49df(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_388_73),(*__elemV_rename_at_426_82),__depth_rename_at_388_75 + 1);
                }
                __elemK_iterator.close(__context__,(__elemK_rename_at_426_81));
                __elemV_iterator.close(__context__,(__elemV_rename_at_426_82));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_27,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "\n"))));
            write_string_chars(das_arg<StringBuilderWriter>::pass(__writer_rename_at_388_73),9,__depth_rename_at_388_75);
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_28,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "}"))));
        };
    } else if ( das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__jsv_rename_at_388_74->value) )
    {
        if ( das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jsv_rename_at_388_74->value,__context__) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_29,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "true"))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_30,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "false"))));
        };
    } else if ( das_get_auto_variant_field<void *,5,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__jsv_rename_at_388_74->value) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_31,cast<StringBuilderWriter &>::from(__writer_rename_at_388_73),cast<char * const >::from(((char *) "null"))));
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_32, cast<char * const >::from(((char *) "unexpected ")), cast<json::JsonValue * const >::from(__jsv_rename_at_388_74))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline char * write_json_b2bd5d3e5bfc13b7 ( Context * __context__, json::JsonValue * const  __val_rename_at_462_83 )
{
    char * __st_rename_at_464_84 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_464_85) DAS_AOT_INLINE_LAMBDA -> void{
        write_value_b3cf9236674b49df(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_464_85),__val_rename_at_462_83,0);
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_464_84);
}

inline char * write_json_746b512bcec82883 ( Context * __context__, json::JsonValue * const  __val_rename_at_470_86 )
{
    return das_auto_cast<char *>::cast(write_json_b2bd5d3e5bfc13b7(__context__,das_cast<json::JsonValue *>::cast(__val_rename_at_470_86)));
}

inline char * try_fixing_broken_json_1c995197dfb2261e ( Context * __context__, char * __bad_rename_at_477_87 ) { das_stack_prologue __prologue(__context__,272,"try_fixing_broken_json " DAS_FILE_LINE);
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_483_88) DAS_AOT_INLINE_LAMBDA -> void{
        builtin_string_peek(__bad_rename_at_477_87,das_make_block<void,TArray<uint8_t> const  &>(__context__,144,0,&__func_info__5a16898e20f97124,[&](TArray<uint8_t> const  & __str_rename_at_484_89) -> void{
            int32_t __i_rename_at_485_90 = 0;
            int32_t __lstr_rename_at_486_91 = ((int32_t)builtin_array_size(__str_rename_at_484_89));
            while ( __i_rename_at_485_90 < __lstr_rename_at_486_91 )
            {
                while ( (__i_rename_at_485_90 < __lstr_rename_at_486_91) && (das_nequ_val(__str_rename_at_484_89(__i_rename_at_485_90,__context__),0x22)) )
                {
                    if ( das_equ_val(__str_rename_at_484_89(__i_rename_at_485_90,__context__),0x2c) )
                    {
                        int32_t __j_rename_at_491_92 = (__i_rename_at_485_90 + 1);
                        while ( (__j_rename_at_491_92 < __lstr_rename_at_486_91) && is_white_space(int32_t(__str_rename_at_484_89(__j_rename_at_491_92,__context__))) )
                        {
                            ++__j_rename_at_491_92;
                        };
                        if ( (__j_rename_at_491_92 < __lstr_rename_at_486_91) && ((das_equ_val(__str_rename_at_484_89(__j_rename_at_491_92,__context__),0x7d)) || (das_equ_val(__str_rename_at_484_89(__j_rename_at_491_92,__context__),0x5d))) )
                        {
                            das_copy(__i_rename_at_485_90,__j_rename_at_491_92);
                            continue;
                        };
                    };
                    if ( (das_equ_val(__str_rename_at_484_89(__i_rename_at_485_90,__context__),0x5c)) && (das_equ_val(__str_rename_at_484_89((__i_rename_at_485_90 + 1),__context__),0x75)) )
                    {
                        __i_rename_at_485_90 += 2;
                        while ( (__i_rename_at_485_90 < __lstr_rename_at_486_91) && is_hex(int32_t(__str_rename_at_484_89(__i_rename_at_485_90,__context__))) )
                        {
                            ++__i_rename_at_485_90;
                        };
                        continue;
                    } else {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),int32_t(__str_rename_at_484_89(__i_rename_at_485_90,__context__)));
                        ++__i_rename_at_485_90;
                    };
                };
                if ( __i_rename_at_485_90 >= __lstr_rename_at_486_91 )
                {
                    break;
                } else {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),34);
                    ++__i_rename_at_485_90;
                    label_0:;;
                    while ( (__i_rename_at_485_90 < __lstr_rename_at_486_91) && (das_nequ_val(__str_rename_at_484_89(__i_rename_at_485_90,__context__),0x22)) )
                    {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),int32_t(__str_rename_at_484_89(__i_rename_at_485_90,__context__)));
                        ++__i_rename_at_485_90;
                    };
                    if ( __i_rename_at_485_90 >= __lstr_rename_at_486_91 )
                    {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),34);
                        break;
                    } else {
                        int32_t __nested_rename_at_527_93 = ((int32_t)__i_rename_at_485_90);
                        int32_t __j_rename_at_528_94 = (__i_rename_at_485_90 + 1);
                        bool __any_white_space_rename_at_529_95 = ((bool)((__j_rename_at_528_94 < __lstr_rename_at_486_91) && is_white_space(int32_t(__str_rename_at_484_89(__j_rename_at_528_94,__context__)))));
                        while ( (__j_rename_at_528_94 < __lstr_rename_at_486_91) && is_white_space(int32_t(__str_rename_at_484_89(__j_rename_at_528_94,__context__))) )
                        {
                            ++__j_rename_at_528_94;
                        };
                        if ( __j_rename_at_528_94 >= __lstr_rename_at_486_91 )
                        {
                            write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),34);
                            break;
                        } else {
                            int32_t __strj_rename_at_538_96 = ((int32_t)int32_t(__str_rename_at_484_89(__j_rename_at_528_94,__context__)));
                            if ( (((__strj_rename_at_538_96 == 58) || (__strj_rename_at_538_96 == 125)) || (__strj_rename_at_538_96 == 93)) || (__strj_rename_at_538_96 == 44) )
                            {
                                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),34);
                                ++__i_rename_at_485_90;
                                continue;
                            } else {
                                if ( __strj_rename_at_538_96 == 43 )
                                {
                                    ++__j_rename_at_528_94;
                                    while ( (__j_rename_at_528_94 < __lstr_rename_at_486_91) && is_white_space(int32_t(__str_rename_at_484_89(__j_rename_at_528_94,__context__))) )
                                    {
                                        ++__j_rename_at_528_94;
                                    };
                                    if ( (__j_rename_at_528_94 < __lstr_rename_at_486_91) && (das_equ_val(__str_rename_at_484_89(__j_rename_at_528_94,__context__),0x22)) )
                                    {
                                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),32);
                                        das_copy(__i_rename_at_485_90,__j_rename_at_528_94 + 1);
                                        goto label_0;
                                    };
                                };
                                if ( __any_white_space_rename_at_529_95 )
                                {
                                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),34);
                                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),44);
                                    das_copy(__i_rename_at_485_90,__nested_rename_at_527_93 + 1);
                                    continue;
                                } else {
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_33,cast<StringBuilderWriter &>::from(__writer_rename_at_483_88),cast<char * const >::from(((char *) "`"))));
                                    das_copy(__i_rename_at_485_90,__nested_rename_at_527_93 + 1);
                                    while ( (__i_rename_at_485_90 < __lstr_rename_at_486_91) && (das_nequ_val(__str_rename_at_484_89(__i_rename_at_485_90,__context__),0x22)) )
                                    {
                                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),int32_t(__str_rename_at_484_89(__i_rename_at_485_90,__context__)));
                                        ++__i_rename_at_485_90;
                                    };
                                    if ( __i_rename_at_485_90 >= __lstr_rename_at_486_91 )
                                    {
                                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_34,cast<StringBuilderWriter &>::from(__writer_rename_at_483_88),cast<char * const >::from(((char *) "`"))));
                                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_483_88),34);
                                        break;
                                    } else {
                                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_35,cast<StringBuilderWriter &>::from(__writer_rename_at_483_88),cast<char * const >::from(((char *) "`"))));
                                        ++__i_rename_at_485_90;
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

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x26c7f8c57e7f7a6d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_118143448cef53b4>>();
    };
    aotLib[0xfe771255bb39b0b8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_ad8689b2c3216ff3>>();
    };
    aotLib[0x6d758929cf31f7ca] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick9663565701927713696_f82670fc7e714dfa>>();
    };
    aotLib[0x324ca317a460a2a8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_80eac3dd8d64249e>>();
    };
    aotLib[0x9e6a074f1b0fa837] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick4044332007967089362_b9a088e792341ef2>>();
    };
    aotLib[0x6bed80b93c56eefd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_814de7e4feafd66>>();
    };
    aotLib[0x7bdd70b160a91d25] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_json_99_1Tickfunction_fbd838db6f25aa6d>>();
    };
    aotLib[0xf7f2b00e1e89bc3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_json_99_1Tickfinalizer_dd59807cf54b4451>>();
    };
    aotLib[0x3f10103878e37dab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_e247a8d469c511cc>>();
    };
    aotLib[0x53889e35b37fc61a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_5302216a93726f8a>>();
    };
    aotLib[0x77122d9d459d9323] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_f90c5a9ce3ca9524>>();
    };
    aotLib[0x128495d538062a51] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick4246857231018487965_30a7dd4f934af59c>>();
    };
    aotLib[0x6888ea818b62be19] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_5d08049ff7d797e2>>();
    };
    aotLib[0xd456a433aac4be3b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1935193042646774172_e8953d3b3cbab6d5>>();
    };
    aotLib[0x4ab5cf4614dd3d42] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_35aac71ff80df937>>();
    };
    aotLib[0x26377e9476c5d68b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_411a9817ab566332>>();
    };
    aotLib[0xe6b34a44300f3114] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_b2f8c005ec090a85>>();
    };
    aotLib[0x84cd7c6b07007f09] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JVNull_7972f25b86b1842d>>();
    };
    aotLib[0xdeb30945dba15108] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_eda1cfc37ee5473f>>();
    };
    aotLib[0x639adaab4d9d7e0c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_eb50e491960f713b>>();
    };
    aotLib[0xddcfc667c6dd8bf9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&lexer_e4c23188cefd4e94>>();
    };
    aotLib[0x14c4e7d2df54b00c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&lexer_6e4965f12d35e1fb>>();
    };
    aotLib[0x8918292b0a0e495c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&next_971a4a5b86ed11b3>>();
    };
    aotLib[0xac1ed3c4316ebdc0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_lexer_a97174d736e96fde>>();
    };
    aotLib[0x4307a4e8f7f50e5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&expect_token_4c6c072b3f5fa12b>>();
    };
    aotLib[0xf2cddf87fc77d789] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&expect_symbol_21380f29090e81b6>>();
    };
    aotLib[0x6fa10fe68a0fa4fb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&parse_value_1716bf8ce4d4c65a>>();
    };
    aotLib[0xf117a36357510521] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&read_json_809f579b141dd044>>();
    };
    aotLib[0xe39e872036eb82db] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&read_json_4ba85080204587fb>>();
    };
    aotLib[0x3da22c91945d16a1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_no_trailing_zeros_995e7aa57f8a54c3>>();
    };
    aotLib[0x53a91a2978434bad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_no_empty_arrays_42235cc25e5283a4>>();
    };
    aotLib[0x89537de20c04935b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_allow_duplicate_keys_b0d075b8cdc0fb30>>();
    };
    aotLib[0x23803fe1576e4e0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&write_value_b3cf9236674b49df>>();
    };
    aotLib[0xa6e663b2cb19054e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&write_json_b2bd5d3e5bfc13b7>>();
    };
    aotLib[0x2c3394acec8586fb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&write_json_746b512bcec82883>>();
    };
    aotLib[0x63fb760b882a900d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&try_fixing_broken_json_1c995197dfb2261e>>();
    };
    // [[ init script ]]
    aotLib[0x10179ab31ca2e18b] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
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
