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
extern TypeInfo __type_info__9b868a595f8cb1d9;
extern TypeInfo __type_info__af63ef4c86020cd5;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63e84c860200f0;
extern TypeInfo __type_info__af63eb4c86020609;

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
TypeInfo __type_info__9b868a595f8cb1d9 = { Type::tStructure, &__struct_info__a442c67b1b45039e, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<regex::Regex>::size, UINT64_C(0x9b868a595f8cb1d9) };
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

TypeInfo * __tinfo_0[1] = { &__type_info__9b868a595f8cb1d9 };
TypeInfo * __tinfo_1[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_2[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_6[1] = { &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_7[2] = { &__type_info__af90fe4c864e9d52, &__type_info__9c60f9702c7c9014 };
TypeInfo * __tinfo_8[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_9[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_12[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_13[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_17[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_22[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd ( Context * __context__, TArray<regex::ReNode *> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759 ( Context * __context__, TArray<regex::ReNode *> & __a_rename_at_1308_2 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d ( Context * __context__, TDim<regex::ReNode *,2> const  & __a_rename_at_586_4 );
inline void finalize_1b3fa79e78928d5e ( Context * __context__, regex::ReNode & ____this_rename_at_29_5 );
inline regex::Regex & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3d380093f2b970c4 ( Context * __context__, regex::Regex & __a_rename_at_50_6 );
inline int32_t _FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec ( Context * __context__, int32_t __ch1_rename_at_240_7, int32_t __ch2_rename_at_240_8 );
inline void finalize_4d9dff2c4111fea5 ( Context * __context__, AutoVariant<regex::ReNode *,void *> & ____this_rename_at_422_9 );
inline void finalize_2e3cca8177873ef7 ( Context * __context__, regex::ReNode * & ____this_rename_at_519_10 );
inline void _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9 ( Context * __context__, TArray<regex::ReNode *> & __Arr_rename_at_181_11, regex::ReNode * __value_rename_at_181_12 );
inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6 ( Context * __context__, TDim<regex::ReNode *,2> & __a_rename_at_1468_13 );
inline void _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5 ( Context * __context__, TArray<AutoTuple<range,char *>> & __Arr_rename_at_181_15, AutoTuple<range,char *> & __value_rename_at_181_16 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) & __it_rename_at_1349_17, uint32_t & __value_rename_at_1349_18 );
inline uint32_t re_gen_get_rep_limit_aa71779c47fee5b8 ( Context * __context__ );
inline void trace_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_79_19, int32_t __offset_rename_at_79_20 );
inline void note_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_86_21, int32_t __offset_rename_at_86_22 );
inline void trace_backtrack_60bdee73e3315732 ( Context * __context__ );
inline void trace_value_599ec0b97b535e0e ( Context * __context__, regex::ReNode * const  __value_rename_at_99_23 );
inline AutoVariant<regex::ReNode *,void *> nada_a36e274f9f880579 ( Context * __context__ );
inline AutoVariant<regex::ReNode *,void *> maybe_cb698f19184a9bc8 ( Context * __context__, regex::ReNode * __value_rename_at_114_24 );
inline bool eos_bcd93149648ad887 ( Context * __context__, char * const  __expr_rename_at_119_25, int32_t __offset_rename_at_119_26 );
inline int32_t at_ac9760d67b4716d0 ( Context * __context__, char * const  __expr_rename_at_123_27, int32_t __offset_rename_at_123_28 );
inline int32_t next_c7b14aa26d681478 ( Context * __context__, AutoVariant<regex::ReNode *,void *> const  & __re_rename_at_142_29 );
inline bool is_set_empty_1f052aa7f260c4eb ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_150_30 );
inline void set_or_char_5d5313390b879abc ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_159_32, int32_t __ch_rename_at_159_33 );
inline void set_or_range_40be02560bdb916d ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_163_34, range __bits_rename_at_163_35 );
inline void set_invert_4fb9b64ebf15e148 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_169_37 );
inline void set_or_set_f7fe9d5d2a5b194b ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_175_39, TDim<uint32_t,8> const  & __eset_rename_at_175_40 );
inline void set_negative_e65d971bcd9eb5c1 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_181_45 );
inline void set_meta_5675e929d5b2f227 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_187_47, int32_t __che_rename_at_187_48 );
inline bool is_meta_character_5210d144f518e88e ( Context * __context__, int32_t __ch_rename_at_218_52 );
inline bool is_set_character_b3eb8d77360f374d ( Context * __context__, int32_t __ch_rename_at_222_53 );
inline int32_t from_hex_34b87ad3a69093f9 ( Context * __context__, int32_t __ch_rename_at_226_54 );
inline char * string_from_hex_3b7c632d00a40daf ( Context * __context__, int32_t __ch1_rename_at_244_55 );
inline char * string_from_hex_7d459c50a54015ae ( Context * __context__, int32_t __ch1_rename_at_248_56, int32_t __ch2_rename_at_248_57 );
inline AutoVariant<regex::ReNode *,void *> re_char_1cacb1cfe082d534 ( Context * __context__, char * const  __expr_rename_at_253_58, int32_t __offset_rename_at_253_59 );
inline AutoVariant<regex::ReNode *,void *> re_set_items_8d1ff416b742d6a0 ( Context * __context__, char * const  __expr_rename_at_302_66, int32_t __offset_rename_at_302_67 );
inline AutoVariant<regex::ReNode *,void *> re_set_a55f6e80806300ed ( Context * __context__, char * const  __expr_rename_at_404_79, int32_t __offset_rename_at_404_80 );
inline AutoVariant<regex::ReNode *,void *> re_any_9e5c841624efd77c ( Context * __context__, char * const  __expr_rename_at_438_84, int32_t __offset_rename_at_438_85 );
inline AutoVariant<regex::ReNode *,void *> re_eos_91af11e09a55593b ( Context * __context__, char * const  __expr_rename_at_450_86, int32_t __offset_rename_at_450_87 );
inline AutoVariant<regex::ReNode *,void *> re_group_a76956b21d051425 ( Context * __context__, char * const  __expr_rename_at_462_88, int32_t __offset_rename_at_462_89 );
inline AutoVariant<regex::ReNode *,void *> re_elementary_8e517f8be58bf1a6 ( Context * __context__, char * const  __expr_rename_at_482_91, int32_t __offset_rename_at_482_92 );
inline regex::ReNode * mk_concat_1ec73c2f3691d1d5 ( Context * __context__, regex::ReNode * __left_rename_at_510_98, regex::ReNode * __right_rename_at_510_99 );
inline regex::ReNode * mk_union_317bd0efb93d42ad ( Context * __context__, regex::ReNode * __left_rename_at_538_101, regex::ReNode * __right_rename_at_538_102 );
inline AutoVariant<regex::ReNode *,void *> re_re_e5f845d3bd4b2bf9 ( Context * __context__, char * const  __expr_rename_at_565_104, int32_t __offset_rename_at_565_105 );
inline AutoVariant<regex::ReNode *,void *> re_basic_f733ea9ca09e6bda ( Context * __context__, char * const  __expr_rename_at_600_110, int32_t __offset_rename_at_600_111 );
inline regex::ReNode * re_parse_a11fbde82111aa69 ( Context * __context__, char * const  __expr_rename_at_620_114 );
inline void visit_top_down_4fecc3dfe95f303c ( Context * __context__, regex::ReNode * __node_rename_at_633_117, Block DAS_COMMENT((void,regex::ReNode *)) const  & __blk_rename_at_633_118 );
inline void re_assign_next_18621bfa0d3dbb85 ( Context * __context__, regex::Regex & __re_rename_at_649_120 );
inline void re_assign_groups_ee95de912bf625ba ( Context * __context__, regex::Regex & __re_rename_at_666_124 );
inline void re_assign_match_functions_d1123942ad08a870 ( Context * __context__, regex::Regex & __re_rename_at_681_127 );
inline uint8_t const  * re_match2_single_char_efd3120a5ed2dc78 ( Context * __context__, regex::Regex & __regex_rename_at_730_129, regex::ReNode * __node_rename_at_730_130, uint8_t const  * const  __str_rename_at_730_131 );
inline uint8_t const  * re_match2_char_563cc8699254595c ( Context * __context__, regex::Regex & __regex_rename_at_753_134, regex::ReNode * __node_rename_at_753_135, uint8_t const  * const  __str_rename_at_753_136 );
inline uint8_t const  * re_match2_union_483bf85dbc5196dd ( Context * __context__, regex::Regex & __regex_rename_at_779_139, regex::ReNode * __node_rename_at_779_140, uint8_t const  * const  __str_rename_at_779_141 );
inline uint8_t const  * re_match2_set_31b09b463a9d7b2b ( Context * __context__, regex::Regex & __regex_rename_at_797_144, regex::ReNode * __node_rename_at_797_145, uint8_t const  * const  __str_rename_at_797_146 );
inline uint8_t const  * re_match2_any_9b00b66687d95b62 ( Context * __context__, regex::Regex & __regex_rename_at_820_149, regex::ReNode * __node_rename_at_820_150, uint8_t const  * const  __str_rename_at_820_151 );
inline uint8_t const  * re_match2_concat_2bd31fc7755cdf2a ( Context * __context__, regex::Regex & __regex_rename_at_840_154, regex::ReNode * __node_rename_at_840_155, uint8_t const  * const  __str_rename_at_840_156 );
inline uint8_t const  * re_match2_eos_a35408df9434ba3c ( Context * __context__, regex::Regex & __regex_rename_at_857_159, regex::ReNode * __node_rename_at_857_160, uint8_t const  * const  __str_rename_at_857_161 );
inline uint8_t const  * re_match2_question_4acdfdc420ce399a ( Context * __context__, regex::Regex & __regex_rename_at_874_163, regex::ReNode * __node_rename_at_874_164, uint8_t const  * const  __str_rename_at_874_165 );
inline uint8_t const  * re_match2_plus_adb26cc707eb0c55 ( Context * __context__, regex::Regex & __regex_rename_at_893_169, regex::ReNode * __node_rename_at_893_170, uint8_t const  * const  __str_rename_at_893_171 );
inline uint8_t const  * re_match2_plus_set_aeb296540242ef22 ( Context * __context__, regex::Regex & __regex_rename_at_928_177, regex::ReNode * __node_rename_at_928_178, uint8_t const  * const  __str_rename_at_928_179 );
inline uint8_t const  * re_match2_star_bbfb62194a4dc1ae ( Context * __context__, regex::Regex & __regex_rename_at_971_184, regex::ReNode * __node_rename_at_971_185, uint8_t const  * const  __str_rename_at_971_186 );
inline uint8_t const  * re_match2_group_82784eced054ba4e ( Context * __context__, regex::Regex & __regex_rename_at_1000_192, regex::ReNode * __node_rename_at_1000_193, uint8_t const  * const  __str_rename_at_1000_194 );
inline bool re_early_out_6710f0abebdc4106 ( Context * __context__, TDim<uint32_t,8> & __cset_rename_at_1020_197, regex::ReNode * const  __node_rename_at_1020_198 );
inline bool is_valid_87c94e9920e8a112 ( Context * __context__, regex::Regex & __re_rename_at_1063_203 );
inline bool regex_compile_b802a48fba17c551 ( Context * __context__, regex::Regex & __re_rename_at_1068_204, char * const  __expr_rename_at_1068_205 );
inline regex::Regex regex_compile_9c6bbe3f7bf967b8 ( Context * __context__, char * const  __expr_rename_at_1082_206 );
inline regex::Regex regex_compile_8b8cd96a70efcb1c ( Context * __context__, regex::Regex & __re_rename_at_1090_208 );
inline int32_t regex_match_d1084a47d6e2d53e ( Context * __context__, regex::Regex & __regex_rename_at_1098_209, char * const  __str_rename_at_1098_210, int32_t __offset_rename_at_1098_211 );
inline char * regex_group_f150ed72d9c6b909 ( Context * __context__, regex::Regex const  & __regex_rename_at_1117_213, int32_t __index_rename_at_1117_214, char * const  __match_rename_at_1117_215 );
inline void regex_foreach_95b590dd0dd4944f ( Context * __context__, regex::Regex & __regex_rename_at_1124_217, char * const  __str_rename_at_1124_218, Block DAS_COMMENT((bool,range)) const  & __blk_rename_at_1124_219 );
inline char * regex_replace_6c020ee2b76e6419 ( Context * __context__, regex::Regex & __regex_rename_at_1172_226, char * const  __str_rename_at_1172_227, Block DAS_COMMENT((char *,char * const )) const  & __blk_rename_at_1172_228 );
inline void regex_debug_35a427292763c452 ( Context * __context__, regex::Regex const  & __regex_rename_at_1226_238 );
inline void debug_set_13e232e40e242fa0 ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_1243_240 );
inline void debug_re_f8fd4cc00f0dc78d ( Context * __context__, regex::ReNode * const  __node_rename_at_1256_242, int32_t __tab_rename_at_1256_243 );
inline int32_t rnd_set_character_6517e452a18c5606 ( Context * __context__, TDim<uint32_t,8> const  & __cset_rename_at_1317_245, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1317_246 );
inline void re_gen2_fail_a5fdf20930267896 ( Context * __context__, regex::ReNode * __node_rename_at_1325_249, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1325_250, StringBuilderWriter & __str_rename_at_1325_251 );
inline void re_gen2_char_e6b9f720990cc2d7 ( Context * __context__, regex::ReNode * __node_rename_at_1332_252, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1332_253, StringBuilderWriter & __str_rename_at_1332_254 );
inline void re_gen2_union_44153b0e184e2888 ( Context * __context__, regex::ReNode * __node_rename_at_1343_256, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1343_257, StringBuilderWriter & __str_rename_at_1343_258 );
inline void re_gen2_set_5483397ec36cfff0 ( Context * __context__, regex::ReNode * __node_rename_at_1354_261, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1354_262, StringBuilderWriter & __str_rename_at_1354_263 );
inline void re_gen2_any_200931e8e5db950 ( Context * __context__, regex::ReNode * __node_rename_at_1365_265, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1365_266, StringBuilderWriter & __str_rename_at_1365_267 );
inline void re_gen2_concat_bb7cd4a1d8751a6c ( Context * __context__, regex::ReNode * __node_rename_at_1378_270, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1378_271, StringBuilderWriter & __str_rename_at_1378_272 );
inline void re_gen2_eos_6a9fe263836b5d74 ( Context * __context__, regex::ReNode * __node_rename_at_1386_274, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1386_275, StringBuilderWriter & __str_rename_at_1386_276 );
inline void re_gen2_question_ac79834a17fb3749 ( Context * __context__, regex::ReNode * __node_rename_at_1396_278, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1396_279, StringBuilderWriter & __str_rename_at_1396_280 );
inline void re_gen2_plus_999e35bd61d42dc9 ( Context * __context__, regex::ReNode * __node_rename_at_1414_284, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1414_285, StringBuilderWriter & __str_rename_at_1414_286 );
inline void re_gen2_star_e57464e7f97360fc ( Context * __context__, regex::ReNode * __node_rename_at_1433_291, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1433_292, StringBuilderWriter & __str_rename_at_1433_293 );
inline void re_gen2_group_cc75131bc454c003 ( Context * __context__, regex::ReNode * __node_rename_at_1452_298, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1452_299, StringBuilderWriter & __str_rename_at_1452_300 );
inline char * re_gen_e78126cc4b992ab9 ( Context * __context__, regex::Regex & __re_rename_at_1466_303, Sequence DAS_COMMENT((uint32_t)) & __rnd_rename_at_1466_304 );

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

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759 ( Context * __context__, TArray<regex::ReNode *> &  __a_rename_at_1308_2 )
{
    {
        bool __need_loop_1310 = true;
        // aV: regex::ReNode? aka TT&
        das_iterator<TArray<regex::ReNode *>> __aV_iterator(__a_rename_at_1308_2);
        regex::ReNode * * __aV_rename_at_1310_3;
        __need_loop_1310 = __aV_iterator.first(__context__,(__aV_rename_at_1310_3)) && __need_loop_1310;
        for ( ; __need_loop_1310 ; __need_loop_1310 = __aV_iterator.next(__context__,(__aV_rename_at_1310_3)) )
        {
            finalize_2e3cca8177873ef7(__context__,(*__aV_rename_at_1310_3));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1310_3));
    };
    builtin_array_free(das_arg<TArray<regex::ReNode *>>::pass(__a_rename_at_1308_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d ( Context * __context__, TDim<regex::ReNode *,2> const  &  __a_rename_at_586_4 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline void finalize_1b3fa79e78928d5e ( Context * __context__, regex::ReNode &  ____this_rename_at_29_5 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759(__context__,das_arg<TArray<regex::ReNode *>>::pass(____this_rename_at_29_5.all));
    finalize_2e3cca8177873ef7(__context__,____this_rename_at_29_5.left);
    finalize_2e3cca8177873ef7(__context__,____this_rename_at_29_5.right);
    finalize_2e3cca8177873ef7(__context__,____this_rename_at_29_5.subexpr);
    memset((void*)&(____this_rename_at_29_5), 0, TypeSize<regex::ReNode>::size);
}

inline regex::Regex & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3d380093f2b970c4 ( Context * __context__, regex::Regex &  __a_rename_at_50_6 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<regex::Regex &>::from(__a_rename_at_50_6)));
    return das_auto_cast_ref<regex::Regex &>::cast(__a_rename_at_50_6);
}

inline int32_t _FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec ( Context * __context__, int32_t __ch1_rename_at_240_7, int32_t __ch2_rename_at_240_8 )
{
    return das_auto_cast<int32_t>::cast((from_hex_34b87ad3a69093f9(__context__,__ch1_rename_at_240_7) * 16) + from_hex_34b87ad3a69093f9(__context__,__ch2_rename_at_240_8));
}

inline void finalize_4d9dff2c4111fea5 ( Context * __context__, AutoVariant<regex::ReNode *,void *> &  ____this_rename_at_422_9 )
{
    if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(____this_rename_at_422_9) )
    {
        finalize_2e3cca8177873ef7(__context__,das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::get(____this_rename_at_422_9));
    };
    memset((void*)&(____this_rename_at_422_9), 0, TypeSize<AutoVariant<regex::ReNode *,void *>>::size);
}

