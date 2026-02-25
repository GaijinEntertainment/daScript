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
    Bos = int32_t(4),
    Group = int32_t(5),
    Plus = int32_t(6),
    Star = int32_t(7),
    Question = int32_t(8),
    Concat = int32_t(9),
    Union = int32_t(10),
    Repeat = int32_t(11),
    WordBoundary = int32_t(12),
    NonWordBoundary = int32_t(13),
    Lookahead = int32_t(14),
    NegativeLookahead = int32_t(15),
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
    int32_t min_rep;
    int32_t max_rep;
    bool lazy;
    uint8_t const  * tail;
};
static_assert(sizeof(ReNode)==160,"structure size mismatch with DAS");
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
static_assert(offsetof(ReNode,min_rep)==140,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,max_rep)==144,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,lazy)==148,"structure field offset mismatch with DAS");
static_assert(offsetof(ReNode,tail)==152,"structure field offset mismatch with DAS");
}
namespace regex {

struct Regex {
    regex::ReNode * root;
    uint8_t const  * match;
    TArray<TTuple<16,range,char *>> groups;
    TDim<uint32_t,8> earlyOut;
    bool canEarlyOut;
    bool caseInsensitive;
    bool dotAll;
};
static_assert(sizeof(Regex)==80,"structure size mismatch with DAS");
static_assert(offsetof(Regex,root)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,match)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,groups)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,earlyOut)==40,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,canEarlyOut)==72,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,caseInsensitive)==73,"structure field offset mismatch with DAS");
static_assert(offsetof(Regex,dotAll)==74,"structure field offset mismatch with DAS");
}
extern StructInfo __struct_info__739e0aa00b63f5bf;
extern StructInfo __struct_info__a442c67b1b45039e;
extern TypeInfo __type_info__6efe4a274318358b;
extern TypeInfo __type_info__d658aa3b467a4710;
extern TypeInfo __type_info__8cbf7e31d94517eb;
extern TypeInfo __type_info__1e3f0d9b1ed71149;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af96fe4c8658cf52;
extern TypeInfo __type_info__b661860848e8711e;
extern TypeInfo __type_info__9c60f9702c7c9014;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__e68681b2b99a9c9a;
extern TypeInfo __type_info__21ea97d8ca8ffd5;
extern TypeInfo __type_info__646c01d58047ed68;
extern TypeInfo __type_info__bc3c477a5c78263e;
extern TypeInfo __type_info__b8d4477a5991973e;
extern TypeInfo __type_info__9b868a595f8cb1d9;
extern TypeInfo __type_info__af63ef4c86020cd5;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63e84c860200f0;
extern TypeInfo __type_info__af63eb4c86020609;

EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_0 = { "Char", 0 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_1 = { "Set", 1 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_2 = { "Any", 2 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_3 = { "Eos", 3 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_4 = { "Bos", 4 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_5 = { "Group", 5 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_6 = { "Plus", 6 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_7 = { "Star", 7 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_8 = { "Question", 8 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_9 = { "Concat", 9 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_10 = { "Union", 10 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_11 = { "Repeat", 11 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_12 = { "WordBoundary", 12 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_13 = { "NonWordBoundary", 13 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_14 = { "Lookahead", 14 };
EnumValueInfo __enum_info__f5eadb7b60b9c74d_value_15 = { "NegativeLookahead", 15 };
EnumValueInfo * __enum_info__f5eadb7b60b9c74d_values [] = { &__enum_info__f5eadb7b60b9c74d_value_0, &__enum_info__f5eadb7b60b9c74d_value_1, &__enum_info__f5eadb7b60b9c74d_value_2, &__enum_info__f5eadb7b60b9c74d_value_3, &__enum_info__f5eadb7b60b9c74d_value_4, &__enum_info__f5eadb7b60b9c74d_value_5, &__enum_info__f5eadb7b60b9c74d_value_6, &__enum_info__f5eadb7b60b9c74d_value_7, &__enum_info__f5eadb7b60b9c74d_value_8, &__enum_info__f5eadb7b60b9c74d_value_9, &__enum_info__f5eadb7b60b9c74d_value_10, &__enum_info__f5eadb7b60b9c74d_value_11, &__enum_info__f5eadb7b60b9c74d_value_12, &__enum_info__f5eadb7b60b9c74d_value_13, &__enum_info__f5eadb7b60b9c74d_value_14, &__enum_info__f5eadb7b60b9c74d_value_15 };
EnumInfo __enum_info__f5eadb7b60b9c74d = { "ReOp", "regex", __enum_info__f5eadb7b60b9c74d_values, 16, UINT64_C(0xf5eadb7b60b9c74d) };
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
VarInfo __struct_info__739e0aa00b63f5bf_field_11 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xf06990a7cd9b8e8c), "next", 96, 17 };
uint32_t __type_info__125ea9a7ea60fe3a_dim_var_8331108043184141759[1] = { 8 };
VarInfo __struct_info__739e0aa00b63f5bf_field_12 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 1, __type_info__125ea9a7ea60fe3a_dim_var_8331108043184141759, 30, 32, UINT64_C(0x125ea9a7ea60fe3a), "cset", 104, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_13 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x4f1b9e3acf383cbd), "index", 136, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_14 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xab89c1000cced4bb), "min_rep", 140, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_15 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x6fa04d382744237d), "max_rep", 144, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_16 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xe2658aa7c1519608), "lazy", 148, 0 };
VarInfo __struct_info__739e0aa00b63f5bf_field_17 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xe2757da7c183c939), "tail", 152, 18 };
VarInfo * __struct_info__739e0aa00b63f5bf_fields[18] =  { &__struct_info__739e0aa00b63f5bf_field_0, &__struct_info__739e0aa00b63f5bf_field_1, &__struct_info__739e0aa00b63f5bf_field_2, &__struct_info__739e0aa00b63f5bf_field_3, &__struct_info__739e0aa00b63f5bf_field_4, &__struct_info__739e0aa00b63f5bf_field_5, &__struct_info__739e0aa00b63f5bf_field_6, &__struct_info__739e0aa00b63f5bf_field_7, &__struct_info__739e0aa00b63f5bf_field_8, &__struct_info__739e0aa00b63f5bf_field_9, &__struct_info__739e0aa00b63f5bf_field_10, &__struct_info__739e0aa00b63f5bf_field_11, &__struct_info__739e0aa00b63f5bf_field_12, &__struct_info__739e0aa00b63f5bf_field_13, &__struct_info__739e0aa00b63f5bf_field_14, &__struct_info__739e0aa00b63f5bf_field_15, &__struct_info__739e0aa00b63f5bf_field_16, &__struct_info__739e0aa00b63f5bf_field_17 };
StructInfo __struct_info__739e0aa00b63f5bf = {"ReNode", "regex", 28, __struct_info__739e0aa00b63f5bf_fields, 18, 160, UINT64_C(0x0), nullptr, UINT64_C(0x739e0aa00b63f5bf), 5 };
VarInfo __struct_info__a442c67b1b45039e_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x37de19b6affa3dd6), "root", 0, 1 };
VarInfo __struct_info__a442c67b1b45039e_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x90ed0cbd06831825), "match", 8, 2 };
VarInfo __struct_info__a442c67b1b45039e_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__6efe4a274318358b, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x6eee618b86e51341), "groups", 16, 7 };
uint32_t __type_info__aa30cda461a0c553_dim_var_11836241002723869598[1] = { 8 };
VarInfo __struct_info__a442c67b1b45039e_field_3 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 1, __type_info__aa30cda461a0c553_dim_var_11836241002723869598, 30, 32, UINT64_C(0xaa30cda461a0c553), "earlyOut", 40, 0 };
VarInfo __struct_info__a442c67b1b45039e_field_4 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x7ce5b1477861b3d1), "canEarlyOut", 72, 0 };
VarInfo __struct_info__a442c67b1b45039e_field_5 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xa88d28e43575dd35), "caseInsensitive", 73, 0 };
VarInfo __struct_info__a442c67b1b45039e_field_6 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x69509cc688abd9eb), "dotAll", 74, 0 };
VarInfo * __struct_info__a442c67b1b45039e_fields[7] =  { &__struct_info__a442c67b1b45039e_field_0, &__struct_info__a442c67b1b45039e_field_1, &__struct_info__a442c67b1b45039e_field_2, &__struct_info__a442c67b1b45039e_field_3, &__struct_info__a442c67b1b45039e_field_4, &__struct_info__a442c67b1b45039e_field_5, &__struct_info__a442c67b1b45039e_field_6 };
StructInfo __struct_info__a442c67b1b45039e = {"Regex", "regex", 28, __struct_info__a442c67b1b45039e_fields, 7, 80, UINT64_C(0x0), nullptr, UINT64_C(0xa442c67b1b45039e), 0 };
VarInfo __func_info__61179b2f43878551_field_0 =  { Type::tRange, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0x83edaa17741d4ac2), "at", 0, 0 };
VarInfo * __func_info__61179b2f43878551_fields[1] =  { &__func_info__61179b2f43878551_field_0 };
FuncInfo __func_info__61179b2f43878551 = {"invoke block<(at:range const):bool const> const", "", __func_info__61179b2f43878551_fields, 1, 32, &__type_info__af81fe4c86352052, nullptr,0,UINT64_C(0x61179b2f43878551), 0x0 };
VarInfo __func_info__8b6d6c464a360a4f_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xc248c77e17cca11a), "node", 0, 0 };
VarInfo * __func_info__8b6d6c464a360a4f_fields[1] =  { &__func_info__8b6d6c464a360a4f_field_0 };
FuncInfo __func_info__8b6d6c464a360a4f = {"invoke block<(var node:regex::ReNode?):void> const", "", __func_info__8b6d6c464a360a4f_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8b6d6c464a360a4f), 0x0 };
TypeInfo * __type_info__6efe4a274318358b_arg_types[2] = { &__type_info__af63ef4c86020cd5, &__type_info__af63ee4c86020b22 };
TypeInfo __type_info__6efe4a274318358b = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__6efe4a274318358b_arg_types, nullptr, 2, 0, nullptr, 16390, 16, UINT64_C(0x6efe4a274318358b) };
TypeInfo __type_info__d658aa3b467a4710 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xd658aa3b467a4710) };
TypeInfo __type_info__8cbf7e31d94517eb = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__e68681b2b99a9c9a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x8cbf7e31d94517eb) };
TypeInfo __type_info__1e3f0d9b1ed71149 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__b661860848e8711e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x1e3f0d9b1ed71149) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af96fe4c8658cf52 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf96fe4c8658cf52) };
TypeInfo __type_info__b661860848e8711e = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xb661860848e8711e) };
TypeInfo __type_info__9c60f9702c7c9014 = { Type::tEnumeration, nullptr, &__enum_info__f5eadb7b60b9c74d, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x9c60f9702c7c9014) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__e68681b2b99a9c9a = { Type::tStructure, &__struct_info__739e0aa00b63f5bf, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 160, UINT64_C(0xe68681b2b99a9c9a) };
TypeInfo __type_info__21ea97d8ca8ffd5 = { Type::tStructure, &__struct_info__a442c67b1b45039e, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 80, UINT64_C(0x21ea97d8ca8ffd5) };
TypeInfo __type_info__646c01d58047ed68 = { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e84c860200f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0x646c01d58047ed68) };
TypeInfo __type_info__bc3c477a5c78263e = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ef4c86020cd5, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xbc3c477a5c78263e) };
TypeInfo __type_info__b8d4477a5991973e = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xb8d4477a5991973e) };
TypeInfo __type_info__9b868a595f8cb1d9 = { Type::tStructure, &__struct_info__a442c67b1b45039e, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 80, UINT64_C(0x9b868a595f8cb1d9) };
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

TypeInfo * __tinfo_0[2] = { &__type_info__9b868a595f8cb1d9, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[2] = { &__type_info__b8d4477a5991973e, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_2[2] = { &__type_info__bc3c477a5c78263e, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_3[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_4[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_8[1] = { &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_9[2] = { &__type_info__af90fe4c864e9d52, &__type_info__9c60f9702c7c9014 };
TypeInfo * __tinfo_10[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_11[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_22[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_27[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_32[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_34[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_494d51b3fa621d18 ( Context * __context__, TArray<regex::ReNode *> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_93105ead5b29fa0 ( Context * __context__, TArray<regex::ReNode *> & __a_rename_at_1336_2 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_fe94899ed7cf7fdb ( Context * __context__, TDim<regex::ReNode *,2> const  & __a_rename_at_598_4 );
inline void finalize_61e451af4e714ca2 ( Context * __context__, regex::ReNode & ____this_rename_at_56_5 );
inline regex::Regex & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_9c8fefcf8a2c4d0d ( Context * __context__, regex::Regex & __a_rename_at_50_6 );
inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a5393f36ccdac4a8 ( Context * __context__, TArray<char *> & __a_rename_at_50_7 );
inline TArray<range> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1b414e731a438fa8 ( Context * __context__, TArray<range> & __a_rename_at_50_8 );
inline int32_t _FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc ( Context * __context__, int32_t __ch1_rename_at_298_9, int32_t __ch2_rename_at_298_10 );
inline void finalize_4c4d68a9a950ec41 ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> & ____this_rename_at_485_11 );
inline void finalize_c680337a54511d24 ( Context * __context__, regex::ReNode * & ____this_rename_at_641_12 );
inline void _FuncbuiltinTickpushTick10769833213962245646_906d8abb52edd211 ( Context * __context__, TArray<regex::ReNode *> & __Arr_rename_at_193_13, regex::ReNode * __value_rename_at_193_14 );
inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_1d04d83c4b41cf2c ( Context * __context__, TDim<regex::ReNode *,2> & __a_rename_at_1523_15 );
inline void _FuncbuiltinTickpushTick10769833213962245646_7df733d16209a68a ( Context * __context__, TArray<TTuple<16,range,char *>> & __Arr_rename_at_193_17, TTuple<16,range,char *> & __value_rename_at_193_18 );
inline void _FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606 ( Context * __context__, TArray<uint8_t const  *> & __Arr_rename_at_193_19, uint8_t const  * __value_rename_at_193_20 );
inline void _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 ( Context * __context__, TArray<char *> & __Arr_rename_at_177_21, char * const  __value_rename_at_177_22 );
inline void _FuncbuiltinTickpushTick14133213201864676143_32d7c798299302f2 ( Context * __context__, TArray<range> & __Arr_rename_at_177_23, range __value_rename_at_177_24 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904 ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) & __it_rename_at_1377_25, uint32_t & __value_rename_at_1377_26 );
inline uint32_t re_gen_get_rep_limit_aa71779c47fee5b8 ( Context * __context__ );
inline void trace_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_112_27, int32_t __offset_rename_at_112_28 );
inline void note_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_119_29, int32_t __offset_rename_at_119_30 );
inline void trace_backtrack_60bdee73e3315732 ( Context * __context__ );
inline void trace_value_f443ec2d74a0c7bd ( Context * __context__, regex::ReNode * const  __value_rename_at_132_31 );
inline TVariant<16,8,regex::ReNode *,void *> nada_ba27de56c6f5dc72 ( Context * __context__ );
inline TVariant<16,8,regex::ReNode *,void *> maybe_249d36fe62a185a7 ( Context * __context__, regex::ReNode * __value_rename_at_147_32 );
inline bool eos_b645f5ba09d5c5 ( Context * __context__, char * const  __expr_rename_at_152_33, int32_t __offset_rename_at_152_34 );
inline int32_t at_ac9760d67b4716d0 ( Context * __context__, char * const  __expr_rename_at_156_35, int32_t __offset_rename_at_156_36 );
inline int32_t next_6ab20ac9bb2c573c ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> const  & __re_rename_at_175_37 );
inline bool is_set_empty_1f052aa7f260c4eb ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_183_38 );
inline void set_or_char_6485de9cac36fb6f ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_192_40, int32_t __ch_rename_at_192_41 );
inline void set_or_range_eead05ea9800cf6a ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_196_42, range __bits_rename_at_196_43 );
inline void set_invert_f44d20bc14c46b29 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_202_45 );
inline void set_or_set_c5a22cc26b33786b ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_208_47, TDim<uint32_t,8> const  & __eset_rename_at_208_48 );
inline void set_negative_959b5894df7dd660 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_214_53 );
inline void set_meta_c297bca2e11798f6 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_220_55, int32_t __che_rename_at_220_56 );
inline bool is_meta_character_6b868635173399ff ( Context * __context__, int32_t __ch_rename_at_268_60 );
inline bool is_set_character_3b302fd2b4140e7f ( Context * __context__, int32_t __ch_rename_at_272_61 );
inline bool is_digit_35d4a4cc9eacb04d ( Context * __context__, int32_t __ch_rename_at_276_62 );
inline bool is_word_character_8422846159268aaf ( Context * __context__, int32_t __ch_rename_at_280_63 );
inline int32_t from_hex_86a2ef9963e757 ( Context * __context__, int32_t __ch_rename_at_284_64 );
inline char * string_from_hex_5698e77bb25d7508 ( Context * __context__, int32_t __ch1_rename_at_302_65 );
inline char * string_from_hex_e27d9e88554466f8 ( Context * __context__, int32_t __ch1_rename_at_306_66, int32_t __ch2_rename_at_306_67 );
inline TVariant<16,8,regex::ReNode *,void *> re_char_5110fe336617b529 ( Context * __context__, char * const  __expr_rename_at_311_68, int32_t __offset_rename_at_311_69 );
inline TVariant<16,8,regex::ReNode *,void *> re_set_items_ca22db507ac86515 ( Context * __context__, char * const  __expr_rename_at_365_76, int32_t __offset_rename_at_365_77 );
inline TVariant<16,8,regex::ReNode *,void *> re_set_3642c8eadc1c1fb5 ( Context * __context__, char * const  __expr_rename_at_467_89, int32_t __offset_rename_at_467_90 );
inline TVariant<16,8,regex::ReNode *,void *> re_any_d551e389d6ffd28 ( Context * __context__, char * const  __expr_rename_at_501_94, int32_t __offset_rename_at_501_95 );
inline TVariant<16,8,regex::ReNode *,void *> re_eos_3dff18197056692d ( Context * __context__, char * const  __expr_rename_at_513_96, int32_t __offset_rename_at_513_97 );
inline TVariant<16,8,regex::ReNode *,void *> re_bos_939db26d2fdba5e2 ( Context * __context__, char * const  __expr_rename_at_525_98, int32_t __offset_rename_at_525_99 );
inline TVariant<16,8,regex::ReNode *,void *> re_group_17f1ffc0bc11a16 ( Context * __context__, char * const  __expr_rename_at_537_100, int32_t __offset_rename_at_537_101 );
inline TVariant<16,8,regex::ReNode *,void *> re_elementary_409594d3d2b7cfa3 ( Context * __context__, char * const  __expr_rename_at_600_111, int32_t __offset_rename_at_600_112 );
inline regex::ReNode * mk_concat_53dbf44b1354a65 ( Context * __context__, regex::ReNode * __left_rename_at_632_119, regex::ReNode * __right_rename_at_632_120 );
inline regex::ReNode * mk_union_b8290c88c89cea8b ( Context * __context__, regex::ReNode * __left_rename_at_660_122, regex::ReNode * __right_rename_at_660_123 );
inline TVariant<16,8,regex::ReNode *,void *> re_re_12ab55bce538b67e ( Context * __context__, char * const  __expr_rename_at_687_125, int32_t __offset_rename_at_687_126 );
inline TVariant<16,8,regex::ReNode *,void *> re_basic_3aca537b6901f1ec ( Context * __context__, char * const  __expr_rename_at_724_131, int32_t __offset_rename_at_724_132 );
inline regex::ReNode * re_parse_be404de54055d495 ( Context * __context__, char * const  __expr_rename_at_825_149 );
inline void visit_top_down_28b61d33d98f34cc ( Context * __context__, regex::ReNode * __node_rename_at_838_152, Block DAS_COMMENT((void,regex::ReNode *)) const  & __blk_rename_at_838_153 );
inline void re_assign_next_5d4c1e040adf8167 ( Context * __context__, regex::Regex & __re_rename_at_855_155 );
inline void re_assign_groups_3ba808af5f6a0682 ( Context * __context__, regex::Regex & __re_rename_at_874_159 );
inline void re_assign_match_functions_96c5fc406200914c ( Context * __context__, regex::Regex & __re_rename_at_896_162 );
inline int32_t ci_lower_ee451210b4105701 ( Context * __context__, int32_t __ch_rename_at_989_164 );
inline void case_fold_set_3fe63201f3f0aed7 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_997_165 );
inline uint8_t const  * re_match2_single_char_7ee863dbfd5bdb67 ( Context * __context__, regex::Regex & __regex_rename_at_1010_169, regex::ReNode * __node_rename_at_1010_170, uint8_t const  * const  __str_rename_at_1010_171 );
inline uint8_t const  * re_match2_char_f2046197ce02637d ( Context * __context__, regex::Regex & __regex_rename_at_1033_174, regex::ReNode * __node_rename_at_1033_175, uint8_t const  * const  __str_rename_at_1033_176 );
inline uint8_t const  * re_match2_single_char_ci_68081666546328b ( Context * __context__, regex::Regex & __regex_rename_at_1059_179, regex::ReNode * __node_rename_at_1059_180, uint8_t const  * const  __str_rename_at_1059_181 );
inline uint8_t const  * re_match2_char_ci_3c6abffae8c4e1d5 ( Context * __context__, regex::Regex & __regex_rename_at_1082_184, regex::ReNode * __node_rename_at_1082_185, uint8_t const  * const  __str_rename_at_1082_186 );
inline uint8_t const  * re_match2_union_bcaea863938bc283 ( Context * __context__, regex::Regex & __regex_rename_at_1107_190, regex::ReNode * __node_rename_at_1107_191, uint8_t const  * const  __str_rename_at_1107_192 );
inline uint8_t const  * re_match2_set_5cb609a7e934b852 ( Context * __context__, regex::Regex & __regex_rename_at_1122_195, regex::ReNode * __node_rename_at_1122_196, uint8_t const  * const  __str_rename_at_1122_197 );
inline uint8_t const  * re_match2_any_e0f778af97b719a3 ( Context * __context__, regex::Regex & __regex_rename_at_1145_200, regex::ReNode * __node_rename_at_1145_201, uint8_t const  * const  __str_rename_at_1145_202 );
inline uint8_t const  * re_match2_concat_2e0ee6aaeac1bff7 ( Context * __context__, regex::Regex & __regex_rename_at_1168_205, regex::ReNode * __node_rename_at_1168_206, uint8_t const  * const  __str_rename_at_1168_207 );
inline uint8_t const  * re_match2_eos_f51ea4d66660c6ee ( Context * __context__, regex::Regex & __regex_rename_at_1182_210, regex::ReNode * __node_rename_at_1182_211, uint8_t const  * const  __str_rename_at_1182_212 );
inline uint8_t const  * re_match2_question_55ffbd37b5f31b71 ( Context * __context__, regex::Regex & __regex_rename_at_1199_214, regex::ReNode * __node_rename_at_1199_215, uint8_t const  * const  __str_rename_at_1199_216 );
inline uint8_t const  * re_match2_plus_cbc885000639d48b ( Context * __context__, regex::Regex & __regex_rename_at_1218_220, regex::ReNode * __node_rename_at_1218_221, uint8_t const  * const  __str_rename_at_1218_222 );
inline uint8_t const  * re_match2_plus_set_e285952ef61ae545 ( Context * __context__, regex::Regex & __regex_rename_at_1253_228, regex::ReNode * __node_rename_at_1253_229, uint8_t const  * const  __str_rename_at_1253_230 );
inline uint8_t const  * re_match2_star_3e354952f52dedc7 ( Context * __context__, regex::Regex & __regex_rename_at_1296_235, regex::ReNode * __node_rename_at_1296_236, uint8_t const  * const  __str_rename_at_1296_237 );
inline uint8_t const  * re_match2_group_cdd84cde5e0cc0e3 ( Context * __context__, regex::Regex & __regex_rename_at_1325_243, regex::ReNode * __node_rename_at_1325_244, uint8_t const  * const  __str_rename_at_1325_245 );
inline uint8_t const  * re_match2_bos_7093e76efb08a7f4 ( Context * __context__, regex::Regex & __regex_rename_at_1341_248, regex::ReNode * __node_rename_at_1341_249, uint8_t const  * const  __str_rename_at_1341_250 );
inline uint8_t const  * re_match2_word_boundary_de93f59a606699b5 ( Context * __context__, regex::Regex & __regex_rename_at_1358_252, regex::ReNode * __node_rename_at_1358_253, uint8_t const  * const  __str_rename_at_1358_254 );
inline uint8_t const  * re_match2_non_word_boundary_d0c20eaf4c18dc8a ( Context * __context__, regex::Regex & __regex_rename_at_1382_258, regex::ReNode * __node_rename_at_1382_259, uint8_t const  * const  __str_rename_at_1382_260 );
inline uint8_t const  * re_match2_lookahead_3ac542e65f4d1cff ( Context * __context__, regex::Regex & __regex_rename_at_1406_264, regex::ReNode * __node_rename_at_1406_265, uint8_t const  * const  __str_rename_at_1406_266 );
inline uint8_t const  * re_match2_negative_lookahead_ba2437211db882de ( Context * __context__, regex::Regex & __regex_rename_at_1426_270, regex::ReNode * __node_rename_at_1426_271, uint8_t const  * const  __str_rename_at_1426_272 );
inline uint8_t const  * re_match2_repeat_3b5f93e4a9fcaadf ( Context * __context__, regex::Regex & __regex_rename_at_1446_276, regex::ReNode * __node_rename_at_1446_277, uint8_t const  * const  __str_rename_at_1446_278 );
inline uint8_t const  * re_match2_question_lazy_63762e1b1147774a ( Context * __context__, regex::Regex & __regex_rename_at_1548_299, regex::ReNode * __node_rename_at_1548_300, uint8_t const  * const  __str_rename_at_1548_301 );
inline uint8_t const  * re_match2_star_lazy_2c4bd54606277ac1 ( Context * __context__, regex::Regex & __regex_rename_at_1579_306, regex::ReNode * __node_rename_at_1579_307, uint8_t const  * const  __str_rename_at_1579_308 );
inline uint8_t const  * re_match2_plus_lazy_e54a263affed7fc8 ( Context * __context__, regex::Regex & __regex_rename_at_1605_313, regex::ReNode * __node_rename_at_1605_314, uint8_t const  * const  __str_rename_at_1605_315 );
inline uint8_t const  * re_match2_repeat_lazy_1105b9a6a4ba9b41 ( Context * __context__, regex::Regex & __regex_rename_at_1637_320, regex::ReNode * __node_rename_at_1637_321, uint8_t const  * const  __str_rename_at_1637_322 );
inline bool re_early_out_65d4b897dbff0d04 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_1708_334, regex::ReNode * const  __node_rename_at_1708_335 );
inline bool is_valid_3c125942baf6bf0a ( Context * __context__, regex::Regex & __re_rename_at_1767_341 );
inline bool regex_compile_e1c30ff6f42517a2 ( Context * __context__, regex::Regex & __re_rename_at_1772_342, char * const  __expr_rename_at_1772_343, bool __case_insensitive_rename_at_1772_344, bool __dot_all_rename_at_1772_345 );
inline regex::Regex regex_compile_6b8af38459f7df28 ( Context * __context__, char * const  __expr_rename_at_1790_346, bool __case_insensitive_rename_at_1790_347, bool __dot_all_rename_at_1790_348 );
inline regex::Regex regex_compile_23e411aa689a04e5 ( Context * __context__, regex::Regex & __re_rename_at_1801_350 );
inline int32_t regex_match_85e0a2ceef06ab24 ( Context * __context__, regex::Regex & __regex_rename_at_1810_351, char * const  __str_rename_at_1810_352, int32_t __offset_rename_at_1810_353 );
inline char * regex_group_dacc7e63de7d674e ( Context * __context__, regex::Regex const  & __regex_rename_at_1829_355, int32_t __index_rename_at_1829_356, char * const  __match_rename_at_1829_357 );
inline void regex_foreach_2cf0e40b2436b6d9 ( Context * __context__, regex::Regex & __regex_rename_at_1836_359, char * const  __str_rename_at_1836_360, Block DAS_COMMENT((bool,range)) const  & __blk_rename_at_1836_361 );
inline char * regex_replace_e2c898a32c8b9a0a ( Context * __context__, regex::Regex & __regex_rename_at_1892_368, char * const  __str_rename_at_1892_369, Block DAS_COMMENT((char *,char * const )) const  & __blk_rename_at_1892_370 );
inline char * expand_replacement_ad68a38d9900aed3 ( Context * __context__, regex::Regex & __regex_rename_at_1952_380, char * const  __str_rename_at_1952_381, char * const  __replacement_rename_at_1952_382, int32_t __match_start_rename_at_1952_383, int32_t __match_end_rename_at_1952_384 );
inline char * regex_replace_7cc2deb86b39b5c3 ( Context * __context__, regex::Regex & __regex_rename_at_2020_400, char * const  __str_rename_at_2020_401, char * const  __replacement_rename_at_2020_402 );
inline int2 regex_search_712ff63ef772d38d ( Context * __context__, regex::Regex & __regex_rename_at_2087_416, char * const  __str_rename_at_2087_417, int32_t __offset_rename_at_2087_418 );
inline TArray<char *> regex_split_ad2c6f596b0add93 ( Context * __context__, regex::Regex & __regex_rename_at_2126_425, char * const  __str_rename_at_2126_426 );
inline TArray<range> regex_match_all_dc6b47bf82eb552 ( Context * __context__, regex::Regex & __regex_rename_at_2142_430, char * const  __str_rename_at_2142_431 );
inline char * regex_group_by_name_14993f975288220c ( Context * __context__, regex::Regex const  & __regex_rename_at_2152_434, char * const  __name_rename_at_2152_435, char * const  __str_rename_at_2152_436 );
inline range _FuncSqblSqbr_f97767f82664a020 ( Context * __context__, regex::Regex const  & __regex_rename_at_2163_438, int32_t __index_rename_at_2163_439 );
inline range _FuncSqblSqbr_9dbe1a1c2a54dde3 ( Context * __context__, regex::Regex const  & __regex_rename_at_2168_440, char * const  __name_rename_at_2168_441 );
inline void regex_debug_55af4874d0243e85 ( Context * __context__, regex::Regex const  & __regex_rename_at_2183_443 );
inline void debug_set_c7915a561c802ef5 ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_2200_445 );
inline void debug_re_c9a788d56ecca6c1 ( Context * __context__, regex::ReNode * const  __node_rename_at_2213_447, int32_t __tab_rename_at_2213_448 );
inline int32_t rnd_set_character_a6ace307aa9880d1 ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_2298_450, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2298_451 );
inline void re_gen2_fail_b917d161e3783b38 ( Context * __context__, regex::ReNode * __node_rename_at_2306_454, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2306_455, StringBuilderWriter & __str_rename_at_2306_456 );
inline void re_gen2_char_4112c00708ab51c6 ( Context * __context__, regex::ReNode * __node_rename_at_2313_457, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2313_458, StringBuilderWriter & __str_rename_at_2313_459 );
inline void re_gen2_union_9b028b61c503f834 ( Context * __context__, regex::ReNode * __node_rename_at_2324_461, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2324_462, StringBuilderWriter & __str_rename_at_2324_463 );
inline void re_gen2_set_b1d4208a13b42b9 ( Context * __context__, regex::ReNode * __node_rename_at_2335_466, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2335_467, StringBuilderWriter & __str_rename_at_2335_468 );
inline void re_gen2_any_5e88209f8e29e4e5 ( Context * __context__, regex::ReNode * __node_rename_at_2346_470, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2346_471, StringBuilderWriter & __str_rename_at_2346_472 );
inline void re_gen2_concat_30030241eb92a8af ( Context * __context__, regex::ReNode * __node_rename_at_2363_475, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2363_476, StringBuilderWriter & __str_rename_at_2363_477 );
inline void re_gen2_eos_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2371_479, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2371_480, StringBuilderWriter & __str_rename_at_2371_481 );
inline void re_gen2_question_1b31bedc870aa21a ( Context * __context__, regex::ReNode * __node_rename_at_2381_483, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2381_484, StringBuilderWriter & __str_rename_at_2381_485 );
inline void re_gen2_plus_2a39935c8341bc2a ( Context * __context__, regex::ReNode * __node_rename_at_2399_489, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2399_490, StringBuilderWriter & __str_rename_at_2399_491 );
inline void re_gen2_star_677f11c532be890b ( Context * __context__, regex::ReNode * __node_rename_at_2418_496, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2418_497, StringBuilderWriter & __str_rename_at_2418_498 );
inline void re_gen2_group_267d4348c759899b ( Context * __context__, regex::ReNode * __node_rename_at_2437_503, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2437_504, StringBuilderWriter & __str_rename_at_2437_505 );
inline void re_gen2_bos_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2451_508, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2451_509, StringBuilderWriter & __str_rename_at_2451_510 );
inline void re_gen2_repeat_d2475d70f23bd85 ( Context * __context__, regex::ReNode * __node_rename_at_2462_512, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2462_513, StringBuilderWriter & __str_rename_at_2462_514 );
inline void re_gen2_word_boundary_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2488_519, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2488_520, StringBuilderWriter & __str_rename_at_2488_521 );
inline void re_gen2_non_word_boundary_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2499_523, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2499_524, StringBuilderWriter & __str_rename_at_2499_525 );
inline void re_gen2_lookahead_267d4348c759899b ( Context * __context__, regex::ReNode * __node_rename_at_2510_527, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2510_528, StringBuilderWriter & __str_rename_at_2510_529 );
inline void re_gen2_negative_lookahead_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2525_532, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2525_533, StringBuilderWriter & __str_rename_at_2525_534 );
inline char * re_gen_bf361240a28b8867 ( Context * __context__, regex::Regex & __re_rename_at_2536_536, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_2536_537 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<bool,0xef68509581809424>(__context__) = false;/*log_parse_enabled*/
    das_global<bool,0xa970cdba254dba34>(__context__) = false;/*log_match_enabled*/
    das_global<bool,0xa65624a8416952c2>(__context__) = false;/*range_check_enabled*/
    das_global<bool,0x79de6c82fae2b617>(__context__) = false;/*log_gen_enabled*/
    das_global<int32_t,0x3cc0153fc83ffc69>(__context__) = 0;/*trace_tab*/
    das_global<uint32_t,0x8ce995ce57e18608>(__context__) = 0x8u;/*re_gen_rep_limit*/
    das_global<char *,0xa1ce201dbf46e0ab>(__context__) = ((char *) "\\+*.()[]|^{}");/*meta*/
    das_global<char *,0x39690c1b5c6d7b5a>(__context__) = ((char *) "wWsSdDnrtfvbB");/*meta_set*/
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_494d51b3fa621d18 ( Context * __context__, TArray<regex::ReNode *> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<regex::ReNode *>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_93105ead5b29fa0 ( Context * __context__, TArray<regex::ReNode *> &  __a_rename_at_1336_2 )
{
    {
        bool __need_loop_1338 = true;
        // aV: regex::ReNode? aka TT&
        das_iterator<TArray<regex::ReNode *>> __aV_iterator(__a_rename_at_1336_2);
        regex::ReNode * * __aV_rename_at_1338_3;
        __need_loop_1338 = __aV_iterator.first(__context__,(__aV_rename_at_1338_3)) && __need_loop_1338;
        for ( ; __need_loop_1338 ; __need_loop_1338 = __aV_iterator.next(__context__,(__aV_rename_at_1338_3)) )
        {
            finalize_c680337a54511d24(__context__,(*__aV_rename_at_1338_3));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1338_3));
    };
    builtin_array_free(das_arg<TArray<regex::ReNode *>>::pass(__a_rename_at_1336_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_fe94899ed7cf7fdb ( Context * __context__, TDim<regex::ReNode *,2> const  &  __a_rename_at_598_4 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline void finalize_61e451af4e714ca2 ( Context * __context__, regex::ReNode &  ____this_rename_at_56_5 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_93105ead5b29fa0(__context__,das_arg<TArray<regex::ReNode *>>::pass(____this_rename_at_56_5.all));
    finalize_c680337a54511d24(__context__,____this_rename_at_56_5.left);
    finalize_c680337a54511d24(__context__,____this_rename_at_56_5.right);
    finalize_c680337a54511d24(__context__,____this_rename_at_56_5.subexpr);
    memset((void*)&(____this_rename_at_56_5), 0, 160);
}

inline regex::Regex & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_9c8fefcf8a2c4d0d ( Context * __context__, regex::Regex &  __a_rename_at_50_6 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<2>(__tinfo_0,cast<regex::Regex &>::from(__a_rename_at_50_6),cast<char * const >::from(((char *) "object can't be returned because it is locked"))));
    return das_auto_cast_ref<regex::Regex &>::cast(__a_rename_at_50_6);
}

inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a5393f36ccdac4a8 ( Context * __context__, TArray<char *> &  __a_rename_at_50_7 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<TArray<char *> &>::from(__a_rename_at_50_7),cast<char * const >::from(((char *) "object can't be returned because it is locked"))));
    return das_auto_cast_ref<TArray<char *> &>::cast(__a_rename_at_50_7);
}

inline TArray<range> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1b414e731a438fa8 ( Context * __context__, TArray<range> &  __a_rename_at_50_8 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<TArray<range> &>::from(__a_rename_at_50_8),cast<char * const >::from(((char *) "object can't be returned because it is locked"))));
    return das_auto_cast_ref<TArray<range> &>::cast(__a_rename_at_50_8);
}

