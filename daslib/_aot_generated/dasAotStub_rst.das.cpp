#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require rst
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
extern VarInfo __var_info__f563cd204981ee23;
extern VarInfo __var_info__934148247e502892;
extern VarInfo __var_info__62310b35c768ef50;
extern VarInfo __var_info__c8b6e136feca1e7d;
extern VarInfo __var_info__bdf35a32817fb4f0;
extern VarInfo __var_info__83c4685915b4f96;
extern VarInfo __var_info__5ac39c9b48fed41d;
extern VarInfo __var_info__5671a754112737a5;
extern FuncInfo __func_info__6edfc1622f67255a;
extern FuncInfo __func_info__4bda63c2c7a42bef;
extern FuncInfo __func_info__f381b060f4dbce7;
extern FuncInfo __func_info__8075d08fbfce07f4;
extern FuncInfo __func_info__aa03c1bcb8f15255;
extern FuncInfo __func_info__c13357a1bc201428;
extern FuncInfo __func_info__9beafd8de389bb7e;
extern FuncInfo __func_info__438460bc7bed8ede;
extern FuncInfo __func_info__5f3d7fba1c40074b;
extern FuncInfo __func_info__2701da5c12e56b0e;
extern FuncInfo __func_info__84319ec9af82a642;
extern FuncInfo __func_info__e0afcd004fbe6de3;

VarInfo __struct_info__d775c4f039534efe_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x5f478141901ca6f8), "name", 0, 1 };
VarInfo __struct_info__d775c4f039534efe_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__f7046ae574272550, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x882ea441b2e06229), "func", 8, 3 };
VarInfo __struct_info__d775c4f039534efe_field_2 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xc15d399143d0bdf0), "hidden", 32, 0 };
VarInfo * __struct_info__d775c4f039534efe_fields[3] =  { &__struct_info__d775c4f039534efe_field_0, &__struct_info__d775c4f039534efe_field_1, &__struct_info__d775c4f039534efe_field_2 };
StructInfo __struct_info__d775c4f039534efe = {"DocGroup", "rst", 28, __struct_info__d775c4f039534efe_fields, 3, 40, UINT64_C(0x0), nullptr, UINT64_C(0xd775c4f039534efe), 0 };
TypeInfo * __type_info__744f16ce3a0187d7_arg_types_var_9474757822741603490[2] = { &__type_info__8e26ff93c3acecc5, &__type_info__774985c53b7f8953 };
const char * __type_info__744f16ce3a0187d7_arg_names_var_9474757822741603490[2] = { "__this", "_ryield1216" };
VarInfo __struct_info__837d1a0a555de4a2_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__744f16ce3a0187d7_arg_types_var_9474757822741603490, __type_info__744f16ce3a0187d7_arg_names_var_9474757822741603490, 2, 0, nullptr, 12, 8, UINT64_C(0x744f16ce3a0187d7), "__lambda", 0, 0 };
TypeInfo * __type_info__ff0b0abb7a32db38_arg_types_var_9474757822741603490[1] = { &__type_info__f08d475fe5d680fc };
const char * __type_info__ff0b0abb7a32db38_arg_names_var_9474757822741603490[1] = { "__this" };
VarInfo __struct_info__837d1a0a555de4a2_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ff0b0abb7a32db38_arg_types_var_9474757822741603490, __type_info__ff0b0abb7a32db38_arg_names_var_9474757822741603490, 1, 0, nullptr, 12, 8, UINT64_C(0xff0b0abb7a32db38), "__finalize", 8, 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x94e091f447dc1a54), "__yield", 16, 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaeeeca2da7dab890), "index", 20, 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_4 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xefb62fe9be79f20e), "snp", 24, 7 };
VarInfo __struct_info__837d1a0a555de4a2_field_5 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x2d3c72fd421646d6), "_loop_at_1216_35", 48, 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_6 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xe668a7c47b738e21), "__i_rename_at_1216_39", 52, 0 };
VarInfo __struct_info__837d1a0a555de4a2_field_7 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x22f7a2c7fa9c2063), "_pvar_0_at_1216_35", 56, 8 };
VarInfo __struct_info__837d1a0a555de4a2_field_8 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0x21aa09afb788eee7), "_source_0_at_1216_35", 64, 9 };
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
VarInfo __func_info__8075d08fbfce07f4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0xec83c0a8da46dd28), "func", 0, 0 };
VarInfo * __func_info__8075d08fbfce07f4_fields[1] =  { &__func_info__8075d08fbfce07f4_field_0 };
FuncInfo __func_info__8075d08fbfce07f4 = {"invoke block<(var func:smart_ptr<ast::Function>):void> const", "", __func_info__8075d08fbfce07f4_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8075d08fbfce07f4), 0x0 };
VarInfo __func_info__aa03c1bcb8f15255_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16452, 8, UINT64_C(0xf05dbeaf5dbf985d), "name", 0, 0 };
VarInfo __func_info__aa03c1bcb8f15255_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ce241e3005cc873b, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0x62310b35c768ef50), "value", 0, 0 };
VarInfo * __func_info__aa03c1bcb8f15255_fields[2] =  { &__func_info__aa03c1bcb8f15255_field_0, &__func_info__aa03c1bcb8f15255_field_1 };
FuncInfo __func_info__aa03c1bcb8f15255 = {"invoke block<(var name:string#;var value:smart_ptr<ast::TypeDecl>):void> const", "", __func_info__aa03c1bcb8f15255_fields, 2, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xaa03c1bcb8f15255), 0x0 };
VarInfo __func_info__c13357a1bc201428_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x2a896b18ac8c5dc), "name", 0, 0 };
VarInfo __func_info__c13357a1bc201428_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x1b2f537dda20b669), "cppName", 0, 0 };
VarInfo __func_info__c13357a1bc201428_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ce241e3005cc873b, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0x5671a754112737a5), "xtype", 0, 0 };
VarInfo __func_info__c13357a1bc201428_field_3 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xc3630b856e2c8315), "offset", 0, 0 };
VarInfo * __func_info__c13357a1bc201428_fields[4] =  { &__func_info__c13357a1bc201428_field_0, &__func_info__c13357a1bc201428_field_1, &__func_info__c13357a1bc201428_field_2, &__func_info__c13357a1bc201428_field_3 };
FuncInfo __func_info__c13357a1bc201428 = {"invoke block<(var name:string;var cppName:string;var xtype:smart_ptr<ast::TypeDecl>;var offset:uint):void> const", "", __func_info__c13357a1bc201428_fields, 4, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xc13357a1bc201428), 0x0 };
VarInfo __func_info__9beafd8de389bb7e_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__cd505ad3b1c59cc6, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0xf563cd204981ee23), "value", 0, 0 };
VarInfo * __func_info__9beafd8de389bb7e_fields[1] =  { &__func_info__9beafd8de389bb7e_field_0 };
FuncInfo __func_info__9beafd8de389bb7e = {"invoke block<(var value:smart_ptr<ast::Enumeration>):void> const", "", __func_info__9beafd8de389bb7e_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x9beafd8de389bb7e), 0x0 };
VarInfo __func_info__438460bc7bed8ede_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__60d16a2d23420951, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0x934148247e502892), "value", 0, 0 };
VarInfo * __func_info__438460bc7bed8ede_fields[1] =  { &__func_info__438460bc7bed8ede_field_0 };
FuncInfo __func_info__438460bc7bed8ede = {"invoke block<(var value:smart_ptr<ast::Structure>):void> const", "", __func_info__438460bc7bed8ede_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x438460bc7bed8ede), 0x0 };
VarInfo __func_info__5f3d7fba1c40074b_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__d0872eb2c1d28358, nullptr, nullptr, nullptr, 0, 0, nullptr, 11264, 8, UINT64_C(0xc8b6e136feca1e7d), "value", 0, 0 };
VarInfo * __func_info__5f3d7fba1c40074b_fields[1] =  { &__func_info__5f3d7fba1c40074b_field_0 };
FuncInfo __func_info__5f3d7fba1c40074b = {"invoke block<(var value:smart_ptr<ast::TypeInfoMacro>):void> const", "", __func_info__5f3d7fba1c40074b_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x5f3d7fba1c40074b), 0x0 };
VarInfo __func_info__2701da5c12e56b0e_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__ccd32e474e9a33eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0xbdf35a32817fb4f0), "value", 0, 0 };
VarInfo * __func_info__2701da5c12e56b0e_fields[1] =  { &__func_info__2701da5c12e56b0e_field_0 };
FuncInfo __func_info__2701da5c12e56b0e = {"invoke block<(var value:smart_ptr<ast::Variable>):void> const", "", __func_info__2701da5c12e56b0e_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x2701da5c12e56b0e), 0x0 };
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

enum class ConstMatters : int32_t {
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
// unused enumeration SideEffects
// unused enumeration CaptureMode
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
static_assert(sizeof(ReNode)==152,"structure size mismatch with DAS");
static_assert(offsetof(ReNode,op)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,id)==4,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,fun2)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,gen2)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,at)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,text)==32,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,textLen)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,all)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,left)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,right)==80,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,subexpr)==88,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,next)==96,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,cset)==104,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,index)==136,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,tail)==144,"structure field offset mismatch with DAS");
}
namespace regex {

struct Regex {
    regex::ReNode * root;
    uint8_t const  * match;
    TArray<TTuple<16,range,char *>> groups;
    TDim<uint32_t,8> earlyOut;
    bool canEarlyOut;
};
static_assert(sizeof(Regex)==80,"structure size mismatch with DAS");
static_assert(offsetof(Regex,root)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,match)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,groups)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,earlyOut)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,canEarlyOut)==72,"structure field offset mismatch with DAS");
}
// unused structure RegexReader
namespace rst {

struct DocGroup {
    char * name;
    TArray<Function *> func;
    bool hidden;
};
static_assert(sizeof(DocGroup)==40,"structure size mismatch with DAS");
static_assert(offsetof(DocGroup,name)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(DocGroup,func)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(DocGroup,hidden)==32,"structure field offset mismatch with DAS");
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
static_assert(sizeof(_lambda_rst_1216_1)==72,"structure size mismatch with DAS");
static_assert(offsetof(_lambda_rst_1216_1,__lambda)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_rst_1216_1,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_rst_1216_1,__yield)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_rst_1216_1,index)==20,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_rst_1216_1,snp)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_rst_1216_1,_loop_at_1216_35)==48,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_rst_1216_1,__i_rename_at_1216_39)==52,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_rst_1216_1,_pvar_0_at_1216_35)==56,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_rst_1216_1,_source_0_at_1216_35)==64,"structure field offset mismatch with DAS");
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9 ( Context * __context__, TDim<TTuple<16,char *,char *>,1> const  & __a_rename_at_581_0 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_75e0f8673d04729f ( Context * __context__, TDim<TTuple<16,char *,char *>,13> const  & __a_rename_at_581_1 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e ( Context * __context__, TArray<TTuple<16,char *,char *>> & __Arr_rename_at_68_2, int32_t __newSize_rename_at_68_3 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_626ed670fc52be6d ( Context * __context__, TDim<TTuple<16,char *,char *>,3> const  & __a_rename_at_581_4 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> & __Arr_rename_at_68_5, int32_t __newSize_rename_at_68_6 );
inline TArray<TTuple<16,char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf ( Context * __context__, TDim<TTuple<16,char *,char *>,1> & __a_rename_at_1343_7 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044333107478717573_19fa17f56443c23c ( Context * __context__, range __rng_rename_at_1245_9 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d ( Context * __context__, TDim<char *,3> const  & __a_rename_at_581_11 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b ( Context * __context__, TDim<char *,2> const  & __a_rename_at_581_12 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> & __a_rename_at_1183_13 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66 ( Context * __context__, TArray<TypeDecl *> & __Arr_rename_at_181_14, TypeDecl * __value_rename_at_181_15 );
inline char * _Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9 ( Context * __context__, Sequence DAS_COMMENT((char * &)) & __it_rename_at_46_16, char * const  __separator_rename_at_46_17 );
inline char * _FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7 ( Context * __context__, Annotation const  & __value_rename_at_61_22 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_60d4e412e550e258 ( Context * __context__, TArray<TArray<char *>> const  & __a_rename_at_585_23 );
inline char * _FuncrstTickfunction_label_nameTick4951524166161443384_aafe521f2cb4dc50 ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_77_24 );
inline void finalize_5bf95341c40e5b82 ( Context * __context__, rst::_lambda_rst_1216_1 & ____this_rename_at_1216_26 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1 ( Context * __context__, TArray<char *> & __Arr_rename_at_181_27, char * __value_rename_at_181_28 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835 ( Context * __context__, TArray<char *> const  & __it_rename_at_22_29, char * const  __separator_rename_at_22_30 );
inline TArray<TTuple<16,char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f ( Context * __context__, TDim<TTuple<16,char *,char *>,13> & __a_rename_at_1343_35 );
inline void _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50 ( Context * __context__, TArray<Function *> & __Arr_rename_at_181_37, Function * __value_rename_at_181_38 );
inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickeach_refTick16137060296763333387_f3ba366f8c86460e ( Context * __context__, Lambda DAS_COMMENT((bool,char * * &)) const  __lam_rename_at_1300_39 );
inline TypeDecl * _FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e ( Context * __context__, smart_ptr<TypeDecl> __src_rename_at_1733_41 );
inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_f84a97f3b3efcea2 ( Context * __context__, Function * const  __value_rename_at_69_42 );
inline TArray<TTuple<16,char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_2c64031744b1c1f2 ( Context * __context__, TDim<TTuple<16,char *,char *>,3> & __a_rename_at_1343_44 );
inline void _FuncrstTickdocument_typedefTick2903024037696058038_24a5c3bc896476d1 ( Context * __context__, FILE const  * const  __doc_file_rename_at_419_46, Module * const  __mod_rename_at_419_47, char * const  __name_rename_at_419_48, smart_ptr<TypeDecl> const  __value_rename_at_419_49 );
inline void _FuncrstTickdocument_enumerationTick4211850574665690795_cd2737074b5000f2 ( Context * __context__, FILE const  * const  __doc_file_rename_at_496_51, Module * const  __mod_rename_at_496_52, smart_ptr<Enumeration> const  __value_rename_at_496_53 );
inline void _FuncrstTickdocument_class_methodTick12589992380795293570_b6d1264d11ee8d48 ( Context * __context__, FILE const  * const  __doc_file_rename_at_573_57, Module * const  __mod_rename_at_573_58, smart_ptr<Structure> const  __value_rename_at_573_59, Structure::FieldDeclaration & __fld_rename_at_573_60 );
inline void _FuncrstTickdocument_structure_annotationTick2734840712723762798_45f2db6d6c9372ba ( Context * __context__, FILE const  * const  __doc_file_rename_at_673_67, Module * const  __mod_rename_at_673_68, Annotation const  & __value_rename_at_673_69 );
inline void _FuncrstTickdocument_function_annotationTick9878220666429632644_62156d6269c1aee6 ( Context * __context__, FILE const  * const  __doc_file_rename_at_762_94, Module * const  __mod_rename_at_762_95, Annotation const  & __value_rename_at_762_96 );
inline void _FuncrstTickdocument_annotationTick7679268031359399902_8b47c8f555ccae04 ( Context * __context__, FILE const  * const  __doc_file_rename_at_789_97, Module * const  __mod_rename_at_789_98, Annotation const  & __value_rename_at_789_99 );
inline void _FuncrstTickdocument_structure_macroTick6088634580083749384_a8cf27101dd30a50 ( Context * __context__, FILE const  * const  __doc_file_rename_at_819_100, Module * const  __mod_rename_at_819_101, Annotation const  & __value_rename_at_819_102 );
inline Function * _FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615 ( Context * __context__, smart_ptr<Function> __src_rename_at_1733_103 );
inline void _FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1008_104, Module * const  __mod_rename_at_1008_105, smart_ptr_raw<Function> __func_rename_at_1008_106 );
inline rst::DocGroup & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_c745a97595700ef ( Context * __context__, rst::DocGroup & __a_rename_at_50_113 );
inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_69_114 );
inline bool _Func_lambda_rst_1216_1Tickfunction_deb6e09d1773cc6a ( Context * __context__, rst::_lambda_rst_1216_1 & ____this_rename_at_1216_116, char * * & ___ryield1216_rename_at_1216_117 );
inline void _Func_lambda_rst_1216_1Tickfinalizer_d8783cc009d376af ( Context * __context__, rst::_lambda_rst_1216_1 * ____this_rename_at_1216_118 );
inline char * _FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1 ( Context * __context__, Module * const  __mod_rename_at_28_119 );
inline Variable * _FuncbuiltinTickget_ptrTick8468476673553620226_f1503a45e3ae58f3 ( Context * __context__, smart_ptr_raw<Variable> const  __src_rename_at_1727_121 );
inline Enumeration * _FuncbuiltinTickget_ptrTick8468476673553620226_d6dabe47fb1a720 ( Context * __context__, smart_ptr_raw<Enumeration> const  __src_rename_at_1727_122 );
inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1727_123 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_37_124, bool __extra_rename_at_37_125, bool __contracts_rename_at_37_126, bool __modules_rename_at_37_127 );
inline char * _FuncrstTickhandle_label_nameTick13616998130381234091_7f23dc2205503d09 ( Context * __context__, TypeAnnotation * const  __value_rename_at_61_128 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_68_129, int32_t __newSize_rename_at_68_130 );
inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_131, char * const  __mode_rename_at_12_132, Block DAS_COMMENT((void,FILE const  * const )) const  & __blk_rename_at_12_133 );
inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_649bcd7e51242beb ( Context * __context__, TDim<char *,3> & __a_rename_at_1343_135 );
inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> & __Arr_rename_at_165_137, char * const  __value_rename_at_165_138 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6 ( Context * __context__, TArray<TArray<char *>> & __Arr_rename_at_287_139, TArray<char *> & __value_rename_at_287_140 );
inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_47_141 );
inline bool _FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6 ( Context * __context__, TypeDecl * const  __tt_rename_at_531_142 );
inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_325c63fe2a174cf6 ( Context * __context__, TDim<char *,2> & __a_rename_at_1343_143 );
inline char * _FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c ( Context * __context__, Module * const  __mod_rename_at_65_145, char * const  __value_rename_at_65_146 );
inline void _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a ( Context * __context__, TTable<Function *,bool> & __Tab_rename_at_948_147, Function * const  __at_rename_at_948_148, bool __val_rename_at_948_149 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_4e6333822225d431 ( Context * __context__, TTable<Function *,bool> const  & __Tab_rename_at_1015_150, Function * const  __at_rename_at_1015_151 );
inline char * _Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f ( Context * __context__, Function * const  __func_rename_at_89_152 );
inline char * _FuncrstTickfunction_label_nameTick4951524166161443384_ded37d7d287c25b5 ( Context * __context__, Function * const  __value_rename_at_77_157 );
inline bool _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72 ( Context * __context__, TTable<Function *,bool> & __Tab_rename_at_742_159, Function * const  __at_rename_at_742_160 );
inline void _FuncrstTickdocument_functionTick8829733267459955112_767667cb9ac814a7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1008_161, Module * const  __mod_rename_at_1008_162, Function * __func_rename_at_1008_163 );
inline bool _FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45 ( Context * __context__, TTable<char *,bool> & __Tab_rename_at_742_170, char * const  __at_rename_at_742_171 );
inline void _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b ( Context * __context__, TTable<char *,bool> & __Tab_rename_at_948_172, char * const  __at_rename_at_948_173, bool __val_rename_at_948_174 );
inline char * alias_label_name_f0e3a6da4e226a3e ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_33_175 );
inline char * global_label_name_2b7e96918b37c00e ( Context * __context__, Variable * const  __value_rename_at_37_176 );
inline char * global_label_name_b721f72143d35230 ( Context * __context__, smart_ptr_raw<Variable> const  __value_rename_at_41_177 );
inline char * enum_label_name_a32575da767b3ecd ( Context * __context__, Enumeration * const  __value_rename_at_45_178 );
inline char * enum_label_name_768485a24af7de17 ( Context * __context__, smart_ptr_raw<Enumeration> const  __value_rename_at_49_179 );
inline char * struct_label_name_7b7faf8095586da6 ( Context * __context__, Structure * const  __value_rename_at_53_180 );
inline char * struct_label_name_6ee1d24e90557aeb ( Context * __context__, smart_ptr_raw<Structure> const  __value_rename_at_57_181 );
inline char * describe_type_short_56370d8993e381ba ( Context * __context__, TypeDecl * const  __td_rename_at_96_182 );
inline char * describe_type_short_847eafd8ddf6216 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_102_183 );
inline char * describe_type_cf1a091488868462 ( Context * __context__, TypeDecl * const  __td_rename_at_110_185 );
inline char * describe_type_a9bac10014c763b2 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_116_186 );
inline char * make_domain_3e6edb06cf9af7bc ( Context * __context__, char * const  __attr_rename_at_294_208 );
inline char * make_label_3368ba1f933c2731 ( Context * __context__, char * const  __name_rename_at_298_209 );
inline char * make_ref_c995159f43ed1b43 ( Context * __context__, char * const  __name_rename_at_302_210, char * const  __text_rename_at_302_211 );
inline char * make_group_8b304b06bc5b7645 ( Context * __context__, char * const  __name_rename_at_311_213, char * const  __plus_rename_at_311_214 );
inline char * make_header_efad6667e51b3e08 ( Context * __context__, char * const  __name_rename_at_316_216, char * const  __lab_rename_at_316_217 );
inline void write_table_separator_2180d51ced094114 ( Context * __context__, StringBuilderWriter & __writer_rename_at_321_219, TArray<int32_t> const  & __wid_rename_at_321_220, char * const  __sep_rename_at_321_221 );
inline void write_table_line_18bbb8245228f037 ( Context * __context__, StringBuilderWriter & __writer_rename_at_328_223, TArray<char *> const  & __line_rename_at_328_224, TArray<int32_t> const  & __wid_rename_at_328_225 );
inline char * make_table_46d5ce8cf5a30e69 ( Context * __context__, TArray<TArray<char *>> const  & __tab_rename_at_335_230, bool __withHeader_rename_at_335_231 );
inline char * topic_9659659d94ddb29a ( Context * __context__, char * const  __category_rename_at_358_242, Module * const  __mod_rename_at_358_243, char * const  __name_rename_at_358_244 );
inline char * topic_b97c7facf4ccb6a9 ( Context * __context__, Module * const  __mod_rename_at_362_245 );
inline void document_topic_c6e2614dff9f2d3e ( Context * __context__, FILE const  * const  __doc_file_rename_at_366_246, char * const  __topic_rename_at_366_247 );
inline void document_bitfield_4340095c1075f95e ( Context * __context__, FILE const  * const  __doc_file_rename_at_384_252, smart_ptr_raw<TypeDecl> const  __value_rename_at_384_253 );
inline void document_variant_c74548ebfca402e7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_397_260, smart_ptr_raw<TypeDecl> const  __value_rename_at_397_261 );
inline void document_tuple_c74548ebfca402e7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_408_268, smart_ptr_raw<TypeDecl> const  __value_rename_at_408_269 );
inline void document_typedefs_d1f98d27346b80af ( Context * __context__, FILE const  * const  __doc_file_rename_at_440_276, Module * const  __mod_rename_at_440_277 );
inline void document_global_constant_378f3e1a10c345f3 ( Context * __context__, FILE const  * const  __doc_file_rename_at_454_283, Module * const  __mod_rename_at_454_284, smart_ptr_raw<Variable> const  __value_rename_at_454_285 );
inline bool is_global_constant_320cc2da3d8764f7 ( Context * __context__, smart_ptr_raw<Variable> const  __value_rename_at_462_286 );
inline void document_global_constants_24362fe0342586e0 ( Context * __context__, FILE const  * const  __doc_file_rename_at_478_287, Module * const  __mod_rename_at_478_288 );
inline void document_enumerations_b052b53cf3b12eb ( Context * __context__, FILE const  * const  __doc_file_rename_at_511_292, Module * const  __mod_rename_at_511_293 );
inline void document_function_declaration_4c978976b1aaa270 ( Context * __context__, FILE const  * const  __doc_file_rename_at_541_297, char * const  __domain_rename_at_541_298, char * const  __func_name_rename_at_541_299, TArray<char *> const  & __argNames_rename_at_542_300, TArray<TypeDecl *> const  & __argTypes_rename_at_542_301, smart_ptr_raw<TypeDecl> const  __resType_rename_at_542_302 );
inline void document_function_arguments_e7469e75af4ed246 ( Context * __context__, FILE const  * const  __doc_file_rename_at_557_312, TArray<char *> const  & __argNames_rename_at_557_313, TArray<TypeDecl *> const  & __argTypes_rename_at_557_314 );
inline void document_classes_55fe4b580059698a ( Context * __context__, FILE const  * const  __doc_file_rename_at_587_321, Module * const  __mod_rename_at_587_322 );
inline void document_structure_96bd65f4eda31cf3 ( Context * __context__, FILE const  * const  __doc_file_rename_at_633_330, Module * const  __mod_rename_at_633_331, smart_ptr_raw<Structure> const  __value_rename_at_633_332 );
inline void document_structures_5fb20d0dee04ba46 ( Context * __context__, FILE const  * const  __doc_file_rename_at_653_336, Module * const  __mod_rename_at_653_337 );
inline void document_structure_annotations_a53005d9c9a5617 ( Context * __context__, FILE const  * const  __doc_file_rename_at_742_341, Module * const  __mod_rename_at_742_342 );
inline void document_function_annotations_5344d92e9ea569ad ( Context * __context__, FILE const  * const  __doc_file_rename_at_769_346, Module * const  __mod_rename_at_769_347 );
inline void document_annotations_128851a794d46d1a ( Context * __context__, FILE const  * const  __doc_file_rename_at_796_351, Module * const  __mod_rename_at_796_352 );
inline void document_structure_macros_7386ca2820215b3d ( Context * __context__, FILE const  * const  __doc_file_rename_at_826_356, Module * const  __mod_rename_at_826_357 );
inline void document_call_macro_e76fbe68d00e7362 ( Context * __context__, FILE const  * const  __doc_file_rename_at_846_361, Module * const  __mod_rename_at_846_362, char * const  __value_rename_at_846_363 );
inline void document_call_macros_56da413fe6ccb104 ( Context * __context__, FILE const  * const  __doc_file_rename_at_853_364, Module * const  __mod_rename_at_853_365 );
inline void document_reader_macro_4006c26786c1908 ( Context * __context__, FILE const  * const  __doc_file_rename_at_867_369, Module * const  __mod_rename_at_867_370, char * const  __value_rename_at_867_371 );
inline void document_reader_macros_585b57fffa625cd ( Context * __context__, FILE const  * const  __doc_file_rename_at_874_372, Module * const  __mod_rename_at_874_373 );
inline void document_variant_macro_84da1b9ee053cb60 ( Context * __context__, FILE const  * const  __doc_file_rename_at_888_377, Module * const  __mod_rename_at_888_378, char * const  __value_rename_at_888_379 );
inline void document_variant_macros_eab4d095021756ef ( Context * __context__, FILE const  * const  __doc_file_rename_at_895_380, Module * const  __mod_rename_at_895_381 );
inline void document_typeinfo_macro_66a7bf37ca0e4fcd ( Context * __context__, FILE const  * const  __doc_file_rename_at_909_385, Module * const  __mod_rename_at_909_386, char * const  __value_rename_at_909_387 );
inline void document_typeinfo_macros_b4b5371c027f780c ( Context * __context__, FILE const  * const  __doc_file_rename_at_916_388, Module * const  __mod_rename_at_916_389 );
inline bool function_needs_documenting_5ed98b6306abb3d7 ( Context * __context__, smart_ptr_raw<Function> const  __func_rename_at_930_393 );
inline void document_warning_cb981aa521e01a92 ( Context * __context__, FILE const  * const  __doc_file_rename_at_993_404, char * const  __issue_rename_at_993_405 );
inline char * function_name_ee62641d633d27c8 ( Context * __context__, char * const  __name_rename_at_998_406 );
inline void document_functions_a509d99cbdb2836d ( Context * __context__, FILE const  * const  __doc_file_rename_at_1027_408, Module * const  __mod_rename_at_1027_409, TArray<rst::DocGroup> & __groups_rename_at_1027_410 );
inline rst::DocGroup debug_group_d90dc61b38b89347 ( Context * __context__, rst::DocGroup & __group_rename_at_1098_423 );
inline void append_to_group_by_regex_819dad4f532ef50d ( Context * __context__, rst::DocGroup & __group_rename_at_1106_425, Module * const  __mod_rename_at_1106_426, regex::Regex & __reg_rename_at_1106_427 );
inline rst::DocGroup group_by_regex_def313b7b3c9e262 ( Context * __context__, char * const  __name_rename_at_1123_430, Module * const  __mod_rename_at_1123_431, regex::Regex & __reg_rename_at_1123_432 );
inline rst::DocGroup hide_group_4868115ae5eeb38f ( Context * __context__, rst::DocGroup & __group_rename_at_1131_434 );
inline void generate_topic_stub_2777760f2b77d9f0 ( Context * __context__, TTable<char *,bool> & __tab_rename_at_1137_435, FILE const  * const  __doc_file_rename_at_1137_436, char * const  __topic_rename_at_1137_437 );
inline void generate_substitute_stub_34b873e38788090 ( Context * __context__, Module * const  __mod_rename_at_1144_438, char * const  __substname_rename_at_1144_439 );
inline char * getDetailIncludePath_fb77bda786a398b2 ( Context * __context__, char * const  __fname_rename_at_1205_452, char * const  __substname_rename_at_1205_453 );
inline void document_96eb220f96118989 ( Context * __context__, char * const  __name_rename_at_1221_462, Module * const  __mod_rename_at_1221_463, char * const  __fname_rename_at_1221_464, char * const  __substname_rename_at_1221_465, TArray<rst::DocGroup> & __groups_rename_at_1221_466 );
inline rst::DocGroup DocGroup_6e55999f1d438d24 ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<bool,0x7292a0a703a50ed7>(__context__) = true;/*log_documentation*/
    das_global<bool,0xaa5d977586266135>(__context__) = false;/*show_hidden_groups*/
    das_global<char *,0xde937609e38e5c08>(__context__) = nullptr;/*topic_root*/
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9 ( Context * __context__, TDim<TTuple<16,char *,char *>,1> const  &  __a_rename_at_581_0 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_75e0f8673d04729f ( Context * __context__, TDim<TTuple<16,char *,char *>,13> const  &  __a_rename_at_581_1 )
{
    return das_auto_cast<int32_t>::cast(13);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e ( Context * __context__, TArray<TTuple<16,char *,char *>> &  __Arr_rename_at_68_2, int32_t __newSize_rename_at_68_3 )
{
    builtin_array_resize(das_arg<TArray<TTuple<16,char *,char *>>>::pass(__Arr_rename_at_68_2),__newSize_rename_at_68_3,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_626ed670fc52be6d ( Context * __context__, TDim<TTuple<16,char *,char *>,3> const  &  __a_rename_at_581_4 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_68_5, int32_t __newSize_rename_at_68_6 )
{
    builtin_array_resize(das_arg<TArray<char *>>::pass(__Arr_rename_at_68_5),__newSize_rename_at_68_6,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<TTuple<16,char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf ( Context * __context__, TDim<TTuple<16,char *,char *>,1> &  __a_rename_at_1343_7 )
{
    TArray<TTuple<16,char *,char *>> __arr_rename_at_1345_8; das_zero(__arr_rename_at_1345_8);
    _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e(__context__,das_arg<TArray<TTuple<16,char *,char *>>>::pass(__arr_rename_at_1345_8),1);
    das_copy(das_cast<TDim<TTuple<16,char *,char *>,1>>::cast(das_ref(__context__,__arr_rename_at_1345_8(0,__context__))),__a_rename_at_1343_7);
    return /* <- */ das_auto_cast_move<TArray<TTuple<16,char *,char *>>>::cast(__arr_rename_at_1345_8);
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044333107478717573_19fa17f56443c23c ( Context * __context__, range __rng_rename_at_1245_9 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1246_10; das_zero(__it_rename_at_1246_10);
    builtin_make_range_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1246_10),__rng_rename_at_1245_9,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1246_10);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d ( Context * __context__, TDim<char *,3> const  &  __a_rename_at_581_11 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b ( Context * __context__, TDim<char *,2> const  &  __a_rename_at_581_12 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> &  __a_rename_at_1183_13 )
{
    builtin_array_free(das_arg<TArray<char *>>::pass(__a_rename_at_1183_13),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
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
    TArray<TTuple<16,char *,char *>> _temp_make_local_79_32_48; _temp_make_local_79_32_48;
    TDim<TTuple<16,char *,char *>,13> _temp_make_local_79_32_80; _temp_make_local_79_32_80;
    char * __mn_rename_at_78_25 = (char *)(((char * const )(get_mangled_name(__value_rename_at_77_24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(__mn_rename_at_78_25,das_invoke_function<char * const >::invoke<char * const ,TArray<TTuple<16,char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 5840488413950466740u)),__mn_rename_at_78_25,das_arg<TArray<TTuple<16,char *,char *>>>::pass((_temp_make_local_79_32_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f(__context__,das_arg<TDim<TTuple<16,char *,char *>,13>>::pass((([&]() -> TDim<TTuple<16,char *,char *>,13>& {
        _temp_make_local_79_32_80(0,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_80;
            das_get_tuple_field<char *,0>::get(__mkt_80) = ((char *) " ");
            das_get_tuple_field<char *,8>::get(__mkt_80) = ((char *) "_");
            return __mkt_80;
        })());
        _temp_make_local_79_32_80(1,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_81;
            das_get_tuple_field<char *,0>::get(__mkt_81) = ((char *) "$");
            das_get_tuple_field<char *,8>::get(__mkt_81) = ((char *) "_builtin_");
            return __mkt_81;
        })());
        _temp_make_local_79_32_80(2,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_82;
            das_get_tuple_field<char *,0>::get(__mkt_82) = ((char *) "#");
            das_get_tuple_field<char *,8>::get(__mkt_82) = ((char *) "_hh_");
            return __mkt_82;
        })());
        _temp_make_local_79_32_80(3,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_83;
            das_get_tuple_field<char *,0>::get(__mkt_83) = ((char *) ":");
            das_get_tuple_field<char *,8>::get(__mkt_83) = ((char *) "_c_");
            return __mkt_83;
        })());
        _temp_make_local_79_32_80(4,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_84;
            das_get_tuple_field<char *,0>::get(__mkt_84) = ((char *) "@");
            das_get_tuple_field<char *,8>::get(__mkt_84) = ((char *) "_at_");
            return __mkt_84;
        })());
        _temp_make_local_79_32_80(5,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_85;
            das_get_tuple_field<char *,0>::get(__mkt_85) = ((char *) "<");
            das_get_tuple_field<char *,8>::get(__mkt_85) = ((char *) "_ls_");
            return __mkt_85;
        })());
        _temp_make_local_79_32_80(6,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_86;
            das_get_tuple_field<char *,0>::get(__mkt_86) = ((char *) ">");
            das_get_tuple_field<char *,8>::get(__mkt_86) = ((char *) "_gr_");
            return __mkt_86;
        })());
        _temp_make_local_79_32_80(7,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_87;
            das_get_tuple_field<char *,0>::get(__mkt_87) = ((char *) "`");
            das_get_tuple_field<char *,8>::get(__mkt_87) = ((char *) "_rq_");
            return __mkt_87;
        })());
        _temp_make_local_79_32_80(8,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_88;
            das_get_tuple_field<char *,0>::get(__mkt_88) = ((char *) "!");
            das_get_tuple_field<char *,8>::get(__mkt_88) = ((char *) "_ex_");
            return __mkt_88;
        })());
        _temp_make_local_79_32_80(9,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_89;
            das_get_tuple_field<char *,0>::get(__mkt_89) = ((char *) "=");
            das_get_tuple_field<char *,8>::get(__mkt_89) = ((char *) "_eq_");
            return __mkt_89;
        })());
        _temp_make_local_79_32_80(10,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_90;
            das_get_tuple_field<char *,0>::get(__mkt_90) = ((char *) "?");
            das_get_tuple_field<char *,8>::get(__mkt_90) = ((char *) "_qm_");
            return __mkt_90;
        })());
        _temp_make_local_79_32_80(11,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_91;
            das_get_tuple_field<char *,0>::get(__mkt_91) = ((char *) "[");
            das_get_tuple_field<char *,8>::get(__mkt_91) = ((char *) "_lb_");
            return __mkt_91;
        })());
        _temp_make_local_79_32_80(12,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_92;
            das_get_tuple_field<char *,0>::get(__mkt_92) = ((char *) "]");
            das_get_tuple_field<char *,8>::get(__mkt_92) = ((char *) "_rb_");
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
    memset(&(____this_rename_at_1216_26), 0, 72);
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

inline TArray<TTuple<16,char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f ( Context * __context__, TDim<TTuple<16,char *,char *>,13> &  __a_rename_at_1343_35 )
{
    TArray<TTuple<16,char *,char *>> __arr_rename_at_1345_36; das_zero(__arr_rename_at_1345_36);
    _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e(__context__,das_arg<TArray<TTuple<16,char *,char *>>>::pass(__arr_rename_at_1345_36),13);
    das_copy(das_cast<TDim<TTuple<16,char *,char *>,13>>::cast(das_ref(__context__,__arr_rename_at_1345_36(0,__context__))),__a_rename_at_1343_35);
    return /* <- */ das_auto_cast_move<TArray<TTuple<16,char *,char *>>>::cast(__arr_rename_at_1345_36);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50 ( Context * __context__, TArray<Function *> &  __Arr_rename_at_181_37, Function * __value_rename_at_181_38 )
{
    das_copy(__Arr_rename_at_181_37(builtin_array_push_back(das_arg<TArray<Function *>>::pass(__Arr_rename_at_181_37),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_38);
}

inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickeach_refTick16137060296763333387_f3ba366f8c86460e ( Context * __context__, Lambda DAS_COMMENT((bool,char * * &)) const  __lam_rename_at_1300_39 )
{
    Sequence DAS_COMMENT((char * *)) __it_rename_at_1301_40; das_zero(__it_rename_at_1301_40);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1301_40),__lam_rename_at_1300_39,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char * &))>::cast(__it_rename_at_1301_40);
}

inline TypeDecl * _FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e ( Context * __context__, smart_ptr<TypeDecl> __src_rename_at_1733_41 )
{
    return das_auto_cast<TypeDecl *>::cast(das_cast<TypeDecl *>::cast(__src_rename_at_1733_41));
}

inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_f84a97f3b3efcea2 ( Context * __context__, Function * const  __value_rename_at_69_42 )
{
    TArray<TTuple<16,char *,char *>> _temp_make_local_72_32_48; _temp_make_local_72_32_48;
    TDim<TTuple<16,char *,char *>,1> _temp_make_local_72_32_80; _temp_make_local_72_32_80;
    char * __mn_rename_at_71_43 = (char *)(((char * const )(to_das_string(__value_rename_at_69_42->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(__mn_rename_at_71_43,das_invoke_function<char * const >::invoke<char * const ,TArray<TTuple<16,char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 5840488413950466740u)),__mn_rename_at_71_43,das_arg<TArray<TTuple<16,char *,char *>>>::pass((_temp_make_local_72_32_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf(__context__,das_arg<TDim<TTuple<16,char *,char *>,1>>::pass((([&]() -> TDim<TTuple<16,char *,char *>,1>& {
        _temp_make_local_72_32_80(0,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_73;
            das_get_tuple_field<char *,0>::get(__mkt_73) = ((char *) "?");
            das_get_tuple_field<char *,8>::get(__mkt_73) = ((char *) "_qm_");
            return __mkt_73;
        })());
        return _temp_make_local_72_32_80;
    })()))))))));
    return das_auto_cast<char *>::cast(__mn_rename_at_71_43);
}

