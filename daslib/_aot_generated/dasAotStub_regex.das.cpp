#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require regex
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
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
namespace _anon_6191221158849729409 {

extern StructInfo __struct_info__739e0aa00b63f5bf;
extern StructInfo __struct_info__a442c67b1b45039e;
extern TypeInfo __type_info__6efe4a274318358b;
extern TypeInfo __type_info__d658aa3b467a4710;
extern TypeInfo __type_info__8cbf7e31d94517eb;
extern TypeInfo __type_info__1bb41dcf345965de;
extern TypeInfo __type_info__634d658d3d3c73e9;
extern TypeInfo __type_info__1e3f0d9b1ed71149;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af96fe4c8658cf52;
extern TypeInfo __type_info__b661860848e8711e;
extern TypeInfo __type_info__9c60f9702c7c9014;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__e68681b2b99a9c9a;
extern TypeInfo __type_info__21ea97d8ca8ffd5;
extern TypeInfo __type_info__646c01d58047ed68;
extern TypeInfo __type_info__af63ef4c86020cd5;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63e84c860200f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__c248c77e17cca11a;
extern VarInfo __var_info__7df7ea7e12e9cd7;
extern VarInfo __var_info__55374dcc02561621;
extern VarInfo __var_info__125ea9a7ea60fe3a;
extern VarInfo __var_info__26a69ea7fb7ba40e;
extern VarInfo __var_info__f02e80a7cd1d35eb;
extern VarInfo __var_info__552745cc023ad889;
extern VarInfo __var_info__4f1b9e3acf383cbd;
extern VarInfo __var_info__f05876a7cd78b90c;
extern VarInfo __var_info__f06990a7cd9b8e8c;
extern VarInfo __var_info__553347cc024f3fef;
extern VarInfo __var_info__9be1db3446ee3f1b;
extern VarInfo __var_info__5f2ead14c0ff50c1;
extern VarInfo __var_info__e2757da7c183c939;
extern VarInfo __var_info__f05d8ca7cd9876b6;
extern VarInfo __var_info__5fe75b5def0592e0;
extern VarInfo __var_info__7ce5b1477861b3d1;
extern VarInfo __var_info__aa30cda461a0c553;
extern VarInfo __var_info__6eee618b86e51341;
extern VarInfo __var_info__90ed0cbd06831825;
extern VarInfo __var_info__37de19b6affa3dd6;
extern FuncInfo __func_info__8b6d6c464a360a4f;
extern EnumInfo __enum_info__f5eadb7b60b9c74d;

EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_0 = { "Char", 0 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_1 = { "Set", 1 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_2 = { "Any", 2 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_3 = { "Eos", 3 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_4 = { "Group", 4 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_5 = { "Plus", 5 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_6 = { "Star", 6 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_7 = { "Question", 7 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_8 = { "Concat", 8 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_9 = { "Union", 9 };
EnumValueInfo * __enum_info__f5eadb7b60b9c74d_values [] = { &__enum_info__f5eadb7b60b9c74d_value_0, &__enum_info__f5eadb7b60b9c74d_value_1, &__enum_info__f5eadb7b60b9c74d_value_2, &__enum_info__f5eadb7b60b9c74d_value_3, &__enum_info__f5eadb7b60b9c74d_value_4, &__enum_info__f5eadb7b60b9c74d_value_5, &__enum_info__f5eadb7b60b9c74d_value_6, &__enum_info__f5eadb7b60b9c74d_value_7, &__enum_info__f5eadb7b60b9c74d_value_8, &__enum_info__f5eadb7b60b9c74d_value_9 };
EnumInfo __enum_info__f5eadb7b60b9c74d = { "ReOp", "regex", __enum_info__f5eadb7b60b9c74d_values, 10, UINT64_C(0xf5eadb7b60b9c74d) };
VarInfo __struct_info__739e0aa00b63f5bf_field_0 =  { Type::tEnumeration, nullptr, &__enum_info__f5eadb7b60b9c74d, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x553347cc024f3fef), "op", 0, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_1 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x552745cc023ad889), "id", 4, 0 };
TypeInfo * __type_info__26a69ea7fb7ba40e_arg_types_var_8331108043184141759[3] = { &__type_info__21ea97d8ca8ffd5, &__type_info__8cbf7e31d94517eb, &__type_info__1e3f0d9b1ed71149 };
const char * __type_info__26a69ea7fb7ba40e_arg_names_var_8331108043184141759[3] = { "regex", "node", "str" };
VarInfo __struct_info__739e0aa00b63f5bf_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__d658aa3b467a4710, nullptr, (TypeInfo **)__type_info__26a69ea7fb7ba40e_arg_types_var_8331108043184141759, __type_info__26a69ea7fb7ba40e_arg_names_var_8331108043184141759, 3, 0, nullptr, 12, 8, UINT64_C(0x26a69ea7fb7ba40e), "fun2", 8, 0 };
TypeInfo * __type_info__f02e80a7cd1d35eb_arg_types_var_8331108043184141759[3] = { &__type_info__8cbf7e31d94517eb, &__type_info__646c01d58047ed68, &__type_info__37d36026a6078a42 };
const char * __type_info__f02e80a7cd1d35eb_arg_names_var_8331108043184141759[3] = { "node", "rnd", "str" };
VarInfo __struct_info__739e0aa00b63f5bf_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f02e80a7cd1d35eb_arg_types_var_8331108043184141759, __type_info__f02e80a7cd1d35eb_arg_names_var_8331108043184141759, 3, 0, nullptr, 12, 8, UINT64_C(0xf02e80a7cd1d35eb), "gen2", 16, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_4 =  { Type::tRange, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0x55374dcc02561621), "at", 24, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_5 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xf05d8ca7cd9876b6), "text", 32, 7 };
VarInfo __struct_info__739e0aa00b63f5bf_field_6 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x5fe75b5def0592e0), "textLen", 40, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_7 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__8cbf7e31d94517eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x7df7ea7e12e9cd7), "all", 48, 8 };
VarInfo __struct_info__739e0aa00b63f5bf_field_8 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xf05876a7cd78b90c), "left", 72, 9 };
VarInfo __struct_info__739e0aa00b63f5bf_field_9 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x9be1db3446ee3f1b), "right", 80, 10 };
VarInfo __struct_info__739e0aa00b63f5bf_field_10 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x5f2ead14c0ff50c1), "subexpr", 88, 11 };
VarInfo __struct_info__739e0aa00b63f5bf_field_11 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xf06990a7cd9b8e8c), "next", 96, 14 };
uint32_t __type_info__125ea9a7ea60fe3a_dim_var_8331108043184141759[1] = { 8 };
VarInfo __struct_info__739e0aa00b63f5bf_field_12 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 1, __type_info__125ea9a7ea60fe3a_dim_var_8331108043184141759, 30, 32, UINT64_C(0x125ea9a7ea60fe3a), "cset", 104, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_13 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x4f1b9e3acf383cbd), "index", 136, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_14 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xe2757da7c183c939), "tail", 144, 15 };
VarInfo * __struct_info__739e0aa00b63f5bf_fields[15] =  { &__struct_info__739e0aa00b63f5bf_field_0, &__struct_info__739e0aa00b63f5bf_field_1, &__struct_info__739e0aa00b63f5bf_field_2, &__struct_info__739e0aa00b63f5bf_field_3, &__struct_info__739e0aa00b63f5bf_field_4, &__struct_info__739e0aa00b63f5bf_field_5, &__struct_info__739e0aa00b63f5bf_field_6, &__struct_info__739e0aa00b63f5bf_field_7, &__struct_info__739e0aa00b63f5bf_field_8, &__struct_info__739e0aa00b63f5bf_field_9, &__struct_info__739e0aa00b63f5bf_field_10, &__struct_info__739e0aa00b63f5bf_field_11, &__struct_info__739e0aa00b63f5bf_field_12, &__struct_info__739e0aa00b63f5bf_field_13, &__struct_info__739e0aa00b63f5bf_field_14 };
StructInfo __struct_info__739e0aa00b63f5bf = {"ReNode", "regex", 28, __struct_info__739e0aa00b63f5bf_fields, 15, 152, UINT64_C(0x0), nullptr, UINT64_C(0x739e0aa00b63f5bf), 5 };
VarInfo __struct_info__a442c67b1b45039e_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x37de19b6affa3dd6), "root", 0, 1 };
VarInfo __struct_info__a442c67b1b45039e_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x90ed0cbd06831825), "match", 8, 2 };
VarInfo __struct_info__a442c67b1b45039e_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__6efe4a274318358b, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x6eee618b86e51341), "groups", 16, 5 };
uint32_t __type_info__aa30cda461a0c553_dim_var_11836241002723869598[1] = { 8 };
VarInfo __struct_info__a442c67b1b45039e_field_3 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 1, __type_info__aa30cda461a0c553_dim_var_11836241002723869598, 30, 32, UINT64_C(0xaa30cda461a0c553), "earlyOut", 40, 0 };
VarInfo __struct_info__a442c67b1b45039e_field_4 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x7ce5b1477861b3d1), "canEarlyOut", 72, 0 };
VarInfo * __struct_info__a442c67b1b45039e_fields[5] =  { &__struct_info__a442c67b1b45039e_field_0, &__struct_info__a442c67b1b45039e_field_1, &__struct_info__a442c67b1b45039e_field_2, &__struct_info__a442c67b1b45039e_field_3, &__struct_info__a442c67b1b45039e_field_4 };
StructInfo __struct_info__a442c67b1b45039e = {"Regex", "regex", 28, __struct_info__a442c67b1b45039e_fields, 5, 80, UINT64_C(0x0), nullptr, UINT64_C(0xa442c67b1b45039e), 0 };
VarInfo __func_info__8b6d6c464a360a4f_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xc248c77e17cca11a), "node", 0, 0 };
VarInfo * __func_info__8b6d6c464a360a4f_fields[1] =  { &__func_info__8b6d6c464a360a4f_field_0 };
FuncInfo __func_info__8b6d6c464a360a4f = {"invoke block<(var node:regex::ReNode?):void> const", "", __func_info__8b6d6c464a360a4f_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8b6d6c464a360a4f), 0x0 };
TypeInfo * __type_info__6efe4a274318358b_arg_types[2] = { &__type_info__af63ef4c86020cd5, &__type_info__af63ee4c86020b22 };
TypeInfo __type_info__6efe4a274318358b = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__6efe4a274318358b_arg_types, nullptr, 2, 0, nullptr, 16390, 16, UINT64_C(0x6efe4a274318358b) };
TypeInfo __type_info__d658aa3b467a4710 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xd658aa3b467a4710) };
TypeInfo __type_info__8cbf7e31d94517eb = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x8cbf7e31d94517eb) };
TypeInfo __type_info__1bb41dcf345965de = { Type::tStructure, &__struct_info__739e0aa00b63f5bf, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 152, UINT64_C(0x1bb41dcf345965de) };
TypeInfo __type_info__634d658d3d3c73e9 = { Type::tStructure, &__struct_info__a442c67b1b45039e, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 80, UINT64_C(0x634d658d3d3c73e9) };
TypeInfo __type_info__1e3f0d9b1ed71149 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x1e3f0d9b1ed71149) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af96fe4c8658cf52 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf96fe4c8658cf52) };
TypeInfo __type_info__b661860848e8711e = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xb661860848e8711e) };
TypeInfo __type_info__9c60f9702c7c9014 = { Type::tEnumeration, nullptr, &__enum_info__f5eadb7b60b9c74d, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x9c60f9702c7c9014) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__e68681b2b99a9c9a = { Type::tStructure, &__struct_info__739e0aa00b63f5bf, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 152, UINT64_C(0xe68681b2b99a9c9a) };
TypeInfo __type_info__21ea97d8ca8ffd5 = { Type::tStructure, &__struct_info__a442c67b1b45039e, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 80, UINT64_C(0x21ea97d8ca8ffd5) };
TypeInfo __type_info__646c01d58047ed68 = { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e84c860200f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0x646c01d58047ed68) };
TypeInfo __type_info__af63ef4c86020cd5 = { Type::tRange, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xaf63ef4c86020cd5) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63e84c860200f0 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e84c860200f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__1bb41dcf345965de };
TypeInfo * __tinfo_1[1] = { &__type_info__634d658d3d3c73e9 };
TypeInfo * __tinfo_2[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_3[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_4[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_5[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_6[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_7[1] = { &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_8[2] = { &__type_info__af90fe4c864e9d52, &__type_info__9c60f9702c7c9014 };
TypeInfo * __tinfo_9[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_11[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_12[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_13[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_14[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_15[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_16[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_17[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_18[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_19[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_20[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_22[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_23[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
namespace regex { struct ReNode; };
namespace regex { struct Regex; };
// unused enumeration ConversionResult
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
template <> struct cast< das::_anon_6191221158849729409::regex::ReOp > : cast_enum < das::_anon_6191221158849729409::regex::ReOp > {};
namespace _anon_6191221158849729409 {
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

inline void _FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd ( Context * __context__, TArray<regex::ReNode *> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759 ( Context * __context__, TArray<regex::ReNode *> & __a_rename_at_1183_2 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d ( Context * __context__, TDim<regex::ReNode *,2> const  & __a_rename_at_581_4 );
inline regex::ReNode & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_958929e0b5e39736 ( Context * __context__, regex::ReNode & __a_rename_at_50_5 );
inline void finalize_1b3fa79e78928d5e ( Context * __context__, regex::ReNode & ____this_rename_at_29_6 );
inline regex::Regex & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_182d4b6f1ba86427 ( Context * __context__, regex::Regex & __a_rename_at_50_7 );
inline int32_t _FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec ( Context * __context__, int32_t __ch1_rename_at_240_8, int32_t __ch2_rename_at_240_9 );
inline void finalize_4d9dff2c4111fea5 ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> & ____this_rename_at_422_10 );
inline void finalize_2e3cca8177873ef7 ( Context * __context__, regex::ReNode * & ____this_rename_at_519_11 );
inline void _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9 ( Context * __context__, TArray<regex::ReNode *> & __Arr_rename_at_181_12, regex::ReNode * __value_rename_at_181_13 );
inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6 ( Context * __context__, TDim<regex::ReNode *,2> & __a_rename_at_1343_14 );
inline void _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5 ( Context * __context__, TArray<TTuple<16,range,char *>> & __Arr_rename_at_181_16, TTuple<16,range,char *> & __value_rename_at_181_17 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) & __it_rename_at_1224_18, uint32_t & __value_rename_at_1224_19 );
inline uint32_t re_gen_get_rep_limit_aa71779c47fee5b8 ( Context * __context__ );
inline void trace_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_79_20, int32_t __offset_rename_at_79_21 );
inline void note_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_86_22, int32_t __offset_rename_at_86_23 );
inline void trace_backtrack_60bdee73e3315732 ( Context * __context__ );
inline void trace_value_599ec0b97b535e0e ( Context * __context__, regex::ReNode * const  __value_rename_at_99_24 );
inline TVariant<16,8,regex::ReNode *,void *> nada_a36e274f9f880579 ( Context * __context__ );
inline TVariant<16,8,regex::ReNode *,void *> maybe_cb698f19184a9bc8 ( Context * __context__, regex::ReNode * __value_rename_at_114_25 );
inline bool eos_bcd93149648ad887 ( Context * __context__, char * const  __expr_rename_at_119_26, int32_t __offset_rename_at_119_27 );
inline int32_t at_ac9760d67b4716d0 ( Context * __context__, char * const  __expr_rename_at_123_28, int32_t __offset_rename_at_123_29 );
inline int32_t next_c7b14aa26d681478 ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> const  & __re_rename_at_142_30 );
inline bool is_set_empty_1f052aa7f260c4eb ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_150_31 );
inline void set_or_char_5d5313390b879abc ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_159_33, int32_t __ch_rename_at_159_34 );
inline void set_or_range_40be02560bdb916d ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_163_35, range __bits_rename_at_163_36 );
inline void set_invert_4fb9b64ebf15e148 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_169_38 );
inline void set_or_set_f7fe9d5d2a5b194b ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_175_40, TDim<uint32_t,8> const  & __eset_rename_at_175_41 );
inline void set_negative_e65d971bcd9eb5c1 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_181_46 );
inline void set_meta_4ca7bb80700cac47 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_187_48, int32_t __che_rename_at_187_49 );
inline bool is_meta_character_5210d144f518e88e ( Context * __context__, int32_t __ch_rename_at_218_53 );
inline bool is_set_character_b3eb8d77360f374d ( Context * __context__, int32_t __ch_rename_at_222_54 );
inline int32_t from_hex_34b87ad3a69093f9 ( Context * __context__, int32_t __ch_rename_at_226_55 );
inline char * string_from_hex_3b7c632d00a40daf ( Context * __context__, int32_t __ch1_rename_at_244_56 );
inline char * string_from_hex_7d459c50a54015ae ( Context * __context__, int32_t __ch1_rename_at_248_57, int32_t __ch2_rename_at_248_58 );
inline TVariant<16,8,regex::ReNode *,void *> re_char_f30c974269a757d6 ( Context * __context__, char * const  __expr_rename_at_253_59, int32_t __offset_rename_at_253_60 );
inline TVariant<16,8,regex::ReNode *,void *> re_set_items_24bb39b249ea7fd4 ( Context * __context__, char * const  __expr_rename_at_302_67, int32_t __offset_rename_at_302_68 );
inline TVariant<16,8,regex::ReNode *,void *> re_set_a55f6e80806300ed ( Context * __context__, char * const  __expr_rename_at_404_80, int32_t __offset_rename_at_404_81 );
inline TVariant<16,8,regex::ReNode *,void *> re_any_994b96521819a3e ( Context * __context__, char * const  __expr_rename_at_438_85, int32_t __offset_rename_at_438_86 );
inline TVariant<16,8,regex::ReNode *,void *> re_eos_12f3534068f58fcd ( Context * __context__, char * const  __expr_rename_at_450_87, int32_t __offset_rename_at_450_88 );
inline TVariant<16,8,regex::ReNode *,void *> re_group_78c2eacdeb9fefc1 ( Context * __context__, char * const  __expr_rename_at_462_89, int32_t __offset_rename_at_462_90 );
inline TVariant<16,8,regex::ReNode *,void *> re_elementary_8e517f8be58bf1a6 ( Context * __context__, char * const  __expr_rename_at_482_92, int32_t __offset_rename_at_482_93 );
inline regex::ReNode * mk_concat_5c3b6674ba9a8a3f ( Context * __context__, regex::ReNode * __left_rename_at_510_99, regex::ReNode * __right_rename_at_510_100 );
inline regex::ReNode * mk_union_cc5d9e9364416c61 ( Context * __context__, regex::ReNode * __left_rename_at_538_102, regex::ReNode * __right_rename_at_538_103 );
inline TVariant<16,8,regex::ReNode *,void *> re_re_57e53314b2b816e9 ( Context * __context__, char * const  __expr_rename_at_565_105, int32_t __offset_rename_at_565_106 );
inline TVariant<16,8,regex::ReNode *,void *> re_basic_468d93210132438e ( Context * __context__, char * const  __expr_rename_at_600_111, int32_t __offset_rename_at_600_112 );
inline regex::ReNode * re_parse_a11fbde82111aa69 ( Context * __context__, char * const  __expr_rename_at_620_115 );
inline void visit_top_down_4fecc3dfe95f303c ( Context * __context__, regex::ReNode * __node_rename_at_633_118, Block DAS_COMMENT((void,regex::ReNode *)) const  & __blk_rename_at_633_119 );
inline void re_assign_next_18621bfa0d3dbb85 ( Context * __context__, regex::Regex & __re_rename_at_649_121 );
inline void re_assign_groups_269827e3d8834c3a ( Context * __context__, regex::Regex & __re_rename_at_666_125 );
inline void re_assign_match_functions_d1123942ad08a870 ( Context * __context__, regex::Regex & __re_rename_at_681_128 );
inline uint8_t const  * re_match2_single_char_efd3120a5ed2dc78 ( Context * __context__, regex::Regex & __regex_rename_at_730_130, regex::ReNode * __node_rename_at_730_131, uint8_t const  * const  __str_rename_at_730_132 );
inline uint8_t const  * re_match2_char_563cc8699254595c ( Context * __context__, regex::Regex & __regex_rename_at_753_135, regex::ReNode * __node_rename_at_753_136, uint8_t const  * const  __str_rename_at_753_137 );
inline uint8_t const  * re_match2_union_483bf85dbc5196dd ( Context * __context__, regex::Regex & __regex_rename_at_779_140, regex::ReNode * __node_rename_at_779_141, uint8_t const  * const  __str_rename_at_779_142 );
inline uint8_t const  * re_match2_set_31b09b463a9d7b2b ( Context * __context__, regex::Regex & __regex_rename_at_797_145, regex::ReNode * __node_rename_at_797_146, uint8_t const  * const  __str_rename_at_797_147 );
inline uint8_t const  * re_match2_any_9b00b66687d95b62 ( Context * __context__, regex::Regex & __regex_rename_at_820_150, regex::ReNode * __node_rename_at_820_151, uint8_t const  * const  __str_rename_at_820_152 );
inline uint8_t const  * re_match2_concat_2bd31fc7755cdf2a ( Context * __context__, regex::Regex & __regex_rename_at_840_155, regex::ReNode * __node_rename_at_840_156, uint8_t const  * const  __str_rename_at_840_157 );
inline uint8_t const  * re_match2_eos_a35408df9434ba3c ( Context * __context__, regex::Regex & __regex_rename_at_857_160, regex::ReNode * __node_rename_at_857_161, uint8_t const  * const  __str_rename_at_857_162 );
inline uint8_t const  * re_match2_question_4acdfdc420ce399a ( Context * __context__, regex::Regex & __regex_rename_at_874_164, regex::ReNode * __node_rename_at_874_165, uint8_t const  * const  __str_rename_at_874_166 );
inline uint8_t const  * re_match2_plus_adb26cc707eb0c55 ( Context * __context__, regex::Regex & __regex_rename_at_893_170, regex::ReNode * __node_rename_at_893_171, uint8_t const  * const  __str_rename_at_893_172 );
inline uint8_t const  * re_match2_plus_set_aeb296540242ef22 ( Context * __context__, regex::Regex & __regex_rename_at_928_178, regex::ReNode * __node_rename_at_928_179, uint8_t const  * const  __str_rename_at_928_180 );
inline uint8_t const  * re_match2_star_bb64c06dd606906e ( Context * __context__, regex::Regex & __regex_rename_at_971_185, regex::ReNode * __node_rename_at_971_186, uint8_t const  * const  __str_rename_at_971_187 );
inline uint8_t const  * re_match2_group_82784eced054ba4e ( Context * __context__, regex::Regex & __regex_rename_at_1000_193, regex::ReNode * __node_rename_at_1000_194, uint8_t const  * const  __str_rename_at_1000_195 );
inline bool re_early_out_80d62a0fdd14cb06 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_1020_198, regex::ReNode * const  __node_rename_at_1020_199 );
inline bool is_valid_87c94e9920e8a112 ( Context * __context__, regex::Regex & __re_rename_at_1063_204 );
inline bool regex_compile_b802a48fba17c551 ( Context * __context__, regex::Regex & __re_rename_at_1068_205, char * const  __expr_rename_at_1068_206 );
inline regex::Regex regex_compile_d14729dab5987744 ( Context * __context__, char * const  __expr_rename_at_1082_207 );
inline regex::Regex regex_compile_8b8cd96a70efcb1c ( Context * __context__, regex::Regex & __re_rename_at_1090_209 );
inline int32_t regex_match_d1084a47d6e2d53e ( Context * __context__, regex::Regex & __regex_rename_at_1098_210, char * const  __str_rename_at_1098_211, int32_t __offset_rename_at_1098_212 );
inline char * regex_group_f150ed72d9c6b909 ( Context * __context__, regex::Regex const  & __regex_rename_at_1117_214, int32_t __index_rename_at_1117_215, char * const  __match_rename_at_1117_216 );
inline void regex_foreach_7115f79e7b67800f ( Context * __context__, regex::Regex & __regex_rename_at_1124_218, char * const  __str_rename_at_1124_219, Block DAS_COMMENT((bool,range)) const  & __blk_rename_at_1124_220 );
inline char * regex_replace_6c020ee2b76e6419 ( Context * __context__, regex::Regex & __regex_rename_at_1172_227, char * const  __str_rename_at_1172_228, Block DAS_COMMENT((char *,char * const )) const  & __blk_rename_at_1172_229 );
inline void regex_debug_35a427292763c452 ( Context * __context__, regex::Regex const  & __regex_rename_at_1226_239 );
inline void debug_set_13e232e40e242fa0 ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_1243_241 );
inline void debug_re_f8fd4cc00f0dc78d ( Context * __context__, regex::ReNode * const  __node_rename_at_1256_243, int32_t __tab_rename_at_1256_244 );
inline int32_t rnd_set_character_6517e452a18c5606 ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_1317_246, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1317_247 );
inline void re_gen2_fail_a5fdf20930267896 ( Context * __context__, regex::ReNode * __node_rename_at_1325_250, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1325_251, StringBuilderWriter & __str_rename_at_1325_252 );
inline void re_gen2_char_e6b9f720990cc2d7 ( Context * __context__, regex::ReNode * __node_rename_at_1332_253, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1332_254, StringBuilderWriter & __str_rename_at_1332_255 );
inline void re_gen2_union_44153b0e184e2888 ( Context * __context__, regex::ReNode * __node_rename_at_1343_257, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1343_258, StringBuilderWriter & __str_rename_at_1343_259 );
inline void re_gen2_set_5483397ec36cfff0 ( Context * __context__, regex::ReNode * __node_rename_at_1354_262, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1354_263, StringBuilderWriter & __str_rename_at_1354_264 );
inline void re_gen2_any_200931e8e5db950 ( Context * __context__, regex::ReNode * __node_rename_at_1365_266, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1365_267, StringBuilderWriter & __str_rename_at_1365_268 );
inline void re_gen2_concat_bb7cd4a1d8751a6c ( Context * __context__, regex::ReNode * __node_rename_at_1378_271, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1378_272, StringBuilderWriter & __str_rename_at_1378_273 );
inline void re_gen2_eos_6a9fe263836b5d74 ( Context * __context__, regex::ReNode * __node_rename_at_1386_275, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1386_276, StringBuilderWriter & __str_rename_at_1386_277 );
inline void re_gen2_question_f2a2ead57fd4a9 ( Context * __context__, regex::ReNode * __node_rename_at_1396_279, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1396_280, StringBuilderWriter & __str_rename_at_1396_281 );
inline void re_gen2_plus_9fe66025caf188a9 ( Context * __context__, regex::ReNode * __node_rename_at_1414_285, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1414_286, StringBuilderWriter & __str_rename_at_1414_287 );
inline void re_gen2_star_5bbabd524a57385c ( Context * __context__, regex::ReNode * __node_rename_at_1433_292, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1433_293, StringBuilderWriter & __str_rename_at_1433_294 );
inline void re_gen2_group_cc75131bc454c003 ( Context * __context__, regex::ReNode * __node_rename_at_1452_299, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1452_300, StringBuilderWriter & __str_rename_at_1452_301 );
inline char * re_gen_e78126cc4b992ab9 ( Context * __context__, regex::Regex & __re_rename_at_1466_304, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1466_305 );
inline regex::ReNode ReNode_a530ee43f17c77ab ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<bool,0xef68509581809424>(__context__) = false;/*log_parse_enabled*/
    das_global<bool,0xa970cdba254dba34>(__context__) = false;/*log_match_enabled*/
    das_global<bool,0xa65624a8416952c2>(__context__) = false;/*range_check_enabled*/
    das_global<bool,0x79de6c82fae2b617>(__context__) = false;/*log_gen_enabled*/
    das_global<int32_t,0x3cc0153fc83ffc69>(__context__) = 0;/*trace_tab*/
    das_global<uint32_t,0x8ce995ce57e18608>(__context__) = 0x8u;/*re_gen_rep_limit*/
    das_global<char *,0xa1ce201dbf46e0ab>(__context__) = ((char *) "\\+-*.()[]|^");/*meta*/
    das_global<char *,0x39690c1b5c6d7b5a>(__context__) = ((char *) "wWsSdD");/*meta_set*/
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd ( Context * __context__, TArray<regex::ReNode *> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<regex::ReNode *>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759 ( Context * __context__, TArray<regex::ReNode *> &  __a_rename_at_1183_2 )
{
    {
        bool __need_loop_1185 = true;
        // aV: regex::ReNode? aka TT&
        das_iterator<TArray<regex::ReNode *>> __aV_iterator(__a_rename_at_1183_2);
        regex::ReNode * * __aV_rename_at_1185_3;
        __need_loop_1185 = __aV_iterator.first(__context__,(__aV_rename_at_1185_3)) && __need_loop_1185;
        for ( ; __need_loop_1185 ; __need_loop_1185 = __aV_iterator.next(__context__,(__aV_rename_at_1185_3)) )
        {
            finalize_2e3cca8177873ef7(__context__,(*__aV_rename_at_1185_3));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1185_3));
    };
    builtin_array_free(das_arg<TArray<regex::ReNode *>>::pass(__a_rename_at_1183_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d ( Context * __context__, TDim<regex::ReNode *,2> const  &  __a_rename_at_581_4 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline regex::ReNode & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_958929e0b5e39736 ( Context * __context__, regex::ReNode &  __a_rename_at_50_5 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<regex::ReNode &>::from(__a_rename_at_50_5)));
    return das_auto_cast_ref<regex::ReNode &>::cast(__a_rename_at_50_5);
}

inline void finalize_1b3fa79e78928d5e ( Context * __context__, regex::ReNode &  ____this_rename_at_29_6 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759(__context__,das_arg<TArray<regex::ReNode *>>::pass(____this_rename_at_29_6.all));
    finalize_2e3cca8177873ef7(__context__,____this_rename_at_29_6.left);
    finalize_2e3cca8177873ef7(__context__,____this_rename_at_29_6.right);
    finalize_2e3cca8177873ef7(__context__,____this_rename_at_29_6.subexpr);
    memset(&(____this_rename_at_29_6), 0, 152);
}

inline regex::Regex & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_182d4b6f1ba86427 ( Context * __context__, regex::Regex &  __a_rename_at_50_7 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<regex::Regex &>::from(__a_rename_at_50_7)));
    return das_auto_cast_ref<regex::Regex &>::cast(__a_rename_at_50_7);
}

inline int32_t _FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec ( Context * __context__, int32_t __ch1_rename_at_240_8, int32_t __ch2_rename_at_240_9 )
{
    return das_auto_cast<int32_t>::cast((from_hex_34b87ad3a69093f9(__context__,__ch1_rename_at_240_8) * 16) + from_hex_34b87ad3a69093f9(__context__,__ch2_rename_at_240_9));
}

inline void finalize_4d9dff2c4111fea5 ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> &  ____this_rename_at_422_10 )
{
    if ( das_get_variant_field<regex::ReNode *,8,0>::is(____this_rename_at_422_10) )
    {
        finalize_2e3cca8177873ef7(__context__,das_get_variant_field<regex::ReNode *,8,0>::get(____this_rename_at_422_10));
    };
    memset(&(____this_rename_at_422_10), 0, 16);
}

inline void finalize_2e3cca8177873ef7 ( Context * __context__, regex::ReNode * & ____this_rename_at_519_11 )
{
    if ( ____this_rename_at_519_11 != nullptr )
    {
        finalize_1b3fa79e78928d5e(__context__,das_arg<regex::ReNode>::pass(das_deref(__context__,____this_rename_at_519_11)));
        das_delete<regex::ReNode *>::clear(__context__,____this_rename_at_519_11);
        das_copy(____this_rename_at_519_11,nullptr);
    };
}

inline void _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9 ( Context * __context__, TArray<regex::ReNode *> &  __Arr_rename_at_181_12, regex::ReNode * __value_rename_at_181_13 )
{
    das_copy(__Arr_rename_at_181_12(builtin_array_push_back(das_arg<TArray<regex::ReNode *>>::pass(__Arr_rename_at_181_12),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_13);
}

inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6 ( Context * __context__, TDim<regex::ReNode *,2> &  __a_rename_at_1343_14 )
{
    TArray<regex::ReNode *> __arr_rename_at_1345_15; das_zero(__arr_rename_at_1345_15);
    _FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd(__context__,das_arg<TArray<regex::ReNode *>>::pass(__arr_rename_at_1345_15),2);
    das_copy(das_cast<TDim<regex::ReNode *,2>>::cast(das_ref(__context__,__arr_rename_at_1345_15(0,__context__))),__a_rename_at_1343_14);
    return /* <- */ das_auto_cast_move<TArray<regex::ReNode *>>::cast(__arr_rename_at_1345_15);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5 ( Context * __context__, TArray<TTuple<16,range,char *>> &  __Arr_rename_at_181_16, TTuple<16,range,char *> &  __value_rename_at_181_17 )
{
    das_copy(__Arr_rename_at_181_16(builtin_array_push_back(das_arg<TArray<TTuple<16,range,char *>>>::pass(__Arr_rename_at_181_16),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_17);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) &  __it_rename_at_1224_18, uint32_t & __value_rename_at_1224_19 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__it_rename_at_1224_18),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1224_19)),__context__));
}

inline uint32_t re_gen_get_rep_limit_aa71779c47fee5b8 ( Context * __context__ )
{
    return das_auto_cast<uint32_t>::cast(das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/);
}

inline void trace_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_79_20, int32_t __offset_rename_at_79_21 )
{
}

inline void note_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_86_22, int32_t __offset_rename_at_86_23 )
{
}

inline void trace_backtrack_60bdee73e3315732 ( Context * __context__ )
{
}

inline void trace_value_599ec0b97b535e0e ( Context * __context__, regex::ReNode * const  __value_rename_at_99_24 )
{
}

inline TVariant<16,8,regex::ReNode *,void *> nada_a36e274f9f880579 ( Context * __context__ )
{
    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((([&]() -> TVariant<16,8,regex::ReNode *,void *> {
        TVariant<16,8,regex::ReNode *,void *> __mkv_111;
        das_get_variant_field<void *,8,1>::set(__mkv_111) = nullptr;
        return __mkv_111;
    })()));
}

inline TVariant<16,8,regex::ReNode *,void *> maybe_cb698f19184a9bc8 ( Context * __context__, regex::ReNode * __value_rename_at_114_25 )
{
    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((([&]() -> TVariant<16,8,regex::ReNode *,void *> {
        TVariant<16,8,regex::ReNode *,void *> __mkv_116;
        das_get_variant_field<regex::ReNode *,8,0>::set(__mkv_116) = __value_rename_at_114_25;
        return __mkv_116;
    })()));
}

inline bool eos_bcd93149648ad887 ( Context * __context__, char * const  __expr_rename_at_119_26, int32_t __offset_rename_at_119_27 )
{
    return das_auto_cast<bool>::cast(__offset_rename_at_119_27 >= builtin_string_length(__expr_rename_at_119_26,__context__));
}

inline int32_t at_ac9760d67b4716d0 ( Context * __context__, char * const  __expr_rename_at_123_28, int32_t __offset_rename_at_123_29 )
{
    return das_auto_cast<int32_t>::cast(get_character_uat(__expr_rename_at_123_28,__offset_rename_at_123_29));
}

inline int32_t next_c7b14aa26d681478 ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> const  &  __re_rename_at_142_30 )
{
    if ( !das_get_variant_field<regex::ReNode *,8,0>::is(__re_rename_at_142_30) )
    {
        builtin_stackwalk(true,true,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_throw(((char *) "expecting value"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<int32_t>::cast(v_extract_yi(v_cast_vec4i(das_get_variant_field<regex::ReNode *,8,0>::as(__re_rename_at_142_30,__context__)->at)) /*y*/);
}

inline bool is_set_empty_1f052aa7f260c4eb ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_150_31 )
{
    {
        bool __need_loop_151 = true;
        // x: uint aka CharSet const&
        das_iterator<TDim<uint32_t,8> const > __x_iterator(__cset_rename_at_150_31);
        uint32_t const  * __x_rename_at_151_32;
        __need_loop_151 = __x_iterator.first(__context__,(__x_rename_at_151_32)) && __need_loop_151;
        for ( ; __need_loop_151 ; __need_loop_151 = __x_iterator.next(__context__,(__x_rename_at_151_32)) )
        {
            if ( (*__x_rename_at_151_32) != 0x0u )
            {
                return das_auto_cast<bool>::cast(false);
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_151_32));
    };
    return das_auto_cast<bool>::cast(true);
}

inline void set_or_char_5d5313390b879abc ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_159_33, int32_t __ch_rename_at_159_34 )
{
    __cset_rename_at_159_33((__ch_rename_at_159_34 >> 5),__context__) |= uint32_t(1 << (__ch_rename_at_159_34 & 31));
}

inline void set_or_range_40be02560bdb916d ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_163_35, range __bits_rename_at_163_36 )
{
    {
        bool __need_loop_164 = true;
        // ch: int const
        das_iterator<range> __ch_iterator(range(v_extract_xi(v_cast_vec4i(__bits_rename_at_163_36)) /*x*/,v_extract_yi(v_cast_vec4i(__bits_rename_at_163_36)) /*y*/ + 1));
        int32_t __ch_rename_at_164_37;
        __need_loop_164 = __ch_iterator.first(__context__,(__ch_rename_at_164_37)) && __need_loop_164;
        for ( ; __need_loop_164 ; __need_loop_164 = __ch_iterator.next(__context__,(__ch_rename_at_164_37)) )
        {
            __cset_rename_at_163_35((__ch_rename_at_164_37 >> 5),__context__) |= uint32_t(1 << (__ch_rename_at_164_37 & 31));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_164_37));
    };
}

inline void set_invert_4fb9b64ebf15e148 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_169_38 )
{
    {
        bool __need_loop_170 = true;
        // x: uint aka CharSet& -const
        das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_169_38);
        uint32_t * __x_rename_at_170_39;
        __need_loop_170 = __x_iterator.first(__context__,(__x_rename_at_170_39)) && __need_loop_170;
        for ( ; __need_loop_170 ; __need_loop_170 = __x_iterator.next(__context__,(__x_rename_at_170_39)) )
        {
            (*__x_rename_at_170_39) ^= 0xffffffffu;
        }
        __x_iterator.close(__context__,(__x_rename_at_170_39));
    };
}

