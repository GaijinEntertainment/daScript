#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require ast_boost
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
namespace _anon_2307684379922914619 {

extern TypeInfo __type_info__6b1c7db4b71a781f;
extern TypeInfo __type_info__af63a74c8601927d;
extern TypeInfo __type_info__3c61146b2bdfb90;
extern TypeInfo __type_info__4fd1011fade7876f;
extern TypeInfo __type_info__fa593d0882a72913;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__8d8d8008262e16ec;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af96fe4c8658cf52;
extern TypeInfo __type_info__b68d800849332aec;
extern TypeInfo __type_info__432249b20bc1758d;
extern TypeInfo __type_info__a57bf935c2dd03;
extern TypeInfo __type_info__ce241e3005cc873b;
extern TypeInfo __type_info__8afce1a80940fc9e;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63d94c8601e773;
extern TypeInfo __type_info__af63db4c8601ead9;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__d922fe078cefab30;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63e84c860200f0;
extern TypeInfo __type_info__d9307e078cfb0f0c;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__1b2f537dda20b669;
extern VarInfo __var_info__4cf955bf86f60534;
extern VarInfo __var_info__ec89c0a8da510f28;
extern VarInfo __var_info__ec83c0a8da46dd28;
extern VarInfo __var_info__c9a6e7cc498a17c9;
extern VarInfo __var_info__2a896b18ac8c5dc;
extern VarInfo __var_info__c3630b856e2c8315;
extern VarInfo __var_info__ceb2c4cde2c49d3a;
extern VarInfo __var_info__447579d184dfd9f7;
extern VarInfo __var_info__8603fbcef5fdd683;
extern VarInfo __var_info__5ac39c9b48fed41d;
extern VarInfo __var_info__5671a754112737a5;
extern FuncInfo __func_info__f381b060f4dbce7;
extern FuncInfo __func_info__3b2c4edb933bef4a;
extern FuncInfo __func_info__806bd08fbfbd09f4;
extern FuncInfo __func_info__8075d08fbfce07f4;
extern FuncInfo __func_info__4dedd3fc50b439aa;
extern FuncInfo __func_info__c13357a1bc201428;
extern FuncInfo __func_info__b50c88059cb1bcc;
extern FuncInfo __func_info__b28b606ddb7a6760;
extern EnumInfo __enum_info__c897fe55afe7f727;

EnumValueInfo __enum_info__c897fe55afe7f727_value_0 = { "none", 0 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_1 = { "autoinfer", 1 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_2 = { "alias", 2 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_3 = { "option", 3 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_4 = { "typeDecl", 4 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_5 = { "typeMacro", 5 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_6 = { "fakeContext", 6 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_7 = { "fakeLineInfo", 7 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_8 = { "anyArgument", 8 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_9 = { "tVoid", 9 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_10 = { "tBool", 10 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_11 = { "tInt8", 11 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_12 = { "tUInt8", 12 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_13 = { "tInt16", 13 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_14 = { "tUInt16", 14 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_15 = { "tInt64", 15 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_16 = { "tUInt64", 16 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_17 = { "tInt", 17 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_18 = { "tInt2", 18 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_19 = { "tInt3", 19 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_20 = { "tInt4", 20 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_21 = { "tUInt", 21 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_22 = { "tUInt2", 22 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_23 = { "tUInt3", 23 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_24 = { "tUInt4", 24 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_25 = { "tFloat", 25 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_26 = { "tFloat2", 26 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_27 = { "tFloat3", 27 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_28 = { "tFloat4", 28 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_29 = { "tDouble", 29 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_30 = { "tRange", 30 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_31 = { "tURange", 31 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_32 = { "tRange64", 32 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_33 = { "tURange64", 33 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_34 = { "tString", 34 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_35 = { "tStructure", 35 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_36 = { "tHandle", 36 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_37 = { "tEnumeration", 37 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_38 = { "tEnumeration8", 38 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_39 = { "tEnumeration16", 39 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_40 = { "tEnumeration64", 40 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_41 = { "tBitfield", 41 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_42 = { "tPointer", 42 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_43 = { "tFunction", 43 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_44 = { "tLambda", 44 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_45 = { "tIterator", 45 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_46 = { "tArray", 46 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_47 = { "tTable", 47 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_48 = { "tBlock", 48 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_49 = { "tTuple", 49 };
EnumValueInfo __enum_info__c897fe55afe7f727_value_50 = { "tVariant", 50 };
EnumValueInfo * __enum_info__c897fe55afe7f727_values [] = { &__enum_info__c897fe55afe7f727_value_0, &__enum_info__c897fe55afe7f727_value_1, &__enum_info__c897fe55afe7f727_value_2, &__enum_info__c897fe55afe7f727_value_3, &__enum_info__c897fe55afe7f727_value_4, &__enum_info__c897fe55afe7f727_value_5, &__enum_info__c897fe55afe7f727_value_6, &__enum_info__c897fe55afe7f727_value_7, &__enum_info__c897fe55afe7f727_value_8, &__enum_info__c897fe55afe7f727_value_9, &__enum_info__c897fe55afe7f727_value_10, &__enum_info__c897fe55afe7f727_value_11, &__enum_info__c897fe55afe7f727_value_12, &__enum_info__c897fe55afe7f727_value_13, &__enum_info__c897fe55afe7f727_value_14, &__enum_info__c897fe55afe7f727_value_15, &__enum_info__c897fe55afe7f727_value_16, &__enum_info__c897fe55afe7f727_value_17, &__enum_info__c897fe55afe7f727_value_18, &__enum_info__c897fe55afe7f727_value_19, &__enum_info__c897fe55afe7f727_value_20, &__enum_info__c897fe55afe7f727_value_21, &__enum_info__c897fe55afe7f727_value_22, &__enum_info__c897fe55afe7f727_value_23, &__enum_info__c897fe55afe7f727_value_24, &__enum_info__c897fe55afe7f727_value_25, &__enum_info__c897fe55afe7f727_value_26, &__enum_info__c897fe55afe7f727_value_27, &__enum_info__c897fe55afe7f727_value_28, &__enum_info__c897fe55afe7f727_value_29, &__enum_info__c897fe55afe7f727_value_30, &__enum_info__c897fe55afe7f727_value_31, &__enum_info__c897fe55afe7f727_value_32, &__enum_info__c897fe55afe7f727_value_33, &__enum_info__c897fe55afe7f727_value_34, &__enum_info__c897fe55afe7f727_value_35, &__enum_info__c897fe55afe7f727_value_36, &__enum_info__c897fe55afe7f727_value_37, &__enum_info__c897fe55afe7f727_value_38, &__enum_info__c897fe55afe7f727_value_39, &__enum_info__c897fe55afe7f727_value_40, &__enum_info__c897fe55afe7f727_value_41, &__enum_info__c897fe55afe7f727_value_42, &__enum_info__c897fe55afe7f727_value_43, &__enum_info__c897fe55afe7f727_value_44, &__enum_info__c897fe55afe7f727_value_45, &__enum_info__c897fe55afe7f727_value_46, &__enum_info__c897fe55afe7f727_value_47, &__enum_info__c897fe55afe7f727_value_48, &__enum_info__c897fe55afe7f727_value_49, &__enum_info__c897fe55afe7f727_value_50 };
EnumInfo __enum_info__c897fe55afe7f727 = { "Type", "rtti", __enum_info__c897fe55afe7f727_values, 51, UINT64_C(0xc897fe55afe7f727) };
VarInfo __func_info__f381b060f4dbce7_field_0 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Annotation"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8226, 88, UINT64_C(0x5ac39c9b48fed41d), "value", 0, 0 };
VarInfo * __func_info__f381b060f4dbce7_fields[1] =  { &__func_info__f381b060f4dbce7_field_0 };
FuncInfo __func_info__f381b060f4dbce7 = {"invoke block<(value:rtti::Annotation const):void> const", "", __func_info__f381b060f4dbce7_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xf381b060f4dbce7), 0x0 };
VarInfo __func_info__3b2c4edb933bef4a_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0x4cf955bf86f60534), "fn", 0, 0 };
VarInfo * __func_info__3b2c4edb933bef4a_fields[1] =  { &__func_info__3b2c4edb933bef4a_field_0 };
FuncInfo __func_info__3b2c4edb933bef4a = {"invoke block<(var fn:smart_ptr<ast::Function>):void> const", "", __func_info__3b2c4edb933bef4a_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x3b2c4edb933bef4a), 0x0 };
VarInfo __func_info__806bd08fbfbd09f4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0xec89c0a8da510f28), "func", 0, 0 };
VarInfo * __func_info__806bd08fbfbd09f4_fields[1] =  { &__func_info__806bd08fbfbd09f4_field_0 };
FuncInfo __func_info__806bd08fbfbd09f4 = {"invoke block<(var func:smart_ptr<ast::Function>):void> const", "", __func_info__806bd08fbfbd09f4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x806bd08fbfbd09f4), 0x0 };
VarInfo __func_info__8075d08fbfce07f4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0xec83c0a8da46dd28), "func", 0, 0 };
VarInfo * __func_info__8075d08fbfce07f4_fields[1] =  { &__func_info__8075d08fbfce07f4_field_0 };
FuncInfo __func_info__8075d08fbfce07f4 = {"invoke block<(var func:smart_ptr<ast::Function>):void> const", "", __func_info__8075d08fbfce07f4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8075d08fbfce07f4), 0x0 };
VarInfo __func_info__4dedd3fc50b439aa_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8afce1a80940fc9e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xc9a6e7cc498a17c9), "mod", 0, 0 };
VarInfo * __func_info__4dedd3fc50b439aa_fields[1] =  { &__func_info__4dedd3fc50b439aa_field_0 };
FuncInfo __func_info__4dedd3fc50b439aa = {"invoke block<(var mod:rtti::Module?):void> const", "", __func_info__4dedd3fc50b439aa_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x4dedd3fc50b439aa), 0x0 };
VarInfo __func_info__c13357a1bc201428_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x2a896b18ac8c5dc), "name", 0, 0 };
VarInfo __func_info__c13357a1bc201428_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x1b2f537dda20b669), "cppName", 0, 0 };
VarInfo __func_info__c13357a1bc201428_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ce241e3005cc873b, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0x5671a754112737a5), "xtype", 0, 0 };
VarInfo __func_info__c13357a1bc201428_field_3 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xc3630b856e2c8315), "offset", 0, 0 };
VarInfo * __func_info__c13357a1bc201428_fields[4] =  { &__func_info__c13357a1bc201428_field_0, &__func_info__c13357a1bc201428_field_1, &__func_info__c13357a1bc201428_field_2, &__func_info__c13357a1bc201428_field_3 };
FuncInfo __func_info__c13357a1bc201428 = {"invoke block<(var name:string;var cppName:string;var xtype:smart_ptr<ast::TypeDecl>;var offset:uint):void> const", "", __func_info__c13357a1bc201428_fields, 4, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xc13357a1bc201428), 0x0 };
VarInfo __func_info__b50c88059cb1bcc_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xceb2c4cde2c49d3a), "pkey", 0, 0 };
VarInfo __func_info__b50c88059cb1bcc_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x447579d184dfd9f7), "pvalue", 0, 0 };
VarInfo * __func_info__b50c88059cb1bcc_fields[2] =  { &__func_info__b50c88059cb1bcc_field_0, &__func_info__b50c88059cb1bcc_field_1 };
FuncInfo __func_info__b50c88059cb1bcc = {"invoke block<(var pkey:void?;var pvalue:void?):void> const", "", __func_info__b50c88059cb1bcc_fields, 2, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xb50c88059cb1bcc), 0x0 };
VarInfo __func_info__b28b606ddb7a6760_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x8603fbcef5fdd683), "value", 0, 0 };
VarInfo * __func_info__b28b606ddb7a6760_fields[1] =  { &__func_info__b28b606ddb7a6760_field_0 };
FuncInfo __func_info__b28b606ddb7a6760 = {"invoke block<(var value:void?):void> const", "", __func_info__b28b606ddb7a6760_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xb28b606ddb7a6760), 0x0 };
TypeInfo __type_info__6b1c7db4b71a781f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 258, 32, UINT64_C(0x6b1c7db4b71a781f) };
TypeInfo __type_info__af63a74c8601927d = { Type::anyArgument, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 16, UINT64_C(0xaf63a74c8601927d) };
TypeInfo __type_info__3c61146b2bdfb90 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 290, 32, UINT64_C(0x3c61146b2bdfb90) };
TypeInfo __type_info__4fd1011fade7876f = { Type::tEnumeration, nullptr, &__enum_info__c897fe55afe7f727, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0x4fd1011fade7876f) };
TypeInfo __type_info__fa593d0882a72913 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16548, 8, UINT64_C(0xfa593d0882a72913) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af96fe4c8658cf52 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf96fe4c8658cf52) };
TypeInfo __type_info__b68d800849332aec = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0xb68d800849332aec) };
TypeInfo * __type_info__432249b20bc1758d_arg_types[9] = { &__type_info__af63df4c8601f1a5, &__type_info__af63e44c8601fa24, &__type_info__af63e84c860200f0, &__type_info__d922fe078cefab30, &__type_info__d9307e078cfb0f0c, &__type_info__af63db4c8601ead9, &__type_info__af63d94c8601e773, &__type_info__af63ee4c86020b22, &__type_info__af63a74c8601927d };
const char * __type_info__432249b20bc1758d_arg_names[9] = { "tBool", "tInt", "tUInt", "tInt64", "tUInt64", "tFloat", "tDouble", "tString", "nothing" };
TypeInfo __type_info__432249b20bc1758d = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__432249b20bc1758d_arg_types, __type_info__432249b20bc1758d_arg_names, 9, 0, nullptr, 16390, 32, UINT64_C(0x432249b20bc1758d) };
TypeInfo __type_info__a57bf935c2dd03 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Function"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 408, UINT64_C(0xa57bf935c2dd03) };
TypeInfo __type_info__ce241e3005cc873b = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::TypeDecl"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 232, UINT64_C(0xce241e3005cc873b) };
TypeInfo __type_info__8afce1a80940fc9e = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Module"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 1272, UINT64_C(0x8afce1a80940fc9e) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63d94c8601e773 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xaf63d94c8601e773) };
TypeInfo __type_info__af63db4c8601ead9 = { Type::tFloat, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63db4c8601ead9) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__d922fe078cefab30 = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xd922fe078cefab30) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63e84c860200f0 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e84c860200f0) };
TypeInfo __type_info__d9307e078cfb0f0c = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xd9307e078cfb0f0c) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__6b1c7db4b71a781f, __type_info__3c61146b2bdfb90, __type_info__a57bf935c2dd03, __type_info__ce241e3005cc873b, __type_info__8afce1a80940fc9e, __type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[2] = { &__type_info__37d36026a6078a42, &__type_info__fa593d0882a72913 };
TypeInfo * __tinfo_2[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_3[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__432249b20bc1758d };
TypeInfo * __tinfo_4[4] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_5[1] = { &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_6[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_7[2] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_8[1] = { &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_9[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[2] = { &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f };
TypeInfo * __tinfo_11[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_12[2] = { &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f };
TypeInfo * __tinfo_13[2] = { &__type_info__af90fe4c864e9d52, &__type_info__4fd1011fade7876f };
TypeInfo * __tinfo_14[4] = { &__type_info__af90fe4c864e9d52, &__type_info__d922fe078cefab30, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_15[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_16[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_17[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_18[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_19[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_20[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_22[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_23[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_24[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_25[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_26[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_27[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_28[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_29[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_30[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_31[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_32[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_33[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_34[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec };
TypeInfo * __tinfo_35[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_36[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__b68d800849332aec };
TypeInfo * __tinfo_37[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_38[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af81fe4c86352052 };
TypeInfo * __tinfo_39[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_40[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_41[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_42[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_43[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_44[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_45[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_46[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_47[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_48[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_49[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_50[3] = { &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_51[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_52[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_53[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_54[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
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
// unused enumeration CompilationError
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
// unused enumeration RefMatters
// unused enumeration ConstMatters
// unused enumeration TemporaryMatters
// unused enumeration ConversionResult
// unused enumeration SideEffects
// unused enumeration CaptureMode
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
static_assert(sizeof(AstFunctionAnnotation)==112,"structure size mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(AstFunctionAnnotation,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
// unused structure AstBlockAnnotation
namespace ast {

struct AstStructureAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
};
static_assert(sizeof(AstStructureAnnotation)==72,"structure size mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(AstStructureAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
}
// unused structure AstPassMacro
namespace ast {

struct AstVariantMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
static_assert(sizeof(AstVariantMacro)==40,"structure size mismatch with DAS");
static_assert(offsetof(AstVariantMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprIsVariant)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprAsVariant)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprSafeAsVariant)==32,"structure field offset mismatch with DAS");
}
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
namespace ast_boost {

struct MacroMacro {
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
static_assert(sizeof(MacroMacro)==112,"structure size mismatch with DAS");
static_assert(offsetof(MacroMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(MacroMacro,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct TagFunctionAnnotation {
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
static_assert(sizeof(TagFunctionAnnotation)==112,"structure size mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,transform)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,verifyCall)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,apply)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,generic_apply)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,finish)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,patch)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,fixup)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,lint)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,complete)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,isCompatible)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,isSpecialized)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionAnnotation,appendToMangledName)==104,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct TagStructureAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
};
static_assert(sizeof(TagStructureAnnotation)==72,"structure size mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(TagStructureAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupAnyAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupAnyAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupAnyAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupFunctionAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupFunctionAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupFunctionAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupBlockAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupBlockAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupBlockAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupStructureAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupStructureAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupStructureAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupEnumerationAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupEnumerationAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupEnumerationAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupContractAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupContractAnnotation)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupContractAnnotation,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupReaderMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupReaderMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupReaderMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupCommentReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupCommentReader)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupCommentReader,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCommentReader,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupVariantMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupVariantMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupVariantMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupForLoopMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupForLoopMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupForLoopMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupCaptureMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupCaptureMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCaptureMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupTypeMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupTypeMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupSimulateMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupSimulateMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupSimulateMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupCallMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupCallMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupCallMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupCallMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupTypeInfoMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupTypeInfoMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupTypeInfoMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupInferMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupInferMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupInferMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupInferMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupDirtyInferMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupDirtyInferMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupDirtyInferMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupLintMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupLintMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupLintMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupLintMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupGlobalLintMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupGlobalLintMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupGlobalLintMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct SetupOptimizationMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
};
static_assert(sizeof(SetupOptimizationMacro)==96,"structure size mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(SetupOptimizationMacro,setup_call)==88,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct TagFunctionMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation)) __finalize;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,ModuleGroup,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotPrefix;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotBody;
    Func DAS_COMMENT((void,ast::AstStructureAnnotation,smart_ptr_raw<Structure>,AnnotationArgumentList const ,StringBuilderWriter)) aotSuffix;
    char * annotation_function_call;
    char * name;
    Func DAS_COMMENT((void,ast_boost::SetupAnyAnnotation,smart_ptr_raw<Structure>,smart_ptr_raw<ExprCall>)) setup_call;
    char * tag;
};
static_assert(sizeof(TagFunctionMacro)==104,"structure size mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,apply)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,finish)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,patch)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,complete)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,aotPrefix)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,aotBody)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,aotSuffix)==64,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,annotation_function_call)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,name)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,setup_call)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(TagFunctionMacro,tag)==96,"structure field offset mismatch with DAS");
}
namespace ast_boost {

struct BetterRttiVisitor {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
static_assert(sizeof(BetterRttiVisitor)==40,"structure size mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,visitExprIsVariant)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,visitExprAsVariant)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterRttiVisitor,visitExprSafeAsVariant)==32,"structure field offset mismatch with DAS");
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d38c0a8fb8b57a95 ( Context * __context__, ast_boost::MacroMacro const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a59c8e1dab321df7 ( Context * __context__, ast_boost::TagFunctionAnnotation const  & __cl_rename_at_116_3 );
inline void _FuncbuiltinTickpushTick10769833213962245646_c677163a1eb520b6 ( Context * __context__, TArray<ast::AstStructureAnnotation *> & __Arr_rename_at_181_4, ast::AstStructureAnnotation * __value_rename_at_181_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_fd822f8ec049c56f ( Context * __context__, ast_boost::SetupFunctionAnnotation const  & __cl_rename_at_116_6 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b1daca88b52c6678 ( Context * __context__, ast_boost::SetupBlockAnnotation const  & __cl_rename_at_116_7 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_34de85a7749a610b ( Context * __context__, ast_boost::SetupStructureAnnotation const  & __cl_rename_at_116_8 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f8a0e6267b2bd3d9 ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  & __cl_rename_at_116_9 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_30f11df7ec48d590 ( Context * __context__, ast_boost::SetupContractAnnotation const  & __cl_rename_at_116_10 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cbd2268f16dd6f7 ( Context * __context__, ast_boost::SetupReaderMacro const  & __cl_rename_at_116_11 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_90f0b6ebf218b95 ( Context * __context__, ast_boost::SetupCommentReader const  & __cl_rename_at_116_12 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4f4d07b67d5c637a ( Context * __context__, ast_boost::SetupCallMacro const  & __cl_rename_at_116_13 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_58a689e6c5544c13 ( Context * __context__, ast_boost::SetupTypeInfoMacro const  & __cl_rename_at_116_14 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9e90922306ddd25a ( Context * __context__, ast_boost::SetupVariantMacro const  & __cl_rename_at_116_15 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d17686a9f7a586f4 ( Context * __context__, ast_boost::SetupForLoopMacro const  & __cl_rename_at_116_16 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_97d75e289ad3a300 ( Context * __context__, ast_boost::SetupCaptureMacro const  & __cl_rename_at_116_17 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5fb85726896385b1 ( Context * __context__, ast_boost::SetupTypeMacro const  & __cl_rename_at_116_18 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2d9c9af4d9e7b493 ( Context * __context__, ast_boost::SetupSimulateMacro const  & __cl_rename_at_116_19 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6d99e55f093884d6 ( Context * __context__, ast_boost::TagStructureAnnotation const  & __cl_rename_at_116_20 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cf4f8fcd2b1e128a ( Context * __context__, ast_boost::TagFunctionMacro const  & __cl_rename_at_116_21 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bc379015cee45784 ( Context * __context__, ast_boost::SetupInferMacro const  & __cl_rename_at_116_22 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_62e05e00785596c6 ( Context * __context__, ast_boost::SetupDirtyInferMacro const  & __cl_rename_at_116_23 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bebaf7033e24000c ( Context * __context__, ast_boost::SetupOptimizationMacro const  & __cl_rename_at_116_24 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_dd66b37998de4f39 ( Context * __context__, ast_boost::SetupLintMacro const  & __cl_rename_at_116_25 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_103841aa9a022d76 ( Context * __context__, ast_boost::SetupGlobalLintMacro const  & __cl_rename_at_116_26 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5fc7cdb5157ef6b5 ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_181_27, ast::AstVariantMacro * __value_rename_at_181_28 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c881cfb5d9ae9ed5 ( Context * __context__, ast_boost::BetterRttiVisitor const  & __cl_rename_at_116_29 );
inline void clone_a23f2772d6a2dd68 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & __dest_rename_at_1060_30, smart_ptr_raw<FunctionAnnotation> const  __src_rename_at_1060_31 );
inline smart_ptr_raw<FunctionAnnotation> _FuncbuiltinTickclone_to_moveTick2007252383599261567_ddf94d0785b39e5b ( Context * __context__, smart_ptr_raw<FunctionAnnotation> const  __clone_src_rename_at_1057_32 );
inline Module * _FuncastTickfind_moduleTick11101329256228773934_52d9ec34594ea355 ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_939_34, char * const  __name_rename_at_939_35 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> & __a_rename_at_1183_38 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9 ( Context * __context__, TArray<smart_ptr_raw<TypeDecl>> & __a_rename_at_1183_39 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c6f9bc68b25c5dd6 ( Context * __context__, TArray<smart_ptr_raw<Variable>> & __a_rename_at_1183_41 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __a_rename_at_1183_43 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1 ( Context * __context__, TArray<char *> & __Arr_rename_at_181_45, char * __value_rename_at_181_46 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835 ( Context * __context__, TArray<char *> const  & __it_rename_at_22_47, char * const  __separator_rename_at_22_48 );
inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> & __Arr_rename_at_165_53, char * const  __value_rename_at_165_54 );
inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_a44b26c5d2257edc ( Context * __context__, TArray<smart_ptr_raw<Function>> & __Arr_rename_at_377_55, smart_ptr_raw<Function> __value_rename_at_377_56 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_aba46312bb49c9f6 ( Context * __context__, TArray<smart_ptr_raw<Function>> & __a_rename_at_1183_57 );
inline void clone_8c54ff7e0079253a ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_283_59, smart_ptr<Function> const  __src_rename_at_283_60 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_37_61, bool __extra_rename_at_37_62, bool __contracts_rename_at_37_63, bool __modules_rename_at_37_64 );
inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1727_65 );
inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_3746f4c2db327a87 ( Context * __context__ );
inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_8c07d4f696b9c5ba ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1733_67 );
inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_bd8120dfe7363955 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1733_68 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_d7d3fcc28de34db6 ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_515_69, smart_ptr_raw<Expression> __enull_rename_at_515_70 );
inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_e95e98afff367ec7 ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_515_88, smart_ptr_raw<Expression> __enull_rename_at_515_89 );
inline void clone_f1f421db208b5182 ( Context * __context__, smart_ptr<TypeDecl> & __dest_rename_at_604_107, smart_ptr_raw<TypeDecl> const  __src_rename_at_604_108 );
inline void clone_9ab0f6a4e0a6006e ( Context * __context__, smart_ptr<Enumeration> & __dest_rename_at_748_109, Enumeration * const  __src_rename_at_748_110 );
inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_34f86625d1f8ecb2 ( Context * __context__, char * const  __name_rename_at_958_111 );
inline void clone_3a3075a3a0c0cb06 ( Context * __context__, smart_ptr<Annotation> & __dest_rename_at_827_112, smart_ptr_raw<Annotation> const  __src_rename_at_827_113 );
inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_47_114 );
inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_403d4a8bdcd4ca52 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1727_115 );
inline void clone_ec5e41b1830fac59 ( Context * __context__, das::vector<das::string> & __args_rename_at_16_116, TArray<char *> & __nargs_rename_at_16_117 );
inline void clone_36538fa38bdd6e0b ( Context * __context__, das::vector<smart_ptr<TypeDecl>> & __args_rename_at_25_120, TArray<smart_ptr_raw<TypeDecl>> & __nargs_rename_at_25_121 );
inline void clone_82cb7bdbc1cd751b ( Context * __context__, das::vector<smart_ptr<Variable>> & __args_rename_at_33_123, TArray<smart_ptr_raw<Variable>> & __nargs_rename_at_33_124 );
inline void clone_ab5928efc1a1d61 ( Context * __context__, das::vector<smart_ptr<Expression>> & __args_rename_at_41_126, TArray<smart_ptr_raw<Expression>> & __nargs_rename_at_41_127 );
inline bool isVectorType_e16b21c2cb0df3cf ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_49_129 );
inline char * describe_f8955cd58aaa956c ( Context * __context__, AnnotationArgumentList const  & __list_rename_at_59_130 );
inline char * describe_16a184ebde2f5850 ( Context * __context__, AnnotationDeclaration const  & __ann_rename_at_63_133 );
inline char * describe_5703109ec79a52b4 ( Context * __context__, AnnotationList const  & __list_rename_at_71_134 );
inline char * describe_9b40ad62f87c706d ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_75_137 );
inline bool isExpression_a7c31959e6c730e8 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_96_138, bool __top_rename_at_96_139 );
inline bool is_same_or_inherited_8fc466ebe75e2336 ( Context * __context__, Structure const  * const  __parent_rename_at_114_140, Structure const  * const  __child_rename_at_114_141 );
inline bool is_class_method_67e7c960daf5e6e2 ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_125_143, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_125_144 );
inline void emplace_new_eadb1a1435b53033 ( Context * __context__, das::vector<smart_ptr<Expression>> & __vec_rename_at_147_145, smart_ptr_raw<Expression> __ptr_rename_at_147_146 );
inline void emplace_new_52cce80de15d9fe4 ( Context * __context__, das::vector<smart_ptr<TypeDecl>> & __vec_rename_at_151_147, smart_ptr_raw<TypeDecl> __ptr_rename_at_151_148 );
inline void emplace_new_e46333e37cb8f35d ( Context * __context__, das::vector<smart_ptr<Variable>> & __vec_rename_at_155_149, smart_ptr_raw<Variable> __ptr_rename_at_155_150 );
inline void emplace_new_2dcb96a028e6e2a ( Context * __context__, MakeStruct & __vec_rename_at_159_151, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_159_152 );
inline bool override_method_38d6ecad3789a50f ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_163_153, char * const  __name_rename_at_163_154, char * const  __funcName_rename_at_163_155 );
inline void for_each_tag_function_bfda4041b94932d7 ( Context * __context__, Module * const  __mod_rename_at_216_159, char * const  __tag_rename_at_216_160, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __blk_rename_at_216_161 );
inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_bb44f3ca6ce3fc6d ( Context * __context__, char * const  __argn_rename_at_242_168, AnnotationArgumentList const  & __args_rename_at_242_169 );
inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_a148d1ff83124bd1 ( Context * __context__, AnnotationArgumentList const  & __args_rename_at_251_171, char * const  __argn_rename_at_251_172 );
inline void apply_tag_annotation_f773cf67c31220 ( Context * __context__, char * const  __tag_rename_at_260_174, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_260_175 );
inline smart_ptr_raw<Function> find_unique_function_aab3bc60151e96ec ( Context * __context__, Module * const  __mod_rename_at_279_180, char * const  __name_rename_at_279_181, bool __canfail_rename_at_279_182 );
inline smart_ptr_raw<Function> find_unique_generic_b6f2e7df17214306 ( Context * __context__, Module * const  __mod_rename_at_295_186, char * const  __name_rename_at_295_187, bool __canfail_rename_at_295_188 );
inline ExprBlock * setup_call_list_ad0757bf84a727d8 ( Context * __context__, char * const  __name_rename_at_311_192, LineInfo const  & __at_rename_at_311_193, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  & __subblock_rename_at_311_194 );
inline ExprBlock * setup_call_list_f7df6b96f49a9cf8 ( Context * __context__, char * const  __name_rename_at_332_199, LineInfo const  & __at_rename_at_332_200, bool __isInit_rename_at_332_201, bool __isPrivate_rename_at_332_202, bool __isLateInit_rename_at_332_203 );
inline ExprBlock * setup_macro_1ed401bbb9e11b66 ( Context * __context__, char * const  __name_rename_at_346_205, LineInfo const  & __at_rename_at_346_206 );
inline void * panic_expr_as_b448efb56dfa5ea ( Context * __context__ );
inline smart_ptr_raw<Expression> walk_and_convert_array_fc0c75b20fd9b5d8 ( Context * __context__, uint8_t const  * const  __data_rename_at_582_219, smart_ptr_raw<TypeDecl> const  __info_rename_at_582_220, LineInfo const  & __at_rename_at_582_221 );
inline smart_ptr_raw<Expression> walk_and_convert_dim_307e83e2e898f055 ( Context * __context__, uint8_t const  * const  __data_rename_at_599_228, smart_ptr_raw<TypeDecl> const  __info_rename_at_599_229, LineInfo const  & __at_rename_at_599_230 );
inline smart_ptr_raw<Expression> walk_and_convert_pointer_99fa96f2fad2fabb ( Context * __context__, uint8_t const  * const  __data_rename_at_614_238, smart_ptr_raw<TypeDecl> const  __info_rename_at_614_239, LineInfo const  & __at_rename_at_614_240 );
inline smart_ptr_raw<Expression> walk_and_convert_tuple_d283511dc79b27c5 ( Context * __context__, uint8_t const  * const  __data_rename_at_623_244, smart_ptr_raw<TypeDecl> const  __info_rename_at_623_245, LineInfo const  & __at_rename_at_623_246 );
inline smart_ptr_raw<Expression> walk_and_convert_structure_461cae90387df9d8 ( Context * __context__, uint8_t const  * const  __data_rename_at_635_251, smart_ptr_raw<TypeDecl> const  __info_rename_at_635_252, LineInfo const  & __at_rename_at_635_253 );
inline smart_ptr_raw<Expression> walk_and_convert_variant_50ce6cc7d88f2c68 ( Context * __context__, uint8_t const  * const  __data_rename_at_656_262, smart_ptr_raw<TypeDecl> const  __info_rename_at_656_263, LineInfo const  & __at_rename_at_656_264 );
inline smart_ptr_raw<Expression> walk_and_convert_table_578c212c6a418c09 ( Context * __context__, uint8_t const  * const  __data_rename_at_671_271, smart_ptr_raw<TypeDecl> const  __info_rename_at_671_272, LineInfo const  & __at_rename_at_671_273 );
inline smart_ptr_raw<Expression> walk_and_convert_basic_abd632a9adbc5f64 ( Context * __context__, uint8_t const  * const  __data_rename_at_697_286, smart_ptr_raw<TypeDecl> const  __info_rename_at_697_287, LineInfo const  & __at_rename_at_697_288 );
inline smart_ptr_raw<Expression> walk_and_convert_enumeration_19e84f10944d8e34 ( Context * __context__, uint8_t const  * const  __data_rename_at_732_290, smart_ptr_raw<TypeDecl> const  __info_rename_at_732_291, LineInfo const  & __at_rename_at_732_292 );
inline smart_ptr_raw<Expression> walk_and_convert_52e7fa0c06656c3c ( Context * __context__, uint8_t const  * const  __data_rename_at_752_296, smart_ptr_raw<TypeDecl> const  __info_rename_at_752_297, LineInfo const  & __at_rename_at_752_298 );
inline Annotation const  * find_annotation_3783ce606e338529 ( Context * __context__, char * const  __mod_name_rename_at_807_299, char * const  __ann_name_rename_at_807_300 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_518bf407af95eedd ( Context * __context__, char * const  __mod_name_rename_at_823_304, char * const  __ann_name_rename_at_823_305, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_823_306 );
inline smart_ptr_raw<AnnotationDeclaration> append_annotation_304ed986ed5830b8 ( Context * __context__, char * const  __mod_name_rename_at_846_312, char * const  __ann_name_rename_at_846_313 );
inline void append_annotation_a089579603ac9094 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_854_316, char * const  __mod_name_rename_at_854_317, char * const  __ann_name_rename_at_854_318 );
inline void append_annotation_df67cdd1c7fda936 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_861_321, char * const  __mod_name_rename_at_861_322, char * const  __ann_name_rename_at_861_323 );
inline void append_annotation_33c7fba9141ce1dc ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_868_326, char * const  __mod_name_rename_at_868_327, char * const  __ann_name_rename_at_868_328 );
inline void append_annotation_50d585b969d10a85 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_875_331, char * const  __mod_name_rename_at_875_332, char * const  __ann_name_rename_at_875_333, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_875_334 );
inline void append_annotation_4809cc819e2c2e25 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_882_337, char * const  __mod_name_rename_at_882_338, char * const  __ann_name_rename_at_882_339, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_882_340 );
inline void append_annotation_c82c20ac1475e6cb ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_889_343, char * const  __mod_name_rename_at_889_344, char * const  __ann_name_rename_at_889_345, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __args_rename_at_889_346 );
inline int32_t add_annotation_argument_aecf50176a737617 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_896_349, char * const  __argName_rename_at_896_350, bool __val_rename_at_896_351 );
inline int32_t add_annotation_argument_584d51af09b7a892 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_903_353, char * const  __argName_rename_at_903_354, int32_t __val_rename_at_903_355 );
inline int32_t add_annotation_argument_2991fd3097c426a8 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_910_357, char * const  __argName_rename_at_910_358, float __val_rename_at_910_359 );
inline int32_t add_annotation_argument_62c7cc55608504df ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_917_361, char * const  __argName_rename_at_917_362, char * const  __val_rename_at_917_363 );
inline int32_t add_annotation_argument_806b0c29e45217e2 ( Context * __context__, AnnotationArgumentList & __arguments_rename_at_924_365, AnnotationArgument const  & __ann_rename_at_924_366 );
inline int32_t get_for_source_index_9bcd632a50af36c1 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_941_368, smart_ptr_raw<Variable> const  __svar_rename_at_941_369 );
inline int32_t get_for_source_index_2b39b717f35b48e5 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_950_374, smart_ptr_raw<Expression> const  __source_rename_at_950_375 );
inline smart_ptr_raw<TypeDecl> function_to_type_84839d9adeaf04f3 ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_998_380 );
inline void visit_finally_27770f49825d1a9b ( Context * __context__, ExprBlock * const  __blk_rename_at_1010_386, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1010_387 );
inline bool isCMRES_ecc22ba10553fb74 ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1016_388 );
inline bool isCMRES_c750d4ae5480733b ( Context * __context__, Function * const  __fun_rename_at_1020_389 );
inline bool isMakeLocal_18c81e2ae52a95c7 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1024_390 );
inline TDim<DAS_COMMENT(bound_enum) das::Type,31> get_workhorse_types_e61ceb837ac4d4c ( Context * __context__ );
inline int32_t find_argument_index_ddbc5417768753b7 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1045_391, char * const  __name_rename_at_1045_392 );
inline bool isCMRESType_be28547f5538e0c5 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1054_397 );
inline void debug_expression_impl_27c6891d2e6065f6 ( Context * __context__, StringBuilderWriter & __writer_rename_at_1062_398, smart_ptr_raw<Expression> const  __expr_rename_at_1062_399, Bitfield __deFlags_rename_at_1062_400, int32_t __tabs_rename_at_1062_401 );
inline char * debug_expression_dc45cab15254a45e ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1168_435, Bitfield __deFlags_rename_at_1168_436 );
inline char * debug_expression_b1e90a58d18306b3 ( Context * __context__, Expression * const  __expr_rename_at_1174_438 );
inline char * describe_9ffb758bbbac7334 ( Context * __context__, Expression * const  __expr_rename_at_1180_439 );
inline int32_t getVectorElementCount_1cefda208a7a5281 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1186_440 );
inline int32_t getVectorElementSize_e8d53271262fdc3c ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1193_441 );
inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_696a4aae05024c01 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1198_442 );
inline int32_t getVectorOffset_b6d06fc98795c1fe ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1207_443, char * const  __ident_rename_at_1207_444 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d38c0a8fb8b57a95 ( Context * __context__, ast_boost::MacroMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_a59c8e1dab321df7 ( Context * __context__, ast_boost::TagFunctionAnnotation const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_c677163a1eb520b6 ( Context * __context__, TArray<ast::AstStructureAnnotation *> &  __Arr_rename_at_181_4, ast::AstStructureAnnotation * __value_rename_at_181_5 )
{
    das_copy(__Arr_rename_at_181_4(builtin_array_push_back(das_arg<TArray<ast::AstStructureAnnotation *>>::pass(__Arr_rename_at_181_4),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_5);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_fd822f8ec049c56f ( Context * __context__, ast_boost::SetupFunctionAnnotation const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b1daca88b52c6678 ( Context * __context__, ast_boost::SetupBlockAnnotation const  &  __cl_rename_at_116_7 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_7.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_34de85a7749a610b ( Context * __context__, ast_boost::SetupStructureAnnotation const  &  __cl_rename_at_116_8 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_8.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f8a0e6267b2bd3d9 ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  &  __cl_rename_at_116_9 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_9.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_30f11df7ec48d590 ( Context * __context__, ast_boost::SetupContractAnnotation const  &  __cl_rename_at_116_10 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_10.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cbd2268f16dd6f7 ( Context * __context__, ast_boost::SetupReaderMacro const  &  __cl_rename_at_116_11 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_11.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_90f0b6ebf218b95 ( Context * __context__, ast_boost::SetupCommentReader const  &  __cl_rename_at_116_12 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_12.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4f4d07b67d5c637a ( Context * __context__, ast_boost::SetupCallMacro const  &  __cl_rename_at_116_13 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_13.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_58a689e6c5544c13 ( Context * __context__, ast_boost::SetupTypeInfoMacro const  &  __cl_rename_at_116_14 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_14.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9e90922306ddd25a ( Context * __context__, ast_boost::SetupVariantMacro const  &  __cl_rename_at_116_15 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_15.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d17686a9f7a586f4 ( Context * __context__, ast_boost::SetupForLoopMacro const  &  __cl_rename_at_116_16 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_16.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_97d75e289ad3a300 ( Context * __context__, ast_boost::SetupCaptureMacro const  &  __cl_rename_at_116_17 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_17.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5fb85726896385b1 ( Context * __context__, ast_boost::SetupTypeMacro const  &  __cl_rename_at_116_18 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_18.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2d9c9af4d9e7b493 ( Context * __context__, ast_boost::SetupSimulateMacro const  &  __cl_rename_at_116_19 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_19.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_6d99e55f093884d6 ( Context * __context__, ast_boost::TagStructureAnnotation const  &  __cl_rename_at_116_20 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_20.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cf4f8fcd2b1e128a ( Context * __context__, ast_boost::TagFunctionMacro const  &  __cl_rename_at_116_21 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_21.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bc379015cee45784 ( Context * __context__, ast_boost::SetupInferMacro const  &  __cl_rename_at_116_22 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_22.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_62e05e00785596c6 ( Context * __context__, ast_boost::SetupDirtyInferMacro const  &  __cl_rename_at_116_23 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_23.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_bebaf7033e24000c ( Context * __context__, ast_boost::SetupOptimizationMacro const  &  __cl_rename_at_116_24 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_24.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_dd66b37998de4f39 ( Context * __context__, ast_boost::SetupLintMacro const  &  __cl_rename_at_116_25 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_25.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_103841aa9a022d76 ( Context * __context__, ast_boost::SetupGlobalLintMacro const  &  __cl_rename_at_116_26 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_26.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5fc7cdb5157ef6b5 ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_181_27, ast::AstVariantMacro * __value_rename_at_181_28 )
{
    das_copy(__Arr_rename_at_181_27(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_181_27),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_28);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c881cfb5d9ae9ed5 ( Context * __context__, ast_boost::BetterRttiVisitor const  &  __cl_rename_at_116_29 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_29.__rtti))).getStructType())));
}

inline void clone_a23f2772d6a2dd68 ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & __dest_rename_at_1060_30, smart_ptr_raw<FunctionAnnotation> const  __src_rename_at_1060_31 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1060_30),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1060_31),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncbuiltinTickclone_to_moveTick2007252383599261567_ddf94d0785b39e5b ( Context * __context__, smart_ptr_raw<FunctionAnnotation> const  __clone_src_rename_at_1057_32 )
{
    smart_ptr_raw<FunctionAnnotation> __clone_dest_rename_at_1059_33; das_zero(__clone_dest_rename_at_1059_33);
    clone_a23f2772d6a2dd68(__context__,__clone_dest_rename_at_1059_33,__clone_src_rename_at_1057_32);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(__clone_dest_rename_at_1059_33);
}

inline Module * _FuncastTickfind_moduleTick11101329256228773934_52d9ec34594ea355 ( Context * __context__, smart_ptr_raw<Program> const  __prog_rename_at_939_34, char * const  __name_rename_at_939_35 ) { das_stack_prologue __prologue(__context__,112,"ast`find_module`11101329256228773934 " DAS_FILE_LINE);
{
    Module * __rm_rename_at_940_36 = 0;
    rtti_builtin_program_for_each_module(__prog_rename_at_939_34,das_make_block<void,Module *>(__context__,96,0,&__func_info__4dedd3fc50b439aa,[&](Module * __mod_rename_at_941_37) -> void{
        if ( SimPolicy<char *>::Equ(cast<char *>::from(((char * const )(to_das_string(das_arg<das::string>::pass(__mod_rename_at_941_37->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),cast<char *>::from(__name_rename_at_939_35),*__context__,nullptr) )
        {
            das_copy(__rm_rename_at_940_36,__mod_rename_at_941_37);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __rm_rename_at_940_36 == nullptr )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) "module '")), cast<char * const >::from(__name_rename_at_939_35), cast<char * const >::from(((char *) "' not found")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<Module *>::cast(__rm_rename_at_940_36);
}}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> &  __a_rename_at_1183_38 )
{
    builtin_array_free(das_arg<TArray<char *>>::pass(__a_rename_at_1183_38),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9 ( Context * __context__, TArray<smart_ptr_raw<TypeDecl>> &  __a_rename_at_1183_39 )
{
    {
        bool __need_loop_1185 = true;
        // aV: smart_ptr<ast::TypeDecl> aka TT&
        das_iterator<TArray<smart_ptr_raw<TypeDecl>>> __aV_iterator(__a_rename_at_1183_39);
        smart_ptr_raw<TypeDecl> * __aV_rename_at_1185_40;
        __need_loop_1185 = __aV_iterator.first(__context__,(__aV_rename_at_1185_40)) && __need_loop_1185;
        for ( ; __need_loop_1185 ; __need_loop_1185 = __aV_iterator.next(__context__,(__aV_rename_at_1185_40)) )
        {
            das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,(*__aV_rename_at_1185_40));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1185_40));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<TypeDecl>>>::pass(__a_rename_at_1183_39),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c6f9bc68b25c5dd6 ( Context * __context__, TArray<smart_ptr_raw<Variable>> &  __a_rename_at_1183_41 )
{
    {
        bool __need_loop_1185 = true;
        // aV: smart_ptr<ast::Variable> aka TT&
        das_iterator<TArray<smart_ptr_raw<Variable>>> __aV_iterator(__a_rename_at_1183_41);
        smart_ptr_raw<Variable> * __aV_rename_at_1185_42;
        __need_loop_1185 = __aV_iterator.first(__context__,(__aV_rename_at_1185_42)) && __need_loop_1185;
        for ( ; __need_loop_1185 ; __need_loop_1185 = __aV_iterator.next(__context__,(__aV_rename_at_1185_42)) )
        {
            das_delete_handle<smart_ptr_raw<Variable>>::clear(__context__,(*__aV_rename_at_1185_42));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1185_42));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Variable>>>::pass(__a_rename_at_1183_41),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __a_rename_at_1183_43 )
{
    {
        bool __need_loop_1185 = true;
        // aV: smart_ptr<ast::Expression> aka TT&
        das_iterator<TArray<smart_ptr_raw<Expression>>> __aV_iterator(__a_rename_at_1183_43);
        smart_ptr_raw<Expression> * __aV_rename_at_1185_44;
        __need_loop_1185 = __aV_iterator.first(__context__,(__aV_rename_at_1185_44)) && __need_loop_1185;
        for ( ; __need_loop_1185 ; __need_loop_1185 = __aV_iterator.next(__context__,(__aV_rename_at_1185_44)) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,(*__aV_rename_at_1185_44));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1185_44));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__a_rename_at_1183_43),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_181_45, char * __value_rename_at_181_46 )
{
    das_copy(__Arr_rename_at_181_45(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_181_45),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_46);
}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835 ( Context * __context__, TArray<char *> const  &  __it_rename_at_22_47, char * const  __separator_rename_at_22_48 )
{
    char * __st_rename_at_27_49 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_27_50) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_28_51 = true;
        {
            bool __need_loop_29 = true;
            // elem: string const&
            das_iterator<TArray<char *> const > __elem_iterator(__it_rename_at_22_47);
            char * const  * __elem_rename_at_29_52;
            __need_loop_29 = __elem_iterator.first(__context__,(__elem_rename_at_29_52)) && __need_loop_29;
            for ( ; __need_loop_29 ; __need_loop_29 = __elem_iterator.next(__context__,(__elem_rename_at_29_52)) )
            {
                if ( __skip_first_rename_at_28_51 )
                {
                    das_copy(__skip_first_rename_at_28_51,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__writer_rename_at_27_50),cast<char * const >::from(__separator_rename_at_22_48)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__writer_rename_at_27_50),cast<char * const >::from((*__elem_rename_at_29_52))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_29_52));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_27_49);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_53, char * const  __value_rename_at_165_54 )
{
    das_copy(__Arr_rename_at_165_53(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_53),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_54);
}

inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_a44b26c5d2257edc ( Context * __context__, TArray<smart_ptr_raw<Function>> &  __Arr_rename_at_377_55, smart_ptr_raw<Function> __value_rename_at_377_56 )
{
    clone_8c54ff7e0079253a(__context__,__Arr_rename_at_377_55(builtin_array_push_back_zero(das_arg<TArray<smart_ptr_raw<Function>>>::pass(__Arr_rename_at_377_55),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_377_56);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_aba46312bb49c9f6 ( Context * __context__, TArray<smart_ptr_raw<Function>> &  __a_rename_at_1183_57 )
{
    {
        bool __need_loop_1185 = true;
        // aV: smart_ptr<ast::Function> aka TT&
        das_iterator<TArray<smart_ptr_raw<Function>>> __aV_iterator(__a_rename_at_1183_57);
        smart_ptr_raw<Function> * __aV_rename_at_1185_58;
        __need_loop_1185 = __aV_iterator.first(__context__,(__aV_rename_at_1185_58)) && __need_loop_1185;
        for ( ; __need_loop_1185 ; __need_loop_1185 = __aV_iterator.next(__context__,(__aV_rename_at_1185_58)) )
        {
            das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,(*__aV_rename_at_1185_58));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1185_58));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Function>>>::pass(__a_rename_at_1183_57),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_8c54ff7e0079253a ( Context * __context__, smart_ptr_raw<Function> & __dest_rename_at_283_59, smart_ptr<Function> const  __src_rename_at_283_60 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_283_59),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_283_60),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_37_61, bool __extra_rename_at_37_62, bool __contracts_rename_at_37_63, bool __modules_rename_at_37_64 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_37_61,__extra_rename_at_37_62,__contracts_rename_at_37_63,__modules_rename_at_37_64,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1727_65 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1727_65));
}

inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _FuncrttiTickRttiValue_nothingTick4715542659269841615_3746f4c2db327a87 ( Context * __context__ )
{
    TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> __t_rename_at_164_66; das_zero(__t_rename_at_164_66);
    set_variant_index(das_arg<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::pass(das_alias<RttiValue>::from(__t_rename_at_164_66)),8);
    return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(das_alias<RttiValue>::from(__t_rename_at_164_66));
}

inline ExprBlock * _FuncbuiltinTickget_ptrTick5807679485210906136_8c07d4f696b9c5ba ( Context * __context__, smart_ptr_raw<ExprBlock> __src_rename_at_1733_67 )
{
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__src_rename_at_1733_67));
}

inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_bd8120dfe7363955 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1733_68 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1733_68));
}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_d7d3fcc28de34db6 ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  __expr_rename_at_515_69, smart_ptr_raw<Expression> __enull_rename_at_515_70 ) { das_stack_prologue __prologue(__context__,816,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_516_71; memset(&__vdr_rename_at_516_71,0,sizeof(__vdr_rename_at_516_71));
    smart_ptr_raw<ExprConstString> __cna_rename_at_517_73; memset(&__cna_rename_at_517_73,0,sizeof(__cna_rename_at_517_73));
    smart_ptr_raw<ExprOp2> __cond_rename_at_518_75; memset(&__cond_rename_at_518_75,0,sizeof(__cond_rename_at_518_75));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_519_77; memset(&__ctype_rename_at_519_77,0,sizeof(__ctype_rename_at_519_77));
    smart_ptr<Expression> __esub_rename_at_523_79; memset(&__esub_rename_at_523_79,0,sizeof(__esub_rename_at_523_79));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_525_80; memset(&__neqz_rename_at_525_80,0,sizeof(__neqz_rename_at_525_80));
    smart_ptr_raw<ExprOp2> __nand_rename_at_529_82; memset(&__nand_rename_at_529_82,0,sizeof(__nand_rename_at_529_82));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_535_84; memset(&__nsub_rename_at_535_84,0,sizeof(__nsub_rename_at_535_84));
    smart_ptr_raw<ExprCast> __vcast_rename_at_541_85; memset(&__vcast_rename_at_541_85,0,sizeof(__vcast_rename_at_541_85));
    smart_ptr_raw<ExprOp3> __tri_rename_at_547_86; memset(&__tri_rename_at_547_86,0,sizeof(__tri_rename_at_547_86));
    /* finally */ auto __finally_515= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_547_86);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_541_85);
    das_delete_handle<smart_ptr<Expression>>::clear(__context__,__esub_rename_at_523_79);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_519_77);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_518_75);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_517_73);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_516_71);
    /* end finally */ });
    __vdr_rename_at_516_71; das_zero(__vdr_rename_at_516_71); das_move(__vdr_rename_at_516_71, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_516) {
        das_copy((__mks_516.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_move((__mks_516.value /*value*/),(clone_expression(__expr_rename_at_515_69->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_516.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_517_73; das_zero(__cna_rename_at_517_73); das_move(__cna_rename_at_517_73, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_517) {
        das_copy((__mks_517.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_517.text /*value*/,__expr_rename_at_515_69->name /*name*/);
        }    }))));
    __cond_rename_at_518_75; das_zero(__cond_rename_at_518_75); das_move(__cond_rename_at_518_75, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_518) {
        das_copy((__mks_518.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_move((__mks_518.left /*left*/),(__vdr_rename_at_516_71));
        das_move((__mks_518.right /*right*/),(__cna_rename_at_517_73));
        {
            set_das_string(das_arg<das::string>::pass(__mks_518.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_519_77; das_zero(__ctype_rename_at_519_77); das_move(__ctype_rename_at_519_77, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_519) {
        das_copy((__mks_519.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_copy((__mks_519.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_519.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_521) {
            das_copy((__mks_521.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            das_copy((__mks_521.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_521.alias /*alias*/,__expr_rename_at_515_69->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_523_79; das_zero(__esub_rename_at_523_79); das_move(__esub_rename_at_523_79, clone_expression(__expr_rename_at_515_69->value /*value*/));
    if ( __expr_rename_at_515_69->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_524= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_529_82);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_525_80);
        /* end finally */ });
        __neqz_rename_at_525_80; das_zero(__neqz_rename_at_525_80); das_move(__neqz_rename_at_525_80, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_525) {
            das_copy((__mks_525.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            das_move((__mks_525.left /*left*/),(clone_expression(__expr_rename_at_515_69->value /*value*/)));
            das_move((__mks_525.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_527) {
                das_copy((__mks_527.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_525.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_529_82; das_zero(__nand_rename_at_529_82); das_move(__nand_rename_at_529_82, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_529) {
            das_copy((__mks_529.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            das_move((__mks_529.left /*left*/),(__neqz_rename_at_525_80));
            das_move((__mks_529.right /*right*/),(__cond_rename_at_518_75));
            {
                set_das_string(das_arg<das::string>::pass(__mks_529.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_518_75),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_529_82),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_534= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_535_84);
        /* end finally */ });
        __nsub_rename_at_535_84; das_zero(__nsub_rename_at_535_84); das_move(__nsub_rename_at_535_84, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_535) {
            das_copy((__mks_535.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
            das_move((__mks_535.subexpr /*subexpr*/),(__esub_rename_at_523_79));
            das_copy((__mks_535.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_523_79),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_535_84),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_541_85; das_zero(__vcast_rename_at_541_85); das_move(__vcast_rename_at_541_85, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_541) {
        das_copy((__mks_541.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_move((__mks_541.subexpr /*subexpr*/),(__esub_rename_at_523_79));
        das_move((__mks_541.castType /*castType*/),(__ctype_rename_at_519_77));
        das_copy((__mks_541.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_541.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_547_86; das_zero(__tri_rename_at_547_86); das_move(__tri_rename_at_547_86, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_547) {
        das_copy((__mks_547.at /*at*/),(__expr_rename_at_515_69->at /*at*/));
        das_move((__mks_547.subexpr /*subexpr*/),(__cond_rename_at_518_75));
        das_move((__mks_547.left /*left*/),(__vcast_rename_at_541_85));
        das_move((__mks_547.right /*right*/),(__enull_rename_at_515_70));
        {
            set_das_string(das_arg<das::string>::pass(__mks_547.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_547_86->genFlags /*genFlags*/,__expr_rename_at_515_69->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_547_86);
}}

inline smart_ptr_raw<ExprOp3> _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_e95e98afff367ec7 ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  __expr_rename_at_515_88, smart_ptr_raw<Expression> __enull_rename_at_515_89 ) { das_stack_prologue __prologue(__context__,816,"ast_boost`makeAsOrSafeAs`3414563941976524297 " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprField> __vdr_rename_at_516_90; memset(&__vdr_rename_at_516_90,0,sizeof(__vdr_rename_at_516_90));
    smart_ptr_raw<ExprConstString> __cna_rename_at_517_92; memset(&__cna_rename_at_517_92,0,sizeof(__cna_rename_at_517_92));
    smart_ptr_raw<ExprOp2> __cond_rename_at_518_94; memset(&__cond_rename_at_518_94,0,sizeof(__cond_rename_at_518_94));
    smart_ptr_raw<TypeDecl> __ctype_rename_at_519_96; memset(&__ctype_rename_at_519_96,0,sizeof(__ctype_rename_at_519_96));
    smart_ptr<Expression> __esub_rename_at_523_98; memset(&__esub_rename_at_523_98,0,sizeof(__esub_rename_at_523_98));
    smart_ptr_raw<ExprOp2> __neqz_rename_at_525_99; memset(&__neqz_rename_at_525_99,0,sizeof(__neqz_rename_at_525_99));
    smart_ptr_raw<ExprOp2> __nand_rename_at_529_101; memset(&__nand_rename_at_529_101,0,sizeof(__nand_rename_at_529_101));
    smart_ptr_raw<ExprRef2Ptr> __nsub_rename_at_535_103; memset(&__nsub_rename_at_535_103,0,sizeof(__nsub_rename_at_535_103));
    smart_ptr_raw<ExprCast> __vcast_rename_at_541_104; memset(&__vcast_rename_at_541_104,0,sizeof(__vcast_rename_at_541_104));
    smart_ptr_raw<ExprOp3> __tri_rename_at_547_105; memset(&__tri_rename_at_547_105,0,sizeof(__tri_rename_at_547_105));
    /* finally */ auto __finally_515= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprOp3>>::clear(__context__,__tri_rename_at_547_105);
    das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_541_104);
    das_delete_handle<smart_ptr<Expression>>::clear(__context__,__esub_rename_at_523_98);
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__ctype_rename_at_519_96);
    das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__cond_rename_at_518_94);
    das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__cna_rename_at_517_92);
    das_delete_handle<smart_ptr_raw<ExprField>>::clear(__context__,__vdr_rename_at_516_90);
    /* end finally */ });
    __vdr_rename_at_516_90; das_zero(__vdr_rename_at_516_90); das_move(__vdr_rename_at_516_90, das_ascend_handle<true,smart_ptr_raw<ExprField>>::make(__context__,nullptr,(([&](ExprField & __mks_516) {
        das_copy((__mks_516.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_move((__mks_516.value /*value*/),(clone_expression(__expr_rename_at_515_88->value /*value*/)));
        {
            set_das_string(das_arg<das::string>::pass(__mks_516.name /*name*/),((char *) "__rtti"));
        }    }))));
    __cna_rename_at_517_92; das_zero(__cna_rename_at_517_92); das_move(__cna_rename_at_517_92, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_517) {
        das_copy((__mks_517.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        {
            das_clone<das::string,das::string>::clone(__mks_517.text /*value*/,__expr_rename_at_515_88->name /*name*/);
        }    }))));
    __cond_rename_at_518_94; das_zero(__cond_rename_at_518_94); das_move(__cond_rename_at_518_94, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_518) {
        das_copy((__mks_518.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_move((__mks_518.left /*left*/),(__vdr_rename_at_516_90));
        das_move((__mks_518.right /*right*/),(__cna_rename_at_517_92));
        {
            set_das_string(das_arg<das::string>::pass(__mks_518.op /*op*/),((char *) "=="));
        }    }))));
    __ctype_rename_at_519_96; das_zero(__ctype_rename_at_519_96); das_move(__ctype_rename_at_519_96, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_519) {
        das_copy((__mks_519.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_copy((__mks_519.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tPointer));
        das_move((__mks_519.firstType /*firstType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_521) {
            das_copy((__mks_521.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            das_copy((__mks_521.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::alias));
            {
                das_clone<das::string,das::string>::clone(__mks_521.alias /*alias*/,__expr_rename_at_515_88->name /*name*/);
            }        })))));
    }))));
    __esub_rename_at_523_98; das_zero(__esub_rename_at_523_98); das_move(__esub_rename_at_523_98, clone_expression(__expr_rename_at_515_88->value /*value*/));
    if ( __expr_rename_at_515_88->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        /* finally */ auto __finally_524= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__nand_rename_at_529_101);
        das_delete_handle<smart_ptr_raw<ExprOp2>>::clear(__context__,__neqz_rename_at_525_99);
        /* end finally */ });
        __neqz_rename_at_525_99; das_zero(__neqz_rename_at_525_99); das_move(__neqz_rename_at_525_99, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_525) {
            das_copy((__mks_525.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            das_move((__mks_525.left /*left*/),(clone_expression(__expr_rename_at_515_88->value /*value*/)));
            das_move((__mks_525.right /*right*/),(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_527) {
                das_copy((__mks_527.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_525.op /*op*/),((char *) "!="));
            }        }))));
        __nand_rename_at_529_101; das_zero(__nand_rename_at_529_101); das_move(__nand_rename_at_529_101, das_ascend_handle<true,smart_ptr_raw<ExprOp2>>::make(__context__,nullptr,(([&](ExprOp2 & __mks_529) {
            das_copy((__mks_529.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            das_move((__mks_529.left /*left*/),(__neqz_rename_at_525_99));
            das_move((__mks_529.right /*right*/),(__cond_rename_at_518_94));
            {
                set_das_string(das_arg<das::string>::pass(__mks_529.op /*op*/),((char *) "&&"));
            }        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__cond_rename_at_518_94),das_auto_cast<smart_ptr_raw<void> &>::cast(__nand_rename_at_529_101),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        /* finally */ auto __finally_534= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprRef2Ptr>>::clear(__context__,__nsub_rename_at_535_103);
        /* end finally */ });
        __nsub_rename_at_535_103; das_zero(__nsub_rename_at_535_103); das_move(__nsub_rename_at_535_103, das_ascend_handle<true,smart_ptr_raw<ExprRef2Ptr>>::make(__context__,nullptr,(([&](ExprRef2Ptr & __mks_535) {
            das_copy((__mks_535.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
            das_move((__mks_535.subexpr /*subexpr*/),(__esub_rename_at_523_98));
            das_copy((__mks_535.genFlags /*genFlags*/),(0x1u));
        }))));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__esub_rename_at_523_98),das_auto_cast<smart_ptr_raw<void> &>::cast(__nsub_rename_at_535_103),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __vcast_rename_at_541_104; das_zero(__vcast_rename_at_541_104); das_move(__vcast_rename_at_541_104, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_541) {
        das_copy((__mks_541.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_move((__mks_541.subexpr /*subexpr*/),(__esub_rename_at_523_98));
        das_move((__mks_541.castType /*castType*/),(__ctype_rename_at_519_96));
        das_copy((__mks_541.castFlags /*castFlags*/),(0x2u));
        das_copy((__mks_541.genFlags /*genFlags*/),(0x1u));
    }))));
    __tri_rename_at_547_105; das_zero(__tri_rename_at_547_105); das_move(__tri_rename_at_547_105, das_ascend_handle<true,smart_ptr_raw<ExprOp3>>::make(__context__,nullptr,(([&](ExprOp3 & __mks_547) {
        das_copy((__mks_547.at /*at*/),(__expr_rename_at_515_88->at /*at*/));
        das_move((__mks_547.subexpr /*subexpr*/),(__cond_rename_at_518_94));
        das_move((__mks_547.left /*left*/),(__vcast_rename_at_541_104));
        das_move((__mks_547.right /*right*/),(__enull_rename_at_515_89));
        {
            set_das_string(das_arg<das::string>::pass(__mks_547.op /*op*/),((char *) "?"));
        }    }))));
    das_copy(__tri_rename_at_547_105->genFlags /*genFlags*/,__expr_rename_at_515_88->genFlags /*genFlags*/);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<ExprOp3>>::cast(__tri_rename_at_547_105);
}}

inline void clone_f1f421db208b5182 ( Context * __context__, smart_ptr<TypeDecl> & __dest_rename_at_604_107, smart_ptr_raw<TypeDecl> const  __src_rename_at_604_108 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_604_107),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_604_108),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_9ab0f6a4e0a6006e ( Context * __context__, smart_ptr<Enumeration> & __dest_rename_at_748_109, Enumeration * const  __src_rename_at_748_110 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_748_109),das_auto_cast<void * const >::cast(__src_rename_at_748_110),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Module * _FuncastTickfind_compiling_moduleTick4523452840392654583_34f86625d1f8ecb2 ( Context * __context__, char * const  __name_rename_at_958_111 )
{
    return das_auto_cast<Module *>::cast(_FuncastTickfind_moduleTick11101329256228773934_52d9ec34594ea355(__context__,compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_958_111));
}

inline void clone_3a3075a3a0c0cb06 ( Context * __context__, smart_ptr<Annotation> & __dest_rename_at_827_112, smart_ptr_raw<Annotation> const  __src_rename_at_827_113 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_827_112),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_827_113),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_47_114 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_47_114,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Expression * _FuncbuiltinTickget_ptrTick8468476673553620226_403d4a8bdcd4ca52 ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1727_115 )
{
    return das_auto_cast<Expression *>::cast(das_cast<Expression *>::cast(__src_rename_at_1727_115));
}

inline void clone_ec5e41b1830fac59 ( Context * __context__, das::vector<das::string> &  __args_rename_at_16_116, TArray<char *> &  __nargs_rename_at_16_117 )
{
    int32_t __tot_rename_at_17_118 = ((int32_t)builtin_array_size(das_arg<TArray<char *>>::pass(__nargs_rename_at_16_117)));
    das_vector_resize(das_arg<das::vector<das::string>>::pass(__args_rename_at_16_116),__tot_rename_at_17_118);
    {
        bool __need_loop_19 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__tot_rename_at_17_118));
        int32_t __i_rename_at_19_119;
        __need_loop_19 = __i_iterator.first(__context__,(__i_rename_at_19_119)) && __need_loop_19;
        for ( ; __need_loop_19 ; __need_loop_19 = __i_iterator.next(__context__,(__i_rename_at_19_119)) )
        {
            set_das_string(das_arg<das::string>::pass(das_index<das::vector<das::string>>::at(__args_rename_at_16_116,__i_rename_at_19_119,__context__)),__nargs_rename_at_16_117(__i_rename_at_19_119,__context__));
        }
        __i_iterator.close(__context__,(__i_rename_at_19_119));
    };
    _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(__nargs_rename_at_16_117));
}

inline void clone_36538fa38bdd6e0b ( Context * __context__, das::vector<smart_ptr<TypeDecl>> &  __args_rename_at_25_120, TArray<smart_ptr_raw<TypeDecl>> &  __nargs_rename_at_25_121 )
{
    das_vector_clear(das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__args_rename_at_25_120));
    {
        bool __need_loop_27 = true;
        // narg: smart_ptr<ast::TypeDecl> aka TypeDeclPtr&
        das_iterator<TArray<smart_ptr_raw<TypeDecl>>> __narg_iterator(__nargs_rename_at_25_121);
        smart_ptr_raw<TypeDecl> * __narg_rename_at_27_122;
        __need_loop_27 = __narg_iterator.first(__context__,(__narg_rename_at_27_122)) && __need_loop_27;
        for ( ; __need_loop_27 ; __need_loop_27 = __narg_iterator.next(__context__,(__narg_rename_at_27_122)) )
        {
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__args_rename_at_25_120),(*__narg_rename_at_27_122));
        }
        __narg_iterator.close(__context__,(__narg_rename_at_27_122));
    };
    _FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9(__context__,das_arg<TArray<smart_ptr_raw<TypeDecl>>>::pass(__nargs_rename_at_25_121));
}

inline void clone_82cb7bdbc1cd751b ( Context * __context__, das::vector<smart_ptr<Variable>> &  __args_rename_at_33_123, TArray<smart_ptr_raw<Variable>> &  __nargs_rename_at_33_124 )
{
    das_vector_clear(das_arg<das::vector<smart_ptr<Variable>>>::pass(__args_rename_at_33_123));
    {
        bool __need_loop_35 = true;
        // narg: smart_ptr<ast::Variable> aka VariablePtr&
        das_iterator<TArray<smart_ptr_raw<Variable>>> __narg_iterator(__nargs_rename_at_33_124);
        smart_ptr_raw<Variable> * __narg_rename_at_35_125;
        __need_loop_35 = __narg_iterator.first(__context__,(__narg_rename_at_35_125)) && __need_loop_35;
        for ( ; __need_loop_35 ; __need_loop_35 = __narg_iterator.next(__context__,(__narg_rename_at_35_125)) )
        {
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Variable>>>::pass(__args_rename_at_33_123),(*__narg_rename_at_35_125));
        }
        __narg_iterator.close(__context__,(__narg_rename_at_35_125));
    };
    _FuncbuiltinTickfinalizeTick13836114024949725080_c6f9bc68b25c5dd6(__context__,das_arg<TArray<smart_ptr_raw<Variable>>>::pass(__nargs_rename_at_33_124));
}

inline void clone_ab5928efc1a1d61 ( Context * __context__, das::vector<smart_ptr<Expression>> &  __args_rename_at_41_126, TArray<smart_ptr_raw<Expression>> &  __nargs_rename_at_41_127 )
{
    das_vector_clear(das_arg<das::vector<smart_ptr<Expression>>>::pass(__args_rename_at_41_126));
    {
        bool __need_loop_43 = true;
        // narg: smart_ptr<ast::Expression> aka ExpressionPtr&
        das_iterator<TArray<smart_ptr_raw<Expression>>> __narg_iterator(__nargs_rename_at_41_127);
        smart_ptr_raw<Expression> * __narg_rename_at_43_128;
        __need_loop_43 = __narg_iterator.first(__context__,(__narg_rename_at_43_128)) && __need_loop_43;
        for ( ; __need_loop_43 ; __need_loop_43 = __narg_iterator.next(__context__,(__narg_rename_at_43_128)) )
        {
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__args_rename_at_41_126),(*__narg_rename_at_43_128));
        }
        __narg_iterator.close(__context__,(__narg_rename_at_43_128));
    };
    _FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__nargs_rename_at_41_127));
}

inline bool isVectorType_e16b21c2cb0df3cf ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __typ_rename_at_49_129 )
{
    return das_auto_cast<bool>::cast(((((((((((((__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tFloat2)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__typ_rename_at_49_129 == DAS_COMMENT(bound_enum) das::Type::tURange64));
}

inline char * describe_f8955cd58aaa956c ( Context * __context__, AnnotationArgumentList const  &  __list_rename_at_59_130 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_60_18_rename_at_60_131; das_zero(____acomp_60_18_rename_at_60_131);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_60_18_rename_at_60_131),false);
        {
            bool __need_loop_60 = true;
            // arg: rtti::AnnotationArgument const&
            das_iterator<AnnotationArgumentList const > __arg_iterator(__list_rename_at_59_130);
            AnnotationArgument const  * __arg_rename_at_60_132;
            __need_loop_60 = __arg_iterator.first(__context__,(__arg_rename_at_60_132)) && __need_loop_60;
            for ( ; __need_loop_60 ; __need_loop_60 = __arg_iterator.next(__context__,(__arg_rename_at_60_132)) )
            {
                TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_60_50_272; _temp_make_local_60_50_272;
                _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1(__context__,das_arg<TArray<char *>>::pass(____acomp_60_18_rename_at_60_131),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_3, cast<das::string const  &>::from((*__arg_rename_at_60_132).name /*name*/), cast<char * const >::from(((char *) "=")), cast<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> &>::from((_temp_make_local_60_50_272 = (das_alias<RttiValue>::from(rtti_builtin_argument_value((*__arg_rename_at_60_132),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_60_132));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_60_18_rename_at_60_131),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_60_18_rename_at_60_131);
    })),((char *) ",")));
}

inline char * describe_16a184ebde2f5850 ( Context * __context__, AnnotationDeclaration const  &  __ann_rename_at_63_133 )
{
    return das_auto_cast<char *>::cast((das_vector_length(__ann_rename_at_63_133.arguments /*arguments*/) != 0) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_4, cast<das::string const  &>::from(__ann_rename_at_63_133.annotation /*annotation*/->name /*name*/), cast<char * const >::from(((char *) "(")), cast<char * const >::from(describe_f8955cd58aaa956c(__context__,__ann_rename_at_63_133.arguments /*arguments*/)), cast<char * const >::from(((char *) ")"))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<das::string const  &>::from(__ann_rename_at_63_133.annotation /*annotation*/->name /*name*/)))));
}

inline char * describe_5703109ec79a52b4 ( Context * __context__, AnnotationList const  &  __list_rename_at_71_134 )
{
    return das_auto_cast<char *>::cast(_Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835(__context__,das_arg<TArray<char *>>::pass(das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_72_18_rename_at_72_135; das_zero(____acomp_72_18_rename_at_72_135);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_72_18_rename_at_72_135),false);
        {
            bool __need_loop_72 = true;
            // arg: smart_ptr<rtti::AnnotationDeclaration> const&
            das_iterator<AnnotationList const > __arg_iterator(__list_rename_at_71_134);
            smart_ptr<AnnotationDeclaration> const  * __arg_rename_at_72_136;
            __need_loop_72 = __arg_iterator.first(__context__,(__arg_rename_at_72_136)) && __need_loop_72;
            for ( ; __need_loop_72 ; __need_loop_72 = __arg_iterator.next(__context__,(__arg_rename_at_72_136)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(____acomp_72_18_rename_at_72_135),describe_16a184ebde2f5850(__context__,das_deref(__context__,(*__arg_rename_at_72_136))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_72_136));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_72_18_rename_at_72_135),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_72_18_rename_at_72_135);
    })),((char *) ",")));
}

inline char * describe_9b40ad62f87c706d ( Context * __context__, smart_ptr_raw<Variable> const  __vvar_rename_at_75_137 )
{
    return das_auto_cast<char *>::cast((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__vvar_rename_at_75_137->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<das::string const  &>::from(__vvar_rename_at_75_137->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__vvar_rename_at_75_137->type /*_type*/,true,true,true))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_7, cast<das::string const  &>::from(__vvar_rename_at_75_137->name /*name*/), cast<char * const >::from(((char *) ":null"))))));
}

inline bool isExpression_a7c31959e6c730e8 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __t_rename_at_96_138, bool __top_rename_at_96_139 )
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__t_rename_at_96_138),das_auto_cast<void * const >::cast(nullptr)) )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( das_vector_length(__t_rename_at_96_138->dim /*dim*/) != 0 )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( __t_rename_at_96_138->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle )
    {
        return das_auto_cast<bool>::cast((eq_dstr_str(__t_rename_at_96_138->annotation /*annotation*/->module /*_module*/->name /*name*/,((char *) "ast"))) ? das_auto_cast<bool>::cast(builtin_string_startswith(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_8, cast<das::string const  &>::from(__t_rename_at_96_138->annotation /*annotation*/->name /*name*/))),((char *) "Expr"),__context__)) : das_auto_cast<bool>::cast(false));
    } else return das_auto_cast<bool>::cast(((__t_rename_at_96_138->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer) && __top_rename_at_96_139) ? das_auto_cast<bool>::cast(isExpression_a7c31959e6c730e8(__context__,__t_rename_at_96_138->firstType /*firstType*/,false)) : das_auto_cast<bool>::cast(false));
}

inline bool is_same_or_inherited_8fc466ebe75e2336 ( Context * __context__, Structure const  * const  __parent_rename_at_114_140, Structure const  * const  __child_rename_at_114_141 )
{
    Structure const  * __ch_rename_at_115_142 = __child_rename_at_114_141;
    while ( __ch_rename_at_115_142 != nullptr )
    {
        if ( __parent_rename_at_114_140 == __ch_rename_at_115_142 )
        {
            return das_auto_cast<bool>::cast(true);
        } else {
            das_copy(__ch_rename_at_115_142,__ch_rename_at_115_142->parent /*parent*/);
        };
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool is_class_method_67e7c960daf5e6e2 ( Context * __context__, smart_ptr_raw<Structure> const  __cinfo_rename_at_125_143, smart_ptr_raw<TypeDecl> const  __finfo_rename_at_125_144 )
{
    return das_auto_cast<bool>::cast((__finfo_rename_at_125_144->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tFunction) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_125_144->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(__finfo_rename_at_125_144->argTypes /*argTypes*/) == 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_125_144->argTypes /*argTypes*/,0,__context__)->baseType /*baseType*/ != DAS_COMMENT(bound_enum) das::Type::tStructure) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((das_vector_length(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_125_144->argTypes /*argTypes*/,0,__context__)->dim /*dim*/) != 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(is_same_or_inherited_8fc466ebe75e2336(__context__,das_reinterpret<Structure const  * const >::pass(das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__finfo_rename_at_125_144->argTypes /*argTypes*/,0,__context__)->structType /*structType*/),das_reinterpret<Structure const  * const >::pass(_FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850(__context__,__cinfo_rename_at_125_143)))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))))))))));
}

inline void emplace_new_eadb1a1435b53033 ( Context * __context__, das::vector<smart_ptr<Expression>> &  __vec_rename_at_147_145, smart_ptr_raw<Expression> __ptr_rename_at_147_146 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__vec_rename_at_147_145),__ptr_rename_at_147_146);
}

inline void emplace_new_52cce80de15d9fe4 ( Context * __context__, das::vector<smart_ptr<TypeDecl>> &  __vec_rename_at_151_147, smart_ptr_raw<TypeDecl> __ptr_rename_at_151_148 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__vec_rename_at_151_147),__ptr_rename_at_151_148);
}

inline void emplace_new_e46333e37cb8f35d ( Context * __context__, das::vector<smart_ptr<Variable>> &  __vec_rename_at_155_149, smart_ptr_raw<Variable> __ptr_rename_at_155_150 )
{
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Variable>>>::pass(__vec_rename_at_155_149),__ptr_rename_at_155_150);
}

inline void emplace_new_2dcb96a028e6e2a ( Context * __context__, MakeStruct &  __vec_rename_at_159_151, smart_ptr_raw<MakeFieldDecl> __ptr_rename_at_159_152 )
{
    mks_vector_emplace(das_arg<MakeStruct>::pass(__vec_rename_at_159_151),__ptr_rename_at_159_152);
}

inline bool override_method_38d6ecad3789a50f ( Context * __context__, smart_ptr_raw<Structure> __str_rename_at_163_153, char * const  __name_rename_at_163_154, char * const  __funcName_rename_at_163_155 ) { das_stack_prologue __prologue(__context__,272,"override_method " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprCast> __vcast_rename_at_166_157; memset(&__vcast_rename_at_166_157,0,sizeof(__vcast_rename_at_166_157));
    {
        bool __need_loop_164 = true;
        // fld: ast::FieldDeclaration&
        das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__str_rename_at_163_153->fields /*fields*/);
        Structure::FieldDeclaration * __fld_rename_at_164_156;
        __need_loop_164 = __fld_iterator.first(__context__,(__fld_rename_at_164_156)) && __need_loop_164;
        for ( ; __need_loop_164 ; __need_loop_164 = __fld_iterator.next(__context__,(__fld_rename_at_164_156)) )
        {
            if ( eq_dstr_str(das_arg<das::string>::pass((*__fld_rename_at_164_156).name /*name*/),__name_rename_at_163_154) )
            {
                /* finally */ auto __finally_165= das_finally([&](){
                das_delete_handle<smart_ptr_raw<ExprCast>>::clear(__context__,__vcast_rename_at_166_157);
                /* end finally */ });
                __vcast_rename_at_166_157; das_zero(__vcast_rename_at_166_157); das_move(__vcast_rename_at_166_157, das_ascend_handle<true,smart_ptr_raw<ExprCast>>::make(__context__,nullptr,(([&](ExprCast & __mks_166) {
                    das_copy((__mks_166.at /*at*/),(__str_rename_at_163_153->at /*at*/));
                    das_move((__mks_166.subexpr /*subexpr*/),(das_ascend_handle<true,smart_ptr_raw<ExprAddr>>::make(__context__,nullptr,(([&](ExprAddr & __mks_167) {
                        das_copy((__mks_167.at /*at*/),(__str_rename_at_163_153->at /*at*/));
                        {
                            set_das_string(das_arg<das::string>::pass(__mks_167.target /*target*/),__funcName_rename_at_163_155);
                        }                    })))));
                    das_move((__mks_166.castType /*castType*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_168) {
                        das_copy((__mks_168.at /*at*/),(__str_rename_at_163_153->at /*at*/));
                        das_copy((__mks_168.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                    })))));
                }))));
                builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast((*__fld_rename_at_164_156).init /*init*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__vcast_rename_at_166_157),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                return das_auto_cast<bool>::cast(true);
            };
        }
        __fld_iterator.close(__context__,(__fld_rename_at_164_156));
    };
    return das_auto_cast<bool>::cast(false);
}}

inline void for_each_tag_function_bfda4041b94932d7 ( Context * __context__, Module * const  __mod_rename_at_216_159, char * const  __tag_rename_at_216_160, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __blk_rename_at_216_161 ) { das_stack_prologue __prologue(__context__,416,"for_each_tag_function " DAS_FILE_LINE);
{
    forEachFunction(__mod_rename_at_216_159,nullptr,das_make_block<void,smart_ptr<Function>>(__context__,80,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_217_162) -> void{
        {
            bool __need_loop_218 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_217_162->annotations /*annotations*/);
            smart_ptr<AnnotationDeclaration> * __ann_rename_at_218_163;
            __need_loop_218 = __ann_iterator.first(__context__,(__ann_rename_at_218_163)) && __need_loop_218;
            for ( ; __need_loop_218 ; __need_loop_218 = __ann_iterator.next(__context__,(__ann_rename_at_218_163)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_218_163)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_218_163)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_220 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_218_163)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_220_164;
                        __need_loop_220 = __t_iterator.first(__context__,(__t_rename_at_220_164)) && __need_loop_220;
                        for ( ; __need_loop_220 ; __need_loop_220 = __t_iterator.next(__context__,(__t_rename_at_220_164)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_220_164).name /*name*/),__tag_rename_at_216_160) )
                            {
                                das_invoke<void>::invoke<smart_ptr<Function>>(__context__,nullptr,__blk_rename_at_216_161,__func_rename_at_217_162);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_220_164));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_218_163));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    forEachGenericFunction(__mod_rename_at_216_159,nullptr,das_make_block<void,smart_ptr<Function>>(__context__,272,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_228_165) -> void{
        {
            bool __need_loop_229 = true;
            // ann: smart_ptr<rtti::AnnotationDeclaration>&
            das_iterator<AnnotationList> __ann_iterator(__func_rename_at_228_165->annotations /*annotations*/);
            smart_ptr<AnnotationDeclaration> * __ann_rename_at_229_166;
            __need_loop_229 = __ann_iterator.first(__context__,(__ann_rename_at_229_166)) && __need_loop_229;
            for ( ; __need_loop_229 ; __need_loop_229 = __ann_iterator.next(__context__,(__ann_rename_at_229_166)) )
            {
                if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__ann_rename_at_229_166)),das_auto_cast<void * const >::cast(nullptr))) && (eq_dstr_str(das_arg<das::string>::pass((*__ann_rename_at_229_166)->annotation /*annotation*/->name /*name*/),((char *) "tag_function"))) )
                {
                    {
                        bool __need_loop_231 = true;
                        // t: rtti::AnnotationArgument&
                        das_iterator<AnnotationArgumentList> __t_iterator((*__ann_rename_at_229_166)->arguments /*arguments*/);
                        AnnotationArgument * __t_rename_at_231_167;
                        __need_loop_231 = __t_iterator.first(__context__,(__t_rename_at_231_167)) && __need_loop_231;
                        for ( ; __need_loop_231 ; __need_loop_231 = __t_iterator.next(__context__,(__t_rename_at_231_167)) )
                        {
                            if ( eq_dstr_str(das_arg<das::string>::pass((*__t_rename_at_231_167).name /*name*/),__tag_rename_at_216_160) )
                            {
                                das_invoke<void>::invoke<smart_ptr<Function>>(__context__,nullptr,__blk_rename_at_216_161,__func_rename_at_228_165);
                            };
                        }
                        __t_iterator.close(__context__,(__t_rename_at_231_167));
                    };
                };
            }
            __ann_iterator.close(__context__,(__ann_rename_at_229_166));
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_bb44f3ca6ce3fc6d ( Context * __context__, char * const  __argn_rename_at_242_168, AnnotationArgumentList const  &  __args_rename_at_242_169 )
{
    {
        bool __need_loop_243 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_242_169);
        AnnotationArgument const  * __a_rename_at_243_170;
        __need_loop_243 = __a_iterator.first(__context__,(__a_rename_at_243_170)) && __need_loop_243;
        for ( ; __need_loop_243 ; __need_loop_243 = __a_iterator.next(__context__,(__a_rename_at_243_170)) )
        {
            if ( eq_dstr_str((*__a_rename_at_243_170).name /*name*/,__argn_rename_at_242_168) )
            {
                return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(das_alias<RttiValue>::from(rtti_builtin_argument_value((*__a_rename_at_243_170),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_243_170));
    };
    return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_3746f4c2db327a87(__context__));
}

inline TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> find_arg_a148d1ff83124bd1 ( Context * __context__, AnnotationArgumentList const  &  __args_rename_at_251_171, char * const  __argn_rename_at_251_172 )
{
    {
        bool __need_loop_252 = true;
        // a: rtti::AnnotationArgument const&
        das_iterator<AnnotationArgumentList const > __a_iterator(__args_rename_at_251_171);
        AnnotationArgument const  * __a_rename_at_252_173;
        __need_loop_252 = __a_iterator.first(__context__,(__a_rename_at_252_173)) && __need_loop_252;
        for ( ; __need_loop_252 ; __need_loop_252 = __a_iterator.next(__context__,(__a_rename_at_252_173)) )
        {
            if ( eq_dstr_str((*__a_rename_at_252_173).name /*name*/,__argn_rename_at_251_172) )
            {
                return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(das_alias<RttiValue>::from(rtti_builtin_argument_value((*__a_rename_at_252_173),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_252_173));
    };
    return das_auto_cast_ref<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>::cast(_FuncrttiTickRttiValue_nothingTick4715542659269841615_3746f4c2db327a87(__context__));
}

inline void apply_tag_annotation_f773cf67c31220 ( Context * __context__, char * const  __tag_rename_at_260_174, smart_ptr_raw<FunctionAnnotation> __ann_rename_at_260_175 ) { das_stack_prologue __prologue(__context__,160,"apply_tag_annotation " DAS_FILE_LINE);
{
    TArray<smart_ptr_raw<Function>> __funcs_rename_at_261_176; memset(&__funcs_rename_at_261_176,0,sizeof(__funcs_rename_at_261_176));
    smart_ptr_raw<FunctionAnnotation> __tagAnn_rename_at_267_179; memset(&__tagAnn_rename_at_267_179,0,sizeof(__tagAnn_rename_at_267_179));
    /* finally */ auto __finally_260= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_aba46312bb49c9f6(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_261_176));
    /* end finally */ });
    __funcs_rename_at_261_176; das_zero(__funcs_rename_at_261_176);
    for_each_tag_function_bfda4041b94932d7(__context__,thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__tag_rename_at_260_174,das_make_block<void,smart_ptr_raw<Function>>(__context__,112,0,&__func_info__806bd08fbfbd09f4,[&](smart_ptr_raw<Function> __func_rename_at_262_177) -> void{
        _FuncbuiltinTickpush_cloneTick2035469273396957942_a44b26c5d2257edc(__context__,das_arg<TArray<smart_ptr_raw<Function>>>::pass(__funcs_rename_at_261_176),__func_rename_at_262_177);
    }));
    {
        bool __need_loop_265 = true;
        // func: smart_ptr<ast::Function> aka FunctionPtr&
        das_iterator<TArray<smart_ptr_raw<Function>>> __func_iterator(__funcs_rename_at_261_176);
        smart_ptr_raw<Function> * __func_rename_at_265_178;
        __need_loop_265 = __func_iterator.first(__context__,(__func_rename_at_265_178)) && __need_loop_265;
        for ( ; __need_loop_265 ; __need_loop_265 = __func_iterator.next(__context__,(__func_rename_at_265_178)) )
        {
            {
                /* finally */ auto __finally_266= das_finally([&](){
                das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__tagAnn_rename_at_267_179);
                /* end finally */ });
                __tagAnn_rename_at_267_179; das_zero(__tagAnn_rename_at_267_179); das_move(__tagAnn_rename_at_267_179, _FuncbuiltinTickclone_to_moveTick2007252383599261567_ddf94d0785b39e5b(__context__,__ann_rename_at_260_175));
                addFunctionFunctionAnnotation((*__func_rename_at_265_178),__tagAnn_rename_at_267_179,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __func_iterator.close(__context__,(__func_rename_at_265_178));
    };
}}

inline smart_ptr_raw<Function> find_unique_function_aab3bc60151e96ec ( Context * __context__, Module * const  __mod_rename_at_279_180, char * const  __name_rename_at_279_181, bool __canfail_rename_at_279_182 ) { das_stack_prologue __prologue(__context__,128,"find_unique_function " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_280_183; memset(&__res_rename_at_280_183,0,sizeof(__res_rename_at_280_183));
    int32_t __count_rename_at_281_184; memset(&__count_rename_at_281_184,0,sizeof(__count_rename_at_281_184));
    /* finally */ auto __finally_279= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_280_183);
    /* end finally */ });
    __res_rename_at_280_183; das_zero(__res_rename_at_280_183);
    __count_rename_at_281_184 = 0;
    forEachFunction(__mod_rename_at_279_180,__name_rename_at_279_181,das_make_block<void,smart_ptr<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_282_185) -> void{
        clone_8c54ff7e0079253a(__context__,__res_rename_at_280_183,__func_rename_at_282_185);
        ++__count_rename_at_281_184;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_281_184 > 1 )
    {
        if ( !__canfail_rename_at_279_182 )
        {
            builtin_throw(((char *) "more than one function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_280_183);
    };
}}

inline smart_ptr_raw<Function> find_unique_generic_b6f2e7df17214306 ( Context * __context__, Module * const  __mod_rename_at_295_186, char * const  __name_rename_at_295_187, bool __canfail_rename_at_295_188 ) { das_stack_prologue __prologue(__context__,128,"find_unique_generic " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __res_rename_at_296_189; memset(&__res_rename_at_296_189,0,sizeof(__res_rename_at_296_189));
    int32_t __count_rename_at_297_190; memset(&__count_rename_at_297_190,0,sizeof(__count_rename_at_297_190));
    /* finally */ auto __finally_295= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__res_rename_at_296_189);
    /* end finally */ });
    __res_rename_at_296_189; das_zero(__res_rename_at_296_189);
    __count_rename_at_297_190 = 0;
    forEachGenericFunction(__mod_rename_at_295_186,__name_rename_at_295_187,das_make_block<void,smart_ptr<Function>>(__context__,112,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_298_191) -> void{
        clone_8c54ff7e0079253a(__context__,__res_rename_at_296_189,__func_rename_at_298_191);
        ++__count_rename_at_297_190;
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __count_rename_at_297_190 > 1 )
    {
        if ( !__canfail_rename_at_295_188 )
        {
            builtin_throw(((char *) "more than one generic function encountered"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(nullptr);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(__res_rename_at_296_189);
    };
}}

inline ExprBlock * setup_call_list_ad0757bf84a727d8 ( Context * __context__, char * const  __name_rename_at_311_192, LineInfo const  &  __at_rename_at_311_193, Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  __subblock_rename_at_311_194 ) { das_stack_prologue __prologue(__context__,192,"setup_call_list " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_312_195; memset(&__fn_rename_at_312_195,0,sizeof(__fn_rename_at_312_195));
    smart_ptr_raw<ExprBlock> __blk_rename_at_318_197; memset(&__blk_rename_at_318_197,0,sizeof(__blk_rename_at_318_197));
    ExprBlock * __res_rename_at_319_198; memset(&__res_rename_at_319_198,0,sizeof(__res_rename_at_319_198));
    /* finally */ auto __finally_311= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_312_195);
    /* end finally */ });
    __fn_rename_at_312_195; das_zero(__fn_rename_at_312_195); das_move(__fn_rename_at_312_195, find_unique_function_aab3bc60151e96ec(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_311_192,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_312_195),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_313= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_318_197);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_312_195),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_314) {
            das_copy((__mks_314.at /*at*/),(__at_rename_at_311_193));
            das_copy((__mks_314.atDecl /*atDecl*/),(__at_rename_at_311_193));
            das_copy((__mks_314.flags /*flags*/),(0x200000u));
            das_move((__mks_314.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_316) {
                das_copy((__mks_316.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_316.at /*at*/),(__at_rename_at_311_193));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_314.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_9, cast<char * const >::from(__name_rename_at_311_192))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_318_197; das_zero(__blk_rename_at_318_197); das_move(__blk_rename_at_318_197, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_318) {
            das_copy((__mks_318.at /*at*/),(__at_rename_at_311_193));
        }))));
        __res_rename_at_319_198 = _FuncbuiltinTickget_ptrTick5807679485210906136_8c07d4f696b9c5ba(__context__,__blk_rename_at_318_197);
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_312_195->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_318_197),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_invoke<void>::invoke<smart_ptr_raw<Function>>(__context__,nullptr,__subblock_rename_at_311_194,__fn_rename_at_312_195);
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_312_195,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_10, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_312_195->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(__res_rename_at_319_198);
    } else if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_312_195->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
    {
        builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__fn_rename_at_312_195->body /*body*/));
}}

inline ExprBlock * setup_call_list_f7df6b96f49a9cf8 ( Context * __context__, char * const  __name_rename_at_332_199, LineInfo const  &  __at_rename_at_332_200, bool __isInit_rename_at_332_201, bool __isPrivate_rename_at_332_202, bool __isLateInit_rename_at_332_203 ) { das_stack_prologue __prologue(__context__,96,"setup_call_list " DAS_FILE_LINE);
{
    return das_auto_cast<ExprBlock *>::cast(setup_call_list_ad0757bf84a727d8(__context__,__name_rename_at_332_199,__at_rename_at_332_200,das_make_block<void,smart_ptr_raw<Function>>(__context__,80,0,&__func_info__3b2c4edb933bef4a,[&](smart_ptr_raw<Function> __fn_rename_at_333_204) -> void{
        if ( __isInit_rename_at_332_201 )
        {
            __fn_rename_at_333_204->flags /*flags*/ |= 0x100u;
            if ( __isLateInit_rename_at_332_203 )
            {
                __fn_rename_at_333_204->moreFlags /*moreFlags*/ |= 0x8u;
            };
        };
        if ( __isPrivate_rename_at_332_202 )
        {
            __fn_rename_at_333_204->flags /*flags*/ |= 0x400000u;
        };
    })));
}}

inline ExprBlock * setup_macro_1ed401bbb9e11b66 ( Context * __context__, char * const  __name_rename_at_346_205, LineInfo const  &  __at_rename_at_346_206 ) { das_stack_prologue __prologue(__context__,480,"setup_macro " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_348_207; memset(&__fn_rename_at_348_207,0,sizeof(__fn_rename_at_348_207));
    smart_ptr_raw<ExprBlock> __blk_rename_at_354_209; memset(&__blk_rename_at_354_209,0,sizeof(__blk_rename_at_354_209));
    smart_ptr_raw<ExprBlock> __iblk_rename_at_355_210; memset(&__iblk_rename_at_355_210,0,sizeof(__iblk_rename_at_355_210));
    ExprBlock * __res_rename_at_356_211; memset(&__res_rename_at_356_211,0,sizeof(__res_rename_at_356_211));
    smart_ptr_raw<ExprCall> __ifm_rename_at_357_212; memset(&__ifm_rename_at_357_212,0,sizeof(__ifm_rename_at_357_212));
    smart_ptr_raw<ExprConstString> __ifmn_rename_at_358_214; memset(&__ifmn_rename_at_358_214,0,sizeof(__ifmn_rename_at_358_214));
    smart_ptr_raw<ExprIfThenElse> __ife_rename_at_360_216; memset(&__ife_rename_at_360_216,0,sizeof(__ife_rename_at_360_216));
    ExprBlock * __blk_rename_at_372_217; memset(&__blk_rename_at_372_217,0,sizeof(__blk_rename_at_372_217));
    ExprIfThenElse * __ife_rename_at_376_218; memset(&__ife_rename_at_376_218,0,sizeof(__ife_rename_at_376_218));
    /* finally */ auto __finally_346= das_finally([&](){
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_348_207);
    /* end finally */ });
    compileProgram(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->flags /*flags*/ |= 0x20u;
    __fn_rename_at_348_207; das_zero(__fn_rename_at_348_207); das_move(__fn_rename_at_348_207, find_unique_function_aab3bc60151e96ec(__context__,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__name_rename_at_346_205,false));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_348_207),das_auto_cast<void * const >::cast(nullptr)) )
    {
        /* finally */ auto __finally_349= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprIfThenElse>>::clear(__context__,__ife_rename_at_360_216);
        das_delete_handle<smart_ptr_raw<ExprConstString>>::clear(__context__,__ifmn_rename_at_358_214);
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__ifm_rename_at_357_212);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__iblk_rename_at_355_210);
        das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_354_209);
        /* end finally */ });
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_348_207),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_350) {
            das_copy((__mks_350.at /*at*/),(__at_rename_at_346_206));
            das_copy((__mks_350.atDecl /*atDecl*/),(__at_rename_at_346_206));
            das_copy((__mks_350.flags /*flags*/),(0x80600000u));
            das_move((__mks_350.result /*result*/),(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_352) {
                das_copy((__mks_352.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
                das_copy((__mks_352.at /*at*/),(__at_rename_at_346_206));
            })))));
            {
                set_das_string(das_arg<das::string>::pass(__mks_350.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_11, cast<char * const >::from(__name_rename_at_346_205))));
            }        })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        __blk_rename_at_354_209; das_zero(__blk_rename_at_354_209); das_move(__blk_rename_at_354_209, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_354) {
            das_copy((__mks_354.at /*at*/),(__at_rename_at_346_206));
        }))));
        __iblk_rename_at_355_210; das_zero(__iblk_rename_at_355_210); das_move(__iblk_rename_at_355_210, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_355) {
            das_copy((__mks_355.at /*at*/),(__at_rename_at_346_206));
        }))));
        __res_rename_at_356_211 = ((ExprBlock *)_FuncbuiltinTickget_ptrTick5807679485210906136_8c07d4f696b9c5ba(__context__,__iblk_rename_at_355_210));
        __ifm_rename_at_357_212; das_zero(__ifm_rename_at_357_212); das_move(__ifm_rename_at_357_212, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_357) {
            das_copy((__mks_357.at /*at*/),(__at_rename_at_346_206));
            {
                set_das_string(das_arg<das::string>::pass(__mks_357.name /*name*/),((char *) "is_compiling_macros_in_module"));
            }        }))));
        __ifmn_rename_at_358_214; das_zero(__ifmn_rename_at_358_214); das_move(__ifmn_rename_at_358_214, das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_358) {
            das_copy((__mks_358.at /*at*/),(__at_rename_at_346_206));
            {
                das_clone<das::string,das::string>::clone(__mks_358.text /*value*/,compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))->name /*name*/);
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__ifm_rename_at_357_212->arguments /*arguments*/),das_reinterpret<smart_ptr<Expression>>::pass(__ifmn_rename_at_358_214));
        __ife_rename_at_360_216; das_zero(__ife_rename_at_360_216); das_move(__ife_rename_at_360_216, das_ascend_handle<true,smart_ptr_raw<ExprIfThenElse>>::make(__context__,nullptr,(([&](ExprIfThenElse & __mks_360) {
            das_copy((__mks_360.at /*at*/),(__at_rename_at_346_206));
            das_move((__mks_360.cond /*cond*/),(__ifm_rename_at_357_212));
            das_move((__mks_360.if_true /*if_true*/),(__iblk_rename_at_355_210));
        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_354_209->list /*list*/),das_reinterpret<smart_ptr<Expression>>::pass(__ife_rename_at_360_216));
        builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_348_207->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_354_209),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        if ( !addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_348_207,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_12, cast<char * const >::from(((char *) "failed to setup macro, can't add function ")), cast<das::string &>::from(__fn_rename_at_348_207->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__res_rename_at_356_211));
    } else {
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__fn_rename_at_348_207->body /*body*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting func.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __blk_rename_at_372_217 = das_cast<ExprBlock *>::cast(__fn_rename_at_348_207->body /*body*/);
        if ( (das_vector_length(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_372_217->list /*list*/)) != 1) || (SimPolicy<char *>::NotEqu(cast<char *>::from(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_372_217->list /*list*/,0,__context__)->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprIfThenElse")),*__context__,nullptr)) )
        {
            builtin_throw(((char *) "expecting is_compiling_macros"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        __ife_rename_at_376_218 = das_cast<ExprIfThenElse *>::cast(das_index<das::vector<smart_ptr<Expression>>>::at(__blk_rename_at_372_217->list /*list*/,0,__context__));
        if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__ife_rename_at_376_218->if_true /*if_true*/->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprBlock")),*__context__,nullptr) )
        {
            builtin_throw(((char *) "expecting ife.if_true.ExprBlock"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<ExprBlock *>::cast(das_cast<ExprBlock *>::cast(__ife_rename_at_376_218->if_true /*if_true*/));
    };
}}

inline void * panic_expr_as_b448efb56dfa5ea ( Context * __context__ )
{
    builtin_throw(((char *) "invalid 'as' expression or null pointer dereference"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast<void *>::cast(nullptr);
}

inline smart_ptr_raw<Expression> walk_and_convert_array_fc0c75b20fd9b5d8 ( Context * __context__, uint8_t const  * const  __data_rename_at_582_219, smart_ptr_raw<TypeDecl> const  __info_rename_at_582_220, LineInfo const  &  __at_rename_at_582_221 ) { das_stack_prologue __prologue(__context__,272,"walk_and_convert_array " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_585_223; memset(&__stride_rename_at_585_223,0,sizeof(__stride_rename_at_585_223));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_586_224; memset(&__mkArr_rename_at_586_224,0,sizeof(__mkArr_rename_at_586_224));
    smart_ptr_raw<ExprCall> __mkToArrayMove_rename_at_591_226; memset(&__mkToArrayMove_rename_at_591_226,0,sizeof(__mkToArrayMove_rename_at_591_226));
    int32_t __total_rename_at_583_222 = ((int32_t)any_array_size(das_auto_cast<void * const >::cast(__data_rename_at_582_219)));
    if ( __total_rename_at_583_222 != 0 )
    {
        /* finally */ auto __finally_584= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToArrayMove_rename_at_591_226);
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_586_224);
        /* end finally */ });
        __stride_rename_at_585_223 = ((int32_t)((das_deref(__context__,__info_rename_at_582_220->firstType /*firstType*/)).getSizeOf()));
        __mkArr_rename_at_586_224; das_zero(__mkArr_rename_at_586_224); das_move(__mkArr_rename_at_586_224, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_586) {
            das_copy((__mks_586.at /*at*/),(__at_rename_at_582_221));
            das_move((__mks_586.makeType /*makeType*/),(clone_type(__info_rename_at_582_220->firstType /*firstType*/)));
        }))));
        any_array_foreach(das_auto_cast<void * const >::cast(__data_rename_at_582_219),__stride_rename_at_585_223,das_make_block<void,void *>(__context__,144,0,&__func_info__b28b606ddb7a6760,[&](void * __value_rename_at_587_225) -> void{
            emplace_new_eadb1a1435b53033(__context__,das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_586_224->values /*values*/),walk_and_convert_52e7fa0c06656c3c(__context__,das_auto_cast<uint8_t const  * const >::cast(__value_rename_at_587_225),__info_rename_at_582_220->firstType /*firstType*/,__at_rename_at_582_221));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_586_224->makeType /*makeType*/->dim /*dim*/),__total_rename_at_583_222);
        __mkToArrayMove_rename_at_591_226; das_zero(__mkToArrayMove_rename_at_591_226); das_move(__mkToArrayMove_rename_at_591_226, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_591) {
            das_copy((__mks_591.at /*at*/),(__at_rename_at_582_221));
            {
                set_das_string(das_arg<das::string>::pass(__mks_591.name /*name*/),((char *) "to_array_move"));
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToArrayMove_rename_at_591_226->arguments /*arguments*/),das_reinterpret<smart_ptr<Expression>>::pass(__mkArr_rename_at_586_224));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkToArrayMove_rename_at_591_226);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_595) {
            das_copy((__mks_595.at /*at*/),(__at_rename_at_582_221));
            das_move((__mks_595.makeType /*makeType*/),(clone_type(__info_rename_at_582_220)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_dim_307e83e2e898f055 ( Context * __context__, uint8_t const  * const  __data_rename_at_599_228, smart_ptr_raw<TypeDecl> const  __info_rename_at_599_229, LineInfo const  &  __at_rename_at_599_230 ) { das_stack_prologue __prologue(__context__,208,"walk_and_convert_dim " DAS_FILE_LINE);
{
    int32_t __stride_rename_at_600_231; memset(&__stride_rename_at_600_231,0,sizeof(__stride_rename_at_600_231));
    int32_t __total_rename_at_601_232; memset(&__total_rename_at_601_232,0,sizeof(__total_rename_at_601_232));
    smart_ptr<TypeDecl> __einfo_rename_at_602_233; memset(&__einfo_rename_at_602_233,0,sizeof(__einfo_rename_at_602_233));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_604_234; memset(&__mkArr_rename_at_604_234,0,sizeof(__mkArr_rename_at_604_234));
    smart_ptr_raw<Expression> __elem_rename_at_607_237; memset(&__elem_rename_at_607_237,0,sizeof(__elem_rename_at_607_237));
    /* finally */ auto __finally_599= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_604_234);
    das_delete_handle<smart_ptr<TypeDecl>>::clear(__context__,__einfo_rename_at_602_233);
    /* end finally */ });
    __stride_rename_at_600_231 = ((int32_t)((das_deref(__context__,__info_rename_at_599_229)).getBaseSizeOf()));
    __total_rename_at_601_232 = ((int32_t)((das_deref(__context__,__info_rename_at_599_229)).getCountOf()));
    __einfo_rename_at_602_233; das_zero(__einfo_rename_at_602_233); das_move(__einfo_rename_at_602_233, clone_type(__info_rename_at_599_229));
    das_vector_clear(das_arg<das::vector<int32_t>>::pass(__einfo_rename_at_602_233->dim /*dim*/));
    __mkArr_rename_at_604_234; das_zero(__mkArr_rename_at_604_234); das_move(__mkArr_rename_at_604_234, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_604) {
        das_copy((__mks_604.at /*at*/),(__at_rename_at_599_230));
        {
            clone_f1f421db208b5182(__context__,__mks_604.makeType /*makeType*/,__info_rename_at_599_229);
        }    }))));
    {
        bool __need_loop_605 = true;
        // x: int const
        das_iterator<range> __x_iterator(mk_range(__total_rename_at_601_232));
        int32_t __x_rename_at_605_236;
        __need_loop_605 = __x_iterator.first(__context__,(__x_rename_at_605_236)) && __need_loop_605;
        for ( ; __need_loop_605 ; __need_loop_605 = __x_iterator.next(__context__,(__x_rename_at_605_236)) )
        {
            __elem_rename_at_607_237; das_zero(__elem_rename_at_607_237); das_move(__elem_rename_at_607_237, walk_and_convert_52e7fa0c06656c3c(__context__,das_ptr_add_int32(__data_rename_at_599_228,__stride_rename_at_600_231 * __x_rename_at_605_236,1),__einfo_rename_at_602_233,__at_rename_at_599_230));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_604_234->values /*values*/),__elem_rename_at_607_237);
        }
        __x_iterator.close(__context__,(__x_rename_at_605_236));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_604_234);
}}