inline TArray<TTuple<16,char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_2c64031744b1c1f2 ( Context * __context__, TDim<TTuple<16,char *,char *>,3> &  __a_rename_at_1343_44 )
{
    TArray<TTuple<16,char *,char *>> __arr_rename_at_1345_45; das_zero(__arr_rename_at_1345_45);
    _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e(__context__,das_arg<TArray<TTuple<16,char *,char *>>>::pass(__arr_rename_at_1345_45),3);
    das_copy(das_cast<TDim<TTuple<16,char *,char *>,3>>::cast(das_ref(__context__,__arr_rename_at_1345_45(0,__context__))),__a_rename_at_1343_44);
    return /* <- */ das_auto_cast_move<TArray<TTuple<16,char *,char *>>>::cast(__arr_rename_at_1345_45);
}

inline void _FuncrstTickdocument_typedefTick2903024037696058038_24a5c3bc896476d1 ( Context * __context__, FILE const  * const  __doc_file_rename_at_419_46, Module * const  __mod_rename_at_419_47, char * const  __name_rename_at_419_48, smart_ptr<TypeDecl> const  __value_rename_at_419_49 )
{
    builtin_fwrite(__doc_file_rename_at_419_46,make_label_3368ba1f933c2731(__context__,alias_label_name_f0e3a6da4e226a3e(__context__,__value_rename_at_419_49)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_419_46,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_419_46,__name_rename_at_419_48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __value_rename_at_419_49->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tBitfield )
    {
        builtin_fwrite(__doc_file_rename_at_419_46,((char *) " is a bitfield\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        document_bitfield_4340095c1075f95e(__context__,__doc_file_rename_at_419_46,__value_rename_at_419_49);
    } else if ( __value_rename_at_419_49->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tVariant )
    {
        builtin_fwrite(__doc_file_rename_at_419_46,((char *) " is a variant type\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        document_variant_c74548ebfca402e7(__context__,__doc_file_rename_at_419_46,__value_rename_at_419_49);
    } else if ( __value_rename_at_419_49->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tTuple )
    {
        builtin_fwrite(__doc_file_rename_at_419_46,((char *) " is a tuple type\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        document_tuple_c74548ebfca402e7(__context__,__doc_file_rename_at_419_46,__value_rename_at_419_49);
    } else {
        char * __tp_rename_at_433_50 = (char *)(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__value_rename_at_419_49,false,false,false));
        das_copy(__tp_rename_at_433_50,((char * const )(builtin_string_replace(__tp_rename_at_433_50,((char *) "$::"),nullptr,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        builtin_fwrite(__doc_file_rename_at_419_46,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<char * const >::from(((char *) " = ")), cast<char *>::from(__tp_rename_at_433_50), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_419_46,topic_9659659d94ddb29a(__context__,((char *) "typedef"),__mod_rename_at_419_47,__name_rename_at_419_48));
}

inline void _FuncrstTickdocument_enumerationTick4211850574665690795_cd2737074b5000f2 ( Context * __context__, FILE const  * const  __doc_file_rename_at_496_51, Module * const  __mod_rename_at_496_52, smart_ptr<Enumeration> const  __value_rename_at_496_53 )
{
    builtin_fwrite(__doc_file_rename_at_496_51,make_label_3368ba1f933c2731(__context__,enum_label_name_768485a24af7de17(__context__,__value_rename_at_496_53)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_496_51,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_496_51,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_7, cast<das::string const  &>::from(__value_rename_at_496_53->name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    TArray<TArray<char *>> __tab_rename_at_500_54; das_zero(__tab_rename_at_500_54);
    {
        bool __need_loop_501 = true;
        // en: ast::EnumEntry const&
        das_iterator<das::vector<Enumeration::EnumEntry> const > __en_iterator(__value_rename_at_496_53->list /*list*/);
        Enumeration::EnumEntry const  * __en_rename_at_501_55;
        __need_loop_501 = __en_iterator.first(__context__,(__en_rename_at_501_55)) && __need_loop_501;
        for ( ; __need_loop_501 ; __need_loop_501 = __en_iterator.next(__context__,(__en_rename_at_501_55)) )
        {
            TArray<char *> __line_rename_at_502_56; das_zero(__line_rename_at_502_56);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_502_56),((char * const )(to_das_string((*__en_rename_at_501_55).name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_502_56),_FuncastTickdescribeTick842554968825501494_452cfc8955963a18(__context__,(*__en_rename_at_501_55).value /*value*/));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_500_54),das_arg<TArray<char *>>::pass(__line_rename_at_502_56));
        }
        __en_iterator.close(__context__,(__en_rename_at_501_55));
    };
    builtin_fwrite(__doc_file_rename_at_496_51,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_500_54),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_496_51,topic_9659659d94ddb29a(__context__,((char *) "enumeration"),__mod_rename_at_496_52,((char * const )(to_das_string(__value_rename_at_496_53->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void _FuncrstTickdocument_class_methodTick12589992380795293570_b6d1264d11ee8d48 ( Context * __context__, FILE const  * const  __doc_file_rename_at_573_57, Module * const  __mod_rename_at_573_58, smart_ptr<Structure> const  __value_rename_at_573_59, Structure::FieldDeclaration &  __fld_rename_at_573_60 )
{
    TArray<char *> __argNames_rename_at_574_61_ConstRef; das_zero(__argNames_rename_at_574_61_ConstRef); das_move(__argNames_rename_at_574_61_ConstRef, ((TArray<char *>)das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_574_21_rename_at_574_62; das_zero(____acomp_574_21_rename_at_574_62);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_574_21_rename_at_574_62),false);
        {
            bool __need_loop_574 = true;
            // arg: $::das_string&
            das_iterator<das::vector<das::string>> __arg_iterator(__fld_rename_at_573_60.type /*_type*/->argNames /*argNames*/);
            das::string * __arg_rename_at_574_63;
            __need_loop_574 = __arg_iterator.first(__context__,(__arg_rename_at_574_63)) && __need_loop_574;
            for ( ; __need_loop_574 ; __need_loop_574 = __arg_iterator.next(__context__,(__arg_rename_at_574_63)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(____acomp_574_21_rename_at_574_62),((char * const )(to_das_string(das_arg<das::string>::pass((*__arg_rename_at_574_63)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_574_63));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_574_21_rename_at_574_62),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_574_21_rename_at_574_62);
    })));
    TArray<char *> const  & __argNames_rename_at_574_61 = __argNames_rename_at_574_61_ConstRef; ;
    TArray<TypeDecl *> __argTypes_rename_at_575_64_ConstRef; das_zero(__argTypes_rename_at_575_64_ConstRef); das_move(__argTypes_rename_at_575_64_ConstRef, ((TArray<TypeDecl *>)das_invoke<TArray<TypeDecl *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<TypeDecl *>{
        TArray<TypeDecl *> ____acomp_575_21_rename_at_575_65; das_zero(____acomp_575_21_rename_at_575_65);
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_575_21_rename_at_575_65),false);
        {
            bool __need_loop_575 = true;
            // arg: smart_ptr<ast::TypeDecl>&
            das_iterator<das::vector<smart_ptr<TypeDecl>>> __arg_iterator(__fld_rename_at_573_60.type /*_type*/->argTypes /*argTypes*/);
            smart_ptr<TypeDecl> * __arg_rename_at_575_66;
            __need_loop_575 = __arg_iterator.first(__context__,(__arg_rename_at_575_66)) && __need_loop_575;
            for ( ; __need_loop_575 ; __need_loop_575 = __arg_iterator.next(__context__,(__arg_rename_at_575_66)) )
            {
                _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66(__context__,das_arg<TArray<TypeDecl *>>::pass(____acomp_575_21_rename_at_575_65),_FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e(__context__,(*__arg_rename_at_575_66)));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_575_66));
        };
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_575_21_rename_at_575_65),true);
        return /* <- */ das_auto_cast_move<TArray<TypeDecl *>>::cast(____acomp_575_21_rename_at_575_65);
    })));
    TArray<TypeDecl *> const  & __argTypes_rename_at_575_64 = __argTypes_rename_at_575_64_ConstRef; ;
    document_function_declaration_4c978976b1aaa270(__context__,__doc_file_rename_at_573_57,((char *) "das:function"),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_8, cast<das::string const  &>::from(__value_rename_at_573_59->name /*name*/), cast<char * const >::from(((char *) ".")), cast<das::string &>::from(__fld_rename_at_573_60.name /*name*/))),__argNames_rename_at_574_61,__argTypes_rename_at_575_64,__fld_rename_at_573_60.type /*_type*/->firstType /*firstType*/);
    if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fld_rename_at_573_60.type /*_type*/->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr))) && !(((das_deref(__context__,__fld_rename_at_573_60.type /*_type*/->firstType /*firstType*/)).isVoid())) )
    {
        builtin_fwrite(__doc_file_rename_at_573_57,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_9, cast<das::string &>::from(__fld_rename_at_573_60.name /*name*/), cast<char * const >::from(((char *) " returns ")), cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__fld_rename_at_573_60.type /*_type*/->firstType /*firstType*/)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( builtin_array_size(__argTypes_rename_at_575_64) > 1 )
    {
        document_function_arguments_e7469e75af4ed246(__context__,__doc_file_rename_at_573_57,__argNames_rename_at_574_61,__argTypes_rename_at_575_64);
    };
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_573_57,topic_9659659d94ddb29a(__context__,((char *) "method"),__mod_rename_at_573_58,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<das::string const  &>::from(__value_rename_at_573_59->name /*name*/), cast<char * const >::from(((char *) ".")), cast<das::string &>::from(__fld_rename_at_573_60.name /*name*/)))));
}

inline void _FuncrstTickdocument_structure_annotationTick2734840712723762798_45f2db6d6c9372ba ( Context * __context__, FILE const  * const  __doc_file_rename_at_673_67, Module * const  __mod_rename_at_673_68, Annotation const  &  __value_rename_at_673_69 ) { das_stack_prologue __prologue(__context__,576,"rst`document_structure_annotation`2734840712723762798 " DAS_FILE_LINE);
{
    bool __any_fields_rename_at_677_70; memset(&__any_fields_rename_at_677_70,0,sizeof(__any_fields_rename_at_677_70));
    bool __any_props_rename_at_678_71; memset(&__any_props_rename_at_678_71,0,sizeof(__any_props_rename_at_678_71));
    BasicStructureAnnotation * __ann_rename_at_679_72; memset(&__ann_rename_at_679_72,0,sizeof(__ann_rename_at_679_72));
    TArray<TArray<char *>> __tab_rename_at_688_77; memset(&__tab_rename_at_688_77,0,sizeof(__tab_rename_at_688_77));
    TArray<TArray<char *>> __tab_rename_at_701_83; memset(&__tab_rename_at_701_83,0,sizeof(__tab_rename_at_701_83));
    TArray<TArray<char *>> __props_rename_at_713_89; memset(&__props_rename_at_713_89,0,sizeof(__props_rename_at_713_89));
    smart_ptr_raw<TypeDecl> __annT_rename_at_714_90; memset(&__annT_rename_at_714_90,0,sizeof(__annT_rename_at_714_90));
    /* finally */ auto __finally_673= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__annT_rename_at_714_90);
    /* end finally */ });
    builtin_fwrite(__doc_file_rename_at_673_67,make_label_3368ba1f933c2731(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7(__context__,__value_rename_at_673_69)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_673_67,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_673_67,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_11, cast<das::string const  &>::from(__value_rename_at_673_69.name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    __any_fields_rename_at_677_70 = false;
    __any_props_rename_at_678_71 = false;
    __ann_rename_at_679_72 = das_cast<BasicStructureAnnotation *>::cast(__value_rename_at_673_69);
    builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_679_72)),das_make_block<void,char *,char *,smart_ptr<TypeDecl>,uint32_t>(__context__,128,0,&__func_info__c13357a1bc201428,[&](char * __name_rename_at_680_73, char * __cppName_rename_at_680_74, smart_ptr<TypeDecl> __xtype_rename_at_680_75, uint32_t __offset_rename_at_680_76) -> void{
        if ( __offset_rename_at_680_76 != 0xffffffffu )
        {
            das_copy(__any_fields_rename_at_677_70,true);
        } else {
            das_copy(__any_props_rename_at_678_71,true);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __any_fields_rename_at_677_70 )
    {
        __tab_rename_at_688_77; das_zero(__tab_rename_at_688_77);
        builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_679_72)),das_make_block<void,char *,char *,smart_ptr<TypeDecl>,uint32_t>(__context__,224,0,&__func_info__c13357a1bc201428,[&](char * __name_rename_at_689_78, char * __cppName_rename_at_689_79, smart_ptr<TypeDecl> __xtype_rename_at_689_80, uint32_t __offset_rename_at_689_81) -> void{
            if ( __offset_rename_at_689_81 != 0xffffffffu )
            {
                TArray<char *> __line_rename_at_691_82; das_zero(__line_rename_at_691_82);
                _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_691_82),__name_rename_at_689_78);
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_691_82),describe_type_a9bac10014c763b2(__context__,__xtype_rename_at_689_80));
                _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_688_77),das_arg<TArray<char *>>::pass(__line_rename_at_691_82));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_67,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_12, cast<das::string const  &>::from(__value_rename_at_673_69.name /*name*/), cast<char * const >::from(((char *) " fields are\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_67,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_688_77),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( __any_props_rename_at_678_71 )
    {
        __tab_rename_at_701_83; das_zero(__tab_rename_at_701_83);
        builtin_structure_for_each_field(das_arg<BasicStructureAnnotation>::pass(das_deref(__context__,__ann_rename_at_679_72)),das_make_block<void,char *,char *,smart_ptr<TypeDecl>,uint32_t>(__context__,352,0,&__func_info__c13357a1bc201428,[&](char * __name_rename_at_702_84, char * __cppName_rename_at_702_85, smart_ptr<TypeDecl> __xtype_rename_at_702_86, uint32_t __offset_rename_at_702_87) -> void{
            if ( __offset_rename_at_702_87 == 0xffffffffu )
            {
                TArray<char *> __line_rename_at_704_88; das_zero(__line_rename_at_704_88);
                _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_704_88),__name_rename_at_702_84);
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_704_88),describe_type_a9bac10014c763b2(__context__,__xtype_rename_at_702_86));
                _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_701_83),das_arg<TArray<char *>>::pass(__line_rename_at_704_88));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_67,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<das::string const  &>::from(__value_rename_at_673_69.name /*name*/), cast<char * const >::from(((char *) " properties are\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_67,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_701_83),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    __props_rename_at_713_89; das_zero(__props_rename_at_713_89);
    __annT_rename_at_714_90; das_zero(__annT_rename_at_714_90); das_move(__annT_rename_at_714_90, das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_714) {
        das_copy((__mks_714.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tHandle));
        das_copy((__mks_714.annotation /*annotation*/),(das_cast<TypeAnnotation *>::cast(__value_rename_at_673_69)));
    }))));
    forEachFunction(__mod_rename_at_673_68,nullptr,das_make_block<void,smart_ptr<Function>>(__context__,512,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_716_91) -> void{
        if ( !das_get_bitfield(__func_rename_at_716_91->moreFlags /*moreFlags*/,1u << 12) )
        {
            return ;
        };
        if ( das_vector_length(das_arg<das::vector<smart_ptr<Variable>>>::pass(__func_rename_at_716_91->arguments /*arguments*/)) != 1 )
        {
            return ;
        };
        char * __fna_rename_at_723_92 = ((char *)(char *)(((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_716_91->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
        if ( !(builtin_string_startswith(__fna_rename_at_723_92,((char *) ".`"),__context__)) || (builtin_string_length(__fna_rename_at_723_92,__context__) <= 2) )
        {
            return ;
        };
        if ( !isSameAstType(das_index<das::vector<smart_ptr<Variable>>>::at(__func_rename_at_716_91->arguments /*arguments*/,0,__context__)->type /*_type*/,__annT_rename_at_714_90,DAS_COMMENT(bound_enum) das::RefMatters::no,DAS_COMMENT(bound_enum) das::ConstMatters::no,DAS_COMMENT(bound_enum) das::TemporaryMatters::no,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            return ;
        };
        TArray<char *> __line_rename_at_730_93; das_zero(__line_rename_at_730_93);
        _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_730_93),((char * const )(builtin_string_slice2(__fna_rename_at_723_92,2,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_730_93),describe_type_a9bac10014c763b2(__context__,__func_rename_at_716_91->result /*result*/));
        _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__props_rename_at_713_89),das_arg<TArray<char *>>::pass(__line_rename_at_730_93));
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !_FuncbuiltinTickemptyTick15399874715904164783_60d4e412e550e258(__context__,das_arg<TArray<TArray<char *>>>::pass(__props_rename_at_713_89)) )
    {
        builtin_fwrite(__doc_file_rename_at_673_67,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_14, cast<das::string const  &>::from(__value_rename_at_673_69.name /*name*/), cast<char * const >::from(((char *) " property operators are\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fwrite(__doc_file_rename_at_673_67,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__props_rename_at_713_89),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_673_67,topic_9659659d94ddb29a(__context__,((char *) "structure_annotation"),__mod_rename_at_673_68,((char * const )(to_das_string(__value_rename_at_673_69.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}}

inline void _FuncrstTickdocument_function_annotationTick9878220666429632644_62156d6269c1aee6 ( Context * __context__, FILE const  * const  __doc_file_rename_at_762_94, Module * const  __mod_rename_at_762_95, Annotation const  &  __value_rename_at_762_96 )
{
    builtin_fwrite(__doc_file_rename_at_762_94,make_label_3368ba1f933c2731(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7(__context__,__value_rename_at_762_96)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_762_94,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_762_94,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_15, cast<das::string const  &>::from(__value_rename_at_762_96.name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_762_94,topic_9659659d94ddb29a(__context__,((char *) "function_annotation"),__mod_rename_at_762_95,((char * const )(to_das_string(__value_rename_at_762_96.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void _FuncrstTickdocument_annotationTick7679268031359399902_8b47c8f555ccae04 ( Context * __context__, FILE const  * const  __doc_file_rename_at_789_97, Module * const  __mod_rename_at_789_98, Annotation const  &  __value_rename_at_789_99 )
{
    builtin_fwrite(__doc_file_rename_at_789_97,make_label_3368ba1f933c2731(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7(__context__,__value_rename_at_789_99)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_789_97,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_789_97,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_16, cast<das::string const  &>::from(__value_rename_at_789_99.name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_789_97,topic_9659659d94ddb29a(__context__,((char *) "any_annotation"),__mod_rename_at_789_98,((char * const )(to_das_string(__value_rename_at_789_99.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void _FuncrstTickdocument_structure_macroTick6088634580083749384_a8cf27101dd30a50 ( Context * __context__, FILE const  * const  __doc_file_rename_at_819_100, Module * const  __mod_rename_at_819_101, Annotation const  &  __value_rename_at_819_102 )
{
    builtin_fwrite(__doc_file_rename_at_819_100,make_label_3368ba1f933c2731(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7(__context__,__value_rename_at_819_102)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_819_100,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_819_100,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_17, cast<das::string const  &>::from(__value_rename_at_819_102.name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_819_100,topic_9659659d94ddb29a(__context__,((char *) "structure_macro"),__mod_rename_at_819_101,((char * const )(to_das_string(__value_rename_at_819_102.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline Function * _FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615 ( Context * __context__, smart_ptr<Function> __src_rename_at_1733_103 )
{
    return das_auto_cast<Function *>::cast(das_cast<Function *>::cast(__src_rename_at_1733_103));
}

inline void _FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1008_104, Module * const  __mod_rename_at_1008_105, smart_ptr_raw<Function> __func_rename_at_1008_106 )
{
    TArray<char *> __argNames_rename_at_1009_107_ConstRef; das_zero(__argNames_rename_at_1009_107_ConstRef); das_move(__argNames_rename_at_1009_107_ConstRef, ((TArray<char *>)das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_1009_21_rename_at_1009_108; das_zero(____acomp_1009_21_rename_at_1009_108);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_108),false);
        {
            bool __need_loop_1009 = true;
            // arg: smart_ptr<ast::Variable>&
            das_iterator<das::vector<smart_ptr<Variable>>> __arg_iterator(__func_rename_at_1008_106->arguments /*arguments*/);
            smart_ptr<Variable> * __arg_rename_at_1009_109;
            __need_loop_1009 = __arg_iterator.first(__context__,(__arg_rename_at_1009_109)) && __need_loop_1009;
            for ( ; __need_loop_1009 ; __need_loop_1009 = __arg_iterator.next(__context__,(__arg_rename_at_1009_109)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_108),((char * const )(to_das_string(das_arg<das::string>::pass((*__arg_rename_at_1009_109)->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_1009_109));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_108),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_1009_21_rename_at_1009_108);
    })));
    TArray<char *> const  & __argNames_rename_at_1009_107 = __argNames_rename_at_1009_107_ConstRef; ;
    TArray<TypeDecl *> __argTypes_rename_at_1010_110_ConstRef; das_zero(__argTypes_rename_at_1010_110_ConstRef); das_move(__argTypes_rename_at_1010_110_ConstRef, ((TArray<TypeDecl *>)das_invoke<TArray<TypeDecl *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<TypeDecl *>{
        TArray<TypeDecl *> ____acomp_1010_21_rename_at_1010_111; das_zero(____acomp_1010_21_rename_at_1010_111);
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_111),false);
        {
            bool __need_loop_1010 = true;
            // arg: smart_ptr<ast::Variable>&
            das_iterator<das::vector<smart_ptr<Variable>>> __arg_iterator(__func_rename_at_1008_106->arguments /*arguments*/);
            smart_ptr<Variable> * __arg_rename_at_1010_112;
            __need_loop_1010 = __arg_iterator.first(__context__,(__arg_rename_at_1010_112)) && __need_loop_1010;
            for ( ; __need_loop_1010 ; __need_loop_1010 = __arg_iterator.next(__context__,(__arg_rename_at_1010_112)) )
            {
                _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66(__context__,das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_111),_FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e(__context__,(*__arg_rename_at_1010_112)->type /*_type*/));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_1010_112));
        };
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_111),true);
        return /* <- */ das_auto_cast_move<TArray<TypeDecl *>>::cast(____acomp_1010_21_rename_at_1010_111);
    })));
    TArray<TypeDecl *> const  & __argTypes_rename_at_1010_110 = __argTypes_rename_at_1010_110_ConstRef; ;
    builtin_fwrite(__doc_file_rename_at_1008_104,make_label_3368ba1f933c2731(__context__,_FuncrstTickfunction_label_nameTick4951524166161443384_aafe521f2cb4dc50(__context__,__func_rename_at_1008_106)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_function_declaration_4c978976b1aaa270(__context__,__doc_file_rename_at_1008_104,((char *) "das:function"),function_name_ee62641d633d27c8(__context__,((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_1008_106->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),__argNames_rename_at_1009_107,__argTypes_rename_at_1010_110,__func_rename_at_1008_106->result /*result*/);
    if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__func_rename_at_1008_106->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) && !(((das_deref(__context__,__func_rename_at_1008_106->result /*result*/)).isVoid())) )
    {
        builtin_fwrite(__doc_file_rename_at_1008_104,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_18, cast<das::string &>::from(__func_rename_at_1008_106->name /*name*/), cast<char * const >::from(((char *) " returns ")), cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__func_rename_at_1008_106->result /*result*/)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( das_get_bitfield(__func_rename_at_1008_106->flags /*flags*/,1u << 14) )
    {
        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1008_104,((char *) "This is unsafe operation."));
    };
    if ( das_get_bitfield(__func_rename_at_1008_106->moreFlags /*moreFlags*/,1u << 8) )
    {
        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1008_104,((char *) "This function is deprecated."));
    };
    document_function_arguments_e7469e75af4ed246(__context__,__doc_file_rename_at_1008_104,__argNames_rename_at_1009_107,__argTypes_rename_at_1010_110);
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_1008_104,topic_9659659d94ddb29a(__context__,((char *) "function"),__mod_rename_at_1008_105,das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_19, cast<char * const >::from(_FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b(__context__,__func_rename_at_1008_106))))));
}

inline rst::DocGroup & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_c745a97595700ef ( Context * __context__, rst::DocGroup &  __a_rename_at_50_113 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_20,cast<rst::DocGroup &>::from(__a_rename_at_50_113)));
    return das_auto_cast_ref<rst::DocGroup &>::cast(__a_rename_at_50_113);
}

inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_69_114 )
{
    TArray<TTuple<16,char *,char *>> _temp_make_local_72_32_48; _temp_make_local_72_32_48;
    TDim<TTuple<16,char *,char *>,1> _temp_make_local_72_32_80; _temp_make_local_72_32_80;
    char * __mn_rename_at_71_115 = (char *)(((char * const )(to_das_string(__value_rename_at_69_114->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(__mn_rename_at_71_115,das_invoke_function<char * const >::invoke<char * const ,TArray<TTuple<16,char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 5840488413950466740u)),__mn_rename_at_71_115,das_arg<TArray<TTuple<16,char *,char *>>>::pass((_temp_make_local_72_32_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf(__context__,das_arg<TDim<TTuple<16,char *,char *>,1>>::pass((([&]() -> TDim<TTuple<16,char *,char *>,1>& {
        _temp_make_local_72_32_80(0,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_73;
            das_get_tuple_field<char *,0>::get(__mkt_73) = ((char *) "?");
            das_get_tuple_field<char *,8>::get(__mkt_73) = ((char *) "_qm_");
            return __mkt_73;
        })());
        return _temp_make_local_72_32_80;
    })()))))))));
    return das_auto_cast<char *>::cast(__mn_rename_at_71_115);
}

inline bool _Func_lambda_rst_1216_1Tickfunction_deb6e09d1773cc6a ( Context * __context__, rst::_lambda_rst_1216_1 &  ____this_rename_at_1216_116, char * * & ___ryield1216_rename_at_1216_117 )
{
    switch (____this_rename_at_1216_116.__yield) {
    case 0: goto label_0;
    case 2: goto label_2;
    case 1: goto label_1;
    case 3: goto label_3;
    case 4: goto label_4;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    das_copy(____this_rename_at_1216_116._loop_at_1216_35,true);
    das_move(____this_rename_at_1216_116._source_0_at_1216_35,_FuncbuiltinTickeachTick4044333107478717573_19fa17f56443c23c(__context__,range(____this_rename_at_1216_116.index,builtin_array_size(das_arg<TArray<char *>>::pass(____this_rename_at_1216_116.snp)))));
    memset(&(____this_rename_at_1216_116.__i_rename_at_1216_39), 0, 4);
    das_copy(____this_rename_at_1216_116._pvar_0_at_1216_35,das_cast<void *>::cast(das_ref(__context__,____this_rename_at_1216_116.__i_rename_at_1216_39)));
    DAS_SETBOOLAND((____this_rename_at_1216_116._loop_at_1216_35),(builtin_iterator_first(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_1216_116._source_0_at_1216_35),____this_rename_at_1216_116._pvar_0_at_1216_35,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    label_2:;;
    if ( !____this_rename_at_1216_116._loop_at_1216_35 )
    {
        goto label_4;
    };
    das_copy(___ryield1216_rename_at_1216_117,das_ref(__context__,____this_rename_at_1216_116.snp(____this_rename_at_1216_116.__i_rename_at_1216_39,__context__)));
    das_copy(____this_rename_at_1216_116.__yield,1);
    return das_auto_cast<bool>::cast(true);
    label_1:;;
    label_3:;;
    DAS_SETBOOLAND((____this_rename_at_1216_116._loop_at_1216_35),(builtin_iterator_next(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_1216_116._source_0_at_1216_35),____this_rename_at_1216_116._pvar_0_at_1216_35,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    goto label_2;
    label_4:;;
    builtin_iterator_close(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_1216_116._source_0_at_1216_35),____this_rename_at_1216_116._pvar_0_at_1216_35,__context__);
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_rst_1216_1Tickfinalizer_d8783cc009d376af ( Context * __context__, rst::_lambda_rst_1216_1 * ____this_rename_at_1216_118 )
{
    finalize_5bf95341c40e5b82(__context__,das_arg<rst::_lambda_rst_1216_1>::pass(das_deref(__context__,____this_rename_at_1216_118)));
    das_delete_lambda_struct<rst::_lambda_rst_1216_1 *>::clear(__context__,____this_rename_at_1216_118);
}

inline char * _FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1 ( Context * __context__, Module * const  __mod_rename_at_28_119 )
{
    char * __mn_rename_at_29_120 = ((char *)(char *)(((char * const )(to_das_string(__mod_rename_at_28_119->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast((SimPolicy<char *>::Equ(cast<char *>::from(__mn_rename_at_29_120),cast<char *>::from(((char *) "$")),*__context__,nullptr)) ? das_auto_cast<char * const >::cast(((char *) "builtin")) : das_auto_cast<char * const >::cast(__mn_rename_at_29_120));
}

inline Variable * _FuncbuiltinTickget_ptrTick8468476673553620226_f1503a45e3ae58f3 ( Context * __context__, smart_ptr_raw<Variable> const  __src_rename_at_1727_121 )
{
    return das_auto_cast<Variable *>::cast(das_cast<Variable *>::cast(__src_rename_at_1727_121));
}

inline Enumeration * _FuncbuiltinTickget_ptrTick8468476673553620226_d6dabe47fb1a720 ( Context * __context__, smart_ptr_raw<Enumeration> const  __src_rename_at_1727_122 )
{
    return das_auto_cast<Enumeration *>::cast(das_cast<Enumeration *>::cast(__src_rename_at_1727_122));
}

inline Structure * _FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850 ( Context * __context__, smart_ptr_raw<Structure> const  __src_rename_at_1727_123 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1727_123));
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_37_124, bool __extra_rename_at_37_125, bool __contracts_rename_at_37_126, bool __modules_rename_at_37_127 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_37_124,__extra_rename_at_37_125,__contracts_rename_at_37_126,__modules_rename_at_37_127,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * _FuncrstTickhandle_label_nameTick13616998130381234091_7f23dc2205503d09 ( Context * __context__, TypeAnnotation * const  __value_rename_at_61_128 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_21, cast<char * const >::from(((char *) "handle-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__value_rename_at_61_128->module /*_module*/)), cast<char * const >::from(((char *) "-")), cast<das::string const  &>::from(__value_rename_at_61_128->name /*name*/))));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_68_129, int32_t __newSize_rename_at_68_130 )
{
    builtin_array_resize(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_68_129),__newSize_rename_at_68_130,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_131, char * const  __mode_rename_at_12_132, Block DAS_COMMENT((void,FILE const  * const )) const  &  __blk_rename_at_12_133 )
{
    FILE const  * __f_rename_at_13_134 = ((FILE const  *)builtin_fopen(__name_rename_at_12_131,__mode_rename_at_12_132));
    das_invoke<void>::invoke<FILE const  * const >(__context__,nullptr,__blk_rename_at_12_133,__f_rename_at_13_134);
    if ( __f_rename_at_13_134 != nullptr )
    {
        builtin_fclose(__f_rename_at_13_134,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_649bcd7e51242beb ( Context * __context__, TDim<char *,3> &  __a_rename_at_1343_135 )
{
    TArray<char *> __arr_rename_at_1345_136; das_zero(__arr_rename_at_1345_136);
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__arr_rename_at_1345_136),3);
    das_copy(das_cast<TDim<char *,3>>::cast(das_ref(__context__,__arr_rename_at_1345_136(0,__context__))),__a_rename_at_1343_135);
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__arr_rename_at_1345_136);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_137, char * const  __value_rename_at_165_138 )
{
    das_copy(__Arr_rename_at_165_137(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_137),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_138);
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6 ( Context * __context__, TArray<TArray<char *>> &  __Arr_rename_at_287_139, TArray<char *> &  __value_rename_at_287_140 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_22,cast<TArray<TArray<char *>> &>::from(__Arr_rename_at_287_139)));
    das_move(__Arr_rename_at_287_139(builtin_array_push_back(das_arg<TArray<TArray<char *>>>::pass(__Arr_rename_at_287_139),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_140);
}

inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_47_141 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_47_141,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline bool _FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6 ( Context * __context__, TypeDecl * const  __tt_rename_at_531_142 )
{
    return das_auto_cast<bool>::cast((__tt_rename_at_531_142->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::fakeContext) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((__tt_rename_at_531_142->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::fakeLineInfo) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))));
}

inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_325c63fe2a174cf6 ( Context * __context__, TDim<char *,2> &  __a_rename_at_1343_143 )
{
    TArray<char *> __arr_rename_at_1345_144; das_zero(__arr_rename_at_1345_144);
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__arr_rename_at_1345_144),2);
    das_copy(das_cast<TDim<char *,2>>::cast(das_ref(__context__,__arr_rename_at_1345_144(0,__context__))),__a_rename_at_1343_143);
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__arr_rename_at_1345_144);
}

inline char * _FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c ( Context * __context__, Module * const  __mod_rename_at_65_145, char * const  __value_rename_at_65_146 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_23, cast<char * const >::from(((char *) "call-macro-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__mod_rename_at_65_145)), cast<char * const >::from(((char *) "-")), cast<char * const >::from(__value_rename_at_65_146))));
}

inline void _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a ( Context * __context__, TTable<Function *,bool> &  __Tab_rename_at_948_147, Function * const  __at_rename_at_948_148, bool __val_rename_at_948_149 )
{
    das_copy(__Tab_rename_at_948_147(__at_rename_at_948_148,__context__),__val_rename_at_948_149);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_4e6333822225d431 ( Context * __context__, TTable<Function *,bool> const  &  __Tab_rename_at_1015_150, Function * const  __at_rename_at_1015_151 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1015_150,__at_rename_at_1015_151));
}

inline char * _Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f ( Context * __context__, Function * const  __func_rename_at_89_152 )
{
    TArray<char *> __args_rename_at_90_153; das_zero(__args_rename_at_90_153); das_move(__args_rename_at_90_153, das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_90_17_rename_at_90_154; das_zero(____acomp_90_17_rename_at_90_154);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_90_17_rename_at_90_154),false);
        {
            bool __need_loop_90 = true;
            // x: smart_ptr<ast::Variable> const&
            das_iterator<das::vector<smart_ptr<Variable>> const > __x_iterator(__func_rename_at_89_152->arguments /*arguments*/);
            smart_ptr<Variable> const  * __x_rename_at_90_155;
            __need_loop_90 = __x_iterator.first(__context__,(__x_rename_at_90_155)) && __need_loop_90;
            for ( ; __need_loop_90 ; __need_loop_90 = __x_iterator.next(__context__,(__x_rename_at_90_155)) )
            {
                _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1(__context__,das_arg<TArray<char *>>::pass(____acomp_90_17_rename_at_90_154),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_24, cast<das::string const  &>::from((*__x_rename_at_90_155)->name /*name*/), cast<char * const >::from(((char *) ":")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,(*__x_rename_at_90_155)->type /*_type*/,false,true,true)))));
            }
            __x_iterator.close(__context__,(__x_rename_at_90_155));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_90_17_rename_at_90_154),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_90_17_rename_at_90_154);
    }));
    char * __res_rename_at_91_156 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_25, cast<das::string const  &>::from(__func_rename_at_89_152->name /*name*/), cast<char * const >::from(((char *) " (")), cast<char * const >::from(_Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835(__context__,das_arg<TArray<char *>>::pass(__args_rename_at_90_153),((char *) ";"))), cast<char * const >::from(((char *) ") : ")), cast<char * const >::from(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__func_rename_at_89_152->result /*result*/,false,true,true))))));
    _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(__args_rename_at_90_153));
    return das_auto_cast<char *>::cast(__res_rename_at_91_156);
}

inline char * _FuncrstTickfunction_label_nameTick4951524166161443384_ded37d7d287c25b5 ( Context * __context__, Function * const  __value_rename_at_77_157 )
{
    TArray<TTuple<16,char *,char *>> _temp_make_local_79_32_48; _temp_make_local_79_32_48;
    TDim<TTuple<16,char *,char *>,13> _temp_make_local_79_32_80; _temp_make_local_79_32_80;
    char * __mn_rename_at_78_158 = (char *)(das_invoke_function<char * const >::invoke<Function * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast::get_mangled_name C1<H<ast::Function>>?*/ 8436048561986127392u)),__value_rename_at_77_157));
    das_copy(__mn_rename_at_78_158,das_invoke_function<char * const >::invoke<char * const ,TArray<TTuple<16,char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 5840488413950466740u)),__mn_rename_at_78_158,das_arg<TArray<TTuple<16,char *,char *>>>::pass((_temp_make_local_79_32_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f(__context__,das_arg<TDim<TTuple<16,char *,char *>,13>>::pass((([&]() -> TDim<TTuple<16,char *,char *>,13>& {
        _temp_make_local_79_32_80(0,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_80;
            das_get_tuple_field<char *,0>::get(__mkt_80) = ((char *) " ");
            das_get_tuple_field<char *,8>::get(__mkt_80) = ((char *) "_");
            return __mkt_80;
        })());
        _temp_make_local_79_32_80(1,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_81;
            das_get_tuple_field<char *,0>::get(__mkt_81) = ((char *) "$");
            das_get_tuple_field<char *,8>::get(__mkt_81) = ((char *) "_builtin_");
            return __mkt_81;
        })());
        _temp_make_local_79_32_80(2,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_82;
            das_get_tuple_field<char *,0>::get(__mkt_82) = ((char *) "#");
            das_get_tuple_field<char *,8>::get(__mkt_82) = ((char *) "_hh_");
            return __mkt_82;
        })());
        _temp_make_local_79_32_80(3,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_83;
            das_get_tuple_field<char *,0>::get(__mkt_83) = ((char *) ":");
            das_get_tuple_field<char *,8>::get(__mkt_83) = ((char *) "_c_");
            return __mkt_83;
        })());
        _temp_make_local_79_32_80(4,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_84;
            das_get_tuple_field<char *,0>::get(__mkt_84) = ((char *) "@");
            das_get_tuple_field<char *,8>::get(__mkt_84) = ((char *) "_at_");
            return __mkt_84;
        })());
        _temp_make_local_79_32_80(5,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_85;
            das_get_tuple_field<char *,0>::get(__mkt_85) = ((char *) "<");
            das_get_tuple_field<char *,8>::get(__mkt_85) = ((char *) "_ls_");
            return __mkt_85;
        })());
        _temp_make_local_79_32_80(6,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_86;
            das_get_tuple_field<char *,0>::get(__mkt_86) = ((char *) ">");
            das_get_tuple_field<char *,8>::get(__mkt_86) = ((char *) "_gr_");
            return __mkt_86;
        })());
        _temp_make_local_79_32_80(7,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_87;
            das_get_tuple_field<char *,0>::get(__mkt_87) = ((char *) "`");
            das_get_tuple_field<char *,8>::get(__mkt_87) = ((char *) "_rq_");
            return __mkt_87;
        })());
        _temp_make_local_79_32_80(8,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_88;
            das_get_tuple_field<char *,0>::get(__mkt_88) = ((char *) "!");
            das_get_tuple_field<char *,8>::get(__mkt_88) = ((char *) "_ex_");
            return __mkt_88;
        })());
        _temp_make_local_79_32_80(9,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_89;
            das_get_tuple_field<char *,0>::get(__mkt_89) = ((char *) "=");
            das_get_tuple_field<char *,8>::get(__mkt_89) = ((char *) "_eq_");
            return __mkt_89;
        })());
        _temp_make_local_79_32_80(10,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_90;
            das_get_tuple_field<char *,0>::get(__mkt_90) = ((char *) "?");
            das_get_tuple_field<char *,8>::get(__mkt_90) = ((char *) "_qm_");
            return __mkt_90;
        })());
        _temp_make_local_79_32_80(11,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_91;
            das_get_tuple_field<char *,0>::get(__mkt_91) = ((char *) "[");
            das_get_tuple_field<char *,8>::get(__mkt_91) = ((char *) "_lb_");
            return __mkt_91;
        })());
        _temp_make_local_79_32_80(12,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_92;
            das_get_tuple_field<char *,0>::get(__mkt_92) = ((char *) "]");
            das_get_tuple_field<char *,8>::get(__mkt_92) = ((char *) "_rb_");
            return __mkt_92;
        })());
        return _temp_make_local_79_32_80;
    })()))))))));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_26, cast<char * const >::from(((char *) "function-")), cast<char *>::from(__mn_rename_at_78_158))));
}