inline void set_or_set_f7fe9d5d2a5b194b ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_175_40, TDim<uint32_t,8> const  &  __eset_rename_at_175_41 )
{
    {
        bool __need_loop_176 = true;
        // x: uint aka CharSet& -const
        das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_175_40);
        uint32_t * __x_rename_at_176_44;
        __need_loop_176 = __x_iterator.first(__context__,(__x_rename_at_176_44)) && __need_loop_176;
        // y: uint aka CharSet const&
        das_iterator<TDim<uint32_t,8> const > __y_iterator(__eset_rename_at_175_41);
        uint32_t const  * __y_rename_at_176_45;
        __need_loop_176 = __y_iterator.first(__context__,(__y_rename_at_176_45)) && __need_loop_176;
        for ( ; __need_loop_176 ; __need_loop_176 = __x_iterator.next(__context__,(__x_rename_at_176_44)) && __y_iterator.next(__context__,(__y_rename_at_176_45)) )
        {
            (*__x_rename_at_176_44) |= (*__y_rename_at_176_45);
        }
        __x_iterator.close(__context__,(__x_rename_at_176_44));
        __y_iterator.close(__context__,(__y_rename_at_176_45));
    };
}

inline void set_negative_e65d971bcd9eb5c1 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_181_46 )
{
    {
        bool __need_loop_182 = true;
        // x: uint aka CharSet& -const
        das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_181_46);
        uint32_t * __x_rename_at_182_47;
        __need_loop_182 = __x_iterator.first(__context__,(__x_rename_at_182_47)) && __need_loop_182;
        for ( ; __need_loop_182 ; __need_loop_182 = __x_iterator.next(__context__,(__x_rename_at_182_47)) )
        {
            das_copy((*__x_rename_at_182_47),~(*__x_rename_at_182_47));
        }
        __x_iterator.close(__context__,(__x_rename_at_182_47));
    };
}