inline void finalize_2e3cca8177873ef7 ( Context * __context__, regex::ReNode * & ____this_rename_at_519_10 )
{
    if ( ____this_rename_at_519_10 != nullptr )
    {
        finalize_1b3fa79e78928d5e(__context__,das_arg<regex::ReNode>::pass(das_deref(__context__,____this_rename_at_519_10)));
        das_delete<regex::ReNode *>::clear(__context__,____this_rename_at_519_10);
        das_copy(____this_rename_at_519_10,nullptr);
    };
}

inline void _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9 ( Context * __context__, TArray<regex::ReNode *> &  __Arr_rename_at_181_11, regex::ReNode * __value_rename_at_181_12 )
{
    das_copy(__Arr_rename_at_181_11(builtin_array_push_back(das_arg<TArray<regex::ReNode *>>::pass(__Arr_rename_at_181_11),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_12);
}

inline TArray<regex::ReNode *> _FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6 ( Context * __context__, TDim<regex::ReNode *,2> &  __a_rename_at_1468_13 )
{
    TArray<regex::ReNode *> __arr_rename_at_1470_14;das_zero(__arr_rename_at_1470_14);
    _FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd(__context__,das_arg<TArray<regex::ReNode *>>::pass(__arr_rename_at_1470_14),2);
    das_copy(das_cast<TDim<regex::ReNode *,2>>::cast(das_ref(__context__,__arr_rename_at_1470_14(0,__context__))),__a_rename_at_1468_13);
    return /* <- */ das_auto_cast_move<TArray<regex::ReNode *>>::cast(__arr_rename_at_1470_14);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5 ( Context * __context__, TArray<AutoTuple<range,char *>> &  __Arr_rename_at_181_15, AutoTuple<range,char *> &  __value_rename_at_181_16 )
{
    das_copy(__Arr_rename_at_181_15(builtin_array_push_back(das_arg<TArray<AutoTuple<range,char *>>>::pass(__Arr_rename_at_181_15),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_16);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d ( Context * __context__, Sequence DAS_COMMENT((uint32_t)) &  __it_rename_at_1349_17, uint32_t & __value_rename_at_1349_18 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__it_rename_at_1349_17),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1349_18)),__context__));
}

inline uint32_t re_gen_get_rep_limit_aa71779c47fee5b8 ( Context * __context__ )
{
    return das_auto_cast<uint32_t>::cast(das_global<uint32_t,0x8ce995ce57e18608>(__context__) /*re_gen_rep_limit*/);
}

inline void trace_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_79_19, int32_t __offset_rename_at_79_20 )
{
}

inline void note_7794f549fda55dde ( Context * __context__, char * const  __msg_rename_at_86_21, int32_t __offset_rename_at_86_22 )
{
}

inline void trace_backtrack_60bdee73e3315732 ( Context * __context__ )
{
}

inline void trace_value_599ec0b97b535e0e ( Context * __context__, regex::ReNode * const  __value_rename_at_99_23 )
{
}

inline AutoVariant<regex::ReNode *,void *> nada_a36e274f9f880579 ( Context * __context__ )
{
    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((([&]() -> AutoVariant<regex::ReNode *,void *> {
        AutoVariant<regex::ReNode *,void *> __mkv_111;
        das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::set(__mkv_111) = nullptr;
        return __mkv_111;
    })()));
}

inline AutoVariant<regex::ReNode *,void *> maybe_cb698f19184a9bc8 ( Context * __context__, regex::ReNode * __value_rename_at_114_24 )
{
    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((([&]() -> AutoVariant<regex::ReNode *,void *> {
        AutoVariant<regex::ReNode *,void *> __mkv_116;
        das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::set(__mkv_116) = __value_rename_at_114_24;
        return __mkv_116;
    })()));
}

inline bool eos_bcd93149648ad887 ( Context * __context__, char * const  __expr_rename_at_119_25, int32_t __offset_rename_at_119_26 )
{
    return das_auto_cast<bool>::cast(__offset_rename_at_119_26 >= builtin_string_length(__expr_rename_at_119_25,__context__));
}

inline int32_t at_ac9760d67b4716d0 ( Context * __context__, char * const  __expr_rename_at_123_27, int32_t __offset_rename_at_123_28 )
{
    return das_auto_cast<int32_t>::cast(get_character_uat(__expr_rename_at_123_27,__offset_rename_at_123_28));
}

inline int32_t next_c7b14aa26d681478 ( Context * __context__, AutoVariant<regex::ReNode *,void *> const  &  __re_rename_at_142_29 )
{
    if ( !das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__re_rename_at_142_29) )
    {
        builtin_stackwalk(true,true,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_throw(((char *) "expecting value"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast<int32_t>::cast(v_extract_yi(v_cast_vec4i(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__re_rename_at_142_29,__context__)->at)) /*y*/);
}

inline bool is_set_empty_1f052aa7f260c4eb ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_150_30 )
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

inline void set_or_char_5d5313390b879abc ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_159_32, int32_t __ch_rename_at_159_33 )
{
    __cset_rename_at_159_32((SimPolicy<int32_t>::BinShr(__ch_rename_at_159_33,5,*__context__,nullptr)),__context__) |= uint32_t(SimPolicy<int32_t>::BinShl(1,(__ch_rename_at_159_33 & 31),*__context__,nullptr));
}

inline void set_or_range_40be02560bdb916d ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_163_34, range __bits_rename_at_163_35 )
{
    {
        bool __need_loop_164 = true;
        // ch: int const
        das_iterator<range> __ch_iterator(range(v_extract_xi(v_cast_vec4i(__bits_rename_at_163_35)) /*x*/,v_extract_yi(v_cast_vec4i(__bits_rename_at_163_35)) /*y*/ + 1));
        int32_t __ch_rename_at_164_36;
        __need_loop_164 = __ch_iterator.first(__context__,(__ch_rename_at_164_36)) && __need_loop_164;
        for ( ; __need_loop_164 ; __need_loop_164 = __ch_iterator.next(__context__,(__ch_rename_at_164_36)) )
        {
            __cset_rename_at_163_34((SimPolicy<int32_t>::BinShr(__ch_rename_at_164_36,5,*__context__,nullptr)),__context__) |= uint32_t(SimPolicy<int32_t>::BinShl(1,(__ch_rename_at_164_36 & 31),*__context__,nullptr));
        }
        __ch_iterator.close(__context__,(__ch_rename_at_164_36));
    };
}

inline void set_invert_4fb9b64ebf15e148 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_169_37 )
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

inline void set_or_set_f7fe9d5d2a5b194b ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_175_39, TDim<uint32_t,8> const  &  __eset_rename_at_175_40 )
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

inline void set_negative_e65d971bcd9eb5c1 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_181_45 )
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

inline void set_meta_5675e929d5b2f227 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_187_47, int32_t __che_rename_at_187_48 )
{
    if ( __che_rename_at_187_48 == 119 )
    {
        set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),range(97,122));
        set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),range(65,90));
        set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),range(48,57));
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),95);
    } else if ( __che_rename_at_187_48 == 87 )
    {
        TDim<uint32_t,8> __eset_rename_at_194_49;das_zero(__eset_rename_at_194_49);
        set_meta_5675e929d5b2f227(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_194_49),119);
        set_or_set_f7fe9d5d2a5b194b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_194_49));
    } else if ( __che_rename_at_187_48 == 115 )
    {
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),32);
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),9);
    } else if ( __che_rename_at_187_48 == 83 )
    {
        TDim<uint32_t,8> __eset_rename_at_201_50;das_zero(__eset_rename_at_201_50);
        set_meta_5675e929d5b2f227(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_201_50),115);
        set_or_set_f7fe9d5d2a5b194b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_201_50));
    } else if ( __che_rename_at_187_48 == 100 )
    {
        set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),range(48,57));
    } else if ( __che_rename_at_187_48 == 68 )
    {
        TDim<uint32_t,8> __eset_rename_at_207_51;das_zero(__eset_rename_at_207_51);
        set_meta_5675e929d5b2f227(__context__,das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_207_51),100);
        set_or_set_f7fe9d5d2a5b194b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),das_arg<TDim<uint32_t,8>>::pass(__eset_rename_at_207_51));
    } else {
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_187_47),__che_rename_at_187_48);
    };
}

inline bool is_meta_character_5210d144f518e88e ( Context * __context__, int32_t __ch_rename_at_218_52 )
{
    return das_auto_cast<bool>::cast(builtin_find_first_char_of(((char *) "\\+-*.()[]|^"),__ch_rename_at_218_52,__context__) != -1);
}

inline bool is_set_character_b3eb8d77360f374d ( Context * __context__, int32_t __ch_rename_at_222_53 )
{
    return das_auto_cast<bool>::cast(builtin_find_first_char_of(((char *) "wWsSdD"),__ch_rename_at_222_53,__context__) != -1);
}