inline int32_t _FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc ( Context * __context__, int32_t __ch1_rename_at_298_9, int32_t __ch2_rename_at_298_10 )
{
    return das_auto_cast<int32_t>::cast((from_hex_86a2ef9963e757(__context__,__ch1_rename_at_298_9) * 16) + from_hex_86a2ef9963e757(__context__,__ch2_rename_at_298_10));
}

inline void finalize_4c4d68a9a950ec41 ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> &  ____this_rename_at_485_11 )
{
    if ( das_get_variant_field<regex::ReNode *,8,0>::is(____this_rename_at_485_11) )
    {
        finalize_c680337a54511d24(__context__,das_get_variant_field<regex::ReNode *,8,0>::get(____this_rename_at_485_11));
    };
    memset((void*)&(____this_rename_at_485_11), 0, 16);
}

inline void finalize_c680337a54511d24 ( Context * __context__, regex::ReNode * & ____this_rename_at_641_12 )
{
    if ( ____this_rename_at_641_12 != nullptr )
    {
        finalize_61e451af4e714ca2(__context__,das_arg<regex::ReNode>::pass(das_deref(__context__,____this_rename_at_641_12,__FILE__,__LINE__)));
        das_delete<regex::ReNode *>::clear(__context__,____this_rename_at_641_12);
        das_copy(____this_rename_at_641_12,nullptr);
    };
}

inline void _FuncbuiltinTickpushTick10769833213962245646_906d8abb52edd211 ( Context * __context__, TArray<regex::ReNode *> &  __Arr_rename_at_193_13, regex::ReNode * __value_rename_at_193_14 )
{
    das_copy(__Arr_rename_at_193_13(builtin_array_push_back(das_arg<TArray<regex::ReNode *>>::pass(__Arr_rename_at_193_13),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_14);
}

inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_1d04d83c4b41cf2c ( Context * __context__, TDim<regex::ReNode *,2> &  __a_rename_at_1523_15 )
{
    TArray<regex::ReNode *> __arr_rename_at_1525_16;das_zero(__arr_rename_at_1525_16);
    _FuncbuiltinTickresizeTick4811697762258667383_494d51b3fa621d18(__context__,das_arg<TArray<regex::ReNode *>>::pass(__arr_rename_at_1525_16),2);
    das_copy(das_cast<TDim<regex::ReNode *,2>>::cast(das_ref(__context__,__arr_rename_at_1525_16(0,__context__))),__a_rename_at_1523_15);
    return /* <- */ das_auto_cast_move<TArray<regex::ReNode *>>::cast(__arr_rename_at_1525_16);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_7df733d16209a68a ( Context * __context__, TArray<TTuple<16,range,char *>> &  __Arr_rename_at_193_17, TTuple<16,range,char *> &  __value_rename_at_193_18 )
{
    das_copy(__Arr_rename_at_193_17(builtin_array_push_back(das_arg<TArray<TTuple<16,range,char *>>>::pass(__Arr_rename_at_193_17),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_18);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606 ( Context * __context__, TArray<uint8_t const  *> &  __Arr_rename_at_193_19, uint8_t const  * __value_rename_at_193_20 )
{
    das_copy(__Arr_rename_at_193_19(builtin_array_push_back(das_arg<TArray<uint8_t const  *>>::pass(__Arr_rename_at_193_19),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_20);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 ( Context * __context__, TArray<char *> &  __Arr_rename_at_177_21, char * const  __value_rename_at_177_22 )
{
    das_copy(__Arr_rename_at_177_21(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_177_21),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_177_22);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_32d7c798299302f2 ( Context * __context__, TArray<range> &  __Arr_rename_at_177_23, range __value_rename_at_177_24 )
{
    das_copy(__Arr_rename_at_177_23(builtin_array_push_back(das_arg<TArray<range>>::pass(__Arr_rename_at_177_23),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_177_24);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904 ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) &  __it_rename_at_1377_25, uint32_t & __value_rename_at_1377_26 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__it_rename_at_1377_25),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1377_26)),__context__));
}

inline uint32_t re_gen_get_rep_limit_aa71779c47fee5b8 ( Context * __context__ )
{
    return das_auto_cast<uint32_t>::cast(das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/);
}

inline void trace_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_112_27, int32_t __offset_rename_at_112_28 )
{
}

inline void note_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_119_29, int32_t __offset_rename_at_119_30 )
{
}

inline void trace_backtrack_60bdee73e3315732 ( Context * __context__ )
{
}

inline void trace_value_f443ec2d74a0c7bd ( Context * __context__, regex::ReNode * const  __value_rename_at_132_31 )
{
}

inline TVariant<16,8,regex::ReNode *,void *> nada_ba27de56c6f5dc72 ( Context * __context__ )
{
    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((([&]() -> TVariant<16,8,regex::ReNode *,void *> {
        TVariant<16,8,regex::ReNode *,void *> __mkv_144;
        das_get_variant_field<void *,8,1>::set(__mkv_144) = nullptr;
        return __mkv_144;
    })()));
}

inline TVariant<16,8,regex::ReNode *,void *> maybe_249d36fe62a185a7 ( Context * __context__, regex::ReNode * __value_rename_at_147_32 )
{
    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((([&]() -> TVariant<16,8,regex::ReNode *,void *> {
        TVariant<16,8,regex::ReNode *,void *> __mkv_149;
        das_get_variant_field<regex::ReNode *,8,0>::set(__mkv_149) = __value_rename_at_147_32;
        return __mkv_149;
    })()));
}

inline bool eos_b645f5ba09d5c5 ( Context * __context__, char * const  __expr_rename_at_152_33, int32_t __offset_rename_at_152_34 )
{
    return das_auto_cast<bool>::cast(__offset_rename_at_152_34 >= builtin_string_length(__expr_rename_at_152_33,__context__));
}

inline int32_t at_ac9760d67b4716d0 ( Context * __context__, char * const  __expr_rename_at_156_35, int32_t __offset_rename_at_156_36 )
{
    return das_auto_cast<int32_t>::cast(get_character_uat(__expr_rename_at_156_35,__offset_rename_at_156_36));
}

inline int32_t next_6ab20ac9bb2c573c ( Context * __context__, TVariant<16,8,regex::ReNode *,void *> const  &  __re_rename_at_175_37 )
{
    if ( !das_get_variant_field<regex::ReNode *,8,0>::is(__re_rename_at_175_37) )
    {
        builtin_stackwalk(true,true,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_throw(((char *) "expecting value"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<int32_t>::cast(v_extract_yi(v_cast_vec4i(das_get_variant_field<regex::ReNode *,8,0>::as(__re_rename_at_175_37,__context__)->at)) /*y*/);
}

inline bool is_set_empty_1f052aa7f260c4eb ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_183_38 )
{
    {
        bool __need_loop_184 = true;
        // x: uint aka CharSet const&
        das_iterator<TDim<uint32_t,8> const > __x_iterator(__cset_rename_at_183_38);
        uint32_t const  * __x_rename_at_184_39;
        __need_loop_184 = __x_iterator.first(__context__,(__x_rename_at_184_39)) && __need_loop_184;
        for ( ; __need_loop_184 ; __need_loop_184 = __x_iterator.next(__context__,(__x_rename_at_184_39)) )
        {
            if ( (*__x_rename_at_184_39) != 0x0u )
            {
                return das_auto_cast<bool>::cast(false);
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_184_39));
    };
    return das_auto_cast<bool>::cast(true);
}

inline void set_or_char_6485de9cac36fb6f ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_192_40, int32_t __ch_rename_at_192_41 )
{
    __cset_rename_at_192_40((SimPolicy<int32_t>::BinShr(__ch_rename_at_192_41,5,*__context__,nullptr)),__context__) |= uint32_t(SimPolicy<int32_t>::BinShl(1,(__ch_rename_at_192_41 & 31),*__context__,nullptr));
}

inline void set_or_range_eead05ea9800cf6a ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_196_42, range __bits_rename_at_196_43 )
{
    {
        bool __need_loop_197 = true;
        // ch: int const
        das_iterator<range> __ch_iterator(range(v_extract_xi(v_cast_vec4i(__bits_rename_at_196_43)) /*x*/,v_extract_yi(v_cast_vec4i(__bits_rename_at_196_43)) /*y*/ + 1));
        int32_t __ch_rename_at_197_44;
        __need_loop_197 = __ch_iterator.first(__context__,(__ch_rename_at_197_44)) && __need_loop_197;
        for ( ; __need_loop_197 ; __need_loop_197 = __ch_iterator.next(__context__,(__ch_rename_at_197_44)) )
        {
            __cset_rename_at_196_42((SimPolicy<int32_t>::BinShr(__ch_rename_at_197_44,5,*__context__,nullptr)),__context__) |= uint32_t(SimPolicy<int32_t>::BinShl(1,(__ch_rename_at_197_44 & 31),*__context__,nullptr));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_197_44));
    };
}

inline void set_invert_f44d20bc14c46b29 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_202_45 )
{
    {
        bool __need_loop_203 = true;
        // x: uint aka CharSet& -const
        das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_202_45);
        uint32_t * __x_rename_at_203_46;
        __need_loop_203 = __x_iterator.first(__context__,(__x_rename_at_203_46)) && __need_loop_203;
        for ( ; __need_loop_203 ; __need_loop_203 = __x_iterator.next(__context__,(__x_rename_at_203_46)) )
        {
            (*__x_rename_at_203_46) ^= 0xffffffffu;
        }
        __x_iterator.close(__context__,(__x_rename_at_203_46));
    };
}

inline void set_or_set_c5a22cc26b33786b ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_208_47, TDim<uint32_t,8> const  &  __eset_rename_at_208_48 )
{
    {
        bool __need_loop_209 = true;
        // x: uint aka CharSet& -const
        das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_208_47);
        uint32_t * __x_rename_at_209_51;
        __need_loop_209 = __x_iterator.first(__context__,(__x_rename_at_209_51)) && __need_loop_209;
        // y: uint aka CharSet const&
        das_iterator<TDim<uint32_t,8> const > __y_iterator(__eset_rename_at_208_48);
        uint32_t const  * __y_rename_at_209_52;
        __need_loop_209 = __y_iterator.first(__context__,(__y_rename_at_209_52)) && __need_loop_209;
        for ( ; __need_loop_209 ; __need_loop_209 = __x_iterator.next(__context__,(__x_rename_at_209_51)) && __y_iterator.next(__context__,(__y_rename_at_209_52)) )
        {
            (*__x_rename_at_209_51) |= (*__y_rename_at_209_52);
        }
        __y_iterator.close(__context__,(__y_rename_at_209_52));
        __x_iterator.close(__context__,(__x_rename_at_209_51));
    };
}

inline void set_negative_959b5894df7dd660 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_214_53 )
{
    {
        bool __need_loop_215 = true;
        // x: uint aka CharSet& -const
        das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_214_53);
        uint32_t * __x_rename_at_215_54;
        __need_loop_215 = __x_iterator.first(__context__,(__x_rename_at_215_54)) && __need_loop_215;
        for ( ; __need_loop_215 ; __need_loop_215 = __x_iterator.next(__context__,(__x_rename_at_215_54)) )
        {
            das_copy((*__x_rename_at_215_54),~(*__x_rename_at_215_54));
        }
        __x_iterator.close(__context__,(__x_rename_at_215_54));
    };
}

inline void set_meta_c297bca2e11798f6 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_220_55, int32_t __che_rename_at_220_56 )
{
    if ( __che_rename_at_220_56 == 119 )
    {
        set_or_range_eead05ea9800cf6a(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),range(97,122));
        set_or_range_eead05ea9800cf6a(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),range(65,90));
        set_or_range_eead05ea9800cf6a(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),range(48,57));
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),95);
    } else if ( __che_rename_at_220_56 == 87 )
    {
        TDim<uint32_t,8> __eset_rename_at_227_57;das_zero(__eset_rename_at_227_57);
        set_meta_c297bca2e11798f6(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_227_57),119);
        set_negative_959b5894df7dd660(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_227_57));
        set_or_set_c5a22cc26b33786b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_227_57));
    } else if ( __che_rename_at_220_56 == 115 )
    {
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),32);
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),9);
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),10);
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),13);
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),12);
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),11);
    } else if ( __che_rename_at_220_56 == 83 )
    {
        TDim<uint32_t,8> __eset_rename_at_239_58;das_zero(__eset_rename_at_239_58);
        set_meta_c297bca2e11798f6(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_239_58),115);
        set_negative_959b5894df7dd660(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_239_58));
        set_or_set_c5a22cc26b33786b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_239_58));
    } else if ( __che_rename_at_220_56 == 100 )
    {
        set_or_range_eead05ea9800cf6a(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),range(48,57));
    } else if ( __che_rename_at_220_56 == 68 )
    {
        TDim<uint32_t,8> __eset_rename_at_246_59;das_zero(__eset_rename_at_246_59);
        set_meta_c297bca2e11798f6(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_246_59),100);
        set_negative_959b5894df7dd660(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_246_59));
        set_or_set_c5a22cc26b33786b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_246_59));
    } else if ( __che_rename_at_220_56 == 110 )
    {
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),10);
    } else if ( __che_rename_at_220_56 == 114 )
    {
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),13);
    } else if ( __che_rename_at_220_56 == 116 )
    {
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),9);
    } else if ( __che_rename_at_220_56 == 102 )
    {
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),12);
    } else if ( __che_rename_at_220_56 == 118 )
    {
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),11);
    } else {
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_220_55),__che_rename_at_220_56);
    };
}

inline bool is_meta_character_6b868635173399ff ( Context * __context__, int32_t __ch_rename_at_268_60 )
{
    return das_auto_cast<bool>::cast(builtin_find_first_char_of(((char *) "\\+*.()[]|^{}"),__ch_rename_at_268_60,__context__) != -1);
}

inline bool is_set_character_3b302fd2b4140e7f ( Context * __context__, int32_t __ch_rename_at_272_61 )
{
    return das_auto_cast<bool>::cast(builtin_find_first_char_of(((char *) "wWsSdDnrtfvbB"),__ch_rename_at_272_61,__context__) != -1);
}

inline bool is_digit_35d4a4cc9eacb04d ( Context * __context__, int32_t __ch_rename_at_276_62 )
{
    return das_auto_cast<bool>::cast((__ch_rename_at_276_62 >= 48) && (__ch_rename_at_276_62 <= 57));
}

inline bool is_word_character_8422846159268aaf ( Context * __context__, int32_t __ch_rename_at_280_63 )
{
    return das_auto_cast<bool>::cast(((((__ch_rename_at_280_63 >= 97) && (__ch_rename_at_280_63 <= 122)) || ((__ch_rename_at_280_63 >= 65) && (__ch_rename_at_280_63 <= 90))) || ((__ch_rename_at_280_63 >= 48) && (__ch_rename_at_280_63 <= 57))) || (__ch_rename_at_280_63 == 95));
}