inline void set_meta_4ca7bb80700cac47 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_187_48, int32_t __che_rename_at_187_49 )
{
    if ( __che_rename_at_187_49 == 119 )
    {
        set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),range(97,122));
        set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),range(65,90));
        set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),range(48,57));
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),95);
    } else if ( __che_rename_at_187_49 == 87 )
    {
        TDim<uint32_t,8> __eset_rename_at_194_50; das_zero(__eset_rename_at_194_50);
        set_meta_4ca7bb80700cac47(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_194_50),119);
        set_or_set_f7fe9d5d2a5b194b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_194_50));
    } else if ( __che_rename_at_187_49 == 115 )
    {
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),32);
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),9);
    } else if ( __che_rename_at_187_49 == 83 )
    {
        TDim<uint32_t,8> __eset_rename_at_201_51; das_zero(__eset_rename_at_201_51);
        set_meta_4ca7bb80700cac47(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_201_51),115);
        set_or_set_f7fe9d5d2a5b194b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_201_51));
    } else if ( __che_rename_at_187_49 == 100 )
    {
        set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),range(48,57));
    } else if ( __che_rename_at_187_49 == 68 )
    {
        TDim<uint32_t,8> __eset_rename_at_207_52; das_zero(__eset_rename_at_207_52);
        set_meta_4ca7bb80700cac47(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_207_52),100);
        set_or_set_f7fe9d5d2a5b194b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_207_52));
    } else {
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_48),__che_rename_at_187_49);
    };
}

inline bool is_meta_character_5210d144f518e88e ( Context * __context__, int32_t __ch_rename_at_218_53 )
{
    return das_auto_cast<bool>::cast(builtin_find_first_char_of(((char *) "\\+-*.()[]|^"),__ch_rename_at_218_53,__context__) != -1);
}

inline bool is_set_character_b3eb8d77360f374d ( Context * __context__, int32_t __ch_rename_at_222_54 )
{
    return das_auto_cast<bool>::cast(builtin_find_first_char_of(((char *) "wWsSdD"),__ch_rename_at_222_54,__context__) != -1);
}

inline int32_t from_hex_34b87ad3a69093f9 ( Context * __context__, int32_t __ch_rename_at_226_55 )
{
    if ( (__ch_rename_at_226_55 >= 48) && (__ch_rename_at_226_55 <= 57) )
    {
        return das_auto_cast<int32_t>::cast(__ch_rename_at_226_55 - 48);
    } else {
        if ( (__ch_rename_at_226_55 >= 97) && (__ch_rename_at_226_55 <= 102) )
        {
            return das_auto_cast<int32_t>::cast((__ch_rename_at_226_55 - 97) + 10);
        } else {
            if ( (__ch_rename_at_226_55 >= 65) && (__ch_rename_at_226_55 <= 70) )
            {
                return das_auto_cast<int32_t>::cast((__ch_rename_at_226_55 - 65) + 10);
            } else {
                builtin_throw(((char *) "from_hex"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                return das_auto_cast<int32_t>::cast(0);
            };
        };
    };
}

inline char * string_from_hex_3b7c632d00a40daf ( Context * __context__, int32_t __ch1_rename_at_244_56 )
{
    return das_auto_cast<char *>::cast(((char * const )(to_string_char(from_hex_34b87ad3a69093f9(__context__,__ch1_rename_at_244_56),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * string_from_hex_7d459c50a54015ae ( Context * __context__, int32_t __ch1_rename_at_248_57, int32_t __ch2_rename_at_248_58 )
{
    return das_auto_cast<char *>::cast(((char * const )(to_string_char(_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec(__context__,__ch1_rename_at_248_57,__ch2_rename_at_248_58),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline TVariant<16,8,regex::ReNode *,void *> re_char_f30c974269a757d6 ( Context * __context__, char * const  __expr_rename_at_253_59, int32_t __offset_rename_at_253_60 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_253_59,__offset_rename_at_253_60) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        int32_t __ch_rename_at_258_61 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_253_59,__offset_rename_at_253_60));
        if ( __ch_rename_at_258_61 == 92 )
        {
            int32_t __ech_rename_at_260_62 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_253_59,__offset_rename_at_253_60 + 1));
            if ( __ech_rename_at_260_62 == 0 )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
            } else {
                if ( __ech_rename_at_260_62 == 120 )
                {
                    int32_t __len_rename_at_266_63 = ((int32_t)builtin_string_length(__expr_rename_at_253_59,__context__));
                    if ( (__offset_rename_at_253_60 + 2) >= __len_rename_at_266_63 )
                    {
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                    } else {
                        int32_t __hex1_rename_at_271_64 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_253_59,__offset_rename_at_253_60 + 2));
                        if ( !das_invoke_function<bool>::invoke<int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::is_hex Ci*/ 13255244687218186342u)),__hex1_rename_at_271_64) )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                        } else {
                            if ( (__offset_rename_at_253_60 + 2) < __len_rename_at_266_63 )
                            {
                                int32_t __hex2_rename_at_277_65 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_253_59,__offset_rename_at_253_60 + 3));
                                if ( das_invoke_function<bool>::invoke<int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::is_hex Ci*/ 13255244687218186342u)),__hex2_rename_at_277_65) )
                                {
                                    note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_2, cast<char * const >::from(((char *) "hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_271_64,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char * const )(to_string_char(__hex2_rename_at_277_65,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec(__context__,__hex1_rename_at_271_64,__hex2_rename_at_277_65))))),__offset_rename_at_253_60);
                                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_280 = ReNode_a530ee43f17c77ab(__context__);
                                        das_copy((__mks_280.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                        das_copy((__mks_280.text),(string_from_hex_7d459c50a54015ae(__context__,__hex1_rename_at_271_64,__hex2_rename_at_277_65)));
                                        das_copy((__mks_280.textLen),(1));
                                        das_copy((__mks_280.at),(range(__offset_rename_at_253_60,__offset_rename_at_253_60 + 3)));
                                        return __mks_280;
                                    })()))));
                                };
                            };
                            note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_3, cast<char * const >::from(((char *) "hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_271_64,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_271_64))))),__offset_rename_at_253_60);
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_284 = ReNode_a530ee43f17c77ab(__context__);
                                das_copy((__mks_284.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                das_copy((__mks_284.text),(string_from_hex_3b7c632d00a40daf(__context__,__hex1_rename_at_271_64)));
                                das_copy((__mks_284.textLen),(1));
                                das_copy((__mks_284.at),(range(__offset_rename_at_253_60,__offset_rename_at_253_60 + 2)));
                                return __mks_284;
                            })()))));
                        };
                    };
                };
                if ( is_set_character_b3eb8d77360f374d(__context__,__ech_rename_at_260_62) )
                {
                    TDim<uint32_t,8> __cset_rename_at_287_66; das_zero(__cset_rename_at_287_66);
                    set_meta_4ca7bb80700cac47(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_287_66),__ech_rename_at_260_62);
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_289 = ReNode_a530ee43f17c77ab(__context__);
                        das_copy((__mks_289.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_289.cset),(__cset_rename_at_287_66));
                        das_copy((__mks_289.at),(range(__offset_rename_at_253_60,__offset_rename_at_253_60 + 2)));
                        return __mks_289;
                    })()))));
                } else {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_291 = ReNode_a530ee43f17c77ab(__context__);
                        das_copy((__mks_291.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_291.text),(((char * const )(to_string_char(__ech_rename_at_260_62,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                        das_copy((__mks_291.textLen),(1));
                        das_copy((__mks_291.at),(range(__offset_rename_at_253_60,__offset_rename_at_253_60 + 2)));
                        return __mks_291;
                    })()))));
                };
            };
        } else if ( is_meta_character_5210d144f518e88e(__context__,__ch_rename_at_258_61) )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        };
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_295 = ReNode_a530ee43f17c77ab(__context__);
            das_copy((__mks_295.op),(DAS_COMMENT(enum) regex::ReOp::Char));
            das_copy((__mks_295.text),(((char * const )(to_string_char(__ch_rename_at_258_61,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
            das_copy((__mks_295.textLen),(1));
            das_copy((__mks_295.at),(range(__offset_rename_at_253_60,__offset_rename_at_253_60 + 1)));
            return __mks_295;
        })()))));
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_set_items_24bb39b249ea7fd4 ( Context * __context__, char * const  __expr_rename_at_302_67, int32_t __offset_rename_at_302_68 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_302_67,__offset_rename_at_302_68) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        TDim<uint32_t,8> __cset_rename_at_307_69; das_zero(__cset_rename_at_307_69);
        int32_t __iofs_rename_at_308_70 = __offset_rename_at_302_68;
        int32_t __prev_char_rename_at_309_71 = -1;
        bool __next_range_rename_at_310_72 = false;
        bool __next_meta_rename_at_311_73 = false;
        while ( !eos_bcd93149648ad887(__context__,__expr_rename_at_302_67,__iofs_rename_at_308_70) )
        {
            int32_t __ch_rename_at_313_74 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_302_67,__iofs_rename_at_308_70));
            int32_t __next_char_rename_at_314_75 = -1;
            if ( __ch_rename_at_313_74 == 93 )
            {
                break;
            } else if ( __ch_rename_at_313_74 == 92 )
            {
                int32_t __che_rename_at_319_76 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_302_67,__iofs_rename_at_308_70 + 1));
                if ( __che_rename_at_319_76 == 0 )
                {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                } else {
                    if ( __che_rename_at_319_76 == 120 )
                    {
                        int32_t __len_rename_at_325_77 = ((int32_t)builtin_string_length(__expr_rename_at_302_67,__context__));
                        if ( (__iofs_rename_at_308_70 + 2) >= __len_rename_at_325_77 )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                        } else {
                            int32_t __hex1_rename_at_330_78 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_302_67,__iofs_rename_at_308_70 + 2));
                            if ( !das_invoke_function<bool>::invoke<int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::is_hex Ci*/ 13255244687218186342u)),__hex1_rename_at_330_78) )
                            {
                                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                            } else {
                                if ( (__iofs_rename_at_308_70 + 2) < __len_rename_at_325_77 )
                                {
                                    int32_t __hex2_rename_at_336_79 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_302_67,__iofs_rename_at_308_70 + 3));
                                    if ( das_invoke_function<bool>::invoke<int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::is_hex Ci*/ 13255244687218186342u)),__hex2_rename_at_336_79) )
                                    {
                                        das_copy(__next_char_rename_at_314_75,_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec(__context__,__hex1_rename_at_330_78,__hex2_rename_at_336_79));
                                        note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_4, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_330_78,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char * const )(to_string_char(__hex2_rename_at_336_79,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec(__context__,__hex1_rename_at_330_78,__hex2_rename_at_336_79))))),__iofs_rename_at_308_70);
                                        __iofs_rename_at_308_70 += 4;
                                    } else {
                                        das_copy(__next_char_rename_at_314_75,from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_330_78));
                                        note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_5, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_330_78,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_330_78))))),__iofs_rename_at_308_70);
                                        __iofs_rename_at_308_70 += 3;
                                    };
                                } else {
                                    das_copy(__next_char_rename_at_314_75,from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_330_78));
                                    note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_6, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_330_78,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_330_78))))),__offset_rename_at_302_68);
                                    __iofs_rename_at_308_70 += 3;
                                };
                            };
                        };
                    } else {
                        if ( __next_range_rename_at_310_72 )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                        } else {
                            if ( is_set_character_b3eb8d77360f374d(__context__,__che_rename_at_319_76) )
                            {
                                das_copy(__next_char_rename_at_314_75,-1);
                                das_copy(__next_meta_rename_at_311_73,true);
                                set_meta_4ca7bb80700cac47(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_307_69),__che_rename_at_319_76);
                            } else {
                                das_copy(__next_char_rename_at_314_75,__che_rename_at_319_76);
                            };
                            __iofs_rename_at_308_70 += 2;
                        };
                    };
                };
            } else if ( __ch_rename_at_313_74 == 45 )
            {
                if ( __prev_char_rename_at_309_71 == -1 )
                {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                } else {
                    das_copy(__next_range_rename_at_310_72,true);
                    __iofs_rename_at_308_70 += 1;
                };
            } else {
                das_copy(__next_char_rename_at_314_75,__ch_rename_at_313_74);
                __iofs_rename_at_308_70 += 1;
            };
            if ( __next_char_rename_at_314_75 != -1 )
            {
                if ( __next_range_rename_at_310_72 )
                {
                    set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_307_69),range(__prev_char_rename_at_309_71,__next_char_rename_at_314_75));
                    das_copy(__next_range_rename_at_310_72,false);
                    das_copy(__prev_char_rename_at_309_71,-1);
                } else {
                    set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_307_69),__next_char_rename_at_314_75);
                    das_copy(__prev_char_rename_at_309_71,__next_char_rename_at_314_75);
                };
            } else if ( __next_meta_rename_at_311_73 )
            {
                das_copy(__next_meta_rename_at_311_73,false);
            } else if ( !__next_range_rename_at_310_72 )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
            };
        };
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_398 = ReNode_a530ee43f17c77ab(__context__);
            das_copy((__mks_398.op),(DAS_COMMENT(enum) regex::ReOp::Set));
            das_copy((__mks_398.cset),(__cset_rename_at_307_69));
            das_copy((__mks_398.at),(range(__offset_rename_at_302_68,__iofs_rename_at_308_70)));
            return __mks_398;
        })()))));
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_set_a55f6e80806300ed ( Context * __context__, char * const  __expr_rename_at_404_80, int32_t __offset_rename_at_404_81 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_404_80,__offset_rename_at_404_81) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_404_80,__offset_rename_at_404_81) != 91 )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        } else {
            bool __negative_rename_at_412_82 = false;
            if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_404_80,__offset_rename_at_404_81 + 1) == 94 )
            {
                das_copy(__negative_rename_at_412_82,true);
            };
            TVariant<16,8,regex::ReNode *,void *> __oset_rename_at_416_83 = re_set_items_24bb39b249ea7fd4(__context__,__expr_rename_at_404_80,__negative_rename_at_412_82 ? das_auto_cast<int32_t>::cast((__offset_rename_at_404_81 + 2)) : das_auto_cast<int32_t>::cast((__offset_rename_at_404_81 + 1)));
            if ( das_get_variant_field<void *,8,1>::is(__oset_rename_at_416_83) )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
            } else {
                if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_404_80,next_c7b14aa26d681478(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oset_rename_at_416_83))) != 93 )
                {
                    finalize_4d9dff2c4111fea5(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oset_rename_at_416_83));
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                } else {
                    regex::ReNode * __oval_rename_at_426_84 = das_get_variant_field<regex::ReNode *,8,0>::as(__oset_rename_at_416_83,__context__);
                    das_copy(das_swizzle_ref<int32_t,range,0>::swizzle(__oval_rename_at_426_84->at) /*x*/,__offset_rename_at_404_81);
                    ++das_swizzle_ref<int32_t,range,1>::swizzle(__oval_rename_at_426_84->at) /*y*/;
                    das_copy(__oval_rename_at_426_84->op,DAS_COMMENT(enum) regex::ReOp::Set);
                    if ( __negative_rename_at_412_82 )
                    {
                        set_negative_e65d971bcd9eb5c1(__context__,das_arg<TDim<uint32_t,8>>::pass(__oval_rename_at_426_84->cset));
                    };
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oset_rename_at_416_83);
                };
            };
        };
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_any_994b96521819a3e ( Context * __context__, char * const  __expr_rename_at_438_85, int32_t __offset_rename_at_438_86 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_438_85,__offset_rename_at_438_86) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_438_85,__offset_rename_at_438_86) == 46 )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_444 = ReNode_a530ee43f17c77ab(__context__);
                das_copy((__mks_444.op),(DAS_COMMENT(enum) regex::ReOp::Any));
                das_copy((__mks_444.at),(range(__offset_rename_at_438_86,__offset_rename_at_438_86 + 1)));
                return __mks_444;
            })()))));
        } else {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        };
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_eos_12f3534068f58fcd ( Context * __context__, char * const  __expr_rename_at_450_87, int32_t __offset_rename_at_450_88 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_450_87,__offset_rename_at_450_88) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_450_87,__offset_rename_at_450_88) == 36 )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_456 = ReNode_a530ee43f17c77ab(__context__);
                das_copy((__mks_456.op),(DAS_COMMENT(enum) regex::ReOp::Eos));
                das_copy((__mks_456.at),(range(__offset_rename_at_450_88,__offset_rename_at_450_88 + 1)));
                return __mks_456;
            })()))));
        } else {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        };
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_group_78c2eacdeb9fefc1 ( Context * __context__, char * const  __expr_rename_at_462_89, int32_t __offset_rename_at_462_90 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_462_89,__offset_rename_at_462_90) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_462_89,__offset_rename_at_462_90) != 40 )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        } else {
            TVariant<16,8,regex::ReNode *,void *> __ore_rename_at_470_91 = re_re_57e53314b2b816e9(__context__,__expr_rename_at_462_89,__offset_rename_at_462_90 + 1);
            if ( das_get_variant_field<void *,8,1>::is(__ore_rename_at_470_91) )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
            } else {
                if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_462_89,next_c7b14aa26d681478(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__ore_rename_at_470_91))) != 41 )
                {
                    finalize_4d9dff2c4111fea5(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__ore_rename_at_470_91));
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                } else {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_478 = ReNode_a530ee43f17c77ab(__context__);
                        das_copy((__mks_478.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                        das_copy((__mks_478.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__ore_rename_at_470_91,__context__)));
                        das_copy((__mks_478.at),(range(__offset_rename_at_462_90,next_c7b14aa26d681478(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__ore_rename_at_470_91)) + 1)));
                        return __mks_478;
                    })()))));
                };
            };
        };
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_elementary_8e517f8be58bf1a6 ( Context * __context__, char * const  __expr_rename_at_482_92, int32_t __offset_rename_at_482_93 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_482_92,__offset_rename_at_482_93) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        TVariant<16,8,regex::ReNode *,void *> __ogr_rename_at_487_94 = re_group_78c2eacdeb9fefc1(__context__,__expr_rename_at_482_92,__offset_rename_at_482_93);
        if ( das_get_variant_field<regex::ReNode *,8,0>::is(__ogr_rename_at_487_94) )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__ogr_rename_at_487_94);
        } else {
            TVariant<16,8,regex::ReNode *,void *> __oany_rename_at_491_95 = re_any_994b96521819a3e(__context__,__expr_rename_at_482_92,__offset_rename_at_482_93);
            if ( das_get_variant_field<regex::ReNode *,8,0>::is(__oany_rename_at_491_95) )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oany_rename_at_491_95);
            } else {
                TVariant<16,8,regex::ReNode *,void *> __oeos_rename_at_495_96 = re_eos_12f3534068f58fcd(__context__,__expr_rename_at_482_92,__offset_rename_at_482_93);
                if ( das_get_variant_field<regex::ReNode *,8,0>::is(__oeos_rename_at_495_96) )
                {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oeos_rename_at_495_96);
                } else {
                    TVariant<16,8,regex::ReNode *,void *> __oset_rename_at_499_97 = re_set_a55f6e80806300ed(__context__,__expr_rename_at_482_92,__offset_rename_at_482_93);
                    if ( das_get_variant_field<regex::ReNode *,8,0>::is(__oset_rename_at_499_97) )
                    {
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oset_rename_at_499_97);
                    } else {
                        TVariant<16,8,regex::ReNode *,void *> __ochr_rename_at_503_98 = re_char_f30c974269a757d6(__context__,__expr_rename_at_482_92,__offset_rename_at_482_93);
                        if ( das_get_variant_field<regex::ReNode *,8,0>::is(__ochr_rename_at_503_98) )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__ochr_rename_at_503_98);
                        } else {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                        };
                    };
                };
            };
        };
    };
}