inline bool _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72 ( Context * __context__, TTable<Function *,bool> &  __Tab_rename_at_742_159, Function * const  __at_rename_at_742_160 )
{
    return das_auto_cast<bool>::cast(__Tab_rename_at_742_159(__at_rename_at_742_160,__context__));
}

inline void _FuncrstTickdocument_functionTick8829733267459955112_767667cb9ac814a7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_1008_161, Module * const  __mod_rename_at_1008_162, Function * __func_rename_at_1008_163 )
{
    TArray<char *> __argNames_rename_at_1009_164_ConstRef; das_zero(__argNames_rename_at_1009_164_ConstRef); das_move(__argNames_rename_at_1009_164_ConstRef, ((TArray<char *>)das_invoke<TArray<char *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<char *>{
        TArray<char *> ____acomp_1009_21_rename_at_1009_165; das_zero(____acomp_1009_21_rename_at_1009_165);
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_165),false);
        {
            bool __need_loop_1009 = true;
            // arg: smart_ptr<ast::Variable>&
            das_iterator<das::vector<smart_ptr<Variable>>> __arg_iterator(__func_rename_at_1008_163->arguments /*arguments*/);
            smart_ptr<Variable> * __arg_rename_at_1009_166;
            __need_loop_1009 = __arg_iterator.first(__context__,(__arg_rename_at_1009_166)) && __need_loop_1009;
            for ( ; __need_loop_1009 ; __need_loop_1009 = __arg_iterator.next(__context__,(__arg_rename_at_1009_166)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_165),((char * const )(to_das_string(das_arg<das::string>::pass((*__arg_rename_at_1009_166)->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_1009_166));
        };
        builtin_set_verify_array_locks(das_arg<TArray<char *>>::pass(____acomp_1009_21_rename_at_1009_165),true);
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(____acomp_1009_21_rename_at_1009_165);
    })));
    TArray<char *> const  & __argNames_rename_at_1009_164 = __argNames_rename_at_1009_164_ConstRef; ;
    TArray<TypeDecl *> __argTypes_rename_at_1010_167_ConstRef; das_zero(__argTypes_rename_at_1010_167_ConstRef); das_move(__argTypes_rename_at_1010_167_ConstRef, ((TArray<TypeDecl *>)das_invoke<TArray<TypeDecl *>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<TypeDecl *>{
        TArray<TypeDecl *> ____acomp_1010_21_rename_at_1010_168; das_zero(____acomp_1010_21_rename_at_1010_168);
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_168),false);
        {
            bool __need_loop_1010 = true;
            // arg: smart_ptr<ast::Variable>&
            das_iterator<das::vector<smart_ptr<Variable>>> __arg_iterator(__func_rename_at_1008_163->arguments /*arguments*/);
            smart_ptr<Variable> * __arg_rename_at_1010_169;
            __need_loop_1010 = __arg_iterator.first(__context__,(__arg_rename_at_1010_169)) && __need_loop_1010;
            for ( ; __need_loop_1010 ; __need_loop_1010 = __arg_iterator.next(__context__,(__arg_rename_at_1010_169)) )
            {
                _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66(__context__,das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_168),_FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e(__context__,(*__arg_rename_at_1010_169)->type /*_type*/));
            }
            __arg_iterator.close(__context__,(__arg_rename_at_1010_169));
        };
        builtin_set_verify_array_locks(das_arg<TArray<TypeDecl *>>::pass(____acomp_1010_21_rename_at_1010_168),true);
        return /* <- */ das_auto_cast_move<TArray<TypeDecl *>>::cast(____acomp_1010_21_rename_at_1010_168);
    })));
    TArray<TypeDecl *> const  & __argTypes_rename_at_1010_167 = __argTypes_rename_at_1010_167_ConstRef; ;
    builtin_fwrite(__doc_file_rename_at_1008_161,make_label_3368ba1f933c2731(__context__,_FuncrstTickfunction_label_nameTick4951524166161443384_ded37d7d287c25b5(__context__,__func_rename_at_1008_163)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_function_declaration_4c978976b1aaa270(__context__,__doc_file_rename_at_1008_161,((char *) "das:function"),function_name_ee62641d633d27c8(__context__,((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_1008_163->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),__argNames_rename_at_1009_164,__argTypes_rename_at_1010_167,__func_rename_at_1008_163->result /*result*/);
    if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__func_rename_at_1008_163->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) && !(((das_deref(__context__,__func_rename_at_1008_163->result /*result*/)).isVoid())) )
    {
        builtin_fwrite(__doc_file_rename_at_1008_161,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_27, cast<das::string &>::from(__func_rename_at_1008_163->name /*name*/), cast<char * const >::from(((char *) " returns ")), cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__func_rename_at_1008_163->result /*result*/)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( das_get_bitfield(__func_rename_at_1008_163->flags /*flags*/,1u << 14) )
    {
        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1008_161,((char *) "This is unsafe operation."));
    };
    if ( das_get_bitfield(__func_rename_at_1008_163->moreFlags /*moreFlags*/,1u << 8) )
    {
        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1008_161,((char *) "This function is deprecated."));
    };
    document_function_arguments_e7469e75af4ed246(__context__,__doc_file_rename_at_1008_161,__argNames_rename_at_1009_164,__argTypes_rename_at_1010_167);
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_1008_161,topic_9659659d94ddb29a(__context__,((char *) "function"),__mod_rename_at_1008_162,das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_28, cast<char * const >::from(_FuncrstTickfunction_file_nameTick11623859247758201540_f84a97f3b3efcea2(__context__,__func_rename_at_1008_163))))));
}