inline int32_t from_hex_86a2ef9963e757 ( Context * __context__, int32_t __ch_rename_at_284_64 )
{
    if ( (__ch_rename_at_284_64 >= 48) && (__ch_rename_at_284_64 <= 57) )
    {
        return das_auto_cast<int32_t>::cast(__ch_rename_at_284_64 - 48);
    } else {
        if ( (__ch_rename_at_284_64 >= 97) && (__ch_rename_at_284_64 <= 102) )
        {
            return das_auto_cast<int32_t>::cast((__ch_rename_at_284_64 - 97) + 10);
        } else {
            if ( (__ch_rename_at_284_64 >= 65) && (__ch_rename_at_284_64 <= 70) )
            {
                return das_auto_cast<int32_t>::cast((__ch_rename_at_284_64 - 65) + 10);
            } else {
                builtin_throw(((char *) "from_hex"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                return das_auto_cast<int32_t>::cast(0);
            };
        };
    };
}

inline char * string_from_hex_5698e77bb25d7508 ( Context * __context__, int32_t __ch1_rename_at_302_65 )
{
    return das_auto_cast<char *>::cast(((char * const )(to_string_char(from_hex_86a2ef9963e757(__context__,__ch1_rename_at_302_65),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * string_from_hex_e27d9e88554466f8 ( Context * __context__, int32_t __ch1_rename_at_306_66, int32_t __ch2_rename_at_306_67 )
{
    return das_auto_cast<char *>::cast(((char * const )(to_string_char(_FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc(__context__,__ch1_rename_at_306_66,__ch2_rename_at_306_67),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline TVariant<16,8,regex::ReNode *,void *> re_char_5110fe336617b529 ( Context * __context__, char * const  __expr_rename_at_311_68, int32_t __offset_rename_at_311_69 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_311_68,__offset_rename_at_311_69) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        int32_t __ch_rename_at_316_70 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_311_68,__offset_rename_at_311_69));
        if ( __ch_rename_at_316_70 == 92 )
        {
            int32_t __ech_rename_at_318_71 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_311_68,__offset_rename_at_311_69 + 1));
            if ( __ech_rename_at_318_71 == 0 )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
            } else {
                if ( __ech_rename_at_318_71 == 120 )
                {
                    int32_t __len_rename_at_324_72 = ((int32_t)builtin_string_length(__expr_rename_at_311_68,__context__));
                    if ( (__offset_rename_at_311_69 + 2) >= __len_rename_at_324_72 )
                    {
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                    } else {
                        int32_t __hex1_rename_at_329_73 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_311_68,__offset_rename_at_311_69 + 2));
                        if ( !is_hex(__hex1_rename_at_329_73) )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                        } else {
                            if ( (__offset_rename_at_311_69 + 2) < __len_rename_at_324_72 )
                            {
                                int32_t __hex2_rename_at_335_74 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_311_68,__offset_rename_at_311_69 + 3));
                                if ( is_hex(__hex2_rename_at_335_74) )
                                {
                                    note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_3, cast<char * const >::from(((char *) "hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_329_73,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char * const )(to_string_char(__hex2_rename_at_335_74,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(_FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc(__context__,__hex1_rename_at_329_73,__hex2_rename_at_335_74))))),__offset_rename_at_311_69);
                                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_338;
                                        das_zero(__mks_338);
                                        das_copy((__mks_338.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                        das_copy((__mks_338.text),(string_from_hex_e27d9e88554466f8(__context__,__hex1_rename_at_329_73,__hex2_rename_at_335_74)));
                                        das_copy((__mks_338.textLen),(1));
                                        das_copy((__mks_338.at),(range(__offset_rename_at_311_69,__offset_rename_at_311_69 + 4)));
                                        return __mks_338;
                                    })()))));
                                };
                            };
                            note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_4, cast<char * const >::from(((char *) "hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_329_73,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_86a2ef9963e757(__context__,__hex1_rename_at_329_73))))),__offset_rename_at_311_69);
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_342;
                                das_zero(__mks_342);
                                das_copy((__mks_342.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                das_copy((__mks_342.text),(string_from_hex_5698e77bb25d7508(__context__,__hex1_rename_at_329_73)));
                                das_copy((__mks_342.textLen),(1));
                                das_copy((__mks_342.at),(range(__offset_rename_at_311_69,__offset_rename_at_311_69 + 3)));
                                return __mks_342;
                            })()))));
                        };
                    };
                };
                if ( is_set_character_3b302fd2b4140e7f(__context__,__ech_rename_at_318_71) )
                {
                    if ( __ech_rename_at_318_71 == 98 )
                    {
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_346;
                            das_zero(__mks_346);
                            das_copy((__mks_346.op),(DAS_COMMENT(enum) regex::ReOp::WordBoundary));
                            das_copy((__mks_346.at),(range(__offset_rename_at_311_69,__offset_rename_at_311_69 + 2)));
                            return __mks_346;
                        })()))));
                    } else if ( __ech_rename_at_318_71 == 66 )
                    {
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_348;
                            das_zero(__mks_348);
                            das_copy((__mks_348.op),(DAS_COMMENT(enum) regex::ReOp::NonWordBoundary));
                            das_copy((__mks_348.at),(range(__offset_rename_at_311_69,__offset_rename_at_311_69 + 2)));
                            return __mks_348;
                        })()))));
                    };
                    TDim<uint32_t,8> __cset_rename_at_350_75;das_zero(__cset_rename_at_350_75);
                    set_meta_c297bca2e11798f6(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_350_75),__ech_rename_at_318_71);
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_352;
                        das_zero(__mks_352);
                        das_copy((__mks_352.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_352.cset),(__cset_rename_at_350_75));
                        das_copy((__mks_352.at),(range(__offset_rename_at_311_69,__offset_rename_at_311_69 + 2)));
                        return __mks_352;
                    })()))));
                } else {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_354;
                        das_zero(__mks_354);
                        das_copy((__mks_354.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_354.text),(((char * const )(to_string_char(__ech_rename_at_318_71,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                        das_copy((__mks_354.textLen),(1));
                        das_copy((__mks_354.at),(range(__offset_rename_at_311_69,__offset_rename_at_311_69 + 2)));
                        return __mks_354;
                    })()))));
                };
            };
        } else if ( is_meta_character_6b868635173399ff(__context__,__ch_rename_at_316_70) )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
        };
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_358;
            das_zero(__mks_358);
            das_copy((__mks_358.op),(DAS_COMMENT(enum) regex::ReOp::Char));
            das_copy((__mks_358.text),(((char * const )(to_string_char(__ch_rename_at_316_70,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
            das_copy((__mks_358.textLen),(1));
            das_copy((__mks_358.at),(range(__offset_rename_at_311_69,__offset_rename_at_311_69 + 1)));
            return __mks_358;
        })()))));
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_set_items_ca22db507ac86515 ( Context * __context__, char * const  __expr_rename_at_365_76, int32_t __offset_rename_at_365_77 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_365_76,__offset_rename_at_365_77) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        TDim<uint32_t,8> __cset_rename_at_370_78;das_zero(__cset_rename_at_370_78);
        int32_t __iofs_rename_at_371_79 = __offset_rename_at_365_77;
        int32_t __prev_char_rename_at_372_80 = -1;
        bool __next_range_rename_at_373_81 = false;
        bool __next_meta_rename_at_374_82 = false;
        while ( !eos_b645f5ba09d5c5(__context__,__expr_rename_at_365_76,__iofs_rename_at_371_79) )
        {
            int32_t __ch_rename_at_376_83 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_365_76,__iofs_rename_at_371_79));
            int32_t __next_char_rename_at_377_84 = -1;
            if ( __ch_rename_at_376_83 == 93 )
            {
                break;
            } else if ( __ch_rename_at_376_83 == 92 )
            {
                int32_t __che_rename_at_382_85 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_365_76,__iofs_rename_at_371_79 + 1));
                if ( __che_rename_at_382_85 == 0 )
                {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                } else {
                    if ( __che_rename_at_382_85 == 120 )
                    {
                        int32_t __len_rename_at_388_86 = ((int32_t)builtin_string_length(__expr_rename_at_365_76,__context__));
                        if ( (__iofs_rename_at_371_79 + 2) >= __len_rename_at_388_86 )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                        } else {
                            int32_t __hex1_rename_at_393_87 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_365_76,__iofs_rename_at_371_79 + 2));
                            if ( !is_hex(__hex1_rename_at_393_87) )
                            {
                                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                            } else {
                                if ( (__iofs_rename_at_371_79 + 2) < __len_rename_at_388_86 )
                                {
                                    int32_t __hex2_rename_at_399_88 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_365_76,__iofs_rename_at_371_79 + 3));
                                    if ( is_hex(__hex2_rename_at_399_88) )
                                    {
                                        das_copy(__next_char_rename_at_377_84,_FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc(__context__,__hex1_rename_at_393_87,__hex2_rename_at_399_88));
                                        note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_3, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_393_87,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char * const )(to_string_char(__hex2_rename_at_399_88,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(_FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc(__context__,__hex1_rename_at_393_87,__hex2_rename_at_399_88))))),__iofs_rename_at_371_79);
                                        __iofs_rename_at_371_79 += 4;
                                    } else {
                                        das_copy(__next_char_rename_at_377_84,from_hex_86a2ef9963e757(__context__,__hex1_rename_at_393_87));
                                        note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_4, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_393_87,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_86a2ef9963e757(__context__,__hex1_rename_at_393_87))))),__iofs_rename_at_371_79);
                                        __iofs_rename_at_371_79 += 3;
                                    };
                                } else {
                                    das_copy(__next_char_rename_at_377_84,from_hex_86a2ef9963e757(__context__,__hex1_rename_at_393_87));
                                    note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_4, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_393_87,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_86a2ef9963e757(__context__,__hex1_rename_at_393_87))))),__offset_rename_at_365_77);
                                    __iofs_rename_at_371_79 += 3;
                                };
                            };
                        };
                    } else {
                        if ( __next_range_rename_at_373_81 )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                        } else {
                            if ( is_set_character_3b302fd2b4140e7f(__context__,__che_rename_at_382_85) )
                            {
                                das_copy(__next_char_rename_at_377_84,-1);
                                das_copy(__next_meta_rename_at_374_82,true);
                                set_meta_c297bca2e11798f6(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_370_78),__che_rename_at_382_85);
                            } else {
                                das_copy(__next_char_rename_at_377_84,__che_rename_at_382_85);
                            };
                            __iofs_rename_at_371_79 += 2;
                        };
                    };
                };
            } else if ( __ch_rename_at_376_83 == 45 )
            {
                if ( __prev_char_rename_at_372_80 == -1 )
                {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                } else {
                    das_copy(__next_range_rename_at_373_81,true);
                    __iofs_rename_at_371_79 += 1;
                };
            } else {
                das_copy(__next_char_rename_at_377_84,__ch_rename_at_376_83);
                __iofs_rename_at_371_79 += 1;
            };
            if ( __next_char_rename_at_377_84 != -1 )
            {
                if ( __next_range_rename_at_373_81 )
                {
                    set_or_range_eead05ea9800cf6a(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_370_78),range(__prev_char_rename_at_372_80,__next_char_rename_at_377_84));
                    das_copy(__next_range_rename_at_373_81,false);
                    das_copy(__prev_char_rename_at_372_80,-1);
                } else {
                    set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_370_78),__next_char_rename_at_377_84);
                    das_copy(__prev_char_rename_at_372_80,__next_char_rename_at_377_84);
                };
            } else if ( __next_meta_rename_at_374_82 )
            {
                das_copy(__next_meta_rename_at_374_82,false);
            } else if ( !__next_range_rename_at_373_81 )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
            };
        };
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_461;
            das_zero(__mks_461);
            das_copy((__mks_461.op),(DAS_COMMENT(enum) regex::ReOp::Set));
            das_copy((__mks_461.cset),(__cset_rename_at_370_78));
            das_copy((__mks_461.at),(range(__offset_rename_at_365_77,__iofs_rename_at_371_79)));
            return __mks_461;
        })()))));
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_set_3642c8eadc1c1fb5 ( Context * __context__, char * const  __expr_rename_at_467_89, int32_t __offset_rename_at_467_90 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_467_89,__offset_rename_at_467_90) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_467_89,__offset_rename_at_467_90) != 91 )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
        } else {
            bool __negative_rename_at_475_91 = false;
            if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_467_89,__offset_rename_at_467_90 + 1) == 94 )
            {
                das_copy(__negative_rename_at_475_91,true);
            };
            TVariant<16,8,regex::ReNode *,void *> __oset_rename_at_479_92 = re_set_items_ca22db507ac86515(__context__,__expr_rename_at_467_89,__negative_rename_at_475_91 ? das_auto_cast<int32_t>::cast((__offset_rename_at_467_90 + 2)) : das_auto_cast<int32_t>::cast((__offset_rename_at_467_90 + 1)));
            if ( das_get_variant_field<void *,8,1>::is(__oset_rename_at_479_92) )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
            } else {
                if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_467_89,next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oset_rename_at_479_92))) != 93 )
                {
                    finalize_4c4d68a9a950ec41(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oset_rename_at_479_92));
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                } else {
                    regex::ReNode * __oval_rename_at_489_93 = das_get_variant_field<regex::ReNode *,8,0>::as(__oset_rename_at_479_92,__context__);
                    das_copy(das_swizzle_ref<int32_t,range,0>::swizzle(__oval_rename_at_489_93->at) /*x*/,__offset_rename_at_467_90);
                    ++das_swizzle_ref<int32_t,range,1>::swizzle(__oval_rename_at_489_93->at) /*y*/;
                    das_copy(__oval_rename_at_489_93->op,DAS_COMMENT(enum) regex::ReOp::Set);
                    if ( __negative_rename_at_475_91 )
                    {
                        set_negative_959b5894df7dd660(__context__,das_arg<TDim<uint32_t,8>>::pass(__oval_rename_at_489_93->cset));
                    };
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oset_rename_at_479_92);
                };
            };
        };
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_any_d551e389d6ffd28 ( Context * __context__, char * const  __expr_rename_at_501_94, int32_t __offset_rename_at_501_95 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_501_94,__offset_rename_at_501_95) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_501_94,__offset_rename_at_501_95) == 46 )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_507;
                das_zero(__mks_507);
                das_copy((__mks_507.op),(DAS_COMMENT(enum) regex::ReOp::Any));
                das_copy((__mks_507.at),(range(__offset_rename_at_501_95,__offset_rename_at_501_95 + 1)));
                return __mks_507;
            })()))));
        } else {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
        };
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_eos_3dff18197056692d ( Context * __context__, char * const  __expr_rename_at_513_96, int32_t __offset_rename_at_513_97 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_513_96,__offset_rename_at_513_97) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_513_96,__offset_rename_at_513_97) == 36 )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_519;
                das_zero(__mks_519);
                das_copy((__mks_519.op),(DAS_COMMENT(enum) regex::ReOp::Eos));
                das_copy((__mks_519.at),(range(__offset_rename_at_513_97,__offset_rename_at_513_97 + 1)));
                return __mks_519;
            })()))));
        } else {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
        };
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_bos_939db26d2fdba5e2 ( Context * __context__, char * const  __expr_rename_at_525_98, int32_t __offset_rename_at_525_99 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_525_98,__offset_rename_at_525_99) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_525_98,__offset_rename_at_525_99) == 94 )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_531;
                das_zero(__mks_531);
                das_copy((__mks_531.op),(DAS_COMMENT(enum) regex::ReOp::Bos));
                das_copy((__mks_531.at),(range(__offset_rename_at_525_99,__offset_rename_at_525_99 + 1)));
                return __mks_531;
            })()))));
        } else {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
        };
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_group_17f1ffc0bc11a16 ( Context * __context__, char * const  __expr_rename_at_537_100, int32_t __offset_rename_at_537_101 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_537_100,__offset_rename_at_537_101) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_537_100,__offset_rename_at_537_101) != 40 )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
        } else {
            int32_t __group_offset_rename_at_545_102 = (__offset_rename_at_537_101 + 1);
            bool __non_capturing_rename_at_546_103 = false;
            char * __group_name_rename_at_547_104 = (char *)(nullptr);
            DAS_COMMENT(enum) regex::ReOp __lookahead_op_rename_at_548_105 = DAS_COMMENT(enum) regex::ReOp::Group;
            if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102) == 63) )
            {
                if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 1)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 1) == 58) )
                {
                    das_copy(__non_capturing_rename_at_546_103,true);
                    __group_offset_rename_at_545_102 += 2;
                } else if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 1)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 1) == 61) )
                {
                    das_copy(__lookahead_op_rename_at_548_105,DAS_COMMENT(enum) regex::ReOp::Lookahead);
                    __group_offset_rename_at_545_102 += 2;
                } else if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 1)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 1) == 33) )
                {
                    das_copy(__lookahead_op_rename_at_548_105,DAS_COMMENT(enum) regex::ReOp::NegativeLookahead);
                    __group_offset_rename_at_545_102 += 2;
                } else if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 1)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 1) == 80) )
                {
                    if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 2)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102 + 2) == 60) )
                    {
                        int32_t __name_start_rename_at_562_106 = (__group_offset_rename_at_545_102 + 3);
                        int32_t __name_end_rename_at_563_107 = __name_start_rename_at_562_106;
                        while ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_537_100,__name_end_rename_at_563_107)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_537_100,__name_end_rename_at_563_107) != 62) )
                        {
                            ++__name_end_rename_at_563_107;
                        };
                        if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_537_100,__name_end_rename_at_563_107) )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                        } else {
                            das_copy(__group_name_rename_at_547_104,((char * const )(builtin_string_slice1(__expr_rename_at_537_100,__name_start_rename_at_562_106,__name_end_rename_at_563_107,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
                            das_copy(__group_offset_rename_at_545_102,__name_end_rename_at_563_107 + 1);
                        };
                    };
                };
            };
            TVariant<16,8,regex::ReNode *,void *> __ore_rename_at_576_108 = re_re_12ab55bce538b67e(__context__,__expr_rename_at_537_100,__group_offset_rename_at_545_102);
            if ( das_get_variant_field<void *,8,1>::is(__ore_rename_at_576_108) )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
            } else {
                if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_537_100,next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__ore_rename_at_576_108))) != 41 )
                {
                    finalize_4c4d68a9a950ec41(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__ore_rename_at_576_108));
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                } else {
                    if ( __lookahead_op_rename_at_548_105 != DAS_COMMENT(enum) regex::ReOp::Group )
                    {
                        regex::ReNode * __node_rename_at_585_109 = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_585;
                            das_zero(__mks_585);
                            das_copy((__mks_585.op),(__lookahead_op_rename_at_548_105));
                            das_copy((__mks_585.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__ore_rename_at_576_108,__context__)));
                            das_copy((__mks_585.at),(range(__offset_rename_at_537_101,next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__ore_rename_at_576_108)) + 1)));
                            return __mks_585;
                        })()));
                        das_copy(__node_rename_at_585_109->index,-1);
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,__node_rename_at_585_109));
                    } else {
                        regex::ReNode * __node_rename_at_589_110 = das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_589;
                            das_zero(__mks_589);
                            das_copy((__mks_589.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                            das_copy((__mks_589.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__ore_rename_at_576_108,__context__)));
                            das_copy((__mks_589.at),(range(__offset_rename_at_537_101,next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__ore_rename_at_576_108)) + 1)));
                            return __mks_589;
                        })()));
                        if ( __non_capturing_rename_at_546_103 )
                        {
                            das_copy(__node_rename_at_589_110->index,-1);
                        };
                        if ( !builtin_empty(__group_name_rename_at_547_104) )
                        {
                            das_copy(__node_rename_at_589_110->text,__group_name_rename_at_547_104);
                        };
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,__node_rename_at_589_110));
                    };
                };
            };
        };
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_elementary_409594d3d2b7cfa3 ( Context * __context__, char * const  __expr_rename_at_600_111, int32_t __offset_rename_at_600_112 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_600_111,__offset_rename_at_600_112) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        TVariant<16,8,regex::ReNode *,void *> __ogr_rename_at_605_113 = re_group_17f1ffc0bc11a16(__context__,__expr_rename_at_600_111,__offset_rename_at_600_112);
        if ( das_get_variant_field<regex::ReNode *,8,0>::is(__ogr_rename_at_605_113) )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__ogr_rename_at_605_113);
        } else {
            TVariant<16,8,regex::ReNode *,void *> __oany_rename_at_609_114 = re_any_d551e389d6ffd28(__context__,__expr_rename_at_600_111,__offset_rename_at_600_112);
            if ( das_get_variant_field<regex::ReNode *,8,0>::is(__oany_rename_at_609_114) )
            {
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oany_rename_at_609_114);
            } else {
                TVariant<16,8,regex::ReNode *,void *> __oeos_rename_at_613_115 = re_eos_3dff18197056692d(__context__,__expr_rename_at_600_111,__offset_rename_at_600_112);
                if ( das_get_variant_field<regex::ReNode *,8,0>::is(__oeos_rename_at_613_115) )
                {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oeos_rename_at_613_115);
                } else {
                    TVariant<16,8,regex::ReNode *,void *> __obos_rename_at_617_116 = re_bos_939db26d2fdba5e2(__context__,__expr_rename_at_600_111,__offset_rename_at_600_112);
                    if ( das_get_variant_field<regex::ReNode *,8,0>::is(__obos_rename_at_617_116) )
                    {
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__obos_rename_at_617_116);
                    } else {
                        TVariant<16,8,regex::ReNode *,void *> __oset_rename_at_621_117 = re_set_3642c8eadc1c1fb5(__context__,__expr_rename_at_600_111,__offset_rename_at_600_112);
                        if ( das_get_variant_field<regex::ReNode *,8,0>::is(__oset_rename_at_621_117) )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oset_rename_at_621_117);
                        } else {
                            TVariant<16,8,regex::ReNode *,void *> __ochr_rename_at_625_118 = re_char_5110fe336617b529(__context__,__expr_rename_at_600_111,__offset_rename_at_600_112);
                            if ( das_get_variant_field<regex::ReNode *,8,0>::is(__ochr_rename_at_625_118) )
                            {
                                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__ochr_rename_at_625_118);
                            } else {
                                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                            };
                        };
                    };
                };
            };
        };
    };
}

inline regex::ReNode * mk_concat_53dbf44b1354a65 ( Context * __context__, regex::ReNode * __left_rename_at_632_119, regex::ReNode * __right_rename_at_632_120 )
{
    if ( __left_rename_at_632_119 == nullptr )
    {
        return das_auto_cast<regex::ReNode *>::cast(__right_rename_at_632_120);
    } else if ( (__left_rename_at_632_119->op == DAS_COMMENT(enum) regex::ReOp::Char) && (__right_rename_at_632_120->op == DAS_COMMENT(enum) regex::ReOp::Char) )
    {
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_632_119->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_632_120->at) /*y*/);
        SimPolicy<char *>::SetAdd((char *)&(__left_rename_at_632_119->text),cast<char *>::from(__right_rename_at_632_120->text),*__context__,nullptr);
        __left_rename_at_632_119->textLen += __right_rename_at_632_120->textLen;
        finalize_c680337a54511d24(__context__,__right_rename_at_632_120);
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_632_119);
    } else if ( ((__left_rename_at_632_119->op == DAS_COMMENT(enum) regex::ReOp::Concat) && (__left_rename_at_632_119->right->op == DAS_COMMENT(enum) regex::ReOp::Char)) && (__right_rename_at_632_120->op == DAS_COMMENT(enum) regex::ReOp::Char) )
    {
        regex::ReNode * __lor_rename_at_646_121 = __left_rename_at_632_119->right;
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_632_119->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_632_120->at) /*y*/);
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__lor_rename_at_646_121->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_632_120->at) /*y*/);
        SimPolicy<char *>::SetAdd((char *)&(__lor_rename_at_646_121->text),cast<char *>::from(__right_rename_at_632_120->text),*__context__,nullptr);
        __lor_rename_at_646_121->textLen += __right_rename_at_632_120->textLen;
        finalize_c680337a54511d24(__context__,__right_rename_at_632_120);
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_632_119);
    } else {
        return das_auto_cast<regex::ReNode *>::cast(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_656;
            das_zero(__mks_656);
            das_copy((__mks_656.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_656.left),(__left_rename_at_632_119));
            das_copy((__mks_656.right),(__right_rename_at_632_120));
            das_copy((__mks_656.at),(range(v_extract_xi(v_cast_vec4i(__left_rename_at_632_119->at)) /*x*/,v_extract_yi(v_cast_vec4i(__right_rename_at_632_120->at)) /*y*/)));
            return __mks_656;
        })())));
    };
}

inline regex::ReNode * mk_union_b8290c88c89cea8b ( Context * __context__, regex::ReNode * __left_rename_at_660_122, regex::ReNode * __right_rename_at_660_123 )
{
    if ( __left_rename_at_660_122->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_660_122->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_660_123->at) /*y*/);
        if ( __right_rename_at_660_123->op == DAS_COMMENT(enum) regex::ReOp::Union )
        {
            {
                bool __need_loop_664 = true;
                // x: regex::ReNode?&
                das_iterator<TArray<regex::ReNode *>> __x_iterator(__right_rename_at_660_123->all);
                regex::ReNode * * __x_rename_at_664_124;
                __need_loop_664 = __x_iterator.first(__context__,(__x_rename_at_664_124)) && __need_loop_664;
                for ( ; __need_loop_664 ; __need_loop_664 = __x_iterator.next(__context__,(__x_rename_at_664_124)) )
                {
                    _FuncbuiltinTickpushTick10769833213962245646_906d8abb52edd211(__context__,das_arg<TArray<regex::ReNode *>>::pass(__left_rename_at_660_122->all),(*__x_rename_at_664_124));
                }
                __x_iterator.close(__context__,(__x_rename_at_664_124));
            };
            finalize_c680337a54511d24(__context__,__right_rename_at_660_123);
        } else {
            _FuncbuiltinTickpushTick10769833213962245646_906d8abb52edd211(__context__,das_arg<TArray<regex::ReNode *>>::pass(__left_rename_at_660_122->all),__right_rename_at_660_123);
        };
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_660_122);
    } else if ( __right_rename_at_660_123->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        das_copy(das_swizzle_ref<int32_t,range,0>::swizzle(__right_rename_at_660_123->at) /*x*/,das_swizzle_ref<int32_t,range,0>::swizzle(__left_rename_at_660_122->at) /*x*/);
        _FuncbuiltinTickpushTick10769833213962245646_906d8abb52edd211(__context__,das_arg<TArray<regex::ReNode *>>::pass(__right_rename_at_660_123->all),__left_rename_at_660_122);
        return das_auto_cast<regex::ReNode *>::cast(__right_rename_at_660_123);
    } else {
        TDim<regex::ReNode *,2> _temp_make_local_679_50_0; _temp_make_local_679_50_0;
        return das_auto_cast<regex::ReNode *>::cast(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_679;
            das_zero(__mks_679);
            das_copy((__mks_679.op),(DAS_COMMENT(enum) regex::ReOp::Union));
            das_move((__mks_679.all),(_FuncbuiltinTickto_array_moveTick3185538323411982277_1d04d83c4b41cf2c(__context__,das_arg<TDim<regex::ReNode *,2>>::pass((([&]() -> TDim<regex::ReNode *,2>& {
                _temp_make_local_679_50_0(0,__context__) = __left_rename_at_660_122;
                _temp_make_local_679_50_0(1,__context__) = __right_rename_at_660_123;
                return _temp_make_local_679_50_0;
            })())))));
            das_copy((__mks_679.at),(range(v_extract_xi(v_cast_vec4i(__left_rename_at_660_122->at)) /*x*/,v_extract_yi(v_cast_vec4i(__right_rename_at_660_123->at)) /*y*/)));
            return __mks_679;
        })())));
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_re_12ab55bce538b67e ( Context * __context__, char * const  __expr_rename_at_687_125, int32_t __offset_rename_at_687_126 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_687_125,__offset_rename_at_687_126) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        int32_t __cofs_rename_at_692_127 = __offset_rename_at_687_126;
        regex::ReNode * __last_rename_at_693_128 = 0;
        while ( !eos_b645f5ba09d5c5(__context__,__expr_rename_at_687_125,__cofs_rename_at_692_127) )
        {
            if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_687_125,__cofs_rename_at_692_127) == 124 )
            {
                if ( __last_rename_at_693_128 == nullptr )
                {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                } else {
                    TVariant<16,8,regex::ReNode *,void *> __oright_rename_at_700_129 = re_re_12ab55bce538b67e(__context__,__expr_rename_at_687_125,__cofs_rename_at_692_127 + 1);
                    if ( das_get_variant_field<void *,8,1>::is(__oright_rename_at_700_129) )
                    {
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                    } else {
                        das_copy(__last_rename_at_693_128,mk_union_b8290c88c89cea8b(__context__,__last_rename_at_693_128,das_get_variant_field<regex::ReNode *,8,0>::as(__oright_rename_at_700_129,__context__)));
                        das_copy(__cofs_rename_at_692_127,das_swizzle_ref<int32_t,range,1>::swizzle(__last_rename_at_693_128->at) /*y*/);
                    };
                };
            } else {
                TVariant<16,8,regex::ReNode *,void *> __onext_rename_at_708_130 = re_basic_3aca537b6901f1ec(__context__,__expr_rename_at_687_125,__cofs_rename_at_692_127);
                if ( das_get_variant_field<void *,8,1>::is(__onext_rename_at_708_130) )
                {
                    TVariant<16,8,regex::ReNode *,void *> _temp_make_local_710_38_1; _temp_make_local_710_38_1;
                    TVariant<16,8,regex::ReNode *,void *> _temp_make_local_710_47_2; _temp_make_local_710_47_2;
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((__last_rename_at_693_128 == nullptr) ? das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((_temp_make_local_710_38_1 = (nada_ba27de56c6f5dc72(__context__)))) : das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast((_temp_make_local_710_47_2 = (maybe_249d36fe62a185a7(__context__,__last_rename_at_693_128)))));
                } else {
                    das_copy(__last_rename_at_693_128,mk_concat_53dbf44b1354a65(__context__,__last_rename_at_693_128,das_get_variant_field<regex::ReNode *,8,0>::as(__onext_rename_at_708_130,__context__)));
                    das_copy(__cofs_rename_at_692_127,das_swizzle_ref<int32_t,range,1>::swizzle(__last_rename_at_693_128->at) /*y*/);
                };
            };
        };
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,__last_rename_at_693_128));
    };
}

inline TVariant<16,8,regex::ReNode *,void *> re_basic_3aca537b6901f1ec ( Context * __context__, char * const  __expr_rename_at_724_131, int32_t __offset_rename_at_724_132 )
{
    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__offset_rename_at_724_132) )
    {
        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
    } else {
        TVariant<16,8,regex::ReNode *,void *> __oelem_rename_at_729_133 = re_elementary_409594d3d2b7cfa3(__context__,__expr_rename_at_724_131,__offset_rename_at_724_132);
        if ( das_get_variant_field<void *,8,1>::is(__oelem_rename_at_729_133) )
        {
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
        } else {
            int32_t __ch_rename_at_733_134 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_729_133))));
            if ( (((__ch_rename_at_733_134 == 42) || (__ch_rename_at_733_134 == 43)) || (__ch_rename_at_733_134 == 63)) || (__ch_rename_at_733_134 == 123) )
            {
                DAS_COMMENT(enum) regex::ReOp __eop_rename_at_736_135 = ((DAS_COMMENT(enum) regex::ReOp)das_get_variant_field<regex::ReNode *,8,0>::as(__oelem_rename_at_729_133,__context__)->op);
                if ( (__eop_rename_at_736_135 == DAS_COMMENT(enum) regex::ReOp::Lookahead) || (__eop_rename_at_736_135 == DAS_COMMENT(enum) regex::ReOp::NegativeLookahead) )
                {
                    note_7794f549fda55dde(__context__,((char *) "quantifier on lookahead is not allowed"),next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_729_133)));
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                };
            };
            if ( __ch_rename_at_733_134 == 42 )
            {
                int32_t __end_ofs_rename_at_743_136 = (next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_729_133)) + 1);
                bool __is_lazy_rename_at_744_137 = false;
                if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__end_ofs_rename_at_743_136)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__end_ofs_rename_at_743_136) == 63) )
                {
                    das_copy(__is_lazy_rename_at_744_137,true);
                    ++__end_ofs_rename_at_743_136;
                };
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_749;
                    das_zero(__mks_749);
                    das_copy((__mks_749.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                    das_copy((__mks_749.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__oelem_rename_at_729_133,__context__)));
                    das_copy((__mks_749.lazy),(__is_lazy_rename_at_744_137));
                    das_copy((__mks_749.at),(range(__offset_rename_at_724_132,__end_ofs_rename_at_743_136)));
                    return __mks_749;
                })()))));
            } else if ( __ch_rename_at_733_134 == 43 )
            {
                int32_t __end_ofs_rename_at_751_138 = (next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_729_133)) + 1);
                bool __is_lazy_rename_at_752_139 = false;
                if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__end_ofs_rename_at_751_138)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__end_ofs_rename_at_751_138) == 63) )
                {
                    das_copy(__is_lazy_rename_at_752_139,true);
                    ++__end_ofs_rename_at_751_138;
                };
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_757;
                    das_zero(__mks_757);
                    das_copy((__mks_757.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                    das_copy((__mks_757.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__oelem_rename_at_729_133,__context__)));
                    das_copy((__mks_757.lazy),(__is_lazy_rename_at_752_139));
                    das_copy((__mks_757.at),(range(__offset_rename_at_724_132,__end_ofs_rename_at_751_138)));
                    return __mks_757;
                })()))));
            } else if ( __ch_rename_at_733_134 == 63 )
            {
                int32_t __end_ofs_rename_at_759_140 = (next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_729_133)) + 1);
                bool __is_lazy_rename_at_760_141 = false;
                if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__end_ofs_rename_at_759_140)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__end_ofs_rename_at_759_140) == 63) )
                {
                    das_copy(__is_lazy_rename_at_760_141,true);
                    ++__end_ofs_rename_at_759_140;
                };
                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_765;
                    das_zero(__mks_765);
                    das_copy((__mks_765.op),(DAS_COMMENT(enum) regex::ReOp::Question));
                    das_copy((__mks_765.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__oelem_rename_at_729_133,__context__)));
                    das_copy((__mks_765.lazy),(__is_lazy_rename_at_760_141));
                    das_copy((__mks_765.at),(range(__offset_rename_at_724_132,__end_ofs_rename_at_759_140)));
                    return __mks_765;
                })()))));
            } else if ( __ch_rename_at_733_134 == 123 )
            {
                int32_t __rofs_rename_at_768_142 = (next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__oelem_rename_at_729_133)) + 1);
                int32_t __min_val_rename_at_769_143 = 0;
                int32_t __max_val_rename_at_770_144 = 0;
                bool __has_comma_rename_at_771_145 = false;
                bool __has_max_rename_at_772_146 = false;
                if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142) || !(is_digit_35d4a4cc9eacb04d(__context__,at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142))) )
                {
                    return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                } else {
                    while ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142)) && is_digit_35d4a4cc9eacb04d(__context__,at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142)) )
                    {
                        das_copy(__min_val_rename_at_769_143,(__min_val_rename_at_769_143 * 10) + (at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142) - 48));
                        ++__rofs_rename_at_768_142;
                    };
                    if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142) )
                    {
                        return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                    } else {
                        int32_t __nch_rename_at_786_147 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142));
                        if ( __nch_rename_at_786_147 == 44 )
                        {
                            das_copy(__has_comma_rename_at_771_145,true);
                            ++__rofs_rename_at_768_142;
                            if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142) )
                            {
                                return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                            } else {
                                if ( is_digit_35d4a4cc9eacb04d(__context__,at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142)) )
                                {
                                    das_copy(__has_max_rename_at_772_146,true);
                                    while ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142)) && is_digit_35d4a4cc9eacb04d(__context__,at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142)) )
                                    {
                                        das_copy(__max_val_rename_at_770_144,(__max_val_rename_at_770_144 * 10) + (at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142) - 48));
                                        ++__rofs_rename_at_768_142;
                                    };
                                };
                            };
                        };
                        if ( eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142) || (at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142) != 125) )
                        {
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(nada_ba27de56c6f5dc72(__context__));
                        } else {
                            ++__rofs_rename_at_768_142;
                            if ( !__has_comma_rename_at_771_145 )
                            {
                                das_copy(__max_val_rename_at_770_144,__min_val_rename_at_769_143);
                            } else if ( !__has_max_rename_at_772_146 )
                            {
                                das_copy(__max_val_rename_at_770_144,-1);
                            };
                            bool __is_lazy_rename_at_815_148 = false;
                            if ( !(eos_b645f5ba09d5c5(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142)) && (at_ac9760d67b4716d0(__context__,__expr_rename_at_724_131,__rofs_rename_at_768_142) == 63) )
                            {
                                das_copy(__is_lazy_rename_at_815_148,true);
                                ++__rofs_rename_at_768_142;
                            };
                            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(maybe_249d36fe62a185a7(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_820;
                                das_zero(__mks_820);
                                das_copy((__mks_820.op),(DAS_COMMENT(enum) regex::ReOp::Repeat));
                                das_copy((__mks_820.subexpr),(das_get_variant_field<regex::ReNode *,8,0>::as(__oelem_rename_at_729_133,__context__)));
                                das_copy((__mks_820.min_rep),(__min_val_rename_at_769_143));
                                das_copy((__mks_820.max_rep),(__max_val_rename_at_770_144));
                                das_copy((__mks_820.lazy),(__is_lazy_rename_at_815_148));
                                das_copy((__mks_820.at),(range(__offset_rename_at_724_132,__rofs_rename_at_768_142)));
                                return __mks_820;
                            })()))));
                        };
                    };
                };
            };
            return das_auto_cast_ref<TVariant<16,8,regex::ReNode *,void *>>::cast(__oelem_rename_at_729_133);
        };
    };
}

inline regex::ReNode * re_parse_be404de54055d495 ( Context * __context__, char * const  __expr_rename_at_825_149 )
{
    TVariant<16,8,regex::ReNode *,void *> __ore_rename_at_826_150 = re_re_12ab55bce538b67e(__context__,__expr_rename_at_825_149,0);
    if ( das_get_variant_field<void *,8,1>::is(__ore_rename_at_826_150) )
    {
        return das_auto_cast<regex::ReNode *>::cast(nullptr);
    } else {
        int32_t __at_rename_at_830_151 = ((int32_t)next_6ab20ac9bb2c573c(__context__,das_arg<TVariant<16,8,regex::ReNode *,void *>>::pass(__ore_rename_at_826_150)));
        return das_auto_cast<regex::ReNode *>::cast((__at_rename_at_830_151 != builtin_string_length(__expr_rename_at_825_149,__context__)) ? das_auto_cast<void *>::cast(nullptr) : das_auto_cast<void *>::cast(das_get_variant_field<regex::ReNode *,8,0>::as(__ore_rename_at_826_150,__context__)));
    };
}

inline void visit_top_down_28b61d33d98f34cc ( Context * __context__, regex::ReNode * __node_rename_at_838_152, Block DAS_COMMENT((void,regex::ReNode *)) const  &  __blk_rename_at_838_153 )
{
    das_invoke<void>::invoke<regex::ReNode *>(__context__,nullptr,__blk_rename_at_838_153,__node_rename_at_838_152);
    {
        bool __need_loop_841 = true;
        // x: regex::ReNode?&
        das_iterator<TArray<regex::ReNode *>> __x_iterator(__node_rename_at_838_152->all);
        regex::ReNode * * __x_rename_at_841_154;
        __need_loop_841 = __x_iterator.first(__context__,(__x_rename_at_841_154)) && __need_loop_841;
        for ( ; __need_loop_841 ; __need_loop_841 = __x_iterator.next(__context__,(__x_rename_at_841_154)) )
        {
            visit_top_down_28b61d33d98f34cc(__context__,(*__x_rename_at_841_154),__blk_rename_at_838_153);
        }
        __x_iterator.close(__context__,(__x_rename_at_841_154));
    };
    if ( __node_rename_at_838_152->subexpr != nullptr )
    {
        visit_top_down_28b61d33d98f34cc(__context__,__node_rename_at_838_152->subexpr,__blk_rename_at_838_153);
    };
    if ( __node_rename_at_838_152->left != nullptr )
    {
        visit_top_down_28b61d33d98f34cc(__context__,__node_rename_at_838_152->left,__blk_rename_at_838_153);
    };
    if ( __node_rename_at_838_152->right != nullptr )
    {
        visit_top_down_28b61d33d98f34cc(__context__,__node_rename_at_838_152->right,__blk_rename_at_838_153);
    };
}