inline int32_t from_hex_34b87ad3a69093f9 ( Context * __context__, int32_t __ch_rename_at_226_54 )
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

inline char * string_from_hex_3b7c632d00a40daf ( Context * __context__, int32_t __ch1_rename_at_244_55 )
{
    return das_auto_cast<char *>::cast(((char * const )(to_string_char(from_hex_34b87ad3a69093f9(__context__,__ch1_rename_at_244_55),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * string_from_hex_7d459c50a54015ae ( Context * __context__, int32_t __ch1_rename_at_248_56, int32_t __ch2_rename_at_248_57 )
{
    return das_auto_cast<char *>::cast(((char * const )(to_string_char(_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec(__context__,__ch1_rename_at_248_56,__ch2_rename_at_248_57),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline AutoVariant<regex::ReNode *,void *> re_char_1cacb1cfe082d534 ( Context * __context__, char * const  __expr_rename_at_253_58, int32_t __offset_rename_at_253_59 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        int32_t __ch_rename_at_258_60 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59));
        if ( __ch_rename_at_258_60 == 92 )
        {
            int32_t __ech_rename_at_260_61 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59 + 1));
            if ( __ech_rename_at_260_61 == 0 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
            } else {
                if ( __ech_rename_at_260_61 == 120 )
                {
                    int32_t __len_rename_at_266_62 = ((int32_t)builtin_string_length(__expr_rename_at_253_58,__context__));
                    if ( (__offset_rename_at_253_59 + 2) >= __len_rename_at_266_62 )
                    {
                        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                    } else {
                        int32_t __hex1_rename_at_271_63 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59 + 2));
                        if ( !is_hex(__hex1_rename_at_271_63) )
                        {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                        } else {
                            if ( (__offset_rename_at_253_59 + 2) < __len_rename_at_266_62 )
                            {
                                int32_t __hex2_rename_at_277_64 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_253_58,__offset_rename_at_253_59 + 3));
                                if ( is_hex(__hex2_rename_at_277_64) )
                                {
                                    note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_1, cast<char * const >::from(((char *) "hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_271_63,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char * const )(to_string_char(__hex2_rename_at_277_64,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec(__context__,__hex1_rename_at_271_63,__hex2_rename_at_277_64))))),__offset_rename_at_253_59);
                                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_280;
                                        das_zero(__mks_280);
                                        das_copy((__mks_280.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                        das_copy((__mks_280.text),(string_from_hex_7d459c50a54015ae(__context__,__hex1_rename_at_271_63,__hex2_rename_at_277_64)));
                                        das_copy((__mks_280.textLen),(1));
                                        das_copy((__mks_280.at),(range(__offset_rename_at_253_59,__offset_rename_at_253_59 + 3)));
                                        return __mks_280;
                                    })()))));
                                };
                            };
                            note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_2, cast<char * const >::from(((char *) "hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_271_63,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_271_63))))),__offset_rename_at_253_59);
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_284;
                                das_zero(__mks_284);
                                das_copy((__mks_284.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                das_copy((__mks_284.text),(string_from_hex_3b7c632d00a40daf(__context__,__hex1_rename_at_271_63)));
                                das_copy((__mks_284.textLen),(1));
                                das_copy((__mks_284.at),(range(__offset_rename_at_253_59,__offset_rename_at_253_59 + 2)));
                                return __mks_284;
                            })()))));
                        };
                    };
                };
                if ( is_set_character_b3eb8d77360f374d(__context__,__ech_rename_at_260_61) )
                {
                    TDim<uint32_t,8> __cset_rename_at_287_65;das_zero(__cset_rename_at_287_65);
                    set_meta_5675e929d5b2f227(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_287_65),__ech_rename_at_260_61);
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_289;
                        das_zero(__mks_289);
                        das_copy((__mks_289.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_289.cset),(__cset_rename_at_287_65));
                        das_copy((__mks_289.at),(range(__offset_rename_at_253_59,__offset_rename_at_253_59 + 2)));
                        return __mks_289;
                    })()))));
                } else {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
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
        } else if ( is_meta_character_5210d144f518e88e(__context__,__ch_rename_at_258_60) )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        };
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
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

inline AutoVariant<regex::ReNode *,void *> re_set_items_8d1ff416b742d6a0 ( Context * __context__, char * const  __expr_rename_at_302_66, int32_t __offset_rename_at_302_67 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_302_66,__offset_rename_at_302_67) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        TDim<uint32_t,8> __cset_rename_at_307_68;das_zero(__cset_rename_at_307_68);
        int32_t __iofs_rename_at_308_69 = __offset_rename_at_302_67;
        int32_t __prev_char_rename_at_309_70 = -1;
        bool __next_range_rename_at_310_71 = false;
        bool __next_meta_rename_at_311_72 = false;
        while ( !eos_bcd93149648ad887(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69) )
        {
            int32_t __ch_rename_at_313_73 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69));
            int32_t __next_char_rename_at_314_74 = -1;
            if ( __ch_rename_at_313_73 == 93 )
            {
                break;
            } else if ( __ch_rename_at_313_73 == 92 )
            {
                int32_t __che_rename_at_319_75 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69 + 1));
                if ( __che_rename_at_319_75 == 0 )
                {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                } else {
                    if ( __che_rename_at_319_75 == 120 )
                    {
                        int32_t __len_rename_at_325_76 = ((int32_t)builtin_string_length(__expr_rename_at_302_66,__context__));
                        if ( (__iofs_rename_at_308_69 + 2) >= __len_rename_at_325_76 )
                        {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                        } else {
                            int32_t __hex1_rename_at_330_77 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69 + 2));
                            if ( !is_hex(__hex1_rename_at_330_77) )
                            {
                                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                            } else {
                                if ( (__iofs_rename_at_308_69 + 2) < __len_rename_at_325_76 )
                                {
                                    int32_t __hex2_rename_at_336_78 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_302_66,__iofs_rename_at_308_69 + 3));
                                    if ( is_hex(__hex2_rename_at_336_78) )
                                    {
                                        das_copy(__next_char_rename_at_314_74,_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec(__context__,__hex1_rename_at_330_77,__hex2_rename_at_336_78));
                                        note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_1, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_330_77,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char * const )(to_string_char(__hex2_rename_at_336_78,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec(__context__,__hex1_rename_at_330_77,__hex2_rename_at_336_78))))),__iofs_rename_at_308_69);
                                        __iofs_rename_at_308_69 += 4;
                                    } else {
                                        das_copy(__next_char_rename_at_314_74,from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_330_77));
                                        note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_2, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_330_77,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_330_77))))),__iofs_rename_at_308_69);
                                        __iofs_rename_at_308_69 += 3;
                                    };
                                } else {
                                    das_copy(__next_char_rename_at_314_74,from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_330_77));
                                    note_7794f549fda55dde(__context__,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_2, cast<char * const >::from(((char *) "set hex \\")), cast<char * const >::from(((char * const )(to_string_char(__hex1_rename_at_330_77,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) " ")), cast<uint32_t>::from(uint32_t(from_hex_34b87ad3a69093f9(__context__,__hex1_rename_at_330_77))))),__offset_rename_at_302_67);
                                    __iofs_rename_at_308_69 += 3;
                                };
                            };
                        };
                    } else {
                        if ( __next_range_rename_at_310_71 )
                        {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                        } else {
                            if ( is_set_character_b3eb8d77360f374d(__context__,__che_rename_at_319_75) )
                            {
                                das_copy(__next_char_rename_at_314_74,-1);
                                das_copy(__next_meta_rename_at_311_72,true);
                                set_meta_5675e929d5b2f227(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_307_68),__che_rename_at_319_75);
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
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
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
                    set_or_range_40be02560bdb916d(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_307_68),range(__prev_char_rename_at_309_70,__next_char_rename_at_314_74));
                    das_copy(__next_range_rename_at_310_71,false);
                    das_copy(__prev_char_rename_at_309_70,-1);
                } else {
                    set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_307_68),__next_char_rename_at_314_74);
                    das_copy(__prev_char_rename_at_309_70,__next_char_rename_at_314_74);
                };
            } else if ( __next_meta_rename_at_311_72 )
            {
                das_copy(__next_meta_rename_at_311_72,false);
            } else if ( !__next_range_rename_at_310_71 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
            };
        };
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_398;
            das_zero(__mks_398);
            das_copy((__mks_398.op),(DAS_COMMENT(enum) regex::ReOp::Set));
            das_copy((__mks_398.cset),(__cset_rename_at_307_68));
            das_copy((__mks_398.at),(range(__offset_rename_at_302_67,__iofs_rename_at_308_69)));
            return __mks_398;
        })()))));
    };
}

inline AutoVariant<regex::ReNode *,void *> re_set_a55f6e80806300ed ( Context * __context__, char * const  __expr_rename_at_404_79, int32_t __offset_rename_at_404_80 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_404_79,__offset_rename_at_404_80) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_404_79,__offset_rename_at_404_80) != 91 )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        } else {
            bool __negative_rename_at_412_81 = false;
            if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_404_79,__offset_rename_at_404_80 + 1) == 94 )
            {
                das_copy(__negative_rename_at_412_81,true);
            };
            AutoVariant<regex::ReNode *,void *> __oset_rename_at_416_82 = re_set_items_8d1ff416b742d6a0(__context__,__expr_rename_at_404_79,__negative_rename_at_412_81 ? das_auto_cast<int32_t>::cast((__offset_rename_at_404_80 + 2)) : das_auto_cast<int32_t>::cast((__offset_rename_at_404_80 + 1)));
            if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__oset_rename_at_416_82) )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
            } else {
                if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_404_79,next_c7b14aa26d681478(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oset_rename_at_416_82))) != 93 )
                {
                    finalize_4d9dff2c4111fea5(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oset_rename_at_416_82));
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                } else {
                    regex::ReNode * __oval_rename_at_426_83 = das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oset_rename_at_416_82,__context__);
                    das_copy(das_swizzle_ref<int32_t,range,0>::swizzle(__oval_rename_at_426_83->at) /*x*/,__offset_rename_at_404_80);
                    ++das_swizzle_ref<int32_t,range,1>::swizzle(__oval_rename_at_426_83->at) /*y*/;
                    das_copy(__oval_rename_at_426_83->op,DAS_COMMENT(enum) regex::ReOp::Set);
                    if ( __negative_rename_at_412_81 )
                    {
                        set_negative_e65d971bcd9eb5c1(__context__,das_arg<TDim<uint32_t,8>>::pass(__oval_rename_at_426_83->cset));
                    };
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oset_rename_at_416_82);
                };
            };
        };
    };
}

inline AutoVariant<regex::ReNode *,void *> re_any_9e5c841624efd77c ( Context * __context__, char * const  __expr_rename_at_438_84, int32_t __offset_rename_at_438_85 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_438_84,__offset_rename_at_438_85) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_438_84,__offset_rename_at_438_85) == 46 )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_444;
                das_zero(__mks_444);
                das_copy((__mks_444.op),(DAS_COMMENT(enum) regex::ReOp::Any));
                das_copy((__mks_444.at),(range(__offset_rename_at_438_85,__offset_rename_at_438_85 + 1)));
                return __mks_444;
            })()))));
        } else {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        };
    };
}

inline AutoVariant<regex::ReNode *,void *> re_eos_91af11e09a55593b ( Context * __context__, char * const  __expr_rename_at_450_86, int32_t __offset_rename_at_450_87 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_450_86,__offset_rename_at_450_87) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_450_86,__offset_rename_at_450_87) == 36 )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_456;
                das_zero(__mks_456);
                das_copy((__mks_456.op),(DAS_COMMENT(enum) regex::ReOp::Eos));
                das_copy((__mks_456.at),(range(__offset_rename_at_450_87,__offset_rename_at_450_87 + 1)));
                return __mks_456;
            })()))));
        } else {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        };
    };
}

inline AutoVariant<regex::ReNode *,void *> re_group_a76956b21d051425 ( Context * __context__, char * const  __expr_rename_at_462_88, int32_t __offset_rename_at_462_89 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_462_88,__offset_rename_at_462_89) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_462_88,__offset_rename_at_462_89) != 40 )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        } else {
            AutoVariant<regex::ReNode *,void *> __ore_rename_at_470_90 = re_re_e5f845d3bd4b2bf9(__context__,__expr_rename_at_462_88,__offset_rename_at_462_89 + 1);
            if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__ore_rename_at_470_90) )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
            } else {
                if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_462_88,next_c7b14aa26d681478(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__ore_rename_at_470_90))) != 41 )
                {
                    finalize_4d9dff2c4111fea5(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__ore_rename_at_470_90));
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                } else {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_478;
                        das_zero(__mks_478);
                        das_copy((__mks_478.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                        das_copy((__mks_478.subexpr),(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__ore_rename_at_470_90,__context__)));
                        das_copy((__mks_478.at),(range(__offset_rename_at_462_89,next_c7b14aa26d681478(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__ore_rename_at_470_90)) + 1)));
                        return __mks_478;
                    })()))));
                };
            };
        };
    };
}