inline regex::ReNode * mk_concat_5c3b6674ba9a8a3f ( Context * __context__, regex::ReNode * __left_rename_at_510_99, regex::ReNode * __right_rename_at_510_100 )
{
    if ( __left_rename_at_510_99 == nullptr )
    {
        return das_auto_cast<regex::ReNode *>::cast(__right_rename_at_510_100);
    } else if ( (__left_rename_at_510_99->op == DAS_COMMENT(enum) regex::ReOp::Char) && (__right_rename_at_510_100->op == DAS_COMMENT(enum) regex::ReOp::Char) )
    {
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_510_99->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_510_100->at) /*y*/);
        SimPolicy<char *>::SetAdd((char *)&(__left_rename_at_510_99->text),cast<char *>::from(__right_rename_at_510_100->text),*__context__,nullptr);
        __left_rename_at_510_99->textLen += __right_rename_at_510_100->textLen;
        finalize_2e3cca8177873ef7(__context__,__right_rename_at_510_100);
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_510_99);
    } else if ( ((__left_rename_at_510_99->op == DAS_COMMENT(enum) regex::ReOp::Concat) && (__left_rename_at_510_99->right->op == DAS_COMMENT(enum) regex::ReOp::Char)) && (__right_rename_at_510_100->op == DAS_COMMENT(enum) regex::ReOp::Char) )
    {
        regex::ReNode * __lor_rename_at_524_101 = __left_rename_at_510_99->right;
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_510_99->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_510_100->at) /*y*/);
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__lor_rename_at_524_101->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_510_100->at) /*y*/);
        SimPolicy<char *>::SetAdd((char *)&(__lor_rename_at_524_101->text),cast<char *>::from(__right_rename_at_510_100->text),*__context__,nullptr);
        __lor_rename_at_524_101->textLen += __right_rename_at_510_100->textLen;
        finalize_2e3cca8177873ef7(__context__,__right_rename_at_510_100);
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_510_99);
    } else {
        return das_auto_cast<regex::ReNode *>::cast(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_534 = ReNode_a530ee43f17c77ab(__context__);
            das_copy((__mks_534.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_534.left),(__left_rename_at_510_99));
            das_copy((__mks_534.right),(__right_rename_at_510_100));
            das_copy((__mks_534.at),(range(v_extract_xi(v_cast_vec4i(__left_rename_at_510_99->at)) /*x*/,v_extract_yi(v_cast_vec4i(__right_rename_at_510_100->at)) /*y*/)));
            return __mks_534;
        })())));
    };
}

inline regex::ReNode * mk_union_cc5d9e9364416c61 ( Context * __context__, regex::ReNode * __left_rename_at_538_102, regex::ReNode * __right_rename_at_538_103 )
{
    if ( __left_rename_at_538_102->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_538_102->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_538_103->at) /*y*/);
        if ( __right_rename_at_538_103->op == DAS_COMMENT(enum) regex::ReOp::Union )
        {
            {
                bool __need_loop_542 = true;
                // x: regex::ReNode?&
                das_iterator<TArray<regex::ReNode *>> __x_iterator(__right_rename_at_538_103->all);
                regex::ReNode * * __x_rename_at_542_104;
                __need_loop_542 = __x_iterator.first(__context__,(__x_rename_at_542_104)) && __need_loop_542;
                for ( ; __need_loop_542 ; __need_loop_542 = __x_iterator.next(__context__,(__x_rename_at_542_104)) )
                {
                    _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9(__context__,das_arg<TArray<regex::ReNode *>>::pass(__left_rename_at_538_102->all),(*__x_rename_at_542_104));
                }
                __x_iterator.close(__context__,(__x_rename_at_542_104));
            };
            finalize_2e3cca8177873ef7(__context__,__right_rename_at_538_103);
        } else {
            _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9(__context__,das_arg<TArray<regex::ReNode *>>::pass(__left_rename_at_538_102->all),__right_rename_at_538_103);
        };
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_538_102);
    } else if ( __right_rename_at_538_103->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        das_copy(das_swizzle_ref<int32_t,range,0>::swizzle(__right_rename_at_538_103->at) /*x*/,das_swizzle_ref<int32_t,range,0>::swizzle(__left_rename_at_538_102->at) /*x*/);
        _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9(__context__,das_arg<TArray<regex::ReNode *>>::pass(__right_rename_at_538_103->all),__left_rename_at_538_102);
        return das_auto_cast<regex::ReNode *>::cast(__right_rename_at_538_103);
    } else {
        TDim<regex::ReNode *,2> _temp_make_local_557_50_64; _temp_make_local_557_50_64;
        return das_auto_cast<regex::ReNode *>::cast(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_557 = ReNode_a530ee43f17c77ab(__context__);
            das_copy((__mks_557.op),(DAS_COMMENT(enum) regex::ReOp::Union));
            das_move((__mks_557.all),(_FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6(__context__,das_arg<TDim<regex::ReNode *,2>>::pass((([&]() -> TDim<regex::ReNode *,2>& {
                _temp_make_local_557_50_64(0,__context__) = __left_rename_at_538_102;
                _temp_make_local_557_50_64(1,__context__) = __right_rename_at_538_103;
                return _temp_make_local_557_50_64;
            })())))));
            das_copy((__mks_557.at),(range(v_extract_xi(v_cast_vec4i(__left_rename_at_538_102->at)) /*x*/,v_extract_yi(v_cast_vec4i(__right_rename_at_538_103->at)) /*y*/)));
            return __mks_557;
        })())));
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_re_57e53314b2b816e9 ( Context * __context__, char * const  __expr_rename_at_565_105, int32_t __offset_rename_at_565_106 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_565_105,__offset_rename_at_565_106) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        int32_t __cofs_rename_at_570_107 = __offset_rename_at_565_106;
        regex::ReNode * __last_rename_at_571_108 = 0;
        while ( !eos_bcd93149648ad887(__context__,__expr_rename_at_565_105,__cofs_rename_at_570_107) )
        {
            if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_565_105,__cofs_rename_at_570_107) == 124 )
            {
                if ( __last_rename_at_571_108 == nullptr )
                {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                } else {
                    TVariant<16,8,regex::ReNode *,void *> __oright_rename_at_578_109 = re_re_57e53314b2b816e9(__context__,__expr_rename_at_565_105,__cofs_rename_at_570_107 + 1);
                    if ( das_get_variant_field<void *,8,1>::is(__oright_rename_at_578_109) )
                    {
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                    } else {
                        das_copy(__last_rename_at_571_108,mk_union_cc5d9e9364416c61(__context__,__last_rename_at_571_108,das_get_variant_field<regex::ReNode *,8,0>::as(__oright_rename_at_578_109,__context__)));
                        das_copy(__cofs_rename_at_570_107,das_swizzle_ref<int32_t,range,1>::swizzle(__last_rename_at_571_108->at) /*y*/);
                    };
                };
            } else {
                TVariant<16,8,regex::ReNode *,void *> __onext_rename_at_586_110 = re_basic_468d93210132438e(__context__,__expr_rename_at_565_105,__cofs_rename_at_570_107);
                if ( das_get_variant_field<void *,8,1>::is(__onext_rename_at_586_110) )
                {
                    TVariant<16,8,regex::ReNode *,void *> _temp_make_local_588_38_96; _temp_make_local_588_38_96;
                    TVariant<16,8,regex::ReNode *,void *> _temp_make_local_588_47_112; _temp_make_local_588_47_112;
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((__last_rename_at_571_108 == nullptr) ? das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((_temp_make_local_588_38_96 = (nada_a36e274f9f880579(__context__)))) : das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((_temp_make_local_588_47_112 = (maybe_cb698f19184a9bc8(__context__,__last_rename_at_571_108)))));
                } else {
                    das_copy(__last_rename_at_571_108,mk_concat_5c3b6674ba9a8a3f(__context__,__last_rename_at_571_108,das_get_variant_field<regex::ReNode *,8,0>::as(__onext_rename_at_586_110,__context__)));
                    das_copy(__cofs_rename_at_570_107,das_swizzle_ref<int32_t,range,1>::swizzle(__last_rename_at_571_108->at) /*y*/);
                };
            };
        };
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,__last_rename_at_571_108));
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_basic_468d93210132438e ( Context * __context__, char * const  __expr_rename_at_600_111, int32_t __offset_rename_at_600_112 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_600_111,__offset_rename_at_600_112) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        TVariant<16,8,regex::ReNode *,void *> __oelem_rename_at_605_113 = re_elementary_8e517f8be58bf1a6(__context__,__expr_rename_at_600_111,__offset_rename_at_600_112);
        if ( das_get_variant_field<void *,8,1>::is(__oelem_rename_at_605_113) )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        } else {
            int32_t __ch_rename_at_609_114 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_600_111,next_c7b14aa26d681478(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_605_113))));
            if ( __ch_rename_at_609_114 == 42 )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_611 = ReNode_a530ee43f17c77ab(__context__);
                    das_copy((__mks_611.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                    das_copy((__mks_611.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__oelem_rename_at_605_113,__context__)));
                    das_copy((__mks_611.at),(range(__offset_rename_at_600_112,next_c7b14aa26d681478(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_605_113)) + 1)));
                    return __mks_611;
                })()))));
            } else if ( __ch_rename_at_609_114 == 43 )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_613 = ReNode_a530ee43f17c77ab(__context__);
                    das_copy((__mks_613.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                    das_copy((__mks_613.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__oelem_rename_at_605_113,__context__)));
                    das_copy((__mks_613.at),(range(__offset_rename_at_600_112,next_c7b14aa26d681478(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_605_113)) + 1)));
                    return __mks_613;
                })()))));
            } else if ( __ch_rename_at_609_114 == 63 )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_615 = ReNode_a530ee43f17c77ab(__context__);
                    das_copy((__mks_615.op),(DAS_COMMENT(enum) regex::ReOp::Question));
                    das_copy((__mks_615.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__oelem_rename_at_605_113,__context__)));
                    das_copy((__mks_615.at),(range(__offset_rename_at_600_112,next_c7b14aa26d681478(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_605_113)) + 1)));
                    return __mks_615;
                })()))));
            };
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oelem_rename_at_605_113);
        };
    };
}

inline regex::ReNode * re_parse_a11fbde82111aa69 ( Context * __context__, char * const  __expr_rename_at_620_115 )
{
    TVariant<16,8,regex::ReNode *,void *> __ore_rename_at_621_116 = re_re_57e53314b2b816e9(__context__,__expr_rename_at_620_115,0);
    if ( das_get_variant_field<void *,8,1>::is(__ore_rename_at_621_116) )
    {
        return das_auto_cast<regex::ReNode *>::cast(nullptr);
    } else {
        int32_t __at_rename_at_625_117 = ((int32_t)next_c7b14aa26d681478(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__ore_rename_at_621_116)));
        return das_auto_cast<regex::ReNode *>::cast((__at_rename_at_625_117 != builtin_string_length(__expr_rename_at_620_115,__context__)) ? das_auto_cast<void *>::cast(nullptr) : das_auto_cast<void *>::cast(das_get_variant_field<regex::ReNode *,8,0>::as(__ore_rename_at_621_116,__context__)));
    };
}

inline void visit_top_down_4fecc3dfe95f303c ( Context * __context__, regex::ReNode * __node_rename_at_633_118, Block DAS_COMMENT((void,regex::ReNode *)) const  &  __blk_rename_at_633_119 )
{
    das_invoke<void>::invoke<regex::ReNode *>(__context__,nullptr,__blk_rename_at_633_119,__node_rename_at_633_118);
    {
        bool __need_loop_635 = true;
        // x: regex::ReNode?&
        das_iterator<TArray<regex::ReNode *>> __x_iterator(__node_rename_at_633_118->all);
        regex::ReNode * * __x_rename_at_635_120;
        __need_loop_635 = __x_iterator.first(__context__,(__x_rename_at_635_120)) && __need_loop_635;
        for ( ; __need_loop_635 ; __need_loop_635 = __x_iterator.next(__context__,(__x_rename_at_635_120)) )
        {
            visit_top_down_4fecc3dfe95f303c(__context__,(*__x_rename_at_635_120),__blk_rename_at_633_119);
        }
        __x_iterator.close(__context__,(__x_rename_at_635_120));
    };
    if ( __node_rename_at_633_118->subexpr != nullptr )
    {
        visit_top_down_4fecc3dfe95f303c(__context__,__node_rename_at_633_118->subexpr,__blk_rename_at_633_119);
    };
    if ( __node_rename_at_633_118->left != nullptr )
    {
        visit_top_down_4fecc3dfe95f303c(__context__,__node_rename_at_633_118->left,__blk_rename_at_633_119);
    };
    if ( __node_rename_at_633_118->right != nullptr )
    {
        visit_top_down_4fecc3dfe95f303c(__context__,__node_rename_at_633_118->right,__blk_rename_at_633_119);
    };
}