inline smart_ptr_raw<Expression> walk_and_convert_pointer_99fa96f2fad2fabb ( Context * __context__, uint8_t const  * const  __data_rename_at_614_238, smart_ptr_raw<TypeDecl> const  __info_rename_at_614_239, LineInfo const  &  __at_rename_at_614_240 )
{
    uint8_t const  * __pdata_rename_at_616_241 = ((uint8_t const  *)das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_614_238)));
    smart_ptr_raw<Expression> __elem_rename_at_617_242; das_zero(__elem_rename_at_617_242); das_move(__elem_rename_at_617_242, walk_and_convert_52e7fa0c06656c3c(__context__,__pdata_rename_at_616_241,__info_rename_at_614_239->firstType /*firstType*/,__at_rename_at_614_240));
    smart_ptr_raw<ExprAscend> __mkAsc_rename_at_618_243; das_zero(__mkAsc_rename_at_618_243); das_move(__mkAsc_rename_at_618_243, das_ascend_handle<true,smart_ptr_raw<ExprAscend>>::make(__context__,nullptr,(([&](ExprAscend & __mks_618) {
        das_copy((__mks_618.at /*at*/),(__at_rename_at_614_240));
        das_move((__mks_618.subexpr /*subexpr*/),(__elem_rename_at_617_242));
    }))));
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkAsc_rename_at_618_243);
}