inline AutoVariant<regex::ReNode *,void *> re_elementary_8e517f8be58bf1a6 ( Context * __context__, char * const  __expr_rename_at_482_91, int32_t __offset_rename_at_482_92 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        AutoVariant<regex::ReNode *,void *> __ogr_rename_at_487_93 = re_group_a76956b21d051425(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
        if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__ogr_rename_at_487_93) )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__ogr_rename_at_487_93);
        } else {
            AutoVariant<regex::ReNode *,void *> __oany_rename_at_491_94 = re_any_9e5c841624efd77c(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
            if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__oany_rename_at_491_94) )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oany_rename_at_491_94);
            } else {
                AutoVariant<regex::ReNode *,void *> __oeos_rename_at_495_95 = re_eos_91af11e09a55593b(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
                if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__oeos_rename_at_495_95) )
                {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oeos_rename_at_495_95);
                } else {
                    AutoVariant<regex::ReNode *,void *> __oset_rename_at_499_96 = re_set_a55f6e80806300ed(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
                    if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__oset_rename_at_499_96) )
                    {
                        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oset_rename_at_499_96);
                    } else {
                        AutoVariant<regex::ReNode *,void *> __ochr_rename_at_503_97 = re_char_1cacb1cfe082d534(__context__,__expr_rename_at_482_91,__offset_rename_at_482_92);
                        if ( das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::is(__ochr_rename_at_503_97) )
                        {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__ochr_rename_at_503_97);
                        } else {
                            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                        };
                    };
                };
            };
        };
    };
}

inline regex::ReNode * mk_concat_1ec73c2f3691d1d5 ( Context * __context__, regex::ReNode * __left_rename_at_510_98, regex::ReNode * __right_rename_at_510_99 )
{
    if ( __left_rename_at_510_98 == nullptr )
    {
        return das_auto_cast<regex::ReNode *>::cast(__right_rename_at_510_99);
    } else if ( (__left_rename_at_510_98->op == DAS_COMMENT(enum) regex::ReOp::Char) && (__right_rename_at_510_99->op == DAS_COMMENT(enum) regex::ReOp::Char) )
    {
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_510_98->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_510_99->at) /*y*/);
        SimPolicy<char *>::SetAdd((char *)&(__left_rename_at_510_98->text),cast<char *>::from(__right_rename_at_510_99->text),*__context__,nullptr);
        __left_rename_at_510_98->textLen += __right_rename_at_510_99->textLen;
        finalize_2e3cca8177873ef7(__context__,__right_rename_at_510_99);
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_510_98);
    } else if ( ((__left_rename_at_510_98->op == DAS_COMMENT(enum) regex::ReOp::Concat) && (__left_rename_at_510_98->right->op == DAS_COMMENT(enum) regex::ReOp::Char)) && (__right_rename_at_510_99->op == DAS_COMMENT(enum) regex::ReOp::Char) )
    {
        regex::ReNode * __lor_rename_at_524_100 = __left_rename_at_510_98->right;
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__left_rename_at_510_98->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_510_99->at) /*y*/);
        das_copy(das_swizzle_ref<int32_t,range,1>::swizzle(__lor_rename_at_524_100->at) /*y*/,das_swizzle_ref<int32_t,range,1>::swizzle(__right_rename_at_510_99->at) /*y*/);
        SimPolicy<char *>::SetAdd((char *)&(__lor_rename_at_524_100->text),cast<char *>::from(__right_rename_at_510_99->text),*__context__,nullptr);
        __lor_rename_at_524_100->textLen += __right_rename_at_510_99->textLen;
        finalize_2e3cca8177873ef7(__context__,__right_rename_at_510_99);
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

inline regex::ReNode * mk_union_317bd0efb93d42ad ( Context * __context__, regex::ReNode * __left_rename_at_538_101, regex::ReNode * __right_rename_at_538_102 )
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
                    _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9(__context__,das_arg<TArray<regex::ReNode *>>::pass(__left_rename_at_538_101->all),(*__x_rename_at_542_103));
                }
                __x_iterator.close(__context__,(__x_rename_at_542_103));
            };
            finalize_2e3cca8177873ef7(__context__,__right_rename_at_538_102);
        } else {
            _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9(__context__,das_arg<TArray<regex::ReNode *>>::pass(__left_rename_at_538_101->all),__right_rename_at_538_102);
        };
        return das_auto_cast<regex::ReNode *>::cast(__left_rename_at_538_101);
    } else if ( __right_rename_at_538_102->op == DAS_COMMENT(enum) regex::ReOp::Union )
    {
        das_copy(das_swizzle_ref<int32_t,range,0>::swizzle(__right_rename_at_538_102->at) /*x*/,das_swizzle_ref<int32_t,range,0>::swizzle(__left_rename_at_538_101->at) /*x*/);
        _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9(__context__,das_arg<TArray<regex::ReNode *>>::pass(__right_rename_at_538_102->all),__left_rename_at_538_101);
        return das_auto_cast<regex::ReNode *>::cast(__right_rename_at_538_102);
    } else {
        TDim<regex::ReNode *,2> _temp_make_local_557_50_0; _temp_make_local_557_50_0;
        return das_auto_cast<regex::ReNode *>::cast(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_557;
            das_zero(__mks_557);
            das_copy((__mks_557.op),(DAS_COMMENT(enum) regex::ReOp::Union));
            das_move((__mks_557.all),(_FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6(__context__,das_arg<TDim<regex::ReNode *,2>>::pass((([&]() -> TDim<regex::ReNode *,2>& {
                _temp_make_local_557_50_0(0,__context__) = __left_rename_at_538_101;
                _temp_make_local_557_50_0(1,__context__) = __right_rename_at_538_102;
                return _temp_make_local_557_50_0;
            })())))));
            das_copy((__mks_557.at),(range(v_extract_xi(v_cast_vec4i(__left_rename_at_538_101->at)) /*x*/,v_extract_yi(v_cast_vec4i(__right_rename_at_538_102->at)) /*y*/)));
            return __mks_557;
        })())));
    };
}

inline AutoVariant<regex::ReNode *,void *> re_re_e5f845d3bd4b2bf9 ( Context * __context__, char * const  __expr_rename_at_565_104, int32_t __offset_rename_at_565_105 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_565_104,__offset_rename_at_565_105) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        int32_t __cofs_rename_at_570_106 = __offset_rename_at_565_105;
        regex::ReNode * __last_rename_at_571_107 = 0;
        while ( !eos_bcd93149648ad887(__context__,__expr_rename_at_565_104,__cofs_rename_at_570_106) )
        {
            if ( at_ac9760d67b4716d0(__context__,__expr_rename_at_565_104,__cofs_rename_at_570_106) == 124 )
            {
                if ( __last_rename_at_571_107 == nullptr )
                {
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                } else {
                    AutoVariant<regex::ReNode *,void *> __oright_rename_at_578_108 = re_re_e5f845d3bd4b2bf9(__context__,__expr_rename_at_565_104,__cofs_rename_at_570_106 + 1);
                    if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__oright_rename_at_578_108) )
                    {
                        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
                    } else {
                        das_copy(__last_rename_at_571_107,mk_union_317bd0efb93d42ad(__context__,__last_rename_at_571_107,das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oright_rename_at_578_108,__context__)));
                        das_copy(__cofs_rename_at_570_106,das_swizzle_ref<int32_t,range,1>::swizzle(__last_rename_at_571_107->at) /*y*/);
                    };
                };
            } else {
                AutoVariant<regex::ReNode *,void *> __onext_rename_at_586_109 = re_basic_f733ea9ca09e6bda(__context__,__expr_rename_at_565_104,__cofs_rename_at_570_106);
                if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__onext_rename_at_586_109) )
                {
                    AutoVariant<regex::ReNode *,void *> _temp_make_local_588_38_1; _temp_make_local_588_38_1;
                    AutoVariant<regex::ReNode *,void *> _temp_make_local_588_47_2; _temp_make_local_588_47_2;
                    return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((__last_rename_at_571_107 == nullptr) ? das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((_temp_make_local_588_38_1 = (nada_a36e274f9f880579(__context__)))) : das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast((_temp_make_local_588_47_2 = (maybe_cb698f19184a9bc8(__context__,__last_rename_at_571_107)))));
                } else {
                    das_copy(__last_rename_at_571_107,mk_concat_1ec73c2f3691d1d5(__context__,__last_rename_at_571_107,das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__onext_rename_at_586_109,__context__)));
                    das_copy(__cofs_rename_at_570_106,das_swizzle_ref<int32_t,range,1>::swizzle(__last_rename_at_571_107->at) /*y*/);
                };
            };
        };
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,__last_rename_at_571_107));
    };
}

inline AutoVariant<regex::ReNode *,void *> re_basic_f733ea9ca09e6bda ( Context * __context__, char * const  __expr_rename_at_600_110, int32_t __offset_rename_at_600_111 )
{
    if ( eos_bcd93149648ad887(__context__,__expr_rename_at_600_110,__offset_rename_at_600_111) )
    {
        return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
    } else {
        AutoVariant<regex::ReNode *,void *> __oelem_rename_at_605_112 = re_elementary_8e517f8be58bf1a6(__context__,__expr_rename_at_600_110,__offset_rename_at_600_111);
        if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__oelem_rename_at_605_112) )
        {
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(nada_a36e274f9f880579(__context__));
        } else {
            int32_t __ch_rename_at_609_113 = ((int32_t)at_ac9760d67b4716d0(__context__,__expr_rename_at_600_110,next_c7b14aa26d681478(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oelem_rename_at_605_112))));
            if ( __ch_rename_at_609_113 == 42 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_611;
                    das_zero(__mks_611);
                    das_copy((__mks_611.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                    das_copy((__mks_611.subexpr),(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oelem_rename_at_605_112,__context__)));
                    das_copy((__mks_611.at),(range(__offset_rename_at_600_111,next_c7b14aa26d681478(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oelem_rename_at_605_112)) + 1)));
                    return __mks_611;
                })()))));
            } else if ( __ch_rename_at_609_113 == 43 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_613;
                    das_zero(__mks_613);
                    das_copy((__mks_613.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                    das_copy((__mks_613.subexpr),(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oelem_rename_at_605_112,__context__)));
                    das_copy((__mks_613.at),(range(__offset_rename_at_600_111,next_c7b14aa26d681478(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oelem_rename_at_605_112)) + 1)));
                    return __mks_613;
                })()))));
            } else if ( __ch_rename_at_609_113 == 63 )
            {
                return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(maybe_cb698f19184a9bc8(__context__,das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_615;
                    das_zero(__mks_615);
                    das_copy((__mks_615.op),(DAS_COMMENT(enum) regex::ReOp::Question));
                    das_copy((__mks_615.subexpr),(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__oelem_rename_at_605_112,__context__)));
                    das_copy((__mks_615.at),(range(__offset_rename_at_600_111,next_c7b14aa26d681478(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__oelem_rename_at_605_112)) + 1)));
                    return __mks_615;
                })()))));
            };
            return das_auto_cast_ref<AutoVariant<regex::ReNode *,void *>>::cast(__oelem_rename_at_605_112);
        };
    };
}

inline regex::ReNode * re_parse_a11fbde82111aa69 ( Context * __context__, char * const  __expr_rename_at_620_114 )
{
    AutoVariant<regex::ReNode *,void *> __ore_rename_at_621_115 = re_re_e5f845d3bd4b2bf9(__context__,__expr_rename_at_620_114,0);
    if ( das_get_auto_variant_field<void *,1,regex::ReNode *,void *>::is(__ore_rename_at_621_115) )
    {
        return das_auto_cast<regex::ReNode *>::cast(nullptr);
    } else {
        int32_t __at_rename_at_625_116 = ((int32_t)next_c7b14aa26d681478(__context__,das_arg<AutoVariant<regex::ReNode *,void *>>::pass(__ore_rename_at_621_115)));
        return das_auto_cast<regex::ReNode *>::cast((__at_rename_at_625_116 != builtin_string_length(__expr_rename_at_620_114,__context__)) ? das_auto_cast<void *>::cast(nullptr) : das_auto_cast<void *>::cast(das_get_auto_variant_field<regex::ReNode *,0,regex::ReNode *,void *>::as(__ore_rename_at_621_115,__context__)));
    };
}

inline void visit_top_down_4fecc3dfe95f303c ( Context * __context__, regex::ReNode * __node_rename_at_633_117, Block DAS_COMMENT((void,regex::ReNode *)) const  &  __blk_rename_at_633_118 )
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
            visit_top_down_4fecc3dfe95f303c(__context__,(*__x_rename_at_635_119),__blk_rename_at_633_118);
        }
        __x_iterator.close(__context__,(__x_rename_at_635_119));
    };
    if ( __node_rename_at_633_117->subexpr != nullptr )
    {
        visit_top_down_4fecc3dfe95f303c(__context__,__node_rename_at_633_117->subexpr,__blk_rename_at_633_118);
    };
    if ( __node_rename_at_633_117->left != nullptr )
    {
        visit_top_down_4fecc3dfe95f303c(__context__,__node_rename_at_633_117->left,__blk_rename_at_633_118);
    };
    if ( __node_rename_at_633_117->right != nullptr )
    {
        visit_top_down_4fecc3dfe95f303c(__context__,__node_rename_at_633_117->right,__blk_rename_at_633_118);
    };
}