inline void re_assign_next_18621bfa0d3dbb85 ( Context * __context__, regex::Regex &  __re_rename_at_649_121 ) { das_stack_prologue __prologue(__context__,128,"re_assign_next " DAS_FILE_LINE);
{
    int32_t __id_rename_at_650_122 = 0;
    visit_top_down_4fecc3dfe95f303c(__context__,__re_rename_at_649_121.root,das_make_block<void,regex::ReNode *>(__context__,96,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_651_123) -> void{
        das_copy(__node_rename_at_651_123->id,__id_rename_at_650_122++);
        if ( __node_rename_at_651_123->op == DAS_COMMENT(enum) regex::ReOp::Concat )
        {
            das_copy(__node_rename_at_651_123->left->next,__node_rename_at_651_123->right);
            das_copy(__node_rename_at_651_123->right->next,__node_rename_at_651_123->next);
        } else if ( __node_rename_at_651_123->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            das_copy(__node_rename_at_651_123->subexpr->next,__node_rename_at_651_123->next);
        } else if ( __node_rename_at_651_123->op == DAS_COMMENT(enum) regex::ReOp::Union )
        {
            {
                bool __need_loop_659 = true;
                // sub: regex::ReNode?&
                das_iterator<TArray<regex::ReNode *>> __sub_iterator(__node_rename_at_651_123->all);
                regex::ReNode * * __sub_rename_at_659_124;
                __need_loop_659 = __sub_iterator.first(__context__,(__sub_rename_at_659_124)) && __need_loop_659;
                for ( ; __need_loop_659 ; __need_loop_659 = __sub_iterator.next(__context__,(__sub_rename_at_659_124)) )
                {
                    das_copy((*__sub_rename_at_659_124)->next,__node_rename_at_651_123->next);
                }
                __sub_iterator.close(__context__,(__sub_rename_at_659_124));
            };
        };
    }));
}}

inline void re_assign_groups_269827e3d8834c3a ( Context * __context__, regex::Regex &  __re_rename_at_666_125 ) { das_stack_prologue __prologue(__context__,144,"re_assign_groups " DAS_FILE_LINE);
{
    TTuple<16,range,char *> _temp_make_local_667_21_32; _temp_make_local_667_21_32;
    _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5(__context__,das_arg<TArray<TTuple<16,range,char *>>>::pass(__re_rename_at_666_125.groups),das_arg<TTuple<16,range,char *>>::pass((([&]() -> TTuple<16,range,char *>& {
        das_get_tuple_field<range,0>::get(_temp_make_local_667_21_32) = range(0,0);
        das_get_tuple_field<char *,8>::get(_temp_make_local_667_21_32) = nullptr;
        return _temp_make_local_667_21_32;
    })())));
    visit_top_down_4fecc3dfe95f303c(__context__,__re_rename_at_666_125.root,das_make_block<void,regex::ReNode *>(__context__,96,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_668_126) -> void{
        if ( __node_rename_at_668_126->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            TTuple<16,range,char *> _temp_make_local_672_29_128; _temp_make_local_672_29_128;
            int32_t __index_rename_at_670_127 = ((int32_t)builtin_array_size(das_arg<TArray<TTuple<16,range,char *>>>::pass(__re_rename_at_666_125.groups)));
            das_copy(__node_rename_at_668_126->index,__index_rename_at_670_127);
            _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5(__context__,das_arg<TArray<TTuple<16,range,char *>>>::pass(__re_rename_at_666_125.groups),das_arg<TTuple<16,range,char *>>::pass((([&]() -> TTuple<16,range,char *>& {
                das_get_tuple_field<range,0>::get(_temp_make_local_672_29_128) = range(0,0);
                das_get_tuple_field<char *,8>::get(_temp_make_local_672_29_128) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_7, cast<int32_t>::from(__index_rename_at_670_127)));
                return _temp_make_local_672_29_128;
            })())));
        };
    }));
}}

inline void re_assign_match_functions_d1123942ad08a870 ( Context * __context__, regex::Regex &  __re_rename_at_681_128 ) { das_stack_prologue __prologue(__context__,96,"re_assign_match_functions " DAS_FILE_LINE);
{
    visit_top_down_4fecc3dfe95f303c(__context__,__re_rename_at_681_128.root,das_make_block<void,regex::ReNode *>(__context__,80,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_682_129) -> void{
        if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Char )
        {
            if ( __node_rename_at_682_129->textLen == 1 )
            {
                das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_single_char S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 6640629096707467737u)));
            } else {
                das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_char S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 4088574829468119624u)));
            };
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_char 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 3380980448633920250u)));
        } else if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Union )
        {
            das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_union S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 2230596821349219651u)));
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_union 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 14178999509996687488u)));
        } else if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Set )
        {
            das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_set S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 3030744506824893410u)));
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_set 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 122017286083467237u)));
        } else if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Any )
        {
            das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_any S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 17605365244121121725u)));
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_any 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 6084357162673744672u)));
        } else if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Eos )
        {
            das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_eos S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 12892453015371357171u)));
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_eos 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 9020662661764102338u)));
        } else if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Concat )
        {
            das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_concat S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 11115845095969446172u)));
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_concat 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 15857368836291994000u)));
        } else if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Plus )
        {
            if ( __node_rename_at_682_129->subexpr->op == DAS_COMMENT(enum) regex::ReOp::Set )
            {
                das_copy(__node_rename_at_682_129->cset,__node_rename_at_682_129->subexpr->cset);
                das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_plus_set S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 6735232670081758825u)));
            } else {
                das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_plus S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 11050022902774489337u)));
            };
            das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_plus S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 11050022902774489337u)));
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_plus 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 5937276475435012011u)));
        } else if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Star )
        {
            das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_star S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 14800874501278811928u)));
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_star 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 14644857912298426154u)));
        } else if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Question )
        {
            das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_question S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 17271855316612624241u)));
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_question 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 16449961801252633731u)));
        } else if ( __node_rename_at_682_129->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            das_copy(__node_rename_at_682_129->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_group S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 9828266076507982921u)));
            das_copy(__node_rename_at_682_129->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_group 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 14186960190802327978u)));
        } else {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_8, cast<char * const >::from(((char *) "unsupported ")), cast<DAS_COMMENT(enum) regex::ReOp>::from(__node_rename_at_682_129->op))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
    }));
}}

inline uint8_t const  * re_match2_single_char_efd3120a5ed2dc78 ( Context * __context__, regex::Regex &  __regex_rename_at_730_130, regex::ReNode * __node_rename_at_730_131, uint8_t const  * const  __str_rename_at_730_132 )
{
    if ( das_equ_val((*(__str_rename_at_730_132)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( get_character_uat(__node_rename_at_730_131->text,0) != int32_t((*(__str_rename_at_730_132))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_741_133 = das_ptr_add_int32(__str_rename_at_730_132,1,1);
            das_copy(__node_rename_at_730_131->tail,__tail_rename_at_741_133);
            regex::ReNode * __node2_rename_at_743_134 = __node_rename_at_730_131->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_743_134 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_743_134->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_730_130),__node2_rename_at_743_134,__tail_rename_at_741_133)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_741_133));
        };
    };
}

inline uint8_t const  * re_match2_char_563cc8699254595c ( Context * __context__, regex::Regex &  __regex_rename_at_753_135, regex::ReNode * __node_rename_at_753_136, uint8_t const  * const  __str_rename_at_753_137 )
{
    if ( das_equ_val((*(__str_rename_at_753_137)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( das_memcmp(das_auto_cast<void * const >::cast(das_cast<uint8_t *>::cast(__node_rename_at_753_136->text)),das_auto_cast<void * const >::cast(__str_rename_at_753_137),__node_rename_at_753_136->textLen) != 0 )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_764_138 = das_ptr_add_int32(__str_rename_at_753_137,__node_rename_at_753_136->textLen,1);
            das_copy(__node_rename_at_753_136->tail,__tail_rename_at_764_138);
            regex::ReNode * __node2_rename_at_766_139 = __node_rename_at_753_136->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_766_139 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_766_139->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_753_135),__node2_rename_at_766_139,__tail_rename_at_764_138)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_764_138));
        };
    };
}

inline uint8_t const  * re_match2_union_483bf85dbc5196dd ( Context * __context__, regex::Regex &  __regex_rename_at_779_140, regex::ReNode * __node_rename_at_779_141, uint8_t const  * const  __str_rename_at_779_142 )
{
    if ( das_equ_val((*(__str_rename_at_779_142)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        {
            bool __need_loop_786 = true;
            // sub: regex::ReNode?&
            das_iterator<TArray<regex::ReNode *>> __sub_iterator(__node_rename_at_779_141->all);
            regex::ReNode * * __sub_rename_at_786_143;
            __need_loop_786 = __sub_iterator.first(__context__,(__sub_rename_at_786_143)) && __need_loop_786;
            for ( ; __need_loop_786 ; __need_loop_786 = __sub_iterator.next(__context__,(__sub_rename_at_786_143)) )
            {
                uint8_t const  * __osub_rename_at_787_144 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,(*__sub_rename_at_786_143)->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_779_140),(*__sub_rename_at_786_143),__str_rename_at_779_142));
                if ( __osub_rename_at_787_144 != nullptr )
                {
                    das_copy(__node_rename_at_779_141->tail,(*__sub_rename_at_786_143)->tail);
                    return das_auto_cast<uint8_t const  *>::cast(__osub_rename_at_787_144);
                };
            }
            __sub_iterator.close(__context__,(__sub_rename_at_786_143));
        };
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    };
}

inline uint8_t const  * re_match2_set_31b09b463a9d7b2b ( Context * __context__, regex::Regex &  __regex_rename_at_797_145, regex::ReNode * __node_rename_at_797_146, uint8_t const  * const  __str_rename_at_797_147 )
{
    if ( das_equ_val((*(__str_rename_at_797_147)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( !is_char_in_set(int32_t((*(__str_rename_at_797_147))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_797_146->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_808_148 = das_ptr_add_int32(__str_rename_at_797_147,1,1);
            das_copy(__node_rename_at_797_146->tail,__tail_rename_at_808_148);
            regex::ReNode * __node2_rename_at_810_149 = __node_rename_at_797_146->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_810_149 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_810_149->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_797_145),__node2_rename_at_810_149,__tail_rename_at_808_148)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_808_148));
        };
    };
}

inline uint8_t const  * re_match2_any_9b00b66687d95b62 ( Context * __context__, regex::Regex &  __regex_rename_at_820_150, regex::ReNode * __node_rename_at_820_151, uint8_t const  * const  __str_rename_at_820_152 )
{
    if ( das_equ_val((*(__str_rename_at_820_152)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        uint8_t const  * __tail_rename_at_828_153 = das_ptr_add_int32(__str_rename_at_820_152,1,1);
        das_copy(__node_rename_at_820_151->tail,__tail_rename_at_828_153);
        regex::ReNode * __node2_rename_at_830_154 = __node_rename_at_820_151->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_830_154 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_830_154->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_820_150),__node2_rename_at_830_154,__tail_rename_at_828_153)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_828_153));
    };
}

inline uint8_t const  * re_match2_concat_2bd31fc7755cdf2a ( Context * __context__, regex::Regex &  __regex_rename_at_840_155, regex::ReNode * __node_rename_at_840_156, uint8_t const  * const  __str_rename_at_840_157 )
{
    if ( das_equ_val((*(__str_rename_at_840_157)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        regex::ReNode * __left_rename_at_847_158 = __node_rename_at_840_156->left;
        uint8_t const  * __oleft_rename_at_848_159 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__left_rename_at_847_158->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_840_155),__left_rename_at_847_158,__str_rename_at_840_157));
        if ( __oleft_rename_at_848_159 == nullptr )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            das_copy(__node_rename_at_840_156->tail,__node_rename_at_840_156->right->tail);
            return das_auto_cast<uint8_t const  *>::cast(__oleft_rename_at_848_159);
        };
    };
}

inline uint8_t const  * re_match2_eos_a35408df9434ba3c ( Context * __context__, regex::Regex &  __regex_rename_at_857_160, regex::ReNode * __node_rename_at_857_161, uint8_t const  * const  __str_rename_at_857_162 )
{
    if ( das_nequ_val((*(__str_rename_at_857_162)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_857_161->tail,__str_rename_at_857_162);
        regex::ReNode * __node2_rename_at_865_163 = __node_rename_at_857_161->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_865_163 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__node2_rename_at_865_163->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_857_160),__node2_rename_at_865_163,__str_rename_at_857_162)) : das_auto_cast<uint8_t const  *>::cast(__str_rename_at_857_162));
    };
}

inline uint8_t const  * re_match2_question_4acdfdc420ce399a ( Context * __context__, regex::Regex &  __regex_rename_at_874_164, regex::ReNode * __node_rename_at_874_165, uint8_t const  * const  __str_rename_at_874_166 )
{
    regex::ReNode * __nsub_rename_at_878_167 = __node_rename_at_874_165->subexpr;
    uint8_t const  * __tail_rename_at_879_168 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_878_167->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_874_164),__nsub_rename_at_878_167,__str_rename_at_874_166);
    if ( __tail_rename_at_879_168 == nullptr )
    {
        das_copy(__tail_rename_at_879_168,__str_rename_at_874_166);
    };
    das_copy(__node_rename_at_874_165->tail,__tail_rename_at_879_168);
    regex::ReNode * __node2_rename_at_884_169 = __node_rename_at_874_165->next;
    return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_884_169 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_884_169->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_874_164),__node2_rename_at_884_169,__tail_rename_at_879_168)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_879_168));
}

inline uint8_t const  * re_match2_plus_adb26cc707eb0c55 ( Context * __context__, regex::Regex &  __regex_rename_at_893_170, regex::ReNode * __node_rename_at_893_171, uint8_t const  * const  __str_rename_at_893_172 )
{
    if ( das_equ_val((*(__str_rename_at_893_172)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        regex::ReNode * __nsub_rename_at_900_173 = __node_rename_at_893_171->subexpr;
        uint8_t const  * __osym_rename_at_901_174 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_900_173->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_893_170),__nsub_rename_at_900_173,__str_rename_at_893_172);
        if ( __osym_rename_at_901_174 == nullptr )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __ofs_rename_at_905_175 = __osym_rename_at_901_174;
            regex::ReNode * __node2_rename_at_906_176 = __node_rename_at_893_171->next;
            if ( __node2_rename_at_906_176 != nullptr )
            {
                while ( __osym_rename_at_901_174 != nullptr )
                {
                    das_copy(__node_rename_at_893_171->tail,__osym_rename_at_901_174);
                    uint8_t const  * __otail_rename_at_910_177 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_906_176->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_893_170),__node2_rename_at_906_176,__osym_rename_at_901_174);
                    if ( __otail_rename_at_910_177 != nullptr )
                    {
                        return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_910_177);
                    } else {
                        das_copy(__osym_rename_at_901_174,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_900_173->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_893_170),__nsub_rename_at_900_173,__osym_rename_at_901_174));
                    };
                };
                return das_auto_cast<uint8_t const  *>::cast(nullptr);
            } else {
                while ( __osym_rename_at_901_174 != nullptr )
                {
                    das_copy(__ofs_rename_at_905_175,__osym_rename_at_901_174);
                    das_copy(__osym_rename_at_901_174,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_900_173->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_893_170),__nsub_rename_at_900_173,__ofs_rename_at_905_175));
                };
                das_copy(__node_rename_at_893_171->tail,__ofs_rename_at_905_175);
                return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_905_175);
            };
        };
    };
}

inline uint8_t const  * re_match2_plus_set_aeb296540242ef22 ( Context * __context__, regex::Regex &  __regex_rename_at_928_178, regex::ReNode * __node_rename_at_928_179, uint8_t const  * const  __str_rename_at_928_180 )
{
    if ( das_equ_val((*(__str_rename_at_928_180)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( !is_char_in_set(int32_t((*(__str_rename_at_928_180))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_928_179->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __ofs_rename_at_940_181 = das_ptr_add_int32(__str_rename_at_928_180,1,1);
            uint8_t const  * __osym_rename_at_941_182 = __ofs_rename_at_940_181;
            regex::ReNode * __node2_rename_at_942_183 = __node_rename_at_928_179->next;
            if ( __node2_rename_at_942_183 != nullptr )
            {
                while ( __osym_rename_at_941_182 != nullptr )
                {
                    das_copy(__node_rename_at_928_179->tail,__osym_rename_at_941_182);
                    uint8_t const  * __otail_rename_at_946_184 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_942_183->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_928_178),__node2_rename_at_942_183,__osym_rename_at_941_182);
                    if ( __otail_rename_at_946_184 != nullptr )
                    {
                        return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_946_184);
                    } else {
                        if ( !is_char_in_set(int32_t((*(__osym_rename_at_941_182))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_928_179->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                        {
                            return das_auto_cast<uint8_t const  *>::cast(nullptr);
                        } else {
                            das_ptr_inc(__osym_rename_at_941_182,1);
                        };
                    };
                };
                return das_auto_cast<uint8_t const  *>::cast(nullptr);
            } else {
                while ( __osym_rename_at_941_182 != nullptr )
                {
                    das_copy(__ofs_rename_at_940_181,__osym_rename_at_941_182);
                    if ( !is_char_in_set(int32_t((*(__osym_rename_at_941_182))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_928_179->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        break;
                    } else {
                        das_ptr_inc(__osym_rename_at_941_182,1);
                    };
                };
                das_copy(__node_rename_at_928_179->tail,__ofs_rename_at_940_181);
                return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_940_181);
            };
        };
    };
}

inline uint8_t const  * re_match2_star_bb64c06dd606906e ( Context * __context__, regex::Regex &  __regex_rename_at_971_185, regex::ReNode * __node_rename_at_971_186, uint8_t const  * const  __str_rename_at_971_187 )
{
    uint8_t const  * __ofs_rename_at_975_188 = __str_rename_at_971_187;
    regex::ReNode * __node2_rename_at_976_189 = __node_rename_at_971_186->next;
    regex::ReNode * __nsub_rename_at_977_190 = __node_rename_at_971_186->subexpr;
    if ( __node2_rename_at_976_189 != nullptr )
    {
        while ( __ofs_rename_at_975_188 != nullptr )
        {
            das_copy(__node_rename_at_971_186->tail,__ofs_rename_at_975_188);
            uint8_t const  * __oany_rename_at_981_191 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_976_189->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_971_185),__node2_rename_at_976_189,__ofs_rename_at_975_188));
            if ( __oany_rename_at_981_191 != nullptr )
            {
                return das_auto_cast<uint8_t const  *>::cast(__oany_rename_at_981_191);
            } else {
                das_copy(__ofs_rename_at_975_188,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_977_190->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_971_185),__nsub_rename_at_977_190,__ofs_rename_at_975_188));
            };
        };
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        uint8_t const  * __osym_rename_at_989_192 = __ofs_rename_at_975_188;
        while ( __osym_rename_at_989_192 != nullptr )
        {
            das_copy(__ofs_rename_at_975_188,__osym_rename_at_989_192);
            das_copy(__osym_rename_at_989_192,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_977_190->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_971_185),__nsub_rename_at_977_190,__ofs_rename_at_975_188));
        };
        das_copy(__node_rename_at_971_186->tail,__ofs_rename_at_975_188);
        return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_975_188);
    };
}