inline smart_ptr_raw<Expression> walk_and_convert_tuple_d283511dc79b27c5 ( Context * __context__, uint8_t const  * const  __data_rename_at_623_244, smart_ptr_raw<TypeDecl> const  __info_rename_at_623_245, LineInfo const  &  __at_rename_at_623_246 )
{
    smart_ptr_raw<ExprMakeTuple> __mkArr_rename_at_624_247; memset(&__mkArr_rename_at_624_247,0,sizeof(__mkArr_rename_at_624_247));
    int32_t __offset_rename_at_626_249; memset(&__offset_rename_at_626_249,0,sizeof(__offset_rename_at_626_249));
    smart_ptr_raw<Expression> __elem_rename_at_628_250; memset(&__elem_rename_at_628_250,0,sizeof(__elem_rename_at_628_250));
    /* finally */ auto __finally_623= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkArr_rename_at_624_247);
    /* end finally */ });
    __mkArr_rename_at_624_247; das_zero(__mkArr_rename_at_624_247); das_move(__mkArr_rename_at_624_247, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_624) {
        das_copy((__mks_624.at /*at*/),(__at_rename_at_623_246));
        das_move((__mks_624.recordType /*recordType*/),(clone_type(__info_rename_at_623_245)));
    }))));
    {
        bool __need_loop_625 = true;
        // idx: int const
        das_iterator<range> __idx_iterator(mk_range(das_vector_length(__info_rename_at_623_245->argTypes /*argTypes*/)));
        int32_t __idx_rename_at_625_248;
        __need_loop_625 = __idx_iterator.first(__context__,(__idx_rename_at_625_248)) && __need_loop_625;
        for ( ; __need_loop_625 ; __need_loop_625 = __idx_iterator.next(__context__,(__idx_rename_at_625_248)) )
        {
            __offset_rename_at_626_249 = ((int32_t)get_tuple_field_offset(__info_rename_at_623_245,__idx_rename_at_625_248,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            __elem_rename_at_628_250; das_zero(__elem_rename_at_628_250); das_move(__elem_rename_at_628_250, walk_and_convert_52e7fa0c06656c3c(__context__,das_ptr_add_int32(__data_rename_at_623_244,__offset_rename_at_626_249,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_623_245->argTypes /*argTypes*/,__idx_rename_at_625_248,__context__),__at_rename_at_623_246));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_624_247->values /*values*/),__elem_rename_at_628_250);
        }
        __idx_iterator.close(__context__,(__idx_rename_at_625_248));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkArr_rename_at_624_247);
}

