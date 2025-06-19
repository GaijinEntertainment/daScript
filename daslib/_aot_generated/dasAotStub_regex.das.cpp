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
 // require regex

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
namespace _anon_6180131985610406295 {

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
extern StructInfo __struct_info__739e0aa00b63f5bf;
extern StructInfo __struct_info__a442c67b1b45039e;
extern TypeInfo __type_info__6efe4a274318358b;
extern TypeInfo __type_info__d658aa3b467a4710;
extern TypeInfo __type_info__8cbf7e31d94517eb;
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
VarInfo __struct_info__739e0aa00b63f5bf_field_0 =  { Type::tEnumeration, nullptr, &__enum_info__f5eadb7b60b9c74d, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<DAS_COMMENT(enum) regex::ReOp>::size, UINT64_C(0x553347cc024f3fef), "op", offsetof(regex::ReNode,op), 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_1 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x552745cc023ad889), "id", offsetof(regex::ReNode,id), 0 };
TypeInfo * __type_info__26a69ea7fb7ba40e_arg_types_var_8331108043184141759[3] = { &__type_info__21ea97d8ca8ffd5, &__type_info__8cbf7e31d94517eb, &__type_info__1e3f0d9b1ed71149 };
const char * __type_info__26a69ea7fb7ba40e_arg_names_var_8331108043184141759[3] = { "regex", "node", "str" };
VarInfo __struct_info__739e0aa00b63f5bf_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__d658aa3b467a4710, nullptr, (TypeInfo **)__type_info__26a69ea7fb7ba40e_arg_types_var_8331108043184141759, __type_info__26a69ea7fb7ba40e_arg_names_var_8331108043184141759, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((uint8_t const  *,regex::Regex,regex::ReNode *,uint8_t const  * const ))>::size, UINT64_C(0x26a69ea7fb7ba40e), "fun2", offsetof(regex::ReNode,fun2), 0 };
TypeInfo * __type_info__f02e80a7cd1d35eb_arg_types_var_8331108043184141759[3] = { &__type_info__8cbf7e31d94517eb, &__type_info__646c01d58047ed68, &__type_info__37d36026a6078a42 };
const char * __type_info__f02e80a7cd1d35eb_arg_names_var_8331108043184141759[3] = { "node", "rnd", "str" };
VarInfo __struct_info__739e0aa00b63f5bf_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f02e80a7cd1d35eb_arg_types_var_8331108043184141759, __type_info__f02e80a7cd1d35eb_arg_names_var_8331108043184141759, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,regex::ReNode *,Sequence DAS_COMMENT((uint32_t)),StringBuilderWriter))>::size, UINT64_C(0xf02e80a7cd1d35eb), "gen2", offsetof(regex::ReNode,gen2), 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_4 =  { Type::tRange, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<range>::size, UINT64_C(0x55374dcc02561621), "at", offsetof(regex::ReNode,at), 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_5 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xf05d8ca7cd9876b6), "text", offsetof(regex::ReNode,text), 7 };
VarInfo __struct_info__739e0aa00b63f5bf_field_6 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x5fe75b5def0592e0), "textLen", offsetof(regex::ReNode,textLen), 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_7 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__8cbf7e31d94517eb, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<regex::ReNode *>>::size, UINT64_C(0x7df7ea7e12e9cd7), "all", offsetof(regex::ReNode,all), 8 };
VarInfo __struct_info__739e0aa00b63f5bf_field_8 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<regex::ReNode *>::size, UINT64_C(0xf05876a7cd78b90c), "left", offsetof(regex::ReNode,left), 9 };
VarInfo __struct_info__739e0aa00b63f5bf_field_9 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<regex::ReNode *>::size, UINT64_C(0x9be1db3446ee3f1b), "right", offsetof(regex::ReNode,right), 10 };
VarInfo __struct_info__739e0aa00b63f5bf_field_10 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<regex::ReNode *>::size, UINT64_C(0x5f2ead14c0ff50c1), "subexpr", offsetof(regex::ReNode,subexpr), 11 };
VarInfo __struct_info__739e0aa00b63f5bf_field_11 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<regex::ReNode *>::size, UINT64_C(0xf06990a7cd9b8e8c), "next", offsetof(regex::ReNode,next), 14 };
uint32_t __type_info__125ea9a7ea60fe3a_dim_var_8331108043184141759[1] = { 8 };
VarInfo __struct_info__739e0aa00b63f5bf_field_12 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 1, __type_info__125ea9a7ea60fe3a_dim_var_8331108043184141759, 30, TypeSize<TDim<uint32_t,8>>::size, UINT64_C(0x125ea9a7ea60fe3a), "cset", offsetof(regex::ReNode,cset), 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_13 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x4f1b9e3acf383cbd), "index", offsetof(regex::ReNode,index), 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_14 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<uint8_t const  *>::size, UINT64_C(0xe2757da7c183c939), "tail", offsetof(regex::ReNode,tail), 15 };
VarInfo * __struct_info__739e0aa00b63f5bf_fields[15] =  { &__struct_info__739e0aa00b63f5bf_field_0, &__struct_info__739e0aa00b63f5bf_field_1, &__struct_info__739e0aa00b63f5bf_field_2, &__struct_info__739e0aa00b63f5bf_field_3, &__struct_info__739e0aa00b63f5bf_field_4, &__struct_info__739e0aa00b63f5bf_field_5, &__struct_info__739e0aa00b63f5bf_field_6, &__struct_info__739e0aa00b63f5bf_field_7, &__struct_info__739e0aa00b63f5bf_field_8, &__struct_info__739e0aa00b63f5bf_field_9, &__struct_info__739e0aa00b63f5bf_field_10, &__struct_info__739e0aa00b63f5bf_field_11, &__struct_info__739e0aa00b63f5bf_field_12, &__struct_info__739e0aa00b63f5bf_field_13, &__struct_info__739e0aa00b63f5bf_field_14 };
StructInfo __struct_info__739e0aa00b63f5bf = {"ReNode", "regex", 28, __struct_info__739e0aa00b63f5bf_fields, 15, TypeSize<regex::ReNode>::size, UINT64_C(0x0), nullptr, UINT64_C(0x739e0aa00b63f5bf), 5 };
VarInfo __struct_info__a442c67b1b45039e_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<regex::ReNode *>::size, UINT64_C(0x37de19b6affa3dd6), "root", offsetof(regex::Regex,root), 1 };
VarInfo __struct_info__a442c67b1b45039e_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<uint8_t const  *>::size, UINT64_C(0x90ed0cbd06831825), "match", offsetof(regex::Regex,match), 2 };
VarInfo __struct_info__a442c67b1b45039e_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__6efe4a274318358b, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<AutoTuple<range,char *>>>::size, UINT64_C(0x6eee618b86e51341), "groups", offsetof(regex::Regex,groups), 5 };
uint32_t __type_info__aa30cda461a0c553_dim_var_11836241002723869598[1] = { 8 };
VarInfo __struct_info__a442c67b1b45039e_field_3 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 1, __type_info__aa30cda461a0c553_dim_var_11836241002723869598, 30, TypeSize<TDim<uint32_t,8>>::size, UINT64_C(0xaa30cda461a0c553), "earlyOut", offsetof(regex::Regex,earlyOut), 0 };
VarInfo __struct_info__a442c67b1b45039e_field_4 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0x7ce5b1477861b3d1), "canEarlyOut", offsetof(regex::Regex,canEarlyOut), 0 };
VarInfo * __struct_info__a442c67b1b45039e_fields[5] =  { &__struct_info__a442c67b1b45039e_field_0, &__struct_info__a442c67b1b45039e_field_1, &__struct_info__a442c67b1b45039e_field_2, &__struct_info__a442c67b1b45039e_field_3, &__struct_info__a442c67b1b45039e_field_4 };
StructInfo __struct_info__a442c67b1b45039e = {"Regex", "regex", 28, __struct_info__a442c67b1b45039e_fields, 5, TypeSize<regex::Regex>::size, UINT64_C(0x0), nullptr, UINT64_C(0xa442c67b1b45039e), 0 };
VarInfo __func_info__8b6d6c464a360a4f_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<regex::ReNode *>::size, UINT64_C(0xc248c77e17cca11a), "node", 0, 0 };
VarInfo * __func_info__8b6d6c464a360a4f_fields[1] =  { &__func_info__8b6d6c464a360a4f_field_0 };
FuncInfo __func_info__8b6d6c464a360a4f = {"invoke block<(var node:regex::ReNode?):void> const", "", __func_info__8b6d6c464a360a4f_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8b6d6c464a360a4f), 0x0 };
TypeInfo * __type_info__6efe4a274318358b_arg_types[2] = { &__type_info__af63ef4c86020cd5, &__type_info__af63ee4c86020b22 };
TypeInfo __type_info__6efe4a274318358b = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__6efe4a274318358b_arg_types, nullptr, 2, 0, nullptr, 16390, TypeSize<AutoTuple<range,char *>>::size, UINT64_C(0x6efe4a274318358b) };
TypeInfo __type_info__d658aa3b467a4710 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<uint8_t const  *>::size, UINT64_C(0xd658aa3b467a4710) };
TypeInfo __type_info__8cbf7e31d94517eb = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<regex::ReNode *>::size, UINT64_C(0x8cbf7e31d94517eb) };
TypeInfo __type_info__634d658d3d3c73e9 = { Type::tStructure, &__struct_info__a442c67b1b45039e, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<regex::Regex>::size, UINT64_C(0x634d658d3d3c73e9) };
TypeInfo __type_info__1e3f0d9b1ed71149 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, TypeSize<uint8_t const  *>::size, UINT64_C(0x1e3f0d9b1ed71149) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int32_t>::size, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af96fe4c8658cf52 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<uint32_t>::size, UINT64_C(0xaf96fe4c8658cf52) };
TypeInfo __type_info__b661860848e8711e = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<uint8_t>::size, UINT64_C(0xb661860848e8711e) };
TypeInfo __type_info__9c60f9702c7c9014 = { Type::tEnumeration, nullptr, &__enum_info__f5eadb7b60b9c74d, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<DAS_COMMENT(enum) regex::ReOp>::size, UINT64_C(0x9c60f9702c7c9014) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<StringBuilderWriter>::size, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__e68681b2b99a9c9a = { Type::tStructure, &__struct_info__739e0aa00b63f5bf, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<regex::ReNode>::size, UINT64_C(0xe68681b2b99a9c9a) };
TypeInfo __type_info__21ea97d8ca8ffd5 = { Type::tStructure, &__struct_info__a442c67b1b45039e, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<regex::Regex>::size, UINT64_C(0x21ea97d8ca8ffd5) };
TypeInfo __type_info__646c01d58047ed68 = { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e84c860200f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, TypeSize<Sequence DAS_COMMENT((uint32_t))>::size, UINT64_C(0x646c01d58047ed68) };
TypeInfo __type_info__af63ef4c86020cd5 = { Type::tRange, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<range>::size, UINT64_C(0xaf63ef4c86020cd5) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63e84c860200f0 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint32_t>::size, UINT64_C(0xaf63e84c860200f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<void>::size, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__634d658d3d3c73e9 };
TypeInfo * __tinfo_1[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_2[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_3[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_4[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_5[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_6[1] = { &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_7[2] = { &__type_info__af90fe4c864e9d52, &__type_info__9c60f9702c7c9014 };
TypeInfo * __tinfo_8[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_9[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_11[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_12[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_13[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_14[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_15[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_16[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_17[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_18[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_19[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_20[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_22[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_7fc2d92ecf01659d ( Context * __context__, TArray<regex::ReNode *> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_4a49cd9427735830 ( Context * __context__, TArray<regex::ReNode *> & __a_rename_at_1192_2 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_590cb9676400afd6 ( Context * __context__, TDim<regex::ReNode *,2> const  & __a_rename_at_586_4 );
inline void finalize_da2baa75dd8697b6 ( Context * __context__, regex::ReNode & ____this_rename_at_29_5 );
inline regex::Regex & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_f5c2782a98a931e5 ( Context * __context__, regex::Regex & __a_rename_at_50_6 );
inline int32_t _FuncregexTickfrom_hexTick16446213900708532977_b8179530d20c164e ( Context * __context__, int32_t __ch1_rename_at_240_7, int32_t __ch2_rename_at_240_8 );
inline void finalize_52d27e6b6d896c8e ( Context * __context__, AutoVariant<regex::ReNode *,void *> & ____this_rename_at_422_9 );
inline void finalize_8f6d6a28264adb8d ( Context * __context__, regex::ReNode * & ____this_rename_at_519_10 );
inline void _FuncbuiltinTickpushTick10769833213962245646_43fccb5b4c62ad5a ( Context * __context__, TArray<regex::ReNode *> & __Arr_rename_at_181_11, regex::ReNode * __value_rename_at_181_12 );
inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_4c5483eb7641701a ( Context * __context__, TDim<regex::ReNode *,2> & __a_rename_at_1352_13 );
inline void _FuncbuiltinTickpushTick10769833213962245646_af4e82606bed0404 ( Context * __context__, TArray<AutoTuple<range,char *>> & __Arr_rename_at_181_15, AutoTuple<range,char *> & __value_rename_at_181_16 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_5eaabf5f150eca27 ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) & __it_rename_at_1233_17, uint32_t & __value_rename_at_1233_18 );
inline uint32_t re_gen_get_rep_limit_372fed7e922113ce ( Context * __context__ );
inline void trace_cd2ce953d9f02f9a ( Context * __context__, char * const  __msg_rename_at_79_19, int32_t __offset_rename_at_79_20 );
inline void note_25c3af148e31112d ( Context * __context__, char * const  __msg_rename_at_86_21, int32_t __offset_rename_at_86_22 );
inline void trace_backtrack_6e9e1f1de5871abc ( Context * __context__ );
inline void trace_value_11b6d845c20bfb2b ( Context * __context__, regex::ReNode * const  __value_rename_at_99_23 );
inline AutoVariant<regex::ReNode *,void *> nada_21a7a887ebf7be7e ( Context * __context__ );
inline AutoVariant<regex::ReNode *,void *> maybe_93c6ce7837ece2e3 ( Context * __context__, regex::ReNode * __value_rename_at_114_24 );
inline bool eos_9cf46cda98559501 ( Context * __context__, char * const  __expr_rename_at_119_25, int32_t __offset_rename_at_119_26 );
inline int32_t at_fb737fdb637e5537 ( Context * __context__, char * const  __expr_rename_at_123_27, int32_t __offset_rename_at_123_28 );
inline int32_t next_ed15f5916dd1d2e8 ( Context * __context__, AutoVariant<regex::ReNode *,void *> const  & __re_rename_at_142_29 );
inline bool is_set_empty_a9584e784a71b800 ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_150_30 );
inline void set_or_char_73a5f6127d860f5d ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_159_32, int32_t __ch_rename_at_159_33 );
inline void set_or_range_df3e4e666738e1c ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_163_34, range __bits_rename_at_163_35 );
inline void set_invert_e53b0a0fb329873f ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_169_37 );
inline void set_or_set_265482f7b59ff469 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_175_39, TDim<uint32_t,8> const  & __eset_rename_at_175_40 );
inline void set_negative_99e8ff767d43b22b ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_181_45 );
inline void set_meta_6351bcbc951c65a2 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_187_47, int32_t __che_rename_at_187_48 );
inline bool is_meta_character_769c0fb37a614b28 ( Context * __context__, int32_t __ch_rename_at_218_52 );
inline bool is_set_character_9cff1ab92756b9ba ( Context * __context__, int32_t __ch_rename_at_222_53 );
inline int32_t from_hex_3cd126717eb6217c ( Context * __context__, int32_t __ch_rename_at_226_54 );
inline char * string_from_hex_95ab4ebb1457623 ( Context * __context__, int32_t __ch1_rename_at_244_55 );
inline char * string_from_hex_5db9509fd24b3404 ( Context * __context__, int32_t __ch1_rename_at_248_56, int32_t __ch2_rename_at_248_57 );
inline AutoVariant<regex::ReNode *,void *> re_char_60a54157df7133ed ( Context * __context__, char * const  __expr_rename_at_253_58, int32_t __offset_rename_at_253_59 );
inline AutoVariant<regex::ReNode *,void *> re_set_items_c147705b8465a388 ( Context * __context__, char * const  __expr_rename_at_302_66, int32_t __offset_rename_at_302_67 );
inline AutoVariant<regex::ReNode *,void *> re_set_35692c162ac593eb ( Context * __context__, char * const  __expr_rename_at_404_79, int32_t __offset_rename_at_404_80 );
inline AutoVariant<regex::ReNode *,void *> re_any_334bc91044491e9b ( Context * __context__, char * const  __expr_rename_at_438_84, int32_t __offset_rename_at_438_85 );
inline AutoVariant<regex::ReNode *,void *> re_eos_2765a197cbc77325 ( Context * __context__, char * const  __expr_rename_at_450_86, int32_t __offset_rename_at_450_87 );
inline AutoVariant<regex::ReNode *,void *> re_group_5f4507d0a2c4f3ff ( Context * __context__, char * const  __expr_rename_at_462_88, int32_t __offset_rename_at_462_89 );
inline AutoVariant<regex::ReNode *,void *> re_elementary_8ee25ec433d368d4 ( Context * __context__, char * const  __expr_rename_at_482_91, int32_t __offset_rename_at_482_92 );
inline regex::ReNode * mk_concat_3517b4044b6da896 ( Context * __context__, regex::ReNode * __left_rename_at_510_98, regex::ReNode * __right_rename_at_510_99 );
inline regex::ReNode * mk_union_a25f3c5c478c43c7 ( Context * __context__, regex::ReNode * __left_rename_at_538_101, regex::ReNode * __right_rename_at_538_102 );
inline AutoVariant<regex::ReNode *,void *> re_re_dd3df7e732e24abc ( Context * __context__, char * const  __expr_rename_at_565_104, int32_t __offset_rename_at_565_105 );
inline AutoVariant<regex::ReNode *,void *> re_basic_7d05cb7188a92536 ( Context * __context__, char * const  __expr_rename_at_600_110, int32_t __offset_rename_at_600_111 );
inline regex::ReNode * re_parse_9d1fe24c108227a1 ( Context * __context__, char * const  __expr_rename_at_620_114 );
inline void visit_top_down_adf9d2fabd97f41f ( Context * __context__, regex::ReNode * __node_rename_at_633_117, Block DAS_COMMENT((void,regex::ReNode *)) const  & __blk_rename_at_633_118 );
inline void re_assign_next_3161851a13e94542 ( Context * __context__, regex::Regex & __re_rename_at_649_120 );
inline void re_assign_groups_60ccbab514dcc1a1 ( Context * __context__, regex::Regex & __re_rename_at_666_124 );
inline void re_assign_match_functions_d300482f26e4219c ( Context * __context__, regex::Regex & __re_rename_at_681_127 );
inline uint8_t const  * re_match2_single_char_9681a659a9acd2f4 ( Context * __context__, regex::Regex & __regex_rename_at_730_129, regex::ReNode * __node_rename_at_730_130, uint8_t const  * const  __str_rename_at_730_131 );
inline uint8_t const  * re_match2_char_94e78160b779ed44 ( Context * __context__, regex::Regex & __regex_rename_at_753_134, regex::ReNode * __node_rename_at_753_135, uint8_t const  * const  __str_rename_at_753_136 );
inline uint8_t const  * re_match2_union_4751f89c2670f07e ( Context * __context__, regex::Regex & __regex_rename_at_779_139, regex::ReNode * __node_rename_at_779_140, uint8_t const  * const  __str_rename_at_779_141 );
inline uint8_t const  * re_match2_set_ba554535354f0cbe ( Context * __context__, regex::Regex & __regex_rename_at_797_144, regex::ReNode * __node_rename_at_797_145, uint8_t const  * const  __str_rename_at_797_146 );
inline uint8_t const  * re_match2_any_ec3fe998dcc6d0d ( Context * __context__, regex::Regex & __regex_rename_at_820_149, regex::ReNode * __node_rename_at_820_150, uint8_t const  * const  __str_rename_at_820_151 );
inline uint8_t const  * re_match2_concat_70ffb9e9661a77a ( Context * __context__, regex::Regex & __regex_rename_at_840_154, regex::ReNode * __node_rename_at_840_155, uint8_t const  * const  __str_rename_at_840_156 );
inline uint8_t const  * re_match2_eos_f6528a26791746bb ( Context * __context__, regex::Regex & __regex_rename_at_857_159, regex::ReNode * __node_rename_at_857_160, uint8_t const  * const  __str_rename_at_857_161 );
inline uint8_t const  * re_match2_question_822bbf9722705b86 ( Context * __context__, regex::Regex & __regex_rename_at_874_163, regex::ReNode * __node_rename_at_874_164, uint8_t const  * const  __str_rename_at_874_165 );
inline uint8_t const  * re_match2_plus_2bacb281be2d3e3d ( Context * __context__, regex::Regex & __regex_rename_at_893_169, regex::ReNode * __node_rename_at_893_170, uint8_t const  * const  __str_rename_at_893_171 );
inline uint8_t const  * re_match2_plus_set_2c131274cab3d4e7 ( Context * __context__, regex::Regex & __regex_rename_at_928_177, regex::ReNode * __node_rename_at_928_178, uint8_t const  * const  __str_rename_at_928_179 );
inline uint8_t const  * re_match2_star_710d04a0dde8fe1b ( Context * __context__, regex::Regex & __regex_rename_at_971_184, regex::ReNode * __node_rename_at_971_185, uint8_t const  * const  __str_rename_at_971_186 );
inline uint8_t const  * re_match2_group_9a7d00db369a905f ( Context * __context__, regex::Regex & __regex_rename_at_1000_192, regex::ReNode * __node_rename_at_1000_193, uint8_t const  * const  __str_rename_at_1000_194 );
inline bool re_early_out_fe884a05310cc120 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_1020_197, regex::ReNode * const  __node_rename_at_1020_198 );
inline bool is_valid_2be9fd6793561ea5 ( Context * __context__, regex::Regex & __re_rename_at_1063_203 );
inline bool regex_compile_6b2534030df9fde6 ( Context * __context__, regex::Regex & __re_rename_at_1068_204, char * const  __expr_rename_at_1068_205 );
inline regex::Regex regex_compile_36659a6d57325d9f ( Context * __context__, char * const  __expr_rename_at_1082_206 );
inline regex::Regex regex_compile_f2f79bd4c9d9738d ( Context * __context__, regex::Regex & __re_rename_at_1090_208 );
inline int32_t regex_match_5c805b227f49611f ( Context * __context__, regex::Regex & __regex_rename_at_1098_209, char * const  __str_rename_at_1098_210, int32_t __offset_rename_at_1098_211 );
inline char * regex_group_b79e86c102f4f752 ( Context * __context__, regex::Regex const  & __regex_rename_at_1117_213, int32_t __index_rename_at_1117_214, char * const  __match_rename_at_1117_215 );
inline void regex_foreach_5a01d237cb4dd08f ( Context * __context__, regex::Regex & __regex_rename_at_1124_217, char * const  __str_rename_at_1124_218, Block DAS_COMMENT((bool,range)) const  & __blk_rename_at_1124_219 );
inline char * regex_replace_63d1ae99224ff8b8 ( Context * __context__, regex::Regex & __regex_rename_at_1172_226, char * const  __str_rename_at_1172_227, Block DAS_COMMENT((char *,char * const )) const  & __blk_rename_at_1172_228 );
inline void regex_debug_faa0082717395089 ( Context * __context__, regex::Regex const  & __regex_rename_at_1226_238 );
inline void debug_set_b2e50f12da6018dc ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_1243_240 );
inline void debug_re_7f15622d8d8cdbd ( Context * __context__, regex::ReNode * const  __node_rename_at_1256_242, int32_t __tab_rename_at_1256_243 );
inline int32_t rnd_set_character_7d9ac437612f4580 ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_1317_245, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1317_246 );
inline void re_gen2_fail_f4a8fc2e202ea042 ( Context * __context__, regex::ReNode * __node_rename_at_1325_249, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1325_250, StringBuilderWriter & __str_rename_at_1325_251 );
inline void re_gen2_char_b094672da38efcc9 ( Context * __context__, regex::ReNode * __node_rename_at_1332_252, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1332_253, StringBuilderWriter & __str_rename_at_1332_254 );
inline void re_gen2_union_2d880d8ac2104b61 ( Context * __context__, regex::ReNode * __node_rename_at_1343_256, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1343_257, StringBuilderWriter & __str_rename_at_1343_258 );
inline void re_gen2_set_ee117f703604ac9b ( Context * __context__, regex::ReNode * __node_rename_at_1354_261, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1354_262, StringBuilderWriter & __str_rename_at_1354_263 );
inline void re_gen2_any_84c4495ae64f3117 ( Context * __context__, regex::ReNode * __node_rename_at_1365_265, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1365_266, StringBuilderWriter & __str_rename_at_1365_267 );
inline void re_gen2_concat_ceb76f519a092f7b ( Context * __context__, regex::ReNode * __node_rename_at_1378_270, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1378_271, StringBuilderWriter & __str_rename_at_1378_272 );
inline void re_gen2_eos_d426964d1c59cee9 ( Context * __context__, regex::ReNode * __node_rename_at_1386_274, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1386_275, StringBuilderWriter & __str_rename_at_1386_276 );
inline void re_gen2_question_bc45a1b91b690115 ( Context * __context__, regex::ReNode * __node_rename_at_1396_278, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1396_279, StringBuilderWriter & __str_rename_at_1396_280 );
inline void re_gen2_plus_597c4ad99a5fba37 ( Context * __context__, regex::ReNode * __node_rename_at_1414_284, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1414_285, StringBuilderWriter & __str_rename_at_1414_286 );
inline void re_gen2_star_86821b5dcfb13c67 ( Context * __context__, regex::ReNode * __node_rename_at_1433_291, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1433_292, StringBuilderWriter & __str_rename_at_1433_293 );
inline void re_gen2_group_26ba1a1d83dd9e6d ( Context * __context__, regex::ReNode * __node_rename_at_1452_298, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1452_299, StringBuilderWriter & __str_rename_at_1452_300 );
inline char * re_gen_8a63cdad2cc3534 ( Context * __context__, regex::Regex & __re_rename_at_1466_303, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1466_304 );

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

inline void _FuncbuiltinTickresizeTick4811697762258667383_7fc2d92ecf01659d ( Context * __context__, TArray<regex::ReNode *> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<regex::ReNode *>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_4a49cd9427735830 ( Context * __context__, TArray<regex::ReNode *> &  __a_rename_at_1192_2 )
{
    {
        bool __need_loop_1194 = true;
        // aV: regex::ReNode? aka TT&
        das_iterator<TArray<regex::ReNode *>> __aV_iterator(__a_rename_at_1192_2);
        regex::ReNode * * __aV_rename_at_1194_3;
        __need_loop_1194 = __aV_iterator.first(__context__,(__aV_rename_at_1194_3)) && __need_loop_1194;
        for ( ; __need_loop_1194 ; __need_loop_1194 = __aV_iterator.next(__context__,(__aV_rename_at_1194_3)) )
        {
            finalize_8f6d6a28264adb8d(__context__,(*__aV_rename_at_1194_3));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1194_3));
    };
    builtin_array_free(das_arg<TArray<regex::ReNode *>>::pass(__a_rename_at_1192_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_590cb9676400afd6 ( Context * __context__, TDim<regex::ReNode *,2> const  &  __a_rename_at_586_4 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline void finalize_da2baa75dd8697b6 ( Context * __context__, regex::ReNode &  ____this_rename_at_29_5 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_4a49cd9427735830(__context__,das_arg<TArray<regex::ReNode *>>::pass(____this_rename_at_29_5.all));
    finalize_8f6d6a28264adb8d(__context__,____this_rename_at_29_5.left);
    finalize_8f6d6a28264adb8d(__context__,____this_rename_at_29_5.right);
    finalize_8f6d6a28264adb8d(__context__,____this_rename_at_29_5.subexpr);
    memset((void*)&(____this_rename_at_29_5), 0, TypeSize<regex::ReNode>::size);
}

inline regex::Regex & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_f5c2782a98a931e5 ( Context * __context__, regex::Regex &  __a_rename_at_50_6 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<regex::Regex &>::from(__a_rename_at_50_6)));
    return das_auto_cast_ref<regex::Regex &>::cast(__a_rename_at_50_6);
}

inline int32_t _FuncregexTickfrom_hexTick16446213900708532977_b8179530d20c164e ( Context * __context__, int32_t __ch1_rename_at_240_7, int32_t __ch2_rename_at_240_8 )
{
    return das_auto_cast<int32_t>::cast((from_hex_3cd126717eb6217c(__context__,__ch1_rename_at_240_7) * 16) + from_hex_3cd126717eb6217c(__context__,__ch2_rename_at_240_8));
}

inline void finalize_52d27e6b6d896c8e ( Context * __context__, AutoVariant<regex::ReNode *,void *> &  ____this_rename_at_422_9 )
{
    if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(____this_rename_at_422_9) )
    {
        finalize_8f6d6a28264adb8d(__context__,das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::get(____this_rename_at_422_9));
    };
    memset((void*)&(____this_rename_at_422_9), 0, TypeSize<AutoVariant<regex::ReNode *,void *>>::size);
}

inline void finalize_8f6d6a28264adb8d ( Context * __context__, regex::ReNode * & ____this_rename_at_519_10 )
{
    if ( ____this_rename_at_519_10 != nullptr )
    {
        finalize_da2baa75dd8697b6(__context__,das_arg<regex::ReNode>::pass(das_deref(__context__,____this_rename_at_519_10)));
        das_delete<regex::ReNode *>::clear(__context__,____this_rename_at_519_10);
        das_copy(____this_rename_at_519_10,nullptr);
    };
}

inline void _FuncbuiltinTickpushTick10769833213962245646_43fccb5b4c62ad5a ( Context * __context__, TArray<regex::ReNode *> &  __Arr_rename_at_181_11, regex::ReNode * __value_rename_at_181_12 )
{
    das_copy(__Arr_rename_at_181_11(builtin_array_push_back(das_arg<TArray<regex::ReNode *>>::pass(__Arr_rename_at_181_11),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_12);
}

inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_4c5483eb7641701a ( Context * __context__, TDim<regex::ReNode *,2> &  __a_rename_at_1352_13 )
{
    TArray<regex::ReNode *> __arr_rename_at_1354_14;das_zero(__arr_rename_at_1354_14);
    _FuncbuiltinTickresizeTick4811697762258667383_7fc2d92ecf01659d(__context__,das_arg<TArray<regex::ReNode *>>::pass(__arr_rename_at_1354_14),2);
    das_copy(das_cast<TDim<regex::ReNode *,2>>::cast(das_ref(__context__,__arr_rename_at_1354_14(0,__context__))),__a_rename_at_1352_13);
    return /* <- */ das_auto_cast_move<TArray<regex::ReNode *>>::cast(__arr_rename_at_1354_14);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_af4e82606bed0404 ( Context * __context__, TArray<AutoTuple<range,char *>> &  __Arr_rename_at_181_15, AutoTuple<range,char *> &  __value_rename_at_181_16 )
{
    das_copy(__Arr_rename_at_181_15(builtin_array_push_back(das_arg<TArray<AutoTuple<range,char *>>>::pass(__Arr_rename_at_181_15),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_16);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_5eaabf5f150eca27 ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) &  __it_rename_at_1233_17, uint32_t & __value_rename_at_1233_18 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__it_rename_at_1233_17),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1233_18)),__context__));
}

inline uint32_t re_gen_get_rep_limit_372fed7e922113ce ( Context * __context__ )
{
    return das_auto_cast<uint32_t>::cast(das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/);
}

inline void trace_cd2ce953d9f02f9a ( Context * __context__, char * const  __msg_rename_at_79_19, int32_t __offset_rename_at_79_20 )
{
}

inline void note_25c3af148e31112d ( Context * __context__, char * const  __msg_rename_at_86_21, int32_t __offset_rename_at_86_22 )
{
}

inline void trace_backtrack_6e9e1f1de5871abc ( Context * __context__ )
{
}

inline void trace_value_11b6d845c20bfb2b ( Context * __context__, regex::ReNode * const  __value_rename_at_99_23 )
{
}

inline AutoVariant<regex::ReNode *,void *> nada_21a7a887ebf7be7e ( Context * __context__ )
{
    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((([&]() -> AutoVariant<regex::ReNode *,void *> {
        AutoVariant<regex::ReNode *,void *> __mkv_111;
        das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::set(__mkv_111) = nullptr;
        return __mkv_111;
    })()));
}

inline AutoVariant<regex::ReNode *,void *> maybe_93c6ce7837ece2e3 ( Context * __context__, regex::ReNode * __value_rename_at_114_24 )
{
    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((([&]() -> AutoVariant<regex::ReNode *,void *> {
        AutoVariant<regex::ReNode *,void *> __mkv_116;
        das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::set(__mkv_116) = __value_rename_at_114_24;
        return __mkv_116;
    })()));
}

inline bool eos_9cf46cda98559501 ( Context * __context__, char * const  __expr_rename_at_119_25, int32_t __offset_rename_at_119_26 )
{
    return das_auto_cast<bool>::cast(__offset_rename_at_119_26 >= builtin_string_length(__expr_rename_at_119_25,__context__));
}

inline int32_t at_fb737fdb637e5537 ( Context * __context__, char * const  __expr_rename_at_123_27, int32_t __offset_rename_at_123_28 )
{
    return das_auto_cast<int32_t>::cast(get_character_uat(__expr_rename_at_123_27,__offset_rename_at_123_28));
}

inline int32_t next_ed15f5916dd1d2e8 ( Context * __context__, AutoVariant<regex::ReNode *,void *> const  &  __re_rename_at_142_29 )
{
    if ( !das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__re_rename_at_142_29) )
    {
        builtin_stackwalk(true,true,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_throw(((char *) "expecting value"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<int32_t>::cast(v_extract_yi(v_cast_vec4i(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__re_rename_at_142_29,__context__)->at)) /*y*/);
}

inline bool is_set_empty_a9584e784a71b800 ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_150_30 )
{
    {
        bool __need_loop_151 = true;
        // x: uint aka CharSet const&
        das_iterator<TDim<uint32_t,8> const > __x_iterator(__cset_rename_at_150_30);
        uint32_t const  * __x_rename_at_151_31;
        __need_loop_151 = __x_iterator.first(__context__,(__x_rename_at_151_31)) && __need_loop_151;
        for ( ; __need_loop_151 ; __need_loop_151 = __x_iterator.next(__context__,(__x_rename_at_151_31)) )
        {
            if ( (*__x_rename_at_151_31) != 0x0u )
            {
                return das_auto_cast<bool>::cast(false);
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_151_31));
    };
    return das_auto_cast<bool>::cast(true);
}

inline void set_or_char_73a5f6127d860f5d ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_159_32, int32_t __ch_rename_at_159_33 )
{
    __cset_rename_at_159_32((__ch_rename_at_159_33 >> 5),__context__) |= uint32_t(1 << (__ch_rename_at_159_33 & 31));
}

inline void set_or_range_df3e4e666738e1c ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_163_34, range __bits_rename_at_163_35 )
{
    {
        bool __need_loop_164 = true;
        // ch: int const
        das_iterator<range> __ch_iterator(range(v_extract_xi(v_cast_vec4i(__bits_rename_at_163_35)) /*x*/,v_extract_yi(v_cast_vec4i(__bits_rename_at_163_35)) /*y*/ + 1));
        int32_t __ch_rename_at_164_36;
        __need_loop_164 = __ch_iterator.first(__context__,(__ch_rename_at_164_36)) && __need_loop_164;
        for ( ; __need_loop_164 ; __need_loop_164 = __ch_iterator.next(__context__,(__ch_rename_at_164_36)) )
        {
            __cset_rename_at_163_34((__ch_rename_at_164_36 >> 5),__context__) |= uint32_t(1 << (__ch_rename_at_164_36 & 31));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_164_36));
    };
}

inline void set_invert_e53b0a0fb329873f ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_169_37 )
{
    {
        bool __need_loop_170 = true;
        // x: uint aka CharSet& -const
        das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_169_37);
        uint32_t * __x_rename_at_170_38;
        __need_loop_170 = __x_iterator.first(__context__,(__x_rename_at_170_38)) && __need_loop_170;
        for ( ; __need_loop_170 ; __need_loop_170 = __x_iterator.next(__context__,(__x_rename_at_170_38)) )
        {
            (*__x_rename_at_170_38) ^= 0xffffffffu;
        }
        __x_iterator.close(__context__,(__x_rename_at_170_38));
    };
}

inline void set_or_set_265482f7b59ff469 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_175_39, TDim<uint32_t,8> const  &  __eset_rename_at_175_40 )
{
    {
        bool __need_loop_176 = true;
        // x: uint aka CharSet& -const
        das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_175_39);
        uint32_t * __x_rename_at_176_43;
        __need_loop_176 = __x_iterator.first(__context__,(__x_rename_at_176_43)) && __need_loop_176;
        // y: uint aka CharSet const&
        das_iterator<TDim<uint32_t,8> const > __y_iterator(__eset_rename_at_175_40);
        uint32_t const  * __y_rename_at_176_44;
        __need_loop_176 = __y_iterator.first(__context__,(__y_rename_at_176_44)) && __need_loop_176;
        for ( ; __need_loop_176 ; __need_loop_176 = __x_iterator.next(__context__,(__x_rename_at_176_43)) && __y_iterator.next(__context__,(__y_rename_at_176_44)) )
        {
            (*__x_rename_at_176_43) |= (*__y_rename_at_176_44);
        }
        __x_iterator.close(__context__,(__x_rename_at_176_43));
        __y_iterator.close(__context__,(__y_rename_at_176_44));
    };
}

inline void set_negative_99e8ff767d43b22b ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_181_45 )
{
    {
        bool __need_loop_182 = true;
        // x: uint aka CharSet& -const
        das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_181_45);
        uint32_t * __x_rename_at_182_46;
        __need_loop_182 = __x_iterator.first(__context__,(__x_rename_at_182_46)) && __need_loop_182;
        for ( ; __need_loop_182 ; __need_loop_182 = __x_iterator.next(__context__,(__x_rename_at_182_46)) )
        {
            das_copy((*__x_rename_at_182_46),~(*__x_rename_at_182_46));
        }
        __x_iterator.close(__context__,(__x_rename_at_182_46));
    };
}

inline void set_meta_6351bcbc951c65a2 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_187_47, int32_t __che_rename_at_187_48 )
{
    if ( __che_rename_at_187_48 == 119 )
    {
        set_or_range_df3e4e666738e1c(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),range(97,122));
        set_or_range_df3e4e666738e1c(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),range(65,90));
        set_or_range_df3e4e666738e1c(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),range(48,57));
        set_or_char_73a5f6127d860f5d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),95);
    } else if ( __che_rename_at_187_48 == 87 )
    {
        TDim<uint32_t,8> __eset_rename_at_194_49;das_zero(__eset_rename_at_194_49);
        set_meta_6351bcbc951c65a2(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_194_49),119);
        set_or_set_265482f7b59ff469(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_194_49));
    } else if ( __che_rename_at_187_48 == 115 )
    {
        set_or_char_73a5f6127d860f5d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),32);
        set_or_char_73a5f6127d860f5d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),9);
    } else if ( __che_rename_at_187_48 == 83 )
    {
        TDim<uint32_t,8> __eset_rename_at_201_50;das_zero(__eset_rename_at_201_50);
        set_meta_6351bcbc951c65a2(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_201_50),115);
        set_or_set_265482f7b59ff469(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_201_50));
    } else if ( __che_rename_at_187_48 == 100 )
    {
        set_or_range_df3e4e666738e1c(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),range(48,57));
    } else if ( __che_rename_at_187_48 == 68 )
    {
        TDim<uint32_t,8> __eset_rename_at_207_51;das_zero(__eset_rename_at_207_51);
        set_meta_6351bcbc951c65a2(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_207_51),100);
        set_or_set_265482f7b59ff469(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_207_51));
    } else {
        set_or_char_73a5f6127d860f5d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),__che_rename_at_187_48);
    };
}