inline uint8_t const  * re_match2_group_82784eced054ba4e ( Context * __context__, regex::Regex &  __regex_rename_at_1000_193, regex::ReNode * __node_rename_at_1000_194, uint8_t const  * const  __str_rename_at_1000_195 )
{
    if ( das_equ_val((*(__str_rename_at_1000_195)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        regex::ReNode * __nsub_rename_at_1007_196 = __node_rename_at_1000_194->subexpr;
        uint8_t const  * __osub_rename_at_1008_197 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_1007_196->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1000_193),__nsub_rename_at_1007_196,__str_rename_at_1000_195));
        if ( __osub_rename_at_1008_197 == nullptr )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            das_copy(das_get_tuple_field<range,0>::get(__regex_rename_at_1000_193.groups(__node_rename_at_1000_194->index,__context__)),range(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__str_rename_at_1000_195),das_auto_cast<void * const >::cast(__regex_rename_at_1000_193.match),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__node_rename_at_1000_194->subexpr->tail),das_auto_cast<void * const >::cast(__regex_rename_at_1000_193.match),1))));
            return das_auto_cast<uint8_t const  *>::cast(__osub_rename_at_1008_197);
        };
    };
}

inline bool re_early_out_80d62a0fdd14cb06 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_1020_198, regex::ReNode * const  __node_rename_at_1020_199 )
{
    if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Char )
    {
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_198),get_character_at(__node_rename_at_1020_199->text,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Set )
    {
        set_or_set_f7fe9d5d2a5b194b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_198),__node_rename_at_1020_199->cset);
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Any )
    {
        {
            bool __need_loop_1028 = true;
            // x: uint aka CharSet& -const
            das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_1020_198);
            uint32_t * __x_rename_at_1028_200;
            __need_loop_1028 = __x_iterator.first(__context__,(__x_rename_at_1028_200)) && __need_loop_1028;
            for ( ; __need_loop_1028 ; __need_loop_1028 = __x_iterator.next(__context__,(__x_rename_at_1028_200)) )
            {
                das_copy((*__x_rename_at_1028_200),0xffffffffu);
            }
            __x_iterator.close(__context__,(__x_rename_at_1028_200));
        };
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Eos )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Group )
    {
        return das_auto_cast<bool>::cast(re_early_out_80d62a0fdd14cb06(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_198),__node_rename_at_1020_199->subexpr));
    } else if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Plus )
    {
        return das_auto_cast<bool>::cast(re_early_out_80d62a0fdd14cb06(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_198),__node_rename_at_1020_199->subexpr));
    } else if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Star )
    {
        re_early_out_80d62a0fdd14cb06(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_198),__node_rename_at_1020_199->subexpr);
        return das_auto_cast<bool>::cast(true);
    } else if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Question )
    {
        re_early_out_80d62a0fdd14cb06(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_198),__node_rename_at_1020_199->subexpr);
        return das_auto_cast<bool>::cast(true);
    } else if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Concat )
    {
        bool __left_rename_at_1045_201 = ((bool)re_early_out_80d62a0fdd14cb06(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_198),__node_rename_at_1020_199->left));
        return das_auto_cast<bool>::cast(__left_rename_at_1045_201 ? das_auto_cast<bool>::cast(re_early_out_80d62a0fdd14cb06(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_198),__node_rename_at_1020_199->right)) : das_auto_cast<bool>::cast(false));
    } else if ( __node_rename_at_1020_199->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        bool __any_rename_at_1048_202 = false;
        {
            bool __need_loop_1049 = true;
            // sub: regex::ReNode? const&
            das_iterator<TArray<regex::ReNode *> const > __sub_iterator(__node_rename_at_1020_199->all);
            regex::ReNode * const  * __sub_rename_at_1049_203;
            __need_loop_1049 = __sub_iterator.first(__context__,(__sub_rename_at_1049_203)) && __need_loop_1049;
            for ( ; __need_loop_1049 ; __need_loop_1049 = __sub_iterator.next(__context__,(__sub_rename_at_1049_203)) )
            {
                das_copy(__any_rename_at_1048_202,re_early_out_80d62a0fdd14cb06(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_198),(*__sub_rename_at_1049_203)) || __any_rename_at_1048_202);
            }
            __sub_iterator.close(__context__,(__sub_rename_at_1049_203));
        };
        return das_auto_cast<bool>::cast(__any_rename_at_1048_202);
    } else {
        builtin_throw(((char *) "unsupported operation"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<bool>::cast(false);
    };
}

inline bool is_valid_87c94e9920e8a112 ( Context * __context__, regex::Regex &  __re_rename_at_1063_204 )
{
    return das_auto_cast<bool>::cast(__re_rename_at_1063_204.root != nullptr);
}

inline bool regex_compile_b802a48fba17c551 ( Context * __context__, regex::Regex &  __re_rename_at_1068_205, char * const  __expr_rename_at_1068_206 )
{
    das_copy(__re_rename_at_1068_205.root,re_parse_a11fbde82111aa69(__context__,__expr_rename_at_1068_206));
    if ( __re_rename_at_1068_205.root != nullptr )
    {
        re_assign_next_18621bfa0d3dbb85(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1068_205));
        re_assign_groups_269827e3d8834c3a(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1068_205));
        re_assign_match_functions_d1123942ad08a870(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1068_205));
        re_early_out_80d62a0fdd14cb06(__context__,das_arg<TDim<uint32_t,8>>::pass(__re_rename_at_1068_205.earlyOut),__re_rename_at_1068_205.root);
        das_copy(__re_rename_at_1068_205.canEarlyOut,!is_set_empty_1f052aa7f260c4eb(__context__,das_arg<TDim<uint32_t,8>>::pass(__re_rename_at_1068_205.earlyOut)));
    };
    return das_auto_cast<bool>::cast(__re_rename_at_1068_205.root != nullptr);
}

inline regex::Regex regex_compile_d14729dab5987744 ( Context * __context__, char * const  __expr_rename_at_1082_207 )
{
    regex::Regex __re_rename_at_1083_208; das_zero(__re_rename_at_1083_208);
    if ( !regex_compile_b802a48fba17c551(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1083_208),__expr_rename_at_1082_207) )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_9, cast<char * const >::from(((char *) "regular expression ")), cast<char * const >::from(__expr_rename_at_1082_207), cast<char * const >::from(((char *) " did not compile")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return /* <- */ das_auto_cast_move<regex::Regex>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_182d4b6f1ba86427(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1083_208)));
}

inline regex::Regex regex_compile_8b8cd96a70efcb1c ( Context * __context__, regex::Regex &  __re_rename_at_1090_209 )
{
    if ( __re_rename_at_1090_209.root != nullptr )
    {
        re_assign_next_18621bfa0d3dbb85(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1090_209));
        re_assign_match_functions_d1123942ad08a870(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1090_209));
    };
    return /* <- */ das_auto_cast_move<regex::Regex>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_182d4b6f1ba86427(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1090_209)));
}

inline int32_t regex_match_d1084a47d6e2d53e ( Context * __context__, regex::Regex &  __regex_rename_at_1098_210, char * const  __str_rename_at_1098_211, int32_t __offset_rename_at_1098_212 )
{
    if ( builtin_empty(__str_rename_at_1098_211) )
    {
        return das_auto_cast<int32_t>::cast(-1);
    } else {
        das_copy(__regex_rename_at_1098_210.match,das_cast<uint8_t *>::cast(__str_rename_at_1098_211));
        uint8_t const  * __mptr_rename_at_1109_213 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__regex_rename_at_1098_210.root->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1098_210),__regex_rename_at_1098_210.root,__regex_rename_at_1098_210.match));
        return das_auto_cast<int32_t>::cast((__mptr_rename_at_1109_213 == nullptr) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__mptr_rename_at_1109_213),das_auto_cast<void * const >::cast(__regex_rename_at_1098_210.match),1))));
    };
}