inline smart_ptr_raw<Expression> walk_and_convert_structure_461cae90387df9d8 ( Context * __context__, uint8_t const  * const  __data_rename_at_635_251, smart_ptr_raw<TypeDecl> const  __info_rename_at_635_252, LineInfo const  &  __at_rename_at_635_253 ) { das_stack_prologue __prologue(__context__,368,"walk_and_convert_structure " DAS_FILE_LINE);
{
    smart_ptr_raw<ExprMakeStruct> __mkStruct_rename_at_636_254; memset(&__mkStruct_rename_at_636_254,0,sizeof(__mkStruct_rename_at_636_254));
    Structure * __stype_rename_at_637_255; memset(&__stype_rename_at_637_255,0,sizeof(__stype_rename_at_637_255));
    smart_ptr_raw<MakeStruct> __mkS_rename_at_638_256; memset(&__mkS_rename_at_638_256,0,sizeof(__mkS_rename_at_638_256));
    uint8_t const  * __fdata_rename_at_642_258; memset(&__fdata_rename_at_642_258,0,sizeof(__fdata_rename_at_642_258));
    smart_ptr_raw<Expression> __elem_rename_at_643_259; memset(&__elem_rename_at_643_259,0,sizeof(__elem_rename_at_643_259));
    smart_ptr_raw<MakeFieldDecl> __mkF_rename_at_644_260; memset(&__mkF_rename_at_644_260,0,sizeof(__mkF_rename_at_644_260));
    /* finally */ auto __finally_635= das_finally([&](){
    das_delete_handle<smart_ptr_raw<MakeStruct>>::clear(__context__,__mkS_rename_at_638_256);
    das_delete_handle<smart_ptr_raw<ExprMakeStruct>>::clear(__context__,__mkStruct_rename_at_636_254);
    /* end finally */ });
    __mkStruct_rename_at_636_254; das_zero(__mkStruct_rename_at_636_254); das_move(__mkStruct_rename_at_636_254, das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_636) {
        das_copy((__mks_636.at /*at*/),(__at_rename_at_635_253));
        das_move((__mks_636.makeType /*makeType*/),(clone_type(__info_rename_at_635_252)));
    }))));
    __stype_rename_at_637_255 = ((Structure *)__info_rename_at_635_252->structType /*structType*/);
    __mkS_rename_at_638_256; das_zero(__mkS_rename_at_638_256); das_move(__mkS_rename_at_638_256, das_new_handle<MakeStruct,true>::make(__context__));
    {
        bool __need_loop_639 = true;
        // x: ast::FieldDeclaration const&
        das_iterator<das::vector<Structure::FieldDeclaration> const > __x_iterator(__stype_rename_at_637_255->fields /*fields*/);
        Structure::FieldDeclaration const  * __x_rename_at_639_257;
        __need_loop_639 = __x_iterator.first(__context__,(__x_rename_at_639_257)) && __need_loop_639;
        for ( ; __need_loop_639 ; __need_loop_639 = __x_iterator.next(__context__,(__x_rename_at_639_257)) )
        {
            TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> _temp_make_local_640_28_208; _temp_make_local_640_28_208;
            if ( das_get_variant_field<vec4f,16,8>::is((_temp_make_local_640_28_208 = (das_alias<RttiValue>::from(find_arg_a148d1ff83124bd1(__context__,(*__x_rename_at_639_257).annotation /*annotation*/,((char *) "do_not_convert")))))) )
            {
                __fdata_rename_at_642_258 = ((uint8_t const  *)das_ptr_add_int32(__data_rename_at_635_251,(*__x_rename_at_639_257).offset /*offset*/,1));
                __elem_rename_at_643_259; das_zero(__elem_rename_at_643_259); das_move(__elem_rename_at_643_259, walk_and_convert_52e7fa0c06656c3c(__context__,__fdata_rename_at_642_258,(*__x_rename_at_639_257).type /*_type*/,__at_rename_at_635_253));
                __mkF_rename_at_644_260; das_zero(__mkF_rename_at_644_260); das_move(__mkF_rename_at_644_260, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_644) {
                    das_copy((__mks_644.at /*at*/),(__at_rename_at_635_253));
                    das_move((__mks_644.value /*value*/),(__elem_rename_at_643_259));
                    {
                        das_clone<das::string,das::string>::clone(__mks_644.name /*name*/,(*__x_rename_at_639_257).name /*name*/);
                    }                }))));
                if ( !((das_deref(__context__,(*__x_rename_at_639_257).type /*_type*/)).canCopy()) )
                {
                    das_copy(__mkF_rename_at_644_260->flags /*flags*/,0x1u);
                };
                emplace_new_2dcb96a028e6e2a(__context__,das_arg<MakeStruct>::pass(das_deref(__context__,__mkS_rename_at_638_256)),__mkF_rename_at_644_260);
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_639_257));
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeStruct>>>::pass(__mkStruct_rename_at_636_254->structs /*structs*/),__mkS_rename_at_638_256);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkStruct_rename_at_636_254);
}}