inline bool is_meta_character_769c0fb37a614b28 ( Context * __context__, int32_t __ch_rename_at_218_52 )
{
    return das_auto_cast<bool>::cast(builtin_find_first_char_of(((char *) "\\+-*.()[]|^"),__ch_rename_at_218_52,__context__) != -1);
}

inline bool is_set_character_9cff1ab92756b9ba ( Context * __context__, int32_t __ch_rename_at_222_53 )
{
    return das_auto_cast<bool>::cast(builtin_find_first_char_of(((char *) "wWsSdD"),__ch_rename_at_222_53,__context__) != -1);
}

inline int32_t from_hex_3cd126717eb6217c ( Context * __context__, int32_t __ch_rename_at_226_54 )
{
    if ( (__ch_rename_at_226_54 >= 48) && (__ch_rename_at_226_54 <= 57) )
    {
        return das_auto_cast<int32_t>::cast(__ch_rename_at_226_54 - 48);
    } else {
        if ( (__ch_rename_at_226_54 >= 97) && (__ch_rename_at_226_54 <= 102) )
        {
            return das_auto_cast<int32_t>::cast((__ch_rename_at_226_54 - 97) + 10);
        } else {
            if ( (__ch_rename_at_226_54 >= 65) && (__ch_rename_at_226_54 <= 70) )
            {
                return das_auto_cast<int32_t>::cast((__ch_rename_at_226_54 - 65) + 10);
            } else {
                builtin_throw(((char *) "from_hex"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                return das_auto_cast<int32_t>::cast(0);
            };
        };
    };
}

inline char * string_from_hex_95ab4ebb1457623 ( Context * __context__, int32_t __ch1_rename_at_244_55 )
{
    return das_auto_cast<char *>::cast(((char * const )(to_string_char(from_hex_3cd126717eb6217c(__context__,__ch1_rename_at_244_55),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * string_from_hex_5db9509fd24b3404 ( Context * __context__, int32_t __ch1_rename_at_248_56, int32_t __ch2_rename_at_248_57 )
{
    return das_auto_cast<char *>::cast(((char * const )(to_string_char(_FuncregexTickfrom_hexTick16446213900708532977_b8179530d20c164e(__context__,__ch1_rename_at_248_56,__ch2_rename_at_248_57),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline AutoVariant<regex::ReNode *,void *> re_char_60a54157df7133ed ( Context * __context__, char * const  __expr_rename_at_253_58, int32_t __offset_rename_at_253_59 )
{
    if ( eos_9cf46cda98559501(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
    } else {
        int32_t __ch_rename_at_258_60 = ((int32_t)at_fb737fdb637e5537(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59));
        if ( __ch_rename_at_258_60 == 92 )
        {
            int32_t __ech_rename_at_260_61 = ((int32_t)at_fb737fdb637e5537(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59 + 1));
            if ( __ech_rename_at_260_61 == 0 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
            } else {
                if ( __ech_rename_at_260_61 == 120 )
                {
                    int32_t __len_rename_at_266_62 = ((int32_t)builtin_string_length(__expr_rename_at_253_58,__context__));
                    if ( (__offset_rename_at_253_59 + 2) >= __len_rename_at_266_62 )
                    {
                        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                    } else {
                        int32_t __hex1_rename_at_271_63 = ((int32_t)at_fb737fdb637e5537(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59 + 2));
                        if ( !is_hex(__hex1_rename_at_271_63) )
                        {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                        } else {
                            if ( (__offset_rename_at_253_59 + 2) < __len_rename_at_266_62 )
                            {
                                int32_t __hex2_rename_at_277_64 = ((int32_t)at_fb737fdb637e5537(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59 + 3));
                                if ( is_hex(__hex2_rename_at_277_64) )
                                {
                                    note_25c3af148e31112d(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_1, cast<char * const >::from(((char *) "hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_271_63,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char * const )(to_string_char(__hex2_rename_at_277_64,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(_FuncregexTickfrom_hexTick16446213900708532977_b8179530d20c164e(__context__,__hex1_rename_at_271_63,__hex2_rename_at_277_64))))),__offset_rename_at_253_59);
                                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_280;
                                        das_zero(__mks_280);
                                        das_copy((__mks_280.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                        das_copy((__mks_280.text),(string_from_hex_5db9509fd24b3404(__context__,__hex1_rename_at_271_63,__hex2_rename_at_277_64)));
                                        das_copy((__mks_280.textLen),(1));
                                        das_copy((__mks_280.at),(range(__offset_rename_at_253_59,__offset_rename_at_253_59 + 3)));
                                        return __mks_280;
                                    })()))));
                                };
                            };
                            note_25c3af148e31112d(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_2, cast<char * const >::from(((char *) "hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_271_63,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_3cd126717eb6217c(__context__,__hex1_rename_at_271_63))))),__offset_rename_at_253_59);
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_284;
                                das_zero(__mks_284);
                                das_copy((__mks_284.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                das_copy((__mks_284.text),(string_from_hex_95ab4ebb1457623(__context__,__hex1_rename_at_271_63)));
                                das_copy((__mks_284.textLen),(1));
                                das_copy((__mks_284.at),(range(__offset_rename_at_253_59,__offset_rename_at_253_59 + 2)));
                                return __mks_284;
                            })()))));
                        };
                    };
                };
                if ( is_set_character_9cff1ab92756b9ba(__context__,__ech_rename_at_260_61) )
                {
                    TDim<uint32_t,8> __cset_rename_at_287_65;das_zero(__cset_rename_at_287_65);
                    set_meta_6351bcbc951c65a2(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_287_65),__ech_rename_at_260_61);
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_289;
                        das_zero(__mks_289);
                        das_copy((__mks_289.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_289.cset),(__cset_rename_at_287_65));
                        das_copy((__mks_289.at),(range(__offset_rename_at_253_59,__offset_rename_at_253_59 + 2)));
                        return __mks_289;
                    })()))));
                } else {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_291;
                        das_zero(__mks_291);
                        das_copy((__mks_291.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_291.text),(((char * const )(to_string_char(__ech_rename_at_260_61,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                        das_copy((__mks_291.textLen),(1));
                        das_copy((__mks_291.at),(range(__offset_rename_at_253_59,__offset_rename_at_253_59 + 2)));
                        return __mks_291;
                    })()))));
                };
            };
        } else if ( is_meta_character_769c0fb37a614b28(__context__,__ch_rename_at_258_60) )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
        };
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_295;
            das_zero(__mks_295);
            das_copy((__mks_295.op),(DAS_COMMENT(enum) regex::ReOp::Char));
            das_copy((__mks_295.text),(((char * const )(to_string_char(__ch_rename_at_258_60,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
            das_copy((__mks_295.textLen),(1));
            das_copy((__mks_295.at),(range(__offset_rename_at_253_59,__offset_rename_at_253_59 + 1)));
            return __mks_295;
        })()))));
    };
}

inline AutoVariant<regex::ReNode *,void *> re_set_items_c147705b8465a388 ( Context * __context__, char * const  __expr_rename_at_302_66, int32_t __offset_rename_at_302_67 )
{
    if ( eos_9cf46cda98559501(__context__,__expr_rename_at_302_66,__offset_rename_at_302_67) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
    } else {
        TDim<uint32_t,8> __cset_rename_at_307_68;das_zero(__cset_rename_at_307_68);
        int32_t __iofs_rename_at_308_69 = __offset_rename_at_302_67;
        int32_t __prev_char_rename_at_309_70 = -1;
        bool __next_range_rename_at_310_71 = false;
        bool __next_meta_rename_at_311_72 = false;
        while ( !eos_9cf46cda98559501(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69) )
        {
            int32_t __ch_rename_at_313_73 = ((int32_t)at_fb737fdb637e5537(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69));
            int32_t __next_char_rename_at_314_74 = -1;
            if ( __ch_rename_at_313_73 == 93 )
            {
                break;
            } else if ( __ch_rename_at_313_73 == 92 )
            {
                int32_t __che_rename_at_319_75 = ((int32_t)at_fb737fdb637e5537(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69 + 1));
                if ( __che_rename_at_319_75 == 0 )
                {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                } else {
                    if ( __che_rename_at_319_75 == 120 )
                    {
                        int32_t __len_rename_at_325_76 = ((int32_t)builtin_string_length(__expr_rename_at_302_66,__context__));
                        if ( (__iofs_rename_at_308_69 + 2) >= __len_rename_at_325_76 )
                        {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                        } else {
                            int32_t __hex1_rename_at_330_77 = ((int32_t)at_fb737fdb637e5537(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69 + 2));
                            if ( !is_hex(__hex1_rename_at_330_77) )
                            {
                                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                            } else {
                                if ( (__iofs_rename_at_308_69 + 2) < __len_rename_at_325_76 )
                                {
                                    int32_t __hex2_rename_at_336_78 = ((int32_t)at_fb737fdb637e5537(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69 + 3));
                                    if ( is_hex(__hex2_rename_at_336_78) )
                                    {
                                        das_copy(__next_char_rename_at_314_74,_FuncregexTickfrom_hexTick16446213900708532977_b8179530d20c164e(__context__,__hex1_rename_at_330_77,__hex2_rename_at_336_78));
                                        note_25c3af148e31112d(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_3, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_330_77,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char * const )(to_string_char(__hex2_rename_at_336_78,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(_FuncregexTickfrom_hexTick16446213900708532977_b8179530d20c164e(__context__,__hex1_rename_at_330_77,__hex2_rename_at_336_78))))),__iofs_rename_at_308_69);
                                        __iofs_rename_at_308_69 += 4;
                                    } else {
                                        das_copy(__next_char_rename_at_314_74,from_hex_3cd126717eb6217c(__context__,__hex1_rename_at_330_77));
                                        note_25c3af148e31112d(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_4, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_330_77,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_3cd126717eb6217c(__context__,__hex1_rename_at_330_77))))),__iofs_rename_at_308_69);
                                        __iofs_rename_at_308_69 += 3;
                                    };
                                } else {
                                    das_copy(__next_char_rename_at_314_74,from_hex_3cd126717eb6217c(__context__,__hex1_rename_at_330_77));
                                    note_25c3af148e31112d(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_5, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_330_77,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_3cd126717eb6217c(__context__,__hex1_rename_at_330_77))))),__offset_rename_at_302_67);
                                    __iofs_rename_at_308_69 += 3;
                                };
                            };
                        };
                    } else {
                        if ( __next_range_rename_at_310_71 )
                        {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                        } else {
                            if ( is_set_character_9cff1ab92756b9ba(__context__,__che_rename_at_319_75) )
                            {
                                das_copy(__next_char_rename_at_314_74,-1);
                                das_copy(__next_meta_rename_at_311_72,true);
                                set_meta_6351bcbc951c65a2(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_307_68),__che_rename_at_319_75);
                            } else {
                                das_copy(__next_char_rename_at_314_74,__che_rename_at_319_75);
                            };
                            __iofs_rename_at_308_69 += 2;
                        };
                    };
                };
            } else if ( __ch_rename_at_313_73 == 45 )
            {
                if ( __prev_char_rename_at_309_70 == -1 )
                {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                } else {
                    das_copy(__next_range_rename_at_310_71,true);
                    __iofs_rename_at_308_69 += 1;
                };
            } else {
                das_copy(__next_char_rename_at_314_74,__ch_rename_at_313_73);
                __iofs_rename_at_308_69 += 1;
            };
            if ( __next_char_rename_at_314_74 != -1 )
            {
                if ( __next_range_rename_at_310_71 )
                {
                    set_or_range_df3e4e666738e1c(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_307_68),range(__prev_char_rename_at_309_70,__next_char_rename_at_314_74));
                    das_copy(__next_range_rename_at_310_71,false);
                    das_copy(__prev_char_rename_at_309_70,-1);
                } else {
                    set_or_char_73a5f6127d860f5d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_307_68),__next_char_rename_at_314_74);
                    das_copy(__prev_char_rename_at_309_70,__next_char_rename_at_314_74);
                };
            } else if ( __next_meta_rename_at_311_72 )
            {
                das_copy(__next_meta_rename_at_311_72,false);
            } else if ( !__next_range_rename_at_310_71 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
            };
        };
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_398;
            das_zero(__mks_398);
            das_copy((__mks_398.op),(DAS_COMMENT(enum) regex::ReOp::Set));
            das_copy((__mks_398.cset),(__cset_rename_at_307_68));
            das_copy((__mks_398.at),(range(__offset_rename_at_302_67,__iofs_rename_at_308_69)));
            return __mks_398;
        })()))));
    };
}