inline void re_assign_next_18621bfa0d3dbb85 ( Context * __context__, regex::Regex &  __re_rename_at_649_120 ) { das_stack_prologue __prologue(__context__,128,"re_assign_next " DAS_FILE_LINE);
{
    int32_t __id_rename_at_650_121 = 0;
    visit_top_down_4fecc3dfe95f303c(__context__,__re_rename_at_649_120.root,das_make_block<void,regex::ReNode *>(__context__,96,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_651_122) -> void{
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

inline void re_assign_groups_ee95de912bf625ba ( Context * __context__, regex::Regex &  __re_rename_at_666_124 ) { das_stack_prologue __prologue(__context__,128,"re_assign_groups " DAS_FILE_LINE);
{
    AutoTuple<range,char *> _temp_make_local_667_21_3; _temp_make_local_667_21_3;
    _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(__re_rename_at_666_124.groups),das_arg<AutoTuple<range,char *>>::pass((([&]() -> AutoTuple<range,char *>& {
        das_get_auto_tuple_field<range,0,range,char *>::get(_temp_make_local_667_21_3) = range(0,0);
        das_get_auto_tuple_field<char *,1,range,char *>::get(_temp_make_local_667_21_3) = nullptr;
        return _temp_make_local_667_21_3;
    })())));
    visit_top_down_4fecc3dfe95f303c(__context__,__re_rename_at_666_124.root,das_make_block<void,regex::ReNode *>(__context__,80,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_668_125) -> void{
        if ( __node_rename_at_668_125->op == DAS_COMMENT(enum) regex::ReOp::Group )
        {
            AutoTuple<range,char *> _temp_make_local_672_29_4; _temp_make_local_672_29_4;
            int32_t __index_rename_at_670_126 = ((int32_t)builtin_array_size(das_arg<TArray<AutoTuple<range,char *>>>::pass(__re_rename_at_666_124.groups)));
            das_copy(__node_rename_at_668_125->index,__index_rename_at_670_126);
            _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(__re_rename_at_666_124.groups),das_arg<AutoTuple<range,char *>>::pass((([&]() -> AutoTuple<range,char *>& {
                das_get_auto_tuple_field<range,0,range,char *>::get(_temp_make_local_672_29_4) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(_temp_make_local_672_29_4) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_6, cast<int32_t>::from(__index_rename_at_670_126)));
                return _temp_make_local_672_29_4;
            })())));
        };
    }));
}}

inline void re_assign_match_functions_d1123942ad08a870 ( Context * __context__, regex::Regex &  __re_rename_at_681_127 ) { das_stack_prologue __prologue(__context__,96,"re_assign_match_functions " DAS_FILE_LINE);
{
    visit_top_down_4fecc3dfe95f303c(__context__,__re_rename_at_681_127.root,das_make_block<void,regex::ReNode *>(__context__,80,0,&__func_info__8b6d6c464a360a4f,[&](regex::ReNode * __node_rename_at_682_128) -> void{
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

inline uint8_t const  * re_match2_single_char_efd3120a5ed2dc78 ( Context * __context__, regex::Regex &  __regex_rename_at_730_129, regex::ReNode * __node_rename_at_730_130, uint8_t const  * const  __str_rename_at_730_131 )
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

inline uint8_t const  * re_match2_char_563cc8699254595c ( Context * __context__, regex::Regex &  __regex_rename_at_753_134, regex::ReNode * __node_rename_at_753_135, uint8_t const  * const  __str_rename_at_753_136 )
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

inline uint8_t const  * re_match2_union_483bf85dbc5196dd ( Context * __context__, regex::Regex &  __regex_rename_at_779_139, regex::ReNode * __node_rename_at_779_140, uint8_t const  * const  __str_rename_at_779_141 )
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

inline uint8_t const  * re_match2_set_31b09b463a9d7b2b ( Context * __context__, regex::Regex &  __regex_rename_at_797_144, regex::ReNode * __node_rename_at_797_145, uint8_t const  * const  __str_rename_at_797_146 )
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

inline uint8_t const  * re_match2_any_9b00b66687d95b62 ( Context * __context__, regex::Regex &  __regex_rename_at_820_149, regex::ReNode * __node_rename_at_820_150, uint8_t const  * const  __str_rename_at_820_151 )
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

inline uint8_t const  * re_match2_concat_2bd31fc7755cdf2a ( Context * __context__, regex::Regex &  __regex_rename_at_840_154, regex::ReNode * __node_rename_at_840_155, uint8_t const  * const  __str_rename_at_840_156 )
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

inline uint8_t const  * re_match2_eos_a35408df9434ba3c ( Context * __context__, regex::Regex &  __regex_rename_at_857_159, regex::ReNode * __node_rename_at_857_160, uint8_t const  * const  __str_rename_at_857_161 )
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

inline uint8_t const  * re_match2_question_4acdfdc420ce399a ( Context * __context__, regex::Regex &  __regex_rename_at_874_163, regex::ReNode * __node_rename_at_874_164, uint8_t const  * const  __str_rename_at_874_165 )
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

inline uint8_t const  * re_match2_plus_adb26cc707eb0c55 ( Context * __context__, regex::Regex &  __regex_rename_at_893_169, regex::ReNode * __node_rename_at_893_170, uint8_t const  * const  __str_rename_at_893_171 )
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

inline uint8_t const  * re_match2_plus_set_aeb296540242ef22 ( Context * __context__, regex::Regex &  __regex_rename_at_928_177, regex::ReNode * __node_rename_at_928_178, uint8_t const  * const  __str_rename_at_928_179 )
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

inline uint8_t const  * re_match2_star_bbfb62194a4dc1ae ( Context * __context__, regex::Regex &  __regex_rename_at_971_184, regex::ReNode * __node_rename_at_971_185, uint8_t const  * const  __str_rename_at_971_186 )
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

inline uint8_t const  * re_match2_group_82784eced054ba4e ( Context * __context__, regex::Regex &  __regex_rename_at_1000_192, regex::ReNode * __node_rename_at_1000_193, uint8_t const  * const  __str_rename_at_1000_194 )
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

inline bool re_early_out_6710f0abebdc4106 ( Context * __context__, TDim<uint32_t,8> &  __cset_rename_at_1020_197, regex::ReNode * const  __node_rename_at_1020_198 )
{
    if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Char )
    {
        set_or_char_5d5313390b879abc(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),get_character_at(__node_rename_at_1020_198->text,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        return das_auto_cast<bool>::cast(false);
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Set )
    {
        set_or_set_f7fe9d5d2a5b194b(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->cset);
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
        return das_auto_cast<bool>::cast(re_early_out_6710f0abebdc4106(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->subexpr));
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Plus )
    {
        return das_auto_cast<bool>::cast(re_early_out_6710f0abebdc4106(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->subexpr));
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Star )
    {
        re_early_out_6710f0abebdc4106(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->subexpr);
        return das_auto_cast<bool>::cast(true);
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Question )
    {
        re_early_out_6710f0abebdc4106(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->subexpr);
        return das_auto_cast<bool>::cast(true);
    } else if ( __node_rename_at_1020_198->op == DAS_COMMENT(enum) regex::ReOp::Concat )
    {
        bool __left_rename_at_1045_200 = ((bool)re_early_out_6710f0abebdc4106(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->left));
        return das_auto_cast<bool>::cast(__left_rename_at_1045_200 ? das_auto_cast<bool>::cast(re_early_out_6710f0abebdc4106(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),__node_rename_at_1020_198->right)) : das_auto_cast<bool>::cast(false));
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
                das_copy(__any_rename_at_1048_201,re_early_out_6710f0abebdc4106(__context__,das_arg<TDim<uint32_t,8>>::pass(__cset_rename_at_1020_197),(*__sub_rename_at_1049_202)) || __any_rename_at_1048_201);
            }
            __sub_iterator.close(__context__,(__sub_rename_at_1049_202));
        };
        return das_auto_cast<bool>::cast(__any_rename_at_1048_201);
    } else {
        builtin_throw(((char *) "unsupported operation"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<bool>::cast(false);
    };
}

inline bool is_valid_87c94e9920e8a112 ( Context * __context__, regex::Regex &  __re_rename_at_1063_203 )
{
    return das_auto_cast<bool>::cast(__re_rename_at_1063_203.root != nullptr);
}

inline bool regex_compile_b802a48fba17c551 ( Context * __context__, regex::Regex &  __re_rename_at_1068_204, char * const  __expr_rename_at_1068_205 )
{
    das_copy(__re_rename_at_1068_204.root,re_parse_a11fbde82111aa69(__context__,__expr_rename_at_1068_205));
    if ( __re_rename_at_1068_204.root != nullptr )
    {
        re_assign_next_18621bfa0d3dbb85(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1068_204));
        re_assign_groups_ee95de912bf625ba(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1068_204));
        re_assign_match_functions_d1123942ad08a870(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1068_204));
        re_early_out_6710f0abebdc4106(__context__,das_arg<TDim<uint32_t,8>>::pass(__re_rename_at_1068_204.earlyOut),__re_rename_at_1068_204.root);
        das_copy(__re_rename_at_1068_204.canEarlyOut,!is_set_empty_1f052aa7f260c4eb(__context__,das_arg<TDim<uint32_t,8>>::pass(__re_rename_at_1068_204.earlyOut)));
    };
    return das_auto_cast<bool>::cast(__re_rename_at_1068_204.root != nullptr);
}

inline regex::Regex regex_compile_9c6bbe3f7bf967b8 ( Context * __context__, char * const  __expr_rename_at_1082_206 )
{
    regex::Regex __re_rename_at_1083_207;das_zero(__re_rename_at_1083_207);
    if ( !regex_compile_b802a48fba17c551(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1083_207),__expr_rename_at_1082_206) )
    {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_8, cast<char * const >::from(((char *) "regular expression ")), cast<char * const >::from(__expr_rename_at_1082_206), cast<char * const >::from(((char *) " did not compile")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return /* <- */ das_auto_cast_move<regex::Regex>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3d380093f2b970c4(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1083_207)));
}

inline regex::Regex regex_compile_8b8cd96a70efcb1c ( Context * __context__, regex::Regex &  __re_rename_at_1090_208 )
{
    if ( __re_rename_at_1090_208.root != nullptr )
    {
        re_assign_next_18621bfa0d3dbb85(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1090_208));
        re_assign_match_functions_d1123942ad08a870(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1090_208));
    };
    return /* <- */ das_auto_cast_move<regex::Regex>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3d380093f2b970c4(__context__,das_arg<regex::Regex>::pass(__re_rename_at_1090_208)));
}

inline int32_t regex_match_d1084a47d6e2d53e ( Context * __context__, regex::Regex &  __regex_rename_at_1098_209, char * const  __str_rename_at_1098_210, int32_t __offset_rename_at_1098_211 )
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