inline smart_ptr_raw<Expression> walk_and_convert_variant_50ce6cc7d88f2c68 ( Context * __context__, uint8_t const  * const  __data_rename_at_656_262, smart_ptr_raw<TypeDecl> const  __info_rename_at_656_263, LineInfo const  &  __at_rename_at_656_264 ) { das_stack_prologue __prologue(__context__,208,"walk_and_convert_variant " DAS_FILE_LINE);
{
    int32_t __vindex_rename_at_658_265 = ((int32_t)das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_656_262)));
    int32_t __offset_rename_at_659_266 = ((int32_t)get_variant_field_offset(__info_rename_at_656_263,__vindex_rename_at_658_265,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    smart_ptr_raw<ExprMakeVariant> __mkVariant_rename_at_660_267; das_zero(__mkVariant_rename_at_660_267); das_move(__mkVariant_rename_at_660_267, das_ascend_handle<true,smart_ptr_raw<ExprMakeVariant>>::make(__context__,nullptr,(([&](ExprMakeVariant & __mks_660) {
        das_copy((__mks_660.at /*at*/),(__at_rename_at_656_264));
        das_move((__mks_660.makeType /*makeType*/),(clone_type(__info_rename_at_656_263)));
    }))));
    smart_ptr_raw<Expression> __elem_rename_at_661_268; das_zero(__elem_rename_at_661_268); das_move(__elem_rename_at_661_268, walk_and_convert_52e7fa0c06656c3c(__context__,das_ptr_add_int32(__data_rename_at_656_262,__offset_rename_at_659_266,1),das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_656_263->argTypes /*argTypes*/,__vindex_rename_at_658_265,__context__),__at_rename_at_656_264));
    smart_ptr_raw<MakeFieldDecl> __mkS_rename_at_662_269; das_zero(__mkS_rename_at_662_269); das_move(__mkS_rename_at_662_269, das_ascend_handle<true,smart_ptr_raw<MakeFieldDecl>>::make(__context__,nullptr,(([&](MakeFieldDecl & __mks_662) {
        das_copy((__mks_662.at /*at*/),(__at_rename_at_656_264));
        das_move((__mks_662.value /*value*/),(__elem_rename_at_661_268));
        {
            das_clone<das::string,das::string>::clone(__mks_662.name /*name*/,das_index<das::vector<das::string> const >::at(__info_rename_at_656_263->argNames /*argNames*/,__vindex_rename_at_658_265,__context__));
        }    }))));
    if ( !((das_deref(__context__,das_index<das::vector<smart_ptr<TypeDecl>> const >::at(__info_rename_at_656_263->argTypes /*argTypes*/,__vindex_rename_at_658_265,__context__))).canCopy()) )
    {
        das_copy(__mkS_rename_at_662_269->flags /*flags*/,0x1u);
    };
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<MakeFieldDecl>>>::pass(__mkVariant_rename_at_660_267->variants /*variants*/),__mkS_rename_at_662_269);
    return das_auto_cast<smart_ptr_raw<Expression>>::cast(__mkVariant_rename_at_660_267);
}}