inline AutoVariant<regex::ReNode *,void *> re_set_35692c162ac593eb ( Context * __context__, char * const  __expr_rename_at_404_79, int32_t __offset_rename_at_404_80 )
{
    if ( eos_9cf46cda98559501(__context__,__expr_rename_at_404_79,__offset_rename_at_404_80) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
    } else {
        if ( at_fb737fdb637e5537(__context__,__expr_rename_at_404_79,__offset_rename_at_404_80) != 91 )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
        } else {
            bool __negative_rename_at_412_81 = false;
            if ( at_fb737fdb637e5537(__context__,__expr_rename_at_404_79,__offset_rename_at_404_80 + 1) == 94 )
            {
                das_copy(__negative_rename_at_412_81,true);
            };
            AutoVariant<regex::ReNode *,void *> __oset_rename_at_416_82 = re_set_items_c147705b8465a388(__context__,__expr_rename_at_404_79,__negative_rename_at_412_81 ? das_auto_cast<int32_t>::cast((__offset_rename_at_404_80 + 2)) : das_auto_cast<int32_t>::cast((__offset_rename_at_404_80 + 1)));
            if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__oset_rename_at_416_82) )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
            } else {
                if ( at_fb737fdb637e5537(__context__,__expr_rename_at_404_79,next_ed15f5916dd1d2e8(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oset_rename_at_416_82))) != 93 )
                {
                    finalize_52d27e6b6d896c8e(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oset_rename_at_416_82));
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                } else {
                    regex::ReNode * __oval_rename_at_426_83 = das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oset_rename_at_416_82,__context__);
                    das_copy(das_swizzle_ref<int32_t,range,0>::swizzle(__oval_rename_at_426_83->at) /*x*/,__offset_rename_at_404_80);
                    ++das_swizzle_ref<int32_t,range,1>::swizzle(__oval_rename_at_426_83->at) /*y*/;
                    das_copy(__oval_rename_at_426_83->op,DAS_COMMENT(enum) regex::ReOp::Set);
                    if ( __negative_rename_at_412_81 )
                    {
                        set_negative_99e8ff767d43b22b(__context__,das_arg<TDim<uint32_t,8>>::pass(__oval_rename_at_426_83->cset));
                    };
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oset_rename_at_416_82);
                };
            };
        };
    };
}