inline bool _FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45 ( Context * __context__, TTable<char *,bool> &  __Tab_rename_at_742_170, char * const  __at_rename_at_742_171 )
{
    return das_auto_cast<bool>::cast(__Tab_rename_at_742_170(__at_rename_at_742_171,__context__));
}

inline void _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b ( Context * __context__, TTable<char *,bool> &  __Tab_rename_at_948_172, char * const  __at_rename_at_948_173, bool __val_rename_at_948_174 )
{
    das_copy(__Tab_rename_at_948_172(__at_rename_at_948_173,__context__),__val_rename_at_948_174);
}

inline char * alias_label_name_f0e3a6da4e226a3e ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_33_175 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_29, cast<char * const >::from(((char *) "alias-")), cast<das::string const  &>::from(__td_rename_at_33_175->alias /*alias*/))));
}

inline char * global_label_name_2b7e96918b37c00e ( Context * __context__, Variable * const  __value_rename_at_37_176 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_30, cast<char * const >::from(((char *) "global-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__value_rename_at_37_176->module /*_module*/)), cast<char * const >::from(((char *) "-")), cast<das::string const  &>::from(__value_rename_at_37_176->name /*name*/))));
}

inline char * global_label_name_b721f72143d35230 ( Context * __context__, smart_ptr_raw<Variable> const  __value_rename_at_41_177 )
{
    return das_auto_cast<char *>::cast(global_label_name_2b7e96918b37c00e(__context__,_FuncbuiltinTickget_ptrTick8468476673553620226_f1503a45e3ae58f3(__context__,__value_rename_at_41_177)));
}

inline char * enum_label_name_a32575da767b3ecd ( Context * __context__, Enumeration * const  __value_rename_at_45_178 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_31, cast<char * const >::from(((char *) "enum-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__value_rename_at_45_178->module /*_module*/)), cast<char * const >::from(((char *) "-")), cast<das::string const  &>::from(__value_rename_at_45_178->name /*name*/))));
}

inline char * enum_label_name_768485a24af7de17 ( Context * __context__, smart_ptr_raw<Enumeration> const  __value_rename_at_49_179 )
{
    return das_auto_cast<char *>::cast(enum_label_name_a32575da767b3ecd(__context__,_FuncbuiltinTickget_ptrTick8468476673553620226_d6dabe47fb1a720(__context__,__value_rename_at_49_179)));
}

inline char * struct_label_name_7b7faf8095586da6 ( Context * __context__, Structure * const  __value_rename_at_53_180 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_32, cast<char * const >::from(((char *) "struct-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__value_rename_at_53_180->module /*_module*/)), cast<char * const >::from(((char *) "-")), cast<das::string const  &>::from(__value_rename_at_53_180->name /*name*/))));
}

inline char * struct_label_name_6ee1d24e90557aeb ( Context * __context__, smart_ptr_raw<Structure> const  __value_rename_at_57_181 )
{
    return das_auto_cast<char *>::cast(struct_label_name_7b7faf8095586da6(__context__,_FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850(__context__,__value_rename_at_57_181)));
}

inline char * describe_type_short_56370d8993e381ba ( Context * __context__, TypeDecl * const  __td_rename_at_96_182 )
{
    return das_auto_cast<char *>::cast(describe_type_short_847eafd8ddf6216(__context__,das_cast<smart_ptr_raw<TypeDecl>>::cast(__td_rename_at_96_182)));
}

inline char * describe_type_short_847eafd8ddf6216 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_102_183 )
{
    if ( (!(builtin_empty_das_string(__td_rename_at_102_183->alias /*alias*/)) && !(((das_deref(__context__,__td_rename_at_102_183)).isAuto()))) && !(((das_deref(__context__,__td_rename_at_102_183)).isAlias())) )
    {
        return das_auto_cast<char *>::cast(((char * const )(to_das_string(__td_rename_at_102_183->alias /*alias*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    } else {
        char * __tp_rename_at_106_184 = ((char *)(char *)(_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,__td_rename_at_102_183,false,false,false)));
        return das_auto_cast<char *>::cast(((char * const )(builtin_string_replace(__tp_rename_at_106_184,((char *) "$::"),nullptr,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    };
}

inline char * describe_type_cf1a091488868462 ( Context * __context__, TypeDecl * const  __td_rename_at_110_185 )
{
    return das_auto_cast<char *>::cast(describe_type_a9bac10014c763b2(__context__,das_cast<smart_ptr_raw<TypeDecl>>::cast(__td_rename_at_110_185)));
}

inline char * describe_type_a9bac10014c763b2 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __td_rename_at_116_186 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_117_187) DAS_AOT_INLINE_LAMBDA -> void{
        if ( (!(builtin_empty_das_string(__td_rename_at_116_186->alias /*alias*/)) && !(((das_deref(__context__,__td_rename_at_116_186)).isAuto()))) && !(((das_deref(__context__,__td_rename_at_116_186)).isAlias())) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_33,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,alias_label_name_f0e3a6da4e226a3e(__context__,__td_rename_at_116_186),((char * const )(to_das_string(__td_rename_at_116_186->alias /*alias*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))));
            return ;
        };
        DAS_COMMENT(bound_enum) das::Type __baseType_rename_at_122_188 = ((DAS_COMMENT(bound_enum) das::Type)__td_rename_at_116_186->baseType /*baseType*/);
        if ( __baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::alias )
        {
            if ( !builtin_empty_das_string(__td_rename_at_116_186->alias /*alias*/) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_34,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<das::string const  &>::from(__td_rename_at_116_186->alias /*alias*/)));
            } else {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_35,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "alias"))));
            };
        } else if ( __baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::autoinfer )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_36,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "auto"))));
            if ( !builtin_empty_das_string(__td_rename_at_116_186->alias /*alias*/) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_37,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_38, cast<char * const >::from(((char *) "(")), cast<das::string const  &>::from(__td_rename_at_116_186->alias /*alias*/), cast<char * const >::from(((char *) ")")))))));
            };
        } else if ( __baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tHandle )
        {
            char * __hname_rename_at_135_189 = ((char *)(char *)(((char * const )(to_das_string(__td_rename_at_116_186->annotation /*annotation*/->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
            if ( builtin_string_startswith(__hname_rename_at_135_189,((char *) "dasvector`smart_ptr`"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_39,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "vector<smart_ptr<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_40,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char * const )(builtin_string_slice2(__hname_rename_at_135_189,20,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_41,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ">>"))));
            } else if ( builtin_string_startswith(__hname_rename_at_135_189,((char *) "dasvector`"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_42,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "vector<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_43,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char * const )(builtin_string_slice2(__hname_rename_at_135_189,10,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_44,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ">"))));
            } else if ( builtin_string_startswith(__hname_rename_at_135_189,((char *) "pair`"),__context__) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_45,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "pair<"))));
                TArray<char *> __parts_rename_at_146_190; das_zero(__parts_rename_at_146_190); das_move(__parts_rename_at_146_190, das_invoke_function<TArray<char *>>::invoke_cmres<char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::split CIs CIs*/ 299216212070491835u)),__hname_rename_at_135_189,((char *) "`")));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_46,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char *>::from(__parts_rename_at_146_190(1,__context__))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_47,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ","))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_48,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char *>::from(__parts_rename_at_146_190(2,__context__))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_49,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ">"))));
            } else {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_50,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,_FuncrstTickhandle_label_nameTick13616998130381234091_7f23dc2205503d09(__context__,__td_rename_at_116_186->annotation /*annotation*/),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_51, cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__td_rename_at_116_186->annotation /*annotation*/->module /*_module*/)), cast<char * const >::from(((char *) "::")), cast<das::string const  &>::from(__td_rename_at_116_186->annotation /*annotation*/->name /*name*/)))))));
            };
        } else if ( __baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tStructure )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_52,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,struct_label_name_7b7faf8095586da6(__context__,__td_rename_at_116_186->structType /*structType*/),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_53, cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__td_rename_at_116_186->structType /*structType*/->module /*_module*/)), cast<char * const >::from(((char *) "::")), cast<das::string const  &>::from(__td_rename_at_116_186->structType /*structType*/->name /*name*/)))))));
        } else if ( (((__baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tEnumeration) || (__baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tEnumeration8)) || (__baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tEnumeration16)) || (__baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tEnumeration64) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_54,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,enum_label_name_a32575da767b3ecd(__context__,__td_rename_at_116_186->enumType /*enumType*/),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_55, cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__td_rename_at_116_186->enumType /*enumType*/->module /*_module*/)), cast<char * const >::from(((char *) "::")), cast<das::string const  &>::from(__td_rename_at_116_186->enumType /*enumType*/->name /*name*/)))))));
        } else if ( __baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tPointer )
        {
            if ( das_get_bitfield(__td_rename_at_116_186->flags /*flags*/,1u << 10) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_56,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "smart_ptr<"))));
            };
            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_186->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_57,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_186->firstType /*firstType*/))));
            } else {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_58,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "void"))));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_59,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(das_get_bitfield(__td_rename_at_116_186->flags /*flags*/,1u << 10) ? das_auto_cast<char * const >::cast(((char *) ">")) : das_auto_cast<char * const >::cast(((char *) "?")))));
        } else if ( __baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tArray )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_60,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "array"))));
            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_186->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_61,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_62,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_186->firstType /*firstType*/))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_63,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ">"))));
            };
        } else if ( __baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tTable )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_64,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "table"))));
            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_186->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_65,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_66,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_186->firstType /*firstType*/))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_67,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ";"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_68,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_186->secondType /*secondType*/))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_69,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ">"))));
            };
        } else if ( __baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tIterator )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_70,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "iterator"))));
            if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_186->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_71,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "<"))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_72,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_186->firstType /*firstType*/))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_73,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ">"))));
            };
        } else if ( ((__baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tBlock) || (__baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tFunction)) || (__baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tLambda) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_74,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char * const )(ast_das_to_string(__baseType_rename_at_122_188,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_75,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "<"))));
            if ( das_vector_length(__td_rename_at_116_186->argTypes /*argTypes*/) != 0 )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_76,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "("))));
                if ( das_vector_length(__td_rename_at_116_186->argNames /*argNames*/) != 0 )
                {
                    bool __first_rename_at_197_191 = true;
                    {
                        bool __need_loop_198 = true;
                        // at: smart_ptr<ast::TypeDecl> const&
                        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__td_rename_at_116_186->argTypes /*argTypes*/);
                        smart_ptr<TypeDecl> const  * __at_rename_at_198_194;
                        __need_loop_198 = __at_iterator.first(__context__,(__at_rename_at_198_194)) && __need_loop_198;
                        // an: $::das_string const&
                        das_iterator<das::vector<das::string> const > __an_iterator(__td_rename_at_116_186->argNames /*argNames*/);
                        das::string const  * __an_rename_at_198_195;
                        __need_loop_198 = __an_iterator.first(__context__,(__an_rename_at_198_195)) && __need_loop_198;
                        for ( ; __need_loop_198 ; __need_loop_198 = __at_iterator.next(__context__,(__at_rename_at_198_194)) && __an_iterator.next(__context__,(__an_rename_at_198_195)) )
                        {
                            if ( __first_rename_at_197_191 )
                            {
                                das_copy(__first_rename_at_197_191,false);
                            } else {
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_77,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ";"))));
                            };
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_78,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<das::string const  &>::from((*__an_rename_at_198_195))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_79,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ":"))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_80,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_198_194)))));
                        }
                        __at_iterator.close(__context__,(__at_rename_at_198_194));
                        __an_iterator.close(__context__,(__an_rename_at_198_195));
                    };
                } else {
                    bool __first_rename_at_209_196 = true;
                    {
                        bool __need_loop_210 = true;
                        // at: smart_ptr<ast::TypeDecl> const&
                        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__td_rename_at_116_186->argTypes /*argTypes*/);
                        smart_ptr<TypeDecl> const  * __at_rename_at_210_197;
                        __need_loop_210 = __at_iterator.first(__context__,(__at_rename_at_210_197)) && __need_loop_210;
                        for ( ; __need_loop_210 ; __need_loop_210 = __at_iterator.next(__context__,(__at_rename_at_210_197)) )
                        {
                            if ( __first_rename_at_209_196 )
                            {
                                das_copy(__first_rename_at_209_196,false);
                            } else {
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_81,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ";"))));
                            };
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_82,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_210_197)))));
                        }
                        __at_iterator.close(__context__,(__at_rename_at_210_197));
                    };
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_83,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ")"))));
                if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__td_rename_at_116_186->firstType /*firstType*/),das_auto_cast<void * const >::cast(nullptr)) )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_84,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ":"))));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_85,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,__td_rename_at_116_186->firstType /*firstType*/))));
                };
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_86,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ">"))));
        } else if ( (__baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tTuple) || (__baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tVariant) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_87,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char * const )(ast_das_to_string(__baseType_rename_at_122_188,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_88,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "<"))));
            if ( das_vector_length(__td_rename_at_116_186->argTypes /*argTypes*/) != 0 )
            {
                if ( das_vector_length(__td_rename_at_116_186->argNames /*argNames*/) != 0 )
                {
                    bool __first_rename_at_231_198 = true;
                    {
                        bool __need_loop_232 = true;
                        // at: smart_ptr<ast::TypeDecl> const&
                        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__td_rename_at_116_186->argTypes /*argTypes*/);
                        smart_ptr<TypeDecl> const  * __at_rename_at_232_201;
                        __need_loop_232 = __at_iterator.first(__context__,(__at_rename_at_232_201)) && __need_loop_232;
                        // an: $::das_string const&
                        das_iterator<das::vector<das::string> const > __an_iterator(__td_rename_at_116_186->argNames /*argNames*/);
                        das::string const  * __an_rename_at_232_202;
                        __need_loop_232 = __an_iterator.first(__context__,(__an_rename_at_232_202)) && __need_loop_232;
                        for ( ; __need_loop_232 ; __need_loop_232 = __at_iterator.next(__context__,(__at_rename_at_232_201)) && __an_iterator.next(__context__,(__an_rename_at_232_202)) )
                        {
                            if ( __first_rename_at_231_198 )
                            {
                                das_copy(__first_rename_at_231_198,false);
                            } else {
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_89,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ";"))));
                            };
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_90,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<das::string const  &>::from((*__an_rename_at_232_202))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_91,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ":"))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_92,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_232_201)))));
                        }
                        __at_iterator.close(__context__,(__at_rename_at_232_201));
                        __an_iterator.close(__context__,(__an_rename_at_232_202));
                    };
                } else {
                    bool __first_rename_at_243_203 = true;
                    {
                        bool __need_loop_244 = true;
                        // at: smart_ptr<ast::TypeDecl> const&
                        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__td_rename_at_116_186->argTypes /*argTypes*/);
                        smart_ptr<TypeDecl> const  * __at_rename_at_244_204;
                        __need_loop_244 = __at_iterator.first(__context__,(__at_rename_at_244_204)) && __need_loop_244;
                        for ( ; __need_loop_244 ; __need_loop_244 = __at_iterator.next(__context__,(__at_rename_at_244_204)) )
                        {
                            if ( __first_rename_at_243_203 )
                            {
                                das_copy(__first_rename_at_243_203,false);
                            } else {
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_93,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ";"))));
                            };
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_94,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_244_204)))));
                        }
                        __at_iterator.close(__context__,(__at_rename_at_244_204));
                    };
                };
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_95,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ">"))));
        } else if ( __baseType_rename_at_122_188 == DAS_COMMENT(bound_enum) das::Type::tBitfield )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_96,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char * const )(ast_das_to_string(__baseType_rename_at_122_188,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_97,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "<"))));
            bool __first_rename_at_258_205 = true;
            {
                bool __need_loop_259 = true;
                // an: $::das_string const&
                das_iterator<das::vector<das::string> const > __an_iterator(__td_rename_at_116_186->argNames /*argNames*/);
                das::string const  * __an_rename_at_259_206;
                __need_loop_259 = __an_iterator.first(__context__,(__an_rename_at_259_206)) && __need_loop_259;
                for ( ; __need_loop_259 ; __need_loop_259 = __an_iterator.next(__context__,(__an_rename_at_259_206)) )
                {
                    if ( __first_rename_at_258_205 )
                    {
                        das_copy(__first_rename_at_258_205,false);
                    } else {
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_98,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ";"))));
                    };
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_99,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<das::string const  &>::from((*__an_rename_at_259_206))));
                }
                __an_iterator.close(__context__,(__an_rename_at_259_206));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_100,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) ">"))));
        } else {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_101,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char * const )(ast_das_to_string(__baseType_rename_at_122_188,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
        };
        if ( das_get_bitfield(__td_rename_at_116_186->flags /*flags*/,1u << 1) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_102,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) " const"))));
        };
        {
            bool __need_loop_274 = true;
            // d: int const&
            das_iterator<das::vector<int32_t> const > __d_iterator(__td_rename_at_116_186->dim /*dim*/);
            int32_t const  * __d_rename_at_274_207;
            __need_loop_274 = __d_iterator.first(__context__,(__d_rename_at_274_207)) && __need_loop_274;
            for ( ; __need_loop_274 ; __need_loop_274 = __d_iterator.next(__context__,(__d_rename_at_274_207)) )
            {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_103,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "["))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_104,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<int32_t>::from((*__d_rename_at_274_207))));
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_105,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "]"))));
            }
            __d_iterator.close(__context__,(__d_rename_at_274_207));
        };
        if ( das_get_bitfield(__td_rename_at_116_186->flags /*flags*/,1u << 0) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_106,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "&"))));
        };
        if ( das_get_bitfield(__td_rename_at_116_186->flags /*flags*/,1u << 2) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_107,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "#"))));
        };
        if ( das_get_bitfield(__td_rename_at_116_186->flags /*flags*/,1u << 3) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_108,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) " implicit"))));
        };
        if ( das_get_bitfield(__td_rename_at_116_186->flags /*flags*/,1u << 8) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_109,cast<StringBuilderWriter &>::from(__writer_rename_at_117_187),cast<char * const >::from(((char *) "!"))));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * make_domain_3e6edb06cf9af7bc ( Context * __context__, char * const  __attr_rename_at_294_208 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_110, cast<char * const >::from(((char *) ".. ")), cast<char * const >::from(__attr_rename_at_294_208), cast<char * const >::from(((char *) ":: ")))));
}

inline char * make_label_3368ba1f933c2731 ( Context * __context__, char * const  __name_rename_at_298_209 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_111, cast<char * const >::from(((char *) ".. _")), cast<char * const >::from(__name_rename_at_298_209), cast<char * const >::from(((char *) ":\n\n")))));
}

inline char * make_ref_c995159f43ed1b43 ( Context * __context__, char * const  __name_rename_at_302_210, char * const  __text_rename_at_302_211 )
{
    TArray<TTuple<16,char *,char *>> _temp_make_local_303_37_48; _temp_make_local_303_37_48;
    TDim<TTuple<16,char *,char *>,3> _temp_make_local_303_37_80; _temp_make_local_303_37_80;
    char * __t_rename_at_303_212 = ((char *)(char *)(das_invoke_function<char * const >::invoke<char * const ,TArray<TTuple<16,char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 5840488413950466740u)),__text_rename_at_302_211,das_arg<TArray<TTuple<16,char *,char *>>>::pass((_temp_make_local_303_37_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_2c64031744b1c1f2(__context__,das_arg<TDim<TTuple<16,char *,char *>,3>>::pass((([&]() -> TDim<TTuple<16,char *,char *>,3>& {
        _temp_make_local_303_37_80(0,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_304;
            das_get_tuple_field<char *,0>::get(__mkt_304) = ((char *) "<");
            das_get_tuple_field<char *,8>::get(__mkt_304) = ((char *) "\\<");
            return __mkt_304;
        })());
        _temp_make_local_303_37_80(1,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_305;
            das_get_tuple_field<char *,0>::get(__mkt_305) = ((char *) ">");
            das_get_tuple_field<char *,8>::get(__mkt_305) = ((char *) "\\>");
            return __mkt_305;
        })());
        _temp_make_local_303_37_80(2,__context__) = (([&]() -> TTuple<16,char *,char *> {
            TTuple<16,char *,char *> __mkt_306;
            das_get_tuple_field<char *,0>::get(__mkt_306) = ((char *) "!");
            das_get_tuple_field<char *,8>::get(__mkt_306) = ((char *) "\\!");
            return __mkt_306;
        })());
        return _temp_make_local_303_37_80;
    })())))))))));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_112, cast<char * const >::from(((char *) " :ref:`")), cast<char * const >::from(__t_rename_at_303_212), cast<char * const >::from(((char *) " <")), cast<char * const >::from(__name_rename_at_302_210), cast<char * const >::from(((char *) ">` ")))));
}

inline char * make_group_8b304b06bc5b7645 ( Context * __context__, char * const  __name_rename_at_311_213, char * const  __plus_rename_at_311_214 )
{
    int32_t __len_rename_at_312_215 = ((int32_t)builtin_string_length(__name_rename_at_311_213,__context__));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<6>(__tinfo_113, cast<char * const >::from(((char * const )(string_repeat(__plus_rename_at_311_214,__len_rename_at_312_215,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__name_rename_at_311_213), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(__plus_rename_at_311_214,__len_rename_at_312_215,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n\n")))));
}

inline char * make_header_efad6667e51b3e08 ( Context * __context__, char * const  __name_rename_at_316_216, char * const  __lab_rename_at_316_217 )
{
    int32_t __wide_rename_at_317_218 = ((int32_t)builtin_string_length(__name_rename_at_316_216,__context__));
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<9>(__tinfo_114, cast<char * const >::from(((char *) ".. _stdlib_")), cast<char * const >::from(__lab_rename_at_316_217), cast<char * const >::from(((char *) ":\n\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "="),__wide_rename_at_317_218,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__name_rename_at_316_216), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "="),__wide_rename_at_317_218,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n\n")))));
}

inline void write_table_separator_2180d51ced094114 ( Context * __context__, StringBuilderWriter &  __writer_rename_at_321_219, TArray<int32_t> const  &  __wid_rename_at_321_220, char * const  __sep_rename_at_321_221 )
{
    {
        bool __need_loop_322 = true;
        // x: int const&
        das_iterator<TArray<int32_t> const > __x_iterator(__wid_rename_at_321_220);
        int32_t const  * __x_rename_at_322_222;
        __need_loop_322 = __x_iterator.first(__context__,(__x_rename_at_322_222)) && __need_loop_322;
        for ( ; __need_loop_322 ; __need_loop_322 = __x_iterator.next(__context__,(__x_rename_at_322_222)) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_115,cast<StringBuilderWriter &>::from(__writer_rename_at_321_219),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_116, cast<char * const >::from(((char *) "+")), cast<char * const >::from(((char * const )(string_repeat(__sep_rename_at_321_221,(*__x_rename_at_322_222),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))))));
        }
        __x_iterator.close(__context__,(__x_rename_at_322_222));
    };
    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_117,cast<StringBuilderWriter &>::from(__writer_rename_at_321_219),cast<char * const >::from(((char *) "+\n"))));
}

inline void write_table_line_18bbb8245228f037 ( Context * __context__, StringBuilderWriter &  __writer_rename_at_328_223, TArray<char *> const  &  __line_rename_at_328_224, TArray<int32_t> const  &  __wid_rename_at_328_225 )
{
    {
        bool __need_loop_329 = true;
        // x: int const&
        das_iterator<TArray<int32_t> const > __x_iterator(__wid_rename_at_328_225);
        int32_t const  * __x_rename_at_329_228;
        __need_loop_329 = __x_iterator.first(__context__,(__x_rename_at_329_228)) && __need_loop_329;
        // l: string const&
        das_iterator<TArray<char *> const > __l_iterator(__line_rename_at_328_224);
        char * const  * __l_rename_at_329_229;
        __need_loop_329 = __l_iterator.first(__context__,(__l_rename_at_329_229)) && __need_loop_329;
        for ( ; __need_loop_329 ; __need_loop_329 = __x_iterator.next(__context__,(__x_rename_at_329_228)) && __l_iterator.next(__context__,(__l_rename_at_329_229)) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_118,cast<StringBuilderWriter &>::from(__writer_rename_at_328_223),cast<char *>::from(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_119, cast<char * const >::from(((char *) "+")), cast<char * const >::from(das_invoke_function<char * const >::invoke<char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::wide CIs Ci*/ 14304877890803689997u)),(*__l_rename_at_329_229),(*__x_rename_at_329_228))))))));
        }
        __x_iterator.close(__context__,(__x_rename_at_329_228));
        __l_iterator.close(__context__,(__l_rename_at_329_229));
    };
    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_120,cast<StringBuilderWriter &>::from(__writer_rename_at_328_223),cast<char * const >::from(((char *) "+\n"))));
}