inline smart_ptr_raw<Expression> walk_and_convert_table_578c212c6a418c09 ( Context * __context__, uint8_t const  * const  __data_rename_at_671_271, smart_ptr_raw<TypeDecl> const  __info_rename_at_671_272, LineInfo const  &  __at_rename_at_671_273 ) { das_stack_prologue __prologue(__context__,384,"walk_and_convert_table " DAS_FILE_LINE);
{
    smart_ptr_raw<TypeDecl> __tupT_rename_at_674_275; memset(&__tupT_rename_at_674_275,0,sizeof(__tupT_rename_at_674_275));
    smart_ptr_raw<ExprMakeArray> __mkArr_rename_at_677_276; memset(&__mkArr_rename_at_677_276,0,sizeof(__mkArr_rename_at_677_276));
    int32_t __key_stride_rename_at_678_277; memset(&__key_stride_rename_at_678_277,0,sizeof(__key_stride_rename_at_678_277));
    int32_t __value_stride_rename_at_679_278; memset(&__value_stride_rename_at_679_278,0,sizeof(__value_stride_rename_at_679_278));
    smart_ptr_raw<ExprCall> __mkToTableMove_rename_at_689_284; memset(&__mkToTableMove_rename_at_689_284,0,sizeof(__mkToTableMove_rename_at_689_284));
    int32_t __total_rename_at_672_274 = ((int32_t)any_table_size(das_auto_cast<void * const >::cast(__data_rename_at_671_271)));
    if ( __total_rename_at_672_274 != 0 )
    {
        /* finally */ auto __finally_673= das_finally([&](){
        das_delete_handle<smart_ptr_raw<ExprCall>>::clear(__context__,__mkToTableMove_rename_at_689_284);
        das_delete_handle<smart_ptr_raw<ExprMakeArray>>::clear(__context__,__mkArr_rename_at_677_276);
        das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__tupT_rename_at_674_275);
        /* end finally */ });
        __tupT_rename_at_674_275; das_zero(__tupT_rename_at_674_275); das_move(__tupT_rename_at_674_275, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_674) {
            das_copy((__mks_674.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tTuple));
            das_copy((__mks_674.at /*at*/),(__at_rename_at_671_273));
        }))));
        emplace_new_52cce80de15d9fe4(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_674_275->argTypes /*argTypes*/),clone_type(__info_rename_at_671_272->firstType /*firstType*/));
        emplace_new_52cce80de15d9fe4(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__tupT_rename_at_674_275->argTypes /*argTypes*/),clone_type(__info_rename_at_671_272->secondType /*secondType*/));
        __mkArr_rename_at_677_276; das_zero(__mkArr_rename_at_677_276); das_move(__mkArr_rename_at_677_276, das_ascend_handle<true,smart_ptr_raw<ExprMakeArray>>::make(__context__,nullptr,(([&](ExprMakeArray & __mks_677) {
            das_copy((__mks_677.at /*at*/),(__at_rename_at_671_273));
            das_move((__mks_677.makeType /*makeType*/),(clone_type(__tupT_rename_at_674_275)));
        }))));
        __key_stride_rename_at_678_277 = ((int32_t)((das_deref(__context__,__info_rename_at_671_272->firstType /*firstType*/)).getSizeOf()));
        __value_stride_rename_at_679_278 = ((int32_t)((das_deref(__context__,__info_rename_at_671_272->secondType /*secondType*/)).getSizeOf()));
        any_table_foreach(das_auto_cast<void * const >::cast(__data_rename_at_671_271),__key_stride_rename_at_678_277,__value_stride_rename_at_679_278,das_make_block<void,void *,void *>(__context__,192,0,&__func_info__b50c88059cb1bcc,[&](void * __pkey_rename_at_680_279, void * __pvalue_rename_at_680_280) -> void{
            smart_ptr_raw<Expression> __key_rename_at_681_281; memset(&__key_rename_at_681_281,0,sizeof(__key_rename_at_681_281));
            smart_ptr_raw<Expression> __value_rename_at_682_282; memset(&__value_rename_at_682_282,0,sizeof(__value_rename_at_682_282));
            smart_ptr_raw<ExprMakeTuple> __mkTup_rename_at_683_283; memset(&__mkTup_rename_at_683_283,0,sizeof(__mkTup_rename_at_683_283));
            /* finally */ auto __finally_680= das_finally([&](){
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__value_rename_at_682_282);
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__key_rename_at_681_281);
            das_delete_handle<smart_ptr_raw<ExprMakeTuple>>::clear(__context__,__mkTup_rename_at_683_283);
            /* end finally */ });
            __key_rename_at_681_281; das_zero(__key_rename_at_681_281); das_move(__key_rename_at_681_281, walk_and_convert_52e7fa0c06656c3c(__context__,das_auto_cast<uint8_t const  * const >::cast(__pkey_rename_at_680_279),__info_rename_at_671_272->firstType /*firstType*/,__at_rename_at_671_273));
            __value_rename_at_682_282; das_zero(__value_rename_at_682_282); das_move(__value_rename_at_682_282, walk_and_convert_52e7fa0c06656c3c(__context__,das_auto_cast<uint8_t const  * const >::cast(__pvalue_rename_at_680_280),__info_rename_at_671_272->secondType /*secondType*/,__at_rename_at_671_273));
            __mkTup_rename_at_683_283; das_zero(__mkTup_rename_at_683_283); das_move(__mkTup_rename_at_683_283, das_ascend_handle<true,smart_ptr_raw<ExprMakeTuple>>::make(__context__,nullptr,(([&](ExprMakeTuple & __mks_683) {
                das_copy((__mks_683.at /*at*/),(__at_rename_at_671_273));
                das_move((__mks_683.recordType /*recordType*/),(clone_type(__tupT_rename_at_674_275)));
                das_copy((__mks_683.isKeyValue /*isKeyValue*/),(true));
            }))));
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_683_283->values /*values*/),__key_rename_at_681_281);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkTup_rename_at_683_283->values /*values*/),__value_rename_at_682_282);
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkArr_rename_at_677_276->values /*values*/),das_reinterpret<smart_ptr<Expression>>::pass(__mkTup_rename_at_683_283));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_vector_push_back_value(das_arg<das::vector<int32_t>>::pass(__mkArr_rename_at_677_276->makeType /*makeType*/->dim /*dim*/),__total_rename_at_672_274);
        __mkToTableMove_rename_at_689_284; das_zero(__mkToTableMove_rename_at_689_284); das_move(__mkToTableMove_rename_at_689_284, das_ascend_handle<true,smart_ptr_raw<ExprCall>>::make(__context__,nullptr,(([&](ExprCall & __mks_689) {
            das_copy((__mks_689.at /*at*/),(__at_rename_at_671_273));
            {
                set_das_string(das_arg<das::string>::pass(__mks_689.name /*name*/),((char *) "to_table_move"));
            }        }))));
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__mkToTableMove_rename_at_689_284->arguments /*arguments*/),das_reinterpret<smart_ptr<Expression>>::pass(__mkArr_rename_at_677_276));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__mkToTableMove_rename_at_689_284);
    } else {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprMakeStruct>>::make(__context__,nullptr,(([&](ExprMakeStruct & __mks_693) {
            das_copy((__mks_693.at /*at*/),(__at_rename_at_671_273));
            das_move((__mks_693.makeType /*makeType*/),(clone_type(__info_rename_at_671_272)));
        }))));
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_basic_abd632a9adbc5f64 ( Context * __context__, uint8_t const  * const  __data_rename_at_697_286, smart_ptr_raw<TypeDecl> const  __info_rename_at_697_287, LineInfo const  &  __at_rename_at_697_288 ) { das_stack_prologue __prologue(__context__,304,"walk_and_convert_basic " DAS_FILE_LINE);
{
    if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt>>::make(__context__,nullptr,(([&](ExprConstInt & __mks_700) {
            das_copy((__mks_700.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_700.cvalue<int32_t>() /*value*/),(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt>>::make(__context__,nullptr,(([&](ExprConstUInt & __mks_702) {
            das_copy((__mks_702.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_702.cvalue<uint32_t>() /*value*/),(das_deref(__context__,das_cast<uint32_t *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstInt64>>::make(__context__,nullptr,(([&](ExprConstInt64 & __mks_704) {
            das_copy((__mks_704.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_704.cvalue<int64_t>() /*value*/),(das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tUInt64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstUInt64>>::make(__context__,nullptr,(([&](ExprConstUInt64 & __mks_706) {
            das_copy((__mks_706.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_706.cvalue<uint64_t>() /*value*/),(das_deref(__context__,das_cast<uint64_t *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstFloat>>::make(__context__,nullptr,(([&](ExprConstFloat & __mks_708) {
            das_copy((__mks_708.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_708.cvalue<float>() /*value*/),(das_deref(__context__,das_cast<float *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tDouble )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstDouble>>::make(__context__,nullptr,(([&](ExprConstDouble & __mks_710) {
            das_copy((__mks_710.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_710.cvalue<double>() /*value*/),(das_deref(__context__,das_cast<double *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBool>>::make(__context__,nullptr,(([&](ExprConstBool & __mks_712) {
            das_copy((__mks_712.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_712.cvalue<bool>() /*value*/),(das_deref(__context__,das_cast<bool *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange>>::make(__context__,nullptr,(([&](ExprConstRange & __mks_714) {
            das_copy((__mks_714.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_714.cvalue<range>() /*value*/),(das_deref(__context__,das_cast<range *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange>>::make(__context__,nullptr,(([&](ExprConstURange & __mks_716) {
            das_copy((__mks_716.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_716.cvalue<urange>() /*value*/),(das_deref(__context__,das_cast<urange *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tRange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstRange64>>::make(__context__,nullptr,(([&](ExprConstRange64 & __mks_718) {
            das_copy((__mks_718.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_718.cvalue<range64>() /*value*/),(das_deref(__context__,das_cast<range64 *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tURange64 )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstURange64>>::make(__context__,nullptr,(([&](ExprConstURange64 & __mks_720) {
            das_copy((__mks_720.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_720.cvalue<urange64>() /*value*/),(das_deref(__context__,das_cast<urange64 *>::cast(__data_rename_at_697_286))));
        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstString>>::make(__context__,nullptr,(([&](ExprConstString & __mks_722) {
            das_copy((__mks_722.at /*at*/),(__at_rename_at_697_288));
            {
                set_das_string(das_arg<das::string>::pass(__mks_722.text /*value*/),das_deref(__context__,das_cast<char * *>::cast(__data_rename_at_697_286)));
            }        }))));
    } else if ( __info_rename_at_697_287->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield )
    {
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstBitfield>>::make(__context__,nullptr,(([&](ExprConstBitfield & __mks_724) {
            das_copy((__mks_724.at /*at*/),(__at_rename_at_697_288));
            das_copy((__mks_724.cvalue<Bitfield>() /*value*/),(das_deref(__context__,das_cast<Bitfield *>::cast(__data_rename_at_697_286))));
        }))));
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<char * const >::from(((char *) "unsupported ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__info_rename_at_697_287->baseType /*baseType*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<smart_ptr_raw<Expression>>::cast(nullptr);
    };
}}

inline smart_ptr_raw<Expression> walk_and_convert_enumeration_19e84f10944d8e34 ( Context * __context__, uint8_t const  * const  __data_rename_at_732_290, smart_ptr_raw<TypeDecl> const  __info_rename_at_732_291, LineInfo const  &  __at_rename_at_732_292 ) { das_stack_prologue __prologue(__context__,144,"walk_and_convert_enumeration " DAS_FILE_LINE);
{
    int64_t __eval_rename_at_733_293 = INT64_C(0);
    if ( __info_rename_at_732_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration )
    {
        das_copy(__eval_rename_at_733_293,int64_t(das_deref(__context__,das_cast<int32_t *>::cast(__data_rename_at_732_290))));
    } else if ( __info_rename_at_732_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8 )
    {
        das_copy(__eval_rename_at_733_293,int64_t(das_deref(__context__,__data_rename_at_732_290)));
    } else if ( __info_rename_at_732_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16 )
    {
        das_copy(__eval_rename_at_733_293,int64_t(das_deref(__context__,das_cast<int16_t *>::cast(__data_rename_at_732_290))));
    } else if ( __info_rename_at_732_291->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64 )
    {
        das_copy(__eval_rename_at_733_293,das_deref(__context__,das_cast<int64_t *>::cast(__data_rename_at_732_290)));
    } else {
        builtin_throw(((char *) "unsupported enumeration"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    char * __name_rename_at_745_294 = ((char *)(char *)(((char * const )(ast_find_enum_name(__info_rename_at_732_291->enumType /*enumType*/,__eval_rename_at_733_293,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_745_294),cast<char *>::from(nullptr),*__context__,nullptr) )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_14, cast<char * const >::from(((char *) "can't find name of enum value ")), cast<int64_t>::from(__eval_rename_at_733_293), cast<char * const >::from(((char *) " in ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__info_rename_at_732_291,true,true,true)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstEnumeration>>::make(__context__,nullptr,(([&](ExprConstEnumeration & __mks_747) {
        {
            set_das_string(das_arg<das::string>::pass(__mks_747.text /*value*/),__name_rename_at_745_294);
            clone_9ab0f6a4e0a6006e(__context__,__mks_747.enumType /*enumType*/,das_cast<Enumeration *>::cast(__info_rename_at_732_291->enumType /*enumType*/));
        }    }))));
}}

inline smart_ptr_raw<Expression> walk_and_convert_52e7fa0c06656c3c ( Context * __context__, uint8_t const  * const  __data_rename_at_752_296, smart_ptr_raw<TypeDecl> const  __info_rename_at_752_297, LineInfo const  &  __at_rename_at_752_298 )
{
    if ( das_vector_length(__info_rename_at_752_297->dim /*dim*/) != 0 )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_dim_307e83e2e898f055(__context__,__data_rename_at_752_296,__info_rename_at_752_297,__at_rename_at_752_298));
    } else if ( __info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tArray )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_array_fc0c75b20fd9b5d8(__context__,__data_rename_at_752_296,__info_rename_at_752_297,__at_rename_at_752_298));
    } else if ( __info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tPointer )
    {
        if ( das_deref(__context__,das_cast<uint8_t const  * *>::cast(__data_rename_at_752_296)) == nullptr )
        {
            return das_auto_cast<smart_ptr_raw<Expression>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_761) {
                das_copy((__mks_761.at /*at*/),(__at_rename_at_752_298));
            }))));
        } else return das_auto_cast<smart_ptr_raw<Expression>>::cast(((equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__info_rename_at_752_297->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr))) || ((das_deref(__context__,__info_rename_at_752_297->firstType /*firstType*/)).isVoid())) ? das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(das_ascend_handle<true,smart_ptr_raw<ExprConstPtr>>::make(__context__,nullptr,(([&](ExprConstPtr & __mks_763) {
            das_copy((__mks_763.at /*at*/),(__at_rename_at_752_298));
        })))) : das_auto_cast<smart_ptr_raw<ExprConstPtr>>::cast(walk_and_convert_pointer_99fa96f2fad2fabb(__context__,__data_rename_at_752_296,__info_rename_at_752_297,__at_rename_at_752_298)));
    } else if ( __info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tStructure )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_structure_461cae90387df9d8(__context__,__data_rename_at_752_296,__info_rename_at_752_297,__at_rename_at_752_298));
    } else if ( __info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tVariant )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_variant_50ce6cc7d88f2c68(__context__,__data_rename_at_752_296,__info_rename_at_752_297,__at_rename_at_752_298));
    } else if ( __info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTuple )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_tuple_d283511dc79b27c5(__context__,__data_rename_at_752_296,__info_rename_at_752_297,__at_rename_at_752_298));
    } else if ( __info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTable )
    {
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(walk_and_convert_table_578c212c6a418c09(__context__,__data_rename_at_752_296,__info_rename_at_752_297,__at_rename_at_752_298));
    } else return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(((((__info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration8) || (__info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration16)) || (__info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration64)) || (__info_rename_at_752_297->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tEnumeration)) ? das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_enumeration_19e84f10944d8e34(__context__,__data_rename_at_752_296,__info_rename_at_752_297,__at_rename_at_752_298)) : das_auto_cast<smart_ptr_raw<Expression>>::cast(walk_and_convert_basic_abd632a9adbc5f64(__context__,__data_rename_at_752_296,__info_rename_at_752_297,__at_rename_at_752_298)));
}

inline Annotation const  * find_annotation_3783ce606e338529 ( Context * __context__, char * const  __mod_name_rename_at_807_299, char * const  __ann_name_rename_at_807_300 ) { das_stack_prologue __prologue(__context__,128,"find_annotation " DAS_FILE_LINE);
{
    Module * __mod_rename_at_808_301 = _FuncastTickfind_compiling_moduleTick4523452840392654583_34f86625d1f8ecb2(__context__,__mod_name_rename_at_807_299);
    if ( __mod_rename_at_808_301 == nullptr )
    {
        return das_auto_cast<Annotation const  *>::cast(nullptr);
    } else {
        Annotation const  * __ann_rename_at_812_302 = 0;
        rtti_builtin_module_for_each_annotation(__mod_rename_at_808_301,das_make_block<void,Annotation const  &>(__context__,112,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_813_303) -> void{
            if ( eq_dstr_str(__value_rename_at_813_303.name /*name*/,__ann_name_rename_at_807_300) )
            {
                das_copy(__ann_rename_at_812_302,das_ref(__context__,__value_rename_at_813_303));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<Annotation const  *>::cast(__ann_rename_at_812_302);
    };
}}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_518bf407af95eedd ( Context * __context__, char * const  __mod_name_rename_at_823_304, char * const  __ann_name_rename_at_823_305, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_823_306 )
{
    Annotation const  * __ann_rename_at_824_307; memset(&__ann_rename_at_824_307,0,sizeof(__ann_rename_at_824_307));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_826_308; memset(&__decl_rename_at_826_308,0,sizeof(__decl_rename_at_826_308));
    char * * __argName_rename_at_829_310; memset(&__argName_rename_at_829_310,0,sizeof(__argName_rename_at_829_310));
    TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> * __arg_rename_at_830_311; memset(&__arg_rename_at_830_311,0,sizeof(__arg_rename_at_830_311));
    /* finally */ auto __finally_823= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_826_308);
    /* end finally */ });
    __ann_rename_at_824_307 = find_annotation_3783ce606e338529(__context__,__mod_name_rename_at_823_304,__ann_name_rename_at_823_305);
    DAS_ASSERT((__ann_rename_at_824_307 != nullptr));
    __decl_rename_at_826_308; das_zero(__decl_rename_at_826_308); das_move(__decl_rename_at_826_308, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_3a3075a3a0c0cb06(__context__,__decl_rename_at_826_308->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_824_307));
    {
        bool __need_loop_828 = true;
        // argP: tuple<argname:string;argvalue:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any> aka RttiValue> const&
        das_iterator<TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const > __argP_iterator(__args_rename_at_823_306);
        TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>> const  * __argP_rename_at_828_309;
        __need_loop_828 = __argP_iterator.first(__context__,(__argP_rename_at_828_309)) && __need_loop_828;
        for ( ; __need_loop_828 ; __need_loop_828 = __argP_iterator.next(__context__,(__argP_rename_at_828_309)) )
        {
            __argName_rename_at_829_310 = ((char * *)&((char * &)(das_get_tuple_field<char *,0>::get((*__argP_rename_at_828_309)))));
            __arg_rename_at_830_311 = ((TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> *)&(das_alias<RttiValue>::from(das_get_tuple_field<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,16>::get((*__argP_rename_at_828_309)))));
            if ( das_get_variant_field<bool,16,0>::is((*__arg_rename_at_830_311)) )
            {
                add_annotation_argument_aecf50176a737617(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_826_308->arguments /*arguments*/),(*__argName_rename_at_829_310),das_get_variant_field<bool,16,0>::as((*__arg_rename_at_830_311),__context__));
            } else if ( das_get_variant_field<int32_t,16,1>::is((*__arg_rename_at_830_311)) )
            {
                add_annotation_argument_584d51af09b7a892(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_826_308->arguments /*arguments*/),(*__argName_rename_at_829_310),das_get_variant_field<int32_t,16,1>::as((*__arg_rename_at_830_311),__context__));
            } else if ( das_get_variant_field<float,16,5>::is((*__arg_rename_at_830_311)) )
            {
                add_annotation_argument_2991fd3097c426a8(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_826_308->arguments /*arguments*/),(*__argName_rename_at_829_310),das_get_variant_field<float,16,5>::as((*__arg_rename_at_830_311),__context__));
            } else if ( das_get_variant_field<char *,16,7>::is((*__arg_rename_at_830_311)) )
            {
                add_annotation_argument_62c7cc55608504df(__context__,das_arg<AnnotationArgumentList>::pass(__decl_rename_at_826_308->arguments /*arguments*/),(*__argName_rename_at_829_310),das_get_variant_field<char *,16,7>::as((*__arg_rename_at_830_311),__context__));
            } else {
                builtin_throw(((char *) "Invalid rtti value"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __argP_iterator.close(__context__,(__argP_rename_at_828_309));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_826_308);
}

inline smart_ptr_raw<AnnotationDeclaration> append_annotation_304ed986ed5830b8 ( Context * __context__, char * const  __mod_name_rename_at_846_312, char * const  __ann_name_rename_at_846_313 )
{
    Annotation const  * __ann_rename_at_847_314; memset(&__ann_rename_at_847_314,0,sizeof(__ann_rename_at_847_314));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_849_315; memset(&__decl_rename_at_849_315,0,sizeof(__decl_rename_at_849_315));
    /* finally */ auto __finally_846= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_849_315);
    /* end finally */ });
    __ann_rename_at_847_314 = find_annotation_3783ce606e338529(__context__,__mod_name_rename_at_846_312,__ann_name_rename_at_846_313);
    DAS_ASSERT((__ann_rename_at_847_314 != nullptr));
    __decl_rename_at_849_315; das_zero(__decl_rename_at_849_315); das_move(__decl_rename_at_849_315, das_new_handle<AnnotationDeclaration,true>::make(__context__));
    clone_3a3075a3a0c0cb06(__context__,__decl_rename_at_849_315->annotation /*annotation*/,das_cast<smart_ptr_raw<Annotation>>::cast(__ann_rename_at_847_314));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<AnnotationDeclaration>>::cast(__decl_rename_at_849_315);
}

inline void append_annotation_a089579603ac9094 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_854_316, char * const  __mod_name_rename_at_854_317, char * const  __ann_name_rename_at_854_318 )
{
    Annotation const  * __ann_rename_at_855_319; memset(&__ann_rename_at_855_319,0,sizeof(__ann_rename_at_855_319));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_857_320; memset(&__decl_rename_at_857_320,0,sizeof(__decl_rename_at_857_320));
    /* finally */ auto __finally_854= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_857_320);
    /* end finally */ });
    __ann_rename_at_855_319 = find_annotation_3783ce606e338529(__context__,__mod_name_rename_at_854_317,__ann_name_rename_at_854_318);
    DAS_ASSERT((__ann_rename_at_855_319 != nullptr));
    __decl_rename_at_857_320; das_zero(__decl_rename_at_857_320); das_move(__decl_rename_at_857_320, append_annotation_304ed986ed5830b8(__context__,__mod_name_rename_at_854_317,__ann_name_rename_at_854_318));
    addAndApplyFunctionAnnotation(__func_rename_at_854_316,__decl_rename_at_857_320,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_df67cdd1c7fda936 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_861_321, char * const  __mod_name_rename_at_861_322, char * const  __ann_name_rename_at_861_323 )
{
    Annotation const  * __ann_rename_at_862_324; memset(&__ann_rename_at_862_324,0,sizeof(__ann_rename_at_862_324));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_864_325; memset(&__decl_rename_at_864_325,0,sizeof(__decl_rename_at_864_325));
    /* finally */ auto __finally_861= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_864_325);
    /* end finally */ });
    __ann_rename_at_862_324 = find_annotation_3783ce606e338529(__context__,__mod_name_rename_at_861_322,__ann_name_rename_at_861_323);
    DAS_ASSERT((__ann_rename_at_862_324 != nullptr));
    __decl_rename_at_864_325; das_zero(__decl_rename_at_864_325); das_move(__decl_rename_at_864_325, append_annotation_304ed986ed5830b8(__context__,__mod_name_rename_at_861_322,__ann_name_rename_at_861_323));
    addAndApplyBlockAnnotation(__blk_rename_at_861_321,__decl_rename_at_864_325,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_33c7fba9141ce1dc ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_868_326, char * const  __mod_name_rename_at_868_327, char * const  __ann_name_rename_at_868_328 )
{
    Annotation const  * __ann_rename_at_869_329; memset(&__ann_rename_at_869_329,0,sizeof(__ann_rename_at_869_329));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_871_330; memset(&__decl_rename_at_871_330,0,sizeof(__decl_rename_at_871_330));
    /* finally */ auto __finally_868= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_871_330);
    /* end finally */ });
    __ann_rename_at_869_329 = find_annotation_3783ce606e338529(__context__,__mod_name_rename_at_868_327,__ann_name_rename_at_868_328);
    DAS_ASSERT((__ann_rename_at_869_329 != nullptr));
    __decl_rename_at_871_330; das_zero(__decl_rename_at_871_330); das_move(__decl_rename_at_871_330, append_annotation_304ed986ed5830b8(__context__,__mod_name_rename_at_868_327,__ann_name_rename_at_868_328));
    addAndApplyStructAnnotation(__st_rename_at_868_326,__decl_rename_at_871_330,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_50d585b969d10a85 ( Context * __context__, smart_ptr_raw<Function> __func_rename_at_875_331, char * const  __mod_name_rename_at_875_332, char * const  __ann_name_rename_at_875_333, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_875_334 )
{
    Annotation const  * __ann_rename_at_876_335; memset(&__ann_rename_at_876_335,0,sizeof(__ann_rename_at_876_335));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_878_336; memset(&__decl_rename_at_878_336,0,sizeof(__decl_rename_at_878_336));
    /* finally */ auto __finally_875= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_878_336);
    /* end finally */ });
    __ann_rename_at_876_335 = find_annotation_3783ce606e338529(__context__,__mod_name_rename_at_875_332,__ann_name_rename_at_875_333);
    DAS_ASSERT((__ann_rename_at_876_335 != nullptr));
    __decl_rename_at_878_336; das_zero(__decl_rename_at_878_336); das_move(__decl_rename_at_878_336, append_annotation_518bf407af95eedd(__context__,__mod_name_rename_at_875_332,__ann_name_rename_at_875_333,das_reinterpret<TArray<TTuple<48,char *,RttiValue>> const >::pass(__args_rename_at_875_334)));
    addAndApplyFunctionAnnotation(__func_rename_at_875_331,__decl_rename_at_878_336,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_4809cc819e2c2e25 ( Context * __context__, smart_ptr_raw<ExprBlock> __blk_rename_at_882_337, char * const  __mod_name_rename_at_882_338, char * const  __ann_name_rename_at_882_339, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_882_340 )
{
    Annotation const  * __ann_rename_at_883_341; memset(&__ann_rename_at_883_341,0,sizeof(__ann_rename_at_883_341));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_885_342; memset(&__decl_rename_at_885_342,0,sizeof(__decl_rename_at_885_342));
    /* finally */ auto __finally_882= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_885_342);
    /* end finally */ });
    __ann_rename_at_883_341 = find_annotation_3783ce606e338529(__context__,__mod_name_rename_at_882_338,__ann_name_rename_at_882_339);
    DAS_ASSERT((__ann_rename_at_883_341 != nullptr));
    __decl_rename_at_885_342; das_zero(__decl_rename_at_885_342); das_move(__decl_rename_at_885_342, append_annotation_518bf407af95eedd(__context__,__mod_name_rename_at_882_338,__ann_name_rename_at_882_339,das_reinterpret<TArray<TTuple<48,char *,RttiValue>> const >::pass(__args_rename_at_882_340)));
    addAndApplyBlockAnnotation(__blk_rename_at_882_337,__decl_rename_at_885_342,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void append_annotation_c82c20ac1475e6cb ( Context * __context__, smart_ptr_raw<Structure> __st_rename_at_889_343, char * const  __mod_name_rename_at_889_344, char * const  __ann_name_rename_at_889_345, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __args_rename_at_889_346 )
{
    Annotation const  * __ann_rename_at_890_347; memset(&__ann_rename_at_890_347,0,sizeof(__ann_rename_at_890_347));
    smart_ptr_raw<AnnotationDeclaration> __decl_rename_at_892_348; memset(&__decl_rename_at_892_348,0,sizeof(__decl_rename_at_892_348));
    /* finally */ auto __finally_889= das_finally([&](){
    das_delete_handle<smart_ptr_raw<AnnotationDeclaration>>::clear(__context__,__decl_rename_at_892_348);
    /* end finally */ });
    __ann_rename_at_890_347 = find_annotation_3783ce606e338529(__context__,__mod_name_rename_at_889_344,__ann_name_rename_at_889_345);
    DAS_ASSERT((__ann_rename_at_890_347 != nullptr));
    __decl_rename_at_892_348; das_zero(__decl_rename_at_892_348); das_move(__decl_rename_at_892_348, append_annotation_518bf407af95eedd(__context__,__mod_name_rename_at_889_344,__ann_name_rename_at_889_345,das_reinterpret<TArray<TTuple<48,char *,RttiValue>> const >::pass(__args_rename_at_889_346)));
    addAndApplyStructAnnotation(__st_rename_at_889_343,__decl_rename_at_892_348,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t add_annotation_argument_aecf50176a737617 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_896_349, char * const  __argName_rename_at_896_350, bool __val_rename_at_896_351 )
{
    int32_t __argIdx_rename_at_897_352 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_896_349),__argName_rename_at_896_350));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_896_349,__argIdx_rename_at_897_352,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tBool);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_896_349,__argIdx_rename_at_897_352,__context__).bValue /*bValue*/,__val_rename_at_896_351);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_897_352);
}

inline int32_t add_annotation_argument_584d51af09b7a892 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_903_353, char * const  __argName_rename_at_903_354, int32_t __val_rename_at_903_355 )
{
    int32_t __argIdx_rename_at_904_356 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_903_353),__argName_rename_at_903_354));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_903_353,__argIdx_rename_at_904_356,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tInt);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_903_353,__argIdx_rename_at_904_356,__context__).iValue /*iValue*/,__val_rename_at_903_355);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_904_356);
}

inline int32_t add_annotation_argument_2991fd3097c426a8 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_910_357, char * const  __argName_rename_at_910_358, float __val_rename_at_910_359 )
{
    int32_t __argIdx_rename_at_911_360 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_910_357),__argName_rename_at_910_358));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_910_357,__argIdx_rename_at_911_360,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tFloat);
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_910_357,__argIdx_rename_at_911_360,__context__).fValue /*fValue*/,__val_rename_at_910_359);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_911_360);
}

inline int32_t add_annotation_argument_62c7cc55608504df ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_917_361, char * const  __argName_rename_at_917_362, char * const  __val_rename_at_917_363 )
{
    int32_t __argIdx_rename_at_918_364 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_917_361),__argName_rename_at_917_362));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_917_361,__argIdx_rename_at_918_364,__context__).type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tString);
    set_das_string(das_arg<das::string>::pass(das_index<AnnotationArgumentList>::at(__arguments_rename_at_917_361,__argIdx_rename_at_918_364,__context__).sValue /*sValue*/),__val_rename_at_917_363);
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_918_364);
}

inline int32_t add_annotation_argument_806b0c29e45217e2 ( Context * __context__, AnnotationArgumentList &  __arguments_rename_at_924_365, AnnotationArgument const  &  __ann_rename_at_924_366 )
{
    int32_t __argIdx_rename_at_925_367 = ((int32_t)rtti_add_annotation_argument(das_arg<AnnotationArgumentList>::pass(__arguments_rename_at_924_365),((char * const )(to_das_string(__ann_rename_at_924_366.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_924_365,__argIdx_rename_at_925_367,__context__).type /*basicType*/,__ann_rename_at_924_366.type /*basicType*/);
    if ( __ann_rename_at_924_366.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tBool )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_924_365,__argIdx_rename_at_925_367,__context__).bValue /*bValue*/,__ann_rename_at_924_366.bValue /*bValue*/);
    } else if ( __ann_rename_at_924_366.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tInt )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_924_365,__argIdx_rename_at_925_367,__context__).iValue /*iValue*/,__ann_rename_at_924_366.iValue /*iValue*/);
    } else if ( __ann_rename_at_924_366.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tFloat )
    {
        das_copy(das_index<AnnotationArgumentList>::at(__arguments_rename_at_924_365,__argIdx_rename_at_925_367,__context__).fValue /*fValue*/,__ann_rename_at_924_366.fValue /*fValue*/);
    } else if ( __ann_rename_at_924_366.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        das_clone<das::string,das::string>::clone(das_index<AnnotationArgumentList>::at(__arguments_rename_at_924_365,__argIdx_rename_at_925_367,__context__).sValue /*sValue*/,__ann_rename_at_924_366.sValue /*sValue*/);
    } else {
        DAS_ASSERTF((false),(((char *) "unsupported annotation type, add more types")));
    };
    return das_auto_cast<int32_t>::cast(__argIdx_rename_at_925_367);
}

inline int32_t get_for_source_index_9bcd632a50af36c1 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_941_368, smart_ptr_raw<Variable> const  __svar_rename_at_941_369 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_942_41_32; _temp_make_local_942_41_32;
    {
        bool __need_loop_942 = true;
        // v: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __v_iterator(__expr_rename_at_941_368->iteratorVariables /*iteratorVariables*/);
        smart_ptr<Variable> const  * __v_rename_at_942_372;
        __need_loop_942 = __v_iterator.first(__context__,(__v_rename_at_942_372)) && __need_loop_942;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_942_41_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_942_373;
        __need_loop_942 = __t_iterator.first(__context__,(__t_rename_at_942_373)) && __need_loop_942;
        for ( ; __need_loop_942 ; __need_loop_942 = __v_iterator.next(__context__,(__v_rename_at_942_372)) && __t_iterator.next(__context__,(__t_rename_at_942_373)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__v_rename_at_942_372)),das_auto_cast<smart_ptr_raw<void> const >::cast(__svar_rename_at_941_369)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_942_373);
            };
        }
        __v_iterator.close(__context__,(__v_rename_at_942_372));
        __t_iterator.close(__context__,(__t_rename_at_942_373));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline int32_t get_for_source_index_2b39b717f35b48e5 ( Context * __context__, smart_ptr_raw<ExprFor> const  __expr_rename_at_950_374, smart_ptr_raw<Expression> const  __source_rename_at_950_375 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_951_31_32; _temp_make_local_951_31_32;
    {
        bool __need_loop_951 = true;
        // s: smart_ptr<ast::Expression> const&
        das_iterator<das::vector<smart_ptr<Expression>> const > __s_iterator(__expr_rename_at_950_374->sources /*sources*/);
        smart_ptr<Expression> const  * __s_rename_at_951_378;
        __need_loop_951 = __s_iterator.first(__context__,(__s_rename_at_951_378)) && __need_loop_951;
        // t: int
        das_iterator_count DAS_COMMENT((_temp_make_local_951_31_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __t_iterator(0,1);
        int32_t __t_rename_at_951_379;
        __need_loop_951 = __t_iterator.first(__context__,(__t_rename_at_951_379)) && __need_loop_951;
        for ( ; __need_loop_951 ; __need_loop_951 = __s_iterator.next(__context__,(__s_rename_at_951_378)) && __t_iterator.next(__context__,(__t_rename_at_951_379)) )
        {
            if ( equ_sptr_sptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__s_rename_at_951_378)),das_auto_cast<smart_ptr_raw<void> const >::cast(__source_rename_at_950_375)) )
            {
                return das_auto_cast<int32_t>::cast(__t_rename_at_951_379);
            };
        }
        __s_iterator.close(__context__,(__s_rename_at_951_378));
        __t_iterator.close(__context__,(__t_rename_at_951_379));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline smart_ptr_raw<TypeDecl> function_to_type_84839d9adeaf04f3 ( Context * __context__, smart_ptr_raw<Function> const  __fn_rename_at_998_380 )
{
    smart_ptr_raw<TypeDecl> __td_rename_at_999_381; memset(&__td_rename_at_999_381,0,sizeof(__td_rename_at_999_381));
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1003_36_80; _temp_make_local_1003_36_80;
    /* finally */ auto __finally_998= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__td_rename_at_999_381);
    /* end finally */ });
    __td_rename_at_999_381; das_zero(__td_rename_at_999_381); das_move(__td_rename_at_999_381, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_999) {
        das_copy((__mks_999.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tFunction));
        das_move((__mks_999.firstType /*firstType*/),(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fn_rename_at_998_380->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<smart_ptr<TypeDecl>>::cast(clone_type(__fn_rename_at_998_380->result /*result*/)) : das_auto_cast<smart_ptr<TypeDecl>>::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_1000) {
            das_copy((__mks_1000.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::autoinfer));
        })))))));
    }))));
    das_vector_resize(das_arg<das::vector<das::string>>::pass(__td_rename_at_999_381->argNames /*argNames*/),das_vector_length(__fn_rename_at_998_380->arguments /*arguments*/));
    {
        bool __need_loop_1003 = true;
        // arg: smart_ptr<ast::Variable> const&
        das_iterator<das::vector<smart_ptr<Variable>> const > __arg_iterator(__fn_rename_at_998_380->arguments /*arguments*/);
        smart_ptr<Variable> const  * __arg_rename_at_1003_384;
        __need_loop_1003 = __arg_iterator.first(__context__,(__arg_rename_at_1003_384)) && __need_loop_1003;
        // argi: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1003_36_80 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __argi_iterator(0,1);
        int32_t __argi_rename_at_1003_385;
        __need_loop_1003 = __argi_iterator.first(__context__,(__argi_rename_at_1003_385)) && __need_loop_1003;
        for ( ; __need_loop_1003 ; __need_loop_1003 = __arg_iterator.next(__context__,(__arg_rename_at_1003_384)) && __argi_iterator.next(__context__,(__argi_rename_at_1003_385)) )
        {
            emplace_new_52cce80de15d9fe4(__context__,das_arg<das::vector<smart_ptr<TypeDecl>>>::pass(__td_rename_at_999_381->argTypes /*argTypes*/),clone_type((*__arg_rename_at_1003_384)->type /*_type*/));
            das_clone<das::string,das::string>::clone(das_index<das::vector<das::string>>::at(__td_rename_at_999_381->argNames /*argNames*/,__argi_rename_at_1003_385,__context__),(*__arg_rename_at_1003_384)->name /*name*/);
        }
        __arg_iterator.close(__context__,(__arg_rename_at_1003_384));
        __argi_iterator.close(__context__,(__argi_rename_at_1003_385));
    };
    return /* <- */ das_auto_cast_move<smart_ptr_raw<TypeDecl>>::cast(__td_rename_at_999_381);
}