inline AutoVariant<regex::ReNode *,void *> re_any_334bc91044491e9b ( Context * __context__, char * const  __expr_rename_at_438_84, int32_t __offset_rename_at_438_85 )
{
    if ( eos_9cf46cda98559501(__context__,__expr_rename_at_438_84,__offset_rename_at_438_85) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
    } else {
        if ( at_fb737fdb637e5537(__context__,__expr_rename_at_438_84,__offset_rename_at_438_85) == 46 )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_444;
                das_zero(__mks_444);
                das_copy((__mks_444.op),(DAS_COMMENT(enum) regex::ReOp::Any));
                das_copy((__mks_444.at),(range(__offset_rename_at_438_85,__offset_rename_at_438_85 + 1)));
                return __mks_444;
            })()))));
        } else {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
        };
    };
}

inline AutoVariant<regex::ReNode *,void *> re_eos_2765a197cbc77325 ( Context * __context__, char * const  __expr_rename_at_450_86, int32_t __offset_rename_at_450_87 )
{
    if ( eos_9cf46cda98559501(__context__,__expr_rename_at_450_86,__offset_rename_at_450_87) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
    } else {
        if ( at_fb737fdb637e5537(__context__,__expr_rename_at_450_86,__offset_rename_at_450_87) == 36 )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_456;
                das_zero(__mks_456);
                das_copy((__mks_456.op),(DAS_COMMENT(enum) regex::ReOp::Eos));
                das_copy((__mks_456.at),(range(__offset_rename_at_450_87,__offset_rename_at_450_87 + 1)));
                return __mks_456;
            })()))));
        } else {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
        };
    };
}

inline AutoVariant<regex::ReNode *,void *> re_group_5f4507d0a2c4f3ff ( Context * __context__, char * const  __expr_rename_at_462_88, int32_t __offset_rename_at_462_89 )
{
    if ( eos_9cf46cda98559501(__context__,__expr_rename_at_462_88,__offset_rename_at_462_89) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
    } else {
        if ( at_fb737fdb637e5537(__context__,__expr_rename_at_462_88,__offset_rename_at_462_89) != 40 )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
        } else {
            AutoVariant<regex::ReNode *,void *> __ore_rename_at_470_90 = re_re_dd3df7e732e24abc(__context__,__expr_rename_at_462_88,__offset_rename_at_462_89 + 1);
            if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__ore_rename_at_470_90) )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
            } else {
                if ( at_fb737fdb637e5537(__context__,__expr_rename_at_462_88,next_ed15f5916dd1d2e8(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__ore_rename_at_470_90))) != 41 )
                {
                    finalize_52d27e6b6d896c8e(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__ore_rename_at_470_90));
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                } else {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_478;
                        das_zero(__mks_478);
                        das_copy((__mks_478.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                        das_copy((__mks_478.subexpr),(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__ore_rename_at_470_90,__context__)));
                        das_copy((__mks_478.at),(range(__offset_rename_at_462_89,next_ed15f5916dd1d2e8(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__ore_rename_at_470_90)) + 1)));
                        return __mks_478;
                    })()))));
                };
            };
        };
    };
}

inline AutoVariant<regex::ReNode *,void *> re_elementary_8ee25ec433d368d4 ( Context * __context__, char * const  __expr_rename_at_482_91, int32_t __offset_rename_at_482_92 )
{
    if ( eos_9cf46cda98559501(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
    } else {
        AutoVariant<regex::ReNode *,void *> __ogr_rename_at_487_93 = re_group_5f4507d0a2c4f3ff(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
        if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__ogr_rename_at_487_93) )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__ogr_rename_at_487_93);
        } else {
            AutoVariant<regex::ReNode *,void *> __oany_rename_at_491_94 = re_any_334bc91044491e9b(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
            if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__oany_rename_at_491_94) )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oany_rename_at_491_94);
            } else {
                AutoVariant<regex::ReNode *,void *> __oeos_rename_at_495_95 = re_eos_2765a197cbc77325(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
                if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__oeos_rename_at_495_95) )
                {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oeos_rename_at_495_95);
                } else {
                    AutoVariant<regex::ReNode *,void *> __oset_rename_at_499_96 = re_set_35692c162ac593eb(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
                    if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__oset_rename_at_499_96) )
                    {
                        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oset_rename_at_499_96);
                    } else {
                        AutoVariant<regex::ReNode *,void *> __ochr_rename_at_503_97 = re_char_60a54157df7133ed(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
                        if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__ochr_rename_at_503_97) )
                        {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__ochr_rename_at_503_97);
                        } else {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                        };
                    };
                };
            };
        };
    };
}

inline regex::ReNode * mk_concat_3517b4044b6da896 ( Context * __context__, regex::ReNode * __left_rename_at_510_98, regex::ReNode * __right_rename_at_510_99 )
{
    if ( __left_rename_at_510_98 == nullptr )
    {
        return das_auto_cast<regex::ReNode *>::cast(__right_rename_at_510_99);
    } else if ( (__left_rename_at_510_98->op == DAS_COMMENT(enum) regex::ReOp::Char) && (__right_rename_at_510_99->op == DAS_COMMENT(enum) regex::ReOp::Char) )
    {
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_510_98->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_510_99->at) /*y*/);
        SimPolicy<char *>::SetAdd((char *)&(__left_rename_at_510_98->text),cast<char *>::from(__right_rename_at_510_99->text),*__context__,nullptr);
        __left_rename_at_510_98->textLen += __right_rename_at_510_99->textLen;
        finalize_8f6d6a28264adb8d(__context__,__right_rename_at_510_99);
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_510_98);
    } else if ( ((__left_rename_at_510_98->op == DAS_COMMENT(enum) regex::ReOp::Concat) && (__left_rename_at_510_98->right->op == DAS_COMMENT(enum) regex::ReOp::Char)) && (__right_rename_at_510_99->op == DAS_COMMENT(enum) regex::ReOp::Char) )
    {
        regex::ReNode * __lor_rename_at_524_100 = __left_rename_at_510_98->right;
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_510_98->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_510_99->at) /*y*/);
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__lor_rename_at_524_100->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_510_99->at) /*y*/);
        SimPolicy<char *>::SetAdd((char *)&(__lor_rename_at_524_100->text),cast<char *>::from(__right_rename_at_510_99->text),*__context__,nullptr);
        __lor_rename_at_524_100->textLen += __right_rename_at_510_99->textLen;
        finalize_8f6d6a28264adb8d(__context__,__right_rename_at_510_99);
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_510_98);
    } else {
        return das_auto_cast<regex::ReNode *>::cast(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_534;
            das_zero(__mks_534);
            das_copy((__mks_534.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_534.left),(__left_rename_at_510_98));
            das_copy((__mks_534.right),(__right_rename_at_510_99));
            das_copy((__mks_534.at),(range(v_extract_xi(v_cast_vec4i(__left_rename_at_510_98->at)) /*x*/,v_extract_yi(v_cast_vec4i(__right_rename_at_510_99->at)) /*y*/)));
            return __mks_534;
        })())));
    };
}

inline regex::ReNode * mk_union_a25f3c5c478c43c7 ( Context * __context__, regex::ReNode * __left_rename_at_538_101, regex::ReNode * __right_rename_at_538_102 )
{
    if ( __left_rename_at_538_101->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_538_101->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_538_102->at) /*y*/);
        if ( __right_rename_at_538_102->op == DAS_COMMENT(enum) regex::ReOp::Union )
        {
            {
                bool __need_loop_542 = true;
                // x: regex::ReNode?&
                das_iterator<TArray<regex::ReNode *>> __x_iterator(__right_rename_at_538_102->all);
                regex::ReNode * * __x_rename_at_542_103;
                __need_loop_542 = __x_iterator.first(__context__,(__x_rename_at_542_103)) && __need_loop_542;
                for ( ; __need_loop_542 ; __need_loop_542 = __x_iterator.next(__context__,(__x_rename_at_542_103)) )
                {
                    _FuncbuiltinTickpushTick10769833213962245646_43fccb5b4c62ad5a(__context__,das_arg<TArray<regex::ReNode *>>::pass(__left_rename_at_538_101->all),(*__x_rename_at_542_103));
                }
                __x_iterator.close(__context__,(__x_rename_at_542_103));
            };
            finalize_8f6d6a28264adb8d(__context__,__right_rename_at_538_102);
        } else {
            _FuncbuiltinTickpushTick10769833213962245646_43fccb5b4c62ad5a(__context__,das_arg<TArray<regex::ReNode *>>::pass(__left_rename_at_538_101->all),__right_rename_at_538_102);
        };
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_538_101);
    } else if ( __right_rename_at_538_102->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        das_copy(das_swizzle_ref<int32_t,range,0>::swizzle(__right_rename_at_538_102->at) /*x*/,das_swizzle_ref<int32_t,range,0>::swizzle(__left_rename_at_538_101->at) /*x*/);
        _FuncbuiltinTickpushTick10769833213962245646_43fccb5b4c62ad5a(__context__,das_arg<TArray<regex::ReNode *>>::pass(__right_rename_at_538_102->all),__left_rename_at_538_101);
        return das_auto_cast<regex::ReNode *>::cast(__right_rename_at_538_102);
    } else {
        TDim<regex::ReNode *,2> _temp_make_local_557_50_0; _temp_make_local_557_50_0;
        return das_auto_cast<regex::ReNode *>::cast(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_557;
            das_zero(__mks_557);
            das_copy((__mks_557.op),(DAS_COMMENT(enum) regex::ReOp::Union));
            das_move((__mks_557.all),(_FuncbuiltinTickto_array_moveTick3185538323411982277_4c5483eb7641701a(__context__,das_arg<TDim<regex::ReNode *,2>>::pass((([&]() -> TDim<regex::ReNode *,2>& {
                _temp_make_local_557_50_0(0,__context__) = __left_rename_at_538_101;
                _temp_make_local_557_50_0(1,__context__) = __right_rename_at_538_102;
                return _temp_make_local_557_50_0;
            })())))));
            das_copy((__mks_557.at),(range(v_extract_xi(v_cast_vec4i(__left_rename_at_538_101->at)) /*x*/,v_extract_yi(v_cast_vec4i(__right_rename_at_538_102->at)) /*y*/)));
            return __mks_557;
        })())));
    };
}

inline AutoVariant<regex::ReNode *,void *> re_re_dd3df7e732e24abc ( Context * __context__, char * const  __expr_rename_at_565_104, int32_t __offset_rename_at_565_105 )
{
    if ( eos_9cf46cda98559501(__context__,__expr_rename_at_565_104,__offset_rename_at_565_105) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
    } else {
        int32_t __cofs_rename_at_570_106 = __offset_rename_at_565_105;
        regex::ReNode * __last_rename_at_571_107 = 0;
        while ( !eos_9cf46cda98559501(__context__,__expr_rename_at_565_104,__cofs_rename_at_570_106) )
        {
            if ( at_fb737fdb637e5537(__context__,__expr_rename_at_565_104,__cofs_rename_at_570_106) == 124 )
            {
                if ( __last_rename_at_571_107 == nullptr )
                {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                } else {
                    AutoVariant<regex::ReNode *,void *> __oright_rename_at_578_108 = re_re_dd3df7e732e24abc(__context__,__expr_rename_at_565_104,__cofs_rename_at_570_106 + 1);
                    if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__oright_rename_at_578_108) )
                    {
                        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
                    } else {
                        das_copy(__last_rename_at_571_107,mk_union_a25f3c5c478c43c7(__context__,__last_rename_at_571_107,das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oright_rename_at_578_108,__context__)));
                        das_copy(__cofs_rename_at_570_106,das_swizzle_ref<int32_t,range,1>::swizzle(__last_rename_at_571_107->at) /*y*/);
                    };
                };
            } else {
                AutoVariant<regex::ReNode *,void *> __onext_rename_at_586_109 = re_basic_7d05cb7188a92536(__context__,__expr_rename_at_565_104,__cofs_rename_at_570_106);
                if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__onext_rename_at_586_109) )
                {
                    AutoVariant<regex::ReNode *,void *> _temp_make_local_588_38_1; _temp_make_local_588_38_1;
                    AutoVariant<regex::ReNode *,void *> _temp_make_local_588_47_2; _temp_make_local_588_47_2;
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((__last_rename_at_571_107 == nullptr) ? das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((_temp_make_local_588_38_1 = (nada_21a7a887ebf7be7e(__context__)))) : das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((_temp_make_local_588_47_2 = (maybe_93c6ce7837ece2e3(__context__,__last_rename_at_571_107)))));
                } else {
                    das_copy(__last_rename_at_571_107,mk_concat_3517b4044b6da896(__context__,__last_rename_at_571_107,das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__onext_rename_at_586_109,__context__)));
                    das_copy(__cofs_rename_at_570_106,das_swizzle_ref<int32_t,range,1>::swizzle(__last_rename_at_571_107->at) /*y*/);
                };
            };
        };
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,__last_rename_at_571_107));
    };
}