inline void re_assign_next_5d4c1e040adf8167 ( Context * __context__, regex::Regex &  __re_rename_at_855_155 ) { das_stack_prologue __prologue(__context__,128,"re_assign_next " DAS_FILE_LINE);
{
    int32_t __id_rename_at_856_156 = 0;
    visit_top_down_28b61d33d98f34cc(__context__,__re_rename_at_855_155.root,das_make_block<void,regex::ReNode *>(__context__,96,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_857_157) -> void{
        das_copy(__node_rename_at_857_157->id,__id_rename_at_856_156++);
        if ( __node_rename_at_857_157->op == DAS_COMMENT(enum) regex::ReOp::Concat )
        {
            das_copy(__node_rename_at_857_157->left->next,__node_rename_at_857_157->right);
            das_copy(__node_rename_at_857_157->right->next,__node_rename_at_857_157->next);
        } else if ( __node_rename_at_857_157->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            das_copy(__node_rename_at_857_157->subexpr->next,__node_rename_at_857_157->next);
        } else if ( (__node_rename_at_857_157->op == DAS_COMMENT(enum) regex::ReOp::Lookahead) || (__node_rename_at_857_157->op == DAS_COMMENT(enum) regex::ReOp::NegativeLookahead) )
        {
            das_copy(__node_rename_at_857_157->subexpr->next,nullptr);
        } else if ( __node_rename_at_857_157->op == DAS_COMMENT(enum) regex::ReOp::Union )
        {
            {
                bool __need_loop_867 = true;
                // sub: regex::ReNode?&
                das_iterator<TArray<regex::ReNode *>> __sub_iterator(__node_rename_at_857_157->all);
                regex::ReNode * * __sub_rename_at_867_158;
                __need_loop_867 = __sub_iterator.first(__context__,(__sub_rename_at_867_158)) && __need_loop_867;
                for ( ; __need_loop_867 ; __need_loop_867 = __sub_iterator.next(__context__,(__sub_rename_at_867_158)) )
                {
                    das_copy((*__sub_rename_at_867_158)->next,__node_rename_at_857_157->next);
                }
                __sub_iterator.close(__context__,(__sub_rename_at_867_158));
            };
        };
    }));
}}

inline void re_assign_groups_3ba808af5f6a0682 ( Context * __context__, regex::Regex &  __re_rename_at_874_159 ) { das_stack_prologue __prologue(__context__,144,"re_assign_groups " DAS_FILE_LINE);
{
    TTuple<16,range,char *> _temp_make_local_875_21_3; _temp_make_local_875_21_3;
    _FuncbuiltinTickpushTick10769833213962245646_7df733d16209a68a(__context__,das_arg<TArray<TTuple<16,range,char *>>>::pass(__re_rename_at_874_159.groups),das_arg<TTuple<16,range,char *>>::pass((([&]() -> TTuple<16,range,char *>& {
        das_get_tuple_field<range,0>::get(_temp_make_local_875_21_3) = range(0,0);
        das_get_tuple_field<char *,8>::get(_temp_make_local_875_21_3) = nullptr;
        return _temp_make_local_875_21_3;
    })())));
    visit_top_down_28b61d33d98f34cc(__context__,__re_rename_at_874_159.root,das_make_block<void,regex::ReNode *>(__context__,80,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_876_160) -> void{
        if ( __node_rename_at_876_160->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            if ( __node_rename_at_876_160->index == -1 )
            {
                return ;
            } else {
                int32_t __index_rename_at_881_161 = ((int32_t)builtin_array_size(das_arg<TArray<TTuple<16,range,char *>>>::pass(__re_rename_at_874_159.groups)));
                das_copy(__node_rename_at_876_160->index,__index_rename_at_881_161);
                if ( !builtin_empty(__node_rename_at_876_160->text) )
                {
                    TTuple<16,range,char *> _temp_make_local_884_33_4; _temp_make_local_884_33_4;
                    _FuncbuiltinTickpushTick10769833213962245646_7df733d16209a68a(__context__,das_arg<TArray<TTuple<16,range,char *>>>::pass(__re_rename_at_874_159.groups),das_arg<TTuple<16,range,char *>>::pass((([&]() -> TTuple<16,range,char *>& {
                        das_get_tuple_field<range,0>::get(_temp_make_local_884_33_4) = range(0,0);
                        das_get_tuple_field<char *,8>::get(_temp_make_local_884_33_4) = __node_rename_at_876_160->text;
                        return _temp_make_local_884_33_4;
                    })())));
                } else {
                    TTuple<16,range,char *> _temp_make_local_886_33_5; _temp_make_local_886_33_5;
                    _FuncbuiltinTickpushTick10769833213962245646_7df733d16209a68a(__context__,das_arg<TArray<TTuple<16,range,char *>>>::pass(__re_rename_at_874_159.groups),das_arg<TTuple<16,range,char *>>::pass((([&]() -> TTuple<16,range,char *>& {
                        das_get_tuple_field<range,0>::get(_temp_make_local_886_33_5) = range(0,0);
                        das_get_tuple_field<char *,8>::get(_temp_make_local_886_33_5) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_8, cast<int32_t>::from(__index_rename_at_881_161)));
                        return _temp_make_local_886_33_5;
                    })())));
                };
            };
        };
    }));
}}

inline void re_assign_match_functions_96c5fc406200914c ( Context * __context__, regex::Regex &  __re_rename_at_896_162 ) { das_stack_prologue __prologue(__context__,96,"re_assign_match_functions " DAS_FILE_LINE);
{
    visit_top_down_28b61d33d98f34cc(__context__,__re_rename_at_896_162.root,das_make_block<void,regex::ReNode *>(__context__,80,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_897_163) -> void{
        if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Char )
        {
            if ( __re_rename_at_896_162.caseInsensitive )
            {
                if ( __node_rename_at_897_163->textLen == 1 )
                {
                    das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_single_char_ci S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x69b53f8e0406fcad)));
                } else {
                    das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_char_ci S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xd1c5e29d2aed28d3)));
                };
            } else {
                if ( __node_rename_at_897_163->textLen == 1 )
                {
                    das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_single_char S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x5c2840fada5f29d9)));
                } else {
                    das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_char S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x38bd89252c0cf248)));
                };
            };
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_char 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x2eeba7ce095d9afa)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Union )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_union S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x1ef4abfda5a2bd43)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_union 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xc4c5ecee47ca5080)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Set )
        {
            if ( __re_rename_at_896_162.caseInsensitive )
            {
                case_fold_set_3fe63201f3f0aed7(__context__,das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_897_163->cset));
            };
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_set S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x2a0f5e124643a3e2)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_set 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x1b17e13417657e5)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Any )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_any S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xf452d2698cd903bd)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_any 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x546ffa9a67c5ab20)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Eos )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_eos S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xb2eb31b5d89f9bf3)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_eos 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x7d2fd4e5b91438c2)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Concat )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_concat S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x9a436a387386d91c)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_concat 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xdc10b0f0cf88a190)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Plus )
        {
            if ( __node_rename_at_897_163->lazy )
            {
                das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_plus_lazy S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xd1f622d838d80e25)));
            } else if ( __node_rename_at_897_163->subexpr->op == DAS_COMMENT(enum) regex::ReOp::Set )
            {
                das_copy(__node_rename_at_897_163->cset,__node_rename_at_897_163->subexpr->cset);
                if ( __re_rename_at_896_162.caseInsensitive )
                {
                    case_fold_set_3fe63201f3f0aed7(__context__,das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_897_163->cset));
                };
                das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_plus_set S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x5d785a6dbbd43669)));
            } else {
                das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_plus S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x99599148d7cde0f9)));
            };
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_plus 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x5265717f53964fab)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Star )
        {
            if ( __node_rename_at_897_163->lazy )
            {
                das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_star_lazy S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xe8bae4c2e47cc252)));
            } else {
                das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_star S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xcd6744f0c5dc4718)));
            };
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_star 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xcb3cfcadf09dcf2a)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Question )
        {
            if ( __node_rename_at_897_163->lazy )
            {
                das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_question_lazy S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x354f7bb28c616acd)));
            } else {
                das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_question S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xefb1f4e676a32f71)));
            };
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_question 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xe44a011a44687c83)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_group S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x886503ec9847cc49)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_group 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xc4e23520b76179aa)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Bos )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_bos S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x2aa5425f600f75c2)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_bos 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x9f4136463966b165)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Repeat )
        {
            if ( __node_rename_at_897_163->lazy )
            {
                das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_repeat_lazy S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x97bf1ff8071eb2c7)));
            } else {
                das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_repeat S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xeb7c88fc9c30801f)));
            };
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_repeat 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x6fe8c937c99ec0a7)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::WordBoundary )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_word_boundary S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xa7331dcfd4ec6e8c)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_word_boundary 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0xb75970bc2f65a745)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::NonWordBoundary )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_non_word_boundary S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x1bc35a383767aa28)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_non_word_boundary 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x6b504ea77b342bed)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::Lookahead )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_lookahead S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0xb011ceff8fb5c83c)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_lookahead 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x2a9225179b0b9711)));
        } else if ( __node_rename_at_897_163->op == DAS_COMMENT(enum) regex::ReOp::NegativeLookahead )
        {
            das_copy(__node_rename_at_897_163->fun2,Func(__context__->fnByMangledName(/*@regex::re_match2_negative_lookahead S<regex::Regex> 1<S<regex::ReNode>>? C1<Cu8>?*/ 0x454ddfbc0f8d9835)));
            das_copy(__node_rename_at_897_163->gen2,Func(__context__->fnByMangledName(/*@regex::re_gen2_negative_lookahead 1<S<regex::ReNode>>? Y<ReGenRandom>1<u>G H<strings::StringBuilderWriter>*/ 0x11feef58f6b05075)));
        } else {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_9, cast<char * const >::from(((char *) "unsupported ")), cast<DAS_COMMENT(enum) regex::ReOp>::from(__node_rename_at_897_163->op))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
    }));
}}

inline int32_t ci_lower_ee451210b4105701 ( Context * __context__, int32_t __ch_rename_at_989_164 )
{
    return das_auto_cast<int32_t>::cast(((__ch_rename_at_989_164 >= 65) && (__ch_rename_at_989_164 <= 90)) ? das_auto_cast<int32_t>::cast((__ch_rename_at_989_164 + 32)) : das_auto_cast<int32_t>::cast(__ch_rename_at_989_164));
}

inline void case_fold_set_3fe63201f3f0aed7 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_997_165 )
{
    {
        bool __need_loop_999 = true;
        // ch: int const
        das_iterator<range> __ch_iterator(range(0,256));
        int32_t __ch_rename_at_999_166;
        __need_loop_999 = __ch_iterator.first(__context__,(__ch_rename_at_999_166)) && __need_loop_999;
        for ( ; __need_loop_999 ; __need_loop_999 = __ch_iterator.next(__context__,(__ch_rename_at_999_166)) )
        {
            if ( is_char_in_set(__ch_rename_at_999_166,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_997_165),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
            {
                int32_t __lo_rename_at_1001_167 = ((int32_t)ci_lower_ee451210b4105701(__context__,__ch_rename_at_999_166));
                int32_t __up_rename_at_1002_168 = ((int32_t)(((__lo_rename_at_1001_167 >= 97) && (__lo_rename_at_1001_167 <= 122)) ? das_auto_cast<int32_t>::cast((__lo_rename_at_1001_167 - 32)) : das_auto_cast<int32_t>::cast(__lo_rename_at_1001_167)));
                set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_997_165),__lo_rename_at_1001_167);
                set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_997_165),__up_rename_at_1002_168);
            };
        }
        __ch_iterator.close(__context__,(__ch_rename_at_999_166));
    };
}

inline uint8_t const  * re_match2_single_char_7ee863dbfd5bdb67 ( Context * __context__, regex::Regex &  __regex_rename_at_1010_169, regex::ReNode * __node_rename_at_1010_170, uint8_t const  * const  __str_rename_at_1010_171 )
{
    if ( das_equ_val((*(__str_rename_at_1010_171)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( get_character_uat(__node_rename_at_1010_170->text,0) != int32_t((*(__str_rename_at_1010_171))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_1021_172 = das_ptr_add_int32(__str_rename_at_1010_171,1,1);
            das_copy(__node_rename_at_1010_170->tail,__tail_rename_at_1021_172);
            regex::ReNode * __node2_rename_at_1023_173 = __node_rename_at_1010_170->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1023_173 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1023_173->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1010_169),__node2_rename_at_1023_173,__tail_rename_at_1021_172)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_1021_172));
        };
    };
}

inline uint8_t const  * re_match2_char_f2046197ce02637d ( Context * __context__, regex::Regex &  __regex_rename_at_1033_174, regex::ReNode * __node_rename_at_1033_175, uint8_t const  * const  __str_rename_at_1033_176 )
{
    if ( das_equ_val((*(__str_rename_at_1033_176)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( das_memcmp(das_auto_cast<void * const >::cast(das_cast<uint8_t *>::cast(__node_rename_at_1033_175->text)),das_auto_cast<void * const >::cast(__str_rename_at_1033_176),__node_rename_at_1033_175->textLen) != 0 )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_1044_177 = das_ptr_add_int32(__str_rename_at_1033_176,__node_rename_at_1033_175->textLen,1);
            das_copy(__node_rename_at_1033_175->tail,__tail_rename_at_1044_177);
            regex::ReNode * __node2_rename_at_1046_178 = __node_rename_at_1033_175->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1046_178 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1046_178->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1033_174),__node2_rename_at_1046_178,__tail_rename_at_1044_177)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_1044_177));
        };
    };
}

inline uint8_t const  * re_match2_single_char_ci_68081666546328b ( Context * __context__, regex::Regex &  __regex_rename_at_1059_179, regex::ReNode * __node_rename_at_1059_180, uint8_t const  * const  __str_rename_at_1059_181 )
{
    if ( das_equ_val((*(__str_rename_at_1059_181)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( ci_lower_ee451210b4105701(__context__,get_character_uat(__node_rename_at_1059_180->text,0)) != ci_lower_ee451210b4105701(__context__,int32_t((*(__str_rename_at_1059_181)))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_1070_182 = das_ptr_add_int32(__str_rename_at_1059_181,1,1);
            das_copy(__node_rename_at_1059_180->tail,__tail_rename_at_1070_182);
            regex::ReNode * __node2_rename_at_1072_183 = __node_rename_at_1059_180->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1072_183 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1072_183->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1059_179),__node2_rename_at_1072_183,__tail_rename_at_1070_182)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_1070_182));
        };
    };
}

inline uint8_t const  * re_match2_char_ci_3c6abffae8c4e1d5 ( Context * __context__, regex::Regex &  __regex_rename_at_1082_184, regex::ReNode * __node_rename_at_1082_185, uint8_t const  * const  __str_rename_at_1082_186 )
{
    if ( das_equ_val((*(__str_rename_at_1082_186)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        {
            bool __need_loop_1090 = true;
            // i: int const
            das_iterator<range> __i_iterator(mk_range(__node_rename_at_1082_185->textLen));
            int32_t __i_rename_at_1090_187;
            __need_loop_1090 = __i_iterator.first(__context__,(__i_rename_at_1090_187)) && __need_loop_1090;
            for ( ; __need_loop_1090 ; __need_loop_1090 = __i_iterator.next(__context__,(__i_rename_at_1090_187)) )
            {
                if ( ci_lower_ee451210b4105701(__context__,get_character_uat(__node_rename_at_1082_185->text,__i_rename_at_1090_187)) != ci_lower_ee451210b4105701(__context__,int32_t((*(das_ptr_add_int32(__str_rename_at_1082_186,__i_rename_at_1090_187,1))))) )
                {
                    return das_auto_cast<uint8_t const  *>::cast(nullptr);
                };
            }
            __i_iterator.close(__context__,(__i_rename_at_1090_187));
        };
        uint8_t const  * __tail_rename_at_1095_188 = das_ptr_add_int32(__str_rename_at_1082_186,__node_rename_at_1082_185->textLen,1);
        das_copy(__node_rename_at_1082_185->tail,__tail_rename_at_1095_188);
        regex::ReNode * __node2_rename_at_1097_189 = __node_rename_at_1082_185->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1097_189 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1097_189->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1082_184),__node2_rename_at_1097_189,__tail_rename_at_1095_188)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_1095_188));
    };
}

inline uint8_t const  * re_match2_union_bcaea863938bc283 ( Context * __context__, regex::Regex &  __regex_rename_at_1107_190, regex::ReNode * __node_rename_at_1107_191, uint8_t const  * const  __str_rename_at_1107_192 )
{
    {
        bool __need_loop_1111 = true;
        // sub: regex::ReNode?&
        das_iterator<TArray<regex::ReNode *>> __sub_iterator(__node_rename_at_1107_191->all);
        regex::ReNode * * __sub_rename_at_1111_193;
        __need_loop_1111 = __sub_iterator.first(__context__,(__sub_rename_at_1111_193)) && __need_loop_1111;
        for ( ; __need_loop_1111 ; __need_loop_1111 = __sub_iterator.next(__context__,(__sub_rename_at_1111_193)) )
        {
            uint8_t const  * __osub_rename_at_1112_194 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,(*__sub_rename_at_1111_193)->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1107_190),(*__sub_rename_at_1111_193),__str_rename_at_1107_192));
            if ( __osub_rename_at_1112_194 != nullptr )
            {
                das_copy(__node_rename_at_1107_191->tail,(*__sub_rename_at_1111_193)->tail);
                return das_auto_cast<uint8_t const  *>::cast(__osub_rename_at_1112_194);
            };
        }
        __sub_iterator.close(__context__,(__sub_rename_at_1111_193));
    };
    return das_auto_cast<uint8_t const  *>::cast(nullptr);
}

inline uint8_t const  * re_match2_set_5cb609a7e934b852 ( Context * __context__, regex::Regex &  __regex_rename_at_1122_195, regex::ReNode * __node_rename_at_1122_196, uint8_t const  * const  __str_rename_at_1122_197 )
{
    if ( das_equ_val((*(__str_rename_at_1122_197)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( !is_char_in_set(int32_t((*(__str_rename_at_1122_197))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_1122_196->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_1133_198 = das_ptr_add_int32(__str_rename_at_1122_197,1,1);
            das_copy(__node_rename_at_1122_196->tail,__tail_rename_at_1133_198);
            regex::ReNode * __node2_rename_at_1135_199 = __node_rename_at_1122_196->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1135_199 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1135_199->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1122_195),__node2_rename_at_1135_199,__tail_rename_at_1133_198)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_1133_198));
        };
    };
}

inline uint8_t const  * re_match2_any_e0f778af97b719a3 ( Context * __context__, regex::Regex &  __regex_rename_at_1145_200, regex::ReNode * __node_rename_at_1145_201, uint8_t const  * const  __str_rename_at_1145_202 )
{
    if ( das_equ_val((*(__str_rename_at_1145_202)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( !(__regex_rename_at_1145_200.dotAll) && (int32_t((*(__str_rename_at_1145_202))) == 10) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __tail_rename_at_1156_203 = das_ptr_add_int32(__str_rename_at_1145_202,1,1);
            das_copy(__node_rename_at_1145_201->tail,__tail_rename_at_1156_203);
            regex::ReNode * __node2_rename_at_1158_204 = __node_rename_at_1145_201->next;
            return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1158_204 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1158_204->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1145_200),__node2_rename_at_1158_204,__tail_rename_at_1156_203)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_1156_203));
        };
    };
}

inline uint8_t const  * re_match2_concat_2e0ee6aaeac1bff7 ( Context * __context__, regex::Regex &  __regex_rename_at_1168_205, regex::ReNode * __node_rename_at_1168_206, uint8_t const  * const  __str_rename_at_1168_207 )
{
    regex::ReNode * __left_rename_at_1172_208 = __node_rename_at_1168_206->left;
    uint8_t const  * __oleft_rename_at_1173_209 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__left_rename_at_1172_208->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1168_205),__left_rename_at_1172_208,__str_rename_at_1168_207));
    if ( __oleft_rename_at_1173_209 == nullptr )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_1168_206->tail,__node_rename_at_1168_206->right->tail);
        return das_auto_cast<uint8_t const  *>::cast(__oleft_rename_at_1173_209);
    };
}

inline uint8_t const  * re_match2_eos_f51ea4d66660c6ee ( Context * __context__, regex::Regex &  __regex_rename_at_1182_210, regex::ReNode * __node_rename_at_1182_211, uint8_t const  * const  __str_rename_at_1182_212 )
{
    if ( das_nequ_val((*(__str_rename_at_1182_212)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_1182_211->tail,__str_rename_at_1182_212);
        regex::ReNode * __node2_rename_at_1190_213 = __node_rename_at_1182_211->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1190_213 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__node2_rename_at_1190_213->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1182_210),__node2_rename_at_1190_213,__str_rename_at_1182_212)) : das_auto_cast<uint8_t const  *>::cast(__str_rename_at_1182_212));
    };
}

inline uint8_t const  * re_match2_question_55ffbd37b5f31b71 ( Context * __context__, regex::Regex &  __regex_rename_at_1199_214, regex::ReNode * __node_rename_at_1199_215, uint8_t const  * const  __str_rename_at_1199_216 )
{
    regex::ReNode * __nsub_rename_at_1203_217 = __node_rename_at_1199_215->subexpr;
    uint8_t const  * __tail_rename_at_1204_218 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_1203_217->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1199_214),__nsub_rename_at_1203_217,__str_rename_at_1199_216);
    if ( __tail_rename_at_1204_218 == nullptr )
    {
        das_copy(__tail_rename_at_1204_218,__str_rename_at_1199_216);
    };
    das_copy(__node_rename_at_1199_215->tail,__tail_rename_at_1204_218);
    regex::ReNode * __node2_rename_at_1209_219 = __node_rename_at_1199_215->next;
    return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1209_219 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1209_219->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1199_214),__node2_rename_at_1209_219,__tail_rename_at_1204_218)) : das_auto_cast<uint8_t const  *>::cast(__tail_rename_at_1204_218));
}

inline uint8_t const  * re_match2_plus_cbc885000639d48b ( Context * __context__, regex::Regex &  __regex_rename_at_1218_220, regex::ReNode * __node_rename_at_1218_221, uint8_t const  * const  __str_rename_at_1218_222 )
{
    if ( das_equ_val((*(__str_rename_at_1218_222)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        regex::ReNode * __nsub_rename_at_1225_223 = __node_rename_at_1218_221->subexpr;
        uint8_t const  * __osym_rename_at_1226_224 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_1225_223->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1218_220),__nsub_rename_at_1225_223,__str_rename_at_1218_222);
        if ( __osym_rename_at_1226_224 == nullptr )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __ofs_rename_at_1230_225 = __osym_rename_at_1226_224;
            regex::ReNode * __node2_rename_at_1231_226 = __node_rename_at_1218_221->next;
            if ( __node2_rename_at_1231_226 != nullptr )
            {
                while ( __osym_rename_at_1226_224 != nullptr )
                {
                    das_copy(__node_rename_at_1218_221->tail,__osym_rename_at_1226_224);
                    uint8_t const  * __otail_rename_at_1235_227 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1231_226->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1218_220),__node2_rename_at_1231_226,__osym_rename_at_1226_224);
                    if ( __otail_rename_at_1235_227 != nullptr )
                    {
                        return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_1235_227);
                    } else {
                        das_copy(__osym_rename_at_1226_224,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1225_223->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1218_220),__nsub_rename_at_1225_223,__osym_rename_at_1226_224));
                    };
                };
                return das_auto_cast<uint8_t const  *>::cast(nullptr);
            } else {
                while ( __osym_rename_at_1226_224 != nullptr )
                {
                    das_copy(__ofs_rename_at_1230_225,__osym_rename_at_1226_224);
                    das_copy(__osym_rename_at_1226_224,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1225_223->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1218_220),__nsub_rename_at_1225_223,__ofs_rename_at_1230_225));
                };
                das_copy(__node_rename_at_1218_221->tail,__ofs_rename_at_1230_225);
                return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_1230_225);
            };
        };
    };
}

inline uint8_t const  * re_match2_plus_set_e285952ef61ae545 ( Context * __context__, regex::Regex &  __regex_rename_at_1253_228, regex::ReNode * __node_rename_at_1253_229, uint8_t const  * const  __str_rename_at_1253_230 )
{
    if ( das_equ_val((*(__str_rename_at_1253_230)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( !is_char_in_set(int32_t((*(__str_rename_at_1253_230))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_1253_229->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            uint8_t const  * __ofs_rename_at_1265_231 = das_ptr_add_int32(__str_rename_at_1253_230,1,1);
            uint8_t const  * __osym_rename_at_1266_232 = __ofs_rename_at_1265_231;
            regex::ReNode * __node2_rename_at_1267_233 = __node_rename_at_1253_229->next;
            if ( __node2_rename_at_1267_233 != nullptr )
            {
                while ( __osym_rename_at_1266_232 != nullptr )
                {
                    das_copy(__node_rename_at_1253_229->tail,__osym_rename_at_1266_232);
                    uint8_t const  * __otail_rename_at_1271_234 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1267_233->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1253_228),__node2_rename_at_1267_233,__osym_rename_at_1266_232);
                    if ( __otail_rename_at_1271_234 != nullptr )
                    {
                        return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_1271_234);
                    } else {
                        if ( (das_equ_val((*(__osym_rename_at_1266_232)),0x0)) || !(is_char_in_set(int32_t((*(__osym_rename_at_1266_232))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_1253_229->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))) )
                        {
                            return das_auto_cast<uint8_t const  *>::cast(nullptr);
                        } else {
                            das_ptr_inc(__osym_rename_at_1266_232,1);
                        };
                    };
                };
                return das_auto_cast<uint8_t const  *>::cast(nullptr);
            } else {
                while ( __osym_rename_at_1266_232 != nullptr )
                {
                    das_copy(__ofs_rename_at_1265_231,__osym_rename_at_1266_232);
                    if ( (das_equ_val((*(__osym_rename_at_1266_232)),0x0)) || !(is_char_in_set(int32_t((*(__osym_rename_at_1266_232))),das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_1253_229->cset),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))) )
                    {
                        break;
                    } else {
                        das_ptr_inc(__osym_rename_at_1266_232,1);
                    };
                };
                das_copy(__node_rename_at_1253_229->tail,__ofs_rename_at_1265_231);
                return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_1265_231);
            };
        };
    };
}

inline uint8_t const  * re_match2_star_3e354952f52dedc7 ( Context * __context__, regex::Regex &  __regex_rename_at_1296_235, regex::ReNode * __node_rename_at_1296_236, uint8_t const  * const  __str_rename_at_1296_237 )
{
    uint8_t const  * __ofs_rename_at_1300_238 = __str_rename_at_1296_237;
    regex::ReNode * __node2_rename_at_1301_239 = __node_rename_at_1296_236->next;
    regex::ReNode * __nsub_rename_at_1302_240 = __node_rename_at_1296_236->subexpr;
    if ( __node2_rename_at_1301_239 != nullptr )
    {
        while ( __ofs_rename_at_1300_238 != nullptr )
        {
            das_copy(__node_rename_at_1296_236->tail,__ofs_rename_at_1300_238);
            uint8_t const  * __oany_rename_at_1306_241 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1301_239->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1296_235),__node2_rename_at_1301_239,__ofs_rename_at_1300_238));
            if ( __oany_rename_at_1306_241 != nullptr )
            {
                return das_auto_cast<uint8_t const  *>::cast(__oany_rename_at_1306_241);
            } else {
                das_copy(__ofs_rename_at_1300_238,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1302_240->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1296_235),__nsub_rename_at_1302_240,__ofs_rename_at_1300_238));
            };
        };
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        uint8_t const  * __osym_rename_at_1314_242 = __ofs_rename_at_1300_238;
        while ( __osym_rename_at_1314_242 != nullptr )
        {
            das_copy(__ofs_rename_at_1300_238,__osym_rename_at_1314_242);
            das_copy(__osym_rename_at_1314_242,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1302_240->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1296_235),__nsub_rename_at_1302_240,__ofs_rename_at_1300_238));
        };
        das_copy(__node_rename_at_1296_236->tail,__ofs_rename_at_1300_238);
        return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_1300_238);
    };
}

inline uint8_t const  * re_match2_group_cdd84cde5e0cc0e3 ( Context * __context__, regex::Regex &  __regex_rename_at_1325_243, regex::ReNode * __node_rename_at_1325_244, uint8_t const  * const  __str_rename_at_1325_245 )
{
    regex::ReNode * __nsub_rename_at_1329_246 = __node_rename_at_1325_244->subexpr;
    uint8_t const  * __osub_rename_at_1330_247 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_1329_246->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1325_243),__nsub_rename_at_1329_246,__str_rename_at_1325_245));
    if ( __osub_rename_at_1330_247 == nullptr )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        if ( __node_rename_at_1325_244->index >= 0 )
        {
            das_copy(das_get_tuple_field<range,0>::get(__regex_rename_at_1325_243.groups(__node_rename_at_1325_244->index,__context__)),range(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__str_rename_at_1325_245),das_auto_cast<void * const >::cast(__regex_rename_at_1325_243.match),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__node_rename_at_1325_244->subexpr->tail),das_auto_cast<void * const >::cast(__regex_rename_at_1325_243.match),1))));
        };
        return das_auto_cast<uint8_t const  *>::cast(__osub_rename_at_1330_247);
    };
}

inline uint8_t const  * re_match2_bos_7093e76efb08a7f4 ( Context * __context__, regex::Regex &  __regex_rename_at_1341_248, regex::ReNode * __node_rename_at_1341_249, uint8_t const  * const  __str_rename_at_1341_250 )
{
    if ( __str_rename_at_1341_250 != __regex_rename_at_1341_248.match )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_1341_249->tail,__str_rename_at_1341_250);
        regex::ReNode * __node2_rename_at_1349_251 = __node_rename_at_1341_249->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1349_251 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__node2_rename_at_1349_251->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1341_248),__node2_rename_at_1349_251,__str_rename_at_1341_250)) : das_auto_cast<uint8_t const  *>::cast(__str_rename_at_1341_250));
    };
}

inline uint8_t const  * re_match2_word_boundary_de93f59a606699b5 ( Context * __context__, regex::Regex &  __regex_rename_at_1358_252, regex::ReNode * __node_rename_at_1358_253, uint8_t const  * const  __str_rename_at_1358_254 )
{
    bool __prev_is_word_rename_at_1363_255 = false;
    if ( __str_rename_at_1358_254 != __regex_rename_at_1358_252.match )
    {
        das_copy(__prev_is_word_rename_at_1363_255,is_word_character_8422846159268aaf(__context__,int32_t((*(das_ptr_add_int32(__str_rename_at_1358_254,-1,1))))));
    };
    bool __curr_is_word_rename_at_1367_256 = ((bool)((das_nequ_val((*(__str_rename_at_1358_254)),0x0)) && is_word_character_8422846159268aaf(__context__,int32_t((*(__str_rename_at_1358_254))))));
    if ( __prev_is_word_rename_at_1363_255 == __curr_is_word_rename_at_1367_256 )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_1358_253->tail,__str_rename_at_1358_254);
        regex::ReNode * __node2_rename_at_1372_257 = __node_rename_at_1358_253->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1372_257 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__node2_rename_at_1372_257->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1358_252),__node2_rename_at_1372_257,__str_rename_at_1358_254)) : das_auto_cast<uint8_t const  *>::cast(__str_rename_at_1358_254));
    };
}

inline uint8_t const  * re_match2_non_word_boundary_d0c20eaf4c18dc8a ( Context * __context__, regex::Regex &  __regex_rename_at_1382_258, regex::ReNode * __node_rename_at_1382_259, uint8_t const  * const  __str_rename_at_1382_260 )
{
    bool __prev_is_word_rename_at_1387_261 = false;
    if ( __str_rename_at_1382_260 != __regex_rename_at_1382_258.match )
    {
        das_copy(__prev_is_word_rename_at_1387_261,is_word_character_8422846159268aaf(__context__,int32_t((*(das_ptr_add_int32(__str_rename_at_1382_260,-1,1))))));
    };
    bool __curr_is_word_rename_at_1391_262 = ((bool)((das_nequ_val((*(__str_rename_at_1382_260)),0x0)) && is_word_character_8422846159268aaf(__context__,int32_t((*(__str_rename_at_1382_260))))));
    if ( __prev_is_word_rename_at_1387_261 != __curr_is_word_rename_at_1391_262 )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_1382_259->tail,__str_rename_at_1382_260);
        regex::ReNode * __node2_rename_at_1396_263 = __node_rename_at_1382_259->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1396_263 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__node2_rename_at_1396_263->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1382_258),__node2_rename_at_1396_263,__str_rename_at_1382_260)) : das_auto_cast<uint8_t const  *>::cast(__str_rename_at_1382_260));
    };
}