inline char * regex_group_f150ed72d9c6b909 ( Context * __context__, regex::Regex const  &  __regex_rename_at_1117_214, int32_t __index_rename_at_1117_215, char * const  __match_rename_at_1117_216 )
{
    range __sub_range_rename_at_1119_217 = ((range)das_get_tuple_field<range,0>::get(__regex_rename_at_1117_214.groups(__index_rename_at_1117_215,__context__)));
    return das_auto_cast<char *>::cast(((char * const )(builtin_string_slice1(__match_rename_at_1117_216,v_extract_xi(v_cast_vec4i(__sub_range_rename_at_1119_217)) /*x*/,v_extract_yi(v_cast_vec4i(__sub_range_rename_at_1119_217)) /*y*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline void regex_foreach_7115f79e7b67800f ( Context * __context__, regex::Regex &  __regex_rename_at_1124_218, char * const  __str_rename_at_1124_219, Block DAS_COMMENT((bool,range)) const  &  __blk_rename_at_1124_220 )
{
    if ( builtin_empty(__str_rename_at_1124_219) )
    {
        return ;
    } else {
        das_copy(__regex_rename_at_1124_218.match,das_cast<uint8_t const  *>::cast(__str_rename_at_1124_219));
        regex::ReNode * __root_rename_at_1131_221 = __regex_rename_at_1124_218.root;
        uint8_t const  * __pstr_rename_at_1132_222 = das_cast<uint8_t const  *>::cast(__str_rename_at_1124_219);
        uint8_t const  * __cstr_rename_at_1133_223 = __pstr_rename_at_1132_222;
        if ( __regex_rename_at_1124_218.canEarlyOut )
        {
            while ( true )
            {
                int32_t __Ch_rename_at_1137_224 = ((int32_t)int32_t((*(__cstr_rename_at_1133_223))));
                if ( __Ch_rename_at_1137_224 == 0 )
                {
                    break;
                } else {
                    if ( is_char_in_set(__Ch_rename_at_1137_224,das_arg<TDim<uint32_t,8>>::pass(__regex_rename_at_1124_218.earlyOut),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        uint8_t const  * __om_rename_at_1142_225 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1131_221->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1124_218),__root_rename_at_1131_221,__cstr_rename_at_1133_223));
                        if ( __om_rename_at_1142_225 != nullptr )
                        {
                            if ( !das_invoke<bool>::invoke<range>(__context__,nullptr,__blk_rename_at_1124_220,range(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1133_223),das_auto_cast<void * const >::cast(__pstr_rename_at_1132_222),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1142_225),das_auto_cast<void * const >::cast(__pstr_rename_at_1132_222),1)))) )
                            {
                                break;
                            } else {
                                das_copy(__cstr_rename_at_1133_223,__om_rename_at_1142_225);
                            };
                        } else {
                            das_ptr_inc(__cstr_rename_at_1133_223,1);
                        };
                    } else {
                        das_ptr_inc(__cstr_rename_at_1133_223,1);
                    };
                };
            };
        } else {
            while ( int32_t((*(__cstr_rename_at_1133_223))) != 0 )
            {
                uint8_t const  * __om_rename_at_1157_226 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1131_221->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1124_218),__root_rename_at_1131_221,__cstr_rename_at_1133_223));
                if ( __om_rename_at_1157_226 != nullptr )
                {
                    if ( !das_invoke<bool>::invoke<range>(__context__,nullptr,__blk_rename_at_1124_220,range(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1133_223),das_auto_cast<void * const >::cast(__pstr_rename_at_1132_222),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1157_226),das_auto_cast<void * const >::cast(__pstr_rename_at_1132_222),1)))) )
                    {
                        break;
                    } else {
                        das_copy(__cstr_rename_at_1133_223,__om_rename_at_1157_226);
                    };
                } else {
                    das_ptr_inc(__cstr_rename_at_1133_223,1);
                };
            };
        };
    };
}

inline char * regex_replace_6c020ee2b76e6419 ( Context * __context__, regex::Regex &  __regex_rename_at_1172_227, char * const  __str_rename_at_1172_228, Block DAS_COMMENT((char *,char * const )) const  &  __blk_rename_at_1172_229 )
{
    return das_auto_cast<char *>::cast(builtin_empty(__str_rename_at_1172_228) ? das_auto_cast<char * const >::cast(nullptr) : das_auto_cast<char * const >::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1178_230) DAS_AOT_INLINE_LAMBDA -> void{
        das_copy(__regex_rename_at_1172_227.match,das_cast<uint8_t const  *>::cast(__str_rename_at_1172_228));
        regex::ReNode * __root_rename_at_1180_231 = __regex_rename_at_1172_227.root;
        uint8_t const  * __pstr_rename_at_1181_232 = das_cast<uint8_t const  *>::cast(__str_rename_at_1172_228);
        uint8_t const  * __cstr_rename_at_1182_233 = __pstr_rename_at_1181_232;
        if ( __regex_rename_at_1172_227.canEarlyOut )
        {
            while ( true )
            {
                int32_t __Ch_rename_at_1186_234 = ((int32_t)int32_t((*(__cstr_rename_at_1182_233))));
                if ( __Ch_rename_at_1186_234 == 0 )
                {
                    break;
                } else {
                    if ( is_char_in_set(__Ch_rename_at_1186_234,das_arg<TDim<uint32_t,8>>::pass(__regex_rename_at_1172_227.earlyOut),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        uint8_t const  * __om_rename_at_1191_235 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1180_231->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1172_227),__root_rename_at_1180_231,__cstr_rename_at_1182_233));
                        if ( __om_rename_at_1191_235 != nullptr )
                        {
                            char * __repl_rename_at_1193_236 = ((char *)(char *)(das_invoke<char *>::invoke<char * const >(__context__,nullptr,__blk_rename_at_1172_229,((char * const )(builtin_string_slice1(__str_rename_at_1172_228,int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1182_233),das_auto_cast<void * const >::cast(__pstr_rename_at_1181_232),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1191_235),das_auto_cast<void * const >::cast(__pstr_rename_at_1181_232),1)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_10,cast<StringBuilderWriter &>::from(__writer_rename_at_1178_230),cast<char * const >::from(__repl_rename_at_1193_236)));
                            das_copy(__cstr_rename_at_1182_233,__om_rename_at_1191_235);
                        } else {
                            write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1178_230),__Ch_rename_at_1186_234);
                            das_ptr_inc(__cstr_rename_at_1182_233,1);
                        };
                    } else {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1178_230),__Ch_rename_at_1186_234);
                        das_ptr_inc(__cstr_rename_at_1182_233,1);
                    };
                };
            };
        } else {
            while ( int32_t((*(__cstr_rename_at_1182_233))) != 0 )
            {
                uint8_t const  * __om_rename_at_1207_237 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1180_231->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1172_227),__root_rename_at_1180_231,__cstr_rename_at_1182_233));
                if ( __om_rename_at_1207_237 != nullptr )
                {
                    char * __repl_rename_at_1209_238 = ((char *)(char *)(das_invoke<char *>::invoke<char * const >(__context__,nullptr,__blk_rename_at_1172_229,((char * const )(builtin_string_slice1(__str_rename_at_1172_228,int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1182_233),das_auto_cast<void * const >::cast(__pstr_rename_at_1181_232),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1207_237),das_auto_cast<void * const >::cast(__pstr_rename_at_1181_232),1)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_1178_230),cast<char * const >::from(__repl_rename_at_1209_238)));
                    das_copy(__cstr_rename_at_1182_233,__om_rename_at_1207_237);
                } else {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1178_230),int32_t((*(__cstr_rename_at_1182_233))));
                    das_ptr_inc(__cstr_rename_at_1182_233,1);
                };
            };
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void regex_debug_35a427292763c452 ( Context * __context__, regex::Regex const  &  __regex_rename_at_1226_239 )
{
    debug_re_f8fd4cc00f0dc78d(__context__,__regex_rename_at_1226_239.root,1);
    builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __regex_rename_at_1226_239.canEarlyOut )
    {
        builtin_print(((char *) "early out: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_set_13e232e40e242fa0(__context__,__regex_rename_at_1226_239.earlyOut);
        builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( builtin_array_size(__regex_rename_at_1226_239.groups) != 0 )
    {
        builtin_print(((char *) "groups:"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        {
            bool __need_loop_1237 = true;
            // g: tuple<range;string> const&
            das_iterator<TArray<TTuple<16,range,char *>> const > __g_iterator(__regex_rename_at_1226_239.groups);
            TTuple<16,range,char *> const  * __g_rename_at_1237_240;
            __need_loop_1237 = __g_iterator.first(__context__,(__g_rename_at_1237_240)) && __need_loop_1237;
            for ( ; __need_loop_1237 ; __need_loop_1237 = __g_iterator.next(__context__,(__g_rename_at_1237_240)) )
            {
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_12, cast<char * const >::from(((char *) "\t")), cast<char * const >::from(das_get_tuple_field<char *,8>::get((*__g_rename_at_1237_240))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            }
            __g_iterator.close(__context__,(__g_rename_at_1237_240));
        };
    };
}

inline void debug_set_13e232e40e242fa0 ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_1243_241 )
{
    {
        bool __need_loop_1245 = true;
        // x: int const
        das_iterator<range> __x_iterator(range(0,256));
        int32_t __x_rename_at_1245_242;
        __need_loop_1245 = __x_iterator.first(__context__,(__x_rename_at_1245_242)) && __need_loop_1245;
        for ( ; __need_loop_1245 ; __need_loop_1245 = __x_iterator.next(__context__,(__x_rename_at_1245_242)) )
        {
            if ( (__cset_rename_at_1243_241((__x_rename_at_1245_242 >> 5),__context__) & uint32_t(1 << (__x_rename_at_1245_242 & 31))) != 0x0u )
            {
                if ( (__x_rename_at_1245_242 >= 32) && (__x_rename_at_1245_242 <= 127) )
                {
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_13, cast<char * const >::from(((char * const )(to_string_char(__x_rename_at_1245_242,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_14, cast<char * const >::from(((char *) "\\")), cast<int32_t>::from(__x_rename_at_1245_242))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_1245_242));
    };
}

inline void debug_re_f8fd4cc00f0dc78d ( Context * __context__, regex::ReNode * const  __node_rename_at_1256_243, int32_t __tab_rename_at_1256_244 )
{
    builtin_print(((char *) "("),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __node_rename_at_1256_243 != nullptr )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_15, cast<char * const >::from(((char *) "#")), cast<int32_t>::from(__node_rename_at_1256_243->id))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        if ( __node_rename_at_1256_243->next != nullptr )
        {
            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_16, cast<char * const >::from(((char *) "->")), cast<int32_t>::from(__node_rename_at_1256_243->next->id))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            builtin_print(((char *) "->!"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        builtin_print(((char *) " "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( __node_rename_at_1256_243 == nullptr )
    {
        builtin_print(((char *) "null)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Char )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_17, cast<char * const >::from(((char *) "Char `")), cast<char * const >::from(__node_rename_at_1256_243->text), cast<char * const >::from(((char *) "`)")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Set )
    {
        builtin_print(((char *) "Set "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_set_13e232e40e242fa0(__context__,__node_rename_at_1256_243->cset);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Any )
    {
        builtin_print(((char *) "Any)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Eos )
    {
        builtin_print(((char *) "Eos)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Group )
    {
        builtin_print(((char *) "Group "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_243->subexpr,__tab_rename_at_1256_244 + 1);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Plus )
    {
        builtin_print(((char *) "Plus "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_243->subexpr,__tab_rename_at_1256_244);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Star )
    {
        builtin_print(((char *) "Star "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_243->subexpr,__tab_rename_at_1256_244);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Question )
    {
        builtin_print(((char *) "Question "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_243->subexpr,__tab_rename_at_1256_244);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Concat )
    {
        builtin_print(((char *) "Contact"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_18, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_244,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_243->left,__tab_rename_at_1256_244 + 1);
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_19, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_244,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_243->right,__tab_rename_at_1256_244 + 1);
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_20, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_244 - 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ")")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_243->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        builtin_print(((char *) "Union\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        {
            bool __need_loop_1304 = true;
            // sub: regex::ReNode? const&
            das_iterator<TArray<regex::ReNode *> const > __sub_iterator(__node_rename_at_1256_243->all);
            regex::ReNode * const  * __sub_rename_at_1304_245;
            __need_loop_1304 = __sub_iterator.first(__context__,(__sub_rename_at_1304_245)) && __need_loop_1304;
            for ( ; __need_loop_1304 ; __need_loop_1304 = __sub_iterator.next(__context__,(__sub_rename_at_1304_245)) )
            {
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_21, cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_244,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                debug_re_f8fd4cc00f0dc78d(__context__,(*__sub_rename_at_1304_245),__tab_rename_at_1256_244 + 1);
                builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            }
            __sub_iterator.close(__context__,(__sub_rename_at_1304_245));
        };
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_22, cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_244 - 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ")")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        builtin_throw(((char *) "unsupported op"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline int32_t rnd_set_character_6517e452a18c5606 ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_1317_246, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1317_247 )
{
    uint32_t __total_rename_at_1318_248 = ((uint32_t)char_set_total(__cset_rename_at_1317_246));
    uint32_t __idx_rename_at_1319_249 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1317_247),__idx_rename_at_1319_249);
    das_copy(__idx_rename_at_1319_249,SimPolicy<uint32_t>::Mod(__idx_rename_at_1319_249,__total_rename_at_1318_248,*__context__,nullptr));
    return das_auto_cast<int32_t>::cast(char_set_element(int32_t(__idx_rename_at_1319_249),__cset_rename_at_1317_246));
}

inline void re_gen2_fail_a5fdf20930267896 ( Context * __context__, regex::ReNode * __node_rename_at_1325_250, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1325_251, StringBuilderWriter &  __str_rename_at_1325_252 )
{
    DAS_ASSERTF((false),(((char *) "not running generated regex")));
}

inline void re_gen2_char_e6b9f720990cc2d7 ( Context * __context__, regex::ReNode * __node_rename_at_1332_253, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1332_254, StringBuilderWriter &  __str_rename_at_1332_255 )
{
    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_23,cast<StringBuilderWriter &>::from(__str_rename_at_1332_255),cast<char *>::from(__node_rename_at_1332_253->text)));
    regex::ReNode * __node2_rename_at_1337_256 = __node_rename_at_1332_253->next;
    if ( __node2_rename_at_1337_256 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1337_256->gen2,__node2_rename_at_1337_256,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1332_254),das_arg<StringBuilderWriter>::pass(__str_rename_at_1332_255));
    };
}

inline void re_gen2_union_44153b0e184e2888 ( Context * __context__, regex::ReNode * __node_rename_at_1343_257, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1343_258, StringBuilderWriter &  __str_rename_at_1343_259 )
{
    uint32_t __nidx_rename_at_1347_260 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1343_258),__nidx_rename_at_1347_260);
    das_copy(__nidx_rename_at_1347_260,SimPolicy<uint32_t>::Mod(__nidx_rename_at_1347_260,uint32_t(builtin_array_size(das_arg<TArray<regex::ReNode *>>::pass(__node_rename_at_1343_257->all))),*__context__,nullptr));
    regex::ReNode * __sub_rename_at_1350_261 = __node_rename_at_1343_257->all(__nidx_rename_at_1347_260,__context__);
    das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__sub_rename_at_1350_261->gen2,__sub_rename_at_1350_261,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1343_258),das_arg<StringBuilderWriter>::pass(__str_rename_at_1343_259));
}

inline void re_gen2_set_5483397ec36cfff0 ( Context * __context__, regex::ReNode * __node_rename_at_1354_262, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1354_263, StringBuilderWriter &  __str_rename_at_1354_264 )
{
    write_string_char(das_arg<StringBuilderWriter>::pass(__str_rename_at_1354_264),rnd_set_character_6517e452a18c5606(__context__,das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_1354_262->cset),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1354_263)));
    regex::ReNode * __node2_rename_at_1359_265 = __node_rename_at_1354_262->next;
    if ( __node2_rename_at_1359_265 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1359_265->gen2,__node2_rename_at_1359_265,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1354_263),das_arg<StringBuilderWriter>::pass(__str_rename_at_1354_264));
    };
}

inline void re_gen2_any_200931e8e5db950 ( Context * __context__, regex::ReNode * __node_rename_at_1365_266, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1365_267, StringBuilderWriter &  __str_rename_at_1365_268 )
{
    uint32_t __ch_rename_at_1369_269 = 0;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1365_267),__ch_rename_at_1369_269);
    write_string_char(das_arg<StringBuilderWriter>::pass(__str_rename_at_1365_268),int32_t((SimPolicy<uint32_t>::Mod(__ch_rename_at_1369_269,0xffu,*__context__,nullptr)) + 0x1u));
    regex::ReNode * __node2_rename_at_1372_270 = __node_rename_at_1365_266->next;
    if ( __node2_rename_at_1372_270 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1372_270->gen2,__node2_rename_at_1372_270,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1365_267),das_arg<StringBuilderWriter>::pass(__str_rename_at_1365_268));
    };
}

inline void re_gen2_concat_bb7cd4a1d8751a6c ( Context * __context__, regex::ReNode * __node_rename_at_1378_271, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1378_272, StringBuilderWriter &  __str_rename_at_1378_273 )
{
    regex::ReNode * __left_rename_at_1382_274 = __node_rename_at_1378_271->left;
    das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__left_rename_at_1382_274->gen2,__left_rename_at_1382_274,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1378_272),das_arg<StringBuilderWriter>::pass(__str_rename_at_1378_273));
}

inline void re_gen2_eos_6a9fe263836b5d74 ( Context * __context__, regex::ReNode * __node_rename_at_1386_275, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1386_276, StringBuilderWriter &  __str_rename_at_1386_277 )
{
    regex::ReNode * __node2_rename_at_1390_278 = __node_rename_at_1386_275->next;
    if ( __node2_rename_at_1390_278 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1390_278->gen2,__node2_rename_at_1390_278,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1386_276),das_arg<StringBuilderWriter>::pass(__str_rename_at_1386_277));
    };
}

inline void re_gen2_question_f2a2ead57fd4a9 ( Context * __context__, regex::ReNode * __node_rename_at_1396_279, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1396_280, StringBuilderWriter &  __str_rename_at_1396_281 )
{
    uint32_t __a_rename_at_1400_282 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1396_280),__a_rename_at_1400_282);
    if ( (__a_rename_at_1400_282 & 0x1u) != 0x0u )
    {
        regex::ReNode * __nsub_rename_at_1403_283 = __node_rename_at_1396_279->subexpr;
        if ( __nsub_rename_at_1403_283 != nullptr )
        {
            das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_1403_283->gen2,__nsub_rename_at_1403_283,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1396_280),das_arg<StringBuilderWriter>::pass(__str_rename_at_1396_281));
        };
    };
    regex::ReNode * __node2_rename_at_1408_284 = __node_rename_at_1396_279->next;
    if ( __node2_rename_at_1408_284 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1408_284->gen2,__node2_rename_at_1408_284,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1396_280),das_arg<StringBuilderWriter>::pass(__str_rename_at_1396_281));
    };
}

inline void re_gen2_plus_9fe66025caf188a9 ( Context * __context__, regex::ReNode * __node_rename_at_1414_285, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1414_286, StringBuilderWriter &  __str_rename_at_1414_287 )
{
    uint32_t __cnt_rename_at_1418_288 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1414_286),__cnt_rename_at_1418_288);
    das_copy(__cnt_rename_at_1418_288,(SimPolicy<uint32_t>::Mod(__cnt_rename_at_1418_288,das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/,*__context__,nullptr)) + 0x1u);
    regex::ReNode * __nsub_rename_at_1421_289 = __node_rename_at_1414_285->subexpr;
    if ( __nsub_rename_at_1421_289 != nullptr )
    {
        {
            bool __need_loop_1423 = true;
            // i: uint const
            das_iterator<urange> __i_iterator(mk_urange(__cnt_rename_at_1418_288));
            uint32_t __i_rename_at_1423_290;
            __need_loop_1423 = __i_iterator.first(__context__,(__i_rename_at_1423_290)) && __need_loop_1423;
            for ( ; __need_loop_1423 ; __need_loop_1423 = __i_iterator.next(__context__,(__i_rename_at_1423_290)) )
            {
                das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_1421_289->gen2,__nsub_rename_at_1421_289,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1414_286),das_arg<StringBuilderWriter>::pass(__str_rename_at_1414_287));
            }
            __i_iterator.close(__context__,(__i_rename_at_1423_290));
        };
    };
    regex::ReNode * __node2_rename_at_1427_291 = __node_rename_at_1414_285->next;
    if ( __node2_rename_at_1427_291 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1427_291->gen2,__node2_rename_at_1427_291,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1414_286),das_arg<StringBuilderWriter>::pass(__str_rename_at_1414_287));
    };
}

inline void re_gen2_star_5bbabd524a57385c ( Context * __context__, regex::ReNode * __node_rename_at_1433_292, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1433_293, StringBuilderWriter &  __str_rename_at_1433_294 )
{
    uint32_t __cnt_rename_at_1437_295 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1433_293),__cnt_rename_at_1437_295);
    das_copy(__cnt_rename_at_1437_295,SimPolicy<uint32_t>::Mod(__cnt_rename_at_1437_295,das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/,*__context__,nullptr));
    regex::ReNode * __nsub_rename_at_1440_296 = __node_rename_at_1433_292->subexpr;
    if ( __nsub_rename_at_1440_296 != nullptr )
    {
        {
            bool __need_loop_1442 = true;
            // i: uint const
            das_iterator<urange> __i_iterator(mk_urange(__cnt_rename_at_1437_295));
            uint32_t __i_rename_at_1442_297;
            __need_loop_1442 = __i_iterator.first(__context__,(__i_rename_at_1442_297)) && __need_loop_1442;
            for ( ; __need_loop_1442 ; __need_loop_1442 = __i_iterator.next(__context__,(__i_rename_at_1442_297)) )
            {
                das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_1440_296->gen2,__nsub_rename_at_1440_296,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1433_293),das_arg<StringBuilderWriter>::pass(__str_rename_at_1433_294));
            }
            __i_iterator.close(__context__,(__i_rename_at_1442_297));
        };
    };
    regex::ReNode * __node2_rename_at_1446_298 = __node_rename_at_1433_292->next;
    if ( __node2_rename_at_1446_298 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1446_298->gen2,__node2_rename_at_1446_298,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1433_293),das_arg<StringBuilderWriter>::pass(__str_rename_at_1433_294));
    };
}

inline void re_gen2_group_cc75131bc454c003 ( Context * __context__, regex::ReNode * __node_rename_at_1452_299, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1452_300, StringBuilderWriter &  __str_rename_at_1452_301 )
{
    regex::ReNode * __nsub_rename_at_1456_302 = __node_rename_at_1452_299->subexpr;
    if ( __nsub_rename_at_1456_302 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_1456_302->gen2,__nsub_rename_at_1456_302,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1452_300),das_arg<StringBuilderWriter>::pass(__str_rename_at_1452_301));
    };
    regex::ReNode * __node2_rename_at_1460_303 = __node_rename_at_1452_299->next;
    if ( __node2_rename_at_1460_303 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1460_303->gen2,__node2_rename_at_1460_303,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1452_300),das_arg<StringBuilderWriter>::pass(__str_rename_at_1452_301));
    };
}

inline char * re_gen_e78126cc4b992ab9 ( Context * __context__, regex::Regex &  __re_rename_at_1466_304, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1466_305 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1467_306) DAS_AOT_INLINE_LAMBDA -> void{
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__re_rename_at_1466_304.root->gen2,__re_rename_at_1466_304.root,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1466_305),das_arg<StringBuilderWriter>::pass(__writer_rename_at_1467_306));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline regex::ReNode ReNode_a530ee43f17c77ab ( Context * __context__ )
{
    regex::ReNode _temp_make_local_29_6_32; _temp_make_local_29_6_32;
    return /* <- */ das_auto_cast_move<regex::ReNode>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_958929e0b5e39736(__context__,das_arg<regex::ReNode>::pass((([&]() -> regex::ReNode& {
        das_zero(_temp_make_local_29_6_32);
        return _temp_make_local_29_6_32;
    })()))));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xca93cf324e9d39b4] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<regex::ReNode *> & , int32_t  ),&_FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd>>();
    };
    aotLib[0x5138dd10452ae233] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<regex::ReNode *> &  ),&_FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759>>();
    };
    aotLib[0xa251302bc4e8ce68] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, TDim<regex::ReNode *,2> const  &  ),&_FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d>>();
    };
    aotLib[0x198622d4e0a84763] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_958929e0b5e39736
        return ctx.code->makeNode<SimNode_Aot<regex::ReNode & (*) ( Context * __context__, regex::ReNode &  ),&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_958929e0b5e39736>>();
    };
    aotLib[0xf176e87a2566021] = +[](Context & ctx) -> SimNode* { // finalize_1b3fa79e78928d5e
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode &  ),&finalize_1b3fa79e78928d5e>>();
    };
    aotLib[0x2bd7e08861206d71] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_182d4b6f1ba86427
        return ctx.code->makeNode<SimNode_Aot<regex::Regex & (*) ( Context * __context__, regex::Regex &  ),&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_182d4b6f1ba86427>>();
    };
    aotLib[0x5145cf36d3c6239d] = +[](Context & ctx) -> SimNode* { // _FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, int32_t , int32_t  ),&_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec>>();
    };
    aotLib[0x83035a1531032165] = +[](Context & ctx) -> SimNode* { // finalize_4d9dff2c4111fea5
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> &  ),&finalize_4d9dff2c4111fea5>>();
    };
    aotLib[0x1161b07eafefef67] = +[](Context & ctx) -> SimNode* { // finalize_2e3cca8177873ef7
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * &  ),&finalize_2e3cca8177873ef7>>();
    };
    aotLib[0x3c5a74bd28e4f38d] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<regex::ReNode *> & , regex::ReNode *  ),&_FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9>>();
    };
    aotLib[0x3876842f774c9ed8] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6
        return ctx.code->makeNode<SimNode_AotCMRES<TArray<regex::ReNode *> (*) ( Context * __context__, TDim<regex::ReNode *,2> &  ),&_FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6>>();
    };
    aotLib[0x304ae8111350c8ef] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TArray<TTuple<16,range,char *>> & , TTuple<16,range,char *> &  ),&_FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5>>();
    };
    aotLib[0x11fdc599388f444d] = +[](Context & ctx) -> SimNode* { // _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) & , uint32_t &  ),&_FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d>>();
    };
    aotLib[0x828562d5af4c8f1a] = +[](Context & ctx) -> SimNode* { // re_gen_get_rep_limit_aa71779c47fee5b8
        return ctx.code->makeNode<SimNode_Aot<uint32_t (*) ( Context * __context__ ),&re_gen_get_rep_limit_aa71779c47fee5b8>>();
    };
    aotLib[0x7ef31621d0849bdb] = +[](Context & ctx) -> SimNode* { // trace_7794f549fda55dde
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, char * const  , int32_t  ),&trace_7794f549fda55dde>>();
    };
    aotLib[0x7ef31621d0849bdb] = +[](Context & ctx) -> SimNode* { // note_7794f549fda55dde
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, char * const  , int32_t  ),&note_7794f549fda55dde>>();
    };
    aotLib[0x7bfa536a1eb16ea9] = +[](Context & ctx) -> SimNode* { // trace_backtrack_60bdee73e3315732
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__ ),&trace_backtrack_60bdee73e3315732>>();
    };
    aotLib[0xb158d3f3d67fbaf] = +[](Context & ctx) -> SimNode* { // trace_value_599ec0b97b535e0e
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * const   ),&trace_value_599ec0b97b535e0e>>();
    };
    aotLib[0x4b0ef1fd815d7e51] = +[](Context & ctx) -> SimNode* { // nada_a36e274f9f880579
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__ ),&nada_a36e274f9f880579>>();
    };
    aotLib[0x510151b6eeb5cda0] = +[](Context & ctx) -> SimNode* { // maybe_cb698f19184a9bc8
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, regex::ReNode *  ),&maybe_cb698f19184a9bc8>>();
    };
    aotLib[0x9fa23e64f694fd37] = +[](Context & ctx) -> SimNode* { // eos_bcd93149648ad887
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, char * const  , int32_t  ),&eos_bcd93149648ad887>>();
    };
    aotLib[0xd412d4b95b50aa6e] = +[](Context & ctx) -> SimNode* { // at_ac9760d67b4716d0
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, char * const  , int32_t  ),&at_ac9760d67b4716d0>>();
    };
    aotLib[0x21b4c055ddc7e860] = +[](Context & ctx) -> SimNode* { // next_c7b14aa26d681478
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> const  &  ),&next_c7b14aa26d681478>>();
    };
    aotLib[0x79ecf294045c1832] = +[](Context & ctx) -> SimNode* { // is_set_empty_1f052aa7f260c4eb
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, TDim<uint32_t,8> const  &  ),&is_set_empty_1f052aa7f260c4eb>>();
    };
    aotLib[0x33463cf95c238469] = +[](Context & ctx) -> SimNode* { // set_or_char_5d5313390b879abc
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TDim<uint32_t,8> & , int32_t  ),&set_or_char_5d5313390b879abc>>();
    };
    aotLib[0x24d714df9b7b9edb] = +[](Context & ctx) -> SimNode* { // set_or_range_40be02560bdb916d
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TDim<uint32_t,8> & , range  ),&set_or_range_40be02560bdb916d>>();
    };
    aotLib[0x80fffd977ea41e19] = +[](Context & ctx) -> SimNode* { // set_invert_4fb9b64ebf15e148
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TDim<uint32_t,8> &  ),&set_invert_4fb9b64ebf15e148>>();
    };
    aotLib[0xb2ff9bfa5c6f4854] = +[](Context & ctx) -> SimNode* { // set_or_set_f7fe9d5d2a5b194b
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TDim<uint32_t,8> & , TDim<uint32_t,8> const  &  ),&set_or_set_f7fe9d5d2a5b194b>>();
    };
    aotLib[0x6c973866131d87e7] = +[](Context & ctx) -> SimNode* { // set_negative_e65d971bcd9eb5c1
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TDim<uint32_t,8> &  ),&set_negative_e65d971bcd9eb5c1>>();
    };
    aotLib[0x9a6d7de69ba4503f] = +[](Context & ctx) -> SimNode* { // set_meta_4ca7bb80700cac47
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TDim<uint32_t,8> & , int32_t  ),&set_meta_4ca7bb80700cac47>>();
    };
    aotLib[0xc995192cd4fe44d7] = +[](Context & ctx) -> SimNode* { // is_meta_character_5210d144f518e88e
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, int32_t  ),&is_meta_character_5210d144f518e88e>>();
    };
    aotLib[0x9f216299c324b404] = +[](Context & ctx) -> SimNode* { // is_set_character_b3eb8d77360f374d
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, int32_t  ),&is_set_character_b3eb8d77360f374d>>();
    };
    aotLib[0x45a4e0926c0ed921] = +[](Context & ctx) -> SimNode* { // from_hex_34b87ad3a69093f9
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, int32_t  ),&from_hex_34b87ad3a69093f9>>();
    };
    aotLib[0x76ce386522503121] = +[](Context & ctx) -> SimNode* { // string_from_hex_3b7c632d00a40daf
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, int32_t  ),&string_from_hex_3b7c632d00a40daf>>();
    };
    aotLib[0x90f2e0c45cee8d1d] = +[](Context & ctx) -> SimNode* { // string_from_hex_7d459c50a54015ae
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, int32_t , int32_t  ),&string_from_hex_7d459c50a54015ae>>();
    };
    aotLib[0xded3196e8dbb48ac] = +[](Context & ctx) -> SimNode* { // re_char_f30c974269a757d6
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, char * const  , int32_t  ),&re_char_f30c974269a757d6>>();
    };
    aotLib[0xf9005065df4eb9f5] = +[](Context & ctx) -> SimNode* { // re_set_items_24bb39b249ea7fd4
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, char * const  , int32_t  ),&re_set_items_24bb39b249ea7fd4>>();
    };
    aotLib[0x1c2d501cf338856a] = +[](Context & ctx) -> SimNode* { // re_set_a55f6e80806300ed
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, char * const  , int32_t  ),&re_set_a55f6e80806300ed>>();
    };
    aotLib[0xfdcc01f50884bf70] = +[](Context & ctx) -> SimNode* { // re_any_994b96521819a3e
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, char * const  , int32_t  ),&re_any_994b96521819a3e>>();
    };
    aotLib[0x9fbe0b8f3df24162] = +[](Context & ctx) -> SimNode* { // re_eos_12f3534068f58fcd
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, char * const  , int32_t  ),&re_eos_12f3534068f58fcd>>();
    };
    aotLib[0x7b85e7770d273d4a] = +[](Context & ctx) -> SimNode* { // re_group_78c2eacdeb9fefc1
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, char * const  , int32_t  ),&re_group_78c2eacdeb9fefc1>>();
    };
    aotLib[0x4bac1edb2c4c5d87] = +[](Context & ctx) -> SimNode* { // re_elementary_8e517f8be58bf1a6
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, char * const  , int32_t  ),&re_elementary_8e517f8be58bf1a6>>();
    };
    aotLib[0xb462f62f95178a36] = +[](Context & ctx) -> SimNode* { // mk_concat_5c3b6674ba9a8a3f
        return ctx.code->makeNode<SimNode_Aot<regex::ReNode * (*) ( Context * __context__, regex::ReNode * , regex::ReNode *  ),&mk_concat_5c3b6674ba9a8a3f>>();
    };
    aotLib[0x965ecdc5ef52e25c] = +[](Context & ctx) -> SimNode* { // mk_union_cc5d9e9364416c61
        return ctx.code->makeNode<SimNode_Aot<regex::ReNode * (*) ( Context * __context__, regex::ReNode * , regex::ReNode *  ),&mk_union_cc5d9e9364416c61>>();
    };
    aotLib[0x62911f44359f16a7] = +[](Context & ctx) -> SimNode* { // re_re_57e53314b2b816e9
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, char * const  , int32_t  ),&re_re_57e53314b2b816e9>>();
    };
    aotLib[0x8a30956918a5de43] = +[](Context & ctx) -> SimNode* { // re_basic_468d93210132438e
        return ctx.code->makeNode<SimNode_AotCMRES<TVariant<16,8,regex::ReNode *,void *> (*) ( Context * __context__, char * const  , int32_t  ),&re_basic_468d93210132438e>>();
    };
    aotLib[0x5c6ca663915ad6ee] = +[](Context & ctx) -> SimNode* { // re_parse_a11fbde82111aa69
        return ctx.code->makeNode<SimNode_Aot<regex::ReNode * (*) ( Context * __context__, char * const   ),&re_parse_a11fbde82111aa69>>();
    };
    aotLib[0xa7fa09bec9c61bf6] = +[](Context & ctx) -> SimNode* { // visit_top_down_4fecc3dfe95f303c
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Block DAS_COMMENT((void,regex::ReNode *)) const  &  ),&visit_top_down_4fecc3dfe95f303c>>();
    };
    aotLib[0x9bcf4022f918481d] = +[](Context & ctx) -> SimNode* { // re_assign_next_18621bfa0d3dbb85
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::Regex &  ),&re_assign_next_18621bfa0d3dbb85>>();
    };
    aotLib[0xe45fcbc3ca870163] = +[](Context & ctx) -> SimNode* { // re_assign_groups_269827e3d8834c3a
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::Regex &  ),&re_assign_groups_269827e3d8834c3a>>();
    };
    aotLib[0xd563b27477c3547a] = +[](Context & ctx) -> SimNode* { // re_assign_match_functions_d1123942ad08a870
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::Regex &  ),&re_assign_match_functions_d1123942ad08a870>>();
    };
    aotLib[0xf1d002e52d7d8ef] = +[](Context & ctx) -> SimNode* { // re_match2_single_char_efd3120a5ed2dc78
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_single_char_efd3120a5ed2dc78>>();
    };
    aotLib[0xb175b89ea6a4dc28] = +[](Context & ctx) -> SimNode* { // re_match2_char_563cc8699254595c
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_char_563cc8699254595c>>();
    };
    aotLib[0xbb4eaa89af769478] = +[](Context & ctx) -> SimNode* { // re_match2_union_483bf85dbc5196dd
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_union_483bf85dbc5196dd>>();
    };
    aotLib[0x4ee42d56faf1697f] = +[](Context & ctx) -> SimNode* { // re_match2_set_31b09b463a9d7b2b
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_set_31b09b463a9d7b2b>>();
    };
    aotLib[0x202c69489fc0c65] = +[](Context & ctx) -> SimNode* { // re_match2_any_9b00b66687d95b62
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_any_9b00b66687d95b62>>();
    };
    aotLib[0xd470e4b78750568] = +[](Context & ctx) -> SimNode* { // re_match2_concat_2bd31fc7755cdf2a
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_concat_2bd31fc7755cdf2a>>();
    };
    aotLib[0x1c110d5d34042eae] = +[](Context & ctx) -> SimNode* { // re_match2_eos_a35408df9434ba3c
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_eos_a35408df9434ba3c>>();
    };
    aotLib[0x3323e83dade858d] = +[](Context & ctx) -> SimNode* { // re_match2_question_4acdfdc420ce399a
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_question_4acdfdc420ce399a>>();
    };
    aotLib[0x83e1a16da9d3db6d] = +[](Context & ctx) -> SimNode* { // re_match2_plus_adb26cc707eb0c55
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_plus_adb26cc707eb0c55>>();
    };
    aotLib[0xc4c36ef2581ab070] = +[](Context & ctx) -> SimNode* { // re_match2_plus_set_aeb296540242ef22
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_plus_set_aeb296540242ef22>>();
    };
    aotLib[0x423a1c803bc508e8] = +[](Context & ctx) -> SimNode* { // re_match2_star_bb64c06dd606906e
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_star_bb64c06dd606906e>>();
    };
    aotLib[0xf4aefc80782e2e4a] = +[](Context & ctx) -> SimNode* { // re_match2_group_82784eced054ba4e
        return ctx.code->makeNode<SimNode_Aot<uint8_t const  * (*) ( Context * __context__, regex::Regex & , regex::ReNode * , uint8_t const  * const   ),&re_match2_group_82784eced054ba4e>>();
    };
    aotLib[0xbf6074d4a4219957] = +[](Context & ctx) -> SimNode* { // re_early_out_80d62a0fdd14cb06
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, TDim<uint32_t,8> & , regex::ReNode * const   ),&re_early_out_80d62a0fdd14cb06>>();
    };
    aotLib[0xcf9dad636341cfbe] = +[](Context & ctx) -> SimNode* { // is_valid_87c94e9920e8a112
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, regex::Regex &  ),&is_valid_87c94e9920e8a112>>();
    };
    aotLib[0xa3e8d2edcdb89554] = +[](Context & ctx) -> SimNode* { // regex_compile_b802a48fba17c551
        return ctx.code->makeNode<SimNode_Aot<bool (*) ( Context * __context__, regex::Regex & , char * const   ),&regex_compile_b802a48fba17c551>>();
    };
    aotLib[0x5cd35434780ac4b5] = +[](Context & ctx) -> SimNode* { // regex_compile_d14729dab5987744
        return ctx.code->makeNode<SimNode_AotCMRES<regex::Regex (*) ( Context * __context__, char * const   ),&regex_compile_d14729dab5987744>>();
    };
    aotLib[0xacddaf961afe165f] = +[](Context & ctx) -> SimNode* { // regex_compile_8b8cd96a70efcb1c
        return ctx.code->makeNode<SimNode_AotCMRES<regex::Regex (*) ( Context * __context__, regex::Regex &  ),&regex_compile_8b8cd96a70efcb1c>>();
    };
    aotLib[0xc9db0e4c83cf82eb] = +[](Context & ctx) -> SimNode* { // regex_match_d1084a47d6e2d53e
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, regex::Regex & , char * const  , int32_t  ),&regex_match_d1084a47d6e2d53e>>();
    };
    aotLib[0x22a5bb3f1de45f9a] = +[](Context & ctx) -> SimNode* { // regex_group_f150ed72d9c6b909
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, regex::Regex const  & , int32_t , char * const   ),&regex_group_f150ed72d9c6b909>>();
    };
    aotLib[0x6e8c5a82f6bdcd37] = +[](Context & ctx) -> SimNode* { // regex_foreach_7115f79e7b67800f
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::Regex & , char * const  , Block DAS_COMMENT((bool,range)) const  &  ),&regex_foreach_7115f79e7b67800f>>();
    };
    aotLib[0x3c85554b3800d3c] = +[](Context & ctx) -> SimNode* { // regex_replace_6c020ee2b76e6419
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, regex::Regex & , char * const  , Block DAS_COMMENT((char *,char * const )) const  &  ),&regex_replace_6c020ee2b76e6419>>();
    };
    aotLib[0xac3beb34a20a0e94] = +[](Context & ctx) -> SimNode* { // regex_debug_35a427292763c452
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::Regex const  &  ),&regex_debug_35a427292763c452>>();
    };
    aotLib[0x1db4112e373f4e6] = +[](Context & ctx) -> SimNode* { // debug_set_13e232e40e242fa0
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TDim<uint32_t,8> const  &  ),&debug_set_13e232e40e242fa0>>();
    };
    aotLib[0x401a0f2fcc15a153] = +[](Context & ctx) -> SimNode* { // debug_re_f8fd4cc00f0dc78d
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * const  , int32_t  ),&debug_re_f8fd4cc00f0dc78d>>();
    };
    aotLib[0x7d5fe16f6c01de1d] = +[](Context & ctx) -> SimNode* { // rnd_set_character_6517e452a18c5606
        return ctx.code->makeNode<SimNode_Aot<int32_t (*) ( Context * __context__, TDim<uint32_t,8> const  & , Sequence DAS_COMMENT((uint32_t)) &  ),&rnd_set_character_6517e452a18c5606>>();
    };
    aotLib[0x81d54f638e260ba5] = +[](Context & ctx) -> SimNode* { // re_gen2_fail_a5fdf20930267896
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_fail_a5fdf20930267896>>();
    };
    aotLib[0xa21be48addeafc5b] = +[](Context & ctx) -> SimNode* { // re_gen2_char_e6b9f720990cc2d7
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_char_e6b9f720990cc2d7>>();
    };
    aotLib[0xe5ca17f129619abc] = +[](Context & ctx) -> SimNode* { // re_gen2_union_44153b0e184e2888
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_union_44153b0e184e2888>>();
    };
    aotLib[0xfac3fb8e7bac4c7a] = +[](Context & ctx) -> SimNode* { // re_gen2_set_5483397ec36cfff0
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_set_5483397ec36cfff0>>();
    };
    aotLib[0xc5869664f0682804] = +[](Context & ctx) -> SimNode* { // re_gen2_any_200931e8e5db950
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_any_200931e8e5db950>>();
    };
    aotLib[0xf8f639c12b4d60ec] = +[](Context & ctx) -> SimNode* { // re_gen2_concat_bb7cd4a1d8751a6c
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_concat_bb7cd4a1d8751a6c>>();
    };
    aotLib[0x45096c6fa48cb3f2] = +[](Context & ctx) -> SimNode* { // re_gen2_eos_6a9fe263836b5d74
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_eos_6a9fe263836b5d74>>();
    };
    aotLib[0x254fd58ee25615f7] = +[](Context & ctx) -> SimNode* { // re_gen2_question_f2a2ead57fd4a9
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_question_f2a2ead57fd4a9>>();
    };
    aotLib[0x17eec7655fca795d] = +[](Context & ctx) -> SimNode* { // re_gen2_plus_9fe66025caf188a9
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_plus_9fe66025caf188a9>>();
    };
    aotLib[0xfbb2a99f9a013ebb] = +[](Context & ctx) -> SimNode* { // re_gen2_star_5bbabd524a57385c
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_star_5bbabd524a57385c>>();
    };
    aotLib[0xc4bf433f55b01416] = +[](Context & ctx) -> SimNode* { // re_gen2_group_cc75131bc454c003
        return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, regex::ReNode * , Sequence DAS_COMMENT((uint32_t)) & , StringBuilderWriter &  ),&re_gen2_group_cc75131bc454c003>>();
    };
    aotLib[0x7beeb3e353b5340a] = +[](Context & ctx) -> SimNode* { // re_gen_e78126cc4b992ab9
        return ctx.code->makeNode<SimNode_Aot<char * (*) ( Context * __context__, regex::Regex & , Sequence DAS_COMMENT((uint32_t)) &  ),&re_gen_e78126cc4b992ab9>>();
    };
    aotLib[0xe976b8bb7350a101] = +[](Context & ctx) -> SimNode* { // ReNode_a530ee43f17c77ab
        return ctx.code->makeNode<SimNode_AotCMRES<regex::ReNode (*) ( Context * __context__ ),&ReNode_a530ee43f17c77ab>>();
    };
    // [[ init script ]]
    aotLib[0xcdd4468ae6f1930c] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<SimNode_Aot<void (*)(Context *, bool),&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_6191221158849729409
AotListBase impl_aot_regex(_anon_6191221158849729409::registerAotFunctions);
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