inline void visit_finally_27770f49825d1a9b ( Context * __context__, ExprBlock * const  __blk_rename_at_1010_386, smart_ptr_raw<VisitorAdapter> const  __adapter_rename_at_1010_387 )
{
    astVisitBlockFinally(das_cast<smart_ptr_raw<ExprBlock>>::cast(__blk_rename_at_1010_386),__adapter_rename_at_1010_387,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool isCMRES_ecc22ba10553fb74 ( Context * __context__, smart_ptr_raw<Function> const  __fun_rename_at_1016_388 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1016_388->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1016_388->flags /*flags*/,1u << 6));
}

inline bool isCMRES_c750d4ae5480733b ( Context * __context__, Function * const  __fun_rename_at_1020_389 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__fun_rename_at_1020_389->flags /*flags*/,1u << 5) || das_get_bitfield(__fun_rename_at_1020_389->flags /*flags*/,1u << 6));
}

inline bool isMakeLocal_18c81e2ae52a95c7 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1024_390 )
{
    return das_auto_cast<bool>::cast(((((SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1024_390->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeLocal")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1024_390->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeStruct")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1024_390->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeVariant")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1024_390->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeArray")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_1024_390->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMakeTuple")),*__context__,nullptr)));
}

inline TDim<DAS_COMMENT(bound_enum) das::Type,31> get_workhorse_types_e61ceb837ac4d4c ( Context * __context__ )
{
    return das_auto_cast_ref<TDim<DAS_COMMENT(bound_enum) das::Type,31>>::cast((([&]() -> TDim<DAS_COMMENT(bound_enum) das::Type,31> {
        TDim<DAS_COMMENT(bound_enum) das::Type,31> __mka_1033;
        __mka_1033(0,__context__) = DAS_COMMENT(bound_enum) das::Type::tBool;
        __mka_1033(1,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt8;
        __mka_1033(2,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt8;
        __mka_1033(3,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt16;
        __mka_1033(4,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt16;
        __mka_1033(5,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt64;
        __mka_1033(6,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt64;
        __mka_1033(7,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration;
        __mka_1033(8,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration8;
        __mka_1033(9,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration16;
        __mka_1033(10,__context__) = DAS_COMMENT(bound_enum) das::Type::tEnumeration64;
        __mka_1033(11,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt;
        __mka_1033(12,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt2;
        __mka_1033(13,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt3;
        __mka_1033(14,__context__) = DAS_COMMENT(bound_enum) das::Type::tInt4;
        __mka_1033(15,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt;
        __mka_1033(16,__context__) = DAS_COMMENT(bound_enum) das::Type::tBitfield;
        __mka_1033(17,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt2;
        __mka_1033(18,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt3;
        __mka_1033(19,__context__) = DAS_COMMENT(bound_enum) das::Type::tUInt4;
        __mka_1033(20,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat;
        __mka_1033(21,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat2;
        __mka_1033(22,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat3;
        __mka_1033(23,__context__) = DAS_COMMENT(bound_enum) das::Type::tFloat4;
        __mka_1033(24,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange;
        __mka_1033(25,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange;
        __mka_1033(26,__context__) = DAS_COMMENT(bound_enum) das::Type::tRange64;
        __mka_1033(27,__context__) = DAS_COMMENT(bound_enum) das::Type::tURange64;
        __mka_1033(28,__context__) = DAS_COMMENT(bound_enum) das::Type::tString;
        __mka_1033(29,__context__) = DAS_COMMENT(bound_enum) das::Type::tDouble;
        __mka_1033(30,__context__) = DAS_COMMENT(bound_enum) das::Type::tPointer;
        return __mka_1033;
    })()));
}

inline int32_t find_argument_index_ddbc5417768753b7 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __typ_rename_at_1045_391, char * const  __name_rename_at_1045_392 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1046_32_32; _temp_make_local_1046_32_32;
    {
        bool __need_loop_1046 = true;
        // a: $::das_string const&
        das_iterator<das::vector<das::string> const > __a_iterator(__typ_rename_at_1045_391->argNames /*argNames*/);
        das::string const  * __a_rename_at_1046_395;
        __need_loop_1046 = __a_iterator.first(__context__,(__a_rename_at_1046_395)) && __need_loop_1046;
        // ai: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1046_32_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __ai_iterator(0,1);
        int32_t __ai_rename_at_1046_396;
        __need_loop_1046 = __ai_iterator.first(__context__,(__ai_rename_at_1046_396)) && __need_loop_1046;
        for ( ; __need_loop_1046 ; __need_loop_1046 = __a_iterator.next(__context__,(__a_rename_at_1046_395)) && __ai_iterator.next(__context__,(__ai_rename_at_1046_396)) )
        {
            if ( eq_dstr_str((*__a_rename_at_1046_395),__name_rename_at_1045_392) )
            {
                return das_auto_cast<int32_t>::cast(__ai_rename_at_1046_396);
            };
        }
        __a_iterator.close(__context__,(__a_rename_at_1046_395));
        __ai_iterator.close(__context__,(__ai_rename_at_1046_396));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline bool isCMRESType_be28547f5538e0c5 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __blockT_rename_at_1054_397 )
{
    return das_auto_cast<bool>::cast(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__blockT_rename_at_1054_397),das_auto_cast<void * const >::cast(nullptr))) && ((das_deref(__context__,__blockT_rename_at_1054_397)).isRefType())) && !(das_get_bitfield(__blockT_rename_at_1054_397->flags /*flags*/,1u << 0)));
}

inline void debug_expression_impl_27c6891d2e6065f6 ( Context * __context__, StringBuilderWriter &  __writer_rename_at_1062_398, smart_ptr_raw<Expression> const  __expr_rename_at_1062_399, Bitfield __deFlags_rename_at_1062_400, int32_t __tabs_rename_at_1062_401 ) { das_stack_prologue __prologue(__context__,528,"debug_expression_impl " DAS_FILE_LINE);
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1062_399),das_auto_cast<void * const >::cast(nullptr)) )
    {
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_15,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) "null"))));
        return ;
    } else {
        if ( builtin_empty(__expr_rename_at_1062_399->__rtti /*__rtti*/) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_16,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) "[NO RTTI]"))));
            return ;
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_18, cast<char * const >::from(((char *) "(")), cast<char * const >::from(__expr_rename_at_1062_399->__rtti /*__rtti*/))))));
            if ( das_get_bitfield(__deFlags_rename_at_1062_400,1u << 0) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_19,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_20, cast<char * const >::from(((char *) "<")), cast<uint32_t>::from(builtin_smart_ptr_use_count(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_1062_399),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))), cast<char * const >::from(((char *) ">")))))));
            };
            if ( builtin_string_startswith(((char * const )(pass_string(__expr_rename_at_1062_399->__rtti /*__rtti*/))),((char *) "ExprConst"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_21,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_22, cast<char * const >::from(((char *) " ")), cast<char * const >::from(_FuncastTickdescribeTick842554968825501494_452cfc8955963a18(__context__,__expr_rename_at_1062_399)), cast<char * const >::from(((char *) ")")))))));
                return ;
            } else {
                BasicStructureAnnotation * __ann_rename_at_1079_402 = das_cast<BasicStructureAnnotation *>::cast(get_expression_annotation(_FuncbuiltinTickget_ptrTick8468476673553620226_403d4a8bdcd4ca52(__context__,__expr_rename_at_1062_399),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
                if ( __ann_rename_at_1079_402 == nullptr )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_23,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) ")"))));
                    return ;
                } else {
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1079_402)),das_make_block<void,char *,char *,smart_ptr<TypeDecl>,uint32_t>(__context__,96,0,&__func_info__c13357a1bc201428,[&](char * __name_rename_at_1084_403, char * __cppName_rename_at_1084_404, smart_ptr<TypeDecl> __xtype_rename_at_1084_405, uint32_t __offset_rename_at_1084_406) -> void{
                        if ( __offset_rename_at_1084_406 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1086_407 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__xtype_rename_at_1084_405,true,true,true)));
                            int8_t * __p8_rename_at_1087_408 = 0;
                            das_copy(__p8_rename_at_1087_408,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1062_399),int32_t(__offset_rename_at_1084_406),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1086_407),cast<char *>::from(((char *) "smart_ptr<ast::Expression>")),*__context__,nullptr) )
                            {
                                smart_ptr_raw<Expression> * __pv_rename_at_1092_409 = ((smart_ptr_raw<Expression> *)das_cast<smart_ptr_raw<Expression> *>::cast(__p8_rename_at_1087_408));
                                char * __ts_rename_at_1093_410 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1062_401 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_24,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_25, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1093_410), cast<char * const >::from(((char *) "(")), cast<char *>::from(__name_rename_at_1084_403), cast<char * const >::from(((char *) " ")))))));
                                debug_expression_impl_27c6891d2e6065f6(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1062_398),das_deref(__context__,__pv_rename_at_1092_409),__deFlags_rename_at_1062_400,__tabs_rename_at_1062_401 + 2);
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_26,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) ")"))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1086_407),cast<char *>::from(((char *) "$::das_string")),*__context__,nullptr) )
                            {
                                das::string * __pv_rename_at_1098_411 = ((das::string *)das_cast<das::string *>::cast(__p8_rename_at_1087_408));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_27,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_28, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1084_403), cast<char * const >::from(((char *) "=\"")), cast<das::string const  &>::from(das_deref(__context__,__pv_rename_at_1098_411)), cast<char * const >::from(((char *) "\"")))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1086_407),cast<char *>::from(((char *) "int")),*__context__,nullptr) )
                            {
                                int32_t * __pv_rename_at_1101_412 = ((int32_t *)das_cast<int32_t *>::cast(__p8_rename_at_1087_408));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_29,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_30, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1084_403), cast<char * const >::from(((char *) "=")), cast<int32_t>::from(das_deref(__context__,__pv_rename_at_1101_412)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1086_407),cast<char *>::from(((char *) "uint")),*__context__,nullptr) )
                            {
                                uint32_t * __pv_rename_at_1104_413 = ((uint32_t *)das_cast<uint32_t *>::cast(__p8_rename_at_1087_408));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_31,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_32, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1084_403), cast<char * const >::from(((char *) "=")), cast<uint32_t>::from(das_deref(__context__,__pv_rename_at_1104_413)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1086_407),cast<char *>::from(((char *) "int64")),*__context__,nullptr) )
                            {
                                int64_t * __pv_rename_at_1107_414 = ((int64_t *)das_cast<int64_t *>::cast(__p8_rename_at_1087_408));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_33,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_34, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1084_403), cast<char * const >::from(((char *) "=")), cast<int64_t>::from(das_deref(__context__,__pv_rename_at_1107_414)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1086_407),cast<char *>::from(((char *) "uint64")),*__context__,nullptr) )
                            {
                                uint64_t * __pv_rename_at_1110_415 = ((uint64_t *)das_cast<uint64_t *>::cast(__p8_rename_at_1087_408));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_35,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_36, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1084_403), cast<char * const >::from(((char *) "=")), cast<uint64_t>::from(das_deref(__context__,__pv_rename_at_1110_415)))))));
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1086_407),cast<char *>::from(((char *) "bool")),*__context__,nullptr) )
                            {
                                bool * __pv_rename_at_1113_416 = ((bool *)das_cast<bool *>::cast(__p8_rename_at_1087_408));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_37,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_38, cast<char * const >::from(((char *) " ")), cast<char *>::from(__name_rename_at_1084_403), cast<char * const >::from(((char *) "=")), cast<bool>::from(das_deref(__context__,__pv_rename_at_1113_416)))))));
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_1079_402)),das_make_block<void,char *,char *,smart_ptr<TypeDecl>,uint32_t>(__context__,320,0,&__func_info__c13357a1bc201428,[&](char * __name_rename_at_1123_417, char * __cppName_rename_at_1123_418, smart_ptr<TypeDecl> __xtype_rename_at_1123_419, uint32_t __offset_rename_at_1123_420) -> void{
                        if ( __offset_rename_at_1123_420 != 0xffffffffu )
                        {
                            char * __tstr_rename_at_1125_421 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__xtype_rename_at_1123_419,true,true,true)));
                            int8_t * __p8_rename_at_1126_422 = 0;
                            das_copy(__p8_rename_at_1126_422,das_ptr_add_int32(das_cast<int8_t *>::cast(__expr_rename_at_1062_399),int32_t(__offset_rename_at_1123_420),1));
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1125_421),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Expression")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Expression>> * __pv_rename_at_1131_423 = ((das::vector<smart_ptr<Expression>> *)das_cast<das::vector<smart_ptr<Expression>> *>::cast(__p8_rename_at_1126_422));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1131_423)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1135_38_400; _temp_make_local_1135_38_400;
                                    char * __ts_rename_at_1133_424 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1062_401 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_39,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_40, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1133_424), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1123_417), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1135 = true;
                                        // l: smart_ptr<ast::Expression> const&
                                        das_iterator<das::vector<smart_ptr<Expression>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1131_423));
                                        smart_ptr<Expression> const  * __l_rename_at_1135_427;
                                        __need_loop_1135 = __l_iterator.first(__context__,(__l_rename_at_1135_427)) && __need_loop_1135;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1135_38_400 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1135_428;
                                        __need_loop_1135 = __i_iterator.first(__context__,(__i_rename_at_1135_428)) && __need_loop_1135;
                                        for ( ; __need_loop_1135 ; __need_loop_1135 = __l_iterator.next(__context__,(__l_rename_at_1135_427)) && __i_iterator.next(__context__,(__i_rename_at_1135_428)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_41,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_42, cast<char * const >::from(__ts_rename_at_1133_424), cast<char * const >::from(((char *) "  ")))))));
                                            debug_expression_impl_27c6891d2e6065f6(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1062_398),(*__l_rename_at_1135_427),__deFlags_rename_at_1062_400,__tabs_rename_at_1062_401 + 2);
                                            if ( __i_rename_at_1135_428 != (das_vector_length(das_deref(__context__,__pv_rename_at_1131_423)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_43,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __l_iterator.close(__context__,(__l_rename_at_1135_427));
                                        __i_iterator.close(__context__,(__i_rename_at_1135_428));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_44,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) "]"))));
                                };
                            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__tstr_rename_at_1125_421),cast<char *>::from(((char *) "$::dasvector`smart_ptr`Variable")),*__context__,nullptr) )
                            {
                                das::vector<smart_ptr<Variable>> * __pv_rename_at_1145_429 = ((das::vector<smart_ptr<Variable>> *)das_cast<das::vector<smart_ptr<Variable>> *>::cast(__p8_rename_at_1126_422));
                                if ( das_vector_length(das_deref(__context__,__pv_rename_at_1145_429)) != 0 )
                                {
                                    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1149_38_480; _temp_make_local_1149_38_480;
                                    char * __ts_rename_at_1147_430 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tabs_rename_at_1062_401 + 2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_45,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_46, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__ts_rename_at_1147_430), cast<char * const >::from(((char *) "[")), cast<char *>::from(__name_rename_at_1123_417), cast<char * const >::from(((char *) "\n")))))));
                                    {
                                        bool __need_loop_1149 = true;
                                        // l: smart_ptr<ast::Variable> const&
                                        das_iterator<das::vector<smart_ptr<Variable>> const > __l_iterator(das_deref(__context__,__pv_rename_at_1145_429));
                                        smart_ptr<Variable> const  * __l_rename_at_1149_433;
                                        __need_loop_1149 = __l_iterator.first(__context__,(__l_rename_at_1149_433)) && __need_loop_1149;
                                        // i: int
                                        das_iterator_count DAS_COMMENT((_temp_make_local_1149_38_480 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
                                        int32_t __i_rename_at_1149_434;
                                        __need_loop_1149 = __i_iterator.first(__context__,(__i_rename_at_1149_434)) && __need_loop_1149;
                                        for ( ; __need_loop_1149 ; __need_loop_1149 = __l_iterator.next(__context__,(__l_rename_at_1149_433)) && __i_iterator.next(__context__,(__i_rename_at_1149_434)) )
                                        {
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_47,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_48, cast<char * const >::from(__ts_rename_at_1147_430), cast<char * const >::from(((char *) "  ")))))));
                                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_49,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_50, cast<das::string const  &>::from((*__l_rename_at_1149_433)->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,(*__l_rename_at_1149_433)->type /*_type*/,true,true,true)))))));
                                            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__l_rename_at_1149_433)->init /*init*/),das_auto_cast<void * const >::cast(nullptr)) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_51,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) " = "))));
                                                debug_expression_impl_27c6891d2e6065f6(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1062_398),(*__l_rename_at_1149_433)->init /*init*/,__deFlags_rename_at_1062_400,__tabs_rename_at_1062_401 + 2);
                                            };
                                            if ( __i_rename_at_1149_434 != (das_vector_length(das_deref(__context__,__pv_rename_at_1145_429)) - 1) )
                                            {
                                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_52,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) "\n"))));
                                            };
                                        }
                                        __l_iterator.close(__context__,(__l_rename_at_1149_433));
                                        __i_iterator.close(__context__,(__i_rename_at_1149_434));
                                    };
                                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_53,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) "]"))));
                                };
                            };
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_54,cast<StringBuilderWriter &>::from(__writer_rename_at_1062_398),cast<char * const >::from(((char *) ")"))));
                };
            };
        };
    };
}}

inline char * debug_expression_dc45cab15254a45e ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_1168_435, Bitfield __deFlags_rename_at_1168_436 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1169_437) DAS_AOT_INLINE_LAMBDA -> void{
        debug_expression_impl_27c6891d2e6065f6(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_1169_437),__expr_rename_at_1168_435,__deFlags_rename_at_1168_436,0);
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * debug_expression_b1e90a58d18306b3 ( Context * __context__, Expression * const  __expr_rename_at_1174_438 )
{
    return das_auto_cast<char *>::cast(debug_expression_dc45cab15254a45e(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1174_438),0x0u));
}

inline char * describe_9ffb758bbbac7334 ( Context * __context__, Expression * const  __expr_rename_at_1180_439 )
{
    return das_auto_cast<char *>::cast(_FuncastTickdescribeTick842554968825501494_452cfc8955963a18(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__expr_rename_at_1180_439)));
}

inline int32_t getVectorElementCount_1cefda208a7a5281 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1186_440 )
{
    return das_auto_cast<int32_t>::cast((((((((__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tInt2)) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tUInt2)) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tRange)) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tURange)) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tRange64)) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(2) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tFloat3) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) ? das_auto_cast<int32_t>::cast(3) : das_auto_cast<int32_t>::cast(((((__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tFloat4) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1186_440 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) ? das_auto_cast<int32_t>::cast(4) : das_auto_cast<int32_t>::cast(0))))));
}

inline int32_t getVectorElementSize_e8d53271262fdc3c ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1193_441 )
{
    return das_auto_cast<int32_t>::cast(((__bt_rename_at_1193_441 == DAS_COMMENT(bound_enum) das::Type::tRange64) || (__bt_rename_at_1193_441 == DAS_COMMENT(bound_enum) das::Type::tURange64)) ? das_auto_cast<int32_t>::cast(8) : das_auto_cast<int32_t>::cast(4));
}

inline DAS_COMMENT(bound_enum) das::Type getVectorElementType_696a4aae05024c01 ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1198_442 )
{
    return das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tFloat2) || (__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tFloat3)) || (__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tFloat4)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tFloat) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tInt2) || (__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tInt3)) || (__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tInt4)) || (__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tRange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast((((((__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tUInt2) || (__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tUInt3)) || (__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tUInt4)) || (__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tURange)) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tRange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(((__bt_rename_at_1198_442 == DAS_COMMENT(bound_enum) das::Type::tURange64) ? das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tUInt64) : das_auto_cast<DAS_COMMENT(bound_enum) das::Type>::cast(DAS_COMMENT(bound_enum) das::Type::tVoid))))))))));
}