inline char * regex_group_f150ed72d9c6b909 ( Context * __context__, regex::Regex const  &  __regex_rename_at_1117_213, int32_t __index_rename_at_1117_214, char * const  __match_rename_at_1117_215 )
{
    range __sub_range_rename_at_1119_216 = ((range)das_get_auto_tuple_field<range,0,range,char *>::get(__regex_rename_at_1117_213.groups(__index_rename_at_1117_214,__context__)));
    return das_auto_cast<char *>::cast(((char * const )(builtin_string_slice1(__match_rename_at_1117_215,v_extract_xi(v_cast_vec4i(__sub_range_rename_at_1119_216)) /*x*/,v_extract_yi(v_cast_vec4i(__sub_range_rename_at_1119_216)) /*y*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline void regex_foreach_95b590dd0dd4944f ( Context * __context__, regex::Regex &  __regex_rename_at_1124_217, char * const  __str_rename_at_1124_218, Block DAS_COMMENT((bool,range)) const  &  __blk_rename_at_1124_219 )
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

inline char * regex_replace_6c020ee2b76e6419 ( Context * __context__, regex::Regex &  __regex_rename_at_1172_226, char * const  __str_rename_at_1172_227, Block DAS_COMMENT((char *,char * const )) const  &  __blk_rename_at_1172_228 )
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
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_9,cast<StringBuilderWriter &>::from(__writer_rename_at_1178_229),cast<char * const >::from(__repl_rename_at_1209_237)));
                    das_copy(__cstr_rename_at_1182_232,__om_rename_at_1207_236);
                } else {
                    write_string_char(das_arg<StringBuilderWriter>::pass(__writer_rename_at_1178_229),int32_t((*(__cstr_rename_at_1182_232))));
                    das_ptr_inc(__cstr_rename_at_1182_232,1);
                };
            };
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
}

inline void regex_debug_35a427292763c452 ( Context * __context__, regex::Regex const  &  __regex_rename_at_1226_238 )
{
    debug_re_f8fd4cc00f0dc78d(__context__,__regex_rename_at_1226_238.root,1);
    builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __regex_rename_at_1226_238.canEarlyOut )
    {
        builtin_print(((char *) "early out: "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_set_13e232e40e242fa0(__context__,__regex_rename_at_1226_238.earlyOut);
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
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_8, cast<char * const >::from(((char *) "\t")), cast<char * const >::from(das_get_auto_tuple_field<char *,1,range,char *>::get((*__g_rename_at_1237_239))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            }
            __g_iterator.close(__context__,(__g_rename_at_1237_239));
        };
    };
}

inline void debug_set_13e232e40e242fa0 ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_1243_240 )
{
    {
        bool __need_loop_1245 = true;
        // x: int const
        das_iterator<range> __x_iterator(range(0,256));
        int32_t __x_rename_at_1245_241;
        __need_loop_1245 = __x_iterator.first(__context__,(__x_rename_at_1245_241)) && __need_loop_1245;
        for ( ; __need_loop_1245 ; __need_loop_1245 = __x_iterator.next(__context__,(__x_rename_at_1245_241)) )
        {
            if ( (__cset_rename_at_1243_240((SimPolicy<int32_t>::BinShr(__x_rename_at_1245_241,5,*__context__,nullptr)),__context__) & uint32_t(SimPolicy<int32_t>::BinShl(1,(__x_rename_at_1245_241 & 31),*__context__,nullptr))) != 0x0u )
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

inline void debug_re_f8fd4cc00f0dc78d ( Context * __context__, regex::ReNode * const  __node_rename_at_1256_242, int32_t __tab_rename_at_1256_243 )
{
    builtin_print(((char *) "("),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __node_rename_at_1256_242 != nullptr )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<char * const >::from(((char *) "#")), cast<int32_t>::from(__node_rename_at_1256_242->id))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        if ( __node_rename_at_1256_242->next != nullptr )
        {
            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_13, cast<char * const >::from(((char *) "->")), cast<int32_t>::from(__node_rename_at_1256_242->next->id))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
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
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_8, cast<char * const >::from(((char *) "Char `")), cast<char * const >::from(__node_rename_at_1256_242->text), cast<char * const >::from(((char *) "`)")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Set )
    {
        builtin_print(((char *) "Set "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_set_13e232e40e242fa0(__context__,__node_rename_at_1256_242->cset);
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
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_242->subexpr,__tab_rename_at_1256_243 + 1);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Plus )
    {
        builtin_print(((char *) "Plus "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_242->subexpr,__tab_rename_at_1256_243);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Star )
    {
        builtin_print(((char *) "Star "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_242->subexpr,__tab_rename_at_1256_243);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Question )
    {
        builtin_print(((char *) "Question "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_242->subexpr,__tab_rename_at_1256_243);
        builtin_print(((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else if ( __node_rename_at_1256_242->op == DAS_COMMENT(enum) regex::ReOp::Concat )
    {
        builtin_print(((char *) "Contact"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_17, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_242->left,__tab_rename_at_1256_243 + 1);
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_17, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        debug_re_f8fd4cc00f0dc78d(__context__,__node_rename_at_1256_242->right,__tab_rename_at_1256_243 + 1);
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_8, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243 - 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ")")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
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
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_12, cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                debug_re_f8fd4cc00f0dc78d(__context__,(*__sub_rename_at_1304_244),__tab_rename_at_1256_243 + 1);
                builtin_print(((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            }
            __sub_iterator.close(__context__,(__sub_rename_at_1304_244));
        };
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_17, cast<char * const >::from(((char * const )(string_repeat(((char *) "\t"),__tab_rename_at_1256_243 - 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ")")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        builtin_throw(((char *) "unsupported op"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline int32_t rnd_set_character_6517e452a18c5606 ( Context * __context__, TDim<uint32_t,8> const  &  __cset_rename_at_1317_245, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1317_246 )
{
    uint32_t __total_rename_at_1318_247 = ((uint32_t)char_set_total(__cset_rename_at_1317_245));
    uint32_t __idx_rename_at_1319_248 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1317_246),__idx_rename_at_1319_248);
    das_copy(__idx_rename_at_1319_248,SimPolicy<uint32_t>::Mod(__idx_rename_at_1319_248,__total_rename_at_1318_247,*__context__,nullptr));
    return das_auto_cast<int32_t>::cast(char_set_element(int32_t(__idx_rename_at_1319_248),__cset_rename_at_1317_245));
}

inline void re_gen2_fail_a5fdf20930267896 ( Context * __context__, regex::ReNode * __node_rename_at_1325_249, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1325_250, StringBuilderWriter &  __str_rename_at_1325_251 )
{
    DAS_ASSERTF((false),(((char *) "not running generated regex")));
}

inline void re_gen2_char_e6b9f720990cc2d7 ( Context * __context__, regex::ReNode * __node_rename_at_1332_252, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1332_253, StringBuilderWriter &  __str_rename_at_1332_254 )
{
    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_22,cast<StringBuilderWriter &>::from(__str_rename_at_1332_254),cast<char *>::from(__node_rename_at_1332_252->text)));
    regex::ReNode * __node2_rename_at_1337_255 = __node_rename_at_1332_252->next;
    if ( __node2_rename_at_1337_255 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1337_255->gen2,__node2_rename_at_1337_255,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1332_253),das_arg<StringBuilderWriter>::pass(__str_rename_at_1332_254));
    };
}

inline void re_gen2_union_44153b0e184e2888 ( Context * __context__, regex::ReNode * __node_rename_at_1343_256, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1343_257, StringBuilderWriter &  __str_rename_at_1343_258 )
{
    uint32_t __nidx_rename_at_1347_259 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1343_257),__nidx_rename_at_1347_259);
    das_copy(__nidx_rename_at_1347_259,SimPolicy<uint32_t>::Mod(__nidx_rename_at_1347_259,uint32_t(builtin_array_size(das_arg<TArray<regex::ReNode *>>::pass(__node_rename_at_1343_256->all))),*__context__,nullptr));
    regex::ReNode * __sub_rename_at_1350_260 = __node_rename_at_1343_256->all(__nidx_rename_at_1347_259,__context__);
    das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__sub_rename_at_1350_260->gen2,__sub_rename_at_1350_260,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1343_257),das_arg<StringBuilderWriter>::pass(__str_rename_at_1343_258));
}

inline void re_gen2_set_5483397ec36cfff0 ( Context * __context__, regex::ReNode * __node_rename_at_1354_261, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1354_262, StringBuilderWriter &  __str_rename_at_1354_263 )
{
    write_string_char(das_arg<StringBuilderWriter>::pass(__str_rename_at_1354_263),rnd_set_character_6517e452a18c5606(__context__,das_arg<TDim<uint32_t,8>>::pass(__node_rename_at_1354_261->cset),das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1354_262)));
    regex::ReNode * __node2_rename_at_1359_264 = __node_rename_at_1354_261->next;
    if ( __node2_rename_at_1359_264 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1359_264->gen2,__node2_rename_at_1359_264,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1354_262),das_arg<StringBuilderWriter>::pass(__str_rename_at_1354_263));
    };
}

inline void re_gen2_any_200931e8e5db950 ( Context * __context__, regex::ReNode * __node_rename_at_1365_265, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1365_266, StringBuilderWriter &  __str_rename_at_1365_267 )
{
    uint32_t __ch_rename_at_1369_268 = 0;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1365_266),__ch_rename_at_1369_268);
    write_string_char(das_arg<StringBuilderWriter>::pass(__str_rename_at_1365_267),int32_t((SimPolicy<uint32_t>::Mod(__ch_rename_at_1369_268,0xffu,*__context__,nullptr)) + 0x1u));
    regex::ReNode * __node2_rename_at_1372_269 = __node_rename_at_1365_265->next;
    if ( __node2_rename_at_1372_269 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1372_269->gen2,__node2_rename_at_1372_269,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1365_266),das_arg<StringBuilderWriter>::pass(__str_rename_at_1365_267));
    };
}

inline void re_gen2_concat_bb7cd4a1d8751a6c ( Context * __context__, regex::ReNode * __node_rename_at_1378_270, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1378_271, StringBuilderWriter &  __str_rename_at_1378_272 )
{
    regex::ReNode * __left_rename_at_1382_273 = __node_rename_at_1378_270->left;
    das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__left_rename_at_1382_273->gen2,__left_rename_at_1382_273,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1378_271),das_arg<StringBuilderWriter>::pass(__str_rename_at_1378_272));
}

inline void re_gen2_eos_6a9fe263836b5d74 ( Context * __context__, regex::ReNode * __node_rename_at_1386_274, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1386_275, StringBuilderWriter &  __str_rename_at_1386_276 )
{
    regex::ReNode * __node2_rename_at_1390_277 = __node_rename_at_1386_274->next;
    if ( __node2_rename_at_1390_277 != nullptr )
    {
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__node2_rename_at_1390_277->gen2,__node2_rename_at_1390_277,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1386_275),das_arg<StringBuilderWriter>::pass(__str_rename_at_1386_276));
    };
}

inline void re_gen2_question_ac79834a17fb3749 ( Context * __context__, regex::ReNode * __node_rename_at_1396_278, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1396_279, StringBuilderWriter &  __str_rename_at_1396_280 )
{
    uint32_t __a_rename_at_1400_281 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1396_279),__a_rename_at_1400_281);
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

inline void re_gen2_plus_999e35bd61d42dc9 ( Context * __context__, regex::ReNode * __node_rename_at_1414_284, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1414_285, StringBuilderWriter &  __str_rename_at_1414_286 )
{
    uint32_t __cnt_rename_at_1418_287 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1414_285),__cnt_rename_at_1418_287);
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

inline void re_gen2_star_e57464e7f97360fc ( Context * __context__, regex::ReNode * __node_rename_at_1433_291, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1433_292, StringBuilderWriter &  __str_rename_at_1433_293 )
{
    uint32_t __cnt_rename_at_1437_294 = 0x0u;
    _FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1433_292),__cnt_rename_at_1437_294);
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

inline void re_gen2_group_cc75131bc454c003 ( Context * __context__, regex::ReNode * __node_rename_at_1452_298, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1452_299, StringBuilderWriter &  __str_rename_at_1452_300 )
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

inline char * re_gen_e78126cc4b992ab9 ( Context * __context__, regex::Regex &  __re_rename_at_1466_303, Sequence DAS_COMMENT((uint32_t)) &  __rnd_rename_at_1466_304 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_1467_305) DAS_AOT_INLINE_LAMBDA -> void{
        das_invoke_function<void>::invoke<regex::ReNode *,Sequence DAS_COMMENT((uint32_t)) &,StringBuilderWriter &>(__context__,nullptr,__re_rename_at_1466_303.root->gen2,__re_rename_at_1466_303.root,das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__rnd_rename_at_1466_304),das_arg<StringBuilderWriter>::pass(__writer_rename_at_1467_305));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}
static vec4f __wrap__FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd ( Context * __context__ ) {
    TArray<regex::ReNode *> &  arg_Arr = cast_aot_arg<TArray<regex::ReNode *> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_newSize = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd(__context__, arg_Arr, arg_newSize);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759 ( Context * __context__ ) {
    TArray<regex::ReNode *> &  arg_a = cast_aot_arg<TArray<regex::ReNode *> & >::to(*__context__,__context__->abiArguments()[0]);
    _FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d ( Context * __context__ ) {
    TDim<regex::ReNode *,2> const  &  arg_a = cast_aot_arg<TDim<regex::ReNode *,2> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(_FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d(__context__, arg_a));
}
static vec4f __wrap_finalize_1b3fa79e78928d5e ( Context * __context__ ) {
    regex::ReNode &  arg___this = cast_aot_arg<regex::ReNode & >::to(*__context__,__context__->abiArguments()[0]);
    finalize_1b3fa79e78928d5e(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3d380093f2b970c4 ( Context * __context__ ) {
    regex::Regex &  arg_a = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<regex::Regex &>::from(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3d380093f2b970c4(__context__, arg_a));
}
static vec4f __wrap__FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec ( Context * __context__ ) {
    int32_t arg_ch1 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_ch2 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec(__context__, arg_ch1, arg_ch2));
}
static vec4f __wrap_finalize_4d9dff2c4111fea5 ( Context * __context__ ) {
    AutoVariant<regex::ReNode *,void *> &  arg___this = cast_aot_arg<AutoVariant<regex::ReNode *,void *> & >::to(*__context__,__context__->abiArguments()[0]);
    finalize_4d9dff2c4111fea5(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap_finalize_2e3cca8177873ef7 ( Context * __context__ ) {
    regex::ReNode * & arg___this = cast_aot_arg<regex::ReNode * &>::to(*__context__,__context__->abiArguments()[0]);
    finalize_2e3cca8177873ef7(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9 ( Context * __context__ ) {
    TArray<regex::ReNode *> &  arg_Arr = cast_aot_arg<TArray<regex::ReNode *> & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_value = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6 ( Context * __context__ ) {
    TDim<regex::ReNode *,2> &  arg_a = cast_aot_arg<TDim<regex::ReNode *,2> & >::to(*__context__,__context__->abiArguments()[0]);
    *((TArray<regex::ReNode *> *) __context__->abiCMRES) = _FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6(__context__, arg_a);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5 ( Context * __context__ ) {
    TArray<AutoTuple<range,char *>> &  arg_Arr = cast_aot_arg<TArray<AutoTuple<range,char *>> & >::to(*__context__,__context__->abiArguments()[0]);
    AutoTuple<range,char *> &  arg_value = cast_aot_arg<AutoTuple<range,char *> & >::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d ( Context * __context__ ) {
    Sequence DAS_COMMENT((uint32_t)) &  arg_it = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[0]);
    uint32_t & arg_value = cast_aot_arg<uint32_t &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d(__context__, arg_it, arg_value));
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
static vec4f __wrap_trace_value_599ec0b97b535e0e ( Context * __context__ ) {
    regex::ReNode * const  arg_value = cast_aot_arg<regex::ReNode * const >::to(*__context__,__context__->abiArguments()[0]);
    trace_value_599ec0b97b535e0e(__context__, arg_value);
    return v_zero();
}
static vec4f __wrap_nada_a36e274f9f880579 ( Context * __context__ ) {
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = nada_a36e274f9f880579(__context__);
    return v_zero();
}
static vec4f __wrap_maybe_cb698f19184a9bc8 ( Context * __context__ ) {
    regex::ReNode * arg_value = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = maybe_cb698f19184a9bc8(__context__, arg_value);
    return v_zero();
}
static vec4f __wrap_eos_bcd93149648ad887 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(eos_bcd93149648ad887(__context__, arg_expr, arg_offset));
}
static vec4f __wrap_at_ac9760d67b4716d0 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(at_ac9760d67b4716d0(__context__, arg_expr, arg_offset));
}
static vec4f __wrap_next_c7b14aa26d681478 ( Context * __context__ ) {
    AutoVariant<regex::ReNode *,void *> const  &  arg_re = cast_aot_arg<AutoVariant<regex::ReNode *,void *> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(next_c7b14aa26d681478(__context__, arg_re));
}
static vec4f __wrap_is_set_empty_1f052aa7f260c4eb ( Context * __context__ ) {
    TDim<uint32_t,8> const  &  arg_cset = cast_aot_arg<TDim<uint32_t,8> const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_set_empty_1f052aa7f260c4eb(__context__, arg_cset));
}
static vec4f __wrap_set_or_char_5d5313390b879abc ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    set_or_char_5d5313390b879abc(__context__, arg_cset, arg_ch);
    return v_zero();
}
static vec4f __wrap_set_or_range_40be02560bdb916d ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    range arg_bits = cast_aot_arg<range>::to(*__context__,__context__->abiArguments()[1]);
    set_or_range_40be02560bdb916d(__context__, arg_cset, arg_bits);
    return v_zero();
}
static vec4f __wrap_set_invert_4fb9b64ebf15e148 ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    set_invert_4fb9b64ebf15e148(__context__, arg_cset);
    return v_zero();
}
static vec4f __wrap_set_or_set_f7fe9d5d2a5b194b ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    TDim<uint32_t,8> const  &  arg_eset = cast_aot_arg<TDim<uint32_t,8> const  & >::to(*__context__,__context__->abiArguments()[1]);
    set_or_set_f7fe9d5d2a5b194b(__context__, arg_cset, arg_eset);
    return v_zero();
}
static vec4f __wrap_set_negative_e65d971bcd9eb5c1 ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    set_negative_e65d971bcd9eb5c1(__context__, arg_cset);
    return v_zero();
}
static vec4f __wrap_set_meta_5675e929d5b2f227 ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_che = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    set_meta_5675e929d5b2f227(__context__, arg_cset, arg_che);
    return v_zero();
}
static vec4f __wrap_is_meta_character_5210d144f518e88e ( Context * __context__ ) {
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_meta_character_5210d144f518e88e(__context__, arg_ch));
}
static vec4f __wrap_is_set_character_b3eb8d77360f374d ( Context * __context__ ) {
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_set_character_b3eb8d77360f374d(__context__, arg_ch));
}
static vec4f __wrap_from_hex_34b87ad3a69093f9 ( Context * __context__ ) {
    int32_t arg_ch = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<int32_t>::from(from_hex_34b87ad3a69093f9(__context__, arg_ch));
}
static vec4f __wrap_string_from_hex_3b7c632d00a40daf ( Context * __context__ ) {
    int32_t arg_ch1 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<char *>::from(string_from_hex_3b7c632d00a40daf(__context__, arg_ch1));
}
static vec4f __wrap_string_from_hex_7d459c50a54015ae ( Context * __context__ ) {
    int32_t arg_ch1 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_ch2 = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(string_from_hex_7d459c50a54015ae(__context__, arg_ch1, arg_ch2));
}
static vec4f __wrap_re_char_1cacb1cfe082d534 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = re_char_1cacb1cfe082d534(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_set_items_8d1ff416b742d6a0 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = re_set_items_8d1ff416b742d6a0(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_set_a55f6e80806300ed ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = re_set_a55f6e80806300ed(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_any_9e5c841624efd77c ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = re_any_9e5c841624efd77c(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_eos_91af11e09a55593b ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = re_eos_91af11e09a55593b(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_group_a76956b21d051425 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = re_group_a76956b21d051425(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_elementary_8e517f8be58bf1a6 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = re_elementary_8e517f8be58bf1a6(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_mk_concat_1ec73c2f3691d1d5 ( Context * __context__ ) {
    regex::ReNode * arg_left = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_right = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    return cast<regex::ReNode *>::from(mk_concat_1ec73c2f3691d1d5(__context__, arg_left, arg_right));
}
static vec4f __wrap_mk_union_317bd0efb93d42ad ( Context * __context__ ) {
    regex::ReNode * arg_left = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_right = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    return cast<regex::ReNode *>::from(mk_union_317bd0efb93d42ad(__context__, arg_left, arg_right));
}
static vec4f __wrap_re_re_e5f845d3bd4b2bf9 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = re_re_e5f845d3bd4b2bf9(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_basic_f733ea9ca09e6bda ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    *((AutoVariant<regex::ReNode *,void *> *) __context__->abiCMRES) = re_basic_f733ea9ca09e6bda(__context__, arg_expr, arg_offset);
    return v_zero();
}
static vec4f __wrap_re_parse_a11fbde82111aa69 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<regex::ReNode *>::from(re_parse_a11fbde82111aa69(__context__, arg_expr));
}
static vec4f __wrap_visit_top_down_4fecc3dfe95f303c ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Block DAS_COMMENT((void,regex::ReNode *)) const  &  arg_blk = cast_aot_arg<Block DAS_COMMENT((void,regex::ReNode *)) const  & >::to(*__context__,__context__->abiArguments()[1]);
    visit_top_down_4fecc3dfe95f303c(__context__, arg_node, arg_blk);
    return v_zero();
}
static vec4f __wrap_re_assign_next_18621bfa0d3dbb85 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    re_assign_next_18621bfa0d3dbb85(__context__, arg_re);
    return v_zero();
}
static vec4f __wrap_re_assign_groups_ee95de912bf625ba ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    re_assign_groups_ee95de912bf625ba(__context__, arg_re);
    return v_zero();
}
static vec4f __wrap_re_assign_match_functions_d1123942ad08a870 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    re_assign_match_functions_d1123942ad08a870(__context__, arg_re);
    return v_zero();
}
static vec4f __wrap_re_match2_single_char_efd3120a5ed2dc78 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_single_char_efd3120a5ed2dc78(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_char_563cc8699254595c ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_char_563cc8699254595c(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_union_483bf85dbc5196dd ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_union_483bf85dbc5196dd(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_set_31b09b463a9d7b2b ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_set_31b09b463a9d7b2b(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_any_9b00b66687d95b62 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_any_9b00b66687d95b62(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_concat_2bd31fc7755cdf2a ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_concat_2bd31fc7755cdf2a(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_eos_a35408df9434ba3c ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_eos_a35408df9434ba3c(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_question_4acdfdc420ce399a ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_question_4acdfdc420ce399a(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_plus_adb26cc707eb0c55 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_plus_adb26cc707eb0c55(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_plus_set_aeb296540242ef22 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_plus_set_aeb296540242ef22(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_star_bbfb62194a4dc1ae ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_star_bbfb62194a4dc1ae(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_match2_group_82784eced054ba4e ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[1]);
    uint8_t const  * const  arg_str = cast_aot_arg<uint8_t const  * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<uint8_t const  *>::from(re_match2_group_82784eced054ba4e(__context__, arg_regex, arg_node, arg_str));
}
static vec4f __wrap_re_early_out_6710f0abebdc4106 ( Context * __context__ ) {
    TDim<uint32_t,8> &  arg_cset = cast_aot_arg<TDim<uint32_t,8> & >::to(*__context__,__context__->abiArguments()[0]);
    regex::ReNode * const  arg_node = cast_aot_arg<regex::ReNode * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(re_early_out_6710f0abebdc4106(__context__, arg_cset, arg_node));
}
static vec4f __wrap_is_valid_87c94e9920e8a112 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<bool>::from(is_valid_87c94e9920e8a112(__context__, arg_re));
}
static vec4f __wrap_regex_compile_b802a48fba17c551 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(regex_compile_b802a48fba17c551(__context__, arg_re, arg_expr));
}
static vec4f __wrap_regex_compile_9c6bbe3f7bf967b8 ( Context * __context__ ) {
    char * const  arg_expr = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    *((regex::Regex *) __context__->abiCMRES) = regex_compile_9c6bbe3f7bf967b8(__context__, arg_expr);
    return v_zero();
}
static vec4f __wrap_regex_compile_8b8cd96a70efcb1c ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    *((regex::Regex *) __context__->abiCMRES) = regex_compile_8b8cd96a70efcb1c(__context__, arg_re);
    return v_zero();
}
static vec4f __wrap_regex_match_d1084a47d6e2d53e ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    int32_t arg_offset = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[2]);
    return cast<int32_t>::from(regex_match_d1084a47d6e2d53e(__context__, arg_regex, arg_str, arg_offset));
}
static vec4f __wrap_regex_group_f150ed72d9c6b909 ( Context * __context__ ) {
    regex::Regex const  &  arg_regex = cast_aot_arg<regex::Regex const  & >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_index = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    char * const  arg_match = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[2]);
    return cast<char *>::from(regex_group_f150ed72d9c6b909(__context__, arg_regex, arg_index, arg_match));
}
static vec4f __wrap_regex_foreach_95b590dd0dd4944f ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    Block DAS_COMMENT((bool,range)) const  &  arg_blk = cast_aot_arg<Block DAS_COMMENT((bool,range)) const  & >::to(*__context__,__context__->abiArguments()[2]);
    regex_foreach_95b590dd0dd4944f(__context__, arg_regex, arg_str, arg_blk);
    return v_zero();
}
static vec4f __wrap_regex_replace_6c020ee2b76e6419 ( Context * __context__ ) {
    regex::Regex &  arg_regex = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_str = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    Block DAS_COMMENT((char *,char * const )) const  &  arg_blk = cast_aot_arg<Block DAS_COMMENT((char *,char * const )) const  & >::to(*__context__,__context__->abiArguments()[2]);
    return cast<char *>::from(regex_replace_6c020ee2b76e6419(__context__, arg_regex, arg_str, arg_blk));
}
static vec4f __wrap_regex_debug_35a427292763c452 ( Context * __context__ ) {
    regex::Regex const  &  arg_regex = cast_aot_arg<regex::Regex const  & >::to(*__context__,__context__->abiArguments()[0]);
    regex_debug_35a427292763c452(__context__, arg_regex);
    return v_zero();
}
static vec4f __wrap_debug_set_13e232e40e242fa0 ( Context * __context__ ) {
    TDim<uint32_t,8> const  &  arg_cset = cast_aot_arg<TDim<uint32_t,8> const  & >::to(*__context__,__context__->abiArguments()[0]);
    debug_set_13e232e40e242fa0(__context__, arg_cset);
    return v_zero();
}
static vec4f __wrap_debug_re_f8fd4cc00f0dc78d ( Context * __context__ ) {
    regex::ReNode * const  arg_node = cast_aot_arg<regex::ReNode * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_tab = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    debug_re_f8fd4cc00f0dc78d(__context__, arg_node, arg_tab);
    return v_zero();
}
static vec4f __wrap_rnd_set_character_6517e452a18c5606 ( Context * __context__ ) {
    TDim<uint32_t,8> const  &  arg_cset = cast_aot_arg<TDim<uint32_t,8> const  & >::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(rnd_set_character_6517e452a18c5606(__context__, arg_cset, arg_rnd));
}
static vec4f __wrap_re_gen2_fail_a5fdf20930267896 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_fail_a5fdf20930267896(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_char_e6b9f720990cc2d7 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_char_e6b9f720990cc2d7(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_union_44153b0e184e2888 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_union_44153b0e184e2888(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_set_5483397ec36cfff0 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_set_5483397ec36cfff0(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_any_200931e8e5db950 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_any_200931e8e5db950(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_concat_bb7cd4a1d8751a6c ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_concat_bb7cd4a1d8751a6c(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_eos_6a9fe263836b5d74 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_eos_6a9fe263836b5d74(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_question_ac79834a17fb3749 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_question_ac79834a17fb3749(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_plus_999e35bd61d42dc9 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_plus_999e35bd61d42dc9(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_star_e57464e7f97360fc ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_star_e57464e7f97360fc(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen2_group_cc75131bc454c003 ( Context * __context__ ) {
    regex::ReNode * arg_node = cast_aot_arg<regex::ReNode *>::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    StringBuilderWriter &  arg_str = cast_aot_arg<StringBuilderWriter & >::to(*__context__,__context__->abiArguments()[2]);
    re_gen2_group_cc75131bc454c003(__context__, arg_node, arg_rnd, arg_str);
    return v_zero();
}
static vec4f __wrap_re_gen_e78126cc4b992ab9 ( Context * __context__ ) {
    regex::Regex &  arg_re = cast_aot_arg<regex::Regex & >::to(*__context__,__context__->abiArguments()[0]);
    Sequence DAS_COMMENT((uint32_t)) &  arg_rnd = cast_aot_arg<Sequence DAS_COMMENT((uint32_t)) & >::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(re_gen_e78126cc4b992ab9(__context__, arg_re, arg_rnd));
}
static vec4f __wrap___init_script ( Context * __context__ ) {
    __init_script(__context__, cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[0]));
    return v_zero();
};

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0xca93cf324e9d39b4, false, (void*)&_FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd, &__wrap__FuncbuiltinTickresizeTick4811697762258667383_839271fc9974c3bd },
    { 0x5138dd10452ae233, false, (void*)&_FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759, &__wrap__FuncbuiltinTickfinalizeTick13836114024949725080_93dd7ca7375a7759 },
    { 0xa251302bc4e8ce68, false, (void*)&_FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d, &__wrap__FuncbuiltinTicklengthTick18150397773952384912_fcdb4107749fac2d },
    { 0xf176e87a2566021, false, (void*)&finalize_1b3fa79e78928d5e, &__wrap_finalize_1b3fa79e78928d5e },
    { 0x9f1419e715d3d4e7, false, (void*)&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3d380093f2b970c4, &__wrap__FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_3d380093f2b970c4 },
    { 0x5145cf36d3c6239d, false, (void*)&_FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec, &__wrap__FuncregexTickfrom_hexTick16446213900708532977_faa1e859112ad2ec },
    { 0x83035a1531032165, false, (void*)&finalize_4d9dff2c4111fea5, &__wrap_finalize_4d9dff2c4111fea5 },
    { 0x1161b07eafefef67, false, (void*)&finalize_2e3cca8177873ef7, &__wrap_finalize_2e3cca8177873ef7 },
    { 0x3c5a74bd28e4f38d, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9, &__wrap__FuncbuiltinTickpushTick10769833213962245646_76636a7a79f1cc9 },
    { 0x3876842f774c9ed8, true, (void*)&_FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6, &__wrap__FuncbuiltinTickto_array_moveTick3185538323411982277_d525ede85535d1d6 },
    { 0x304ae8111350c8ef, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5, &__wrap__FuncbuiltinTickpushTick10769833213962245646_689721e2e03454a5 },
    { 0x11fdc599388f444d, false, (void*)&_FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d, &__wrap__FuncbuiltinTicknextTick17450348357676149856_78a056decb3ca45d },
    { 0x828562d5af4c8f1a, false, (void*)&re_gen_get_rep_limit_aa71779c47fee5b8, &__wrap_re_gen_get_rep_limit_aa71779c47fee5b8 },
    { 0x7ef31621d0849bdb, false, (void*)&trace_7794f549fda55dde, &__wrap_trace_7794f549fda55dde },
    { 0x7ef31621d0849bdb, false, (void*)&note_7794f549fda55dde, &__wrap_note_7794f549fda55dde },
    { 0x7bfa536a1eb16ea9, false, (void*)&trace_backtrack_60bdee73e3315732, &__wrap_trace_backtrack_60bdee73e3315732 },
    { 0xb158d3f3d67fbaf, false, (void*)&trace_value_599ec0b97b535e0e, &__wrap_trace_value_599ec0b97b535e0e },
    { 0x4b0ef1fd815d7e51, true, (void*)&nada_a36e274f9f880579, &__wrap_nada_a36e274f9f880579 },
    { 0x510151b6eeb5cda0, true, (void*)&maybe_cb698f19184a9bc8, &__wrap_maybe_cb698f19184a9bc8 },
    { 0x9fa23e64f694fd37, false, (void*)&eos_bcd93149648ad887, &__wrap_eos_bcd93149648ad887 },
    { 0xd412d4b95b50aa6e, false, (void*)&at_ac9760d67b4716d0, &__wrap_at_ac9760d67b4716d0 },
    { 0x21b4c055ddc7e860, false, (void*)&next_c7b14aa26d681478, &__wrap_next_c7b14aa26d681478 },
    { 0x79ecf294045c1832, false, (void*)&is_set_empty_1f052aa7f260c4eb, &__wrap_is_set_empty_1f052aa7f260c4eb },
    { 0x33463cf95c238469, false, (void*)&set_or_char_5d5313390b879abc, &__wrap_set_or_char_5d5313390b879abc },
    { 0x24d714df9b7b9edb, false, (void*)&set_or_range_40be02560bdb916d, &__wrap_set_or_range_40be02560bdb916d },
    { 0x80fffd977ea41e19, false, (void*)&set_invert_4fb9b64ebf15e148, &__wrap_set_invert_4fb9b64ebf15e148 },
    { 0xb2ff9bfa5c6f4854, false, (void*)&set_or_set_f7fe9d5d2a5b194b, &__wrap_set_or_set_f7fe9d5d2a5b194b },
    { 0x6c973866131d87e7, false, (void*)&set_negative_e65d971bcd9eb5c1, &__wrap_set_negative_e65d971bcd9eb5c1 },
    { 0xa9455914f5f98c75, false, (void*)&set_meta_5675e929d5b2f227, &__wrap_set_meta_5675e929d5b2f227 },
    { 0xc995192cd4fe44d7, false, (void*)&is_meta_character_5210d144f518e88e, &__wrap_is_meta_character_5210d144f518e88e },
    { 0x9f216299c324b404, false, (void*)&is_set_character_b3eb8d77360f374d, &__wrap_is_set_character_b3eb8d77360f374d },
    { 0x45a4e0926c0ed921, false, (void*)&from_hex_34b87ad3a69093f9, &__wrap_from_hex_34b87ad3a69093f9 },
    { 0x76ce386522503121, false, (void*)&string_from_hex_3b7c632d00a40daf, &__wrap_string_from_hex_3b7c632d00a40daf },
    { 0x90f2e0c45cee8d1d, false, (void*)&string_from_hex_7d459c50a54015ae, &__wrap_string_from_hex_7d459c50a54015ae },
    { 0x98bf24f0f9a68bfc, true, (void*)&re_char_1cacb1cfe082d534, &__wrap_re_char_1cacb1cfe082d534 },
    { 0x9fcaef32f386865, true, (void*)&re_set_items_8d1ff416b742d6a0, &__wrap_re_set_items_8d1ff416b742d6a0 },
    { 0xc12ea6c2fec8368d, true, (void*)&re_set_a55f6e80806300ed, &__wrap_re_set_a55f6e80806300ed },
    { 0xfef54bd03e3ddd5c, true, (void*)&re_any_9e5c841624efd77c, &__wrap_re_any_9e5c841624efd77c },
    { 0x895307b0e2d2f267, true, (void*)&re_eos_91af11e09a55593b, &__wrap_re_eos_91af11e09a55593b },
    { 0x600928ce9ed85a0b, true, (void*)&re_group_a76956b21d051425, &__wrap_re_group_a76956b21d051425 },
    { 0xb17a47a9bffa827b, true, (void*)&re_elementary_8e517f8be58bf1a6, &__wrap_re_elementary_8e517f8be58bf1a6 },
    { 0xe33ebcfa64e4e8cc, false, (void*)&mk_concat_1ec73c2f3691d1d5, &__wrap_mk_concat_1ec73c2f3691d1d5 },
    { 0x1ef8bdea84af92cc, false, (void*)&mk_union_317bd0efb93d42ad, &__wrap_mk_union_317bd0efb93d42ad },
    { 0xb68976e489d3d52b, true, (void*)&re_re_e5f845d3bd4b2bf9, &__wrap_re_re_e5f845d3bd4b2bf9 },
    { 0xb07efcae1b0b0681, true, (void*)&re_basic_f733ea9ca09e6bda, &__wrap_re_basic_f733ea9ca09e6bda },
    { 0x628812d039d4eade, false, (void*)&re_parse_a11fbde82111aa69, &__wrap_re_parse_a11fbde82111aa69 },
    { 0xa7fa09bec9c61bf6, false, (void*)&visit_top_down_4fecc3dfe95f303c, &__wrap_visit_top_down_4fecc3dfe95f303c },
    { 0x9bcf4022f918481d, false, (void*)&re_assign_next_18621bfa0d3dbb85, &__wrap_re_assign_next_18621bfa0d3dbb85 },
    { 0x13d39a7bc2b8f76c, false, (void*)&re_assign_groups_ee95de912bf625ba, &__wrap_re_assign_groups_ee95de912bf625ba },
    { 0x428e227d93fc48b0, false, (void*)&re_assign_match_functions_d1123942ad08a870, &__wrap_re_assign_match_functions_d1123942ad08a870 },
    { 0xf1d002e52d7d8ef, false, (void*)&re_match2_single_char_efd3120a5ed2dc78, &__wrap_re_match2_single_char_efd3120a5ed2dc78 },
    { 0xb175b89ea6a4dc28, false, (void*)&re_match2_char_563cc8699254595c, &__wrap_re_match2_char_563cc8699254595c },
    { 0xbb4eaa89af769478, false, (void*)&re_match2_union_483bf85dbc5196dd, &__wrap_re_match2_union_483bf85dbc5196dd },
    { 0x4ee42d56faf1697f, false, (void*)&re_match2_set_31b09b463a9d7b2b, &__wrap_re_match2_set_31b09b463a9d7b2b },
    { 0x202c69489fc0c65, false, (void*)&re_match2_any_9b00b66687d95b62, &__wrap_re_match2_any_9b00b66687d95b62 },
    { 0xd470e4b78750568, false, (void*)&re_match2_concat_2bd31fc7755cdf2a, &__wrap_re_match2_concat_2bd31fc7755cdf2a },
    { 0x1c110d5d34042eae, false, (void*)&re_match2_eos_a35408df9434ba3c, &__wrap_re_match2_eos_a35408df9434ba3c },
    { 0x3323e83dade858d, false, (void*)&re_match2_question_4acdfdc420ce399a, &__wrap_re_match2_question_4acdfdc420ce399a },
    { 0x83e1a16da9d3db6d, false, (void*)&re_match2_plus_adb26cc707eb0c55, &__wrap_re_match2_plus_adb26cc707eb0c55 },
    { 0xc4c36ef2581ab070, false, (void*)&re_match2_plus_set_aeb296540242ef22, &__wrap_re_match2_plus_set_aeb296540242ef22 },
    { 0x2d60e1fcc7f92ed2, false, (void*)&re_match2_star_bbfb62194a4dc1ae, &__wrap_re_match2_star_bbfb62194a4dc1ae },
    { 0xf4aefc80782e2e4a, false, (void*)&re_match2_group_82784eced054ba4e, &__wrap_re_match2_group_82784eced054ba4e },
    { 0x27572f9dc16c3c09, false, (void*)&re_early_out_6710f0abebdc4106, &__wrap_re_early_out_6710f0abebdc4106 },
    { 0xcf9dad636341cfbe, false, (void*)&is_valid_87c94e9920e8a112, &__wrap_is_valid_87c94e9920e8a112 },
    { 0x8ad7a1bf6eb6b889, false, (void*)&regex_compile_b802a48fba17c551, &__wrap_regex_compile_b802a48fba17c551 },
    { 0xcdf3356bc8a30b91, true, (void*)&regex_compile_9c6bbe3f7bf967b8, &__wrap_regex_compile_9c6bbe3f7bf967b8 },
    { 0x9089764bca990b9d, true, (void*)&regex_compile_8b8cd96a70efcb1c, &__wrap_regex_compile_8b8cd96a70efcb1c },
    { 0xc9db0e4c83cf82eb, false, (void*)&regex_match_d1084a47d6e2d53e, &__wrap_regex_match_d1084a47d6e2d53e },
    { 0x22a5bb3f1de45f9a, false, (void*)&regex_group_f150ed72d9c6b909, &__wrap_regex_group_f150ed72d9c6b909 },
    { 0xa2f6655941dedc23, false, (void*)&regex_foreach_95b590dd0dd4944f, &__wrap_regex_foreach_95b590dd0dd4944f },
    { 0x3c85554b3800d3c, false, (void*)&regex_replace_6c020ee2b76e6419, &__wrap_regex_replace_6c020ee2b76e6419 },
    { 0xac3beb34a20a0e94, false, (void*)&regex_debug_35a427292763c452, &__wrap_regex_debug_35a427292763c452 },
    { 0x1db4112e373f4e6, false, (void*)&debug_set_13e232e40e242fa0, &__wrap_debug_set_13e232e40e242fa0 },
    { 0x401a0f2fcc15a153, false, (void*)&debug_re_f8fd4cc00f0dc78d, &__wrap_debug_re_f8fd4cc00f0dc78d },
    { 0x7d5fe16f6c01de1d, false, (void*)&rnd_set_character_6517e452a18c5606, &__wrap_rnd_set_character_6517e452a18c5606 },
    { 0x81d54f638e260ba5, false, (void*)&re_gen2_fail_a5fdf20930267896, &__wrap_re_gen2_fail_a5fdf20930267896 },
    { 0xa21be48addeafc5b, false, (void*)&re_gen2_char_e6b9f720990cc2d7, &__wrap_re_gen2_char_e6b9f720990cc2d7 },
    { 0xe5ca17f129619abc, false, (void*)&re_gen2_union_44153b0e184e2888, &__wrap_re_gen2_union_44153b0e184e2888 },
    { 0xfac3fb8e7bac4c7a, false, (void*)&re_gen2_set_5483397ec36cfff0, &__wrap_re_gen2_set_5483397ec36cfff0 },
    { 0xc5869664f0682804, false, (void*)&re_gen2_any_200931e8e5db950, &__wrap_re_gen2_any_200931e8e5db950 },
    { 0xf8f639c12b4d60ec, false, (void*)&re_gen2_concat_bb7cd4a1d8751a6c, &__wrap_re_gen2_concat_bb7cd4a1d8751a6c },
    { 0x45096c6fa48cb3f2, false, (void*)&re_gen2_eos_6a9fe263836b5d74, &__wrap_re_gen2_eos_6a9fe263836b5d74 },
    { 0x19abbf868f4c092d, false, (void*)&re_gen2_question_ac79834a17fb3749, &__wrap_re_gen2_question_ac79834a17fb3749 },
    { 0xff22bac43c320f7, false, (void*)&re_gen2_plus_999e35bd61d42dc9, &__wrap_re_gen2_plus_999e35bd61d42dc9 },
    { 0x2a51211cae2bc527, false, (void*)&re_gen2_star_e57464e7f97360fc, &__wrap_re_gen2_star_e57464e7f97360fc },
    { 0xc4bf433f55b01416, false, (void*)&re_gen2_group_cc75131bc454c003, &__wrap_re_gen2_group_cc75131bc454c003 },
    { 0x7beeb3e353b5340a, false, (void*)&re_gen_e78126cc4b992ab9, &__wrap_re_gen_e78126cc4b992ab9 },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
    // [[ init script ]]
    aotLib.emplace(0xcdd4468ae6f1930c, AotFactory(false, (void*)&__init_script, &__wrap___init_script));
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