inline AutoVariant<regex::ReNode *,void *> re_basic_7d05cb7188a92536 ( Context * __context__, char * const  __expr_rename_at_600_110, int32_t __offset_rename_at_600_111 )
{
    if ( eos_9cf46cda98559501(__context__,__expr_rename_at_600_110,__offset_rename_at_600_111) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
    } else {
        AutoVariant<regex::ReNode *,void *> __oelem_rename_at_605_112 = re_elementary_8ee25ec433d368d4(__context__,__expr_rename_at_600_110,__offset_rename_at_600_111);
        if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__oelem_rename_at_605_112) )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_21a7a887ebf7be7e(__context__));
        } else {
            int32_t __ch_rename_at_609_113 = ((int32_t)at_fb737fdb637e5537(__context__,__expr_rename_at_600_110,next_ed15f5916dd1d2e8(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oelem_rename_at_605_112))));
            if ( __ch_rename_at_609_113 == 42 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_611;
                    das_zero(__mks_611);
                    das_copy((__mks_611.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                    das_copy((__mks_611.subexpr),(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oelem_rename_at_605_112,__context__)));
                    das_copy((__mks_611.at),(range(__offset_rename_at_600_111,next_ed15f5916dd1d2e8(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oelem_rename_at_605_112)) + 1)));
                    return __mks_611;
                })()))));
            } else if ( __ch_rename_at_609_113 == 43 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_613;
                    das_zero(__mks_613);
                    das_copy((__mks_613.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                    das_copy((__mks_613.subexpr),(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oelem_rename_at_605_112,__context__)));
                    das_copy((__mks_613.at),(range(__offset_rename_at_600_111,next_ed15f5916dd1d2e8(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oelem_rename_at_605_112)) + 1)));
                    return __mks_613;
                })()))));
            } else if ( __ch_rename_at_609_113 == 63 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_93c6ce7837ece2e3(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_615;
                    das_zero(__mks_615);
                    das_copy((__mks_615.op),(DAS_COMMENT(enum) regex::ReOp::Question));
                    das_copy((__mks_615.subexpr),(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oelem_rename_at_605_112,__context__)));
                    das_copy((__mks_615.at),(range(__offset_rename_at_600_111,next_ed15f5916dd1d2e8(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oelem_rename_at_605_112)) + 1)));
                    return __mks_615;
                })()))));
            };
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oelem_rename_at_605_112);
        };
    };
}

inline regex::ReNode * re_parse_9d1fe24c108227a1 ( Context * __context__, char * const  __expr_rename_at_620_114 )
{
    AutoVariant<regex::ReNode *,void *> __ore_rename_at_621_115 = re_re_dd3df7e732e24abc(__context__,__expr_rename_at_620_114,0);
    if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__ore_rename_at_621_115) )
    {
        return das_auto_cast<regex::ReNode *>::cast(nullptr);
    } else {
        int32_t __at_rename_at_625_116 = ((int32_t)next_ed15f5916dd1d2e8(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__ore_rename_at_621_115)));
        return das_auto_cast<regex::ReNode *>::cast((__at_rename_at_625_116 != builtin_string_length(__expr_rename_at_620_114,__context__)) ? das_auto_cast<void *>::cast(nullptr) : das_auto_cast<void *>::cast(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__ore_rename_at_621_115,__context__)));
    };
}

inline void visit_top_down_adf9d2fabd97f41f ( Context * __context__, regex::ReNode * __node_rename_at_633_117, Block DAS_COMMENT((void,regex::ReNode *)) const  &  __blk_rename_at_633_118 )
{
    das_invoke<void>::invoke<regex::ReNode *>(__context__,nullptr,__blk_rename_at_633_118,__node_rename_at_633_117);
    {
        bool __need_loop_635 = true;
        // x: regex::ReNode?&
        das_iterator<TArray<regex::ReNode *>> __x_iterator(__node_rename_at_633_117->all);
        regex::ReNode * * __x_rename_at_635_119;
        __need_loop_635 = __x_iterator.first(__context__,(__x_rename_at_635_119)) && __need_loop_635;
        for ( ; __need_loop_635 ; __need_loop_635 = __x_iterator.next(__context__,(__x_rename_at_635_119)) )
        {
            visit_top_down_adf9d2fabd97f41f(__context__,(*__x_rename_at_635_119),__blk_rename_at_633_118);
        }
        __x_iterator.close(__context__,(__x_rename_at_635_119));
    };
    if ( __node_rename_at_633_117->subexpr != nullptr )
    {
        visit_top_down_adf9d2fabd97f41f(__context__,__node_rename_at_633_117->subexpr,__blk_rename_at_633_118);
    };
    if ( __node_rename_at_633_117->left != nullptr )
    {
        visit_top_down_adf9d2fabd97f41f(__context__,__node_rename_at_633_117->left,__blk_rename_at_633_118);
    };
    if ( __node_rename_at_633_117->right != nullptr )
    {
        visit_top_down_adf9d2fabd97f41f(__context__,__node_rename_at_633_117->right,__blk_rename_at_633_118);
    };
}

inline void re_assign_next_3161851a13e94542 ( Context * __context__, regex::Regex &  __re_rename_at_649_120 ) { das_stack_prologue __prologue(__context__,128,"re_assign_next " DAS_FILE_LINE);
{
    int32_t __id_rename_at_650_121 = 0;
    visit_top_down_adf9d2fabd97f41f(__context__,__re_rename_at_649_120.root,das_make_block<void,regex::ReNode *>(__context__,96,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_651_122) -> void{
        das_copy(__node_rename_at_651_122->id,__id_rename_at_650_121++);
        if ( __node_rename_at_651_122->op == DAS_COMMENT(enum) regex::ReOp::Concat )
        {
            das_copy(__node_rename_at_651_122->left->next,__node_rename_at_651_122->right);
            das_copy(__node_rename_at_651_122->right->next,__node_rename_at_651_122->next);
        } else if ( __node_rename_at_651_122->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            das_copy(__node_rename_at_651_122->subexpr->next,__node_rename_at_651_122->next);
        } else if ( __node_rename_at_651_122->op == DAS_COMMENT(enum) regex::ReOp::Union )
        {
            {
                bool __need_loop_659 = true;
                // sub: regex::ReNode?&
                das_iterator<TArray<regex::ReNode *>> __sub_iterator(__node_rename_at_651_122->all);
                regex::ReNode * * __sub_rename_at_659_123;
                __need_loop_659 = __sub_iterator.first(__context__,(__sub_rename_at_659_123)) && __need_loop_659;
                for ( ; __need_loop_659 ; __need_loop_659 = __sub_iterator.next(__context__,(__sub_rename_at_659_123)) )
                {
                    das_copy((*__sub_rename_at_659_123)->next,__node_rename_at_651_122->next);
                }
                __sub_iterator.close(__context__,(__sub_rename_at_659_123));
            };
        };
    }));
}}

inline void re_assign_groups_60ccbab514dcc1a1 ( Context * __context__, regex::Regex &  __re_rename_at_666_124 ) { das_stack_prologue __prologue(__context__,144,"re_assign_groups " DAS_FILE_LINE);
{
    AutoTuple<range,char *> _temp_make_local_667_21_3; _temp_make_local_667_21_3;
    _FuncbuiltinTickpushTick10769833213962245646_af4e82606bed0404(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(__re_rename_at_666_124.groups),das_arg<AutoTuple<range,char *>>::pass((([&]() -> AutoTuple<range,char *>& {
        das_get_auto_tuple_field<range,0,range,char *>::get(_temp_make_local_667_21_3) = range(0,0);
        das_get_auto_tuple_field<char *,1,range,char *>::get(_temp_make_local_667_21_3) = nullptr;
        return _temp_make_local_667_21_3;
    })())));
    visit_top_down_adf9d2fabd97f41f(__context__,__re_rename_at_666_124.root,das_make_block<void,regex::ReNode *>(__context__,96,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_668_125) -> void{
        if ( __node_rename_at_668_125->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            AutoTuple<range,char *> _temp_make_local_672_29_4; _temp_make_local_672_29_4;
            int32_t __index_rename_at_670_126 = ((int32_t)builtin_array_size(das_arg<TArray<AutoTuple<range,char *>>>::pass(__re_rename_at_666_124.groups)));
            das_copy(__node_rename_at_668_125->index,__index_rename_at_670_126);
            _FuncbuiltinTickpushTick10769833213962245646_af4e82606bed0404(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(__re_rename_at_666_124.groups),das_arg<AutoTuple<range,char *>>::pass((([&]() -> AutoTuple<range,char *>& {
                das_get_auto_tuple_field<range,0,range,char *>::get(_temp_make_local_672_29_4) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(_temp_make_local_672_29_4) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_6, cast<int32_t>::from(__index_rename_at_670_126)));
                return _temp_make_local_672_29_4;
            })())));
        };
    }));
}}

inline void re_assign_match_functions_d300482f26e4219c ( Context * __context__, regex::Regex &  __re_rename_at_681_127 ) { das_stack_prologue __prologue(__context__,96,"re_assign_match_functions " DAS_FILE_LINE);
{
    visit_top_down_adf9d2fabd97f41f(__context__,__re_rename_at_681_127.root,das_make_block<void,regex::ReNode *>(__context__,80,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_682_128) -> void{
        if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Char )
        {
            if ( __node_rename_at_682_128->textLen == 1 )
            {
                das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_single_char S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x5c2840fada5f29d9)));
            } else {
                das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_char S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x38bd89252c0cf248)));
            };
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_char 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x2eeba7ce095d9afa)));
        } else if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Union )
        {
            das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_union S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x1ef4abfda5a2bd43)));
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_union 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xc4c5ecee47ca5080)));
        } else if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Set )
        {
            das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_set S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x2a0f5e124643a3e2)));
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_set 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x1b17e13417657e5)));
        } else if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Any )
        {
            das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_any S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xf452d2698cd903bd)));
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_any 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x546ffa9a67c5ab20)));
        } else if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Eos )
        {
            das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_eos S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xb2eb31b5d89f9bf3)));
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_eos 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x7d2fd4e5b91438c2)));
        } else if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Concat )
        {
            das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_concat S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x9a436a387386d91c)));
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_concat 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xdc10b0f0cf88a190)));
        } else if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Plus )
        {
            if ( __node_rename_at_682_128->subexpr->op == DAS_COMMENT(enum) regex::ReOp::Set )
            {
                das_copy(__node_rename_at_682_128->cset,__node_rename_at_682_128->subexpr->cset);
                das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_plus_set S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x5d785a6dbbd43669)));
            } else {
                das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_plus S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x99599148d7cde0f9)));
            };
            das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_plus S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x99599148d7cde0f9)));
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_plus 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x5265717f53964fab)));
        } else if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Star )
        {
            das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_star S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xcd6744f0c5dc4718)));
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_star 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xcb3cfcadf09dcf2a)));
        } else if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Question )
        {
            das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_question S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xefb1f4e676a32f71)));
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_question 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xe44a011a44687c83)));
        } else if ( __node_rename_at_682_128->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            das_copy(__node_rename_at_682_128->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_group S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x886503ec9847cc49)));
            das_copy(__node_rename_at_682_128->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_group 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xc4e23520b76179aa)));
        } else {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_7, cast<char * const >::from(((char *) "unsupported ")), cast<DAS_COMMENT(enum) regex::ReOp>::from(__node_rename_at_682_128->op))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
    }));
}}

inline uint8_t const  * re_match2_single_char_9681a659a9acd2f4 ( Context * __context__, regex::Regex &  __regex_rename_at_730_129, regex::ReNode * __node_rename_at_730_130, uint8_t const  * const  __str_rename_at_730_131 )
{
    if ( das_equ_val((*(__str_rename_at_730_131)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( get_character_uat(__node_rename_at_730_130->text,0) != int32_t((*(__str_rename_at_730_131))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_741_132 = das_ptr_add_int32(__str_rename_at_730_131,1,1);
            das_copy(__node_rename_at_730_130->tail,__tail_rename_at_741_132);
            regex::ReNode * __node2_rename_at_743_133 = __node_rename_at_730_130->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_743_133 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_743_133->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_730_129),__node2_rename_at_743_133,__tail_rename_at_741_132)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_741_132));
        };
    };
}

inline uint8_t const  * re_match2_char_94e78160b779ed44 ( Context * __context__, regex::Regex &  __regex_rename_at_753_134, regex::ReNode * __node_rename_at_753_135, uint8_t const  * const  __str_rename_at_753_136 )
{
    if ( das_equ_val((*(__str_rename_at_753_136)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( das_memcmp(das_auto_cast<void * const >::cast(das_cast<uint8_t *>::cast(__node_rename_at_753_135->text)),das_auto_cast<void * const >::cast(__str_rename_at_753_136),__node_rename_at_753_135->textLen) != 0 )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_764_137 = das_ptr_add_int32(__str_rename_at_753_136,__node_rename_at_753_135->textLen,1);
            das_copy(__node_rename_at_753_135->tail,__tail_rename_at_764_137);
            regex::ReNode * __node2_rename_at_766_138 = __node_rename_at_753_135->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_766_138 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_766_138->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_753_134),__node2_rename_at_766_138,__tail_rename_at_764_137)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_764_137));
        };
    };
}

inline uint8_t const  * re_match2_union_4751f89c2670f07e ( Context * __context__, regex::Regex &  __regex_rename_at_779_139, regex::ReNode * __node_rename_at_779_140, uint8_t const  * const  __str_rename_at_779_141 )
{
    if ( das_equ_val((*(__str_rename_at_779_141)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        {
            bool __need_loop_786 = true;
            // sub: regex::ReNode?&
            das_iterator<TArray<regex::ReNode *>> __sub_iterator(__node_rename_at_779_140->all);
            regex::ReNode * * __sub_rename_at_786_142;
            __need_loop_786 = __sub_iterator.first(__context__,(__sub_rename_at_786_142)) && __need_loop_786;
            for ( ; __need_loop_786 ; __need_loop_786 = __sub_iterator.next(__context__,(__sub_rename_at_786_142)) )
            {
                uint8_t const  * __osub_rename_at_787_143 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,(*__sub_rename_at_786_142)->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_779_139),(*__sub_rename_at_786_142),__str_rename_at_779_141));
                if ( __osub_rename_at_787_143 != nullptr )
                {
                    das_copy(__node_rename_at_779_140->tail,(*__sub_rename_at_786_142)->tail);
                    return das_auto_cast<uint8_t const  *>::cast(__osub_rename_at_787_143);
                };
            }
            __sub_iterator.close(__context__,(__sub_rename_at_786_142));
        };
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    };
}

inline uint8_t const  * re_match2_set_ba554535354f0cbe ( Context * __context__, regex::Regex &  __regex_rename_at_797_144, regex::ReNode * __node_rename_at_797_145, uint8_t const  * const  __str_rename_at_797_146 )
{
    if ( das_equ_val((*(__str_rename_at_797_146)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( !is_char_in_set(int32_t((*(__str_rename_at_797_146))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_797_145->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_808_147 = das_ptr_add_int32(__str_rename_at_797_146,1,1);
            das_copy(__node_rename_at_797_145->tail,__tail_rename_at_808_147);
            regex::ReNode * __node2_rename_at_810_148 = __node_rename_at_797_145->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_810_148 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_810_148->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_797_144),__node2_rename_at_810_148,__tail_rename_at_808_147)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_808_147));
        };
    };
}