inline uint8_t const  * re_match2_lookahead_3ac542e65f4d1cff ( Context * __context__, regex::Regex &  __regex_rename_at_1406_264, regex::ReNode * __node_rename_at_1406_265, uint8_t const  * const  __str_rename_at_1406_266 )
{
    regex::ReNode * __nsub_rename_at_1410_267 = __node_rename_at_1406_265->subexpr;
    uint8_t const  * __osub_rename_at_1411_268 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_1410_267->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1406_264),__nsub_rename_at_1410_267,__str_rename_at_1406_266));
    if ( __osub_rename_at_1411_268 == nullptr )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_1406_265->tail,__str_rename_at_1406_266);
        regex::ReNode * __node2_rename_at_1417_269 = __node_rename_at_1406_265->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1417_269 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__node2_rename_at_1417_269->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1406_264),__node2_rename_at_1417_269,__str_rename_at_1406_266)) : das_auto_cast<uint8_t const  *>::cast(__str_rename_at_1406_266));
    };
}

inline uint8_t const  * re_match2_negative_lookahead_ba2437211db882de ( Context * __context__, regex::Regex &  __regex_rename_at_1426_270, regex::ReNode * __node_rename_at_1426_271, uint8_t const  * const  __str_rename_at_1426_272 )
{
    regex::ReNode * __nsub_rename_at_1430_273 = __node_rename_at_1426_271->subexpr;
    uint8_t const  * __osub_rename_at_1431_274 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_1430_273->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1426_270),__nsub_rename_at_1430_273,__str_rename_at_1426_272));
    if ( __osub_rename_at_1431_274 != nullptr )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_1426_271->tail,__str_rename_at_1426_272);
        regex::ReNode * __node2_rename_at_1437_275 = __node_rename_at_1426_271->next;
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1437_275 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__node2_rename_at_1437_275->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1426_270),__node2_rename_at_1437_275,__str_rename_at_1426_272)) : das_auto_cast<uint8_t const  *>::cast(__str_rename_at_1426_272));
    };
}

inline uint8_t const  * re_match2_repeat_3b5f93e4a9fcaadf ( Context * __context__, regex::Regex &  __regex_rename_at_1446_276, regex::ReNode * __node_rename_at_1446_277, uint8_t const  * const  __str_rename_at_1446_278 )
{
    regex::ReNode * __nsub_rename_at_1450_279 = __node_rename_at_1446_277->subexpr;
    uint8_t const  * __ofs_rename_at_1451_280 = __str_rename_at_1446_278;
    {
        bool __need_loop_1453 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__node_rename_at_1446_277->min_rep));
        int32_t __i_rename_at_1453_281;
        __need_loop_1453 = __i_iterator.first(__context__,(__i_rename_at_1453_281)) && __need_loop_1453;
        for ( ; __need_loop_1453 ; __need_loop_1453 = __i_iterator.next(__context__,(__i_rename_at_1453_281)) )
        {
            if ( ((((das_equ_val((*(__ofs_rename_at_1451_280)),0x0)) && (__nsub_rename_at_1450_279->op != DAS_COMMENT(enum) regex::ReOp::Eos)) && (__nsub_rename_at_1450_279->op != DAS_COMMENT(enum) regex::ReOp::Bos)) && (__nsub_rename_at_1450_279->op != DAS_COMMENT(enum) regex::ReOp::WordBoundary)) && (__nsub_rename_at_1450_279->op != DAS_COMMENT(enum) regex::ReOp::NonWordBoundary) )
            {
                return das_auto_cast<uint8_t const  *>::cast(nullptr);
            } else {
                uint8_t const  * __osym_rename_at_1457_282 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1450_279->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1446_276),__nsub_rename_at_1450_279,__ofs_rename_at_1451_280);
                if ( __osym_rename_at_1457_282 == nullptr )
                {
                    return das_auto_cast<uint8_t const  *>::cast(nullptr);
                } else {
                    das_copy(__ofs_rename_at_1451_280,__osym_rename_at_1457_282);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1453_281));
    };
    regex::ReNode * __node2_rename_at_1464_283 = __node_rename_at_1446_277->next;
    if ( __node_rename_at_1446_277->max_rep == -1 )
    {
        if ( __node2_rename_at_1464_283 != nullptr )
        {
            TArray<uint8_t const  *> __positions_rename_at_1469_284;das_zero(__positions_rename_at_1469_284);
            _FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606(__context__,das_arg<TArray<uint8_t const  *>>::pass(__positions_rename_at_1469_284),__ofs_rename_at_1451_280);
            while ( true )
            {
                uint8_t const  * __osym_rename_at_1472_285 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1450_279->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1446_276),__nsub_rename_at_1450_279,__ofs_rename_at_1451_280);
                if ( (__osym_rename_at_1472_285 == nullptr) || (__osym_rename_at_1472_285 == __ofs_rename_at_1451_280) )
                {
                    break;
                } else {
                    _FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606(__context__,das_arg<TArray<uint8_t const  *>>::pass(__positions_rename_at_1469_284),__osym_rename_at_1472_285);
                    das_copy(__ofs_rename_at_1451_280,__osym_rename_at_1472_285);
                };
            };
            int32_t __pidx_rename_at_1480_286 = (builtin_array_size(das_arg<TArray<uint8_t const  *>>::pass(__positions_rename_at_1469_284)) - 1);
            while ( __pidx_rename_at_1480_286 >= 0 )
            {
                uint8_t const  * __pos_rename_at_1482_287 = __positions_rename_at_1469_284(__pidx_rename_at_1480_286,__context__);
                das_copy(__node_rename_at_1446_277->tail,__pos_rename_at_1482_287);
                uint8_t const  * __otail_rename_at_1484_288 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1464_283->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1446_276),__node2_rename_at_1464_283,__pos_rename_at_1482_287);
                if ( __otail_rename_at_1484_288 != nullptr )
                {
                    return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_1484_288);
                } else {
                    --__pidx_rename_at_1480_286;
                };
            };
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            while ( true )
            {
                uint8_t const  * __osym_rename_at_1494_289 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1450_279->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1446_276),__nsub_rename_at_1450_279,__ofs_rename_at_1451_280);
                if ( (__osym_rename_at_1494_289 == nullptr) || (__osym_rename_at_1494_289 == __ofs_rename_at_1451_280) )
                {
                    break;
                } else {
                    das_copy(__ofs_rename_at_1451_280,__osym_rename_at_1494_289);
                };
            };
            das_copy(__node_rename_at_1446_277->tail,__ofs_rename_at_1451_280);
            return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_1451_280);
        };
    } else {
        int32_t __remaining_rename_at_1505_290 = ((int32_t)(__node_rename_at_1446_277->max_rep - __node_rename_at_1446_277->min_rep));
        if ( __node2_rename_at_1464_283 != nullptr )
        {
            TArray<uint8_t const  *> __positions_rename_at_1507_291;das_zero(__positions_rename_at_1507_291);
            _FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606(__context__,das_arg<TArray<uint8_t const  *>>::pass(__positions_rename_at_1507_291),__ofs_rename_at_1451_280);
            {
                bool __need_loop_1509 = true;
                // i: int const
                das_iterator<range> __i_iterator(mk_range(__remaining_rename_at_1505_290));
                int32_t __i_rename_at_1509_292;
                __need_loop_1509 = __i_iterator.first(__context__,(__i_rename_at_1509_292)) && __need_loop_1509;
                for ( ; __need_loop_1509 ; __need_loop_1509 = __i_iterator.next(__context__,(__i_rename_at_1509_292)) )
                {
                    uint8_t const  * __osym_rename_at_1510_293 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1450_279->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1446_276),__nsub_rename_at_1450_279,__ofs_rename_at_1451_280);
                    if ( (__osym_rename_at_1510_293 == nullptr) || (__osym_rename_at_1510_293 == __ofs_rename_at_1451_280) )
                    {
                        break;
                    } else {
                        _FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606(__context__,das_arg<TArray<uint8_t const  *>>::pass(__positions_rename_at_1507_291),__osym_rename_at_1510_293);
                        das_copy(__ofs_rename_at_1451_280,__osym_rename_at_1510_293);
                    };
                }
                __i_iterator.close(__context__,(__i_rename_at_1509_292));
            };
            int32_t __pidx_rename_at_1518_294 = (builtin_array_size(das_arg<TArray<uint8_t const  *>>::pass(__positions_rename_at_1507_291)) - 1);
            while ( __pidx_rename_at_1518_294 >= 0 )
            {
                uint8_t const  * __pos_rename_at_1520_295 = __positions_rename_at_1507_291(__pidx_rename_at_1518_294,__context__);
                das_copy(__node_rename_at_1446_277->tail,__pos_rename_at_1520_295);
                uint8_t const  * __otail_rename_at_1522_296 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1464_283->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1446_276),__node2_rename_at_1464_283,__pos_rename_at_1520_295);
                if ( __otail_rename_at_1522_296 != nullptr )
                {
                    return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_1522_296);
                } else {
                    --__pidx_rename_at_1518_294;
                };
            };
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            {
                bool __need_loop_1530 = true;
                // i: int const
                das_iterator<range> __i_iterator(mk_range(__remaining_rename_at_1505_290));
                int32_t __i_rename_at_1530_297;
                __need_loop_1530 = __i_iterator.first(__context__,(__i_rename_at_1530_297)) && __need_loop_1530;
                for ( ; __need_loop_1530 ; __need_loop_1530 = __i_iterator.next(__context__,(__i_rename_at_1530_297)) )
                {
                    uint8_t const  * __osym_rename_at_1531_298 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1450_279->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1446_276),__nsub_rename_at_1450_279,__ofs_rename_at_1451_280);
                    if ( (__osym_rename_at_1531_298 == nullptr) || (__osym_rename_at_1531_298 == __ofs_rename_at_1451_280) )
                    {
                        break;
                    } else {
                        das_copy(__ofs_rename_at_1451_280,__osym_rename_at_1531_298);
                    };
                }
                __i_iterator.close(__context__,(__i_rename_at_1530_297));
            };
            das_copy(__node_rename_at_1446_277->tail,__ofs_rename_at_1451_280);
            return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_1451_280);
        };
    };
}

inline uint8_t const  * re_match2_question_lazy_63762e1b1147774a ( Context * __context__, regex::Regex &  __regex_rename_at_1548_299, regex::ReNode * __node_rename_at_1548_300, uint8_t const  * const  __str_rename_at_1548_301 )
{
    regex::ReNode * __node2_rename_at_1553_302 = __node_rename_at_1548_300->next;
    if ( __node2_rename_at_1553_302 != nullptr )
    {
        das_copy(__node_rename_at_1548_300->tail,__str_rename_at_1548_301);
        uint8_t const  * __otail_rename_at_1556_303 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__node2_rename_at_1553_302->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1548_299),__node2_rename_at_1553_302,__str_rename_at_1548_301);
        if ( __otail_rename_at_1556_303 != nullptr )
        {
            return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_1556_303);
        };
    };
    regex::ReNode * __nsub_rename_at_1562_304 = __node_rename_at_1548_300->subexpr;
    uint8_t const  * __tail_rename_at_1563_305 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_1562_304->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1548_299),__nsub_rename_at_1562_304,__str_rename_at_1548_301);
    if ( __tail_rename_at_1563_305 == nullptr )
    {
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1553_302 == nullptr) ? das_auto_cast<uint8_t const  * const >::cast(__str_rename_at_1548_301) : das_auto_cast<uint8_t const  * const >::cast(nullptr));
    } else {
        das_copy(__node_rename_at_1548_300->tail,__tail_rename_at_1563_305);
        return das_auto_cast<uint8_t const  *>::cast((__node2_rename_at_1553_302 != nullptr) ? das_auto_cast<uint8_t const  *>::cast(das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1553_302->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1548_299),__node2_rename_at_1553_302,__tail_rename_at_1563_305)) : das_auto_cast<uint8_t const  *>::cast(__str_rename_at_1548_301));
    };
}

inline uint8_t const  * re_match2_star_lazy_2c4bd54606277ac1 ( Context * __context__, regex::Regex &  __regex_rename_at_1579_306, regex::ReNode * __node_rename_at_1579_307, uint8_t const  * const  __str_rename_at_1579_308 )
{
    uint8_t const  * __ofs_rename_at_1583_309 = __str_rename_at_1579_308;
    regex::ReNode * __node2_rename_at_1584_310 = __node_rename_at_1579_307->next;
    regex::ReNode * __nsub_rename_at_1585_311 = __node_rename_at_1579_307->subexpr;
    if ( __node2_rename_at_1584_310 != nullptr )
    {
        while ( __ofs_rename_at_1583_309 != nullptr )
        {
            das_copy(__node_rename_at_1579_307->tail,__ofs_rename_at_1583_309);
            uint8_t const  * __oany_rename_at_1590_312 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1584_310->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1579_306),__node2_rename_at_1584_310,__ofs_rename_at_1583_309));
            if ( __oany_rename_at_1590_312 != nullptr )
            {
                return das_auto_cast<uint8_t const  *>::cast(__oany_rename_at_1590_312);
            } else {
                das_copy(__ofs_rename_at_1583_309,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1585_311->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1579_306),__nsub_rename_at_1585_311,__ofs_rename_at_1583_309));
            };
        };
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        das_copy(__node_rename_at_1579_307->tail,__str_rename_at_1579_308);
        return das_auto_cast<uint8_t const  *>::cast(__str_rename_at_1579_308);
    };
}

inline uint8_t const  * re_match2_plus_lazy_e54a263affed7fc8 ( Context * __context__, regex::Regex &  __regex_rename_at_1605_313, regex::ReNode * __node_rename_at_1605_314, uint8_t const  * const  __str_rename_at_1605_315 )
{
    if ( das_equ_val((*(__str_rename_at_1605_315)),0x0) )
    {
        return das_auto_cast<uint8_t const  *>::cast(nullptr);
    } else {
        regex::ReNode * __nsub_rename_at_1612_316 = __node_rename_at_1605_314->subexpr;
        uint8_t const  * __osym_rename_at_1613_317 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  * const >(__context__,nullptr,__nsub_rename_at_1612_316->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1605_313),__nsub_rename_at_1612_316,__str_rename_at_1605_315);
        if ( __osym_rename_at_1613_317 == nullptr )
        {
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            regex::ReNode * __node2_rename_at_1617_318 = __node_rename_at_1605_314->next;
            if ( __node2_rename_at_1617_318 != nullptr )
            {
                while ( __osym_rename_at_1613_317 != nullptr )
                {
                    das_copy(__node_rename_at_1605_314->tail,__osym_rename_at_1613_317);
                    uint8_t const  * __otail_rename_at_1622_319 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1617_318->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1605_313),__node2_rename_at_1617_318,__osym_rename_at_1613_317);
                    if ( __otail_rename_at_1622_319 != nullptr )
                    {
                        return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_1622_319);
                    } else {
                        das_copy(__osym_rename_at_1613_317,das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1612_316->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1605_313),__nsub_rename_at_1612_316,__osym_rename_at_1613_317));
                    };
                };
                return das_auto_cast<uint8_t const  *>::cast(nullptr);
            } else {
                das_copy(__node_rename_at_1605_314->tail,__osym_rename_at_1613_317);
                return das_auto_cast<uint8_t const  *>::cast(__osym_rename_at_1613_317);
            };
        };
    };
}

inline uint8_t const  * re_match2_repeat_lazy_1105b9a6a4ba9b41 ( Context * __context__, regex::Regex &  __regex_rename_at_1637_320, regex::ReNode * __node_rename_at_1637_321, uint8_t const  * const  __str_rename_at_1637_322 )
{
    regex::ReNode * __nsub_rename_at_1641_323 = __node_rename_at_1637_321->subexpr;
    uint8_t const  * __ofs_rename_at_1642_324 = __str_rename_at_1637_322;
    {
        bool __need_loop_1644 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__node_rename_at_1637_321->min_rep));
        int32_t __i_rename_at_1644_325;
        __need_loop_1644 = __i_iterator.first(__context__,(__i_rename_at_1644_325)) && __need_loop_1644;
        for ( ; __need_loop_1644 ; __need_loop_1644 = __i_iterator.next(__context__,(__i_rename_at_1644_325)) )
        {
            if ( ((((das_equ_val((*(__ofs_rename_at_1642_324)),0x0)) && (__nsub_rename_at_1641_323->op != DAS_COMMENT(enum) regex::ReOp::Eos)) && (__nsub_rename_at_1641_323->op != DAS_COMMENT(enum) regex::ReOp::Bos)) && (__nsub_rename_at_1641_323->op != DAS_COMMENT(enum) regex::ReOp::WordBoundary)) && (__nsub_rename_at_1641_323->op != DAS_COMMENT(enum) regex::ReOp::NonWordBoundary) )
            {
                return das_auto_cast<uint8_t const  *>::cast(nullptr);
            } else {
                uint8_t const  * __osym_rename_at_1648_326 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1641_323->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1637_320),__nsub_rename_at_1641_323,__ofs_rename_at_1642_324);
                if ( __osym_rename_at_1648_326 == nullptr )
                {
                    return das_auto_cast<uint8_t const  *>::cast(nullptr);
                } else {
                    das_copy(__ofs_rename_at_1642_324,__osym_rename_at_1648_326);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1644_325));
    };
    regex::ReNode * __node2_rename_at_1655_327 = __node_rename_at_1637_321->next;
    if ( __node_rename_at_1637_321->max_rep == -1 )
    {
        if ( __node2_rename_at_1655_327 != nullptr )
        {
            while ( true )
            {
                das_copy(__node_rename_at_1637_321->tail,__ofs_rename_at_1642_324);
                uint8_t const  * __otail_rename_at_1661_328 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1655_327->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1637_320),__node2_rename_at_1655_327,__ofs_rename_at_1642_324);
                if ( __otail_rename_at_1661_328 != nullptr )
                {
                    return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_1661_328);
                } else {
                    uint8_t const  * __osym_rename_at_1665_329 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1641_323->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1637_320),__nsub_rename_at_1641_323,__ofs_rename_at_1642_324);
                    if ( (__osym_rename_at_1665_329 == nullptr) || (__osym_rename_at_1665_329 == __ofs_rename_at_1642_324) )
                    {
                        break;
                    } else {
                        das_copy(__ofs_rename_at_1642_324,__osym_rename_at_1665_329);
                    };
                };
            };
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            das_copy(__node_rename_at_1637_321->tail,__ofs_rename_at_1642_324);
            return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_1642_324);
        };
    } else {
        int32_t __remaining_rename_at_1679_330 = ((int32_t)(__node_rename_at_1637_321->max_rep - __node_rename_at_1637_321->min_rep));
        if ( __node2_rename_at_1655_327 != nullptr )
        {
            {
                bool __need_loop_1681 = true;
                // i: int const
                das_iterator<range> __i_iterator(mk_range(__remaining_rename_at_1679_330 + 1));
                int32_t __i_rename_at_1681_331;
                __need_loop_1681 = __i_iterator.first(__context__,(__i_rename_at_1681_331)) && __need_loop_1681;
                for ( ; __need_loop_1681 ; __need_loop_1681 = __i_iterator.next(__context__,(__i_rename_at_1681_331)) )
                {
                    das_copy(__node_rename_at_1637_321->tail,__ofs_rename_at_1642_324);
                    uint8_t const  * __otail_rename_at_1683_332 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__node2_rename_at_1655_327->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1637_320),__node2_rename_at_1655_327,__ofs_rename_at_1642_324);
                    if ( __otail_rename_at_1683_332 != nullptr )
                    {
                        return das_auto_cast<uint8_t const  *>::cast(__otail_rename_at_1683_332);
                    } else {
                        if ( __i_rename_at_1681_331 < __remaining_rename_at_1679_330 )
                        {
                            uint8_t const  * __osym_rename_at_1688_333 = das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__nsub_rename_at_1641_323->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1637_320),__nsub_rename_at_1641_323,__ofs_rename_at_1642_324);
                            if ( (__osym_rename_at_1688_333 == nullptr) || (__osym_rename_at_1688_333 == __ofs_rename_at_1642_324) )
                            {
                                break;
                            } else {
                                das_copy(__ofs_rename_at_1642_324,__osym_rename_at_1688_333);
                            };
                        };
                    };
                }
                __i_iterator.close(__context__,(__i_rename_at_1681_331));
            };
            return das_auto_cast<uint8_t const  *>::cast(nullptr);
        } else {
            das_copy(__node_rename_at_1637_321->tail,__ofs_rename_at_1642_324);
            return das_auto_cast<uint8_t const  *>::cast(__ofs_rename_at_1642_324);
        };
    };
}

inline bool re_early_out_65d4b897dbff0d04 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_1708_334, regex::ReNode * const  __node_rename_at_1708_335 )
{
    if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Char )
    {
        set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),get_character_at(__node_rename_at_1708_335->text,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        int32_t __fc_rename_at_1712_336 = ((int32_t)get_character_at(__node_rename_at_1708_335->text,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        if ( (__fc_rename_at_1712_336 >= 65) && (__fc_rename_at_1712_336 <= 90) )
        {
            set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__fc_rename_at_1712_336 + 32);
        } else if ( (__fc_rename_at_1712_336 >= 97) && (__fc_rename_at_1712_336 <= 122) )
        {
            set_or_char_6485de9cac36fb6f(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__fc_rename_at_1712_336 - 32);
        };
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Set )
    {
        set_or_set_c5a22cc26b33786b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__node_rename_at_1708_335->cset);
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Any )
    {
        {
            bool __need_loop_1723 = true;
            // x: uint aka CharSet& -const
            das_iterator<TDim<uint32_t,8>> __x_iterator(__cset_rename_at_1708_334);
            uint32_t * __x_rename_at_1723_337;
            __need_loop_1723 = __x_iterator.first(__context__,(__x_rename_at_1723_337)) && __need_loop_1723;
            for ( ; __need_loop_1723 ; __need_loop_1723 = __x_iterator.next(__context__,(__x_rename_at_1723_337)) )
            {
                das_copy((*__x_rename_at_1723_337),0xffffffffu);
            }
            __x_iterator.close(__context__,(__x_rename_at_1723_337));
        };
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Eos )
    {
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Group )
    {
        return das_auto_cast<bool>::cast(re_early_out_65d4b897dbff0d04(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__node_rename_at_1708_335->subexpr));
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Plus )
    {
        return das_auto_cast<bool>::cast(re_early_out_65d4b897dbff0d04(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__node_rename_at_1708_335->subexpr));
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Star )
    {
        re_early_out_65d4b897dbff0d04(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__node_rename_at_1708_335->subexpr);
        return das_auto_cast<bool>::cast(true);
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Question )
    {
        re_early_out_65d4b897dbff0d04(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__node_rename_at_1708_335->subexpr);
        return das_auto_cast<bool>::cast(true);
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Concat )
    {
        bool __left_rename_at_1740_338 = ((bool)re_early_out_65d4b897dbff0d04(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__node_rename_at_1708_335->left));
        return das_auto_cast<bool>::cast(__left_rename_at_1740_338 ? das_auto_cast<bool>::cast(re_early_out_65d4b897dbff0d04(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__node_rename_at_1708_335->right)) : das_auto_cast<bool>::cast(false));
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        bool __any_rename_at_1743_339 = false;
        {
            bool __need_loop_1744 = true;
            // sub: regex::ReNode? const&
            das_iterator<TArray<regex::ReNode *> const > __sub_iterator(__node_rename_at_1708_335->all);
            regex::ReNode * const  * __sub_rename_at_1744_340;
            __need_loop_1744 = __sub_iterator.first(__context__,(__sub_rename_at_1744_340)) && __need_loop_1744;
            for ( ; __need_loop_1744 ; __need_loop_1744 = __sub_iterator.next(__context__,(__sub_rename_at_1744_340)) )
            {
                das_copy(__any_rename_at_1743_339,re_early_out_65d4b897dbff0d04(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),(*__sub_rename_at_1744_340)) || __any_rename_at_1743_339);
            }
            __sub_iterator.close(__context__,(__sub_rename_at_1744_340));
        };
        return das_auto_cast<bool>::cast(__any_rename_at_1743_339);
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Bos )
    {
        return das_auto_cast<bool>::cast(true);
    } else if ( (__node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::WordBoundary) || (__node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::NonWordBoundary) )
    {
        return das_auto_cast<bool>::cast(true);
    } else if ( (__node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Lookahead) || (__node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::NegativeLookahead) )
    {
        return das_auto_cast<bool>::cast(true);
    } else if ( __node_rename_at_1708_335->op == DAS_COMMENT(enum) regex::ReOp::Repeat )
    {
        re_early_out_65d4b897dbff0d04(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1708_334),__node_rename_at_1708_335->subexpr);
        return das_auto_cast<bool>::cast(__node_rename_at_1708_335->min_rep == 0);
    } else {
        builtin_throw(((char *) "unsupported operation"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<bool>::cast(false);
    };
}

inline bool is_valid_3c125942baf6bf0a ( Context * __context__, regex::Regex &  __re_rename_at_1767_341 )
{
    return das_auto_cast<bool>::cast(__re_rename_at_1767_341.root != nullptr);
}

inline bool regex_compile_e1c30ff6f42517a2 ( Context * __context__, regex::Regex &  __re_rename_at_1772_342, char * const  __expr_rename_at_1772_343, bool __case_insensitive_rename_at_1772_344, bool __dot_all_rename_at_1772_345 )
{
    das_copy(__re_rename_at_1772_342.caseInsensitive,__case_insensitive_rename_at_1772_344);
    das_copy(__re_rename_at_1772_342.dotAll,__dot_all_rename_at_1772_345);
    das_copy(__re_rename_at_1772_342.root,re_parse_be404de54055d495(__context__,__expr_rename_at_1772_343));
    if ( __re_rename_at_1772_342.root != nullptr )
    {
        re_assign_next_5d4c1e040adf8167(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1772_342));
        re_assign_groups_3ba808af5f6a0682(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1772_342));
        re_assign_match_functions_96c5fc406200914c(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1772_342));
        re_early_out_65d4b897dbff0d04(__context__,das_arg<TDim<uint32_t,8>>::pass(__re_rename_at_1772_342.earlyOut),__re_rename_at_1772_342.root);
        das_copy(__re_rename_at_1772_342.canEarlyOut,!is_set_empty_1f052aa7f260c4eb(__context__,das_arg<TDim<uint32_t,8>>::pass(__re_rename_at_1772_342.earlyOut)));
    };
    return das_auto_cast<bool>::cast(__re_rename_at_1772_342.root != nullptr);
}

inline regex::Regex regex_compile_6b8af38459f7df28 ( Context * __context__, char * const  __expr_rename_at_1790_346, bool __case_insensitive_rename_at_1790_347, bool __dot_all_rename_at_1790_348 )
{
    regex::Regex __re_rename_at_1794_349;das_zero(__re_rename_at_1794_349);
    if ( !regex_compile_e1c30ff6f42517a2(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1794_349),__expr_rename_at_1790_346,__case_insensitive_rename_at_1790_347,__dot_all_rename_at_1790_348) )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<char * const >::from(((char *) "regular expression ")), cast<char * const >::from(__expr_rename_at_1790_346), cast<char * const >::from(((char *) " did not compile")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return /* <- */ das_auto_cast_move<regex::Regex>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_9c8fefcf8a2c4d0d(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1794_349)));
}

inline regex::Regex regex_compile_23e411aa689a04e5 ( Context * __context__, regex::Regex &  __re_rename_at_1801_350 )
{
    if ( __re_rename_at_1801_350.root != nullptr )
    {
        re_assign_next_5d4c1e040adf8167(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1801_350));
        re_assign_match_functions_96c5fc406200914c(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1801_350));
    };
    return /* <- */ das_auto_cast_move<regex::Regex>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_9c8fefcf8a2c4d0d(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1801_350)));
}

inline int32_t regex_match_85e0a2ceef06ab24 ( Context * __context__, regex::Regex &  __regex_rename_at_1810_351, char * const  __str_rename_at_1810_352, int32_t __offset_rename_at_1810_353 )
{
    if ( builtin_empty(__str_rename_at_1810_352) )
    {
        return das_auto_cast<int32_t>::cast(-1);
    } else {
        das_copy(__regex_rename_at_1810_351.match,das_cast<uint8_t *>::cast(__str_rename_at_1810_352));
        uint8_t const  * __mptr_rename_at_1821_354 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__regex_rename_at_1810_351.root->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1810_351),__regex_rename_at_1810_351.root,das_ptr_add_int32(__regex_rename_at_1810_351.match,__offset_rename_at_1810_353,1)));
        return das_auto_cast<int32_t>::cast((__mptr_rename_at_1821_354 == nullptr) ? das_auto_cast<int32_t>::cast(-1) : das_auto_cast<int32_t>::cast(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__mptr_rename_at_1821_354),das_auto_cast<void * const >::cast(__regex_rename_at_1810_351.match),1))));
    };
}