inline int32_t getVectorOffset_b6d06fc98795c1fe ( Context * __context__, DAS_COMMENT(bound_enum) das::Type __bt_rename_at_1207_443, char * const  __ident_rename_at_1207_444 )
{
    int32_t __ofs_rename_at_1208_445 = -1;
    if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "x")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "X")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "r")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "R")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1208_445,0);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "y")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "Y")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "g")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "G")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1208_445,1);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "z")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "Z")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "b")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "B")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1208_445,2);
    } else if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "w")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "W")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "a")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_1207_444),cast<char *>::from(((char *) "A")),*__context__,nullptr)) )
    {
        das_copy(__ofs_rename_at_1208_445,3);
    };
    int32_t __count_rename_at_1218_446 = ((int32_t)getVectorElementCount_1cefda208a7a5281(__context__,__bt_rename_at_1207_443));
    return das_auto_cast<int32_t>::cast((__ofs_rename_at_1208_445 >= __count_rename_at_1218_446) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(__ofs_rename_at_1208_445));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x17cd053e5d46f3ad] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & , ast::AstFunctionAnnotation *  ),&_FuncbuiltinTickpushTick10769833213962245646_17d5a7b0b5dc7d2a>>();
    };
    aotLib[0x56f4e029324f78f5] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_d38c0a8fb8b57a95
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::MacroMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_d38c0a8fb8b57a95>>();
    };
    aotLib[0x3039d9dd6c6a63c9] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_a59c8e1dab321df7
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::TagFunctionAnnotation const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_a59c8e1dab321df7>>();
    };
    aotLib[0xd432d8270fb6a9f1] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_c677163a1eb520b6
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstStructureAnnotation *> & , ast::AstStructureAnnotation *  ),&_FuncbuiltinTickpushTick10769833213962245646_c677163a1eb520b6>>();
    };
    aotLib[0x5b308b94e094e71c] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_fd822f8ec049c56f
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupFunctionAnnotation const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_fd822f8ec049c56f>>();
    };
    aotLib[0x64594eb3a985957d] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_b1daca88b52c6678
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupBlockAnnotation const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_b1daca88b52c6678>>();
    };
    aotLib[0xf39b2660b287a1b4] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_34de85a7749a610b
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupStructureAnnotation const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_34de85a7749a610b>>();
    };
    aotLib[0x7cf336f67f8086e0] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_f8a0e6267b2bd3d9
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupEnumerationAnnotation const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_f8a0e6267b2bd3d9>>();
    };
    aotLib[0x4d7ade45163068fe] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_30f11df7ec48d590
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupContractAnnotation const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_30f11df7ec48d590>>();
    };
    aotLib[0x3c9f2bdd4e92c2ef] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_cbd2268f16dd6f7
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupReaderMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_cbd2268f16dd6f7>>();
    };
    aotLib[0x195bfefa66ae4602] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_90f0b6ebf218b95
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupCommentReader const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_90f0b6ebf218b95>>();
    };
    aotLib[0x3f9364bf781486ff] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_4f4d07b67d5c637a
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupCallMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_4f4d07b67d5c637a>>();
    };
    aotLib[0x648431c1ca4b4201] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_58a689e6c5544c13
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupTypeInfoMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_58a689e6c5544c13>>();
    };
    aotLib[0xe41dacbde7ce41c7] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_9e90922306ddd25a
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupVariantMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_9e90922306ddd25a>>();
    };
    aotLib[0xafe46acc43f9a7eb] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_d17686a9f7a586f4
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupForLoopMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_d17686a9f7a586f4>>();
    };
    aotLib[0x3d6b5e29898c3a14] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_97d75e289ad3a300
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupCaptureMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_97d75e289ad3a300>>();
    };
    aotLib[0x7c4dc57c0f98b04e] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_5fb85726896385b1
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupTypeMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_5fb85726896385b1>>();
    };
    aotLib[0x8f39af5519403720] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_2d9c9af4d9e7b493
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupSimulateMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_2d9c9af4d9e7b493>>();
    };
    aotLib[0x741bbe130a8f4ae3] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_6d99e55f093884d6
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::TagStructureAnnotation const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_6d99e55f093884d6>>();
    };
    aotLib[0x24655d0b373d632] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_cf4f8fcd2b1e128a
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::TagFunctionMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_cf4f8fcd2b1e128a>>();
    };
    aotLib[0xb364d8c46fd3d8c3] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_bc379015cee45784
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupInferMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_bc379015cee45784>>();
    };
    aotLib[0x1131042fa64986d5] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_62e05e00785596c6
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupDirtyInferMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_62e05e00785596c6>>();
    };
    aotLib[0x81eb87c709937c42] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_bebaf7033e24000c
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupOptimizationMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_bebaf7033e24000c>>();
    };
    aotLib[0xd217a15bbe4f36da] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_dd66b37998de4f39
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupLintMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_dd66b37998de4f39>>();
    };
    aotLib[0xf51611fbe9c8d84b] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_103841aa9a022d76
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::SetupGlobalLintMacro const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_103841aa9a022d76>>();
    };
    aotLib[0xc8eef7ae328438ae] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_5fc7cdb5157ef6b5
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<ast::AstVariantMacro *> & , ast::AstVariantMacro *  ),&_FuncbuiltinTickpushTick10769833213962245646_5fc7cdb5157ef6b5>>();
    };
    aotLib[0x97e2f4bbf0d3ba07] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickclass_infoTick15801393167907430156_c881cfb5d9ae9ed5
        return ctx.code->makeNode<SimNode_Aot<StructInfo const  * (*) ( Context * __context__, ast_boost::BetterRttiVisitor const  &  ),&_FuncrttiTickclass_infoTick15801393167907430156_c881cfb5d9ae9ed5>>();
    };
    aotLib[0xa9dd19487d9457be] = +[](Context & ctx) -> SimNode* { // clone_a23f2772d6a2dd68
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr_raw<FunctionAnnotation> & , smart_ptr_raw<FunctionAnnotation> const   ),&clone_a23f2772d6a2dd68>>();
    };
    aotLib[0xab48dcd96347fd3b] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickclone_to_moveTick2007252383599261567_ddf94d0785b39e5b
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<FunctionAnnotation> (*) ( Context * __context__, smart_ptr_raw<FunctionAnnotation> const   ),&_FuncbuiltinTickclone_to_moveTick2007252383599261567_ddf94d0785b39e5b>>();
    };
    aotLib[0x7f44f15ef830603b] = +[](Context & ctx) -> SimNode* { // _FuncastTickfind_moduleTick11101329256228773934_52d9ec34594ea355
        return ctx.code->makeNode<SimNode_Aot<Module * (*) ( Context * __context__, smart_ptr_raw<Program> const  , char * const   ),&_FuncastTickfind_moduleTick11101329256228773934_52d9ec34594ea355>>();
    };
    aotLib[0x78948a08f0d98f51] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<char *> &  ),&_FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8>>();
    };
    aotLib[0x36832c0122a65a24] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<smart_ptr_raw<TypeDecl>> &  ),&_FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9>>();
    };
    aotLib[0x69266b84daeea44e] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickfinalizeTick13836114024949725080_c6f9bc68b25c5dd6
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<smart_ptr_raw<Variable>> &  ),&_FuncbuiltinTickfinalizeTick13836114024949725080_c6f9bc68b25c5dd6>>();
    };
    aotLib[0x9707e43dd37f682e] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  ),&_FuncbuiltinTickfinalizeTick13836114024949725080_473a42788c45df36>>();
    };
    aotLib[0xde6e54abcc6b70c2] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<char *> & , char *  ),&_FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1>>();
    };
    aotLib[0x17c076c38e276651] = +[](Context & ctx) -> SimNode* { // _Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, TArray<char *> const  & , char * const   ),&_Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835>>();
    };
    aotLib[0x3c395992954dd60d] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<char *> & , char * const   ),&_FuncbuiltinTickpushTick14133213201864676143_d626258b591934af>>();
    };
    aotLib[0x2898e91ce589d994] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpush_cloneTick2035469273396957942_a44b26c5d2257edc
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<smart_ptr_raw<Function>> & , smart_ptr_raw<Function>  ),&_FuncbuiltinTickpush_cloneTick2035469273396957942_a44b26c5d2257edc>>();
    };
    aotLib[0x3b46012ec8f86527] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickfinalizeTick13836114024949725080_aba46312bb49c9f6
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<smart_ptr_raw<Function>> &  ),&_FuncbuiltinTickfinalizeTick13836114024949725080_aba46312bb49c9f6>>();
    };
    aotLib[0xa4bb37681cd26aba] = +[](Context & ctx) -> SimNode* { // clone_8c54ff7e0079253a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr_raw<Function> & , smart_ptr<Function> const   ),&clone_8c54ff7e0079253a>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* { // _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const  , bool , bool , bool  ),&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    aotLib[0x398d67528cabe5c6] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850
        return ctx.code->makeNode<SimNode_Aot<Structure * (*) ( Context * __context__, smart_ptr_raw<Structure> const   ),&_FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850>>();
    };
    aotLib[0x6a6a8acedd776000] = +[](Context & ctx) -> SimNode* { // _FuncrttiTickRttiValue_nothingTick4715542659269841615_3746f4c2db327a87
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> (*) ( Context * __context__ ),&_FuncrttiTickRttiValue_nothingTick4715542659269841615_3746f4c2db327a87>>();
    };
    aotLib[0x18f85abcacdc86ef] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick5807679485210906136_8c07d4f696b9c5ba
        return ctx.code->makeNode<SimNode_Aot<ExprBlock * (*) ( Context * __context__, smart_ptr_raw<ExprBlock>  ),&_FuncbuiltinTickget_ptrTick5807679485210906136_8c07d4f696b9c5ba>>();
    };
    aotLib[0x5668daa1685b0ea8] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick5807679485210906136_bd8120dfe7363955
        return ctx.code->makeNode<SimNode_Aot<Structure * (*) ( Context * __context__, smart_ptr_raw<Structure>  ),&_FuncbuiltinTickget_ptrTick5807679485210906136_bd8120dfe7363955>>();
    };
    aotLib[0x4df527949c9c5212] = +[](Context & ctx) -> SimNode* { // _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_d7d3fcc28de34db6
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<ExprOp3> (*) ( Context * __context__, smart_ptr_raw<ExprAsVariant> const  , smart_ptr_raw<Expression>  ),&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_d7d3fcc28de34db6>>();
    };
    aotLib[0x599e7cfd51b1a631] = +[](Context & ctx) -> SimNode* { // _Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_e95e98afff367ec7
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<ExprOp3> (*) ( Context * __context__, smart_ptr_raw<ExprSafeAsVariant> const  , smart_ptr_raw<Expression>  ),&_Funcast_boostTickmakeAsOrSafeAsTick3414563941976524297_e95e98afff367ec7>>();
    };
    aotLib[0x8d72b4cfb0c6ecd7] = +[](Context & ctx) -> SimNode* { // clone_f1f421db208b5182
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr<TypeDecl> & , smart_ptr_raw<TypeDecl> const   ),&clone_f1f421db208b5182>>();
    };
    aotLib[0x6c2d7a1325fbe337] = +[](Context & ctx) -> SimNode* { // clone_9ab0f6a4e0a6006e
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr<Enumeration> & , Enumeration * const   ),&clone_9ab0f6a4e0a6006e>>();
    };
    aotLib[0xd7ad598efd7d5f4d] = +[](Context & ctx) -> SimNode* { // _FuncastTickfind_compiling_moduleTick4523452840392654583_34f86625d1f8ecb2
        return ctx.code->makeNode<SimNode_Aot<Module * (*) ( Context * __context__, char * const   ),&_FuncastTickfind_compiling_moduleTick4523452840392654583_34f86625d1f8ecb2>>();
    };
    aotLib[0x418aeaaaf2e8ad98] = +[](Context & ctx) -> SimNode* { // clone_3a3075a3a0c0cb06
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr<Annotation> & , smart_ptr_raw<Annotation> const   ),&clone_3a3075a3a0c0cb06>>();
    };
    aotLib[0xfbfe800d5700e08f] = +[](Context & ctx) -> SimNode* { // _FuncastTickdescribeTick842554968825501494_452cfc8955963a18
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<Expression> const   ),&_FuncastTickdescribeTick842554968825501494_452cfc8955963a18>>();
    };
    aotLib[0x6d67d30c5527e1cf] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick8468476673553620226_403d4a8bdcd4ca52
        return ctx.code->makeNode<SimNode_Aot<Expression * (*) ( Context * __context__, smart_ptr_raw<Expression> const   ),&_FuncbuiltinTickget_ptrTick8468476673553620226_403d4a8bdcd4ca52>>();
    };
    aotLib[0x6d436a2c230abb18] = +[](Context & ctx) -> SimNode* { // clone_ec5e41b1830fac59
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, das::vector<das::string> & , TArray<char *> &  ),&clone_ec5e41b1830fac59>>();
    };
    aotLib[0xe4c4497a696cc05c] = +[](Context & ctx) -> SimNode* { // clone_36538fa38bdd6e0b
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, das::vector<smart_ptr<TypeDecl>> & , TArray<smart_ptr_raw<TypeDecl>> &  ),&clone_36538fa38bdd6e0b>>();
    };
    aotLib[0xd64ef80cbe9d9158] = +[](Context & ctx) -> SimNode* { // clone_82cb7bdbc1cd751b
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, das::vector<smart_ptr<Variable>> & , TArray<smart_ptr_raw<Variable>> &  ),&clone_82cb7bdbc1cd751b>>();
    };
    aotLib[0x5b357276d1c15c55] = +[](Context & ctx) -> SimNode* { // clone_ab5928efc1a1d61
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, das::vector<smart_ptr<Expression>> & , TArray<smart_ptr_raw<Expression>> &  ),&clone_ab5928efc1a1d61>>();
    };
    aotLib[0x1ff341022810d743] = +[](Context & ctx) -> SimNode* { // isVectorType_e16b21c2cb0df3cf
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, DAS_COMMENT(bound_enum) das::Type  ),&isVectorType_e16b21c2cb0df3cf>>();
    };
    aotLib[0x7a542f6e1edae4e3] = +[](Context & ctx) -> SimNode* { // describe_f8955cd58aaa956c
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, AnnotationArgumentList const  &  ),&describe_f8955cd58aaa956c>>();
    };
    aotLib[0x63c6a571c2d74c4] = +[](Context & ctx) -> SimNode* { // describe_16a184ebde2f5850
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, AnnotationDeclaration const  &  ),&describe_16a184ebde2f5850>>();
    };
    aotLib[0x73a423d4ed696f66] = +[](Context & ctx) -> SimNode* { // describe_5703109ec79a52b4
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, AnnotationList const  &  ),&describe_5703109ec79a52b4>>();
    };
    aotLib[0xbb5f03f92b7a832d] = +[](Context & ctx) -> SimNode* { // describe_9b40ad62f87c706d
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<Variable> const   ),&describe_9b40ad62f87c706d>>();
    };
    aotLib[0x1d64bc585c61009] = +[](Context & ctx) -> SimNode* { // isExpression_a7c31959e6c730e8
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const  , bool  ),&isExpression_a7c31959e6c730e8>>();
    };
    aotLib[0x5dd04630b4f3a14] = +[](Context & ctx) -> SimNode* { // is_same_or_inherited_8fc466ebe75e2336
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, Structure const  * const  , Structure const  * const   ),&is_same_or_inherited_8fc466ebe75e2336>>();
    };
    aotLib[0x382bec11c180e68e] = +[](Context & ctx) -> SimNode* { // is_class_method_67e7c960daf5e6e2
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, smart_ptr_raw<Structure> const  , smart_ptr_raw<TypeDecl> const   ),&is_class_method_67e7c960daf5e6e2>>();
    };
    aotLib[0x1b965d44531ef205] = +[](Context & ctx) -> SimNode* { // emplace_new_eadb1a1435b53033
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, das::vector<smart_ptr<Expression>> & , smart_ptr_raw<Expression>  ),&emplace_new_eadb1a1435b53033>>();
    };
    aotLib[0x6838d87b5dfd43bf] = +[](Context & ctx) -> SimNode* { // emplace_new_52cce80de15d9fe4
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, das::vector<smart_ptr<TypeDecl>> & , smart_ptr_raw<TypeDecl>  ),&emplace_new_52cce80de15d9fe4>>();
    };
    aotLib[0x6cc57e0331a385cc] = +[](Context & ctx) -> SimNode* { // emplace_new_e46333e37cb8f35d
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, das::vector<smart_ptr<Variable>> & , smart_ptr_raw<Variable>  ),&emplace_new_e46333e37cb8f35d>>();
    };
    aotLib[0x2501e72ebe4a3bd9] = +[](Context & ctx) -> SimNode* { // emplace_new_2dcb96a028e6e2a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, MakeStruct & , smart_ptr_raw<MakeFieldDecl>  ),&emplace_new_2dcb96a028e6e2a>>();
    };
    aotLib[0x47845919b80afc97] = +[](Context & ctx) -> SimNode* { // override_method_38d6ecad3789a50f
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, smart_ptr_raw<Structure> , char * const  , char * const   ),&override_method_38d6ecad3789a50f>>();
    };
    aotLib[0x24bce9e357346e74] = +[](Context & ctx) -> SimNode* { // for_each_tag_function_bfda4041b94932d7
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, Module * const  , char * const  , Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  ),&for_each_tag_function_bfda4041b94932d7>>();
    };
    aotLib[0xe6f1253fd6bee4d1] = +[](Context & ctx) -> SimNode* { // find_arg_bb44f3ca6ce3fc6d
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> (*) ( Context * __context__, char * const  , AnnotationArgumentList const  &  ),&find_arg_bb44f3ca6ce3fc6d>>();
    };
    aotLib[0x830f648bb5cd4d28] = +[](Context & ctx) -> SimNode* { // find_arg_a148d1ff83124bd1
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f> (*) ( Context * __context__, AnnotationArgumentList const  & , char * const   ),&find_arg_a148d1ff83124bd1>>();
    };
    aotLib[0x35dbaaaa9d2abfa9] = +[](Context & ctx) -> SimNode* { // apply_tag_annotation_f773cf67c31220
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, char * const  , smart_ptr_raw<FunctionAnnotation>  ),&apply_tag_annotation_f773cf67c31220>>();
    };
    aotLib[0xeed3772339d45207] = +[](Context & ctx) -> SimNode* { // find_unique_function_aab3bc60151e96ec
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Function> (*) ( Context * __context__, Module * const  , char * const  , bool  ),&find_unique_function_aab3bc60151e96ec>>();
    };
    aotLib[0xec1ff6bcbffe9136] = +[](Context & ctx) -> SimNode* { // find_unique_generic_b6f2e7df17214306
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Function> (*) ( Context * __context__, Module * const  , char * const  , bool  ),&find_unique_generic_b6f2e7df17214306>>();
    };
    aotLib[0x48180d2fcfd11fce] = +[](Context & ctx) -> SimNode* { // setup_call_list_ad0757bf84a727d8
        return ctx.code->makeNode<SimNode_Aot<ExprBlock * (*) ( Context * __context__, char * const  , LineInfo const  & , Block DAS_COMMENT((void,smart_ptr_raw<Function>)) const  &  ),&setup_call_list_ad0757bf84a727d8>>();
    };
    aotLib[0xba7bba954ad350fe] = +[](Context & ctx) -> SimNode* { // setup_call_list_f7df6b96f49a9cf8
        return ctx.code->makeNode<SimNode_Aot<ExprBlock * (*) ( Context * __context__, char * const  , LineInfo const  & , bool , bool , bool  ),&setup_call_list_f7df6b96f49a9cf8>>();
    };
    aotLib[0xa415cb14fb81834b] = +[](Context & ctx) -> SimNode* { // setup_macro_1ed401bbb9e11b66
        return ctx.code->makeNode<SimNode_Aot<ExprBlock * (*) ( Context * __context__, char * const  , LineInfo const  &  ),&setup_macro_1ed401bbb9e11b66>>();
    };
    aotLib[0xd6c8c6a40fb2e00e] = +[](Context & ctx) -> SimNode* { // panic_expr_as_b448efb56dfa5ea
        return ctx.code->makeNode<SimNode_Aot<void * (*) ( Context * __context__ ),&panic_expr_as_b448efb56dfa5ea>>();
    };
    aotLib[0x75687a3935df917f] = +[](Context & ctx) -> SimNode* { // walk_and_convert_array_fc0c75b20fd9b5d8
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_array_fc0c75b20fd9b5d8>>();
    };
    aotLib[0xb2be0673141b416a] = +[](Context & ctx) -> SimNode* { // walk_and_convert_dim_307e83e2e898f055
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_dim_307e83e2e898f055>>();
    };
    aotLib[0xcaef79fda822a77f] = +[](Context & ctx) -> SimNode* { // walk_and_convert_pointer_99fa96f2fad2fabb
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_pointer_99fa96f2fad2fabb>>();
    };
    aotLib[0xeb86cba8923b3a27] = +[](Context & ctx) -> SimNode* { // walk_and_convert_tuple_d283511dc79b27c5
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_tuple_d283511dc79b27c5>>();
    };
    aotLib[0xb1c287cc3ce35506] = +[](Context & ctx) -> SimNode* { // walk_and_convert_structure_461cae90387df9d8
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_structure_461cae90387df9d8>>();
    };
    aotLib[0x726f07c0324a46cf] = +[](Context & ctx) -> SimNode* { // walk_and_convert_variant_50ce6cc7d88f2c68
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_variant_50ce6cc7d88f2c68>>();
    };
    aotLib[0x32a4577a23ef7839] = +[](Context & ctx) -> SimNode* { // walk_and_convert_table_578c212c6a418c09
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_table_578c212c6a418c09>>();
    };
    aotLib[0xc0e9b25e94a70426] = +[](Context & ctx) -> SimNode* { // walk_and_convert_basic_abd632a9adbc5f64
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_basic_abd632a9adbc5f64>>();
    };
    aotLib[0x8fd78acb5635b5cb] = +[](Context & ctx) -> SimNode* { // walk_and_convert_enumeration_19e84f10944d8e34
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_enumeration_19e84f10944d8e34>>();
    };
    aotLib[0x35d9d221107e213a] = +[](Context & ctx) -> SimNode* { // walk_and_convert_52e7fa0c06656c3c
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<Expression> (*) ( Context * __context__, uint8_t const  * const  , smart_ptr_raw<TypeDecl> const  , LineInfo const  &  ),&walk_and_convert_52e7fa0c06656c3c>>();
    };
    aotLib[0x2ec1191f594c06c] = +[](Context & ctx) -> SimNode* { // find_annotation_3783ce606e338529
        return ctx.code->makeNode<SimNode_Aot<Annotation const  * (*) ( Context * __context__, char * const  , char * const   ),&find_annotation_3783ce606e338529>>();
    };
    aotLib[0x2871a52c0ab10758] = +[](Context & ctx) -> SimNode* { // append_annotation_518bf407af95eedd
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<AnnotationDeclaration> (*) ( Context * __context__, char * const  , char * const  , TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  ),&append_annotation_518bf407af95eedd>>();
    };
    aotLib[0x21e7d51c927eb322] = +[](Context & ctx) -> SimNode* { // append_annotation_304ed986ed5830b8
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<AnnotationDeclaration> (*) ( Context * __context__, char * const  , char * const   ),&append_annotation_304ed986ed5830b8>>();
    };
    aotLib[0xa21b15bfceaf5227] = +[](Context & ctx) -> SimNode* { // append_annotation_a089579603ac9094
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr_raw<Function> , char * const  , char * const   ),&append_annotation_a089579603ac9094>>();
    };
    aotLib[0xe611a77f8fdaa718] = +[](Context & ctx) -> SimNode* { // append_annotation_df67cdd1c7fda936
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr_raw<ExprBlock> , char * const  , char * const   ),&append_annotation_df67cdd1c7fda936>>();
    };
    aotLib[0xf3a804c596b50ee5] = +[](Context & ctx) -> SimNode* { // append_annotation_33c7fba9141ce1dc
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr_raw<Structure> , char * const  , char * const   ),&append_annotation_33c7fba9141ce1dc>>();
    };
    aotLib[0xd037c6eb8f44195c] = +[](Context & ctx) -> SimNode* { // append_annotation_50d585b969d10a85
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr_raw<Function> , char * const  , char * const  , TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  ),&append_annotation_50d585b969d10a85>>();
    };
    aotLib[0x7be1185c6f5e934f] = +[](Context & ctx) -> SimNode* { // append_annotation_4809cc819e2c2e25
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr_raw<ExprBlock> , char * const  , char * const  , TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  ),&append_annotation_4809cc819e2c2e25>>();
    };
    aotLib[0x3ff904546f657393] = +[](Context & ctx) -> SimNode* { // append_annotation_c82c20ac1475e6cb
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, smart_ptr_raw<Structure> , char * const  , char * const  , TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  ),&append_annotation_c82c20ac1475e6cb>>();
    };
    aotLib[0xa1068c4707fb2469] = +[](Context & ctx) -> SimNode* { // add_annotation_argument_aecf50176a737617
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, AnnotationArgumentList & , char * const  , bool  ),&add_annotation_argument_aecf50176a737617>>();
    };
    aotLib[0xe4768b6bf458cd14] = +[](Context & ctx) -> SimNode* { // add_annotation_argument_584d51af09b7a892
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, AnnotationArgumentList & , char * const  , int32_t  ),&add_annotation_argument_584d51af09b7a892>>();
    };
    aotLib[0x355c69c8d9d54ba2] = +[](Context & ctx) -> SimNode* { // add_annotation_argument_2991fd3097c426a8
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, AnnotationArgumentList & , char * const  , float  ),&add_annotation_argument_2991fd3097c426a8>>();
    };
    aotLib[0x8b0f97bfd17fe680] = +[](Context & ctx) -> SimNode* { // add_annotation_argument_62c7cc55608504df
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, AnnotationArgumentList & , char * const  , char * const   ),&add_annotation_argument_62c7cc55608504df>>();
    };
    aotLib[0x2edda877d8598cc] = +[](Context & ctx) -> SimNode* { // add_annotation_argument_806b0c29e45217e2
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, AnnotationArgumentList & , AnnotationArgument const  &  ),&add_annotation_argument_806b0c29e45217e2>>();
    };
    aotLib[0xe32acd7320b36fd6] = +[](Context & ctx) -> SimNode* { // get_for_source_index_9bcd632a50af36c1
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, smart_ptr_raw<ExprFor> const  , smart_ptr_raw<Variable> const   ),&get_for_source_index_9bcd632a50af36c1>>();
    };
    aotLib[0x1958a7b00178c238] = +[](Context & ctx) -> SimNode* { // get_for_source_index_2b39b717f35b48e5
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, smart_ptr_raw<ExprFor> const  , smart_ptr_raw<Expression> const   ),&get_for_source_index_2b39b717f35b48e5>>();
    };
    aotLib[0x5de7edea2c452a58] = +[](Context & ctx) -> SimNode* { // function_to_type_84839d9adeaf04f3
        return ctx.code->makeNode<SimNode_Aot<smart_ptr_raw<TypeDecl> (*) ( Context * __context__, smart_ptr_raw<Function> const   ),&function_to_type_84839d9adeaf04f3>>();
    };
    aotLib[0x368396182dac86c8] = +[](Context & ctx) -> SimNode* { // visit_finally_27770f49825d1a9b
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, ExprBlock * const  , smart_ptr_raw<VisitorAdapter> const   ),&visit_finally_27770f49825d1a9b>>();
    };
    aotLib[0x1f949321dd7325de] = +[](Context & ctx) -> SimNode* { // isCMRES_ecc22ba10553fb74
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, smart_ptr_raw<Function> const   ),&isCMRES_ecc22ba10553fb74>>();
    };
    aotLib[0xba7d49323eb8b4b3] = +[](Context & ctx) -> SimNode* { // isCMRES_c750d4ae5480733b
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, Function * const   ),&isCMRES_c750d4ae5480733b>>();
    };
    aotLib[0x5280ef8d6e365480] = +[](Context & ctx) -> SimNode* { // isMakeLocal_18c81e2ae52a95c7
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, smart_ptr_raw<Expression> const   ),&isMakeLocal_18c81e2ae52a95c7>>();
    };
    aotLib[0xd9867e9107e78e5d] = +[](Context & ctx) -> SimNode* { // get_workhorse_types_e61ceb837ac4d4c
        return ctx.code->makeNode<SimNode_AotCMRES<TDim<DAS_COMMENT(bound_enum) das::Type,31> (*) ( Context * __context__ ),&get_workhorse_types_e61ceb837ac4d4c>>();
    };
    aotLib[0x7c1035002eec890b] = +[](Context & ctx) -> SimNode* { // find_argument_index_ddbc5417768753b7
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const  , char * const   ),&find_argument_index_ddbc5417768753b7>>();
    };
    aotLib[0xfb1068b7ce1df416] = +[](Context & ctx) -> SimNode* { // isCMRESType_be28547f5538e0c5
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const   ),&isCMRESType_be28547f5538e0c5>>();
    };
    aotLib[0xfa35cf1a0b61f6cc] = +[](Context & ctx) -> SimNode* { // debug_expression_impl_27c6891d2e6065f6
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, StringBuilderWriter & , smart_ptr_raw<Expression> const  , Bitfield , int32_t  ),&debug_expression_impl_27c6891d2e6065f6>>();
    };
    aotLib[0xc7c79e53e7f4b6fd] = +[](Context & ctx) -> SimNode* { // debug_expression_dc45cab15254a45e
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<Expression> const  , Bitfield  ),&debug_expression_dc45cab15254a45e>>();
    };
    aotLib[0x3ceadf7844d1f1cd] = +[](Context & ctx) -> SimNode* { // debug_expression_b1e90a58d18306b3
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Expression * const   ),&debug_expression_b1e90a58d18306b3>>();
    };
    aotLib[0x8a5e40d1540a8efa] = +[](Context & ctx) -> SimNode* { // describe_9ffb758bbbac7334
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Expression * const   ),&describe_9ffb758bbbac7334>>();
    };
    aotLib[0xe6497f2b4f4a696] = +[](Context & ctx) -> SimNode* { // getVectorElementCount_1cefda208a7a5281
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, DAS_COMMENT(bound_enum) das::Type  ),&getVectorElementCount_1cefda208a7a5281>>();
    };
    aotLib[0x1300b9a480e53894] = +[](Context & ctx) -> SimNode* { // getVectorElementSize_e8d53271262fdc3c
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, DAS_COMMENT(bound_enum) das::Type  ),&getVectorElementSize_e8d53271262fdc3c>>();
    };
    aotLib[0xfa5f1ddc3ff1a5b5] = +[](Context & ctx) -> SimNode* { // getVectorElementType_696a4aae05024c01
        return ctx.code->makeNode<SimNode_Aot<DAS_COMMENT(bound_enum) das::Type (*) ( Context * __context__, DAS_COMMENT(bound_enum) das::Type  ),&getVectorElementType_696a4aae05024c01>>();
    };
    aotLib[0x8676129577d44df] = +[](Context & ctx) -> SimNode* { // getVectorOffset_b6d06fc98795c1fe
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, DAS_COMMENT(bound_enum) das::Type , char * const   ),&getVectorOffset_b6d06fc98795c1fe>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_2307684379922914619
AotListBase impl_aot_ast_boost(_anon_2307684379922914619::registerAotFunctions);
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