inline uint8_t const  * re_match2_any_ec3fe998dcc6d0d ( Context * __context__, regex::Regex &  __regex_rename_at_820_149, regex::ReNode * __node_rename_at_820_150, uint8_t const  * const  __str_rename_at_820_151 )
{
    if ( das_equ_val((*(__str_rename_at_820_151)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        uint8_t const  * __tail_rename_at_828_152 = das_ptr_add_int32(__str_rename_at_820_151,1,1);
        das_copy(__node_rename_at_820_150->tail,__tail_rename_at_828_152);
        regex::ReNode * __node2_rename_at_830_153 = __node_rename_at_820_150->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_830_153 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_830_153->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_820_149),__node2_rename_at_830_153,__tail_rename_at_828_152)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_828_152));
    };
}

inline uint8_t const  * re_match2_concat_70ffb9e9661a77a ( Context * __context__, regex::Regex &  __regex_rename_at_840_154, regex::ReNode * __node_rename_at_840_155, uint8_t const  * const  __str_rename_at_840_156 )
{
    if ( das_equ_val((*(__str_rename_at_840_156)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        regex::ReNode * __left_rename_at_847_157 = __node_rename_at_840_155->left;
        uint8_t const  * __oleft_rename_at_848_158 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__left_rename_at_847_157->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_840_154),__left_rename_at_847_157,__str_rename_at_840_156));
        if ( __oleft_rename_at_848_158 == nullptr )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            das_copy(__node_rename_at_840_155->tail,__node_rename_at_840_155->right->tail);
            return das_auto_cast<uint8_t const  *>::cast(__oleft_rename_at_848_158);
        };
    };
}

inline uint8_t const  * re_match2_eos_f6528a26791746bb ( Context * __context__, regex::Regex &  __regex_rename_at_857_159, regex::ReNode * __node_rename_at_857_160, uint8_t const  * const  __str_rename_at_857_161 )
{
    if ( das_nequ_val((*(__str_rename_at_857_161)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_857_160->tail,__str_rename_at_857_161);
        regex::ReNode * __node2_rename_at_865_162 = __node_rename_at_857_160->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_865_162 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__node2_rename_at_865_162->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_857_159),__node2_rename_at_865_162,__str_rename_at_857_161)) : das_auto_cast<uint8_t const  *>::cast(__str_rename_at_857_161));
    };
}

inline uint8_t const  * re_match2_question_822bbf9722705b86 ( Context * __context__, regex::Regex &  __regex_rename_at_874_163, regex::ReNode * __node_rename_at_874_164, uint8_t const  * const  __str_rename_at_874_165 )
{
    regex::ReNode * __nsub_rename_at_878_166 = __node_rename_at_874_164->subexpr;
    uint8_t const  * __tail_rename_at_879_167 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_878_166->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_874_163),__nsub_rename_at_878_166,__str_rename_at_874_165);
    if ( __tail_rename_at_879_167 == nullptr )
    {
        das_copy(__tail_rename_at_879_167,__str_rename_at_874_165);
    };
    das_copy(__node_rename_at_874_164->tail,__tail_rename_at_879_167);
    regex::ReNode * __node2_rename_at_884_168 = __node_rename_at_874_164->next;
    return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_884_168 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_884_168->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_874_163),__node2_rename_at_884_168,__tail_rename_at_879_167)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_879_167));
}

inline uint8_t const  * re_match2_plus_2bacb281be2d3e3d ( Context * __context__, regex::Regex &  __regex_rename_at_893_169, regex::ReNode * __node_rename_at_893_170, uint8_t const  * const  __str_rename_at_893_171 )
{
    if ( das_equ_val((*(__str_rename_at_893_171)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        regex::ReNode * __nsub_rename_at_900_172 = __node_rename_at_893_170->subexpr;
        uint8_t const  * __osym_rename_at_901_173 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_900_172->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_893_169),__nsub_rename_at_900_172,__str_rename_at_893_171);
        if ( __osym_rename_at_901_173 == nullptr )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __ofs_rename_at_905_174 = __osym_rename_at_901_173;
            regex::ReNode * __node2_rename_at_906_175 = __node_rename_at_893_170->next;
            if ( __node2_rename_at_906_175 != nullptr )
            {
                while ( __osym_rename_at_901_173 != nullptr )
                {
                    das_copy(__node_rename_at_893_170->tail,__osym_rename_at_901_173);
                    uint8_t const  * __otail_rename_at_910_176 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_906_175->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_893_169),__node2_rename_at_906_175,__osym_rename_at_901_173);
                    if ( __otail_rename_at_910_176 != nullptr )
                    {
                        return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_910_176);
                    } else {
                        das_copy(__osym_rename_at_901_173,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_900_172->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_893_169),__nsub_rename_at_900_172,__osym_rename_at_901_173));
                    };
                };
                return das_auto_cast<uint8_t const  *>::cast(nullptr);
            } else {
                while ( __osym_rename_at_901_173 != nullptr )
                {
                    das_copy(__ofs_rename_at_905_174,__osym_rename_at_901_173);
                    das_copy(__osym_rename_at_901_173,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_900_172->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_893_169),__nsub_rename_at_900_172,__ofs_rename_at_905_174));
                };
                das_copy(__node_rename_at_893_170->tail,__ofs_rename_at_905_174);
                return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_905_174);
            };
        };
    };
}

inline uint8_t const  * re_match2_plus_set_2c131274cab3d4e7 ( Context * __context__, regex::Regex &  __regex_rename_at_928_177, regex::ReNode * __node_rename_at_928_178, uint8_t const  * const  __str_rename_at_928_179 )
{
    if ( das_equ_val((*(__str_rename_at_928_179)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( !is_char_in_set(int32_t((*(__str_rename_at_928_179))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_928_178->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __ofs_rename_at_940_180 = das_ptr_add_int32(__str_rename_at_928_179,1,1);
            uint8_t const  * __osym_rename_at_941_181 = __ofs_rename_at_940_180;
            regex::ReNode * __node2_rename_at_942_182 = __node_rename_at_928_178->next;
            if ( __node2_rename_at_942_182 != nullptr )
            {
                while ( __osym_rename_at_941_181 != nullptr )
                {
                    das_copy(__node_rename_at_928_178->tail,__osym_rename_at_941_181);
                    uint8_t const  * __otail_rename_at_946_183 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_942_182->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_928_177),__node2_rename_at_942_182,__osym_rename_at_941_181);
                    if ( __otail_rename_at_946_183 != nullptr )
                    {
                        return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_946_183);
                    } else {
                        if ( !is_char_in_set(int32_t((*(__osym_rename_at_941_181))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_928_178->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                        {
                            return das_auto_cast<uint8_t const  *>::cast(nullptr);
                        } else {
                            das_ptr_inc(__osym_rename_at_941_181,1);
                        };
                    };
                };
                return das_auto_cast<uint8_t const  *>::cast(nullptr);
            } else {
                while ( __osym_rename_at_941_181 != nullptr )
                {
                    das_copy(__ofs_rename_at_940_180,__osym_rename_at_941_181);
                    if ( !is_char_in_set(int32_t((*(__osym_rename_at_941_181))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_928_178->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        break;
                    } else {
                        das_ptr_inc(__osym_rename_at_941_181,1);
                    };
                };
                das_copy(__node_rename_at_928_178->tail,__ofs_rename_at_940_180);
                return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_940_180);
            };
        };
    };
}

inline uint8_t const  * re_match2_star_710d04a0dde8fe1b ( Context * __context__, regex::Regex &  __regex_rename_at_971_184, regex::ReNode * __node_rename_at_971_185, uint8_t const  * const  __str_rename_at_971_186 )
{
    uint8_t const  * __ofs_rename_at_975_187 = __str_rename_at_971_186;
    regex::ReNode * __node2_rename_at_976_188 = __node_rename_at_971_185->next;
    regex::ReNode * __nsub_rename_at_977_189 = __node_rename_at_971_185->subexpr;
    if ( __node2_rename_at_976_188 != nullptr )
    {
        while ( __ofs_rename_at_975_187 != nullptr )
        {
            das_copy(__node_rename_at_971_185->tail,__ofs_rename_at_975_187);
            uint8_t const  * __oany_rename_at_981_190 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_976_188->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_971_184),__node2_rename_at_976_188,__ofs_rename_at_975_187));
            if ( __oany_rename_at_981_190 != nullptr )
            {
                return das_auto_cast<uint8_t const  *>::cast(__oany_rename_at_981_190);
            } else {
                das_copy(__ofs_rename_at_975_187,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_977_189->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_971_184),__nsub_rename_at_977_189,__ofs_rename_at_975_187));
            };
        };
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        uint8_t const  * __osym_rename_at_989_191 = __ofs_rename_at_975_187;
        while ( __osym_rename_at_989_191 != nullptr )
        {
            das_copy(__ofs_rename_at_975_187,__osym_rename_at_989_191);
            das_copy(__osym_rename_at_989_191,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_977_189->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_971_184),__nsub_rename_at_977_189,__ofs_rename_at_975_187));
        };
        das_copy(__node_rename_at_971_185->tail,__ofs_rename_at_975_187);
        return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_975_187);
    };
}

inline uint8_t const  * re_match2_group_9a7d00db369a905f ( Context * __context__, regex::Regex &  __regex_rename_at_1000_192, regex::ReNode * __node_rename_at_1000_193, uint8_t const  * const  __str_rename_at_1000_194 )
{
    if ( das_equ_val((*(__str_rename_at_1000_194)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        regex::ReNode * __nsub_rename_at_1007_195 = __node_rename_at_1000_193->subexpr;
        uint8_t const  * __osub_rename_at_1008_196 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_1007_195->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1000_192),__nsub_rename_at_1007_195,__str_rename_at_1000_194));
        if ( __osub_rename_at_1008_196 == nullptr )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            das_copy(das_get_auto_tuple_field<range,0,range,char *>::get(__regex_rename_at_1000_192.groups(__node_rename_at_1000_193->index,__context__)),range(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__str_rename_at_1000_194),das_auto_cast<void * const >::cast(__regex_rename_at_1000_192.match),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__node_rename_at_1000_193->subexpr->tail),das_auto_cast<void * const >::cast(__regex_rename_at_1000_192.match),1))));
            return das_auto_cast<uint8_t const  *>::cast(__osub_rename_at_1008_196);
        };
    };
}

inline bool re_early_out_fe884a05310cc120 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_1020_197, regex::ReNode * const  __node_rename_at_1020_198 )
{
    if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Char )
    {
        set_or_char_73a5f6127d860f5d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),get_character_at(__node_rename_at_1020_198->text,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Set )
    {
        set_or_set_265482f7b59ff469(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->cset);
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Any )
    {
        {
            bool __need_loop_1028 = true;
            // x: uint aka CharSet& -const
            das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_1020_197);
            uint32_t * __x_rename_at_1028_199;
            __need_loop_1028 = __x_iterator.first(__context__,(__x_rename_at_1028_199)) && __need_loop_1028;
            for ( ; __need_loop_1028 ; __need_loop_1028 = __x_iterator.next(__context__,(__x_rename_at_1028_199)) )
            {
                das_copy((*__x_rename_at_1028_199),0xffffffffu);
            }
            __x_iterator.close(__context__,(__x_rename_at_1028_199));
        };
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Eos )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Group )
    {
        return das_auto_cast<bool>::cast(re_early_out_fe884a05310cc120(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->subexpr));
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Plus )
    {
        return das_auto_cast<bool>::cast(re_early_out_fe884a05310cc120(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->subexpr));
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Star )
    {
        re_early_out_fe884a05310cc120(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->subexpr);
        return das_auto_cast<bool>::cast(true);
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Question )
    {
        re_early_out_fe884a05310cc120(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->subexpr);
        return das_auto_cast<bool>::cast(true);
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Concat )
    {
        bool __left_rename_at_1045_200 = ((bool)re_early_out_fe884a05310cc120(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->left));
        return das_auto_cast<bool>::cast(__left_rename_at_1045_200 ? das_auto_cast<bool>::cast(re_early_out_fe884a05310cc120(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->right)) : das_auto_cast<bool>::cast(false));
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        bool __any_rename_at_1048_201 = false;
        {
            bool __need_loop_1049 = true;
            // sub: regex::ReNode? const&
            das_iterator<TArray<regex::ReNode *> const > __sub_iterator(__node_rename_at_1020_198->all);
            regex::ReNode * const  * __sub_rename_at_1049_202;
            __need_loop_1049 = __sub_iterator.first(__context__,(__sub_rename_at_1049_202)) && __need_loop_1049;
            for ( ; __need_loop_1049 ; __need_loop_1049 = __sub_iterator.next(__context__,(__sub_rename_at_1049_202)) )
            {
                das_copy(__any_rename_at_1048_201,re_early_out_fe884a05310cc120(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),(*__sub_rename_at_1049_202)) || __any_rename_at_1048_201);
            }
            __sub_iterator.close(__context__,(__sub_rename_at_1049_202));
        };
        return das_auto_cast<bool>::cast(__any_rename_at_1048_201);
    } else {
        builtin_throw(((char *) "unsupported operation"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<bool>::cast(false);
    };
}

inline bool is_valid_2be9fd6793561ea5 ( Context * __context__, regex::Regex &  __re_rename_at_1063_203 )
{
    return das_auto_cast<bool>::cast(__re_rename_at_1063_203.root != nullptr);
}

inline bool regex_compile_6b2534030df9fde6 ( Context * __context__, regex::Regex &  __re_rename_at_1068_204, char * const  __expr_rename_at_1068_205 )
{
    das_copy(__re_rename_at_1068_204.root,re_parse_9d1fe24c108227a1(__context__,__expr_rename_at_1068_205));
    if ( __re_rename_at_1068_204.root != nullptr )
    {
        re_assign_next_3161851a13e94542(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1068_204));
        re_assign_groups_60ccbab514dcc1a1(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1068_204));
        re_assign_match_functions_d300482f26e4219c(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1068_204));
        re_early_out_fe884a05310cc120(__context__,das_arg<TDim<uint32_t,8>>::pass(__re_rename_at_1068_204.earlyOut),__re_rename_at_1068_204.root);
        das_copy(__re_rename_at_1068_204.canEarlyOut,!is_set_empty_a9584e784a71b800(__context__,das_arg<TDim<uint32_t,8>>::pass(__re_rename_at_1068_204.earlyOut)));
    };
    return das_auto_cast<bool>::cast(__re_rename_at_1068_204.root != nullptr);
}

inline regex::Regex regex_compile_36659a6d57325d9f ( Context * __context__, char * const  __expr_rename_at_1082_206 )
{
    regex::Regex __re_rename_at_1083_207;das_zero(__re_rename_at_1083_207);
    if ( !regex_compile_6b2534030df9fde6(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1083_207),__expr_rename_at_1082_206) )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_8, cast<char * const >::from(((char *) "regular expression ")), cast<char * const >::from(__expr_rename_at_1082_206), cast<char * const >::from(((char *) " did not compile")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return /* <- */ das_auto_cast_move<regex::Regex>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_f5c2782a98a931e5(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1083_207)));
}

inline regex::Regex regex_compile_f2f79bd4c9d9738d ( Context * __context__, regex::Regex &  __re_rename_at_1090_208 )
{
    if ( __re_rename_at_1090_208.root != nullptr )
    {
        re_assign_next_3161851a13e94542(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1090_208));
        re_assign_match_functions_d300482f26e4219c(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1090_208));
    };
    return /* <- */ das_auto_cast_move<regex::Regex>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_f5c2782a98a931e5(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1090_208)));
}

inline int32_t regex_match_5c805b227f49611f ( Context * __context__, regex::Regex &  __regex_rename_at_1098_209, char * const  __str_rename_at_1098_210, int32_t __offset_rename_at_1098_211 )
{
    if ( builtin_empty(__str_rename_at_1098_210) )
    {
        return das_auto_cast<int32_t>::cast(-1);
    } else {
        das_copy(__regex_rename_at_1098_209.match,das_cast<uint8_t *>::cast(__str_rename_at_1098_210));
        uint8_t const  * __mptr_rename_at_1109_212 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__regex_rename_at_1098_209.root->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1098_209),__regex_rename_at_1098_209.root,__regex_rename_at_1098_209.match));
        return das_auto_cast<int32_t>::cast((__mptr_rename_at_1109_212 == nullptr) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__mptr_rename_at_1109_212),das_auto_cast<void * const >::cast(__regex_rename_at_1098_209.match),1))));
    };
}