inline char * regex_group_dacc7e63de7d674e ( Context * __context__, regex::Regex const  &  __regex_rename_at_1829_355, int32_t __index_rename_at_1829_356, char * const  __match_rename_at_1829_357 )
{
    range __sub_range_rename_at_1831_358 = ((range)das_get_tuple_field<range,0>::get(__regex_rename_at_1829_355.groups(__index_rename_at_1829_356,__context__)));
    return das_auto_cast<char *>::cast(((char * const )(builtin_string_slice1(__match_rename_at_1829_357,v_extract_xi(v_cast_vec4i(__sub_range_rename_at_1831_358)) /*x*/,v_extract_yi(v_cast_vec4i(__sub_range_rename_at_1831_358)) /*y*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline void regex_foreach_2cf0e40b2436b6d9 ( Context * __context__, regex::Regex &  __regex_rename_at_1836_359, char * const  __str_rename_at_1836_360, Block DAS_COMMENT((bool,range)) const  &  __blk_rename_at_1836_361 )
{
    if ( builtin_empty(__str_rename_at_1836_360) )
    {
        return ;
    } else {
        das_copy(__regex_rename_at_1836_359.match,das_cast<uint8_t const  *>::cast(__str_rename_at_1836_360));
        regex::ReNode * __root_rename_at_1843_362 = __regex_rename_at_1836_359.root;
        uint8_t const  * __pstr_rename_at_1844_363 = das_cast<uint8_t const  *>::cast(__str_rename_at_1836_360);
        uint8_t const  * __cstr_rename_at_1845_364 = __pstr_rename_at_1844_363;
        if ( __regex_rename_at_1836_359.canEarlyOut )
        {
            while ( true )
            {
                int32_t __Ch_rename_at_1849_365 = ((int32_t)int32_t((*(__cstr_rename_at_1845_364))));
                if ( __Ch_rename_at_1849_365 == 0 )
                {
                    break;
                } else {
                    if ( is_char_in_set(__Ch_rename_at_1849_365,das_arg<TDim<uint32_t,8>>::pass(__regex_rename_at_1836_359.earlyOut),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        uint8_t const  * __om_rename_at_1854_366 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1843_362->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1836_359),__root_rename_at_1843_362,__cstr_rename_at_1845_364));
                        if ( __om_rename_at_1854_366 != nullptr )
                        {
                            if ( !das_invoke<bool>::invoke<range>(__context__,nullptr,__blk_rename_at_1836_361,range(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1845_364),das_auto_cast<void * const >::cast(__pstr_rename_at_1844_363),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1854_366),das_auto_cast<void * const >::cast(__pstr_rename_at_1844_363),1)))) )
                            {
                                break;
                            } else {
                                if ( __om_rename_at_1854_366 == __cstr_rename_at_1845_364 )
                                {
                                    das_ptr_inc(__cstr_rename_at_1845_364,1);
                                } else {
                                    das_copy(__cstr_rename_at_1845_364,__om_rename_at_1854_366);
                                };
                            };
                        } else {
                            das_ptr_inc(__cstr_rename_at_1845_364,1);
                        };
                    } else {
                        das_ptr_inc(__cstr_rename_at_1845_364,1);
                    };
                };
            };
        } else {
            while ( int32_t((*(__cstr_rename_at_1845_364))) != 0 )
            {
                uint8_t const  * __om_rename_at_1873_367 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1843_362->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1836_359),__root_rename_at_1843_362,__cstr_rename_at_1845_364));
                if ( __om_rename_at_1873_367 != nullptr )
                {
                    if ( !das_invoke<bool>::invoke<range>(__context__,nullptr,__blk_rename_at_1836_361,range(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1845_364),das_auto_cast<void * const >::cast(__pstr_rename_at_1844_363),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1873_367),das_auto_cast<void * const >::cast(__pstr_rename_at_1844_363),1)))) )
                    {
                        break;
                    } else {
                        if ( __om_rename_at_1873_367 == __cstr_rename_at_1845_364 )
                        {
                            das_ptr_inc(__cstr_rename_at_1845_364,1);
                        } else {
                            das_copy(__cstr_rename_at_1845_364,__om_rename_at_1873_367);
                        };
                    };
                } else {
                    das_ptr_inc(__cstr_rename_at_1845_364,1);
                };
            };
        };
    };
}

inline char * regex_replace_e2c898a32c8b9a0a ( Context * __context__, regex::Regex &  __regex_rename_at_1892_368, char * const  __str_rename_at_1892_369, Block DAS_COMMENT((char *,char * const )) const  &  __blk_rename_at_1892_370 )
{
    return das_auto_cast<char *>::cast(builtin_empty(__str_rename_at_1892_369) ? das_auto_cast<char * const >::cast(nullptr) : das_auto_cast<char * const >::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1898_371) DAS_AOT_INLINE_LAMBDA -> void{
        das_copy(__regex_rename_at_1892_368.match,das_cast<uint8_t const  *>::cast(__str_rename_at_1892_369));
        regex::ReNode * __root_rename_at_1900_372 = __regex_rename_at_1892_368.root;
        uint8_t const  * __pstr_rename_at_1901_373 = das_cast<uint8_t const  *>::cast(__str_rename_at_1892_369);
        uint8_t const  * __cstr_rename_at_1902_374 = __pstr_rename_at_1901_373;
        if ( __regex_rename_at_1892_368.canEarlyOut )
        {
            while ( true )
            {
                int32_t __Ch_rename_at_1906_375 = ((int32_t)int32_t((*(__cstr_rename_at_1902_374))));
                if ( __Ch_rename_at_1906_375 == 0 )
                {
                    break;
                } else {
                    if ( is_char_in_set(__Ch_rename_at_1906_375,das_arg<TDim<uint32_t,8>>::pass(__regex_rename_at_1892_368.earlyOut),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        uint8_t const  * __om_rename_at_1911_376 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1900_372->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1892_368),__root_rename_at_1900_372,__cstr_rename_at_1902_374));
                        if ( __om_rename_at_1911_376 != nullptr )
                        {
                            char * __repl_rename_at_1913_377 = ((char *)(char *)(das_invoke<char *>::invoke<char * const >(__context__,nullptr,__blk_rename_at_1892_370,((char * const )(builtin_string_slice1(__str_rename_at_1892_369,int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1902_374),das_auto_cast<void * const >::cast(__pstr_rename_at_1901_373),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1911_376),das_auto_cast<void * const >::cast(__pstr_rename_at_1901_373),1)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_1898_371),cast<char * const >::from(__repl_rename_at_1913_377)));
                            if ( __om_rename_at_1911_376 == __cstr_rename_at_1902_374 )
                            {
                                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1898_371),__Ch_rename_at_1906_375);
                                das_ptr_inc(__cstr_rename_at_1902_374,1);
                            } else {
                                das_copy(__cstr_rename_at_1902_374,__om_rename_at_1911_376);
                            };
                        } else {
                            write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1898_371),__Ch_rename_at_1906_375);
                            das_ptr_inc(__cstr_rename_at_1902_374,1);
                        };
                    } else {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1898_371),__Ch_rename_at_1906_375);
                        das_ptr_inc(__cstr_rename_at_1902_374,1);
                    };
                };
            };
        } else {
            while ( int32_t((*(__cstr_rename_at_1902_374))) != 0 )
            {
                uint8_t const  * __om_rename_at_1932_378 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_1900_372->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_1892_368),__root_rename_at_1900_372,__cstr_rename_at_1902_374));
                if ( __om_rename_at_1932_378 != nullptr )
                {
                    char * __repl_rename_at_1934_379 = ((char *)(char *)(das_invoke<char *>::invoke<char * const >(__context__,nullptr,__blk_rename_at_1892_370,((char * const )(builtin_string_slice1(__str_rename_at_1892_369,int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_1902_374),das_auto_cast<void * const >::cast(__pstr_rename_at_1901_373),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_1932_378),das_auto_cast<void * const >::cast(__pstr_rename_at_1901_373),1)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_1898_371),cast<char * const >::from(__repl_rename_at_1934_379)));
                    if ( __om_rename_at_1932_378 == __cstr_rename_at_1902_374 )
                    {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1898_371),int32_t((*(__cstr_rename_at_1902_374))));
                        das_ptr_inc(__cstr_rename_at_1902_374,1);
                    } else {
                        das_copy(__cstr_rename_at_1902_374,__om_rename_at_1932_378);
                    };
                } else {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1898_371),int32_t((*(__cstr_rename_at_1902_374))));
                    das_ptr_inc(__cstr_rename_at_1902_374,1);
                };
            };
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline char * expand_replacement_ad68a38d9900aed3 ( Context * __context__, regex::Regex &  __regex_rename_at_1952_380, char * const  __str_rename_at_1952_381, char * const  __replacement_rename_at_1952_382, int32_t __match_start_rename_at_1952_383, int32_t __match_end_rename_at_1952_384 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1955_385) DAS_AOT_INLINE_LAMBDA -> void{
        int32_t __i_rename_at_1956_386 = 0;
        int32_t __rlen_rename_at_1957_387 = ((int32_t)builtin_string_length(__replacement_rename_at_1952_382,__context__));
        while ( __i_rename_at_1956_386 < __rlen_rename_at_1957_387 )
        {
            int32_t __ch_rename_at_1959_388 = ((int32_t)get_character_at(__replacement_rename_at_1952_382,__i_rename_at_1956_386,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            if ( (__ch_rename_at_1959_388 == 36) && ((__i_rename_at_1956_386 + 1) < __rlen_rename_at_1957_387) )
            {
                int32_t __nch_rename_at_1961_389 = ((int32_t)get_character_at(__replacement_rename_at_1952_382,__i_rename_at_1956_386 + 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
                if ( __nch_rename_at_1961_389 == 36 )
                {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1955_385),36);
                    __i_rename_at_1956_386 += 2;
                } else if ( (__nch_rename_at_1961_389 == 38) || (__nch_rename_at_1961_389 == 48) )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_1955_385),cast<char * const >::from(((char * const )(builtin_string_slice1(__str_rename_at_1952_381,__match_start_rename_at_1952_383,__match_end_rename_at_1952_384,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                    __i_rename_at_1956_386 += 2;
                } else if ( (__nch_rename_at_1961_389 >= 49) && (__nch_rename_at_1961_389 <= 57) )
                {
                    int32_t __group_num_rename_at_1969_390 = ((int32_t)(__nch_rename_at_1961_389 - 48));
                    if ( __group_num_rename_at_1969_390 < builtin_array_size(das_arg<TArray<TTuple<16,range,char *>>>::pass(__regex_rename_at_1952_380.groups)) )
                    {
                        range __grng_rename_at_1971_391 = ((range)das_get_tuple_field<range,0>::get(__regex_rename_at_1952_380.groups(__group_num_rename_at_1969_390,__context__)));
                        das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_1955_385),cast<char * const >::from(((char * const )(builtin_string_slice1(__str_rename_at_1952_381,v_extract_xi(v_cast_vec4i(__grng_rename_at_1971_391)) /*x*/,v_extract_yi(v_cast_vec4i(__grng_rename_at_1971_391)) /*y*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                    };
                    __i_rename_at_1956_386 += 2;
                } else if ( __nch_rename_at_1961_389 == 123 )
                {
                    int32_t __close_rename_at_1976_392 = ((int32_t)builtin_string_find1(__replacement_rename_at_1952_382,((char *) "}"),__i_rename_at_1956_386 + 2,__context__));
                    if ( __close_rename_at_1976_392 != -1 )
                    {
                        char * __name_rename_at_1978_393 = ((char *)(char *)(((char * const )(builtin_string_slice1(__replacement_rename_at_1952_382,__i_rename_at_1956_386 + 2,__close_rename_at_1976_392,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                        bool __is_numeric_rename_at_1980_394 = !(builtin_empty(__name_rename_at_1978_393));
                        {
                            bool __need_loop_1981 = true;
                            // nc: int const
                            das_iterator<char * const > __nc_iterator(__name_rename_at_1978_393);
                            int32_t __nc_rename_at_1981_395;
                            __need_loop_1981 = __nc_iterator.first(__context__,(__nc_rename_at_1981_395)) && __need_loop_1981;
                            for ( ; __need_loop_1981 ; __need_loop_1981 = __nc_iterator.next(__context__,(__nc_rename_at_1981_395)) )
                            {
                                if ( (__nc_rename_at_1981_395 < 48) || (__nc_rename_at_1981_395 > 57) )
                                {
                                    das_copy(__is_numeric_rename_at_1980_394,false);
                                    break;
                                };
                            }
                            __nc_iterator.close(__context__,(__nc_rename_at_1981_395));
                        };
                        if ( __is_numeric_rename_at_1980_394 )
                        {
                            int32_t __group_num_rename_at_1988_396 = 0;
                            {
                                bool __need_loop_1989 = true;
                                // nc: int const
                                das_iterator<char * const > __nc_iterator(__name_rename_at_1978_393);
                                int32_t __nc_rename_at_1989_397;
                                __need_loop_1989 = __nc_iterator.first(__context__,(__nc_rename_at_1989_397)) && __need_loop_1989;
                                for ( ; __need_loop_1989 ; __need_loop_1989 = __nc_iterator.next(__context__,(__nc_rename_at_1989_397)) )
                                {
                                    das_copy(__group_num_rename_at_1988_396,(__group_num_rename_at_1988_396 * 10) + (__nc_rename_at_1989_397 - 48));
                                }
                                __nc_iterator.close(__context__,(__nc_rename_at_1989_397));
                            };
                            if ( __group_num_rename_at_1988_396 == 0 )
                            {
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_1955_385),cast<char * const >::from(((char * const )(builtin_string_slice1(__str_rename_at_1952_381,__match_start_rename_at_1952_383,__match_end_rename_at_1952_384,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                            } else if ( __group_num_rename_at_1988_396 < builtin_array_size(das_arg<TArray<TTuple<16,range,char *>>>::pass(__regex_rename_at_1952_380.groups)) )
                            {
                                range __grng_rename_at_1995_398 = ((range)das_get_tuple_field<range,0>::get(__regex_rename_at_1952_380.groups(__group_num_rename_at_1988_396,__context__)));
                                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_1955_385),cast<char * const >::from(((char * const )(builtin_string_slice1(__str_rename_at_1952_381,v_extract_xi(v_cast_vec4i(__grng_rename_at_1995_398)) /*x*/,v_extract_yi(v_cast_vec4i(__grng_rename_at_1995_398)) /*y*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
                            };
                        } else {
                            char * __grp_rename_at_1999_399 = ((char *)(char *)(regex_group_by_name_14993f975288220c(__context__,das_arg<regex::Regex>::pass(__regex_rename_at_1952_380),__name_rename_at_1978_393,__str_rename_at_1952_381)));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_1955_385),cast<char * const >::from(__grp_rename_at_1999_399)));
                        };
                        das_copy(__i_rename_at_1956_386,__close_rename_at_1976_392 + 1);
                    } else {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1955_385),__ch_rename_at_1959_388);
                        ++__i_rename_at_1956_386;
                    };
                } else {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1955_385),__ch_rename_at_1959_388);
                    ++__i_rename_at_1956_386;
                };
            } else {
                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1955_385),__ch_rename_at_1959_388);
                ++__i_rename_at_1956_386;
            };
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * regex_replace_7cc2deb86b39b5c3 ( Context * __context__, regex::Regex &  __regex_rename_at_2020_400, char * const  __str_rename_at_2020_401, char * const  __replacement_rename_at_2020_402 )
{
    return das_auto_cast<char *>::cast(builtin_empty(__str_rename_at_2020_401) ? das_auto_cast<char * const >::cast(nullptr) : das_auto_cast<char * const >::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_2029_403) DAS_AOT_INLINE_LAMBDA -> void{
        das_copy(__regex_rename_at_2020_400.match,das_cast<uint8_t const  *>::cast(__str_rename_at_2020_401));
        regex::ReNode * __root_rename_at_2031_404 = __regex_rename_at_2020_400.root;
        uint8_t const  * __pstr_rename_at_2032_405 = das_cast<uint8_t const  *>::cast(__str_rename_at_2020_401);
        uint8_t const  * __cstr_rename_at_2033_406 = __pstr_rename_at_2032_405;
        if ( __regex_rename_at_2020_400.canEarlyOut )
        {
            while ( true )
            {
                int32_t __Ch_rename_at_2036_407 = ((int32_t)int32_t((*(__cstr_rename_at_2033_406))));
                if ( __Ch_rename_at_2036_407 == 0 )
                {
                    break;
                } else {
                    if ( is_char_in_set(__Ch_rename_at_2036_407,das_arg<TDim<uint32_t,8>>::pass(__regex_rename_at_2020_400.earlyOut),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        uint8_t const  * __om_rename_at_2041_408 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_2031_404->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_2020_400),__root_rename_at_2031_404,__cstr_rename_at_2033_406));
                        if ( __om_rename_at_2041_408 != nullptr )
                        {
                            int32_t __match_start_rename_at_2043_409 = ((int32_t)int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_2033_406),das_auto_cast<void * const >::cast(__pstr_rename_at_2032_405),1)));
                            int32_t __match_end_rename_at_2044_410 = ((int32_t)int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_2041_408),das_auto_cast<void * const >::cast(__pstr_rename_at_2032_405),1)));
                            char * __repl_rename_at_2045_411 = ((char *)(char *)(expand_replacement_ad68a38d9900aed3(__context__,das_arg<regex::Regex>::pass(__regex_rename_at_2020_400),__str_rename_at_2020_401,__replacement_rename_at_2020_402,__match_start_rename_at_2043_409,__match_end_rename_at_2044_410)));
                            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_2029_403),cast<char * const >::from(__repl_rename_at_2045_411)));
                            if ( __om_rename_at_2041_408 == __cstr_rename_at_2033_406 )
                            {
                                write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_2029_403),__Ch_rename_at_2036_407);
                                das_ptr_inc(__cstr_rename_at_2033_406,1);
                            } else {
                                das_copy(__cstr_rename_at_2033_406,__om_rename_at_2041_408);
                            };
                        } else {
                            write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_2029_403),__Ch_rename_at_2036_407);
                            das_ptr_inc(__cstr_rename_at_2033_406,1);
                        };
                    } else {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_2029_403),__Ch_rename_at_2036_407);
                        das_ptr_inc(__cstr_rename_at_2033_406,1);
                    };
                };
            };
        } else {
            while ( int32_t((*(__cstr_rename_at_2033_406))) != 0 )
            {
                uint8_t const  * __om_rename_at_2064_412 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_2031_404->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_2020_400),__root_rename_at_2031_404,__cstr_rename_at_2033_406));
                if ( __om_rename_at_2064_412 != nullptr )
                {
                    int32_t __match_start_rename_at_2066_413 = ((int32_t)int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_2033_406),das_auto_cast<void * const >::cast(__pstr_rename_at_2032_405),1)));
                    int32_t __match_end_rename_at_2067_414 = ((int32_t)int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_2064_412),das_auto_cast<void * const >::cast(__pstr_rename_at_2032_405),1)));
                    char * __repl_rename_at_2068_415 = ((char *)(char *)(expand_replacement_ad68a38d9900aed3(__context__,das_arg<regex::Regex>::pass(__regex_rename_at_2020_400),__str_rename_at_2020_401,__replacement_rename_at_2020_402,__match_start_rename_at_2066_413,__match_end_rename_at_2067_414)));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_2029_403),cast<char * const >::from(__repl_rename_at_2068_415)));
                    if ( __om_rename_at_2064_412 == __cstr_rename_at_2033_406 )
                    {
                        write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_2029_403),int32_t((*(__cstr_rename_at_2033_406))));
                        das_ptr_inc(__cstr_rename_at_2033_406,1);
                    } else {
                        das_copy(__cstr_rename_at_2033_406,__om_rename_at_2064_412);
                    };
                } else {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_2029_403),int32_t((*(__cstr_rename_at_2033_406))));
                    das_ptr_inc(__cstr_rename_at_2033_406,1);
                };
            };
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline int2 regex_search_712ff63ef772d38d ( Context * __context__, regex::Regex &  __regex_rename_at_2087_416, char * const  __str_rename_at_2087_417, int32_t __offset_rename_at_2087_418 )
{
    if ( builtin_empty(__str_rename_at_2087_417) )
    {
        return das_auto_cast<int2>::cast(int2(-1,-1));
    } else {
        das_copy(__regex_rename_at_2087_416.match,das_cast<uint8_t const  *>::cast(__str_rename_at_2087_417));
        regex::ReNode * __root_rename_at_2096_419 = __regex_rename_at_2087_416.root;
        uint8_t const  * __pstr_rename_at_2097_420 = das_cast<uint8_t const  *>::cast(__str_rename_at_2087_417);
        uint8_t const  * __cstr_rename_at_2098_421 = das_ptr_add_int32(__pstr_rename_at_2097_420,__offset_rename_at_2087_418,1);
        if ( __regex_rename_at_2087_416.canEarlyOut )
        {
            while ( true )
            {
                int32_t __Ch_rename_at_2101_422 = ((int32_t)int32_t((*(__cstr_rename_at_2098_421))));
                if ( __Ch_rename_at_2101_422 == 0 )
                {
                    break;
                } else {
                    if ( is_char_in_set(__Ch_rename_at_2101_422,das_arg<TDim<uint32_t,8>>::pass(__regex_rename_at_2087_416.earlyOut),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))) )
                    {
                        uint8_t const  * __om_rename_at_2106_423 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_2096_419->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_2087_416),__root_rename_at_2096_419,__cstr_rename_at_2098_421));
                        if ( __om_rename_at_2106_423 != nullptr )
                        {
                            return das_auto_cast<int2>::cast(int2(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_2098_421),das_auto_cast<void * const >::cast(__pstr_rename_at_2097_420),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_2106_423),das_auto_cast<void * const >::cast(__pstr_rename_at_2097_420),1))));
                        };
                    };
                    das_ptr_inc(__cstr_rename_at_2098_421,1);
                };
            };
        } else {
            while ( int32_t((*(__cstr_rename_at_2098_421))) != 0 )
            {
                uint8_t const  * __om_rename_at_2115_424 = ((uint8_t const  *)das_invoke_function<uint8_t const  *>::invoke<regex::Regex &,regex::ReNode *,uint8_t const  *>(__context__,nullptr,__root_rename_at_2096_419->fun2,das_arg<regex::Regex>::pass(__regex_rename_at_2087_416),__root_rename_at_2096_419,__cstr_rename_at_2098_421));
                if ( __om_rename_at_2115_424 != nullptr )
                {
                    return das_auto_cast<int2>::cast(int2(int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__cstr_rename_at_2098_421),das_auto_cast<void * const >::cast(__pstr_rename_at_2097_420),1)),int32_t(i_das_ptr_diff(das_auto_cast<void * const >::cast(__om_rename_at_2115_424),das_auto_cast<void * const >::cast(__pstr_rename_at_2097_420),1))));
                } else {
                    das_ptr_inc(__cstr_rename_at_2098_421,1);
                };
            };
        };
        return das_auto_cast<int2>::cast(int2(-1,-1));
    };
}

inline TArray<char *> regex_split_ad2c6f596b0add93 ( Context * __context__, regex::Regex &  __regex_rename_at_2126_425, char * const  __str_rename_at_2126_426 ) { das_stack_prologue __prologue(__context__,112,"regex_split " DAS_FILE_LINE);
{
    TArray<char *> __result_rename_at_2128_427;das_zero(__result_rename_at_2128_427);
    if ( builtin_empty(__str_rename_at_2126_426) )
    {
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a5393f36ccdac4a8(__context__,das_arg<TArray<char *>>::pass(__result_rename_at_2128_427)));
    } else {
        int32_t __last_end_rename_at_2132_428 = 0;
        regex_foreach_2cf0e40b2436b6d9(__context__,das_arg<regex::Regex>::pass(__regex_rename_at_2126_425),__str_rename_at_2126_426,das_make_block<bool,range>(__context__,96,0,&__func_info__61179b2f43878551,[&](range __at_rename_at_2133_429) -> bool{
            _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49(__context__,das_arg<TArray<char *>>::pass(__result_rename_at_2128_427),((char * const )(builtin_string_slice1(__str_rename_at_2126_426,__last_end_rename_at_2132_428,v_extract_xi(v_cast_vec4i(__at_rename_at_2133_429)) /*x*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            das_copy(__last_end_rename_at_2132_428,das_swizzle_ref<int32_t const ,range const ,1>::swizzle(__at_rename_at_2133_429) /*y*/);
            return das_auto_cast<bool>::cast(true);
        }));
        _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49(__context__,das_arg<TArray<char *>>::pass(__result_rename_at_2128_427),((char * const )(builtin_string_slice2(__str_rename_at_2126_426,__last_end_rename_at_2132_428,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a5393f36ccdac4a8(__context__,das_arg<TArray<char *>>::pass(__result_rename_at_2128_427)));
    };
}}

inline TArray<range> regex_match_all_dc6b47bf82eb552 ( Context * __context__, regex::Regex &  __regex_rename_at_2142_430, char * const  __str_rename_at_2142_431 ) { das_stack_prologue __prologue(__context__,96,"regex_match_all " DAS_FILE_LINE);
{
    TArray<range> __result_rename_at_2144_432;das_zero(__result_rename_at_2144_432);
    regex_foreach_2cf0e40b2436b6d9(__context__,das_arg<regex::Regex>::pass(__regex_rename_at_2142_430),__str_rename_at_2142_431,das_make_block<bool,range>(__context__,80,0,&__func_info__61179b2f43878551,[&](range __at_rename_at_2145_433) -> bool{
        _FuncbuiltinTickpushTick14133213201864676143_32d7c798299302f2(__context__,das_arg<TArray<range>>::pass(__result_rename_at_2144_432),__at_rename_at_2145_433);
        return das_auto_cast<bool>::cast(true);
    }));
    return /* <- */ das_auto_cast_move<TArray<range>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1b414e731a438fa8(__context__,das_arg<TArray<range>>::pass(__result_rename_at_2144_432)));
}}