inline char * make_table_46d5ce8cf5a30e69 ( Context * __context__, TArray<TArray<char *>> const  &  __tab_rename_at_335_230, bool __withHeader_rename_at_335_231 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_336_232) DAS_AOT_INLINE_LAMBDA -> void{
        Sequence DAS_COMMENT((int32_t)) _temp_make_local_349_30_224; _temp_make_local_349_30_224;
        int32_t __cols_rename_at_338_233 = 0;
        {
            bool __need_loop_339 = true;
            // c: array<string> const&
            das_iterator<TArray<TArray<char *>> const > __c_iterator(__tab_rename_at_335_230);
            TArray<char *> const  * __c_rename_at_339_234;
            __need_loop_339 = __c_iterator.first(__context__,(__c_rename_at_339_234)) && __need_loop_339;
            for ( ; __need_loop_339 ; __need_loop_339 = __c_iterator.next(__context__,(__c_rename_at_339_234)) )
            {
                das_copy(__cols_rename_at_338_233,SimPolicy<int32_t>::Max(__cols_rename_at_338_233,builtin_array_size((*__c_rename_at_339_234)),*__context__,nullptr));
            }
            __c_iterator.close(__context__,(__c_rename_at_339_234));
        };
        TArray<int32_t> __wid_rename_at_342_235; das_zero(__wid_rename_at_342_235);
        _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51(__context__,das_arg<TArray<int32_t>>::pass(__wid_rename_at_342_235),__cols_rename_at_338_233);
        {
            bool __need_loop_344 = true;
            // cc: array<string> const&
            das_iterator<TArray<TArray<char *>> const > __cc_iterator(__tab_rename_at_335_230);
            TArray<char *> const  * __cc_rename_at_344_236;
            __need_loop_344 = __cc_iterator.first(__context__,(__cc_rename_at_344_236)) && __need_loop_344;
            for ( ; __need_loop_344 ; __need_loop_344 = __cc_iterator.next(__context__,(__cc_rename_at_344_236)) )
            {
                {
                    bool __need_loop_345 = true;
                    // x: int const
                    das_iterator<range> __x_iterator(mk_range(__cols_rename_at_338_233));
                    int32_t __x_rename_at_345_237;
                    __need_loop_345 = __x_iterator.first(__context__,(__x_rename_at_345_237)) && __need_loop_345;
                    for ( ; __need_loop_345 ; __need_loop_345 = __x_iterator.next(__context__,(__x_rename_at_345_237)) )
                    {
                        das_copy(__wid_rename_at_342_235(__x_rename_at_345_237,__context__),SimPolicy<int32_t>::Max(__wid_rename_at_342_235(__x_rename_at_345_237,__context__),builtin_string_length((*__cc_rename_at_344_236)(__x_rename_at_345_237,__context__),__context__),*__context__,nullptr));
                    }
                    __x_iterator.close(__context__,(__x_rename_at_345_237));
                };
            }
            __cc_iterator.close(__context__,(__cc_rename_at_344_236));
        };
        {
            bool __need_loop_349 = true;
            // ccc: array<string> const&
            das_iterator<TArray<TArray<char *>> const > __ccc_iterator(__tab_rename_at_335_230);
            TArray<char *> const  * __ccc_rename_at_349_240;
            __need_loop_349 = __ccc_iterator.first(__context__,(__ccc_rename_at_349_240)) && __need_loop_349;
            // idx: int
            das_iterator_count DAS_COMMENT((_temp_make_local_349_30_224 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __idx_iterator(0,1);
            int32_t __idx_rename_at_349_241;
            __need_loop_349 = __idx_iterator.first(__context__,(__idx_rename_at_349_241)) && __need_loop_349;
            for ( ; __need_loop_349 ; __need_loop_349 = __ccc_iterator.next(__context__,(__ccc_rename_at_349_240)) && __idx_iterator.next(__context__,(__idx_rename_at_349_241)) )
            {
                write_table_separator_2180d51ced094114(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_336_232),das_arg<TArray<int32_t>>::pass(__wid_rename_at_342_235),((__idx_rename_at_349_241 == 1) && __withHeader_rename_at_335_231) ? das_auto_cast<char * const >::cast(((char *) "=")) : das_auto_cast<char * const >::cast(((char *) "-")));
                write_table_line_18bbb8245228f037(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_336_232),(*__ccc_rename_at_349_240),das_arg<TArray<int32_t>>::pass(__wid_rename_at_342_235));
            }
            __ccc_iterator.close(__context__,(__ccc_rename_at_349_240));
            __idx_iterator.close(__context__,(__idx_rename_at_349_241));
        };
        write_table_separator_2180d51ced094114(__context__,das_arg<StringBuilderWriter>::pass(__writer_rename_at_336_232),das_arg<TArray<int32_t>>::pass(__wid_rename_at_342_235),((char *) "-"));
        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_121,cast<StringBuilderWriter &>::from(__writer_rename_at_336_232),cast<char * const >::from(((char *) "\n\n"))));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * topic_9659659d94ddb29a ( Context * __context__, char * const  __category_rename_at_358_242, Module * const  __mod_rename_at_358_243, char * const  __name_rename_at_358_244 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<7>(__tinfo_122, cast<char * const >::from(((char *) "|")), cast<char * const >::from(__category_rename_at_358_242), cast<char * const >::from(((char *) "-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__mod_rename_at_358_243)), cast<char * const >::from(((char *) "-")), cast<char * const >::from(__name_rename_at_358_244), cast<char * const >::from(((char *) "|")))));
}

inline char * topic_b97c7facf4ccb6a9 ( Context * __context__, Module * const  __mod_rename_at_362_245 )
{
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_123, cast<char * const >::from(((char *) "|module-")), cast<char * const >::from(_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1(__context__,__mod_rename_at_362_245)), cast<char * const >::from(((char *) "|")))));
}