inline char * regex_group_b79e86c102f4f752 ( Context * __context__, regex::Regex const  &  __regex_rename_at_1117_213, int32_t __index_rename_at_1117_214, char * const  __match_rename_at_1117_215 )
{
    range __sub_range_rename_at_1119_216 = ((range)das_get_auto_tuple_field<range,0,range,char *>::get(__regex_rename_at_1117_213.groups(__index_rename_at_1117_214,__context__)));
    return das_auto_cast<char *>::cast(((char * const )(builtin_string_slice1(__match_rename_at_1117_215,v_extract_xi(v_cast_vec4i(__sub_range_rename_at_1119_216)) /*x*/,v_extract_yi(v_cast_vec4i(__sub_range_rename_at_1119_216)) /*y*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline void regex_foreach_5a01d237cb4dd08f ( Context * __context__, regex::Regex &  __regex_rename_at_1124_217, char * const  __str_rename_at_1124_218, Block DAS_COMMENT((bool,range)) const  &  __blk_rename_at_1124_219 )
{
    if ( builtin_empty(__str_rename_at_1124_218) )
    {
        return ;
    } else {
        das_copy(__regex_rename_at_1124_217.match,das_cast<uint8_t const  *>::cast(__str_rename_at_1124_218));
        regex::ReNode * __root_rename_at_1131_220 = __regex_rename_at_1124_217.root;
        uint8_t const  * __pstr_rename_at_1132_221 = das_cast<uint8_t const  *>::cast(__str_rename_at_1124_218);
        uint8_t const  * __cstr_rename_at_1133_222 = __pstr_rename_at_1132_221;
        if ( __regex_rename_at_1124_217.canEarlyOut )
        {
            while ( true )
            {
                int32_t __Ch_rename_at_1137_223 = ((int32_t)int32_t((*(__cstr_rename_at_1133_222))));
                if ( __Ch_rename_at_1137_223 == 0 )
                {
                    break;
                } else {
                    if ( is_char_in_set(__Ch_rename_at_1137_223,das_arg<TDim<uint32_t,8>>::pass(__regex_rename_at_1124_217.earlyOut),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        uint8_t const  * __om_rename_at_1142_224 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1131_220->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1124_217),__root_rename_at_1131_220,__cstr_rename_at_1133_222));
                        if ( __om_rename_at_1142_224 != nullptr )
                        {
                            if ( !das_invoke<bool>::invoke<range>(__context__,nullptr,__blk_rename_at_1124_219,range(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1133_222),das_auto_cast<void * const >::cast(__pstr_rename_at_1132_221),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1142_224),das_auto_cast<void * const >::cast(__pstr_rename_at_1132_221),1)))) )
                            {
                                break;
                            } else {
                                das_copy(__cstr_rename_at_1133_222,__om_rename_at_1142_224);
                            };
                        } else {
                            das_ptr_inc(__cstr_rename_at_1133_222,1);
                        };
                    } else {
                        das_ptr_inc(__cstr_rename_at_1133_222,1);
                    };
                };
            };
        } else {
            while ( int32_t((*(__cstr_rename_at_1133_222))) != 0 )
            {
                uint8_t const  * __om_rename_at_1157_225 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1131_220->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1124_217),__root_rename_at_1131_220,__cstr_rename_at_1133_222));
                if ( __om_rename_at_1157_225 != nullptr )
                {
                    if ( !das_invoke<bool>::invoke<range>(__context__,nullptr,__blk_rename_at_1124_219,range(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1133_222),das_auto_cast<void * const >::cast(__pstr_rename_at_1132_221),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1157_225),das_auto_cast<void * const >::cast(__pstr_rename_at_1132_221),1)))) )
                    {
                        break;
                    } else {
                        das_copy(__cstr_rename_at_1133_222,__om_rename_at_1157_225);
                    };
                } else {
                    das_ptr_inc(__cstr_rename_at_1133_222,1);
                };
            };
        };
    };
}

inline char * regex_replace_63d1ae99224ff8b8 ( Context * __context__, regex::Regex &  __regex_rename_at_1172_226, char * const  __str_rename_at_1172_227, Block DAS_COMMENT((char *,char * const )) const  &  __blk_rename_at_1172_228 )
{
    return das_auto_cast<char *>::cast(builtin_empty(__str_rename_at_1172_227) ? das_auto_cast<char * const >::cast(nullptr) : das_auto_cast<char * const >::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1178_229) DAS_AOT_INLINE_LAMBDA -> void{
        das_copy(__regex_rename_at_1172_226.match,das_cast<uint8_t const  *>::cast(__str_rename_at_1172_227));
        regex::ReNode * __root_rename_at_1180_230 = __regex_rename_at_1172_226.root;
        uint8_t const  * __pstr_rename_at_1181_231 = das_cast<uint8_t const  *>::cast(__str_rename_at_1172_227);
        uint8_t const  * __cstr_rename_at_1182_232 = __pstr_rename_at_1181_231;
        if ( __regex_rename_at_1172_226.canEarlyOut )
        {
            while ( true )
            {
                int32_t __Ch_rename_at_1186_233 = ((int32_t)int32_t((*(__cstr_rename_at_1182_232))));
                if ( __Ch_rename_at_1186_233 == 0 )
                {
                    break;
                } else {
                    if ( is_char_in_set(__Ch_rename_at_1186_233,das_arg<TDim<uint32_t,8>>::pass(__regex_rename_at_1172_226.earlyOut),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        uint8_t const  * __om_rename_at_1191_234 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1180_230->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1172_226),__root_rename_at_1180_230,__cstr_rename_at_1182_232));
                        if ( __om_rename_at_1191_234 != nullptr )
                        {
                            char * __repl_rename_at_1193_235 = ((char *)(char *)(das_invoke<char *>::invoke<char * const >(__context__,nullptr,__blk_rename_at_1172_228,((char * const )(builtin_string_slice1(__str_rename_at_1172_227,int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1182_232),das_auto_cast<void * const >::cast(__pstr_rename_at_1181_231),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1191_234),das_auto_cast<void * const >::cast(__pstr_rename_at_1181_231),1)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_9,cast<StringBuilderWriter &>::from(__writer_rename_at_1178_229),cast<char * const >::from(__repl_rename_at_1193_235)));
                            das_copy(__cstr_rename_at_1182_232,__om_rename_at_1191_234);
                        } else {
                            write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1178_229),__Ch_rename_at_1186_233);
                            das_ptr_inc(__cstr_rename_at_1182_232,1);
                        };
                    } else {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1178_229),__Ch_rename_at_1186_233);
                        das_ptr_inc(__cstr_rename_at_1182_232,1);
                    };
                };
            };
        } else {
            while ( int32_t((*(__cstr_rename_at_1182_232))) != 0 )
            {
                uint8_t const  * __om_rename_at_1207_236 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1180_230->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1172_226),__root_rename_at_1180_230,__cstr_rename_at_1182_232));
                if ( __om_rename_at_1207_236 != nullptr )
                {
                    char * __repl_rename_at_1209_237 = ((char *)(char *)(das_invoke<char *>::invoke<char * const >(__context__,nullptr,__blk_rename_at_1172_228,((char * const )(builtin_string_slice1(__str_rename_at_1172_227,int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1182_232),das_auto_cast<void * const >::cast(__pstr_rename_at_1181_231),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1207_236),das_auto_cast<void * const >::cast(__pstr_rename_at_1181_231),1)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_10,cast<StringBuilderWriter &>::from(__writer_rename_at_1178_229),cast<char * const >::from(__repl_rename_at_1209_237)));
                    das_copy(__cstr_rename_at_1182_232,__om_rename_at_1207_236);
                } else {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1178_229),int32_t((*(__cstr_rename_at_1182_232))));
                    das_ptr_inc(__cstr_rename_at_1182_232,1);
                };
            };
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void regex_debug_faa0082717395089 ( Context * __context__, regex::Regex const  &  __regex_rename_at_1226_238 )
{
    debug_re_7f15622d8d8cdbd(__context__,__regex_rename_at_1226_238.root,1);
    builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __regex_rename_at_1226_238.canEarlyOut )
    {
        builtin_print(((char *) "early out: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_set_b2e50f12da6018dc(__context__,__regex_rename_at_1226_238.earlyOut);
        builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( builtin_array_size(__regex_rename_at_1226_238.groups) != 0 )
    {
        builtin_print(((char *) "groups:"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        {
            bool __need_loop_1237 = true;
            // g: tuple<range;string> const&
            das_iterator<TArray<AutoTuple<range,char *>> const > __g_iterator(__regex_rename_at_1226_238.groups);
            AutoTuple<range,char *> const  * __g_rename_at_1237_239;
            __need_loop_1237 = __g_iterator.first(__context__,(__g_rename_at_1237_239)) && __need_loop_1237;
            for ( ; __need_loop_1237 ; __need_loop_1237 = __g_iterator.next(__context__,(__g_rename_at_1237_239)) )
            {
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_11, cast<char * const >::from(((char *) "\t")), cast<char * const >::from(das_get_auto_tuple_field<char *,1,range,char *>::get((*__g_rename_at_1237_239))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            }
            __g_iterator.close(__context__,(__g_rename_at_1237_239));
        };
    };
}

inline void debug_set_b2e50f12da6018dc ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_1243_240 )
{
    {
        bool __need_loop_1245 = true;
        // x: int const
        das_iterator<range> __x_iterator(range(0,256));
        int32_t __x_rename_at_1245_241;
        __need_loop_1245 = __x_iterator.first(__context__,(__x_rename_at_1245_241)) && __need_loop_1245;
        for ( ; __need_loop_1245 ; __need_loop_1245 = __x_iterator.next(__context__,(__x_rename_at_1245_241)) )
        {
            if ( (__cset_rename_at_1243_240((__x_rename_at_1245_241 >> 5),__context__) & uint32_t(1 << (__x_rename_at_1245_241 & 31))) != 0x0u )
            {
                if ( (__x_rename_at_1245_241 >= 32) && (__x_rename_at_1245_241 <= 127) )
                {
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_12, cast<char * const >::from(((char * const )(to_string_char(__x_rename_at_1245_241,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<char * const >::from(((char *) "\\")), cast<int32_t>::from(__x_rename_at_1245_241))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_1245_241));
    };
}

inline void debug_re_7f15622d8d8cdbd ( Context * __context__, regex::ReNode * const  __node_rename_at_1256_242, int32_t __tab_rename_at_1256_243 )
{
    builtin_print(((char *) "("),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __node_rename_at_1256_242 != nullptr )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_14, cast<char * const >::from(((char *) "#")), cast<int32_t>::from(__node_rename_at_1256_242->id))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        if ( __node_rename_at_1256_242->next != nullptr )
        {
            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_15, cast<char * const >::from(((char *) "->")), cast<int32_t>::from(__node_rename_at_1256_242->next->id))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            builtin_print(((char *) "->!"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        builtin_print(((char *) " "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( __node_rename_at_1256_242 == nullptr )
    {
        builtin_print(((char *) "null)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Char )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_16, cast<char * const >::from(((char *) "Char `")), cast<char * const >::from(__node_rename_at_1256_242->text), cast<char * const >::from(((char *) "`)")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Set )
    {
        builtin_print(((char *) "Set "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_set_b2e50f12da6018dc(__context__,__node_rename_at_1256_242->cset);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Any )
    {
        builtin_print(((char *) "Any)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Eos )
    {
        builtin_print(((char *) "Eos)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Group )
    {
        builtin_print(((char *) "Group "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_7f15622d8d8cdbd(__context__,__node_rename_at_1256_242->subexpr,__tab_rename_at_1256_243 + 1);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Plus )
    {
        builtin_print(((char *) "Plus "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_7f15622d8d8cdbd(__context__,__node_rename_at_1256_242->subexpr,__tab_rename_at_1256_243);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Star )
    {
        builtin_print(((char *) "Star "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_7f15622d8d8cdbd(__context__,__node_rename_at_1256_242->subexpr,__tab_rename_at_1256_243);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Question )
    {
        builtin_print(((char *) "Question "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_7f15622d8d8cdbd(__context__,__node_rename_at_1256_242->subexpr,__tab_rename_at_1256_243);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Concat )
    {
        builtin_print(((char *) "Contact"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_17, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_7f15622d8d8cdbd(__context__,__node_rename_at_1256_242->left,__tab_rename_at_1256_243 + 1);
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_18, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_7f15622d8d8cdbd(__context__,__node_rename_at_1256_242->right,__tab_rename_at_1256_243 + 1);
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_19, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243 - 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ")")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        builtin_print(((char *) "Union\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        {
            bool __need_loop_1304 = true;
            // sub: regex::ReNode? const&
            das_iterator<TArray<regex::ReNode *> const > __sub_iterator(__node_rename_at_1256_242->all);
            regex::ReNode * const  * __sub_rename_at_1304_244;
            __need_loop_1304 = __sub_iterator.first(__context__,(__sub_rename_at_1304_244)) && __need_loop_1304;
            for ( ; __need_loop_1304 ; __need_loop_1304 = __sub_iterator.next(__context__,(__sub_rename_at_1304_244)) )
            {
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_20, cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                debug_re_7f15622d8d8cdbd(__context__,(*__sub_rename_at_1304_244),__tab_rename_at_1256_243 + 1);
                builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            }
            __sub_iterator.close(__context__,(__sub_rename_at_1304_244));
        };
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_21, cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243 - 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ")")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        builtin_throw(((char *) "unsupported op"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline int32_t rnd_set_character_7d9ac437612f4580 ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_1317_245, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1317_246 )
{
    uint32_t __total_rename_at_1318_247 = ((uint32_t)char_set_total(__cset_rename_at_1317_245));
    uint32_t __idx_rename_at_1319_248 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_5eaabf5f150eca27(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1317_246),__idx_rename_at_1319_248);
    das_copy(__idx_rename_at_1319_248,SimPolicy<uint32_t>::Mod(__idx_rename_at_1319_248,__total_rename_at_1318_247,*__context__,nullptr));
    return das_auto_cast<int32_t>::cast(char_set_element(int32_t(__idx_rename_at_1319_248),__cset_rename_at_1317_245));
}

inline void re_gen2_fail_f4a8fc2e202ea042 ( Context * __context__, regex::ReNode * __node_rename_at_1325_249, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1325_250, StringBuilderWriter &  __str_rename_at_1325_251 )
{
    DAS_ASSERTF((false),(((char *) "not running generated regex")));
}

inline void re_gen2_char_b094672da38efcc9 ( Context * __context__, regex::ReNode * __node_rename_at_1332_252, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1332_253, StringBuilderWriter &  __str_rename_at_1332_254 )
{
    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_22,cast<StringBuilderWriter &>::from(__str_rename_at_1332_254),cast<char *>::from(__node_rename_at_1332_252->text)));
    regex::ReNode * __node2_rename_at_1337_255 = __node_rename_at_1332_252->next;
    if ( __node2_rename_at_1337_255 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1337_255->gen2,__node2_rename_at_1337_255,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1332_253),das_arg<StringBuilderWriter>::pass(__str_rename_at_1332_254));
    };
}

inline void re_gen2_union_2d880d8ac2104b61 ( Context * __context__, regex::ReNode * __node_rename_at_1343_256, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1343_257, StringBuilderWriter &  __str_rename_at_1343_258 )
{
    uint32_t __nidx_rename_at_1347_259 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_5eaabf5f150eca27(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1343_257),__nidx_rename_at_1347_259);
    das_copy(__nidx_rename_at_1347_259,SimPolicy<uint32_t>::Mod(__nidx_rename_at_1347_259,uint32_t(builtin_array_size(das_arg<TArray<regex::ReNode *>>::pass(__node_rename_at_1343_256->all))),*__context__,nullptr));
    regex::ReNode * __sub_rename_at_1350_260 = __node_rename_at_1343_256->all(__nidx_rename_at_1347_259,__context__);
    das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__sub_rename_at_1350_260->gen2,__sub_rename_at_1350_260,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1343_257),das_arg<StringBuilderWriter>::pass(__str_rename_at_1343_258));
}

inline void re_gen2_set_ee117f703604ac9b ( Context * __context__, regex::ReNode * __node_rename_at_1354_261, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1354_262, StringBuilderWriter &  __str_rename_at_1354_263 )
{
    write_string_char(das_arg<StringBuilderWriter>::pass(__str_rename_at_1354_263),rnd_set_character_7d9ac437612f4580(__context__,das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_1354_261->cset),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1354_262)));
    regex::ReNode * __node2_rename_at_1359_264 = __node_rename_at_1354_261->next;
    if ( __node2_rename_at_1359_264 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1359_264->gen2,__node2_rename_at_1359_264,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1354_262),das_arg<StringBuilderWriter>::pass(__str_rename_at_1354_263));
    };
}

inline void re_gen2_any_84c4495ae64f3117 ( Context * __context__, regex::ReNode * __node_rename_at_1365_265, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1365_266, StringBuilderWriter &  __str_rename_at_1365_267 )
{
    uint32_t __ch_rename_at_1369_268 = 0;
    _FuncbuiltinTicknextTick17450348357676149856_5eaabf5f150eca27(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1365_266),__ch_rename_at_1369_268);
    write_string_char(das_arg<StringBuilderWriter>::pass(__str_rename_at_1365_267),int32_t((SimPolicy<uint32_t>::Mod(__ch_rename_at_1369_268,0xffu,*__context__,nullptr)) + 0x1u));
    regex::ReNode * __node2_rename_at_1372_269 = __node_rename_at_1365_265->next;
    if ( __node2_rename_at_1372_269 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1372_269->gen2,__node2_rename_at_1372_269,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1365_266),das_arg<StringBuilderWriter>::pass(__str_rename_at_1365_267));
    };
}

inline void re_gen2_concat_ceb76f519a092f7b ( Context * __context__, regex::ReNode * __node_rename_at_1378_270, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1378_271, StringBuilderWriter &  __str_rename_at_1378_272 )
{
    regex::ReNode * __left_rename_at_1382_273 = __node_rename_at_1378_270->left;
    das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__left_rename_at_1382_273->gen2,__left_rename_at_1382_273,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1378_271),das_arg<StringBuilderWriter>::pass(__str_rename_at_1378_272));
}

inline void re_gen2_eos_d426964d1c59cee9 ( Context * __context__, regex::ReNode * __node_rename_at_1386_274, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1386_275, StringBuilderWriter &  __str_rename_at_1386_276 )
{
    regex::ReNode * __node2_rename_at_1390_277 = __node_rename_at_1386_274->next;
    if ( __node2_rename_at_1390_277 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1390_277->gen2,__node2_rename_at_1390_277,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1386_275),das_arg<StringBuilderWriter>::pass(__str_rename_at_1386_276));
    };
}

inline void re_gen2_question_bc45a1b91b690115 ( Context * __context__, regex::ReNode * __node_rename_at_1396_278, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1396_279, StringBuilderWriter &  __str_rename_at_1396_280 )
{
    uint32_t __a_rename_at_1400_281 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_5eaabf5f150eca27(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1396_279),__a_rename_at_1400_281);
    if ( (__a_rename_at_1400_281 & 0x1u) != 0x0u )
    {
        regex::ReNode * __nsub_rename_at_1403_282 = __node_rename_at_1396_278->subexpr;
        if ( __nsub_rename_at_1403_282 != nullptr )
        {
            das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_1403_282->gen2,__nsub_rename_at_1403_282,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1396_279),das_arg<StringBuilderWriter>::pass(__str_rename_at_1396_280));
        };
    };
    regex::ReNode * __node2_rename_at_1408_283 = __node_rename_at_1396_278->next;
    if ( __node2_rename_at_1408_283 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1408_283->gen2,__node2_rename_at_1408_283,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1396_279),das_arg<StringBuilderWriter>::pass(__str_rename_at_1396_280));
    };
}

inline void re_gen2_plus_597c4ad99a5fba37 ( Context * __context__, regex::ReNode * __node_rename_at_1414_284, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1414_285, StringBuilderWriter &  __str_rename_at_1414_286 )
{
    uint32_t __cnt_rename_at_1418_287 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_5eaabf5f150eca27(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1414_285),__cnt_rename_at_1418_287);
    das_copy(__cnt_rename_at_1418_287,(SimPolicy<uint32_t>::Mod(__cnt_rename_at_1418_287,das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/,*__context__,nullptr)) + 0x1u);
    regex::ReNode * __nsub_rename_at_1421_288 = __node_rename_at_1414_284->subexpr;
    if ( __nsub_rename_at_1421_288 != nullptr )
    {
        {
            bool __need_loop_1423 = true;
            // i: uint const
            das_iterator<urange> __i_iterator(mk_urange(__cnt_rename_at_1418_287));
            uint32_t __i_rename_at_1423_289;
            __need_loop_1423 = __i_iterator.first(__context__,(__i_rename_at_1423_289)) && __need_loop_1423;
            for ( ; __need_loop_1423 ; __need_loop_1423 = __i_iterator.next(__context__,(__i_rename_at_1423_289)) )
            {
                das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_1421_288->gen2,__nsub_rename_at_1421_288,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1414_285),das_arg<StringBuilderWriter>::pass(__str_rename_at_1414_286));
            }
            __i_iterator.close(__context__,(__i_rename_at_1423_289));
        };
    };
    regex::ReNode * __node2_rename_at_1427_290 = __node_rename_at_1414_284->next;
    if ( __node2_rename_at_1427_290 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1427_290->gen2,__node2_rename_at_1427_290,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1414_285),das_arg<StringBuilderWriter>::pass(__str_rename_at_1414_286));
    };
}

inline void re_gen2_star_86821b5dcfb13c67 ( Context * __context__, regex::ReNode * __node_rename_at_1433_291, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1433_292, StringBuilderWriter &  __str_rename_at_1433_293 )
{
    uint32_t __cnt_rename_at_1437_294 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_5eaabf5f150eca27(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1433_292),__cnt_rename_at_1437_294);
    das_copy(__cnt_rename_at_1437_294,SimPolicy<uint32_t>::Mod(__cnt_rename_at_1437_294,das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/,*__context__,nullptr));
    regex::ReNode * __nsub_rename_at_1440_295 = __node_rename_at_1433_291->subexpr;
    if ( __nsub_rename_at_1440_295 != nullptr )
    {
        {
            bool __need_loop_1442 = true;
            // i: uint const
            das_iterator<urange> __i_iterator(mk_urange(__cnt_rename_at_1437_294));
            uint32_t __i_rename_at_1442_296;
            __need_loop_1442 = __i_iterator.first(__context__,(__i_rename_at_1442_296)) && __need_loop_1442;
            for ( ; __need_loop_1442 ; __need_loop_1442 = __i_iterator.next(__context__,(__i_rename_at_1442_296)) )
            {
                das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_1440_295->gen2,__nsub_rename_at_1440_295,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1433_292),das_arg<StringBuilderWriter>::pass(__str_rename_at_1433_293));
            }
            __i_iterator.close(__context__,(__i_rename_at_1442_296));
        };
    };
    regex::ReNode * __node2_rename_at_1446_297 = __node_rename_at_1433_291->next;
    if ( __node2_rename_at_1446_297 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1446_297->gen2,__node2_rename_at_1446_297,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1433_292),das_arg<StringBuilderWriter>::pass(__str_rename_at_1433_293));
    };
}

inline void re_gen2_group_26ba1a1d83dd9e6d ( Context * __context__, regex::ReNode * __node_rename_at_1452_298, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1452_299, StringBuilderWriter &  __str_rename_at_1452_300 )
{
    regex::ReNode * __nsub_rename_at_1456_301 = __node_rename_at_1452_298->subexpr;
    if ( __nsub_rename_at_1456_301 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_1456_301->gen2,__nsub_rename_at_1456_301,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1452_299),das_arg<StringBuilderWriter>::pass(__str_rename_at_1452_300));
    };
    regex::ReNode * __node2_rename_at_1460_302 = __node_rename_at_1452_298->next;
    if ( __node2_rename_at_1460_302 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1460_302->gen2,__node2_rename_at_1460_302,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1452_299),das_arg<StringBuilderWriter>::pass(__str_rename_at_1452_300));
    };
}

inline char * re_gen_8a63cdad2cc3534 ( Context * __context__, regex::Regex &  __re_rename_at_1466_303, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1466_304 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1467_305) DAS_AOT_INLINE_LAMBDA -> void{
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__re_rename_at_1466_303.root->gen2,__re_rename_at_1466_303.root,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1466_304),das_arg<StringBuilderWriter>::pass(__writer_rename_at_1467_305));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xcfdf08e1762022ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_7fc2d92ecf01659d>>();
    };
    aotLib[0xcb143188cffce5ac] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_4a49cd9427735830>>();
    };
    aotLib[0x95c5496cb14bf31d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_590cb9676400afd6>>();
    };
    aotLib[0xa6385d78e91451d7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_da2baa75dd8697b6>>();
    };
    aotLib[0x593d61936f6e847c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_f5c2782a98a931e5>>();
    };
    aotLib[0x44e53dc87f7b398f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncregexTickfrom_hexTick16446213900708532977_b8179530d20c164e>>();
    };
    aotLib[0x51c0b2413029cf84] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_52d27e6b6d896c8e>>();
    };
    aotLib[0x88a1e47f5c1f934b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_8f6d6a28264adb8d>>();
    };
    aotLib[0xf30d76f809df4f62] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_43fccb5b4c62ad5a>>();
    };
    aotLib[0x6454458160c24896] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_4c5483eb7641701a>>();
    };
    aotLib[0x1f767f39343eebb4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_af4e82606bed0404>>();
    };
    aotLib[0xbfa0c6db95fa124b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_5eaabf5f150eca27>>();
    };
    aotLib[0xa1603993bca79899] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen_get_rep_limit_372fed7e922113ce>>();
    };
    aotLib[0x9b4be595d03dc857] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&trace_cd2ce953d9f02f9a>>();
    };
    aotLib[0xebab27819aa6d6f1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&note_25c3af148e31112d>>();
    };
    aotLib[0x726ac855b72cf14e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&trace_backtrack_6e9e1f1de5871abc>>();
    };
    aotLib[0x70feed4b90815360] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&trace_value_11b6d845c20bfb2b>>();
    };
    aotLib[0xa7c3ea9810bef734] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&nada_21a7a887ebf7be7e>>();
    };
    aotLib[0xfa2bffe5ddea2ec0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&maybe_93c6ce7837ece2e3>>();
    };
    aotLib[0x7637035b17848cb4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&eos_9cf46cda98559501>>();
    };
    aotLib[0x4b9d167cd5c2ebef] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&at_fb737fdb637e5537>>();
    };
    aotLib[0xaa4ea1146ff9afca] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&next_ed15f5916dd1d2e8>>();
    };
    aotLib[0x4b1a527bf40aa8dc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_set_empty_a9584e784a71b800>>();
    };
    aotLib[0x2e65631d9b9f072f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_or_char_73a5f6127d860f5d>>();
    };
    aotLib[0xf948238c25a78915] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_or_range_df3e4e666738e1c>>();
    };
    aotLib[0xea32c003e39d9751] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_invert_e53b0a0fb329873f>>();
    };
    aotLib[0x4d6ab4ea8053f95a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_or_set_265482f7b59ff469>>();
    };
    aotLib[0xa7849bfd34a7ea31] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_negative_99e8ff767d43b22b>>();
    };
    aotLib[0x40fceaa5249b7ebf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_meta_6351bcbc951c65a2>>();
    };
    aotLib[0xd8d0283c8e789352] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_meta_character_769c0fb37a614b28>>();
    };
    aotLib[0x3dd257b300190f8b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_set_character_9cff1ab92756b9ba>>();
    };
    aotLib[0x921a6ff1a2be2c62] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&from_hex_3cd126717eb6217c>>();
    };
    aotLib[0x86f7988e877d6b12] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&string_from_hex_95ab4ebb1457623>>();
    };
    aotLib[0x52b75ea384cc2f31] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&string_from_hex_5db9509fd24b3404>>();
    };
    aotLib[0xcb1318f8ba8c5e50] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&re_char_60a54157df7133ed>>();
    };
    aotLib[0xb752c1a50c0dead1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&re_set_items_c147705b8465a388>>();
    };
    aotLib[0x6e2ca2d10672e226] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&re_set_35692c162ac593eb>>();
    };
    aotLib[0xa1229d1e03445517] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&re_any_334bc91044491e9b>>();
    };
    aotLib[0x8b28a2e2e1550eec] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&re_eos_2765a197cbc77325>>();
    };
    aotLib[0xfc4ee56e440775c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&re_group_5f4507d0a2c4f3ff>>();
    };
    aotLib[0x16a1d88645bc96fd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&re_elementary_8ee25ec433d368d4>>();
    };
    aotLib[0x3c482729be513e44] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mk_concat_3517b4044b6da896>>();
    };
    aotLib[0x5ccaf8a59dce108f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mk_union_a25f3c5c478c43c7>>();
    };
    aotLib[0x7f0cf77f82ecaf57] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&re_re_dd3df7e732e24abc>>();
    };
    aotLib[0x742230668a0f5551] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&re_basic_7d05cb7188a92536>>();
    };
    aotLib[0x589cffef9e3b7940] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_parse_9d1fe24c108227a1>>();
    };
    aotLib[0x7fda449a2a9f9be] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&visit_top_down_adf9d2fabd97f41f>>();
    };
    aotLib[0x834bc4089d6e1e4c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_assign_next_3161851a13e94542>>();
    };
    aotLib[0xd5bb8d104792c882] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_assign_groups_60ccbab514dcc1a1>>();
    };
    aotLib[0x854e82d9d7485350] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_assign_match_functions_d300482f26e4219c>>();
    };
    aotLib[0xc82419cea102b102] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_single_char_9681a659a9acd2f4>>();
    };
    aotLib[0xbcb619bf43089223] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_char_94e78160b779ed44>>();
    };
    aotLib[0xed64e4ca8575b3e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_union_4751f89c2670f07e>>();
    };
    aotLib[0xc886074aa494f275] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_set_ba554535354f0cbe>>();
    };
    aotLib[0x2118bb9d818df137] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_any_ec3fe998dcc6d0d>>();
    };
    aotLib[0xcf57e4bf0e062ebc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_concat_70ffb9e9661a77a>>();
    };
    aotLib[0x9528932e8dcc5ba2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_eos_f6528a26791746bb>>();
    };
    aotLib[0x1e2b10be4e778b7e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_question_822bbf9722705b86>>();
    };
    aotLib[0x7c6ac7bbb9413cd1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_plus_2bacb281be2d3e3d>>();
    };
    aotLib[0x82b0878fde237be9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_plus_set_2c131274cab3d4e7>>();
    };
    aotLib[0x4237f39ad7519841] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_star_710d04a0dde8fe1b>>();
    };
    aotLib[0x60cbf2a92ecac3dd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_match2_group_9a7d00db369a905f>>();
    };
    aotLib[0x169c2210c434744d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_early_out_fe884a05310cc120>>();
    };
    aotLib[0x19e2532a7b7c85d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_valid_2be9fd6793561ea5>>();
    };
    aotLib[0xb18d1ed1977f4fcc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&regex_compile_6b2534030df9fde6>>();
    };
    aotLib[0x16f59c0c4a792d90] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&regex_compile_36659a6d57325d9f>>();
    };
    aotLib[0xf4c59f245b79afdd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&regex_compile_f2f79bd4c9d9738d>>();
    };
    aotLib[0xd94e19a7688ffcb6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&regex_match_5c805b227f49611f>>();
    };
    aotLib[0xf69acbe198ae0b85] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&regex_group_b79e86c102f4f752>>();
    };
    aotLib[0x58cc901a9ff4284] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&regex_foreach_5a01d237cb4dd08f>>();
    };
    aotLib[0x7d64895303811ba4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&regex_replace_63d1ae99224ff8b8>>();
    };
    aotLib[0xc9a9a4aea2be4e4d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&regex_debug_faa0082717395089>>();
    };
    aotLib[0x708111b90969d70] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_set_b2e50f12da6018dc>>();
    };
    aotLib[0xe7cf2932555a8d16] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_re_7f15622d8d8cdbd>>();
    };
    aotLib[0xb6d16dd53064f3a9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&rnd_set_character_7d9ac437612f4580>>();
    };
    aotLib[0xb1dff6e4977c48c6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_fail_f4a8fc2e202ea042>>();
    };
    aotLib[0xe2d4f9ed58c19fc2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_char_b094672da38efcc9>>();
    };
    aotLib[0x764be7274db93010] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_union_2d880d8ac2104b61>>();
    };
    aotLib[0xf882099a0e02077a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_set_ee117f703604ac9b>>();
    };
    aotLib[0x283261801bf73be9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_any_84c4495ae64f3117>>();
    };
    aotLib[0x615504516da58daf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_concat_ceb76f519a092f7b>>();
    };
    aotLib[0xd53960eef57d546d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_eos_d426964d1c59cee9>>();
    };
    aotLib[0x1fae86417e0a3d2c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_question_bc45a1b91b690115>>();
    };
    aotLib[0x84195ac7278d9f63] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_plus_597c4ad99a5fba37>>();
    };
    aotLib[0x4b80ab5c22c57651] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_star_86821b5dcfb13c67>>();
    };
    aotLib[0xcaf44e07434e0e27] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen2_group_26ba1a1d83dd9e6d>>();
    };
    aotLib[0x9d14edd814161ce4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&re_gen_8a63cdad2cc3534>>();
    };
    // [[ init script ]]
    aotLib[0x2918e001398a2d9] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_6180131985610406295
AotListBase impl_aot_regex(_anon_6180131985610406295::registerAotFunctions);
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