inline char * regex_group_by_name_14993f975288220c ( Context * __context__, regex::Regex const  &  __regex_rename_at_2152_434, char * const  __name_rename_at_2152_435, char * const  __str_rename_at_2152_436 )
{
    {
        bool __need_loop_2155 = true;
        // g: tuple<range;string> const&
        das_iterator<TArray<TTuple<16,range,char *>> const > __g_iterator(__regex_rename_at_2152_434.groups);
        TTuple<16,range,char *> const  * __g_rename_at_2155_437;
        __need_loop_2155 = __g_iterator.first(__context__,(__g_rename_at_2155_437)) && __need_loop_2155;
        for ( ; __need_loop_2155 ; __need_loop_2155 = __g_iterator.next(__context__,(__g_rename_at_2155_437)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(das_get_tuple_field<char *,8>::get((*__g_rename_at_2155_437))),cast<char *>::from(__name_rename_at_2152_435),*__context__,nullptr) )
            {
                return das_auto_cast<char *>::cast(((char * const )(builtin_string_slice1(__str_rename_at_2152_436,v_extract_xi(v_cast_vec4i(das_get_tuple_field<range,0>::get((*__g_rename_at_2155_437)))) /*x*/,v_extract_yi(v_cast_vec4i(das_get_tuple_field<range,0>::get((*__g_rename_at_2155_437)))) /*y*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            };
        }
        __g_iterator.close(__context__,(__g_rename_at_2155_437));
    };
    return das_auto_cast<char *>::cast(nullptr);
}

inline range _FuncSqblSqbr_f97767f82664a020 ( Context * __context__, regex::Regex const  &  __regex_rename_at_2163_438, int32_t __index_rename_at_2163_439 )
{
    return das_auto_cast<range>::cast(das_get_tuple_field<range,0>::get(__regex_rename_at_2163_438.groups(__index_rename_at_2163_439,__context__)));
}

inline range _FuncSqblSqbr_9dbe1a1c2a54dde3 ( Context * __context__, regex::Regex const  &  __regex_rename_at_2168_440, char * const  __name_rename_at_2168_441 )
{
    {
        bool __need_loop_2171 = true;
        // g: tuple<range;string> const&
        das_iterator<TArray<TTuple<16,range,char *>> const > __g_iterator(__regex_rename_at_2168_440.groups);
        TTuple<16,range,char *> const  * __g_rename_at_2171_442;
        __need_loop_2171 = __g_iterator.first(__context__,(__g_rename_at_2171_442)) && __need_loop_2171;
        for ( ; __need_loop_2171 ; __need_loop_2171 = __g_iterator.next(__context__,(__g_rename_at_2171_442)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(das_get_tuple_field<char *,8>::get((*__g_rename_at_2171_442))),cast<char *>::from(__name_rename_at_2168_441),*__context__,nullptr) )
            {
                return das_auto_cast<range>::cast(das_get_tuple_field<range,0>::get((*__g_rename_at_2171_442)));
            };
        }
        __g_iterator.close(__context__,(__g_rename_at_2171_442));
    };
    return das_auto_cast<range>::cast(range(0,0));
}

inline void regex_debug_55af4874d0243e85 ( Context * __context__, regex::Regex const  &  __regex_rename_at_2183_443 )
{
    debug_re_c9a788d56ecca6c1(__context__,__regex_rename_at_2183_443.root,1);
    builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __regex_rename_at_2183_443.canEarlyOut )
    {
        builtin_print(((char *) "early out: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_set_c7915a561c802ef5(__context__,__regex_rename_at_2183_443.earlyOut);
        builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( builtin_array_size(__regex_rename_at_2183_443.groups) != 0 )
    {
        builtin_print(((char *) "groups:"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        {
            bool __need_loop_2194 = true;
            // g: tuple<range;string> const&
            das_iterator<TArray<TTuple<16,range,char *>> const > __g_iterator(__regex_rename_at_2183_443.groups);
            TTuple<16,range,char *> const  * __g_rename_at_2194_444;
            __need_loop_2194 = __g_iterator.first(__context__,(__g_rename_at_2194_444)) && __need_loop_2194;
            for ( ; __need_loop_2194 ; __need_loop_2194 = __g_iterator.next(__context__,(__g_rename_at_2194_444)) )
            {
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<char * const >::from(((char *) "\t")), cast<char * const >::from(das_get_tuple_field<char *,8>::get((*__g_rename_at_2194_444))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            }
            __g_iterator.close(__context__,(__g_rename_at_2194_444));
        };
    };
}

inline void debug_set_c7915a561c802ef5 ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_2200_445 )
{
    {
        bool __need_loop_2202 = true;
        // x: int const
        das_iterator<range> __x_iterator(range(0,256));
        int32_t __x_rename_at_2202_446;
        __need_loop_2202 = __x_iterator.first(__context__,(__x_rename_at_2202_446)) && __need_loop_2202;
        for ( ; __need_loop_2202 ; __need_loop_2202 = __x_iterator.next(__context__,(__x_rename_at_2202_446)) )
        {
            if ( (__cset_rename_at_2200_445((SimPolicy<int32_t>::BinShr(__x_rename_at_2202_446,5,*__context__,nullptr)),__context__) & uint32_t(SimPolicy<int32_t>::BinShl(1,(__x_rename_at_2202_446 & 31),*__context__,nullptr))) != 0x0u )
            {
                if ( (__x_rename_at_2202_446 >= 32) && (__x_rename_at_2202_446 <= 127) )
                {
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_21, cast<char * const >::from(((char * const )(to_string_char(__x_rename_at_2202_446,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_22, cast<char * const >::from(((char *) "\\")), cast<int32_t>::from(__x_rename_at_2202_446))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            };
        }
        __x_iterator.close(__context__,(__x_rename_at_2202_446));
    };
}

inline void debug_re_c9a788d56ecca6c1 ( Context * __context__, regex::ReNode * const  __node_rename_at_2213_447, int32_t __tab_rename_at_2213_448 )
{
    builtin_print(((char *) "("),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __node_rename_at_2213_447 != nullptr )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_22, cast<char * const >::from(((char *) "#")), cast<int32_t>::from(__node_rename_at_2213_447->id))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        if ( __node_rename_at_2213_447->next != nullptr )
        {
            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_22, cast<char * const >::from(((char *) "->")), cast<int32_t>::from(__node_rename_at_2213_447->next->id))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            builtin_print(((char *) "->!"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        builtin_print(((char *) " "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    if ( __node_rename_at_2213_447 == nullptr )
    {
        builtin_print(((char *) "null)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Char )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<char * const >::from(((char *) "Char `")), cast<char * const >::from(__node_rename_at_2213_447->text), cast<char * const >::from(((char *) "`)")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Set )
    {
        builtin_print(((char *) "Set "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_set_c7915a561c802ef5(__context__,__node_rename_at_2213_447->cset);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Any )
    {
        builtin_print(((char *) "Any)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Eos )
    {
        builtin_print(((char *) "Eos)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Group )
    {
        if ( __node_rename_at_2213_447->index == -1 )
        {
            builtin_print(((char *) "NonCapGroup "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else if ( !builtin_empty(__node_rename_at_2213_447->text) )
        {
            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<char * const >::from(((char *) "Group<")), cast<char * const >::from(__node_rename_at_2213_447->text), cast<char * const >::from(((char *) "> ")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            builtin_print(((char *) "Group "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        debug_re_c9a788d56ecca6c1(__context__,__node_rename_at_2213_447->subexpr,__tab_rename_at_2213_448 + 1);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Plus )
    {
        builtin_print(__node_rename_at_2213_447->lazy ? das_auto_cast<char * const >::cast(((char *) "Plus? ")) : das_auto_cast<char * const >::cast(((char *) "Plus ")),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_c9a788d56ecca6c1(__context__,__node_rename_at_2213_447->subexpr,__tab_rename_at_2213_448);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Star )
    {
        builtin_print(__node_rename_at_2213_447->lazy ? das_auto_cast<char * const >::cast(((char *) "Star? ")) : das_auto_cast<char * const >::cast(((char *) "Star ")),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_c9a788d56ecca6c1(__context__,__node_rename_at_2213_447->subexpr,__tab_rename_at_2213_448);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Question )
    {
        builtin_print(__node_rename_at_2213_447->lazy ? das_auto_cast<char * const >::cast(((char *) "Question? ")) : das_auto_cast<char * const >::cast(((char *) "Question ")),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_c9a788d56ecca6c1(__context__,__node_rename_at_2213_447->subexpr,__tab_rename_at_2213_448);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Concat )
    {
        builtin_print(((char *) "Contact"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_27, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_2213_448,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_c9a788d56ecca6c1(__context__,__node_rename_at_2213_447->left,__tab_rename_at_2213_448 + 1);
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_27, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_2213_448,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_c9a788d56ecca6c1(__context__,__node_rename_at_2213_447->right,__tab_rename_at_2213_448 + 1);
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_2213_448 - 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ")")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        builtin_print(((char *) "Union\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        {
            bool __need_loop_2267 = true;
            // sub: regex::ReNode? const&
            das_iterator<TArray<regex::ReNode *> const > __sub_iterator(__node_rename_at_2213_447->all);
            regex::ReNode * const  * __sub_rename_at_2267_449;
            __need_loop_2267 = __sub_iterator.first(__context__,(__sub_rename_at_2267_449)) && __need_loop_2267;
            for ( ; __need_loop_2267 ; __need_loop_2267 = __sub_iterator.next(__context__,(__sub_rename_at_2267_449)) )
            {
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_21, cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_2213_448,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                debug_re_c9a788d56ecca6c1(__context__,(*__sub_rename_at_2267_449),__tab_rename_at_2213_448 + 1);
                builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            }
            __sub_iterator.close(__context__,(__sub_rename_at_2267_449));
        };
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_27, cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_2213_448 - 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ")")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Bos )
    {
        builtin_print(((char *) "Bos)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Repeat )
    {
        builtin_print(__node_rename_at_2213_447->lazy ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_32, cast<char * const >::from(((char *) "Repeat? {")), cast<int32_t>::from(__node_rename_at_2213_447->min_rep), cast<char * const >::from(((char *) ",")), cast<int32_t>::from(__node_rename_at_2213_447->max_rep), cast<char * const >::from(((char *) "} "))))) : das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_32, cast<char * const >::from(((char *) "Repeat {")), cast<int32_t>::from(__node_rename_at_2213_447->min_rep), cast<char * const >::from(((char *) ",")), cast<int32_t>::from(__node_rename_at_2213_447->max_rep), cast<char * const >::from(((char *) "} "))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_c9a788d56ecca6c1(__context__,__node_rename_at_2213_447->subexpr,__tab_rename_at_2213_448);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::WordBoundary )
    {
        builtin_print(((char *) "WordBoundary)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::NonWordBoundary )
    {
        builtin_print(((char *) "NonWordBoundary)"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::Lookahead )
    {
        builtin_print(((char *) "Lookahead "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_c9a788d56ecca6c1(__context__,__node_rename_at_2213_447->subexpr,__tab_rename_at_2213_448 + 1);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_2213_447->op == DAS_COMMENT(enum) regex::ReOp::NegativeLookahead )
    {
        builtin_print(((char *) "NegLookahead "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_c9a788d56ecca6c1(__context__,__node_rename_at_2213_447->subexpr,__tab_rename_at_2213_448 + 1);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        builtin_throw(((char *) "unsupported op"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline int32_t rnd_set_character_a6ace307aa9880d1 ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_2298_450, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2298_451 )
{
    uint32_t __total_rename_at_2299_452 = ((uint32_t)char_set_total(__cset_rename_at_2298_450));
    uint32_t __idx_rename_at_2300_453 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2298_451),__idx_rename_at_2300_453);
    das_copy(__idx_rename_at_2300_453,SimPolicy<uint32_t>::Mod(__idx_rename_at_2300_453,__total_rename_at_2299_452,*__context__,nullptr));
    return das_auto_cast<int32_t>::cast(char_set_element(int32_t(__idx_rename_at_2300_453),__cset_rename_at_2298_450));
}

inline void re_gen2_fail_b917d161e3783b38 ( Context * __context__, regex::ReNode * __node_rename_at_2306_454, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2306_455, StringBuilderWriter &  __str_rename_at_2306_456 )
{
    DAS_ASSERTF((false),(((char *) "not running generated regex")));
}

inline void re_gen2_char_4112c00708ab51c6 ( Context * __context__, regex::ReNode * __node_rename_at_2313_457, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2313_458, StringBuilderWriter &  __str_rename_at_2313_459 )
{
    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_34,cast<StringBuilderWriter &>::from(__str_rename_at_2313_459),cast<char *>::from(__node_rename_at_2313_457->text)));
    regex::ReNode * __node2_rename_at_2318_460 = __node_rename_at_2313_457->next;
    if ( __node2_rename_at_2318_460 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2318_460->gen2,__node2_rename_at_2318_460,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2313_458),das_arg<StringBuilderWriter>::pass(__str_rename_at_2313_459));
    };
}

inline void re_gen2_union_9b028b61c503f834 ( Context * __context__, regex::ReNode * __node_rename_at_2324_461, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2324_462, StringBuilderWriter &  __str_rename_at_2324_463 )
{
    uint32_t __nidx_rename_at_2328_464 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2324_462),__nidx_rename_at_2328_464);
    das_copy(__nidx_rename_at_2328_464,SimPolicy<uint32_t>::Mod(__nidx_rename_at_2328_464,uint32_t(builtin_array_size(das_arg<TArray<regex::ReNode *>>::pass(__node_rename_at_2324_461->all))),*__context__,nullptr));
    regex::ReNode * __sub_rename_at_2331_465 = __node_rename_at_2324_461->all(__nidx_rename_at_2328_464,__context__);
    das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__sub_rename_at_2331_465->gen2,__sub_rename_at_2331_465,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2324_462),das_arg<StringBuilderWriter>::pass(__str_rename_at_2324_463));
}

inline void re_gen2_set_b1d4208a13b42b9 ( Context * __context__, regex::ReNode * __node_rename_at_2335_466, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2335_467, StringBuilderWriter &  __str_rename_at_2335_468 )
{
    write_string_char(das_arg<StringBuilderWriter>::pass(__str_rename_at_2335_468),rnd_set_character_a6ace307aa9880d1(__context__,das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_2335_466->cset),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2335_467)));
    regex::ReNode * __node2_rename_at_2340_469 = __node_rename_at_2335_466->next;
    if ( __node2_rename_at_2340_469 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2340_469->gen2,__node2_rename_at_2340_469,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2335_467),das_arg<StringBuilderWriter>::pass(__str_rename_at_2335_468));
    };
}

inline void re_gen2_any_5e88209f8e29e4e5 ( Context * __context__, regex::ReNode * __node_rename_at_2346_470, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2346_471, StringBuilderWriter &  __str_rename_at_2346_472 )
{
    uint32_t __ch_rename_at_2350_473 = 0;
    _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2346_471),__ch_rename_at_2350_473);
    das_copy(__ch_rename_at_2350_473,(SimPolicy<uint32_t>::Mod(__ch_rename_at_2350_473,0xfeu,*__context__,nullptr)) + 0x1u);
    if ( __ch_rename_at_2350_473 >= 0xau )
    {
        ++__ch_rename_at_2350_473;
    };
    write_string_char(das_arg<StringBuilderWriter>::pass(__str_rename_at_2346_472),int32_t(__ch_rename_at_2350_473));
    regex::ReNode * __node2_rename_at_2357_474 = __node_rename_at_2346_470->next;
    if ( __node2_rename_at_2357_474 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2357_474->gen2,__node2_rename_at_2357_474,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2346_471),das_arg<StringBuilderWriter>::pass(__str_rename_at_2346_472));
    };
}

inline void re_gen2_concat_30030241eb92a8af ( Context * __context__, regex::ReNode * __node_rename_at_2363_475, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2363_476, StringBuilderWriter &  __str_rename_at_2363_477 )
{
    regex::ReNode * __left_rename_at_2367_478 = __node_rename_at_2363_475->left;
    das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__left_rename_at_2367_478->gen2,__left_rename_at_2367_478,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2363_476),das_arg<StringBuilderWriter>::pass(__str_rename_at_2363_477));
}

inline void re_gen2_eos_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2371_479, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2371_480, StringBuilderWriter &  __str_rename_at_2371_481 )
{
    regex::ReNode * __node2_rename_at_2375_482 = __node_rename_at_2371_479->next;
    if ( __node2_rename_at_2375_482 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2375_482->gen2,__node2_rename_at_2375_482,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2371_480),das_arg<StringBuilderWriter>::pass(__str_rename_at_2371_481));
    };
}

inline void re_gen2_question_1b31bedc870aa21a ( Context * __context__, regex::ReNode * __node_rename_at_2381_483, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2381_484, StringBuilderWriter &  __str_rename_at_2381_485 )
{
    uint32_t __a_rename_at_2385_486 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2381_484),__a_rename_at_2385_486);
    if ( (__a_rename_at_2385_486 & 0x1u) != 0x0u )
    {
        regex::ReNode * __nsub_rename_at_2388_487 = __node_rename_at_2381_483->subexpr;
        if ( __nsub_rename_at_2388_487 != nullptr )
        {
            das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_2388_487->gen2,__nsub_rename_at_2388_487,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2381_484),das_arg<StringBuilderWriter>::pass(__str_rename_at_2381_485));
        };
    };
    regex::ReNode * __node2_rename_at_2393_488 = __node_rename_at_2381_483->next;
    if ( __node2_rename_at_2393_488 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2393_488->gen2,__node2_rename_at_2393_488,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2381_484),das_arg<StringBuilderWriter>::pass(__str_rename_at_2381_485));
    };
}

inline void re_gen2_plus_2a39935c8341bc2a ( Context * __context__, regex::ReNode * __node_rename_at_2399_489, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2399_490, StringBuilderWriter &  __str_rename_at_2399_491 )
{
    uint32_t __cnt_rename_at_2403_492 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2399_490),__cnt_rename_at_2403_492);
    das_copy(__cnt_rename_at_2403_492,(SimPolicy<uint32_t>::Mod(__cnt_rename_at_2403_492,das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/,*__context__,nullptr)) + 0x1u);
    regex::ReNode * __nsub_rename_at_2406_493 = __node_rename_at_2399_489->subexpr;
    if ( __nsub_rename_at_2406_493 != nullptr )
    {
        {
            bool __need_loop_2408 = true;
            // i: uint const
            das_iterator<urange> __i_iterator(mk_urange(__cnt_rename_at_2403_492));
            uint32_t __i_rename_at_2408_494;
            __need_loop_2408 = __i_iterator.first(__context__,(__i_rename_at_2408_494)) && __need_loop_2408;
            for ( ; __need_loop_2408 ; __need_loop_2408 = __i_iterator.next(__context__,(__i_rename_at_2408_494)) )
            {
                das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_2406_493->gen2,__nsub_rename_at_2406_493,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2399_490),das_arg<StringBuilderWriter>::pass(__str_rename_at_2399_491));
            }
            __i_iterator.close(__context__,(__i_rename_at_2408_494));
        };
    };
    regex::ReNode * __node2_rename_at_2412_495 = __node_rename_at_2399_489->next;
    if ( __node2_rename_at_2412_495 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2412_495->gen2,__node2_rename_at_2412_495,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2399_490),das_arg<StringBuilderWriter>::pass(__str_rename_at_2399_491));
    };
}

inline void re_gen2_star_677f11c532be890b ( Context * __context__, regex::ReNode * __node_rename_at_2418_496, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2418_497, StringBuilderWriter &  __str_rename_at_2418_498 )
{
    uint32_t __cnt_rename_at_2422_499 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2418_497),__cnt_rename_at_2422_499);
    das_copy(__cnt_rename_at_2422_499,SimPolicy<uint32_t>::Mod(__cnt_rename_at_2422_499,das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/,*__context__,nullptr));
    regex::ReNode * __nsub_rename_at_2425_500 = __node_rename_at_2418_496->subexpr;
    if ( __nsub_rename_at_2425_500 != nullptr )
    {
        {
            bool __need_loop_2427 = true;
            // i: uint const
            das_iterator<urange> __i_iterator(mk_urange(__cnt_rename_at_2422_499));
            uint32_t __i_rename_at_2427_501;
            __need_loop_2427 = __i_iterator.first(__context__,(__i_rename_at_2427_501)) && __need_loop_2427;
            for ( ; __need_loop_2427 ; __need_loop_2427 = __i_iterator.next(__context__,(__i_rename_at_2427_501)) )
            {
                das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_2425_500->gen2,__nsub_rename_at_2425_500,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2418_497),das_arg<StringBuilderWriter>::pass(__str_rename_at_2418_498));
            }
            __i_iterator.close(__context__,(__i_rename_at_2427_501));
        };
    };
    regex::ReNode * __node2_rename_at_2431_502 = __node_rename_at_2418_496->next;
    if ( __node2_rename_at_2431_502 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2431_502->gen2,__node2_rename_at_2431_502,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2418_497),das_arg<StringBuilderWriter>::pass(__str_rename_at_2418_498));
    };
}

inline void re_gen2_group_267d4348c759899b ( Context * __context__, regex::ReNode * __node_rename_at_2437_503, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2437_504, StringBuilderWriter &  __str_rename_at_2437_505 )
{
    regex::ReNode * __nsub_rename_at_2441_506 = __node_rename_at_2437_503->subexpr;
    if ( __nsub_rename_at_2441_506 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_2441_506->gen2,__nsub_rename_at_2441_506,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2437_504),das_arg<StringBuilderWriter>::pass(__str_rename_at_2437_505));
    };
    regex::ReNode * __node2_rename_at_2445_507 = __node_rename_at_2437_503->next;
    if ( __node2_rename_at_2445_507 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2445_507->gen2,__node2_rename_at_2445_507,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2437_504),das_arg<StringBuilderWriter>::pass(__str_rename_at_2437_505));
    };
}

inline void re_gen2_bos_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2451_508, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2451_509, StringBuilderWriter &  __str_rename_at_2451_510 )
{
    regex::ReNode * __node2_rename_at_2456_511 = __node_rename_at_2451_508->next;
    if ( __node2_rename_at_2456_511 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2456_511->gen2,__node2_rename_at_2456_511,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2451_509),das_arg<StringBuilderWriter>::pass(__str_rename_at_2451_510));
    };
}

inline void re_gen2_repeat_d2475d70f23bd85 ( Context * __context__, regex::ReNode * __node_rename_at_2462_512, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2462_513, StringBuilderWriter &  __str_rename_at_2462_514 )
{
    uint32_t __cnt_rename_at_2466_515 = 0;
    if ( __node_rename_at_2462_512->max_rep == -1 )
    {
        _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2462_513),__cnt_rename_at_2466_515);
        das_copy(__cnt_rename_at_2466_515,(SimPolicy<uint32_t>::Mod(__cnt_rename_at_2466_515,das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/,*__context__,nullptr)) + uint32_t(__node_rename_at_2462_512->min_rep));
    } else if ( __node_rename_at_2462_512->max_rep == __node_rename_at_2462_512->min_rep )
    {
        das_copy(__cnt_rename_at_2466_515,uint32_t(__node_rename_at_2462_512->min_rep));
    } else {
        _FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2462_513),__cnt_rename_at_2466_515);
        das_copy(__cnt_rename_at_2466_515,uint32_t(__node_rename_at_2462_512->min_rep) + (SimPolicy<uint32_t>::Mod(__cnt_rename_at_2466_515,uint32_t((__node_rename_at_2462_512->max_rep - __node_rename_at_2462_512->min_rep) + 1),*__context__,nullptr)));
    };
    regex::ReNode * __nsub_rename_at_2476_516 = __node_rename_at_2462_512->subexpr;
    if ( __nsub_rename_at_2476_516 != nullptr )
    {
        {
            bool __need_loop_2478 = true;
            // i: uint const
            das_iterator<urange> __i_iterator(mk_urange(__cnt_rename_at_2466_515));
            uint32_t __i_rename_at_2478_517;
            __need_loop_2478 = __i_iterator.first(__context__,(__i_rename_at_2478_517)) && __need_loop_2478;
            for ( ; __need_loop_2478 ; __need_loop_2478 = __i_iterator.next(__context__,(__i_rename_at_2478_517)) )
            {
                das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_2476_516->gen2,__nsub_rename_at_2476_516,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2462_513),das_arg<StringBuilderWriter>::pass(__str_rename_at_2462_514));
            }
            __i_iterator.close(__context__,(__i_rename_at_2478_517));
        };
    };
    regex::ReNode * __node2_rename_at_2482_518 = __node_rename_at_2462_512->next;
    if ( __node2_rename_at_2482_518 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2482_518->gen2,__node2_rename_at_2482_518,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2462_513),das_arg<StringBuilderWriter>::pass(__str_rename_at_2462_514));
    };
}

inline void re_gen2_word_boundary_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2488_519, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2488_520, StringBuilderWriter &  __str_rename_at_2488_521 )
{
    regex::ReNode * __node2_rename_at_2493_522 = __node_rename_at_2488_519->next;
    if ( __node2_rename_at_2493_522 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2493_522->gen2,__node2_rename_at_2493_522,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2488_520),das_arg<StringBuilderWriter>::pass(__str_rename_at_2488_521));
    };
}

inline void re_gen2_non_word_boundary_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2499_523, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2499_524, StringBuilderWriter &  __str_rename_at_2499_525 )
{
    regex::ReNode * __node2_rename_at_2504_526 = __node_rename_at_2499_523->next;
    if ( __node2_rename_at_2504_526 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2504_526->gen2,__node2_rename_at_2504_526,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2499_524),das_arg<StringBuilderWriter>::pass(__str_rename_at_2499_525));
    };
}

inline void re_gen2_lookahead_267d4348c759899b ( Context * __context__, regex::ReNode * __node_rename_at_2510_527, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2510_528, StringBuilderWriter &  __str_rename_at_2510_529 )
{
    regex::ReNode * __nsub_rename_at_2515_530 = __node_rename_at_2510_527->subexpr;
    if ( __nsub_rename_at_2515_530 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__nsub_rename_at_2515_530->gen2,__nsub_rename_at_2515_530,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2510_528),das_arg<StringBuilderWriter>::pass(__str_rename_at_2510_529));
    };
    regex::ReNode * __node2_rename_at_2519_531 = __node_rename_at_2510_527->next;
    if ( __node2_rename_at_2519_531 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2519_531->gen2,__node2_rename_at_2519_531,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2510_528),das_arg<StringBuilderWriter>::pass(__str_rename_at_2510_529));
    };
}

inline void re_gen2_negative_lookahead_2859c3cfba5b66cf ( Context * __context__, regex::ReNode * __node_rename_at_2525_532, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2525_533, StringBuilderWriter &  __str_rename_at_2525_534 )
{
    regex::ReNode * __node2_rename_at_2530_535 = __node_rename_at_2525_532->next;
    if ( __node2_rename_at_2530_535 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_2530_535->gen2,__node2_rename_at_2530_535,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2525_533),das_arg<StringBuilderWriter>::pass(__str_rename_at_2525_534));
    };
}

inline char * re_gen_bf361240a28b8867 ( Context * __context__, regex::Regex &  __re_rename_at_2536_536, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_2536_537 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_2538_538) DAS_AOT_INLINE_LAMBDA -> void{
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__re_rename_at_2536_536.root->gen2,__re_rename_at_2536_536.root,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_2536_537),das_arg<StringBuilderWriter>::pass(__writer_rename_at_2538_538));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}
static vec4f __wrap__FuncbuiltinTickresizeTick4811697762258667383_494d51b3fa621d18 ( Context * __context__ ) {
    TArray<regex::ReNode *> &  arg_Arr = cast_aot_arg<TArray<regex::ReNode *> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickresizeTick4811697762258667383_494d51b3fa621d18(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickfinalizeTick13836114024949725080_93105ead5b29fa0 ( Context * __context__ ) {
    TArray<regex::ReNode *> &  arg_a = cast_aot_arg<TArray<regex::ReNode *> & >::to(*__context__,__context__->abiArguments()[0]);
    _FuncbuiltinTickfinalizeTick13836114024949725080_93105ead5b29fa0(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTicklengthTick18150397773952384912_fe94899ed7cf7fdb ( Context * __context__ ) {
    TDim<regex::ReNode *,2> const  &  arg_a = cast_aot_arg<TDim<regex::ReNode *,2> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(_FuncbuiltinTicklengthTick18150397773952384912_fe94899ed7cf7fdb(__context__, arg_a));
}
static vec4f __wrap_finalize_61e451af4e714ca2 ( Context * __context__ ) {
    regex::ReNode &  arg___this = cast_aot_arg<regex::ReNode & >::to(*__context__,__context__->abiArguments()[0]);
    finalize_61e451af4e714ca2(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_9c8fefcf8a2c4d0d ( Context * __context__ ) {
    regex::Regex &  arg_a = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<regex::Regex &>::from(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_9c8fefcf8a2c4d0d(__context__, arg_a));
}
static vec4f __wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a5393f36ccdac4a8 ( Context * __context__ ) {
    TArray<char *> &  arg_a = cast_aot_arg<TArray<char *> & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<TArray<char *> &>::from(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a5393f36ccdac4a8(__context__, arg_a));
}
static vec4f __wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1b414e731a438fa8 ( Context * __context__ ) {
    TArray<range> &  arg_a = cast_aot_arg<TArray<range> & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<TArray<range> &>::from(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1b414e731a438fa8(__context__, arg_a));
}
static vec4f __wrap__FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc ( Context * __context__ ) {
    int32_t arg_ch1 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_ch2 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(_FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc(__context__, arg_ch1, arg_ch2));
}
static vec4f __wrap_finalize_4c4d68a9a950ec41 ( Context * __context__ ) {
    TVariant<16,8,regex::ReNode *,void *> &  arg___this = cast_aot_arg<TVariant<16,8,regex::ReNode *,void *> & >::to(*__context__,__context__->abiArguments()[0]);
    finalize_4c4d68a9a950ec41(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap_finalize_c680337a54511d24 ( Context * __context__ ) {
    regex::ReNode * & arg___this = cast_aot_arg<regex::ReNode * &>::to(*__context__,__context__->abiArguments()[0]);
    finalize_c680337a54511d24(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_906d8abb52edd211 ( Context * __context__ ) {
    TArray<regex::ReNode *> &  arg_Arr = cast_aot_arg<TArray<regex::ReNode *> & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_value = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_906d8abb52edd211(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_1d04d83c4b41cf2c ( Context * __context__ ) {
    TDim<regex::ReNode *,2> &  arg_a = cast_aot_arg<TDim<regex::ReNode *,2> & >::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<regex::ReNode *> *) __context__->abiCMRES) = _FuncbuiltinTickto_array_moveTick3185538323411982277_1d04d83c4b41cf2c(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_7df733d16209a68a ( Context * __context__ ) {
    TArray<TTuple<16,range,char *>> &  arg_Arr = cast_aot_arg<TArray<TTuple<16,range,char *>> & >::to(*__context__,__context__->abiArguments()[0]);
    TTuple<16,range,char *> &  arg_value = cast_aot_arg<TTuple<16,range,char *> & >::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_7df733d16209a68a(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606 ( Context * __context__ ) {
    TArray<uint8_t const  *> &  arg_Arr = cast_aot_arg<TArray<uint8_t const  *> & >::to(*__context__,__context__->abiArguments()[0]);
    uint8_t const  * arg_value = cast_aot_arg<uint8_t const  *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 ( Context * __context__ ) {
    TArray<char *> &  arg_Arr = cast_aot_arg<TArray<char *> & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_value = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick14133213201864676143_32d7c798299302f2 ( Context * __context__ ) {
    TArray<range> &  arg_Arr = cast_aot_arg<TArray<range> & >::to(*__context__,__context__->abiArguments()[0]);
    range arg_value = cast_aot_arg<range>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick14133213201864676143_32d7c798299302f2(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904 ( Context * __context__ ) {
    Sequence DAS_COMMENT((uint32_t)) &  arg_it = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[0]);
    uint32_t & arg_value = cast_aot_arg<uint32_t &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904(__context__, arg_it, arg_value));
}
static vec4f __wrap_re_gen_get_rep_limit_aa71779c47fee5b8 ( Context * __context__ ) {
    return cast<uint32_t>::from(re_gen_get_rep_limit_aa71779c47fee5b8(__context__));
}
static vec4f __wrap_trace_7794f549fda55dde ( Context * __context__ ) {
    char * const  arg_msg = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    trace_7794f549fda55dde(__context__, arg_msg, arg_offset);
    return v_zero();
}
static vec4f __wrap_note_7794f549fda55dde ( Context * __context__ ) {
    char * const  arg_msg = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    note_7794f549fda55dde(__context__, arg_msg, arg_offset);
    return v_zero();
}
static vec4f __wrap_trace_backtrack_60bdee73e3315732 ( Context * __context__ ) {
    trace_backtrack_60bdee73e3315732(__context__);
    return v_zero();
}
static vec4f __wrap_trace_value_f443ec2d74a0c7bd ( Context * __context__ ) {
    regex::ReNode * const  arg_value = cast_aot_arg<regex::ReNode * const >::to(*__context__,__context__->abiArguments()[0]);
    trace_value_f443ec2d74a0c7bd(__context__, arg_value);
    return v_zero();
}
static vec4f __wrap_nada_ba27de56c6f5dc72 ( Context * __context__ ) {
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = nada_ba27de56c6f5dc72(__context__);
    return v_zero();
}
static vec4f __wrap_maybe_249d36fe62a185a7 ( Context * __context__ ) {
    regex::ReNode * arg_value = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = maybe_249d36fe62a185a7(__context__, arg_value);
    return v_zero();
}
static vec4f __wrap_eos_b645f5ba09d5c5 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(eos_b645f5ba09d5c5(__context__, arg_expr, arg_offset));
}
static vec4f __wrap_at_ac9760d67b4716d0 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(at_ac9760d67b4716d0(__context__, arg_expr, arg_offset));
}
static vec4f __wrap_next_6ab20ac9bb2c573c ( Context * __context__ ) {
    TVariant<16,8,regex::ReNode *,void *> const  &  arg_re = cast_aot_arg<TVariant<16,8,regex::ReNode *,void *> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(next_6ab20ac9bb2c573c(__context__, arg_re));
}
static vec4f __wrap_is_set_empty_1f052aa7f260c4eb ( Context * __context__ ) {
    TDim<uint32_t,8> const  &  arg_cset = cast_aot_arg<TDim<uint32_t,8> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_set_empty_1f052aa7f260c4eb(__context__, arg_cset));
}
static vec4f __wrap_set_or_char_6485de9cac36fb6f ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    set_or_char_6485de9cac36fb6f(__context__, arg_cset, arg_ch);
    return v_zero();
}
static vec4f __wrap_set_or_range_eead05ea9800cf6a ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    range arg_bits = cast_aot_arg<range>::to(*__context__,__context__->abiArguments()[1]);
    set_or_range_eead05ea9800cf6a(__context__, arg_cset, arg_bits);
    return v_zero();
}
static vec4f __wrap_set_invert_f44d20bc14c46b29 ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    set_invert_f44d20bc14c46b29(__context__, arg_cset);
    return v_zero();
}
static vec4f __wrap_set_or_set_c5a22cc26b33786b ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    TDim<uint32_t,8> const  &  arg_eset = cast_aot_arg<TDim<uint32_t,8> const  & >::to(*__context__,__context__->abiArguments()[1]);
    set_or_set_c5a22cc26b33786b(__context__, arg_cset, arg_eset);
    return v_zero();
}
static vec4f __wrap_set_negative_959b5894df7dd660 ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    set_negative_959b5894df7dd660(__context__, arg_cset);
    return v_zero();
}
static vec4f __wrap_set_meta_c297bca2e11798f6 ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_che = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    set_meta_c297bca2e11798f6(__context__, arg_cset, arg_che);
    return v_zero();
}
static vec4f __wrap_is_meta_character_6b868635173399ff ( Context * __context__ ) {
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_meta_character_6b868635173399ff(__context__, arg_ch));
}
static vec4f __wrap_is_set_character_3b302fd2b4140e7f ( Context * __context__ ) {
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_set_character_3b302fd2b4140e7f(__context__, arg_ch));
}
static vec4f __wrap_is_digit_35d4a4cc9eacb04d ( Context * __context__ ) {
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_digit_35d4a4cc9eacb04d(__context__, arg_ch));
}
static vec4f __wrap_is_word_character_8422846159268aaf ( Context * __context__ ) {
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_word_character_8422846159268aaf(__context__, arg_ch));
}
static vec4f __wrap_from_hex_86a2ef9963e757 ( Context * __context__ ) {
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(from_hex_86a2ef9963e757(__context__, arg_ch));
}
static vec4f __wrap_string_from_hex_5698e77bb25d7508 ( Context * __context__ ) {
    int32_t arg_ch1 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(string_from_hex_5698e77bb25d7508(__context__, arg_ch1));
}
static vec4f __wrap_string_from_hex_e27d9e88554466f8 ( Context * __context__ ) {
    int32_t arg_ch1 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_ch2 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(string_from_hex_e27d9e88554466f8(__context__, arg_ch1, arg_ch2));
}
static vec4f __wrap_re_char_5110fe336617b529 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_char_5110fe336617b529(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_set_items_ca22db507ac86515 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_set_items_ca22db507ac86515(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_set_3642c8eadc1c1fb5 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_set_3642c8eadc1c1fb5(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_any_d551e389d6ffd28 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_any_d551e389d6ffd28(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_eos_3dff18197056692d ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_eos_3dff18197056692d(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_bos_939db26d2fdba5e2 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_bos_939db26d2fdba5e2(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_group_17f1ffc0bc11a16 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_group_17f1ffc0bc11a16(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_elementary_409594d3d2b7cfa3 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_elementary_409594d3d2b7cfa3(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_mk_concat_53dbf44b1354a65 ( Context * __context__ ) {
    regex::ReNode * arg_left = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_right = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    return cast<regex::ReNode *>::from(mk_concat_53dbf44b1354a65(__context__, arg_left, arg_right));
}
static vec4f __wrap_mk_union_b8290c88c89cea8b ( Context * __context__ ) {
    regex::ReNode * arg_left = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_right = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    return cast<regex::ReNode *>::from(mk_union_b8290c88c89cea8b(__context__, arg_left, arg_right));
}
static vec4f __wrap_re_re_12ab55bce538b67e ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_re_12ab55bce538b67e(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_basic_3aca537b6901f1ec ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((TVariant<16,8,regex::ReNode *,void *> *) __context__->abiCMRES) = re_basic_3aca537b6901f1ec(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_parse_be404de54055d495 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<regex::ReNode *>::from(re_parse_be404de54055d495(__context__, arg_expr));
}
static vec4f __wrap_visit_top_down_28b61d33d98f34cc ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Block DAS_COMMENT((void,regex::ReNode *)) const  &  arg_blk = cast_aot_arg<Block DAS_COMMENT((void,regex::ReNode *)) const  & >::to(*__context__,__context__->abiArguments()[1]);
    visit_top_down_28b61d33d98f34cc(__context__, arg_node, arg_blk);
    return v_zero();
}
static vec4f __wrap_re_assign_next_5d4c1e040adf8167 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    re_assign_next_5d4c1e040adf8167(__context__, arg_re);
    return v_zero();
}
static vec4f __wrap_re_assign_groups_3ba808af5f6a0682 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    re_assign_groups_3ba808af5f6a0682(__context__, arg_re);
    return v_zero();
}
static vec4f __wrap_re_assign_match_functions_96c5fc406200914c ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    re_assign_match_functions_96c5fc406200914c(__context__, arg_re);
    return v_zero();
}
static vec4f __wrap_ci_lower_ee451210b4105701 ( Context * __context__ ) {
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(ci_lower_ee451210b4105701(__context__, arg_ch));
}
static vec4f __wrap_case_fold_set_3fe63201f3f0aed7 ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    case_fold_set_3fe63201f3f0aed7(__context__, arg_cset);
    return v_zero();
}
static vec4f __wrap_re_match2_single_char_7ee863dbfd5bdb67 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_single_char_7ee863dbfd5bdb67(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_char_f2046197ce02637d ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_char_f2046197ce02637d(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_single_char_ci_68081666546328b ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_single_char_ci_68081666546328b(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_char_ci_3c6abffae8c4e1d5 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_char_ci_3c6abffae8c4e1d5(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_union_bcaea863938bc283 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_union_bcaea863938bc283(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_set_5cb609a7e934b852 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_set_5cb609a7e934b852(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_any_e0f778af97b719a3 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_any_e0f778af97b719a3(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_concat_2e0ee6aaeac1bff7 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_concat_2e0ee6aaeac1bff7(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_eos_f51ea4d66660c6ee ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_eos_f51ea4d66660c6ee(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_question_55ffbd37b5f31b71 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_question_55ffbd37b5f31b71(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_plus_cbc885000639d48b ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_plus_cbc885000639d48b(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_plus_set_e285952ef61ae545 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_plus_set_e285952ef61ae545(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_star_3e354952f52dedc7 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_star_3e354952f52dedc7(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_group_cdd84cde5e0cc0e3 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_group_cdd84cde5e0cc0e3(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_bos_7093e76efb08a7f4 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_bos_7093e76efb08a7f4(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_word_boundary_de93f59a606699b5 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_word_boundary_de93f59a606699b5(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_non_word_boundary_d0c20eaf4c18dc8a ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_non_word_boundary_d0c20eaf4c18dc8a(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_lookahead_3ac542e65f4d1cff ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_lookahead_3ac542e65f4d1cff(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_negative_lookahead_ba2437211db882de ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_negative_lookahead_ba2437211db882de(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_repeat_3b5f93e4a9fcaadf ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_repeat_3b5f93e4a9fcaadf(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_question_lazy_63762e1b1147774a ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_question_lazy_63762e1b1147774a(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_star_lazy_2c4bd54606277ac1 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_star_lazy_2c4bd54606277ac1(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_plus_lazy_e54a263affed7fc8 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_plus_lazy_e54a263affed7fc8(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_repeat_lazy_1105b9a6a4ba9b41 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_repeat_lazy_1105b9a6a4ba9b41(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_early_out_65d4b897dbff0d04 ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * const  arg_node = cast_aot_arg<regex::ReNode * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(re_early_out_65d4b897dbff0d04(__context__, arg_cset, arg_node));
}
static vec4f __wrap_is_valid_3c125942baf6bf0a ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_valid_3c125942baf6bf0a(__context__, arg_re));
}
static vec4f __wrap_regex_compile_e1c30ff6f42517a2 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    bool arg_case_insensitive = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    bool arg_dot_all = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[3]);
    return cast<bool>::from(regex_compile_e1c30ff6f42517a2(__context__, arg_re, arg_expr, arg_case_insensitive, arg_dot_all));
}
static vec4f __wrap_regex_compile_6b8af38459f7df28 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    bool arg_case_insensitive = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[1]);
    bool arg_dot_all = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[2]);
    *((regex::Regex *) __context__->abiCMRES) = regex_compile_6b8af38459f7df28(__context__, arg_expr, arg_case_insensitive, arg_dot_all);
    return v_zero();
}
static vec4f __wrap_regex_compile_23e411aa689a04e5 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    *((regex::Regex *) __context__->abiCMRES) = regex_compile_23e411aa689a04e5(__context__, arg_re);
    return v_zero();
}
static vec4f __wrap_regex_match_85e0a2ceef06ab24 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(regex_match_85e0a2ceef06ab24(__context__, arg_regex, arg_str, arg_offset));
}
static vec4f __wrap_regex_group_dacc7e63de7d674e ( Context * __context__ ) {
    regex::Regex const  &  arg_regex = cast_aot_arg<regex::Regex const  & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_index = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_match = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<char *>::from(regex_group_dacc7e63de7d674e(__context__, arg_regex, arg_index, arg_match));
}
static vec4f __wrap_regex_foreach_2cf0e40b2436b6d9 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    Block DAS_COMMENT((bool,range)) const  &  arg_blk = cast_aot_arg<Block DAS_COMMENT((bool,range)) const  & >::to(*__context__,__context__->abiArguments()[2]);
    regex_foreach_2cf0e40b2436b6d9(__context__, arg_regex, arg_str, arg_blk);
    return v_zero();
}
static vec4f __wrap_regex_replace_e2c898a32c8b9a0a ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    Block DAS_COMMENT((char *,char * const )) const  &  arg_blk = cast_aot_arg<Block DAS_COMMENT((char *,char * const )) const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<char *>::from(regex_replace_e2c898a32c8b9a0a(__context__, arg_regex, arg_str, arg_blk));
}
static vec4f __wrap_expand_replacement_ad68a38d9900aed3 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_replacement = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    int32_t arg_match_start = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[3]);
    int32_t arg_match_end = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[4]);
    return cast<char *>::from(expand_replacement_ad68a38d9900aed3(__context__, arg_regex, arg_str, arg_replacement, arg_match_start, arg_match_end));
}
static vec4f __wrap_regex_replace_7cc2deb86b39b5c3 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_replacement = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<char *>::from(regex_replace_7cc2deb86b39b5c3(__context__, arg_regex, arg_str, arg_replacement));
}
static vec4f __wrap_regex_search_712ff63ef772d38d ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int2>::from(regex_search_712ff63ef772d38d(__context__, arg_regex, arg_str, arg_offset));
}
static vec4f __wrap_regex_split_ad2c6f596b0add93 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    *((TArray<char *> *) __context__->abiCMRES) = regex_split_ad2c6f596b0add93(__context__, arg_regex, arg_str);
    return v_zero();
}
static vec4f __wrap_regex_match_all_dc6b47bf82eb552 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    *((TArray<range> *) __context__->abiCMRES) = regex_match_all_dc6b47bf82eb552(__context__, arg_regex, arg_str);
    return v_zero();
}
static vec4f __wrap_regex_group_by_name_14993f975288220c ( Context * __context__ ) {
    regex::Regex const  &  arg_regex = cast_aot_arg<regex::Regex const  & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<char *>::from(regex_group_by_name_14993f975288220c(__context__, arg_regex, arg_name, arg_str));
}
static vec4f __wrap__FuncSqblSqbr_f97767f82664a020 ( Context * __context__ ) {
    regex::Regex const  &  arg_regex = cast_aot_arg<regex::Regex const  & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_index = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<range>::from(_FuncSqblSqbr_f97767f82664a020(__context__, arg_regex, arg_index));
}
static vec4f __wrap__FuncSqblSqbr_9dbe1a1c2a54dde3 ( Context * __context__ ) {
    regex::Regex const  &  arg_regex = cast_aot_arg<regex::Regex const  & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<range>::from(_FuncSqblSqbr_9dbe1a1c2a54dde3(__context__, arg_regex, arg_name));
}
static vec4f __wrap_regex_debug_55af4874d0243e85 ( Context * __context__ ) {
    regex::Regex const  &  arg_regex = cast_aot_arg<regex::Regex const  & >::to(*__context__,__context__->abiArguments()[0]);
    regex_debug_55af4874d0243e85(__context__, arg_regex);
    return v_zero();
}
static vec4f __wrap_debug_set_c7915a561c802ef5 ( Context * __context__ ) {
    TDim<uint32_t,8> const  &  arg_cset = cast_aot_arg<TDim<uint32_t,8> const  & >::to(*__context__,__context__->abiArguments()[0]);
    debug_set_c7915a561c802ef5(__context__, arg_cset);
    return v_zero();
}
static vec4f __wrap_debug_re_c9a788d56ecca6c1 ( Context * __context__ ) {
    regex::ReNode * const  arg_node = cast_aot_arg<regex::ReNode * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_tab = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    debug_re_c9a788d56ecca6c1(__context__, arg_node, arg_tab);
    return v_zero();
}
static vec4f __wrap_rnd_set_character_a6ace307aa9880d1 ( Context * __context__ ) {
    TDim<uint32_t,8> const  &  arg_cset = cast_aot_arg<TDim<uint32_t,8> const  & >::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(rnd_set_character_a6ace307aa9880d1(__context__, arg_cset, arg_rnd));
}
static vec4f __wrap_re_gen2_fail_b917d161e3783b38 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_fail_b917d161e3783b38(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_char_4112c00708ab51c6 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_char_4112c00708ab51c6(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_union_9b028b61c503f834 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_union_9b028b61c503f834(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_set_b1d4208a13b42b9 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_set_b1d4208a13b42b9(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_any_5e88209f8e29e4e5 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_any_5e88209f8e29e4e5(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_concat_30030241eb92a8af ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_concat_30030241eb92a8af(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_eos_2859c3cfba5b66cf ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_eos_2859c3cfba5b66cf(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_question_1b31bedc870aa21a ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_question_1b31bedc870aa21a(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_plus_2a39935c8341bc2a ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_plus_2a39935c8341bc2a(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_star_677f11c532be890b ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_star_677f11c532be890b(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_group_267d4348c759899b ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_group_267d4348c759899b(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_bos_2859c3cfba5b66cf ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_bos_2859c3cfba5b66cf(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_repeat_d2475d70f23bd85 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_repeat_d2475d70f23bd85(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_word_boundary_2859c3cfba5b66cf ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_word_boundary_2859c3cfba5b66cf(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_non_word_boundary_2859c3cfba5b66cf ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_non_word_boundary_2859c3cfba5b66cf(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_lookahead_267d4348c759899b ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_lookahead_267d4348c759899b(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_negative_lookahead_2859c3cfba5b66cf ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_negative_lookahead_2859c3cfba5b66cf(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen_bf361240a28b8867 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(re_gen_bf361240a28b8867(__context__, arg_re, arg_rnd));
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0xdbf0e18ae20caedc, false, (void*)&_FuncbuiltinTickresizeTick4811697762258667383_494d51b3fa621d18, &__wrap__FuncbuiltinTickresizeTick4811697762258667383_494d51b3fa621d18 },
    { 0x77be6be16e93219b, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_93105ead5b29fa0, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_93105ead5b29fa0 },
    { 0x2511b86aeb95b119, false, (void*)&_FuncbuiltinTicklengthTick18150397773952384912_fe94899ed7cf7fdb, &__wrap__FuncbuiltinTicklengthTick18150397773952384912_fe94899ed7cf7fdb },
    { 0xf3df5658f6ea5e05, false, (void*)&finalize_61e451af4e714ca2, &__wrap_finalize_61e451af4e714ca2 },
    { 0x3a41090f4b216241, false, (void*)&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_9c8fefcf8a2c4d0d, &__wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_9c8fefcf8a2c4d0d },
    { 0xb2aa7c99cd110c66, false, (void*)&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a5393f36ccdac4a8, &__wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_a5393f36ccdac4a8 },
    { 0xd1041780acb6ab3d, false, (void*)&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1b414e731a438fa8, &__wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_1b414e731a438fa8 },
    { 0x388a5d156a42341, false, (void*)&_FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc, &__wrap__FuncregexTickfrom_hexTick16446213900708532977_58323259cd2319bc },
    { 0x28024eb619e3bf57, false, (void*)&finalize_4c4d68a9a950ec41, &__wrap_finalize_4c4d68a9a950ec41 },
    { 0xa606af3b102b70b1, false, (void*)&finalize_c680337a54511d24, &__wrap_finalize_c680337a54511d24 },
    { 0xcab8c705e80dd723, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_906d8abb52edd211, &__wrap__FuncbuiltinTickpushTick10769833213962245646_906d8abb52edd211 },
    { 0xbc38f92a140e699, true, (void*)&_FuncbuiltinTickto_array_moveTick3185538323411982277_1d04d83c4b41cf2c, &__wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_1d04d83c4b41cf2c },
    { 0x2b343c424e6947f8, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_7df733d16209a68a, &__wrap__FuncbuiltinTickpushTick10769833213962245646_7df733d16209a68a },
    { 0xf3ce3e3a3e7e6914, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606, &__wrap__FuncbuiltinTickpushTick10769833213962245646_dede1ff0483be606 },
    { 0x4a9a547cf73d3b9c, false, (void*)&_FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49, &__wrap__FuncbuiltinTickpushTick14133213201864676143_a73bd8f330e36d49 },
    { 0xe286e119ef09e35a, false, (void*)&_FuncbuiltinTickpushTick14133213201864676143_32d7c798299302f2, &__wrap__FuncbuiltinTickpushTick14133213201864676143_32d7c798299302f2 },
    { 0x3c74af1ee1ef2acc, false, (void*)&_FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904, &__wrap__FuncbuiltinTicknextTick17450348357676149856_f6dec21d064b2904 },
    { 0x828562d5af4c8f1a, false, (void*)&re_gen_get_rep_limit_aa71779c47fee5b8, &__wrap_re_gen_get_rep_limit_aa71779c47fee5b8 },
    { 0x7ef31621d0849bdb, false, (void*)&trace_7794f549fda55dde, &__wrap_trace_7794f549fda55dde },
    { 0x7ef31621d0849bdb, false, (void*)&note_7794f549fda55dde, &__wrap_note_7794f549fda55dde },
    { 0x7bfa536a1eb16ea9, false, (void*)&trace_backtrack_60bdee73e3315732, &__wrap_trace_backtrack_60bdee73e3315732 },
    { 0xb45c748e30433958, false, (void*)&trace_value_f443ec2d74a0c7bd, &__wrap_trace_value_f443ec2d74a0c7bd },
    { 0x32ccd936acdd5cdf, true, (void*)&nada_ba27de56c6f5dc72, &__wrap_nada_ba27de56c6f5dc72 },
    { 0xd69d1ae1aa20031a, true, (void*)&maybe_249d36fe62a185a7, &__wrap_maybe_249d36fe62a185a7 },
    { 0xb140760cfdfb6625, false, (void*)&eos_b645f5ba09d5c5, &__wrap_eos_b645f5ba09d5c5 },
    { 0xd412d4b95b50aa6e, false, (void*)&at_ac9760d67b4716d0, &__wrap_at_ac9760d67b4716d0 },
    { 0xb1a56e868af48f7a, false, (void*)&next_6ab20ac9bb2c573c, &__wrap_next_6ab20ac9bb2c573c },
    { 0x79ecf294045c1832, false, (void*)&is_set_empty_1f052aa7f260c4eb, &__wrap_is_set_empty_1f052aa7f260c4eb },
    { 0xc5915df77fa8f08a, false, (void*)&set_or_char_6485de9cac36fb6f, &__wrap_set_or_char_6485de9cac36fb6f },
    { 0x6469f7224a1374dd, false, (void*)&set_or_range_eead05ea9800cf6a, &__wrap_set_or_range_eead05ea9800cf6a },
    { 0xb0b5b061847fa50e, false, (void*)&set_invert_f44d20bc14c46b29, &__wrap_set_invert_f44d20bc14c46b29 },
    { 0xab62fe0e0ddde0c4, false, (void*)&set_or_set_c5a22cc26b33786b, &__wrap_set_or_set_c5a22cc26b33786b },
    { 0xd761a2510691e255, false, (void*)&set_negative_959b5894df7dd660, &__wrap_set_negative_959b5894df7dd660 },
    { 0xb37d780aba78b6cf, false, (void*)&set_meta_c297bca2e11798f6, &__wrap_set_meta_c297bca2e11798f6 },
    { 0x68360fa971182b5, false, (void*)&is_meta_character_6b868635173399ff, &__wrap_is_meta_character_6b868635173399ff },
    { 0x2ad43f7acae2060, false, (void*)&is_set_character_3b302fd2b4140e7f, &__wrap_is_set_character_3b302fd2b4140e7f },
    { 0xe31e661289fe59aa, false, (void*)&is_digit_35d4a4cc9eacb04d, &__wrap_is_digit_35d4a4cc9eacb04d },
    { 0x1d746d4656c1990d, false, (void*)&is_word_character_8422846159268aaf, &__wrap_is_word_character_8422846159268aaf },
    { 0xd680b413948bc32d, false, (void*)&from_hex_86a2ef9963e757, &__wrap_from_hex_86a2ef9963e757 },
    { 0x7e378a26f228ead0, false, (void*)&string_from_hex_5698e77bb25d7508, &__wrap_string_from_hex_5698e77bb25d7508 },
    { 0xac4c07c199236a32, false, (void*)&string_from_hex_e27d9e88554466f8, &__wrap_string_from_hex_e27d9e88554466f8 },
    { 0x7b4406dab62d9e83, true, (void*)&re_char_5110fe336617b529, &__wrap_re_char_5110fe336617b529 },
    { 0x82ea178a4ca4d147, true, (void*)&re_set_items_ca22db507ac86515, &__wrap_re_set_items_ca22db507ac86515 },
    { 0x794b3587d02c9484, true, (void*)&re_set_3642c8eadc1c1fb5, &__wrap_re_set_3642c8eadc1c1fb5 },
    { 0xa77d64df660a6bba, true, (void*)&re_any_d551e389d6ffd28, &__wrap_re_any_d551e389d6ffd28 },
    { 0x5fd9696fdf5761ab, true, (void*)&re_eos_3dff18197056692d, &__wrap_re_eos_3dff18197056692d },
    { 0xc473e38c90684dfa, true, (void*)&re_bos_939db26d2fdba5e2, &__wrap_re_bos_939db26d2fdba5e2 },
    { 0x9d951ef060524202, true, (void*)&re_group_17f1ffc0bc11a16, &__wrap_re_group_17f1ffc0bc11a16 },
    { 0x19533c98917b4224, true, (void*)&re_elementary_409594d3d2b7cfa3, &__wrap_re_elementary_409594d3d2b7cfa3 },
    { 0xf389fae11386e5e8, false, (void*)&mk_concat_53dbf44b1354a65, &__wrap_mk_concat_53dbf44b1354a65 },
    { 0xbdadca90f92b8ba9, false, (void*)&mk_union_b8290c88c89cea8b, &__wrap_mk_union_b8290c88c89cea8b },
    { 0x8500736b9db8efe, true, (void*)&re_re_12ab55bce538b67e, &__wrap_re_re_12ab55bce538b67e },
    { 0xd3b00c32b3151ed, true, (void*)&re_basic_3aca537b6901f1ec, &__wrap_re_basic_3aca537b6901f1ec },
    { 0xce998929e9efa63d, false, (void*)&re_parse_be404de54055d495, &__wrap_re_parse_be404de54055d495 },
    { 0x2a8e9184c21d9c0, false, (void*)&visit_top_down_28b61d33d98f34cc, &__wrap_visit_top_down_28b61d33d98f34cc },
    { 0x550d35a9a63667e8, false, (void*)&re_assign_next_5d4c1e040adf8167, &__wrap_re_assign_next_5d4c1e040adf8167 },
    { 0xf7178825ced3c36d, false, (void*)&re_assign_groups_3ba808af5f6a0682, &__wrap_re_assign_groups_3ba808af5f6a0682 },
    { 0xb879c2d5d66134c4, false, (void*)&re_assign_match_functions_96c5fc406200914c, &__wrap_re_assign_match_functions_96c5fc406200914c },
    { 0xde1b23365519a4f9, false, (void*)&ci_lower_ee451210b4105701, &__wrap_ci_lower_ee451210b4105701 },
    { 0x9b3a0ec0d2033b08, false, (void*)&case_fold_set_3fe63201f3f0aed7, &__wrap_case_fold_set_3fe63201f3f0aed7 },
    { 0xcc781832940265f, false, (void*)&re_match2_single_char_7ee863dbfd5bdb67, &__wrap_re_match2_single_char_7ee863dbfd5bdb67 },
    { 0xc1addf2c4c8ecd65, false, (void*)&re_match2_char_f2046197ce02637d, &__wrap_re_match2_char_f2046197ce02637d },
    { 0x809321607d7f8a66, false, (void*)&re_match2_single_char_ci_68081666546328b, &__wrap_re_match2_single_char_ci_68081666546328b },
    { 0x55be8bd4e6f0782c, false, (void*)&re_match2_char_ci_3c6abffae8c4e1d5, &__wrap_re_match2_char_ci_3c6abffae8c4e1d5 },
    { 0xa2fbd7612afa9eaf, false, (void*)&re_match2_union_bcaea863938bc283, &__wrap_re_match2_union_bcaea863938bc283 },
    { 0x409d06b2eb80f726, false, (void*)&re_match2_set_5cb609a7e934b852, &__wrap_re_match2_set_5cb609a7e934b852 },
    { 0x5de8c43848acaf95, false, (void*)&re_match2_any_e0f778af97b719a3, &__wrap_re_match2_any_e0f778af97b719a3 },
    { 0x5ebd0dd7e004e796, false, (void*)&re_match2_concat_2e0ee6aaeac1bff7, &__wrap_re_match2_concat_2e0ee6aaeac1bff7 },
    { 0x77f1730866a4ff17, false, (void*)&re_match2_eos_f51ea4d66660c6ee, &__wrap_re_match2_eos_f51ea4d66660c6ee },
    { 0xa990d12424e9c12d, false, (void*)&re_match2_question_55ffbd37b5f31b71, &__wrap_re_match2_question_55ffbd37b5f31b71 },
    { 0x84851e2f307bb71c, false, (void*)&re_match2_plus_cbc885000639d48b, &__wrap_re_match2_plus_cbc885000639d48b },
    { 0x5d22ea6c21b04d96, false, (void*)&re_match2_plus_set_e285952ef61ae545, &__wrap_re_match2_plus_set_e285952ef61ae545 },
    { 0x32749d0e448ae4aa, false, (void*)&re_match2_star_3e354952f52dedc7, &__wrap_re_match2_star_3e354952f52dedc7 },
    { 0x41a9fb70aef4ed23, false, (void*)&re_match2_group_cdd84cde5e0cc0e3, &__wrap_re_match2_group_cdd84cde5e0cc0e3 },
    { 0x891ea29e311c4562, false, (void*)&re_match2_bos_7093e76efb08a7f4, &__wrap_re_match2_bos_7093e76efb08a7f4 },
    { 0x3528c157c0a92efa, false, (void*)&re_match2_word_boundary_de93f59a606699b5, &__wrap_re_match2_word_boundary_de93f59a606699b5 },
    { 0x9795dc3f982ec9a3, false, (void*)&re_match2_non_word_boundary_d0c20eaf4c18dc8a, &__wrap_re_match2_non_word_boundary_d0c20eaf4c18dc8a },
    { 0xf00a710f40d9c755, false, (void*)&re_match2_lookahead_3ac542e65f4d1cff, &__wrap_re_match2_lookahead_3ac542e65f4d1cff },
    { 0xcb493db471d910d, false, (void*)&re_match2_negative_lookahead_ba2437211db882de, &__wrap_re_match2_negative_lookahead_ba2437211db882de },
    { 0xb6131b17102cc7dd, false, (void*)&re_match2_repeat_3b5f93e4a9fcaadf, &__wrap_re_match2_repeat_3b5f93e4a9fcaadf },
    { 0xa947c57f5dc63dd9, false, (void*)&re_match2_question_lazy_63762e1b1147774a, &__wrap_re_match2_question_lazy_63762e1b1147774a },
    { 0xea9637d55d7fb17c, false, (void*)&re_match2_star_lazy_2c4bd54606277ac1, &__wrap_re_match2_star_lazy_2c4bd54606277ac1 },
    { 0xe06699408a6e6d66, false, (void*)&re_match2_plus_lazy_e54a263affed7fc8, &__wrap_re_match2_plus_lazy_e54a263affed7fc8 },
    { 0x7a05dd35358d5971, false, (void*)&re_match2_repeat_lazy_1105b9a6a4ba9b41, &__wrap_re_match2_repeat_lazy_1105b9a6a4ba9b41 },
    { 0x356bc22e67e9f21d, false, (void*)&re_early_out_65d4b897dbff0d04, &__wrap_re_early_out_65d4b897dbff0d04 },
    { 0x556e025326f09517, false, (void*)&is_valid_3c125942baf6bf0a, &__wrap_is_valid_3c125942baf6bf0a },
    { 0x9427c90aca1fa6cd, false, (void*)&regex_compile_e1c30ff6f42517a2, &__wrap_regex_compile_e1c30ff6f42517a2 },
    { 0x5642384e29a1b1c, true, (void*)&regex_compile_6b8af38459f7df28, &__wrap_regex_compile_6b8af38459f7df28 },
    { 0xd1734bb23adefbe6, true, (void*)&regex_compile_23e411aa689a04e5, &__wrap_regex_compile_23e411aa689a04e5 },
    { 0x735b5abc94de1a6e, false, (void*)&regex_match_85e0a2ceef06ab24, &__wrap_regex_match_85e0a2ceef06ab24 },
    { 0xdaf73f4e94b08742, false, (void*)&regex_group_dacc7e63de7d674e, &__wrap_regex_group_dacc7e63de7d674e },
    { 0x8bd0eac1882dc8bf, false, (void*)&regex_foreach_2cf0e40b2436b6d9, &__wrap_regex_foreach_2cf0e40b2436b6d9 },
    { 0x57d83aa5cb13ef5c, false, (void*)&regex_replace_e2c898a32c8b9a0a, &__wrap_regex_replace_e2c898a32c8b9a0a },
    { 0x698b89210ed60a96, false, (void*)&expand_replacement_ad68a38d9900aed3, &__wrap_expand_replacement_ad68a38d9900aed3 },
    { 0xeaaf9f33c740cf4b, false, (void*)&regex_replace_7cc2deb86b39b5c3, &__wrap_regex_replace_7cc2deb86b39b5c3 },
    { 0xd1a40d4897d79fa7, false, (void*)&regex_search_712ff63ef772d38d, &__wrap_regex_search_712ff63ef772d38d },
    { 0x96e77144a9e17916, true, (void*)&regex_split_ad2c6f596b0add93, &__wrap_regex_split_ad2c6f596b0add93 },
    { 0x7eb1a0e8ae221cf1, true, (void*)&regex_match_all_dc6b47bf82eb552, &__wrap_regex_match_all_dc6b47bf82eb552 },
    { 0x57e55b56b20765b0, false, (void*)&regex_group_by_name_14993f975288220c, &__wrap_regex_group_by_name_14993f975288220c },
    { 0xe3cb47f17cd05387, false, (void*)&_FuncSqblSqbr_f97767f82664a020, &__wrap__FuncSqblSqbr_f97767f82664a020 },
    { 0xed0859f4d64c9364, false, (void*)&_FuncSqblSqbr_9dbe1a1c2a54dde3, &__wrap__FuncSqblSqbr_9dbe1a1c2a54dde3 },
    { 0x6b20bac577e33ebd, false, (void*)&regex_debug_55af4874d0243e85, &__wrap_regex_debug_55af4874d0243e85 },
    { 0xe753d41a2177fa4f, false, (void*)&debug_set_c7915a561c802ef5, &__wrap_debug_set_c7915a561c802ef5 },
    { 0x3ea3ddd3d2171db1, false, (void*)&debug_re_c9a788d56ecca6c1, &__wrap_debug_re_c9a788d56ecca6c1 },
    { 0xe6eec213ace91d85, false, (void*)&rnd_set_character_a6ace307aa9880d1, &__wrap_rnd_set_character_a6ace307aa9880d1 },
    { 0x2f226c7bd0fb4e6f, false, (void*)&re_gen2_fail_b917d161e3783b38, &__wrap_re_gen2_fail_b917d161e3783b38 },
    { 0x5d3df5389dfff858, false, (void*)&re_gen2_char_4112c00708ab51c6, &__wrap_re_gen2_char_4112c00708ab51c6 },
    { 0x6d88bc3c1d17ad46, false, (void*)&re_gen2_union_9b028b61c503f834, &__wrap_re_gen2_union_9b028b61c503f834 },
    { 0x8abb2e1137bdfbb4, false, (void*)&re_gen2_set_b1d4208a13b42b9, &__wrap_re_gen2_set_b1d4208a13b42b9 },
    { 0x2805395474810ff7, false, (void*)&re_gen2_any_5e88209f8e29e4e5, &__wrap_re_gen2_any_5e88209f8e29e4e5 },
    { 0x68ca8721709eb06a, false, (void*)&re_gen2_concat_30030241eb92a8af, &__wrap_re_gen2_concat_30030241eb92a8af },
    { 0x51e5fd54b53ed14f, false, (void*)&re_gen2_eos_2859c3cfba5b66cf, &__wrap_re_gen2_eos_2859c3cfba5b66cf },
    { 0x5ab32de0a5ba2805, false, (void*)&re_gen2_question_1b31bedc870aa21a, &__wrap_re_gen2_question_1b31bedc870aa21a },
    { 0x40c792d956c527bd, false, (void*)&re_gen2_plus_2a39935c8341bc2a, &__wrap_re_gen2_plus_2a39935c8341bc2a },
    { 0x9ce67ed4d90d6afa, false, (void*)&re_gen2_star_677f11c532be890b, &__wrap_re_gen2_star_677f11c532be890b },
    { 0xd8a994e92c70ccf9, false, (void*)&re_gen2_group_267d4348c759899b, &__wrap_re_gen2_group_267d4348c759899b },
    { 0x51e5fd54b53ed14f, false, (void*)&re_gen2_bos_2859c3cfba5b66cf, &__wrap_re_gen2_bos_2859c3cfba5b66cf },
    { 0x92064533a87c095, false, (void*)&re_gen2_repeat_d2475d70f23bd85, &__wrap_re_gen2_repeat_d2475d70f23bd85 },
    { 0x51e5fd54b53ed14f, false, (void*)&re_gen2_word_boundary_2859c3cfba5b66cf, &__wrap_re_gen2_word_boundary_2859c3cfba5b66cf },
    { 0x51e5fd54b53ed14f, false, (void*)&re_gen2_non_word_boundary_2859c3cfba5b66cf, &__wrap_re_gen2_non_word_boundary_2859c3cfba5b66cf },
    { 0xd8a994e92c70ccf9, false, (void*)&re_gen2_lookahead_267d4348c759899b, &__wrap_re_gen2_lookahead_267d4348c759899b },
    { 0x51e5fd54b53ed14f, false, (void*)&re_gen2_negative_lookahead_2859c3cfba5b66cf, &__wrap_re_gen2_negative_lookahead_2859c3cfba5b66cf },
    { 0x37855e584c105069, false, (void*)&re_gen_bf361240a28b8867, &__wrap_re_gen_bf361240a28b8867 },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
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