inline void document_topic_c6e2614dff9f2d3e ( Context * __context__, FILE const  * const  __doc_file_rename_at_366_246, char * const  __topic_rename_at_366_247 ) { das_stack_prologue __prologue(__context__,288,"document_topic " DAS_FILE_LINE);
{
    char * __topic_file_name_rename_at_367_248 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_124, cast<char *>::from(das_global<char *,0xde937609e38e5c08>(__context__) /*topic_root*/), cast<char * const >::from(((char *) "/")), cast<char * const >::from(((char * const )(builtin_string_slice1(__topic_rename_at_366_247,1,-1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ".rst"))))));
    FStat __st_rename_at_368_249; das_zero(__st_rename_at_368_249);
    if ( builtin_stat(__topic_file_name_rename_at_367_248,das_arg<FStat>::pass(__st_rename_at_368_249)) )
    {
        _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,__topic_file_name_rename_at_367_248,((char *) "rb"),das_make_block<void,FILE const  * const >(__context__,256,0,&__func_info__4bda63c2c7a42bef,[&](FILE const  * const  __topic_file_rename_at_370_250) -> void{
            if ( __topic_file_rename_at_370_250 != nullptr )
            {
                char * __instxt_rename_at_372_251 = ((char *)(char *)(((char * const )(builtin_fread(__topic_file_rename_at_370_250,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                builtin_fwrite(__doc_file_rename_at_366_246,__instxt_rename_at_372_251,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                builtin_fwrite(__doc_file_rename_at_366_246,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            } else {
                builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_125, cast<char * const >::from(((char *) "can't open ")), cast<char * const >::from(__topic_file_name_rename_at_367_248))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }));
    } else {
        builtin_fwrite(__doc_file_rename_at_366_246,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_126, cast<char * const >::from(__topic_rename_at_366_247), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_bitfield_4340095c1075f95e ( Context * __context__, FILE const  * const  __doc_file_rename_at_384_252, smart_ptr_raw<TypeDecl> const  __value_rename_at_384_253 )
{
    TArray<char *> _temp_make_local_386_17_64; _temp_make_local_386_17_64;
    TDim<char *,3> _temp_make_local_386_17_96; _temp_make_local_386_17_96;
    TArray<TArray<char *>> __tab_rename_at_385_254; das_zero(__tab_rename_at_385_254);
    _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_385_254),das_arg<TArray<char *>>::pass((_temp_make_local_386_17_64 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_649bcd7e51242beb(__context__,das_arg<TDim<char *,3>>::pass((([&]() -> TDim<char *,3>& {
        _temp_make_local_386_17_96(0,__context__) = ((char *) "field");
        _temp_make_local_386_17_96(1,__context__) = ((char *) "bit");
        _temp_make_local_386_17_96(2,__context__) = ((char *) "value");
        return _temp_make_local_386_17_96;
    })())))))));
    {
        bool __need_loop_387 = true;
        // an: $::das_string const&
        das_iterator<das::vector<das::string> const > __an_iterator(__value_rename_at_384_253->argNames /*argNames*/);
        das::string const  * __an_rename_at_387_257;
        __need_loop_387 = __an_iterator.first(__context__,(__an_rename_at_387_257)) && __need_loop_387;
        // bit: int const
        das_iterator<range> __bit_iterator(range(0,32));
        int32_t __bit_rename_at_387_258;
        __need_loop_387 = __bit_iterator.first(__context__,(__bit_rename_at_387_258)) && __need_loop_387;
        for ( ; __need_loop_387 ; __need_loop_387 = __an_iterator.next(__context__,(__an_rename_at_387_257)) && __bit_iterator.next(__context__,(__bit_rename_at_387_258)) )
        {
            TArray<char *> __line_rename_at_388_259; das_zero(__line_rename_at_388_259);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_388_259),((char * const )(to_das_string((*__an_rename_at_387_257),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_388_259),((char * const )(das_lexical_cast_int_i32(__bit_rename_at_387_258,false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_388_259),((char * const )(das_lexical_cast_int_i32(1 << __bit_rename_at_387_258,false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_385_254),das_arg<TArray<char *>>::pass(__line_rename_at_388_259));
        }
        __an_iterator.close(__context__,(__an_rename_at_387_257));
        __bit_iterator.close(__context__,(__bit_rename_at_387_258));
    };
    builtin_fwrite(__doc_file_rename_at_384_252,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_385_254),true),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void document_variant_c74548ebfca402e7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_397_260, smart_ptr_raw<TypeDecl> const  __value_rename_at_397_261 )
{
    TArray<TArray<char *>> __tab_rename_at_398_262; das_zero(__tab_rename_at_398_262);
    {
        bool __need_loop_399 = true;
        // an: $::das_string const&
        das_iterator<das::vector<das::string> const > __an_iterator(__value_rename_at_397_261->argNames /*argNames*/);
        das::string const  * __an_rename_at_399_265;
        __need_loop_399 = __an_iterator.first(__context__,(__an_rename_at_399_265)) && __need_loop_399;
        // at: smart_ptr<ast::TypeDecl> const&
        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__value_rename_at_397_261->argTypes /*argTypes*/);
        smart_ptr<TypeDecl> const  * __at_rename_at_399_266;
        __need_loop_399 = __at_iterator.first(__context__,(__at_rename_at_399_266)) && __need_loop_399;
        for ( ; __need_loop_399 ; __need_loop_399 = __an_iterator.next(__context__,(__an_rename_at_399_265)) && __at_iterator.next(__context__,(__at_rename_at_399_266)) )
        {
            TArray<char *> __line_rename_at_400_267; das_zero(__line_rename_at_400_267);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_400_267),((char * const )(to_das_string((*__an_rename_at_399_265),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_400_267),describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_399_266)));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_398_262),das_arg<TArray<char *>>::pass(__line_rename_at_400_267));
        }
        __an_iterator.close(__context__,(__an_rename_at_399_265));
        __at_iterator.close(__context__,(__at_rename_at_399_266));
    };
    builtin_fwrite(__doc_file_rename_at_397_260,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_398_262),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void document_tuple_c74548ebfca402e7 ( Context * __context__, FILE const  * const  __doc_file_rename_at_408_268, smart_ptr_raw<TypeDecl> const  __value_rename_at_408_269 )
{
    TArray<TArray<char *>> __tab_rename_at_409_270; das_zero(__tab_rename_at_409_270);
    {
        bool __need_loop_410 = true;
        // an: $::das_string const&
        das_iterator<das::vector<das::string> const > __an_iterator(__value_rename_at_408_269->argNames /*argNames*/);
        das::string const  * __an_rename_at_410_273;
        __need_loop_410 = __an_iterator.first(__context__,(__an_rename_at_410_273)) && __need_loop_410;
        // at: smart_ptr<ast::TypeDecl> const&
        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __at_iterator(__value_rename_at_408_269->argTypes /*argTypes*/);
        smart_ptr<TypeDecl> const  * __at_rename_at_410_274;
        __need_loop_410 = __at_iterator.first(__context__,(__at_rename_at_410_274)) && __need_loop_410;
        for ( ; __need_loop_410 ; __need_loop_410 = __an_iterator.next(__context__,(__an_rename_at_410_273)) && __at_iterator.next(__context__,(__at_rename_at_410_274)) )
        {
            TArray<char *> __line_rename_at_411_275; das_zero(__line_rename_at_411_275);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_411_275),((char * const )(to_das_string((*__an_rename_at_410_273),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_411_275),describe_type_a9bac10014c763b2(__context__,(*__at_rename_at_410_274)));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_409_270),das_arg<TArray<char *>>::pass(__line_rename_at_411_275));
        }
        __an_iterator.close(__context__,(__an_rename_at_410_273));
        __at_iterator.close(__context__,(__at_rename_at_410_274));
    };
    builtin_fwrite(__doc_file_rename_at_408_268,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_409_270),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void document_typedefs_d1f98d27346b80af ( Context * __context__, FILE const  * const  __doc_file_rename_at_440_276, Module * const  __mod_rename_at_440_277 ) { das_stack_prologue __prologue(__context__,176,"document_typedefs " DAS_FILE_LINE);
{
    bool __any_rename_at_441_278 = false;
    for_each_typedef(__mod_rename_at_440_277,das_make_block<void,char *,smart_ptr<TypeDecl>>(__context__,96,0,&__func_info__aa03c1bcb8f15255,[&](char * __name_rename_at_442_279, smart_ptr<TypeDecl> __value_rename_at_442_280) -> void{
        das_copy(__any_rename_at_441_278,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_441_278 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_440_276,((char *) "++++++++++++\nType aliases\n++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_typedef(__mod_rename_at_440_277,das_make_block<void,char *,smart_ptr<TypeDecl>>(__context__,160,0,&__func_info__aa03c1bcb8f15255,[&](char * __name_rename_at_449_281, smart_ptr<TypeDecl> __value_rename_at_449_282) -> void{
            _FuncrstTickdocument_typedefTick2903024037696058038_24a5c3bc896476d1(__context__,__doc_file_rename_at_440_276,__mod_rename_at_440_277,__name_rename_at_449_281,__value_rename_at_449_282);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_global_constant_378f3e1a10c345f3 ( Context * __context__, FILE const  * const  __doc_file_rename_at_454_283, Module * const  __mod_rename_at_454_284, smart_ptr_raw<Variable> const  __value_rename_at_454_285 )
{
    builtin_fwrite(__doc_file_rename_at_454_283,make_label_3368ba1f933c2731(__context__,global_label_name_b721f72143d35230(__context__,__value_rename_at_454_285)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_454_283,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_454_283,((char * const )(to_das_string(__value_rename_at_454_285->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_454_283,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_127, cast<char * const >::from(((char *) " = ")), cast<char * const >::from(_FuncastTickdescribeTick842554968825501494_452cfc8955963a18(__context__,__value_rename_at_454_285->init /*init*/)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_454_283,topic_9659659d94ddb29a(__context__,((char *) "variable"),__mod_rename_at_454_284,((char * const )(to_das_string(__value_rename_at_454_285->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline bool is_global_constant_320cc2da3d8764f7 ( Context * __context__, smart_ptr_raw<Variable> const  __value_rename_at_462_286 )
{
    return das_auto_cast<bool>::cast(das_get_bitfield(__value_rename_at_462_286->flags /*flags*/,1u << 10) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(das_get_bitfield(__value_rename_at_462_286->type /*_type*/->flags /*flags*/,1u << 1)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(((das_deref(__context__,__value_rename_at_462_286->type /*_type*/)).isWorkhorseType())) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__value_rename_at_462_286->init /*init*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))))));
}

inline void document_global_constants_24362fe0342586e0 ( Context * __context__, FILE const  * const  __doc_file_rename_at_478_287, Module * const  __mod_rename_at_478_288 ) { das_stack_prologue __prologue(__context__,176,"document_global_constants " DAS_FILE_LINE);
{
    bool __any_rename_at_479_289 = false;
    for_each_global(__mod_rename_at_478_288,das_make_block<void,smart_ptr<Variable>>(__context__,96,0,&__func_info__2701da5c12e56b0e,[&](smart_ptr<Variable> __value_rename_at_480_290) -> void{
        if ( is_global_constant_320cc2da3d8764f7(__context__,__value_rename_at_480_290) )
        {
            das_copy(__any_rename_at_479_289,true);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_479_289 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_478_287,((char *) "+++++++++\nConstants\n+++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_global(__mod_rename_at_478_288,das_make_block<void,smart_ptr<Variable>>(__context__,160,0,&__func_info__2701da5c12e56b0e,[&](smart_ptr<Variable> __value_rename_at_489_291) -> void{
            if ( is_global_constant_320cc2da3d8764f7(__context__,__value_rename_at_489_291) )
            {
                document_global_constant_378f3e1a10c345f3(__context__,__doc_file_rename_at_478_287,__mod_rename_at_478_288,__value_rename_at_489_291);
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_enumerations_b052b53cf3b12eb ( Context * __context__, FILE const  * const  __doc_file_rename_at_511_292, Module * const  __mod_rename_at_511_293 ) { das_stack_prologue __prologue(__context__,176,"document_enumerations " DAS_FILE_LINE);
{
    bool __any_rename_at_512_294 = false;
    for_each_enumeration(__mod_rename_at_511_293,das_make_block<void,smart_ptr<Enumeration>>(__context__,96,0,&__func_info__9beafd8de389bb7e,[&](smart_ptr<Enumeration> __value_rename_at_513_295) -> void{
        if ( __value_rename_at_513_295->isPrivate /*isPrivate*/ )
        {
            return ;
        };
        das_copy(__any_rename_at_512_294,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_512_294 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_511_292,((char *) "++++++++++++\nEnumerations\n++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_enumeration(__mod_rename_at_511_293,das_make_block<void,smart_ptr<Enumeration>>(__context__,160,0,&__func_info__9beafd8de389bb7e,[&](smart_ptr<Enumeration> __value_rename_at_523_296) -> void{
            if ( __value_rename_at_523_296->isPrivate /*isPrivate*/ )
            {
                return ;
            };
            _FuncrstTickdocument_enumerationTick4211850574665690795_cd2737074b5000f2(__context__,__doc_file_rename_at_511_292,__mod_rename_at_511_293,__value_rename_at_523_296);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_function_declaration_4c978976b1aaa270 ( Context * __context__, FILE const  * const  __doc_file_rename_at_541_297, char * const  __domain_rename_at_541_298, char * const  __func_name_rename_at_541_299, TArray<char *> const  &  __argNames_rename_at_542_300, TArray<TypeDecl *> const  &  __argTypes_rename_at_542_301, smart_ptr_raw<TypeDecl> const  __resType_rename_at_542_302 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_545_42_32; _temp_make_local_545_42_32;
    builtin_fwrite(__doc_file_rename_at_541_297,make_domain_3e6edb06cf9af7bc(__context__,__domain_rename_at_541_298),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_541_297,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_128, cast<char * const >::from(__func_name_rename_at_541_299), cast<char * const >::from(((char *) "(")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_545 = true;
        // an: string const&
        das_iterator<TArray<char *> const > __an_iterator(__argNames_rename_at_542_300);
        char * const  * __an_rename_at_545_309;
        __need_loop_545 = __an_iterator.first(__context__,(__an_rename_at_545_309)) && __need_loop_545;
        // at: ast::TypeDecl? const&
        das_iterator<TArray<TypeDecl *> const > __at_iterator(__argTypes_rename_at_542_301);
        TypeDecl * const  * __at_rename_at_545_310;
        __need_loop_545 = __at_iterator.first(__context__,(__at_rename_at_545_310)) && __need_loop_545;
        // i: int
        das_iterator_count DAS_COMMENT((_temp_make_local_545_42_32 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
        int32_t __i_rename_at_545_311;
        __need_loop_545 = __i_iterator.first(__context__,(__i_rename_at_545_311)) && __need_loop_545;
        for ( ; __need_loop_545 ; __need_loop_545 = __an_iterator.next(__context__,(__an_rename_at_545_309)) && __at_iterator.next(__context__,(__at_rename_at_545_310)) && __i_iterator.next(__context__,(__i_rename_at_545_311)) )
        {
            if ( _FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6(__context__,(*__at_rename_at_545_310)) )
            {
                if ( __i_rename_at_545_311 != 0 )
                {
                    builtin_fwrite(__doc_file_rename_at_541_297,((char *) "; "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
                builtin_fwrite(__doc_file_rename_at_541_297,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_129, cast<char * const >::from((*__an_rename_at_545_309)), cast<char * const >::from(((char *) ": ")), cast<char * const >::from(describe_type_short_56370d8993e381ba(__context__,(*__at_rename_at_545_310))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __an_iterator.close(__context__,(__an_rename_at_545_309));
        __at_iterator.close(__context__,(__at_rename_at_545_310));
        __i_iterator.close(__context__,(__i_rename_at_545_311));
    };
    builtin_fwrite(__doc_file_rename_at_541_297,((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_541_297,((char *) "\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void document_function_arguments_e7469e75af4ed246 ( Context * __context__, FILE const  * const  __doc_file_rename_at_557_312, TArray<char *> const  &  __argNames_rename_at_557_313, TArray<TypeDecl *> const  &  __argTypes_rename_at_557_314 )
{
    TArray<char *> _temp_make_local_559_17_64; _temp_make_local_559_17_64;
    TDim<char *,2> _temp_make_local_559_17_96; _temp_make_local_559_17_96;
    TArray<TArray<char *>> __tab_rename_at_558_315; das_zero(__tab_rename_at_558_315);
    _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_558_315),das_arg<TArray<char *>>::pass((_temp_make_local_559_17_64 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_325c63fe2a174cf6(__context__,das_arg<TDim<char *,2>>::pass((([&]() -> TDim<char *,2>& {
        _temp_make_local_559_17_96(0,__context__) = ((char *) "argument");
        _temp_make_local_559_17_96(1,__context__) = ((char *) "argument type");
        return _temp_make_local_559_17_96;
    })())))))));
    {
        bool __need_loop_560 = true;
        // an: string const&
        das_iterator<TArray<char *> const > __an_iterator(__argNames_rename_at_557_313);
        char * const  * __an_rename_at_560_318;
        __need_loop_560 = __an_iterator.first(__context__,(__an_rename_at_560_318)) && __need_loop_560;
        // at: ast::TypeDecl? const&
        das_iterator<TArray<TypeDecl *> const > __at_iterator(__argTypes_rename_at_557_314);
        TypeDecl * const  * __at_rename_at_560_319;
        __need_loop_560 = __at_iterator.first(__context__,(__at_rename_at_560_319)) && __need_loop_560;
        for ( ; __need_loop_560 ; __need_loop_560 = __an_iterator.next(__context__,(__an_rename_at_560_318)) && __at_iterator.next(__context__,(__at_rename_at_560_319)) )
        {
            if ( _FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6(__context__,(*__at_rename_at_560_319)) )
            {
                TArray<char *> __line_rename_at_562_320; das_zero(__line_rename_at_562_320);
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_562_320),(*__an_rename_at_560_318));
                _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_562_320),describe_type_cf1a091488868462(__context__,(*__at_rename_at_560_319)));
                _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_558_315),das_arg<TArray<char *>>::pass(__line_rename_at_562_320));
            };
        }
        __an_iterator.close(__context__,(__an_rename_at_560_318));
        __at_iterator.close(__context__,(__at_rename_at_560_319));
    };
    if ( builtin_array_size(das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_558_315)) != 1 )
    {
        builtin_fwrite(__doc_file_rename_at_557_312,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_558_315),true),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void document_classes_55fe4b580059698a ( Context * __context__, FILE const  * const  __doc_file_rename_at_587_321, Module * const  __mod_rename_at_587_322 ) { das_stack_prologue __prologue(__context__,528,"document_classes " DAS_FILE_LINE);
{
    bool __any_rename_at_588_323 = false;
    for_each_structure(__mod_rename_at_587_322,das_make_block<void,smart_ptr<Structure>>(__context__,96,0,&__func_info__438460bc7bed8ede,[&](smart_ptr<Structure> __value_rename_at_589_324) -> void{
        if ( das_get_bitfield(__value_rename_at_589_324->flags /*flags*/,1u << 0) && !(das_get_bitfield(__value_rename_at_589_324->flags /*flags*/,1u << 7)) )
        {
            das_copy(__any_rename_at_588_323,true);
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_588_323 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_587_321,((char *) "+++++++\nClasses\n+++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_structure(__mod_rename_at_587_322,das_make_block<void,smart_ptr<Structure>>(__context__,160,0,&__func_info__438460bc7bed8ede,[&](smart_ptr<Structure> __value_rename_at_598_325) -> void{
            if ( !(das_get_bitfield(__value_rename_at_598_325->flags /*flags*/,1u << 0)) || das_get_bitfield(__value_rename_at_598_325->flags /*flags*/,1u << 7) )
            {
                return ;
            };
            builtin_fwrite(__doc_file_rename_at_587_321,make_label_3368ba1f933c2731(__context__,struct_label_name_6ee1d24e90557aeb(__context__,__value_rename_at_598_325)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            builtin_fwrite(__doc_file_rename_at_587_321,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            builtin_fwrite(__doc_file_rename_at_587_321,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_598_325->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            if ( __value_rename_at_598_325->parent /*parent*/ != nullptr )
            {
                builtin_fwrite(__doc_file_rename_at_587_321,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_130, cast<char * const >::from(((char *) " : ")), cast<das::string &>::from(__value_rename_at_598_325->parent /*parent*/->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
            builtin_fwrite(__doc_file_rename_at_587_321,((char *) "\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_587_321,topic_9659659d94ddb29a(__context__,((char *) "class"),__mod_rename_at_587_322,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_598_325->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
            int32_t __w_rename_at_610_326 = 0;
            {
                bool __need_loop_611 = true;
                // fld: ast::FieldDeclaration&
                das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__value_rename_at_598_325->fields /*fields*/);
                Structure::FieldDeclaration * __fld_rename_at_611_327;
                __need_loop_611 = __fld_iterator.first(__context__,(__fld_rename_at_611_327)) && __need_loop_611;
                for ( ; __need_loop_611 ; __need_loop_611 = __fld_iterator.next(__context__,(__fld_rename_at_611_327)) )
                {
                    if ( (!(das_invoke_function<bool>::invoke<smart_ptr_raw<Structure> const ,smart_ptr_raw<TypeDecl> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::is_class_method CY<StructurePtr>1<H<ast::Structure>>?M CY<TypeDeclPtr>1<H<ast::TypeDecl>>?M*/ 11657066889097786130u)),__value_rename_at_598_325,(*__fld_rename_at_611_327).type /*_type*/)) && !(das_get_bitfield((*__fld_rename_at_611_327).flags /*flags*/,1u << 3))) && !(das_get_bitfield((*__fld_rename_at_611_327).flags /*flags*/,1u << 1)) )
                    {
                        das_copy(__w_rename_at_610_326,SimPolicy<int32_t>::Max(__w_rename_at_610_326,builtin_ext_string_length(das_arg<das::string>::pass((*__fld_rename_at_611_327).name /*name*/)),*__context__,nullptr));
                    };
                }
                __fld_iterator.close(__context__,(__fld_rename_at_611_327));
            };
            if ( __w_rename_at_610_326 != 0 )
            {
                builtin_fwrite(__doc_file_rename_at_587_321,((char *) "it defines as follows\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                {
                    bool __need_loop_618 = true;
                    // fld: ast::FieldDeclaration&
                    das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__value_rename_at_598_325->fields /*fields*/);
                    Structure::FieldDeclaration * __fld_rename_at_618_328;
                    __need_loop_618 = __fld_iterator.first(__context__,(__fld_rename_at_618_328)) && __need_loop_618;
                    for ( ; __need_loop_618 ; __need_loop_618 = __fld_iterator.next(__context__,(__fld_rename_at_618_328)) )
                    {
                        if ( ((!(das_invoke_function<bool>::invoke<smart_ptr_raw<Structure> const ,smart_ptr_raw<TypeDecl> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::is_class_method CY<StructurePtr>1<H<ast::Structure>>?M CY<TypeDeclPtr>1<H<ast::TypeDecl>>?M*/ 11657066889097786130u)),__value_rename_at_598_325,(*__fld_rename_at_618_328).type /*_type*/)) && !(das_get_bitfield((*__fld_rename_at_618_328).flags /*flags*/,1u << 3))) && !(das_get_bitfield((*__fld_rename_at_618_328).flags /*flags*/,1u << 1))) && (neq_dstr_str(das_arg<das::string>::pass((*__fld_rename_at_618_328).name /*name*/),((char *) "__rtti"))) )
                        {
                            builtin_fwrite(__doc_file_rename_at_587_321,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_131, cast<char * const >::from(((char *) "  | ")), cast<char * const >::from(das_invoke_function<char * const >::invoke<char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::wide CIs Ci*/ 14304877890803689997u)),((char * const )(to_das_string(das_arg<das::string>::pass((*__fld_rename_at_618_328).name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),__w_rename_at_610_326)), cast<char * const >::from(((char *) " : ")), cast<char * const >::from(describe_type_a9bac10014c763b2(__context__,(*__fld_rename_at_618_328).type /*_type*/)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        };
                    }
                    __fld_iterator.close(__context__,(__fld_rename_at_618_328));
                };
                builtin_fwrite(__doc_file_rename_at_587_321,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
            {
                bool __need_loop_625 = true;
                // fld: ast::FieldDeclaration&
                das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__value_rename_at_598_325->fields /*fields*/);
                Structure::FieldDeclaration * __fld_rename_at_625_329;
                __need_loop_625 = __fld_iterator.first(__context__,(__fld_rename_at_625_329)) && __need_loop_625;
                for ( ; __need_loop_625 ; __need_loop_625 = __fld_iterator.next(__context__,(__fld_rename_at_625_329)) )
                {
                    if ( ((das_invoke_function<bool>::invoke<smart_ptr_raw<Structure> const ,smart_ptr_raw<TypeDecl> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::is_class_method CY<StructurePtr>1<H<ast::Structure>>?M CY<TypeDeclPtr>1<H<ast::TypeDecl>>?M*/ 11657066889097786130u)),__value_rename_at_598_325,(*__fld_rename_at_625_329).type /*_type*/) && !(das_get_bitfield((*__fld_rename_at_625_329).flags /*flags*/,1u << 3))) && !(das_get_bitfield((*__fld_rename_at_625_329).flags /*flags*/,1u << 1))) && das_get_bitfield((*__fld_rename_at_625_329).flags /*flags*/,1u << 8) )
                    {
                        _FuncrstTickdocument_class_methodTick12589992380795293570_b6d1264d11ee8d48(__context__,__doc_file_rename_at_587_321,__mod_rename_at_587_322,__value_rename_at_598_325,das_arg<Structure::FieldDeclaration>::pass((*__fld_rename_at_625_329)));
                    };
                }
                __fld_iterator.close(__context__,(__fld_rename_at_625_329));
            };
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_structure_96bd65f4eda31cf3 ( Context * __context__, FILE const  * const  __doc_file_rename_at_633_330, Module * const  __mod_rename_at_633_331, smart_ptr_raw<Structure> const  __value_rename_at_633_332 )
{
    builtin_fwrite(__doc_file_rename_at_633_330,make_label_3368ba1f933c2731(__context__,struct_label_name_6ee1d24e90557aeb(__context__,__value_rename_at_633_332)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_633_330,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_633_330,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_132, cast<das::string const  &>::from(__value_rename_at_633_332->name /*name*/), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __value_rename_at_633_332->parent /*parent*/ != nullptr )
    {
        builtin_fwrite(__doc_file_rename_at_633_330,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_133, cast<char * const >::from(((char *) " : ")), cast<das::string const  &>::from(__value_rename_at_633_332->parent /*parent*/->name /*name*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    builtin_fwrite(__doc_file_rename_at_633_330,((char *) "\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    TArray<TArray<char *>> __tab_rename_at_641_333; das_zero(__tab_rename_at_641_333);
    {
        bool __need_loop_642 = true;
        // fld: ast::FieldDeclaration const&
        das_iterator<das::vector<Structure::FieldDeclaration> const > __fld_iterator(__value_rename_at_633_332->fields /*fields*/);
        Structure::FieldDeclaration const  * __fld_rename_at_642_334;
        __need_loop_642 = __fld_iterator.first(__context__,(__fld_rename_at_642_334)) && __need_loop_642;
        for ( ; __need_loop_642 ; __need_loop_642 = __fld_iterator.next(__context__,(__fld_rename_at_642_334)) )
        {
            TArray<char *> __line_rename_at_643_335; das_zero(__line_rename_at_643_335);
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_643_335),((char * const )(to_das_string((*__fld_rename_at_642_334).name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__line_rename_at_643_335),describe_type_a9bac10014c763b2(__context__,(*__fld_rename_at_642_334).type /*_type*/));
            _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_641_333),das_arg<TArray<char *>>::pass(__line_rename_at_643_335));
        }
        __fld_iterator.close(__context__,(__fld_rename_at_642_334));
    };
    builtin_fwrite(__doc_file_rename_at_633_330,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_134, cast<das::string const  &>::from(__value_rename_at_633_332->name /*name*/), cast<char * const >::from(((char *) " fields are\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_633_330,make_table_46d5ce8cf5a30e69(__context__,das_arg<TArray<TArray<char *>>>::pass(__tab_rename_at_641_333),false),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_633_330,topic_9659659d94ddb29a(__context__,((char *) "structure"),__mod_rename_at_633_331,((char * const )(to_das_string(__value_rename_at_633_332->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void document_structures_5fb20d0dee04ba46 ( Context * __context__, FILE const  * const  __doc_file_rename_at_653_336, Module * const  __mod_rename_at_653_337 ) { das_stack_prologue __prologue(__context__,176,"document_structures " DAS_FILE_LINE);
{
    bool __any_rename_at_654_338 = false;
    for_each_structure(__mod_rename_at_653_337,das_make_block<void,smart_ptr<Structure>>(__context__,96,0,&__func_info__438460bc7bed8ede,[&](smart_ptr<Structure> __value_rename_at_655_339) -> void{
        if ( (das_get_bitfield(__value_rename_at_655_339->flags /*flags*/,1u << 0) || das_get_bitfield(__value_rename_at_655_339->flags /*flags*/,1u << 4)) || das_get_bitfield(__value_rename_at_655_339->flags /*flags*/,1u << 7) )
        {
            return ;
        };
        das_copy(__any_rename_at_654_338,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_654_338 )
    {
        return ;
    } else {
        for_each_structure(__mod_rename_at_653_337,das_make_block<void,smart_ptr<Structure>>(__context__,160,0,&__func_info__438460bc7bed8ede,[&](smart_ptr<Structure> __value_rename_at_665_340) -> void{
            if ( (das_get_bitfield(__value_rename_at_665_340->flags /*flags*/,1u << 0) || das_get_bitfield(__value_rename_at_665_340->flags /*flags*/,1u << 4)) || das_get_bitfield(__value_rename_at_665_340->flags /*flags*/,1u << 7) )
            {
                return ;
            };
            document_structure_96bd65f4eda31cf3(__context__,__doc_file_rename_at_653_336,__mod_rename_at_653_337,__value_rename_at_665_340);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_structure_annotations_a53005d9c9a5617 ( Context * __context__, FILE const  * const  __doc_file_rename_at_742_341, Module * const  __mod_rename_at_742_342 ) { das_stack_prologue __prologue(__context__,176,"document_structure_annotations " DAS_FILE_LINE);
{
    bool __any_rename_at_743_343 = false;
    rtti_builtin_module_for_each_annotation(__mod_rename_at_742_342,das_make_block<void,Annotation const  &>(__context__,96,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_744_344) -> void{
        if ( !((__value_rename_at_744_344).rtti_isBasicStructureAnnotation()) )
        {
            return ;
        };
        das_copy(__any_rename_at_743_343,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_743_343 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_742_341,((char *) "++++++++++++++++++\nHandled structures\n++++++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        rtti_builtin_module_for_each_annotation(__mod_rename_at_742_342,das_make_block<void,Annotation const  &>(__context__,160,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_754_345) -> void{
            if ( !((__value_rename_at_754_345).rtti_isBasicStructureAnnotation()) )
            {
                return ;
            };
            _FuncrstTickdocument_structure_annotationTick2734840712723762798_45f2db6d6c9372ba(__context__,__doc_file_rename_at_742_341,__mod_rename_at_742_342,__value_rename_at_754_345);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_function_annotations_5344d92e9ea569ad ( Context * __context__, FILE const  * const  __doc_file_rename_at_769_346, Module * const  __mod_rename_at_769_347 ) { das_stack_prologue __prologue(__context__,176,"document_function_annotations " DAS_FILE_LINE);
{
    bool __any_rename_at_770_348 = false;
    rtti_builtin_module_for_each_annotation(__mod_rename_at_769_347,das_make_block<void,Annotation const  &>(__context__,96,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_771_349) -> void{
        if ( !((__value_rename_at_771_349).rtti_isFunctionAnnotation()) )
        {
            return ;
        };
        das_copy(__any_rename_at_770_348,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_770_348 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_769_346,((char *) "++++++++++++++++++++\nFunction annotations\n++++++++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        rtti_builtin_module_for_each_annotation(__mod_rename_at_769_347,das_make_block<void,Annotation const  &>(__context__,160,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_781_350) -> void{
            if ( !((__value_rename_at_781_350).rtti_isFunctionAnnotation()) )
            {
                return ;
            };
            _FuncrstTickdocument_function_annotationTick9878220666429632644_62156d6269c1aee6(__context__,__doc_file_rename_at_769_346,__mod_rename_at_769_347,__value_rename_at_781_350);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_annotations_128851a794d46d1a ( Context * __context__, FILE const  * const  __doc_file_rename_at_796_351, Module * const  __mod_rename_at_796_352 ) { das_stack_prologue __prologue(__context__,176,"document_annotations " DAS_FILE_LINE);
{
    bool __any_rename_at_797_353 = false;
    rtti_builtin_module_for_each_annotation(__mod_rename_at_796_352,das_make_block<void,Annotation const  &>(__context__,96,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_798_354) -> void{
        if ( !(((__value_rename_at_798_354).rtti_isHandledTypeAnnotation())) || ((__value_rename_at_798_354).rtti_isBasicStructureAnnotation()) )
        {
            return ;
        };
        das_copy(__any_rename_at_797_353,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_797_353 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_796_351,((char *) "+++++++++++++\nHandled types\n+++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        rtti_builtin_module_for_each_annotation(__mod_rename_at_796_352,das_make_block<void,Annotation const  &>(__context__,160,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_808_355) -> void{
            if ( !(((__value_rename_at_808_355).rtti_isHandledTypeAnnotation())) || ((__value_rename_at_808_355).rtti_isBasicStructureAnnotation()) )
            {
                return ;
            };
            if ( builtin_string_startswith(((char * const )(to_das_string(__value_rename_at_808_355.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "dasvector`"),__context__) )
            {
                return ;
            };
            _FuncrstTickdocument_annotationTick7679268031359399902_8b47c8f555ccae04(__context__,__doc_file_rename_at_796_351,__mod_rename_at_796_352,__value_rename_at_808_355);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_structure_macros_7386ca2820215b3d ( Context * __context__, FILE const  * const  __doc_file_rename_at_826_356, Module * const  __mod_rename_at_826_357 ) { das_stack_prologue __prologue(__context__,176,"document_structure_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_827_358 = false;
    rtti_builtin_module_for_each_annotation(__mod_rename_at_826_357,das_make_block<void,Annotation const  &>(__context__,96,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_828_359) -> void{
        if ( (((__value_rename_at_828_359).rtti_isHandledTypeAnnotation()) || ((__value_rename_at_828_359).rtti_isBasicStructureAnnotation())) || ((__value_rename_at_828_359).rtti_isFunctionAnnotation()) )
        {
            return ;
        };
        das_copy(__any_rename_at_827_358,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_827_358 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_826_356,((char *) "++++++++++++++++\nStructure macros\n++++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        rtti_builtin_module_for_each_annotation(__mod_rename_at_826_357,das_make_block<void,Annotation const  &>(__context__,160,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_838_360) -> void{
            if ( (((__value_rename_at_838_360).rtti_isHandledTypeAnnotation()) || ((__value_rename_at_838_360).rtti_isBasicStructureAnnotation())) || ((__value_rename_at_838_360).rtti_isFunctionAnnotation()) )
            {
                return ;
            };
            _FuncrstTickdocument_structure_macroTick6088634580083749384_a8cf27101dd30a50(__context__,__doc_file_rename_at_826_356,__mod_rename_at_826_357,__value_rename_at_838_360);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_call_macro_e76fbe68d00e7362 ( Context * __context__, FILE const  * const  __doc_file_rename_at_846_361, Module * const  __mod_rename_at_846_362, char * const  __value_rename_at_846_363 )
{
    builtin_fwrite(__doc_file_rename_at_846_361,make_label_3368ba1f933c2731(__context__,_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c(__context__,__mod_rename_at_846_362,__value_rename_at_846_363)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_846_361,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_846_361,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_135, cast<char * const >::from(__value_rename_at_846_363), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_846_361,topic_9659659d94ddb29a(__context__,((char *) "function_annotation"),__mod_rename_at_846_362,__value_rename_at_846_363));
}

inline void document_call_macros_56da413fe6ccb104 ( Context * __context__, FILE const  * const  __doc_file_rename_at_853_364, Module * const  __mod_rename_at_853_365 ) { das_stack_prologue __prologue(__context__,176,"document_call_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_854_366 = false;
    for_each_call_macro(__mod_rename_at_853_365,das_make_block<void,char *>(__context__,96,0,&__func_info__e0afcd004fbe6de3,[&](char * __value_rename_at_855_367) -> void{
        das_copy(__any_rename_at_854_366,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_854_366 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_853_364,((char *) "+++++++++++\nCall macros\n+++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_call_macro(__mod_rename_at_853_365,das_make_block<void,char *>(__context__,160,0,&__func_info__e0afcd004fbe6de3,[&](char * __value_rename_at_862_368) -> void{
            document_call_macro_e76fbe68d00e7362(__context__,__doc_file_rename_at_853_364,__mod_rename_at_853_365,__value_rename_at_862_368);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_reader_macro_4006c26786c1908 ( Context * __context__, FILE const  * const  __doc_file_rename_at_867_369, Module * const  __mod_rename_at_867_370, char * const  __value_rename_at_867_371 )
{
    builtin_fwrite(__doc_file_rename_at_867_369,make_label_3368ba1f933c2731(__context__,_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c(__context__,__mod_rename_at_867_370,__value_rename_at_867_371)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_867_369,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_867_369,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_136, cast<char * const >::from(__value_rename_at_867_371), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_867_369,topic_9659659d94ddb29a(__context__,((char *) "reader_macro"),__mod_rename_at_867_370,__value_rename_at_867_371));
}

inline void document_reader_macros_585b57fffa625cd ( Context * __context__, FILE const  * const  __doc_file_rename_at_874_372, Module * const  __mod_rename_at_874_373 ) { das_stack_prologue __prologue(__context__,176,"document_reader_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_875_374 = false;
    for_each_reader_macro(__mod_rename_at_874_373,das_make_block<void,char *>(__context__,96,0,&__func_info__e0afcd004fbe6de3,[&](char * __value_rename_at_876_375) -> void{
        das_copy(__any_rename_at_875_374,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_875_374 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_874_372,((char *) "+++++++++++++\nReader macros\n+++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_reader_macro(__mod_rename_at_874_373,das_make_block<void,char *>(__context__,160,0,&__func_info__e0afcd004fbe6de3,[&](char * __value_rename_at_883_376) -> void{
            document_reader_macro_4006c26786c1908(__context__,__doc_file_rename_at_874_372,__mod_rename_at_874_373,__value_rename_at_883_376);
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_variant_macro_84da1b9ee053cb60 ( Context * __context__, FILE const  * const  __doc_file_rename_at_888_377, Module * const  __mod_rename_at_888_378, char * const  __value_rename_at_888_379 )
{
    builtin_fwrite(__doc_file_rename_at_888_377,make_label_3368ba1f933c2731(__context__,_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c(__context__,__mod_rename_at_888_378,__value_rename_at_888_379)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_888_377,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_888_377,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_137, cast<char * const >::from(__value_rename_at_888_379), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_888_377,topic_9659659d94ddb29a(__context__,((char *) "variant_macro"),__mod_rename_at_888_378,__value_rename_at_888_379));
}

inline void document_variant_macros_eab4d095021756ef ( Context * __context__, FILE const  * const  __doc_file_rename_at_895_380, Module * const  __mod_rename_at_895_381 ) { das_stack_prologue __prologue(__context__,176,"document_variant_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_896_382 = false;
    for_each_variant_macro(__mod_rename_at_895_381,das_make_block<void,smart_ptr<VariantMacro>>(__context__,96,0,&__func_info__84319ec9af82a642,[&](smart_ptr<VariantMacro> __value_rename_at_897_383) -> void{
        das_copy(__any_rename_at_896_382,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_896_382 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_895_380,((char *) "++++++++++++++\nVariant macros\n++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_variant_macro(__mod_rename_at_895_381,das_make_block<void,smart_ptr<VariantMacro>>(__context__,160,0,&__func_info__84319ec9af82a642,[&](smart_ptr<VariantMacro> __value_rename_at_904_384) -> void{
            document_variant_macro_84da1b9ee053cb60(__context__,__doc_file_rename_at_895_380,__mod_rename_at_895_381,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_904_384->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline void document_typeinfo_macro_66a7bf37ca0e4fcd ( Context * __context__, FILE const  * const  __doc_file_rename_at_909_385, Module * const  __mod_rename_at_909_386, char * const  __value_rename_at_909_387 )
{
    builtin_fwrite(__doc_file_rename_at_909_385,make_label_3368ba1f933c2731(__context__,_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c(__context__,__mod_rename_at_909_386,__value_rename_at_909_387)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_909_385,((char *) ".. das:attribute:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_909_385,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_138, cast<char * const >::from(__value_rename_at_909_387), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_909_385,topic_9659659d94ddb29a(__context__,((char *) "typeinfo_macro"),__mod_rename_at_909_386,__value_rename_at_909_387));
}

inline void document_typeinfo_macros_b4b5371c027f780c ( Context * __context__, FILE const  * const  __doc_file_rename_at_916_388, Module * const  __mod_rename_at_916_389 ) { das_stack_prologue __prologue(__context__,176,"document_typeinfo_macros " DAS_FILE_LINE);
{
    bool __any_rename_at_917_390 = false;
    for_each_typeinfo_macro(__mod_rename_at_916_389,das_make_block<void,smart_ptr<TypeInfoMacro>>(__context__,96,0,&__func_info__5f3d7fba1c40074b,[&](smart_ptr<TypeInfoMacro> __value_rename_at_918_391) -> void{
        das_copy(__any_rename_at_917_390,true);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !__any_rename_at_917_390 )
    {
        return ;
    } else {
        builtin_fwrite(__doc_file_rename_at_916_388,((char *) "+++++++++++++++\nTypeinfo macros\n+++++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        for_each_typeinfo_macro(__mod_rename_at_916_389,das_make_block<void,smart_ptr<TypeInfoMacro>>(__context__,160,0,&__func_info__5f3d7fba1c40074b,[&](smart_ptr<TypeInfoMacro> __value_rename_at_925_392) -> void{
            document_typeinfo_macro_66a7bf37ca0e4fcd(__context__,__doc_file_rename_at_916_388,__mod_rename_at_916_389,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_925_392->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}

inline bool function_needs_documenting_5ed98b6306abb3d7 ( Context * __context__, smart_ptr_raw<Function> const  __func_rename_at_930_393 )
{
    if ( das_get_bitfield(__func_rename_at_930_393->flags /*flags*/,1u << 21) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( das_get_bitfield(__func_rename_at_930_393->flags /*flags*/,1u << 27) )
        {
            return das_auto_cast<bool>::cast(false);
        } else {
            if ( das_get_bitfield(__func_rename_at_930_393->flags /*flags*/,1u << 22) )
            {
                return das_auto_cast<bool>::cast(false);
            } else {
                if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__func_rename_at_930_393->fromGeneric /*fromGeneric*/),das_auto_cast<void * const >::cast(nullptr)) )
                {
                    return das_auto_cast<bool>::cast(false);
                } else {
                    if ( das_get_bitfield(__func_rename_at_930_393->moreFlags /*moreFlags*/,1u << 12) )
                    {
                        return das_auto_cast<bool>::cast(false);
                    } else {
                        bool __isBuiltin_rename_at_946_394 = false;
                        peek_das_string_T(__func_rename_at_930_393->module /*_module*/->name /*name*/,[&](char * const  __name_rename_at_947_395) DAS_AOT_INLINE_LAMBDA -> void{
                            if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_947_395),cast<char *>::from(((char *) "$")),*__context__,nullptr) )
                            {
                                das_copy(__isBuiltin_rename_at_946_394,true);
                            };
                        },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        bool __isOperator_rename_at_953_396 = false;
                        if ( __isBuiltin_rename_at_946_394 )
                        {
                            peek_das_string_T(__func_rename_at_930_393->name /*name*/,[&](char * const  __name_rename_at_955_397) DAS_AOT_INLINE_LAMBDA -> void{
                                int32_t __ch_rename_at_956_398 = ((int32_t)get_character_at(__name_rename_at_955_397,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
                                if ( (!(is_alpha(__ch_rename_at_956_398)) && !(is_number(__ch_rename_at_956_398))) && (__ch_rename_at_956_398 != 95) )
                                {
                                    das_copy(__isOperator_rename_at_953_396,true);
                                };
                            },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        };
                        if ( __isOperator_rename_at_953_396 )
                        {
                            return das_auto_cast<bool>::cast(false);
                        } else {
                            bool __isTypeConstructor_rename_at_966_399 = false;
                            if ( __isBuiltin_rename_at_946_394 )
                            {
                                peek_das_string_T(__func_rename_at_930_393->name /*name*/,[&](char * const  __name_rename_at_968_400) DAS_AOT_INLINE_LAMBDA -> void{
                                    if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__func_rename_at_930_393->result /*result*/),das_auto_cast<void * const >::cast(nullptr))) && ((das_deref(__context__,__func_rename_at_930_393->result /*result*/)).isCtorType()) )
                                    {
                                        char * __tname_rename_at_970_401 = ((char *)(char *)(((char * const )(ast_das_to_string(__func_rename_at_930_393->result /*result*/->baseType /*baseType*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                                        if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_968_400),cast<char *>::from(__tname_rename_at_970_401),*__context__,nullptr) )
                                        {
                                            das_copy(__isTypeConstructor_rename_at_966_399,true);
                                        };
                                    };
                                },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                            };
                            if ( __isTypeConstructor_rename_at_966_399 )
                            {
                                return das_auto_cast<bool>::cast(false);
                            } else {
                                bool __isBuiltinName_rename_at_981_402 = false;
                                peek_das_string_T(__func_rename_at_930_393->name /*name*/,[&](char * const  __name_rename_at_982_403) DAS_AOT_INLINE_LAMBDA -> void{
                                    if ( builtin_string_startswith(__name_rename_at_982_403,((char *) "_builtin"),__context__) || builtin_string_startswith(__name_rename_at_982_403,((char *) "__builtin"),__context__) )
                                    {
                                        das_copy(__isBuiltinName_rename_at_981_402,true);
                                    };
                                },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                                return das_auto_cast<bool>::cast(__isBuiltinName_rename_at_981_402 ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true));
                            };
                        };
                    };
                };
            };
        };
    };
}

inline void document_warning_cb981aa521e01a92 ( Context * __context__, FILE const  * const  __doc_file_rename_at_993_404, char * const  __issue_rename_at_993_405 )
{
    builtin_fwrite(__doc_file_rename_at_993_404,((char *) ".. warning:: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__doc_file_rename_at_993_404,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_139, cast<char * const >::from(((char *) "\n  ")), cast<char * const >::from(__issue_rename_at_993_405), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * function_name_ee62641d633d27c8 ( Context * __context__, char * const  __name_rename_at_998_406 )
{
    if ( !builtin_empty(__name_rename_at_998_406) )
    {
        int32_t __ch_rename_at_1000_407 = ((int32_t)get_character_at(__name_rename_at_998_406,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        if ( (__ch_rename_at_1000_407 != 95) && !(is_alpha(__ch_rename_at_1000_407)) )
        {
            return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_140, cast<char * const >::from(((char *) "operator ")), cast<char * const >::from(__name_rename_at_998_406))));
        };
    };
    return das_auto_cast<char *>::cast(__name_rename_at_998_406);
}

inline void document_functions_a509d99cbdb2836d ( Context * __context__, FILE const  * const  __doc_file_rename_at_1027_408, Module * const  __mod_rename_at_1027_409, TArray<rst::DocGroup> &  __groups_rename_at_1027_410 ) { das_stack_prologue __prologue(__context__,384,"document_functions " DAS_FILE_LINE);
{
    TTable<Function *,bool> __tab_rename_at_1028_411; das_zero(__tab_rename_at_1028_411);
    {
        bool __need_loop_1029 = true;
        // grp: rst::DocGroup&
        das_iterator<TArray<rst::DocGroup>> __grp_iterator(__groups_rename_at_1027_410);
        rst::DocGroup * __grp_rename_at_1029_412;
        __need_loop_1029 = __grp_iterator.first(__context__,(__grp_rename_at_1029_412)) && __need_loop_1029;
        for ( ; __need_loop_1029 ; __need_loop_1029 = __grp_iterator.next(__context__,(__grp_rename_at_1029_412)) )
        {
            if ( builtin_array_size(das_arg<TArray<Function *>>::pass((*__grp_rename_at_1029_412).func)) != 0 )
            {
                if ( true && (*__grp_rename_at_1029_412).hidden )
                {
                    {
                        bool __need_loop_1032 = true;
                        // func: ast::Function?&
                        das_iterator<TArray<Function *>> __func_iterator((*__grp_rename_at_1029_412).func);
                        Function * * __func_rename_at_1032_413;
                        __need_loop_1032 = __func_iterator.first(__context__,(__func_rename_at_1032_413)) && __need_loop_1032;
                        for ( ; __need_loop_1032 ; __need_loop_1032 = __func_iterator.next(__context__,(__func_rename_at_1032_413)) )
                        {
                            _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_411),(*__func_rename_at_1032_413),true);
                        }
                        __func_iterator.close(__context__,(__func_rename_at_1032_413));
                    };
                } else {
                    builtin_fwrite(__doc_file_rename_at_1027_408,make_group_8b304b06bc5b7645(__context__,(*__grp_rename_at_1029_412).name,((char *) "+")),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    if ( (*__grp_rename_at_1029_412).hidden )
                    {
                        document_warning_cb981aa521e01a92(__context__,__doc_file_rename_at_1027_408,((char *) "This group of functions is hidden. It will not be in the final documentation."));
                    };
                    {
                        bool __need_loop_1040 = true;
                        // func: ast::Function?&
                        das_iterator<TArray<Function *>> __func_iterator((*__grp_rename_at_1029_412).func);
                        Function * * __func_rename_at_1040_414;
                        __need_loop_1040 = __func_iterator.first(__context__,(__func_rename_at_1040_414)) && __need_loop_1040;
                        for ( ; __need_loop_1040 ; __need_loop_1040 = __func_iterator.next(__context__,(__func_rename_at_1040_414)) )
                        {
                            if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_4e6333822225d431(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_411),(*__func_rename_at_1040_414)) )
                            {
                                char * __descr_rename_at_1042_415 = ((char *)(char *)(_Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f(__context__,(*__func_rename_at_1040_414))));
                                char * __labn_rename_at_1043_416 = (char *)(_FuncrstTickfunction_label_nameTick4951524166161443384_ded37d7d287c25b5(__context__,(*__func_rename_at_1040_414)));
                                builtin_fwrite(__doc_file_rename_at_1027_408,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_141, cast<char * const >::from(((char *) "  * ")), cast<char * const >::from(make_ref_c995159f43ed1b43(__context__,__labn_rename_at_1043_416,__descr_rename_at_1042_415)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                            };
                        }
                        __func_iterator.close(__context__,(__func_rename_at_1040_414));
                    };
                    builtin_fwrite(__doc_file_rename_at_1027_408,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    {
                        bool __need_loop_1048 = true;
                        // func: ast::Function?&
                        das_iterator<TArray<Function *>> __func_iterator((*__grp_rename_at_1029_412).func);
                        Function * * __func_rename_at_1048_417;
                        __need_loop_1048 = __func_iterator.first(__context__,(__func_rename_at_1048_417)) && __need_loop_1048;
                        for ( ; __need_loop_1048 ; __need_loop_1048 = __func_iterator.next(__context__,(__func_rename_at_1048_417)) )
                        {
                            if ( _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_411),(*__func_rename_at_1048_417)) )
                            {
                                continue;
                            } else {
                                _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_411),(*__func_rename_at_1048_417),true);
                                _FuncrstTickdocument_functionTick8829733267459955112_767667cb9ac814a7(__context__,__doc_file_rename_at_1027_408,__mod_rename_at_1027_409,(*__func_rename_at_1048_417));
                            };
                        }
                        __func_iterator.close(__context__,(__func_rename_at_1048_417));
                    };
                };
            };
        }
        __grp_iterator.close(__context__,(__grp_rename_at_1029_412));
    };
    bool __first_function_rename_at_1058_418 = true;
    forEachFunction(__mod_rename_at_1027_409,nullptr,das_make_block<void,smart_ptr<Function>>(__context__,272,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_1059_419) -> void{
        if ( !function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1059_419) )
        {
            return ;
        };
        Function * __pfunc_rename_at_1063_420 = _FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615(__context__,__func_rename_at_1059_419);
        if ( _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_411),__pfunc_rename_at_1063_420) )
        {
            return ;
        };
        _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_411),__pfunc_rename_at_1063_420,true);
        if ( __first_function_rename_at_1058_418 )
        {
            builtin_fwrite(__doc_file_rename_at_1027_408,((char *) "+++++++++++++\nUncategorized\n+++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            das_copy(__first_function_rename_at_1058_418,false);
        };
        _FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00(__context__,__doc_file_rename_at_1027_408,__mod_rename_at_1027_409,__func_rename_at_1059_419);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    for_each_generic(__mod_rename_at_1027_409,das_make_block<void,smart_ptr<Function>>(__context__,352,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_1074_421) -> void{
        if ( !function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1074_421) )
        {
            return ;
        };
        Function * __pfunc_rename_at_1078_422 = _FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615(__context__,__func_rename_at_1074_421);
        if ( _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_411),__pfunc_rename_at_1078_422) )
        {
            return ;
        };
        _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a(__context__,das_arg<TTable<Function *,bool>>::pass(__tab_rename_at_1028_411),__pfunc_rename_at_1078_422,true);
        if ( __first_function_rename_at_1058_418 )
        {
            builtin_fwrite(__doc_file_rename_at_1027_408,((char *) "+++++++++++++\nUncategorized\n+++++++++++++\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            das_copy(__first_function_rename_at_1058_418,false);
        };
        _FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00(__context__,__doc_file_rename_at_1027_408,__mod_rename_at_1027_409,__func_rename_at_1074_421);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline rst::DocGroup debug_group_d90dc61b38b89347 ( Context * __context__, rst::DocGroup &  __group_rename_at_1098_423 )
{
    toLog(10000,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_142, cast<char * const >::from(((char *) "Group ")), cast<char *>::from(__group_rename_at_1098_423.name), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_1100 = true;
        // func: ast::Function?&
        das_iterator<TArray<Function *>> __func_iterator(__group_rename_at_1098_423.func);
        Function * * __func_rename_at_1100_424;
        __need_loop_1100 = __func_iterator.first(__context__,(__func_rename_at_1100_424)) && __need_loop_1100;
        for ( ; __need_loop_1100 ; __need_loop_1100 = __func_iterator.next(__context__,(__func_rename_at_1100_424)) )
        {
            toLog(10000,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_143, cast<char * const >::from(((char *) "\t")), cast<char * const >::from(_Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f(__context__,(*__func_rename_at_1100_424))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }
        __func_iterator.close(__context__,(__func_rename_at_1100_424));
    };
    return /* <- */ das_auto_cast_move<rst::DocGroup>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_c745a97595700ef(__context__,das_arg<rst::DocGroup>::pass(__group_rename_at_1098_423)));
}

inline void append_to_group_by_regex_819dad4f532ef50d ( Context * __context__, rst::DocGroup &  __group_rename_at_1106_425, Module * const  __mod_rename_at_1106_426, regex::Regex &  __reg_rename_at_1106_427 ) { das_stack_prologue __prologue(__context__,160,"append_to_group_by_regex " DAS_FILE_LINE);
{
    forEachFunction(__mod_rename_at_1106_426,nullptr,das_make_block<void,smart_ptr<Function>>(__context__,80,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_1107_428) -> void{
        if ( function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1107_428) )
        {
            if ( das_invoke_function<int32_t>::invoke<regex::Regex &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_match S<regex::Regex> Cs Ci*/ 186815859536751917u)),das_arg<regex::Regex>::pass(__reg_rename_at_1106_427),((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_1107_428->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),0) != -1 )
            {
                _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50(__context__,das_arg<TArray<Function *>>::pass(__group_rename_at_1106_425.func),_FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615(__context__,__func_rename_at_1107_428));
            };
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    for_each_generic(__mod_rename_at_1106_426,das_make_block<void,smart_ptr<Function>>(__context__,144,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_1114_429) -> void{
        if ( function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1114_429) )
        {
            if ( das_invoke_function<int32_t>::invoke<regex::Regex &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_match S<regex::Regex> Cs Ci*/ 186815859536751917u)),das_arg<regex::Regex>::pass(__reg_rename_at_1106_427),((char * const )(to_das_string(das_arg<das::string>::pass(__func_rename_at_1114_429->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),0) != -1 )
            {
                _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50(__context__,das_arg<TArray<Function *>>::pass(__group_rename_at_1106_425.func),_FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615(__context__,__func_rename_at_1114_429));
            };
        };
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline rst::DocGroup group_by_regex_def313b7b3c9e262 ( Context * __context__, char * const  __name_rename_at_1123_430, Module * const  __mod_rename_at_1123_431, regex::Regex &  __reg_rename_at_1123_432 )
{
    rst::DocGroup __group_rename_at_1126_433; das_zero(__group_rename_at_1126_433); das_move(__group_rename_at_1126_433, (([&]() -> rst::DocGroup {
        rst::DocGroup __mks_1126 = DocGroup_6e55999f1d438d24(__context__);
        das_copy((__mks_1126.name),(__name_rename_at_1123_430));
        das_copy((__mks_1126.hidden),(false));
        return __mks_1126;
    })()));
    append_to_group_by_regex_819dad4f532ef50d(__context__,das_arg<rst::DocGroup>::pass(__group_rename_at_1126_433),__mod_rename_at_1123_431,das_arg<regex::Regex>::pass(__reg_rename_at_1123_432));
    return /* <- */ das_auto_cast_move<rst::DocGroup>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_c745a97595700ef(__context__,das_arg<rst::DocGroup>::pass(__group_rename_at_1126_433)));
}

inline rst::DocGroup hide_group_4868115ae5eeb38f ( Context * __context__, rst::DocGroup &  __group_rename_at_1131_434 )
{
    das_copy(__group_rename_at_1131_434.hidden,true);
    return /* <- */ das_auto_cast_move<rst::DocGroup>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_c745a97595700ef(__context__,das_arg<rst::DocGroup>::pass(__group_rename_at_1131_434)));
}

inline void generate_topic_stub_2777760f2b77d9f0 ( Context * __context__, TTable<char *,bool> &  __tab_rename_at_1137_435, FILE const  * const  __doc_file_rename_at_1137_436, char * const  __topic_rename_at_1137_437 )
{
    if ( !_FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1137_435),__topic_rename_at_1137_437) )
    {
        _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1137_435),__topic_rename_at_1137_437,true);
        builtin_fwrite(__doc_file_rename_at_1137_436,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_144, cast<char * const >::from(((char *) ".. ")), cast<char * const >::from(__topic_rename_at_1137_437), cast<char * const >::from(((char *) " replace:: to be documented in ")), cast<char * const >::from(__topic_rename_at_1137_437), cast<char * const >::from(((char *) ".rst\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void generate_substitute_stub_34b873e38788090 ( Context * __context__, Module * const  __mod_rename_at_1144_438, char * const  __substname_rename_at_1144_439 ) { das_stack_prologue __prologue(__context__,864,"generate_substitute_stub " DAS_FILE_LINE);
{
    FStat __st_rename_at_1145_440; das_zero(__st_rename_at_1145_440);
    if ( builtin_stat(__substname_rename_at_1144_439,das_arg<FStat>::pass(__st_rename_at_1145_440)) )
    {
        return ;
    } else {
        _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,__substname_rename_at_1144_439,((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,240,0,&__func_info__6edfc1622f67255a,[&](FILE const  * const  __stub_file_rename_at_1149_441) -> void{
            if ( __stub_file_rename_at_1149_441 != nullptr )
            {
                TTable<char *,bool> __tab_rename_at_1151_442; das_zero(__tab_rename_at_1151_442);
                for_each_typedef(__mod_rename_at_1144_438,das_make_block<void,char *,smart_ptr<TypeDecl>>(__context__,352,0,&__func_info__aa03c1bcb8f15255,[&](char * __name_rename_at_1153_443, smart_ptr<TypeDecl> __value_rename_at_1153_444) -> void{
                    generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "typedef"),__mod_rename_at_1144_438,__name_rename_at_1153_443));
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                for_each_enumeration(__mod_rename_at_1144_438,das_make_block<void,smart_ptr<Enumeration>>(__context__,416,0,&__func_info__9beafd8de389bb7e,[&](smart_ptr<Enumeration> __value_rename_at_1157_445) -> void{
                    generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "enumeration"),__mod_rename_at_1144_438,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_1157_445->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                for_each_structure(__mod_rename_at_1144_438,das_make_block<void,smart_ptr<Structure>>(__context__,480,0,&__func_info__438460bc7bed8ede,[&](smart_ptr<Structure> __value_rename_at_1161_446) -> void{
                    if ( das_get_bitfield(__value_rename_at_1161_446->flags /*flags*/,1u << 0) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "class"),__mod_rename_at_1144_438,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_1161_446->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                        {
                            bool __need_loop_1165 = true;
                            // fld: ast::FieldDeclaration&
                            das_iterator<das::vector<Structure::FieldDeclaration>> __fld_iterator(__value_rename_at_1161_446->fields /*fields*/);
                            Structure::FieldDeclaration * __fld_rename_at_1165_447;
                            __need_loop_1165 = __fld_iterator.first(__context__,(__fld_rename_at_1165_447)) && __need_loop_1165;
                            for ( ; __need_loop_1165 ; __need_loop_1165 = __fld_iterator.next(__context__,(__fld_rename_at_1165_447)) )
                            {
                                if ( ((das_invoke_function<bool>::invoke<smart_ptr_raw<Structure> const ,smart_ptr_raw<TypeDecl> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::is_class_method CY<StructurePtr>1<H<ast::Structure>>?M CY<TypeDeclPtr>1<H<ast::TypeDecl>>?M*/ 11657066889097786130u)),__value_rename_at_1161_446,(*__fld_rename_at_1165_447).type /*_type*/) && !(das_get_bitfield((*__fld_rename_at_1165_447).flags /*flags*/,1u << 3))) && !(das_get_bitfield((*__fld_rename_at_1165_447).flags /*flags*/,1u << 1))) && das_get_bitfield((*__fld_rename_at_1165_447).flags /*flags*/,1u << 8) )
                                {
                                    generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "method"),__mod_rename_at_1144_438,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_145, cast<das::string &>::from(__value_rename_at_1161_446->name /*name*/), cast<char * const >::from(((char *) ".")), cast<das::string &>::from((*__fld_rename_at_1165_447).name /*name*/)))));
                                };
                            }
                            __fld_iterator.close(__context__,(__fld_rename_at_1165_447));
                        };
                    } else if ( !das_get_bitfield(__value_rename_at_1161_446->flags /*flags*/,1u << 4) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "structure"),__mod_rename_at_1144_438,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_1161_446->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                forEachFunction(__mod_rename_at_1144_438,nullptr,das_make_block<void,smart_ptr<Function>>(__context__,656,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_1175_448) -> void{
                    if ( function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1175_448) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "function"),__mod_rename_at_1144_438,das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_146, cast<char * const >::from(_FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b(__context__,__func_rename_at_1175_448))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                for_each_generic(__mod_rename_at_1144_438,das_make_block<void,smart_ptr<Function>>(__context__,720,0,&__func_info__8075d08fbfce07f4,[&](smart_ptr<Function> __func_rename_at_1180_449) -> void{
                    if ( function_needs_documenting_5ed98b6306abb3d7(__context__,__func_rename_at_1180_449) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "function"),__mod_rename_at_1144_438,das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_147, cast<char * const >::from(_FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b(__context__,__func_rename_at_1180_449))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                rtti_builtin_module_for_each_annotation(__mod_rename_at_1144_438,das_make_block<void,Annotation const  &>(__context__,784,0,&__func_info__f381b060f4dbce7,[&](Annotation const  & __value_rename_at_1186_450) -> void{
                    if ( ((__value_rename_at_1186_450).rtti_isBasicStructureAnnotation()) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "structure_annotation"),__mod_rename_at_1144_438,((char * const )(to_das_string(__value_rename_at_1186_450.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                    } else if ( ((__value_rename_at_1186_450).rtti_isHandledTypeAnnotation()) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "any_annotation"),__mod_rename_at_1144_438,((char * const )(to_das_string(__value_rename_at_1186_450.name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                for_each_global(__mod_rename_at_1144_438,das_make_block<void,smart_ptr<Variable>>(__context__,848,0,&__func_info__2701da5c12e56b0e,[&](smart_ptr<Variable> __value_rename_at_1194_451) -> void{
                    if ( is_global_constant_320cc2da3d8764f7(__context__,__value_rename_at_1194_451) )
                    {
                        generate_topic_stub_2777760f2b77d9f0(__context__,das_arg<TTable<char *,bool>>::pass(__tab_rename_at_1151_442),__stub_file_rename_at_1149_441,topic_9659659d94ddb29a(__context__,((char *) "variable"),__mod_rename_at_1144_438,((char * const )(to_das_string(das_arg<das::string>::pass(__value_rename_at_1194_451->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                    };
                }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            } else {
                builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_148, cast<char * const >::from(((char *) "can't generate stub at ")), cast<char * const >::from(__substname_rename_at_1144_439), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }));
    };
}}

inline char * getDetailIncludePath_fb77bda786a398b2 ( Context * __context__, char * const  __fname_rename_at_1205_452, char * const  __substname_rename_at_1205_453 )
{
    Sequence DAS_COMMENT((char * *)) _temp_make_local_1216_35_160; _temp_make_local_1216_35_160;
    TArray<char *> __fnp_rename_at_1206_454; das_zero(__fnp_rename_at_1206_454); das_move(__fnp_rename_at_1206_454, das_invoke_function<TArray<char *>>::invoke_cmres<char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::split_by_chars CIs CIs*/ 12647279460675647942u)),__fname_rename_at_1205_452,((char *) "\\/")));
    TArray<char *> __snp_rename_at_1207_455; das_zero(__snp_rename_at_1207_455); das_move(__snp_rename_at_1207_455, das_invoke_function<TArray<char *>>::invoke_cmres<char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::split_by_chars CIs CIs*/ 12647279460675647942u)),__substname_rename_at_1205_453,((char *) "\\/")));
    int32_t __index_rename_at_1208_456 = 0;
    {
        bool __need_loop_1209 = true;
        // f: string&
        das_iterator<TArray<char *>> __f_iterator(__fnp_rename_at_1206_454);
        char * * __f_rename_at_1209_459;
        __need_loop_1209 = __f_iterator.first(__context__,(__f_rename_at_1209_459)) && __need_loop_1209;
        // s: string&
        das_iterator<TArray<char *>> __s_iterator(__snp_rename_at_1207_455);
        char * * __s_rename_at_1209_460;
        __need_loop_1209 = __s_iterator.first(__context__,(__s_rename_at_1209_460)) && __need_loop_1209;
        for ( ; __need_loop_1209 ; __need_loop_1209 = __f_iterator.next(__context__,(__f_rename_at_1209_459)) && __s_iterator.next(__context__,(__s_rename_at_1209_460)) )
        {
            if ( SimPolicy<char *>::NotEqu(cast<char *>::from((*__f_rename_at_1209_459)),cast<char *>::from((*__s_rename_at_1209_460)),*__context__,nullptr) )
            {
                break;
            } else {
                ++__index_rename_at_1208_456;
            };
        }
        __f_iterator.close(__context__,(__f_rename_at_1209_459));
        __s_iterator.close(__context__,(__s_rename_at_1209_460));
    };
    char * __ipath_rename_at_1216_461 = (char *)(_Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9(__context__,das_arg<Sequence DAS_COMMENT((char *))>::pass((_temp_make_local_1216_35_160 = (_FuncbuiltinTickeach_refTick16137060296763333387_f3ba366f8c86460e(__context__,das_ascend<bool,rst::_lambda_rst_1216_1>::make(__context__,&__type_info__8e26ff93c3acecc5,(([&]() -> rst::_lambda_rst_1216_1 {
        rst::_lambda_rst_1216_1 __mks_1216;
        das_zero(__mks_1216);
        das_copy((__mks_1216.__lambda),(Func(__context__->fnByMangledName(/*@rst::_lambda_rst_1216_1`function XS<rst::_lambda_rst_1216_1> &1<s>?*/ 2338192599070949714u))));
        das_copy((__mks_1216.__finalize),(Func(__context__->fnByMangledName(/*@rst::_lambda_rst_1216_1`finalizer X1<S<rst::_lambda_rst_1216_1>>?*/ 7941040411089408739u))));
        das_copy((__mks_1216.index),(__index_rename_at_1208_456));
        das_move((__mks_1216.snp),(__snp_rename_at_1207_455));
        return __mks_1216;
    })())))))),((char *) "/")));
    return das_auto_cast<char *>::cast(__ipath_rename_at_1216_461);
}

inline void document_96eb220f96118989 ( Context * __context__, char * const  __name_rename_at_1221_462, Module * const  __mod_rename_at_1221_463, char * const  __fname_rename_at_1221_464, char * const  __substname_rename_at_1221_465, TArray<rst::DocGroup> &  __groups_rename_at_1221_466 )
{
    toLog(20000,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_149, cast<char * const >::from(((char *) "Documenting ")), cast<char * const >::from(__name_rename_at_1221_462), cast<char * const >::from(((char *) " into ")), cast<char * const >::from(__fname_rename_at_1221_464), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __mod_rename_at_1221_463 == nullptr )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_150, cast<char * const >::from(((char *) "missing module ")), cast<char * const >::from(__name_rename_at_1221_462))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    FILE const  * __doc_file_rename_at_1231_467 = builtin_fopen(__fname_rename_at_1221_464,((char *) "wb"));
    if ( __doc_file_rename_at_1231_467 == nullptr )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_151, cast<char * const >::from(((char *) "can't open ")), cast<char * const >::from(__fname_rename_at_1221_464))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    builtin_fwrite(__doc_file_rename_at_1231_467,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    char * __mod_name_rename_at_1236_468 = (char *)(((char * const )(to_das_string(__mod_rename_at_1221_463->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__mod_name_rename_at_1236_468),cast<char *>::from(((char *) "$")),*__context__,nullptr) )
    {
        das_copy(__mod_name_rename_at_1236_468,((char *) "_builtin"));
    };
    builtin_fwrite(__doc_file_rename_at_1231_467,make_header_efad6667e51b3e08(__context__,__name_rename_at_1221_462,__mod_name_rename_at_1236_468),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( builtin_string_length(__substname_rename_at_1221_465,__context__) != 0 )
    {
        builtin_fwrite(__doc_file_rename_at_1231_467,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_152, cast<char * const >::from(((char *) ".. include:: ")), cast<char * const >::from(getDetailIncludePath_fb77bda786a398b2(__context__,__fname_rename_at_1221_464,__substname_rename_at_1221_465)), cast<char * const >::from(((char *) "\n\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        generate_substitute_stub_34b873e38788090(__context__,__mod_rename_at_1221_463,__substname_rename_at_1221_465);
    };
    document_topic_c6e2614dff9f2d3e(__context__,__doc_file_rename_at_1231_467,topic_b97c7facf4ccb6a9(__context__,__mod_rename_at_1221_463));
    document_typedefs_d1f98d27346b80af(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_global_constants_24362fe0342586e0(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_enumerations_b052b53cf3b12eb(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_structures_5fb20d0dee04ba46(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_structure_annotations_a53005d9c9a5617(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_function_annotations_5344d92e9ea569ad(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_call_macros_56da413fe6ccb104(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_reader_macros_585b57fffa625cd(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_variant_macros_eab4d095021756ef(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_typeinfo_macros_b4b5371c027f780c(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_annotations_128851a794d46d1a(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_structure_macros_7386ca2820215b3d(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_classes_55fe4b580059698a(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463);
    document_functions_a509d99cbdb2836d(__context__,__doc_file_rename_at_1231_467,__mod_rename_at_1221_463,das_arg<TArray<rst::DocGroup>>::pass(__groups_rename_at_1221_466));
    builtin_fwrite(__doc_file_rename_at_1231_467,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fclose(__doc_file_rename_at_1231_467,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline rst::DocGroup DocGroup_6e55999f1d438d24 ( Context * __context__ )
{
    rst::DocGroup _temp_make_local_1091_13_32; _temp_make_local_1091_13_32;
    return /* <- */ das_auto_cast_move<rst::DocGroup>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_c745a97595700ef(__context__,das_arg<rst::DocGroup>::pass((([&]() -> rst::DocGroup& {
        das_zero(_temp_make_local_1091_13_32);
        return _temp_make_local_1091_13_32;
    })()))));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x393f34d50cd6136b] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, TDim<TTuple<16,char *,char *>,1> const  &  ),&_FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9>>();
    };
    aotLib[0x1a7ae42b3df9f4ba] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTicklengthTick18150397773952384912_75e0f8673d04729f
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, TDim<TTuple<16,char *,char *>,13> const  &  ),&_FuncbuiltinTicklengthTick18150397773952384912_75e0f8673d04729f>>();
    };
    aotLib[0x734e45a36711660] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<TTuple<16,char *,char *>> & , int32_t  ),&_FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e>>();
    };
    aotLib[0x8287ab4f1cdf8595] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTicklengthTick18150397773952384912_626ed670fc52be6d
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, TDim<TTuple<16,char *,char *>,3> const  &  ),&_FuncbuiltinTicklengthTick18150397773952384912_626ed670fc52be6d>>();
    };
    aotLib[0x380b7455b0a2f9f5] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<char *> & , int32_t  ),&_FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1>>();
    };
    aotLib[0x615f2ea0adab2f0] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf
        return ctx.code->makeNode<SimNode_AotCMRES<TArray<TTuple<16,char *,char *>> (*) ( Context * __context__, TDim<TTuple<16,char *,char *>,1> &  ),&_FuncbuiltinTickto_array_moveTick3185538323411982277_b2a83ed3c65f5adf>>();
    };
    aotLib[0x863c4893da830593] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickeachTick4044333107478717573_19fa17f56443c23c
        return ctx.code->makeNode<SimNode_AotCMRES<Sequence DAS_COMMENT((int32_t)) (*) ( Context * __context__, range  ),&_FuncbuiltinTickeachTick4044333107478717573_19fa17f56443c23c>>();
    };
    aotLib[0x913a8f2e9b96def6] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, TDim<char *,3> const  &  ),&_FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d>>();
    };
    aotLib[0x1d18c10ccd1f35e1] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, TDim<char *,2> const  &  ),&_FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b>>();
    };
    aotLib[0x78948a08f0d98f51] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<char *> &  ),&_FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8>>();
    };
    aotLib[0xc6d010c722352347] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<TypeDecl *> & , TypeDecl *  ),&_FuncbuiltinTickpushTick10769833213962245646_5351ef8f27690b66>>();
    };
    aotLib[0xe473f497a092ecde] = +[](Context & ctx) -> SimNode* { // _Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Sequence DAS_COMMENT((char * &)) & , char * const   ),&_Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9>>();
    };
    aotLib[0xc8c9111ff7052fb0] = +[](Context & ctx) -> SimNode* { // _FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Annotation const  &  ),&_FuncrstTickhandle_label_nameTick13616998130381234091_32d14a7ff33c9a7>>();
    };
    aotLib[0x47ee94f223768edd] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickemptyTick15399874715904164783_60d4e412e550e258
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, TArray<TArray<char *>> const  &  ),&_FuncbuiltinTickemptyTick15399874715904164783_60d4e412e550e258>>();
    };
    aotLib[0x39421012d4f6b3c0] = +[](Context & ctx) -> SimNode* { // _FuncrstTickfunction_label_nameTick4951524166161443384_aafe521f2cb4dc50
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<Function> const   ),&_FuncrstTickfunction_label_nameTick4951524166161443384_aafe521f2cb4dc50>>();
    };
    aotLib[0xa7c81ec8c5749cc0] = +[](Context & ctx) -> SimNode* { // finalize_5bf95341c40e5b82
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, rst::_lambda_rst_1216_1 &  ),&finalize_5bf95341c40e5b82>>();
    };
    aotLib[0xde6e54abcc6b70c2] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<char *> & , char *  ),&_FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1>>();
    };
    aotLib[0x17c076c38e276651] = +[](Context & ctx) -> SimNode* { // _Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, TArray<char *> const  & , char * const   ),&_Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835>>();
    };
    aotLib[0x6bfd914a4a3cd5f6] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f
        return ctx.code->makeNode<SimNode_AotCMRES<TArray<TTuple<16,char *,char *>> (*) ( Context * __context__, TDim<TTuple<16,char *,char *>,13> &  ),&_FuncbuiltinTickto_array_moveTick3185538323411982277_15a22c6b6b0bc46f>>();
    };
    aotLib[0x15ac5ce5ae27e745] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<Function *> & , Function *  ),&_FuncbuiltinTickpushTick10769833213962245646_8dc00978e9428d50>>();
    };
    aotLib[0xa930d5b7ad34076e] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickeach_refTick16137060296763333387_f3ba366f8c86460e
        return ctx.code->makeNode<SimNode_AotCMRES<Sequence DAS_COMMENT((char * &)) (*) ( Context * __context__, Lambda DAS_COMMENT((bool,char * * &)) const   ),&_FuncbuiltinTickeach_refTick16137060296763333387_f3ba366f8c86460e>>();
    };
    aotLib[0x680e3616c07f88fa] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e
        return ctx.code->makeNode<SimNode_Aot<TypeDecl * (*) ( Context * __context__, smart_ptr<TypeDecl>  ),&_FuncbuiltinTickget_ptrTick5807679485210906136_cf4fdd6fc99fcf2e>>();
    };
    aotLib[0xb0b31de6adf6ab5a] = +[](Context & ctx) -> SimNode* { // _FuncrstTickfunction_file_nameTick11623859247758201540_f84a97f3b3efcea2
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Function * const   ),&_FuncrstTickfunction_file_nameTick11623859247758201540_f84a97f3b3efcea2>>();
    };
    aotLib[0x12491fe0302ad886] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickto_array_moveTick3185538323411982277_2c64031744b1c1f2
        return ctx.code->makeNode<SimNode_AotCMRES<TArray<TTuple<16,char *,char *>> (*) ( Context * __context__, TDim<TTuple<16,char *,char *>,3> &  ),&_FuncbuiltinTickto_array_moveTick3185538323411982277_2c64031744b1c1f2>>();
    };
    aotLib[0xeb083a76a7b3ee6] = +[](Context & ctx) -> SimNode* { // _FuncrstTickdocument_typedefTick2903024037696058038_24a5c3bc896476d1
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , char * const  , smart_ptr<TypeDecl> const   ),&_FuncrstTickdocument_typedefTick2903024037696058038_24a5c3bc896476d1>>();
    };
    aotLib[0x6181c373c15dd97c] = +[](Context & ctx) -> SimNode* { // _FuncrstTickdocument_enumerationTick4211850574665690795_cd2737074b5000f2
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , smart_ptr<Enumeration> const   ),&_FuncrstTickdocument_enumerationTick4211850574665690795_cd2737074b5000f2>>();
    };
    aotLib[0xd6774f94607a3ea5] = +[](Context & ctx) -> SimNode* { // _FuncrstTickdocument_class_methodTick12589992380795293570_b6d1264d11ee8d48
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , smart_ptr<Structure> const  , Structure::FieldDeclaration &  ),&_FuncrstTickdocument_class_methodTick12589992380795293570_b6d1264d11ee8d48>>();
    };
    aotLib[0xcb52f85a549cbb9a] = +[](Context & ctx) -> SimNode* { // _FuncrstTickdocument_structure_annotationTick2734840712723762798_45f2db6d6c9372ba
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , Annotation const  &  ),&_FuncrstTickdocument_structure_annotationTick2734840712723762798_45f2db6d6c9372ba>>();
    };
    aotLib[0x2ecb0d6b1a1807b4] = +[](Context & ctx) -> SimNode* { // _FuncrstTickdocument_function_annotationTick9878220666429632644_62156d6269c1aee6
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , Annotation const  &  ),&_FuncrstTickdocument_function_annotationTick9878220666429632644_62156d6269c1aee6>>();
    };
    aotLib[0x499f06d37f5cdbfb] = +[](Context & ctx) -> SimNode* { // _FuncrstTickdocument_annotationTick7679268031359399902_8b47c8f555ccae04
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , Annotation const  &  ),&_FuncrstTickdocument_annotationTick7679268031359399902_8b47c8f555ccae04>>();
    };
    aotLib[0x7359db810e0a9362] = +[](Context & ctx) -> SimNode* { // _FuncrstTickdocument_structure_macroTick6088634580083749384_a8cf27101dd30a50
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , Annotation const  &  ),&_FuncrstTickdocument_structure_macroTick6088634580083749384_a8cf27101dd30a50>>();
    };
    aotLib[0x611e4c1295d672ad] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615
        return ctx.code->makeNode<SimNode_Aot<Function * (*) ( Context * __context__, smart_ptr<Function>  ),&_FuncbuiltinTickget_ptrTick5807679485210906136_a467400e4353f615>>();
    };
    aotLib[0xa4b642a3a8289fc7] = +[](Context & ctx) -> SimNode* { // _FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , smart_ptr_raw<Function>  ),&_FuncrstTickdocument_functionTick8829733267459955112_be48b0c3b23e8a00>>();
    };
    aotLib[0x7b26a6aa2b6ea57b] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_c745a97595700ef
        return ctx.code->makeNode<SimNode_Aot<rst::DocGroup & (*) ( Context * __context__, rst::DocGroup &  ),&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_c745a97595700ef>>();
    };
    aotLib[0x29b001e592d236a5] = +[](Context & ctx) -> SimNode* { // _FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<Function> const   ),&_FuncrstTickfunction_file_nameTick11623859247758201540_91a8d3feafb81f1b>>();
    };
    aotLib[0xf812fb454f3c6150] = +[](Context & ctx) -> SimNode* { // _Func_lambda_rst_1216_1Tickfunction_deb6e09d1773cc6a
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, rst::_lambda_rst_1216_1 & , char * * &  ),&_Func_lambda_rst_1216_1Tickfunction_deb6e09d1773cc6a>>();
    };
    aotLib[0xc5b67635f73c9aaf] = +[](Context & ctx) -> SimNode* { // _Func_lambda_rst_1216_1Tickfinalizer_d8783cc009d376af
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, rst::_lambda_rst_1216_1 *  ),&_Func_lambda_rst_1216_1Tickfinalizer_d8783cc009d376af>>();
    };
    aotLib[0xdb83ef5e07f54fc9] = +[](Context & ctx) -> SimNode* { // _FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Module * const   ),&_FuncrstTickmodule_nameTick8696646154046195577_13ad62cf25938ff1>>();
    };
    aotLib[0xdb4c66fdb62e6c3d] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick8468476673553620226_f1503a45e3ae58f3
        return ctx.code->makeNode<SimNode_Aot<Variable * (*) ( Context * __context__, smart_ptr_raw<Variable> const   ),&_FuncbuiltinTickget_ptrTick8468476673553620226_f1503a45e3ae58f3>>();
    };
    aotLib[0xa47525acff80991] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick8468476673553620226_d6dabe47fb1a720
        return ctx.code->makeNode<SimNode_Aot<Enumeration * (*) ( Context * __context__, smart_ptr_raw<Enumeration> const   ),&_FuncbuiltinTickget_ptrTick8468476673553620226_d6dabe47fb1a720>>();
    };
    aotLib[0x398d67528cabe5c6] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850
        return ctx.code->makeNode<SimNode_Aot<Structure * (*) ( Context * __context__, smart_ptr_raw<Structure> const   ),&_FuncbuiltinTickget_ptrTick8468476673553620226_88f5777a81397850>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* { // _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const  , bool , bool , bool  ),&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    aotLib[0x6701dc1abcd033d2] = +[](Context & ctx) -> SimNode* { // _FuncrstTickhandle_label_nameTick13616998130381234091_7f23dc2205503d09
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, TypeAnnotation * const   ),&_FuncrstTickhandle_label_nameTick13616998130381234091_7f23dc2205503d09>>();
    };
    aotLib[0xd5b298fc293f15ad] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<int32_t> & , int32_t  ),&_FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51>>();
    };
    aotLib[0x7c2e30b4091b7513] = +[](Context & ctx) -> SimNode* { // _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, char * const  , char * const  , Block DAS_COMMENT((void,FILE const  * const )) const  &  ),&_FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac>>();
    };
    aotLib[0x47ab915efca2326e] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickto_array_moveTick3185538323411982277_649bcd7e51242beb
        return ctx.code->makeNode<SimNode_AotCMRES<TArray<char *> (*) ( Context * __context__, TDim<char *,3> &  ),&_FuncbuiltinTickto_array_moveTick3185538323411982277_649bcd7e51242beb>>();
    };
    aotLib[0x3c395992954dd60d] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<char *> & , char * const   ),&_FuncbuiltinTickpushTick14133213201864676143_d626258b591934af>>();
    };
    aotLib[0x70f9e95328fb7cf5] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<TArray<char *>> & , TArray<char *> &  ),&_FuncbuiltinTickemplaceTick13923705686753630697_442a74d76462f0e6>>();
    };
    aotLib[0xfbfe800d5700e08f] = +[](Context & ctx) -> SimNode* { // _FuncastTickdescribeTick842554968825501494_452cfc8955963a18
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<Expression> const   ),&_FuncastTickdescribeTick842554968825501494_452cfc8955963a18>>();
    };
    aotLib[0x6e7a2bd0539ae9f5] = +[](Context & ctx) -> SimNode* { // _FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, TypeDecl * const   ),&_FuncrstTickargument_needs_documentingTick6302275088705453951_273f3d7792cf4bf6>>();
    };
    aotLib[0x611db07d61b26349] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickto_array_moveTick3185538323411982277_325c63fe2a174cf6
        return ctx.code->makeNode<SimNode_AotCMRES<TArray<char *> (*) ( Context * __context__, TDim<char *,2> &  ),&_FuncbuiltinTickto_array_moveTick3185538323411982277_325c63fe2a174cf6>>();
    };
    aotLib[0x1e8ffe44dcbaaa69] = +[](Context & ctx) -> SimNode* { // _FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Module * const  , char * const   ),&_FuncrstTickcall_macro_label_nameTick15436321614735730922_476b17e4e84f215c>>();
    };
    aotLib[0x4d18bbfb8b74944a] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TTable<Function *,bool> & , Function * const  , bool  ),&_FuncbuiltinTickinsertTick12964066441666329206_f55f0c6b1e544f2a>>();
    };
    aotLib[0x9ce6017dea85c52e] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickkey_existsTick16808803843923989214_4e6333822225d431
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, TTable<Function *,bool> const  & , Function * const   ),&_FuncbuiltinTickkey_existsTick16808803843923989214_4e6333822225d431>>();
    };
    aotLib[0x8fd1c19f4baf74f7] = +[](Context & ctx) -> SimNode* { // _Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Function * const   ),&_Funcast_boostTickdescribe_function_shortTick7837223431506487711_64a6f94268194e7f>>();
    };
    aotLib[0xc005a879f7e77d70] = +[](Context & ctx) -> SimNode* { // _FuncrstTickfunction_label_nameTick4951524166161443384_ded37d7d287c25b5
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Function * const   ),&_FuncrstTickfunction_label_nameTick4951524166161443384_ded37d7d287c25b5>>();
    };
    aotLib[0x6a48f146592c6948] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, TTable<Function *,bool> & , Function * const   ),&_FuncbuiltinTickget_valueTick6803070933163225147_f9c6c05d98ece72>>();
    };
    aotLib[0xde03b8865c4448be] = +[](Context & ctx) -> SimNode* { // _FuncrstTickdocument_functionTick8829733267459955112_767667cb9ac814a7
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , Function *  ),&_FuncrstTickdocument_functionTick8829733267459955112_767667cb9ac814a7>>();
    };
    aotLib[0xf7214cd218a61fdf] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, TTable<char *,bool> & , char * const   ),&_FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45>>();
    };
    aotLib[0xec1fda1174e8c717] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TTable<char *,bool> & , char * const  , bool  ),&_FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b>>();
    };
    aotLib[0x90571d6b86291165] = +[](Context & ctx) -> SimNode* { // alias_label_name_f0e3a6da4e226a3e
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const   ),&alias_label_name_f0e3a6da4e226a3e>>();
    };
    aotLib[0x47063727e0446768] = +[](Context & ctx) -> SimNode* { // global_label_name_2b7e96918b37c00e
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Variable * const   ),&global_label_name_2b7e96918b37c00e>>();
    };
    aotLib[0xe4f1e61c41734632] = +[](Context & ctx) -> SimNode* { // global_label_name_b721f72143d35230
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<Variable> const   ),&global_label_name_b721f72143d35230>>();
    };
    aotLib[0x8adbc84bf1b50840] = +[](Context & ctx) -> SimNode* { // enum_label_name_a32575da767b3ecd
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Enumeration * const   ),&enum_label_name_a32575da767b3ecd>>();
    };
    aotLib[0x337720e26d36d7f5] = +[](Context & ctx) -> SimNode* { // enum_label_name_768485a24af7de17
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<Enumeration> const   ),&enum_label_name_768485a24af7de17>>();
    };
    aotLib[0x9ec1f091cdac453c] = +[](Context & ctx) -> SimNode* { // struct_label_name_7b7faf8095586da6
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Structure * const   ),&struct_label_name_7b7faf8095586da6>>();
    };
    aotLib[0x92785b64e1a53e97] = +[](Context & ctx) -> SimNode* { // struct_label_name_6ee1d24e90557aeb
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<Structure> const   ),&struct_label_name_6ee1d24e90557aeb>>();
    };
    aotLib[0xf0e8710aab95d4e1] = +[](Context & ctx) -> SimNode* { // describe_type_short_56370d8993e381ba
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, TypeDecl * const   ),&describe_type_short_56370d8993e381ba>>();
    };
    aotLib[0x4d3013b6e4e564bd] = +[](Context & ctx) -> SimNode* { // describe_type_short_847eafd8ddf6216
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const   ),&describe_type_short_847eafd8ddf6216>>();
    };
    aotLib[0x6e43b5e5f4f54c08] = +[](Context & ctx) -> SimNode* { // describe_type_cf1a091488868462
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, TypeDecl * const   ),&describe_type_cf1a091488868462>>();
    };
    aotLib[0xd4983f60ed4583f0] = +[](Context & ctx) -> SimNode* { // describe_type_a9bac10014c763b2
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, smart_ptr_raw<TypeDecl> const   ),&describe_type_a9bac10014c763b2>>();
    };
    aotLib[0x8b7847da4d370b82] = +[](Context & ctx) -> SimNode* { // make_domain_3e6edb06cf9af7bc
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, char * const   ),&make_domain_3e6edb06cf9af7bc>>();
    };
    aotLib[0xf09d70dabfadabf7] = +[](Context & ctx) -> SimNode* { // make_label_3368ba1f933c2731
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, char * const   ),&make_label_3368ba1f933c2731>>();
    };
    aotLib[0xf05b4a0a1b199255] = +[](Context & ctx) -> SimNode* { // make_ref_c995159f43ed1b43
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, char * const  , char * const   ),&make_ref_c995159f43ed1b43>>();
    };
    aotLib[0xa4007e96ab229d2e] = +[](Context & ctx) -> SimNode* { // make_group_8b304b06bc5b7645
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, char * const  , char * const   ),&make_group_8b304b06bc5b7645>>();
    };
    aotLib[0x4d2bdb499c09940e] = +[](Context & ctx) -> SimNode* { // make_header_efad6667e51b3e08
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, char * const  , char * const   ),&make_header_efad6667e51b3e08>>();
    };
    aotLib[0x71a6dd85e12a4e1b] = +[](Context & ctx) -> SimNode* { // write_table_separator_2180d51ced094114
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, StringBuilderWriter & , TArray<int32_t> const  & , char * const   ),&write_table_separator_2180d51ced094114>>();
    };
    aotLib[0x139b75dbca9873b9] = +[](Context & ctx) -> SimNode* { // write_table_line_18bbb8245228f037
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, StringBuilderWriter & , TArray<char *> const  & , TArray<int32_t> const  &  ),&write_table_line_18bbb8245228f037>>();
    };
    aotLib[0xbd5d6d3969b4183a] = +[](Context & ctx) -> SimNode* { // make_table_46d5ce8cf5a30e69
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, TArray<TArray<char *>> const  & , bool  ),&make_table_46d5ce8cf5a30e69>>();
    };
    aotLib[0x3701c1118ea995a2] = +[](Context & ctx) -> SimNode* { // topic_9659659d94ddb29a
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, char * const  , Module * const  , char * const   ),&topic_9659659d94ddb29a>>();
    };
    aotLib[0x222ad1b8211a6a8] = +[](Context & ctx) -> SimNode* { // topic_b97c7facf4ccb6a9
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, Module * const   ),&topic_b97c7facf4ccb6a9>>();
    };
    aotLib[0xf589aea3422e4045] = +[](Context & ctx) -> SimNode* { // document_topic_c6e2614dff9f2d3e
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , char * const   ),&document_topic_c6e2614dff9f2d3e>>();
    };
    aotLib[0x50085a99d8fdfcdb] = +[](Context & ctx) -> SimNode* { // document_bitfield_4340095c1075f95e
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , smart_ptr_raw<TypeDecl> const   ),&document_bitfield_4340095c1075f95e>>();
    };
    aotLib[0xbe303b3ce156301] = +[](Context & ctx) -> SimNode* { // document_variant_c74548ebfca402e7
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , smart_ptr_raw<TypeDecl> const   ),&document_variant_c74548ebfca402e7>>();
    };
    aotLib[0xbe303b3ce156301] = +[](Context & ctx) -> SimNode* { // document_tuple_c74548ebfca402e7
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , smart_ptr_raw<TypeDecl> const   ),&document_tuple_c74548ebfca402e7>>();
    };
    aotLib[0xf1274c919f617831] = +[](Context & ctx) -> SimNode* { // document_typedefs_d1f98d27346b80af
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_typedefs_d1f98d27346b80af>>();
    };
    aotLib[0xb7d01a42ca3c9835] = +[](Context & ctx) -> SimNode* { // document_global_constant_378f3e1a10c345f3
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , smart_ptr_raw<Variable> const   ),&document_global_constant_378f3e1a10c345f3>>();
    };
    aotLib[0x1044e0d4b1e31a84] = +[](Context & ctx) -> SimNode* { // is_global_constant_320cc2da3d8764f7
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, smart_ptr_raw<Variable> const   ),&is_global_constant_320cc2da3d8764f7>>();
    };
    aotLib[0x4c660ac033acfc94] = +[](Context & ctx) -> SimNode* { // document_global_constants_24362fe0342586e0
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_global_constants_24362fe0342586e0>>();
    };
    aotLib[0x3661a0956be8cf1a] = +[](Context & ctx) -> SimNode* { // document_enumerations_b052b53cf3b12eb
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_enumerations_b052b53cf3b12eb>>();
    };
    aotLib[0x50b89e7ee73e968f] = +[](Context & ctx) -> SimNode* { // document_function_declaration_4c978976b1aaa270
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , char * const  , char * const  , TArray<char *> const  & , TArray<TypeDecl *> const  & , smart_ptr_raw<TypeDecl> const   ),&document_function_declaration_4c978976b1aaa270>>();
    };
    aotLib[0x2dbc5e90fa262740] = +[](Context & ctx) -> SimNode* { // document_function_arguments_e7469e75af4ed246
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , TArray<char *> const  & , TArray<TypeDecl *> const  &  ),&document_function_arguments_e7469e75af4ed246>>();
    };
    aotLib[0x19d0595a88fd52c8] = +[](Context & ctx) -> SimNode* { // document_classes_55fe4b580059698a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_classes_55fe4b580059698a>>();
    };
    aotLib[0xf3c9f2972bb5d9a] = +[](Context & ctx) -> SimNode* { // document_structure_96bd65f4eda31cf3
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , smart_ptr_raw<Structure> const   ),&document_structure_96bd65f4eda31cf3>>();
    };
    aotLib[0x8f31513dfda2e2f4] = +[](Context & ctx) -> SimNode* { // document_structures_5fb20d0dee04ba46
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_structures_5fb20d0dee04ba46>>();
    };
    aotLib[0xbee53c52788bd560] = +[](Context & ctx) -> SimNode* { // document_structure_annotations_a53005d9c9a5617
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_structure_annotations_a53005d9c9a5617>>();
    };
    aotLib[0x86e8180286ceee80] = +[](Context & ctx) -> SimNode* { // document_function_annotations_5344d92e9ea569ad
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_function_annotations_5344d92e9ea569ad>>();
    };
    aotLib[0x78128c402cb8ae85] = +[](Context & ctx) -> SimNode* { // document_annotations_128851a794d46d1a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_annotations_128851a794d46d1a>>();
    };
    aotLib[0x3713eeac0dfd25e4] = +[](Context & ctx) -> SimNode* { // document_structure_macros_7386ca2820215b3d
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_structure_macros_7386ca2820215b3d>>();
    };
    aotLib[0x65115697bb66c2b6] = +[](Context & ctx) -> SimNode* { // document_call_macro_e76fbe68d00e7362
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , char * const   ),&document_call_macro_e76fbe68d00e7362>>();
    };
    aotLib[0xe145b4236e0a830b] = +[](Context & ctx) -> SimNode* { // document_call_macros_56da413fe6ccb104
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_call_macros_56da413fe6ccb104>>();
    };
    aotLib[0x9aa9dd6dd19ca370] = +[](Context & ctx) -> SimNode* { // document_reader_macro_4006c26786c1908
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , char * const   ),&document_reader_macro_4006c26786c1908>>();
    };
    aotLib[0x8f59e0fcfcceb77] = +[](Context & ctx) -> SimNode* { // document_reader_macros_585b57fffa625cd
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_reader_macros_585b57fffa625cd>>();
    };
    aotLib[0x71d153c31909d9ec] = +[](Context & ctx) -> SimNode* { // document_variant_macro_84da1b9ee053cb60
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , char * const   ),&document_variant_macro_84da1b9ee053cb60>>();
    };
    aotLib[0x8b34cba7048615c0] = +[](Context & ctx) -> SimNode* { // document_variant_macros_eab4d095021756ef
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_variant_macros_eab4d095021756ef>>();
    };
    aotLib[0xe0eead8c010234a5] = +[](Context & ctx) -> SimNode* { // document_typeinfo_macro_66a7bf37ca0e4fcd
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , char * const   ),&document_typeinfo_macro_66a7bf37ca0e4fcd>>();
    };
    aotLib[0x44a03a7023e1ec22] = +[](Context & ctx) -> SimNode* { // document_typeinfo_macros_b4b5371c027f780c
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const   ),&document_typeinfo_macros_b4b5371c027f780c>>();
    };
    aotLib[0xcfe2328fbcdd4da3] = +[](Context & ctx) -> SimNode* { // function_needs_documenting_5ed98b6306abb3d7
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, smart_ptr_raw<Function> const   ),&function_needs_documenting_5ed98b6306abb3d7>>();
    };
    aotLib[0xaa891474801b90df] = +[](Context & ctx) -> SimNode* { // document_warning_cb981aa521e01a92
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , char * const   ),&document_warning_cb981aa521e01a92>>();
    };
    aotLib[0x7eccf71cb654622d] = +[](Context & ctx) -> SimNode* { // function_name_ee62641d633d27c8
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, char * const   ),&function_name_ee62641d633d27c8>>();
    };
    aotLib[0xac8a41cd6e1573b4] = +[](Context & ctx) -> SimNode* { // document_functions_a509d99cbdb2836d
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, FILE const  * const  , Module * const  , TArray<rst::DocGroup> &  ),&document_functions_a509d99cbdb2836d>>();
    };
    aotLib[0x3e619c34da3f11cf] = +[](Context & ctx) -> SimNode* { // debug_group_d90dc61b38b89347
        return ctx.code->makeNode<SimNode_AotCMRES<rst::DocGroup (*) ( Context * __context__, rst::DocGroup &  ),&debug_group_d90dc61b38b89347>>();
    };
    aotLib[0x2fe1ea86d2c836d1] = +[](Context & ctx) -> SimNode* { // append_to_group_by_regex_819dad4f532ef50d
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, rst::DocGroup & , Module * const  , regex::Regex &  ),&append_to_group_by_regex_819dad4f532ef50d>>();
    };
    aotLib[0xd1fa1679ed715eb1] = +[](Context & ctx) -> SimNode* { // group_by_regex_def313b7b3c9e262
        return ctx.code->makeNode<SimNode_AotCMRES<rst::DocGroup (*) ( Context * __context__, char * const  , Module * const  , regex::Regex &  ),&group_by_regex_def313b7b3c9e262>>();
    };
    aotLib[0x148e31e9b6c3bf86] = +[](Context & ctx) -> SimNode* { // hide_group_4868115ae5eeb38f
        return ctx.code->makeNode<SimNode_AotCMRES<rst::DocGroup (*) ( Context * __context__, rst::DocGroup &  ),&hide_group_4868115ae5eeb38f>>();
    };
    aotLib[0x92f6119b810e330f] = +[](Context & ctx) -> SimNode* { // generate_topic_stub_2777760f2b77d9f0
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TTable<char *,bool> & , FILE const  * const  , char * const   ),&generate_topic_stub_2777760f2b77d9f0>>();
    };
    aotLib[0xae3f3d29d97152f7] = +[](Context & ctx) -> SimNode* { // generate_substitute_stub_34b873e38788090
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, Module * const  , char * const   ),&generate_substitute_stub_34b873e38788090>>();
    };
    aotLib[0xfe4a4edf9d1c0a9e] = +[](Context & ctx) -> SimNode* { // getDetailIncludePath_fb77bda786a398b2
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, char * const  , char * const   ),&getDetailIncludePath_fb77bda786a398b2>>();
    };
    aotLib[0x7d5b831863ce00d3] = +[](Context & ctx) -> SimNode* { // document_96eb220f96118989
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, char * const  , Module * const  , char * const  , char * const  , TArray<rst::DocGroup> &  ),&document_96eb220f96118989>>();
    };
    aotLib[0x494570d7b60e0bcb] = +[](Context & ctx) -> SimNode* { // DocGroup_6e55999f1d438d24
        return ctx.code->makeNode<SimNode_AotCMRES<rst::DocGroup (*) ( Context * __context__ ),&DocGroup_6e55999f1d438d24>>();
    };
    // [[ init script ]]
    aotLib[0x185482f89db39a9] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<SimNode_Aot<void (*)(Context *, bool),&__init_script>>();
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
